#include "Lidar.h"
#include "stdint.h"
#include "task.h"

static int old_dist = -1;
QueueHandle_t received_distance;

void
vLidarTask(void *pvParameters)
{
  int dist;
  while (1)
  {
    dist = Lidar_get_distance();
    xQueueSendToBack(received_distance, (void *) &dist, 0);
    vTaskDelay(1);
  }
}

/**
 * @brief Starts UART for Lidar sensor and reads buffer for the first time, since it will most probably fail.
 */
void
Lidar_start()
{
  received_distance = xQueueCreate(1, sizeof(int));
  //Heavy relies on mqtt, so if mqtt is not started then functionality is very limited.
  (void)xTaskCreate (vLidarTask, "vLidarTask", configMINIMAL_STACK_SIZE * 10, NULL,
                     tskIDLE_PRIORITY + 2, NULL);

  Lidar_get_distance();
}

/**
 * @brief Gets distance measurement from the UART buffer, connected to Lidar sensor.
 * 
 * @return int distance to the obstacle. (<= 1200 cm) Returns -1 if there is no value available in the buffer.
 */
int
Lidar_get_distance()
{
  int distance = -1;
  int count = UART_3_GetRxBufferSize();
  int i = 0;
  uint8_t bytes[LIDAR_PKG_SIZE * 6 + 1]; //The multiplication should depend on the UART buffer size.

  if(count >= LIDAR_PKG_SIZE)
    for(int q = 0; q < count; ++q)
      bytes[q] = UART_3_GetByte();

  /*
  It is a bad idea to treat any consequent 0x59 0x59 as a start of UART transaction,
  but it will do for our purposes.
  */
  while (i < count - (LIDAR_PKG_SIZE - 1))
  {
    if (bytes[i] == LIDAR_PKG_HEADER && bytes[i + 1] == LIDAR_PKG_HEADER)
    {
      distance = bytes[i + 2] | (bytes[i + 3] << 8);
      old_dist = distance;
      break;
    }
    ++i;
  }

  return old_dist;
}
