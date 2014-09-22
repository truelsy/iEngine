################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/NLogic/Function.cpp 

OBJS += \
./src/NLogic/Function.o 

CPP_DEPS += \
./src/NLogic/Function.d 


# Each subdirectory must supply rules for building sources it contributes
src/NLogic/%.o: ../src/NLogic/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -D__cplusplus=201103L -I../../iEngine -I../include -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


