/*
 * MPU6050.c
 *
 *  Created on: Dec 30, 2025
 *      Author: Harisama1
 */

#include "MPU6050.h"


uint8_t dev_addr;
static uint16_t MPU_ADDR(I2C_HandleTypeDef *hi2c)
{
    uint8_t id = 0;
    // Thử với địa chỉ 0x68
    if (HAL_I2C_Mem_Read(hi2c, MPU6050_ADDR << 1, WHO_AM_I, 1, &id, 1, 100) == HAL_OK && id == 0x68) {
        return MPU6050_ADDR << 1;
    }
    // Nếu không được, thử với địa chỉ 0x69
    if (HAL_I2C_Mem_Read(hi2c, MPU6050_ADDR2 << 1, WHO_AM_I, 1, &id, 1, 100) == HAL_OK && id == 0x68) {
        return MPU6050_ADDR2 << 1;
    }
    return 0; // Không tìm thấy
}

/**
 * @brief Khởi tạo MPU6050 với các tham số cấu hình tùy chọn
 * @param hi2c: Con trỏ tới bộ ngoại vi I2C
 * @param config: Cấu trúc chứa các thiết lập (Dải đo, Bộ lọc,...)
 * @retval 1 nếu thành công, 0 nếu thất bại
 */
uint8_t MPU6050_Init(I2C_HandleTypeDef *hi2c, MPU6050_Config_t *config) {
    uint8_t data;
    dev_addr = MPU_ADDR(hi2c);

    // Kiểm tra xem có tìm thấy cảm biến không
    if (dev_addr == 0) return 0;

    // 1. Reset thiết bị (DEVICE_RESET bit 7)
    data = 0x80;
    HAL_I2C_Mem_Write(hi2c, dev_addr, PWR_MGMT_1, I2C_MEMADD_SIZE_8BIT, &data, 1, 100);
    HAL_Delay(100); // Đợi MPU6050 khởi động lại hoàn toàn

    // 2. Đánh thức thiết bị & Chọn nguồn xung nhịp (CLKSEL bit 2:0)
    // Chọn Internal 8MHz làm Clock Source (0x00)
    data = 0x00;
    HAL_I2C_Mem_Write(hi2c, dev_addr, PWR_MGMT_1, I2C_MEMADD_SIZE_8BIT, &data, 1, 100);

    // 3. Cấu hình Tốc độ lấy mẫu (Sample Rate Divider)
    HAL_I2C_Mem_Write(hi2c, dev_addr, SMPLRT_DIV, I2C_MEMADD_SIZE_8BIT, &config->SampleRate_Div, 1, 100);

    // 4. Cấu hình Bộ lọc số (DLPF)
    HAL_I2C_Mem_Write(hi2c, dev_addr, CONFIG, I2C_MEMADD_SIZE_8BIT, (uint8_t*)&config->DLPF_Config, 1, 100);

    // 5. Cấu hình Dải đo Gyroscope
    HAL_I2C_Mem_Write(hi2c, dev_addr, GYRO_CONFIG, I2C_MEMADD_SIZE_8BIT, (uint8_t*)&config->Gyro_FullScale, 1, 100);

    // 6. Cấu hình Dải đo Accelerometer
    HAL_I2C_Mem_Write(hi2c, dev_addr, ACCEL_CONFIG, I2C_MEMADD_SIZE_8BIT, (uint8_t*)&config->Accel_FullScale, 1, 100);

    return 1; // Khởi tạo thành công
}

void MPU_6050_RAW_data(I2C_HandleTypeDef *hi2c, MPU6050_RawData_t *Data)
{
	uint8_t buffer[14];

	HAL_I2C_Mem_Read(hi2c, dev_addr, ACCEL_XOUT_H, I2C_MEMADD_SIZE_8BIT, buffer, 14, 100);

	Data->Accel_X_RAW = (int16_t)(buffer[0] << 8 | buffer[1]);
	Data->Accel_Y_RAW = (int16_t)(buffer[2] << 8 | buffer[3]);
	Data->Accel_Z_RAW = (int16_t)(buffer[4] << 8 | buffer[5]);
	Data->Temp_RAW    = (int16_t)(buffer[6] << 8 | buffer[7]);
	Data->Gyro_X_RAW  = (int16_t)(buffer[8] << 8 | buffer[9]);
	Data->Gyro_Y_RAW  = (int16_t)(buffer[10] << 8 | buffer[11]);
	Data->Gyro_Z_RAW  = (int16_t)(buffer[12] << 8 | buffer[13]);
}

static const float Accel_Table[] = {16384.0f, 8192.0f, 4096.0f, 2048.0f};
static const float Gyro_Table[]  = {131.0f, 65.5f, 32.8f, 16.4f};

#define AZ_offset 0.22

void MPU6050_Get_Scaled_Data(MPU6050_Config_t *config, MPU6050_RawData_t *raw, MPU6050_ScaledData_t *scaled) {
    // Lấy index từ cấu hình bằng cách dịch phải 3 bit (chia 8)
    uint8_t a_idx = config->Accel_FullScale >> 3;
    uint8_t g_idx = config->Gyro_FullScale >> 3;

    // Tính toán Gia tốc
    scaled->Ax = (float)raw->Accel_X_RAW / Accel_Table[a_idx] - 0;
    scaled->Ay = (float)raw->Accel_Y_RAW / Accel_Table[a_idx] - 0.02;
    scaled->Az = (float)raw->Accel_Z_RAW / Accel_Table[a_idx] - 0.22;

    // Tính toán Gyro
    scaled->Gx = (float)raw->Gyro_X_RAW / Gyro_Table[g_idx];
    scaled->Gy = (float)raw->Gyro_Y_RAW / Gyro_Table[g_idx];
    scaled->Gz = (float)raw->Gyro_Z_RAW / Gyro_Table[g_idx];

    // Tính toán Nhiệt độ
    scaled->Temperature = ((float)raw->Temp_RAW / 340.0f) + 36.53f;
}

