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
#include <stdio.h>
#include <stdlib.h>
//#include <stdio.h>


#define CIRCLE_GRUNT_SPEED 1
#define CIRCLE_GRUNT_HP 100

#define PIXEL_GRUNT_SPEED 5
#define PIXEL_GRUNT_HP 5

#define TRIANGLE_GRUNT_SPEED 5
#define TRIANGLE_GRUNT_HP 5

void enter_spectate_mode();
void enter_build_mode();
void exit_groundwar();

short groundwar_points;
short groundwar_level_health;

struct menu_t groundwar_menu[] = {
    {"return", VERT_ITEM, FUNCTION, {(struct menu_t *)enter_spectate_mode}},
    {"build", VERT_ITEM|DEFAULT_ITEM, FUNCTION, {(struct menu_t *)enter_build_mode}},
    {"exit", VERT_ITEM|LAST_ITEM, FUNCTION, {(struct menu_t *)exit_groundwar}},
};

#define NUM_MINION_TYPES 4
#define NUM_WAVES 5

struct groundwar_level_t {
    // Max of 5 points
    short unit_path[6][2];
    unsigned char num_path_points;
    unsigned char current_wave;
    unsigned char distribution_of_units[NUM_WAVES][NUM_MINION_TYPES];
};

enum groundwar_minion_type{
    DOT,
    TRIANGLE,
    SQUARE,
    CIRCLE,
    UNUSED_MINION
};

const unsigned char minion_speeds[] = {
        2, 4, 7, 15
};

const unsigned char minion_hp[] = {
        2,
        12,
        64,
        150
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
    UNUSED_DEFENSE
};


short defense_cost[] = {10, 35, 55};
const unsigned char defense_damage[] = {
        1,
        3,
        7
};

const unsigned char defense_range[] = {
        23,
        30,
        60
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
    INIT_LEVEL,
    INIT_WAVE,
    DRAW,
    STEP,
    NEXT_WAVE,
    IO,
    GAME_OVER,
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

#define WAVES_DISTRIBUTION lvl.distribution_of_units[lvl.current_wave]

enum groundwar_state state = INIT_LEVEL;
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

    while(i < NUM_MINIONS && type_idx < NUM_MINION_TYPES){
    //for(i=0; i < NUM_MINIONS && type_idx < NUM_MINION_TYPES; i++){
        // If the minion object doesn't get touched, make sure it is default dead
        minions[i].current_hp = 0;
        //if(create_count < lvl.distribution_of_units[type_idx]){
        if(create_count < WAVES_DISTRIBUTION[type_idx]){
            minions[i].type = type_idx;
            minions[i].current_hp = minion_hp[type_idx];
            minions[i].x = 7 + lvl.unit_path[0][0] +  (RAND_NUM(i) % 7) - (RAND_NUM(i+1) % 7);
            minions[i].y = 7 + lvl.unit_path[0][1] +  (RAND_NUM(i + 2) % 7) - (RAND_NUM(i+ 3) % 7);

            minions[i].point_heading = 1;
            create_count++;
            i++;
        }
        //else if(create_count >= lvl.distribution_of_units[type_idx]) {
        else if(create_count >= WAVES_DISTRIBUTION[type_idx]) {
            create_count = 0;
            type_idx++;
        }
    }

}


void init_defenses(struct groundwar_defense_t defenses[MAX_DEFENSE_STRUCTURES]){
    unsigned char i = 0, type_idx=0, create_count = 0;
    for(i=0; i < MAX_DEFENSE_STRUCTURES; i++){
        defenses[i].type = UNUSED_DEFENSE;
        defenses[i].x = 40;
        defenses[i].y = 50;
        defenses[i].projectile.targeted_minion = DEFENSE_NOT_TARGETING;
        defenses[i].projectile.x = 40;
        defenses[i].projectile.y = 40;
    }
}


//void init_level_0(struct )

unsigned char groundwar_step(struct groundwar_minion_t minions[NUM_MINIONS],
                    struct groundwar_defense_t defenses[MAX_DEFENSE_STRUCTURES],
                    struct groundwar_level_t lvl) {
    unsigned char x1, y1, i, j, alive_count=0;
    static unsigned char step_cnt = 0;

    // draw our minions
    for(i=0; i < NUM_MINIONS; i++){
        if(minions[i].current_hp == 0){
            continue;
        }
        alive_count++;

        //if(minions[i].type == TRIANGLE && ((step_cnt%minion_speeds[TRIANGLE]) == 0))
        if(((step_cnt%minion_speeds[minions[i].type]) != 0))
            continue;

        x1 = lvl.unit_path[minions[i].point_heading][0];
        y1 = lvl.unit_path[minions[i].point_heading][1];
        path_between_points(&minions[i].x, &minions[i].y,
                             x1, y1);

        if(RAND_NUM(i) %3){
            minions[i].x += RAND_NUM(i)%2 - RAND_NUM(i)%2;
            minions[i].y += RAND_NUM(i)%2 - RAND_NUM(i)%2;
        }

        if( abs(minions[i].x - lvl.unit_path[minions[i].point_heading][0]) < 3
            && abs(minions[i].y - lvl.unit_path[minions[i].point_heading][1]) < 3){
            if(minions[i].point_heading == (lvl.num_path_points - 1 )){
                //minion is done
                // to do - takeaway player health

                if(groundwar_level_health < minions[i].current_hp)
                    groundwar_level_health = 0;
                else
                    groundwar_level_health -= minions[i].current_hp;

                minions[i].current_hp = 0;
                minions[i].type = UNUSED_MINION;
            }
            else{
                minions[i].point_heading++;
            }
        }
    }
    if(alive_count == 0){
        state = NEXT_WAVE;
        return 0;
    }

#define DEFENSES_TARGET_IDX defenses[i].projectile.targeted_minion
    for(i=0; i < MAX_DEFENSE_STRUCTURES && i < selected_defense_idx; i++){

        if(defenses[i].type == UNUSED_DEFENSE)
            continue;

        if(defenses[i].projectile.targeted_minion == DEFENSE_NOT_TARGETING){
            //TODO:find front minion within range
            for(j=0; j<NUM_MINIONS; j++){
                if(minions[j].type != UNUSED_MINION && minions[j].current_hp != 0
&& distance_between_coords(minions[j].x, minions[j].y, defenses[i].x, defenses[i].y) <= defense_range[defenses[i].type]) {
                    defenses[i].projectile.targeted_minion = j;
                    defenses[i].projectile.x = defenses[i].x;
                    defenses[i].projectile.y = defenses[i].y;

                    break;
                }
            }
        }
        else if(minions[defenses[i].projectile.targeted_minion].type == UNUSED_MINION
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
                    groundwar_points += (unsigned short)minion_hp[minions[DEFENSES_TARGET_IDX].type]>>1;
                    minions[DEFENSES_TARGET_IDX].type = UNUSED_MINION;
                    minions[DEFENSES_TARGET_IDX].current_hp = 0;
                    setNote(50, 700);
                }
                else{
                    minions[DEFENSES_TARGET_IDX].current_hp -= defense_damage[defenses[i].type];
                }
                defenses[i].projectile.targeted_minion = DEFENSE_NOT_TARGETING;
            }
        }
    }

    step_cnt++;
    return alive_count;
}

void groundwar_draw_defense(unsigned char x, unsigned char y,
                            unsigned char defense_id,
                            unsigned char with_placement_outline) {

    switch(defense_id){
        case SMALL:
            if(with_placement_outline){

                FbColor(WHITE);
                FbMove(x-5, y-6);
                FbRectangle(11, 11);
            }
            // Draw stem
            FbMove(x-1, y-5);
            FbColor(MAGENTA);
            FbFilledRectangle(3, 10);

            FbColor(GREY16);
            FbMove(x-4, y-5);
            FbFilledRectangle(10, 3);

            break;
        case MEDIUM:
            if(with_placement_outline){
                FbColor(WHITE);
                FbMove(x-6, y-6);
                FbRectangle(11, 11);
            }
            FbColor(MAGENTA);
            FbMove(x-3, y-5);
            FbFilledRectangle(6, 2);

            FbColor(GREEN);
            FbMove(x-1, y-5);
            FbFilledRectangle(2, 5);

            FbColor(GREY16);
            FbMove(x-5, y);
            FbFilledRectangle(10, 5);

            break;
        case LARGE:

            if(with_placement_outline){
                FbColor(WHITE);
                FbMove(x-6, y-6);
                FbRectangle(12, 11);
            }
            FbColor(MAGENTA);
            FbMove(x-5, y-5);
            FbFilledRectangle(11, 5);

            FbColor(GREEN);
            FbMove(x-5, y);
            FbFilledRectangle(2, 5);

            FbMove(x-1, y);
            FbFilledRectangle(1, 5);

            FbMove(x+1, y);
            FbFilledRectangle(1, 5);

            FbMove(x+4, y);
            FbFilledRectangle(2, 5);

            break;
    }

}

void groundwar_draw(struct groundwar_minion_t minions[NUM_MINIONS],
                    struct groundwar_defense_t defenses[MAX_DEFENSE_STRUCTURES],
                    struct groundwar_level_t lvl){
    unsigned char i = 0;
    short shared_vertices[8][2] = {0};
    unsigned char pt_str[6] = {' ', ' ', ' ', ' ', ' ', ' '};

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

    //Seperator
    FbColor(YELLOW);
    FbLine(1, 118, 130, 118);

    // -----
    // LIST health
    if(groundwar_level_health > 80)
        FbColor(GREEN);
    else if(groundwar_level_health > 50)
        FbColor(YELLOW);
    else
        FbColor(RED);
    FbMove(85, 120);
    FbWriteLine("HP:");

    FbMove(105, 120);

    badge_itoa((int)groundwar_level_health, pt_str);
    FbWriteLine(pt_str);

    // -----
    // List points
    FbMove(1, 120);
    FbColor(WHITE);
    FbWriteLine("PTS:");
    badge_itoa((int)groundwar_points, pt_str);

    FbMove(30, 120);
    FbWriteLine(pt_str);


    if(game_state != BUILDING)
        return;

//    FbColor(WHITE);
//    FbMove(120, 90);
//    FbFilledRectangle(12, 40);
//
//
//    FbColor(BLACK);
//    FbMove(121, 100);
//    FbCharacter('P');
//    FbMove(121, 110);
//    FbCharacter('U');
//    FbMove(121, 120);
//    FbCharacter('T');
    FbMove(1, 110);
    if(defense_cost[defenses[selected_defense_idx].type] <= groundwar_points){
        FbColor(WHITE);
    } else
        FbColor(YELLOW);
    FbWriteLine("Cost:");

    badge_itoa((int)defense_cost[defenses[selected_defense_idx].type], pt_str);
    FbWriteLine(pt_str);

    groundwar_draw_defense(defenses[selected_defense_idx].x,
                           defenses[selected_defense_idx].y,
                           defenses[selected_defense_idx].type, 1);

}


#define HOLD_MOVE_CNT 50
void groundwar_io(struct groundwar_defense_t defenses[MAX_DEFENSE_STRUCTURES],
                  struct groundwar_level_t lvl){

    unsigned char i=0, is_colliding = 0;
#define selected_defense defenses[selected_defense_idx]
    // main button brings up menu and is used to select items in the menu?
    if(game_state != MENUING){
        if(BUTTON_PRESSED_AND_CONSUME){
            game_state = MENUING;
            setNote(70, 256);
        }

        if(game_state == BUILDING){

            if(UP_BTN_AND_CONSUME || UP_BTN_HOLD(HOLD_MOVE_CNT)){
                selected_defense.y -= 2;
                setNote(70, 256);
            }
            if(DOWN_BTN_AND_CONSUME || DOWN_BTN_HOLD(HOLD_MOVE_CNT)){
                selected_defense.y += 2;
                setNote(70, 256);
            }
            if(LEFT_BTN_AND_CONSUME || LEFT_BTN_HOLD(HOLD_MOVE_CNT)){
                selected_defense.x -= 2;
                setNote(70, 256);
            }
            if(RIGHT_BTN_AND_CONSUME || RIGHT_BTN_HOLD(HOLD_MOVE_CNT)){
                selected_defense.x += 2;
                setNote(70, 256);
            }

            if(selected_defense.type == UNUSED_DEFENSE)
                selected_defense.type = MEDIUM;

            if(UP_TOUCH_AND_CONSUME){
                if(selected_defense.type == LARGE)
                    selected_defense.type = SMALL;
                else {
                    selected_defense.type = (selected_defense.type+1);
                    setNote(88, 512);
                }
            }

            if(DOWN_TOUCH_AND_CONSUME){

                for(i=0; i< MAX_DEFENSE_STRUCTURES && defenses[i].type != UNUSED_DEFENSE; i++){

                    // don't compare to self!
                    if(i == selected_defense_idx) continue;

                    is_colliding |= check_box_collision(selected_defense.x,
                                                        selected_defense.y,
                                                        10, 10,
                                                        defenses[i].x, defenses[i].y,
                                                        10, 10);
                }

                if(!is_colliding &&
                        (groundwar_points >= defense_cost[selected_defense.type]))
                {
                    groundwar_points -= defense_cost[selected_defense.type];
                    selected_defense_idx++;
                    selected_defense.x = defenses[selected_defense_idx - 1].x - 5;
                    selected_defense.y = defenses[selected_defense_idx - 1].y - 5;
                    selected_defense.type = defenses[selected_defense_idx - 1].type;
                    setNote(70, 256);
                }
                else{
                    setNote(90, 756);
                }
            }
        }
    }
}


#define SET_LVL_WAVE_DIST(wave_n, d, t, s, c) \
lvl.distribution_of_units[wave_n][DOT] = d;\
lvl.distribution_of_units[wave_n][TRIANGLE] = t;\
lvl.distribution_of_units[wave_n][SQUARE] = s;\
lvl.distribution_of_units[wave_n][CIRCLE] = c;\

void groundwar_task(void* p_arg) {
    unsigned char cnt= 0;
    const TickType_t tick_rate = 5 / portTICK_PERIOD_MS;
    unsigned char wave_num = 0, current_level = 0, unlocked_level = 0, alive_count=0;
    unsigned char str[8];

    struct groundwar_minion_t groundwar_minions[NUM_MINIONS] = {0};
    struct groundwar_defense_t defenses[MAX_DEFENSE_STRUCTURES];

    struct groundwar_level_t lvl = {
            .unit_path = {{10, 0}, {10, 30}, {70, 30}, {70, 90}, {110, 90}},
            .num_path_points = 5,
            .current_wave = 0,
            //.distribution_of_units = {15, 4, 2, 1}
            .distribution_of_units = {
                    {0, 0, 0, 0},
                    {0, 0, 0, 0},
                    {0, 0, 0, 0},
                    {0, 0, 0, 0},
                    {0, 0, 0, 0},
            }
    };

#ifdef SDL_BADGE
    while(G_Fb.status)
#else
    for(;;)
#endif
    {
        switch (state){
            case INIT_LEVEL:
                lvl.current_wave = 0;
                init_defenses(defenses);
                selected_defense_idx = 0;
                if(current_level == 0){
                    groundwar_level_health = 100;
                    groundwar_points = 15;
                    SET_LVL_WAVE_DIST(0, 3, 0, 0, 0);
                    SET_LVL_WAVE_DIST(1, 7, 0, 0, 0);
                    SET_LVL_WAVE_DIST(2, 10, 1, 0, 0);
                    SET_LVL_WAVE_DIST(3, 15, 2, 0, 0);
                    SET_LVL_WAVE_DIST(4, 25, 3, 0, 0);
                }
                else if(current_level == 1){
                    groundwar_level_health = 100;
                    groundwar_points = 20;
                    SET_LVL_WAVE_DIST(0, 5, 1, 0, 0);
                    SET_LVL_WAVE_DIST(1, 10, 2, 0, 0);
                    SET_LVL_WAVE_DIST(2, 12, 2, 0, 0);
                    SET_LVL_WAVE_DIST(3, 25, 2, 0, 0);
                    SET_LVL_WAVE_DIST(4, 25, 5, 0, 0);
                }
                else if(current_level == 2){
                    groundwar_level_health = 100;
                    groundwar_points = 20;
                    SET_LVL_WAVE_DIST(0, 10, 2, 0, 0);
                    SET_LVL_WAVE_DIST(1, 10, 3, 1, 0);
                    SET_LVL_WAVE_DIST(2, 12, 4, 2, 0);
                    SET_LVL_WAVE_DIST(3, 15, 4, 3, 0);
                    SET_LVL_WAVE_DIST(4, 15, 5, 3, 0);
                }
                else if(current_level == 3){
                    groundwar_level_health = 100;
                    groundwar_points = 30;
                    SET_LVL_WAVE_DIST(0, 10, 4, 1, 0);
                    SET_LVL_WAVE_DIST(1, 10, 4, 2, 0);
                    SET_LVL_WAVE_DIST(2, 12, 5, 4, 1);
                    SET_LVL_WAVE_DIST(3, 15, 5, 5, 1);
                    SET_LVL_WAVE_DIST(4, 15, 6, 5, 1);
                }
                else if(current_level == 4){
                    groundwar_level_health = 100;
                    groundwar_points = 40;
                    SET_LVL_WAVE_DIST(0, 10, 6, 1, 1);
                    SET_LVL_WAVE_DIST(1, 10, 7, 2, 2);
                    SET_LVL_WAVE_DIST(2, 10, 9, 4, 2);
                    SET_LVL_WAVE_DIST(3, 10, 11, 4, 3);
                    SET_LVL_WAVE_DIST(4, 10, 13, 5, 5);
                }                
                // TEST BUILD
                //enter_build_mode();
//                defenses[0].type = LARGE;
//                defenses[0].x = 10;
//                defenses[0].y = 50;
//                selected_defense_idx = 1;

                //break;
            case INIT_WAVE:
                init_minions(groundwar_minions, lvl);
                alive_count = 1;
                state = DRAW;
                break;
            case DRAW:
                if(game_state == MENUING) {
                    genericMenu((struct menu_t *) groundwar_menu,
                                WHITE_ON_BLACK);
                }
                else {
                    groundwar_draw(groundwar_minions, defenses, lvl);
                    // Throw in level and wave number
                    FbColor(WHITE);
                    FbMove(50, 2);
                    FbWriteLine("LVL:");
                    badge_itoa((int)current_level, str);
                    FbMove(70, 2);
                    FbWriteLine(str);

                    FbColor(WHITE);
                    FbMove(98, 2);
                    FbWriteLine("WV:");
                    badge_itoa((int)lvl.current_wave, str);
                    FbMove(105, 2);
                    FbWriteLine(str);
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
                        alive_count = groundwar_step(groundwar_minions, defenses, lvl);

                    cnt = 0;
                }
                if(groundwar_level_health == 0){
                    state = GAME_OVER;
                }
                else if(alive_count)
                    state = DRAW;
                else
                    state = NEXT_WAVE;
                break;
            case NEXT_WAVE:
                if(lvl.current_wave < NUM_WAVES - 1){
                    lvl.current_wave++;
                    state = INIT_WAVE;
                }
                else{
                    current_level++;
                    state = INIT_LEVEL;
                    //TODO: next level
                }
                break;
            case GAME_OVER:
                FbMove(10, 60);
                FbColor(RED);
                FbWriteLine("GAME OVER!");
                FbPushBuffer();
                vTaskDelay(5000/portTICK_PERIOD_MS);
                state = EXIT;
                break;

            case SLEEP:
                state = DRAW;
                vTaskDelay(tick_rate);
                break;
            case EXIT:
                state = INIT_LEVEL;
#ifndef SDL_BADGE
                returnToMenus();
#else
                G_Fb.status = 0;
#endif
        }

    }
}

