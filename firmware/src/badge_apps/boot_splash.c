#include "fb.h"
#include "app.h"
#include "buttons.h"
#include "app.h"
#include "assetList.h"
#include "colors.h"

enum boot_splash_state {
    INIT,
    HACKRVA,
    RVASEC,
    PROMPT,
    EXIT
};


#define SPLASH_SHIFT_DOWN 85
void boot_splash_task(void* p_arg) {
    
    const TickType_t xDelay = 50 / portTICK_PERIOD_MS;
    unsigned char state = HACKRVA;
    unsigned char cnt = 0, loading_txt_idx = 0, load_bar=0;
    const unsigned char splash_words1[] = "Loading";
#define NUM_WORD_THINGS 18
    const unsigned char *splash_word_things[] = {"Cognition Module",
                                                 "useless bits",
                                                  "NSA backdoor",
                                                  "breathalyzer",
                                                  "sub-zero",
                                                  "lifting tables",
                                                  "X-badge scripts",
                                                  "important bits",
                                                  "bitcoin miner",
                                                   "stuxnet",
                                                  "broken feature",
                                                    "NTFS", "Wall hacks",
                                                  "passive aggress",
                                                  "Key logger",
                                "smtp server", "sshd", "cryptolocker", };
    const unsigned char splash_words_btn1[] = "Press the button";
    const unsigned char splash_words_btn2[] = "to continue!";
    
    for(;;){
        switch(state){
            case HACKRVA:
                FbImage(HACKRVA4, 0);
                FbSwapBuffers();
                if(cnt == 100 || BUTTON_PRESSED_AND_CONSUME){
                    state = RVASEC;
                    cnt = 0;
                }
                
                break;
            case RVASEC:
                FbMove(0, 2);
                FbImage(RVASEC2016, 0);
                FbMove(10,SPLASH_SHIFT_DOWN);

                FbColor(WHITE);
                FbRectangle(100, 20);

                FbMove(35, SPLASH_SHIFT_DOWN - 13);
                FbColor(YELLOW);
                FbWriteLine(splash_words1);

                FbMove(11, SPLASH_SHIFT_DOWN+1);
                FbColor(GREEN);
                FbFilledRectangle((load_bar++ << 1) + 1,19);
                
                
                FbColor(WHITE);
                FbMove(4, 113);
                FbWriteLine(splash_word_things[loading_txt_idx%NUM_WORD_THINGS]);
                if(!(cnt%2))
                    loading_txt_idx++;                
                
                FbSwapBuffers();
                
                if(BUTTON_PRESSED_AND_CONSUME){
                    state = EXIT;
                }
                else if (load_bar == 50){
                    state = PROMPT;
                    cnt = 0;
                }
                break;
            case PROMPT:

                FbMove(0, 2);
                FbImage(RVASEC2016, 0);
                FbMove(10,SPLASH_SHIFT_DOWN);

                FbColor(GREEN);
                FbMove(1, 70);
                FbWriteLine(splash_words_btn1);

                FbMove(15, 80);
                FbWriteLine(splash_words_btn2);
                if(BUTTON_PRESSED_AND_CONSUME){
                    state = EXIT;
                }
                else if (cnt == 100){
                    state = HACKRVA;
                    cnt = 0;
                }                
                FbSwapBuffers();
                break;
            case EXIT:
                returnToMenus();
                break;
        }
        vTaskDelay(xDelay);
        cnt++;
    }
}