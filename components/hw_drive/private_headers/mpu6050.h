//
// Created by fabia on 11/27/2024.
//

#ifndef MPU6050_H
#define MPU6050_H
#include <stdint.h>

typedef struct {
  int16_t X;
  int16_t Y;
  int16_t Z;
} S_Gyro;

#define USE_BNO055 1

#if USE_BNO055
#define REGISTER_GYRO_X 0x14
#define REGISTER_GYRO_Y 0x16
#define REGISTER_GYRO_Z 0x18

#define REGISTER_ACCEL_X 0x08
#define REGISTER_ACCEL_Y 0x0A
#define REGISTER_ACCEL_Z 0x0C
#else
#define REGISTER_GYRO_X 0x43
#define REGISTER_GYRO_Y 0x45
#define REGISTER_GYRO_Z 0x47

#define REGISTER_ACCEL_X 0x3b
#define REGISTER_ACCEL_Y 0x3d
#define REGISTER_ACCEL_Z 0x3f
#endif

void mpu6050_init();

S_Gyro mpu6050_get_acceleration(void);
S_Gyro mpu6050_get_position(void);

#define MPU6050_ADDR 0x68

#endif //MPU6050_H
