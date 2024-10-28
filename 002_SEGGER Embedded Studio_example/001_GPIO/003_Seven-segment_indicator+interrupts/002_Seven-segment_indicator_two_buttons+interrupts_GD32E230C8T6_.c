/**-----------------------------------------------------------------------
 \date  28.10.2024
 *
 | ** В проекте две кнопки + сегментный индикатор. Первая кнопка при нажатии увеличивает значение
 |    от 0 до 9 на семи сегментном индикаторе,
 |    вторая кнопка уменьшает от 9 до 0. Светодиод на выводе PC13 моргает постоянно.
 | ** Код написан в IDE SEGGER Embedded Studio for ARM.
 *   BluePill
 *   GD32E230C8T6
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
 *  |       PC.13| ---->  LED
 *  |            |
 *  |        PB.0| <---- up button
 *  |       PB.12| <---- down button
 *
 | ** В качестве кнопок использую модуль сенсорной кнопки емкостной TTP223.
 | ** https://www.chipdip.ru/product/ttp223
 *
 | ** Cегментный индикатор
 | ** https://aliexpress.ru/item/1005004029382373.html?spm=a2g2w.orderdetail.0.0.55654aa6FMpuCd&sku_id=12000033882401395
 *
 * Файлы добавленые в исходнный проект: 
 * gd32e23x_syscfg.c
 * gd32e23x_exti.c
 * gd32e23x_gpio.c
 * gd32e23x_it.c
 * gd32e23x_rcu.c
 * systick.c
 */

/********************************************************************/
#include "gd32e23x.h"
#include "stdbool.h"
#include "systick.h"
#include <stdio.h>
#include <stdlib.h>

typedef uint8_t u8;
u8 Count = 5;
__IO bool Zero = true;

u8 CW[10] = {0x3F, 0X6, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x7, 0x7F, 0x67};

void EXTI4_15_IRQHandler(void) {
  if (RESET != exti_interrupt_flag_get(EXTI_5)) {
    Zero = false;
    Count++;
    if (Count > 10)
      Count = 10;
    exti_interrupt_flag_clear(EXTI_5);
  } else if (RESET != exti_interrupt_flag_get(EXTI_6)) {
    Zero = false;
    Count--;
    if (Count <= 0)
      Count = 1;
    exti_interrupt_flag_clear(EXTI_6);
  }
}

int main(void) {
  systick_config();
  /* enable the peripherals clock GPIOA */
  rcu_periph_clock_enable(RCU_GPIOA);
  /* enable the peripherals clock GPIOB */
  rcu_periph_clock_enable(RCU_GPIOB);
  /* enable the peripherals clock GPIOC */
  rcu_periph_clock_enable(RCU_GPIOC);

  rcu_periph_clock_enable(RCU_CFGCMP);

  /* GPIOC output */
  gpio_mode_set(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_13);          /* output mode */
  gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, GPIO_PIN_13); /* push pull mode */
  /* GPIOB input  */
  gpio_mode_set(GPIOB, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO_PIN_5 | GPIO_PIN_6); /* input mode */
  //gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, GPIO_PIN_5|GPIO_PIN_6);

  gpio_mode_set(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);
  gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);

  nvic_irq_enable(EXTI4_15_IRQn, 1U); /* enable and set key EXTI interrupt priority */
  syscfg_exti_line_config(EXTI_SOURCE_GPIOB, EXTI_SOURCE_PIN5);
  syscfg_exti_line_config(EXTI_SOURCE_GPIOB, EXTI_SOURCE_PIN6);
  exti_init(EXTI_5, EXTI_INTERRUPT, EXTI_TRIG_RISING);
  exti_init(EXTI_6, EXTI_INTERRUPT, EXTI_TRIG_RISING);
  exti_interrupt_flag_clear(EXTI_5 | EXTI_6);

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
/*************************** End of file ****************************/
