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

using namespace microhal;
using namespace std::literals::chrono_literals;

static MPL115A1* mpl115;

// For logging:
static char str[100];
static int len = 0;

static uint16_t press;

// For measurements timer
APP_TIMER_DEF(m_timer_t);

void measTimeout ( void * p_context )
{
	if ( mpl115->getPressure_hPa ( press ) )
	{
#ifdef PRESS_RES_UART
		len = sprintf ( str, "Press = %i [hPa]\n\n", (int)press );
		console.write ( str, len );
#endif
		beacon_SetMajMin ( (uint16_t)press, 0x0000 );
	}
	else
	{
		beacon_SetMajMin ( 0xDEAD, 0xBEEF );
	}
}

static void power_manage(void)
{
	sd_app_evt_wait();		// For power saving
}

int main ( void )
{
	beacon_Init ();
	beacon_AdvStart ();

    nrf52::SPI::spi1.init();
    nrf52::SPI::spi1.setMode(nrf52::SPI::Mode::Mode0);
    nrf52::SPI::spi1.speed(4000000);
    nrf52::SPI::spi1.enable();

	mpl115 = new MPL115A1 ( SPIM, CSpin );

	app_timer_create(&m_timer_t, APP_TIMER_MODE_REPEATED, measTimeout );
 	app_timer_start (m_timer_t, APP_TIMER_TICKS (MEAS_PERIOD_MS, 0), NULL );

    // Enter main loop.
    for (;; )
    {
		power_manage();
	}
}

/**
 * @}
 */



