/**
* @mainpage ZumoBot Project
* @brief    You can make your own ZumoBot with various sensors.
* @details  <br><br>
    <p>
    <B>General</B><br>
    You will use Pololu Zumo Shields for your robot project with
CY8CKIT-059(PSoC 5LP) from Cypress semiconductor.This library has basic methods
of various sensors and communications so that you can make what you want with
them. <br> <br><br>
    </p>
    <p>
    <B>Sensors</B><br>
    &nbsp;Included: <br>
        &nbsp;&nbsp;&nbsp;&nbsp;LSM303D: Accelerometer & Magnetometer<br>
        &nbsp;&nbsp;&nbsp;&nbsp;L3GD20H: Gyroscope<br>
        &nbsp;&nbsp;&nbsp;&nbsp;Reflectance sensor<br>
        &nbsp;&nbsp;&nbsp;&nbsp;Motors
    &nbsp;Wii nunchuck<br>
    &nbsp;TSOP-2236: IR Receiver<br>
    &nbsp;HC-SR04: Ultrasonic sensor<br>
    &nbsp;APDS-9301: Ambient light sensor<br>
    &nbsp;IR LED <br><br><br>
    </p>
    <p>
    <B>Communication</B><br>
    I2C, UART, Serial<br>
    </p>
*/

#include "Accel_magnet.h"
#include "Beep.h"
#include "FreeRTOS.h"
#include "Gyro.h"
#include "IR.h"
#include "I2C_Common.h"
#include "LSM303D.h"
#include "MQTTClient.h"
#include "Motor.h"
#include "Nunchuk.h"
#include "Reflectance.h"
#include "Ultra.h"
#include "mqtt_sender.h"
#include "serial1.h"
#include "task.h"
#include <project.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include "json_str.h"
#include "zumo_config.h"
#include "Lidar.h"

#define CONST_COEF 20

/**
 * 
 * @file    main.c
 * @brief
 * @details  ** Enable global interrupt since Zumo library uses interrupts.
 * **<br>&nbsp;&nbsp;&nbsp;CyGlobalIntEnable;<br>
 */
#if 0
void
zmain (json_command* cmd)
{
  int ctr = 0;

  printf ("\nBoot\n");
  Lidar_start();
  send_mqtt ("Zumo01/debug", "Boot");

  while (SW1_Read() == 1); //Don't start without a button press.

  // BatteryLed_Write(1); // Switch led on
  BatteryLed_Write (0); // Switch led off
      char status_message[400] = { 0 };
      const int message_len
          = snprintf (status_message, 400, "{\"status\":%d}", 1);
  print_mqtt ("t_status", "%.*s", message_len, status_message);
  while (true)
    {
      //printf ("Ctr: %d, Button: %d\n", ctr, SW1_Read ());
      #if TEST == 1
        cmd->mode = true;
        cmd->duration = 10;
      #endif
      json_str_handle_cmd(cmd);

     if(cmd){
      vTaskDelay (cmd->duration);
      } else{
      vTaskDelay(500);
    }
      ctr++;
    }
}
#endif

#if LIDAR
void
zmain (json_command* cmd)
{
  printf ("\nBoot\n");
  int distance = 0;
  uint64_t ticks = 0;
  uint64_t res = 0;
  Lidar_start();
  while(true)
  {
    ticks++;
    distance = Lidar_get_distance();
    if(distance != -1)
    {
      res++;
      printf("(%llu:%llu) Distance = %d\r\n", ticks, res, distance);
    }
    //if(distance != -1) printf("Distance = %d\r\n", distance);
    vTaskDelay(1);
  }
}
#endif

int16_t
convert_raw(uint8_t high, uint8_t low)
{
  return (int16_t) (high << 8) | low;
}

//gyroscope//
void zmain()
{
  CyGlobalIntEnable; 
  UART_1_Start();

  I2C_Start();

  uint8 Z_L_G, Z_H_G;
  int16 Z_AXIS_G = 0;
  
  I2C_Write(GYRO_ADDR, GYRO_CTRL1_REG, 0x0F);             // set gyroscope into active mode
  I2C_Write(GYRO_ADDR, GYRO_CTRL4_REG, 0x30);             // set full scale selection to 2000dps

  while(1) {
    int temp = 0;
    Z_AXIS_G = 0;
    for(int i = 0; i < (CONST_COEF + 1); i++)
    {
      Z_L_G = I2C_Read(GYRO_ADDR, OUT_Z_AXIS_L);
      Z_H_G = I2C_Read(GYRO_ADDR, OUT_Z_AXIS_H);
      temp += convert_raw(Z_H_G, Z_L_G);
      vTaskDelay(5);
    }
    Z_AXIS_G = temp / (CONST_COEF * CONST_COEF);
    printf("%d \r\n", Z_AXIS_G);
    vTaskDelay(5);
  }
}

/* [] END OF FILE */