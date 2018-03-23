/*
 * Name         drv_LedRGB.c
 * Author       Maksim Golikov (SW developer)
 * Created      19 MARCH 2018
 * Description  driver for control RGB led (source file)
 */
 
 #include "DRV/drv_LedRGB.h"
 #include "DRV/drv_PWM.h"
 #include "DRV/drv_SysClock.h"
 
 #define STEP_INTENSITY    1
 
 typedef struct{
   uint8_t            numberPWM_Red;
	 uint8_t            numberPWM_Green;
	 uint8_t            numberPWM_Blue;
	 
	 rgbLed_WorkMode_t  workMode;
	 rgbLed_WorkMode_t  state;
	 uint32_t           blinkPeriod;
	 uint32_t           startBlinkTime;
	 
	 uint16_t            red;
	 uint16_t            green;
	 uint16_t            blue;
	 uint16_t            redSlide;
	 uint16_t            greenSlide;
	 uint16_t            blueSlide;
 }LedDescription_t;
 
 
 
 static uint16_t defineColors [color_Amount][3] = {
	 // Red  Green    Blue
	   {100,  0,      100},   //Red
	   {100,  100,    0},   //Blue		 
		 {100,  100,    100}, //
		 {0,    100,    100}, //
     {0,    0,      0},		//
		 {100,  100,    100}  //White
 };
 
 
 
 
 
 
 
 
 static uint8_t lastLedNumber = 0;
 
 static LedDescription_t rgbLeds[rgb_Amount] = {
//  numPWM_red  numPWM_green  numPWM_blue     mode          state     period   startTime    red   green   blue  Rslide  Gslide  Bslide
	    { 0,           0,           0,       mode_BLINK,    mode_OFF,    1000,       0,       255,   255,   255,    0,      0,      0}// rgb_INDICATE
 };
 
 
 
 void drv_LedRGB_Init(TIM_HandleTypeDef* timer_Red, uint8_t channel_Red,
                      TIM_HandleTypeDef* timer_Green, uint8_t channel_Green,
					            TIM_HandleTypeDef* timer_Blue,  uint8_t channel_Blue){
						 
	 if(timer_Red != NULL && timer_Green!= NULL && timer_Blue != NULL &&
		  channel_Red > 0   &&  channel_Red < 5 &&
	    channel_Green > 0 &&  channel_Green < 5 &&
	    channel_Blue > 0  &&  channel_Blue < 5 &&
	    lastLedNumber < rgb_Amount){
		 				
			rgbLeds[lastLedNumber].numberPWM_Red =   (lastLedNumber * 3) + 0;
			rgbLeds[lastLedNumber].numberPWM_Green = (lastLedNumber * 3) + 1;
			rgbLeds[lastLedNumber].numberPWM_Blue =  (lastLedNumber * 3) + 2;
			rgbLeds[lastLedNumber].startBlinkTime = drv_SysClock_GetCurrentTime();
				
      lastLedNumber += 1;

      drv_PWM_Init(timer_Red,   channel_Red);
      drv_PWM_Init(timer_Green, channel_Green);
      drv_PWM_Init(timer_Blue,  channel_Blue);				
	 }					 
}
 

void drv_LedRGB_SetMode(rgbLeds_t led, rgbLed_WorkMode_t newMode){
	if(led < rgb_Amount){
		rgbLeds[led].workMode = newMode;
		
		switch(rgbLeds[led].workMode){
			default:
			case mode_OFF:
				drv_PWM_Stop(rgbLeds[led].numberPWM_Red);
		  	drv_PWM_Stop(rgbLeds[led].numberPWM_Green);
			  drv_PWM_Stop(rgbLeds[led].numberPWM_Blue);
				break;
			case mode_ON:						
			case mode_BLINK_SLIDE:
				drv_PWM_Start(rgbLeds[led].numberPWM_Red);
				drv_PWM_Start(rgbLeds[led].numberPWM_Green);
				drv_PWM_Start(rgbLeds[led].numberPWM_Blue);
				break;
		}		
	}	
}

rgbLed_WorkMode_t drv_LedRGB_GetMode(rgbLeds_t led){
	rgbLed_WorkMode_t result = mode_OFF;	
	if(led < rgb_Amount){
		result = rgbLeds[led].workMode;
	}	
	return result;
}

void drv_LedRGB_SetBlinkPeriod(uint32_t period){
	if(period > 0){
		for(uint8_t led = 0; led < rgb_Amount; led++){
			 rgbLeds[led].blinkPeriod = period;
		}
	}
}

uint32_t drv_LedRGB_GetBlinkPeriod(void){
	return rgbLeds[0].blinkPeriod;
}

void  drv_LedRGB_SetColor(rgbLeds_t led, uint16_t  redPart, uint16_t greenPart, uint16_t bluePart){
	if(led < rgb_Amount){
		rgbLeds[led].red   = redPart;
		rgbLeds[led].green = greenPart;
		rgbLeds[led].blue  = bluePart;
		rgbLeds[led].redSlide   = redPart;
		rgbLeds[led].greenSlide = greenPart;
		rgbLeds[led].blueSlide  = bluePart;
		
		
		
		drv_PWM_SetPuls(rgbLeds[led].numberPWM_Red, rgbLeds[led].red);
		drv_PWM_SetPuls(rgbLeds[led].numberPWM_Green, rgbLeds[led].green);
		drv_PWM_SetPuls(rgbLeds[led].numberPWM_Blue, rgbLeds[led].blue);
	}
}

void  drv_LedRGB_SetDefineColor(rgbLeds_t led, uint8_t  color){
	if(color < color_Amount && led < rgb_Amount){
		drv_LedRGB_SetColor(led, defineColors[color][0], defineColors[color][1], defineColors[color][2]);
	}
}



void drv_LedRGB_GetColor(rgbLeds_t led, uint16_t  *redPart, uint16_t *greenPart, uint16_t *bluePart){
	if(led < rgb_Amount){
		*redPart   = rgbLeds[led].red;
		*greenPart = rgbLeds[led].green;
		*bluePart  = rgbLeds[led].blue;
	}
}



void drv_LedRGB_Run(void){
	
	for(uint8_t led = 0; led < rgb_Amount; led ++){
		
		if(rgbLeds[led].workMode == mode_BLINK){
					if( drv_SysClock_IsTimeSpent(rgbLeds[led].startBlinkTime, rgbLeds[led].blinkPeriod) ){
					 if(rgbLeds[led].state == mode_ON){
						 rgbLeds[led].state = mode_OFF;
						 drv_PWM_Stop(rgbLeds[led].numberPWM_Red);
						 drv_PWM_Stop(rgbLeds[led].numberPWM_Green);
						 drv_PWM_Stop(rgbLeds[led].numberPWM_Blue);
					 }else{
						 rgbLeds[led].state = mode_ON;
						 drv_PWM_Start(rgbLeds[led].numberPWM_Red);
						 drv_PWM_Start(rgbLeds[led].numberPWM_Green);
						 drv_PWM_Start(rgbLeds[led].numberPWM_Blue);
					 }
					 rgbLeds[led].startBlinkTime = drv_SysClock_GetCurrentTime();
				 }
		}			
		if(rgbLeds[led].workMode == mode_BLINK_SLIDE){
			
			if( drv_SysClock_IsTimeSpent(rgbLeds[led].startBlinkTime, rgbLeds[led].blinkPeriod) ){
					 if(rgbLeds[led].state == mode_OFF){
						 if( rgbLeds[led].red > 0 ){
							   rgbLeds[led].red -= STEP_INTENSITY;
						 }
						 if( rgbLeds[led].green > 0 ){
							   rgbLeds[led].green -= STEP_INTENSITY;
						 }
						 if( rgbLeds[led].blue > 0 ){
							   rgbLeds[led].blue -= STEP_INTENSITY;
						 }
						 
						 
						 if( rgbLeds[led].red == 0 && rgbLeds[led].green == 0 && rgbLeds[led].blue == 0){
							 rgbLeds[led].state = mode_ON;
						 }						 
					 }else{
						 if(rgbLeds[led].red < rgbLeds[led].redSlide){
							 rgbLeds[led].red += STEP_INTENSITY;
						 }
						 if(rgbLeds[led].green < rgbLeds[led].greenSlide){
							 rgbLeds[led].green += STEP_INTENSITY;
						 }
						 if(rgbLeds[led].blue < rgbLeds[led].blueSlide){
							 rgbLeds[led].blue += STEP_INTENSITY;
						 }
						 
						 if( rgbLeds[led].red   == rgbLeds[led].redSlide &&
							   rgbLeds[led].green == rgbLeds[led].greenSlide &&
						     rgbLeds[led].blue  == rgbLeds[led].blueSlide){
									 rgbLeds[led].state = mode_OFF;
						 }
					 }
					 drv_PWM_SetPuls(rgbLeds[led].numberPWM_Red, rgbLeds[led].red);
		       drv_PWM_SetPuls(rgbLeds[led].numberPWM_Green, rgbLeds[led].green);
		       drv_PWM_SetPuls(rgbLeds[led].numberPWM_Blue, rgbLeds[led].blue);
					 
					 rgbLeds[led].startBlinkTime = drv_SysClock_GetCurrentTime();
			}
		}		 
	}	
	
	drv_PWM_Run();
}


