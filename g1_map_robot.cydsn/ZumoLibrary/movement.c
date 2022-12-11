#include <project.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

#include "Beep.h"
#include "FreeRTOS.h"
#include "Gyro.h"
#include "I2C_Common.h"
#include "LSM303D.h" //?
#include "Lidar.h"
#include "MQTTClient.h"
#include "Motor.h"
#include "Nunchuk.h" //?
#include "json_str.h"
#include "mqtt_sender.h"
#include "serial1.h"
#include "task.h"
#include "zumo_config.h"
#include "queue.h"

#include "movement.h"

#if START_MQTT == 1
QueueHandle_t received_settings_mqtt;
QueueHandle_t received_distance;

void
vMovementTask( void *pvParameters)
{
  int distance = 0;
  json_command command;
  command.direction = 0;
  command.duration = 0;
  command.speed = 0;
  command.forced_stop = true;
  print_mqtt("t_debug", "%.*s", 13, "MQTT Ready\r\n");
  print_mqtt("t_debug", "%.*s", 28, "Movement handler started.\r\n");
  print_mqtt("t_debug", "%.*s", 28, "Waiting for button press.\r\n");
  while (SW1_Read ());
  print_mqtt("t_debug", "%.*s", 17, "Button pressed\r\n");
  
  Lidar_start();

  while (1)
  {
    //Wait on Lidar queue for distance value.
    xQueueReceive(received_distance, &distance, pdMS_TO_TICKS(10));

    //Wait on mqtt queue for command.
    xQueueReceive(received_settings_mqtt, &command, 0);

    char buf[255];
    snprintf(buf, 255, "Distance: %d", distance);
    print_mqtt("t_debug", "%.*s", strlen(buf), buf);
    //Execute command received from mqtt.
    if(command.forced_stop)
    {
      if((distance <= 15 && command.direction == M_DIR_FORWARD) ||
       (distance <= 10 && (command.direction == M_DIR_LEFT || command.direction == M_DIR_RIGHT)))
      {
        motor_forward(0, 0);
        Beep(50, 25);
      }
      else
      {
        json_str_handle_cmd(&command);
      }
    }
    else
    {
      json_str_handle_cmd(&command);
    }
    //Block here to allow idle task to run in case if queue is always full.
    vTaskDelay(1);
  }
}
#else
vMovementTask( void *pvParameters)
{
  printf("And what do you expect me to do?\r\n");
  while (1)
  {
    //Place code to execute here. Robot shouldn't be used without mqtt
    
    //Block here to allow idle task to run.
    vTaskDelay(1);
  }
}
#endif
