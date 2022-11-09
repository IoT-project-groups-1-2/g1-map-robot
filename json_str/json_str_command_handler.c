#include "Motor.h"
#include "json_str.h"

/**
 * @brief Handle the command stucture & execute
 *
 * @param cmd pointer to structure for command parameters
 */
void
json_str_handle_cmd (json_command *cmd)
{
  motor_stop ();
  motor_start ();
  switch (cmd->direction)
    {
    case M_DIR_FORWARD:
      motor_forward (cmd->speed, cmd->duration);
      break;
    case M_DIR_BACKWARD:
      motor_backward (cmd->speed, cmd->duration);
      break;
    case M_DIR_LEFT:
      motor_turn (cmd->speed, 0, cmd->duration);
      break;
    case M_DIR_RIGHT:
      motor_turn (0, cmd->speed, cmd->duration);
      break;
    default:
      break;
    }
}