#ifndef SD_TASK_H
#define SD_TASK_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* 0 = idle/gray  1 = write OK/green  2 = error/red */
extern volatile uint8_t sd_write_status;

void SD_Task(void *argument);

#ifdef __cplusplus
}
#endif

#endif /* SD_TASK_H */
