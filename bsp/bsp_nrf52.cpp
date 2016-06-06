/* ========================================================================================================================== *//**
 @license    BSD 3-Clause
 @copyright  microHAL
 @version    $Id$
 @brief      board support package for stm32f4Discovery board

 @authors    Kubaszek Mateusz
 created on: 16-04-2014
 last modification: <DD-MM-YYYY>

 @copyright Copyright (c) 2014, microHAL
 All rights reserved.
 Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following
 conditions are met:
 	 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 	 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer
 	 	in the documentation and/or other materials provided with the distribution.
 	 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived
 	 	from this software without specific prior written permission.
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,
 BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 *//* ========================================================================================================================== */

#include "microhal.h"

using namespace microhal;
using namespace nrf52;

void hardwareConfig(void) {
	SystemInit();

#ifdef PRESS_RES_UART
	IOManager::routeSerial<0, Txd, nrf52::GPIO::Port0, 6>();
	IOManager::routeSerial<0, Rxd, nrf52::GPIO::Port0, 8>();

	microhal::nrf52::SerialPort::Serial0.open(nrf52::SerialPort::OpenMode::ReadWrite);
	microhal::nrf52::SerialPort::Serial0.setBaudRate(nrf52::SerialPort::BaudRate::Baud38400);
	microhal::nrf52::SerialPort::Serial0.setParity(nrf52::SerialPort::Parity::NoParity);
#endif

    IOManager::routeSPImaster<1, MISO, nrf52::GPIO::Port0, 4>();
    IOManager::routeSPImaster<1, MOSI, nrf52::GPIO::Port0, 3>();
    IOManager::routeSPImaster<1, SCK, nrf52::GPIO::Port0, 28>();

// FIXME Causes problem with softDevice (NVIC)
//    nrf52::SPI::spi1.init();
//    nrf52::SPI::spi1.setMode(nrf52::SPI::Mode::Mode0);
//    nrf52::SPI::spi1.speed(1000000);
//    nrf52::SPI::spi1.enable();

//	IOManager::routeSerialEasyDMA<0, Txd, nrf52::GPIO::Port0, 6>();
//	IOManager::routeSerialEasyDMA<0, Rxd, nrf52::GPIO::Port0, 8>();
//
//	microhal::nrf52::SerialPortEasyDMA::Serial0.open(nrf52::SerialPortEasyDMA::OpenMode::ReadWrite);
//	microhal::nrf52::SerialPortEasyDMA::Serial0.setBaudRate(nrf52::SerialPortEasyDMA::BaudRate::Baud38400);
//	microhal::nrf52::SerialPortEasyDMA::Serial0.setParity(nrf52::SerialPort::Parity::NoParity);

    SysTick_Config(64000000/1000);
}

volatile uint64_t SysTick_time = 0;

extern "C" void SysTick_Handler(void)
{
	SysTick_time++;
}
