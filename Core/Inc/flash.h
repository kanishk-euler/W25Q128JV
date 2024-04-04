/*
 * flash.h
 *
 *  Created on: Apr 4, 2024
 *      Author: Kanishk-TRN0148
 */
#include <stdint.h>

#ifndef FLASH_H_
#define FLASH_H_

#define FLASH_PAGE_SIZE         256
#define FLASH_SECTOR_SIZE       4096     // 4KB
#define FLASH_SECTOR_COUNT      256
#define FLASH_BLOCK_SIZE        65536    // 64KB
#define FLASH_PAGES_PER_SECTOR  (FLASH_SECTOR_SIZE / FLASH_PAGE_SIZE)

#define CHIPID							0xEF4018

#define CMD_ManufacturerID				0x90
#define CMD_JedeceID					0x9F

#define CMD_WRITE_ENABLE				0x06
#define CMD_WRITE_DISABLE				0x04
#define CMD_READ_STATUS_REG1  			0x05
#define CMD_WRITE_STATUS_REG1  			0x01
#define CMD_READ   						0x03
//#define W25Q128JV_FastReadData		0x0B
//#define W25Q128JV_FastReadDual		0x3B
#define CMD_WRITE						0x02
#define CMD_SECTOR_ERASE      			0x20
#define CMD_BLOCK_ERASE					0xD8
#define CMD_CHIP_ERASE    				0xC7
//#define W25Q128JV_SetPowerDown		0xB9
//#define W25Q128JV_SetReleasePowerDown	0xAB
//#define W25Q128JV_DeviceID			0xAB


#define FLASH_CS_0()			HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, GPIO_PIN_RESET);
#define FLASH_CS_1()			HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, GPIO_PIN_SET);

#define Flash_Check()			((Flash_GetChipID() == Flash32_CHIPID) ? 1 : 0)

int32_t SPIFLASH_disk_initialize(void);
void	SPI2_Init(void);
uint8_t SPI_Read_Byte(void);
uint8_t SPI_Write_Byte(uint8_t data);

int		Flash_GetChipID(void);
char 	Flash_Read_StatusReg1(void);
void	Flash_Write_StatusReg1(uint8_t reg);
void	Flash_Write_Enable(void);
void	Flash_Write_Disable(void);
void	Flash_Erase_Chip(void);
void	Flash_Erase_Sector(uint32_t nDest);
void	Flash_Erase_Block(uint32_t nDest);
void	Flash_Wait_Busy(void);
//void	Flash_PowerDown(void);
//void	Flash_ReleasePowerDown(void);
void	Flash_Read_Bytes(uint32_t nDest, char* pBuffer, char nBytes);
void	Flash_Write_Bytes(uint32_t nDest,char* pBuffer, uint8_t nBytes);
void	Flash_Read_Sector(uint32_t nSector, char* pBuffer);
void	Flash_Write_Sector(uint32_t nSector, char* pBuffer);

#endif /* FLASH_H_ */
