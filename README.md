# Coded exposure system control

> Synchronization between an external shutter and a camera with an Arduino board



## Hardware Implementation



#### Prototype system 

![proto](_asset\prototype.png)



#### System connection

![connection](_asset\connect.png)

![6pin-cable](_asset\camera_6pin_cable.png)

#### Component list

- [Thorlabs-Liquid crystal optical shutter - LCC1620A](https://www.thorlabs.com/newgrouppage9.cfm?objectgroup_id=8166)
- JAI-GO-5100C-USB camera
- [YwRobot Arduino nano controller board](https://i-item.jd.com/10035386897105.html#crumb-wrap)



## Camera setting

> Note: (1) The settings can be found in the Guru mode (changed from [Beginner] to [Guru] from above the config window). (2) Whether to enable the auto white balance and gamma transformation depends on the specific application. 

![camera_setting](_asset\camera_setting.png)





## Arduino Software and Code

#### IDE and Driver

>  install the IDE first, then install the driver.

- [Arduino IDE](https://www.arduino.cc/en/software)
- [Arduino_driver](./Arduino_driver_CH341SER)



#### Synchronization code

```C
/*
Arduino board code for synchronization between the camera and the external shutter
function:
	exposure encoding sequence 0: board-A0 high level, shutter open
	exposure encoding sequence 1: board-A0 low level, shutter close
	exposure encoding duration: board-A1 high level
	after exposure encoding: board-A1 low level
	the led will blink according to the encoding sequence for reference
note: 
	after sending the whole encoding sequence, the code will make the shutter `keep the last status (0/1)` for an interval.
	Don't know why, perhaps for system test purpose, e.g., to serve as the start flag to check if the output sequence is true.
*/


// Set a pin number
const int ledPin  =  LED_BUILTIN; // the number of the LED pin
const int shutter =  A0; // the number of the shutter pin
const int camera =  A1; // the number of the camera pin

// ledState used to set the LED
int ledState = LOW;             

// Store the last time LED was updated
// Generally, you should use "unsigned long" for variables that hold time(milliseconds), 
// because the value will quickly become too large for an int to store.
unsigned long previousMillis = 0;        


// Exposure encoding sequence setting
const long interval = 800;  # duration of a single exposure, i.e., the entire encoding sequence duration         
const long seq[] = {0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0}; # encoding sequence
const long seqlen = 32; // length of the encoding sequence


// set the digital pin as output:
void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(shutter, OUTPUT);
  pinMode(camera, OUTPUT);
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // enter the encoding process
    
    // save the last time you blinked the LED
    previousMillis = currentMillis;
	
	// set camera signal to high level
	digitalWrite(camera, 1);

    // exposure encoding
	for (int i=0;i<seqlen;i++){
      ledState = seq[i];
      // set the LED with the ledState of the variable:
      digitalWrite(ledPin, ledState);
      digitalWrite(shutter, !ledState);
      delay(interval / seqlen); // interval / seqlen = exposure time for one bit
    }
	
	// set camera signal to low level
    digitalWrite(camera, 0);
    
	// zzh: why delay after each exposure? For test purpose? (serve as the start flag to check if the output sequence is true?)
	delay(interval);
  }
}

```



