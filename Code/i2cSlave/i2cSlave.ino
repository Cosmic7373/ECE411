// Bliss 
// 11-29-19
// Bliss Brass
// ECE 411
// Turning the ItsyBitsy 32u4 3V breakout board into a I2C Slave and controlling outputs
// Be sure to select the Itsy Bitsy 32u4 3V board in the Tools menu

#include <Wire.h>

int z = 370;
int control1 = 13;
int control2 = 9;
int control3 = 10;
int control4 = 6;

int brightness = 0;
int fadeAmount = 5;

void setup() {
  Wire.begin(4);                   // Turning Atmega into a slave on address 0x04
  Wire.onReceive(receiveEvent);    // On start sequence call function
  Serial.begin(9600);              // Set the Baud rate

  pinMode(control1, OUTPUT);
  pinMode(control2, OUTPUT);
  pinMode(control3, OUTPUT);
  //pinMode(control4, OUTPUT);
  // PB6 Pin 30 and PB7 Pin 12 paired
  // PD6 Pin 26 and PD7 Pin 27 Paired
}

void loop() {
  //delay(100);
  // Noise is to the ??? turn
  if ((360 >= z) && (z > 200)) {
    for (int i = 10000; i > 0; i--) {  
      analogWrite(control1, 255);
      delayMicroseconds(50);  // Delaying 50 Micro Seconds
      analogWrite(control1, 0);
      delayMicroseconds(50);
    }
    z = 370;  // Setting Z outside the trigger range
    /*analogWrite(control1, brightness);
    brightness = brightness + fadeAmount;
    if (brightness <= 0 || brightness >= 255) {
      fadeAmount = -fadeAmount;
    }
    delay(30);*/
  }
  // Noise is to the ??? turn
  else if ((z >= 0) && (z < 160)) {
    for (int i = 10000; i > 0; i--) {  
      analogWrite(control2, 255);
      delayMicroseconds(50);  // Delaying 50 Micro Seconds
      analogWrite(control2, 0);
      delayMicroseconds(50);
    }
    z = 370;  // Setting Z outside the trigger range
  }

  // Noise is behind drive straight 
  else if ((200 >= z) && (z >= 160)) {
    for (int i = 10000; i > 0; i--) {  
      analogWrite(control3, 255); // Setting V to max (Voltage)
      delayMicroseconds(50);      // Delaying 50 Micro Seconds for PWM
      analogWrite(control3, 0);   // Setting V to 0
      delayMicroseconds(50);
    }
    z = 370;  // Setting Z outside the trigger range
  }
  else {
    
  }
}

void receiveEvent(int howMany) {
  // Loop through all sent bytes
  String buff;
  
  while(0 < Wire.available()) {
    char c = Wire.read();
    //Serial.print(c);
    buff = buff + c;
  }
  Serial.println(buff);
  
  z = buff.toInt();
  Serial.print("Z is ");
  Serial.println(z);

}
