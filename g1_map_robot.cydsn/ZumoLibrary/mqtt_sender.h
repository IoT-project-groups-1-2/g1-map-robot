#ifndef MQTT_SENDER_H_
#define MQTT_SENDER_H_

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

void send_mqtt (const char *topic, const char *message);
void MQTTSendTask (void *pvParameters);
void MQTTSendTaskInit (void);
int print_mqtt (const char *topic, const char *fmt, ...);

//Public queue
QueueHandle_t received_settings_mqtt;

#endif