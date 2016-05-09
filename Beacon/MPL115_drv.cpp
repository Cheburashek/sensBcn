/*
 * MPL115_drv.cpp
 *
 *  Created on: 7 maj 2016
 *      Author: Cheburashek
 */

// TODO : app error!
#include "MPL115_drv.h"
#include "microhal.h"
#include "microhal_bsp.h" // FIXME : not compilinig ('IODevice' does not name a type) when CEpin located in bsp

namespace microhal
{     // todo: NAMESPACE nrf52 ?

// From microhal_bsp.h
constexpr GPIO::IOPin CSpin ( nrf52::GPIO::Port0, 29 );
IODevice &console = nrf52::SerialPort::Serial0;
SPI &SPIM = nrf52::SPI::spi1;

// Constructor/destructor
MPL115::MPL115 ()
{
	spiDev = new SPIDevice ( nrf52::SPI::spi1, CSpin );     // TODO: move to bsp (problem)
	a0 = 0.0;
	b1 = 0.0;
	b2 = 0.0;
	c12 = 0.0;
}

MPL115::~MPL115 ()
{
}

void MPL115::init ( void )
{
	nrf52::SPI::spi1.init ();		// TODO : to be selectable with constructor
	nrf52::SPI::spi1.setMode ( SPI::Mode::Mode0 );
	nrf52::SPI::spi1.speed ( MPL_SPI_SPEED );
	GPIO CS ( CSpin, GPIO::Direction::Output );			// CS pin config
	nrf52::SPI::spi1.enable ();
	calibrate();

}

bool MPL115::startConv ( void )
{
	return spiDev->writeBuffer ( startBuff, sizeof(startBuff), true );
}

bool MPL115::readRawPress ( uint16_t *buff )
{
	bool retVal = false;
	uint8_t tempBuf;

	if ( spiDev->readRegister ( MPL_READ_PRESS_MSB, tempBuf ) ) 	// Pressure MSB
	{
		*buff = (static_cast<uint16_t> ( tempBuf )) << 2;		// 8 from 10bits
		retVal = spiDev->readRegister ( MPL_READ_PRESS_LSB, tempBuf );     // Pressure LSB -2bits
		*buff |= (tempBuf >> 6) & 0x03;
	}

	return retVal;
}

bool MPL115::readRawTemp ( uint16_t *buff )
{
	bool retVal = false;
	uint8_t tempBuf;

	if ( spiDev->readRegister ( MPL_READ_TEMP_MSB, tempBuf ) ) 	// Temperature MSB
	{
		*buff = (static_cast<uint16_t> ( tempBuf )) << 2;		// 8 from 10bits
		retVal = spiDev->readRegister ( MPL_READ_TEMP_LSB, tempBuf );     // Temperature LSB -2bits
		*buff |= (tempBuf >> 6) & 0x03;
	}

	return retVal;
}

// Private methods:

bool MPL115::calibrate ( void )
{
	uint8_t errBit = 0x00;
	uint8_t tempBuf = 0x00;
	uint16_t temp16 = 0x00;
	int8_t sign = 0x00;

	// ---- Reading of a0:
	errBit = (errBit<<1) | spiDev->readRegister ( MPL_READ_COEFF_BASE, tempBuf );		// 0x88->a0 MSB	@16bit
	temp16 = (static_cast<uint16_t> ( tempBuf )) << 8;
	errBit = (errBit<<1) | spiDev->readRegister ( MPL_READ_COEFF_BASE+0x02, tempBuf );	// 0x8A->a0 LSB	@16bit
	temp16 |= tempBuf;

	if ( temp16 & 0x8000 )	// Negative int part (U2)
	{
		temp16 = ~temp16 + 1;
		sign = -1;
	}
	else sign = 1;

	a0 = (((temp16 & 0x7FFF) >> 3) + ((float)(temp16 & 0x0007)) * 0.125 ) * sign; 			// 12 int bits, 3 fractional

	// ---- Reading of b1:
	errBit = (errBit<<1) | spiDev->readRegister ( MPL_READ_COEFF_BASE+0x04, tempBuf );		// 0x8C->b1 MSB	@16bit
	temp16 = (static_cast<uint16_t> ( tempBuf )) << 8;
	errBit = (errBit<<1) | spiDev->readRegister ( MPL_READ_COEFF_BASE+0x06, tempBuf );		// 0x8E->b1 LSB	@16bit
	temp16 |= tempBuf;

	if ( temp16 & 0x8000 )	// Negative int part (U2)
	{
		temp16 = ~temp16 + 1;
		sign = -1;
	}
	else sign = 1;

	b1 = (((temp16 & 0x7FFF) >> 13) + ((float)(temp16 & 0x1FFF)) * 0.0001220703125) * sign; // 2 int bits, 13 fractional

	// ---- Reading of b2:
	errBit = (errBit<<1) | spiDev->readRegister ( MPL_READ_COEFF_BASE+0x08, tempBuf );		// 0x90->b2 MSB	@16bit
	temp16 = (static_cast<uint16_t> ( tempBuf )) << 8;
	errBit = (errBit<<1) | spiDev->readRegister ( MPL_READ_COEFF_BASE+0x0A, tempBuf );		// 0x92->b2 LSB	@16bit
	temp16 |= tempBuf;

	if ( temp16 & 0x8000 )	// Negative int part (U2)
	{
		temp16 = ~temp16 + 1;
		sign = -1;
	}
	else sign = 1;

	b2 = (((temp16 & 0x7FFF) >> 14) + ((float)(temp16 & 0x3FFF)) * 0.00006103515625) * sign;// 1 int bit, 14 fractional

	// ---- Reading of c12:
	errBit = (errBit<<1) | spiDev->readRegister ( MPL_READ_COEFF_BASE+0x0C, tempBuf );		// 0x94->c12 MSB @14bit
	temp16 = (static_cast<uint16_t> ( tempBuf )) << 6;
	errBit = (errBit<<1) | spiDev->readRegister ( MPL_READ_COEFF_BASE+0x0E, tempBuf );		// 0x96->c12 LSB @14bit
	temp16 |= (tempBuf >> 2) & 0x3F;

	if ( temp16 & 0x8000 )	// Negative int part (U2)
	{
		temp16 = ~temp16 + 1;
		sign = -1;
	}
	else sign = 1;

	c12 = (((double)(temp16 & 0x3FFF)) * 0.000000059604539) * sign;						// 0 int bit, 13 fractional, decimal pad = 9

//	char str[256];
//	int len = sprintf ( str, "\na0 = %f\nb1 = %f\nb2 = %f\nc12 = %f\n", a0, b1, b2, c12 );
//	console.write( str, len );

	if ( 0xFF == errBit ) return true;
	else return false;
}

}     // namespace microhal
