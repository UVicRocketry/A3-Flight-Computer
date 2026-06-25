// Required includes for the file system handler
#include "cmsis_os2.h"
#include "app_freertos.h"
#include "cmsis_os2.h"
#include "ff.h"
#include "main.h"
#include "stm32h5xx_hal_dma_ex.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "can_handler.h"

void fileManagementTask(void *argument);

void FS_Init(void);
void mk_log_dir(void);
void createFilePath(uint8_t *path, uint32_t max_length);
void logError(void);
