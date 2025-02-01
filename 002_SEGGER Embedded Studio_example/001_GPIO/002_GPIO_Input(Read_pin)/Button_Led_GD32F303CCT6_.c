/**-----------------------------------------------------------------------
 \date  01.02.2025
 *
 | ** Второй пример: при нажатии кнопки светодиод горит.
 | ** Код написан в IDE SEGGER Embedded Studio for ARM 
 *   BluePill+
 *   GD32F303CCT6   
 *   ------------ 
 *  |            |
 *  |            |
 *  |            |
 *  |        PB.2| ---->  LED
 *  |            | 
 *  |       PA.0 | <----  Button
 *  |            |
 *  |      +3.3V |
 *  |        GND | 
 */
 
/*************************** The first example **********************/
#include "gd32f30x.h"
#include "stdbool.h"

volatile bool condition = false;

void GPIO_Init(void) {
  // Включение тактирования GPIOA и GPIOB
  RCU_REG_VAL(RCU_GPIOA) |= BIT(RCU_BIT_POS(RCU_GPIOA));
  rcu_periph_clock_enable(RCU_GPIOB);
  // rcu_periph_clock_enable(RCU_GPIOA);

  // Настройка PA0 как плавающего входа
  gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_0);
  // Настройка PB2 как LED
  gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_2);
}

int main(void) {
  GPIO_Init();
  while (1) {
    condition = gpio_input_bit_get(GPIOA, GPIO_PIN_0);
    (condition == true) ? (gpio_bit_set(GPIOB, GPIO_PIN_2)) : (gpio_bit_reset(GPIOB, GPIO_PIN_2));
  }
}
/*************************** End of file ****************************/

