#ifndef BALANCE_H
#define BALANCE_H

#include "FreeRTOS.h"
#include "Gyro.h"
#include "I2C_Common.h"
#include <stdlib.h>

/* set gyroscope into active mode */
#define GYRO_MODE_ACTIVE 0x0F
/* set full scale selection to 2000dps */
#define GYRO_FULL_SCALE_SELECTION 0x30
/* coefficent to divide a list of values with the size of this
 * 20 is found to be best
 */
#define Z_PLANE_DIVISION_COEFFICIENT 0x14
#define SQUARE(n)                                                             \
  if (n % 2)                                                                  \
  ? ((n >> 1) << 2) : (((n >> 1) << 2) + ((n >> 1) << 2) + 1)
#define Z_PLANE_D_C_SQUARED SQUARE (Z_PLANE_DIVISION_COEFFICIENT)

int16_t z_plane_get_current ();
uint8_t predict_motor_direction(int16_t z_plane_velocity);



#endif /* BALANCE_H */
