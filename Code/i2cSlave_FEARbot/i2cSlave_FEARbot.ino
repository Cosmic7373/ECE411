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
  delay(10); // Small delay for Z to update properlly
  // Noise is to the Back-Right turn LEFT
  if ((250 > z) && (z > 130)) {
    //Serial.print("Z is RIGHT");
    //Serial.println(z);
    
    for (int i = 5000; i > 0; i--) {  
      analogWrite(leftBackwards, 255);
      analogWrite(rightForwards, 255);
      delayMicroseconds(75);  // Delaying 50 Micro Seconds
      analogWrite(leftBackwards, 0);
      analogWrite(rightForwards, 0);
      delayMicroseconds(25);
    }
    delay(350); // Delay to counter act motor sound self activation
    z = 370;  // Setting Z outside the trigger range
    /*analogWrite(control1, brightness);
    brightness = brightness + fadeAmount;
    if (brightness <= 0 || brightness >= 255) {
      fadeAmount = -fadeAmount;
    }
    delay(30);*/
  }
  // MIGHT HAVE ERRORS
  // Noise is directly in FRONT turn around!
  else if (z == 270) {
    for (int i = 11900; i > 0; i--) {  
      analogWrite(leftBackwards, 255);
      analogWrite(rightForwards, 255);
      delayMicroseconds(75);  // Delaying 50 Micro Seconds
      analogWrite(leftBackwards, 0);
      analogWrite(rightForwards, 0);
      delayMicroseconds(25);
    }
    delay(350); // Delay to counter act motor sound self activation
    z = 370;  // Setting Z outside the trigger range
  }
  // Noise is to the front left turn RIGHT
  else if (z == 250) {
    for (int i = 11500; i > 0; i--) {  
      analogWrite(leftBackwards, 255);
      analogWrite(rightForwards, 255);
      delayMicroseconds(75);  // Delaying 50 Micro Seconds
      analogWrite(leftBackwards, 0);
      analogWrite(rightForwards, 0);
      delayMicroseconds(25);
    }
    delay(350); // Delay to counter act motor sound self activation
    z = 370;  // Setting Z outside the trigger range
  }
  // Noise is to the front right turn LEFT
  else if (z == 290) {
    for (int i = 11500; i > 0; i--) {  
      analogWrite(leftForwards, 255);
      analogWrite(rightBackwards, 255);
      delayMicroseconds(75);  // Delaying 50 Micro Seconds
      analogWrite(leftForwards, 0);
      analogWrite(rightBackwards, 0);
      delayMicroseconds(25);
    }
    delay(350); // Delay to counter act motor sound self activation
    z = 370;  // Setting Z outside the trigger range
  }

  // Noise is to the left turn RIGHT
  else if (((50 > z) && (z >= 0)) || ((360 >= z) && (z > 290))) {
    //Serial.print("Z is LEFT");
    //Serial.println(z);

    for (int i = 5000; i > 0; i--) {  
      analogWrite(leftForwards, 255);
      analogWrite(rightBackwards, 255);
      delayMicroseconds(75);  // Delaying 50 Micro Seconds
      analogWrite(leftForwards, 0);
      analogWrite(rightBackwards, 0);
      delayMicroseconds(25);
    }
    delay(350); // Delay to counter act motor sound self activation
    z = 370;  // Setting Z outside the trigger range
  }

  // Noise is behind drive STRAIGHT 
  else if ((130 >= z) && (z >= 50)) {
    //Serial.print("Z is STRAIGHT ");
    //Serial.println(z);
    for (int i = 6000; i > 0; i--) {  
      analogWrite(leftForwards, 255); // Setting V to max (Voltage)
      analogWrite(rightForwards, 255);
      delayMicroseconds(75);      // Delaying 50 Micro Seconds for PWM
      analogWrite(leftForwards, 0);   // Setting V to 0
      analogWrite(rightForwards, 0);
      delayMicroseconds(25);
    }
    delay(350); // Delay to counter act motor sound self activation
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
