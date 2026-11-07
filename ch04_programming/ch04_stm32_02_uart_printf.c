/* Vol.1 section 4-11 - Example 2: printf over the ST-LINK virtual COM port
 * Board: NUCLEO-F446RE, USART2 (PA2 TX, PA3 RX) is wired to the ST-LINK.
 * CubeMX: USART2 = Asynchronous, 115200 8N1 (default for the Nucleo board).
 * Open any serial terminal at 115200 baud on the ST-LINK COM port.
 * For printing float values add "-u _printf_float" to the linker flags.
 */

/* USER CODE BEGIN Includes */
#include <stdio.h>
/* USER CODE END Includes */

/* USER CODE BEGIN 0 */
int __io_putchar(int ch)            /* called by printf() through _write() */
{
  HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
  return ch;
}
/* USER CODE END 0 */

/* USER CODE BEGIN WHILE */
  uint32_t n = 0;
  while (1)
  {
    printf("Hello robot! tick=%lu ms  n=%lu\r\n", HAL_GetTick(), n++);
    HAL_Delay(1000);
    /* USER CODE END WHILE */
    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
