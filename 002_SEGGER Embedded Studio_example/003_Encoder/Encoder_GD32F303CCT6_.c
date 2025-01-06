/**-----------------------------------------------------------------------
 \date  06.01.2025
 *
 | **                                                  ** |
 | ** Код написан в IDE SEGGER Embedded Studio for ARM ** |
 *   BluePill+
 *   GD32F303CCT6       encoder
 *   ------------ 
 *  |            |
 *  |            |
 *  |        PB.0| <---  _|¯|_ (S1)
 *  |            |
 *  |        PB.1| <---  _|¯|_ (S2)
 *  |            |
 *  |      +3.3V | --->   +5V
 *  |        GND | --->   GND
 *
 *  https://alexgyver.ru/encoder/  весь код для примеров взят отсюда 
 *  Для тестирования и отладки использован "круглый энкодер" с RC цепями для аппаратного подавления дребезга контактов.
 *
 *  Максимально быстрый универсальный код для обработки энкодера
 *  Работает на прерывании (используется одно прерывание на PB.0 !)
 *
 *  По итогу всё работает, но периодически проглатывает шаги энкодера )))
 */
/********************************************************************/
// -------------------------   ПРИМЕР 1   --------------------------//
// ---------- Быстрая обработка энкодера digitalRead() -------------//

#include "gd32f30x.h"
#include "stdbool.h"
#include "systick.h"
#include <stdio.h>

#define ENC_TYPE 1

volatile int encCounter = 0;
volatile bool state0 = 0; 
volatile bool lastState = 0;
volatile bool turnFlag = 0;

void EXTI0_IRQHandler(void) {
  if (RESET != exti_interrupt_flag_get(EXTI_0)) {
  state0 = gpio_input_bit_get(GPIOB, GPIO_PIN_0);//A
   if (state0 != lastState) {
#if (ENC_TYPE == 1)
    turnFlag = !turnFlag;
    if (turnFlag)
      encCounter += (gpio_input_bit_get(GPIOB, GPIO_PIN_1) != lastState) ? -1 : 1;//B
#else
    encCounter += (gpio_input_bit_get(GPIOB, GPIO_PIN_1) != lastState) ? -1 : 1;
#endif
    lastState = state0;
  }
    exti_interrupt_flag_clear(EXTI_0);
  }
}

int main(void) {
  systick_config();
  rcu_periph_clock_enable(RCU_GPIOB);
  rcu_periph_clock_enable(RCU_AF);
  gpio_init(GPIOB, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_0);
  gpio_init(GPIOB, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_1);
  nvic_irq_enable(EXTI0_IRQn, 2U, 0U);
  gpio_exti_source_select(GPIO_PORT_SOURCE_GPIOB, GPIO_PIN_SOURCE_0);
  exti_init(EXTI_0, EXTI_INTERRUPT, EXTI_TRIG_RISING);
  exti_interrupt_flag_clear(EXTI_0);
  while (1) {
    delay_1ms(500);
    printf("Val = %d\n", encCounter);
  }
}