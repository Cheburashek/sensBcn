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

#include "nrf_delay.h"

using namespace microhal;
using namespace std::literals::chrono_literals;

int main ( void )
{

    GPIO Led4(led4, GPIO::Direction::Output);

	console.write ( "\nApplication started!\n" );

	beacon_Init ();
	beacon_AdvStart ();

	    nrf52::SPI::spi1.init();
	    nrf52::SPI::spi1.setMode(nrf52::SPI::Mode::Mode0);
	    nrf52::SPI::spi1.speed(1000000);
	    nrf52::SPI::spi1.enable();

	while ( 1 )
	{

		nrf_delay_ms ( 1000 );
	    nrf52::SPI::spi1.write( 0xAB, true );

		//std::this_thread::sleep_for(250ms);

		Led4.toggle();

	}
}

/**
 * @}
 */

