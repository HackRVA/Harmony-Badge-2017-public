#include "app.h"
#include "queue.h"
#include "buttons.h"
#include "ir.h"
#include "flash.h"
// badge
#include "colors.h"
#include "fb.h"
#include "rgb_led.h"
#include "task.h"
#include "badge_menu.h"
#include "assetList.h"


enum tutorial_state{
    //START,
    MAIN_BUTTON_PROMPT,
    MAIN_BUTTON_COMPLETE,
    D_PAD_PROMPT,
    D_PAD_COMPLETE,
    SLIDER_PROMPT,
    SLIDER_COMPLETE,
    MENU_EXPLAINATION,
    EXIT
};

void draw_dpad(unsigned char dpad_up, 
                unsigned char dpad_down, 
                unsigned char dpad_left, 
                unsigned char dpad_right){
    FbColor(GREY8);

    FbMove(25, 65);
    FbFilledRectangle(10, 40);

    FbMove(10, 80);
    FbFilledRectangle(40, 10);
    FbColor(YELLOW);
    
    if(dpad_up){
        FbMove(26, 66);
        FbFilledRectangle(8, 10);
    }
    if(dpad_down){
        FbMove(26, 94);
        FbFilledRectangle(8, 10);
    }
    if(dpad_left){
        FbMove(11, 81);
        FbFilledRectangle(10, 8);
    }
    if(dpad_right){
        FbMove(38, 81);
        FbFilledRectangle(10, 8);
    }       
}

void badge_tutorial_task(void* p_arg)
{
    unsigned char state = MAIN_BUTTON_PROMPT;
    unsigned char i, j;
    unsigned char dpad_up=0, dpad_down=0, dpad_left=0, dpad_right=0;
    unsigned char slider[10] = {0};
    for(;;){
        if(G_button_cnt > 250)
            state=EXIT;
        
        switch(state){
            case MAIN_BUTTON_PROMPT:
                FbColor(GREEN);
                FbMove(2, 7);
                FbWriteLine("Badge17 Tutorial");
                
                FbColor(WHITE);
                
                FbMove(20, 50);
                FbWriteLine("Press the ");        
                FbColor(CYAN);
                FbMove(15, 65);
                FbWriteLine("blue button");
                
                FbColor(WHITE);
                FbMove(7, 80);
                FbWriteLine("in the bottom");                  
                FbMove(10, 90);
                FbWriteLine("right corner");
                //FbMove(100, 100);
                
                FbSwapBuffers();
                state = MAIN_BUTTON_COMPLETE;   
                break;
            case MAIN_BUTTON_COMPLETE:
                if(BUTTON_PRESSED_AND_CONSUME){
                    FbColor(GREEN);
                    FbMove(3, 30);
                    FbWriteLine("Most impressive!");                  
                    FbMove(16, 70);
                    FbWriteLine("youve done");
                    FbMove(13, 80);
                    FbWriteLine("this before");                    
                    FbSwapBuffers();
                    vTaskDelay(6000 / portTICK_PERIOD_MS);
                    state = D_PAD_PROMPT;
                }  
                break;
            case D_PAD_PROMPT:
                FbColor(GREEN);
                FbMove(7, 20);
                FbWriteLine("Find the D-Pad");
                
                FbColor(WHITE);
                FbMove(10, 30);
                FbWriteLine("at the bottom");                
                FbMove(15, 40);
                FbWriteLine("left corner");
                
                if(UP_BTN_AND_CONSUME)
                    dpad_up = 1;
                if(DOWN_BTN_AND_CONSUME)
                    dpad_down = 1;
                if(LEFT_BTN_AND_CONSUME)
                    dpad_left = 1;
                if(RIGHT_BTN_AND_CONSUME)
                    dpad_right = 1;
                
                draw_dpad(dpad_up , dpad_down , dpad_left , dpad_right);
                
                FbSwapBuffers();
                if(dpad_up && dpad_down && dpad_left && dpad_right)
                    state = D_PAD_COMPLETE;
                else 
                    vTaskDelay(10 / portTICK_PERIOD_MS);
                break;
            case D_PAD_COMPLETE:
                
                {
                //if(BUTTON_PRESSED_AND_CONSUME){
                    FbColor(GREEN);
                    FbMove(4, 20);
                    FbWriteLine("Most impressive!");                               
                    draw_dpad(dpad_up , dpad_down , dpad_left , dpad_right);
                    FbSwapBuffers();
                    vTaskDelay(3000 / portTICK_PERIOD_MS);
                    state = SLIDER_PROMPT;
                }                  
                break;
            case SLIDER_PROMPT:
                FbColor(GREEN);
                FbMove(9, 20);
                FbWriteLine("Find slider");
                FbMove(7, 30);
                FbWriteLine("traingles to");                  
                FbMove(15, 40);
                FbWriteLine("the right");   

                //if(G_touch_pct > 1)
                slider[G_touch_pct/10] = 1;                
                
                for(i=0; i<10; i++){
                    FbColor(WHITE);
                    FbMove(110, 110 - i*10 + 1);
                    FbRectangle(15, 8);                    
                    if (slider[i]){
                        j++;
                        FbColor(YELLOW);
                        FbMove(110, 110 - i*10 + 1);
                        FbFilledRectangle(15, 8);
                    }
                }
                if(j > 4){
                    FbColor(CYAN);
                    FbMove(15, 70);
                    FbWriteLine("B-Button");           
                    FbMove(15, 80);
                    FbWriteLine("to continue");   
                    if(BUTTON_PRESSED_AND_CONSUME)
                    {
                        state=SLIDER_COMPLETE;
                    }
                }
                
                j = 0;
                
                FbSwapBuffers();               
                break;
            case SLIDER_COMPLETE:
                //if(BUTTON_PRESSED_AND_CONSUME)
                {
                    FbColor(GREEN);
                    FbMove(3, 30);
                    FbWriteLine("Most impressive!");                  
                    FbMove(8, 70);
                    FbWriteLine("You are now a");
                    FbColor(YELLOW);
                    FbMove(10, 80);
                    FbWriteLine("badge expert");                    
                    FbSwapBuffers();
                    vTaskDelay(6000 / portTICK_PERIOD_MS);
                    state = MENU_EXPLAINATION;
                }                  
                break;
            case MENU_EXPLAINATION:
                FbColor(GREEN);
                FbMove(3, 30);
                FbWriteLine("Use the d-pad");                  
                FbMove(3, 40);
                FbWriteLine("to navigate the");
                FbMove(3, 50);
                FbWriteLine("menu and the");   
                FbMove(20, 65);
                FbColor(CYAN);
                FbWriteLine("B-button");
                FbMove(3, 80);
                FbColor(GREEN);
                FbWriteLine("to select items");                  
                FbSwapBuffers();
                vTaskDelay(8000 / portTICK_PERIOD_MS);                
                state=EXIT;
            case EXIT:
                returnToMenus();
                break;
        }
    }
}
