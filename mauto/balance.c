#include "balance.h"

static inline int16_t
convert_raw (uint8_t high, uint8_t low)
{
  return (int16_t) (high << 8) | low;
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

int integral = 0;
static uint8_t
pid (uint8_t motor_speed, int z_plane_velocity)
{
  float kP = 0.6, kI = 0.05, kD = 0.125;
  int error = 0, last_error = 0, derivative = 0;
  error = z_plane_velocity;
  last_error = error;
  integral += error;
  derivative = error - last_error;
  return (uint8_t) (motor_speed
                    + ((kP * error) + (kI * integral) + (kD * derivative)));
}

uint8_t
predict_motor_direction (int z_plane_velocity, uint8_t current_speed_l,
                         uint8_t current_speed_r)
{
  /** TODO
   * - get direction
   * - get absolute value of velocity
   * - correct value with PID
   */
  if (z_plane_velocity > 0)
    { // left case
      SetMotors (0, 0, pid (current_speed_l, z_plane_velocity),
                 current_speed_r, 50);
    }
  else if (z_plane_velocity < 0)
    { // rigth case
      SetMotors (0, 0, current_speed_l,
                 pid (current_speed_r, z_plane_velocity), 50);
    }
  else
    { // forward
      SetMotors (0, 0, current_speed_l, current_speed_r, 50);
    }

  return 0;
}
