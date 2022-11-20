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
  int i = 0;
  uint8_t bytes[57];

  if(count >= 9)
    for(int q = 0; q < count; ++q)
      bytes[q] = UART_3_GetByte();

  //It is a bad idea to treat any consequent 0x59 0x59 as a start of UART transaction,
  //but it will do for now.
  while (i < count - 8)
  {
    if (bytes[i] == 0x59 && bytes[i + 1] == 0x59)
    {
      distance = bytes[i + 2] | (bytes[i + 3] << 8);
      old_dist = distance;
      break;
    }
    ++i;
  }

  return old_dist;
}
