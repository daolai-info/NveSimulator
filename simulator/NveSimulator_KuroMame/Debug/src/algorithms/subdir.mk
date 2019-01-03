################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/algorithms/KDisjoinShortestPaths.cpp \
../src/algorithms/KShortestPaths.cpp \
../src/algorithms/ShortestPath.cpp \
../src/algorithms/ShortestPathWithDijkstra.cpp \
../src/algorithms/ShortestPathWithFloyd.cpp 

OBJS += \
./src/algorithms/KDisjoinShortestPaths.o \
./src/algorithms/KShortestPaths.o \
./src/algorithms/ShortestPath.o \
./src/algorithms/ShortestPathWithDijkstra.o \
./src/algorithms/ShortestPathWithFloyd.o 

CPP_DEPS += \
./src/algorithms/KDisjoinShortestPaths.d \
./src/algorithms/KShortestPaths.d \
./src/algorithms/ShortestPath.d \
./src/algorithms/ShortestPathWithDijkstra.d \
./src/algorithms/ShortestPathWithFloyd.d 


# Each subdirectory must supply rules for building sources it contributes
src/algorithms/%.o: ../src/algorithms/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -I/usr/lib/jvm/java-8-oracle/ -I/usr/lib/jvm/java-8-oracle/include -I/usr/lib/jvm/java-8-oracle/include/linux -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


