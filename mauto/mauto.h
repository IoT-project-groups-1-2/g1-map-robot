#ifndef MAUTO_H
#define MAUTO_H

#include "Accel_magnet.h"
#include "Beep.h"
#include "Motor.h"
#include "Ultra.h"
#include "json_str.h"

void
auto_start();

void
auto_handle(json_command *cmd);

void
auto_stop();

#endif //MAUTO_H