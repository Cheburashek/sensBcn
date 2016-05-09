/*
 * MPL115_drv.h
 *
 *  Created on: 7 maj 2016
 *      Author: Cheburashek
 */

#ifndef MPL115_DRV_H_
#define MPL115_DRV_H_

#include "microhal.h"
#include "SPIDevice.h"

// SPI commands:
#define MPL_CONVSTART		0x24
#define MPL_READ_PRESS_MSB 	0x80
#define MPL_READ_PRESS_LSB 	0x82
#define MPL_READ_TEMP_MSB 	0x84
#define MPL_READ_TEMP_LSB 	0x86
#define MPL_READ_COEFF_BASE 0x88

// Class
namespace microhal {
class MPL115:  private SPIDevice {

public:

	// Constructors:
	MPL115 ( SPI &spi, const GPIO::IOPin CEpin);
	~MPL115 ();

	// Public methods:
	bool getMeasurements ( float &press, float &temp );

private:

	// Private methods:
	bool readRawPress ( uint16_t &buff );
	bool readRawTemp ( uint16_t &buff );
	bool startConv ( void );
	bool calibrate ( void );

	// Private variables:
	// Coefficients:
	float a0, b1, b2;
	double c12;

};

} // namespace microhal

#endif /* MPL115_DRV_H_ */
