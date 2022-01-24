/*
 * TMP116.c
 * TMP116 Digital Temperature Sensor I2C Driver
 *  Created on: Jan 24, 2022
 *      Author: Mehmet Caner ÇELİKTAŞ
 */

#include "TMP116.h"


/*Convert the data inside of HIGH and LOW registers*/
float floatconvertdata(unsigned char HiByte, unsigned char LoByte) {
    float temp;
    temp = (float) (HiByte << 8 | LoByte);
    temp *= 0.0078125;
    return temp;
}

/*INITIALIZATION FUNCTIONS*/
unsigned char TMP116_Initialization(TMP116 *dev, I2C_HandleTypeDef *i2cHandle)
{
	unsigned char err_no;
	HAL_StatusTypeDef status;
	unsigned short regData;		//Register Data

	dev->i2cHandler = i2cHandle;
	dev->temp_data 	= 0.0f;

	HAL_Delay(2);		/*2ms delay -After the supply voltage reaches within the operating range,
						the device requires 1.5 ms to power up before conversions begin*/

	/*Check device ID*/
	status = TMP116_ReadRegister(dev, TMP116DEVICEID, &regData, sizeof(regData));
	if(status != HAL_OK)
		err_no += 1;

	if(regData != DEVICE_ID)
	{
		printf("\nDevice ID Error!");
		return 0xFF;
	}

	/*Load the default configurations*/
	regData = TMP116CONFIGREGISTERMOD1;								/*Shutdown conversation mod for low power*/
	regData |= (TMP116CONFIGREGISTERCONV2 & 0);						/*Make conversion time 125 ms. After conversion, sensor will be shutdown*/
	regData |= TMP116CONFIGREGISTERDRALERT;							/*ALERT pin reflects the status of the data ready flag*/

	status = TMP116_WriteRegister(dev, TMP116CONFIGURATION, &regData, sizeof(regData));
	if(status != HAL_OK)
	{
			err_no += 1;
			printf("\nCould not load default settings.");
	}

	regData |= (TMP116CONFIGREGISTERMOD1|TMP116CONFIGREGISTERMOD0);		//Trigger the one-shot mode
	status = TMP116_WriteRegister(dev, TMP116CONFIGURATION, &regData, sizeof(regData));
	if(status != HAL_OK)
	{
			err_no += 1;
			printf("\nCould not set one-shot mode.");
	}
	return err_no;
}

/*READ TEMPERATURE DATA FUNCTION*/
HAL_StatusTypeDef TMP116_ReadTemperature(TMP116 *dev)
{
	/*Datasheet p.25*/
	short regData;
	unsigned char lowByte, highByte;

	/*Read the temperature data register*/
	HAL_StatusTypeDef status = TMP116_ReadRegister(dev, TMP116TEMPERATUREREADING, &regData, sizeof(regData));

	if(regData != (-256))	/*Following power-up or a general-call reset, the temperature register reads –256°C */
	{					/*until the first conversion is complete*/
		lowByte = (regData & 0x00FF);
		highByte =(regData & 0xFF00)>>8;

		/*Assing the data*/
		dev->temp_data = floatconvertdata(highByte, lowByte);
		return status;
	}
	else
	{
		return HAL_BUSY;
	}


}


/*LOW LEVEL FUNCTIONS*/
HAL_StatusTypeDef TMP116_ReadRegister(TMP116 *dev, unsigned char reg, unsigned char *data, unsigned char length)
{
	return HAL_I2C_Mem_Read(dev->i2cHandler, DEVICE_ADDRESS, reg, I2C_MEMADD_SIZE_8BIT, data, length, HAL_MAX_DELAY);
}
HAL_StatusTypeDef TMP116_WriteRegister(TMP116 *dev, unsigned char reg, unsigned char *data, unsigned char length)
{
	return HAL_I2C_Mem_Write(dev->i2cHandler, DEVICE_ADDRESS, reg, I2C_MEMADD_SIZE_8BIT, data, length, HAL_MAX_DELAY);
}
