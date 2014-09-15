################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../src/NMessage/ChatMessage.pb.cc 

OBJS += \
./src/NMessage/ChatMessage.pb.o 

CC_DEPS += \
./src/NMessage/ChatMessage.pb.d 


# Each subdirectory must supply rules for building sources it contributes
src/NMessage/%.o: ../src/NMessage/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	/usr/bin/g++-4.8 -D__GXX_EXPERIMENTAL_CXX0X__ -I../../iEngine -I../include -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


