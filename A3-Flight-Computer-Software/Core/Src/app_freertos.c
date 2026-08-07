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
extern FDCAN_HandleTypeDef hfdcan2;
extern UART_HandleTypeDef huart1;

fc_status_t fc_stat = {0};

volatile uint16_t can_status = 0;


/* USER CODE END Variables */
/* Definitions for fileManagementTask */
osThreadId_t fileManagementTaskHandle;
const osThreadAttr_t fileManagementTask_attributes = {
  .name = "fileManagementTask",
  .priority = (osPriority_t) osPriorityNormal3,
  .stack_size = 512 * 4
};
/* Definitions for telemetryHandlerTask */
osThreadId_t telemetryHandlerTaskHandle;
const osThreadAttr_t telemetryHandlerTask_attributes = {
  .name = "telemetryHandlerTask",
  .priority = (osPriority_t) osPriorityBelowNormal5,
  .stack_size = 128 * 4
};
/* Definitions for i2cSensorReadTask */
osThreadId_t i2cSensorReadTaskHandle;
const osThreadAttr_t i2cSensorReadTask_attributes = {
  .name = "i2cSensorReadTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 128 * 4
};
/* Definitions for sensorData */
osMessageQueueId_t sensorDataHandle;
const osMessageQueueAttr_t sensorData_attributes = {
  .name = "sensorData"
};
/* Definitions for logQueue */
osMessageQueueId_t logQueueHandle;
const osMessageQueueAttr_t logQueue_attributes = {
  .name = "logQueue"
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
  sensorDataHandle = osMessageQueueNew (16, sizeof(SensorPayload_t), &sensorData_attributes);
  /* creation of logQueue */
  //logQueueHandle = osMessageQueueNew (16, sizeof(log_t), &logQueue_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */
  /* creation of fileManagementTask */
  fileManagementTaskHandle = osThreadNew(fileManagementTask, NULL, &fileManagementTask_attributes);

  /* creation of telemetryHandlerTask */
  telemetryHandlerTaskHandle = osThreadNew(telemetryHandler, NULL, &telemetryHandlerTask_attributes);

  /* creation of i2cSensorReadTask */
  i2cSensorReadTaskHandle = osThreadNew(i2cSensorReadTask, NULL, &i2cSensorReadTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}
/* USER CODE BEGIN Header_telemetryHandler */
/**
* @brief Function implementing the telemetryHandlerTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_telemetryHandler */
void telemetryHandler(void *argument)
{
  /* USER CODE BEGIN telemetryHandlerTask */
  cam_status_t cam1_stat;
  cam_status_t cam2_stat;
  int32_t flags;
  FDCAN_TxHeaderTypeDef txHeader;
  sys_status_t stat_msg = {0};
  
 
  for(;;)
  {
    flags = osThreadFlagsWait(TELEM_ARM_EVENT | TELEM_DISARM_EVENT | TELEM_STAT_EVENT, osFlagsWaitAny, osWaitForever);
    if (flags < 0){
      // osThreadFlagsClear()
    } else if(flags & TELEM_ARM_EVENT) {
      cam1_stat = camera_start(CAM1);
      if(cam1_stat == REPLY_ERROR || cam1_stat == REPLY_INVALID_CMD){
        //log error
      }

      cam2_stat = camera_start(CAM2);
      if(cam2_stat == REPLY_ERROR || cam2_stat == REPLY_INVALID_CMD){
        //log error
      }

      txHeader.Identifier = CAN_NODE_WAKE_ID;
      HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan2, &txHeader, NULL);
    } else if(flags & TELEM_DISARM_EVENT) {
      cam1_stat = camera_stop(CAM1);
      if(cam1_stat == REPLY_ERROR || cam1_stat == REPLY_INVALID_CMD){
        //log error
      }

      cam2_stat = camera_stop(CAM2);
      if(cam2_stat == REPLY_ERROR || cam2_stat == REPLY_INVALID_CMD){
        //log error
      }

      txHeader.Identifier = CAN_NODE_SLEEP_ID;
      HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan2, &txHeader, NULL);
    } else if(flags & TELEM_STAT_EVENT) {

      cam1_stat = camera_status(CAM1);
      cam2_stat = camera_status(CAM2);

      taskENTER_CRITICAL();
      stat_msg.can_nodes = can_status;
      can_status = 0;

      stat_msg.flight_comp = (fc_stat.status & FC_OK) == FC_OK ? 1 : 0;
      fc_stat.status = 0;
      taskEXIT_CRITICAL();

      stat_msg.cam_1 = (cam1_stat == REPLY_RECORDING) ? 1 : 0;
      stat_msg.cam_2 = (cam2_stat == REPLY_RECORDING) ? 1 : 0;

      if(HAL_UART_Transmit(&huart1, (uint8_t *)&stat_msg.status, sizeof(sys_status_t), 100) != HAL_OK){
        //log error
      }
    }
  }
  /* USER CODE END telemetryHandlerTask */
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

  /* Infinite loop */
  for(;;)
  {
    flags = osThreadFlagsWait(BMP581_EVENT | ADXL375_EVENT, osFlagsWaitAny, osWaitForever);

    if(flags & ADXL375_EVENT){
      HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);
      HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);

      if(ADXL375_get_acceleration(&acceleration) == HAL_OK){
        payload.sensor_type = SENSOR_ACCEL_ADXL375;
        payload.time = time;
        payload.data.accel = acceleration;

        fc_stat.hg_accel = 1;

        osMessageQueuePut(sensorDataHandle, &payload, 0, 10);
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

        fc_stat.barometer = 1;

        osMessageQueuePut(sensorDataHandle, &payload, 0, 10);
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

        fc_stat.lg_accel = 1;

        osMessageQueuePut(sensorDataHandle, &payload, 0, 10);
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

        fc_stat.lg_accel = 1;

        osMessageQueuePut(sensorDataHandle, &payload, 0, 10);
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

