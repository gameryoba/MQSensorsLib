/*
  MQUnifiedsensor Library - reading an MQ4

  Demonstrates the use a MQ4 sensor.
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
#define type "MQ-4" //MQ4
#define ADC_Bit_Resolution 10 // For arduino UNO/MEGA/NANO
#define RatioMQ4CleanAir 4.4  //RS / R0 = 4.4 ppm 
//#define calibration_button 13 //Pin to calibrate your sensor

//Declare Sensor
MQUnifiedsensor MQ4(placa, Voltage_Resolution, ADC_Bit_Resolution, pin, type);

void setup() {
  //Init the serial port communication - to debug the library
  Serial.begin(9600); //Init serial port

  //Set math model to calculate the PPM concentration and the value of constants
  MQ4.setRegressionMethod(1); //_PPM =  a*ratio^b
  /*
    Exponential regression:
  Gas    | a      | b
  LPG    | 3811.9 | -3.113
  CH4    | 1012.7 | -2.786
  CO     | 200000000000000 | -19.05
  Alcohol| 60000000000 | -14.01
  smoke  | 30000000 | -8.308
  */

  
  
  /*****************************  MQ CAlibration ********************************************/ 
  // Explanation: 
  // In this routine the sensor will measure the resistance of the sensor supposing before was pre-heated
  // and now is on clean air (Calibration conditions), and it will setup R0 value.
  // We recomend execute this routine only on setup or on the laboratory and save on the eeprom of your arduino
  // This routine not need to execute to every restart, you can load your R0 if you know the value
  // Acknowledgements: https://jayconsystems.com/blog/understanding-a-gas-sensor
  Serial.print("Calibrating please wait.");
  float calcR0 = 0;
  for(int i = 0; i<=10; i ++)
  {
    MQ4.update(); // Update data, the arduino will be read the voltage on the analog pin
    calcR0 += MQ4.calibrate(RatioMQ4CleanAir);
    Serial.print(".");
  }
  MQ4.setR0(calcR0/10);
  Serial.println("  done!.");
  
  if(isinf(calcR0)) {Serial.println("Warning: Conection issue founded, R0 is infite (Open circuit detected) please check your wiring and supply"); while(1);}
  if(calcR0 == 0){Serial.println("Warning: Conection issue founded, R0 is zero (Analog pin with short circuit to ground) please check your wiring and supply"); while(1);}
  /*****************************  MQ CAlibration ********************************************/ 

  /* 
    //If the RL value is different from 10K please assign your RL value with the following method:
    MQ4.setRL(10);
  */

  /*****************************  MQ Init ********************************************/ 
  //Input: setup flag, if this function are on setup will print the headers (Optional - Default value: False)
  //Output: print on serial port the information about sensor and sensor readings
  //Remarks: Configure the pin of arduino as input.
  /************************************************************************************/ 
  MQ4.init(); 

  Serial.println("*************************** Lectures from MQ-board ***************************");
  Serial.println("|    LPG   |  CH4 |   CO    |    Alcohol    |   Smoke    |");  
}

void loop() {
  MQ4.update(); // Update data, the arduino will be read the voltage on the analog pin
  
  MQ4.setA(3811.9); MQ4.setB(-3.113); // Configurate the ecuation values to get CH4 concentration
  float LPG = MQ4.readSensor(); // Sensor will read PPM concentration using the model and a and b values setted before or in the setup
  
  MQ4.setA(1012.7); MQ4.setB(-2.786); // Configurate the ecuation values to get CH4 concentration
  float CH4 = MQ4.readSensor(); // Sensor will read PPM concentration using the model and a and b values setted before or in the setup
  
  MQ4.setA(200000000000000); MQ4.setB(-19.05); // Configurate the ecuation values to get CH4 concentration
  float CO = MQ4.readSensor(); // Sensor will read PPM concentration using the model and a and b values setted before or in the setup
  
  MQ4.setA(60000000000); MQ4.setB(-14.01); // Configurate the ecuation values to get CH4 concentration
  float Alcohol = MQ4.readSensor(); // Sensor will read PPM concentration using the model and a and b values setted before or in the setup
  
  MQ4.setA(30000000); MQ4.setB(-8.308); // Configurate the ecuation values to get CH4 concentration
  float Smoke = MQ4.readSensor(); // Sensor will read PPM concentration using the model and a and b values setted before or in the setup
  
  Serial.print("|    "); Serial.print(LPG);
  Serial.print("    |    "); Serial.print(CH4);
  Serial.print("    |    "); Serial.print(CO);
  Serial.print("    |    "); Serial.print(Alcohol);
  Serial.print("    |    "); Serial.print(Smoke);
  Serial.println("    |");

  delay(500); //Sampling frequency
}