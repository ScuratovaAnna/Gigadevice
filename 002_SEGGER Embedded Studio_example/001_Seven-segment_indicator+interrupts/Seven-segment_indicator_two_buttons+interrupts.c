/**-----------------------------------------------------------------------
 *\date  30.05.2024
 *\brief
 *
 * ** Код написан в SEGGER Embedded Studio for ARM 7.32a**
 *
 *  В проекте две кнопки + сегментный индикатор. Одна кнопка при нажатии увеличивает значение от 0 до 9 на семи сегментном индикаторе,
 *  вторая кнопка уменьшает от 9 до 0. Светодиод на выводе PC13 моргает постоянно.
 *
 *   GD32F303CCT6    Семисегментный индикатор, красный 5161AS (5611AH)    
 *   ------------        
 *  |            |
 *  |            |
 *  |        PA.0| ---->  ---
 *  |        PA.1| ----> |   |
 *  |        PA.2| ----> |   |
 *  |        PA.3| ---->  ---
 *  |        PA.4| ----> |   |
 *  |        PA.5| ----> |   |
 *  |        PA.6| ---->  ---
 *  |            |
 *  |            |
 *  |        PB.0| <---- up button
 *  |       PB.12| <---- down button
 *
 * Семисегментный индикатор, красный 5161AS (5611AH) --> https://hcomp.ru/shop/5161as/
 *
 *\authors        PivnevNikolay.
 *\сode debugging PivnevNikolay.
 *
 *\brief
 */
//************************************************************************

#include "gd32f30x.h"
#include "stdbool.h"
#include "systick.h"
#include <stdio.h>

typedef uint8_t u8;
u8 Count = 1;
__IO bool Zero = true;

u8 CW[10] = {0x3F, 0X6, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x7, 0x7F, 0x67};

void EXTI0_IRQHandler(void) {
  if (RESET != exti_interrupt_flag_get(EXTI_0)) {
    Zero = false;
    Count++;
    if (Count > 10)
      Count = 10;
    exti_interrupt_flag_clear(EXTI_0);
  }
}

void EXTI10_15_IRQHandler(void) {
  if (RESET != exti_interrupt_flag_get(EXTI_12)) {
    Zero = false;
    Count--;
    if (Count <= 0)
      Count = 1;
    exti_interrupt_flag_clear(EXTI_12);
  }
}

int main(void) {
  systick_config();
  rcu_periph_clock_enable(RCU_GPIOA);
  gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);
  
  rcu_periph_clock_enable(RCU_GPIOC);
  gpio_init(GPIOC, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_13);
  
  rcu_periph_clock_enable(RCU_GPIOB);
  rcu_periph_clock_enable(RCU_AF);
  gpio_init(GPIOB, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_0);
  nvic_irq_enable(EXTI0_IRQn, 2U, 0U);
  gpio_exti_source_select(GPIO_PORT_SOURCE_GPIOB, GPIO_PIN_SOURCE_0);
  exti_init(EXTI_0, EXTI_INTERRUPT, EXTI_TRIG_RISING);
  exti_interrupt_flag_clear(EXTI_0);
  
  gpio_init(GPIOB, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_12);
  nvic_irq_enable(EXTI10_15_IRQn, 1U, 0U);
  gpio_exti_source_select(GPIO_PORT_SOURCE_GPIOB, GPIO_PIN_SOURCE_12);
  exti_init(EXTI_12, EXTI_INTERRUPT, EXTI_TRIG_RISING);
  exti_interrupt_flag_clear(EXTI_12);

  while (1) {
    switch (Count) {
    case 1:
      gpio_port_write(GPIOA, CW[0]);
      Zero = true;
      break;
    case 2:
      gpio_port_write(GPIOA, CW[1]);
      Zero = true;
      break;
    case 3:
      gpio_port_write(GPIOA, CW[2]);
      Zero = true;
      break;
    case 4:
      gpio_port_write(GPIOA, CW[3]);
      Zero = true;
      break;
    case 5:
      gpio_port_write(GPIOA, CW[4]);
      Zero = true;
      break;
    case 6:
      gpio_port_write(GPIOA, CW[5]);
      Zero = true;
      break;
    case 7:
      gpio_port_write(GPIOA, CW[6]);
      Zero = true;
      break;
    case 8:
      gpio_port_write(GPIOA, CW[7]);
      Zero = true;
      break;
    case 9:
      gpio_port_write(GPIOA, CW[8]);
      Zero = true;
      break;
    case 10:
      gpio_port_write(GPIOA, CW[9]);
      Zero = true;
      break;
    }
    while (Zero) {
      gpio_bit_set(GPIOC, GPIO_PIN_13);
      delay_1ms(100);
      gpio_bit_reset(GPIOC, GPIO_PIN_13);
      delay_1ms(100);
    }
  }
}