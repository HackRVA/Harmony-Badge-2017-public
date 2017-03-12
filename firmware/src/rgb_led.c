#include "app.h"

// TODO:  why are these ints? also pwm
void red(int val)
{
        SYS_PORTS_PinWrite( PORTS_ID_0, PORT_CHANNEL_C, PORTS_BIT_POS_0, val );
}

void green(int val)
{
        SYS_PORTS_PinWrite( PORTS_ID_0, PORT_CHANNEL_B, PORTS_BIT_POS_3, val );
}

void blue(int val)
{
        SYS_PORTS_PinWrite( PORTS_ID_0, PORT_CHANNEL_C, PORTS_BIT_POS_1, val ); 
}

void led(int r, int g, int b){
    red(r);
    green(g);
    blue(b);
}
