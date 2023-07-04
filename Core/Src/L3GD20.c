/*
 * L3GD20.c
 *
 *  Created on: Jul 3, 2023
 *      Author: Oğuz Kaan
 */

#include "L3GD20.h"

uint8_t rawXL , rawXH , rawZL , rawZH , rawYL , rawYH;
uint16_t x;
float multipler;



void L3GD20_Init(Mode scale)
{
	uint8_t scl =0;
	// cs goes low to start conv.
	switch(scale)
	{
	case FS_245dps: scl = 1;
	multipler = 0.00875;
	break;

	case FS_500dps: scl = 17;
	multipler = 0.01750;
	break;

	case FS_2000dps: scl = 49;
	multipler = 0.070;
		break;
	}


	SPI_Write(CTRL_REG1, 0x0F );
	SPI_Write(CTRL_REG4, scl);

}


/*
 * MSB, 1 olduğunda bir okuma işlemi, 0 olduğunda bir yazma işlemi yapılacağını belirtir.
 Bu nedenle, bir okuma veya yazma işlemi yapmadan önce, register adresinin MSB'sini uygun şekilde ayarlamamız gerekiyor.

Örneğin, bir register adresine veri yazmak istiyorsanız, adresin MSB'sini 1 olarak ayarlamak için
 0x80 (binary olarak 10000000) ile OR işlemi yaparız.
Bu, adresin MSB'sini 1 yapar ve geri kalan bitleri olduğu gibi bırakır.

Aynı şekilde, bir register adresinden veri okumak istiyorsanız, adresin MSB'sini 1 olarak ayarlamak için
0x80 ile OR işlemi yaparız.

 * */

void SPI_Write(uint8_t address , uint8_t msg )
{
	uint8_t wData [1]= {0};
	uint8_t wAddr [1]={0};



	wData[0] = msg;
	wAddr[0] =  address;

	// cs goes low to start conv.
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, RESET);
	HAL_Delay(20);


	HAL_SPI_Transmit(&hspi1, wAddr, 1, 1000);
	HAL_SPI_Transmit(&hspi1, wData, 1, 1000);

	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, SET);



}

uint8_t SPI_Read(uint8_t address )
{
	uint8_t rData = 0;
	uint8_t rAddr [1]={0};


		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, RESET);
		HAL_Delay(20);
		rAddr[0] = 0x80 | (address) ;


		HAL_SPI_Transmit(&hspi1, rAddr, 1, 1000);

		HAL_SPI_Receive(&hspi1, &rData, 1, 1000);

		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, SET);



	return rData;
}

float Gyro_Read_X()
{
	int16_t gyroX =0;


	rawXL = SPI_Read(OUT_X_L);
	rawXH = SPI_Read(OUT_X_H);
	gyroX = (int16_t)((rawXH << 8 | rawXL ));
	gyroX = gyroX*multipler;
	return gyroX;

}

float Gyro_Read_Y()
{
	int16_t gyroy =0;

	rawYL = SPI_Read(OUT_Y_L);
	rawYH = SPI_Read(OUT_Y_H);
	gyroy = (int16_t)((rawYH << 8 | rawYL ));
	gyroy = (float)gyroy * multipler;
	return gyroy;
}

float Gyro_Read_Z()
{
	int16_t gyroZ =0;

	rawZL = SPI_Read(OUT_Z_L);
	rawZH = SPI_Read(OUT_Z_H);
	gyroZ = (int16_t)((rawZH << 8 | rawZL ));
	gyroZ = (float)gyroZ * multipler;
	return gyroZ;
}
