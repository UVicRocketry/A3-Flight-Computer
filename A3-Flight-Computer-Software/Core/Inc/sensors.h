#include "main.h"

/*----------- Sensor Types -----------*/

typedef enum {
    SENSOR_STRAIN,
    SENSOR_TEMPERATURE,
    SENSOR_RTD,
    SENSOR_PRESSURE,
    SENSOR_BAROMETER,
    SENSOR_ACCEL_ADXL375,
    SENSOR_ACCEL_LSM6DS032,
    SENSOR_GYRO
} SensorType_t;


/*----------- Sensor Payload Structures -----------*/


// Strain Data
typedef struct {
    int16_t left_gauge_uV;
    int16_t center_gauge_uV;
    int16_t right_gauge_uV;
    uint8_t channel_id;
} StrainData_t;

// Accelerometer Data
typedef struct {
    int16_t accel_x_G;
    int16_t accel_y_G;
    int16_t accel_z_G;
} AccelData_t;

// Gryoscope Data
typedef struct {
    int16_t yaw_dps;
    int16_t pitch_dps;
    int16_t roll_dps;
} GyroData_t;

typedef struct {
    int32_t pressure;
    int32_t temperature;
} BaroData_t;


/*----------- Sensor Payload Union -----------*/


typedef union {
    StrainData_t strain;
    float_t temperature;
    float_t pressure;
    AccelData_t accel;
    GyroData_t gyro;
    int16_t temperature_mv;
    BaroData_t baro_data;
} SensorData_t;


/*----------- Main Structure -----------*/


typedef struct {
    RTC_TimeTypeDef time;
    uint8_t node_id;
    SensorType_t sensor_type;
    SensorData_t data;
} SensorPayload_t;

//Hardware pin defines for ADXL375
#define ADXL375_INT1_PIN  GPIO_PIN_4
#define ADXL375_INT2_PIN  GPIO_PIN_5

//I2C addresses for ADXL375
#define ADXL375_WRITE_ADDR      0xA6
#define ADXL375_READ_ADDR       0xA7

//Register addresses for ADXL375
#define ADXL375_DEV_ID_REG      0x00
#define ADXL375_OFSX            0x1E
#define ADXL375_OFSY            0x1F
#define ADXL375_OFSZ            0x20
#define ADXL375_DUR             0x21
#define ADXL375_LATENT          0x22
#define ADXL375_WINDOW          0x23
#define ADXL375_THRESH_ACT      0x24
#define ADXL375_THRESH_INACT    0x25
#define ADXL375_TIME_INACT      0x26
#define ADXL375_ACT_INACT_CTL   0x27
#define ADXL375_SHOCK_AXES      0x2A
#define ADXL375_ACT_SHOCK_STAT  0x2B
#define ADXL375_BW_RATE         0x2C
#define ADXL375_PWR_CTL         0x2D
#define ADXL375_INT_ENABLE      0x2E
#define ADXL375_INT_MAP         0x2F
#define ADXL375_INT_SOURCE      0x30
#define ADXL375_DATA_FORMAT     0x31
#define ADXL375_DATAX0          0x32
#define ADXL375_DATAX1          0x33
#define ADXL375_DATAY0          0x34
#define ADXL375_DATAY1          0x35
#define ADXL375_DATAZ0          0x36
#define ADXL375_DATAZ1          0x37
#define ADXL375_FIFO_CTL        0x38
#define ADXL375_FIFO_STATUS     0x39

#define ADXL375_DEVICE_ID       0xE5

#define ADXL375_800_BW          0b1101
#define ADXL375_400_BW          0b1100
#define ADXL375_100_BW          0b1010

#define ADXL375_LOW_POWER       0b00010000

//Defines for BMP581 Barometer
#define BMP581_INT_PIN          GPIO_PIN_0

#define BMP581_WRITE_ADDR       0x8C
#define BMP581_READ_ADDR        0x8D

#define BMP581_CHIP_ID_REG      0x01
#define BMP581_CHIP_STATUS      0x11
#define BMP581_DEV_CONFIG       0x13
#define BMP581_INT_CONFIG       0x14
#define BMP581_INT_SOURCE       0x15
#define BMP581_FIFO_CONFIG      0x16
#define BMP581_FIFO_COUNT       0x17
#define BMP581_FIFO_SEL         0x18
#define BMP581_TEMP_DATA_0      0x1D
#define BMP581_TEMP_DATA_1      0x1E
#define BMP581_TEMP_DATA_2      0x1F
#define BMP581_PRES_DATA_0      0x20
#define BMP581_PRES_DATA_1      0x21
#define BMP581_PRES_DATA_2      0x22
#define BMP581_INT_STATUS       0x27
#define BMP581_STATUS           0x28
#define BMP581_FIFO_DATA        0x29
#define BMP581_OSR_CONFIG       0x36
#define BMP581_ODR_CONFIG       0x37
#define BMP581_OSR_EFF          0x38

#define BMP581_CHIP_ID          0x50

#define BMP581_OSR_1X           0x00
#define BMP581_OSR_2X           0x01
#define BMP581_OSR_4X           0x02
#define BMP581_OSR_8X           0x03
#define BMP581_OSR_16X          0x04
#define BMP581_OSR_32X          0x05
#define BMP581_OSR_64X          0x06
#define BMP581_OSR_128X         0x07

#define BMP581_ODR_120HZ        0x8
#define BMP581_ODR_60HZ         0xE
#define BMP581_ODR_30HZ         0x13
#define BMP581_ODR_15HZ         0x16

#define BMP581_MODE_STDBY       0x0
#define BMP581_MODE_NORMAL      0x1
#define BMP581_MODE_FORCED      0x2
#define BMP581_MODE_NONSTOP     0x3

#define BMP581_PRES_ENABLE      0x40

#define BMP581_INT_POL_HIGH     0x02
#define BMP581_INT_PUSH_PULL    0x04
#define BMP581_INT_DRDY         0x01
#define BMP581_INT_ENABLE       0x08

#define BMP581_DEEPSTDBY_DIS    0X80
#define BMP581_ODR_MASK         0x83

//Defines for LSM6DS032
#define LSM6DSO32_WRITE_ADDR       0xD4
#define LSM6DSO32_READ_ADDR        0xD5

#define LSM6DSO32_INT1_PIN         GPIO_PIN_6
#define LSM6DSO32_INT2_PIN         GPIO_PIN_7

#define LSM6DSO32_DEV_ID           0x6C
#define LSM6DSO32_DEV_ID_REG       0x0F
#define LSM6DSO32_PIN_CTRL         0x02
#define LSM6DSO32_FIFO_CTRL1       0x07
#define LSM6DSO32_FIFO_CTRL2       0x08
#define LSM6DSO32_FIFO_CTRL3       0x09
#define LSM6DSO32_FIFO_CTRL4       0x0A
#define LSM6DSO32_CNTR_BDR_REG1    0x0B
#define LSM6DSO32_CNTR_BDR_REG2    0x0C
#define LSM6DSO32_INT1_CTRL        0x0D
#define LSM6DSO32_INT2_CTRL        0x0E
#define LSM6DSO32_CTRL1_XL         0x10
#define LSM6DSO32_CTRL2_G          0x11
#define LSM6DSO32_CTRL3_C          0x12
#define LSM6DSO32_CTRL4_C          0x13
#define LSM6DSO32_CTRL5_C          0x14
#define LSM6DSO32_CTRL6_C          0x15
#define LSM6DSO32_CTRL7_G          0x16
#define LSM6DSO32_CTRL8_XL         0x17
#define LSM6DSO32_CTRL9_XL         0x18
#define LSM6DSO32_CTRL10_G         0x19
#define LSM6DSO32_ALL_INT_SRC      0x1A
#define LSM6DSO32_WAKE_UP_SRC      0x1B
#define LSM6DSO32_TAP_SRC          0x1C
#define LSM6DSO32_D6D_SRC          0x1D
#define LSM6DSO32_STATUS_REG       0x1E
#define LSM6DSO32_OUT_TEMP_L       0x20
#define LSM6DSO32_OUT_TEMP_H       0x21
#define LSM6DSO32_OUTX_L_G         0x22
#define LSM6DSO32_OUTX_H_G         0x23
#define LSM6DSO32_OUTY_L_G         0x24
#define LSM6DSO32_OUTY_H_G         0x25
#define LSM6DSO32_OUTZ_L_G         0x26
#define LSM6DSO32_OUTZ_H_G         0x27
#define LSM6DSO32_OUTX_L_A         0x28
#define LSM6DSO32_OUTX_H_A         0x29
#define LSM6DSO32_OUTY_L_A         0x2A
#define LSM6DSO32_OUTY_H_A         0x2B
#define LSM6DSO32_OUTZ_L_A         0x2C
#define LSM6DSO32_OUTZ_H_A         0x2D
#define LSM6DSO32_TAP_CFG_2        0x58

#define LSM6DSO32_INT_DRDY_A       0x01
#define LSM6DSO32_INT_DRDY_G       0x02

#define LSM6DSO32_ODR_416HZ        0x60
#define LSM6DSO32_ODR_833HZ        0x70
#define LSM6DSO32_ODR_104HZ        0x40
#define LSM6DSO32_PWR_DWN          0x00
#define LSM6DS032_ODR_MASK         0x0F

#define LSM6DS032_FULL_SCALE       0x0C

#define LSM6DSO32_G_ENABlE         0x40
#define LSM6DSO32_INT_EN           0x80

HAL_StatusTypeDef ADXL375_Init(void);
uint8_t ADXL375_read_single_byte(uint8_t reg);
HAL_StatusTypeDef ADXL375_read_mult_byte(uint8_t reg, uint8_t *pdata, uint8_t length);
HAL_StatusTypeDef ADXL375_write_single_byte(uint8_t reg, uint8_t data);
HAL_StatusTypeDef ADXL375_get_acceleration(AccelData_t *pAccelData);
HAL_StatusTypeDef ADXL375_Deinit(void);


HAL_StatusTypeDef BMP581_Init(void);
uint8_t BMP581_read_single_byte(uint8_t reg);
HAL_StatusTypeDef BMP581_read_mult_byte(uint8_t reg, uint8_t *pdata, uint8_t length);
HAL_StatusTypeDef BMP581_write_single_byte(uint8_t reg, uint8_t data);
HAL_StatusTypeDef BMP581_get_temperature_pressure(BaroData_t *pbarodata);
HAL_StatusTypeDef BMP581_Deinit(void);


HAL_StatusTypeDef LSM6DSO32_Init(void);
uint8_t LSM6DSO32_read_single_byte(uint8_t reg);
HAL_StatusTypeDef LSM6DSO32_read_mult_byte(uint8_t reg, uint8_t *pdata, uint8_t length);
HAL_StatusTypeDef LSM6DSO32_write_single_byte(uint8_t reg, uint8_t data);
HAL_StatusTypeDef LSM6DSO32_get_acceleration(AccelData_t *pAccelData);
HAL_StatusTypeDef LSM6DSO32_get_gyro(GyroData_t *pGyroData);
HAL_StatusTypeDef LSM6DSO32_Deinit(void);



