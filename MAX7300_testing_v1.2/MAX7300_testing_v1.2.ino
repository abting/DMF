#include "Wire.h"
#include "Math.h"

//char elec;
int elecInt;
int count=0;


boolean e = false; //flag for seeing 'e'
boolean s = false; //flag for seeing 's'
boolean p = false; //flag for seeing 'p'
boolean endT = false; //flag for last array element

boolean isChara = false;
boolean complete = false;
boolean dataFinish = false; //flag if all data has been read and processed

boolean skip = false;

int ledDelay = 350; 
byte max7300_01 = 0x40; //max7300 slave 1
byte max7300_02 = 0x41; //max7300 slave 2
//Pins addressed from 5 to 24 following the pin configuration diagram

void setup()
{
  Wire.begin();
  initializeMax(max7300_01);
  clearLED();
  Serial.begin(9600);
  Serial.println("Enter number");
}

void loop()
{
  char  dataRead[100];
  int numArray[10]; 
  //Reads the entire string
  while(endT == false){
    if(Serial.available()>0){
      Serial.readBytes(dataRead,500);        //reads upto 1000 bytes
      endT=true;
      }
  }
  
  String StArray [10];    //Stores each element as a String
  int count = 0;
  char * data = strtok(dataRead, ",");  //Stores first element before ","

  //while ((data = strtok (NULL, ",")) != NULL)
  while(data!=NULL && dataFinish != true)
    {
      StArray[count] = data;      //Stores subsequent elements surrounded by ","
      numArray[count] = StArray[count].toInt();
      Serial.println(numArray[count]);
      data = strtok(NULL,",");
      count++;
    }
    dataFinish = true;    //Can remove this condition if serial.println(numArray[count]) is removed   
    
    while (!complete)
    {
      for (int i=0;i<10;i++) //going through the array
      {
        isCharacter(numArray[i]); //find out if there was a character or not
        if (isChara)
        {
          i++; //go to the next value only if you've found a character
          isChara = false; //set it back to false.
        }
        
        if(s) //if s is flagged to be true
        {
          int elec = numArray[i]; 
          Serial.println("");
          Serial.print("turning on: ");
          Serial.println(elec);
          delay(ledDelay);
          turnOn(elec); //this should happen sequentially
          delay(ledDelay);
          clearLED();
        }
         
        if(p) //if p i flagged to be true
        {
          int elec = numArray[i]; //.toInt();
          Serial.println("");
          Serial.print("turning on: ");
          Serial.print(elec);
          Serial.println(" in parallel");
          turnOn(elec); //this should happen sequentially w/o delay
        }
        
        if(e) //if e is flagged to be true
        {
          complete = true; //exit the for loop as soon as you see the 'e'nd mark
          clearLED();
        }
     }
   }
}

void isCharacter(int num)// find out if there is a character or not, handle appropriately
{
  if(num == 1000) //if you see an 's'
    {
      Serial.println("there is an s!");
      if (p)//if before this p was true
      {
        Serial.println("there was a p before!(case s)Turning on:");
        delay(ledDelay);
        clearLED();
      }  
      setS();
      isChara = true; //flag that there is a character
    }

    else if (num == 1001)  //Multiple electrodes turned on simultaneously
    {
      Serial.println("there is a p!");
      if (p)//if before this p was true
      {
        Serial.print("there was a p before!(case p)");
        delay(ledDelay);
        clearLED();       
      }
      setP();
      isChara = true;
    }

    else if (num == 1002)   //end of transmission
    {
      if (p)//if before this p was true
     {
      Serial.println("there was a p before!(case e)");
        delay(ledDelay);
     }
     Serial.println("there is an e!");
     setE();
     isChara = true;
    }
}
 
void turnOn(int num)
{
  byte address;
  int decNum;
  if (num<25){
    address = max7300_01;
    decNum = num+39;
  }
  else{
    address = max7300_02;
    decNum = num+19;
    }
  
  sendI2C(address,decNum,1);
}

void clearLED()
{
  sendI2C(max7300_01,0x44,0);
  sendI2C(max7300_01,0x4C,0);
  sendI2C(max7300_01,0x54,0);
  sendI2C(max7300_01,0x5C,0);
  sendI2C(max7300_02,0x44,0);
  sendI2C(max7300_02,0x4C,0);
  sendI2C(max7300_02,0x54,0);
  sendI2C(max7300_02,0x5C,0);
}

void setS()
{
  s=true;
  p=false;
  e=false;
}

void setP()
{
  s=false;
  p=true;
  e=false;
}

void setE()
{
  s=false;
  p=false;
  e=true;
}

void sendI2C (byte Max_Address, byte command, byte data)//write function
{
  Wire.beginTransmission (Max_Address);
  Wire.write (command);
  Wire.write(data);
  Wire.endTransmission();
}

void initializeMax(byte Max_Address){
   Wire.beginTransmission (Max_Address); //max7300 address. slave address(7 bits) + R/W (0)
  //configuration port, set shutdown mode off
  sendI2C(Max_Address,0x04,0x01);
  //set ports in output mode (all of them)
  sendI2C(Max_Address,0x09,0x55); 
  sendI2C(Max_Address,0x0A,0x55);
  sendI2C(Max_Address,0x0B,0x55);
  sendI2C(Max_Address,0x0C,0x55);
  sendI2C(Max_Address,0x0D,0x55);
  sendI2C(Max_Address,0x0E,0x55);
  sendI2C(Max_Address,0x0F,0x55);
  Wire.endTransmission();
  }

