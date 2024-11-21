/**-----------------------------------------------------------------------
 \date  21.11.2024
 *
 | **                                                  ** |
 | ** Код написан в IDE SEGGER Embedded Studio for ARM ** |
 *   BluePill+
 *   GD32F303CCT6   
 *   ------------ 
 *  |            |
 *  |            |
 *  |            |
 *  |        PB.5| ---->  _|¯|_
 *  |            | 
 *  |            |
 *  |      +3.3V |
 *  |        GND | 
 */
/********************************************************************/

#include "gd32f30x.h"
#include "stdbool.h"
#include <stdio.h>

#define SYSTICK_LOAD (SystemCoreClock / 1000000)
#define SYSTICK_DELAY_CALIB (SYSTICK_LOAD >> 1)

__IO uint32_t SysTick_CNT = 0;

void SysTick_Init(void);
void DElay_mS(uint32_t mS);
void DELAY_US(uint32_t us);

void SysTick_Handler(void) {
  if (SysTick_CNT > 0)
    SysTick_CNT--;
}

int main(void) {
  SysTick_Init();
  /* enable the peripherals clock */
  rcu_periph_clock_enable(RCU_GPIOB);
  /* GPIOB output */
  gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_2MHZ, GPIO_PIN_5);
  while (1) {
    gpio_bit_set(GPIOB, GPIO_PIN_5);
    // GPIO_BOP(GPIOB) |= (1 << 5);
    // DElay_mS(100);
    DELAY_US(1);//1us
    gpio_bit_reset(GPIOB, GPIO_PIN_5);
    // GPIO_BC(GPIOB) |= (1 << 5);
    // DElay_mS(100);
    DELAY_US(1);//1us
  }
}

void SysTick_Init(void) {
  SysTick->LOAD &= ~SysTick_LOAD_RELOAD_Msk; // старые значения интервала счета в 0
  SysTick->LOAD = SystemCoreClock / 1000 - 1;
  SysTick->VAL &= ~SysTick_VAL_CURRENT_Msk;  // счетчик в 0
  SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk; // запуск счетчика
}
/********************************************************************/
void DElay_mS(uint32_t mS) {
  SysTick->VAL &= ~SysTick_VAL_CURRENT_Msk; //значения текущего счета в 0
  SysTick->VAL = SystemCoreClock / 1000 - 1;
  SysTick_CNT = mS;
  while (SysTick_CNT) {
  }
}
/********************************************************************/
void DELAY_US(uint32_t us) {
  SysTick->VAL &= ~SysTick_VAL_CURRENT_Msk; //значения текущего счета в 0
  do {
    uint32_t start = SysTick->VAL;
    uint32_t ticks = (us * SYSTICK_LOAD) - SYSTICK_DELAY_CALIB;
    while ((start - SysTick->VAL) < ticks);
  } 
  while (0);
}
/*************************** End of file ****************************/
