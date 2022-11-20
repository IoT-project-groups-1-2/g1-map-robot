#include "mauto.h"

static bool avoiding = false;

/**
 * @brief Goes forward and turns left in case there is an obstacle.
 * 
 * @param cmd 
 */
void
auto_handle(json_command *cmd)
{
  int dist = Lidar_get_distance();
  if (dist == -1) return;

  printf("\nDistance is: %d\n", dist);

  if(dist <= 25)
  {
    printf("\nObstacle.\n");
    if(!avoiding) motor_backward(70, 400);
    avoiding = true;
    motor_tank_turn_left(70, 500);
  }
  else
  {
    avoiding = false;
    motor_forward(70, 100);
  }
}
