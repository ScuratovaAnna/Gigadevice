/**-----------------------------------------------------------------------
 \date  14.12.2025
 *
 | ** Первый пример поморгаем светодиодами.  FreeRTOS        ** |
 | ** Код написан в IDE SEGGER Embedded Studio for ARM 7.32a ** |
 *   BluePill+
 *   GD32F303CCT6
 *   ------------
 *  |            |
 *  |            |
 *  |        PB.3| ---->  LED
 *  |            |
 *  |        PB.6| ---->  LED
 *  |            |
 *  |        PB.9| ---->  LED
 *  |            |
 *  |            |
 *  |      +3.3V |
 *  |        GND |
 *
 *
 *\ author         ScuratovaAnna 
 *\ сode debugging PivnevNikolay
 */

/*************************** The first example **********************/
#include "gd32f30x.h"
#include "systick.h"
/* Kernel includes. */
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
#include "timers.h"

void LED_PB3_Task(void *arg) {
  (void)arg;
  rcu_periph_clock_enable(RCU_GPIOB);
  gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_3);
  while (1) {
    gpio_bit_set(GPIOB, GPIO_PIN_3);
    vTaskDelay(250);
    gpio_bit_reset(GPIOB, GPIO_PIN_3);
    vTaskDelay(250);
  }
}

void LED_PB6_Task(void *arg) {
  (void)arg;
  rcu_periph_clock_enable(RCU_GPIOB);
  gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_6);
  while (1) {
    gpio_bit_set(GPIOB, GPIO_PIN_6);
    vTaskDelay(500);
    gpio_bit_reset(GPIOB, GPIO_PIN_6);
    vTaskDelay(500);
  }
}

void LED_PB9_Task(void *arg) {
  (void)arg;
  rcu_periph_clock_enable(RCU_GPIOB);
  gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);
  while (1) {
    gpio_bit_set(GPIOB, GPIO_PIN_9);
    vTaskDelay(1000);
    gpio_bit_reset(GPIOB, GPIO_PIN_9);
    vTaskDelay(1000);
  }
}

int main(void) {
  systick_config();
  xTaskCreate(
      LED_PB3_Task,             /* Task function */
      "LED Blink PB0",          /* Task name (string) */
      configMINIMAL_STACK_SIZE, /* Task stack, allocated from heap */
      0,                        /* No param passed to task function */
      1,                        /* Low priority */
      0);
  xTaskCreate(LED_PB6_Task, "LED Blink PB6", configMINIMAL_STACK_SIZE, 0, 1, 0);
  xTaskCreate(LED_PB9_Task, "LED Blink PB9", configMINIMAL_STACK_SIZE, 0, 1, 0);

  vTaskStartScheduler();
  while (1) {
  }
}

/*************************** End of file ****************************/