// defines pins numbers
#include <SoftwareSerial.h>
#include "Wire.h"

// I2Cdev and MPU6050 must be installed as libraries, or else the .cpp/.h files
// for both classes must be in the include path of your project
#include "I2Cdev.h"
#include "MPU6050.h"

// class default I2C address is 0x68
// specific I2C addresses may be passed as a parameter here
// AD0 low = 0x68 (default for InvenSense evaluation board)
// AD0 high = 0x69
MPU6050 accelgyro;

int16_t ax, ay, az;
int16_t gx, gy, gz;
SoftwareSerial mySerial(10, 11); // RX, TX
int hPin=9;
int value1;
int myInts[2];
const int trigPin = 3;
const int echoPin = 2;
long duration;
int distance;
int hPin1=6;
int hState1=0;
int value;
int hState=0;
int olds=0;
int od;
int olds1=0;

// accelerometer values
int accel_reading;
int accel_corrected;
int accel_offset = 200;
float accel_angle;
float accel_scale = 1; // set to 0.01
void setup() {
    // join I2C bus (I2Cdev library doesn't do this                                                                                                                      automatically)
Wire.begin();
 		accelgyro.initialize();
  		pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
pinMode(echoPin, INPUT); // Sets the echoPin as an Input

  	// put your setup code here, to run once:

Serial.begin(9600);
  		// put your setup code here, to run once:
pinMode(hPin,INPUT);
pinMode(hPin1,INPUT);

 		mySerial.begin(9600);
  		pinMode(LED_BUILTIN, OUTPUT);
olds=olds1=80;
}

void loop() {
  
digitalWrite(trigPin, LOW);
delayMicroseconds(2);
// Sets the trigPin on HIGH state for 10 micro seconds
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
// Reads the echoPin, returns the sound wave travel time in microseconds
duration = pulseIn(echoPin, HIGH);
// Calculating the distance
distance= duration*0.034/2;
// Prints the distance on the Serial Monitor

accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  		// accelerometer_X_Axis angle calc
  		accel_reading = ax;
  		accel_corrected = accel_reading - accel_offset;
  		accel_corrected = map(accel_corrected, -16800, 16800, -90, 90);
  		accel_corrected = constrain(accel_corrected, -90, 90);
  		accel_angle = (float)(accel_corrected *                     accel_scale);
  


  
 		 // put your main code here, to run repeatedly:
  		hState=digitalRead(hPin);
  		hState1=digitalRead(hPin1);

if(hState != olds || hState1 != olds1){
 
 			if (hState==LOW && hState1==LOW){

value=1;
olds = hState;
olds1 = hState1;
}
if (hState1==HIGH && hState==LOW){

value=2;
olds = hState;
olds1 = hState1; }
if (hState1==LOW && hState==HIGH){

value=3;
olds = hState;
olds1 = hState1;}

if (hState1==HIGH && hState==HIGH){
value=4;
olds = hState;
olds1 = hState1;
}
}


else{


 	 if(accel_angle>=0 && accel_angle<=30)
 	 {
  		value = 5;
  		od=value;
  	 }
  	else if(accel_angle>=30 && accel_angle<70)
  	{
    		value=6;
    		od=value;
  	}
  	else if(accel_angle>=70 && accel_angle<=90)
  	{
    		value=7;
    		od=value;
  	}
  	else{
    		if(od==0){
      			value=5;
    		}
    		else{
    			value=od;}
  	}
}

mySerial.println(value);

Serial.println(value);

delay(1000);



}

