#include "app.h"
#include <xc.h>
#include <sys/attribs.h>

unsigned char G_red_cnt=0;
volatile unsigned char G_red_pwm=0;

unsigned char G_green_cnt=0;
volatile unsigned char G_green_pwm=0;

unsigned char G_blue_cnt=0;
volatile unsigned char G_blue_pwm=0;

//void led_task(void* p_arg)
void doPWM()
{
    //for(;;)
    {
        /* red */
        G_red_cnt++;
        if (G_red_cnt < G_red_pwm)
            LATCbits.LATC0 = 1;
        else
            LATCbits.LATC0 = 0;

        // just let it wrap around if (G_red_cnt == 255) G_red_cnt = 0;

        /* Green */
        G_green_cnt++;
        if (G_green_cnt < G_green_pwm)
            LATBbits.LATB3 = 1;
        else
            LATBbits.LATB3 = 0;

        // just let it wrap around if (G_green_cnt == 255) G_green_cnt = 0;

        /* Blue */
        G_blue_cnt++;
        if (G_blue_cnt < G_blue_pwm)
            LATCbits.LATC1 = 1;
        else
            LATCbits.LATC1 = 0;

        //vTaskDelay(5 / portTICK_PERIOD_MS);
        //vTaskDelay(1000);
    }
    // just let it wrap around if (G_blue_cnt == 255) G_blue_cnt = 0;

    //G_backlight_cnt++;
    //if (G_backlight_cnt < G_backlight)
    //    LATCbits.LATC9 = 1;
    //else
    //    LATCbits.LATC9 = 0;    
}

// TODO:  why are these ints? also pwm
void red(unsigned char pwm)
{
    // TODO: Brightness shift
    //G_red_pwm = pwm;
    SYS_PORTS_PinWrite( PORTS_ID_0, PORT_CHANNEL_C, PORTS_BIT_POS_0, pwm );
}

void green(unsigned char pwm)
{
    //G_green_pwm = pwm;
    SYS_PORTS_PinWrite( PORTS_ID_0, PORT_CHANNEL_B, PORTS_BIT_POS_3, pwm );
}

void blue(unsigned char pwm)
{
    //G_blue_pwm = pwm;
    SYS_PORTS_PinWrite( PORTS_ID_0, PORT_CHANNEL_C, PORTS_BIT_POS_1, pwm ); 
}

void led(unsigned char r, unsigned char g, unsigned char b){
    red(r);
    green(g);
    blue(b);
}
