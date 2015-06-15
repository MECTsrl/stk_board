################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../example/src/BoardInit.c \
../example/src/STKfw.c \
../example/src/cr_startup_lpc15xx.c \
../example/src/sysinit.c 

OBJS += \
./example/src/BoardInit.o \
./example/src/STKfw.o \
./example/src/cr_startup_lpc15xx.o \
./example/src/sysinit.o 

C_DEPS += \
./example/src/BoardInit.d \
./example/src/STKfw.d \
./example/src/cr_startup_lpc15xx.d \
./example/src/sysinit.d 


# Each subdirectory must supply rules for building sources it contributes
example/src/%.o: ../example/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DDEBUG -D__CODE_RED -D__USE_LPCOPEN -DCORE_M3 -I"C:\Progetti\DemoKit\stk_board\lpc_chip_15xx\inc" -O0 -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


