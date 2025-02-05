/**-----------------------------------------------------------------------
 \date  02.02.2024
 *
 | ** DWT                                                       ** |
 | ** Код написан в IDE SEGGER Embedded Studio for ARM 7.32a    ** |
 | ** J-Link Software and Documentation pack V7.90 [2023-08-02] ** |
 *   BluePill+
 *   GD32F303CCT6   
 *   ------------ 
 *  |            |
 *  |            |
 *  |            |
 *  |        PB.2| ---->  LED
 *  |        PB.1| ---->  _|¯|_
 *  |            | 
 *  |            |
 *  |      +3.3V |
 *  |        GND |
 *
 * https://deepbluembedded.com/stm32-delay-microsecond-millisecond-utility-dwt-delay-timer-delay/
 * https://istarik.ru/blog/stm32/131.html
 * https://habr.com/ru/articles/476582/
 *
 * (DWT) — это компонент, встроенный в ядро ARM Cortex-M4, которое используется 
 * в микроконтроллере GD32F303CCT6. DWT предоставляет возможность отслеживать и 
 * отлаживать выполнение программы, включая функции, такие как 
 * Data Watchpoint Trigger (триггер на наблюдение за данными). Это позволяет 
 * отслеживать доступ к определенным адресам памяти или регистрам 
 * (чтение, запись или и то, и другое). 
 */
/********************************************************************/
#include "gd32f30x.h"

// #define __SYSTEM_CLOCK_72M_PLL_HXTAL            (uint32_t)(72000000)

__STATIC_INLINE void DWT_Init(void);
__STATIC_INLINE void DWT_Delay_ms(volatile uint32_t au32_milliseconds);
__STATIC_INLINE void DWT_Delay_us(volatile uint32_t au32_microseconds);

void GPIO_Init(void) {
  rcu_periph_clock_enable(RCU_GPIOB);                                             // Включение тактирования GPIOB
  gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_1 | GPIO_PIN_2); // Настройка PB2 PB1
}

int main(void) {
  GPIO_Init();
  DWT_Init();
  while (1) {
    gpio_bit_set(GPIOB, GPIO_PIN_1 | GPIO_PIN_2);
    // DWT_Delay_ms(50);
    DWT_Delay_us(50);
    gpio_bit_reset(GPIOB, GPIO_PIN_1 | GPIO_PIN_2);
    // DWT_Delay_ms(50);
    DWT_Delay_us(50);
  }
}
/********************************************************************/
// Инициализация DWT
__STATIC_INLINE void DWT_Init(void) {
  CoreDebug->DEMCR &= ~CoreDebug_DEMCR_TRCENA_Msk; // Выключеник DWT
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;  // Включение DWT
  DWT->CTRL &= ~DWT_CTRL_CYCCNTENA_Msk;            // Выключение счётчика
  DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;             // Включение счетчика CYCCNT
  __ASM volatile("NOP");
  __ASM volatile("NOP");
  __ASM volatile("NOP");
}
/********************************************************************/
__STATIC_INLINE void DWT_Delay_ms(volatile uint32_t au32_milliseconds) {
  uint32_t au32_initial_ticks = DWT->CYCCNT;
  uint32_t au32_ticks = (SystemCoreClock / 1000);
  au32_milliseconds *= au32_ticks;
  while ((DWT->CYCCNT - au32_initial_ticks) < au32_milliseconds);
}
/********************************************************************/
__STATIC_INLINE void DWT_Delay_us(volatile uint32_t au32_microseconds) {
  uint32_t au32_initial_ticks = DWT->CYCCNT;
  uint32_t au32_ticks = (SystemCoreClock / 1000000);
  au32_microseconds *= au32_ticks;
  while ((DWT->CYCCNT - au32_initial_ticks) < au32_microseconds - au32_ticks);
}
/*************************** End of file ****************************/
