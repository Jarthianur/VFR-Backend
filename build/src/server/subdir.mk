################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/server/SocketImplBoost.cpp \
../src/server/TcpInterfaceImplBoost.cpp 

OBJS += \
./src/server/SocketImplBoost.o \
./src/server/TcpInterfaceImplBoost.o 

CPP_DEPS += \
./src/server/SocketImplBoost.d \
./src/server/TcpInterfaceImplBoost.d 


# Each subdirectory must supply rules for building sources it contributes
src/server/%.o: ../src/server/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	$(CXX) -std=c++1y $(BOOST_I) -O$(OPT_LVL) $(DBG) -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


