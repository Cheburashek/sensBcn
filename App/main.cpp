/*
 * main.cpp
 *
 *  Created on: 20 kwi 2016
 *      Author: Chebu
 */
//
#include "microhal.h"
#include "microhal_bsp.h"
#include "microhalPortConfig_nrf52.h"

#include "beacon.h"
#include "MPL115_drv.h"

#include "nrf_delay.h"

using namespace microhal;
using namespace std::literals::chrono_literals;

static MPL115* mpl115;

uint16_t tempBuff;

int main ( void )
{

	uint16_t tempData = 0;
	char str[100];
	int len = 0;
	float temp, press;

    GPIO Led3(led3, GPIO::Direction::Output);
    GPIO Led4(led4, GPIO::Direction::Output);

	console.write ( "Application started!\n" );

	beacon_Init ();
	beacon_AdvStart ();

	Led3.reset();
	nrf_delay_ms ( 100 );
	Led3.set();

    nrf52::SPI::spi1.init();			// fixme : from bsp_nrf52.h, because of softdevice
    nrf52::SPI::spi1.setMode(nrf52::SPI::Mode::Mode0);
    nrf52::SPI::spi1.speed(4000000);
    nrf52::SPI::spi1.enable();

	mpl115 = new MPL115 ( SPIM, CSpin );


	while ( 1 )
	{

		mpl115->getMeasurements ( press, temp );

		len = sprintf ( str, "Press = %i [hPa]\n Temp = %f [C]\n\n", (int)press, temp );
		console.write ( str, len );

		beacon_SetMajMin ( (uint16_t)press, 0xBEEF );

		Led4.toggle();

		nrf_delay_ms ( 1000 );
	}
}

/**
 * @}
 */

