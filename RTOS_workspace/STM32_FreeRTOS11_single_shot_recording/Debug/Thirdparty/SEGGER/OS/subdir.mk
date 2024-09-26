################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Thirdparty/SEGGER/OS/SEGGER_SYSVIEW_FreeRTOS.c 

OBJS += \
./Thirdparty/SEGGER/OS/SEGGER_SYSVIEW_FreeRTOS.o 

C_DEPS += \
./Thirdparty/SEGGER/OS/SEGGER_SYSVIEW_FreeRTOS.d 


# Each subdirectory must supply rules for building sources it contributes
Thirdparty/SEGGER/OS/%.o Thirdparty/SEGGER/OS/%.su Thirdparty/SEGGER/OS/%.cyclo: ../Thirdparty/SEGGER/OS/%.c Thirdparty/SEGGER/OS/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I"C:/Users/shrey/STM32CubeIDE/RTOS_workspace/STM32_FreeRTOS11_single_shot_recording/Thirdparty/SEGGER/Config" -I"C:/Users/shrey/STM32CubeIDE/RTOS_workspace/STM32_FreeRTOS11_single_shot_recording/Thirdparty/SEGGER/OS" -I"C:/Users/shrey/STM32CubeIDE/RTOS_workspace/STM32_FreeRTOS11_single_shot_recording/Thirdparty/SEGGER/SEGGER" -I"C:/Users/shrey/STM32CubeIDE/RTOS_workspace/STM32_FreeRTOS11_single_shot_recording/Thirdparty/FreeRTOS" -I"C:/Users/shrey/STM32CubeIDE/RTOS_workspace/STM32_FreeRTOS11_single_shot_recording/Thirdparty/FreeRTOS/include" -I"C:/Users/shrey/STM32CubeIDE/RTOS_workspace/STM32_FreeRTOS11_single_shot_recording/Thirdparty/FreeRTOS/portable/GCC/ARM_CM4F" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Thirdparty-2f-SEGGER-2f-OS

clean-Thirdparty-2f-SEGGER-2f-OS:
	-$(RM) ./Thirdparty/SEGGER/OS/SEGGER_SYSVIEW_FreeRTOS.cyclo ./Thirdparty/SEGGER/OS/SEGGER_SYSVIEW_FreeRTOS.d ./Thirdparty/SEGGER/OS/SEGGER_SYSVIEW_FreeRTOS.o ./Thirdparty/SEGGER/OS/SEGGER_SYSVIEW_FreeRTOS.su

.PHONY: clean-Thirdparty-2f-SEGGER-2f-OS

