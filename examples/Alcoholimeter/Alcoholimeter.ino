/*
  MQUnifiedsensor Library - reading an MQ3

  Demonstrates the use a MQ3 sensor.
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
#define type "MQ-3" //MQ3
#define ADC_Bit_Resolution 10 // For arduino UNO/MEGA/NANO
//#define calibration_button 13 //Pin to calibrate your sensor

double alcoholPPM = 0;
//Declare Sensor
MQUnifiedsensor MQ3(placa, Voltage_Resolution, ADC_Bit_Resolution, pin, type);

void setup() {
  //Init the serial port communication - to debug the library
  Serial.begin(9600); //Init serial port

  //Set math model to calculate the PPM concentration and the value of constants
  MQ3.setRegressionMethod(1); //_PPM =  a*ratio^b
  MQ3.setA(0.3934); MQ3.setB(-1.504); // Configurate the ecuation values to get Alcohol concentration
  /*
    Exponential regression:
  Gas    | a      | b
  LPG    | 44771  | -3.245
  CH4    | 2*10^31| 19.01
  CO     | 521853 | -3.821
  Alcohol| 0.3934 | -1.504
  Benzene| 4.8387 | -2.68
  Hexane | 7585.3 | -2.849
  */

  // Calibration setup
  MQ3.setR0(3.86018237);

  /* 
    //If the RL value is different from 10K please assign your RL value with the following method:
    MQ3.setRL(10);
  */

  /*****************************  MQ Init ********************************************/ 
  //Remarks: Configure the pin of arduino as input.
  /************************************************************************************/ 
  MQ3.init(); 
}

void loop() {
  MQ3.update(); // Update data, the arduino will be read the voltage on the analog pin
  alcoholPPM = MQ3.readSensor(); // Sensor will read PPM concentration using the model and a and b values setted before or in the setup
  Serial.print("Alcohol now (PPM): ");
  Serial.println(alcoholPPM);
  delay(500); //Sampling frequency
}
