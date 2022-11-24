#ifndef BALANCE_H
#define BALANCE_H

#include "FreeRTOS.h"
#include "Gyro.h"
#include "I2C_Common.h"
#include "Motor.h"
#include "task.h"
#include <stdbool.h>
#include <stdlib.h>

/* set gyroscope into active mode */
#define GYRO_MODE_ACTIVE 0x0F
/* set full scale selection to 2000dps */
#define GYRO_FULL_SCALE_SELECTION 0x30
/* coefficent to divide a list of values with the size of this
 * 20 is found to be best
 */
#define Z_PLANE_DIVISION_COEFFICIENT 0x14
#define Z_PLANE_D_C_SQUARED                                                   \
  Z_PLANE_DIVISION_COEFFICIENT *Z_PLANE_DIVISION_COEFFICIENT
#define PREDICTION_DURATION 50
#define IS_LEFT > 0
#define IS_RIGHT < 0

int16_t z_plane_get_current ();
uint8_t predict_motor_direction (int z_plane_velocity, uint8_t current_speed,
                                 int *integral);

#endif /* BALANCE_H */
