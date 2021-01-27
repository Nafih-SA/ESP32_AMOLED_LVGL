#include <Arduino.h>
#include <SPI.h>
#include <SD.h>

#include "main2.h"
#include "mmc_sd.h"
#include "lcd.h"

uint8_t buffer[512];

//---------------------------------------------------------
/*
unsigned char SPI_WriteByte(uint8_t val)
{
    unsigned char ret;

    // SPI0DAT = val;		// Special Function Register: SPI Data
    // while(!SPIF);      //  The SPIF (SPI0CN.7) flag is set to logic 1 at the end of the transfer                
    // SPIF = 0;
    ret = SPI.transfer(val);
    return ret;          
}

unsigned char SPI_ReadByte(void)
{   
    unsigned char ret;

	// SPI0DAT  = 0xff;
	ret = SPI.transfer(0xff);
    // while (!SPIF);
	// SPIF = 0;  
	// ret = SPI0DAT;
    return ret;
}

unsigned char MMC_SD_SendCommand(uint8_t cmd, uint32_t arg)
{
	unsigned char r1;
	unsigned char  retry=0;
	
	SPI_WriteByte(0xff);
	SPI_WriteByte(0xff);
	SPI_WriteByte(0xff);
	SPI_WriteByte(0xff);
	SPI_WriteByte(0xff);
	SPI_WriteByte(0xff);

	SPI_CS_Assert();
	
	SPI_WriteByte(cmd | 0x40);
	SPI_WriteByte(arg>>24);
	SPI_WriteByte(arg>>16);
	SPI_WriteByte(arg>>8);
	SPI_WriteByte(arg); 
	SPI_WriteByte(0x95); 
	
	while((r1 = SPI_WriteByte(0xff)) == 0xff)
		if(retry++ > 100) break;  //time out error

	SPI_CS_Deassert();

	return r1;
}

unsigned char MMC_SD_Reset(void)
{
	unsigned char i;
	unsigned char retry;
	unsigned char r1=0;

	retry = 0;
	// CLKSEL = 0x03;		// (Ext. Oscillator)/2

	SPI0CKR   = (48000000/(2*400000)) - 1;   //92kHz SPI clock
	do
	{
		for(i=0;i<100;i++) SPI_WriteByte(0xff);
		r1 = MMC_SD_SendCommand(0, 0);	//send idle command
		retry++;
		if(retry>10) break;	//time out
	} while(r1 != 0x01);	

	retry = 0;
	do
	{
	    do
	    {
		    r1 = MMC_SD_SendCommand(55, 0);	
		    retry++;
		    if(retry>100) return 1;	//time out
	    } while(r1!=0X01);

	    retry = 0;
		r1 = MMC_SD_SendCommand(41, 0);	
		retry++;
		if(retry>100) return 1;	//time out
	} while(r1!=0X00);

	if (r1!=0X00)
	{
	    retry = 0;
	    do
	    {
		    r1 = MMC_SD_SendCommand(1, 0);	//send active command
		    retry++;
		    if(retry>100) return 1;	//time out
	    } while(r1);
	}
	r1 = MMC_SD_SendCommand(59, 0);	//disable CRC

	r1 = MMC_SD_SendCommand(16, 512);	//set sector size to 512
    SPI0CKR   = (48000000/(2*12000000)) - 1; 
	// CLKSEL = 0x00;
	return 0;	
}

unsigned char MMC_SD_WriteSingleBlock(uint32_t sector)
{
	unsigned char r1;
	unsigned int i;
	unsigned char retry=0;

	// CLKSEL = 0x03;// (Ext. Oscillator)/2
	do
	{
		r1 = MMC_SD_SendCommand(24, sector<<9);//send command
		retry++;
		if(retry>10) return 1;		//time out
	} while(r1 != 0x00);	



	SPI_CS_Assert();
	
	SPI_WriteByte(0xff);

	SPI_WriteByte(0xfe);			//send start byte
	
	for(i=0; i<512; i++)		//send 512 bytes data
	{
		SPI_WriteByte(buffer[i]);   
	}
	
	SPI_WriteByte(0xff);
	SPI_WriteByte(0xff);
	
	r1 = SPI_WriteByte(0xff);
	
	if( (r1&0x1f) != 0x05)
	{
		SPI_CS_Deassert();
		return r1;
	}
			
	while(!SPI_WriteByte(0xff));

	SPI_CS_Deassert();
	// CLKSEL = 0x00;
	return 0;
}
*/
void MMC_SD_ReadSingleBlock(fs::FS &fs, const char *path)
{
	// unsigned char r1;
	// unsigned int i;
	// unsigned char retry=0;
	// Serial.printf("Reading file: %s\n", path);
	File file = fs.open(path, FILE_READ);
	delay(1000);
	if (!file)
	{
		Serial.println("Failed to open file for Reading");
		// return;
	}
	else
	{
		// Serial.println(path);
		while (file.available())
		{
			for (uint16_t i = 0; i < PIC_WIDTH*PIC_HEIGHT*2; i++)
			{
				while (file.read() == 'x')
				{

					uint16_t temp = convertCharToHex(file.read()) << 12 | convertCharToHex(file.read()) << 8 | convertCharToHex(file.read()) << 4 | convertCharToHex(file.read());
					digitalWrite(CS0, LOW);
					digitalWrite(SD_SS, HIGH);
					SPI.write16(temp);
					digitalWrite(CS0, HIGH);
					digitalWrite(SD_SS, LOW);
					if (file.read() == ',')
						break;
				}
			}
			Serial.println("SDread complete");
		}
		file.close();
	}

	/*
	// CLKSEL = 0x03;//(Ext. Oscillator)/2
	do
	{
		r1 = MMC_SD_SendCommand(17, sector<<9);	//read command
		retry++;
		if(retry>10) return 1;	//time out
	} while(r1 != 0x00);	


	SPI_CS_Assert(); //wait to start recieve data
	
	while(SPI_ReadByte() != 0xfe);

	for(i=0; i<512; i++)	//read 512 bytes
	{
		buffer[i] = SPI_ReadByte();
	}

	SPI_WriteByte(0xff);//crc
	SPI_WriteByte(0xff);
	
	SPI_CS_Deassert();
	// CLKSEL = 0x00;
	return 0;

	*/
}

void SD_Init(void)
{
	SD.begin(15); // SD Init(CS_Pin)
}

char convertCharToHex(char ch)
{
	char returnType;
	switch (ch)
	{
	case '0':
		returnType = 0;
		break;
	case '1':
		returnType = 1;
		break;
	case '2':
		returnType = 2;
		break;
	case '3':
		returnType = 3;
		break;
	case '4':
		returnType = 4;
		break;
	case '5':
		returnType = 5;
		break;
	case '6':
		returnType = 6;
		break;
	case '7':
		returnType = 7;
		break;
	case '8':
		returnType = 8;
		break;
	case '9':
		returnType = 9;
		break;
	case 'a':
		returnType = 10;
		break;
	case 'b':
		returnType = 11;
		break;
	case 'c':
		returnType = 12;
		break;
	case 'd':
		returnType = 13;
		break;
	case 'e':
		returnType = 14;
		break;
	case 'f':
		returnType = 15;
		break;
	default:
		returnType = 0;
		break;
	}
	return returnType;
}