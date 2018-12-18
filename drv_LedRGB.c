/*
 * Name         drv_LedRGB.c
 * Author       Maksim Golikov (SW developer)
 * Created      19 MARCH 2018
 * Description  driver for control RGB led (source file)
 */
 
 #include "DRV/drv_LedRGB.h"
 #include "DRV/drv_SysClock.h"
 
 
 #define STEP_INTENSITY    1
 

typedef enum{
	RED,
	GREEN,
	BLUE
}led_colors_t;


typedef struct{
	TIM_HandleTypeDef *timer;
	uint8_t           channel;
}pwm_color_t;

typedef struct{
	uint8_t red;
	uint8_t green;
	uint8_t blue;
}color_t;

typedef struct{
	uint8_t             color;
	
	pwm_color_t         red;
	pwm_color_t         green;
	pwm_color_t         blue;

	rgbLed_SwitchMode_t turnOnMode;
	rgbLed_WorkMode_t   workMode;
	rgbLed_WorkMode_t   state;
	uint32_t            blinkPeriod;
	uint32_t            startBlinkTime;

	color_t             current_PWM;
	color_t		          max_PWM;
}LedDescription_t;


 
 void LounchPWM(rgbLeds_t led);
 void Set_Puls(rgbLeds_t led, led_colors_t color);

 
 static uint16_t defineColors [color_Amount][3] = {
	 // Red_part  Green_part  Blue_part	 
	   {20,          0,       0  },  //Red 
	   {0,           0,       20 },  //Blue
	   {20,          4,       0  },  //Yellow
	   {0,           20,      0  },  //Green
	   {20,          0,       20 },  //Violet
       {20,          2,       0  },	 //Orange
	   {20,          20,      20 }   //White
 };
 
 

 static LedDescription_t leds[rgb_Amount] = {
/*     color           pwmRED  pwmGREEN  pwmBLUE    LounchBy            mode              state        period   startTime   currentPULS   maxPULS*/
  {  color_WHITE,       {0},     {0},      {0},    SWITCH_BY_HIGH,  rgb_mode_BLINK,    rgb_mode_OFF,    1000,       0,         {0},          {0} }// rgb_INDICATE
 };
 
 
 


 void drv_LedRGB_Init(rgbLeds_t name,
		 	 	 	  TIM_HandleTypeDef* timer_Red,   uint8_t channel_Red,
                      TIM_HandleTypeDef* timer_Green, uint8_t channel_Green,
					  TIM_HandleTypeDef* timer_Blue,  uint8_t channel_Blue,
                      rgbLed_SwitchMode_t turnOnMode){
						 
	 if(timer_Red != NULL && timer_Green!= NULL && timer_Blue != NULL && name < rgb_Amount){
		 	leds[name].red.timer      = timer_Red;
		 	leds[name].red.channel    = channel_Red;

		 	leds[name].green.timer    = timer_Green;
		 	leds[name].green.channel  = channel_Green;

		 	leds[name].blue.timer     = timer_Blue;
		 	leds[name].blue.channel   = channel_Blue;

		 	leds[name].startBlinkTime = drv_SysClock_GetCurrentTime();
		 	leds[name].turnOnMode     = turnOnMode;
	 }					 
}
 


void drv_LedRGB_SetMode(rgbLeds_t led, rgbLed_WorkMode_t newMode){

	if(led < rgb_Amount){
		leds[led].workMode = newMode;
		
		switch(leds[led].workMode){
			default:
			case rgb_mode_OFF:
				HAL_TIM_PWM_Stop(leds[led].red.timer,   leds[led].red.channel);
				HAL_TIM_PWM_Stop(leds[led].green.timer, leds[led].green.channel);
				HAL_TIM_PWM_Stop(leds[led].blue.timer,  leds[led].blue.channel);
				break;
			case rgb_mode_ON:
			case rgb_mode_BLINK_SLIDE:
			   LounchPWM(led);
				break;
		}		
	}	
}


//
rgbLed_WorkMode_t drv_LedRGB_GetMode(rgbLeds_t led){
	rgbLed_WorkMode_t result = rgb_mode_OFF;
	if(led < rgb_Amount){
		result = leds[led].workMode;
	}	
	return result;
}

//
void drv_LedRGB_SetBlinkPeriod(uint32_t period){
	if(period > 0){
		for(uint8_t led = 0; led < rgb_Amount; led++){
			 leds[led].blinkPeriod = period;
		}
	}
}

//
uint32_t drv_LedRGB_GetBlinkPeriod(void){
	uint32_t period = 0;

	if(rgb_Amount > 0){
		period = leds[0].blinkPeriod;
	}

	return period;
}

//
void  drv_LedRGB_SetColor(rgbLeds_t led, uint16_t  redPart, uint16_t greenPart, uint16_t bluePart){
	if(led < rgb_Amount){
				
		leds[led].current_PWM.red   = redPart;
		leds[led].current_PWM.green = greenPart;
		leds[led].current_PWM.blue  = bluePart;

		leds[led].max_PWM.red       = redPart;
		leds[led].max_PWM.green     = greenPart;
		leds[led].max_PWM.blue      = bluePart;

		Set_Puls(led, RED);
		Set_Puls(led, GREEN);
		Set_Puls(led, BLUE);
		
		LounchPWM(led);
	}
}

//
void  drv_LedRGB_SetDefinedColor(rgbLeds_t led, PossibleColors_t  color){
	if(color < color_Amount && led < rgb_Amount){
		leds[led].color = color;
		drv_LedRGB_SetColor(led, defineColors[color][0], defineColors[color][1], defineColors[color][2]);
	}
}



//
void drv_LedRGB_GetColor(rgbLeds_t led, uint16_t  *redPart, uint16_t *greenPart, uint16_t *bluePart){
	if(led < rgb_Amount){
		*redPart   = leds[led].current_PWM.red;
		*greenPart = leds[led].current_PWM.green;
		*bluePart  = leds[led].current_PWM.blue;
	}
}

PossibleColors_t  drv_LedRGB_GetDefinedColor(rgbLeds_t led){
	PossibleColors_t color = color_Amount;
	if(led < rgb_Amount){
		color = leds[led].color;
	}
	return color;
}

//
void drv_LedRGB_Run(void){
	
	for(uint8_t led = 0; led < rgb_Amount; led ++){
		switch(leds[led].workMode){
		case rgb_mode_BLINK:{
			if( drv_SysClock_IsTimeSpent(leds[led].startBlinkTime, leds[led].blinkPeriod) ){
				 if(leds[led].state == rgb_mode_ON){
						 leds[led].state = rgb_mode_OFF;
						 HAL_TIM_PWM_Stop(leds[led].red.timer,   leds[led].red.channel);
						 HAL_TIM_PWM_Stop(leds[led].green.timer, leds[led].green.channel);
					     HAL_TIM_PWM_Stop(leds[led].blue.timer,  leds[led].blue.channel);
					 }else{
						 leds[led].state = rgb_mode_ON;
						 LounchPWM((rgbLeds_t)led);
					 }
					 leds[led].startBlinkTime = drv_SysClock_GetCurrentTime();
			 }
		}break;
		case rgb_mode_BLINK_SLIDE:{
			if( drv_SysClock_IsTimeSpent(leds[led].startBlinkTime, leds[led].blinkPeriod) ){
				 if(leds[led].state == rgb_mode_OFF){
					 if( leds[led].current_PWM.red > 0 ){
						   leds[led].current_PWM.red -= STEP_INTENSITY;
					 }
					 if( leds[led].current_PWM.green > 0 ){
						   leds[led].current_PWM.green -= STEP_INTENSITY;
					 }
					 if( leds[led].current_PWM.blue > 0 ){
						 leds[led].current_PWM.blue -= STEP_INTENSITY;
					 }
					 if( leds[led].current_PWM.red == 0 && leds[led].current_PWM.green == 0 && leds[led].current_PWM.blue == 0){
						 leds[led].state = rgb_mode_ON;
					 }
				 }else{
					   if(leds[led].current_PWM.red < leds[led].max_PWM.red){
							 leds[led].current_PWM.red += STEP_INTENSITY;
						 }
						 if(leds[led].current_PWM.green < leds[led].max_PWM.green){
							 leds[led].current_PWM.green += STEP_INTENSITY;
						 }
						 if(leds[led].current_PWM.blue < leds[led].max_PWM.blue){
							 leds[led].current_PWM.blue += STEP_INTENSITY;
						 }
						 if( leds[led].current_PWM.red   == leds[led].max_PWM.red &&
								 leds[led].current_PWM.green == leds[led].max_PWM.green &&
								 leds[led].current_PWM.blue  == leds[led].max_PWM.blue){
								 leds[led].state = rgb_mode_OFF;
						 }
				 }
				 Set_Puls((rgbLeds_t)led, RED);
				 Set_Puls((rgbLeds_t)led, GREEN);
				 Set_Puls((rgbLeds_t)led, BLUE);

			     LounchPWM((rgbLeds_t)led);
			     leds[led].startBlinkTime = drv_SysClock_GetCurrentTime();
			}
		}break;
		default:
			continue;			
		}
   }
}


 
//===
void LounchPWM(rgbLeds_t led){

	if(leds[led].current_PWM.red){		
		HAL_TIM_PWM_Start(leds[led].red.timer,  leds[led].red.channel);
	}else{
		HAL_TIM_PWM_Stop(leds[led].red.timer,  leds[led].red.channel);
	}
	if(leds[led].current_PWM.green){
		HAL_TIM_PWM_Start(leds[led].green.timer,  leds[led].green.channel);
	}else{
		HAL_TIM_PWM_Stop(leds[led].green.timer,  leds[led].green.channel);
	}
	if(leds[led].current_PWM.blue){
		HAL_TIM_PWM_Start(leds[led].blue.timer,  leds[led].blue.channel);
	}else{
		HAL_TIM_PWM_Stop(leds[led].blue.timer,  leds[led].blue.channel);
	}
 }
 
void Set_Puls(rgbLeds_t led, led_colors_t selectColor){
	switch(selectColor){
		case RED:
			switch(leds[led].red.channel){
				case 0:
					leds[led].red.timer->Instance->CCR1 = leds[led].current_PWM.red;
				break;
				case 4:
					leds[led].red.timer->Instance->CCR2 = leds[led].current_PWM.red;
				break;
				case 8:
					leds[led].red.timer->Instance->CCR3 = leds[led].current_PWM.red;
				break;
				case 12:
					leds[led].red.timer->Instance->CCR4 = leds[led].current_PWM.red;
				break;
				default:
				break;
			}
			break;
		case GREEN:
			switch(leds[led].green.channel){
				case 0:
					leds[led].green.timer->Instance->CCR1 = leds[led].current_PWM.green;
				break;
				case 4:
					leds[led].green.timer->Instance->CCR2 = leds[led].current_PWM.green;
				break;
				case 8:
					leds[led].green.timer->Instance->CCR3 = leds[led].current_PWM.green;
				break;
				case 12:
					leds[led].green.timer->Instance->CCR4 = leds[led].current_PWM.green;
				break;
				default:
				break;
			}
			break;
		case BLUE:
			switch(leds[led].blue.channel){
				case 0:
					leds[led].blue.timer->Instance->CCR1 = leds[led].current_PWM.blue;
				break;
				case 4:
					leds[led].blue.timer->Instance->CCR2 = leds[led].current_PWM.blue;
				break;
				case 8:
					leds[led].blue.timer->Instance->CCR3 = leds[led].current_PWM.blue;
				break;
				case 12:
					leds[led].blue.timer->Instance->CCR4 = leds[led].current_PWM.blue;
				break;
				default:
				break;
			}
			break;
		default:
			break;
	}
}
