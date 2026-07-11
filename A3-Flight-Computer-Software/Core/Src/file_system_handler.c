#include "file_system_handler.h"
#include <stdio.h>

int8_t path;
FATFS file_sys;
FIL file;
uint8_t data[30];
uint8_t buff[15];

extern osMessageQueueId_t sensorDataHandle;

static inline void FS_Init(void);
static inline void mk_log_dir(void);

/* USER CODE BEGIN Header_fileManagementTask */
/**
* @brief Function implementing the fileManagementTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_fileManagementTask */
void fileManagementTask(void *argument)
{
  /* USER CODE BEGIN fileManagementTask */
  FRESULT stat = FR_NO_FILESYSTEM;
  int count = 0;
  int size = 0;
  int bytes_written = 0;
  canPacket_t payload;
  FS_Init();
  mk_log_dir();
  /* Infinite loop */
  for(;;)
  {
    if(osMessageQueueGet(sensorDataHandle, &payload, NULL, 3000) == osOK){
      f_open(&file, "log.txt", FA_WRITE|FA_OPEN_APPEND);

      size = sprintf(data, "%d:%d:%d.%d, %X\n", payload.time.Hours,
                                              payload.time.Minutes,
                                              payload.time.Seconds,
                                              payload.time.SubSeconds,
                                              payload.rxHeader.Identifier);
      f_write(&file, data, size, &bytes_written);

      f_close(&file);
    }
  }
  /* USER CODE END fileManagementTask */
}

static inline void FS_Init(){
  FRESULT stat = FR_NO_FILESYSTEM;
  stat = FATFS_LinkDriver(&SD_DMA_Driver, &path);
  if(stat != FR_OK){
    Error_Handler();
  }

  stat = f_mount(&file_sys, &path, 1);

  if(stat != FR_OK){
    Error_Handler();
  }
}

static inline void mk_log_dir(void){
  FRESULT stat = FR_NO_FILESYSTEM;
  uint8_t bytes_rw;
  uint32_t open_count;

  stat = f_open(&file, "open", FA_READ | FA_WRITE | FA_OPEN_ALWAYS);
  if(f_size(&file) == 0){
    f_write(&file, "0", 2, &bytes_rw);
    open_count = 0;
    f_close(&file);
  } else {
    stat = f_read(&file, buff, 8, &bytes_rw);
    f_close(&file);
    stat = f_open(&file, "open", FA_WRITE);
    open_count = atoi(buff);
    open_count++;
    uint32_t size = sprintf(buff, "%d", open_count);
    stat = f_write(&file, buff, size, &bytes_rw);
    f_close(&file);
  }

  sprintf(buff, "logs-%d", open_count);
  stat = f_mkdir(buff);
  stat = f_chdir(buff);
}