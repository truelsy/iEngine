################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/NCommand/AUTH.cpp \
../src/NCommand/CHAT.cpp 

OBJS += \
./src/NCommand/AUTH.o \
./src/NCommand/CHAT.o 

CPP_DEPS += \
./src/NCommand/AUTH.d \
./src/NCommand/CHAT.d 


# Each subdirectory must supply rules for building sources it contributes
src/NCommand/%.o: ../src/NCommand/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	/usr/bin/g++-4.8 -D__GXX_EXPERIMENTAL_CXX0X__ -I../../iEngine -I../include -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

