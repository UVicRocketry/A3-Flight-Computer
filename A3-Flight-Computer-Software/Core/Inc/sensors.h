#include "main.h"
#include <math.h>

/*----------- Sensor Types -----------*/

typedef enum {
    SENSOR_STRAIN,
    SENSOR_TEMPERATURE,
    SENSOR_RTD,
    SENSOR_PRESSURE,
    SENSOR_ACCEL_ADXL375,
    SENSOR_GYRO
} SensorType_t;


/*----------- Sensor Payload Structures -----------*/


// Strain Data
typedef struct {
    int16_t left_gauge_uV;
    int16_t center_gauge_uV;
    int16_t right_gauge_uV;
} StrainData_t;

// Accelerometer Data
typedef struct {
    float accel_x_G;
    float accel_y_G;
    float accel_z_G;
} AccelData_t;

// Gryoscope Data
typedef struct {
    float yaw_dps;
    float pitch_dps;
    float roll_dps;
} GyroData_t;


/*----------- Sensor Payload Union -----------*/


typedef union {
    StrainData_t strain;
    float_t temperature;
    float_t pressure;
    AccelData_t accel;
    GyroData_t gyro;
    int16_t temperature_mv;
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


HAL_StatusTypeDef ADXL375_Init(void);
uint8_t ADXL375_read_single_byte(uint8_t reg);
HAL_StatusTypeDef ADXL375_read_mult_byte(uint8_t reg, uint8_t *pdata, uint8_t length);
HAL_StatusTypeDef ADXL375_write_single_byte(uint8_t reg, uint8_t data);
HAL_StatusTypeDef ADXL375_get_acceleration(AccelData_t *pAccelData);

HAL_StatusTypeDef BMP581_Init(void);
uint8_t BMP581_read_single_byte(uint8_t reg);
HAL_StatusTypeDef BMP5810_read_mult_byte(uint8_t reg, uint8_t *pdata, uint8_t length);
HAL_StatusTypeDef BMP581_write_single_byte(uint8_t reg, uint8_t data);
HAL_StatusTypeDef BMP581_get_temperature_pressure(float_t *pTemperature, float_t *pPressure);

