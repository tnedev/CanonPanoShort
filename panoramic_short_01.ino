#include <Stepper.h>
#include <SD.h>
#include <SPI.h>

File myFile;

int focusPin = 2;
int shutterPin = 3;
byte dataBitsRead;

float ng = 6.0; // "A" gear ratio
byte nv = 2; // "B" speed
int ts = 10; // "C" startup delay
int nb = 1; // "D" bracketing
int nw = 360; // "E" panorama width
int ns = 6; // "F" number of shooting positions
byte tf = 0; // "G" enable focus before shooting
int tb = 1; // "H" delay before
int ta = 1; // "I" delay after
int np = 1; // "J" number of panoramas
int tp = 60; // "K" delay between panoramas 

int i,k;

char cardReader[100];
char currentRead[4];
const int stepsPerRevolution=200;

Stepper motor(stepsPerRevolution,7,8,9,10);

void setup() {
  Serial.begin(9600);
  motor.setSpeed(60);
  
  Serial.print("Initializing SD card...");
  // On the Ethernet Shield, CS is pin 4. It's set as an output by default.
  // Note that even if it's not used as the CS pin, the hardware SS pin 
  // (10 on most Arduino boards, 53 on the Mega) must be left as an output 
  // or the SD library functions will not work. 
   pinMode(10, OUTPUT);
   
  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
  
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = SD.open("test.txt", FILE_WRITE);
  
  // if the file opened okay, write to it:
  if (myFile) {
    Serial.print("Writing to test.txt...");
    myFile.println("testing 1, 2, 3.");
	// close the file:
    myFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
  
  // re-open the file for reading:
  i=0;
  myFile = SD.open("data.txt");
  if (myFile) {
    while (myFile.available()) {
    	cardReader[i] = myFile.read();
        i++ ; 
    
    }
     dataBitsRead = i;
    // close the file:
    myFile.close();
  } else {
  	// if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
}

void run(){
  int rotatedAngle;
  
  delay(ts*1000); //startup delay
  for(int i=0; i<np ; i++){ // number of panoramas
    for(int k=0; k<ns; k++){
      delay(tb*1000);
      if(tf){
        digitalWrite(focusPin, HIGH);
        delay(50);
        digitalWrite(focusPin, LOW);
      }
      for(int t=0; t<nb; t++){
        digitalWrite(shutterPin, HIGH);
        delay(50);
        digitalWrite(shutterPin, LOW);
        delay(ta*1000);
      }
      motor.step((nw*ng)/ns);
      rotatedAngle = rotatedAngle + nw/ns; // count the rotated angle 
      }
      motor.step((360 - rotatedAngle%360)*ng); // return to initial position
      delay(tp*1000);
    }
  }

void setValue () // Take the values read from the card and store them in the operating values
{
  if(dataBitsRead>20)
  {
    for(i=0;i<=dataBitsRead;i++)
    {
      currentRead[0] = '0';
      currentRead[1] = '0';
      currentRead[2] = '0';
      currentRead[3] = '0';
      
      if(cardReader[i] == 'B')
      {
        k=i+1;
        while (cardReader[k] != 'C')
        {
        currentRead[k-i-1] = cardReader[k];
        }
        nv = ( ((int)currentRead[0]-(int)'0')*100 + ((int)currentRead[1]-(int)'0')*10 + (int)currentRead[2]-(int)'0' );
      }
      
       if(cardReader[i] == 'C')
      {
        k=i+1;
        while (cardReader[k] != 'D')
        {
        currentRead[k-i-1] = cardReader[k];
        }
       ts =( ((int)currentRead[0]-(int)'0')*1000 + ((int)currentRead[1]-(int)'0')*100 + ((int)currentRead[2]-(int)'0')*10 + (int)currentRead[3]-(int)'0' );

      }
      if(cardReader[i] == 'D')
      {
        k=i+1;
        while (cardReader[k] != 'E')
        {
        currentRead[k-i-1] = cardReader[k];
        }
       nb =( ((int)currentRead[0]-(int)'0')*1000 + ((int)currentRead[1]-(int)'0')*100 + ((int)currentRead[2]-(int)'0')*10 + (int)currentRead[3]-(int)'0' );

      }
      
      if(cardReader[i] == 'E')
      {
        k=i+1;
        while (cardReader[k] != 'F')
        {
        currentRead[k-i-1] = cardReader[k];
        }
       nw =( ((int)currentRead[0]-(int)'0')*1000 + ((int)currentRead[1]-(int)'0')*100 + ((int)currentRead[2]-(int)'0')*10 + (int)currentRead[3]-(int)'0' );

      }
      
      if(cardReader[i] == 'F')
      {
        k=i+1;
        while (cardReader[k] != 'G')
        {
        currentRead[k-i-1] = cardReader[k];
        }
       ns =( ((int)currentRead[0]-(int)'0')*1000 + ((int)currentRead[1]-(int)'0')*100 + ((int)currentRead[2]-(int)'0')*10 + (int)currentRead[3]-(int)'0' );

      }
      
       if(cardReader[i] == 'G')
      {
        k=i+1;
        while (cardReader[k] != 'H')
        {
        currentRead[k-i-1] = cardReader[k];
        }
        tf = ( ((int)currentRead[0]-(int)'0')*100 + ((int)currentRead[1]-(int)'0')*10 + (int)currentRead[2]-(int)'0' );
      }
      if(cardReader[i] == 'H')
      {
        k=i+1;
        while (cardReader[k] != 'I')
        {
        currentRead[k-i-1] = cardReader[k];
        }
       tb =( ((int)currentRead[0]-(int)'0')*1000 + ((int)currentRead[1]-(int)'0')*100 + ((int)currentRead[2]-(int)'0')*10 + (int)currentRead[3]-(int)'0' );

      }
      if(cardReader[i] == 'I')
      {
        k=i+1;
        while (cardReader[k] != 'J')
        {
        currentRead[k-i-1] = cardReader[k];
        }
       ta =( ((int)currentRead[0]-(int)'0')*1000 + ((int)currentRead[1]-(int)'0')*100 + ((int)currentRead[2]-(int)'0')*10 + (int)currentRead[3]-(int)'0' );

      }
      if(cardReader[i] == 'J')
      {
        k=i+1;
        while (cardReader[k] != 'K')
        {
        currentRead[k-i-1] = cardReader[k];
        }
       np =( ((int)currentRead[0]-(int)'0')*1000 + ((int)currentRead[1]-(int)'0')*100 + ((int)currentRead[2]-(int)'0')*10 + (int)currentRead[3]-(int)'0' );

      }
      if(cardReader[i] == 'K')
      {
        k=i+1;
        while (cardReader[k] != 'L')
        {
        currentRead[k-i-1] = cardReader[k];
        }
       tp =( ((int)currentRead[0]-(int)'0')*1000 + ((int)currentRead[1]-(int)'0')*100 + ((int)currentRead[2]-(int)'0')*10 + (int)currentRead[3]-(int)'0' );

      }
    }
  }
    if (nv==1)
    motor.setSpeed(40);
      if (nv==2)
    motor.setSpeed(60);
      if (nv==3)
    motor.setSpeed(80);
}

void loop() {
  
  setValue();
 
 if(digitalRead(A0)) 
 {
   run();
 }
  
}
