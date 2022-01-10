################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/home/remy/Documents/MSE/DeSem/DesenetLaboratory/desenet-work/work/desenet-sensor/src/common/platform/nucleo-stm32l476rg/mcu/critical/critical.c 

C_DEPS += \
./desenet-common/platform/nucleo-stm32l476rg/mcu/critical/critical.d 

OBJS += \
./desenet-common/platform/nucleo-stm32l476rg/mcu/critical/critical.o 


# Each subdirectory must supply rules for building sources it contributes
desenet-common/platform/nucleo-stm32l476rg/mcu/critical/critical.o: /home/remy/Documents/MSE/DeSem/DesenetLaboratory/desenet-work/work/desenet-sensor/src/common/platform/nucleo-stm32l476rg/mcu/critical/critical.c desenet-common/platform/nucleo-stm32l476rg/mcu/critical/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L476xx -DIDE_STM32CUBEIDE -DTC_STM32CUBEIDE -DUSE_DISPLAY -c -I"/home/remy/Documents/MSE/DeSem/DesenetLaboratory/desenet-work/work/desenet-sensor/ide-stm32cubeide/.." -I"/home/remy/Documents/MSE/DeSem/DesenetLaboratory/desenet-work/work/desenet-sensor/ide-stm32cubeide/../src" -I"/home/remy/Documents/MSE/DeSem/DesenetLaboratory/desenet-work/work/desenet-sensor/ide-stm32cubeide/platform/nucleo-stm32l476rg" -I"/home/remy/Documents/MSE/DeSem/DesenetLaboratory/desenet-work/work/desenet-sensor/src/common/platform/nucleo-stm32l476rg" -I"/home/remy/Documents/MSE/DeSem/DesenetLaboratory/desenet-work/work/desenet-sensor/src/common/platform/nucleo-stm32l476rg/mcu" -I"/home/remy/Documents/MSE/DeSem/DesenetLaboratory/desenet-work/work/desenet-sensor/src/common/platform/nucleo-stm32l476rg/board" -I"/home/remy/Documents/MSE/DeSem/DesenetLaboratory/desenet-work/work/desenet-sensor/src/common/platform/platform-common" -I"/home/remy/Documents/MSE/DeSem/DesenetLaboratory/desenet-work/work/desenet-sensor/src/common/platform/platform-common/board" -I"/home/remy/Documents/MSE/DeSem/DesenetLaboratory/desenet-work/work/desenet-sensor/src/common/platform" -I"/home/remy/Documents/MSE/DeSem/DesenetLaboratory/desenet-work/work/desenet-sensor/src/common/mdw" -I"/home/remy/Documents/MSE/DeSem/DesenetLaboratory/desenet-work/work/desenet-sensor/src/common/mdw/desenet" -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

