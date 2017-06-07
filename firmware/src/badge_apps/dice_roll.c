#include "fb.h"
#include "app.h"
#include "buttons.h"
#include "app.h"
#include "assetList.h"
#include "colors.h"

enum boot_splash_state {
    INIT,
    SHAKE,
    ROLL,
    SHOW,
    EXIT
};

#define DICE_SQUARE_SIZE 50
#define DOT_SQUARE_SIZE 4
#define DICE_X 40
#define DICE_Y 40
void draw_1(){
    FbMove(DICE_X, DICE_Y);
    FbColor(WHITE);
    FbFilledRectangle(DICE_SQUARE_SIZE, DICE_SQUARE_SIZE);
    
    FbMove(DICE_X + DICE_SQUARE_SIZE/2, DICE_Y + DICE_SQUARE_SIZE/2);
    FbColor(BLACK);
    FbFilledRectangle(DOT_SQUARE_SIZE, DOT_SQUARE_SIZE);
}

void draw_2(){
    FbMove(DICE_X, DICE_Y);
    FbColor(WHITE);
    FbFilledRectangle(DICE_SQUARE_SIZE, DICE_SQUARE_SIZE);
    
    FbColor(BLACK);   
    
    FbMove(DICE_X + DICE_SQUARE_SIZE/4, DICE_Y + DICE_SQUARE_SIZE/4);
    FbFilledRectangle(DOT_SQUARE_SIZE, DOT_SQUARE_SIZE);

    FbMove(DICE_X + 3*DICE_SQUARE_SIZE/4, DICE_Y + 3*DICE_SQUARE_SIZE/4);
    FbFilledRectangle(DOT_SQUARE_SIZE, DOT_SQUARE_SIZE);    
}    

void draw_3(){
    FbMove(DICE_X, DICE_Y);
    FbColor(WHITE);
    FbFilledRectangle(DICE_SQUARE_SIZE, DICE_SQUARE_SIZE);
    
    FbMove(DICE_X + DICE_SQUARE_SIZE/2, DICE_Y + DICE_SQUARE_SIZE/2);
    FbColor(BLACK);
    FbFilledRectangle(DOT_SQUARE_SIZE, DOT_SQUARE_SIZE);
    
    FbMove(DICE_X + DICE_SQUARE_SIZE/4, DICE_Y + DICE_SQUARE_SIZE/4);
    FbFilledRectangle(DOT_SQUARE_SIZE, DOT_SQUARE_SIZE);

    FbMove(DICE_X + 3*DICE_SQUARE_SIZE/4, DICE_Y + 3*DICE_SQUARE_SIZE/4);
    FbFilledRectangle(DOT_SQUARE_SIZE, DOT_SQUARE_SIZE);       
}  

void draw_4(){
    FbMove(DICE_X, DICE_Y);
    FbColor(WHITE);
    FbFilledRectangle(DICE_SQUARE_SIZE, DICE_SQUARE_SIZE);
    
    FbColor(BLACK);
    
    FbMove(DICE_X + DICE_SQUARE_SIZE/4, DICE_Y + DICE_SQUARE_SIZE/4);
    FbFilledRectangle(DOT_SQUARE_SIZE, DOT_SQUARE_SIZE);

    FbMove(DICE_X + 3*DICE_SQUARE_SIZE/4, DICE_Y + 3*DICE_SQUARE_SIZE/4);
    FbFilledRectangle(DOT_SQUARE_SIZE, DOT_SQUARE_SIZE);
    
    FbMove(DICE_X + DICE_SQUARE_SIZE/4, DICE_Y + 3*DICE_SQUARE_SIZE/4);
    FbFilledRectangle(DOT_SQUARE_SIZE, DOT_SQUARE_SIZE);

    FbMove(DICE_X + 3*DICE_SQUARE_SIZE/4, DICE_Y + DICE_SQUARE_SIZE/4);
    FbFilledRectangle(DOT_SQUARE_SIZE, DOT_SQUARE_SIZE);       
}

void draw_5(){
    FbMove(DICE_X, DICE_Y);
    FbColor(WHITE);
    FbFilledRectangle(DICE_SQUARE_SIZE, DICE_SQUARE_SIZE);
    
    FbColor(BLACK);
    FbMove(DICE_X + DICE_SQUARE_SIZE/2, DICE_Y + DICE_SQUARE_SIZE/2);
    FbFilledRectangle(DOT_SQUARE_SIZE, DOT_SQUARE_SIZE);   
    
    FbMove(DICE_X + DICE_SQUARE_SIZE/4, DICE_Y + DICE_SQUARE_SIZE/4);
    FbFilledRectangle(DOT_SQUARE_SIZE, DOT_SQUARE_SIZE);

    FbMove(DICE_X + 3*DICE_SQUARE_SIZE/4, DICE_Y + 3*DICE_SQUARE_SIZE/4);
    FbFilledRectangle(DOT_SQUARE_SIZE, DOT_SQUARE_SIZE);
    
    FbMove(DICE_X + DICE_SQUARE_SIZE/4, DICE_Y + 3*DICE_SQUARE_SIZE/4);
    FbFilledRectangle(DOT_SQUARE_SIZE, DOT_SQUARE_SIZE);

    FbMove(DICE_X + 3*DICE_SQUARE_SIZE/4, DICE_Y + DICE_SQUARE_SIZE/4);
    FbFilledRectangle(DOT_SQUARE_SIZE, DOT_SQUARE_SIZE);       
}

void draw_6(){
    FbMove(DICE_X, DICE_Y);
    FbColor(WHITE);
    FbFilledRectangle(DICE_SQUARE_SIZE, DICE_SQUARE_SIZE);
    
    FbColor(BLACK);
    FbMove(DICE_X + DICE_SQUARE_SIZE/4, DICE_Y + DICE_SQUARE_SIZE/4);
    FbFilledRectangle(DOT_SQUARE_SIZE, DOT_SQUARE_SIZE);
    
    FbMove(DICE_X + DICE_SQUARE_SIZE/4, DICE_Y + DICE_SQUARE_SIZE/2);
    FbFilledRectangle(DOT_SQUARE_SIZE, DOT_SQUARE_SIZE);    

    FbMove(DICE_X + 3*DICE_SQUARE_SIZE/4, DICE_Y + 3*DICE_SQUARE_SIZE/4);
    FbFilledRectangle(DOT_SQUARE_SIZE, DOT_SQUARE_SIZE);
    
    FbMove(DICE_X + DICE_SQUARE_SIZE/4, DICE_Y + 3*DICE_SQUARE_SIZE/4);
    FbFilledRectangle(DOT_SQUARE_SIZE, DOT_SQUARE_SIZE);

    FbMove(DICE_X + 3*DICE_SQUARE_SIZE/4, DICE_Y + DICE_SQUARE_SIZE/4);
    FbFilledRectangle(DOT_SQUARE_SIZE, DOT_SQUARE_SIZE);       
    
    FbMove(DICE_X + 3*DICE_SQUARE_SIZE/4, DICE_Y + DICE_SQUARE_SIZE/2);
    FbFilledRectangle(DOT_SQUARE_SIZE, DOT_SQUARE_SIZE);        
}  

#define ANY_DPAD (UP_BTN_AND_CONSUME || DOWN_BTN_AND_CONSUME || LEFT_BTN_AND_CONSUME || RIGHT_BTN_AND_CONSUME)
#define SPLASH_SHIFT_DOWN 85
void dice_roll_task(void* p_arg) {

    unsigned char state = INIT;
    unsigned char dice_value = 0;
    for(;;){
        switch(state){
            case INIT:
//                state = SHAKE;
//                FbMove(20, 20);
//                FbColor(WHITE);
//                FbWriteLine("D-PAD To Shake!");
//                if(ANY_DPAD){
//                    state = SHAKE;
//                }
//                FbSwapBuffers();
//                break;
            case SHAKE:
                dice_value = (rand()%6) +1;
                switch (dice_value){
                    case 1:
                        draw_1();
                        break;
                    case 2:
                        draw_2();
                        break;
                    case 3:
                        draw_3();
                        break;
                    case 4:
                        draw_4();
                        break;
                    case 5:
                        draw_5();
                        break;                        
                    case 6:
                        draw_6();
                        break;
                }
         
                
                FbSwapBuffers();
                
                state = SHOW;
                break;
                        
            case SHOW:
                if(BUTTON_PRESSED_AND_CONSUME)
                    state = EXIT;
                break;
            case EXIT:
                returnToMenus();
                break;
        } 
    }

}
