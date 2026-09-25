/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file   fatfs.c
  * @brief  Code for fatfs applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
#include "fatfs.h"
#include "rtc.h"

uint8_t retSD;    /* Return value for SD */
char SDPath[4];   /* SD logical drive path */
FATFS SDFatFS;    /* File system object for SD logical drive */
FIL SDFile;       /* File object for SD */

/* USER CODE BEGIN Variables */

uint8_t Start_uSD_Test = 0;
uint8_t retUSBH;    /* Return value for USBH */
char USBHPath[4];   /* USBH logical drive path */
FATFS USBHFatFS;    /* File system object for USBH logical drive */
FIL USBHFile;       /* File object for USBH */


#define LED_OK                     LED1
#define LED_ERROR                  LED2
#define APP_OK                     0
#define APP_ERROR                  -1
#define APP_SD_UNPLUGGED           -2
#define APP_INIT                   1
#define  FATFS_MKFS_ALLOWED   1
typedef enum {
  APPLICATION_IDLE = 0,
  APPLICATION_INIT,
  APPLICATION_RUNNING,
  APPLICATION_SD_UNPLUGGED,
}FS_FileOperationsTypeDef;

FS_FileOperationsTypeDef Appli_state_uSD = APPLICATION_IDLE;
uint32_t is_initialized = 0;
int32_t ProcessStatus = 0;
static int32_t FS_FileOperations(void);
static void SD_Init(void);
static uint8_t workBuffer[_MAX_SS]; /* a work buffer for the f_mkfs() */

volatile  int32_t process_res = APP_OK;

static int32_t FS_FileOperations(void)
{
  FRESULT res; /* FatFs function common result code */
  uint32_t byteswritten, bytesread; /* File write/read counts */
  uint8_t wtext[] = "This is STM32 working with FatFs and uSD diskio driver"; /* File write buffer */
  uint8_t rtext[100]; /* File read buffer */

  /* Register the file system object to the FatFs module */
  if(f_mount(&SDFatFS, (TCHAR const*)SDPath, 0) == FR_OK)
  {
	  res = f_open(&SDFile, "STM32.TXT", FA_CREATE_ALWAYS | FA_WRITE);
    /* Create and Open a new text file object with write access */
    if(res == FR_OK)
    {
      /* Write data to the text file */
      res = f_write(&SDFile, wtext, sizeof(wtext), (void *)&byteswritten);

      if((byteswritten > 0) && (res == FR_OK))
      {
        /* Close the open text file */
        f_close(&SDFile);

        /* Open the text file object with read access */
        if(f_open(&SDFile, "STM32.TXT", FA_READ) == FR_OK)
        {
          /* Read data from the text file */
          res = f_read(&SDFile, rtext, sizeof(rtext), (void *)&bytesread);

          if((bytesread > 0) && (res == FR_OK))
          {
            /* Close the open text file */
            f_close(&SDFile);

            /* Compare read data with the expected data */
            if((bytesread == byteswritten))
            {
             // if(Buffercmp((uint32_t *)rtext, (uint32_t *)wtext, sizeof(rtext)))
              {  /* Success of the demo: no error occurrence */
              return 0;
              }
            }
          }
        }
      }
    }
  }
  /* Error */
  return -1;
}
/* USER CODE END Variables */

void MX_FATFS_Init(void)
{
  /*## FatFS: Link the SD driver ###########################*/
  retSD = FATFS_LinkDriver(&SD_Driver, SDPath);

  /* USER CODE BEGIN Init */
  Appli_state_uSD = APPLICATION_INIT;
  /* additional user code for init */
  /* USER CODE END Init */
}

/**
  * @brief  Gets Time from RTC
  * @param  None
  * @retval Time in DWORD
  */
DWORD get_fattime(void)
{
    RTC_TimeTypeDef sTime = {0};
    RTC_DateTypeDef sDate = {0};

    /*
     * On STM32U5 (and H7): GetTime must be called before GetDate to
     * unlock the shadow register — otherwise Date returns a stale value.
     */
    HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);

    return ((DWORD)(sDate.Year + 20) << 25)
         | ((DWORD)sDate.Month        << 21)
         | ((DWORD)sDate.Date         << 16)
         | ((DWORD)sTime.Hours        << 11)
         | ((DWORD)sTime.Minutes      << 5)
         | ((DWORD)sTime.Seconds      >> 1);
}

/* USER CODE BEGIN Application */
void MX_FATFS_Process(void *argument)
{
  /* USER CODE BEGIN MX_FATFS_Process */
	Start_uSD_Test = 1;
  /* Mass Storage Application State Machine */
  while(1)
  {
	  if(Start_uSD_Test)
	  {
  switch(Appli_state_uSD)
  {
  case APPLICATION_INIT:
    if(BSP_SD_IsDetected() == SD_PRESENT)
    {
     // SD_Init();
#if FATFS_MKFS_ALLOWED
      FRESULT res;

      res = f_mkfs(SDPath, FM_FAT32, 0, workBuffer, sizeof(workBuffer));

      if (res != FR_OK)
      {
        process_res = APP_ERROR;
      }
      else
      {
        process_res = APP_INIT;
        Appli_state_uSD = APPLICATION_RUNNING;
      }
#else
      process_res = APP_INIT;
      Appli_state_uSD = APPLICATION_RUNNING;
#endif
    }
    else
    {
    Appli_state_uSD = APPLICATION_SD_UNPLUGGED;

    }

    break;
  case APPLICATION_RUNNING:
     // SD_Init();
      process_res = FS_FileOperations();
      Appli_state_uSD = APPLICATION_IDLE;
      Start_uSD_Test = process_res;
    break;

  case APPLICATION_SD_UNPLUGGED:
    process_res = APP_SD_UNPLUGGED;
    break;

  case APPLICATION_IDLE:
  default:
    break;
  }
  ProcessStatus = process_res;
  }
  osDelay(1000);
  }
  /* USER CODE END MX_FATFS_Process */
}

/**
  * @brief  BSP SD Initialization
  * @param  None
  * @retval None.
  */
static void __attribute__((unused)) SD_Init()
{
  if (!is_initialized)
  {
     BSP_SD_Init();

     is_initialized = 1;
  }
}

/* USER CODE END Application */
