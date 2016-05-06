/*
 * main.cpp
 *
 *  Created on: 20 kwi 2016
 *      Author: Chebu
 */


#include "microhal.h"
#include "microhal_bsp.h"
#include "microhalPortConfig_nrf52.h"

#include "beacon.h"

#include "boards.h"
#include "nrf_nvic.h"

#include "nrf52.h"
#include "core_cm4.h"

#include "SPIDevice.h"

using namespace microhal;
using namespace std::literals::chrono_literals;


int main(void)
{
    GPIO Led4(led4, GPIO::Direction::Output);

    console.write("\nApplication started!\n");
//
//    sd_nvic_SetPriority( SWI2_EGU2_IRQn, 7 );
//    sd_nvic_DisableIRQ( SWI2_EGU2_IRQn );

    //
//
//    sd_nvic_SetPriority( (IRQn_Type)0, 4 );
//    sd_nvic_SetPriority( (IRQn_Type)13, 4 );
//    sd_nvic_SetPriority( (IRQn_Type)14, 4 );
//    sd_nvic_SetPriority( (IRQn_Type)15, 4 );
//    sd_nvic_SetPriority( (IRQn_Type)17, 6 );
//    sd_nvic_SetPriority( (IRQn_Type)20, 6 );
//    sd_nvic_SetPriority( (IRQn_Type)21, 7 );
//    sd_nvic_SetPriority( (IRQn_Type)25, 4 );
//    //


//    beacon_Init ();
//	  beacon_AdvStart ();


    SPIDevice *spiDev = new SPIDevice();
    spiDev->spi.speed(100000);
    spiDev->spi.setMode(spiDev.spi.Mode0);
    spiDev->spi.enable();



    while (1) {

    	spiDev.write(0xAB);


    	std::this_thread::sleep_for(250ms);
    	Led4.toggle();

    }
}


/**
 * @}
 */
