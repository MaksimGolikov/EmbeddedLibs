/*
 * Name        drv_OLED_Display.c
 * Author      Maksim Golikov (SW developer)
 * Created on: 03 APLIL, 2018
 * Description driver for control OLED display ssd1351 (source file)
 */

#include "DRV/drv_OLED_Display.h"
#include "DRV/drv_OLED_DisplayCommand.h"
#include "string.h"


#define SEND_COMMAND   0
#define SEND_DATA      1

#define SCREEN_WIDTH       0x7F
#define SCREEN_HEIGH       0x7F



typedef struct {
	GPIO_TypeDef *port;
	uint32_t      pin;
}Connect_t;

typedef struct{
	SPI_HandleTypeDef  *spiConfigure;
	
	Connect_t CS;
	Connect_t DC;
	Connect_t RES;

}displayControl_t;


displayControl_t display;



void SendCommand(uint8_t command);
void SendData(uint8_t data);
void ResetDisplay(uint8_t state);






void drv_OLED_Display_Init(SPI_HandleTypeDef *spi,
		                   GPIO_TypeDef *_portCS, GPIO_TypeDef *_portDC, GPIO_TypeDef *_portRES,
		                   uint8_t _pinCS, uint8_t _pinDC, uint8_t _pinRES){
	display.spiConfigure = spi;

	display.CS.port     = _portCS;
	display.DC.port     = _portDC;
	display.RES.port    = _portRES;
	display.CS.pin      = GPIO_PIN_0 << _pinCS;
	display.DC.pin      = GPIO_PIN_0 << _pinDC;
	display.RES.pin     = GPIO_PIN_0 << _pinRES;


	//Initialize display process
	ResetDisplay(GPIO_PIN_RESET);
	HAL_Delay(100);
	ResetDisplay(GPIO_PIN_SET);
	HAL_Delay(100);
	ResetDisplay(GPIO_PIN_RESET);
	HAL_Delay(100);
	ResetDisplay(GPIO_PIN_SET);
	HAL_Delay(100);


    SendCommand(COMMAND_COMMAND_LOCK);
    SendData(0x12);
    SendCommand(COMMAND_COMMAND_LOCK);
    SendData(0xB1);

    SendCommand(COMMAND_DISPLAY_OFF);
    SendCommand(COMMAND_CLOCK_DIV);
    SendData(0xF0);

    SendCommand(COMMAND_MUXRATIO);
    SendData(0x7F);

    SendCommand(COMMAND_DISPLAY_OFFSET);
    SendData(0x00);

    SendCommand(COMMAND_STARTLINE);
    SendData(0x00);

    SendCommand(COMMAND_SET_REMAP);
    SendData(0x74);

    SendCommand(COMMAND_SET_GPIO);
    SendData(0x00);

    SendCommand(COMMAND_FUNCTION_SELECT);
    SendData(0xC1);

    SendCommand(COMMAND_SET_VSL);
    SendData(0xA0);
    SendData(0xB5);
    SendData(0x55);

    SendCommand(COMMAND_CONTRAST_ABC);
	SendData(0xC8);  //Color A //8a
	SendData(0xA8);  //Color B //51
	SendData(0xC8);  //Color C //8a

	SendCommand(COMMAND_CONTRAST_MASTER);
	SendData(0x0F);

	SendCommand(COMMAND_SETGRAY);
	for(uint8_t i = 5; i < 23; ++i){
		SendData(i);
	}
	SendData(0x18);
	SendData(0x1a);
	SendData(0x1b);
	SendData(0x1C);
	SendData(0x1D);
	SendData(0x1F);
	SendData(0x21);
	SendData(0x23);
	SendData(0x25);
	SendData(0x27);
	for (uint8_t i = 42; i <= 72; i += 3) {
		SendData(i);
	}
	for (uint8_t i = 76; i <= 120; i += 4) {
		SendData(i);
	}
	for (uint8_t i = 125; i <= 180; i += 5) {
		SendData(i);
	}

	SendCommand(COMMAND_PRECHARGE);
	SendData(0x32);

	SendCommand(COMMAND_PRECHARGE_LEVEL);
	SendData(0x16);

	SendCommand(COMMAND_PRECHARGE2);
	SendData(0x01);
	SendData(0x09);
	SendData(0x0F);

	SendCommand(COMMAND_VCOMH);
	SendData(0x05);

	SendCommand(COMMAND_NORMAL_DISPLAY);

	SendCommand(COMMAND_SET_COLUMN);
	SendData(0x00);
	SendData(SCREEN_WIDTH);

	SendCommand(COMMAND_SET_ROW);
	SendData(0x00);
	SendData(SCREEN_HEIGH);

	SendCommand(COMMAND_DISPLAY_ON);
}


void drv_OLED_Display_Clear(){
	SendCommand(COMMAND_SET_COLUMN);
	SendData(0x00);
	SendData(SCREEN_WIDTH);

	SendCommand(COMMAND_SET_ROW);
	SendData(0x00);
	SendData(SCREEN_HEIGH);

	SendCommand(COMMAND_WRITE_RAM);
    uint16_t pixelsCount = SCREEN_HEIGH * SCREEN_WIDTH;

    for(uint16_t pixel = 0; pixel < pixelsCount; ++pixel){
    	SendData(0xFF);
    	SendData(0xFF);
    }
}


void drv_OLED_Display_FillScreen(uint16_t color){
	SendCommand(COMMAND_SET_COLUMN);
	SendData(0x00);
	SendData(SCREEN_WIDTH);

	SendCommand(COMMAND_SET_ROW);
	SendData(0x00);
	SendData(SCREEN_HEIGH);

	SendCommand(COMMAND_WRITE_RAM);
	uint16_t pixelsCount = (SCREEN_HEIGH + 1) * (SCREEN_WIDTH + 1);

	for (uint16_t pixel = 0; pixel < pixelsCount; ++pixel) {
		SendData(color >> 8);
		SendData(color);
	}
}


void drv_OLED_Display_FillRectangle(uint8_t x_st,  uint8_t y_st,
		                                uint8_t x_end, uint8_t y_end,
									                  uint16_t color) {
	if ((x_st < x_end) && (y_st < y_end)) {

		SendCommand(COMMAND_SET_COLUMN);
		SendData(x_st);
		SendData(x_end);

		SendCommand(COMMAND_SET_ROW);
		SendData(y_st);
		SendData(y_end);

		SendCommand(COMMAND_WRITE_RAM);
		uint16_t pixelsCount = (x_end - x_st + 1) * (y_end - y_st + 1);

		for (uint16_t pixel = 0; pixel < pixelsCount; ++pixel) {
			SendData(color >> 8);
			SendData(color);
		}

	}
}


void drv_OLED_Display_DrawPixel(uint8_t x, uint8_t y, uint16_t color){
	SendCommand(COMMAND_SET_COLUMN);
	SendData(x);
	SendData(SCREEN_WIDTH - 1);

	SendCommand(COMMAND_SET_ROW);
	SendData(y);
	SendData(SCREEN_HEIGH - 1);

	SendCommand(COMMAND_WRITE_RAM);
	SendData(color >> 8);
	SendData(color);
}


void drv_OLED_Display_DrawPictire(uint8_t x_st,  uint8_t y_st,
		                              uint8_t width, uint8_t heigh,
								                  uint8_t *ptrPicture, uint16_t color, uint8_t paintFon){
	if ((width > 0) && (heigh > 0)){

		SendCommand(COMMAND_SET_COLUMN);
		SendData( x_st );
		SendData( x_st + heigh );

		SendCommand(COMMAND_SET_ROW);
		SendData( y_st );
		SendData( y_st + width );

		SendCommand(COMMAND_WRITE_RAM);
		for(uint8_t y = 0; y < width; ++y){
			for(uint8_t x = 0; x < heigh; ++x){
				uint8_t dot = *(ptrPicture + (x * width) + y);
				if (dot == paintFon) {
					uint8_t x_position = x_st + x;
					uint8_t y_position = y_st + y;
					drv_OLED_Display_DrawPixel(x_position, y_position, color);
				}
			}
		}

	}
}


void  drv_OLED_Display_DrawImage(uint8_t x_st,  uint8_t y_st,
                                 uint8_t width, uint8_t heigh,
		                         uint16_t *ptrPicture){
	if ((width > 0) && (heigh > 0)){

			SendCommand(COMMAND_SET_COLUMN);
			SendData( x_st );
			SendData( x_st + width );

			SendCommand(COMMAND_SET_ROW);
			SendData( y_st );
			SendData( y_st + heigh );

			SendCommand(COMMAND_WRITE_RAM);
			for(uint8_t y = 0; y < width; ++y){
				for(uint8_t x = 0; x < heigh; ++x){
					uint16_t dotColor = *(ptrPicture + (x * width) + y);
					uint8_t x_position = x_st + x;
					uint8_t y_position = y_st + y;
					drv_OLED_Display_DrawPixel(x_position, y_position, dotColor);
				}
			}

		}
}


void drv_OLED_Display_ScalePicture( uint8_t *ptrPicture, uint8_t base_Width, uint8_t base_Height,
		                                uint8_t scale, uint8_t *ptrScaledPicture){

	if(scale > 0){
		uint16_t scaled_Width = base_Width * scale;

		for (uint8_t i = 0; i < base_Width; ++i) {
			for (uint8_t j = 0; j < base_Height; ++j) {
				uint16_t dotColor = *(ptrPicture + (i * base_Width) + j);

				uint8_t x_st = i * scale;
				uint8_t y_st = j * scale;
				uint8_t x_end = x_st + scale;
				uint8_t y_end = y_st + scale;

				for (uint8_t x = x_st; x < x_end; ++x) {
					for (uint8_t y = y_st; y < y_end; ++y) {
						*(ptrScaledPicture + (x * scaled_Width) + y) = dotColor;
					}
				}
			}
		}
		
	}
}











void SendCommand(uint8_t command){
	HAL_GPIO_WritePin(display.CS.port, display.CS.pin, 0);
	HAL_GPIO_WritePin(display.DC.port, display.DC.pin, SEND_COMMAND);	
	HAL_SPI_Transmit(display.spiConfigure, &command, 1, 100);
	HAL_GPIO_WritePin(display.CS.port, display.CS.pin, 1);
}

void SendData(uint8_t data){
	HAL_GPIO_WritePin(display.CS.port, display.CS.pin, 0);
	HAL_GPIO_WritePin(display.DC.port, display.DC.pin, SEND_DATA);	
	HAL_SPI_Transmit(display.spiConfigure, &data, 1, 100);
	HAL_GPIO_WritePin(display.CS.port, display.CS.pin, 1);
}

void ResetDisplay(uint8_t state){
	uint8_t pinState = 0;
	if(state){
		pinState = 1;
	}
	HAL_GPIO_WritePin(display.RES.port, display.RES.pin, pinState);
}

