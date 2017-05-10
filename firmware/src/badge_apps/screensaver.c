#include "colors.h"

#ifndef SDL_BADGE
#include "app.h"
#include "fb.h"
#include "assetList.h"
#include "buttons.h"
#else
#include "sdl_fb.h"
#include "sdl_buttons.h"
#endif
#include "utils.h"

void draw_test_poly(){
    FbPoint(10,10);
    FbPoint(11,10);
    FbPoint(12,10);
    FbPoint(13,10);
    FbPoint(14,10);
    FbPoint(15,10);
    FbPoint(16,10);
    FbPoint(17,10);
    FbPoint(18,10);
    FbPoint(19,10);
    FbPoint(20,10);
    FbPoint(21,10);
    FbPoint(22,10);
    FbPoint(23,10);
    FbPoint(24,10);
    FbPoint(25,10);
    FbPoint(26,10);
    FbPoint(27,10);
    FbPoint(28,10);
    FbPoint(29,10);
    FbPoint(10,11);
    FbPoint(11,11);
    FbPoint(12,11);
    FbPoint(13,11);
    FbPoint(14,11);
    FbPoint(15,11);
    FbPoint(16,11);
    FbPoint(17,11);
    FbPoint(18,11);
    FbPoint(19,11);
    FbPoint(20,11);
    FbPoint(21,11);
    FbPoint(22,11);
    FbPoint(23,11);
    FbPoint(24,11);
    FbPoint(25,11);
    FbPoint(26,11);
    FbPoint(27,11);
    FbPoint(28,11);
    FbPoint(10,12);
    FbPoint(11,12);
    FbPoint(12,12);
    FbPoint(13,12);
    FbPoint(14,12);
    FbPoint(15,12);
    FbPoint(16,12);
    FbPoint(17,12);
    FbPoint(18,12);
    FbPoint(19,12);
    FbPoint(20,12);
    FbPoint(21,12);
    FbPoint(22,12);
    FbPoint(23,12);
    FbPoint(24,12);
    FbPoint(25,12);
    FbPoint(26,12);
    FbPoint(27,12);
    FbPoint(28,12);
    FbPoint(11,13);
    FbPoint(12,13);
    FbPoint(13,13);
    FbPoint(14,13);
    FbPoint(15,13);
    FbPoint(16,13);
    FbPoint(17,13);
    FbPoint(18,13);
    FbPoint(19,13);
    FbPoint(20,13);
    FbPoint(21,13);
    FbPoint(22,13);
    FbPoint(23,13);
    FbPoint(24,13);
    FbPoint(25,13);
    FbPoint(26,13);
    FbPoint(27,13);
    FbPoint(11,14);
    FbPoint(12,14);
    FbPoint(13,14);
    FbPoint(14,14);
    FbPoint(15,14);
    FbPoint(16,14);
    FbPoint(17,14);
    FbPoint(18,14);
    FbPoint(19,14);
    FbPoint(20,14);
    FbPoint(21,14);
    FbPoint(22,14);
    FbPoint(23,14);
    FbPoint(24,14);
    FbPoint(25,14);
    FbPoint(26,14);
    FbPoint(27,14);
    FbPoint(11,15);
    FbPoint(12,15);
    FbPoint(13,15);
    FbPoint(14,15);
    FbPoint(15,15);
    FbPoint(16,15);
    FbPoint(17,15);
    FbPoint(18,15);
    FbPoint(19,15);
    FbPoint(20,15);
    FbPoint(21,15);
    FbPoint(22,15);
    FbPoint(23,15);
    FbPoint(24,15);
    FbPoint(25,15);
    FbPoint(26,15);
    FbPoint(11,16);
    FbPoint(12,16);
    FbPoint(13,16);
    FbPoint(14,16);
    FbPoint(15,16);
    FbPoint(16,16);
    FbPoint(17,16);
    FbPoint(18,16);
    FbPoint(19,16);
    FbPoint(20,16);
    FbPoint(21,16);
    FbPoint(22,16);
    FbPoint(23,16);
    FbPoint(24,16);
    FbPoint(25,16);
    FbPoint(26,16);
    FbPoint(11,17);
    FbPoint(12,17);
    FbPoint(13,17);
    FbPoint(14,17);
    FbPoint(15,17);
    FbPoint(16,17);
    FbPoint(17,17);
    FbPoint(18,17);
    FbPoint(19,17);
    FbPoint(20,17);
    FbPoint(21,17);
    FbPoint(22,17);
    FbPoint(23,17);
    FbPoint(24,17);
    FbPoint(25,17);
    FbPoint(11,18);
    FbPoint(12,18);
    FbPoint(13,18);
    FbPoint(14,18);
    FbPoint(15,18);
    FbPoint(16,18);
    FbPoint(17,18);
    FbPoint(18,18);
    FbPoint(19,18);
    FbPoint(20,18);
    FbPoint(21,18);
    FbPoint(22,18);
    FbPoint(23,18);
    FbPoint(24,18);
    FbPoint(25,18);
    FbPoint(12,19);
    FbPoint(13,19);
    FbPoint(14,19);
    FbPoint(15,19);
    FbPoint(16,19);
    FbPoint(17,19);
    FbPoint(18,19);
    FbPoint(19,19);
    FbPoint(20,19);
    FbPoint(21,19);
    FbPoint(22,19);
    FbPoint(23,19);
    FbPoint(24,19);
    FbPoint(12,20);
    FbPoint(13,20);
    FbPoint(14,20);
    FbPoint(15,20);
    FbPoint(16,20);
    FbPoint(17,20);
    FbPoint(18,20);
    FbPoint(19,20);
    FbPoint(20,20);
    FbPoint(21,20);
    FbPoint(22,20);
    FbPoint(23,20);
    FbPoint(24,20);
    FbPoint(12,21);
    FbPoint(13,21);
    FbPoint(14,21);
    FbPoint(15,21);
    FbPoint(16,21);
    FbPoint(17,21);
    FbPoint(18,21);
    FbPoint(19,21);
    FbPoint(20,21);
    FbPoint(21,21);
    FbPoint(22,21);
    FbPoint(23,21);
    FbPoint(12,22);
    FbPoint(13,22);
    FbPoint(14,22);
    FbPoint(15,22);
    FbPoint(16,22);
    FbPoint(17,22);
    FbPoint(18,22);
    FbPoint(19,22);
    FbPoint(20,22);
    FbPoint(21,22);
    FbPoint(22,22);
    FbPoint(23,22);
    FbPoint(12,23);
    FbPoint(13,23);
    FbPoint(14,23);
    FbPoint(15,23);
    FbPoint(16,23);
    FbPoint(17,23);
    FbPoint(18,23);
    FbPoint(19,23);
    FbPoint(20,23);
    FbPoint(21,23);
    FbPoint(22,23);
    FbPoint(12,24);
    FbPoint(13,24);
    FbPoint(14,24);
    FbPoint(15,24);
    FbPoint(16,24);
    FbPoint(17,24);
    FbPoint(18,24);
    FbPoint(19,24);
    FbPoint(20,24);
    FbPoint(21,24);
    FbPoint(22,24);
    FbPoint(13,25);
    FbPoint(14,25);
    FbPoint(15,25);
    FbPoint(16,25);
    FbPoint(17,25);
    FbPoint(18,25);
    FbPoint(19,25);
    FbPoint(20,25);
    FbPoint(21,25);
    FbPoint(13,26);
    FbPoint(14,26);
    FbPoint(15,26);
    FbPoint(16,26);
    FbPoint(17,26);
    FbPoint(18,26);
    FbPoint(19,26);
    FbPoint(20,26);
    FbPoint(21,26);
    FbPoint(13,27);
    FbPoint(14,27);
    FbPoint(15,27);
    FbPoint(16,27);
    FbPoint(17,27);
    FbPoint(18,27);
    FbPoint(19,27);
    FbPoint(20,27);
    FbPoint(13,28);
    FbPoint(14,28);
    FbPoint(15,28);
    FbPoint(16,28);
    FbPoint(17,28);
    FbPoint(18,28);
    FbPoint(19,28);
    FbPoint(20,28);
    FbPoint(13,29);
    FbPoint(14,29);
    FbPoint(15,29);
    FbPoint(16,29);
    FbPoint(17,29);
    FbPoint(18,29);
    FbPoint(19,29);
    FbPoint(13,30);
    FbPoint(14,30);
    FbPoint(15,30);
    FbPoint(16,30);
    FbPoint(17,30);
    FbPoint(18,30);
    FbPoint(19,30);
    FbPoint(14,31);
    FbPoint(15,31);
    FbPoint(16,31);
    FbPoint(17,31);
    FbPoint(18,31);
    FbPoint(14,32);
    FbPoint(15,32);
    FbPoint(16,32);
    FbPoint(17,32);
    FbPoint(18,32);
    FbPoint(14,33);
    FbPoint(15,33);
    FbPoint(16,33);
    FbPoint(17,33);
    FbPoint(14,34);
    FbPoint(15,34);
    FbPoint(16,34);
    FbPoint(17,34);
    FbPoint(14,35);
    FbPoint(15,35);
    FbPoint(16,35);
    FbPoint(14,36);
    FbPoint(15,36);
    FbPoint(16,36);
    FbPoint(15,37);
    FbPoint(15,38);    
}

void random_dots(void* p_arg){
    unsigned int cnt = 0, x, y;

    for(;;)
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

        draw_test_poly();
        
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
void boot_splash_task(void* p_arg){
    short points[15][2] = {0};
    unsigned char n_edges = 3, i=0;
    const TickType_t tick_rate = 10 / portTICK_PERIOD_MS;
    float rads[NUM_POLYS] = {0.0}, rad_scale=.001;
    FbColor(GREEN);
    for(;;)
    {
        for(rad_scale=0.001; rad_scale < 0.05; rad_scale += 0.0001)
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

            vTaskDelay(tick_rate);
        
            if(BUTTON_PRESSED_AND_CONSUME)
                return;            
        }

    }
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
    cell_grid[x][y+2].is_active = 1;
    cell_grid[x+1][y+2].is_active = 1;
    cell_grid[x+2][y+2].is_active = 1;
    cell_grid[x+2][y+1].is_active = 1;
    cell_grid[x+1][y].is_active = 1;
}


void game_of_life_task(void* p_arg)
{
    const TickType_t tick_rate = 10 / portTICK_PERIOD_MS;
    unsigned char i = 0, j=0, redraw=1;
    //unsigned char grid[GRID_DIM][GRID_DIM] = {0};
    struct gof_cell_t cell_grid[GRID_DIM][GRID_DIM] = {{0}};

    create_flier(cell_grid, 1, 1);
    create_flier(cell_grid, 6, 1);

    FbClear();
    
    for(;;)
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

        // Don't use full range so we
        // dont have to do boundary checks
        for(i=1; i < (GRID_DIM - 1); i++) {
            for(j=1; j < (GRID_DIM - 1); j++){
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

        for(i=1; i < (GRID_DIM - 1); i++) {
            for(j=1; j < (GRID_DIM - 1); j++){


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

               if(cell_grid[i][j].is_active){
                    FbMove(i*GRID_CELL_DIM,
                           j*GRID_CELL_DIM);

                    FbFilledRectangle(GRID_CELL_DIM,
                                      GRID_CELL_DIM);
                }
                //printf("%d, ", cell_grid[i][j]);
                cell_grid[i][j].neighbors = 0;
            }
        }

        if(BUTTON_PRESSED_AND_CONSUME)
            return;     
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
        game_of_life_task(NULL);
        boot_splash_task(NULL);
        random_dots(NULL);
#ifndef SDL_BADGE
        returnToMenus();
#endif
    }
}