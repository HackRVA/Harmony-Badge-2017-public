#include "app.h"
#include "colors.h"
#include "fb.h"
#include "assetList.h"
#include "buttons.h"
#include "utils.h"

#define FALSE 0;
#define TRUE !FALSE;
#define SET_RAND(X) (unsigned char)(quick_rand(G_up_touch_cnt)%X)*game.level
#define N_STARS 15
#define MAX_TORP 10

//Game modes
unsigned int ss_game_counter = 0;
unsigned char ss_ai_count = 0;

enum {
    SS_BATTLE,
    SS_CRUISE,
    SS_GAME_OVER,
    SS_NEW_LEVEL,
};

enum {
    SS_AI,
    SS_PLAYER,
};

//Game control struct
struct _game {
    unsigned char level;
    unsigned char mode;
    unsigned char mode_last;
    void (*game_state)(void);
};

struct _ss_asteroid {
    unsigned char x;
    unsigned char y;
    unsigned char exist;
};

struct _ss_stars {
    unsigned char x[N_STARS];
    unsigned char y[N_STARS];
    unsigned char move;
    struct _ss_asteroid asteroid;
};

struct _ss_torp_map {
    unsigned char player_x[MAX_TORP];
    unsigned char player_y[MAX_TORP];
    unsigned char player_phaser_on;
    unsigned char ai_x[MAX_TORP];
    unsigned char ai_y[MAX_TORP];
};

//Player structure
struct _ss_player {
    unsigned char x;
    unsigned char y;
    unsigned char torps;
    char shields;
    unsigned char phasers;
    char hull;
    unsigned char team;
    unsigned short sprite[5];
};

//Game State Functions
void ss_init(void);
void ss_draw(void);
void ss_events(void);
void ss_exit(void);

//render functions
void ss_init_stars(void);
void ss_draw_stars_and_torps(void);
void ss_draw_player(struct _ss_player * ship_to_draw);
void ss_draw_hit(struct _ss_player * ship);

//play functions
void ss_do_ai(void);
void ss_reset_player(void);
void ss_phasers(void);
void ss_torps(struct _ss_player * ship);
unsigned char ss_detect_hits(struct _ss_player * ship);
void ss_init_alien(void);
unsigned short explosion[] = {0b0000001010001000,
                              0b0001010101010000,
                              0b0000111110100000,
                              0b0111010111001000,
                              0b0000010010000000,};
unsigned short shields[] = {0b0001111110001100,
                            0b0111100000000010,
                            0b1111000000000001,
                            0b0111100000000010,
                            0b0001111110001100,};
//Game control variables
struct _game game = {1,SS_NEW_LEVEL,SS_NEW_LEVEL,ss_init};
struct _ss_player ss_player = {16,16,10,100,100,100,SS_PLAYER,{0b1111111000011000,
                                                               0b0000110001111110,
                                                               0b0001111111111111,
                                                               0b0000110001111110,
                                                               0b1111111000011000,}};
struct _ss_player ss_ai;
struct _ss_stars ss_stars;
struct _ss_torp_map ss_torp_map;

void star_shooter_task(void* p_arg){
    static unsigned char init = 1;
    if(init) {
        game.game_state = ss_init;
        init = 0;
    }
    const TickType_t ss_tick_rate = 20 / portTICK_PERIOD_MS;
    for(;;) {
        game.game_state();
        FbSwapBuffers();
        vTaskDelay(ss_tick_rate);
        ss_game_counter++;
        if(ss_game_counter == 65000) {
            ss_game_counter = 0;
        }
        if(!(ss_game_counter%100)&&game.mode == SS_CRUISE) {
            if(ss_ai_count <= 10) {
                game.mode_last = game.mode;
                game.mode = SS_BATTLE;
                ss_ai_count++;
                ss_init_alien();
            }
            else {
                game.mode_last = game.mode;
                game.mode = SS_NEW_LEVEL;
                ss_reset_player();
            }
        }
    } 
}

void ss_init(void) {
    unsigned char x;
    FbBackgroundColor(BLACK);
    FbClear();
    ss_init_stars();
    for(x=0;x<MAX_TORP;x++) {
        ss_torp_map.player_x[x] = 0;
        ss_torp_map.player_y[x] = 0;
        ss_torp_map.ai_x[x] = 0;
        ss_torp_map.ai_y[x] = 0;
    }
    ss_torp_map.player_phaser_on = FALSE;
    game.mode_last = game.mode;
    game.mode = SS_CRUISE;
    
    game.game_state = ss_draw;
}

void ss_draw(void) {
    ss_draw_stars_and_torps();
    ss_draw_player(&ss_player);
    if(game.mode == SS_BATTLE) {
        ss_draw_player(&ss_ai);
        if(ss_detect_hits(&ss_player)) {
            ss_draw_hit(&ss_player);
        }
        if(ss_detect_hits(&ss_ai)) {
            ss_draw_hit(&ss_ai);
        }
    }
    
    game.game_state = ss_events;
}

void ss_do_ai(void) {
    if(ss_ai.hull <= 0 && game.mode == SS_BATTLE) {
        game.mode_last = game.mode;
        game.mode = SS_CRUISE;
    }
    else {
        if(((ss_ai.y-ss_player.y)<3 && (ss_ai.y-ss_player.y)>0) ||
           ((ss_player.y-ss_ai.y)<3 && (ss_player.y-ss_ai.y)>0)) {
            ss_torps(&ss_ai);
        }
        if(ss_ai.hull < 5) {//AI Run
            if((ss_ai.x-ss_player.x) < 20) {
                if(ss_ai.x < (132-16)) {
                    ss_ai.x++;
                }
                if((ss_ai.y-ss_player.y) > 0) {
                    if((ss_ai.y-ss_player.y) > 7) {
                        if(ss_ai.y < (132-5)) {
                            ss_ai.y ++;
                        }
                    }
                }
                else {
                    if((ss_player.y-ss_ai.y) > 7) {
                        if(ss_ai.y > 0){
                            ss_ai.y --;
                        }
                    }
                }
            }
        }
        else {//AI Attack
            if((ss_ai.y - ss_player.y) > 0) {
                if((ss_ai.y - ss_player.y) > 2) {
                    ss_ai.y --;
                }
            }
            else {
                if((ss_player.y-ss_ai.y) > 2) {
                    ss_ai.y ++;
                }
            }
            if((ss_ai.x - ss_player.x) > 35) {
                ss_ai.x --;
            }
            else if((ss_ai.x - ss_player.x) < 22){
                ss_ai.x ++;
            }
        }
    }
}

void ss_check_dead(void) {
    if(ss_player.hull <= 0) {
        game.mode_last = game.mode;
        game.mode = SS_GAME_OVER;
        game.game_state = ss_exit;
    }
}

void ss_events(void) {
    game.game_state = ss_draw;
    ss_torp_map.player_phaser_on = FALSE;
    if(BUTTON_PRESSED_AND_CONSUME) {
        game.game_state = ss_exit;
    }
    if(DOWN_BTN) {
        if(ss_player.y<129) {
            ss_player.y++;
        }
    }
    if(UP_BTN) {
        if(ss_player.y>0) {
            ss_player.y--;
        }
    }
    if(LEFT_BTN) {
        if(ss_player.x>0) {
            ss_player.x--;
        }
    }
    if(RIGHT_BTN) {
        if(ss_player.x<126) {
            ss_player.x++;
        }
    }
    if(DOWN_TOUCH_AND_CONSUME) {
        ss_torps(&ss_player);
    }
    if(UP_TOUCH_AND_CONSUME) {
        ss_phasers();
    }
    ss_do_ai();
    ss_check_dead();
}

void ss_init_stars(void) {
    unsigned char j,k;
    k=0;
    for(j=0;j<N_STARS;j++) {
        k+=20;
        ss_stars.x[j]=(132*j)/N_STARS;
        ss_stars.y[j]=(unsigned char)((quick_rand(j+k))%132)*game.level;
    }
    ss_stars.move = TRUE;
    ss_stars.asteroid.exist = FALSE;
    ss_stars.asteroid.x = 0;
    ss_stars.asteroid.y = 0;
}

void ss_init_alien(void) {
    ss_ai.x = 100;
    ss_ai.hull = SET_RAND(10);
    ss_ai.shields = SET_RAND(10);
    ss_ai.torps = 100;
    ss_ai.phasers = 0;
    ss_ai.team = SS_AI;
    ss_ai.y = (unsigned char)(quick_rand(7)%100)+10;
    ss_ai.sprite[0] = 0b0000000000111111;
    ss_ai.sprite[1] = 0b0011000001111100;
    ss_ai.sprite[2] = 0b0111111111111110;
    ss_ai.sprite[3] = 0b0011000001111100;
    ss_ai.sprite[4] = 0b0000000000111111;
}

void ss_phasers(void) {
    ss_torp_map.player_phaser_on = TRUE;
}

void ss_torps(struct _ss_player * ship) {
    unsigned char j;
    if(ship->torps == 0) {
        return;
    }
    else {
        ship->torps--;
    }
    if(ship->team==SS_PLAYER) {
        //if torp x value is zero then it doesn't exist
        for(j=0;ss_torp_map.player_x[j]>0&&j<MAX_TORP;j++) {}
        if(j<MAX_TORP) {
            //create a torpedo in this position
            ss_torp_map.player_x[j]=ship->x+16;
            ss_torp_map.player_y[j]=ship->y+2;
        }    
    }
    else {
        for(j=0;ss_torp_map.ai_x[j]>0&&j<MAX_TORP;j++) {}
        if(j<MAX_TORP) {
            ss_torp_map.ai_x[j]=ship->x;
            ss_torp_map.ai_y[j]=ship->y;
        }
    }
}

void ss_draw_game_over(void) {
    FbClear();
    FbColor(RED);
    FbMove(10,65);
    FbWriteLine("game over");
    FbSwapBuffers();
    vTaskDelay(2000);
}

void ss_reset_player(void) {
    ss_player.hull=100;
    ss_player.phasers=100;
    ss_player.shields=100;
    ss_player.x=16;
    ss_player.y=16;
    ss_player.torps=10;
}

void ss_exit(void) {
    if(game.mode == SS_GAME_OVER) {
        game.game_state = ss_init;
        ss_reset_player();
        ss_draw_game_over();
    }
    else {
        game.game_state = ss_draw;
    }
    returnToMenus();
}

void ss_draw_stars_and_torps(void) {
    unsigned char j;
    FbColor(WHITE);
    for(j=0;j<N_STARS;j++) {
        FbPoint(ss_stars.x[j],ss_stars.y[j]);//draw stars
        if(game.mode == SS_CRUISE) {
            if(ss_stars.x[j]==0) {//replace stars that move off screen
                ss_stars.x[j]=132;
                ss_stars.y[j]=SET_RAND(132);
            }
            else {
                ss_stars.x[j]--;//move stars
            }
        }
    }
    FbColor(RED);
    for(j=0;j<MAX_TORP;j++) {
        if(ss_torp_map.ai_x[j]>0) {//render and move AI torps
            FbPoint(ss_torp_map.ai_x[j],ss_torp_map.ai_y[j]);
            FbPoint(ss_torp_map.ai_x[j]-1,ss_torp_map.ai_y[j]);
            if(ss_torp_map.ai_x[j] == 1) {
                ss_torp_map.ai_x[j] = 0;
                ss_torp_map.ai_y[j] = 0;
            }
            ss_torp_map.ai_x[j]--;
        }
        if(ss_torp_map.player_x[j]>0) {//render and move player torps
            FbPoint(ss_torp_map.player_x[j],ss_torp_map.player_y[j]);
            FbPoint(ss_torp_map.player_x[j]-1,ss_torp_map.player_y[j]);
            if(ss_torp_map.player_x[j] == 131) {
                ss_torp_map.player_x[j] = 0;
                ss_torp_map.player_y[j] = 0;
            }
            else {
                ss_torp_map.player_x[j]++;
            }
        }
    }
}

unsigned char ss_detect_hits(struct _ss_player * ship) {
    //if hit detected reduce health and check ai kill and switch mode from battle
    unsigned char j,k;
    k=0;
    for(j=0;j<MAX_TORP;j++) {
        if(ship->team == SS_PLAYER) {
            if(check_box_collision(ship->x, ship->y, 16, 5, ss_torp_map.ai_x[j], 
                                   ss_torp_map.ai_y[j], 2, 1)) {
                k++;
                ss_torp_map.ai_x[j]=0;
                ss_torp_map.ai_y[j]=0;
                if(ship->shields>0) {
                    ship->shields-=10;
                }
                else {
                    ship->hull-=10;
                }
            }
        }
        else {
            if(check_box_collision(ship->x, ship->y, 16, 5, ss_torp_map.player_x[j], 
                                   ss_torp_map.player_y[j], 2, 1)) {
                k++;
                ss_torp_map.player_x[j]=0;
                ss_torp_map.player_y[j]=0;
                if(ship->shields>0) {
                    ship->shields-=10;
                }
                else {
                    ship->hull-=10;
                }
            }
        }
    }
    return k;
}

void ss_draw_sprite(struct _ss_player * ship, unsigned short * sprite) {
    unsigned short mask = 0b1000000000000000;
    unsigned char j,k;
    for(j=0;j<5;j++) {
        for(k=0;k<16;k++) {
            if(sprite[j]&mask) {
                FbPoint(k+ship->x,j+ship->y);
            }
            mask = mask >> 1;
        }
        mask = 0b1000000000000000;
    }
}

void ss_draw_hit(struct _ss_player * ship) {
    unsigned short * sprite;
    if(ship->shields>0) {
        FbColor(BLUE);
        sprite = &shields;
    }
    else {
        FbColor(RED);
        sprite = &explosion;
    }
    ss_draw_sprite(ship,sprite);
}

void ss_draw_player(struct _ss_player * ship_to_draw) {
    if(ship_to_draw->team == SS_AI) {
        FbColor(GREEN);
    }
    else {
        unsigned char x_cord = 132;
        if((ship_to_draw->y+2)>ss_ai.y && (ship_to_draw->y+2)<(ss_ai.y+5)) {
            x_cord = ss_ai.x;
            if(ship_to_draw->phasers > 0 && ss_torp_map.player_phaser_on) {
                if(ss_ai.shields>=0){
                    ss_draw_hit(&ss_ai);
                    ss_ai.shields-=5;
                }
                else {
                    ss_draw_hit(&ss_ai);
                    ss_ai.hull-=5;
                }
            }
        }
        FbColor(YELLOW);
        if(ship_to_draw->phasers > 0 && ss_torp_map.player_phaser_on) {
            FbLine(ship_to_draw->x+16,ship_to_draw->y+2,x_cord,ship_to_draw->y+2);
        }
    }
    ss_draw_sprite(ship_to_draw,&ship_to_draw->sprite);
}
