/**-----------------------------------------------------------------------
 \date  20.01.2025
 *
 *   BluePill+
 *   GD32F303CC  
 *   ------------ 
 *  |            |
 *  |            |
 *  |            |
 *  |        PB.2| ---->  LED
 *  |            | 
 *  |        PA.0| <----  Button
 *  |            |
 *  |      +3.3V |
 *  |        GND | 
 */
/********************************************************************/
#include "gd32f30x.h"
#include "stdbool.h"
#include <stdio.h>

__IO bool led_state = 0;

// Простая задержка
void delay_simple(volatile uint32_t count) {
  for (; count != 0; count--);
}

// Функция антидребезга (программная задержка)
uint8_t button_debounce(void) {
  if (gpio_input_bit_get(GPIOA, GPIO_PIN_0) == SET) // Кнопка нажата?
  {
    delay_simple(10000);                              // Задержка 10-20 мс
    if (gpio_input_bit_get(GPIOA, GPIO_PIN_0) == SET) // Проверяем еще раз
    {
      // Ждем отпускания кнопки
      while (gpio_input_bit_get(GPIOA, GPIO_PIN_0) == SET);
      delay_simple(1000); // Задержка после отпускания
      return 1;           // Кнопка была нажата
    }
  }
  return 0; // Кнопка не нажата
}

int main(void) {
  rcu_periph_clock_enable(RCU_GPIOA);
  gpio_init(GPIOA, GPIO_MODE_IPD, GPIO_OSPEED_50MHZ, GPIO_PIN_0); // Button
  rcu_periph_clock_enable(RCU_GPIOB);
  gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_2); // Led

  while (1) {
    // Опрашиваем кнопку с антидребезгом
    if (button_debounce()) {
      // Меняем состояние светодиода
      led_state = !led_state;
      if (led_state)
        gpio_bit_set(GPIOB, GPIO_PIN_2); // Включить
      else
        gpio_bit_reset(GPIOB, GPIO_PIN_2); // Выключить
    }
  }
}
/*************************** End of file ****************************/

