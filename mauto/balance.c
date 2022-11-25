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
      vTaskDelay (5);
    }
  return buffer_list / (Z_PLANE_D_C_SQUARED);
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

uint8_t
try_to_correct (int zv_sum, uint8_t *cvl, uint8_t *cvr)
{
  if (zv_sum IS_LEFT)
  {
    if(zv_sum > 100) zv_sum = 100;
    if (*cvr > 0) *cvr -= (zv_sum / 4) ? (zv_sum / 4) : 1;
    else if (*cvl < 256) *cvl += (zv_sum / 4) ? (zv_sum / 4) : 1;

    if (*cvr < 0) *cvr = 0;
    if (*cvl > 255) *cvl = 255;
  }
  else if (zv_sum IS_RIGHT)
  {
    zv_sum *= -1;
    if(zv_sum > 100) zv_sum = 100;
    if (*cvl > 0) *cvl -= (zv_sum / 4) ? (zv_sum / 4) : 1;
    else if (*cvr < 256) *cvr += (zv_sum / 4) ? (zv_sum / 4) : 1;

    if (*cvl < 0) *cvl = 0;
    if (*cvr > 255) *cvr = 255;
  }

  if (*cvr < 90 && *cvl < 90)
  {
    *cvr += 50;
    *cvl += 50;
  }
  SetMotors (0, 0, *cvl, *cvr, 0);
  return 0;
}
