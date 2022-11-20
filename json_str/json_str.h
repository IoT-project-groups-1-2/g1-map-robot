//
// Created by tylen on 8.11.2022.
//

#ifndef JSON_JSON_STRING_H
#define JSON_JSON_STRING_H

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>

#define JSON_STR_MAX_STRING_LENGTH_FROM_CTX 512
#define JSON_STR_SEND_MAX_STRING_LEN 1024
#define JSON_STR_NULL ((char *)0)

typedef enum _md
{
  M_DIR_FORWARD,
  M_DIR_BACKWARD,
  M_DIR_LEFT,
  M_DIR_RIGHT
} MotorDirection;

typedef struct json_command
{
  unsigned int duration;
  int speed;
  MotorDirection direction;
  bool mode;
} json_command;

bool json_str_int_from_context (char *json_str, const char *context,
                                int *dest);
bool json_str_str_from_context (char *json_str, const char *context,
                                char *dest);
void json_str_handle_cmd (json_command *cmd);

#endif // JSON_JSON_STRING_H
