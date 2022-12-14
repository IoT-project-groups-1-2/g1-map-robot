#ifndef MAUTO_H
#define MAUTO_H

#include <stdbool.h>
#include <project.h>
#include "serial.h"
#include "serial1.h"
#include "Accel_magnet.h"
#include "Beep.h"
#include "Motor.h"
#include "Lidar.h"
#include "json_str.h"

void
auto_handle(json_command *cmd);

#endif //MAUTO_H
