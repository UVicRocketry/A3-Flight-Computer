/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32h5xx_it.c
  * @brief   Interrupt Service Routines.
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
#include "main.h"
#include "stm32h5xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern FDCAN_HandleTypeDef hfdcan2;
extern DMA_HandleTypeDef handle_GPDMA1_Channel7;
extern SD_HandleTypeDef hsd1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern UART_HandleTypeDef huart4;
extern UART_HandleTypeDef huart5;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern TIM_HandleTypeDef htim1;

/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
   while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/******************************************************************************/
/* STM32H5xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32h5xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles EXTI Line0 interrupt.
  */
void EXTI0_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI0_IRQn 0 */

  /* USER CODE END EXTI0_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
  /* USER CODE BEGIN EXTI0_IRQn 1 */

  /* USER CODE END EXTI0_IRQn 1 */
}

/**
  * @brief This function handles EXTI Line4 interrupt.
  */
void EXTI4_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI4_IRQn 0 */

  /* USER CODE END EXTI4_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_4);
  /* USER CODE BEGIN EXTI4_IRQn 1 */

  /* USER CODE END EXTI4_IRQn 1 */
}

/**
  * @brief This function handles EXTI Line5 interrupt.
  */
void EXTI5_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI5_IRQn 0 */

  /* USER CODE END EXTI5_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_5);
  /* USER CODE BEGIN EXTI5_IRQn 1 */

  /* USER CODE END EXTI5_IRQn 1 */
}

/**
  * @brief This function handles EXTI Line6 interrupt.
  */
void EXTI6_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI6_IRQn 0 */

  /* USER CODE END EXTI6_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_6);
  /* USER CODE BEGIN EXTI6_IRQn 1 */

  /* USER CODE END EXTI6_IRQn 1 */
}

/**
  * @brief This function handles EXTI Line7 interrupt.
  */
void EXTI7_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI7_IRQn 0 */

  /* USER CODE END EXTI7_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_7);
  /* USER CODE BEGIN EXTI7_IRQn 1 */

  /* USER CODE END EXTI7_IRQn 1 */
}

/**
  * @brief This function handles GPDMA1 Channel 7 global interrupt.
  */
void GPDMA1_Channel7_IRQHandler(void)
{
  /* USER CODE BEGIN GPDMA1_Channel7_IRQn 0 */

  /* USER CODE END GPDMA1_Channel7_IRQn 0 */
  HAL_DMA_IRQHandler(&handle_GPDMA1_Channel7);
  /* USER CODE BEGIN GPDMA1_Channel7_IRQn 1 */

  /* USER CODE END GPDMA1_Channel7_IRQn 1 */
}

/**
  * @brief This function handles TIM1 Update interrupt.
  */
void TIM1_UP_IRQHandler(void)
{
  /* USER CODE BEGIN TIM1_UP_IRQn 0 */

  /* USER CODE END TIM1_UP_IRQn 0 */
  HAL_TIM_IRQHandler(&htim1);
  /* USER CODE BEGIN TIM1_UP_IRQn 1 */

  /* USER CODE END TIM1_UP_IRQn 1 */
}

/**
  * @brief This function handles TIM2 global interrupt.
  */
void TIM2_IRQHandler(void)
{
  /* USER CODE BEGIN TIM2_IRQn 0 */

  /* USER CODE END TIM2_IRQn 0 */
  HAL_TIM_IRQHandler(&htim2);
  /* USER CODE BEGIN TIM2_IRQn 1 */

  /* USER CODE END TIM2_IRQn 1 */
}

/**
  * @brief This function handles TIM3 global interrupt.
  */
void TIM3_IRQHandler(void)
{
  /* USER CODE BEGIN TIM3_IRQn 0 */

  /* USER CODE END TIM3_IRQn 0 */
  HAL_TIM_IRQHandler(&htim3);
  /* USER CODE BEGIN TIM3_IRQn 1 */

  /* USER CODE END TIM3_IRQn 1 */
}

/**
  * @brief This function handles USART1 global interrupt.
  */
void USART1_IRQHandler(void)
{
  /* USER CODE BEGIN USART1_IRQn 0 */

  /* USER CODE END USART1_IRQn 0 */
  HAL_UART_IRQHandler(&huart1);
  /* USER CODE BEGIN USART1_IRQn 1 */

  /* USER CODE END USART1_IRQn 1 */
}

/**
  * @brief This function handles USART2 global interrupt.
  */
void USART2_IRQHandler(void)
{
  /* USER CODE BEGIN USART2_IRQn 0 */

  /* USER CODE END USART2_IRQn 0 */
  HAL_UART_IRQHandler(&huart2);
  /* USER CODE BEGIN USART2_IRQn 1 */

  /* USER CODE END USART2_IRQn 1 */
}

/**
  * @brief This function handles UART4 global interrupt.
  */
void UART4_IRQHandler(void)
{
  /* USER CODE BEGIN UART4_IRQn 0 */

  /* USER CODE END UART4_IRQn 0 */
  HAL_UART_IRQHandler(&huart4);
  /* USER CODE BEGIN UART4_IRQn 1 */

  /* USER CODE END UART4_IRQn 1 */
}

/**
  * @brief This function handles UART5 global interrupt.
  */
void UART5_IRQHandler(void)
{
  /* USER CODE BEGIN UART5_IRQn 0 */

  /* USER CODE END UART5_IRQn 0 */
  HAL_UART_IRQHandler(&huart5);
  /* USER CODE BEGIN UART5_IRQn 1 */

  /* USER CODE END UART5_IRQn 1 */
}

/**
  * @brief This function handles SDMMC1 global interrupt.
  */
void SDMMC1_IRQHandler(void)
{
  /* USER CODE BEGIN SDMMC1_IRQn 0 */

  /* USER CODE END SDMMC1_IRQn 0 */
  HAL_SD_IRQHandler(&hsd1);
  /* USER CODE BEGIN SDMMC1_IRQn 1 */

  /* USER CODE END SDMMC1_IRQn 1 */
}

/**
  * @brief This function handles FDCAN2 interrupt 0.
  */
void FDCAN2_IT0_IRQHandler(void)
{
  /* USER CODE BEGIN FDCAN2_IT0_IRQn 0 */

  /* USER CODE END FDCAN2_IT0_IRQn 0 */
  HAL_FDCAN_IRQHandler(&hfdcan2);
  /* USER CODE BEGIN FDCAN2_IT0_IRQn 1 */

  /* USER CODE END FDCAN2_IT0_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
