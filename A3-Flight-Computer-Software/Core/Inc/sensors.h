#include "main.h"

/*----------- Sensor Types -----------*/

typedef enum
{
    SENSOR_STRAIN,
    SENSOR_TEMPERATURE,
    SENSOR_PRESSURE,
    SENSOR_ACCEL,
    SENSOR_GYRO,
    SENSOR_SYSTEM

} SensorType_t;


/*----------- Sensor Payload Structures -----------*/


// Strain Data
typedef struct {
    int16_t left_gauge_uV;
    int16_t center_gauge_uV;
    int16_t right_gauge_uV;
} StrainData_t;

// Strain Data
typedef struct {
    int16_t RTD_mV;
} RTDData_t;

// Temperature Data
typedef struct {
    float temperature_C;
} TempData_t;

// Presure Data
typedef struct {
    float pressure_Pa;
} PressureData_t;

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
    TempData_t temp;
    PressureData_t pressure;
    AccelData_t accel;
    GyroData_t gyro;
    RTDData_t temp_mv;
} SensorPayload_t;


/*----------- Main Structure -----------*/


typedef struct {
    RTC_TimeTypeDef time;

    uint16_t mess_id;

    uint8_t node_id;

    SensorType_t sensor_type;

    SensorPayload_t data;

} TelemetryMessage_t;


// We could also add a enum for the type if you thought it was useful, or we can just dervie it from a message ID
typedef enum {
    BUS_CAN,
    BUS_I2C,
} BusType_t;

#define ADXL375_WRITE_ADDR      0xA6
#define ADXL375_READ_ADDR       0xA7

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
#define ADXL375_LOW_POWER       0b00010000


HAL_StatusTypeDef ADXL375_Init(void);
uint8_t ADXL375_read_single_byte(uint8_t reg);
HAL_StatusTypeDef ADXL375_read_mult_byte(uint8_t reg, uint8_t *pdata, uint8_t length);
HAL_StatusTypeDef ADXL375_write_single_byte(uint8_t reg, uint8_t data);
HAL_StatusTypeDef ADXL375_get_acceleration(AccelData_t *pAccelData);

