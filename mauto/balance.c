#include "balance.h"

static inline int16_t
convert_raw (uint8_t high, uint8_t low)
{
  return (int16_t)(high << 8) | low;
}

/**
 * @brief Get a rounded velocity of gyro's z_plane
 *
 * @return int16_t (negative = right, positive = left, 0 = centered)
 */
int16_t
z_plane_get_current ()
{
  uint8 z_plane_raw_low, z_plane_raw_high;
  int16_t buffer_list = 0;

  I2C_Start ();
  I2C_Write (GYRO_ADDR, GYRO_CTRL1_REG, GYRO_MODE_ACTIVE);
  I2C_Write (GYRO_ADDR, GYRO_CTRL4_REG, GYRO_FULL_SCALE_SELECTION);

  for (size_t i = 0; i <= Z_PLANE_DIVISION_COEFFICIENT; i++)
    {
      z_plane_raw_low = I2C_Read (GYRO_ADDR, OUT_Z_AXIS_L);
      z_plane_raw_high = I2C_Read (GYRO_ADDR, OUT_Z_AXIS_H);
      buffer_list += convert_raw (z_plane_raw_high, z_plane_raw_low);
      //vTaskDelay (5);
    }
  return buffer_list / (Z_PLANE_D_C_SQUARED);
}

/**
 * @brief Get raw velocity of gyro's z_plane
 *
 * @return int16_t (negative = right, positive = left, 0 = centered)
 */
int16_t
z_plane_get_current_raw ()
{
  uint8 z_plane_raw_low, z_plane_raw_high;
  int16_t z_plane_raw = 0;

  I2C_Start ();
  I2C_Write (GYRO_ADDR, GYRO_CTRL1_REG, GYRO_MODE_ACTIVE);
  I2C_Write (GYRO_ADDR, GYRO_CTRL4_REG, GYRO_FULL_SCALE_SELECTION);

  z_plane_raw_low = I2C_Read (GYRO_ADDR, OUT_Z_AXIS_L);
  z_plane_raw_high = I2C_Read (GYRO_ADDR, OUT_Z_AXIS_H);
  z_plane_raw = convert_raw (z_plane_raw_high, z_plane_raw_low);

  return z_plane_raw;
}

static uint8_t
pid (uint8_t motor_speed, int z_plane_velocity, int *integral)
{
  float kP = 0.6, kI = 0.125;
  int error = 0;
  error = z_plane_velocity;
  *integral += error;
  int corrected_speed
      = (motor_speed + ((kP * error) + (kI * (*integral))) * 1.2);

  /* Check for uint8_t boundaries */
  if (corrected_speed >= 255)
    {
      return (uint8_t)255;
    }
  return (uint8_t)corrected_speed;
}

uint8_t
predict_motor_direction (int z_plane_velocity, uint8_t current_speed,
                         int *integral)
{
  uint8_t corrected_speed = pid (current_speed, z_plane_velocity, integral);

  if (z_plane_velocity IS_LEFT)
    {
      SetMotors (0, 0, corrected_speed, current_speed, PREDICTION_DURATION);
    }
  else if (z_plane_velocity IS_RIGHT)
    {
      SetMotors (0, 0, current_speed, corrected_speed, PREDICTION_DURATION);
    }
  else
    {
      SetMotors (0, 0, current_speed, current_speed, PREDICTION_DURATION);
    }

  return 0;
}

//Pass angle_sum
uint8_t
try_to_correct (int ang_sum, uint8_t *cvl, uint8_t *cvr)
{
  if (ang_sum IS_LEFT)
  {
    if (*cvr > 90) *cvr -= 1;
    else if (*cvl < 180) *cvl += 1;
  }
  else if (ang_sum IS_RIGHT)
  {
    ang_sum *= -1;
    if (*cvl > 90) *cvl -= 1;
    else if (*cvr < 180) *cvr += 1;
  }
  SetMotors (0, 0, *cvl, *cvr, 0);
  return 0;
}

//Pass angle_sum
uint8_t
fix_heading (int *ang_sum, size_t loop_duration)
{
  int ang_for_speed = 0;
  int speed = 0;
  int tick = 0;
  int z_plane = 0;
  int i = 0;
  while (*ang_sum != 0 && i < loop_duration )
  {
    i++;
    //Remember time
    tick = xTaskGetTickCount();
    z_plane = z_plane_get_current();
    //Get time
    tick = xTaskGetTickCount() - tick;
    //Get angle
    *ang_sum += z_plane;

    //Constrict ang_sum to 90 in order to determine speed
    if(*ang_sum < -90 || *ang_sum > 90)
      ang_for_speed = 90;
    else if(*ang_sum < 0)
      ang_for_speed = *ang_sum * -1;
    else
      ang_for_speed = *ang_sum;

    speed = (ang_for_speed < 30) ? 50 : ang_for_speed * 2;
    //IS_RIGHT?
    if(*ang_sum IS_LEFT)
    {
      motor_tank_turn_right(speed, 33);
    }
    //IS_LEFT?
    else if(*ang_sum IS_RIGHT)
    {
      motor_tank_turn_left(speed, 55);
    }
  }
  motor_forward(0, 0);
  return 0;
}