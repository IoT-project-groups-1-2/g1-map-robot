/**
 * @file    IR.h
 * @brief   IR receiver header file
 * @details If you want to use IR methods, Include IR.h file.
 */
#ifndef IR_H_
#define IR_H_

#include <stdbool.h>
#include <stdint.h>

#define IR_LOWER_LIMIT 500
#define IR_UPPER_LIMIT 5000
#define IR_SIGNAL_HIGH 0x80000000UL
#define IR_SIGNAL_MASK 0x7FFFFFFFUL

void IR_Start (void);

/* Get IR pulse length, if received value has IR_SIGNAL_HIGH set then pulse is
 * high. Use IR_SIGNAL_MASK to extract pulse length in microseconds ( value &
 * IR_SIGNAL_MASK ) */
bool IR_get (uint32_t *value, uint32_t wait);

/* Flush all previously received IR values */
void IR_flush (void);

/* Wait for a high pulse with length between IR_LOWER_LIMIT and IR_UPPER_LIMIT
 */
void IR_wait (void);

#endif
