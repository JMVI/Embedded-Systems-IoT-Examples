/* -----------------------------------------------------------------------------
 * Copyright (C) 2023 Jaime M. Villegas I. <jmvillegasi96@gmail.com>
 * -----------------------------------------------------------------------------
 * Filename      : BlinkyRTOS.ino
 * Description   : Blinky example using FreeRTOS for ESP32 (DOIT ESP32 DEVKIT V1)
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

//----------------------------------------------------------------------------//
//                            General definitions                             //
//----------------------------------------------------------------------------//

// Constriction to use only one CPU core
#if CONFIG_FREERTOS_UNICORE
    static const BaseType_t app_cpu = 0;
#else
    static const BaseType_t app_cpu = 1;
#endif

//----------------------------------------------------------------------------//
//                              Local functions                               //
//----------------------------------------------------------------------------//

/**
@brief  LED toggle function. Blinks LED
@param  pParameters: Task parameters
@retval None
*/
void blinky_toggle_led(void* pParameters)
{
    // Blinks LED every 1 s
    while(TRUE)
    {
        digitalWrite(LED_BUILTIN, HIGH);
        vTaskDelay(1000/portTICK_PERIOD_MS);
        digitalWrite(LED_BUILTIN, LOW);
        vTaskDelay(1000/portTICK_PERIOD_MS);

        /*
        OBSERVATIONS:

        - vTaskDelay is the FreeRTOS in-built delay function based on a tick timer. 
          It tells the scheduler to run other tasks until the specified timer is up.
          This function expects the number of ticks to delay.
        - A tick timer is one of the MCU hardware timers allocated to interrupt the 
          processor at a specific interval.
        - By default, FreeRTOS sets the tick period to 1 ms.
        */
    }
}

//----------------------------------------------------------------------------//
//                                Sketch setup                                //
//----------------------------------------------------------------------------//

/**
@brief  Sketch Setup. Sets the LED digital pin as output and creates blink task
@note   FreeRTOS routines specific for ESP32 board are used in this example
*/
void setup() {

    // Initialize digital pin LED_BUILTIN as an output.
    pinMode(LED_BUILTIN, OUTPUT);

    // Create Blink task
    xTaskCreatePinnedToCore(
        blinky_toggle_led,   // Task core routine to be called
        "Toggle LED",        // Task label
        1024,                // Stack size in bytes (in words for a general FreeRTOS distribution)
        NULL,                // Parameter to task function
        1,                   // Task priority (0 - configMAX_PRIORITIES - 1)
        NULL,                // Task 
        app_cpu              // CPU core selector (task to be run in one core for this example). Not available on general FreeRTOS distro
    );

    /*
    OBSERVATIONS:

    - xTaskCreatePinnedToCore indicates that the indicated task will be executed 
      by one of the CPU cores. In a general FreeRTOS distribution, xTaskCreate() 
      is used.
    - In Arduino sketch structure, setup() and loop() functions exist inside 
      their own task, separated from the main entry point
    - vTaskStartScheduler must be called inside main() function for a regular 
      FreeRTOS project to begin task execution.
    - Scheduler is spawned as soon as xTaskCreatePinnedToCore() function is called.
    */
}


//----------------------------------------------------------------------------//
//                                 Main loop                                  //
//----------------------------------------------------------------------------//

/**
@brief  Main program
@note   Blink task created during setup
*/
void loop() 
{
    // Void loop
    while(TRUE);
}
