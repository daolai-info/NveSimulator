################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/com/CommonConstants.cpp \
../src/com/CommonFunctions.cpp \
../src/com/MessageSet.cpp \
../src/com/Timer.cpp 

OBJS += \
./src/com/CommonConstants.o \
./src/com/CommonFunctions.o \
./src/com/MessageSet.o \
./src/com/Timer.o 

CPP_DEPS += \
./src/com/CommonConstants.d \
./src/com/CommonFunctions.d \
./src/com/MessageSet.d \
./src/com/Timer.d 


# Each subdirectory must supply rules for building sources it contributes
src/com/%.o: ../src/com/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


