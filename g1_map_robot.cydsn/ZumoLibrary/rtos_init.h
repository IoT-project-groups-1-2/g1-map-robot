#ifndef RTOS_INIT_H_
#define RTOS_INIT_H_

#include <project.h>
#include <stdio.h>

/* RTOS includes. */
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "task.h"

#include "debug_uart.h"
#include "json_str.h"
#include "mqtt_sender.h"
#include "serial.h"
#include "serial1.h"
#include "simulator.h"
#include "zumo_config.h"
#include "movement.h"

/*
 * Installs the RTOS interrupt handlers and starts the peripherals.
 */
void prvHardwareSetup (void);

#endif //RTOS_INIT_H_
