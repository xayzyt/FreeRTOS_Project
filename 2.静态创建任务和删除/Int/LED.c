#include "LED.h"

/**
 * @description: ����LED
 * @param {LED} led
 */
void LED_Turn_On(LED led)
{
    HAL_GPIO_WritePin(GPIOA, led, GPIO_PIN_RESET);
}

/**
 * @description: Ϩ��LED
 * @param {LED} led
 */
void LED_Turn_Off(LED led)
{
    HAL_GPIO_WritePin(GPIOA, led, GPIO_PIN_SET);
}

/**
 * @description: ��תLED��״̬
 * @param {LED} led
 **/
void LED_Toggle(LED led)
{
    HAL_GPIO_TogglePin(GPIOA, led);
}

/**
 * @description: �ر�����LED
 * @param {LED} led
 * @param {uint8_t} len
 * @return {*}
 **/
void LED_Turn_Off_All(LED led[], uint8_t len)
{
    uint8_t i;
    for (i = 0; i < len; i++)
    {
        LED_Turn_Off(led[i]);
    }
}
