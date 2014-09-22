################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../NPacket/Packet.cpp \
../NPacket/Pool.cpp 

OBJS += \
./NPacket/Packet.o \
./NPacket/Pool.o 

CPP_DEPS += \
./NPacket/Packet.d \
./NPacket/Pool.d 


# Each subdirectory must supply rules for building sources it contributes
NPacket/%.o: ../NPacket/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -D__cplusplus=201103L -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


