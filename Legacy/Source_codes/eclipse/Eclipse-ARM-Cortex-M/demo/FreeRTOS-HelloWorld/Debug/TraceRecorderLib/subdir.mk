################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../TraceRecorderLib/trcKernelPort.c \
../TraceRecorderLib/trcSnapshotRecorder.c \
../TraceRecorderLib/trcStreamingRecorder.c 

OBJS += \
./TraceRecorderLib/trcKernelPort.o \
./TraceRecorderLib/trcSnapshotRecorder.o \
./TraceRecorderLib/trcStreamingRecorder.o 

C_DEPS += \
./TraceRecorderLib/trcKernelPort.d \
./TraceRecorderLib/trcSnapshotRecorder.d \
./TraceRecorderLib/trcStreamingRecorder.d 


# Each subdirectory must supply rules for building sources it contributes
TraceRecorderLib/%.o: ../TraceRecorderLib/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DSTM32F446xx -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -I"../include" -I"E:\Workspace\EclipseWorkspace\demo\FreeRTOS-HelloWorld\TraceRecorderLib\include" -I"E:\Workspace\EclipseWorkspace\demo\FreeRTOS-HelloWorld\TraceRecorderLib\include" -I"E:\Workspace\EclipseWorkspace\demo\FreeRTOS-HelloWorld\FreeRTOSv9.0\Source\portable\GCC\ARM_CM4F" -I"E:\Workspace\EclipseWorkspace\demo\FreeRTOS-HelloWorld\config" -I"E:\Workspace\EclipseWorkspace\demo\FreeRTOS-HelloWorld\FreeRTOSv9.0\Source\include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f4xx" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


