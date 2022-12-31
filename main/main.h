/**
  ******************************************************************************
  *		(c) Copyright 2012-2022; USEFUL ELECTRONICS
  *
  *	All rights reserved.  Protected by international copyright laws.
  *	Knowledge of the source code may not be used to write a similar
  *	product.  This file may only be used in accordance with a license
  *	and should not be redistributed in any way.
  *
  *****************************************************************************/

/**
 ******************************************************************************
 * @Channel Link    :  https://www.youtube.com/user/wardzx1
 * @file    		:  main.h
 * @author  		:  Ward Almasarani
 * @version 		:  v.1.0
 * @date    		:  Dec 31, 2022
 * @brief   		:
 *
 ******************************************************************************/

#ifndef MAIN_H_
#define MAIN_H_


/* INCLUDES ------------------------------------------------------------------*/
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/rmt_tx.h"
#include "led_strip_encoder.h"

/* PRIVATE STRUCTRES ---------------------------------------------------------*/

/* VARIABLES -----------------------------------------------------------------*/

/* DEFINITIONS ---------------------------------------------------------------*/

/* MACROS --------------------------------------------------------------------*/
#define RMT_LED_STRIP_RESOLUTION_HZ 10000000 // 10MHz resolution, 1 tick = 0.1us (led strip needs a high resolution)
#define RMT_LED_STRIP_GPIO_NUM      2

#define EXAMPLE_LED_NUMBERS         69
#define EXAMPLE_CHASE_SPEED_MS      100

/* PRIVATE FUNCTIONS DECLARATION ---------------------------------------------*/

/* FUNCTION PROTOTYPES -------------------------------------------------------*/
/**
 * @brief 	Convert Hue, saturation and brightness values to 8 bit RGB value
 *
 * @param 	h	:	Used hue value which is the color angle between 0 and 360 that is used for conversion .
 *
 * @param 	s	:	Used saturation to determine how colorful the color is. this parameter is used for conversion.
 *
 * @param 	v	:	Used color brightness value to determine how bright the LED is. this parameter is used for conversion.
 *
 * @param 	r	:	Red color 8 bit value. Target pointer is passed to this function.
 *
 * @param 	g	:	Green color 8 bit value. Target pointer is passed to this function.
 *
 * @param 	b	:	Blue color 8 bit value. Target pointer is passed to this function.
 */
void led_strip_hsv2rgb(uint32_t h, uint32_t s, uint32_t v, uint32_t *r, uint32_t *g, uint32_t *b);

#endif /* MAIN_MAIN_H_ */


/**************************  Useful Electronics  ****************END OF FILE***/
