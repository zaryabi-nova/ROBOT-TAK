/* Vol.1 section 4-11 - Example 5: read a potentiometer on PA0 (A0) with ADC1
 * CubeMX: ADC1 > IN0, Resolution 12 bit, other settings default.
 * Potentiometer: ends to 3.3 V and GND, wiper to PA0 (never above 3.3 V).
 * Also enable USART2 and the printf code from example 2.
 */

/* USER CODE BEGIN WHILE */
  while (1)
  {
    HAL_ADC_Start(&hadc1);
    if (HAL_ADC_PollForConversion(&hadc1, 10) == HAL_OK)
    {
      uint32_t raw = HAL_ADC_GetValue(&hadc1);             /* 0 .. 4095 */
      uint32_t mv  = raw * 3300 / 4095;
      printf("raw=%lu  %lu mV\r\n", raw, mv);
    }
    HAL_ADC_Stop(&hadc1);
    HAL_Delay(200);
    /* USER CODE END WHILE */
    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
