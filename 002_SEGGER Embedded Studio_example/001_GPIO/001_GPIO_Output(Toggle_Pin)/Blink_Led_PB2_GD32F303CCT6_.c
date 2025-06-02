/**-------------------------------------------------------------------
 \date  16.10.2024
 *
 | ** Первый пример поморгаем светодиодами.            ** |
 | ** Код написан в IDE SEGGER Embedded Studio for ARM ** |
 *   BluePill+
 *   GD32F303CCT6   
 *   ------------ 
 *  |            |
 *  |            |
 *  |            |
 *  |        PB.2| ---->  LED
 *  |            | 
 *  |            |
 *  |      +3.3V |
 *  |        GND | 
 */
 
/*************************** The first example **********************/
#include "gd32f30x.h"
#include "systick.h"
#include <stdio.h>

int main(void)
{
    systick_config(); 
    /* enable the peripherals clock */	
    rcu_periph_clock_enable(RCU_GPIOB);    
    /* GPIOB output */
    gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_2);
    
    while(1){
        // gpio_bit_set(GPIOB, GPIO_PIN_2);
        GPIO_BOP(GPIOB) |= (1 << 2);
        delay_1ms(500);
        //gpio_bit_reset(GPIOB, GPIO_PIN_2);
        GPIO_BC(GPIOB)  |= (1 << 2);
        delay_1ms(500);
    }
}

/*************************** End of file ****************************/
/**-------------------------------------------------------------------
 \date  02.06.2025
 *
 | ** Поморгаем двумя светодиодами.            ** |
 | ** Код написан в IDE SEGGER Embedded Studio for ARM ** |
 *   BluePill+
 *   GD32F303CCT6   
 *   ------------ 
 *  |            |
 *  |            |
 *  |            |
 *  |        PB.6| ---->  LED
 *  |       PB.13| ---->  LED
 *  |            |
 *  |            |
 *  |      +3.3V |
 *  |        GND | 
 */
/************************** The second example **********************/
#include "gd32f30x.h"
#include "systick.h"

#define SET_BIT(REG, BIT) ((REG) |= (BIT))
#define CLEAR_BIT(REG, BIT) ((REG) &= ~(BIT))
#define READ_BIT(REG, BIT) ((REG) & (BIT))
#define CLEAR_REG(REG) ((REG) = (0x0))
#define WRITE_REG(REG, VAL) ((REG) = (VAL))
#define READ_REG(REG) ((REG))
#define MODIFY_REG(REG, CLEARMASK, SETMASK) WRITE_REG((REG), (((READ_REG(REG)) & (~(CLEARMASK))) | (SETMASK)))

#define MD6   0x3U << 24
#define CTL6  0x0U << 26

#define CTL13 0x0U << 22
#define MD13  0x3U << 20

int main(void) {
  systick_config();
  gpio_deinit(GPIOB);
  /* enable the peripherals clock */
  // rcu_periph_clock_enable(RCU_GPIOB);
  SET_BIT(RCU_APB2EN, RCU_APB2EN_PBEN);
  /* GPIOB output */
  // gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_6);
  MODIFY_REG(GPIO_CTL0(GPIOB), GPIO_CTL0_MD6,  MD6);
  MODIFY_REG(GPIO_CTL0(GPIOB), GPIO_CTL0_CTL6, CTL6);
  // gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_13);
  MODIFY_REG(GPIO_CTL1(GPIOB), GPIO_CTL1_MD13,  MD13);
  MODIFY_REG(GPIO_CTL1(GPIOB), GPIO_CTL1_CTL13, CTL13);

  while (1) {
    SET_BIT(GPIO_BOP(GPIOB), GPIO_BOP_BOP6);
    SET_BIT(GPIO_BC(GPIOB), GPIO_BC_CR13);
    // gpio_bit_set(GPIOB, GPIO_PIN_0);
    // GPIO_BOP(GPIOB) |= (1 << 0);
    delay_1ms(100);
    // gpio_bit_reset(GPIOB, GPIO_PIN_0);
    // GPIO_BC(GPIOB)  |= (1 << 0);
    SET_BIT(GPIO_BC(GPIOB), GPIO_BC_CR6);
    SET_BIT(GPIO_BOP(GPIOB), GPIO_BOP_BOP13);
    delay_1ms(100);
  }
}
/*************************** End of file ****************************/