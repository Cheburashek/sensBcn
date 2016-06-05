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
#include "nrf_soc.h"
#include "app_timer.h"

#define MEAS_PERIOD_MS 1000

#define NRF52_ONRAM1_OFFRAM0    POWER_RAM_POWER_S0POWER_On      << POWER_RAM_POWER_S0POWER_Pos      \
							  | POWER_RAM_POWER_S1POWER_On      << POWER_RAM_POWER_S1POWER_Pos      \
							  | POWER_RAM_POWER_S0RETENTION_Off << POWER_RAM_POWER_S0RETENTION_Pos  \
							  | POWER_RAM_POWER_S1RETENTION_Off << POWER_RAM_POWER_S1RETENTION_Pos;

using namespace microhal;
using namespace std::literals::chrono_literals;

static MPL115A1* mpl115;

// For logging:
static char str[100];
static int len = 0;

static uint16_t press;

// For measurements timer
GPIO Led2(led2, GPIO::Direction::Output);
APP_TIMER_DEF(m_timer_t);

void configure_ram_retention(void)
{
	// Configure nRF52 RAM retention parameters. Set for System On 64kB RAM retention
	NRF_POWER->RAM[0].POWER = NRF52_ONRAM1_OFFRAM0;
	NRF_POWER->RAM[1].POWER = NRF52_ONRAM1_OFFRAM0;
	NRF_POWER->RAM[2].POWER = NRF52_ONRAM1_OFFRAM0;
	NRF_POWER->RAM[3].POWER = NRF52_ONRAM1_OFFRAM0;
	NRF_POWER->RAM[4].POWER = NRF52_ONRAM1_OFFRAM0;
	NRF_POWER->RAM[5].POWER = NRF52_ONRAM1_OFFRAM0;
	NRF_POWER->RAM[6].POWER = NRF52_ONRAM1_OFFRAM0;
	NRF_POWER->RAM[7].POWER = NRF52_ONRAM1_OFFRAM0;
}


void measTimeout ( void * p_context )
{
	Led2.toggle();

	if ( mpl115->getPressure_hPa ( press ) )
	{
		len = sprintf ( str, "Press = %i [hPa]\n\n", (int)press );
		console.write ( str, len );
		beacon_SetMajMin ( (uint16_t)press, 0x0000 );
	}
	else
	{
		len = sprintf ( str, "ERROR from SPI" );
		console.write ( str, len );
		beacon_SetMajMin ( 0xDEAD, 0xBEEF );
	}
}


int main ( void )
{
	configure_ram_retention();	// For power saving
	beacon_Init ();
	beacon_AdvStart ();

    nrf52::SPI::spi1.init();
    nrf52::SPI::spi1.setMode(nrf52::SPI::Mode::Mode0);
    nrf52::SPI::spi1.speed(4000000);
    nrf52::SPI::spi1.enable();

	mpl115 = new MPL115A1 ( SPIM, CSpin );

	app_timer_create(&m_timer_t, APP_TIMER_MODE_REPEATED, measTimeout );
 	app_timer_start (m_timer_t, APP_TIMER_TICKS (MEAS_PERIOD_MS, 0), NULL );

 	console.write ( "sensBcn started!\n" );

	while ( 1 )
	{
		 sd_app_evt_wait();		// For power saving
	}
}

/**
 * @}
 */

