/*
  MQUnifiedsensor Library - reading an MQ303

  Demonstrates the use a MQ303 sensor.
  Library originally added 01 may 2019
  by Miguel A Califa, Yersson Carrillo, Ghiordy Contreras, Mario Rodriguez
 
  Added example
  modified 23 May 2019
  by Miguel Califa 

  Updated library usage
  modified 26 March 2020
  by Miguel Califa 

 This example code is in the public domain.

*/

//Include the library
#include <MQUnifiedsensor.h>

//Definitions
#define placa "Arduino UNO"
#define Voltage_Resolution 5
#define pin A0 //Analog input 0 of your arduino
#define type "MQ-303" //MQ303
#define ADC_Bit_Resolution 10 // For arduino UNO/MEGA/NANO
//#define calibration_button 13 //Pin to calibrate your sensor

//Declare Sensor
MQUnifiedsensor MQ303(placa, Voltage_Resolution, ADC_Bit_Resolution, pin, type);

void setup() {
  //Init the serial port communication - to debug the library
  Serial.begin(9600); //Init serial port

  //Set math model to calculate the PPM concentration and the value of constants
  MQ303.setRegressionMethod(1); //_PPM =  a*ratio^b
  MQ303.setA(6.2144); MQ303.setB(-2.894); // Configurate the ecuation values to get Isobutano concentration

  /*
    Exponential regression:
  GAS        | a      | b
  Iso_butano | 6.2144 | -2.894
  Hydrogen   | 4.1487 | -2.716
  Ethanol    | 3.4916 | -2.432
  */

  // Calibration setup
  MQ303.setR0(10);

  /* 
    //If the RL value is different from 10K please assign your RL value with the following method:
    MQ303.setRL(10);
  */

  /*****************************  MQ Init ********************************************/ 
  //Remarks: Configure the pin of arduino as input.
  /************************************************************************************/ 
  MQ303.init(); 
  /*****************************  MQ Init ********************************************/ 
  //Input: setup flag, if this function are on setup will print the headers (Optional - Default value: False)
  //Output: print on serial port the information about sensor and sensor readings
  //Remarks: Configure the pin of arduino as input.
  /************************************************************************************/ 
  MQ303.serialDebug(true);
}

void loop() {
  MQ303.update(); // Update data, the arduino will be read the voltage on the analog pin
  MQ303.readSensor(); // Sensor will read PPM concentration using the model and a and b values setted before or in the setup
  MQ303.serialDebug(); // Will print the table on the serial port
  delay(500); //Sampling frequency
}