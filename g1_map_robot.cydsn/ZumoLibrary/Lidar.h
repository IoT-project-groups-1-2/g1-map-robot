#ifndef LIDAR_H_
#define LIDAR_H_

#include "stdint.h"
#include <project.h>

#define LIDAR_PKG_HEADER 0x59
#define LIDAR_PKG_SIZE 9

void
Lidar_start();

int
Lidar_get_distance();

#endif //LIDAR_H_
