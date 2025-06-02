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
 *  |        PB.3| ---->  LED
 *  |            |
 *  |      PA.10 | <----  Button
 *  |            |
 *  |      +3.3V |
 *  |        GND | 
 */
/************************** The second example **********************/
#include "gd32f30x.h"
#include "stdbool.h"
#include "systick.h"

#define SET_BIT(REG, BIT) ((REG) |= (BIT))
#define CLEAR_BIT(REG, BIT) ((REG) &= ~(BIT))
#define READ_BIT(REG, BIT) ((REG) & (BIT))
#define CLEAR_REG(REG) ((REG) = (0x0))
#define WRITE_REG(REG, VAL) ((REG) = (VAL))
#define READ_REG(REG) ((REG))
#define MODIFY_REG(REG, CLEARMASK, SETMASK) WRITE_REG((REG), (((READ_REG(REG)) & (~(CLEARMASK))) | (SETMASK)))

#define MD3   0x3U << 12
#define CTL3  0x0U << 14

#define MD10  0b00 << 8
#define CTL10 0b01 << 10

volatile bool condition = false;


int main(void) {
 systick_config();
  // rcu_periph_clock_enable(RCU_GPIOA);
  SET_BIT(RCU_APB2EN, RCU_APB2EN_PAEN);
  //rcu_periph_clock_enable(RCU_GPIOB);
  SET_BIT(RCU_APB2EN, RCU_APB2EN_PBEN);

  //gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_2MHZ, GPIO_PIN_3);
  MODIFY_REG(GPIO_CTL0(GPIOB), GPIO_CTL0_MD3,  MD3);
  MODIFY_REG(GPIO_CTL0(GPIOB), GPIO_CTL0_CTL3, CTL3);

  //gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_2MHZ, GPIO_PIN_10);
  MODIFY_REG(GPIO_CTL1(GPIOA), GPIO_CTL1_MD10,  MD10);
  MODIFY_REG(GPIO_CTL1(GPIOA), GPIO_CTL1_CTL10, CTL10);

   while (1) {
  //condition = gpio_input_bit_get(GPIOA, GPIO_PIN_10);
  condition = READ_BIT(GPIO_ISTAT(GPIOA), GPIO_PIN_10);
  //(condition == 0x0u) ? (gpio_bit_set(GPIOB, GPIO_PIN_3)) : (gpio_bit_reset(GPIOB, GPIO_PIN_3));
  (condition == false) ? (SET_BIT(GPIO_BOP(GPIOB), GPIO_BOP_BOP3)) : (SET_BIT(GPIO_BC(GPIOB), GPIO_BC_CR3));
  }
}
/*************************** End of file ****************************/
