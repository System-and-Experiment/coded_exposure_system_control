/*
 * Info: Arduino code for synchronizing the camera with the external shutter
 * Last Modified: 2023-05-29 22:07:53
 * Modified By: Zhihong Zhang <z_zhi_hong@163.com>
 */

// Arduino pin definition
const int ledPin  =  LED_BUILTIN; // the number of the LED pin
const int shutter =  A0; // the number of the shutter pin
const int camera =  A1; // the number of the camera pin

// coded exposure config: debug test
// const long seq[] = {1,0,1,0}; 
// const long seqlen = 4; 
// const long exposure_time = 1000;           // exposure time of camera to capture one image 
// const long exposure_delay = 1;            // delay time to make sure the hardware are prepared for next capture

// coded exposure config: cebd
const long seq[] = {1, 1, 1, 0, 0, 1, 0, 1}; 
const long seqlen = 8;
const long exposure_time = 200;           // exposure time of the camera to capture one image (200ms/exposure, 25ms/encoding-bit)
const long exposure_delay = 1;            // delay time (1ms) to make sure the hardware are prepared for next capture

// variable init
int ledState = LOW;             // ledState used to set the LED
unsigned long previousMillis = 0;        // will store last time LED was updated

// pin init
void setup() {
  // set the digital pin as output:
  pinMode(ledPin, OUTPUT);
  pinMode(shutter, OUTPUT);
  pinMode(camera, OUTPUT);
}

// run
void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= exposure_time) {
    digitalWrite(camera, 1);
    
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    for (int i=0;i<seqlen;i++){
      ledState = seq[i];
      // set the LED with the ledState of the variable:
      digitalWrite(ledPin, ledState);
      digitalWrite(shutter, !ledState);
      delay(exposure_time / seqlen);
    }
    digitalWrite(camera, 0);
    delay(1); // delay 1ms after one exposure to reset camera for the next exposure
  }
}
