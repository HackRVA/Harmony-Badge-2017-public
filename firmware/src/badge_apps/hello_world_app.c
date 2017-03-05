#include "app.h"
#include "queue.h"

// badge
#include "colors.h"
#include "fb.h"
#include "rgb_led.h"

void doText()
{
    FbTransparentIndex(0);
    //FbBackgroundColor(WHITE);
    //FbClear();

    FbMove(16, 16);
    FbColor(GREEN);
    FbWriteLine("hello badge");
    FbSwapBuffers();
}

void hello_world_task(void* p_arg)
{
    for(;;)
    {
        doText();
        red(1);
        green(1);
    }
}