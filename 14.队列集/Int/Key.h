#ifndef __KEY_H
#define __KEY_H

#include "main.h"

#define KEY1        HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_8)   /* ???KEY1????????SW3?? */
#define KEY2        HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_9)  /* ???KEY2????????SW4?? */
#define KEY3        HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_10)  /* ???KEY3????????SW5?? */
#define KEY4        HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_11)   /* ???KEY4????????SW6?? */


#define KEY1_PRESS    1              
#define KEY2_PRESS    2              
#define KEY3_PRESS    3              
#define KEY4_PRESS    4              


uint8_t Key_Detect(void);


#endif
