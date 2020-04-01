//slave for just the LEDs
#include <Wire.h>
#include <Adafruit_NeoPixel.h>
// On a Trinket or Gemma we suggest changing this to 1:
#define LED_PIN    6
// How many NeoPixels are attached to the Arduino?
#define LED_COUNT 32
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
int y;
int mode;
int LEDon = 0;

void setup() {
  // put your setup code here, to run once:
Wire.begin(4);
Wire.onReceive(receviedEvent);
Serial.begin(9600);
strip.begin();
strip.show();
}

void loop() {
switch(y){
  case 0:
  Serial.println(y);
  if(LEDon == 1){
    clearLEDs();
    LEDon = 0;
  }
  break;
  case 1:
  Serial.println(y);
   LEDon = 1;
    flashLEDs();
    break;
  case 2:
  Serial.println(y);
    flashLEDs();
    break;
  case 3:
  Serial.println(y);
    flashLEDs();
    break;
}
}
void clearLEDs(){
   for(int l =0;l<=32;l++){
        strip.setPixelColor(l, strip.Color(0,0,0));
      }
      strip.show();
}
void receviedEvent(int howMany){
  int x = Wire.read();
  y = x;
}
void flashLEDs(){
  int z = 0;
  while(z<5){
   for(int j =0;j<=16;j++){
        strip.setPixelColor(j, strip.Color(250,0,0));
      }
      strip.show();
       for(int m =16;m<=32;m++){
        strip.setPixelColor(m, strip.Color(0,0,0));
      }
      strip.show();
      delay(400);
      for(int k =0;k<=16;k++){
        strip.setPixelColor(k, strip.Color(0,0,0));
      }
      strip.show();
       for(int l =16;l<=32;l++){
        strip.setPixelColor(l, strip.Color(250,0,0));
      }
      strip.show();
      delay(400);
    z++;
}
}
