/* Vol.1 section 4-11 - Example 3: hobby servo / motor PWM with TIM3 CH1 (PA6 = D12)
 * CubeMX: TIM3 > Channel1 = PWM Generation CH1.
 *   Timer clock (APB1 timer clock, see Clock Configuration) = F MHz
 *   Prescaler = F - 1           -> 1 MHz counter (1 tick = 1 us)
 *   Counter Period (ARR) = 20000 - 1   -> 20 ms = 50 Hz for servos
 *   (for a DC motor driver use ARR = 1000 - 1 -> 1 kHz)
 * Servo signal wire -> PA6, servo power from a separate 5-6 V supply,
 * common GND with the board.
 */

/* USER CODE BEGIN 2 */
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
/* USER CODE END 2 */

/* USER CODE BEGIN WHILE */
  while (1)
  {
    for (uint32_t us = 1000; us <= 2000; us += 10)        /* 0 -> 180 deg */
    {
      __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, us);   /* pulse width */
      HAL_Delay(15);
    }
    for (uint32_t us = 2000; us >= 1000; us -= 10)
    {
      __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, us);
      HAL_Delay(15);
    }
    /* USER CODE END WHILE */
    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
