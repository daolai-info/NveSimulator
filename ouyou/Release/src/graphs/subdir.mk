################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/graphs/BaseGraph.cpp \
../src/graphs/BaseLink.cpp \
../src/graphs/BaseNode.cpp \
../src/graphs/BasePath.cpp \
../src/graphs/Paths.cpp \
../src/graphs/SubNwGraph.cpp \
../src/graphs/SubNwLink.cpp \
../src/graphs/SubNwNode.cpp \
../src/graphs/VirNwGraph.cpp \
../src/graphs/VirNwLink.cpp \
../src/graphs/VirNwNode.cpp 

OBJS += \
./src/graphs/BaseGraph.o \
./src/graphs/BaseLink.o \
./src/graphs/BaseNode.o \
./src/graphs/BasePath.o \
./src/graphs/Paths.o \
./src/graphs/SubNwGraph.o \
./src/graphs/SubNwLink.o \
./src/graphs/SubNwNode.o \
./src/graphs/VirNwGraph.o \
./src/graphs/VirNwLink.o \
./src/graphs/VirNwNode.o 

CPP_DEPS += \
./src/graphs/BaseGraph.d \
./src/graphs/BaseLink.d \
./src/graphs/BaseNode.d \
./src/graphs/BasePath.d \
./src/graphs/Paths.d \
./src/graphs/SubNwGraph.d \
./src/graphs/SubNwLink.d \
./src/graphs/SubNwNode.d \
./src/graphs/VirNwGraph.d \
./src/graphs/VirNwLink.d \
./src/graphs/VirNwNode.d 


# Each subdirectory must supply rules for building sources it contributes
src/graphs/%.o: ../src/graphs/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


