
#include "flash.h"
#include "badge_menu.h"
#include "buttons.h"

#include "ir.h"
#include "fb.h"
#include "utils.h"
#include "app.h"

#include "colors.h"

extern unsigned int G_currentstamp;
void silence_task(void *p_arg){
    unsigned char num_pinged = 0;

    union IRpacket_u pkt;
    pkt.p.command = IR_WRITE;
    pkt.p.address = IR_APP7;
    pkt.p.badgeId = 0;
    pkt.p.data = ADMIN_SILENCE;
    IRqueueSend(pkt);

    returnToMenus();
}

void note_crazy_task(void *p_arg){
    unsigned short cnter = 0;
    unsigned short freq = 0;

    FbMove(20, 50);
    FbColor(YELLOW);
    FbWriteLine("WHAT A FUCKTARD");
    FbSwapBuffers();
    
    union IRpacket_u pkt;
    for(;;){
        
        pkt.p.command = IR_WRITE;
        pkt.p.address = IR_LIVEAUDIO;
        pkt.p.badgeId = 0;
        freq = 40 + (quick_rand(66) %200);
        pkt.p.data = (((2048 >> 8) & 0xF) << 12) | (freq & 0x0FFF) ;
        IRqueueSend(pkt);
        cnter = 0;
        vTaskDelay(200 / portTICK_PERIOD_MS);


        if(BUTTON_PRESSED_AND_CONSUME){
            cnter = 0;
            returnToMenus();
        }
    }
}