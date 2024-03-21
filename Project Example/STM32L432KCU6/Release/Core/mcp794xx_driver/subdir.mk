################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/mcp794xx_driver/mcp794xx_driver.c \
../Core/mcp794xx_driver/mcp794xx_driver_basic.c \
../Core/mcp794xx_driver/mcp794xx_driver_interface.c 

OBJS += \
./Core/mcp794xx_driver/mcp794xx_driver.o \
./Core/mcp794xx_driver/mcp794xx_driver_basic.o \
./Core/mcp794xx_driver/mcp794xx_driver_interface.o 

C_DEPS += \
./Core/mcp794xx_driver/mcp794xx_driver.d \
./Core/mcp794xx_driver/mcp794xx_driver_basic.d \
./Core/mcp794xx_driver/mcp794xx_driver_interface.d 


# Each subdirectory must supply rules for building sources it contributes
Core/mcp794xx_driver/%.o Core/mcp794xx_driver/%.su: ../Core/mcp794xx_driver/%.c Core/mcp794xx_driver/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32L432xx -c -I../Core/Inc -I"E:/Library Master/mcp794xx External RTC/Project Example/STM32L432KCU6/Core/mcp794xx_driver" -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-mcp794xx_driver

clean-Core-2f-mcp794xx_driver:
	-$(RM) ./Core/mcp794xx_driver/mcp794xx_driver.d ./Core/mcp794xx_driver/mcp794xx_driver.o ./Core/mcp794xx_driver/mcp794xx_driver.su ./Core/mcp794xx_driver/mcp794xx_driver_basic.d ./Core/mcp794xx_driver/mcp794xx_driver_basic.o ./Core/mcp794xx_driver/mcp794xx_driver_basic.su ./Core/mcp794xx_driver/mcp794xx_driver_interface.d ./Core/mcp794xx_driver/mcp794xx_driver_interface.o ./Core/mcp794xx_driver/mcp794xx_driver_interface.su

.PHONY: clean-Core-2f-mcp794xx_driver

