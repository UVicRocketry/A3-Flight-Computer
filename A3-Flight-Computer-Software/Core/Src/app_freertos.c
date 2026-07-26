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
#include "cmsis_os2.h"
#include "stm32h5xx_hal_gpio.h"

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

/* USER CODE END Variables */
/* Definitions for fileManagementTask */
osThreadId_t fileManagementTaskHandle;
const osThreadAttr_t fileManagementTask_attributes = {
  .name = "fileManagementTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 512 * 4
};
/* Definitions for telemetryHandlerTask */
osThreadId_t telemetryHandlerTaskHandle;
const osThreadAttr_t telemetryHandlerTask_attributes = {
  .name = "telemetryHandlerTask",
  .priority = (osPriority_t) osPriorityLow,
  .stack_size = 128 * 4
};
/* Definitions for i2cSensorReadTask */
osThreadId_t i2cSensorReadTaskHandle;
const osThreadAttr_t i2cSensorReadTask_attributes = {
  .name = "i2cSensorReadTask",
  .priority = (osPriority_t) osPriorityLow,
  .stack_size = 128 * 4
};
/* Definitions for genericTimer */
osTimerId_t genericTimerHandle;
const osTimerAttr_t genericTimer_attributes = {
  .name = "genericTimer"
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
  /* creation of genericTimer */
  genericTimerHandle = osTimerNew(genericCallback01, osTimerOnce, NULL, &genericTimer_attributes);

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

  uint8_t stop_sent = 0;
  
  /* Infinite loop */
  cam1_stat = camera_start(CAM1);

  if(cam1_stat != REPLY_STARTING || cam1_stat != REPLY_RECORDING){
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
  }

  cam2_stat = camera_start(CAM2);
  
  if(cam2_stat != REPLY_STARTING || cam1_stat != REPLY_RECORDING){
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
  }

  for(;;)
  {
    osDelay(100);
    cam1_stat = camera_status(CAM1);
    if(cam1_stat == REPLY_INVALID_CMD || cam1_stat == REPLY_ERROR){
      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
    }

    cam2_stat = camera_status(CAM2);
    if(cam1_stat == REPLY_INVALID_CMD || cam1_stat == REPLY_ERROR){
      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
    }

    if(cam1_stat == REPLY_RECORDING || cam2_stat == REPLY_RECORDING && stop_sent != 1){
      osTimerStart(genericTimerHandle, 15 * 1000);
      stop_sent = 1;
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
  float_t temperature;
  float_t pressure;

  ADXL375_Init();
  BMP581_Init();
  /* Infinite loop */
  for(;;)
  {
    flags = osThreadFlagsWait(0x0000000, osFlagsWaitAny, 0);

    if(flags & ADXL375_EVENT){
      HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);
      HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);

      if(ADXL375_get_acceleration(&acceleration) == HAL_OK){
        payload.sensor_type = SENSOR_ACCEL_ADXL375;
        payload.time = time;
        payload.data.accel = acceleration;

        osMessageQueuePut(sensorDataHandle, &payload, 0, 10);
      } else {
        //Log error
      }
    }
    if (flags & BMP581_EVENT) {
      uint8_t data = BMP581_read_single_byte(BMP581_INT_STATUS);

      BMP581_get_temperature_pressure(&temperature, &pressure);
    }
  }
  /* USER CODE END i2cSensorReadTask */
}

/* genericCallback01 function */
void genericCallback01(void *argument)
{
  cam_status_t cam1_stat;
  cam_status_t cam2_stat;
  /* USER CODE BEGIN genericCallback01 */
  cam1_stat = camera_stop( CAM1);
  cam2_stat = camera_stop( CAM2);

  if(cam1_stat != REPLY_STOPPED || cam2_stat != REPLY_STOPPED){
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
  }
  /* USER CODE END genericCallback01 */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

