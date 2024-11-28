//
// Created by Thomas Abplanalp on 28.11.24.
//

#include "mpu6050.h"

#include <driver/i2c.h>

void mpu6050_init() {
	i2c_config_t conf = {
		.mode = I2C_MODE_MASTER,
		.sda_io_num = 6,
		.sda_pullup_en = GPIO_PULLUP_DISABLE,
		.scl_io_num = 7,
		.scl_pullup_en = GPIO_PULLUP_DISABLE,
		.master.clk_speed = 400000,
		.clk_flags = 0
	};
	i2c_param_config(I2C_NUM_0, &conf);
	i2c_driver_install(I2C_NUM_0, I2C_MODE_MASTER, 0, 0, 0);
}


static esp_err_t i2c_write_register_byte(uint8_t reg_addr, uint8_t data) {
	i2c_cmd_handle_t cmd = i2c_cmd_link_create();
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, (MPU6050_ADDR << 1) | I2C_MASTER_WRITE, true);
	i2c_master_write_byte(cmd, reg_addr, true);
	i2c_master_write_byte(cmd, data, true);
	i2c_master_stop(cmd);
	esp_err_t ret = i2c_master_cmd_begin(I2C_NUM_0, cmd, pdMS_TO_TICKS(1000));
	i2c_cmd_link_delete(cmd);
	return ret;
}

static esp_err_t i2c_read_register_byte(uint8_t reg, uint8_t *data) {
	i2c_cmd_handle_t cmd = i2c_cmd_link_create();
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, (MPU6050_ADDR << 1) | I2C_MASTER_WRITE, true);
	i2c_master_write_byte(cmd, reg, true);
	i2c_master_stop(cmd);
	esp_err_t ret = i2c_master_cmd_begin(I2C_NUM_0, cmd, pdMS_TO_TICKS(1000));
	i2c_cmd_link_delete(cmd);

	if (ret != ESP_OK) {
		return ret;
	}

	cmd = i2c_cmd_link_create();
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, (MPU6050_ADDR << 1) | I2C_MASTER_READ, true);
	i2c_master_read_byte(cmd, data, I2C_MASTER_NACK);
	i2c_master_stop(cmd);
	ret = i2c_master_cmd_begin(I2C_NUM_0, cmd, pdMS_TO_TICKS(1000));
	i2c_cmd_link_delete(cmd);
	return ret;
}

static int16_t _make_2c(uint16_t zahl) {
	if(zahl > 0x8000) {
		return -((65535 - zahl) + 1);
	}
	return zahl;
}

static esp_err_t i2c_read_register_word(uint8_t reg, uint16_t *data) {
	uint8_t dd = 0;
	esp_err_t ret = i2c_read_register_byte(reg, &dd);
	if(ret == ESP_OK) {
		*data = dd<<8;
		ret = i2c_read_register_byte(reg+1, &dd);
		*data |= dd;
		return ret;
	}

	return ret;
}



S_Gyro mpu6050_get_acceleration(void) {
	S_Gyro accel = {0};

	uint16_t data = 0;
	i2c_read_register_word(0x3b, &data);
	accel.X = _make_2c(data);

	i2c_read_register_word(0x3d, &data);
	accel.Y = _make_2c(data);

	i2c_read_register_word(0x3f, &data);
	accel.Z = _make_2c(data);

	return accel;
}

S_Gyro mpu6050_get_position(void) {
	S_Gyro position = {0};

	uint16_t data = 0;
	i2c_read_register_word(0x43, &data);
	position.X = _make_2c(data);

	i2c_read_register_word(0x45, &data);
	position.Y = _make_2c(data);

	i2c_read_register_word(0x47, &data);
	position.Z = _make_2c(data);

	return position;
}