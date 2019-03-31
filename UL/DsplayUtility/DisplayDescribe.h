/*
 * DisplayDescribe.h
 *
 *  Created on: Mar 27, 2019
 *      Author: MGO
 */

#ifndef DISPLAYDESCRIBE_H_
#define DISPLAYDESCRIBE_H_

#include <stdint.h>

#define MAX_EL_ON_SCREEN  10

//========= DESCRIPTIONS OF DATA TYPE
typedef struct{
	uint8_t pos_x;
	uint8_t pos_y;
	uint8_t width;
	uint8_t height;

	uint8_t shouldDraw :1;
	void    (*Draw)  (void* ptrEl);
}baseContext_t;


/**
 * ELEMENTS
 */
typedef struct{
	baseContext_t base;

	uint16_t      color;
	uint16_t      dataSize;
	uint8_t       *ptrToData;
}Picture_t;

typedef struct{
	baseContext_t base;

	uint8_t       isIncreased :1;
	uint8_t       borderDraw  :1;
	uint8_t       isVerticalOrintation :1;
	uint16_t      borderColor;
	uint16_t      fillingColor;
	uint16_t      maxValue;
	uint16_t      minValue;
	uint16_t      currentValue;

}Statusbar_t;

typedef struct{
	baseContext_t base;

	uint32_t      timeout;
	uint32_t      time;
	uint16_t      color;
	uint8_t       numberOfImages : 4;
	uint8_t       selectedImage  : 4;
	uint8_t       *ptrToData;
	uint16_t      size;
}Animation_t;

typedef struct{
	baseContext_t base;

	uint16_t      color;
	uint16_t      length;
	uint8_t       *ptrToData;
	uint16_t      value;
	uint16_t      oldValueDegree;
	uint8_t       isItText :1;
	uint8_t       scale;
}Label_t;
/**
 * END OF ELEMENTS
 */



typedef struct{
	uint8_t typeOfElement;
	void    *ptrToElement;
}ElementContext_t;

typedef struct{
	uint8_t pos_x;
	uint8_t pos_y;
	uint8_t width;
	uint8_t height;

	ElementContext_t control[MAX_EL_ON_SCREEN];
}ScreenContext_t;

//===========================================




//Possible type of elements
enum{
	ELEMENT_PICTURE,
	ELEMENT_STATUSBAR,
	ELEMENT_ANIMATION,
	ELEMENT_LABEL,
	ELEMENT_Amount
};

// Names of existed statusbar
enum{
	LOAD_STATUSBAR,
	Amount_STATUSBAR
};

// Names of existed picture
enum{
    P1_PICTURE,
	P2_PICTURE,
	Amount_PICTURE
};

// Names of existed animations
enum{
	LOAD_ANIMATION,
	Amount_ANIATION
};

// Names of existed animations
enum{
	NAME_LABEL,
	NUMBER_LABEL,
	Amount_LABEL
};

// Name of existed screens
enum{
	FIRST_SCREEN,
	SECOND_SCREEN,
	Amount_SCREEN
};



static Statusbar_t stb_els  [Amount_STATUSBAR]   ={0};
static Picture_t   pic_els  [Amount_PICTURE]     ={0};
static Animation_t anim_els [Amount_ANIATION]    ={0};
static Label_t     label_els[Amount_LABEL]       ={0};



static ScreenContext_t screens [Amount_SCREEN] ={
		{
		   0,0,
		   127, 127,
		   { {ELEMENT_PICTURE,  &pic_els[P1_PICTURE]},
			 {ELEMENT_STATUSBAR,&stb_els  [LOAD_STATUSBAR]},
			 //{ELEMENT_ANIMATION,&anim_els [LOAD_ANIMATION]},
			 {ELEMENT_LABEL,    &label_els[NAME_LABEL]},
			 {ELEMENT_LABEL,    &label_els[NUMBER_LABEL]},
		   }
		},
	//=====
		{
		   0,0,
		   127, 127,
		   { {ELEMENT_PICTURE,  &pic_els[P2_PICTURE]},
		     {ELEMENT_STATUSBAR,&stb_els[LOAD_STATUSBAR]},
		   }
		}
};




#endif /* DISPLAYDESCRIBE_H_ */
