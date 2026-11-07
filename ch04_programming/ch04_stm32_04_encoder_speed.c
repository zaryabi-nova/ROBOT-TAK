/* Vol.1 section 4-11 - Example 4: quadrature encoder with TIM4 in encoder mode
 * Encoder A -> PB6 (TIM4_CH1), B -> PB7 (TIM4_CH2), encoder powered from 3.3 V
 * (or use a level shifter for 5 V encoders).
 * CubeMX: TIM4 > Combined Channels = Encoder Mode, Encoder Mode = TI1 and TI2
 *   (counts 4 edges per pulse), Counter Period = 65535, Input filter = 5.
 * Also enable USART2 and the printf code from example 2.
 */

/* USER CODE BEGIN 0 */
#define COUNTS_PER_REV  (11 * 4 * 34)   /* 11 PPR x4 edges x 1:34 gearbox */
/* USER CODE END 0 */

/* USER CODE BEGIN 2 */
  HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_ALL);
  int16_t last = (int16_t)__HAL_TIM_GET_COUNTER(&htim4);
/* USER CODE END 2 */

/* USER CODE BEGIN WHILE */
  while (1)
  {
    HAL_Delay(100);                                        /* 10 Hz */
    int16_t now = (int16_t)__HAL_TIM_GET_COUNTER(&htim4);
    int16_t diff = (int16_t)(now - last);                  /* handles wrap */
    last = now;
    int32_t rpm = (int32_t)diff * 600 / COUNTS_PER_REV;    /* per 0.1 s -> per min */
    printf("count=%d  rpm=%ld\r\n", now, rpm);
    /* USER CODE END WHILE */
    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
