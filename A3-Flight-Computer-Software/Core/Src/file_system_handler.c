#include "file_system_handler.h"
#include "sensors.h"
#include <stdio.h>

int8_t path;
FATFS file_sys;
FIL file;
uint8_t data[500];
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
  int size = 0;
  int bytes_written = 0;
  TelemetryMessage_t payload;
  float_t seconds;
  uint8_t log_file_path[12];

  FS_Init();
  mk_log_dir();
  /* Infinite loop */
  for(;;)
  {
    if(osMessageQueueGet(sensorDataHandle, &payload, NULL, 3000) == osOK){
      seconds = (float_t)payload.time.Seconds + (float_t)(payload.time.SecondFraction - payload.time.SubSeconds)/(payload.time.SecondFraction - 1);
      switch (payload.sensor_type) {
        case SENSOR_STRAIN:
          sprintf(log_file_path, "strain_%X.txt", payload.node_id);
          f_open(&file, log_file_path, FA_WRITE|FA_OPEN_APPEND);
          size = sprintf(data, "%d:%d:%f,%d,%d,%d\n", payload.time.Hours,
                                                    payload.time.Minutes,
                                                    seconds,
                                                    payload.data.strain.left_gauge_uV,
                                                    payload.data.strain.center_gauge_uV,
                                                    payload.data.strain.right_gauge_uV);
        
          f_write(&file, data, size, &bytes_written);
          f_close(&file);
          break;
        case SENSOR_TEMPERATURE:
          break;
        case SENSOR_RTD:
          sprintf(log_file_path, "rtd_%X.txt", payload.node_id);
          f_open(&file, log_file_path, FA_WRITE|FA_OPEN_APPEND);
          size = sprintf(data, "%d:%d:%f,%d\n", payload.time.Hours,
                                                payload.time.Minutes,
                                                seconds,
                                                payload.data.temperature_mv);
        
          f_write(&file, data, size, &bytes_written);
          f_close(&file);
          break;
        case SENSOR_PRESSURE:
          
          break;
        case SENSOR_ACCEL_ADXL375:
        
          break;
        case SENSOR_GYRO:

          break;
        default:
          break;
      }
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