// RHHardwareSPI.h
// Author: Mike McCauley (mikem@airspayce.com)
// Copyright (C) 2011 Mike McCauley
// Contributed by Joanna Rutkowska
// $Id: RHHardwareSPI.cpp,v 1.12 2015/07/01 00:46:05 mikem Exp $

#include <RHHardwareSPI.h>

// Declare a single default instance of the hardware SPI interface class
RHHardwareSPI hardware_spi;

#ifdef RH_HAVE_HARDWARE_SPI

#if (RH_PLATFORM == RH_PLATFORM_STM32) // Maple etc
// Declare an SPI interface to use
HardwareSPI SPI(1);
#elif (RH_PLATFORM == RH_PLATFORM_STM32STD) // STM32F4 Discovery
// Declare an SPI interface to use
HardwareSPI SPI(1);
#endif

// Arduino Due has default SPI pins on central SPI headers, and not on 10, 11, 12, 13
// as per otherArduinos
// http://21stdigitalhome.blogspot.com.au/2013/02/arduino-due-hardware-spi.html
#if defined (__arm__) && !defined(CORE_TEENSY)
 // Arduino Due in 1.5.5 has no definitions for SPI dividers
 // SPI clock divider is based on MCK of 84MHz  
 #define SPI_CLOCK_DIV16 (VARIANT_MCK/84000000) // 1MHz
 #define SPI_CLOCK_DIV8  (VARIANT_MCK/42000000) // 2MHz
 #define SPI_CLOCK_DIV4  (VARIANT_MCK/21000000) // 4MHz
 #define SPI_CLOCK_DIV2  (VARIANT_MCK/10500000) // 8MHz
 #define SPI_CLOCK_DIV1  (VARIANT_MCK/5250000)  // 16MHz
#endif

RHHardwareSPI::RHHardwareSPI(Frequency frequency, BitOrder bitOrder, DataMode dataMode)
    :
    RHGenericSPI(frequency, bitOrder, dataMode)
{
}

uint8_t RHHardwareSPI::transfer(uint8_t data) 
{
    return SPI.transfer(data);
}

void RHHardwareSPI::attachInterrupt() 
{
#if (RH_PLATFORM == RH_PLATFORM_ARDUINO) || (RH_PLATFORM == RH_PLATFORM_TEENSY)
    SPI.attachInterrupt();
#endif
}

void RHHardwareSPI::detachInterrupt() 
{
#if (RH_PLATFORM == RH_PLATFORM_ARDUINO) || (RH_PLATFORM == RH_PLATFORM_TEENSY)
    SPI.detachInterrupt();
#endif
}
    
void RHHardwareSPI::begin() 
{
    // Sigh: there are no common symbols for some of these SPI options across all platforms
#if (RH_PLATFORM == RH_PLATFORM_ARDUINO) || (RH_PLATFORM == RH_PLATFORM_UNO32) || (RH_PLATFORM == RH_PLATFORM_TEENSY) || (RH_PLATFORM == RH_PLATFORM_ESP8266)
    uint8_t dataMode;
	uint8_t bOrder;
	
    if (_dataMode == DataMode0) {
		dataMode = SPI_MODE0;
    } else if (_dataMode == DataMode1) {
		dataMode = SPI_MODE1;
    } else if (_dataMode == DataMode2) {
		dataMode = SPI_MODE2;
    } else if (_dataMode == DataMode3) {
		dataMode = SPI_MODE3;
    } else {
		dataMode = SPI_MODE0;
	}
	
	if (RH_PLATFORM == RH_PLATFORM_ESP8266 && dataMode != SPI_MODE0){
		//ESP8266 cannot work if dataMode is not SPI_MODE0!
		return;
	}
	
	if (_bitOrder == BitOrderLSBFirst) {
		bOrder = LSBFIRST;
	} else {
		bOrder = MSBFIRST;
	}
	//#if (RH_PLATFORM == RH_PLATFORM_ARDUINO) && defined(__arm__) && defined(CORE_TEENSY)
	#if defined(SPI_HAS_TRANSACTION)
	//#if ((RH_PLATFORM == RH_PLATFORM_ARDUINO) && defined(SPI_HAS_TRANSACTION)) || (RH_PLATFORM == RH_PLATFORM_TEENSY) || (RH_PLATFORM == RH_PLATFORM_ESP8266)
		uint32_t frequency32;
		if (_frequency == Frequency16MHz) {
			frequency32 = 16000000;
		} else if (_frequency == Frequency8MHz) {
			frequency32 = 8000000;
		} else if (_frequency == Frequency4MHz) {
			frequency32 = 4000000;
		} else if (_frequency == Frequency2MHz) {
			frequency32 = 2000000;
		} else {
			frequency32 = 1000000;
		}
		_settings = SPISettings(frequency32, bOrder, dataMode);
	#else//no SPI_HAS_TRANSACTION
		uint8_t divider;
		#if (RH_PLATFORM == RH_PLATFORM_TEENSY)
			// Temporary work-around due to problem where avr_emulation.h does not work properly for the setDataMode() cal
			SPCR &= ~SPI_MODE_MASK;
		#else
			
		#endif
		#if (RH_PLATFORM == RH_PLATFORM_ARDUINO) && defined (__arm__) && !defined(CORE_TEENSY)
			// Arduino Due in 1.5.5 has its own BitOrder :-(
			::BitOrder bitOrder;
		#else
			uint8_t bitOrder;
		#endif
			
			switch (_frequency)
			{
			case Frequency1MHz:
			default:
			#if F_CPU == 8000000
				divider = SPI_CLOCK_DIV8;
			#else
				divider = SPI_CLOCK_DIV16;
			#endif
				break;

			case Frequency2MHz:
			#if F_CPU == 8000000
				divider = SPI_CLOCK_DIV4;
			#else
				divider = SPI_CLOCK_DIV8;
			#endif
				break;

			case Frequency4MHz:
			#if F_CPU == 8000000
				divider = SPI_CLOCK_DIV2;
			#else
				divider = SPI_CLOCK_DIV4;
			#endif
				break;

			case Frequency8MHz:
				divider = SPI_CLOCK_DIV2; // 4MHz on an 8MHz Arduino
				break;

			case Frequency16MHz:
				divider = SPI_CLOCK_DIV2; // Not really 16MHz, only 8MHz. 4MHz on an 8MHz Arduino
				break;

			}
			SPI.setDataMode(dataMode);
			SPI.setBitOrder(bitOrder);
			SPI.setClockDivider(divider);
	#endif//no SPI Transactions
    SPI.begin();
#elif (RH_PLATFORM == RH_PLATFORM_STM32) // Maple etc
    spi_mode dataMode;
    // Hmmm, if we do this as a switch, GCC on maple gets v confused!
    if (_dataMode == DataMode0)
	dataMode = SPI_MODE_0;
    else if (_dataMode == DataMode1)
	dataMode = SPI_MODE_1;
    else if (_dataMode == DataMode2)
	dataMode = SPI_MODE_2;
    else if (_dataMode == DataMode3)
	dataMode = SPI_MODE_3;
    else
	dataMode = SPI_MODE_0;

    uint32 bitOrder;
    if (_bitOrder == BitOrderLSBFirst)
	bitOrder = LSBFIRST;
    else
	bitOrder = MSBFIRST;

    SPIFrequency frequency; // Yes, I know these are not exact equivalents.
    switch (_frequency)
    {
	case Frequency1MHz:
	default:
	    frequency = SPI_1_125MHZ;
	    break;

	case Frequency2MHz:
	    frequency = SPI_2_25MHZ;
	    break;

	case Frequency4MHz:
	    frequency = SPI_4_5MHZ;
	    break;

	case Frequency8MHz:
	    frequency = SPI_9MHZ;
	    break;

	case Frequency16MHz:
	    frequency = SPI_18MHZ;
	    break;

    }
    SPI.begin(frequency, bitOrder, dataMode);

#elif (RH_PLATFORM == RH_PLATFORM_STM32STD) // STM32F4 discovery
    uint8_t dataMode;
    if (_dataMode == DataMode0)
	dataMode = SPI_MODE0;
    else if (_dataMode == DataMode1)
	dataMode = SPI_MODE1;
    else if (_dataMode == DataMode2)
	dataMode = SPI_MODE2;
    else if (_dataMode == DataMode3)
	dataMode = SPI_MODE3;
    else
	dataMode = SPI_MODE0;

    uint32_t bitOrder;
    if (_bitOrder == BitOrderLSBFirst)
	bitOrder = LSBFIRST;
    else
	bitOrder = MSBFIRST;

    SPIFrequency frequency; // Yes, I know these are not exact equivalents.
    switch (_frequency)
    {
	case Frequency1MHz:
	default:
	    frequency = SPI_1_3125MHZ;
	    break;

	case Frequency2MHz:
	    frequency = SPI_2_625MHZ;
	    break;

	case Frequency4MHz:
	    frequency = SPI_5_25MHZ;
	    break;

	case Frequency8MHz:
	    frequency = SPI_10_5MHZ;
	    break;

	case Frequency16MHz:
	    frequency = SPI_21_0MHZ;
	    break;

    }
    SPI.begin(frequency, bitOrder, dataMode);
#elif (RH_PLATFORM == RH_PLATFORM_RASPI) // Raspberry PI
	uint8_t dataMode;
	uint8_t bitOrder;
	uint32_t divider;
  
	if (_dataMode == DataMode0)
		dataMode = BCM2835_SPI_MODE0;
	else if (_dataMode == DataMode1)
		dataMode = BCM2835_SPI_MODE1;
	else if (_dataMode == DataMode2)
		dataMode = BCM2835_SPI_MODE2;
	else if (_dataMode == DataMode3)
		dataMode = BCM2835_SPI_MODE3;

	if (_bitOrder == BitOrderLSBFirst)
		bitOrder = BCM2835_SPI_BIT_ORDER_LSBFIRST;
	else
		bitOrder = BCM2835_SPI_BIT_ORDER_MSBFIRST;

	switch (_frequency)
	{
		case Frequency1MHz:
		default:
		divider = BCM2835_SPI_CLOCK_DIVIDER_256;
		break;
		case Frequency2MHz:
		divider = BCM2835_SPI_CLOCK_DIVIDER_128;
		break;
		case Frequency4MHz:
		divider = BCM2835_SPI_CLOCK_DIVIDER_64;
		break;
		case Frequency8MHz:
		divider = BCM2835_SPI_CLOCK_DIVIDER_32;
		break;
		case Frequency16MHz:
		divider = BCM2835_SPI_CLOCK_DIVIDER_16;
		break;
	}
	SPI.begin(divider, bitOrder, dataMode);
#else
	#warning RHHardwareSPI does not support this platform yet. Consider adding it and contributing a patch.
#endif
}

void RHHardwareSPI::end() 
{
    return SPI.end();
}

#endif

