/*******************************************************************************
  MPLAB Harmony Application Source File
  
  Company:
    Microchip Technology Inc.
  
  File Name:
    app.c

  Summary:
    This file contains the source code for the MPLAB Harmony application.

  Description:
    This file contains the source code for the MPLAB Harmony application.  It 
    implements the logic of the application's state machine and it may call 
    API routines of other MPLAB Harmony modules in the system, such as drivers,
    system services, and middleware.  However, it does not call any of the
    system interfaces (such as the "Initialize" and "Tasks" functions) of any of
    the modules in the system or make any assumptions about when those functions
    are called.  That is the responsibility of the configuration-specific system
    files.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013-2014 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
 *******************************************************************************/
// DOM-IGNORE-END


// *****************************************************************************
// *****************************************************************************
// Section: Included Files 
// *****************************************************************************
// *****************************************************************************

#include "app.h"
#include "queue.h"

// badge
#include "colors.h"
#include "fb.h"
#include "rgb_led.h"
#include "LCDcolor.h"
#include "badge_apps.h"
#include "badge_menu.h"
#include "touch_ctmu.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP_Initialize function.
    
    Application strings and buffers are be defined outside this structure.
*/
QueueHandle_t USBDeviceTask_EventQueue_Handle;

APP_DATA appData;

/* Read Data Buffer */
uint8_t APP_MAKE_BUFFER_DMA_READY com1ReadBuffer[APP_READ_BUFFER_SIZE] ;


/* Write Data Buffer. Size is same as read buffer size */
uint8_t APP_MAKE_BUFFER_DMA_READY WriteBuffer[APP_WRITE_BUFFER_SIZE] ;

/* Read Data Buffer */
uint8_t APP_MAKE_BUFFER_DMA_READY com2ReadBuffer[APP_READ_BUFFER_SIZE];

SemaphoreHandle_t cdc_write_buffer_lock;
static TaskHandle_t cdc_write_requester = NULL;

void USBDevice_Task(void* p_arg);
// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

/*************************************************
 * Application Device Layer Event Handler
 *************************************************/

void APP_USBDeviceEventHandler(USB_DEVICE_EVENT event, void * pData,
                               uintptr_t context)
{
    uint8_t configurationValue;
    uint32_t USB_Event = 0;
    portBASE_TYPE xHigherPriorityTaskWoken1 = pdFALSE;

    switch( event )
    {
        case USB_DEVICE_EVENT_POWER_REMOVED:
            /* Attach the device */
            USB_DEVICE_Detach (appData.deviceHandle);
            break;
        case USB_DEVICE_EVENT_RESET:
        case USB_DEVICE_EVENT_DECONFIGURED:

            /* Device was either de-configured or reset */

            /* Update LED indication */
            led(0, 0, 1);
//            BSP_LEDOn ( APP_USB_LED_1);
//            BSP_LEDOn ( APP_USB_LED_2);
//            BSP_LEDOff ( APP_USB_LED_3);
            break;

        case USB_DEVICE_EVENT_CONFIGURED:

            /* pData will point to the configuration. Check the configuration */
            configurationValue = ((USB_DEVICE_EVENT_DATA_CONFIGURED *)pData)->configurationValue;
            if(configurationValue == 1)
            {
                /* Register the CDC Device application event handler here.
                 * Note how the appData object pointer is passed as the
                 * user data */
                USB_DEVICE_CDC_EventHandlerSet(0,
                        APP_USBDeviceCDCEventHandler, (uintptr_t)&appData);

                USB_DEVICE_CDC_EventHandlerSet(1,
                        APP_USBDeviceCDCEventHandler, (uintptr_t)&appData);

                /* Update LED indication */
                led(0, 1, 0);
//                BSP_LEDOff ( APP_USB_LED_1 );
//                BSP_LEDOff ( APP_USB_LED_2 );
//                BSP_LEDOn ( APP_USB_LED_3 );

                /*let processing USB Task know USB if configured..*/
                USB_Event = USBDEVICETASK_USBCONFIGURED_EVENT;

                xQueueSendToBackFromISR(USBDeviceTask_EventQueue_Handle, &USB_Event, 
                    &xHigherPriorityTaskWoken1);
                portEND_SWITCHING_ISR( xHigherPriorityTaskWoken1 );
            }

            break;

        case USB_DEVICE_EVENT_SUSPENDED:
            /* Update LED indication */
            led(1, 0, 0);
//            BSP_LEDOff ( APP_USB_LED_1 );
//            BSP_LEDOn ( APP_USB_LED_2 );
//            BSP_LEDOn ( APP_USB_LED_3 );
            break;

        case USB_DEVICE_EVENT_RESUMED:
            break;
        case USB_DEVICE_EVENT_POWER_DETECTED:
            /*let processing USB Task know USB is powered..*/
            USB_Event = USBDEVICETASK_USBPOWERED_EVENT;

            xQueueSendToBackFromISR(USBDeviceTask_EventQueue_Handle, &USB_Event, 
                &xHigherPriorityTaskWoken1);
            portEND_SWITCHING_ISR( xHigherPriorityTaskWoken1 );
            /* Attach the device */
            //USB_DEVICE_Attach (appData.deviceHandle);
            break;
        case USB_DEVICE_EVENT_ERROR:
        default:
            break;
    }
}


/************************************************
 * CDC Function Driver Application Event Handler
 ************************************************/

USB_DEVICE_CDC_EVENT_RESPONSE APP_USBDeviceCDCEventHandler
(
    USB_DEVICE_CDC_INDEX index ,
    USB_DEVICE_CDC_EVENT event ,
    void* pData,
    uintptr_t userData
)
{

    APP_DATA * appDataObject;
    appDataObject = (APP_DATA *)userData;
    USB_CDC_CONTROL_LINE_STATE * controlLineStateData;
    uint16_t * breakData;
    uint32_t  USB_Event = 0;
    portBASE_TYPE xHigherPriorityTaskWoken1 = pdFALSE;

    switch ( event )
    {
        case USB_DEVICE_CDC_EVENT_GET_LINE_CODING:

            /* This means the host wants to know the current line
             * coding. This is a control transfer request. Use the
             * USB_DEVICE_ControlSend() function to send the data to
             * host.  */

            USB_DEVICE_ControlSend(appDataObject->deviceHandle,
                    &appDataObject->appCOMPortObjects[index].getLineCodingData,
                    sizeof(USB_CDC_LINE_CODING));

            break;

        case USB_DEVICE_CDC_EVENT_SET_LINE_CODING:

            /* This means the host wants to set the line coding.
             * This is a control transfer request. Use the
             * USB_DEVICE_ControlReceive() function to receive the
             * data from the host */

            USB_DEVICE_ControlReceive(appDataObject->deviceHandle,
                    &appDataObject->appCOMPortObjects[index].setLineCodingData,
                    sizeof(USB_CDC_LINE_CODING));

            break;

        case USB_DEVICE_CDC_EVENT_SET_CONTROL_LINE_STATE:

            /* This means the host is setting the control line state.
             * Read the control line state. We will accept this request
             * for now. */

            controlLineStateData = (USB_CDC_CONTROL_LINE_STATE *)pData;
            appDataObject->appCOMPortObjects[index].controlLineStateData.dtr
                    = controlLineStateData->dtr;
            appDataObject->appCOMPortObjects[index].controlLineStateData.carrier
                    = controlLineStateData->carrier;

            USB_DEVICE_ControlStatus(appDataObject->deviceHandle, USB_DEVICE_CONTROL_STATUS_OK);

            break;

        case USB_DEVICE_CDC_EVENT_SEND_BREAK:

            /* This means that the host is requesting that a break of the
             * specified duration be sent. Read the break duration */

            breakData = (uint16_t *)pData;
            appDataObject->appCOMPortObjects[index].breakData = *breakData;
            break;

        case USB_DEVICE_CDC_EVENT_READ_COMPLETE:
            if(index == 0)
                USB_Event = USBDEVICETASK_READDONECOM1_EVENT;
            else 
                USB_Event = USBDEVICETASK_READDONECOM2_EVENT;
            
            /*let processing USB Task know USB if configured..*/
            xQueueSendToBackFromISR(USBDeviceTask_EventQueue_Handle, &USB_Event, 
                &xHigherPriorityTaskWoken1);
            portEND_SWITCHING_ISR( xHigherPriorityTaskWoken1 );
            break;

        case USB_DEVICE_CDC_EVENT_CONTROL_TRANSFER_DATA_RECEIVED:

            /* The data stage of the last control transfer is
             * complete. For now we accept all the data */

            USB_DEVICE_ControlStatus(appDataObject->deviceHandle, USB_DEVICE_CONTROL_STATUS_OK);
            break;

        case USB_DEVICE_CDC_EVENT_CONTROL_TRANSFER_DATA_SENT:

            /* This means the GET LINE CODING function data is valid. We don't
             * do much with this data in this demo. */

            break;

        case USB_DEVICE_CDC_EVENT_WRITE_COMPLETE:
            if(index == 0)
                USB_Event = USBDEVICETASK_WRITEDONECOM1_EVENT;
            else 
                USB_Event = USBDEVICETASK_WRITEDONECOM2_EVENT;
            
            /*let processing USB Task know USB if configured..*/
            xQueueSendToBackFromISR(USBDeviceTask_EventQueue_Handle, &USB_Event, 
                &xHigherPriorityTaskWoken1);
            portEND_SWITCHING_ISR( xHigherPriorityTaskWoken1 );
            break;

        default:
            break;
    }
    return USB_DEVICE_CDC_EVENT_RESPONSE_NONE;
}

#define USBDEVICETASK_WRITE_BUFFER_TO_COM1 7
// *****************************************************************************
/* Function:
    void APP_USB_DEVICE_AttachTask(void)

  Summary:
    It is an RTOS task for Attaching and Configuring USB Device to Host.

  Description:
    This function is an RTOS task for attaching the USB Device to Host. Following
 are the actions done by this Task.
 1) Open an instance of Harmony USB Device Framework by periodically calling
    (in every 1 milli Second) USB_DEVICE_Open()function until Harmony USB Device
     framework is successfully opened.
 2) If the USB Device Framework is opened successfully pass an application event
    Handler to the USB framework for receiving USB Device Events.
 3) Attach to the USB Host by calling USB attach function.
 4) Acquire a binary semaphore to wait until USB Host Configures the Device. The
    semaphore is released when a USB_DEVICE_EVENT_CONFIGURED event is received at
    the USB Device event handler.
 5) Resume all CDC read/write tasks.
 6) Suspend USB attach task.

  Returns:
     None
*/
void USBDevice_Task(void* p_arg)
{
    BaseType_t errStatus;
    uint32_t USBDeviceTask_State = USBDEVICETASK_OPENUSB_STATE;
    uint32_t USBDeviceTask_Event = 0;
    USB_DEVICE_CDC_TRANSFER_HANDLE COM1Read_Handle, COM1Write_Handle, 
                                   COM2Read_Handle, COM2Write_Handle;

    COM1Read_Handle = USB_DEVICE_CDC_TRANSFER_HANDLE_INVALID;
    COM1Write_Handle = USB_DEVICE_CDC_TRANSFER_HANDLE_INVALID;
    COM2Read_Handle = USB_DEVICE_CDC_TRANSFER_HANDLE_INVALID;
    COM2Write_Handle = USB_DEVICE_CDC_TRANSFER_HANDLE_INVALID;

    for(;;)
    {
        switch(USBDeviceTask_State)
        {
            case USBDEVICETASK_OPENUSB_STATE:
                appData.deviceHandle = USB_DEVICE_Open( USB_DEVICE_INDEX_0,
                    DRV_IO_INTENT_READWRITE );
                /*do we have access to usb, if not try again*/
                if(appData.deviceHandle != USB_DEVICE_HANDLE_INVALID)
                {
                    //USBDeviceTask_State = USBDEVICETASK_PROCESSUSBEVENTS_STATE;
                    USB_DEVICE_EventHandlerSet(appData.deviceHandle, 
                            APP_USBDeviceEventHandler, 0);
                    USBDeviceTask_State = USBDEVICETASK_ATTACHUSB_STATE;
                    break;
                }
                /*try again in 10msec*/
                USBDeviceTask_State = USBDEVICETASK_OPENUSB_STATE;
                vTaskDelay(10 / portTICK_PERIOD_MS);
                break;
            case USBDEVICETASK_ATTACHUSB_STATE: 
                USB_DEVICE_Attach (appData.deviceHandle);
                USBDeviceTask_State = USBDEVICETASK_PROCESSUSBEVENTS_STATE;
                break;
            case USBDEVICETASK_PROCESSUSBEVENTS_STATE:
                /*once here, usb task becomes event driven, user input will 
                  will generate events. */
                USBDeviceTask_State = USBDEVICETASK_PROCESSUSBEVENTS_STATE;                

                /*wait for an event to occur and process, see event handler*/
                errStatus = xQueueReceive(USBDeviceTask_EventQueue_Handle,
                                &USBDeviceTask_Event,portMAX_DELAY);

                /*make sure event was successfully received*/
                if(errStatus == pdFALSE)
                    break;

                switch(USBDeviceTask_Event)
                {
                    case USBDEVICETASK_USBPOWERED_EVENT:
                        USB_DEVICE_Attach (appData.deviceHandle);
                        break;
                    case USBDEVICETASK_USBCONFIGURED_EVENT:
                        /*USB ready, wait for user input on either com port*/

                        /* Schedule a CDC read on COM1 */
                        USB_DEVICE_CDC_Read(0, &COM1Read_Handle,
                            com1ReadBuffer,APP_READ_BUFFER_SIZE);
                        /* Schedule a CDC read on COM2 */
                        USB_DEVICE_CDC_Read(1,&COM2Read_Handle,
                            com2ReadBuffer,APP_READ_BUFFER_SIZE);
                        break;                    
                    case USBDEVICETASK_READDONECOM1_EVENT:
                        /* Send the received data to COM2 */
//                        USB_DEVICE_CDC_Write(1, &COM2Write_Handle,
//                            &com1ReadBuffer, 1,
//                            USB_DEVICE_CDC_TRANSFER_FLAGS_DATA_COMPLETE);

//                        FbClear();
//                        FbMove(16, 32);
//                        FbColor(GREEN);
//                        FbWriteLine("com2: ");
//                        FbWriteLine(com1ReadBuffer);
//                        FbSwapBuffers();
                      

                        break;
                    case USBDEVICETASK_READDONECOM2_EVENT:
                        /* Send the received data to COM1 */
//                        USB_DEVICE_CDC_Write(0, &COM1Write_Handle,
//                            &com2ReadBuffer, 1,
//                            USB_DEVICE_CDC_TRANSFER_FLAGS_DATA_COMPLETE);

//                        FbClear();
//                        FbMove(16, 48);
//                        FbColor(RED);
//                        FbWriteLine("com1: ");
//                        FbWriteLine(com2ReadBuffer);
//                        FbSwapBuffers();
                      

                         break;
                    case USBDEVICETASK_WRITEDONECOM1_EVENT:
                        /* Schedule a CDC read on COM2 */
//                        USB_DEVICE_CDC_Read(1,&COM2Read_Handle,
//                            com2ReadBuffer,APP_READ_BUFFER_SIZE);
                        
                        // Not sure if things other than badge code
                        // will set off this write event, so double check
                        // that the write requester is set
                        if(cdc_write_requester != NULL)
                            // Let the write requester know that the write is complete
                            xTaskNotify(cdc_write_requester, 2u, eSetValueWithoutOverwrite);
                        break;
                    case USBDEVICETASK_WRITEDONECOM2_EVENT:
                        /* Schedule a CDC read on COM1 */
//                        USB_DEVICE_CDC_Read(0, &COM1Read_Handle,
//                            com1ReadBuffer,APP_READ_BUFFER_SIZE);
                        break;
                    case USBDEVICETASK_WRITE_BUFFER_TO_COM1:                    
                        USB_DEVICE_CDC_Write(0, &COM1Write_Handle, WriteBuffer,
                                            APP_WRITE_BUFFER_SIZE, 
                                            USB_DEVICE_CDC_TRANSFER_FLAGS_DATA_COMPLETE);  
                        
                    
                        break;
                    default:
                        break;
                }
                break;
            default:
                break;
        }
    }
}

// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_Initialize ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Initialize ( void )
{
    cdc_write_buffer_lock = xSemaphoreCreateMutex();
    if(cdc_write_buffer_lock == NULL)
    {
        led(1, 0, 0);
        while(1);
    }    
    
    USBDeviceTask_EventQueue_Handle = xQueueCreate(15, sizeof(uint32_t));
    if(USBDeviceTask_EventQueue_Handle == NULL)
    {
        led(1, 0, 0);
        while(1);
    }
	/* Initialize the application object */
    appData.deviceHandle = USB_DEVICE_HANDLE_INVALID;
    appData.appCOMPortObjects[0].getLineCodingData.dwDTERate = 9600;
    appData.appCOMPortObjects[0].getLineCodingData.bDataBits = 8;
    appData.appCOMPortObjects[0].getLineCodingData.bParityType = 0;
    appData.appCOMPortObjects[0].getLineCodingData.bCharFormat = 0;

    appData.appCOMPortObjects[1].getLineCodingData.dwDTERate = 9600;
    appData.appCOMPortObjects[1].getLineCodingData.bDataBits = 8;
    appData.appCOMPortObjects[1].getLineCodingData.bParityType = 0;
    appData.appCOMPortObjects[1].getLineCodingData.bCharFormat = 0;


    /* badgy badge badge */
    SYS_PORTS_PinDirectionSelect(PORTS_ID_0, SYS_PORTS_DIRECTION_OUTPUT, PORT_CHANNEL_C, PORTS_BIT_POS_0); // red
    SYS_PORTS_PinDirectionSelect(PORTS_ID_0, SYS_PORTS_DIRECTION_OUTPUT, PORT_CHANNEL_B, PORTS_BIT_POS_3); // green
    SYS_PORTS_PinDirectionSelect(PORTS_ID_0, SYS_PORTS_DIRECTION_OUTPUT, PORT_CHANNEL_C, PORTS_BIT_POS_1); // blue

    SYS_PORTS_PinDirectionSelect(PORTS_ID_0, SYS_PORTS_DIRECTION_OUTPUT, PORT_CHANNEL_C, PORTS_BIT_POS_6); // SDA == SDIN == serial data to LCD
    SYS_PORTS_PinDirectionSelect(PORTS_ID_0, SYS_PORTS_DIRECTION_OUTPUT, PORT_CHANNEL_C, PORTS_BIT_POS_7); // A0 == data=1 or command=0
    SYS_PORTS_PinDirectionSelect(PORTS_ID_0, SYS_PORTS_DIRECTION_OUTPUT, PORT_CHANNEL_C, PORTS_BIT_POS_8); // reset == active low
    SYS_PORTS_PinDirectionSelect(PORTS_ID_0, SYS_PORTS_DIRECTION_OUTPUT, PORT_CHANNEL_C, PORTS_BIT_POS_9); // backlight

    SYS_PORTS_PinDirectionSelect(PORTS_ID_0, SYS_PORTS_DIRECTION_OUTPUT, PORT_CHANNEL_B, PORTS_BIT_POS_8); // CS active low out  chip select
    SYS_PORTS_PinDirectionSelect(PORTS_ID_0, SYS_PORTS_DIRECTION_OUTPUT, PORT_CHANNEL_B, PORTS_BIT_POS_9); // SCLK out  serial clock

    SYS_PORTS_PinDirectionSelect(PORTS_ID_0, SYS_PORTS_DIRECTION_OUTPUT, PORT_CHANNEL_A, PORTS_BIT_POS_9); // piezo 

    SYS_PORTS_PinDirectionSelect(PORTS_ID_0, SYS_PORTS_DIRECTION_INPUT, PORT_CHANNEL_C, PORTS_BIT_POS_3);  // button 
    CNPUCbits.CNPUC3 = 1; // pullup == on

    //PLIB_PORTS_ChangeNoticePullUpPerPortEnable(PORTS_ID_0, PORT_CHANNEL_C, PORTS_BIT_POS_3);
            
    led(0, 0, 1);
    LCDInitPins();
    led(1, 0, 0);
    LCDReset();
    led(0, 1, 0);
    LCDBars();
    LCDBacklight(1);

    FbInit();
}

void button_task(void* p_arg)
{
    const TickType_t xDelay = 20 / portTICK_PERIOD_MS;
    //TickType_t xLastWakeTime;
    //const TickType_t xFrequency = 30 / portTICK_PERIOD_MS;
    // reset globals
    G_button = 0;
    G_buttonCnt = 0;
    G_buttonDebounce = 0;
   
    for(;;){
        //xLastWakeTime = xTaskGetTickCount();
        if (!PORTCbits.RC3) {
            if (G_buttonCnt > 3)
            {
                G_button = 1;
            }

            if (G_buttonCnt == 255) {
               //if (menu_escape_cb != NULL) menu_escape_cb();
               G_buttonCnt=0;
            }
            else
               G_buttonCnt++;
        }
        else {
            G_buttonCnt = 0;
            G_button = 0;
        }
        vTaskDelay(xDelay);
        //vTaskDelayUntil(&xLastWakeTime, xDelay);
    }
}


void print_to_com1(unsigned char buffer[APP_WRITE_BUFFER_SIZE]){
    portBASE_TYPE xHigherPriorityTaskWoken1 = pdFALSE;
    uint32_t ulNotifiedValue = 0;
    uint32_t USB_Event = USBDEVICETASK_WRITE_BUFFER_TO_COM1;
    unsigned char i=0;
    

    //----------
    // Write Buffer LOCKED
    xSemaphoreTake(cdc_write_buffer_lock, portMAX_DELAY); // MaxDelay to block indefinitely
    
    // Track this task requesting the write so the USB task
    // can notify it when done
    cdc_write_requester = xTaskGetCurrentTaskHandle();
    
    // copy the data to the Write Buffer
    for(i=0; i < APP_WRITE_BUFFER_SIZE & buffer[i] != 0; i++){
        WriteBuffer[i] = buffer[i];
    }
    
    // Tell the USB task that we want the write buffer printed
    // 'FromISR' should make this ISR-safe
    xQueueSendToBackFromISR(USBDeviceTask_EventQueue_Handle, &USB_Event, 
                            &xHigherPriorityTaskWoken1);
    // posting to the queue may have unblocked a high priority task (USB)
    // so yield some time to that task (only matters if in interrupt...?)
    portEND_SWITCHING_ISR( xHigherPriorityTaskWoken1 );
    
    // The USB task will send a notification to this task once the write
    // buffer has been written (sets the 2nd bit in the 'mailbox')
    while (xTaskNotifyWait(0, // Don't clear anything on entry
                          2u, // clear bit 2 in the mailbox when done
                          &ulNotifiedValue, // Mailbox value copied here before clearing
                          50 / portTICK_PERIOD_MS) // Timeout for block is 50ms
                          == pdFALSE);
    // If bit 2 wasn't set for some reason....
    if(ulNotifiedValue != 2)
        led(1, 0, 0);
    
    // Zero out the buffer
    for(i = 0; i < APP_WRITE_BUFFER_SIZE; i++)
        WriteBuffer[i] = 0;
    
    // Reset the requester
    cdc_write_requester = NULL;
    
    xSemaphoreGive(cdc_write_buffer_lock);
    // Write Buffer UNLOCKED
    //---------------
}

void print_task_high_water_mark_to_CDC(TaskHandle_t xHandle)
{
    int8_t to_print[8] = {0};
    UBaseType_t uxHighWaterMark;

    uxHighWaterMark = uxTaskGetStackHighWaterMark( xHandle );
    
    to_print[0] = '0' + (unsigned char)((uxHighWaterMark/1000) % 10);
    to_print[1] = '0' + (unsigned char)((uxHighWaterMark/100) % 10);
    to_print[2] = '0' + (unsigned char)((uxHighWaterMark/10)  % 10);
    to_print[3] = '0' + (unsigned char)((uxHighWaterMark)     % 10);    
    
    to_print[4] = '.';
    to_print[5] = '\n';
    to_print[6] = '\r';
    to_print[7] = 0;    
    print_to_com1(to_print);

    // Red LEDs everywhere
    if(uxHighWaterMark == 0)
        led(1, 0, 0);
}

void print_high_water_marks(){
    TaskHandle_t xHandle = NULL;
    
    // The 'main' MHC provided task, stack size configured in MHC
    xHandle = xTaskGetHandle("APP Tasks");
    print_to_com1("main:\0");
    print_task_high_water_mark_to_CDC(xHandle);
    
    // The running app, started from app tasks
    xHandle = xTaskGetHandle("exec_app");
    print_to_com1("app:\0");
    print_task_high_water_mark_to_CDC(xHandle);
    
    // Button polling task
    xHandle = xTaskGetHandle("button_task");
    print_to_com1("btn:\0");
    print_task_high_water_mark_to_CDC(xHandle);            

    // USB task
    xHandle = xTaskGetHandle("USB_AttachTask");
    print_to_com1("usb:\0");
    print_task_high_water_mark_to_CDC(xHandle);              

    print_to_com1("----\n\r\0");
}


void test_task(void* p_arg)
{
    TaskHandle_t xHandle = NULL;
    BaseType_t xReturned;
    
    // Swap out 'hello_world_task' for task function needing testing
    xReturned = xTaskCreate((TaskFunction_t) hello_world_task,
            "exec_app",
            256u,
            NULL,
            1u,
            &xHandle);
    
    for(;;)
    {
        // show high water marks on CDC com1
        if(G_button){
            print_high_water_marks();
            vTaskDelay(100 / portTICK_PERIOD_MS);
        }

        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
    
    vTaskDelete( NULL );
}


/******************************************************************************
  Function:
    void APP_Tasks ( void )

  Remarks:
    See prototype in app.h.
 */
void APP_Tasks ( void )
{
    BaseType_t errStatus;

    errStatus = xTaskCreate((TaskFunction_t) USBDevice_Task,
            "USB_AttachTask",
            USBDEVICETASK_SIZE,
            NULL,
            USBDEVICETASK_PRIO,
            NULL);

    if(errStatus != pdTRUE){
        red(1);
        while(1);
    }

    errStatus = xTaskCreate((TaskFunction_t) button_task, 
            "button_task",
            128u,
            NULL,
            1u,
            NULL);
    
    if(errStatus != pdTRUE){
        red(1);
        while(1);
    }
    
    test_task(NULL);
}
 

/*******************************************************************************
 End of File
 */
