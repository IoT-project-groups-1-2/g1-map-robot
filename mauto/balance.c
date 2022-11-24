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

uint8_t
predict_motor_direction (int16_t z_plane_velocity)
{
  /** TODO
   * - get direction
   * - get absolute value of velocity
   * - correct value with PID
   */
  return 0;
}
