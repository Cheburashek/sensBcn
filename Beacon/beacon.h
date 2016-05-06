/*
 * beacon.h
 *
 *  Created on: 8 kwi 2016
 *      Author: Cheburashek
 */

#ifndef APP_BEACON_H_
#define APP_BEACON_H_

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************************
   GLOBAL INCLUDES
*/

/*****************************************************************************************
   GLOBAL DEFINITIONS
*/
/*****************************************************************************************
   GLOBAL TYPEDEFS
*/
/*****************************************************************************************
   GLOBAL FUNCTIONS DECLARATIONS
*/

void beacon_Init ( void );

/**@brief Function for starting advertising.
 */
void beacon_AdvStart ( void );


#ifdef __cplusplus
} // extern
#endif

#endif /* APP_BEACON_H_ */
