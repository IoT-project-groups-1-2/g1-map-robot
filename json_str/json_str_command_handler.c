#include "Motor.h"
#include "json_str.h"
#include "mauto.h"

static bool motors_working = false;

/**
 * @brief Handle the command stucture & execute
 *
 * @param cmd pointer to structure for command parameters
 */
void
json_str_handle_cmd (json_command *cmd)
{
  if (cmd->speed == 0 && cmd->duration == 0 && motors_working)
  {
    motor_stop();
    motors_working = false;
  }
  else
  {
    if (!motors_working)
    {
      motor_start ();
      motors_working = true;
    }
    switch (cmd->direction)
    {
    case M_DIR_FORWARD:
      motor_forward (cmd->speed, cmd->duration);
      break;
    case M_DIR_BACKWARD:
      motor_backward (cmd->speed, cmd->duration);
      break;
    case M_DIR_LEFT:
      motor_turn (cmd->speed/50, cmd->speed, cmd->duration);
      break;
    case M_DIR_RIGHT:
      motor_turn (cmd->speed, cmd->speed/50, cmd->duration);
      break;
    default:
      break;
    }
  }
}