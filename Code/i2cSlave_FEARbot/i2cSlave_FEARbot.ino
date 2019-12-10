// Bliss Brass
// 11-29-19
// ECE 411 Capstone Project FEAR BOT
// 'Runs' away from detected noises
// Set board to Leonardo ETH (should be same pinout as normal Leonardo)
// To Re-program hold physical Reset button down until it says "Uploading"

#include <Wire.h>

int z = 370;
int rightForwards = 10; // 10 and 11 paired
int rightBackwards = 11;
int leftBackwards = 12; // 12 and 6 paired
int leftForwards = 6;

//int brightness = 0;
//int fadeAmount = 5;

void setup() {
  Wire.begin(4);                   // Turning Atmega into a slave on address 0x04
  Wire.onReceive(receiveEvent);    // On start sequence call function
  Serial.begin(9600);              // Set the Baud rate

  pinMode(rightForwards, OUTPUT);
  pinMode(rightBackwards, OUTPUT);
  pinMode(leftForwards, OUTPUT);
  pinMode(leftBackwards, OUTPUT);
  // AtMega32u4 physical pins
  // PB6 Pin 30 and PB7 Pin 12 paired
  // PD6 Pin 26 and PD7 Pin 27 Paired
}



void loop() {
  delay(100);
  // Noise is to the Back-Right turn LEFT
  if ((270 >= z) && (z > 110)) {
    // Add 1 millisecond delay
    delay(1);
    for (int i = 1000; i > 0; i--) {  
      analogWrite(leftBackwards, 255);
      analogWrite(rightForwards, 255);
      delayMicroseconds(75);  // Delaying 50 Micro Seconds
      analogWrite(leftBackwards, 0);
      analogWrite(rightForwards, 0);
      delayMicroseconds(25);
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
  // MIGHT HAVE ERRORS
  // Noise is to the Back-Left turn RIGHT
  else if (((70 > z) && (z >= 0)) || ((360 >= z) && (z > 270))) {
    delay(1);
    for (int i = 1000; i > 0; i--) {  
      analogWrite(leftForwards, 255);
      analogWrite(rightBackwards, 255);
      delayMicroseconds(75);  // Delaying 50 Micro Seconds
      analogWrite(leftForwards, 0);
      analogWrite(rightBackwards, 0);
      delayMicroseconds(25);
    }
    z = 370;  // Setting Z outside the trigger range
  }

  // Noise is behind drive STRAIGHT 
  else if ((110 >= z) && (z >= 70)) {
    delay(1);
    for (int i = 1000; i > 0; i--) {  
      analogWrite(leftForwards, 255); // Setting V to max (Voltage)
      analogWrite(rightForwards, 255);
      delayMicroseconds(75);      // Delaying 50 Micro Seconds for PWM
      analogWrite(leftForwards, 0);   // Setting V to 0
      analogWrite(rightForwards, 0);
      delayMicroseconds(25);
    }
    z = 370;  // Setting Z outside the trigger range
  }
  else {
    // Idle state
  }
}

void receiveEvent(int howMany) {
  // Loop through all sent bytes
  String buff;

  // insert analogWrite all zero'd out here testing delays
  analogWrite(rightForwards, 0);
  analogWrite(rightBackwards, 0);
  analogWrite(leftForwards, 0);
  analogWrite(leftBackwards, 0);
  //
  
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
