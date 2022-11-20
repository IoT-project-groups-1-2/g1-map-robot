#include "mauto.h"

#define _CLOSE_TO_OBSTACLE_ 15
#define _AVE_SPEED_ 70
#define _TANK_TURN_TIME_ 700
#define _BACKWARD_TIME_ 400

static bool avoiding = false;

/**
 * @brief Goes forward and turns left in case there is an obstacle.
 * 
 * @param cmd received json command (Currently unused.)
 */
void
auto_handle(json_command *cmd)
{
  int dist = Lidar_get_distance();
  if (dist == -1) return;

  printf("\nDistance is: %d\n", dist);

  if(dist <= _CLOSE_TO_OBSTACLE_)
  {
    printf("\nObstacle.\n");
    if(!avoiding) motor_backward(_AVE_SPEED_, _BACKWARD_TIME_);
    avoiding = true;
    motor_tank_turn_left(_AVE_SPEED_, _TANK_TURN_TIME_);
  }
  else
  {
    avoiding = false;
    motor_forward(_AVE_SPEED_, 0);
  }
}
