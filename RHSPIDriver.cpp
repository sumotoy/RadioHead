// RHSPIDriver.cpp
//
// Copyright (C) 2014 Mike McCauley
// $Id: RHSPIDriver.cpp,v 1.9 2014/05/03 00:20:36 mikem Exp $

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

// Ugly hack for testing SPI.beginTransaction...
#if defined(SPI_HAS_TRANSACTION)
	#undef ATOMIC_BLOCK_START
	#undef ATOMIC_BLOCK_END
	#define ATOMIC_BLOCK_START 		SPI.beginTransaction(_spi._settings)
	#define ATOMIC_BLOCK_END   		SPI.endTransaction()
#endif

uint8_t RHSPIDriver::spiRead(uint8_t reg)
{
    uint8_t val;
    ATOMIC_BLOCK_START;
	#if defined(__MK20DX128__) || defined(__MK20DX256__) || defined(__MKL26Z64__)//teensy stuff
		digitalWriteFast(_slaveSelectPin, LOW);
		_spi.transfer(reg & ~RH_SPI_WRITE_MASK); // Send the address with the write mask off
		val = _spi.transfer(0); // The written value is ignored, reg value is read
		digitalWriteFast(_slaveSelectPin, HIGH);
	#else
		digitalWrite(_slaveSelectPin, LOW);
		_spi.transfer(reg & ~RH_SPI_WRITE_MASK); // Send the address with the write mask off
		val = _spi.transfer(0); // The written value is ignored, reg value is read
		digitalWrite(_slaveSelectPin, HIGH);
	#endif
    ATOMIC_BLOCK_END;
    return val;
}

uint8_t RHSPIDriver::spiWrite(uint8_t reg, uint8_t val)
{
    uint8_t status = 0;
    ATOMIC_BLOCK_START;
	#if defined(__MK20DX128__) || defined(__MK20DX256__) || defined(__MKL26Z64__)//teensy stuff
		digitalWriteFast(_slaveSelectPin, LOW);
		status = _spi.transfer(reg | RH_SPI_WRITE_MASK); // Send the address with the write mask on
		_spi.transfer(val); // New value follows
		digitalWriteFast(_slaveSelectPin, HIGH);
	#else
		digitalWrite(_slaveSelectPin, LOW);
		status = _spi.transfer(reg | RH_SPI_WRITE_MASK); // Send the address with the write mask on
		_spi.transfer(val); // New value follows
		digitalWrite(_slaveSelectPin, HIGH);
	#endif
    ATOMIC_BLOCK_END;
    return status;
}

uint8_t RHSPIDriver::spiBurstRead(uint8_t reg, uint8_t* dest, uint8_t len)
{
    uint8_t status = 0;
    ATOMIC_BLOCK_START;
	#if defined(__MK20DX128__) || defined(__MK20DX256__) || defined(__MKL26Z64__)//teensy stuff
		digitalWriteFast(_slaveSelectPin, LOW);
		status = _spi.transfer(reg & ~RH_SPI_WRITE_MASK); // Send the start address with the write mask off
		while (len--)
		*dest++ = _spi.transfer(0);
		digitalWriteFast(_slaveSelectPin, HIGH);
	#else
		digitalWrite(_slaveSelectPin, LOW);
		status = _spi.transfer(reg & ~RH_SPI_WRITE_MASK); // Send the start address with the write mask off
		while (len--)
		*dest++ = _spi.transfer(0);
		digitalWrite(_slaveSelectPin, HIGH);
	#endif
    ATOMIC_BLOCK_END;
    return status;
}

uint8_t RHSPIDriver::spiBurstWrite(uint8_t reg, const uint8_t* src, uint8_t len)
{
    uint8_t status = 0;
    ATOMIC_BLOCK_START;
	#if defined(__MK20DX128__) || defined(__MK20DX256__) || defined(__MKL26Z64__)//teensy stuff
		digitalWriteFast(_slaveSelectPin, LOW);
		status = _spi.transfer(reg | RH_SPI_WRITE_MASK); // Send the start address with the write mask on
		while (len--)
		_spi.transfer(*src++);
		digitalWriteFast(_slaveSelectPin, HIGH);
	#else
		digitalWrite(_slaveSelectPin, LOW);
		status = _spi.transfer(reg | RH_SPI_WRITE_MASK); // Send the start address with the write mask on
		while (len--)
		_spi.transfer(*src++);
		digitalWrite(_slaveSelectPin, HIGH);
	#endif
    ATOMIC_BLOCK_END;
    return status;
}