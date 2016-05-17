//I2C bus address is 0X20

#include "Wire.h"
#include "Math.h"

char elec;
int elecInt;
int count=0;
//String numArray [100]; // = {s,1,2,3,p,5,2,s,3,e}; //test this
//String dataString = {s,1,2,3,p,5,2,s,3,e}; //test this

boolean e = false; //flag for seeing 'e'
boolean s = false; //flag for seeing 's'
boolean p = false; //flag for seeing 'p'
boolean endT = false; //flag for last array element

boolean isChara = false;
boolean complete = false;
boolean dataFinish = false;

int parallelAdd = 0; //keep track of the added values

boolean skip = false;

int ledDelay = 50; 

void setup()
{
 Wire.begin(); // wake up I2C bus
 // set I/O pins to outputs
 Wire.beginTransmission(0x20);
 Wire.write(0x00); // IODIRA register
 Wire.write(0x00); // set all of port A to outputs
 Wire.endTransmission();

 Wire.beginTransmission(0x20);
 Wire.write(0x01); // IODIRB register
 Wire.write(0x00); // set all of port B to outputs
 Wire.endTransmission();
 clearLED();

Serial.begin(9600);

Serial.println("Enter number");

}

void loop()
{
char  dataRead[300];
int numArray[80]; 
//Reads the entire string
  while(endT == false){
    if(Serial.available()>0){
      Serial.readBytes(dataRead,2000);        //reads upto 1000 bytes
      endT=true;
      }
  }



  String StArray [80];    //Stores each element as a String
       //Stores each element as an int
  int count = 0;
  char * data = strtok(dataRead, ",");  //Stores first element before ","
  //StArray[count] = data;
  //numArray[count] = StArray[count].toInt();
 // Serial.println(StArray[count]);
  //Serial.println(numArray[count]);
     

  //while ((data = strtok (NULL, ",")) != NULL)
  while((data!=NULL && dataFinish != true))
    {
      //Serial.println("end4");
      //delay(50);
      StArray[count] = data;      //Stores subsequent elements surrounded by ","
      numArray[count] = StArray[count].toInt();
     // Serial.println(StArray[count]);
     Serial.println(numArray[count]);
      data = strtok(NULL,",");
      count++;
      
    }
    dataFinish = true;
 
    //Serial.println("end");

      
    
  //Receive data and create an array
 // while(endT == false){
//    if(Serial.available()>0){
//      data=Serial.readString();
//      numArray[count] = data;
//      count++;
//      if(data == "1002"){
//        endT=true;
//      }
//    }
  
  
  while (!complete)
  {
    for (int i=0;i<80;i++) //going through the array
    {delay(50);
      isCharacter(numArray[i]); //find out if there was a character or not
      if (isChara)
      {
        i++; //go to the next value only if you've found a character
        isChara = false; //set it back to false.
      }

     if(s) //if s is flagged to be true
    {
      int elec = numArray[i]; //.toInt();
      //int elecInt = convertFromASCII(elec); //convert your received ASCII # into number
      Serial.println("");
      Serial.print("turning on: ");
      Serial.println(elec);
      turnOn(elec); //this should happen sequentially
    }

     if(p) //if p i flagged to be true
    {
      int elec = numArray[i]; //.toInt();
      //int elecInt = convertFromASCII(elec); //convert your received ASCII # into number
      parallelAdd = parallelAdd + convertToBin(elec); //keep count in binary as you go
      Serial.println("");
      Serial.print("keeping count: ");
      Serial.println(parallelAdd);
    }

     if(e) //if e is flagged to be true
    {
      complete = true; //exit the for loop as soon as you see the 'e'nd mark
      clearLED();
      //Serial.print("end1");
    }
   }
 }
}

//int convertFromASCII(int x)
//{
//  int elecInt = int(x)-48; //converting the char received into number
//  return elecInt;
//}

void isCharacter(int num)// find out if there is a character or not, handle appropriately
{
  if(num == 1000) //if you see an 's'
    {
      Serial.println("there is an s!");
      if (p)//if before this p was true
      {
        Serial.print("there was a p before!(case s)Turning on:");
        turnOn(parallelAdd);//turnOn whatever value you had stored in parallel before
        Serial.println(parallelAdd);
        parallelAdd = 0; //reset the value of the parallelAdd = 0
        setS();
      }
      else
      {
        setS();
      }
      isChara = true; //flag that there is a character
    }

    else if (num == 1001)  //Multiple electrodes turned on simultaneously
    {
      Serial.println("there is a p!");
      if (p)//if before this p was true
      {
        Serial.print("there was a p before!(case p)Turning on:");
        turnOn(parallelAdd);//turnOn whatever value you had stored in parallel before
        Serial.println(parallelAdd);
        parallelAdd = 0; //reset the value of the parallelAdd = 0
        setP();
      }
      else
      {
       setP();
      }
      isChara = true;
    }

    else if (num == 1002)   //end of transmission
    {
      Serial.println("there is an e!");
      if (p)//if before this p was true
      {
        Serial.println("there was a p before!(case e)Turning on:");
        Serial.println(parallelAdd);
        turnOn(parallelAdd);//turnOn whatever value you had stored in parallel before
        parallelAdd = 0; //reset the value of the parallelAdd = 0
        setE();
      }
      else
      {
        setE();
      }
      isChara = true;
    }
}
 
void turnOn(int num)
{
  clearLED();
  int column = 0;
  int decNum;

  while (!skip)
  {
    if (p) //assuming that when in parallel, will always be column 1
    {
      decNum = num; //don't convert as it is already converted
      column = 1;
      skip = true;
    }

    else
    {
      if (num<6)
      {
        column = 1;
        decNum = convertToBin(num);
        skip = true;
      }
      if (num>=6)
      {
        num -= 5;
        decNum = convertToBin(num);
        column = 2;
        skip = true;
      }
    }
  }

    if (column ==1) //for port B (1-2-3-4-5)
     {
      Wire.beginTransmission(0x20);
      Wire.write(0x13); //port B
      Wire.write(decNum);
      Wire.endTransmission();
      delay(ledDelay);
      skip = false;
     }
     
     if (column ==2) //for port B (1-2-3-4-5)
     {
      Wire.beginTransmission(0x20);
      Wire.write(0x12); //port A
      Wire.write(decNum);
      Wire.endTransmission();
      delay(ledDelay);
      skip = false;
     }
  
}

int convertToBin(int dec)
{
//  if (dec ==1||dec==2)
//  {
//    return dec;
//  }
//  else
//  {
//    int dec1 = (dec-1);
//    int dec2 = pow(2,dec1)+1;
//    return dec2;
//  }

  
    return 1 << (dec-1);    //shift 1 by decimal number
}

void clearLED()
{
 Wire.beginTransmission(0x20);
 Wire.write(0x13); //port B
 Wire.write(0);
 Wire.endTransmission();
 Wire.beginTransmission(0x20);
 Wire.write(0x12); //port A
 Wire.write(0);
 Wire.endTransmission();
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


