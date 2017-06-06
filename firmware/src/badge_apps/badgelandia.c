//#define SDL_BADGE

#ifndef SDL_BADGE
#include "app.h"
#endif
#include "colors.h"

#ifdef SDL_BADGE
#include "sdl_fb.h"
#include "sdl_buttons.h"
#else
#include "fb.h"
#include "buttons.h"
#endif
//#include "rgb_led.h"
#include <math.h>
#include <stdlib.h>
#include "utils.h"

#define WORLD_HEIGHT 1024
#define WORLD_WIDTH  1024
struct ship_description{
};

struct badgelandia_basic_object_t {
       float loc_x, loc_y, // quantize to fixed point?
             vel_x, vel_y;
};

void ctor_badgelandia_basic_object(struct badgelandia_basic_object_t *o){
    o->loc_x = .0;
    o->loc_y = .0;
    o->vel_x = .0;
    o->vel_y = .0;
}

struct badgelandia_basic_ship_t {
    struct badgelandia_basic_object_t o;

    float orientation_rads;
    unsigned char ship_id;

    unsigned char max_fuel, current_fuel, ticks_per_fuel;
    unsigned char accel, max_speed;
    unsigned char max_hp, current_hp;
};

struct badgelandia_basic_inventory_t {
    unsigned int credits;
    unsigned char health_kits, repair_kits;

    unsigned char available_weapons_mask;
    unsigned char available_thruster_mask;
};

struct badgelandia_player_t {

   struct badgelandia_basic_ship_t ship;
   struct badgelandia_basic_inventory_t inventory;

};

struct badgelandia_goon_t {
    struct badgelandia_basic_object_t o;
    unsigned char health;
};

void badgelandia_growth(){
    const TickType_t tick_rate = 10 / portTICK_PERIOD_MS;
    FbBackgroundColor(GREY2);
    FbClear();

    unsigned char i=0, y = 0, new_y=60, freq_bins[100]={0};

    for(;;){
        //if(BUTTON_PRESSED_AND_CONSUME)
        //    return;

        if(G_touch_pct >0){
            new_y = 132 - (G_touch_pct + 10);
            if(freq_bins[G_touch_pct] < 120)
                freq_bins[G_touch_pct]++;
        }

        if(new_y != y)
        {

            for(i=0; i < 100; i++){
                FbColor(GREEN);

                FbMove(10, 132 - (i + 10));
                FbFilledRectangle(freq_bins[i]+1,
                                  1);
            }

            FbMove(100, y);
            FbColor(GREY2);
            FbFilledRectangle(5, 15);


            //FbMove(1, new_y);
            FbColor(WHITE);
            //FbFilledRectangle(5, 15);
            FbLine(10, 10, 40, 40);
//            FbEquilateralPolygon(10, new_y,
//                                 5, 3, 0);

            FbSwapBuffers();
            y = new_y;
        }
        vTaskDelay(tick_rate);
    }
}

#define SET_PT_XY(pts, i, x, y) pts[i][0] = x; pts[i][1] = y;
void goon_follow(struct badgelandia_player_t goons,
                 short target_x, short target_y) {

}

void create_player_vector(short pts[][2]){
    //pts[0][0] =
    SET_PT_XY(pts, 0,
              9, 0);
    SET_PT_XY(pts, 1,
              -9, -9);
    SET_PT_XY(pts, 2,
              -3, 0);
    SET_PT_XY(pts, 3,
              -9, 9);

}

#define DEBOUNCE_CNT 3
void badgelandia_task(void* p_arg){
    const TickType_t tick_rate = 10 / portTICK_PERIOD_MS;

    unsigned char i = 0, redraw=1;

    unsigned char spkr_factor = 10;
    
    // Use floats to accumulate small changes, cast before draw
    struct badgelandia_player_t player;
#define N_GOONS 5
#define GOON_ROTATION_DELTA .01
    struct badgelandia_goon_t goons[N_GOONS];

    float ship_rotation = 0.0;
    float goon_rotation = 0.0;
    float cos_rot=1.0, sin_rot=0.0;
    char thruster_pct = 0;
    short shared_vertices[8][2] = {0};
    #define THRUSTER_COOL_DOWN 0
    unsigned char thruster_cool_down = THRUSTER_COOL_DOWN;

    player.ship.o.vel_x = player.ship.o.vel_y = 0.0;
    player.ship.o.loc_x = player.ship.o.loc_y = 50.0;

    for(i=0; i< N_GOONS; i++){
        ctor_badgelandia_basic_object(&(goons[i].o));
        goons[i].o.loc_x = 10 + (i*20);
        goons[i].o.loc_y = 10;
        goons[i].o.vel_x = 10;
        goons[i].o.vel_y = 10;
    }

    FbBackgroundColor(GREY2);
    FbClear();

#ifdef SDL_BADGE
    while(G_Fb.status)
#else
    for(;;)
#endif
    {
#ifndef SDL_BADGE
        if(BUTTON_PRESSED_AND_CONSUME)
            returnToMenus();
#endif
        
        if(UP_BTN_HOLD(3)){
            //green(100);
            //setNote(10*spkr_factor, 2048);
            //spkr_factor++;
            //setNote(100, 1024);
        }

        if(DOWN_BTN_HOLD(3)){
            //green(0);
        }
        
        if(LEFT_BTN_HOLD(3))
        {
            ship_rotation -= ((float)G_left_button_cnt)/150.0;
            if(ship_rotation < 0.0)
                ship_rotation = 6.282 + ship_rotation;

            sin_rot = sin(ship_rotation);
            cos_rot = cos(ship_rotation);
            redraw = 1;
        }

        if(RIGHT_BTN_HOLD(3))
        {
            ship_rotation += ((float)G_right_button_cnt)/150.0;
            if(ship_rotation > 6.282)
                ship_rotation = ship_rotation - 6.282;

            sin_rot = sin(ship_rotation);
            cos_rot = cos(ship_rotation);
            redraw = 1;
        }

        if(redraw || (G_touch_pct > 0)
            || (player.ship.o.vel_x != 0.0)
                || (player.ship.o.vel_y != 0.0)){

            //thruster_cool_down = THRUSTER_COOL_DOWN;
            // Shift the zero point up so it's easier to go slow
            thruster_pct = G_touch_pct > 40 ? G_touch_pct - 40 : 0;
#ifdef SDL_BADGE
            thruster_pct = ((unsigned char) thruster_pct);
#else
            thruster_pct = ((unsigned char) thruster_pct) >> 4;
#endif
                
            //red(thruster_pct*2);

            if( abs(player.ship.o.vel_x) < 6.0)
                player.ship.o.vel_x += thruster_pct * cos_rot;
            if( abs(player.ship.o.vel_y) < 6.0)
                player.ship.o.vel_y += thruster_pct * sin_rot;

            player.ship.o.loc_x += player.ship.o.vel_x;
            player.ship.o.loc_y += player.ship.o.vel_y;

#define HIGH_RESET 132
#define LOW_RESET 1

            // Wrap around
            if(player.ship.o.loc_y  < LOW_RESET)
                player.ship.o.loc_y = HIGH_RESET - 1;
            else if(player.ship.o.loc_y  > HIGH_RESET)
                player.ship.o.loc_y  = LOW_RESET + 1;

            if(player.ship.o.loc_x < LOW_RESET)
                player.ship.o.loc_x=HIGH_RESET - 1;
            else if(player.ship.o.loc_x > HIGH_RESET)
                player.ship.o.loc_x = LOW_RESET + 1;


            FbColor(WHITE);
            create_player_vector(shared_vertices);

            //rotate_points_to(points, 3, ship_rotation);
            rotate_points(shared_vertices, 4, ship_rotation);

            FbPolygonFromPoints(shared_vertices, //points,
                                4,
                                (short)player.ship.o.loc_x,
                                (short)player.ship.o.loc_y);


            // -----
            // DO GOON
            goon_rotation += GOON_ROTATION_DELTA;
            if(goon_rotation > TWO_PI)
                goon_rotation = 0;

            equilateral_polygon_points(shared_vertices, 9.0, 6,
                                       0);
                                       //goon_rotation);
            rotate_points(shared_vertices, 6, goon_rotation);

            FbColor(GREEN);
            for(i=0; i< N_GOONS; i++){
                scale_points(shared_vertices, 6, 1.0/((float)(i+1)));
                FbPolygonFromPoints(shared_vertices,//goon_vertices,
                                    6,
                                    (short)goons[i].o.loc_x,
                                    (short)goons[i].o.loc_y);
            }

            FbSwapBuffers();
            redraw = 0;
            player.ship.o.vel_x /= 1.2;
            player.ship.o.vel_y /= 1.2;

            if(abs(player.ship.o.vel_x) < 0.01)
                player.ship.o.vel_x = 0.0;

            if(abs(player.ship.o.vel_y) < 0.01)
                player.ship.o.vel_y = 0.0;
        }

        vTaskDelay(tick_rate);
    }
}
