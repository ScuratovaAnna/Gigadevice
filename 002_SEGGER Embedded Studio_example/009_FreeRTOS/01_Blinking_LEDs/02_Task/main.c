/**-----------------------------------------------------------------------
 \date  14.12.2025
 *
 | ** Второй пример поморгаем светодиодами.  FreeRTOS_Task   ** |
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
// Kernel includes.
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
#include "timers.h"

// GPIO LED
#define RedLEDPin    GPIO_PIN_3
#define GreenLEDPin  GPIO_PIN_6
#define YellowLEDPin GPIO_PIN_9

// Структура для представления светодиода
typedef struct  {
  uint32_t gpio_periph;   // Порт GPIO
  uint32_t pin;           // Номер пина
  unsigned long delay_ms;
} ledStruct;

void blinkLED(void *arg) {
  ledStruct *myLED = (ledStruct *)arg;

  rcu_periph_clock_enable(RCU_GPIOB);
  gpio_init(myLED->gpio_periph, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, myLED->pin);
  while (1) {
    gpio_bit_set(myLED->gpio_periph, myLED->pin);
    vTaskDelay(myLED->delay_ms);
    gpio_bit_reset(myLED->gpio_periph, myLED->pin); 
    vTaskDelay(myLED->delay_ms);
  }
}

ledStruct RedLED    = {GPIOB,RedLEDPin,    1000};
ledStruct GreenLED  = {GPIOB,GreenLEDPin,  500};
ledStruct YellowLED = {GPIOB,YellowLEDPin, 250};

int main(void) {
 systick_config();
 xTaskCreate(blinkLED , "LED Blink PB3", configMINIMAL_STACK_SIZE, &RedLED,    1, 0);
 xTaskCreate(blinkLED , "LED Blink PB6", configMINIMAL_STACK_SIZE, &GreenLED,  1, 0);
 xTaskCreate(blinkLED,  "LED Blink PB9", configMINIMAL_STACK_SIZE, &YellowLED, 1, 0);
 vTaskStartScheduler();
  while (1) {
  }
}

/*************************** End of file ****************************/