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

#ifndef MICROHALPORTCONFIG_H_
#define MICROHALPORTCONFIG_H_


//***********************************************************************************************//
//                                       configure interfaces                                    //
//***********************************************************************************************//
//***********************************************************************************************//
//                                    Serial Port configurations                                 //
//***********************************************************************************************//
//#define MICROHAL_USE_SERIAL_PORT1_INTERRUPT			//available settings are MICROHAL_USE_SERIAL_PORTx_POLLING
													//						 MICROHAL_USE_SERIAL_PORTx_INTERRUPT
													//						 MICROHAL_USE_SERIAL_PORTx_DMA
//#define MICROHAL_USE_SERIAL_UART0_POLLING
#define MICROHAL_USE_SERIAL_UART0_POLLING
#define MICROHAL_SERIAL_UART0_TX_BUFFER_SIZE 512
#define MICROHAL_SERIAL_UART0_RX_BUFFER_SIZE 512

//#define MICROHAL_USE_SERIAL_UARTE0_DMA
//#define MICROHAL_SERIAL_UARTE0_TX_BUFFER_SIZE 50
//#define MICROHAL_SERIAL_UARTE0_RX_BUFFER_SIZE 20

//***********************************************************************************************//
//                                        I2C and SPI configurations                                     //
//***********************************************************************************************//
// only one of the configuration below could be chosen
/* available settings are 	MICROHAL_USE I2C_TWIM0_POLLING
 * 							MICROHAL_USE I2C_TWIM0_DMA
 * 							MICROHAL_USE I2C_TWIM1_POLLING
 * 							MICROHAL_USE I2C_TWIM1_DMA
 */
#define MICROHAL_USE_I2C_TWIM0_DMA

//***********************************************************************************************//
//                                        SPI configurations                                     //
//***********************************************************************************************//
/*							MICROHAL_USE I2C_SPIM0_POLLING
 * 							MICROHAL_USE I2C_SPIM0_DMA
 * 							MICROHAL_USE I2C_SPIM1_POLLING
 * 							MICROHAL_USE I2C_SPIM1_DMA
 */
#define MICROHAL_USE_SPI_SPIM1_DMA


#endif /* MICROHALPORTCONFIG_H_ */
