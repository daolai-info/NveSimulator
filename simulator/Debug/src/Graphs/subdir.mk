################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Graphs/BaseGraph.cpp \
../src/Graphs/BaseLink.cpp \
../src/Graphs/BaseNode.cpp \
../src/Graphs/SubNwGraph.cpp \
../src/Graphs/SubNwLink.cpp \
../src/Graphs/SubNwNode.cpp \
../src/Graphs/VirNwGraph.cpp \
../src/Graphs/VirNwLink.cpp \
../src/Graphs/VirNwNode.cpp 

OBJS += \
./src/Graphs/BaseGraph.o \
./src/Graphs/BaseLink.o \
./src/Graphs/BaseNode.o \
./src/Graphs/SubNwGraph.o \
./src/Graphs/SubNwLink.o \
./src/Graphs/SubNwNode.o \
./src/Graphs/VirNwGraph.o \
./src/Graphs/VirNwLink.o \
./src/Graphs/VirNwNode.o 

CPP_DEPS += \
./src/Graphs/BaseGraph.d \
./src/Graphs/BaseLink.d \
./src/Graphs/BaseNode.d \
./src/Graphs/SubNwGraph.d \
./src/Graphs/SubNwLink.d \
./src/Graphs/SubNwNode.d \
./src/Graphs/VirNwGraph.d \
./src/Graphs/VirNwLink.d \
./src/Graphs/VirNwNode.d 


# Each subdirectory must supply rules for building sources it contributes
src/Graphs/%.o: ../src/Graphs/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


