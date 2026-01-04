/*
 * MPU6050.h
 *
 *  Created on: Dec 30, 2025
 *      Author: Harisama1
 */

#ifndef INC_MPU6050_H_
#define INC_MPU6050_H_

#include "stm32f4xx_hal.h"

#define MPU6050_ADDR 	0x68
#define MPU6050_ADDR2	0x69



typedef enum {
    /* Self Test Registers */
    SELF_TEST_X          = 13,
    SELF_TEST_Y          = 14,
    SELF_TEST_Z          = 15,
    SELF_TEST_A          = 16,

    /* Configuration Registers */
    SMPLRT_DIV           = 25,
    CONFIG               = 26,
    GYRO_CONFIG          = 27,
    ACCEL_CONFIG         = 28,
    FF_THR               = 29,
    FF_DUR               = 30,
    MOT_THR              = 31,
    MOT_DUR              = 32,
    ZRMOT_THR            = 33,
    ZRMOT_DUR            = 34,
    FIFO_EN              = 35,

    /* I2C Master Control */
    I2C_MST_CTRL         = 36,
    I2C_SLV0_ADDR        = 37,
    I2C_SLV0_REG         = 38,
    I2C_SLV0_CTRL        = 39,
    I2C_SLV1_ADDR        = 40,
    I2C_SLV1_REG         = 41,
    I2C_SLV1_CTRL        = 42,
    I2C_SLV2_ADDR        = 43,
    I2C_SLV2_REG         = 44,
    I2C_SLV2_CTRL        = 45,
    I2C_SLV3_ADDR        = 46,
    I2C_SLV3_REG         = 47,
    I2C_SLV3_CTRL        = 48,
    I2C_SLV4_ADDR        = 49,
    I2C_SLV4_REG         = 50,
    I2C_SLV4_DO          = 51,
    I2C_SLV4_CTRL        = 52,
    I2C_SLV4_DI          = 53,
    I2C_MST_STATUS       = 54,

    /* Interrupt & External Sensor Data */
    INT_PIN_CFG          = 55,
    INT_ENABLE           = 56,
    INT_STATUS           = 58,

    /* Accelerometer Data */
    ACCEL_XOUT_H         = 59,
    ACCEL_XOUT_L         = 60,
    ACCEL_YOUT_H         = 61,
    ACCEL_YOUT_L         = 62,
    ACCEL_ZOUT_H         = 63,
    ACCEL_ZOUT_L         = 64,

    /* Temperature Data */
    TEMP_OUT_H           = 65,
    TEMP_OUT_L           = 66,

    /* Gyroscope Data */
    GYRO_XOUT_H          = 67,
    GYRO_XOUT_L          = 68,
    GYRO_YOUT_H          = 69,
    GYRO_YOUT_L          = 70,
    GYRO_ZOUT_H          = 71,
    GYRO_ZOUT_L          = 72,

    /* External Sensor Data (Gồm 24 thanh ghi liên tiếp) */
    EXT_SENS_DATA_00     = 73,  EXT_SENS_DATA_01     = 74,  EXT_SENS_DATA_02     = 75,
    EXT_SENS_DATA_03     = 76,  EXT_SENS_DATA_04     = 77,  EXT_SENS_DATA_05     = 78,
    EXT_SENS_DATA_06     = 79,  EXT_SENS_DATA_07     = 80,  EXT_SENS_DATA_08     = 81,
    EXT_SENS_DATA_09     = 82,  EXT_SENS_DATA_10     = 83,  EXT_SENS_DATA_11     = 84,
    EXT_SENS_DATA_12     = 85,  EXT_SENS_DATA_13     = 86,  EXT_SENS_DATA_14     = 87,
    EXT_SENS_DATA_15     = 88,  EXT_SENS_DATA_16     = 89,  EXT_SENS_DATA_17     = 90,
    EXT_SENS_DATA_18     = 91,  EXT_SENS_DATA_19     = 92,  EXT_SENS_DATA_20     = 93,
    EXT_SENS_DATA_21     = 94,  EXT_SENS_DATA_22     = 95,  EXT_SENS_DATA_23     = 96,

    /* I2C Slave Data Out */
    I2C_SLV0_DO          = 99,
    I2C_SLV1_DO          = 100,
    I2C_SLV2_DO          = 101,
    I2C_SLV3_DO          = 102,

    /* I2C Master Delay Control */
    I2C_MST_DELAY_CTRL   = 103,
    SIGNAL_PATH_RESET    = 104,
    MOT_DETECT_CTRL      = 105,
    USER_CTRL            = 106,

    /* Power Management */
    PWR_MGMT_1           = 107,
    PWR_MGMT_2           = 108,

    /* FIFO Count & Read/Write */
    FIFO_COUNTH          = 114,
    FIFO_COUNTL          = 115,
    FIFO_R_W             = 116,

    /* Device ID */
    WHO_AM_I             = 117
} MPU_add_register;

typedef struct {
    int16_t Accel_X_RAW;
    int16_t Accel_Y_RAW;
    int16_t Accel_Z_RAW;
    int16_t Temp_RAW;
    int16_t Gyro_X_RAW;
    int16_t Gyro_Y_RAW;
    int16_t Gyro_Z_RAW;
} MPU6050_RawData_t;

/**
 * @brief  Cấu hình dải đo Gia tốc (Accelerometer Full Scale)
 * Bit [4:3] của thanh ghi ACCEL_CONFIG (0x1C)
 */
typedef enum {
    ACCEL_FS_2G  = 0x00,  // Sai số thấp nhất, dùng cho đo độ nghiêng chính xác
    ACCEL_FS_4G  = 0x08,
    ACCEL_FS_8G  = 0x10,
    ACCEL_FS_16G = 0x18   // Dải đo lớn nhất, dùng cho va chạm mạnh
} MPU6050_Accel_FS_t;

/**
 * @brief  Cấu hình dải đo Con quay hồi chuyển (Gyro Full Scale)
 * Bit [4:3] của thanh ghi GYRO_CONFIG (0x1B)
 */
typedef enum {
    GYRO_FS_250  = 0x00,  // Tối đa xoay 250 độ/giây (Nhạy nhất)
    GYRO_FS_500  = 0x08,
    GYRO_FS_1000 = 0x10,
    GYRO_FS_2000 = 0x18   // Xoay cực nhanh không bị kịch kim
} MPU6050_Gyro_FS_t;


/**
 * @brief  Cấu hình bộ lọc số (Digital Low Pass Filter)
 * Bit [2:0] của thanh ghi CONFIG (0x1A)
 * Giúp triệt tiêu rung động cơ khí (nhiễu)
 */
typedef enum {
    DLPF_260HZ = 0x00,    // Không lọc (Nhiễu nhiều, độ trễ thấp)
    DLPF_184HZ = 0x01,
    DLPF_94HZ  = 0x02,
    DLPF_44HZ  = 0x03,    // Giá trị khuyên dùng cho cân bằng robot
    DLPF_21HZ  = 0x04,
    DLPF_10HZ  = 0x05,
    DLPF_5HZ   = 0x06     // Lọc rất mạnh (Dữ liệu cực mượt nhưng trễ)
} MPU6050_DLPF_t;

typedef struct {
    MPU6050_Accel_FS_t Accel_FullScale;
    MPU6050_Gyro_FS_t  Gyro_FullScale;
    MPU6050_DLPF_t     DLPF_Config;
    uint8_t            SampleRate_Div; // Tốc độ lấy mẫu = 8kHz / (1 + Div)
} MPU6050_Config_t;

/**
 * @brief Cấu trúc lưu trữ dữ liệu đã được quy đổi sang đơn vị vật lý
 */
typedef struct {
    float Ax;          // Gia tốc trục X (đơn vị: g)
    float Ay;          // Gia tốc trục Y (đơn vị: g)
    float Az;          // Gia tốc trục Z (đơn vị: g)

    float Gx;          // Vận tốc góc trục X (đơn vị: deg/s)
    float Gy;          // Vận tốc góc trục Y (đơn vị: deg/s)
    float Gz;          // Vận tốc góc trục Z (đơn vị: deg/s)

    float Temperature; // Nhiệt độ (đơn vị: Celsius)
} MPU6050_ScaledData_t;

uint8_t MPU6050_Init(I2C_HandleTypeDef *hi2c, MPU6050_Config_t *config);
void MPU_6050_RAW_data(I2C_HandleTypeDef *hi2c, MPU6050_RawData_t *Data);
void MPU6050_Get_Scaled_Data(MPU6050_Config_t *config, MPU6050_RawData_t *raw, MPU6050_ScaledData_t *scaled);


#endif /* INC_MPU6050_H_ */
