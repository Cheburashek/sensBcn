/*
 * MPL115_drv.cpp
 *
 *  Created on: 7 maj 2016
 *      Author: Cheburashek
 */

#include "MPL115A1_drv.h"
#include <thread>

namespace microhal
{
//*******************************************************************************************************************
// Constructor
MPL115A1::MPL115A1 ( SPI &spi, const GPIO::IOPin CEpin ) :
			SPIDevice ( spi, CEpin )
{
	calibrate ();
}

MPL115A1::~MPL115A1 ()
{
}

//*******************************************************************************************************************
bool MPL115A1::getPressure_hPa ( uint16_t &press )
{
	uint8_t errBit = 0x00;
	uint16_t Padc = 0x00;
	uint16_t Tadc = 0x00;
	float Pcomp = 0.0;

	errBit = !startConv ();
	std::this_thread::sleep_for ( std::chrono::milliseconds { 3 } );

	errBit = (errBit << 1) | !readRawPress ( Padc );
	errBit = (errBit << 1) | !readRawTemp ( Tadc );

	Pcomp = a0 + (b1 + ((double) Tadc) * c12) * ((float) Padc) + b2 * ((float) Tadc);
	press = (uint16_t)(Pcomp * 0.63538612 + 500);    	// Pressure in [hPa]

	if ( errBit ) return false;
	else return true;
}

// Private methods:

//*******************************************************************************************************************
bool MPL115A1::startConv ( void )
{
	uint8_t startBuff[2] = { MPL_CONVSTART, 0x00 };
	return writeBuffer ( startBuff, sizeof(startBuff), true );
}

//*******************************************************************************************************************
bool MPL115A1::readRawPress ( uint16_t &buff )
{
	bool retVal = false;
	uint8_t tempBuf;

	if ( readRegister ( MPL_READ_PRESS_MSB, tempBuf ) ) 			// Pressure MSB
	{
		buff = ((uint16_t) tempBuf) << 8;							// 8 from 10bits
		retVal = readRegister ( MPL_READ_PRESS_LSB, tempBuf );    	// Pressure LSB -2bits
		buff |= tempBuf;
		buff >>= 6;	// 10bit as uint16
	}

	return retVal;
}

//*******************************************************************************************************************
bool MPL115A1::readRawTemp ( uint16_t &buff )
{
	bool retVal = false;
	uint8_t tempBuf;

	if ( readRegister ( MPL_READ_TEMP_MSB, tempBuf ) )    			// Temperature MSB
	{
		buff = ((uint16_t) tempBuf) << 8;							// 8 from 10bits
		retVal = readRegister ( MPL_READ_TEMP_LSB, tempBuf );     	// Temperature LSB 2bits
		buff |= tempBuf;
		buff >>= 6;	// 10bit as uint16
	}

	return retVal;
}

//*******************************************************************************************************************
bool MPL115A1::calibrate ( void )
{
	uint8_t errBit = 0;
	uint8_t tempBuf = 0;
	uint16_t temp16 = 0;
	int8_t sign = 1;

	// ---- Reading of a0:
	errBit = readRegister ( MPL_READ_COEFF_BASE, tempBuf );    						   // 0x88->a0 MSB	@16bit
	temp16 = ((uint16_t) tempBuf ) << 8;
	errBit = (errBit << 1) | readRegister ( MPL_READ_COEFF_BASE + 0x02, tempBuf );     // 0x8A->a0 LSB	@16bit
	temp16 |= tempBuf;

	if ( temp16 & 0x8000 )     // Negative int part (U2)
	{
		temp16 = ~temp16 - 1;
		sign = -1;
	}
	else sign = 1;

	a0 = (((temp16 & 0x7FFF) >> 3) + ((float) (temp16 & 0x0007)) * 0.125) * sign; 		// 12 int bits, 3 fractional

	// ---- Reading of b1:
	errBit = (errBit << 1) | readRegister ( MPL_READ_COEFF_BASE + 0x04, tempBuf );		// 0x8C->b1 MSB	@16bit
	temp16 = ((uint16_t) tempBuf ) << 8;
	errBit = (errBit << 1) | readRegister ( MPL_READ_COEFF_BASE + 0x06, tempBuf );		// 0x8E->b1 LSB	@16bit
	temp16 |= tempBuf;

	if ( temp16 & 0x8000 )     // Negative int part (U2)
	{
		temp16 = ~temp16 - 1;
		sign = -1;
	}
	else sign = 1;

	b1 = (((temp16 & 0x7FFF) >> 13) + ((float) (temp16 & 0x1FFF)) * 0.0001220703125) * sign;     // 2 int bits, 13 fractional

	// ---- Reading of b2:
	errBit = (errBit << 1) | readRegister ( MPL_READ_COEFF_BASE + 0x08, tempBuf );		// 0x90->b2 MSB	@16bit
	temp16 = ((uint16_t) tempBuf ) << 8;
	errBit = (errBit << 1) | readRegister ( MPL_READ_COEFF_BASE + 0x0A, tempBuf );		// 0x92->b2 LSB	@16bit
	temp16 |= tempBuf;

	if ( temp16 & 0x8000 )     // Negative int part (U2)
	{
		temp16 = ~temp16 - 1;
		sign = -1;
	}
	else sign = 1;

	b2 = (((temp16 & 0x7FFF) >> 14) + ((float) (temp16 & 0x3FFF)) * 0.00006103515625) * sign;     // 1 int bit, 14 fractional

	// ---- Reading of c12:
	errBit = (errBit << 1) | readRegister ( MPL_READ_COEFF_BASE + 0x0C, tempBuf );		// 0x94->c12 MSB @14bit
	temp16 = ((uint16_t) tempBuf ) << 8;
	errBit = (errBit << 1) | readRegister ( MPL_READ_COEFF_BASE + 0x0E, tempBuf );		// 0x96->c12 LSB @14bit
	temp16 |= (tempBuf >> 2) & 0x3F;

	if ( temp16 & 0x8000 )     // Negative int part (U2)
	{
		temp16 = ~temp16 - 1;
		sign = -1;
	}
	else sign = 1;

	c12 = (((double) (temp16 & 0x3FFF)) * 0.000000059604539) * sign;     // 0 int bit, 13 fractional, decimal pad = 9

	if ( 0xFF == errBit ) return true;
	else return false;
}
//*******************************************************************************************************************

}// namespace microhal

