/* 
==About==
An Arduino Intergrated Development PLatform (IDE) sketch for recieveing 
gyroscopic and accelerometer values from the MPU-6050

==Hardware==
An Arduino board or compatible microcontroller capable of I2C interface
MPU-6050 (a 6-axis micro-electromechanical system) Intergrated Circuit board. 

==Software==
The open-source libaries utilised are licensed under Arduino,
with extensive explanation of all libaries included on Arduino.cc
Wire.h libary
*/


#include <Wire.h> // A libary for I2C communications.

long xAcceleration, yAcceleration, zAcceleration, xGyroscope, yGyroscope, zGyroscope;
float xForce, yForce, zForce, xRotation, yRotation, zRotation, xPrevRotation, yPrevRotation, zPrevRotation;
float AccelSensitivity = 16384.0, GyroSensitivity = 131.00;
unsigned long Time; 
void setup() {
  /*
  Commence serial communications of the Arduino Circuit.
  The value implaced within the parenthesis should be the baud rate capable of the intergrated circuit.
  */
  SerialBegin(9600); 
  // Begin I2C wire communications.
  WireBegin();
  // Initilaise the MPU-6050 through using I2C wire functions, to establish the gyroscope and accelerometer.
  StartMPU();

}

void loop() {
  Time = millis() / 1000;
  ProcessAccelerationData();
  ProcessGyroscopeData();
  PrintMPU();
}

void SerialBegin(int BaudRate) {
  Serial.begin(BaudRate);
  while(!Serial) {
    delay(1); //Wait for Serial communications to commence
  }
  Serial.println(F("Serial Communications have begun"));
}

void WireBegin() {
    Wire.begin();
    Serial.println(F("Initiated and established Wire libary for I2C communications"));
}

void StartMPU() {
  Wire.beginTransmission(0b1101000);
  Wire.write(0x6B);
  Wire.write(0b00000000);
  Wire.endTransmission();
  Wire.beginTransmission(0b1101000);
  Wire.write(0x1B);
  Wire.write(0b00000000);
  Wire.endTransmission();
  Wire.beginTransmission(0b1101000);
  Wire.write(0x1C);
  Wire.write(0b00000000);
  Wire.endTransmission();
}

void ProcessAccelerationData() {
  Wire.beginTransmission(0b1101000); //I2C address of the MPU
  Wire.write(0x3B); //Starting register for Accel Readings
  Wire.endTransmission();
  Wire.requestFrom(0b1101000,6); //Request Accel Registers (3B - 40)
  while(Wire.available() < 6);
  xAcceleration = Wire.read()<<8|Wire.read(); //Store first two bytes into accelX
  yAcceleration = Wire.read()<<8|Wire.read(); //Store middle two bytes into accelY
  zAcceleration = Wire.read()<<8|Wire.read(); //Store last two bytes into accelZ
  xForce = xAcceleration / AccelSensitivity;
  yForce = yAcceleration / AccelSensitivity;
  zForce = zAcceleration / AccelSensitivity;
}

void ProcessGyroscopeData() {
  Wire.beginTransmission(0b1101000); //I2C address of the MPU
  Wire.write(0x43); //Starting register for Gyro Readings
  Wire.endTransmission();
  Wire.requestFrom(0b1101000,6); //Request Gyro Registers (43 - 48)
  while(Wire.available() < 6);
  xGyroscope = Wire.read()<<8|Wire.read(); //Store first two bytes into accelX
  yGyroscope = Wire.read()<<8|Wire.read(); //Store middle two bytes into accelY
  zGyroscope = Wire.read()<<8|Wire.read(); //Store last two bytes into accelZ
  xRotation = round((xGyroscope / GyroSensitivity) / 10) * 10 + xPrevRotation;
  yRotation = round((yGyroscope / GyroSensitivity) / 10) * 10 + yPrevRotation;
  zRotation = round((zGyroscope / GyroSensitivity) / 10) * 10 + zPrevRotation;
  xPrevRotation = xRotation;
  yPrevRotation = xRotation;
  zPrevRotation = xRotation;
}

void PrintMPU() {
  Serial.print("Uptime: ");
  Serial.print(Time);
  Serial.print("s ");
  Serial.print("Gyroscope:");
  Serial.print(" x = ");
  Serial.print(xRotation);
  Serial.print("| y = ");
  Serial.print(yRotation);
  Serial.print("| z = ");
  Serial.print(zRotation);
  Serial.print(" Accelerometer:");
  Serial.print(" x = ");
  Serial.print(xForce);
  Serial.print("| y = ");
  Serial.print(yForce);
  Serial.print("| z = ");
  Serial.println(zForce);
}
