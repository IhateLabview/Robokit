//
// Created by Thomas Abplanalp on 26.11.24.
//
#include <Wire.h>
#include "mpu6050.h"

const int MPU = 0x68; // MPU6050 I2C address
float AccX, AccY, AccZ; //linear acceleration
float GyroX, GyroY, GyroZ; //angular velocity
float accAngleX, accAngleY, gyroAngleX, gyroAngleY, gyroAngleZ; //used in void loop()
float roll, pitch, yaw;
float AccErrorX, AccErrorY, GyroErrorX, GyroErrorY, GyroErrorZ;
float elapsedTime, currentTime, previousTime;
int c = 0;

void imu_init(void) {


void imu_update(void) {


void imu_upgrade_drive(void) {

}

void imu_clear(void) {

}



  // S_Gyro myAcc = mpu6050_get_acceleration();

void loop() {
  // === Read accelerometer (on the MPU6050) data === //
  readAcceleration();


  // === Read gyroscope (on the MPU6050) data === //
  previousTime = currentTime;
  currentTime = micros();
  elapsedTime = (currentTime - previousTime) / 1000000; // Divide by 1000 to get seconds
  readGyro();
  // Correct the outputs with the calculated error values
  GyroX -= GyroErrorX; //GyroErrorX is calculated in the calculateError() function
  GyroY -= GyroErrorY;
  GyroZ -= GyroErrorZ;
  // Currently the raw values are in degrees per seconds, deg/s, so we need to multiply by sendonds (s) to get the angle in degrees
  gyroAngleX += GyroX * elapsedTime; // deg/s * s = deg
  gyroAngleY += GyroY * elapsedTime;
  yaw += GyroZ * elapsedTime;
  //combine accelerometer- and gyro-estimated angle values. 0.96 and 0.04 values are determined through trial and error by other people
  roll = 0.96 * gyroAngleX + 0.04 * accAngleX;
  pitch = 0.96 * gyroAngleY + 0.04 * accAngleY;
  angle = roll; //if you mounted MPU6050 in a different orientation to me, angle may not = roll. It can roll, pitch, yaw or minus version of the three
  //for me, turning right reduces angle. Turning left increases angle.

  // Print the values on the serial monitor
  if(Serial.available()){
    char c = Serial.read ();
    if (c == 'w') { //drive forward
      Serial.println("forward");
      isDriving = true;
    } else if (c == 'a') { //turn left
      Serial.println("left");
      targetAngle += 90;
      if (targetAngle > 180){
        targetAngle -= 360;
      }
      isDriving = false;
    } else if (c == 'd') { //turn right
      Serial.println("right");
      targetAngle -= 90;
      if (targetAngle <= -180){
        targetAngle += 360;
      }
      isDriving = false;
    } else if (c == 'q') { //stop or brake
      Serial.println("stop");
      isDriving = false;
    } else if (c == 'i') { //print information. When car is stationary, GyroX should approx. = 0.
      Serial.print("angle: ");
      Serial.println(angle);
      Serial.print("targetAngle: ");
      Serial.println(targetAngle);
      Serial.print("GyroX: ");
      Serial.println(GyroX);
      Serial.print("elapsedTime (in ms): "); //estimates time to run void loop() once
      Serial.println(elapsedTime * pow(10, 3));
      Serial.print("equilibriumSpeed: ");
      Serial.println(equilibriumSpeed);
    } else if (c == 'p') { //pause the program
      paused = !paused;
      stopCar();
      isDriving = false;
      Serial.println("key p was pressed, which pauses/unpauses the program");
    }
  }

  static int count;
  static int countStraight;
  if (count < 6){
    count ++;
  } else { //runs once after void loop() runs 7 times. void loop runs about every 2.8ms, so this else condition runs every 19.6ms or 50 times/second
    count = 0;
    if (!paused){
      if (isDriving != prevIsDriving){
          leftSpeedVal = equilibriumSpeed;
          countStraight = 0;
          Serial.print("mode changed, isDriving: ");
          Serial.println(isDriving);
      }
      if (isDriving) {
        if (abs(targetAngle - angle) < 3){
          if (countStraight < 20){
            countStraight ++;
          } else {
            countStraight = 0;
            equilibriumSpeed = leftSpeedVal; //to find equilibrium speed, 20 consecutive readings need to indicate car is going straight
            Serial.print("EQUILIBRIUM reached, equilibriumSpeed: ");
            Serial.println(equilibriumSpeed);
          }
        } else {
          countStraight = 0;
        }
        driving();
      } else {
        rotate();
      }
      prevIsDriving = isDriving;
    }
  }
}

void driving (){//called by void loop(), which isDriving = true
  int deltaAngle = round(targetAngle - angle); //rounding is neccessary, since you never get exact values in reality
  forward();
  if (deltaAngle != 0){
    controlSpeed ();
    rightSpeedVal = maxSpeed;
    analogWrite(rightSpeed, rightSpeedVal);
    analogWrite(leftSpeed, leftSpeedVal);
  }
}

void controlSpeed (){//this function is called by driving ()
  int deltaAngle = round(targetAngle - angle);
  int targetGyroX;

  //setting up propoertional control, see Step 3 on the website
  if (deltaAngle > 30){
      targetGyroX = 60;
  } else if (deltaAngle < -30){
    targetGyroX = -60;
  } else {
    targetGyroX = 2 * deltaAngle;
  }

  if (round(targetGyroX - GyroX) == 0){
    ;
  } else if (targetGyroX > GyroX){
    leftSpeedVal = changeSpeed(leftSpeedVal, -1); //would increase GyroX
  } else {
    leftSpeedVal = changeSpeed(leftSpeedVal, +1);
  }
}

void rotate (){//called by void loop(), which isDriving = false
  int deltaAngle = round(targetAngle - angle);
  int targetGyroX;
  if (abs(deltaAngle) <= 1){
    stopCar();
  } else {
    if (angle > targetAngle) { //turn left
      left();
    } else if (angle < targetAngle) {//turn right
      right();
    }

    //setting up propoertional control, see Step 3 on the website
    if (abs(deltaAngle) > 30){
      targetGyroX = 60;
    } else {
      targetGyroX = 2 * abs(deltaAngle);
    }

    if (round(targetGyroX - abs(GyroX)) == 0){
      ;
    } else if (targetGyroX > abs(GyroX)){
      leftSpeedVal = changeSpeed(leftSpeedVal, +1); //would increase abs(GyroX)
    } else {
      leftSpeedVal = changeSpeed(leftSpeedVal, -1);
    }
    rightSpeedVal = leftSpeedVal;
    analogWrite(rightSpeed, rightSpeedVal);
    analogWrite(leftSpeed, leftSpeedVal);
  }
}

int changeSpeed (int motorSpeed, int increment){
  motorSpeed += increment;
  if (motorSpeed > maxSpeed){ //to prevent motorSpeed from exceeding 255, which is a problem when using analogWrite
    motorSpeed = maxSpeed;
  } else if (motorSpeed < minSpeed){
    motorSpeed = minSpeed;
  }
  return motorSpeed;
}

void calculateError() {
  //When this function is called, ensure the car is stationary. See Step 2 for more info


  // Read gyro values 200 times
  while (c < 200) {
    readGyro();
    // Sum all readings
    GyroErrorX += GyroX;
    GyroErrorY += GyroY;
    GyroErrorZ += GyroZ;
    c++;
  }
  //Divide the sum by 200 to get the error value
  GyroErrorX = GyroErrorX / 200;
  GyroErrorY = GyroErrorY / 200;
  GyroErrorZ = GyroErrorZ / 200;
  Serial.println("The the gryoscope setting in MPU6050 has been calibrated");
}

void readAcceleration() {
  Wire.beginTransmission(MPU);
  Wire.write(0x3B); // Start with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 6, true); // Read 6 registers total, each axis value is stored in 2 registers
  //For a range of +-2g, we need to divide the raw values by 16384, according to the MPU6050 datasheet
  AccX = (Wire.read() << 8 | Wire.read()) / 16384.0; // X-axis value
  AccY = (Wire.read() << 8 | Wire.read()) / 16384.0; // Y-axis value
  AccZ = (Wire.read() << 8 | Wire.read()) / 16384.0; // Z-axis value
}

void readGyro() {
  Wire.beginTransmission(MPU);
  Wire.write(0x43);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 6, true);
  GyroX = (Wire.read() << 8 | Wire.read()) / 131.0;
  GyroY = (Wire.read() << 8 | Wire.read()) / 131.0;
  GyroZ = (Wire.read() << 8 | Wire.read()) / 131.0;
}

void stopCar(){
  digitalWrite(right1, LOW);
  digitalWrite(right2, LOW);
  digitalWrite(left1, LOW);
  digitalWrite(left2, LOW);
  analogWrite(rightSpeed, 0);
  analogWrite(leftSpeed, 0);
}

void forward(){ //drives the car forward, assuming leftSpeedVal and rightSpeedVal are set high enough
  digitalWrite(right1, HIGH); //the right motor rotates FORWARDS when right1 is HIGH and right2 is LOW
  digitalWrite(right2, LOW);
  digitalWrite(left1, HIGH);
  digitalWrite(left2, LOW);
}

void left(){ //rotates the car left, assuming speed leftSpeedVal and rightSpeedVal are set high enough
  digitalWrite(right1, LOW);
  digitalWrite(right2, HIGH);
  digitalWrite(left1, HIGH);
  digitalWrite(left2, LOW);
}

void right(){
  digitalWrite(right1, HIGH);
  digitalWrite(right2, LOW);
  digitalWrite(left1, LOW);
  digitalWrite(left2, HIGH);
}
