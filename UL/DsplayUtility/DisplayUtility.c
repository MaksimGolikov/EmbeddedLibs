/*
 * DisplayUtility.c
 *
 *  Created on: Feb 28, 2019
 *      Author: MGO
 */

#include "DisplayUtility.h"
#include "drv_OLED_Display.h"

#define FONT_WIDTH                5
#define FONT_HEIGHT               6

#define FONT_BIG_LETTERS_START    65
#define FONT_BIG_LETTERS_END      90
#define FONT_LITTLE_LETTERS_START 97
#define FONT_LITTLE_LETTERS_END   122
#define FONT_NUMBER_START         48
#define FONT_NUMBER_END           57


typedef struct {
	SPI_HandleTypeDef *spi;
	GPIO_TypeDef      *PortCS;  uint8_t pinCS;
	GPIO_TypeDef      *PortRES; uint8_t pinRES;
	GPIO_TypeDef      *PortDC;  uint8_t pinDC;
}DisplayContext_t;

__IO DisplayContext_t display;
__IO uint8_t          selectedScreen;


extern uint8_t font[];

static uint8_t IsTimeSpent(uint32_t startTime, uint32_t timeout){
	uint8_t res = 0;

	if( (HAL_GetTick() - startTime) > timeout ){
		res = 1;
	}

	return res;
}




void Draw_StatusBar(void* stb);
void Draw_Picture(void* pic);
void Draw_Animation(void* anim);
void Draw_Label(void* label);
void Draw_Number(void* label);



void DisplayUtility_Init(SPI_HandleTypeDef *spi, GPIO_TypeDef *PortCS,  uint8_t pinCS,
												 GPIO_TypeDef *PortRES, uint8_t pinRES,
												 GPIO_TypeDef *PortDC,  uint8_t pinDC
												 ){
	display.spi     = spi;
	display.PortCS  = PortCS;
	display.pinCS   = pinCS;
	display.PortDC  = PortDC;
	display.pinDC   = pinDC;
	display.PortRES = PortRES;
	display.pinRES  = pinRES;

	drv_OLED_Display_Init(  display.spi,
			                PortCS, PortDC, PortRES,
			                pinCS, pinDC, pinRES);

	drv_OLED_Display_FillScreen(COLOR_BLACK);

	DisplayUtility_SetScreen(FIRST_SCREEN);
}



void DisplayUtility_ChangeData(uint8_t elType, uint8_t elName, uint16_t value){
	switch(elType){
		case ELEMENT_PICTURE:
			pic_els[elName].base.shouldDraw = 1;
			pic_els[elName].color           = value;
			break;
		case ELEMENT_STATUSBAR:
			stb_els[elName].base.shouldDraw = 1;

			if(value > stb_els[elName].currentValue){
				stb_els[elName].isIncreased = 1;
			}
			stb_els[elName].currentValue    = value;
			break;
		case ELEMENT_ANIMATION:
			anim_els[elName].base.shouldDraw = 1;
			anim_els[elName].timeout         = value;
			break;
		case ELEMENT_LABEL:
			label_els[elName].base.shouldDraw = 1;

			if(label_els[elName].isItText){
				label_els[elName].color           = value;
			}else{
				label_els[elName].value           = value;
			}
			break;
		default:
			break;
	}
}

void DisplayUtility_SetScreen(uint8_t screen){
	selectedScreen = screen;

	drv_OLED_Display_FillRectangle( screens[selectedScreen].pos_x,
									screens[selectedScreen].pos_y,
									screens[selectedScreen].pos_x + screens[selectedScreen].width,
									screens[selectedScreen].pos_y + screens[selectedScreen].height,
									screens[selectedScreen].fon_color);


	for(uint8_t el = 0; el < MAX_EL_ON_SCREEN; el++){
		if(screens[selectedScreen].control[el].ptrToElement != 0){
			switch(screens[selectedScreen].control[el].typeOfElement){
				case ELEMENT_PICTURE:
					((Picture_t*)screens[selectedScreen].control[el].ptrToElement)->base.shouldDraw   = 1;
					break;
				case ELEMENT_STATUSBAR:
					((Statusbar_t*)screens[selectedScreen].control[el].ptrToElement)->base.shouldDraw = 1;
					((Statusbar_t*)screens[selectedScreen].control[el].ptrToElement)->borderDraw      = 1;
					break;
				case ELEMENT_ANIMATION:
					((Animation_t*)screens[selectedScreen].control[el].ptrToElement)->base.shouldDraw = 1;
					break;
				case ELEMENT_LABEL:
					((Label_t*)screens[selectedScreen].control[el].ptrToElement)->base.shouldDraw     = 1;
					break;
				default:
					break;
				}
		}
	}
}

void DisplayUtility_Run(void){
    for(uint8_t el = 0; el < MAX_EL_ON_SCREEN; el ++){
    	if(screens[selectedScreen].control[el].ptrToElement != 0){
			switch(screens[selectedScreen].control[el].typeOfElement){
			case ELEMENT_PICTURE:
				((Picture_t*)screens[selectedScreen].control[el].ptrToElement)->base.Draw(screens[selectedScreen].control[el].ptrToElement);
				break;
			case ELEMENT_STATUSBAR:
				((Statusbar_t*)screens[selectedScreen].control[el].ptrToElement)->base.Draw(screens[selectedScreen].control[el].ptrToElement);
				break;
			case ELEMENT_ANIMATION:
				((Animation_t*)screens[selectedScreen].control[el].ptrToElement)->base.Draw(screens[selectedScreen].control[el].ptrToElement);
				break;
			case ELEMENT_LABEL:
				((Label_t*)screens[selectedScreen].control[el].ptrToElement)->base.Draw(screens[selectedScreen].control[el].ptrToElement);
				break;
			default:
				break;
			}
    	}
    }
}





//=== DECLARATION ELEMENTS
int8_t DisplayUtility_Config_StatusBar(uint8_t name,
		                               uint8_t verticalOrientation,
		                               uint8_t x_pos, uint8_t y_pos,
									   uint8_t width, uint8_t height,
									   uint16_t min_value, uint16_t max_value,
									   uint16_t colorBorder, uint16_t colorFilling){

int8_t res = -1;
	if(name < Amount_STATUSBAR){
		stb_els[name].isVerticalOrintation = verticalOrientation;
		stb_els[name].base.pos_x           = x_pos;
		stb_els[name].base.pos_y           = y_pos;
		stb_els[name].base.width           = width;
		stb_els[name].base.height          = height;
		stb_els[name].base.shouldDraw      = 0;
		stb_els[name].base.Draw            = Draw_StatusBar;
		stb_els[name].borderColor          = colorBorder;
		stb_els[name].fillingColor         = colorFilling;
		stb_els[name].currentValue         = 0;
		stb_els[name].maxValue             = max_value;
		stb_els[name].minValue             = min_value;
		stb_els[name].isIncreased          = 1;

		res = 1;
	}


return res;
}

int8_t DisplayUtility_Config_Picture(uint8_t name,
		                             uint8_t x_pos, uint8_t y_pos,
									 uint8_t width, uint8_t height,
									 uint8_t *ptrToPicture,
									 uint16_t color){

	int8_t res = -1;

	if(name < Amount_PICTURE){
		res = 1;

		pic_els[name].base.pos_x      = x_pos;
		pic_els[name].base.pos_y      = y_pos;
		pic_els[name].base.width      = width;
		pic_els[name].base.height     = height;
		pic_els[name].base.shouldDraw = 1;
		pic_els[name].base.Draw       = Draw_Picture;
		pic_els[name].color           = color;
		pic_els[name].dataSize        = (width*height);
		pic_els[name].ptrToData       = ptrToPicture;

	}

	return res;
}

int8_t DisplayUtility_Config_Animation(uint8_t name,
									   uint8_t x_pos, uint8_t y_pos,
									   uint8_t width, uint8_t height,
									   uint8_t *ptrToPicture,
									   uint16_t color, uint16_t sizeImage,
									   uint8_t numberOfImages, uint32_t timeoutChanges){

	int8_t res = -1;

	if(name < Amount_ANIATION){
		res = 1;

		anim_els[name].base.pos_x      = x_pos;
		anim_els[name].base.pos_y      = y_pos;
		anim_els[name].base.width      = width;
		anim_els[name].base.height     = height;
		anim_els[name].base.shouldDraw = 1;
		anim_els[name].base.Draw       = Draw_Animation;
		anim_els[name].color           = color;
		anim_els[name].numberOfImages  = numberOfImages;
		anim_els[name].selectedImage   = 0;
		anim_els[name].timeout         = timeoutChanges;
		anim_els[name].time            = HAL_GetTick();
		anim_els[name].ptrToData       = ptrToPicture;
		anim_els[name].size            = sizeImage;

	}

	return res;
}


int8_t DisplayUtility_Config_Label(uint8_t name, uint8_t isItTtext,
								   uint8_t x_pos, uint8_t y_pos,
								   uint8_t scale,
								   const char *ptrToPicture, uint8_t length,
								   uint16_t color){

	int8_t res = -1;

	if(name < Amount_LABEL){
		res = 1;

		label_els[name].base.pos_x      = x_pos;
		label_els[name].base.pos_y      = y_pos;
		label_els[name].base.width      = FONT_WIDTH;
		label_els[name].base.height     = FONT_HEIGHT;
		label_els[name].base.shouldDraw = 1;
		label_els[name].isItText        = isItTtext;
		if(isItTtext){
			label_els[name].base.Draw   = Draw_Label;
		}else{
			label_els[name].base.Draw   = Draw_Number;
		}
		label_els[name].color           = color;
		label_els[name].scale           = scale;
		label_els[name].length          = length;
		label_els[name].ptrToData       = ptrToPicture;

	}

	return res;
}


//=====




//====PRIVATE
void Draw_StatusBar(void* stb){
	Statusbar_t* ptrStb = stb;

	if(ptrStb->base.shouldDraw){
		ptrStb->base.shouldDraw = 0;

        if(ptrStb->borderDraw){
        	ptrStb->borderDraw = 0;

        	drv_OLED_Display_FillRectangle(ptrStb->base.pos_x,
        										   ptrStb->base.pos_y,
        										   (ptrStb->base.pos_x + ptrStb->base.width),
        										   (ptrStb->base.pos_y + ptrStb->base.height),
        										   ptrStb->borderColor);
        	drv_OLED_Display_FillRectangle( (ptrStb->base.pos_x  + 1),
        								    (ptrStb->base.pos_y + 1),
        								    (ptrStb->base.pos_x + ptrStb->base.width - 1),
        								    (ptrStb->base.pos_y + ptrStb->base.height - 1),
											screens[selectedScreen].fon_color);
        }

	   uint8_t  start_x;
	   uint8_t  start_y;
	   uint8_t  finish_x;
	   uint8_t  finish_y;
	   uint16_t color;


	   if(ptrStb->isVerticalOrintation){
		   uint8_t  heightDelta = ((ptrStb->currentValue * ptrStb->base.height) / ptrStb->maxValue);

		  if(ptrStb->isIncreased){
		   start_x  = ptrStb->base.pos_x + 1;
		   if(ptrStb->currentValue >= ptrStb->maxValue -1){start_y =  ptrStb->base.pos_y + 1;}
		   else {   start_y  = ptrStb->base.pos_y + (ptrStb->base.height - heightDelta);}
		   finish_x = ptrStb->base.pos_x + ptrStb->base.width - 1;
		   finish_y = ptrStb->base.pos_y + ptrStb->base.height - 1;
		   color    = ptrStb->fillingColor;
		  }else{
		   start_x  = ptrStb->base.pos_x + 1;
		   start_y  = start_y  = ptrStb->base.pos_y + (ptrStb->base.height - heightDelta);
		   finish_x = ptrStb->base.pos_x + ptrStb->base.width - 1;
		   finish_y = ptrStb->base.pos_y + ptrStb->base.height - 1;
		   color    = ptrStb->fillingColor;

		   drv_OLED_Display_FillRectangle( start_x, start_y,
		  									   finish_x,finish_y,
		  									   color);

		   start_x  = ptrStb->base.pos_x + 1;
		   start_y  = ptrStb->base.pos_y + 1;
		   finish_x = ptrStb->base.pos_x + ptrStb->base.width - 1;
		   finish_y = ptrStb->base.pos_y + ptrStb->base.height - heightDelta - 1;
		   color    = screens[selectedScreen].fon_color;
		  }
	   }else{
		  uint16_t  widthDelta = ((ptrStb->currentValue * ptrStb->base.width) / ptrStb->maxValue);

		  if(ptrStb->isIncreased){
		   start_x  = ptrStb->base.pos_x + 1;
		   start_y =  ptrStb->base.pos_y + 1;

		   finish_x = ptrStb->base.pos_x + widthDelta - 1;
		   if(ptrStb->currentValue >= ptrStb->maxValue -1){finish_x = ptrStb->base.pos_x + ptrStb->base.width - 1;}

		   finish_y = ptrStb->base.pos_y + ptrStb->base.height - 1;
		   color    = ptrStb->fillingColor;
		  }else{

			start_x  = ptrStb->base.pos_x + 1;
		    start_y  = ptrStb->base.pos_y + 1;
		    finish_x = ptrStb->base.pos_x + widthDelta - 1;
		    if(ptrStb->currentValue >= ptrStb->maxValue -1){finish_x = ptrStb->base.pos_x + ptrStb->base.width - 1;}
		    finish_y = ptrStb->base.pos_y + ptrStb->base.height - 1;
		    color    = ptrStb->fillingColor;

		    drv_OLED_Display_FillRectangle( start_x, start_y,
										   finish_x,finish_y,
										   color);

			start_x   = ptrStb->base.pos_x + widthDelta + 1;
			start_y   = ptrStb->base.pos_y + 1;
		    finish_x  = ptrStb->base.pos_x + ptrStb->base.width - 1;
		    finish_y  =  ptrStb->base.pos_y + ptrStb->base.height - 1;

		    color     = screens[selectedScreen].fon_color;
		  }
	   }




       ptrStb->isIncreased = 0;
	   drv_OLED_Display_FillRectangle( start_x, start_y,
									   finish_x,finish_y,
									   color);
	}
}

void Draw_Picture(void* pic){
	Picture_t *ptrPic = pic;

	if(ptrPic->base.shouldDraw){
		ptrPic->base.shouldDraw = 0;

		uint8_t scale = 1;
		drv_OLED_Display_ScalePicture(ptrPic->base.pos_x,
				                      ptrPic->base.pos_y,
									  ptrPic->ptrToData,
									  ptrPic->base.height,
									  ptrPic->base.width,
									  scale,
									  ptrPic->color);
	}

}

void Draw_Animation(void* anim){
	Animation_t *ptrAnim = anim;

	if(ptrAnim->base.shouldDraw){
		 if(IsTimeSpent(ptrAnim->time, ptrAnim->timeout)){
			 ptrAnim->time = HAL_GetTick();


			uint8_t  scale       = 1;
			uint8_t  *ptrToFrame = &ptrAnim->ptrToData[ptrAnim->selectedImage * ptrAnim->size];

			ptrAnim->selectedImage ++;
			if(ptrAnim->selectedImage >= ptrAnim->numberOfImages){
				ptrAnim->selectedImage = 0;
			}

			drv_OLED_Display_ScalePicture(ptrAnim->base.pos_x,
										  ptrAnim->base.pos_y,
										  ptrToFrame,
										  ptrAnim->base.height,
										  ptrAnim->base.width,
										  scale,
										  ptrAnim->color);

		 }
	}
}

uint16_t Get_IndexByID(uint8_t symbvol_id){
	uint16_t symbvol_index = 0;
	if(symbvol_id >= FONT_NUMBER_START && symbvol_id <= FONT_NUMBER_END){
		symbvol_index = symbvol_id - FONT_NUMBER_START;
	}else if(symbvol_id >= FONT_BIG_LETTERS_START && symbvol_id <= FONT_BIG_LETTERS_END){
		symbvol_index = (symbvol_id - FONT_BIG_LETTERS_START) + 10;
	}else if(symbvol_id >= FONT_LITTLE_LETTERS_START && symbvol_id <= FONT_LITTLE_LETTERS_END){
		symbvol_index = (symbvol_id - FONT_LITTLE_LETTERS_START) + 10;
	}else{
		switch(symbvol_id){
		case 32:
			symbvol_index = 40;
			break;
		case 33:
			symbvol_index = 38;
			break;
		case 45:
			symbvol_index = 37;
			break;
		case 58:
			symbvol_index = 36;
			break;
		default :
			symbvol_index = 39;
			break;
		}
	}
	symbvol_index *= 4;

	return symbvol_index;
}
void Draw_Label(void* label){

	Label_t *lab = label;

	if(lab->base.shouldDraw){
		lab->base.shouldDraw = 0;

		for(uint8_t i = 0; i < lab->length; i++){

			uint8_t mass[4]  = {0};
			uint16_t stIndex = Get_IndexByID(lab->ptrToData[i]);

			memcpy(mass, &font[stIndex], sizeof(mass));

			 drv_OLED_Display_ScalePicture( (lab->base.pos_x +(i * FONT_WIDTH * lab->scale) ),
					                         lab->base.pos_y,
											 mass,
											 FONT_HEIGHT,
											 FONT_WIDTH,
											 lab->scale,
											 lab->color);
		}
	}

}


void Draw_Numeral(uint8_t x, uint8_t y, uint8_t scale, uint8_t value, uint16_t color){
	uint8_t mass[4] = {0};

	uint16_t stIndex = value * 4;
	memcpy(mass, &font[stIndex], sizeof(mass));

	 drv_OLED_Display_ScalePicture(  x,
									 y,
									 mass,
									 FONT_HEIGHT,
									 FONT_WIDTH,
									 scale,
									 color);
}
void Draw_Number(void* label){
	Label_t *lab = label;

	if(lab->base.shouldDraw){
		uint8_t step          = (FONT_WIDTH * lab->scale) - lab->scale;
		uint8_t drawSchema    = 1;
		uint8_t values[4]     = {0};

		lab->base.shouldDraw = 0;

		values[0]            = lab->value / 1000;
		values[1]            = (lab->value % 1000) / 100;
		values[2]            = (lab->value % 100)  / 10;
		values[3]            = (lab->value % 10)   / 1;


		if(values[0] != 0){
			drawSchema = 4;
		}else{
			if(values[1] != 0){
				drawSchema = 3;
			}else{
				if(values[2] != 0){
					drawSchema = 2;
				}
			}
		}


		uint8_t element = 0;
		drv_OLED_Display_FillRectangle(lab->base.pos_x,
									   lab->base.pos_y,
									   lab->base.pos_x + FONT_WIDTH*lab->oldValueDegree*lab->scale,
									   lab->base.pos_y + FONT_HEIGHT * lab->scale,
									   screens[selectedScreen].fon_color);
		for(uint8_t itr = (4-drawSchema); itr < 4; itr ++){
			Draw_Numeral( (lab->base.pos_x + step * element),
					       lab->base.pos_y,
					       lab->scale,
						   values[itr],
						   lab->color);
			element ++;
		}
		lab->oldValueDegree = drawSchema;

	}
}

