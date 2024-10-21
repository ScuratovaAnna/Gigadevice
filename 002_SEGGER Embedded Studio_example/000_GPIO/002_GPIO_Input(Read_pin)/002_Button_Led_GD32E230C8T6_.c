/**-----------------------------------------------------------------------
 \date  21.10.2024
 *
 | ** Второй пример: при нажатии кнопки светодиод горит.
 | ** Код написан в IDE SEGGER Embedded Studio for ARM.
 *   BluePill
 *   GD32E230C8T6
 *   ------------
 *  |            |
 *  |            |
 *  |            |
 *  |       PC.13| ---->  LED
 *  |            |
 *  |            |
 *  |       PB.5 | <----  Button
 *  |            |
 *  |      +3.3V |
 *  |        GND |
 * 
 | **  В качестве кнопки использую модуль сенсорной кнопки емкостной TTP223.
 | **  https://www.chipdip.ru/product/ttp223
 */

/*************************** The first example **********************/
#include "gd32e23x.h"
#include "systick.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {
  systick_config();
  /* enable the peripherals clock GPIOC */
  // rcu_periph_clock_enable(RCU_GPIOC);
  RCU_REG_VAL(RCU_GPIOC) |= BIT(RCU_BIT_POS(RCU_GPIOC));
  /* enable the peripherals clock GPIOB */
  // rcu_periph_clock_enable(RCU_GPIOB);
  RCU_REG_VAL(RCU_GPIOB) |= BIT(RCU_BIT_POS(RCU_GPIOB));
  /* GPIOC output */
  gpio_mode_set(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_13);          /* output mode */
  gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, GPIO_PIN_13); /* push pull mode */
  /* GPIOB input  */
  gpio_mode_set(GPIOB, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO_PIN_5);            /* input mode */
  //gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, GPIO_PIN_5);

  /* typedef enum {DISABLE = 0, ENABLE = !DISABLE} EventStatus, ControlStatus;
     typedef enum {RESET = 0, SET = !RESET} FlagStatus;
     typedef enum {ERROR = 0, SUCCESS = !ERROR} ErrStatus; */

  while (1) {
    if (RESET == gpio_input_bit_get(GPIOB, GPIO_PIN_5)) {
      gpio_bit_set(GPIOC, GPIO_PIN_13);
    } else {
      gpio_bit_reset(GPIOC, GPIO_PIN_13);
    }
  }
}
/*************************** End of file ****************************/
