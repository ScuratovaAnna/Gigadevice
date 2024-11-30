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
// -------------------------   ПРИМЕР 1   --------------------------//

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

void DElay_mS(uint32_t mS) {
  SysTick->VAL &= ~SysTick_VAL_CURRENT_Msk; //значения текущего счета в 0
  SysTick->VAL = SystemCoreClock / 1000 - 1;
  SysTick_CNT = mS;
  while (SysTick_CNT) {
  }
}

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
// -------------------------   ПРИМЕР 2   --------------------------//
/********************************************************************
 *   BluePill+
 *   GD32F303CCT6   
 *   ------------ 
 *  |            |
 *  |            |
 *  |            |
 *  |        PB.2| ---->  LED PB.2 
 *  |            |
 *  |        PB.5| ---->  _|¯|_  Period=200ms +Width=100ms -Width=100ms
 *  |            |
 *  |      +3.3V |
 *  |        GND | 
 *
 ********************************************************************/

#include "gd32f30x.h"
#include <stdio.h>
//#define     __IO    volatile   /*!< Defines 'read / write' permissions */
__IO uint32_t SYSTEM_MS;

uint32_t millis(void);
void system_msdelay(uint32_t delay_ms);
void systick_setup(void);

void SysTick_Handler(void) {
  SYSTEM_MS++;
}

int main(void) {
  systick_setup();////Настройка SysTick ↓ ↓ ↓ смотри ниже описание функции
  rcu_periph_clock_enable(RCU_GPIOB);
  gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_10MHZ, GPIO_PIN_2);
  gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_10MHZ, GPIO_PIN_5);

  while (1) {
    gpio_bit_set(GPIOB, GPIO_PIN_2);
    gpio_bit_set(GPIOB, GPIO_PIN_5);
    system_msdelay(100);
    gpio_bit_reset(GPIOB, GPIO_PIN_2);
    gpio_bit_reset(GPIOB, GPIO_PIN_5);
    system_msdelay(100);
  }
}
//Настройка SysTick, не подключен #include "systick.h" и не задействована функция настройки void systick_config(void)
void systick_setup(void) {
  SysTick->LOAD &= ~SysTick_LOAD_RELOAD_Msk;
  SysTick->LOAD = (SystemCoreClock / 1000U) - 1U;                                         // 1ms tick
  SysTick->VAL = 0x00;                                                                    // explicitly set start value (undefined on reset)
  SysTick->CTRL |= (1U << SysTick_CTRL_TICKINT_Pos) | (1U << SysTick_CTRL_CLKSOURCE_Pos); // enable systick interrupt, source processor clock
  SysTick->CTRL |= (1U << SysTick_CTRL_ENABLE_Pos);                                       // enable SysTick
}
//
uint32_t millis(void) {
  return SYSTEM_MS;
}
// Задержка на указанное количество миллисекунд (мс). 1 секунда = 1'000 мс.
void system_msdelay(uint32_t delay_ms) {
  uint32_t end = millis() + delay_ms;
  while (millis() != end);
}
/*************************** End of file ****************************/
// -------------------------   ПРИМЕР 3   --------------------------//
/********************************************************************
 *   BluePill+
 *   GD32F303CCT6   
 *   ------------ 
 *  |            |
 *  |            |
 *  |            |
 *  |        PB.2| ---->  LED PB.2 
 *  |            | 
 *  |        PB.5| ---->  _|¯|_  Period=150ms +Width=75ms -Width=75ms
 *  |            |
 *  |      +3.3V |
 *  |        GND | 
 *
 ********************************************************************/

#include "gd32f30x.h"
#include <stdio.h>
#include "systick.h"//!!!

//#define     __IO    volatile   /*!< Defines 'read / write' permissions */
__IO uint32_t gd_ticks;

void delay(uint32_t ms);
uint32_t getCurrentMillis(void);

void SysTick_Handler(void) {
  gd_ticks++;
}

int main(void) {
  systick_config();
  rcu_periph_clock_enable(RCU_GPIOB);
  gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_10MHZ, GPIO_PIN_2);
  gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_10MHZ, GPIO_PIN_5);

  while (1) {
    gpio_bit_set(GPIOB, GPIO_PIN_2);
    gpio_bit_set(GPIOB, GPIO_PIN_5);
    delay(75);
    gpio_bit_reset(GPIOB, GPIO_PIN_2);
    gpio_bit_reset(GPIOB, GPIO_PIN_5);
    delay(75);
  }
}
//аналог arduino → delay(time)
//Задержка на указанное количество миллисекунд (мс). 1 секунда = 1'000 мс
void delay(uint32_t ms)
{
    if (ms != 0) {
        uint32_t start = getCurrentMillis();
        do {
            __NOP();
        } while (getCurrentMillis() - start < ms);
    }
}

uint32_t getCurrentMillis(void)
{
    return  gd_ticks;
}
/*************************** End of file ****************************/
// -------------------------   ПРИМЕР 4   --------------------------//
/********************************************************************
 *   BluePill+
 *   GD32F303CCT6
 *   ------------
 *  |            |
 *  |            |
 *  |            |
 *  |            |
 *  |        PB.5| ---->  _|¯|_  Period=100us +Width=50us -Width=50us
 *  |            |
 *  |      +3.3V |
 *  |        GND |
 *
 ********************************************************************/

#include "gd32f30x.h"
#include "systick.h"//!!!
#include <stdio.h>

void delayMicroseconds(unsigned int us);

int main(void) {
  systick_config();

  rcu_periph_clock_enable(RCU_GPIOB);
  gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_10MHZ, GPIO_PIN_5);

  while (1) {
    gpio_bit_set(GPIOB, GPIO_PIN_5);
    delayMicroseconds(50);
    gpio_bit_reset(GPIOB, GPIO_PIN_5);
    delayMicroseconds(50);
  }
}
/*Аналог функции delayMicroseconds().
  us: количество микросекунд, на которое приостанавливается выполнение программы. (unsigned int)
  Останавливает выполнение программы на заданное в параметре количество микросекунд (1 000 000 микросекунд в 1 секунде).*/
void delayMicroseconds(unsigned int us) {
  __IO uint32_t currentTicks = SysTick->VAL;
  /* Number of ticks per millisecond */
  const uint32_t tickPerMs = SysTick->LOAD + 1;
  /* Number of ticks to count */
  const uint32_t nbTicks = ((us - ((us > 0) ? 1 : 0)) * tickPerMs) / 1000;
  /* Number of elapsed ticks */
  uint32_t elapsedTicks = 0;
  __IO uint32_t oldTicks = currentTicks;
  do {
    currentTicks = SysTick->VAL;
    elapsedTicks += (oldTicks < currentTicks) ? tickPerMs + oldTicks - currentTicks : oldTicks - currentTicks;
    oldTicks = currentTicks;
  } while (nbTicks > elapsedTicks);
}

/*************************** End of file ****************************/
