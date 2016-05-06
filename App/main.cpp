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
#include "nrf_clock.h"
//#include "boards.h"
//#include "nrf_nvic.h"
//
#include "nrf52.h"
//#include "core_cm4.h"
//
//#include "spi_spim_nrf52.h"

using namespace microhal;
using namespace std::literals::chrono_literals;


int main(void)
{
//    GPIO Led4(led4, GPIO::Direction::Output);

    //console.write("\nApplication started!\n");


//	if ( nrf_clock_lf_is_running() ) console.write("\nLFCLK running\n");
//	else console.write("\nLFCLK NOT running\n");


//	nrf_clock_lf_src_set(NRF_CLOCK_LFCLK_Synth);
//
//	nrf_clock_task_trigger(NRF_CLOCK_TASK_LFCLKSTART);
//
//
//	std::this_thread::sleep_for(250ms);

//	if ( nrf_clock_lf_is_running() ) console.write("\nLFCLK running\n");
//	else console.write("\nLFCLK NOT running\n");

    beacon_Init ();
	  beacon_AdvStart ();




    while (1) {




    	//std::this_thread::sleep_for(250ms);
    	//Led4.toggle();

    }
}



//fixme zmieniony system_nrf52.c


/**
 * @}
 */





