#include "can_handler.h"

extern osMessageQueueId_t sensorDataHandle;

void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs){
  FDCAN_RxHeaderTypeDef rxHeader;
  uint8_t data[8];

  if(HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &rxHeader, data) != HAL_OK){
    
  }
  HAL_FDCAN_ActivateNotification(hfdcan, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0);

}
