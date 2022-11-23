/**
 * @file    Magnet.c
 * @brief   Basic methods for operating magnetometer. For more details, please
 * refer to Accel_magnet.h file.
 * @details part number: LSM303D (simultaneously used with accelerometer and
 * included in Zumo shield)
 */
#include "Accel_magnet.h"
#include "FreeRTOS.h"
#include "task.h"

static vector3
vector_cross(const vector3 a, const vector3 b);
static double
vector_dot(const vector3 a, const vector3 b);
static vector3
vector_normalize(vector3 *a);

/**
 * @brief    Heading
 * @details Defined as the angle between X axis and Y axis.
 * Measured in a clockwise direction when viewing from the top of the device = -Z
 * @param    double X_AXIS : X axis value
 * @param    double Y_AXIS : Y axis value
 */
double
heading (vector3 mag, vector3 acc)
{
  vector3 from = {0, 0, 1};
  vector3 east;
  vector3 north;

  east = vector_cross(mag, acc);
  vector_normalize(&east);
  north = vector_cross(acc, east);
  vector_normalize(&north);

  double heading = 180 * atan2 (vector_dot(east, from), vector_dot(north, from)) / M_PI;

  if (heading < 0)
    heading += 360;

  // If you want to print out the value
  printf("\nheading: %7.3f \r\n\n", heading);
  return heading;
}

static vector3
vector_cross(const vector3 a, const vector3 b)
{
  vector3 res;
  res.x = (a.y * b.z) - (a.z * b.y);
  res.y = (a.z * b.x) - (a.x * b.z);
  res.z = (a.x * b.y) - (a.y * b.x);

  return res;
}

static double
vector_dot(const vector3 a, const vector3 b)
{
  return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

static vector3
vector_normalize(vector3 *a)
{
  double mag = sqrt(vector_dot(*a, *a));
  a->x /= mag;
  a->y /= mag;
  a->z /= mag;

  return *a;
}

#if 0
//magnetometer//
int main()
{
    CyGlobalIntEnable; 
    UART_1_Start();
  
    I2C_Start();
   
    uint8 X_L_M, X_H_M, Y_L_M, Y_H_M, Z_L_M, Z_H_M;
    int16 X_AXIS, Y_AXIS, Z_AXIS;
    
    I2C_write(GYRO_ADDR, GYRO_CTRL1_REG, 0x0F);             // set gyroscope into active mode
    I2C_write(GYRO_ADDR, GYRO_CTRL4_REG, 0x30);             // set full scale selection to 2000dps
    I2C_write(ACCEL_MAG_ADDR, ACCEL_CTRL1_REG, 0x37);           // set accelerometer & magnetometer into active mode
    I2C_write(ACCEL_MAG_ADDR, ACCEL_CTRL7_REG, 0x22);
    
    
    for(;;)
    {
        X_L_M = I2C_read(ACCEL_MAG_ADDR, OUT_X_L_M);
        X_H_M = I2C_read(ACCEL_MAG_ADDR, OUT_X_H_M);
        X_AXIS = convert_raw(X_L_M, X_H_M);
        
        Y_L_M = I2C_read(ACCEL_MAG_ADDR, OUT_Y_L_M);
        Y_H_M = I2C_read(ACCEL_MAG_ADDR, OUT_Y_H_M);
        Y_AXIS = convert_raw(Y_L_M, Y_H_M);
        
        Z_L_M = I2C_read(ACCEL_MAG_ADDR, OUT_Z_L_M);
        Z_H_M = I2C_read(ACCEL_MAG_ADDR, OUT_Z_H_M);
        Z_AXIS = convert_raw(Z_L_M, Z_H_M);
        
        heading(X_AXIS, Y_AXIS);
        printf("MAGNET: %d %d %d %d %d %d \r\n", X_L_M, X_H_M, Y_L_M, Y_H_M, Z_L_M, Z_H_M);
        printf("%d %d %d \r\n", X_AXIS,Y_AXIS, Z_AXIS);
        vTaskDelay(50);      
    }
}
#endif
