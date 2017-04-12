#include "app.h"
#include "buttons.h"

unsigned char G_pressed_button = 0;

// Solo bottom right btn
unsigned char G_button_cnt = 0;

// D-Pad
unsigned char G_up_button_cnt = 0;
unsigned char G_down_button_cnt = 0;
unsigned char G_left_button_cnt = 0;
unsigned char G_right_button_cnt = 0;

// Cap Touch
unsigned char G_up_touch_cnt = 0;
unsigned char G_middle_touch_cnt = 0;
unsigned char G_down_touch_cnt = 0;
char G_touch_pct = 0;


unsigned int G_entropy_pool = 961748927;
unsigned int timestamp = 0;

void init_CTMU()
{
    /* setup analog pins */
    // PEB RA0/CTED1 RA1/CTED2 RB1/CTED12 RB2/CTED13
    // make them inputs
    // MSS: Don't touch last years slider pins
    //TRISAbits.TRISA0 = 1;
    //TRISAbits.TRISA1 = 1;
    // This years two pads
    TRISBbits.TRISB1 = 1; //AN3
    TRISBbits.TRISB2 = 1; //AN4

    //ANSELA = (1<<0) | (1<<1); //RA0,1
    ANSELB = (1<<1) | (1<<2); //RB1,2
    vTaskDelay(1 / portTICK_PERIOD_MS);    

    // CTMU Setup
    CTMUCONbits.IRNG = 3;  // 1 = 0.55uA 2 = 5.5uA 3 = 55uA 0 = 550uA
    CTMUCONbits.ON = 1;    // Turn on CTMU
    vTaskDelay(1 / portTICK_PERIOD_MS);    // Wait 1 msec

    // ADC Setup
    AD1CON2 = 0x0; // VR+ = AVDD, V- = AVSS, Don't scan, MUX A only

    // ADC clock derived from peripheral buss clock
    // Tadc = 4 * Tpbus = 4 * 25 ns = 100 ns > 65 ns required
    // Tadc = 2*(    1       +1)*Tpbus
    // Tadc = 2*(AD1CON3<7:0>+1)*Tpbus
    AD1CON3 = 1;          // PEB 16 * Tpb;
    AD1CSSL = 0x0;        // No channels scanned
    IEC0bits.AD1IE = 0;   // Disable ADC interrupts
    AD1CON1bits.ON = 1;   // Turn on ADC
}

void button_task(void* p_arg)
{
    const unsigned char ButtonADCChannels[2] = {3,4};
    const unsigned char n_averages = 64, log2_n_averages = 6;
    unsigned short int ButtonVavgADCs[2]={0,0};
    
    unsigned short int VmeasADC; // Measured Voltages, 65536 = Full Scale
    unsigned long int ADC_Sum; // For averaging multiple ADC measurements
    unsigned char i = 0, chan_idx = 0;
    
    TickType_t xDelay = 10 / portTICK_PERIOD_MS;

    //Analog pins: AN3 (B1) is outside pad, AN4 (B2) is interior pad
    #define AN3 ButtonVavgADCs[0]
    #define AN4 ButtonVavgADCs[1]
#define BADGE_2017
#ifdef BADGE_2016
    #define Y1 (31000 - 2*AN3)
    #define Y2 (  900 + 2*AN3)
    #define Y3 (-6000 + 2*AN3)
    #define TOUCH_PCT_CALC (char)((AN3*-3 + AN4*1 + 25043) >> 8)
#endif
#ifdef BADGE_2017
    #define Y1 (28000 - 2*AN3)
    #define Y2 (  -1500 + 2*AN3)
    #define Y3 (-7500 + 2*AN3)
    #define TOUCH_PCT_CALC (char)((AN3*-3 + AN4*1 + 32934) >> 8)
#endif

    init_CTMU();
    
    for(;;){
        timestamp++;
    //------------------
    //---CTMU---
        for(chan_idx=0; chan_idx < 2; chan_idx++)
        {
            AD1CHSbits.CH0SA = ButtonADCChannels[chan_idx];
            for(i=0; i < n_averages; i++)
            {
                //CTMUCONCLR = 0x03000000;
                AD1CON1bits.SAMP = 1;
                CTMUCONbits.IDISSEN = 1;
                vTaskDelay(1 / portTICK_PERIOD_MS);
                CTMUCONbits.IDISSEN = 0;

                // TODO: CHECK THESE STEPS against docs
                CTMUCONbits.EDG1STAT = 1; // Begin charging the circuit
                unsigned char nops;
                // TODO: Can we give up less time to make this faster?
                // Or give longer sleep for coop with other tasks
                for(nops=0; nops < 10; nops++); 
                AD1CON1bits.SAMP = 0;     // Begin analog-to-digital conversion
                CTMUCONbits.EDG1STAT = 0; // Stop charging circuit
                while(!(AD1CON1bits.DONE))
                {

                }
                AD1CON1bits.DONE = 0; // ADC conversion done, clear flag
                VmeasADC = ADC1BUF0;  // Get the value from the ADC
                ADC_Sum += VmeasADC;  // Update averaging sum                
            }
            
            if ( log2_n_averages-6 > 0 )
                ButtonVavgADCs[chan_idx] = ADC_Sum >> (log2_n_averages-6); // Full scale = 2^10<<6 = 65536
            else 
                ButtonVavgADCs[chan_idx] = ADC_Sum << (6-log2_n_averages); // Full scale = 2^10<<6 = 65536

            // May be mixing too often...
            G_entropy_pool = (ADC_Sum ^ (G_entropy_pool)<<(0xF&timestamp));
            ADC_Sum = 0;
        }
        //---------
        // Check activation boundaries
        if(AN4 < Y1)
        {                        
            if((AN4 < Y3) && (G_down_touch_cnt < 255))
                G_down_touch_cnt++;
            
            if((AN4 < Y2) && (AN4 > Y3) && (G_middle_touch_cnt < 255))
                G_middle_touch_cnt++;
            
            if((AN4 > Y2) && (G_up_touch_cnt < 255))
                G_up_touch_cnt++;
            
            // TODO: This cast may cause problems
            // Could just use this pct to detect buttons...
            G_touch_pct = TOUCH_PCT_CALC;
            
            if(G_touch_pct > 100)
                G_touch_pct = 100;
            else if(G_touch_pct < 0)
                G_touch_pct = 0;
        }
        else{
            G_up_touch_cnt = G_down_touch_cnt = G_middle_touch_cnt = G_touch_pct =0;
            REMOVE_FROM_MASK(G_pressed_button, ALL_TOUCH_MASK);
        }

//#define PRINT_TOUCH_VALS
#ifdef PRINT_TOUCH_VALS
        char words[8]={0};
        static unsigned char bcnt = 0;
        vTaskDelay(100/portTICK_PERIOD_MS);        
        
        
        itoa(words, (int)AN3, 10);
        print_to_com1(words);
        print_to_com1(",");
        itoa(words, (int)AN4, 10);
        print_to_com1(words);

//        print_to_com1(",");
//        itoa(words, (int)G_down_touch_cnt, 10);
//        print_to_com1(words);
//
//        print_to_com1(",");
//        itoa(words, (int)G_middle_touch_cnt, 10);
//        print_to_com1(words);
//        
//        print_to_com1(",");
//        itoa(words, (int)G_up_touch_cnt, 10);
//        print_to_com1(words);
        
        print_to_com1(",");
        //itoa(words, (int)G_button_cnt, 10);
        if(G_button_cnt >5){
            bcnt++;
            vTaskDelay(2000/portTICK_PERIOD_MS);   
        }
        itoa(words, (int)bcnt, 10);
        print_to_com1(words);
        
//        if(TOUCH_DOWN_PRESSED)
//        {
//            print_to_com1(",");
//            print_to_com1("DOWN");
//            led(0, 0, 1);
//        }
//        if(TOUCH_MIDDLE_PRESSED)
//        {
//            print_to_com1(",");
//            print_to_com1("MID");
//            led(1, 0, 0);
//        }   
//        if(TOUCH_UP_PRESSED)
//        {
//            print_to_com1(",");
//            print_to_com1("UP");
//            led(0, 1, 0);
//        }         
//        
        print_to_com1("\n\r");
        
#endif        

    //---------------------
    //---Tactile Buttons---
    // If btn is being pressed
    //   - increment its counter, but don't overflow
    // else if btn is not being pressed
    //   - Zero its counter and zero out 'pressed' bit flag
        
        // Standalone button
        if(!PLIB_PORTS_PinGet(PORTS_ID_0,PORT_CHANNEL_C,PORTS_BIT_POS_3)){
            if(G_button_cnt < 255)
                G_button_cnt++;
        }
        else{
            G_button_cnt = 0;
            REMOVE_FROM_MASK(G_pressed_button, SOLO_BTN_MASK);
        }
        
        //UP:
        if(!PLIB_PORTS_PinGet(PORTS_ID_0,PORT_CHANNEL_B,PORTS_BIT_POS_14)){
            if(G_up_button_cnt < 255)
                G_up_button_cnt++;
        }
        else{
            
            G_up_button_cnt = 0;
            REMOVE_FROM_MASK(G_pressed_button, UP_BTN_MASK);
        }
        
        //LEFT:
        if(!PLIB_PORTS_PinGet(PORTS_ID_0,PORT_CHANNEL_B,PORTS_BIT_POS_15)){
            if(G_left_button_cnt < 255)
                G_left_button_cnt++;
        }
        else{
            G_left_button_cnt = 0;
            REMOVE_FROM_MASK(G_pressed_button, LEFT_BTN_MASK);
        }
        
        //RIGHT:
        if(!PLIB_PORTS_PinGet(PORTS_ID_0,PORT_CHANNEL_A,PORTS_BIT_POS_0)){
            if(G_right_button_cnt < 255)
                G_right_button_cnt++;
        }
        else{
            G_right_button_cnt = 0;
            REMOVE_FROM_MASK(G_pressed_button, RIGHT_BTN_MASK);
        }
        
        //DOWN:
        if(!PLIB_PORTS_PinGet(PORTS_ID_0,PORT_CHANNEL_A,PORTS_BIT_POS_1))
            G_down_button_cnt++;
        else{
            G_down_button_cnt = 0;
            REMOVE_FROM_MASK(G_pressed_button, DOWN_BTN_MASK);
        }

        vTaskDelay(xDelay);
    }
}

