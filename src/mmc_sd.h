//#include<main.h>

#ifndef __MMC_SD_h__
#define __MMC_SD_h__

/*  The SPI can be operated in either 3-wire or 4-wire modes, depending on the state of the NSSMD1-
NSSMD0 bits in register SPI0CN. */
// #define SPI_CS_Assert()    NSSMD0   = 0;   // 0: 3-Wire Master or 3-Wire Slave Mode (slave-select(NSS))
// #define SPI_CS_Deassert()  NSSMD0   = 1;   // 1: 4-Wire Slave or Multi-Master Mode

extern uint8_t buffer[512];

extern void  SD_Init(void);
extern void MMC_SD_ReadSingleBlock(fs::FS &fs, const char*);
// extern unsigned char MMC_SD_WriteSingleBlock(unsigned long sector);
// extern unsigned char MMC_SD_SendCommand(unsigned char cmd, unsigned long arg);
// extern unsigned char SPI_WriteByte(unsigned char val);
char convertCharToHex(char ch);

#endif

