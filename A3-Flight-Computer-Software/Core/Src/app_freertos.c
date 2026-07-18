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
/* Definitions for sensorData */
osMessageQueueId_t sensorDataHandle;
const osMessageQueueAttr_t sensorData_attributes = {
  .name = "sensorData"
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

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */
  /* creation of sensorData */
  sensorDataHandle = osMessageQueueNew (16, sizeof(uint16_t), &sensorData_attributes);

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
  /* Infinite loop */
  for(;;)
  {
    
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
  AccelData_t acceleration;

  ADXL375_Init();
  /* Infinite loop */
  for(;;)
  {
    flags = osThreadFlagsWait(0x0000000, osFlagsWaitAny, 0);
    if(flags & ADXL375_EVENT){
      //stat = HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BCD);
      //stat = HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BCD);
      ADXL375_get_acceleration(&acceleration);
    }
  }
  /* USER CODE END i2cSensorReadTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

