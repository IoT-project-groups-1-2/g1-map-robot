#include "rtos_init.h"

void
vApplicationStackOverflowHook (TaskHandle_t pxTask, char *pcTaskName)
{
  (void)pxTask;
  (void)pcTaskName;
  /* The stack space has been execeeded for a task, considering allocating
   * more. */
  taskDISABLE_INTERRUPTS ();
  for (;;)
    ;
}

void
vApplicationMallocFailedHook (void)
{
  /* The heap space has been execeeded. */
  taskDISABLE_INTERRUPTS ();
  for (;;)
    ;
}

void
prvHardwareSetup (void)
{
  /* Port layer functions that need to be copied into the vector table. */
  extern void xPortPendSVHandler (void);
  extern void xPortSysTickHandler (void);
  extern void vPortSVCHandler (void);
  extern cyisraddress CyRamVectors[];

  /* Install the OS Interrupt Handlers. */
  CyRamVectors[11] = (cyisraddress)vPortSVCHandler;
  CyRamVectors[14] = (cyisraddress)xPortPendSVHandler;
  CyRamVectors[15] = (cyisraddress)xPortSysTickHandler;

  /* Start-up the peripherals. */
  UART_1_Start ();
  UART_2_Start ();

  /* Task initializations */
  vSerial1PortInitMinimal (256);
  RetargetInit ();

#if START_MQTT
  MQTTSendTaskInit ();
#endif

#if ZUMO_SIMULATOR
  SimulatorTaskInit ();
#endif
}
