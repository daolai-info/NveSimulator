################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/embed/EndUserRequest.cpp \
../src/embed/VirtualNetworkEmbedding.cpp \
../src/embed/VirtualNetworkRequest.cpp 

OBJS += \
./src/embed/EndUserRequest.o \
./src/embed/VirtualNetworkEmbedding.o \
./src/embed/VirtualNetworkRequest.o 

CPP_DEPS += \
./src/embed/EndUserRequest.d \
./src/embed/VirtualNetworkEmbedding.d \
./src/embed/VirtualNetworkRequest.d 


# Each subdirectory must supply rules for building sources it contributes
src/embed/%.o: ../src/embed/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -I/usr/lib/jvm/java-8-oracle/ -I/usr/lib/jvm/java-8-oracle/include -I/usr/lib/jvm/java-8-oracle/include/linux -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


