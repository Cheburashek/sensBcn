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

static MPL115 mpl115;

uint16_t tempBuff;

int main ( void )
{

	uint16_t tempData = 0;
	char str[100];
	int len = 0;

    GPIO Led3(led3, GPIO::Direction::Output);
    GPIO Led4(led4, GPIO::Direction::Output);



	console.write ( "Application started!\n" );

	beacon_Init ();
	//beacon_AdvStart ();

	Led3.reset();
	nrf_delay_ms ( 100 );
	Led3.set();


	mpl115.init();

	while ( 1 )
	{
		nrf_delay_ms ( 500 );

		mpl115.readTempRaw( &tempBuff );

		len = sprintf ( str, "Buff: %i\n", tempBuff );
		console.write ( str, len );

		Led4.toggle();
	}
}

/**
 * @}
 */

