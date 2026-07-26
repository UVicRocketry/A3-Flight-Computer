// Required includes for the file system handler
#include "ff.h"
#include "main.h"
#include "sd_diskio_dma_rtos.h"

// typedef struct {
//     log_source_e source,
//     log_
// } log_t;

// typedef struct {

// }log_source_e;

void fileManagementTask(void *argument);

void createFilePath(uint8_t *path, uint32_t max_length);
void logError(void);
