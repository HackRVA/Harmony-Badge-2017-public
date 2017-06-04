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
        spirals_task(NULL);
        random_dots(NULL);
#ifndef SDL_BADGE
        returnToMenus();
#endif
    }
}
