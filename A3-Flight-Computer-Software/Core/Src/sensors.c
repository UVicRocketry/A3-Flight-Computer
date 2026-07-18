#include "sensors.h"


extern I2C_HandleTypeDef hi2c3;

HAL_StatusTypeDef ADXL375_Init(void) {
    uint8_t reg_result;

    reg_result = ADXL375_read_single_byte(ADXL375_DEV_ID_REG);
    
    if(reg_result != ADXL375_DEVICE_ID){
        return HAL_ERROR;
    }


    ADXL375_write_single_byte(ADXL375_INT_ENABLE, 0x00);

    reg_result = ADXL375_read_single_byte(ADXL375_FIFO_CTL);
    reg_result |= 0b10000000;
    ADXL375_write_single_byte(ADXL375_FIFO_CTL, 0);

    ADXL375_write_single_byte(ADXL375_BW_RATE, ADXL375_100_BW);

    reg_result = ADXL375_read_single_byte(ADXL375_DATA_FORMAT);
    reg_result |= 0b10000000;
    ADXL375_write_single_byte(ADXL375_DATA_FORMAT, 0);
    
    reg_result = ADXL375_read_single_byte(ADXL375_PWR_CTL);
    reg_result |= 0b00001000;
    ADXL375_write_single_byte(ADXL375_PWR_CTL, reg_result);

    reg_result = ADXL375_read_single_byte(ADXL375_INT_SOURCE);

    reg_result = ADXL375_read_single_byte(ADXL375_INT_ENABLE);
    reg_result |= 0b10000000;
    ADXL375_write_single_byte(ADXL375_INT_ENABLE, reg_result);

    return HAL_OK;
}

uint8_t ADXL375_read_single_byte(uint8_t reg){
    uint8_t data;
    if(HAL_I2C_Master_Transmit(&hi2c3, ADXL375_WRITE_ADDR, &reg, 1, 1000) != HAL_OK) {
        return HAL_ERROR;
    }

    if(HAL_I2C_Master_Receive(&hi2c3, ADXL375_READ_ADDR, &data, 1, 1000) != HAL_OK) {
        return HAL_ERROR;
    }

    return data;
}

HAL_StatusTypeDef ADXL375_read_mult_byte(uint8_t reg, uint8_t *pdata, uint8_t length) {
    if(HAL_I2C_Master_Transmit(&hi2c3, ADXL375_WRITE_ADDR, &reg, 1, 1000) != HAL_OK) {
        return HAL_ERROR;
    }

    if(HAL_I2C_Master_Receive(&hi2c3, ADXL375_READ_ADDR, &pdata, length, 1000) != HAL_OK) {
        return HAL_ERROR;
    }

    return HAL_OK;
}

HAL_StatusTypeDef ADXL375_write_single_byte(uint8_t reg, uint8_t data) {
    uint8_t pdata[2] = {reg, data};

    if(HAL_I2C_Master_Transmit(&hi2c3, ADXL375_WRITE_ADDR, pdata, 2, 1000) != HAL_OK) {
        return HAL_ERROR;
    }

    return HAL_OK;
}

HAL_StatusTypeDef ADXL375_get_acceleration(AccelData_t *pAccelData) {
    uint8_t xdata[2];
    uint8_t ydata[2];
    uint8_t zdata[2];

    ADXL375_read_mult_byte(ADXL375_DATAX0, xdata, 2);
    ADXL375_read_mult_byte(ADXL375_DATAY0, ydata, 2);
    ADXL375_read_mult_byte(ADXL375_DATAZ0, zdata, 2);

    pAccelData->accel_x_G = (int16_t)(xdata[1] << 8 | xdata[0]);
    pAccelData->accel_y_G = (int16_t)(ydata[1] << 8 | ydata[0]);
    pAccelData->accel_z_G = (int16_t)(zdata[1] << 8 | zdata[0]);

    return HAL_OK;
}