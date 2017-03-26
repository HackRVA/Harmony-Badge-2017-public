#include "app.h"
#include "queue.h"

// badge
#include "colors.h"
#include "fb.h"
#include "rgb_led.h"
#include "task.h"

void hello_world_task(void* p_arg)
{
    //static unsigned char call_count = 0;
    const TickType_t xDelay = 1000 / portTICK_PERIOD_MS;
    FbTransparentIndex(0);
    FbColor(GREEN);
    BaseType_t notify_ret;
    TaskHandle_t xHandle = xTaskGetHandle("APP Tasks");
    if(xHandle == NULL)
        led(1, 0, 0);
    
    for(;;)
    {
        FbMove(16, 16);
        FbWriteLine("hello badge");
        FbSwapBuffers();
        vTaskDelay(xDelay);
        
        FbMove(16, 36);
        FbWriteLine("goodbye badge");
        //FbCharacter(call_count + '0');
        FbSwapBuffers();        
        vTaskDelay(xDelay);
//        call_count++;
//
//        notify_ret = xTaskNotify(xHandle,
//                                 1u,
//                                 eSetBits);
//        vTaskSuspend(NULL);
    }
    
    vTaskDelete( NULL );
}