#include "colors.h"

#ifndef SDL_BADGE
#include "app.h"
#include "fb.h"
#include "buttons.h"
#else
#include "sdl_fb.h"
#include "sdl_buttons.h"
#endif
#include "utils.h"
#include "assetList.h"

unsigned char stop_screensaver = 0;


void matrix(){
    unsigned char w = 0, h = 0;
    unsigned char lcd_font_map[] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '.', ':', '!', '-', '_', ' '
    };    
    led(0, 50, 0);
    while(!stop_screensaver){
        FbColor(GREEN);
        for(w = 0; w <132; w+=10){
            for(h = 0; h < 132; h+=10){
                FbMove(w, h);
                FbCharacter(lcd_font_map[quick_rand(w+h)%42]);
            }
        }
        FbSwapBuffers();
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
    led(0, 0, 0);
}

//const unsigned char badgetip_more_you_know = 
void just_the_badge_tips(void* p_arg){
    unsigned char badgetips_header[] = "--Badge Tip--";
    unsigned short anim_cnt = 0;
    static unsigned char tipnum = 0;
    unsigned short popup_time;
    while(!stop_screensaver){
        if(!anim_cnt){
            tipnum = quick_rand(popup_time) % 19;
            anim_cnt++;
        }

        FbBackgroundColor(BLACK);
        FbColor(GREEN);
        FbMove(12, 4);
        FbWriteLine(badgetips_header);

        FbColor(YELLOW);
        switch(tipnum){
            case 0:
                FbMove(2, 20);
                FbWriteLine("Dont lick badge");
                break;
            case 1:
                FbMove(2, 20);
                FbWriteLine("These are land");
                FbMove(2, 30);
                FbWriteLine("dwelling badges.");
                FbMove(2, 40);
                FbWriteLine("Avoid water");
                break;
            case 2:
                FbMove(2, 20);
                FbWriteLine("Do not leave");
                FbMove(2, 30);
                FbWriteLine("battery in");
                FbMove(2, 40);
                FbWriteLine("when using USB");
                break;
            case 3:
                FbMove(2, 20);
                FbWriteLine("Wash your hands");
                FbMove(2, 30);
                FbWriteLine("next time.");
                break;
            case 4:
                FbMove(2, 20);
                FbWriteLine("Say hi to");
                FbMove(2, 30);
                FbWriteLine("my creators");
                FbMove(2, 40);
                FbWriteLine("over at hackrva");
                break;
            case 5:
                FbMove(2, 20);
                FbWriteLine("Have another");
                FbMove(2, 30);
                FbWriteLine("beer.");
                break;
            case 6:
                FbMove(2, 20);
                FbWriteLine("Jet fuel cant");
                FbMove(2, 30);
                FbWriteLine("melt steel beams.");
                break;
            case 7:
                FbMove(2, 20);
                FbWriteLine("Sit down and");
                FbMove(2, 30);
                FbWriteLine("   STFU");
                break;
            case 8:
                FbMove(2, 20);
                FbWriteLine("checkout the");
                FbMove(2, 30);
                FbWriteLine("   CTF!");
                break;
            case 9:
                FbMove(2, 20);
                FbWriteLine("Its impolite");
                FbMove(2, 30);
                FbWriteLine("to stare at");
                FbMove(2, 40);
                FbWriteLine("other peoples");
                FbMove(2, 50);
                FbWriteLine("badges.");
                break;
            case 10:
                FbMove(2, 20);
                FbWriteLine("Every badge is");
                FbMove(2, 30);
                FbWriteLine("different. Get");
                FbMove(2, 40);
                FbWriteLine("to know yours!");
                break;
            case 11:
                FbMove(2, 20);
                FbWriteLine("Badges have the");
                FbMove(2, 30);
                FbWriteLine("best hygiene.");
                FbMove(2, 40);
                FbWriteLine("Unlike all of");
                FbMove(2, 50);
                FbWriteLine("you.");
                break;
            case 12:
                FbMove(2, 20);
                FbWriteLine("Unix or bust.");
                break;
            case 13:
                FbMove(2, 20);
                FbWriteLine("If you cant");
                FbMove(2, 30);
                FbWriteLine("read this your");
                FbMove(2, 40);
                FbWriteLine("badge is broken");
                break;
            case 14:
                FbMove(2, 20);
                FbWriteLine("The D-Pad");
                FbMove(2, 30);
                FbWriteLine("was 3D printed");
                FbMove(2, 40);
                FbWriteLine("at hackrva");
//                FbMove(2, 50);
//                FbWriteLine("the conference.");
                break;
            case 15:
                FbMove(2, 20);
                FbWriteLine("Dont touch");
                FbMove(2, 30);
                FbWriteLine("the screen.");
                break;
            case 16:
                FbMove(2, 20);
                FbWriteLine("If you are");
                FbMove(2, 30);
                FbWriteLine("having problems");
                FbMove(2, 40);
                FbWriteLine("with your badge");
                FbMove(2, 50);
                FbWriteLine("go to hackrva");
                FbMove(2, 60);
                FbWriteLine("near the CTF");
                break;
            case 17:
                FbMove(2, 20);
                FbWriteLine("These arent the");
                FbMove(2, 30);
                FbWriteLine("badges your ");
                FbMove(2, 40);
                FbWriteLine("looking for.");
                break;
            case 18:
                FbMove(2, 20);
                FbWriteLine("Bitches love");
                FbMove(2, 30);
                FbWriteLine("badges. Woof.");
                //FbMove(2, 40);
                //FbWriteLine("");
                break;
        }

        FbSwapBuffers();
        
        vTaskDelay(200 / portTICK_PERIOD_MS);
    }
}



void for_president(void* p_arg){
    unsigned char president1[] = "badge FOR";
    unsigned char president2[] = "president";
    unsigned char president3[] = "make badge";
    unsigned char president4[] = "great again";
    unsigned short popup_time = 0;
    unsigned short anim_cnt = 0;
    FbBackgroundColor(GREY8);
    FbMove(20, 17);
    FbColor(WHITE);
    FbWriteLine(president1);

    FbMove(30, 35);
    FbWriteLine(president2);

    while(!stop_screensaver){
        if(popup_time > 30){
            unsigned char i = 0;
            for(i=0; i<8; i++){
                FbColor((i+anim_cnt)%2 ? WHITE: RED);
                FbMove(0, 50 + (i*10));
                FbFilledRectangle(132, 10);
            }
        }
        else{
            FbColor(WHITE);
            FbMove(30, 70);
            FbWriteLine(president3);

            FbMove(10, 80);
            FbWriteLine(president4);
        }
        anim_cnt++;
        popup_time++;
        FbSwapBuffers();
        vTaskDelay(20 / portTICK_PERIOD_MS);
    }
   
    
    led(0, 0, 0);
   

}


#define TUNNEL_COLOR CYAN
void carzy_tunnel_animator(void* p_arg){
   unsigned short anim_cnt = 0;
   FbBackgroundColor(GREY2);
   FbClear();
    while(!stop_screensaver){
        anim_cnt++;

        if(!anim_cnt){
            FbColor(TUNNEL_COLOR);
            FbMove(65,65);

            FbRectangle(2, 2);
            red(0);
            green(1);
            blue(1);
        }
        else if(anim_cnt == 200){
            FbColor(TUNNEL_COLOR);
            FbMove(64,64);

            FbRectangle(4, 4);
            red(0);
            green(5);
            blue(5);
        }
        else if(anim_cnt == 400){
            FbColor(TUNNEL_COLOR);
            FbMove(62,62);

            FbRectangle(8, 8);
            red(0);
            green(20);
            blue(20);
        }
        else if(anim_cnt == 600){
            FbColor(TUNNEL_COLOR);
            FbMove(58,58);

            FbRectangle(16, 16);
            red(0);
            green(45);
            blue(45);
        }
        else if(anim_cnt == 800){
            FbColor(TUNNEL_COLOR);
            FbMove(50,50);

            FbRectangle(32, 32);
            red(0);
            green(75);
            blue(75);
        }
        else if(anim_cnt == 1000){
            FbColor(TUNNEL_COLOR);
            FbMove(34,34);

            FbRectangle(64, 64);
            red(0);
            green(100);
            blue(100);
        }
        else if(anim_cnt > 1199)
            anim_cnt = 0 - 1;

        FbSwapBuffers();
    }
   led(0, 0, 0);
}

void random_dots(void* p_arg){
    unsigned int cnt = 0, x, y;
    FbBackgroundColor(MAGENTA);
    FbClear();

    //for(;;)
    while(!stop_screensaver)
    {
        cnt++;
        if(cnt < 50)
            FbColor(WHITE);
        else if(cnt < 100)
            FbColor(RED);
        else if(cnt < 150)
            FbColor(GREEN);
        else if(cnt < 200)
            FbColor(YELLOW);
        else if(cnt < 250)
            FbColor(BLUE);
        else
            cnt=0;

        //draw_test_poly();

        x = quick_rand(cnt)%125 + 2;
        y = quick_rand(cnt+1337)%125 + 2;
        FbMove(x,
               y);
        FbFilledRectangle(5, 5);

        // Push after some pixels accumulated
        if(!(cnt % 13))
            FbPushBuffer();

        //FbPushRegion(x, y, 3, 3);
        if(BUTTON_PRESSED_AND_CONSUME)
            return;
    }
}


#define NUM_POLYS 11
void spirals_task(void* p_arg){
    short points[15][2] = {0};
    unsigned char n_edges = 3, i=0, led_brightness=0;
    const TickType_t tick_rate = 10 / portTICK_PERIOD_MS;
    float rads[NUM_POLYS] = {0.0}, rad_scale=.001;
    FbBackgroundColor(GREY1);
    FbClear();    
    FbColor(GREEN);
    //for(;;)
    while(!stop_screensaver)
    {
        for(rad_scale=0.001; rad_scale < 0.05 && !stop_screensaver; rad_scale += 0.0001)
        {
            for(i=0; i<NUM_POLYS; i++){

                equilateral_polygon_points(points, (float)(5 + (6*i)), n_edges + i, 
                                           rads[i]);            
                FbPolygonFromPoints(points, n_edges + i,
                                    65, 65);

                rads[i] = rads[i] + ((float)(NUM_POLYS - i) * rad_scale);
                if(rads[i] > 6.282)
                    rads[i] = 0.0;
            }
            FbSwapBuffers();
            led(0, led_brightness, 0);
            led_brightness++;
            vTaskDelay(tick_rate);                
        }
    }
    led(0, 0, 0);
}



struct gof_cell_t{
   unsigned char is_active : 1;
   unsigned char neighbors : 3;
   
};

#define GRID_CELL_DIM 6
#define GRID_DIM 22
#define LAST_COORD (GRID_CELL_DIM * GRID_DIM)
#define COUNT_OF_NEIGHBORS(VAL) VAL > 0 ? (VAL - 1) : 0
#define INCR_NEIGHBORS(VAL) VAL > 0 ? (VAL++) : 2
#define DECR_NEIGHBORS(VAL) VAL == ? (VAL++) : 2
void create_flier(struct gof_cell_t cell_grid[][GRID_DIM],
                  unsigned char x, unsigned char y){
    if(x < 1){
        x = 1;
    }
    else if(x > 90){
        x = 80;
    }
    
    if(y < 1){
        y = 1;
    }
    else if(y > 90){
        y = 80;
    }
    cell_grid[x][y+2].is_active = 1;
    cell_grid[x+1][y+2].is_active = 1;
    cell_grid[x+2][y+2].is_active = 1;
    cell_grid[x+2][y+1].is_active = 1;
    cell_grid[x+1][y].is_active = 1;
}

void update_gof_grid(struct gof_cell_t cell_grid[GRID_DIM][GRID_DIM],
                     unsigned short cell_color){
    unsigned char i=0, j=0;
    FbColor(cell_color);
    // Don't use full range so we
    // dont have to do boundary checks
    for(i=2; i < (GRID_DIM - 2); i++) {
        for(j=2; j < (GRID_DIM - 2); j++){
            if(cell_grid[i][j].is_active){

                cell_grid[i][j+1].neighbors += 1;
                cell_grid[i+1][j+1].neighbors += 1;
                cell_grid[i+1][j].neighbors += 1;
                cell_grid[i+1][j-1].neighbors += 1;

                cell_grid[i][j-1].neighbors += 1;
                cell_grid[i-1][j-1].neighbors += 1;
                cell_grid[i-1][j].neighbors += 1;
                cell_grid[i-1][j+1].neighbors += 1;
            }
        }
    }
    
    for(i=2; i < (GRID_DIM - 2); i++) {
        for(j=2; j < (GRID_DIM - 2); j++){
            if((cell_grid[i][j].neighbors > 1)
               && (cell_grid[i][j].neighbors < 4)
                    && (cell_grid[i][j].is_active)){
                    cell_grid[i][j].is_active = 1;
            }
            else if( (cell_grid[i][j].neighbors == 3)
                     && (!cell_grid[i][j].is_active) ){
                cell_grid[i][j].is_active = 1;
            }
            else if((cell_grid[i][j].neighbors >= 4)){
                cell_grid[i][j].is_active = 0;
            }
            else if((cell_grid[i][j].neighbors <= 1)){
                cell_grid[i][j].is_active = 0;
            }

           if(cell_grid[i][j].is_active && cell_color != 0){
                FbMove(i*GRID_CELL_DIM,
                       j*GRID_CELL_DIM);

                FbFilledRectangle(GRID_CELL_DIM,
                                  GRID_CELL_DIM);
            }
            cell_grid[i][j].neighbors = 0;
        }
    }

}


void game_of_life_task(void* p_arg)
{
    const TickType_t tick_rate = 10 / portTICK_PERIOD_MS;
    unsigned char i = 0, j=0, redraw=1;
    //unsigned char grid[GRID_DIM][GRID_DIM] = {0};
    struct gof_cell_t cell_grid[GRID_DIM][GRID_DIM] = {{0}};

    FbBackgroundColor(GREY1);
    FbClear();
    
    for(i=0; i< 5; i++){
        create_flier(cell_grid, 
                        10 + ((unsigned char)(quick_rand(i)%50)),
                        11 + ((unsigned char)(quick_rand(i)%50)));
    }

    if(stop_screensaver)
        led(100, 0, 0);
    FbClear();
    
    while(!stop_screensaver)
    {
        if(redraw){
            FbColor(YELLOW);
            for(i=0; i < GRID_DIM; i++){
                for(j=0; j < GRID_DIM; j++){
/*
 For a space that is 'populated':
    Each cell with one or no neighbors dies, as if by solitude.
    Each cell with four or more neighbors dies, as if by overpopulation.
    Each cell with two or three neighbors survives.
For a space that is 'empty' or 'unpopulated'
    Each cell with three neighbors becomes populated.
 */
                    if(cell_grid[i][j].is_active)
                    {
                        FbMove(i*GRID_CELL_DIM,
                               j*GRID_CELL_DIM);

                        FbFilledRectangle(GRID_CELL_DIM,
                                          GRID_CELL_DIM);
                    }
                }
            }
        }
        FbColor(WHITE);
        for(i=GRID_CELL_DIM; 
                i < LAST_COORD; i+= GRID_CELL_DIM)
        {
            FbVerticalLine(i, 1,
                          i, 131);
            FbHorizontalLine(1, i,
                          131, i);

        }
        FbSwapBuffers();

        vTaskDelay(tick_rate);

        update_gof_grid(cell_grid, YELLOW);

//        if(BUTTON_PRESSED_AND_CONSUME)
//            return;     
    }

}

void about_the_bird(void* p_arg){

    const TickType_t tick_rate = 10 / portTICK_PERIOD_MS;
    unsigned char cnt=0, x, y;
    FbBackgroundColor(CYAN);
    FbClear();
    //while(!BUTTON_PRESSED_AND_CONSUME){
    //for(;;)
    while(!stop_screensaver){
        x = quick_rand(cnt++)%110;
        y = quick_rand(cnt++)%110;
        FbMove(x, y);
        FbTransparentIndex(3);
        FbImage(BADGEY_BIRD, 0);
        FbPushBuffer();
        vTaskDelay(tick_rate);
        if(BUTTON_PRESSED_AND_CONSUME) {
            FbBackgroundColor(BLACK);
            FbClear();
            return;
        }
    }
}

void screensaver_task(void* p_arg)
{
    unsigned char i = 0;

#ifdef SDL_BADGE
    while(G_Fb.status)
#else
    for(;;)
#endif
    {
        about_the_bird(NULL);
        game_of_life_task(NULL);
        spirals_task(NULL);
        random_dots(NULL);
#ifndef SDL_BADGE
        returnToMenus();
#endif
    }
}

#define NUM_SCREEN_SAVERS 8
void random_screen_saver(void* p_arg){
    unsigned char rnd = 2;//quick_rand(42)%NUM_SCREEN_SAVERS;
    
    switch(rnd){
        case 0:
            about_the_bird(NULL);
            break;
        case 1:
            //game_of_life_task(NULL);
            //break;            
        case 2:
            spirals_task(NULL);
            break;       
        case 3:
            random_dots(NULL);  
            break;     
        case 4:
            //just_the_badge_tips(NULL);
            //break;                   
        case 5:
//            for_president(NULL);
//            break;
        case 6:
            carzy_tunnel_animator(NULL);
            break;
        case 7:
            matrix();
            break;
    }
    
#ifndef SDL_BADGE
    returnToMenus();
#endif
}


