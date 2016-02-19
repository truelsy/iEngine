################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../NHandler/Handler.cpp \
../NHandler/Static.cpp 

OBJS += \
./NHandler/Handler.o \
./NHandler/Static.o 

CPP_DEPS += \
./NHandler/Handler.d \
./NHandler/Static.d 


# Each subdirectory must supply rules for building sources it contributes
NHandler/%.o: ../NHandler/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	/usr/bin/g++-4.8 -D__GXX_EXPERIMENTAL_CXX0X__ -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


