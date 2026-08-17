#include "sensors.h"

extern I2C_HandleTypeDef hi2c3;

HAL_StatusTypeDef ADXL375_Init(void) {
    uint8_t data;

    data = ADXL375_read_single_byte(ADXL375_DEV_ID_REG);
    
    if(data != ADXL375_DEVICE_ID){
        return HAL_ERROR;
    }

    ADXL375_write_single_byte(ADXL375_INT_ENABLE, 0x00);

    data = ADXL375_read_single_byte(ADXL375_FIFO_CTL);
    data |= 0b10000000;
    ADXL375_write_single_byte(ADXL375_FIFO_CTL, 0);

    ADXL375_write_single_byte(ADXL375_BW_RATE, ADXL375_400_BW);

    data = ADXL375_read_single_byte(ADXL375_DATA_FORMAT);
    data |= 0b10000000;
    ADXL375_write_single_byte(ADXL375_DATA_FORMAT, 0);
    
    data = ADXL375_read_single_byte(ADXL375_PWR_CTL);
    data |= 0b00001000;
    ADXL375_write_single_byte(ADXL375_PWR_CTL, data);

    data = ADXL375_read_single_byte(ADXL375_INT_SOURCE);

    data = ADXL375_read_single_byte(ADXL375_INT_ENABLE);
    data |= 0b10000000;
    ADXL375_write_single_byte(ADXL375_INT_ENABLE, data);

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

    if(HAL_I2C_Master_Receive(&hi2c3, ADXL375_READ_ADDR, pdata, length, 1000) != HAL_OK) {
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
    uint8_t xdata[2] = {0};
    uint8_t ydata[2] = {0};
    uint8_t zdata[2] = {0};


    uint8_t stat = ADXL375_read_single_byte(ADXL375_INT_SOURCE);

    ADXL375_read_mult_byte(ADXL375_DATAX0, xdata, 2);
    ADXL375_read_mult_byte(ADXL375_DATAY0, ydata, 2);
    ADXL375_read_mult_byte(ADXL375_DATAZ0, zdata, 2);

    pAccelData->accel_x_G = (float_t)((int16_t)(xdata[1] << 8 | xdata[0]) * 0.049f);
    pAccelData->accel_y_G = (float_t)((int16_t)(ydata[1] << 8 | ydata[0]) * 0.049f);
    pAccelData->accel_z_G = (float_t)((int16_t)(zdata[1] << 8 | zdata[0]) * 0.049f);

    return HAL_OK;
}

HAL_StatusTypeDef BMP581_Init(void){
    uint8_t data = BMP581_read_single_byte(BMP581_CHIP_ID_REG);
    if(data != BMP581_CHIP_ID){
        return HAL_ERROR;
    }

    data = BMP581_read_single_byte(BMP581_INT_CONFIG);
    data &= ~(BMP581_INT_ENABLE);
    BMP581_write_single_byte(BMP581_INT_CONFIG, data);

    data = BMP581_read_single_byte(BMP581_ODR_CONFIG);
    data &= BMP581_ODR_MASK;
    data |= (BMP581_ODR_30HZ) << 2 | 0x01;
    BMP581_write_single_byte(BMP581_ODR_CONFIG, data);


    data = BMP581_read_single_byte(BMP581_OSR_CONFIG);
    data |= BMP581_PRES_ENABLE | (BMP581_OSR_8X) << 3 | (BMP581_OSR_8X);
    BMP581_write_single_byte(BMP581_OSR_CONFIG, data);

    data = BMP581_read_single_byte(BMP581_OSR_EFF);

    data = BMP581_read_single_byte(BMP581_INT_SOURCE);
    data |= BMP581_INT_DRDY;
    BMP581_write_single_byte(BMP581_INT_SOURCE, data);

    data = BMP581_read_single_byte(BMP581_INT_CONFIG);
    data &= ~(BMP581_INT_PUSH_PULL);
    data |= BMP581_INT_POL_HIGH | BMP581_INT_ENABLE;
    BMP581_write_single_byte(BMP581_INT_CONFIG, data);

    data = BMP581_read_single_byte(BMP581_ODR_CONFIG);
    data |= BMP581_DEEPSTDBY_DIS | BMP581_MODE_NORMAL;
    BMP581_write_single_byte(BMP581_ODR_CONFIG, data);

    data = BMP581_read_single_byte(BMP581_INT_STATUS);
}

uint8_t BMP581_read_single_byte(uint8_t reg){
    uint8_t data;
    if(HAL_I2C_Master_Transmit(&hi2c3, BMP581_WRITE_ADDR, &reg, 1, 1000) != HAL_OK) {
        return HAL_ERROR;
    }

    if(HAL_I2C_Master_Receive(&hi2c3, BMP581_READ_ADDR, &data, 1, 1000) != HAL_OK) {
        return HAL_ERROR;
    }

    return data;
}

HAL_StatusTypeDef BMP581_read_mult_byte(uint8_t reg, uint8_t *pdata, uint8_t length) {
    if(HAL_I2C_Master_Transmit(&hi2c3, BMP581_WRITE_ADDR, &reg, 1, 1000) != HAL_OK) {
        return HAL_ERROR;
    }

    if(HAL_I2C_Master_Receive(&hi2c3, BMP581_READ_ADDR, pdata, length, 1000) != HAL_OK) {
        return HAL_ERROR;
    }

    return HAL_OK;
}


HAL_StatusTypeDef BMP581_write_single_byte(uint8_t reg, uint8_t data) {
    uint8_t pdata[2] = {reg, data};

    if(HAL_I2C_Master_Transmit(&hi2c3, BMP581_WRITE_ADDR, pdata, 2, 1000) != HAL_OK) {
        return HAL_ERROR;
    }

    return HAL_OK;
}

HAL_StatusTypeDef BMP581_get_temperature_pressure(BaroData_t *pbarodata){
    uint8_t data[6];

    BMP581_read_mult_byte(BMP581_TEMP_DATA_0, data, 6);

    pbarodata->temperature = (float_t)(1.52587891e-5 * (int32_t)( data[2] << 16 | data[1] << 8 | data[0]));
    pbarodata->pressure = (float_t)(1.525625e-2 * (int32_t)( data[5] << 16 | data[4] << 8 | data[3]));
    
    return HAL_OK;
}

HAL_StatusTypeDef LSM6DSO32_Init() {
    uint8_t data = LSM6DSO32_read_single_byte(LSM6DSO32_DEV_ID_REG);
    if(data != LSM6DSO32_DEV_ID){
        return HAL_ERROR;
    }

    data = LSM6DSO32_read_single_byte(LSM6DSO32_INT1_CTRL);
    data |= LSM6DSO32_INT_DRDY_A;
    LSM6DSO32_write_single_byte(LSM6DSO32_INT1_CTRL, data);

    data = LSM6DSO32_read_single_byte(LSM6DSO32_INT2_CTRL);
    data |= LSM6DSO32_INT_DRDY_G;
    LSM6DSO32_write_single_byte(LSM6DSO32_INT2_CTRL, data);

    data = LSM6DSO32_read_single_byte(LSM6DSO32_CTRL1_XL);
    data &= LSM6DS032_ODR_MASK;
    data |= LSM6DSO32_ODR_416HZ | LSM6DS032_FULL_SCALE;
    LSM6DSO32_write_single_byte(LSM6DSO32_CTRL1_XL, data);

    data = LSM6DSO32_read_single_byte(LSM6DSO32_CTRL2_G);
    data &= LSM6DS032_ODR_MASK;
    data |= LSM6DSO32_ODR_416HZ | LSM6DS032_FULL_SCALE;
    LSM6DSO32_write_single_byte(LSM6DSO32_CTRL2_G, data);

    data = LSM6DSO32_read_single_byte(LSM6DSO32_TAP_CFG_2);
    data |= LSM6DSO32_INT_EN;
    LSM6DSO32_write_single_byte(LSM6DSO32_TAP_CFG_2, data);

    data = LSM6DSO32_read_single_byte(LSM6DSO32_CTRL4_C);
    data |= 0x08;
    LSM6DSO32_write_single_byte(LSM6DSO32_CTRL4_C, data);

    return HAL_OK;
}

uint8_t LSM6DSO32_read_single_byte(uint8_t reg){
    uint8_t data;
    if(HAL_I2C_Master_Transmit(&hi2c3, LSM6DSO32_WRITE_ADDR, &reg, 1, 1000) != HAL_OK) {
        return HAL_ERROR;
    }

    if(HAL_I2C_Master_Receive(&hi2c3, LSM6DSO32_READ_ADDR, &data, 1, 1000) != HAL_OK) {
        return HAL_ERROR;
    }

    return data;
}

HAL_StatusTypeDef LSM6DSO32_read_mult_byte(uint8_t reg, uint8_t *pdata, uint8_t length) {
    if(HAL_I2C_Master_Transmit(&hi2c3, LSM6DSO32_WRITE_ADDR, &reg, 1, 1000) != HAL_OK) {
        return HAL_ERROR;
    }

    if(HAL_I2C_Master_Receive(&hi2c3, LSM6DSO32_READ_ADDR, pdata, length, 1000) != HAL_OK) {
        return HAL_ERROR;
    }

    return HAL_OK;
}

HAL_StatusTypeDef LSM6DSO32_write_single_byte(uint8_t reg, uint8_t data) {
    uint8_t pdata[2] = {reg, data};

    if(HAL_I2C_Master_Transmit(&hi2c3, LSM6DSO32_WRITE_ADDR, pdata, 2, 1000) != HAL_OK) {
        return HAL_ERROR;
    }

    return HAL_OK;
}

HAL_StatusTypeDef LSM6DSO32_get_acceleration(AccelData_t *pAccelData){
    uint8_t data[6] = {0};

    if(LSM6DSO32_read_mult_byte(LSM6DSO32_OUTX_L_A, data, 6) != HAL_OK){
        return HAL_ERROR;
    }

    pAccelData->accel_x_G = (float_t)((int16_t)(data[1] << 8 | data[0]) * 0.976e-3f);
    pAccelData->accel_y_G = (float_t)((int16_t)(data[3] << 8 | data[2]) * 0.976e-3f);
    pAccelData->accel_z_G = (float_t)((int16_t)(data[5] << 8 | data[4]) * 0.976e-3f);

    return HAL_OK;
}


HAL_StatusTypeDef LSM6DSO32_get_gyro(GyroData_t *pGyroData){
    uint8_t data[6] = {0};

    if(LSM6DSO32_read_mult_byte(LSM6DSO32_OUTX_L_G, data, 6) != HAL_OK){
        return HAL_ERROR;
    }

    pGyroData->pitch_dps = (float_t)((int16_t)(data[1] << 8 | data[0]) * 0.070);
    pGyroData->roll_dps = (float_t)((int16_t)(data[3] << 8 | data[2]) * 0.070);
    pGyroData->yaw_dps = (float_t)((int16_t)(data[5] << 8 | data[4]) * 0.070);

    return HAL_OK;
}
