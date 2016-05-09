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
#include "MPL115A1_drv.h"
#include "nrf_delay.h"

using namespace microhal;
using namespace std::literals::chrono_literals;

static MPL115A1* mpl115;

static char str[100];
static int len = 0;
static uint16_t press;

int main ( void )
{
    GPIO Led3(led3, GPIO::Direction::Output);
    GPIO Led4(led4, GPIO::Direction::Output);

	console.write ( "sensBcn started!\n" );

	beacon_Init ();
	beacon_AdvStart ();

	Led3.reset();
	nrf_delay_ms ( 100 );
	Led3.set();

    nrf52::SPI::spi1.init();			// fixme : from bsp_nrf52.h, because of softdevice
    nrf52::SPI::spi1.setMode(nrf52::SPI::Mode::Mode0);
    nrf52::SPI::spi1.speed(4000000);
    nrf52::SPI::spi1.enable();

	mpl115 = new MPL115A1 ( SPIM, CSpin );


	while ( 1 )
	{

		if ( mpl115->getPressure_hPa ( press ) )
		{
			len = sprintf ( str, "Press = %i [hPa]\n\n", (int)press );
			console.write ( str, len );
			beacon_SetMajMin ( (uint16_t)press, 0x0000 );
		}
		else	// TODO: better manner
		{
			len = sprintf ( str, "ERROR from SPI" );
			console.write ( str, len );
			beacon_SetMajMin ( 0xDEAD, 0xBEEF );
		}

		Led4.reset();
		nrf_delay_ms ( 120 );
		Led4.set();
	}
}

/**
 * @}
 */

