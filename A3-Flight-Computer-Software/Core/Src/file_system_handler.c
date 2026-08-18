#include "file_system_handler.h"
#include "app_freertos.h"
#include "ff.h"
#include <string.h>

int8_t path;
FATFS file_sys;
FIL file;
uint8_t data[64] = {0};
uint8_t strain_buff[512] = {0};
uint16_t strain_buff_size = 0;
uint8_t rtd_buff[512] = {0};
uint16_t rtd_buff_size = 0;
uint8_t hg_buff[512] = {0};
uint16_t hg_buff_size = 0;
uint8_t lg_buff[512] = {0};
uint16_t lg_buff_size = 0;
uint8_t gyro_buff[512] = {0};
uint16_t gyro_buff_size = 0;
uint8_t baro_buff[512] = {0};
uint16_t baro_buff_size = 0;

const uint8_t null_buff[512] = {0};

uint8_t log_trigger = 0;

FATFS* file_sys_ref = &file_sys;

uint8_t buff[15];
FRESULT stat = FR_NO_FILESYSTEM;
int size = 0;
int bytes_written = 0;
SensorPayload_t payload;
float_t seconds;
uint8_t log_file_path[17];
uint8_t seconds_buf[20];

uint32_t free_clusters = 0;
uint32_t free_sectors = 0;

extern osMessageQueueId_t sensorDataHandle;
extern fc_status_t fc_stat;

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
  FS_Init();
  mk_log_dir();
  /* Infinite loop */
  for(;;)
  {
    if(osMessageQueueGet(sensorDataHandle, &payload, NULL, osWaitForever) == osOK){
      f_getfree(&path, &free_clusters, &file_sys_ref);
      free_sectors = free_clusters * file_sys_ref->n_fatent;
      if(free_sectors > 1000){
        seconds = (float_t)payload.time.Seconds + (float_t)(payload.time.SecondFraction - payload.time.SubSeconds)/(payload.time.SecondFraction - 1);
        ftoa(seconds, seconds_buf, 6);
        switch (payload.sensor_type) {
          case SENSOR_STRAIN:
            snprintf(data, 100, "%d:%d:%s,%X,%d,%d,%d,%d\n", payload.time.Hours,
                                                      payload.time.Minutes,
                                                      seconds_buf,
                                                      payload.node_id,
                                                      payload.data.strain.left_gauge_uV,
                                                      payload.data.strain.center_gauge_uV,
                                                      payload.data.strain.right_gauge_uV, 
                                                      payload.data.strain.channel_id);
            if((64 + strain_buff_size) > 512) {
              stat = f_open(&file, "strain.csv", FA_WRITE|FA_OPEN_APPEND);
              stat = f_write(&file, strain_buff, 512, &bytes_written);
              stat = f_close(&file);
              memcpy(strain_buff, data, 64);
              strain_buff_size = 64;
            } else {
              memcpy(strain_buff + strain_buff_size, data, 64);
              strain_buff_size += 64;
            }
            memcpy(data, null_buff, 64);
            break;
          case SENSOR_TEMPERATURE:
            break;
          case SENSOR_RTD:
            snprintf(data, 64, "%d:%d:%s,%X,%d\n", payload.time.Hours,
                                                  payload.time.Minutes,
                                                  seconds_buf,
                                                  payload.node_id,
                                                  payload.data.temperature_mv);
            if((64 + rtd_buff_size) > 512) {
              stat = f_open(&file, "rtd.csv", FA_WRITE|FA_OPEN_APPEND);
              stat = f_write(&file, rtd_buff, 512, &bytes_written);
              stat = f_close(&file);
              memcpy(rtd_buff, data, 64);
              rtd_buff_size = 64;
            } else {
              memcpy(rtd_buff + rtd_buff_size, data, 64);
              rtd_buff_size += 64;
            }
            memcpy(data, null_buff, 64);
            break;
          case SENSOR_PRESSURE:
            
            break;
          case SENSOR_ACCEL_ADXL375:
            snprintf(data, 100, "%d:%d:%s,%d,%d,%d\n", payload.time.Hours,
                                                  payload.time.Minutes,
                                                  seconds_buf,
                                                  payload.data.accel.accel_x_G,
                                                  payload.data.accel.accel_y_G,
                                                  payload.data.accel.accel_z_G);
            if((64 + hg_buff_size) > 512) {
              stat = f_open(&file, "hgaccel.csv", FA_WRITE|FA_OPEN_APPEND);
              stat = f_write(&file, hg_buff, 512, &bytes_written);
              stat = f_close(&file);
              memcpy(hg_buff, data, 64);
              hg_buff_size = 64;
            } else {
              memcpy(hg_buff + hg_buff_size, data, 64);
              hg_buff_size += 64;
            }
            strcpy(data, null_buff);
            break;
          case SENSOR_ACCEL_LSM6DS032:
            snprintf(data, 100, "%d:%d:%s,%d,%d,%d\n", payload.time.Hours,
                                                  payload.time.Minutes,
                                                  seconds_buf,
                                                  payload.data.accel.accel_x_G,
                                                  payload.data.accel.accel_y_G,
                                                  payload.data.accel.accel_z_G);
            if((64 + lg_buff_size) > 512) {
              stat = f_open(&file, "lgaccel.csv", FA_WRITE|FA_OPEN_APPEND);
              stat = f_write(&file, lg_buff, 512, &bytes_written);
              stat = f_close(&file);
              memcpy(lg_buff, data, 64);
              lg_buff_size = 64;
            } else {
              memcpy(lg_buff + lg_buff_size, data, 64);
              lg_buff_size += 64;
            }
            memcpy(data, null_buff, 64);
            break;
          case SENSOR_GYRO:
            snprintf(data, 100, "%d:%d:%s,%d,%d,%d\n", payload.time.Hours,
                                                  payload.time.Minutes,
                                                  seconds_buf,
                                                  payload.data.gyro.pitch_dps,
                                                  payload.data.gyro.roll_dps,
                                                  payload.data.gyro.yaw_dps);
            if((64 + gyro_buff_size) > 512) {
              stat = f_open(&file, "gyro.csv", FA_WRITE|FA_OPEN_APPEND);
              stat = f_write(&file, gyro_buff, 512, &bytes_written);
              stat = f_close(&file);
              memcpy(gyro_buff, data, 64);
              gyro_buff_size = 64;
            } else {
              memcpy(gyro_buff + gyro_buff_size, data, 64);
              gyro_buff_size += 64;
            }
            memcpy(data, null_buff, 64);
            break;
          case SENSOR_BAROMETER:
            snprintf(data, 100, "%d:%d:%s,%d,%d\n", payload.time.Hours,
                                                  payload.time.Minutes,
                                                  seconds_buf,
                                                  payload.data.baro_data.pressure,
                                                  payload.data.baro_data.temperature);
            if((64 + baro_buff_size) > 512) {
              stat = f_open(&file, "baro.csv", FA_WRITE|FA_OPEN_APPEND);
              stat = f_write(&file, baro_buff, 512, &bytes_written);
              stat = f_close(&file);
              memcpy(baro_buff, data, 64);
              baro_buff_size = 64;
            } else {
              memcpy(baro_buff + baro_buff_size, data, 64);
              baro_buff_size += 64;
            }
            memcpy(data, null_buff, 64);
            break;
          default:
            break;
        }
        if(stat != FR_OK || free_sectors < 1000){
          fc_stat.file_sys = 0;
        }
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
    open_count = 0;
    f_write(&file, &open_count, sizeof(uint32_t), &bytes_rw);
    f_close(&file);
  } else {
    stat = f_read(&file, &open_count, sizeof(uint32_t), &bytes_rw);
    f_close(&file);
    stat = f_open(&file, "open", FA_WRITE);
    open_count++;
    stat = f_write(&file, &open_count, sizeof(uint32_t), &bytes_rw);
    f_close(&file);
  }

  sprintf(buff, "logs%d", open_count);
  stat = f_mkdir(buff);
  stat = f_chdir(buff);
}