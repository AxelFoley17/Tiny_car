################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/LPC845_Project_UH_tavmero.c \
../source/SysTimer.c \
../source/mtb.c \
../source/semihost_hardfault.c \
../source/sys.c 

C_DEPS += \
./source/LPC845_Project_UH_tavmero.d \
./source/SysTimer.d \
./source/mtb.d \
./source/semihost_hardfault.d \
./source/sys.d 

OBJS += \
./source/LPC845_Project_UH_tavmero.o \
./source/SysTimer.o \
./source/mtb.o \
./source/semihost_hardfault.o \
./source/sys.o 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c source/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_LPC845M301JBD48 -DCPU_LPC845M301JBD48_cm0plus -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"F:\MCU_WorkSpaces\tavmero_kodbol_jo_pin\LPC845_Project_UH_tavmero\board" -I"F:\MCU_WorkSpaces\tavmero_kodbol_jo_pin\LPC845_Project_UH_tavmero\source" -I"F:\MCU_WorkSpaces\tavmero_kodbol_jo_pin\LPC845_Project_UH_tavmero\drivers" -I"F:\MCU_WorkSpaces\tavmero_kodbol_jo_pin\LPC845_Project_UH_tavmero\CMSIS" -I"F:\MCU_WorkSpaces\tavmero_kodbol_jo_pin\LPC845_Project_UH_tavmero\device" -I"F:\MCU_WorkSpaces\tavmero_kodbol_jo_pin\LPC845_Project_UH_tavmero\component\uart" -I"F:\MCU_WorkSpaces\tavmero_kodbol_jo_pin\LPC845_Project_UH_tavmero\utilities" -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-source

clean-source:
	-$(RM) ./source/LPC845_Project_UH_tavmero.d ./source/LPC845_Project_UH_tavmero.o ./source/SysTimer.d ./source/SysTimer.o ./source/mtb.d ./source/mtb.o ./source/semihost_hardfault.d ./source/semihost_hardfault.o ./source/sys.d ./source/sys.o

.PHONY: clean-source

