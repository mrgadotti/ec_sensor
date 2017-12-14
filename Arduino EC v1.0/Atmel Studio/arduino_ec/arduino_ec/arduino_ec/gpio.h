/*
 * gpio.h
 *
 * Created: 09/05/2017 10:03:15
 *  Author: marce
 */ 


#ifndef GPIO_H_
#define GPIO_H_

#include <Arduino.h>

/*
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
*/

#define RS		12
#define EN		11
#define D4		5
#define D5		4
#define D6		3
#define D7		2

#define HB		13	// Heart beat

#define CE		0	// sensor de CE no canal AD 0

#define DS		10	// sensor de temperatura ds18b20

#endif /* GPIO_H_ */