/**
 * @file    Motor.c
 * @brief   Basic methods for operating motor sensor. For more details, please
 * refer to Motor.h file.
 * @details included in Zumo shield
 */
#include <stdio.h>

#include "Motor.h"
#include "FreeRTOS.h"
#include "task.h"
#include "zumo_config.h"
#include "mqtt_sender.h"

#if ZUMO_SIMULATOR == 0

void
SetMotors (uint8 left_dir, uint8 right_dir, uint8 left_speed,
           uint8 right_speed, uint32 delay)
{
  MotorDirLeft_Write (left_dir);   // 0 = forward, 1 = backward
  MotorDirRight_Write (right_dir); // 0 = forward, 1 = backward
  PWM_WriteCompare1 (left_speed);
  PWM_WriteCompare2 (right_speed);
  vTaskDelay (delay);
}
/**
 * @brief    Starting motor sensors
 * @details
 */
void
motor_start ()
{
  PWM_Start ();
}

/**
 * @brief    Stopping motor sensors
 * @details
 */
void
motor_stop ()
{
  PWM_Stop ();
}

#endif

/**
 * @brief    Moving motors forward
 * @details  giveing same speed to each side of PWM to make motors go forward
 * @param    uint8 speed : speed value
 * @param    uint32 delay : delay time
 */
void
motor_forward (uint8 speed, uint32 delay)
{
  // set LeftMotor forward mode
  // set RightMotor forward mode
  SetMotors (0, 0, speed, speed, delay);
}

/**
 * @brief Moving motors forward for amount of seconds
 * @details This function calls for balancing algorithm internally
 * @param speed : speed value
 * @param seconds : time in seconds
 */
int
motor_forward_for_s (uint8_t speed, size_t seconds, int *angle_sum)
{
  size_t global_delay_ms = seconds * 1000;
  size_t loop_duration = global_delay_ms / PREDICTION_DURATION;
  //int error_integral = 0;
  int16_t z_plane = 0;
  uint8_t cvl = speed;
  uint8_t cvr = speed;
  char status_message[400] = { 0 };
  int message_len = 0;
  int tick = 0;
  for (size_t i = 0; i < loop_duration; i++)
  {
    //Remember time
    tick = xTaskGetTickCount();
    z_plane = z_plane_get_current();
    //Exit point in case we hit something.
    if(z_plane > 40)
    {
      motor_forward(0, 0);
      *angle_sum = 0;
      return 1;
    }
    //Get time
    tick = xTaskGetTickCount() - tick;
    //Get angle
    *angle_sum += z_plane * 100 / tick;

    //Pass angle.
    try_to_correct(*angle_sum, &cvl, &cvr);
    //predict_motor_direction (z_plane, speed, &error_integral);
  }
  motor_forward(0, 0);
//DEBUG
  message_len = snprintf (status_message, 400, "{Stopped with: %d ang}", *angle_sum);
  print_mqtt ("t_status", "%.*s", message_len, status_message);
///DEBUG
  //Take over from here in order to fix heading.
  fix_heading(angle_sum, loop_duration);
//DEBUG
  message_len = snprintf (status_message, 400, "{Finished with: %d ang}", *angle_sum);
  print_mqtt ("t_status", "%.*s", message_len, status_message);
///DEBUG
  return 0;
}

/**
 * @brief    Moving motors to make a turn
 * @details  moving left when l_speed < r_speed or moving right when l_speed >
 * r_speed
 * @param    uint8 l_speed : left motor speed value
 * @param    uint8 r_speed : right motor speed value
 * @param    uint32 delay : delay time
 */
void
motor_turn (uint8 l_speed, uint8 r_speed, uint32 delay)
{
  SetMotors (0, 0, l_speed, r_speed, delay);
}

/**
 * @brief    Moving motors to make a left tank turn.
 * @details  Sets backwards mode to left motor.
 * @param    uint8 speed : speed value
 * @param    uint32 delay : delay time
 */
void
motor_tank_turn_left (uint8 speed, uint32 delay)
{
  SetMotors (1, 0, speed, speed, delay);
}

/**
 * @brief    Moving motors to make a right tank turn.
 * @details  Sets backwards mode to right motor.
 * @param    uint8 speed : speed value
 * @param    uint32 delay : delay time
 */
void
motor_tank_turn_right (uint8 speed, uint32 delay)
{
  SetMotors (0, 1, speed, speed, delay);
}

/**
 * @brief    Moving motors backward
 * @details  setting backward mode to each motors and gives same speed to each
 * side of PWM
 * @param    uint8 speed : speed value
 * @param    uint32 delay : delay time
 */
void
motor_backward (uint8 speed, uint32 delay)
{
  // set LeftMotor backward mode
  // set RightMotor backward mode
  SetMotors (1, 1, speed, speed, delay);
}
