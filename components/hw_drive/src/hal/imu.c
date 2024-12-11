//
// Created by Thomas Abplanalp on 26.11.24.
//
#include <robokit_log.h>
#include <tgmath.h>

#include "mpu6050.h"
#include "../../../../../esp-idf/components/esp_timer/include/esp_timer.h"


static S_Gyro standard_gyro;
static S_Gyro standard_accel;
static uint8_t is_initialized = 0;
static uint64_t last_accel_timestamp = 0;
static int32_t position_fz = 0;

void _robokit_retrieve_defaults(void) {
	int32_t gyro_x=0, gyro_y=0, gyro_z=0;

	ROBOKIT_LOGI("ROBOKIT_RETRIEVE_DEFAULTS");
	for(int e=0;e<200;e++) {
		S_Gyro data = mpu6050_get_acceleration();
		gyro_x += data.X;
		gyro_y += data.Y;
		gyro_z += data.Z;
	}
	is_initialized = 1;

	standard_accel = (S_Gyro){
		.X = gyro_x/200,
		.Y = gyro_y/200,
		.Z = gyro_z/200,
	};
	last_accel_timestamp = esp_timer_get_time()/1000;
}

void imu_init(void) {
	ROBOKIT_LOGI("imu_init()");
	mpu6050_set_power_management( E_MPU6050_POWER_MGMT_1 );
	_robokit_retrieve_defaults();
}

static void _robokit_convert(S_Gyro *gyro, const S_Gyro *adjust) {
	gyro->X -= adjust->X;
	gyro->Y -= adjust->Y;
	gyro->Z -= adjust->Z;
}

void imu_update(void) {
	if(!is_initialized)
		return;

	static uint8_t counter = 0;
	static int32_t last_position = 0;

	if(++counter > 5) {
		counter = 0;
		S_Gyro angle = mpu6050_get_acceleration();
		_robokit_convert(&angle, &standard_accel);

		uint64_t timestamp = esp_timer_get_time()/1000;
		int32_t timestamp_delta = timestamp - last_accel_timestamp;
		last_accel_timestamp = timestamp;

		last_position += timestamp_delta * timestamp_delta / 2 * angle.Z /1000000;

		printf("Position: %ld\n", last_position);
	}
}

void imu_upgrade_drive(void) {

}

void imu_clear(void) {

}