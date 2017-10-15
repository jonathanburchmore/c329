################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
C:/Users/burch/arduino-1.0/libraries/SPI/SPI.cpp 

OBJS += \
./SPI/SPI.o 

CPP_DEPS += \
./SPI/SPI.d 


# Each subdirectory must supply rules for building sources it contributes
SPI/SPI.o: C:/Users/burch/arduino-1.0/libraries/SPI/SPI.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: AVR C++ Compiler'
	avr-g++ -I"C:\Users\burch\arduino-1.0\hardware\arduino\cores\arduino" -I"C:\Users\burch\arduino-1.0\hardware\arduino\variants\standard" -I"C:\Users\burch\Arduino\c329" -I"C:\Users\burch\arduino-1.0\libraries\SPI" -I"C:\Users\burch\arduino-1.0\libraries\Adafruit_GFX" -I"C:\Users\burch\arduino-1.0\libraries\Adafruit_ST7735" -D__IN_ECLIPSE__=1 -DARDUINO=100 -Wall -Os -ffunction-sections -fdata-sections -fno-exceptions -g -mmcu=atmega328p -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" -x c++ "$<"
	@echo 'Finished building: $<'
	@echo ' '


