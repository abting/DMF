#include <Wire.h>


void setup() {

  Wire.begin();

  Wire.beginTransmission (0x40); //max7300 address. slave address(7 bits) + R/W (0)

  sendI2C(0x04,0x01); //configuration port, set shutdown mode off

  //set ports in output mode (all of them)
  sendI2C(0x09,0x55); 
  sendI2C(0x0A,0x55);
  sendI2C(0x0B,0x55);
  sendI2C(0x0C,0x55);
  sendI2C(0x0D,0x55);
  sendI2C(0x0E,0x55);
  sendI2C(0x0F,0x55);
  
  Wire.endTransmission();

 Serial.begin(9600);
}

void loop() {
  sendI2C(0x2E,1);
}

void sendI2C (byte command, byte data)//write function
{
  Wire.beginTransmission (0x40);
  Wire.write (command);
  Wire.write(data);
  Wire.endTransmission();
}
void clearAll()
{
  Wire.beginTransmission(0x40);
  Wire.write(0x00);
  Wire.write(0x00);
  Wire.endTransmission();
}



