#include "sd_task.h"
#include "fatfs.h"
#include "bsp_driver_sd.h"
#include "fatfs_platform.h"
#include "rtc.h"
#include "cmsis_os.h"
#include <string.h>

volatile uint8_t sd_card_detected = 0;
volatile uint8_t sd_write_status = 0; /* 0=idle, 1=OK, 2=error */

/* debug */
volatile FRESULT dbg_mount_result = FR_OK;
volatile FRESULT dbg_open_result = FR_OK;
volatile FRESULT dbg_write_result = FR_OK;
volatile FRESULT dbg_close_result = FR_OK;

static const char months_str[] = "JanFebMarAprMayJunJulAugSepOctNovDec";

static uint8_t sd_parse_month(void)
{
    for (uint8_t i = 0; i < 12; i++) {
        if (months_str[i * 3]     == __DATE__[0] &&
            months_str[i * 3 + 1] == __DATE__[1] &&
            months_str[i * 3 + 2] == __DATE__[2])
            return i + 1;
    }
    return 1;
}

#define RTC_INIT_MAGIC  0xA55AU

static void SD_RTC_InitCompileTime(void)
{
    if (HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR0) == RTC_INIT_MAGIC)
    {
        RTC_DateTypeDef check = {0};
        HAL_RTC_GetDate(&hrtc, &check, RTC_FORMAT_BIN);
        if (check.Year >= 24)
            return;
    }

    RTC_TimeTypeDef t = {0};
    RTC_DateTypeDef d = {0};
    d.Month = sd_parse_month();
    d.Date = (uint8_t)((__DATE__[4] == ' ' ? 0 : (__DATE__[4] - '0')) * 10 + (__DATE__[5] - '0'));
    d.Year = (uint8_t)(((__DATE__[9] - '0') * 10) + (__DATE__[10] - '0'));
    d.WeekDay = RTC_WEEKDAY_MONDAY;
    t.Hours = (uint8_t)((__TIME__[0] - '0') * 10 + (__TIME__[1] - '0'));
    t.Minutes = (uint8_t)((__TIME__[3] - '0') * 10 + (__TIME__[4] - '0'));
    t.Seconds = (uint8_t)((__TIME__[6] - '0') * 10 + (__TIME__[7] - '0'));
    HAL_RTC_SetTime(&hrtc, &t, RTC_FORMAT_BIN);
    HAL_RTC_SetDate(&hrtc, &d, RTC_FORMAT_BIN);
    HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR0, RTC_INIT_MAGIC);
}

void SD_Task(void *argument)
{
    SD_RTC_InitCompileTime();

    osDelay(500);

    uint8_t sd_was_present = 0;

    for (;;)
    {
        if (BSP_SD_IsDetected() == SD_PRESENT)
        {
            sd_card_detected = 1;

            if (!sd_was_present)
            {
                osDelay(500); /* let card settle */

                dbg_mount_result = f_mount(&SDFatFS, SDPath, 1);

                if (dbg_mount_result == FR_OK)
                {
                    sd_was_present = 1;

                    FIL file;
                    dbg_open_result = f_open(&file, "0:/TEST.TXT", FA_CREATE_ALWAYS | FA_WRITE);
                    if (dbg_open_result == FR_OK)
                    {
                        static const char header[] =
                        		"SD card write test - OK\r\n"
                        		"To verify the write works: delete this file and power-cycle the board.\r\n"
                        		"If the file reappears, SD card write is working correctly.\r\n";
                        UINT bw;
                        dbg_write_result = f_write(&file, header, sizeof(header) - 1, &bw);
                        dbg_close_result = f_close(&file);

                        sd_write_status = (dbg_write_result == FR_OK && dbg_close_result == FR_OK) ? 1u : 2u;
                    }
                    else
                    {
                        sd_write_status = 2;
                        BSP_SD_DeInit();
                    }

                    f_mount(NULL, SDPath, 0);
                }
                else
                {
                    sd_write_status = 0;
                    BSP_SD_DeInit();
                }
            }
            else
            {
                if (BSP_SD_GetCardState() != SD_TRANSFER_OK)
                {
                    BSP_SD_DeInit();
                    sd_was_present = 0;
                    sd_card_detected = 0;
                    sd_write_status = 0;
                    dbg_mount_result = FR_OK;
                    dbg_open_result = FR_OK;
                    dbg_write_result = FR_OK;
                    dbg_close_result = FR_OK;
                }
            }
        }
        else
        {
            if (sd_was_present)
                BSP_SD_DeInit();
            sd_card_detected = 0;
            sd_was_present = 0;
            sd_write_status = 0;
            dbg_mount_result = FR_OK;
            dbg_open_result = FR_OK;
            dbg_write_result = FR_OK;
            dbg_close_result = FR_OK;
        }

        osDelay(500);
    }
}
