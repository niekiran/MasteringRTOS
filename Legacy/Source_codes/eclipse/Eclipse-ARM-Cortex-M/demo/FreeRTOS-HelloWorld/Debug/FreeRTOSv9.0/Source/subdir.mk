################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../FreeRTOSv9.0/Source/croutine.c \
../FreeRTOSv9.0/Source/event_groups.c \
../FreeRTOSv9.0/Source/list.c \
../FreeRTOSv9.0/Source/queue.c \
../FreeRTOSv9.0/Source/tasks.c \
../FreeRTOSv9.0/Source/timers.c 

OBJS += \
./FreeRTOSv9.0/Source/croutine.o \
./FreeRTOSv9.0/Source/event_groups.o \
./FreeRTOSv9.0/Source/list.o \
./FreeRTOSv9.0/Source/queue.o \
./FreeRTOSv9.0/Source/tasks.o \
./FreeRTOSv9.0/Source/timers.o 

C_DEPS += \
./FreeRTOSv9.0/Source/croutine.d \
./FreeRTOSv9.0/Source/event_groups.d \
./FreeRTOSv9.0/Source/list.d \
./FreeRTOSv9.0/Source/queue.d \
./FreeRTOSv9.0/Source/tasks.d \
./FreeRTOSv9.0/Source/timers.d 


# Each subdirectory must supply rules for building sources it contributes
FreeRTOSv9.0/Source/%.o: ../FreeRTOSv9.0/Source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DSTM32F446xx -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -I"../include" -I"E:\Workspace\EclipseWorkspace\demo\FreeRTOS-HelloWorld\TraceRecorderLib\include" -I"E:\Workspace\EclipseWorkspace\demo\FreeRTOS-HelloWorld\TraceRecorderLib\include" -I"E:\Workspace\EclipseWorkspace\demo\FreeRTOS-HelloWorld\FreeRTOSv9.0\Source\portable\GCC\ARM_CM4F" -I"E:\Workspace\EclipseWorkspace\demo\FreeRTOS-HelloWorld\config" -I"E:\Workspace\EclipseWorkspace\demo\FreeRTOS-HelloWorld\FreeRTOSv9.0\Source\include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f4xx" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


