################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Thirdparty/SEGGER/SEGGER/SEGGER_RTT.c \
../Thirdparty/SEGGER/SEGGER/SEGGER_RTT_printf.c \
../Thirdparty/SEGGER/SEGGER/SEGGER_SYSVIEW.c 

S_UPPER_SRCS += \
../Thirdparty/SEGGER/SEGGER/SEGGER_RTT_ASM_ARMv7M.S 

OBJS += \
./Thirdparty/SEGGER/SEGGER/SEGGER_RTT.o \
./Thirdparty/SEGGER/SEGGER/SEGGER_RTT_ASM_ARMv7M.o \
./Thirdparty/SEGGER/SEGGER/SEGGER_RTT_printf.o \
./Thirdparty/SEGGER/SEGGER/SEGGER_SYSVIEW.o 

S_UPPER_DEPS += \
./Thirdparty/SEGGER/SEGGER/SEGGER_RTT_ASM_ARMv7M.d 

C_DEPS += \
./Thirdparty/SEGGER/SEGGER/SEGGER_RTT.d \
./Thirdparty/SEGGER/SEGGER/SEGGER_RTT_printf.d \
./Thirdparty/SEGGER/SEGGER/SEGGER_SYSVIEW.d 


# Each subdirectory must supply rules for building sources it contributes
Thirdparty/SEGGER/SEGGER/%.o Thirdparty/SEGGER/SEGGER/%.su Thirdparty/SEGGER/SEGGER/%.cyclo: ../Thirdparty/SEGGER/SEGGER/%.c Thirdparty/SEGGER/SEGGER/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I"C:/Users/shrey/STM32CubeIDE/MasteringRTOS/RTOS_workspace/STM32_FreeRTOS11_continuous_recording/Thirdparty/SEGGER/Config" -I"C:/Users/shrey/STM32CubeIDE/MasteringRTOS/RTOS_workspace/STM32_FreeRTOS11_continuous_recording/Thirdparty/SEGGER/OS" -I"C:/Users/shrey/STM32CubeIDE/MasteringRTOS/RTOS_workspace/STM32_FreeRTOS11_continuous_recording/Thirdparty/SEGGER/SEGGER" -I"C:/Users/shrey/STM32CubeIDE/MasteringRTOS/RTOS_workspace/STM32_FreeRTOS11_continuous_recording/Thirdparty/FreeRTOS" -I"C:/Users/shrey/STM32CubeIDE/MasteringRTOS/RTOS_workspace/STM32_FreeRTOS11_continuous_recording/Thirdparty/FreeRTOS/include" -I"C:/Users/shrey/STM32CubeIDE/MasteringRTOS/RTOS_workspace/STM32_FreeRTOS11_continuous_recording/Thirdparty/FreeRTOS/portable/GCC/ARM_CM4F" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Thirdparty/SEGGER/SEGGER/%.o: ../Thirdparty/SEGGER/SEGGER/%.S Thirdparty/SEGGER/SEGGER/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m4 -g3 -DDEBUG -c -I"C:/Users/shrey/STM32CubeIDE/MasteringRTOS/RTOS_workspace/STM32_FreeRTOS11_continuous_recording/Thirdparty/SEGGER/Config" -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

clean: clean-Thirdparty-2f-SEGGER-2f-SEGGER

clean-Thirdparty-2f-SEGGER-2f-SEGGER:
	-$(RM) ./Thirdparty/SEGGER/SEGGER/SEGGER_RTT.cyclo ./Thirdparty/SEGGER/SEGGER/SEGGER_RTT.d ./Thirdparty/SEGGER/SEGGER/SEGGER_RTT.o ./Thirdparty/SEGGER/SEGGER/SEGGER_RTT.su ./Thirdparty/SEGGER/SEGGER/SEGGER_RTT_ASM_ARMv7M.d ./Thirdparty/SEGGER/SEGGER/SEGGER_RTT_ASM_ARMv7M.o ./Thirdparty/SEGGER/SEGGER/SEGGER_RTT_printf.cyclo ./Thirdparty/SEGGER/SEGGER/SEGGER_RTT_printf.d ./Thirdparty/SEGGER/SEGGER/SEGGER_RTT_printf.o ./Thirdparty/SEGGER/SEGGER/SEGGER_RTT_printf.su ./Thirdparty/SEGGER/SEGGER/SEGGER_SYSVIEW.cyclo ./Thirdparty/SEGGER/SEGGER/SEGGER_SYSVIEW.d ./Thirdparty/SEGGER/SEGGER/SEGGER_SYSVIEW.o ./Thirdparty/SEGGER/SEGGER/SEGGER_SYSVIEW.su

.PHONY: clean-Thirdparty-2f-SEGGER-2f-SEGGER

