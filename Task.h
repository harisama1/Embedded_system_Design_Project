#ifndef SIMPLE_PERIODIC_TT_SCHEDULER_H_
#define SIMPLE_PERIODIC_TT_SCHEDULER_H_

#include "main.h"
#include <stdio.h>
#include "MPU6050.h"
#include "SHT3x.h"
#include "LCD_4wire.h"

// --- BIẾN NGOẠI VI ---
extern I2C_HandleTypeDef hi2c1;
extern I2C_HandleTypeDef hi2c2;
extern TIM_HandleTypeDef htim3;
extern UART_HandleTypeDef huart2;

// --- DỮ LIỆU CẢM BIẾN ---
extern MPU6050_RawData_t myRawData;
extern SHT3x_Data_t mySHT_Data;

// --- ĐỊNH NGHĨA BẢNG TÁC VỤ ---
typedef void (*TaskPtr)(void);
extern TaskPtr TaskTable[];
#define M_TASKS 3

// --- PROTOTYPES ---
void Task_MPU_Read(void);
void Task_SHT_Read(void);
void Task_LCD_Display(void);
void System_Sleep(void);

#endif
