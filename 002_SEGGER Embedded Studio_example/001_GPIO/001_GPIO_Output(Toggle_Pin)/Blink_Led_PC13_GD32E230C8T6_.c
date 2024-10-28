/**-----------------------------------------------------------------------
 \date  16.10.2024
 *
 | ** Первый пример поморгаем светодиодами.            ** |
 | ** Код написан в IDE SEGGER Embedded Studio for ARM ** |
 *   BluePill
 *   GD32E230C8T6   
 *   ------------ 
 *  |            |
 *  |            |
 *  |            |
 *  |       PC.13| ---->  LED
 *  |            | 
 *  |            |
 *  |      +3.3V |
 *  |        GND | 
 */
 
/*************************** The first example **********************/
#include "gd32e23x.h"
#include "systick.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {
  systick_config();
  /* enable the peripherals clock */
  rcu_periph_clock_enable(RCU_GPIOC);
  /* GPIOC output */
  gpio_mode_set(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_13);/* output mode */
  gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, GPIO_PIN_13);/* push pull mode */

  while (1) {
    // gpio_bit_set(GPIOB, GPIO_PIN_2);
    // GPIO_BOP(GPIOC) |= (1 << 13);
    //GPIO_BOP(GPIOC) = 0x2000;
    //delay_1ms(50);
    // gpio_bit_reset(GPIOB, GPIO_PIN_2);
    // GPIO_BC(GPIOC)  |= (1 << 13);
    //GPIO_BC(GPIOC) = 0x2000;
    //delay_1ms(50);
    gpio_bit_toggle (GPIOC, GPIO_PIN_13);
    GPIO_TG(GPIOC) = 13;
    delay_1ms(50);
  }
}
/*************************** End of file ****************************/

