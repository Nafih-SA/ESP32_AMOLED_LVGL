#ifndef __MAIN_h__
#define __MAIN_h__

// unsigned char buffer[512];
#define ROW 448 //
#define COL 388

//Control Pins
#define CS0 5 	//SPI Chip Select
#define RS 2  	//SPI DCX
#define VCI_EN 4
#define LCD_RST 22
#define LCD_TE 9 //Tearing Effect

#define SD_SS 15

#define FONT_W 45
#define FONT_H 18

#define PIC_WIDTH 200 //Width of image
#define PIC_HEIGHT 108

#define RED 0xF800 //
#define GREEN 0x07E0
#define BLUE 0x001F
#define WHITE 0xFFFF
#define BLACK 0x0000
#define GRAY 0xEF5D //0x2410
#define GRAY75 0x39E7
#define GRAY50 0x7BEF
#define GRAY25 0xADB5

#define X_min 0x0043 //
#define X_max 0x07AE
#define Y_min 0x00A1
#define Y_max 0x0759

#endif
