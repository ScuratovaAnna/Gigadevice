/**-----------------------------------------------------------------------
 \date  05.02.2024
 *
 | ** DWT    https://istarik.ru/blog/stm32/131.html             ** |
 | ** Код написан в IDE SEGGER Embedded Studio for ARM 7.32a    ** |
 | ** J-Link Software and Documentation pack V7.90 [2023-08-02] ** |
 *   BluePill+
 *   GD32F303CCT6   
 *   ------------ 
 *  |            |
 *  |            |
 *  |            |
 *  |            | 
 *  |            |
 *  |      +3.3V |
 *  |        GND |
 *
 *  Измерение времени выполнения участка программы, либо функции.
 *
 */
/********************************************************************/
#include "gd32f30x.h"
#include <stdio.h>

   // Инициализация DWT
void DWT_Init(void) {
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk; // Включение DWT
    DWT->CYCCNT = 0;                                // Сброс счетчика циклов
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;            // Включение счетчика CYCCNT
}

   // Измерение времени выполнения функции
uint32_t DWT_Measure_Delay(void (*function)(void)) {
    DWT->CYCCNT = 0;                                // Сброс счетчика
    function();                                     // Вызов функции
    return DWT->CYCCNT;                             // Возврат количества тактов
}

    // Пример функции с задержкой
void example_function(void) {
    for (volatile int i = 0; i < 100; i++);      // Искусственная задержка
}

int main(void) {
    // Инициализация DWT
    DWT_Init();

    // Измерение времени выполнения функции
    uint32_t cycles = DWT_Measure_Delay(example_function);
    printf("Function executed in %lu cycles\n", cycles);

    // Преобразование тактов в микросекунды (при 120 МГц)
    uint32_t time_us = (cycles * 1000000) / SystemCoreClock;
    printf("Time: %lu us\n", time_us);

    while(1);
}