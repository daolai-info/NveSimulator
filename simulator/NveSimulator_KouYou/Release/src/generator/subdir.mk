################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/generator/RandomGenerator.cpp 

OBJS += \
./src/generator/RandomGenerator.o 

CPP_DEPS += \
./src/generator/RandomGenerator.d 


# Each subdirectory must supply rules for building sources it contributes
src/generator/%.o: ../src/generator/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


