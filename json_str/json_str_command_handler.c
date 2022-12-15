#include "Motor.h"
#include "json_str.h"
#include "mauto.h"

static bool motors_working = false;

static MotorDirection last_dir = M_DIR_FORWARD;

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
      last_dir = M_DIR_FORWARD;
      break;
    case M_DIR_BACKWARD:
      motor_backward (cmd->speed, cmd->duration);
      last_dir = M_DIR_BACKWARD;
      break;
    case M_DIR_LEFT:
      if(cmd->speed == 0)
      {
        motor_tank_turn_left(150, cmd->duration);
      }
      else
      {
        if(last_dir == M_DIR_FORWARD)
          motor_turn (cmd->speed/5, cmd->speed, cmd->duration);
        else
          motor_turn_backward (cmd->speed/5, cmd->speed, cmd->duration);
      }
      
      break;
    case M_DIR_RIGHT:
      if(cmd->speed == 0)
      {
        motor_tank_turn_right(150, cmd->duration);
      }
      else
      {
        if(last_dir == M_DIR_FORWARD)
          motor_turn (cmd->speed, cmd->speed/5, cmd->duration);
        else
          motor_turn_backward (cmd->speed, cmd->speed/5, cmd->duration);
      }
      break;
    default:
      break;
    }
  }
}