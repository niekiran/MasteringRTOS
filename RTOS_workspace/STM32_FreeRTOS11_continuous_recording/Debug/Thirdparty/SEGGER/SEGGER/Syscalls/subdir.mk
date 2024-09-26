################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Thirdparty/SEGGER/SEGGER/Syscalls/SEGGER_RTT_Syscalls_GCC.c 

OBJS += \
./Thirdparty/SEGGER/SEGGER/Syscalls/SEGGER_RTT_Syscalls_GCC.o 

C_DEPS += \
./Thirdparty/SEGGER/SEGGER/Syscalls/SEGGER_RTT_Syscalls_GCC.d 


# Each subdirectory must supply rules for building sources it contributes
Thirdparty/SEGGER/SEGGER/Syscalls/%.o Thirdparty/SEGGER/SEGGER/Syscalls/%.su Thirdparty/SEGGER/SEGGER/Syscalls/%.cyclo: ../Thirdparty/SEGGER/SEGGER/Syscalls/%.c Thirdparty/SEGGER/SEGGER/Syscalls/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I"C:/Users/shrey/STM32CubeIDE/MasteringRTOS/RTOS_workspace/STM32_FreeRTOS11_continuous_recording/Thirdparty/SEGGER/Config" -I"C:/Users/shrey/STM32CubeIDE/MasteringRTOS/RTOS_workspace/STM32_FreeRTOS11_continuous_recording/Thirdparty/SEGGER/OS" -I"C:/Users/shrey/STM32CubeIDE/MasteringRTOS/RTOS_workspace/STM32_FreeRTOS11_continuous_recording/Thirdparty/SEGGER/SEGGER" -I"C:/Users/shrey/STM32CubeIDE/MasteringRTOS/RTOS_workspace/STM32_FreeRTOS11_continuous_recording/Thirdparty/FreeRTOS" -I"C:/Users/shrey/STM32CubeIDE/MasteringRTOS/RTOS_workspace/STM32_FreeRTOS11_continuous_recording/Thirdparty/FreeRTOS/include" -I"C:/Users/shrey/STM32CubeIDE/MasteringRTOS/RTOS_workspace/STM32_FreeRTOS11_continuous_recording/Thirdparty/FreeRTOS/portable/GCC/ARM_CM4F" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Thirdparty-2f-SEGGER-2f-SEGGER-2f-Syscalls

clean-Thirdparty-2f-SEGGER-2f-SEGGER-2f-Syscalls:
	-$(RM) ./Thirdparty/SEGGER/SEGGER/Syscalls/SEGGER_RTT_Syscalls_GCC.cyclo ./Thirdparty/SEGGER/SEGGER/Syscalls/SEGGER_RTT_Syscalls_GCC.d ./Thirdparty/SEGGER/SEGGER/Syscalls/SEGGER_RTT_Syscalls_GCC.o ./Thirdparty/SEGGER/SEGGER/Syscalls/SEGGER_RTT_Syscalls_GCC.su

.PHONY: clean-Thirdparty-2f-SEGGER-2f-SEGGER-2f-Syscalls

