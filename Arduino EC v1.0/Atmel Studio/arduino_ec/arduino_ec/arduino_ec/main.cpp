/*
 * main.c
 *	Created: 09/05/2017 10:03:15
 *  Author: Marcelo R Gadotti
 *	Versão de testes para o circuíto do sensor de CE
 */ 

#include "main.h"

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);

byte customChar[] = {
	B00110,
	B01001,
	B01001,
	B00110,
	B00000,
	B00000,
	B00000,
	B00000
};

OneWire  ds(DS);  // on pin 10 (a 4.7K resistor is necessary)

byte i;
byte data[12];
byte addr[8];
float celsius;
float k = 0;
float ec = 0;
float ec_ref = 0;
// Constants for measure EC
#define Q	1
#define teta	0.9
#define t_ref	25
// adc variables for moving average
unsigned int adc_avg = 0, adc_total = 0, 
			 adc[10] = {0,0,0,0,0,0,0,0,0,0};
unsigned char adc_index = 0;
#define ADC_N		10
#define V_REF_K		0.00488	

// update lcd
unsigned long previousMillis = 0, currentMillis = 0;
#define interval	100

/* Initialize DS18B20*/
void init_ds18b20(void)
{
	if ( !ds.search(addr)) {
		ds.reset_search();
		delay(250);
		return;
	}
}


void update_ds18b20(void)
{
	// routines for DS18B20
	ds.reset();
	ds.select(addr);
	ds.write(0x44, 1);        // start conversion, with parasite power on at the end
	// wait conversion
	delay(50);
	ds.reset();
	ds.select(addr);
	ds.write(0xBE);
	// read 9 bytes frm sensor
	for ( i = 0; i < 9; i++) {           // we need 9 bytes
		data[i] = ds.read();
	}
	// get 16 bits sensor data
	int16_t raw = ((data[1] << 8) | data[0]) & ~1;
	// convert measured values to Celsius
	celsius = (float)raw / 16.0;
}

/* Update conductance from ADC */
void update_k(void)
{
	adc_total = adc_total - adc[adc_index];
	adc[adc_index] = analogRead(CE);
	adc_total = adc_total + adc[adc_index];
	adc_avg = adc_total / ADC_N;
	k = (float)adc_avg * V_REF_K;
	adc_index++;
	if(adc_index > 9) adc_index = 0;
}

void setup() {
  
	// configure GPIO
	pinMode(HB,OUTPUT);
	// create custom char °
    lcd.createChar(0, customChar);
	// set up the LCD's number of columns and rows:
	lcd.begin(16, 2);
	lcd.setCursor(0,0);
	// print firmware version for 1s
	lcd.print(VERSION); 
	delay(1000);
	// clear screen and print fixed fields
	lcd.clear();
	//lcd.print("Temp.:");
	// print °C and EC
	lcd.setCursor(4,0);
	lcd.write(byte(0));
	lcd.setCursor(5,0);
	lcd.print("C ");
	lcd.setCursor(11,0);
	lcd.print("mS/cm");

	// print EC 25°C 
	lcd.setCursor(0,1);
	lcd.print("EC25");
	lcd.setCursor(4,1);
	lcd.write(byte(0));
	lcd.setCursor(5,1);
	lcd.print("C");
	lcd.setCursor(11,1);
	lcd.print("mS/cm");

	init_ds18b20();
	
	// serial port (not used yet)
	Serial.begin(9600);
	Serial.println(VERSION);
}

void loop() {
  
	// get new temperature on ds18b20
	update_ds18b20();
	
	// get new value from adc
	update_k();
	// get ec from conductance
	ec = k * Q;
	// equation with temperature coefficient
	ec_ref = (100*ec)/(100+teta*(celsius - t_ref));
		
	// update lcd value every interval
	currentMillis = millis();
	if (currentMillis - previousMillis >= interval) 
	{
		//update temperature
		lcd.setCursor(0,0);
		lcd.print(celsius,1);
		// update ec value
		lcd.setCursor(7,0);
		lcd.print(ec,2);
		// update ec value 25°C
		lcd.setCursor(7,1);
		if (ec_ref >= 0)
		{
			lcd.print(ec_ref,2);
		} 
		else
		{
			lcd.print(0.00,2);
		}
		
		
		// heart beat, blink every lcd update
		digitalWrite(HB,!digitalRead(HB));
		
		previousMillis = currentMillis;


	}  
}
