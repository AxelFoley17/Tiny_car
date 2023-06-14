################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/SysTimer.c \
../source/fixSpeed.c \
../source/kisauto.c \
../source/letsdothis.c \
../source/mtb.c \
../source/semihost_hardfault.c \
../source/sys.c \
../source/timeBetween.c 

C_DEPS += \
./source/SysTimer.d \
./source/fixSpeed.d \
./source/kisauto.d \
./source/letsdothis.d \
./source/mtb.d \
./source/semihost_hardfault.d \
./source/sys.d \
./source/timeBetween.d 

OBJS += \
./source/SysTimer.o \
./source/fixSpeed.o \
./source/kisauto.o \
./source/letsdothis.o \
./source/mtb.o \
./source/semihost_hardfault.o \
./source/sys.o \
./source/timeBetween.o 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c source/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_LPC845M301JBD48 -DCPU_LPC845M301JBD48_cm0plus -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"F:\1study\6f\Mikro\_project\Tiny_car\code_mcuxpresso_ws\kisauto\board" -I"F:\1study\6f\Mikro\_project\Tiny_car\code_mcuxpresso_ws\kisauto\source" -I"F:\1study\6f\Mikro\_project\Tiny_car\code_mcuxpresso_ws\kisauto\drivers" -I"F:\1study\6f\Mikro\_project\Tiny_car\code_mcuxpresso_ws\kisauto\CMSIS" -I"F:\1study\6f\Mikro\_project\Tiny_car\code_mcuxpresso_ws\kisauto\device" -I"F:\1study\6f\Mikro\_project\Tiny_car\code_mcuxpresso_ws\kisauto\component\uart" -I"F:\1study\6f\Mikro\_project\Tiny_car\code_mcuxpresso_ws\kisauto\utilities" -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-source

clean-source:
	-$(RM) ./source/SysTimer.d ./source/SysTimer.o ./source/fixSpeed.d ./source/fixSpeed.o ./source/kisauto.d ./source/kisauto.o ./source/letsdothis.d ./source/letsdothis.o ./source/mtb.d ./source/mtb.o ./source/semihost_hardfault.d ./source/semihost_hardfault.o ./source/sys.d ./source/sys.o ./source/timeBetween.d ./source/timeBetween.o

.PHONY: clean-source

