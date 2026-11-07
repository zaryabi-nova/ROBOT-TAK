/* Vol.1 section 4-11 - Example 6: two FreeRTOS tasks (CMSIS-RTOS v2)
 * CubeMX: Middleware > FREERTOS > Interface = CMSIS_V2.
 *   SYS > Timebase Source = TIM6 (not SysTick; FreeRTOS uses SysTick).
 *   Tasks and Queues: add two tasks
 *     ledTask    - Entry Function StartLedTask    - osPriorityNormal
 *     reportTask - Entry Function StartReportTask - osPriorityBelowNormal
 *   CubeMX writes both functions at the end of main.c; paste the bodies
 *   below between their "USER CODE BEGIN" and "USER CODE END" lines.
 * Also enable USART2 and the printf code from example 2.
 */

/* ---- body of StartLedTask() ---- */
  for (;;)
  {
    HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
    osDelay(250);                    /* sleep; the CPU runs other tasks */
  }

/* ---- body of StartReportTask() ---- */
  uint32_t n = 0;
  for (;;)
  {
    printf("report %lu  free heap=%u bytes\r\n", n++, (unsigned)xPortGetFreeHeapSize());
    osDelay(1000);
  }
