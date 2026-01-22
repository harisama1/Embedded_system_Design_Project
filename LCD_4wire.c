/*
 * LCD_4wire.c
 *
 *  Created on: Jan 3, 2026
 *      Author: Harisama1
 */

#include "LCD_4wire.h"

#include <stdio.h>
#include <string.h>

/* LCD 16x2 Port C Definitions */
#define LCD_PORT GPIOC
#define RS_PIN   GPIO_PIN_0
#define EN_PIN   GPIO_PIN_1
#define D4_PIN   GPIO_PIN_2
#define D5_PIN   GPIO_PIN_3
#define D6_PIN   GPIO_PIN_4
#define D7_PIN   GPIO_PIN_5

/* Hàm tạo xung chốt dữ liệu */
void LCD_Enable(void) {
    HAL_GPIO_WritePin(LCD_PORT, EN_PIN, GPIO_PIN_SET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(LCD_PORT, EN_PIN, GPIO_PIN_RESET);
    HAL_Delay(1);
}

/* Hàm gửi 4 bit dữ liệu */
void LCD_Send_Nibble(uint8_t data) {
    HAL_GPIO_WritePin(LCD_PORT, D4_PIN, (data >> 0) & 0x01);
    HAL_GPIO_WritePin(LCD_PORT, D5_PIN, (data >> 1) & 0x01);
    HAL_GPIO_WritePin(LCD_PORT, D6_PIN, (data >> 2) & 0x01);
    HAL_GPIO_WritePin(LCD_PORT, D7_PIN, (data >> 3) & 0x01);
    LCD_Enable();
}

/* Hàm gửi lệnh */
void LCD_Send_Cmd(uint8_t cmd) {
    HAL_GPIO_WritePin(LCD_PORT, RS_PIN, GPIO_PIN_RESET); // RS = 0
    LCD_Send_Nibble(cmd >> 4);
    LCD_Send_Nibble(cmd & 0x0F);
}

/* Hàm gửi 1 ký tự dữ liệu */
void LCD_Send_Data(uint8_t data) {
    HAL_GPIO_WritePin(LCD_PORT, RS_PIN, GPIO_PIN_SET);   // RS = 1
    LCD_Send_Nibble(data >> 4);
    LCD_Send_Nibble(data & 0x0F);
}

/* Khởi tạo LCD chế độ 4-bit */
void LCD_Init(void) {
    HAL_Delay(50);
    LCD_Send_Nibble(0x03); HAL_Delay(5);
    LCD_Send_Nibble(0x03); HAL_Delay(1);
    LCD_Send_Nibble(0x03);
    LCD_Send_Nibble(0x02); // Mode 4-bit

    LCD_Send_Cmd(0x28); // 2 lines, 5x8 font
    LCD_Send_Cmd(0x0C); // Display ON, Cursor OFF
    LCD_Send_Cmd(0x06); // Auto increment
    LCD_Send_Cmd(0x01); // Clear screen
    HAL_Delay(2);
}

/* In chuỗi ký tự */
void LCD_Print(char *str) {
    while (*str) LCD_Send_Data(*str++);
}

/* Đặt con trỏ (row 0-1, col 0-15) */
void LCD_SetCursor(uint8_t row, uint8_t col) {
    uint8_t addr = (row == 0) ? (0x80 + col) : (0xC0 + col);
    LCD_Send_Cmd(addr);
}
