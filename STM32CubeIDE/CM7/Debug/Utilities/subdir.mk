################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/wiemar001/Desktop/SD_CARD_10/SD_CARD_10/SD_Card_10STM32H7/Utilities/JPEG/jpeg_utils.c 

C_DEPS += \
./Utilities/jpeg_utils.d 

OBJS += \
./Utilities/jpeg_utils.o 


# Each subdirectory must supply rules for building sources it contributes
Utilities/jpeg_utils.o: C:/Users/wiemar001/Desktop/SD_CARD_10/SD_CARD_10/SD_Card_10STM32H7/Utilities/JPEG/jpeg_utils.c Utilities/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCORE_CM7 -DUSE_HAL_DRIVER -DSTM32H757xx -c -I../../../CM7/Core/Inc -I../../../CM7/FATFS/Target -I../../../CM7/FATFS/App -I../../../CM7/TouchGFX/App -I../../../CM7/TouchGFX/target/generated -I../../../CM7/TouchGFX/target -I../../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../../Utilities/JPEG -I../../../Middlewares/Third_Party/FatFs/src -I../../../Middlewares/Third_Party/FreeRTOS/Source/include -I../../../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../../../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../../Drivers/CMSIS/Include -I../../../Drivers/CMSIS/RTOS2/Include -I../../../CM7/USB_HOST/App -I../../../CM7/USB_HOST/Target -I../../../Middlewares/ST/STM32_USB_Host_Library/Core/Inc -I../../../Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Inc -I../../CM7/../../CM7/Middlewares/ST/touchgfx/framework/include -I../../CM7/../../CM7/TouchGFX/generated/fonts/include -I../../CM7/../../CM7/TouchGFX/generated/gui_generated/include -I../../CM7/../../CM7/TouchGFX/generated/images/include -I../../CM7/../../CM7/TouchGFX/generated/texts/include -I../../CM7/../../CM7/TouchGFX/generated/videos/include -I../../CM7/../../CM7/TouchGFX/gui/include -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Utilities

clean-Utilities:
	-$(RM) ./Utilities/jpeg_utils.cyclo ./Utilities/jpeg_utils.d ./Utilities/jpeg_utils.o ./Utilities/jpeg_utils.su

.PHONY: clean-Utilities

