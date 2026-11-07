/* USER CODE BEGIN 2 */
HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);   // hardware encoder mode
/* USER CODE END 2 */

while (1) {
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);        // LED on many Nucleo boards
    int32_t pos = (int16_t)__HAL_TIM_GET_COUNTER(&htim2);
    HAL_Delay(100);
}
