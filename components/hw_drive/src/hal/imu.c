//
// Created by Thomas Abplanalp on 26.11.24.
//
#include <robokit_log.h>

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
	ROBOKIT_LOGI("imu_init()");
	mpu6050_set_power_management( E_MPU6050_POWER_MGMT_1 );

}

void imu_update(void) {
	static uint8_t counter = 0;
	if(++counter > 5) {
		counter = 0;
		S_Gyro pos = mpu6050_get_position();
		ROBOKIT_LOGI("POS %d %d %d", pos.X, pos.Y, pos.Z);

		pos = mpu6050_get_acceleration();
		ROBOKIT_LOGI("ACC %d %d %d", pos.X, pos.Y, pos.Z);
	}
}

void imu_upgrade_drive(void) {

}

void imu_clear(void) {

}