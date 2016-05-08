/*
 * MPL115_drv.cpp
 *
 *  Created on: 7 maj 2016
 *      Author: Cheburashek
 */

#include "MPL115_drv.h"
//#include "microhal_bsp.h" // FIXME : not compilinig ('IODevice' does not name a type) when CEpin located in bsp

namespace microhal {// todo: NAMESPACE nrf52 ?

constexpr GPIO::IOPin CSpin(nrf52::GPIO::Port0, 29);

// Constructor/destructor
MPL115::MPL115()
{

	spiDev = new SPIDevice ( nrf52::SPI::spi1, CSpin );// TODO: move to bsp (problem)
}

MPL115::~MPL115(){}

void MPL115::init ( void )
{
	nrf52::SPI::spi1.init();							// TODO : to be selectable with constructor
	nrf52::SPI::spi1.setMode(SPI::Mode::Mode0);
	nrf52::SPI::spi1.speed(1000000);
	GPIO CS(CSpin, GPIO::Direction::Output);			// CS pin config
	nrf52::SPI::spi1.enable();

}

bool MPL115::readTempRaw ( uint16_t *buff )
{
	bool retVal = false;
	uint8_t tempBuf;

	if ( spiDev->readRegister ( MPL_READ_TEMP_LSB, tempBuf ) ) 	  		// Temperature LSB
	{
		*buff = static_cast<uint16_t> (tempBuf);
		retVal = spiDev->readRegister ( MPL_READ_TEMP_MSB, tempBuf );    // Temperature MSB
		*buff |= (static_cast<uint16_t> (tempBuf))<<8;
	}

	return retVal;
}


} // namespace microhal