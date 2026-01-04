/*
 * SHT3x.c
 *
 *  Created on: Jan 4, 2026
 *      Author: Harisama1
 */

#include "SHT3x.h"

HAL_StatusTypeDef SHT3x_Init(I2C_HandleTypeDef *hi2c) {
    uint8_t cmd[2] = {0x30, 0xA2}; // Soft Reset
    return HAL_I2C_Master_Transmit(hi2c, SHT30_I2C_ADDR, cmd, 2, 100);
}

HAL_StatusTypeDef SHT3x_Read(I2C_HandleTypeDef *hi2c, SHT3x_Data_t *data) {
    uint8_t cmd[2] = {0x24, 0x00}; // Lệnh đo Single Shot
    uint8_t buf[6];

    // 1. Gửi lệnh đo
    HAL_I2C_Master_Transmit(hi2c, SHT30_I2C_ADDR, cmd, 2, 100);

    // 2. Đợi đo xong (High repeatability cần tối đa 15ms)
    HAL_Delay(20);

    // 3. Đọc 6 byte (Temp_H, Temp_L, Temp_CRC, Humi_H, Humi_L, Humi_CRC)
    if (HAL_I2C_Master_Receive(hi2c, SHT30_I2C_ADDR, buf, 6, 100) != HAL_OK) {
        return HAL_ERROR;
    }

    // 4. Chuyển đổi dữ liệu (Công thức chuẩn Datasheet)
    uint16_t rawTemp = (buf[0] << 8) | buf[1];
    uint16_t rawHumi = (buf[3] << 8) | buf[4];

    data->Temperature = -45.0f + 175.0f * ((float)rawTemp / 65535.0f);
    data->Humidity = 100.0f * ((float)rawHumi / 65535.0f);

    return HAL_OK;
}

