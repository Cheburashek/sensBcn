/*
 * MPL115_drv.h
 *
 *  Created on: 7 maj 2016
 *      Author: Cheburashek
 */

#ifndef MPL115_DRV_H_
#define MPL115_DRV_H_

#include <stdint.h>
#include "SPIDevice.h"


// SPI commands:
#define MPL_READ_TEMP_MSB 	0x84
#define MPL_READ_TEMP_LSB 	0x86

// Class
namespace microhal {

class MPL115 {

public:

	// Constructors:
	MPL115 ( );
	~MPL115 ( );

	// MEthods:
	void init ( void );
	bool readTempRaw ( uint16_t *buff );

	// Variables/objects:
	SPIDevice *spiDev;

};

} // namespace microhal

#endif /* MPL115_DRV_H_ */
