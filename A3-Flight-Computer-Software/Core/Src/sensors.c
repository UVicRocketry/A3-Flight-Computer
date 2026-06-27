#include "sensors.h"
#include "stm32h5xx_hal_def.h"
#include <stdint.h>

extern I2C_HandleTypeDef hi2c3;

HAL_StatusTypeDef ADXL375_Init(void) {
    uint8_t reg_result;

    reg_result = ADXL375_read_single_byte(ADXL375_DEV_ID_REG);
    
    if(reg_result != ADXL375_DEVICE_ID){
        return HAL_ERROR;
    }

    ADXL375_write_single_byte(ADXL375_FIFO_CTL, 0x00);
    ADXL375_write_single_byte(ADXL375_BW_RATE, ADXL375_400_BW);
    reg_result = ADXL375_read_single_byte(ADXL375_DEV_ID_REG);

    reg_result |= 0b00001000;
    ADXL375_write_single_byte(ADXL375_DEV_ID_REG, reg_result);

    return HAL_OK;
}

uint8_t ADXL375_read_single_byte(uint8_t reg){
    uint8_t data;
    if(HAL_I2C_Master_Transmit(&hi2c3, ADXL375_WRITE_ADDR, &reg, 1, 1000) != HAL_OK) {
        return HAL_ERROR;
    }

    if(HAL_I2C_Master_Transmit(&hi2c3, ADXL375_READ_ADDR, &data, 1, 1000) != HAL_OK) {
        return HAL_ERROR;
    }

    return data;
}

HAL_StatusTypeDef ADXL375_read_mult_byte(uint8_t reg, uint8_t *pdata, uint8_t length) {
    if(HAL_I2C_Master_Transmit(&hi2c3, ADXL375_WRITE_ADDR, &reg, 1, 1000) != HAL_OK) {
        return HAL_ERROR;
    }

    if(HAL_I2C_Master_Transmit(&hi2c3, ADXL375_READ_ADDR, &pdata, length, 1000) != HAL_OK) {
        return HAL_ERROR;
    }

    return HAL_OK;
}

HAL_StatusTypeDef ADXL375_write_single_byte(uint8_t reg, uint8_t data) {
    uint8_t pdata[2] = {reg, data};

    if(HAL_I2C_Master_Transmit(&hi2c3, ADXL375_WRITE_ADDR, &pdata, 2, 1000) != HAL_OK) {
        return HAL_ERROR;
    }

    return HAL_OK;
}

HAL_StatusTypeDef ADXL375_get_acceleration(AccelData_t *pAccelData) {
    uint8_t data[6];
    ADXL375_read_mult_byte(ADXL375_DATAX0, data, 6);

    pAccelData->accel_x_G = (int16_t)(data[1] << 8 | data[0]) * 0.049;
    pAccelData->accel_y_G = (int16_t)(data[3] << 8 | data[2]) * 0.049;
    pAccelData->accel_z_G = (int16_t)(data[5] << 8 | data[4]) * 0.049;

    return HAL_OK;
}