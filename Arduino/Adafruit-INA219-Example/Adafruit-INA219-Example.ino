/*******************************************************************************
 * Author: Jaime M. Villegas I. (2025)
 *******************************************************************************
 * Sketch name   : Adafruit-INA219-Example.ino
 * Description   : Adafruit INA219 setup and application example.
 * Version       : 01.00
 * Last modified : January, 2025
 *******************************************************************************
 */

//****************************************************************************//
//                                Header files                                //
//****************************************************************************//

#include <Adafruit_INA219.h>

//****************************************************************************//
//                                   Macro                                    //
//****************************************************************************//

// Initialization delay
#define DELAY_INIT       10

// Polling period
#define DELAY_MAIN_LOOP  5000

// Serial port baudrate
#define SERIAL_BAUDRATE  115200

//****************************************************************************//
//                            General definitions                             //
//****************************************************************************//

// INA219 class handler
Adafruit_INA219 INA219;

//****************************************************************************//
//                               Sketch setup                                 //
//****************************************************************************//

/**
 * @brief  Sketch setup. Initializes serial port and Adafruit INA219 module.
 * @param  None
 * @retval None
 */
void setup(void) 
{
    // Initializes serial port
    Serial.begin(SERIAL_BAUDRATE);

    while(!Serial) 
    {
        delay(1);
    }
  
    Serial.println("START");
    Serial.println("Initializing INA219...");
  
  // Initializes the INA219.
  /** 
   * @note - begin() method initializes I2C serial communication.
   *       - SDA and SCL wires correspond respectively to A4 and A5 pins on the Arduino UNO board.
   */
  if(!INA219.begin()) 
  {
      Serial.println("INA219 initialization failed");
      while (1) {}
  }

  // Sets calibration mode
  /** 
   * @note - The calculation for calibration value is performed by Adafruit library. 0.1 Ohm shunt resistor is assumed.
   *
   *       1. setCalibration_32V_2A()
   *          - Max. shunt voltage    = 320 mV
   *          - Max. expected current = 2.0 A
   *          - Current resolution    = 0.488 mA
   * 
   *       2. setCalibration_32V_1A()
   *          - Max. shunt voltage    = 320 mV
   *          - Max. expected current = 1.0 A
   *          - Current resolution    = 0.244 mA
   * 
   *       3. setCalibration_16V_400mA()
   *          - Max. shunt voltage    = 40 mV
   *          - Max. expected current = 400 mA
   *          - Current resolution    = 0.098 mA
   * 
   */
  INA219.setCalibration_16V_400mA();

  Serial.println("----------------------");
}

//****************************************************************************//
//                                Main loop                                   //
//****************************************************************************//

/**
 * @brief  Main loop. Polls voltage and current from INA219 every time period
           defined by DELAY_MAIN_LOOP.
 * @param  None
 * @retval None
 */
void loop(void) 
{
    // Shunt resistor voltage drop (in mV)
    float rShuntVoltage = 0;

    // Bus voltage
    float busVoltage = 0;

    // Load current (in mA)
    float currentLoad = 0;

    // Load voltage
    float loadVoltage = 0;

    // Load power (in mW)
    float loadPower = 0;

    // Gets voltage, current and power measurements
    busVoltage = INA219.getBusVoltage_V();
    rShuntVoltage = INA219.getShuntVoltage_mV();
    currentLoad = INA219.getCurrent_mA();
    loadPower = INA219.getPower_mW();

    /** 
     * @note - The bus voltage is the total voltage between power and ground.
     *       - The shunt voltage is the voltage drop across the shunt resistor (in series with connected load).
     */
    loadVoltage = busVoltage + (rShuntVoltage / 1000);
    
    Serial.print("Vbus   = "); 
    Serial.print(busVoltage); 
    Serial.println(" V");
    
    Serial.print("Vshunt = "); 
    Serial.print(rShuntVoltage); 
    Serial.println(" mV");

    Serial.print("Iload  = "); 
    Serial.print(currentLoad); 
    Serial.println(" mA");

    Serial.print("Pload  = "); 
    Serial.print(loadPower); 
    Serial.println(" mW");

    Serial.print("Vload  = "); 
    Serial.print(loadVoltage); 
    Serial.println(" V");

    Serial.println("----------------------");

    delay(DELAY_MAIN_LOOP);
}
