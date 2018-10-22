/*
 * Name        drv_OLED_Display.c
 * Author      Maksim Golikov (SW developer)
 * Created on: 03 APLIL, 2018
 * Description driver for control OLED display ssd1351 (source file)
 */

#include "DRV/drv_OLED_Display.h"
#include "DRV/drv_OLED_DisplayCommand.h"
#include "string.h"
#include "stdbool.h"

#define SCREEN_WIDTH       0x7F
#define SCREEN_HEIGH       0x7F

#define SEND_COMMAND       0
#define SEND_DATA          1


typedef struct {
	GPIO_TypeDef *port;
	uint8_t      pin;
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

void ResetDisplay(void);
void OLED_Select(void);
void OLED_Unselect(void);
void OLED_SetAddressWindow(uint8_t x_st, uint8_t y_st, uint8_t x_end, uint8_t y_end);


HAL_StatusTypeDef SendOLED(uint8_t *data, uint16_t size, bool CmdData);
HAL_StatusTypeDef SendDataSync(uint8_t *data, uint16_t size);
HAL_StatusTypeDef WriteDispSpiSync(uint8_t *ptrData, uint16_t size, bool CmdData);
HAL_StatusTypeDef SendCommandSync(uint8_t *cmd);


void drv_OLED_Display_Init(SPI_HandleTypeDef *spi,
		                   GPIO_TypeDef *_portCS, GPIO_TypeDef *_portDC, GPIO_TypeDef *_portRES,
		                   uint8_t _pinCS, uint8_t _pinDC, uint8_t _pinRES){
												 
	display.spiConfigure = spi;
	display.CS.port     = _portCS;
	display.DC.port     = _portDC;
	display.RES.port    = _portRES;
	display.CS.pin      = _pinCS;
	display.DC.pin      = _pinDC;
	display.RES.pin     = _pinRES;

	//Initialize display process
	OLED_Select();
	ResetDisplay();

	uint8_t data[10] = {0};
	data[0] = 0x12;
	uint8_t cmd = COMMAND_COMMAND_LOCK;
	WriteDispSpiSync(&cmd, 1, SEND_COMMAND);
	WriteDispSpiSync(data, sizeof(data), SEND_DATA);

	data[0] = 0xB1;
	cmd = COMMAND_COMMAND_LOCK;
	WriteDispSpiSync(&cmd, 1, SEND_COMMAND);
	WriteDispSpiSync(data, sizeof(data), SEND_DATA);

	cmd = COMMAND_DISPLAY_OFF;
	WriteDispSpiSync(&cmd, 1, SEND_COMMAND);

	data[0] = 0x10;
	cmd = COMMAND_CLOCK_DIV;
	WriteDispSpiSync(&cmd, 1, SEND_COMMAND);
	WriteDispSpiSync(data, sizeof(data), SEND_DATA);
	
	data[0] = 0x7F;
	cmd = COMMAND_MUXRATIO;
	WriteDispSpiSync(&cmd, 1, SEND_COMMAND);
	WriteDispSpiSync(data, sizeof(data), SEND_DATA);

	data[0] = 0x00;
	cmd = COMMAND_DISPLAY_OFFSET;
	WriteDispSpiSync(&cmd, 1, SEND_COMMAND);
	WriteDispSpiSync(data, sizeof(data), SEND_DATA);

	data[0] = 0x00;
	cmd = COMMAND_STARTLINE;
	WriteDispSpiSync(&cmd, 1, SEND_COMMAND);
	WriteDispSpiSync(data, sizeof(data), SEND_DATA);

	data[0] = 0x74;
	cmd = COMMAND_SET_REMAP;
	WriteDispSpiSync(&cmd, 1, SEND_COMMAND);
	WriteDispSpiSync(data, sizeof(data), SEND_DATA);

	data[0] = 0x00;
	cmd = COMMAND_SET_GPIO;
	WriteDispSpiSync(&cmd, 1, SEND_COMMAND);
	WriteDispSpiSync(data, sizeof(data), SEND_DATA);

	data[0] = 0xC1;
	cmd = COMMAND_FUNCTION_SELECT;
	WriteDispSpiSync(&cmd, 1, SEND_COMMAND);
	WriteDispSpiSync(data, sizeof(data), SEND_DATA);

	data[0] = 0xA0;
	data[1] = 0xB5;
	data[2] = 0x55;
	cmd = COMMAND_SET_VSL;
	WriteDispSpiSync(&cmd, 1, SEND_COMMAND);
	WriteDispSpiSync(data, sizeof(data), SEND_DATA);

	data[0] = 0xC8;//Color A //8a
	data[1] = 0xA8;//Color B //51
	data[2] = 0xC8;//Color C //8a
	cmd = COMMAND_CONTRAST_ABC;
	WriteDispSpiSync(&cmd, 1, SEND_COMMAND);
	WriteDispSpiSync(data, sizeof(data), SEND_DATA);

	data[0] = 0x0F;	
	cmd = COMMAND_CONTRAST_MASTER;
	WriteDispSpiSync(&cmd, 1, SEND_COMMAND);
	WriteDispSpiSync(data, sizeof(data), SEND_DATA);

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

	data[0] = 0x32;
	cmd = COMMAND_PRECHARGE;
	WriteDispSpiSync(&cmd, 1, SEND_COMMAND);
	WriteDispSpiSync(data, sizeof(data), SEND_DATA);

	data[0] = 0x16;
	cmd = COMMAND_PRECHARGE_LEVEL;
	WriteDispSpiSync(&cmd, 1, SEND_COMMAND);
	WriteDispSpiSync(data, sizeof(data), SEND_DATA);

	data[0] = 0x01;
	data[1] = 0x09;
	data[2] = 0x0F;
	cmd = COMMAND_PRECHARGE2;
	WriteDispSpiSync(&cmd, 1, SEND_COMMAND);
	WriteDispSpiSync(data, sizeof(data), SEND_DATA);

	data[0] = 0x05;
	cmd = COMMAND_VCOMH;
	WriteDispSpiSync(&cmd, 1, SEND_COMMAND);
	WriteDispSpiSync(data, sizeof(data), SEND_DATA);

	cmd = COMMAND_NORMAL_DISPLAY;
	WriteDispSpiSync(&cmd, 1, SEND_COMMAND);

	data[0] = 0;
	data[1] = SCREEN_WIDTH;
	cmd = COMMAND_SET_COLUMN;
	WriteDispSpiSync(&cmd, 1, SEND_COMMAND);
	WriteDispSpiSync(data, sizeof(data), SEND_DATA);

	data[0] = 0;
	data[1] = SCREEN_HEIGH;
	cmd = COMMAND_SET_ROW;
	WriteDispSpiSync(&cmd, 1, SEND_COMMAND);
	WriteDispSpiSync(data, sizeof(data), SEND_DATA);

	cmd = COMMAND_DISPLAY_ON;
	WriteDispSpiSync(&cmd, 1, SEND_COMMAND);
	
	OLED_Unselect();
}


void drv_OLED_Display_Clear(void){
	drv_OLED_Display_FillRectangle(0,0,127,127,COLOR_WHITE);
}

void drv_OLED_Display_FillRectangle( uint8_t x_st,  uint8_t y_st,
		                             uint8_t x_end, uint8_t y_end,
									                uint16_t color) {
	
	if ((x_st <= x_end) && (y_st <= y_end)) {
		OLED_SetAddressWindow(x_st, y_st, x_end, y_end);
		
		uint16_t pixelsCount = (x_end - x_st + 1) * (y_end - y_st + 1);
		uint16_t inv_color = (color >> 8) | ((color & 0xFF) << 8);
		uint16_t buff[128];
		for(uint32_t i = 0;i < 128; i++){
			buff[i] = inv_color;
		}
		for(uint32_t i = 0; i <= pixelsCount/128; i++){
			WriteDispSpiSync((uint8_t*)buff, sizeof(buff), SEND_DATA);
		}
		if( (pixelsCount & 0x7F) != 0){
			WriteDispSpiSync((uint8_t*)buff, (pixelsCount & 0x7F) * 2, SEND_DATA);
		}
	}
}
																		
void drv_OLED_Display_FillScreen(uint16_t color){
	drv_OLED_Display_FillRectangle(0, 0, SCREEN_HEIGH, SCREEN_WIDTH, color);
}

void drv_OLED_Display_DrawPixel(uint8_t x, uint8_t y, uint16_t color){
	if((x >= SCREEN_WIDTH) || (y >= SCREEN_HEIGH)){return;}
	OLED_Select();
	
	OLED_SetAddressWindow(x, y, x+1, y+1);
	uint8_t data[] = { color >> 8, color & 0xFF };
	WriteDispSpiSync(data, sizeof(data), SEND_DATA);
	
	OLED_Unselect();
}

void drv_OLED_Display_DrawPictire(  uint8_t x_st,  uint8_t y_st,
		                            uint8_t width, uint8_t heigh,
								    uint8_t *ptrPicture, uint16_t color, uint8_t paintFon){
	if ((width > 0) && (heigh > 0)){
		OLED_SetAddressWindow(x_st, y_st, (x_st + heigh) - 1, (y_st + width) - 1);
		uint16_t inv_color = (color >> 8) | ((color & 0xFF) << 8);
		uint16_t buff[128];
		uint8_t count = 0;
		uint8_t dot = 0;
		for(uint8_t y = 0; y < width; ++y){
			for(uint8_t x = 0; x < heigh; ++x){
				dot = *(ptrPicture + (x * width) + y);
				if (dot == paintFon){
					buff[count] = inv_color;
				}
				else{
					buff[count] = 0;
				}
				count++;
				if(count == 128){
					WriteDispSpiSync((uint8_t*)buff, sizeof(buff), SEND_DATA);
					count = 0;
				}
			}
		}
		if(count > 0){
			WriteDispSpiSync((uint8_t*)buff, count*2, SEND_DATA);
		}
	}
}
void drvDrawPictire(uint8_t x_st,  uint8_t y_st,
		                              uint8_t width, uint8_t heigh,
								                  uint8_t *ptrPicture, uint16_t color, uint8_t paintFon){
	if ((width > 0) && (heigh > 0)){
		OLED_SetAddressWindow(x_st, y_st, (x_st + heigh) - 1, (y_st + width) - 1);
		uint16_t inv_color = (color >> 8) | ((color & 0xFF) << 8);
		uint16_t buff[128] = {0};
		uint32_t count = 0;
		uint32_t size = heigh * width;
		for(uint32_t nPix = 0; nPix < size; ++nPix){
			if ((ptrPicture[nPix >> 3] & (1 << (nPix & 7))) != 0){
				buff[count] = inv_color;
			}
			else{
				buff[count] = 0;
			}
			count++;
			if(count == 128){
				WriteDispSpiSync((uint8_t*)buff, sizeof(buff), SEND_DATA);
				count = 0;
			}		
		}
		if(count > 0){
			WriteDispSpiSync((uint8_t*)buff, count*2, SEND_DATA);
		}
	}
}


void drv_OLED_Display_DrawImage(uint8_t x_st,  uint8_t y_st,
                                 uint8_t width, uint8_t heigh,
		                         uint16_t *ptrPicture){
	if ((width > 0) && (heigh > 0)){
		uint8_t data[2] = {x_st, x_st + heigh};
		uint8_t cmd = COMMAND_SET_COLUMN;
		WriteDispSpiSync(&cmd, 1, SEND_COMMAND);
		WriteDispSpiSync(data, sizeof(data), SEND_DATA);
		
		data[0] = y_st;
		data[1] = y_st + width;
		cmd = COMMAND_SET_ROW;
		WriteDispSpiSync(&cmd, 1, SEND_COMMAND);
		WriteDispSpiSync(data, sizeof(data), SEND_DATA);

		cmd = COMMAND_WRITE_RAM;
		WriteDispSpiSync(&cmd, 1, SEND_COMMAND);
		uint16_t dotColor = 0;
		uint8_t x_position = 0;
		uint8_t y_position = 0;
		for(uint8_t y = 0; y < width; ++y){
			for(uint8_t x = 0; x < heigh; ++x){
				dotColor = *(ptrPicture + (x * width) + y);
				x_position = x_st + x;
				y_position = y_st + y;
				drv_OLED_Display_DrawPixel(x_position, y_position, dotColor);
			}
		}
	}
}

void drv_OLED_Display_ScalePicture( uint8_t x_st,  uint8_t y_st, uint8_t *ptrSource, uint8_t base_Height, uint8_t  base_Width,
		                            uint8_t scale, uint16_t color){
	if ((base_Width > 0) && (base_Height > 0)){
		OLED_SetAddressWindow(x_st, y_st, (x_st + (base_Width * scale)) - 1, (y_st + (base_Height * scale)) - 1);
		uint16_t inv_color = (color >> 8) | ((color & 0xFF) << 8);
		uint16_t buff[128] = {0};
		uint32_t count = 0;
		for(uint32_t y = 0; y < base_Height; y++){
			for(uint32_t verScale = 0; verScale < scale; verScale++){
				for(uint32_t x = 0; x < base_Width; x++){
					uint32_t nPix = y * base_Width + x;
					uint16_t dotColor = ((ptrSource[nPix >> 3] & (1 << (nPix & 7))) != 0) ? inv_color : 0;
					for(uint32_t scPix = 0; scPix < scale; scPix++){
						buff[count] = dotColor;
						count++;
						if(count == 128){
							WriteDispSpiSync((uint8_t*)buff, sizeof(buff), SEND_DATA);
							count = 0;
						}		
					}
				}
			}
		}
		if(count > 0){
			WriteDispSpiSync((uint8_t*)buff, count*2, SEND_DATA);
		}
	}	
}
																	

void SendCommand(uint8_t command){
	HAL_GPIO_WritePin(display.DC.port, GPIO_PIN_0 << display.DC.pin, (GPIO_PinState)SEND_COMMAND);	
	HAL_SPI_Transmit(display.spiConfigure, &command, 1, 100);	
}

void SendData(uint8_t data){
	HAL_GPIO_WritePin(display.DC.port, GPIO_PIN_0 << display.DC.pin, (GPIO_PinState)SEND_DATA);	
	HAL_SPI_Transmit(display.spiConfigure, &data, 1, 100);
}

void OLED_SetAddressWindow(uint8_t x_st, uint8_t y_st, uint8_t x_end, uint8_t y_end) {
    // column address set
	uint8_t cmd = COMMAND_SET_COLUMN;
	WriteDispSpiSync(&cmd, 1, SEND_COMMAND);

	uint8_t data[] = { x_st, x_end };
	WriteDispSpiSync(data, sizeof(data), SEND_DATA);
	
	// row address set
	cmd = COMMAND_SET_ROW;
	WriteDispSpiSync(&cmd, 1, SEND_COMMAND);
	uint8_t data_y[] = { y_st, y_end };
	WriteDispSpiSync(data_y, sizeof(data_y), SEND_DATA);

	// write to RAM
	cmd = COMMAND_WRITE_RAM;
	WriteDispSpiSync(&cmd, 1, SEND_COMMAND); 
}

HAL_StatusTypeDef SendDataSync(uint8_t *data, uint16_t size){
	HAL_StatusTypeDef status;
	OLED_Select();
	HAL_GPIO_WritePin(display.DC.port, GPIO_PIN_0 << display.DC.pin, (GPIO_PinState)SEND_DATA);	
	status = HAL_SPI_Transmit(display.spiConfigure, data, size, 500);
	OLED_Unselect();
	return status;
}

HAL_StatusTypeDef SendCommandSync(uint8_t *cmd){
	HAL_StatusTypeDef status;
	OLED_Select();
	HAL_GPIO_WritePin(display.DC.port, GPIO_PIN_0 << display.DC.pin, (GPIO_PinState)SEND_COMMAND);
	status = HAL_SPI_Transmit(display.spiConfigure, cmd, 1, 500);
	OLED_Unselect();
	return status;
}

HAL_StatusTypeDef WriteDispSpiSync(uint8_t *ptrData, uint16_t size, bool CmdData){
	HAL_StatusTypeDef status;
	status = SendOLED(ptrData, size, CmdData);
	return status;
}

HAL_StatusTypeDef SendOLED(uint8_t *data, uint16_t size, bool CmdData){
	HAL_StatusTypeDef status;
	if(CmdData == SEND_DATA){
		status = SendDataSync(data, size);
	}
	else if(CmdData == SEND_COMMAND){
		status = SendCommandSync(data);
	}
	
	return status;
}

void ResetDisplay(void){
	HAL_GPIO_WritePin(display.RES.port, display.RES.pin, (GPIO_PinState)RESET);
	HAL_Delay(50);
	HAL_GPIO_WritePin(display.RES.port, display.RES.pin, (GPIO_PinState)SET);
	HAL_Delay(50);
	HAL_GPIO_WritePin(display.RES.port, display.RES.pin, (GPIO_PinState)RESET);
	HAL_Delay(50);
	HAL_GPIO_WritePin(display.RES.port, display.RES.pin, (GPIO_PinState)SET);
	HAL_Delay(50);
}

void OLED_Select(void) {
	HAL_GPIO_WritePin(display.CS.port, GPIO_PIN_0 << display.CS.pin, (GPIO_PinState)0);
}

void OLED_Unselect(void) {
    HAL_GPIO_WritePin(display.CS.port, GPIO_PIN_0 << display.CS.pin, (GPIO_PinState)1);
}
