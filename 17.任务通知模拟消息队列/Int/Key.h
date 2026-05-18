#ifndef __KEY_H
#define __KEY_H

#include "main.h"

#define KEY1        HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_8)   /* ¶ÁÈ¡KEY1Òý½Å×´Ì¬£¨SW3£© */
#define KEY2        HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_9)  /* ¶ÁÈ¡KEY2Òý½Å×´Ì¬£¨SW4£© */
#define KEY3        HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_10)  /* ¶ÁÈ¡KEY3Òý½Å×´Ì¬£¨SW5£© */
#define KEY4        HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_11)   /* ¶ÁÈ¡KEY4Òý½Å×´Ì¬£¨SW6£© */


#define KEY1_PRESS    1              
#define KEY2_PRESS    2              
#define KEY3_PRESS    3              
#define KEY4_PRESS    4              


uint8_t Key_Detect(void);


#endif
