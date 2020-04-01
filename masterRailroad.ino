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

//master for the railroad crossing implemented as a state machine

//libraries that this arduino will use in the machine
#include <Wire.h>
#include <Adafruit_NeoPixel.h>

// On a Trinket or Gemma we suggest changing this to 1:
// This is the pin that the DATA_IN pin is connected to on the first ring
#define LED_PIN    6

// How many NeoPixels are attached to the Arduino?
#define LED_COUNT 32

//must declare the strip using the built in declaration statement
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

//sensor pins, these are attached to the physical build of the device to sense when the arm is either up or down
int upSensor = 7;
int downSensor = 8;

//buttons for state transition, these will determine if the arm will go up or down depending on which state the machine is in
//for example, if we are in state 0 and the up button is pressed, it won't do anything, but if the down button is pressed then we change states
int upButton = 4;
int downButton = 5;

//vars to store state of the above buttons
int buttonStateUp;
int buttonStateDown;

//comflag to tell if we have already communicated with the other device without a state transition
int comflag = 0;

//to tell the previous states of the machine 
int prevState = 0;

//state var for switch case
int mode = 0;
/*
 * for reference in the switch case here are the following states and the corressponding numbers
 * Start = 0: this is the default start case where the arm is up and the LEDs are off (no train)
 * Move down/flash = 1: this is the state where the arm is moving down and the LEDs are flashing (incoming train)
 * flash (sound?) = 2: this is where the arm has completed its movement and the LEDs are flashing (train is passing)
 * move up/flash = 3: this is the state where the arm moves back up and the LEDs are still flashing
 * 
 * Note: I only commented on the first case since the cases are mostly the same
 */


void setup() {
//begin the I2C communication  protocol
Wire.begin();
//defining the inputs and outputs of the system
pinMode(upButton, INPUT);
pinMode(downButton, INPUT);
pinMode(upSensor, INPUT);
pinMode(downSensor, INPUT);
//begin the serial communication protocol for debugging
Serial.begin(9600);
//start the strip
strip.begin();
strip.show();
}

//byte to send of the I2C communication line
byte x = 0;

void loop() {
//read state of the buttons
  buttonStateDown = digitalRead(downButton);

  //state machine portion. This will determine the output of the I2C data line which will be read by the slave 
  switch(mode) {
    case 0:
    //clear the LEDs 
    clearLeds();
    //tell whether we have communicated with the other boards since we have been in this state
      if(prevState != mode){
        //actually send the data to the slaves
          sendData(mode);
      }

      //determine if the button is pressed
      if(buttonStateDown == HIGH){
        //if the button is pressed then we update the prevState to the current state and then change the mode
        prevState = mode;
        mode = 1;
      }
      else{
        //update the prevState and mode
        prevState = mode;
        mode = 0;
        //switch case will not work properly without this break statement         
       break;
      }
    case 1:
      if(prevState != mode){
          sendData(mode);
      }
      if(digitalRead(downSensor) == HIGH){
        prevState = mode;
        mode = 2;
        
      }
      else{        
        prevState = mode;
        mode = 1;
        break;
      }
    case 2:
     buttonStateUp = digitalRead(upButton);
     if(prevState != mode){
          sendData(mode);
        }
      if(buttonStateUp == HIGH){
        prevState = mode;
        mode = 3;
        
      }
      else{
        
        prevState = mode;
        mode = 2;
        break;
      }
    case 3:
    if(prevState != mode){
          sendData(mode);
    }
    if(digitalRead(upSensor) == HIGH){
      prevState = mode;
      comflag = 0;
      mode = 0;
    }
    else{
      prevState = mode;
      mode = 3;         
      break;
    }
  }

}

void clearLeds(){
   for(int a =0;a<=32;a++){
        strip.setPixelColor(a, strip.Color(0,0,0));
      }
      strip.show();
}

int sendData(byte stateNum){
  //begin wire transmisison to the given address
  Wire.beginTransmission(8);
  //send the state number to the slave
  Wire.write(stateNum);
  //end the transmission
  Wire.endTransmission();
  //begin transmission to the other slave
   Wire.beginTransmission(4);
  Wire.write(stateNum);
  Wire.endTransmission();
  //display when we have sent the data.
  Serial.println("Sent data to slave(s)");
}
