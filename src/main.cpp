#include <Arduino.h>
#include <SPI.h>
#include<SD.h>
#include <lvgl.h>
#include <TFT_eSPI.h>

#include "main2.h"
#include "data.h"

void renderIMG(unsigned int array[],int xpos, int ypos);
void showTime(uint32_t);

TFT_eSPI tft = TFT_eSPI();       // Invoke custom library

#define minimum(a,b)     (((a) < (b)) ? (a) : (b))

void setup()
{
  // pinMode(CS0, OUTPUT);
  // pinMode(RS, OUTPUT);
  pinMode(VCI_EN, OUTPUT);
  pinMode(LCD_RST, OUTPUT);
  // pinMode(SD_SS, OUTPUT);

  Serial.begin(115200);
  Serial.print("\nInitializing...");
  // SPI.beginTransaction(SPISettings(80000000, MSBFIRST, SPI_MODE0));
  // SPI.begin();

  delay(20);
  digitalWrite(VCI_EN, HIGH);
  delay(20);
  digitalWrite(LCD_RST, HIGH);
  // LCD_Init();
  Serial.println("OLED Initialized");

  tft.init();  
}

void loop()
{
  tft.setRotation(0);  // portrait
  tft.fillScreen(random(0xFFFF));
  uint16_t t1, t2;
  t1 = millis();
  
  renderIMG(pic2, 40, 0);
  renderIMG(pic2, 40, 106);
  renderIMG(pic2, 40, 212);
  renderIMG(pic2, 40, 318); // Draw a jpeg image stored in memory at x,y
  t2 = millis();
  Serial.printf("Time taken for writting %d pixels = %d ms\n", 200 * 318, t2 - t1);
  Serial.printf("Avg Frame rate: %0.2f FPS \n", 1000 / ((t2 - t1) * 1.0));
  delay(2000);


  /**********************
  uint16_t t1, t2;

  t1 = millis();
  // DispColor(WHITE);
  // DispPic(); 
  tft.fillScreen(TFT_GREEN);
  delay(1000);
  tft.fillScreen(TFT_RED);
  delay(1000);
  tft.fillScreen(TFT_BLUE);
  delay(1000);
  tft.fillScreen(TFT_WHITE);

  t2 = millis();
  Serial.printf("Time taken for writting %d pixels = %d ms\n", COL * ROW, t2 - t1);
  Serial.printf("Avg Frame rate: %0.2f FPS \n", 1000 / ((t2 - t1) * 1.0));

  
  // EnterSLP();
  // delay(2000);
  // ExitSLP();
  ********************/
}

//####################################################################################################
// Draw a JPEG on the TFT, images will be cropped on the right/bottom sides if they do not fit
//####################################################################################################
// This function assumes xpos,ypos is a valid screen coordinate. For convenience images that do not
// fit totally on the screen are cropped to the nearest MCU size and may leave right/bottom borders.
void renderIMG(unsigned int array[], int xpos, int ypos) {

  // retrieve infomration about the image
  unsigned int *pImg;
  // uint16_t mcu_w = JpegDec.MCUWidth;
  // uint16_t mcu_h = JpegDec.MCUHeight;
  // uint32_t max_x = JpegDec.width;
  // uint32_t max_y = JpegDec.height;

  // // Jpeg images are draw as a set of image block (tiles) called Minimum Coding Units (MCUs)
  // // Typically these MCUs are 16x16 pixel blocks
  // // Determine the width and height of the right and bottom edge image blocks
  // uint32_t min_w = minimum(mcu_w, max_x % mcu_w);
  // uint32_t min_h = minimum(mcu_h, max_y % mcu_h);

  // save the current image block size
  // uint32_t win_w = mcu_w;
  // uint32_t win_h = mcu_h;

  // record the current time so we can measure how long it takes to draw an image
  // uint32_t drawTime = millis();

  // save the coordinate of the right and bottom edges to assist image cropping
  // to the screen size
  // max_x += xpos;
  // max_y += ypos;

  // // read each MCU block until there are no more
  // while (JpegDec.read()) {
	  
    // save a pointer to the image block
    pImg = array ;

    // calculate where the image block should be drawn on the screen
    // int mcu_x = JpegDec.MCUx * mcu_w + xpos;  // Calculate coordinates of top left corner of current MCU
    // int mcu_y = JpegDec.MCUy * mcu_h + ypos;

    // // check if the image block size needs to be changed for the right edge
    // if (mcu_x + mcu_w <= max_x) win_w = mcu_w;
    // else win_w = min_w;

    // // check if the image block size needs to be changed for the bottom edge
    // if (mcu_y + mcu_h <= max_y) win_h = mcu_h;
    // else win_h = min_h;

    // // copy pixels into a contiguous block
    // if (win_w != mcu_w)
    // {
    //   uint16_t *cImg;
    //   int p = 0;
    //   cImg = pImg + win_w;
    //   for (int h = 1; h < win_h; h++)
    //   {
    //     p += mcu_w;
    //     for (int w = 0; w < win_w; w++)
    //     {
    //       *cImg = *(pImg + w + p);
    //       cImg++;
    //     }
    //   }
    // }

    // calculate how many pixels must be drawn
    uint32_t mcu_pixels = 200 * 106;

    tft.startWrite();

    // draw image MCU block only if it will fit on the screen
    // if (( mcu_x + win_w ) <= tft.width() && ( mcu_y + win_h ) <= tft.height())
    // {

      // Now set a MCU bounding window on the TFT to push pixels into (x, y, x + width - 1, y + height - 1)
      tft.setAddrWindow(xpos, ypos, 200, 106);

      // Write all MCU pixels to the TFT window
      while (mcu_pixels--) {
        // Push each pixel to the TFT MCU area
        tft.pushColor(*pImg++);
      }

    // }
    // else if ( (mcu_y + win_h) >= tft.height()) JpegDec.abort(); // Image has run off bottom of screen so abort decoding

    tft.endWrite();
  

  // calculate how long it took to draw the image
  // drawTime = millis() - drawTime;

  // // print the results to the serial port
  // Serial.print(F(  "Total render time was    : ")); Serial.print(drawTime); Serial.println(F(" ms"));
  // Serial.println(F(""));
}


