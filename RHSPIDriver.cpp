// RHSPIDriver.cpp
//
// Copyright (C) 2014 Mike McCauley
// $Id: RHSPIDriver.cpp,v 1.10 2015/12/16 04:55:33 mikem Exp $

#include <RHSPIDriver.h>

RHSPIDriver::RHSPIDriver(uint8_t slaveSelectPin, RHGenericSPI& spi)
    : 
    _spi(spi),
    _slaveSelectPin(slaveSelectPin)
{
}

bool RHSPIDriver::init()
{
    // start the SPI library with the default speeds etc:
    // On Arduino Due this defaults to SPI1 on the central group of 6 SPI pins
    _spi.begin();

    // Initialise the slave select pin
    // On Maple, this must be _after_ spi.begin
    pinMode(_slaveSelectPin, OUTPUT);
    digitalWrite(_slaveSelectPin, HIGH);

    delay(100);
    return true;
}

void RHSPIDriver::startTransaction(void)
{
	#if defined(SPI_HAS_TRANSACTION)
		SPI.beginTransaction(_spi._settings);
	#else
		ATOMIC_BLOCK_START
	#endif
	#if (RH_PLATFORM == RH_PLATFORM_TEENSY)
		digitalWriteFast(_slaveSelectPin, LOW);
	#else
		digitalWrite(_slaveSelectPin, LOW);
	#endif
}

void RHSPIDriver::endTransaction(void)
{
	#if (RH_PLATFORM == RH_PLATFORM_TEENSY)
		digitalWriteFast(_slaveSelectPin, HIGH);
	#else
		digitalWrite(_slaveSelectPin, HIGH);
	#endif
	#if defined(SPI_HAS_TRANSACTION)
		SPI.endTransaction();
	#else
		ATOMIC_BLOCK_END
	#endif
}

uint8_t RHSPIDriver::spiRead(uint8_t reg)
{
    uint8_t val;
	startTransaction();
		_spi.transfer(reg & ~RH_SPI_WRITE_MASK); // Send the address with the write mask off
		val = _spi.transfer(0); // The written value is ignored, reg value is read
	endTransaction();
	return val;
}

uint8_t RHSPIDriver::spiWrite(uint8_t reg, uint8_t val)
{
    uint8_t status = 0;
	startTransaction();
		status = _spi.transfer(reg | RH_SPI_WRITE_MASK); // Send the address with the write mask on
		_spi.transfer(val); // New value follows
	endTransaction();
	return status;
}

uint8_t RHSPIDriver::spiBurstRead(uint8_t reg, uint8_t* dest, uint8_t len)
{
    uint8_t status = 0;
	startTransaction();
		status = _spi.transfer(reg & ~RH_SPI_WRITE_MASK); // Send the start address with the write mask off
		while (len--)
		*dest++ = _spi.transfer(0);
	endTransaction();
	return status;
}

uint8_t RHSPIDriver::spiBurstWrite(uint8_t reg, const uint8_t* src, uint8_t len)
{
    uint8_t status = 0;
	startTransaction();
		status = _spi.transfer(reg | RH_SPI_WRITE_MASK); // Send the start address with the write mask on
		while (len--)
		_spi.transfer(*src++);
	endTransaction();
	return status;
}
void RHSPIDriver::setSlaveSelectPin(uint8_t slaveSelectPin)
{
    _slaveSelectPin = slaveSelectPin;
}