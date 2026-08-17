/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : app_freertos.c
  * Description        : FreeRTOS applicative file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "app_freertos.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

extern RTC_HandleTypeDef hrtc;
extern TIM_HandleTypeDef htim2;
/* USER CODE END Variables */
/* Definitions for fileManagementTask */
osThreadId_t fileManagementTaskHandle;
const osThreadAttr_t fileManagementTask_attributes = {
  .name = "fileManagementTask",
  .priority = (osPriority_t) osPriorityNormal3,
  .stack_size = 1024 * 4
};
/* Definitions for i2cSensorReadTask */
osThreadId_t i2cSensorReadTaskHandle;
const osThreadAttr_t i2cSensorReadTask_attributes = {
  .name = "i2cSensorReadTask",
  .priority = (osPriority_t) osPriorityNormal4,
  .stack_size = 1024 * 4
};
/* Definitions for sensorData */
osMessageQueueId_t sensorDataHandle;
const osMessageQueueAttr_t sensorData_attributes = {
  .name = "sensorData"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

/* USER CODE BEGIN 1 */
/* Functions needed when configGENERATE_RUN_TIME_STATS is on */
__weak void configureTimerForRunTimeStats(void)
{

}

__weak unsigned long getRunTimeCounterValue(void)
{
return 0;
}
/* USER CODE END 1 */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */
  /* creation of sensorData */
  sensorDataHandle = osMessageQueueNew (90, sizeof(SensorPayload_t), &sensorData_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */
  /* creation of fileManagementTask */
  fileManagementTaskHandle = osThreadNew(fileManagementTask, NULL, &fileManagementTask_attributes);

  /* creation of i2cSensorReadTask */
  i2cSensorReadTaskHandle = osThreadNew(i2cSensorReadTask, NULL, &i2cSensorReadTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}
/* USER CODE BEGIN Header_i2cSensorReadTask */
/**
* @brief Function implementing the i2cSensorReadTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_i2cSensorReadTask */
void i2cSensorReadTask(void *argument)
{
  /* USER CODE BEGIN i2cSensorReadTask */
  RTC_TimeTypeDef time;
  RTC_DateTypeDef date;
  HAL_StatusTypeDef stat;
  uint32_t flags;
  SensorPayload_t payload;
  AccelData_t acceleration;
  GyroData_t gyro_data;
  BaroData_t baro_data;

  ADXL375_Init();
  BMP581_Init();
  LSM6DSO32_Init();
  HAL_TIM_Base_Start_IT(&htim2);   
  /* Infinite loop */
  for(;;)
  {
    flags = osThreadFlagsWait(SENSOR_INIT | SENSOR_DEINIT | LSM6DSO32_GYRO_EVENT | LSM6DSO32_ACCEL_EVENT | BMP581_EVENT | ADXL375_EVENT, osFlagsWaitAny, osWaitForever);

    if(flags & ADXL375_EVENT){
      HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);
      HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);

      if(ADXL375_get_acceleration(&acceleration) == HAL_OK){
        payload.sensor_type = SENSOR_ACCEL_ADXL375;
        payload.time = time;
        payload.data.accel = acceleration;

        osMessageQueuePut(sensorDataHandle, &payload, 0, 0);
      } else {
        //Log error
      }
    }
    if (flags & BMP581_EVENT) {
      HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);
      HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);

      uint8_t data = BMP581_read_single_byte(BMP581_INT_STATUS);
      if(BMP581_get_temperature_pressure(&baro_data) == HAL_OK){
        payload.sensor_type = SENSOR_BAROMETER;
        payload.time = time;
        payload.data.baro_data = baro_data;

        osMessageQueuePut(sensorDataHandle, &payload, 0, 0);
      } else {
        //Log error
      }
    }
    if(flags & LSM6DSO32_ACCEL_EVENT){
      HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);
      HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);

      if(LSM6DSO32_get_acceleration(&acceleration) == HAL_OK){
        payload.sensor_type = SENSOR_ACCEL_LSM6DS032;
        payload.time = time;
        payload.data.accel = acceleration;

        osMessageQueuePut(sensorDataHandle, &payload, 0, 0);
      } else {
        //Log error
      }
    }
    if (flags & LSM6DSO32_GYRO_EVENT) {
      HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);
      HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);

      if(LSM6DSO32_get_gyro(&gyro_data) == HAL_OK){
        payload.sensor_type = SENSOR_GYRO;
        payload.time = time;
        payload.data.gyro = gyro_data;

        osMessageQueuePut(sensorDataHandle, &payload, 0, 0);
      } else {
        //Log error
      }
    }
  }
  /* USER CODE END i2cSensorReadTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

