#include "flash.h"
#include <stdint.h>
#include "main.h"

typedef uint8_t u8;
extern SPI_HandleTypeDef hspi2;

int32_t SPIFLASH_disk_initialize(void)
{
	return 0;
}

uint8_t SPI_Read_Byte(void)
{
	return (SPI_Write_Byte(0xFF));
}

uint8_t SPI_Write_Byte(uint8_t data)
{
	char read_data;

	HAL_SPI_TransmitReceive_IT(&hspi2, &data, &read_data, 1);
	return read_data;
}

int Flash_GetChipID(void)
{
	int nID = 0;

	FLASH_CS_0();

	SPI_Write_Byte(CMD_JedeceID);
	nID = SPI_Read_Byte();
	nID <<= 8;
	nID |= SPI_Read_Byte();
	nID <<= 8;
	nID |= SPI_Read_Byte();

	FLASH_CS_1();

	return nID;
}

char Flash_Read_StatusReg(void)
{	char u8 = 0;
	FLASH_CS_0();
	SPI_Write_Byte(CMD_READ_STATUS_REG1);
	u8 = SPI_Read_Byte();
	FLASH_CS_1();
	return u8;
}

void Flash_Write_StatusReg(uint8_t reg)
{	FLASH_CS_0();
	SPI_Write_Byte(CMD_WRITE_STATUS_REG1);
	SPI_Write_Byte(reg);
	FLASH_CS_1();
}

void Flash_Write_Enable(void)
{	FLASH_CS_0();
	SPI_Write_Byte(CMD_WRITE_ENABLE);
	Flash_Read_StatusReg();
	FLASH_CS_1();
}

void Flash_Write_Disable(void)
{	FLASH_CS_0();
	SPI_Write_Byte(CMD_WRITE_DISABLE);
	Flash_Read_StatusReg();
	FLASH_CS_1();
}

void Flash_Wait_Busy(void)
{	while(Flash_Read_StatusReg() == 0x03)
		Flash_Read_StatusReg();
}

void Flash_Erase_Sector(uint32_t nDest)
{
	nDest *= FLASH_SECTOR_SIZE;

	FLASH_CS_0();
	Flash_Write_Enable();
	FLASH_CS_0();
	SPI_Write_Byte(CMD_SECTOR_ERASE);
	SPI_Write_Byte((char)((nDest & 0xFFFFFF) >> 16));
	SPI_Write_Byte((char)((nDest & 0xFFFF) >> 8));
	SPI_Write_Byte((char)nDest & 0xFF);
	FLASH_CS_1();
	Flash_Wait_Busy();
}

void Flash_Erase_Block(uint32_t nDest)
{
	nDest *= FLASH_BLOCK_SIZE;

	FLASH_CS_0();
	Flash_Write_Enable();
	FLASH_CS_0();
	SPI_Write_Byte(CMD_SECTOR_ERASE);
	SPI_Write_Byte((char)((nDest & 0xFFFFFF) >> 16));
	SPI_Write_Byte((char)((nDest & 0xFFFF) >> 8));
	SPI_Write_Byte((char)nDest & 0xFF);
	FLASH_CS_1();
	Flash_Wait_Busy();
}

void Flash_Erase_Chip(void)
{
	FLASH_CS_0();
	Flash_Write_Enable();
	FLASH_CS_0();
	Flash_Wait_Busy();
	FLASH_CS_0();
	SPI_Write_Byte(CMD_CHIP_ERASE);
	FLASH_CS_1();
	Flash_Wait_Busy();
}

void Flash_Read_Bytes(uint32_t nDest, char* pBuffer, char nBytes)
{	uint16_t i;

	FLASH_CS_0();
	SPI_Write_Byte(CMD_READ);
	SPI_Write_Byte((char)(nDest >> 16));
	SPI_Write_Byte((char)(nDest >> 8));
	SPI_Write_Byte((char)nDest);
	for(i=0;i<nBytes;i++)
	{	pBuffer[i] = SPI_Read_Byte();
	}

	FLASH_CS_1();
	Flash_Wait_Busy();
}

void Flash_Read_Sector(uint32_t nSector, char* pBuffer)
{	uint16_t i;

	nSector *= FLASH_SECTOR_SIZE;

	FLASH_CS_0();
	SPI_Write_Byte(CMD_READ);
	SPI_Write_Byte((char)(nSector >> 16));
	SPI_Write_Byte((char)(nSector>> 8));
	SPI_Write_Byte((char) nSector);

	for(i=0;i<FLASH_SECTOR_SIZE;i++)
	{	pBuffer[i] = SPI_Read_Byte();
	}
	FLASH_CS_1();
	Flash_Wait_Busy();
}

void Flash_Write_Bytes(uint32_t nSector,char* pBuffer, uint8_t nBytes)
{
	int i;

	FLASH_CS_0();
	Flash_Write_Enable();
	FLASH_CS_0();

	SPI_Write_Byte(CMD_WRITE);
	SPI_Write_Byte((u8)(nSector >> 16));
	SPI_Write_Byte((u8)(nSector >> 8));
	SPI_Write_Byte((u8) nSector);
	for(i=0;i<nBytes;i++)
	{	SPI_Write_Byte(pBuffer[i]);
	}
	FLASH_CS_1();
	Flash_Wait_Busy();
}

void Flash_Write_Sector(uint32_t nSector, char* pBuffer)
{
	int i,j;
	nSector *= FLASH_SECTOR_SIZE;
	for(j=0;j<FLASH_PAGES_PER_SECTOR;j++)
	{
		FLASH_CS_0();
		Flash_Write_Enable();
		FLASH_CS_0();

		SPI_Write_Byte(CMD_WRITE);
		SPI_Write_Byte((char)(nSector >> 16));
		SPI_Write_Byte((char)(nSector >> 8));
		SPI_Write_Byte((char) nSector);

		for(i=0;i<FLASH_PAGE_SIZE;i++)
			SPI_Write_Byte(pBuffer[i]);

		pBuffer += FLASH_PAGE_SIZE;
		nSector += FLASH_PAGE_SIZE;

		FLASH_CS_1();
		Flash_Wait_Busy();
	}
}
