/*
  Software serial multple serial test

 Receives from the hardware serial, sends to software serial.
 Receives from software serial, sends to hardware serial.

 The circuit:
 * RX is digital pin 10 (connect to TX of other device)
 * TX is digital pin 11 (connect to RX of other device)

 Note:
 Not all pins on the Mega and Mega 2560 support change interrupts,
 so only the following can be used for RX:
 10, 11, 12, 13, 50, 51, 52, 53, 62, 63, 64, 65, 66, 67, 68, 69

 Not all pins on the Leonardo and Micro support change interrupts,
 so only the following can be used for RX:
 8, 9, 10, 11, 14 (MISO), 15 (SCK), 16 (MOSI).

 created back in the mists of time
 modified 25 May 2012
 by Tom Igoe
 based on Mikal Hart's example

 This example code is in the public domain.

  20 character 4 line I2C Display
 Backpack Interface labelled "YwRobot Arduino LCM1602 IIC V1"
 Connect Vcc and Ground, SDA to A4, SCL to A5 on Arduino
 terry@yourduino.com 

 */
 /*-----( Import needed libraries )-----*/
#include <Wire.h>  // Comes with Arduino IDE
#include <mcp_can.h>
#include <SPI.h>
#include <SoftwareSerial.h>


SoftwareSerial mySerial(10, 11, true); // RX, TX
SoftwareSerial mySerial2(12, 7, true); // RX, TX

// Get the LCD I2C Library here: 
// https://bitbucket.org/fmalpartida/new-liquidcrystal/downloads
// Move any other LCD libraries to another folder or delete them
// See Library "Docs" folder for possible commands etc.
#include <LiquidCrystal_I2C.h>

/*-----( Declare Constants )-----*/
/*-----( Declare objects )-----*/
// set the LCD address to 0x27 for a 20 chars 4 line display
// Set the pins on the I2C chip used for LCD connections:
//                    addr, en,rw,rs,d4,d5,d6,d7,bl,blpol
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, NEGATIVE);  // Set the LCD I2C address

uint16_t theserial; 
bool calib_flag = false;

void setup() 
{
  // Open serial communications and wait for port to open:
  Serial.begin(57600);
  while (!Serial) 
  {
    ; // wait for serial port to connect. Needed for native USB port only

  }


  Serial.println("Startup! ");

  lcd.begin(20,4);         // initialize the lcd for 20 chars 4 lines, turn on backlight
// ------- Quick 3 blinks of backlight  -------------
  for(int i = 0; i< 3; i++)
  {
    lcd.backlight();
    delay(250);
    lcd.noBacklight();
    delay(250);
  }
  lcd.backlight(); // finish with backlight on  
  lcd.setCursor(3,0); //Start at character 4 on line 0
  lcd.print("Hello, world!");


  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);
  mySerial2.begin(9600);
  //mySerial.println("Hello, world?");

  lcd.clear();
  lcd.setCursor(1,0); //Start at character 2 on line 0
  lcd.print(" AFR ");
}

void loop() 
{ // run over and over
 
  if (mySerial2.available()) 
     {
       theserial = mySerial2.read();
       
       if ((calib_flag == true) && (theserial == 190))
          {
            calib_flag == false;
            lcd.setCursor(0,2); //Start at character 0 on line 2
            lcd.print("                  ");
          }
       
       if ((theserial < 191) && (theserial > 89) || (calib_flag == true))
          {
            //this is a data value
            //Serial.print("a data ");
            //Serial.print(theserial);
            //Serial.print(" ");
            lcd.setCursor(6,0); //Start at character 6 on line 0
            lcd.print("    "); 
            if (theserial >99)
            {
              lcd.setCursor(6,0); //Start at character 6 on line 0
              lcd.print(theserial);
            }
            else
            {
              lcd.setCursor(7,0); //Start at character 6 on line 0
              lcd.print(theserial);
            }
            lcd.setCursor(8,0); //Start at character 8 on line 0
            lcd.print(".");
            int tmp1, tmp2;
            //tmp1 = (theserial/10);
            tmp1 = divu10(theserial);
            //tmp2 = (tmp1*10);
            tmp2 = mult10(tmp1);
            
            lcd.print((theserial-tmp2));
            
            Serial.print(theserial);
            Serial.print(" ");   
            
            mySerial.write(theserial);
          }
       else
          {
            //is a command
            //Serial.print("a cmd ");
            //Serial.print(theserial);
            //Serial.print(" ");   
            decode_command(theserial);
        
          }
          
    
     }
     
  if (Serial.available())
     {
      int datain;
      datain = Serial.read();
      //softserial_command(datain);
       //mySerial.write(datain);
       //Serial.print(datain);
     }
  
}

void decode_command(int cmd1)
{
  
  int cmd2;
  int cmd3;
  int cmd4;
 switch (cmd1)  
        {
          case 17:
           while (mySerial2.available() == 0) {}
                cmd2 = mySerial2.read();
               
                if (cmd2 == 28)
                    {
                     while (mySerial2.available() == 0) {}
                           cmd3 = mySerial2.read();
                           
                     while (mySerial2.available() == 0) {}
                           cmd4 = mySerial2.read();
                          
                           switch (cmd3)
                                 {
                                  case 35:
                                     switch (cmd4)
                                           {
                                            case 1:
                                            Serial.print("fuel type LAMBDA ");
                                            lcd.setCursor(0,1); //Start at character 0 on line 1
                                            lcd.print("                  ");
                                            lcd.setCursor(0,1); //Start at character 0 on line 1
                                            lcd.print("fuel type LAMBDA");
                                            break;
                                            case 2:
                                            Serial.print("fuel type E85 ");
                                            lcd.setCursor(0,1); //Start at character 0 on line 1
                                            lcd.print("                  ");
                                            lcd.setCursor(0,1); //Start at character 0 on line 1
                                            lcd.print("fuel type E85");
                                            break;
                                            case 3:
                                            Serial.print("fuel type PETROL / GAS ");
                                            lcd.setCursor(0,1); //Start at character 0 on line 1
                                            lcd.print("                  ");
                                            lcd.setCursor(0,1); //Start at character 0 on line 1
                                            lcd.print("fuel type PETROL");
                                            break;
                                            case 4:
                                            Serial.print("fuel type DIESEL ");
                                            lcd.setCursor(0,1); //Start at character 0 on line 1
                                            lcd.print("                  ");
                                            lcd.setCursor(0,1); //Start at character 0 on line 1
                                            lcd.print("fuel type DIESEL");
                                            break;
                                            case 5:
                                            Serial.print("fuel type METHANOL ");
                                            lcd.setCursor(0,1); //Start at character 0 on line 1
                                            lcd.print("                  ");
                                            lcd.setCursor(0,1); //Start at character 0 on line 1
                                            lcd.print("fuel type METHANOL");
                                            break;
                                           }  
                                  break;
                                  
                                  case 36:
                                      //nb min
                                      Serial.print(" NB Min ");
                                      Serial.print(cmd4);
                                  break;
                                  
                                  case 37:
                                     //nb max
                                     Serial.print(" NB Max ");
                                     Serial.print(cmd4);
                                  break;
                                  
                                  case 38:
                                     // analog afr low limit
                                     Serial.print(" AFR MIN ");
                                     Serial.print(cmd4);
                                  break;
                                  
                                  case 39:
                                     //analog afr high limit
                                     Serial.print(" AFR MAX ");
                                     Serial.print(cmd4);
                                  break;
                                 }
            
                    }
                     mySerial.write(cmd1); // print cmd1 to the led display
                     mySerial.write(cmd2); // print cmd2 to the led display
                     mySerial.write(cmd3); // print cmd3 to the led display
                     mySerial.write(cmd4); // print cmd4 to the led display
          break;

          case 18:
               while (mySerial2.available() == 0) {}
                cmd2 = mySerial2.read();
               
                if (cmd2 == 11)
                    {
                     while (mySerial2.available() == 0) {}
                           cmd3 = mySerial2.read();
                           if (cmd3 == 0)
                           {
                            //now in program mode choose fuel type
                            Serial.print(" SET FUEL type ");                          
                           }
                    }       
                mySerial.write(cmd1); // print cmd1 to the led display
                mySerial.write(cmd2); // print cmd2 to the led display
                mySerial.write(cmd3); // print cmd3 to the led display           
          break;

          case 226:
                while (mySerial2.available() == 0) {}
                      cmd2 = mySerial2.read();
                      switch (cmd2)
                            {                             
                              case 12:
                              //set NB low limit
                               Serial.print(" SET NB LOW Limit ");                                
                               lcd.setCursor(0,2); //Start at character 0 on line 2
                               lcd.print("                  ");
                               lcd.setCursor(0,2); //Start at character 0 on line 2
                               lcd.print("SET NB LOW Limit");
                              break;
                              
                              case 13:
                              //set nb high limit
                               Serial.print(" SET NB HIGH Limit ");                                
                               lcd.setCursor(0,2); //Start at character 0 on line 2
                               lcd.print("                  ");
                               lcd.setCursor(0,2); //Start at character 0 on line 2
                               lcd.print("SET NB HIGH Limit");
                              break;
                              
                              case 14:
                              //set afr low limit
                               Serial.print(" SET AFR LOW Limit ");                                
                               lcd.setCursor(0,2); //Start at character 0 on line 2
                               lcd.print("                  ");
                               lcd.setCursor(0,2); //Start at character 0 on line 2
                               lcd.print("SET AFR LOW Limit");
                              break;
                              
                              case 15:
                              //set afr high limit
                               Serial.print(" SET AFR HIGH Limit ");                                
                               lcd.setCursor(0,2); //Start at character 0 on line 2
                               lcd.print("                  ");
                               lcd.setCursor(0,2); //Start at character 0 on line 2
                               lcd.print("SET AFR HIGH Limit");
                              break;
                              
                              case 16:
                              // calibrate
                               Serial.print(" CALIBRATE ");                               
                               lcd.setCursor(0,2); //Start at character 0 on line 2
                               lcd.print("                  ");
                               lcd.setCursor(0,2); //Start at character 0 on line 2
                               lcd.print(" CALIBRATE"); 
                               calib_flag = true;
                              break;
                              
                            }
                      
                           while (mySerial2.available() == 0) {}
                           cmd3 = mySerial2.read();
                           if (cmd3 == 0)
                         {
                          //exit
                         }
                mySerial.write(cmd1); // print cmd1 to the led display
                mySerial.write(cmd2); // print cmd2 to the led display
                mySerial.write(cmd3); // print cmd3 to the led display           
          break;
        }
}

void the_serial_command()
{
switch (mySerial.read())
          {
          case 1:
                  
          break; 

          case 203:
                  
          break;
           
          case 92:
                  
          break;
           
          case 93:
                  
          break; 
          
          case 30:
                  
          break; 
          
          }
}

void softserial_command(int thedata)
 {
   switch (thedata)
          {
          case 'a':
          mySerial.write(18); 
          Serial.print(18);      
          mySerial.write(11);    
          Serial.print(11);         
          mySerial.write((byte)0);    
          Serial.print((byte)0);         
         // mySerial.write(3);     
         // Serial.print(3);        
          break; 
  
          case 'z':
          mySerial.write(190);   
          Serial.print(190);          
          break;

          case 'b':
          mySerial.write(226); 
          Serial.print(226);      
          mySerial.write(13);    
          Serial.print(13);         
          mySerial.write((byte)0);    
          Serial.print((byte)0);         
         // mySerial.write(140);     
         // Serial.print(140); 
          break;

          case 'd':
          mySerial.write(226); 
          Serial.print(226);      
          mySerial.write(14);    
          Serial.print(14);         
          mySerial.write((byte)0);    
          Serial.print((byte)0);
          break; 

          case 'e':
          mySerial.write((byte)0); 
          Serial.print((byte)0);
          break;

          case 'f':
          mySerial.write(18); 
          Serial.print(18);
          break;

          case 'g':
          mySerial.write(152); 
          Serial.print(152);
          break;

          case 'h':
          mySerial.write(130); 
          Serial.print(130);
          break;

          case 'i':
          mySerial.write(1); 
          Serial.print(1);
          break;

          case 'j':
          mySerial.write(2); 
          Serial.print(2);
          break;

          case 'k':
          mySerial.write(7); 
          Serial.print(7);
          break;

          case 'l':
          mySerial.write(5); 
          Serial.print(5);
          break;

          case 'm':
          mySerial.write(6); 
          Serial.print(6);
          break;

          case 'n':
          mySerial.write(8); 
          Serial.print(8);
          break;
          }

 }                    

 unsigned divu10(unsigned n)  
  {
    unsigned q, r;
    q = (n >> 1) + (n>> 2);
    q = q + (q >> 4);
    q = q + (q >> 8);
    q = q + (q >> 16);
    q = q >> 3;
    r = n - (((q << 2) + q) << 1);
    return q +(r > 9);
  }

unsigned mult10(unsigned n)
  {
    unsigned i;
    i = (n << 3) + (n << 1);
    return i;
  }

