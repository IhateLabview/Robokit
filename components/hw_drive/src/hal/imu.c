//
// Created by Thomas Abplanalp on 26.11.24.
//
#include <robokit_log.h>
#include <tgmath.h>

#include "mpu6050.h"
#include "../../../../../esp-idf/components/esp_timer/include/esp_timer.h"

static float current_time, previous_time, elapsed_time;
static float AccErrorX = 0, AccErrorY = 0, GyroErrorX = 0, GyroErrorY = 0, GyroErrorZ = 0;
static float accAngleX, accAngleY, gyroAngleX, gyroAngleY, gyroAngleZ;
static float roll, pitch, yaw;

static void calculateError() {

	for(int e=0;e < 200;e++) {
		S_Gyro acc = mpu6050_get_acceleration();
		AccErrorX += (atan((acc.Y) / sqrt(pow((acc.X), 2) + pow((acc.Z), 2))) * 180.0f / 3.14159f);
		AccErrorY += (atan(-1 * (acc.X) / sqrt(pow((acc.Y), 2) + pow((acc.Z), 2))) * 180.0f / 3.14159f);
	}

	AccErrorX = AccErrorX / 200;
	AccErrorY = AccErrorY / 200;

	for(int e=0;e < 200;e++) {
		S_Gyro acc = mpu6050_get_position();
		GyroErrorX = acc.X;
		GyroErrorY = acc.Y;
		GyroErrorZ = acc.Z;
	}

	GyroErrorX = GyroErrorX / 200;
	GyroErrorY = GyroErrorY / 200;
	GyroErrorZ = GyroErrorZ / 200;
}


void imu_init(void) {
	ROBOKIT_LOGI("imu_init()");
	mpu6050_set_power_management( E_MPU6050_POWER_MGMT_1 );

	calculateError();

	current_time = esp_timer_get_time();
}

void imu_update(void) {
	static uint8_t counter = 0;
	if(++counter > 5) {
		counter = 0;
		S_Gyro acc = mpu6050_get_acceleration();

		accAngleX = (atan(acc.Y / sqrt(pow(acc.X, 2) + pow(acc.Z, 2))) * 180.0f / 3.14159f) - AccErrorX; //AccErrorX is calculated in the calculateError() function
		accAngleY = (atan(-1 * acc.X / sqrt(pow(acc.Y, 2) + pow(acc.Z, 2))) * 180.0f / 3.14159f) - AccErrorY;

		previous_time = current_time;
		current_time = esp_timer_get_time();

		elapsed_time = (current_time - previous_time) / 1000000;
		S_Gyro pos = mpu6050_get_position();
		pos.X -= GyroErrorX; //GyroErrorX is calculated in the calculateError() function
		pos.Y -= GyroErrorY;
		pos.Z -= GyroErrorZ;

		gyroAngleX += pos.X * elapsed_time; // deg/s * s = deg
		gyroAngleY += pos.Y * elapsed_time;
		yaw += pos.Z * elapsed_time;

		roll = 0.96 * gyroAngleX + 0.04 * accAngleX;
		pitch = 0.96 * gyroAngleY + 0.04 * accAngleY;

		ROBOKIT_LOGI("%f %f %f", roll, pitch, yaw);
	}
}

void imu_upgrade_drive(void) {

}

void imu_clear(void) {

}