/* Vol.1 section 4-11 - Example 1: blink LD2, faster while B1 is pressed
 * Board: NUCLEO-F446RE (LD2 = PA5, B1 = PC13, button is active LOW)
 * CubeMX: start from the board selector ("Initialize all peripherals with
 * their default mode" = Yes). Paste each block into the matching
 * USER CODE section of Core/Src/main.c.
 */

/* USER CODE BEGIN WHILE */
  while (1)
  {
    uint32_t wait = 500;                                   /* ms */
    if (HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) == GPIO_PIN_RESET)
      wait = 100;                                          /* button pressed */
    HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
    HAL_Delay(wait);
    /* USER CODE END WHILE */
    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
