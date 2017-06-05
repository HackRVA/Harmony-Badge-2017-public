//
// Created by morgan on 5/20/17.
//

#include "colors.h"

#ifdef SDL_BADGE
//#include "rand.h"
#include "sdl_fb.h"
#include "sdl_buttons.h"
#else
#include "fb.h"
#include "app.h"
#include "buttons.h"
#endif
#include "badge_menu.h"
#include "utils.h"


#define CIRCLE_GRUNT_SPEED 1
#define CIRCLE_GRUNT_HP 100

#define PIXEL_GRUNT_SPEED 5
#define PIXEL_GRUNT_HP 5

#define TRIANGLE_GRUNT_SPEED 5
#define TRIANGLE_GRUNT_HP 5

void enter_spectate_mode();
void enter_build_mode();
void exit_groundwar();

short groundwar_points = 0;
short groundwar_level_health = 100;

struct menu_t groundwar_menu[] = {
    {"return", VERT_ITEM, FUNCTION, {(struct menu_t *)enter_spectate_mode}},
    {"build", VERT_ITEM|DEFAULT_ITEM, FUNCTION, {(struct menu_t *)enter_build_mode}},
    {"exit", VERT_ITEM|LAST_ITEM, FUNCTION, {(struct menu_t *)exit_groundwar}},
};

#define NUM_MINION_TYPES 4

struct groundwar_level_t {
    // Max of 5 points
    short unit_path[6][2];
    unsigned char num_path_points;
    unsigned char distribution_of_units[NUM_MINION_TYPES];
};

enum groundwar_minion_type{
    DOT,
    TRIANGLE,
    SQUARE,
    CIRCLE
};

unsigned char minion_speeds[] = {
        2, 4, 7, 15
};

unsigned char minion_hp[] = {
        1,
        3,
        7,
        17
};

struct groundwar_minion_t {
    unsigned char type;
    unsigned char x, y, current_hp,
            point_heading; // index of point in the level path that this is moving towards
                            // always start at 0
};


#define NUM_DEFENSE_TYPES 3
enum groundwar_defense_type{
    SMALL,
    MEDIUM,
    LARGE,
    UNUSED
};

unsigned char defense_damage[] = {
        1,
        3,
        7
};

unsigned char defense_range[] = {
        10,
        20,
        40
};

unsigned char defense_cooldown[] = {
        2,
        5,
        13
};

#define MAX_DEFENSE_STRUCTURES 30
#define DEFENSE_NOT_TARGETING 255
struct groundwar_projectile_t{
    unsigned char targeted_minion, x, y;
};

struct groundwar_defense_t{
    unsigned char type;
    unsigned char x, y;
    struct groundwar_projectile_t projectile;
};

char selected_defense_idx = 0;

// basic high-level app states
enum groundwar_state {
    INIT,
    DRAW,
    STEP,
    IO,
    SLEEP,
    EXIT
};

enum groundwar_game_state {
    MENUING,
    SPECTATING,
    BUILDING
};

#define NUM_MINIONS 30
#ifdef SDL_BADGE
#define RAND_NUM(s) rand()
#else
//#define RAND_NUM(s) quick_rand(s)
#define RAND_NUM(s) rand()
#endif


enum groundwar_state state = INIT;
enum groundwar_game_state game_state = SPECTATING;

void enter_spectate_mode(){
    game_state = SPECTATING;
}

void enter_build_mode(){
    game_state = BUILDING;
}

void exit_groundwar(){
    state=EXIT;
}

void init_minions(struct groundwar_minion_t minions[NUM_MINIONS],
                  struct groundwar_level_t lvl){

    unsigned char i = 0, type_idx=0, create_count = 0;

    for(i=0; i < NUM_MINIONS && type_idx < NUM_MINION_TYPES; i++){
        // If the minion object doesn't get touched, make sure it is default dead
        minions[i].current_hp = 0;
        if(create_count < lvl.distribution_of_units[type_idx]){
            minions[i].type = type_idx;
            minions[i].current_hp = minion_hp[type_idx];
            minions[i].x = 7 + lvl.unit_path[0][0] +  (RAND_NUM(i) % 7) - (RAND_NUM(i+1) % 7);
            minions[i].y = 7 + lvl.unit_path[0][1] +  (RAND_NUM(i + 2) % 7) - (RAND_NUM(i+ 3) % 7);

            minions[i].point_heading = 1;
            create_count++;
        }
        else if(create_count >= lvl.distribution_of_units[type_idx]) {
            create_count = 0;
            type_idx++;
        }
    }

}


void init_defenses(struct groundwar_defense_t defenses[MAX_DEFENSE_STRUCTURES]){
    unsigned char i = 0, type_idx=0, create_count = 0;
    for(i=0; i < MAX_DEFENSE_STRUCTURES; i++){
        defenses[i].type = UNUSED;
        defenses[i].x = 40;
        defenses[i].y = 40;
        defenses[i].projectile.targeted_minion = DEFENSE_NOT_TARGETING;
        defenses[i].projectile.x = 40;
        defenses[i].projectile.y = 40;
    }
}

void groundwar_step(struct groundwar_minion_t minions[NUM_MINIONS],
                    struct groundwar_defense_t defenses[MAX_DEFENSE_STRUCTURES],
                    struct groundwar_level_t lvl) {
    int dx, dy,
        sx, sy,
        err, e2;
    unsigned char x0, y0, x1, y1, i, j;
    static unsigned char step_cnt = 0;

    // draw our minions
    for(i=0; i < NUM_MINIONS; i++){
        if(minions[i].current_hp == 0){
            continue;
        }
        //if(minions[i].type == TRIANGLE && ((step_cnt%minion_speeds[TRIANGLE]) == 0))
        if(((step_cnt%minion_speeds[minions[i].type]) != 0))
            continue;

        x1 = lvl.unit_path[minions[i].point_heading][0];
        y1 = lvl.unit_path[minions[i].point_heading][1];
        x0 = minions[i].x;
        y0 = minions[i].y;

        dx = abs(x1 - x0);
        dy = abs(y1 - y0);
        sx = x0 < x1 ? 1: -1;
        sy = y0 < y1 ? 1: -1;
        err = (dx > dy ? dx : -dy)/2;

        e2 = err;
        if (e2 > -dx) { err -= dy; x0 += sx; }
        if (e2 < dy) { err += dx; y0 += sy; }


        if(RAND_NUM(i) %3){
            x0 += RAND_NUM(i)%2 - RAND_NUM(i)%2;
            y0 += RAND_NUM(i)%2 - RAND_NUM(i)%2;
        }
        minions[i].x = x0;
        minions[i].y = y0;

        if(dx < 3 && dy < 3){
            if(minions[i].point_heading == (lvl.num_path_points - 1 )){
                //minion is done
                // to do - takeaway player health
                minions[i].current_hp = 0;
                minions[i].type = UNUSED;

                if(groundwar_level_health < minions[i].current_hp)
                    groundwar_level_health = 0;
                else
                    groundwar_level_health -= minions[i].current_hp;
            }
            else{
                minions[i].point_heading++;
            }
        }
    }

#define DEFENSES_TARGET_IDX defenses[i].projectile.targeted_minion
    for(i=0; i < MAX_DEFENSE_STRUCTURES; i++){

        if(defenses[i].type == UNUSED)
            continue;


        if(defenses[i].projectile.targeted_minion == DEFENSE_NOT_TARGETING){
            //TODO:find front minion within range
            for(j=0; j<NUM_MINIONS; j++){
                if(minions[j].type != UNUSED && minions[j].current_hp != 0) {
                    defenses[i].projectile.targeted_minion = j;
                    defenses[i].projectile.x = defenses[i].x;
                    defenses[i].projectile.y = defenses[i].y;

                    break;
                }
            }
        }
        else if(minions[defenses[i].projectile.targeted_minion].type == UNUSED
                    || minions[defenses[i].projectile.targeted_minion].current_hp == 0){
                defenses[i].projectile.targeted_minion = DEFENSE_NOT_TARGETING;
            }
        else{


            path_between_points(&defenses[i].projectile.x,
                                &defenses[i].projectile.y,
                                minions[DEFENSES_TARGET_IDX].x, minions[DEFENSES_TARGET_IDX].y);
            // If projectile gets close, decrease health of minion
            // and destroy projectile
            if((abs(minions[DEFENSES_TARGET_IDX].x - defenses[i].projectile.x) < 2)
                    &&(abs(minions[DEFENSES_TARGET_IDX].y - defenses[i].projectile.y) < 2))
            {

                if(minions[DEFENSES_TARGET_IDX].current_hp <= defense_damage[defenses[i].type]) {
                    minions[DEFENSES_TARGET_IDX].type = UNUSED;
                    minions[DEFENSES_TARGET_IDX].current_hp = 0;
                    groundwar_points += minion_hp[minions[DEFENSES_TARGET_IDX].type];
                    defenses[i].projectile.targeted_minion = DEFENSE_NOT_TARGETING;
                }
                else{
                    minions[DEFENSES_TARGET_IDX].current_hp -= defense_damage[defenses[i].type];
                }
            }
        }
    }

    step_cnt++;
}

void groundwar_draw_defense(unsigned char x, unsigned char y,
                            unsigned char defense_id,
                            unsigned char with_placement_outline) {

    switch(defense_id){
        case SMALL:
            if(with_placement_outline){

                FbColor(WHITE);
                FbMove(x-6, y-6);
                FbRectangle(11, 11);
            }
            // Draw stem
            FbMove(x-1, y-5);
            FbColor(GREEN);
            FbFilledRectangle(3, 10);

            FbMove(x-4, y-5);
            FbFilledRectangle(10, 3);

            break;
        case MEDIUM:
            break;
        case LARGE:
            break;
    }

}

void groundwar_draw(struct groundwar_minion_t minions[NUM_MINIONS],
                    struct groundwar_defense_t defenses[MAX_DEFENSE_STRUCTURES],
                    struct groundwar_level_t lvl){
    unsigned char i = 0;
    short shared_vertices[8][2] = {0};
    static unsigned char place_x = 40, place_y = 40;

    FbColor(WHITE);
    FbDrawVectors(lvl.unit_path, lvl.num_path_points, 5, 5, 0);
    for(i=0; i < NUM_MINIONS; i++){
        if(minions[i].current_hp == 0)
            continue;

        switch (minions[i].type){
            case DOT:
                FbColor(GREEN);
                FbMove(minions[i].x, minions[i].y);
                FbFilledRectangle(2, 2);
                break;
            case TRIANGLE:
                FbColor(YELLOW);
                equilateral_polygon_points(shared_vertices, 5.0, 3,
                                           0);

                FbPolygonFromPoints(shared_vertices,//goon_vertices,
                                    3,
                                    (short)minions[i].x,
                                    (short)minions[i].y);
                break;
            case SQUARE:
                FbColor(RED);
                FbMove(minions[i].x,
                       minions[i].y);
                FbFilledRectangle(7, 7);
                break;
            case CIRCLE:
                FbColor(WHITE);
                equilateral_polygon_points(shared_vertices, 8.0, 8,
                                           0);
                FbPolygonFromPoints(shared_vertices,//goon_vertices,
                                    8,
                                    (short)minions[i].x,
                                    (short)minions[i].y);
                break;
        }
    }

    for(i=0; i<MAX_DEFENSE_STRUCTURES && i != selected_defense_idx; i++){

        groundwar_draw_defense(
                defenses[i].x,
                defenses[i].y,
                defenses[i].type,
                0
        );

        if(defenses[i].projectile.targeted_minion != DEFENSE_NOT_TARGETING){

            FbColor(WHITE);
            FbMove(defenses[i].projectile.x, defenses[i].projectile.y);
            FbFilledRectangle(2, 2);
        }
    }

    if(game_state != BUILDING)
        return;

    //FbMove(2, 32);
    //FbColor(GREEN);
    groundwar_draw_defense(defenses[selected_defense_idx].x,
                           defenses[selected_defense_idx].y,
                           defenses[selected_defense_idx].type, 1);

}



void groundwar_io(struct groundwar_defense_t defenses[MAX_DEFENSE_STRUCTURES],
                  struct groundwar_level_t lvl){

#define selected_defense defenses[selected_defense_idx]
    // main button brings up menu and is used to select items in the menu?
    if(game_state != MENUING){
        if(BUTTON_PRESSED_AND_CONSUME){
            game_state = MENUING;
        }

        if(game_state == BUILDING){

            if(UP_BTN_AND_CONSUME){
                selected_defense.y -= 2;
            }
            if(DOWN_BTN_AND_CONSUME){
                selected_defense.y += 2;
            }
            if(LEFT_BTN_AND_CONSUME){
                selected_defense.x -= 2;
            }
            if(RIGHT_BTN_AND_CONSUME){
                selected_defense.x += 2;
            }

            if(selected_defense.type == UNUSED)
                selected_defense.type = SMALL;

            if(UP_TOUCH_AND_CONSUME){
                if(selected_defense.type == LARGE)
                    selected_defense.type = SMALL;
                else {
                    selected_defense.type = (selected_defense.type++);

                }
            }

            if(DOWN_TOUCH_AND_CONSUME){
                selected_defense_idx++;
                selected_defense.x = defenses[selected_defense_idx-1].x - 5;
                selected_defense.y = defenses[selected_defense_idx-1].y - 5;
                selected_defense.type = defenses[selected_defense_idx-1].type;

            }
        }
    }
}


void groundwar_task(void* p_arg) {
    unsigned char cnt= 0;
    const TickType_t tick_rate = 10 / portTICK_PERIOD_MS;

    struct groundwar_minion_t groundwar_minions[NUM_MINIONS] = {0};
    struct groundwar_defense_t defenses[MAX_DEFENSE_STRUCTURES];

    struct groundwar_level_t lvl = {
            .unit_path = {{10, 0}, {10, 30}, {70, 30}, {70, 90}, {120, 90}},
            .num_path_points = 5,
            //.distribution_of_units = {15, 4, 2, 1}
            .distribution_of_units = {15, 0, 0, 0}
    };

#ifdef SDL_BADGE
    while(G_Fb.status)
#else
    for(;;)
#endif
    {
        switch (state){
            case INIT:
                init_minions(groundwar_minions, lvl);
                init_defenses(defenses);
                // TEST BUILD
                //enter_build_mode();
                defenses[0].type = SMALL;
                defenses[0].x = 10;
                defenses[0].y = 50;
                selected_defense_idx = 1;

                state = DRAW;
                break;
            case DRAW:
                if(game_state == MENUING) {
                    genericMenu((struct menu_t *) groundwar_menu,
                                WHITE_ON_BLACK);
                }
                else {
                    groundwar_draw(groundwar_minions, defenses, lvl);
                    state = IO;
                }

                FbSwapBuffers();
                break;
            case IO:
                if(game_state != MENUING){
                    groundwar_io(defenses, lvl);
                }
                state = STEP;
                break;
            case STEP:
#ifdef SDL_BADGE
                if(cnt++ == 105) {
#else
                    {
#endif
                    if(game_state != MENUING)
                        groundwar_step(groundwar_minions, defenses, lvl);

                    cnt = 0;
                }
                state = DRAW;
                break;
            case SLEEP:
                state = DRAW;
                vTaskDelay(tick_rate);
                break;
            case EXIT:
                state = INIT;
#ifndef SDL_BADGE
                returnToMenus();
#else
                G_Fb.status = 0;
#endif
        }

    }
}

