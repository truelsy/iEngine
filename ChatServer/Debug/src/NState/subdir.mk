################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/NState/AuthState.cpp \
../src/NState/ChatState.cpp \
../src/NState/State.cpp 

OBJS += \
./src/NState/AuthState.o \
./src/NState/ChatState.o \
./src/NState/State.o 

CPP_DEPS += \
./src/NState/AuthState.d \
./src/NState/ChatState.d \
./src/NState/State.d 


# Each subdirectory must supply rules for building sources it contributes
src/NState/%.o: ../src/NState/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -D__cplusplus=201103L -I../../iEngine -I../include -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


