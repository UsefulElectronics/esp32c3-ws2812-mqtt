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
 * @file    		:  main.c
 * @author  		:  Ward Almasarani
 * @version 		:  v.1.0
 * @date    		:  Dec 31, 2022
 * @brief   		:
 *
 ******************************************************************************/


/* INCLUDES ------------------------------------------------------------------*/
#include "main.h"
#include "mqtt.h"
#include "wifi_connect.h"
/* PRIVATE STRUCTRES ---------------------------------------------------------*/

/* VARIABLES -----------------------------------------------------------------*/
static const char *TAG = "example";

static uint8_t led_strip_pixels[EXAMPLE_LED_NUMBERS * 3];

static uint8_t colorMode = COLOR_SELECT;

/* DEFINITIONS ---------------------------------------------------------------*/

/* MACROS --------------------------------------------------------------------*/

/* PRIVATE FUNCTIONS DECLARATION ---------------------------------------------*/
static void mqtt_msg_pars_task		(void *param);

static void colorful_effect_task	(void *param);
/* FUNCTION PROTOTYPES -------------------------------------------------------*/
static void colorful_effect_task(void *param)
{

}

static void mqtt_msg_pars_task(void *param)
{

    uint32_t red 			= 0;
    uint32_t green 			= 0;
    uint32_t blue 			= 0;

    uint32_t hue 			= 0;
    uint32_t sat 			= 100;
    uint32_t bright			= 100;


    mqtt_buffer_t mqttSubscribeBuffer;

    ESP_LOGI(TAG, "Create RMT TX channel");
    rmt_channel_handle_t led_chan = NULL;
    rmt_tx_channel_config_t tx_chan_config =
    {
        .clk_src = RMT_CLK_SRC_DEFAULT, // select source clock
        .gpio_num = RMT_LED_STRIP_GPIO_NUM,
        .mem_block_symbols = 64, 		// increase the block size can make the LED less flickering
        .resolution_hz = RMT_LED_STRIP_RESOLUTION_HZ,
        .trans_queue_depth = 4, 		// set the number of transactions that can be pending in the background
    };
    ESP_ERROR_CHECK(rmt_new_tx_channel(&tx_chan_config, &led_chan));

    ESP_LOGI(TAG, "Install led strip encoder");
    rmt_encoder_handle_t led_encoder = NULL;
    led_strip_encoder_config_t encoder_config =
    {
        .resolution = RMT_LED_STRIP_RESOLUTION_HZ,
    };
    ESP_ERROR_CHECK(rmt_new_led_strip_encoder(&encoder_config, &led_encoder));

    ESP_LOGI(TAG, "Enable RMT TX channel");
    ESP_ERROR_CHECK(rmt_enable(led_chan));

    ESP_LOGI(TAG, "Start LED rainbow chase");
    rmt_transmit_config_t tx_config =
    {
        .loop_count = 0, // no transfer loop
    };

	    while (1)
	    {
	    	if(xQueueReceive(mqttSubscribe_queue, (void *)&mqttSubscribeBuffer, portMAX_DELAY))
	    	{
	    		 if(0 == memcmp(mqttSubscribeBuffer.topicString, MQTT_COLOR_TOPIC, 5))
	    		 {
	    			 sscanf(mqttSubscribeBuffer.data, "hsv(%d, %d%%, %d%%)", &hue, &sat, &bright);

	    			 printf("h: %d, s: %d, v: %d\n", hue, sat, bright);

	    			 colorMode = COLOR_SELECT;
	    		 }
	    		 else if(0 == memcmp(mqttSubscribeBuffer.topicString, MQTT_SWITCH_TOPIC, 5))
				 {
	    			 if(0 == memcmp(mqttSubscribeBuffer.data, FALSE, 3))
	    			 {
	    				 bright = 0;
	    			 }
	    			 else
	    			 {
	    				 bright = 100;
	    			 }

	    			 colorMode = COLOR_SELECT;
				 }
	    		 else if(0 == memcmp(mqttSubscribeBuffer.topicString, MQTT_MODE_TOPIC, 5))
				 {

	    			 if(0 == memcmp(mqttSubscribeBuffer.data, HUE_PLAY, 3))
	    			 {
	    				 colorMode = COLOR_HUE_PLAY;
	    			 }
	    			 else if (0 == memcmp(mqttSubscribeBuffer.data, SAT_PLAY, 3))
	    			 {
	    				 colorMode = COLOR_SAT_PLAY;
	    			 }
				 }
	    		 else if(0 == memcmp(mqttSubscribeBuffer.topicString, MQTT_FREQUENCY_TOPIC, 5))
				 {

				 }


//	  	    	hue ++ ;
	  	        for (int i = 0; i < 3; i++)
	  	        {
	  	            for (int j = i; j < EXAMPLE_LED_NUMBERS ; j += 3)
	  	            {
	  	                // Build RGB pixels
	  	//                hue = j * 360 / EXAMPLE_LED_NUMBERS + start_rgb;

	  	                led_strip_hsv2rgb(hue, sat, bright, &red, &green, &blue);
	  	                led_strip_pixels[j * 3 + 0] = green;
	  	                led_strip_pixels[j * 3 + 1] = red;
	  	                led_strip_pixels[j * 3 + 2] = blue;
	  	            }
	  	            // Flush RGB values to LEDs
	  	            ESP_ERROR_CHECK(rmt_transmit(led_chan, led_encoder, led_strip_pixels, sizeof(led_strip_pixels), &tx_config));

	  	        }
	    	}

	}

}




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
void led_strip_hsv2rgb(uint32_t h, uint32_t s, uint32_t v, uint32_t *r, uint32_t *g, uint32_t *b)
{
    h %= 360; // h -> [0,360]
    uint32_t rgb_max = v * 2.55f;
    uint32_t rgb_min = rgb_max * (100 - s) / 100.0f;

    uint32_t i = h / 60;
    uint32_t diff = h % 60;

    // RGB adjustment amount by hue
    uint32_t rgb_adj = (rgb_max - rgb_min) * diff / 60;

    switch (i) {
    case 0:
        *r = rgb_max;
        *g = rgb_min + rgb_adj;
        *b = rgb_min;
        break;
    case 1:
        *r = rgb_max - rgb_adj;
        *g = rgb_max;
        *b = rgb_min;
        break;
    case 2:
        *r = rgb_min;
        *g = rgb_max;
        *b = rgb_min + rgb_adj;
        break;
    case 3:
        *r = rgb_min;
        *g = rgb_max - rgb_adj;
        *b = rgb_max;
        break;
    case 4:
        *r = rgb_min + rgb_adj;
        *g = rgb_min;
        *b = rgb_max;
        break;
    default:
        *r = rgb_max;
        *g = rgb_min;
        *b = rgb_max - rgb_adj;
        break;
    }
}

void app_main(void)
{


	wifi_connect();

	mqtt_app_start();

	xTaskCreatePinnedToCore(mqtt_msg_pars_task, "pars MQTT Message", 10000, NULL, 4, NULL, 1);

	xTaskCreatePinnedToCore(colorful_effect_task, "colorful led effect", 10000, NULL, 4, NULL, 1);
}

/**************************  Useful Electronics  ****************END OF FILE***/
