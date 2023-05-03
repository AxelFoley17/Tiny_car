################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../startup/startup_lpc845.c 

C_DEPS += \
./startup/startup_lpc845.d 

OBJS += \
./startup/startup_lpc845.o 


# Each subdirectory must supply rules for building sources it contributes
startup/%.o: ../startup/%.c startup/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_LPC845M301JBD48 -DCPU_LPC845M301JBD48_cm0plus -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"F:\1study\6f\Mikro\_project\Tiny_car\code_mcuxpresso_ws\kisauto\board" -I"F:\1study\6f\Mikro\_project\Tiny_car\code_mcuxpresso_ws\kisauto\source" -I"F:\1study\6f\Mikro\_project\Tiny_car\code_mcuxpresso_ws\kisauto\drivers" -I"F:\1study\6f\Mikro\_project\Tiny_car\code_mcuxpresso_ws\kisauto\CMSIS" -I"F:\1study\6f\Mikro\_project\Tiny_car\code_mcuxpresso_ws\kisauto\device" -I"F:\1study\6f\Mikro\_project\Tiny_car\code_mcuxpresso_ws\kisauto\component\uart" -I"F:\1study\6f\Mikro\_project\Tiny_car\code_mcuxpresso_ws\kisauto\utilities" -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-startup

clean-startup:
	-$(RM) ./startup/startup_lpc845.d ./startup/startup_lpc845.o

.PHONY: clean-startup

