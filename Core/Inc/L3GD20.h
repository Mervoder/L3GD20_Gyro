/*
 * L3GD20.h
 *
 *  Created on: Jul 3, 2023
 *      Author: Oğuz Kaan
 */

#ifndef INC_L3GD20_H_
#define INC_L3GD20_H_

#include <stm32f4xx.h>

#define CTRL_REG1	0x20
#define CTRL_REG4	0x23

#define OUT_X_L		0x28
#define OUT_X_H		0x29
#define OUT_Y_L		0x2A
#define OUT_Y_H		0x2B
#define OUT_Z_L		0x2C
#define OUT_Z_H		0x2D




extern SPI_HandleTypeDef hspi1;


typedef enum
{
    FS_245dps = 0,
	FS_500dps = 1,
	FS_2000dps = 2

}Mode;


void L3GD20_Init(Mode scale);

uint8_t SPI_Read(uint8_t address );
void SPI_Write(uint8_t address , uint8_t msg);

float Gyro_Read_X();
float Gyro_Read_Z();
float Gyro_Read_Y();

#endif /* INC_L3GD20_H_ */
