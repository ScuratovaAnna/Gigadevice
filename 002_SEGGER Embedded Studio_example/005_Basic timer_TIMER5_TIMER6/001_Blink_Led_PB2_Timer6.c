/**-----------------------------------------------------------------------
 \date  06.02.2024
 *
 | ** Blink_Led_TIMER6                                          ** |
 | ** Код написан в IDE SEGGER Embedded Studio for ARM 7.32a    ** |
 | ** J-Link Software and Documentation pack V7.90 [2023-08-02] ** |
 *   BluePill+
 *   GD32F303CCT6   
 *   ------------ 
 *  |            |
 *  |            |
 *  |            |
 *  |        PB.2| ---->  LED
 *  |        PB.1| ----> _|¯|_  Period=200ms +Width=100ms -Width=100ms
 *  |            | 
 *  |            |
 *  |      +3.3V |
 *  |        GND |
 *
 *  Базовые таймеры (TIMER5, TIMER6) в микроконтроллерах GD32F303CCT6 предназначены
 *  для выполнения простых задач, таких как генерация задержек, периодических прерываний
 *  или синхронизация. Они имеют ограниченную функциональность и идеально подходят для выполнения простых задач.
 * -------------------------------------------------------------------
 * Counter width: 16 bits.
 * Source of count clock is internal clock only.
 * Multiple counter modes: count up.
 * Programmable prescaler: 16 bits. Factor can be changed on the go.
 * Auto-reload function.
 * Interrupt output or DMA request on update event.
 * -------------------------------------------------------------------
 */
/********************************************************************/
#include "gd32f30x.h"
#include "stdbool.h"
#include <stdio.h>

volatile bool i = 0;

void TIMER6_Init(void) {
  // Включение тактирования TIMER6
  rcu_periph_clock_enable(RCU_TIMER6);

  // Настройка таймера
  timer_parameter_struct timer_init_struct;
  timer_struct_para_init(&timer_init_struct);
  // #define __SYSTEM_CLOCK_120M_PLL_HXTAL -> 120 МГц
  timer_init_struct.prescaler = 11999; // Предделитель (120 МГц / 12000 = 10 кГц)
  timer_init_struct.period = 999;      // Период (10 кГц / 1000 = 10 Гц → 100 мс)
  timer_init(TIMER6, &timer_init_struct);
  // Включение прерывания
  timer_interrupt_enable(TIMER6, TIMER_INT_UP);
  nvic_irq_enable(TIMER6_IRQn, 0, 0);
  // Запуск таймера
  timer_enable(TIMER6);
}

void TIMER6_IRQHandler(void) {
  if (timer_interrupt_flag_get(TIMER6, TIMER_INT_FLAG_UP)) {
    gpio_bit_write(GPIOB, GPIO_PIN_2|GPIO_PIN_1, (i == 0) ? (i = SET) : (i = RESET));// Изменяем значение на выводе PB.0|PB.1 каждые 100 мс
    timer_interrupt_flag_clear(TIMER6, TIMER_INT_FLAG_UP);
  }
}

int main(void) {
  TIMER6_Init();
  rcu_periph_clock_enable(RCU_GPIOB);
  gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_2|GPIO_PIN_1);

  while (1) {

  }
}
