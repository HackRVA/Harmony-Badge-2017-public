/***************************************************************************
 * flash_test.c
 *
 *************************************************************************** */

#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "FreeRTOS.h"
#include "task.h"
#include "badge_menu.h"
#include "flash_test.h"
#include "buttons.h"
#include "fb.h"
#include "flash.h"
#include "rgb_led.h"
#include "driver/flash/drv_flash.h"
#include "app.h"


#define FLASH_TEST_APP_ID 7

void show_str_num(const char const *str, int32_t num, uint8_t line) {
    char buff[9];
    uint8_t x, y;
    x = 5;
    y = 5 + 12 * (line - 1);
    ltoa(buff, num, 16);
    FbMove(x, y);
    FbWriteLine(str);
    x += 10 * strlen(str);
    FbMove(x, y);
    FbWriteLine(buff);
}

void ushow_str_num(const char const *str, uint32_t num, uint8_t line) {
    char buff[9];
    uint8_t x, y;
    x = 5;
    y = 5 + 12 * (line - 1);
    ultoa(buff, num, 16);
    FbMove(x, y);
    FbWriteLine(str);
    x += 10 * strlen(str);
    FbMove(x, y);
    FbWriteLine(buff);
}

extern long initial_read;

/**
 * Standard task function to run simple tests of the flash routines.
 *
 * @param p_arg not used.
 */
void flash_test_task(void *p_arg) {
    int bytesRead = 0;
    int errcode = 0;
    uint32_t num = 0x89ABCDEF;

    while (!BUTTON_PRESSED_AND_CONSUME) {
        FbClear();
        FbMove(10, 5);
        FbWriteLine("Hello");
        FbSwapBuffers();
        // led(0, 1, 0);
        vTaskDelay(2000 / portTICK_PERIOD_MS);

        // verifies flash address and that it's aligned
        ushow_str_num("addr", G_flashAddr, 1);

        // print 1st four bytes stored in flash
        ushow_str_num("data", *((unsigned long *) G_flashAddr), 2);

        bytesRead = NVread(FLASH_TEST_APP_ID, 0xAB, &num, sizeof (num));
        show_str_num("bytes", bytesRead, 3);
        ushow_str_num("value", num, 4);

        // write num as data, with key 0xAB
        errcode = NVwrite(FLASH_TEST_APP_ID, 0xAB, (unsigned char *) &num, sizeof (num));
        show_str_num("wrote", errcode, 5);

        num++;
        FbSwapBuffers();
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
    returnToMenus();
    // above should have killed this task, just in case...
    vTaskDelete(NULL);
}


/* *****************************************************************************
 End of File
 */
