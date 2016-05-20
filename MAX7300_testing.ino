#include <Wire.h>

void setup() {

  Wire.begin();

  sendI2C(0x40,0x04,0x01); //configuration port, set shutdown mode off
  sendI2C(0x41,0x04,0x01);
  
  //set ports in output mode (all of them)
  sendI2C(0x40,0x09,0x55); 
  sendI2C(0x40,0x0A,0x55);
  sendI2C(0x40,0x0B,0x55);
  sendI2C(0x40,0x0C,0x55);
  sendI2C(0x40,0x0D,0x55);
  sendI2C(0x40,0x0E,0x55);
  sendI2C(0x40,0x0F,0x55);

  sendI2C(0x41,0x09,0x55); 
  sendI2C(0x41,0x0A,0x55);
  sendI2C(0x41,0x0B,0x55);
  sendI2C(0x41,0x0C,0x55);
  sendI2C(0x41,0x0D,0x55);
  sendI2C(0x41,0x0E,0x55);
  sendI2C(0x41,0x0F,0x55);

 Serial.begin(9600);
}

void loop() {
  delay(200);
  clearAll();
  
  sendI2C(0x40,0x2E,1);
  sendI2C(0x41,0x2E,1);

  delay(200);
  clearAll();
}

void sendI2C (byte MAX, byte command, byte data)//write function
{
  Wire.beginTransmission (MAX);
  Wire.write (command);
  Wire.write(data);
  Wire.endTransmission();
}

void clearAll() //group ports together and turn them off
{
  sendI2C(0x40,0x4C,0);
  sendI2C(0x40,0x53,0);
  sendI2C(0x40,0x5A,0);
  sendI2C(0x41,0x4C,0);
  sendI2C(0x41,0x53,0);
  sendI2C(0x41,0x5A,0);
}

