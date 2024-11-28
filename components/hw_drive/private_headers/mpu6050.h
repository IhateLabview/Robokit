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

S_Gyro mpu6050_get_acceleration(void);
S_Gyro mpu6050_get_position(void);

#endif //MPU6050_H
