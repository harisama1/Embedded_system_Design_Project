/*
 * LCD_4wire.h
 *
 *  Created on: Jan 3, 2026
 *      Author: Harisam1
 */

#ifndef INC_LCD_4WIRE_H_
#define INC_LCD_4WIRE_H_

#include "stm32f4xx_hal.h"

#define LCD_PORT GPIOC
#define RS_PIN   GPIO_PIN_0
#define EN_PIN   GPIO_PIN_1
#define D4_PIN   GPIO_PIN_2
#define D5_PIN   GPIO_PIN_3
#define D6_PIN   GPIO_PIN_4
#define D7_PIN   GPIO_PIN_5

/* Hàm tạo xung chốt dữ liệu */
void LCD_Enable(void);

/* Hàm gửi 4 bit dữ liệu */
void LCD_Send_Nibble(uint8_t data);

/* Hàm gửi lệnh */
void LCD_Send_Cmd(uint8_t cmd);

/* Hàm gửi 1 ký tự dữ liệu */
void LCD_Send_Data(uint8_t data);

/* Khởi tạo LCD chế độ 4-bit */
void LCD_Init(void);

/* In chuỗi ký tự */
void LCD_Print(char *str);

/* Đặt con trỏ (row 0-1, col 0-15) */
void LCD_SetCursor(uint8_t row, uint8_t col);

#endif /* INC_LCD_4WIRE_H_ */
