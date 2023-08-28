/* -----------------------------------------------------------------------------
 * Copyright (C) 2023 Jaime M. Villegas I. <jmvillegasi96@gmail.com>
 * -----------------------------------------------------------------------------
 * Filename      : RTOSpreemption.ino
 * Description   : Task preemption example using FreeRTOS for ESP32 (DOIT ESP32 
 *                 DEVKIT V1)
 * Version       : 01.00
 * Last modified : August, 2023
 * -----------------------------------------------------------------------------
 */

//----------------------------------------------------------------------------//
//                                  Macros                                    //
//----------------------------------------------------------------------------//

// Boolean values
#define TRUE  1
#define FALSE 0

// On-board LED digital pin
#define LED_BUILTIN 2

// Serial port baudrate
#define BAUD_RATE 300

// Time delays (in ms)
#define PORT_SETUP_DELAY      1000
#define PORT_PRINT_MSG_DELAY  1000
#define PORT_PRINT_CHAR_DELAY 100
#define TASK_BLOCKING_DELAY   2000

// Task suspend count
#define TASK_BLOCK_COUNT 5

//----------------------------------------------------------------------------//
//                            General definitions                             //
//----------------------------------------------------------------------------//

// Constriction to use only one CPU core
#if CONFIG_FREERTOS_UNICORE
    static const BaseType_t app_cpu = 0;
#else
    static const BaseType_t app_cpu = 1;
#endif

// Test string
const char sTestString[] = "This is a FreeRTOS task preemption test.";

// Task handlers
static TaskHandle_t pTask1 = NULL;
static TaskHandle_t pTask2 = NULL;

//----------------------------------------------------------------------------//
//                              Local functions                               //
//----------------------------------------------------------------------------//

/**
@brief  Prints test message on serial monitor with low priority
@param  pParameters: Task parameters
@retval None
*/
void task_println(void* pParameters)
{
    int iMsgLen = strlen(sTestString);     // Message length

    // Prints test message character by character
    while(TRUE)
    {
        Serial.println();

        for(int i = 0; i < iMsgLen; i++)
        {
            Serial.print(sTestString[i]);
        }

        Serial.println();

        vTaskDelay(PORT_PRINT_MSG_DELAY/portTICK_PERIOD_MS);

        /*
        OBSERVATIONS:

        - Test message should be printed character by character in this example.
          Otherwise, task 2 preemption would not be seen.
        */
    }
}

/**
@brief  Prints test character on serial monitor with high priority
@param  pParameters: Task parameters
@retval None
*/
void task_printchar(void* pParameters)
{
    // Prints character
    while(TRUE)
    {
        Serial.println();
        Serial.print("*");
        vTaskDelay(PORT_PRINT_CHAR_DELAY/portTICK_PERIOD_MS);
    }
}

//----------------------------------------------------------------------------//
//                                Sketch setup                                //
//----------------------------------------------------------------------------//

/**
@brief  Sketch Setup. Sets serial monitor and creates tasks
@note   FreeRTOS routines specific for ESP32 board are used in this example
*/
void setup() {

    // Configures serial port
    Serial.begin(BAUD_RATE);
    vTaskDelay(PORT_SETUP_DELAY/portTICK_PERIOD_MS);

    // Prints header message
    Serial.println();
    Serial.println("*----------- FreeRTOS preemption test -----------*");

    // Prints main task info
    Serial.print("setup() and loop() task running on CPU core number ");
    Serial.print(xPortGetCoreID());
    Serial.print(" with priority ");
    Serial.println(uxTaskPriorityGet(NULL));

    // Create message printing task
    xTaskCreatePinnedToCore(
        task_println,         // Task core routine to be called
        "Test message",       // Task label
        1024,                 // Stack size in bytes (in words for a general FreeRTOS distribution)
        NULL,                 // Parameter to task function
        1,                    // Task priority (0 - configMAX_PRIORITIES - 1)
        &pTask1,              // Task handler 
        app_cpu               // CPU core selector (task to be run in one core for this example). Not available on general FreeRTOS distro
    );

    // Create character printing task
    xTaskCreatePinnedToCore(
        task_printchar,       // Task core routine to be called
        "Test character",     // Task label
        1024,                 // Stack size in bytes (in words for a general FreeRTOS distribution)
        NULL,                 // Parameter to task function
        2,                    // Task priority (0 - configMAX_PRIORITIES - 1)
        &pTask2,              // Task handler 
        app_cpu               // CPU core selector (task to be run in one core for this example). Not available on general FreeRTOS distro
    );

    /*
    OBSERVATIONS:

    - Here, task handlers are used to access task pointers and suspend or delete them.
    */
}


//----------------------------------------------------------------------------//
//                                 Main loop                                  //
//----------------------------------------------------------------------------//

/**
@brief  Main program
@note   Tasks created during setup
*/
void loop() 
{
    // Suspends the higher priority task
    for(int i = 0; i < TASK_BLOCK_COUNT; i++)
    {
        vTaskSuspend(pTask2);
        vTaskDelay(TASK_BLOCKING_DELAY/portTICK_PERIOD_MS);
        vTaskResume(pTask2);
        vTaskDelay(TASK_BLOCKING_DELAY/portTICK_PERIOD_MS);
    }

    // Deletes the lower priority task
    if(pTask1 != NULL)
    {
        vTaskDelete(pTask1);
        pTask1 = NULL;
    }
}
