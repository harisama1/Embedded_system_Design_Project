/*
 * SHT3x.h
 *
 *  Created on: Jan 4, 2026
 *      Author: Harisama1
 */

#ifndef INC_SHT3X_H_
#define INC_SHT3X_H_

#include "stm32f4xx_hal.h"
#define SHT30_I2C_ADDR         (0x44 << 1)

// Lệnh đo ở chế độ High Repeatability (Độ chính xác cao nhất)
// MSB = 0x24, LSB = 0x00
#define SHT30_CMD_MEAS_HIGH    0x2400

typedef struct {
    float Temperature;
    float Humidity;
} SHT3x_Data_t;


#endif /* INC_SHT3X_H_ */
