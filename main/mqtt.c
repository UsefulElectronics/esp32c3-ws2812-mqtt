/**
  ******************************************************************************
  *		(c) Copyright 2012-2023; USEFUL ELECTRONICS
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
 * @file    		:  mqtt.c
 * @author  		:  Ward Almasarani
 * @version 		:  v.1.0
 * @date    		:  Jan 2, 2023
 * @brief   		:
 *
 ******************************************************************************/


/* INCLUDES ------------------------------------------------------------------*/
#include "mqtt.h"

/* PRIVATE STRUCTRES ---------------------------------------------------------*/

/* VARIABLES -----------------------------------------------------------------*/
static const char *TAG = "MQTT";

/* DEFINITIONS ---------------------------------------------------------------*/

/* MACROS --------------------------------------------------------------------*/

/* PRIVATE FUNCTIONS DECLARATION ---------------------------------------------*/
static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data);
/* FUNCTION PROTOTYPES -------------------------------------------------------*/
/**
 * @brief Event handler registered to receive MQTT events
 *
 *  This function is called by the MQTT client event loop.
 *
 * @param handler_args user data registered to the event.
 *
 * @param base Event base for the handler(always MQTT Base in this example).
 *
 * @param event_id The id for the received event.
 *
 * @param event_data The data for the event, esp_mqtt_event_handle_t.
 */
static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    ESP_LOGD(TAG, "Event dispatched from event loop base=%s, event_id=%d", base, event_id);
    esp_mqtt_event_handle_t event = event_data;
    esp_mqtt_client_handle_t client = event->client;
    uint32_t msg = 0;
    switch ((esp_mqtt_event_id_t)event_id)
    {
    case MQTT_EVENT_CONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
//        msg_id = esp_mqtt_client_publish(client, "/topic/qos1", "data_3", 0, 1, 0);
//        ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);

        msg = esp_mqtt_client_subscribe(client, "/topic/switch", 0);
        ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg);

//        msg_id = esp_mqtt_client_subscribe(client, "/topic/qos1", 1);
//        ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);
//
//        msg_id = esp_mqtt_client_unsubscribe(client, "/topic/qos1");
//        ESP_LOGI(TAG, "sent unsubscribe successful, msg_id=%d", msg_id);
        break;
    case MQTT_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
        break;

    case MQTT_EVENT_SUBSCRIBED:
        ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
//        msg_id = esp_mqtt_client_publish(client, "/topic/qos0", "data", 0, 0, 0);
        ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg);
        break;
    case MQTT_EVENT_UNSUBSCRIBED:
        ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_PUBLISHED:
        ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_DATA:
        ESP_LOGI(TAG, "MQTT_EVENT_DATA");
        printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
        printf("DATA=%.*s\r\n", event->data_len, event->data);
        break;
    case MQTT_EVENT_ERROR:
        ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
//        if (event->error_handle->error_type == MQTT_ERROR_TYPE_TCP_TRANSPORT)
//        {
//            log_error_if_nonzero("reported from esp-tls", event->error_handle->esp_tls_last_esp_err);
//            log_error_if_nonzero("reported from tls stack", event->error_handle->esp_tls_stack_err);
//            log_error_if_nonzero("captured as transport's socket errno",  event->error_handle->esp_transport_sock_errno);
//            ESP_LOGI(TAG, "Last errno string (%s)", strerror(event->error_handle->esp_transport_sock_errno));
//
//        }
        break;
    default:
        ESP_LOGI(TAG, "Other event id:%d", event->event_id);
        break;
    }
}

void mqtt_app_start(void)
{
    esp_mqtt_client_config_t mqtt_cfg =
    {
        .broker.address.uri = "192.168.1.110:1880",
    };
//#if CONFIG_BROKER_URL_FROM_STDIN
//    char line[128];
//
//    if (strcmp(mqtt_cfg.broker.address.uri, "FROM_STDIN") == 0) {
//        int count = 0;
//        printf("Please enter url of mqtt broker\n");
//        while (count < 128) {
//            int c = fgetc(stdin);
//            if (c == '\n') {
//                line[count] = '\0';
//                break;
//            } else if (c > 0 && c < 127) {
//                line[count] = c;
//                ++count;
//            }
//            vTaskDelay(10 / portTICK_PERIOD_MS);
//        }
//        mqtt_cfg.broker.address.uri = line;
//        printf("Broker url: %s\n", line);
//    } else {
//        ESP_LOGE(TAG, "Configuration mismatch: wrong broker url");
//        abort();
//    }
//#endif /* CONFIG_BROKER_URL_FROM_STDIN */

    esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_cfg);
    /* The last argument may be used to pass data to the event handler, in this example mqtt_event_handler */
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    esp_mqtt_client_start(client);
}

/**************************  Useful Electronics  ****************END OF FILE***/
