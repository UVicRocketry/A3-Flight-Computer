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
#include "camera_driver.h"


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
extern TIM_HandleTypeDef htim2;

fc_status_t fc_stat = {0};

volatile uint16_t can_status = 0;


/* USER CODE END Variables */
/* Definitions for fileManagementTask */
osThreadId_t fileManagementTaskHandle;
const osThreadAttr_t fileManagementTask_attributes = {
  .name = "fileManagementTask",
  .priority = (osPriority_t) osPriorityNormal3,
  .stack_size = 1024 * 4
};
/* Definitions for telemetryHandlerTask */
osThreadId_t telemetryHandlerTaskHandle;
const osThreadAttr_t telemetryHandlerTask_attributes = {
  .name = "telemetryHandlerTask",
  .priority = (osPriority_t) osPriorityNormal2,
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
  FDCAN_TxHeaderTypeDef txHeader = {0};
  sys_status_t stat_msg = {0};
  HAL_StatusTypeDef uart_flags;
  
 
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
      osThreadFlagsSet(i2cSensorReadTaskHandle, SENSOR_INIT);
      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, 1);
      osDelay(500);
      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, 0);
      osDelay(500);
      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, 1);
      osDelay(500);
      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, 0);
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
      osThreadFlagsSet(i2cSensorReadTaskHandle, SENSOR_DEINIT);
    } else if(flags & TELEM_STAT_EVENT) {

      cam1_stat = camera_status(CAM1);
      cam2_stat = camera_status(CAM2);

      taskENTER_CRITICAL();
      stat_msg.can_nodes = can_status;
      can_status = 0;

      stat_msg.flight_comp = (fc_stat.status & FC_OK) == FC_OK ? 1 : 0;
      fc_stat.status = 0;
      fc_stat.file_sys = 1;
      taskEXIT_CRITICAL();

      switch (cam1_stat) {
        case REPLY_STOPPED:
        case REPLY_STOPPING:
          stat_msg.cam_1 = CAMERA_STOP;
          break;
        case REPLY_STARTING:
          stat_msg.cam_1 = CAMERA_START;
          break;
        case REPLY_RECORDING:
          stat_msg.cam_1 = CAMERA_RECORD;
          break;
        case REPLY_ERROR:
        case REPLY_INVALID_CMD:
        case REPLY_BUSY:
        default:
          stat_msg.cam_1 = CAMERA_ERROR;
          break;
      }
      
       switch (cam2_stat) {
        case REPLY_STOPPED:
        case REPLY_STOPPING:
          stat_msg.cam_2 = CAMERA_STOP;
          break;
        case REPLY_STARTING:
          stat_msg.cam_2 = CAMERA_START;
          break;
        case REPLY_RECORDING:
          stat_msg.cam_2 = CAMERA_RECORD;
          break;
        case REPLY_ERROR:
        case REPLY_INVALID_CMD:
        case REPLY_BUSY:
        default:
          stat_msg.cam_2 = CAMERA_ERROR;
          break;
      }
      
      stat_msg.dummy = ~stat_msg.dummy;

      uart_flags = HAL_UART_Transmit(&huart1, (uint8_t *)&stat_msg.status, sizeof(sys_status_t), 100);

      if(uart_flags){
        stat_msg.status = 0x0BAD;
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

        fc_stat.hg_accel = 1;

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

        fc_stat.barometer = 1;

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

        fc_stat.lg_accel = 1;

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

        fc_stat.lg_accel = 1;

        osMessageQueuePut(sensorDataHandle, &payload, 0, 0);
      } else {
        //Log error
      }
    }
    if (flags & SENSOR_INIT) {
        ADXL375_Init();
        BMP581_Init();
        LSM6DSO32_Init();
        HAL_TIM_Base_Start_IT(&htim2);
    }
    if (flags & SENSOR_DEINIT) {
        ADXL375_Deinit();
        BMP581_Deinit();
        LSM6DSO32_Deinit();
        HAL_TIM_Base_Stop_IT(&htim2);   
    }
  }
  /* USER CODE END i2cSensorReadTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

