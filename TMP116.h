/*
 * TMP116.c
 * TMP116 Digital Temperature Sensor I2C Driver
 *  Created on: Jan 24, 2022
 *      Author: Mehmet Caner ÇELİKTAŞ
 */


#ifndef _TMP116_H
#define _TMP116_H

#include "stm32f4xx_hal.h"  /*This header contain i2c and several driver*/

#define DEVICE_ADDRESS	(0x48<<1)	/*1001000x device addres A0->GND	Datasheet p.20 */
#define DEVICE_ID		0x1116		/*Datasheet p.31*/

typedef struct {
	/*I2C Handle*/
	I2C_HandleTypeDef *i2cHandler;

	/*Temperature Data*/
	float temp_data;

}TMP116;

float floatconvertdata(unsigned char HiByte, unsigned char LoByte);


/*Initialization function*/
unsigned char TMP116_Initialization(TMP116 *dev, I2C_HandleTypeDef *i2cHandle);

/*Temperature data read function*/
HAL_StatusTypeDef TMP116_ReadTemperature(TMP116 *dev);

/*Device Low Level Functions*/
HAL_StatusTypeDef TMP116_ReadRegister(TMP116 *dev, unsigned char reg, unsigned char *data, unsigned char length);
HAL_StatusTypeDef TMP116_WriteRegister(TMP116 *dev, unsigned char reg, unsigned char *data, unsigned char length);

/*Sensor Register Map (Datasheet p.24)*/
#define TMP116TEMPERATUREREADING 	0x00
#define TMP116CONFIGURATION 		0x01
#define TMP116ALERTHIGH 			0x02
#define TMP116ALERTLOW 				0x03
#define TMP116EEPROMUNLOCK 			0x04
#define TMP116EEPROM1 				0x05
#define TMP116EEPROM2 				0x06
#define TMP116EEPROM3 				0x07
#define TMP116EEPROM4 				0x08
#define TMP116DEVICEID 				0x0F


/*Configuration Register Field p.26*/
/*First 8 bits of the register*/
#define TMP116CONFIGREGISTERDRALERT		0x4
#define TMP116CONFIGREGISTERPOL 		0x8
#define TMP116CONFIGREGISTERTnA 		0x10
#define TMP116CONFIGREGISTERAVG0 		0x20
#define TMP116CONFIGREGISTERAVG1 		0x40
#define TMP116CONFIGREGISTERCONV0 		0x80

/*Last 8 bits of the register*/
#define TMP116CONFIGREGISTERCONV1 		0x100
#define TMP116CONFIGREGISTERCONV2 		0x200
#define TMP116CONFIGREGISTERMOD0 		0x400
#define TMP116CONFIGREGISTERMOD1 		0x800
#define TMP116CONFIGREGISTEREEPROMBUSY 	0x1000
#define TMP116CONFIGREGISTERDATAREADY 	0x2000
#define TMP116CONFIGREGISTERLOWALERT	0x4000
#define TMP116CONFIGREGISTERHIGHALERT 	0x8000

#define TMP116SHUTDOWN 0x04
#define TMP116ONESHOT 0x06


#endif
