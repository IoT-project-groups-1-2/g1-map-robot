#include "Lidar.h"
#include "stdint.h"

static int old_dist = -1;

void
Lidar_start()
{
  UART_3_Start();
  Lidar_get_distance();
}

int
Lidar_get_distance()
{
  int distance = -1;
  int count = UART_3_GetRxBufferSize();

  if(count)
  {
    uint8_t bytes[57];
    for(int i = 0; i < count; ++i)
    {
      bytes[i] = UART_3_GetByte();
    }
    if(count % 9 == 0 && bytes[0] == 0x59 && bytes[1] == 0x59)
    {
      distance = bytes[2] | (bytes[3] << 8);
      old_dist = distance;
    }
  }
  return old_dist;
}
