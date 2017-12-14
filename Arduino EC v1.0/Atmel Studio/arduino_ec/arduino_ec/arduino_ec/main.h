/*
 * main.h
 *
 * Created: 09/05/2017 09:58:34
 *  Author: marce
 */ 


#ifndef MAIN_H_
#define MAIN_H_

#include <Arduino.h>
#include "LiquidCrystal.h"
#include "gpio.h"
#include "OneWire.h"

#define VERSION "SOILLESS v1.0"

void update_ds18b20(void);
void init_ds18b20(void);


#endif /* MAIN_H_ */