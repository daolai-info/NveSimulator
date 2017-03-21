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
	g++ -I/usr/lib/jvm/java-8-oracle/ -I/usr/lib/jvm/java-8-oracle/include -I/usr/lib/jvm/java-8-oracle/include/linux -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


