/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : app_freertos.h
  * Description        : FreeRTOS applicative header file
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __APP_FREERTOS_H
#define __APP_FREERTOS_H

#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os2.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "sensors.h"
#include "camera_driver.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

typedef union {
  struct {
    uint16_t can_nodes    : 8;
    uint16_t cam_1        : 3;
    uint16_t cam_2        : 3;
    // uint16_t flight_comp  : 1;
    uint16_t telem        : 1;
  };
  uint16_t status;
} sys_status_t;

typedef union {
  struct {
    uint8_t barometer    : 1;
    uint8_t lg_accel     : 1;
    uint8_t hg_accel     : 1;
    uint8_t file_sys     : 1;
  };
  uint8_t status;
} fc_status_t;

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define ADXL375_EVENT           0b00000001
#define BMP581_EVENT            0b00000010
#define LSM6DSO32_GYRO_EVENT    0b00000100
#define LSM6DSO32_ACCEL_EVENT   0b00001000


#define TELEM_ARM_EVENT    0b000000001
#define TELEM_DISARM_EVENT 0b000000010
#define TELEM_STAT_EVENT   0b000000100

#define CAN_NODE_WAKE_ID    0x001
#define CAN_NODE_SLEEP_ID   0x010

#define FC_OK 0XF

/* USER CODE END PD */

/* Exported macro -------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */
extern osThreadId_t fileManagementTaskHandle;
extern osThreadId_t telemetryHandlerTaskHandle;
extern osThreadId_t i2cSensorReadTaskHandle;
extern osMessageQueueId_t sensorDataHandle;

/* Exported function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

extern void fileManagementTask(void *argument);
void telemetryHandler(void *argument);
void i2cSensorReadTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* Hook prototypes */
void configureTimerForRunTimeStats(void);
unsigned long getRunTimeCounterValue(void);
/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

#ifdef __cplusplus
}
#endif
#endif /* __APP_FREERTOS_H */
