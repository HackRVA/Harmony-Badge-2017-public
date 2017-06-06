
#include "colors.h"
#include "assetList.h"

#ifdef SDL_BADGE
#include "sdl_fb.h"
#include "sdl_buttons.h"
#else
#include "fb.h"
#include "app.h"
#include "buttons.h"
#include "app.h"
#include "app.h"
#include "fb.h"
#include "buttons.h"
#endif
#include "colors.h"

enum boot_splash_state {
    INIT,
    HACKRVA,
    SHELL_LAUNCH,
    RVASEC,
    PROMPT,
    EXIT
};


#define SPLASH_SHIFT_DOWN 85
void boot_splash_task(void* p_arg) {
    
    const TickType_t xDelay = 50 / portTICK_PERIOD_MS;
    unsigned char state = HACKRVA;
    unsigned char i, cnt = 0, loading_txt_idx = 0, load_bar=0;
    const unsigned char splash_words1[] = "Loading",
                        splash_words2[] = "./run_badge17.sh";
    
#ifdef BADGE_2016_LOAD
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
#else
#define NUM_WORD_THINGS 18
    const unsigned char *splash_word_things[] = {"Fire retardant",
                                                 "important bits",
                                                  "russian backdoor",
                                                  "d-pad driver",
                                                  "fake fake news",
                                                  "hoisting tables",
                                                  "X-badge scripts",
                                                  "important bits",
                                                  "bitcoin2.0",
                                                   "badgey bear",
                                                  "podestas emails",
                                                    "salt miner", 
    "border wall",
                                                  "submission module",
                                                  "thought logger",
                                "app drm", "removing covefe", "ransomware", };    
#endif
    const unsigned char splash_words_btn1[] = "Press the blue";
    const unsigned char splash_words_btn2[] = "button to continue!";
    short shared_vertices[8][2] = {0};
    equilateral_polygon_points(shared_vertices, 13.0, 6, 0);

    for(;;){
        switch(state){
            case HACKRVA:
                FbImage(HACKRVA4, 0);
                FbSwapBuffers();
                if(cnt == 100 || BUTTON_PRESSED_AND_CONSUME){
                    state = RVASEC;
                    cnt = 0;
                    load_bar = 0;
                }
                
                break;
            case SHELL_LAUNCH:
                FbMove(2, 2);
                FbColor(WHITE);
                FbWriteLine("$>");       
                        
                if(cnt == 100 || BUTTON_PRESSED_AND_CONSUME){
                    state = RVASEC;
                    cnt = 0;
                    load_bar = 0;
                }
                break;
            case RVASEC:

                FbMove(0, 2);
                FbImage(RVASEC2016, 0);
                FbMove(10, SPLASH_SHIFT_DOWN);
#ifdef NOPE
                FbColor(WHITE);
                FbRectangle(100, 20);

                FbMove(35, SPLASH_SHIFT_DOWN - 13);
                FbColor(YELLOW);
                FbWriteLine(splash_words1);

                FbMove(11, SPLASH_SHIFT_DOWN+1);
                FbColor(GREEN);
                FbFilledRectangle((load_bar++ << 1) + 1,19);
#endif
                FbColor(GREEN);
                //FbPolygonFromPoints(&shared_vertices[load_bar%6],//goon_vertices,
                //                    2,
                //                    60,
                //                    SPLASH_SHIFT_DOWN);
                //FbDrawVectors(&shared_vertices[load_bar%6],//goon_vertices,
                //                    2,
                //                    60,
                //                    SPLASH_SHIFT_DOWN, 0);
                //for(i=0; i<6; i++){
                i = load_bar%6;
                if (i!=5){
                    FbLine(
                           (unsigned char)shared_vertices[i][0] + 65,
                           (unsigned char)shared_vertices[i][1] + SPLASH_SHIFT_DOWN,
                           (unsigned char)shared_vertices[i+1][0] + 65,
                           (unsigned char)shared_vertices[i+1][1] + SPLASH_SHIFT_DOWN
                    );
                }
                else{
                    FbLine(
                           (unsigned char)shared_vertices[i][0] + 65,
                           (unsigned char)shared_vertices[i][1] + SPLASH_SHIFT_DOWN,
                           (unsigned char)shared_vertices[0][0] + 65,
                           (unsigned char)shared_vertices[0][1] + SPLASH_SHIFT_DOWN
                    );
                }

                load_bar++;
                FbColor(WHITE);
                FbMove(4, 115);
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
                FbMove(5, 70);
                FbWriteLine(splash_words_btn1);

                FbMove(1, 80);
                FbWriteLine(splash_words_btn2);
                
                FbColor(WHITE);
                FbLine(65, 90, 90, 110);
                FbColor(GREEN);
                FbLine(80, 108, 90, 110);
                FbLine(87, 100, 90, 110);
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
#ifndef SDL_BADGE
                returnToMenus();
#endif
                break;
        }
        vTaskDelay(xDelay);
        cnt++;
    }
}