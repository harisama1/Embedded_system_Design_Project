#include "Simple_Periodic_TT_Scheduler.h"

/* --- TASK 0: ĐỌC MPU6050 --- */
void Task_MPU_Read(void) {
    // Sử dụng driver: MPU_6050_RAW_data(hi2c, Data)
    MPU_6050_RAW_data(&hi2c1, &myRawData);
}

/* --- TASK 1: ĐỌC SHT3x --- */
void Task_SHT_Read(void) {
    // Sử dụng driver: SHT3x_Read(hi2c, data)
    SHT3x_Read(&hi2c2, &mySHT_Data);
}

/* --- TASK 2: HIỂN THỊ LCD 4-WIRE --- */
void Task_LCD_Display(void) {
    char line1[20], line2[20];

    // 1. Chuẩn bị dữ liệu chuỗi
    sprintf(line1, "T:%.1fC H:%.1f%%", mySHT_Data.Temperature, mySHT_Data.Humidity);
    sprintf(line2, "Ax:%d Ay:%d", myRawData.Accel_X_RAW, myRawData.Accel_Y_RAW);

    // 2. Sử dụng driver LCD_4wire của bạn
    LCD_SetCursor(0, 0);
    LCD_Print(line1);
    LCD_SetCursor(1, 0);
    LCD_Print(line2);
}
