/*
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

*/


//slave 1. This device controls the motor power, motor direction and the sound.
#include <Wire.h>

int motorPin = 9;
int soundPin = 10;
int motorPwr = 8;
int y;
int soundFlag = 0;

void setup() {
pinMode(motorPin, OUTPUT);
pinMode(motorPwr,OUTPUT);
pinMode(soundPin, OUTPUT);

Wire.begin(8);
Wire.onReceive(receivedEvent);
Serial.begin(9600);

}

void loop() {
  Serial.println(y);
switch(y){
  
  case 0:
   //motor off nothing is happening

   motorOff(motorPwr);
   if(soundFlag == 0){
    digitalWrite(soundPin, HIGH);
   }
  if(soundFlag == 1){
  soundOff(soundPin);
  soundFlag = 0;
   }
  break;
   
  case 1:
    //motor on down

   motorOn(motorPwr);
   mDown(motorPin);

  soundOn(soundPin);
  soundFlag = 1;

  break;
  
  case 2:
    //motor off


   motorOff(motorPwr);


   break;
  case 3:
    //motor on up 
   motorOn(motorPwr);
   mUp(motorPin);
   break;
  }
  
}

void receivedEvent(int howMany){
  int x = Wire.read();
  y = x;
}
void motorOff(int mtrPwr){
  digitalWrite(mtrPwr, LOW);
}
void soundOff(int pinNum){
  digitalWrite(pinNum, HIGH);
  delay(10);
  digitalWrite(pinNum,LOW);
  delay(10);
}
void motorOn(int mtrPwr){
  digitalWrite(mtrPwr, HIGH);
}
void soundOn(int pinNum){
  digitalWrite(pinNum, LOW);
}
void mUp(int motorPin){
  digitalWrite(motorPin, LOW);
}
void mDown(int motorPin){
  digitalWrite(motorPin, HIGH);
}
