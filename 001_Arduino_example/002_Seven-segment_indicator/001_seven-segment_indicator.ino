/**-----------------------------------------------------------------------
 *\date  29.05.2024
 *\brief
 *
 * ** Код написан в Arduino 1.8.19 **
 *
 *   GD32F303CCT6    Семисегментный индикатор, красный 5161AS (5611AH)    
 *   ------------        
 *  |            |
 *  |            |
 *  |        PA.0| ---->  ---
 *  |        PA.1| ----> |   |
 *  |        PA.2| ----> |   |
 *  |        PA.3| ---->  ---
 *  |        PA.4| ----> |   |
 *  |        PA.5| ----> |   |
 *  |        PA.6| ---->  ---
 *  |            |
 *  |            |
 *
 * Семисегментный индикатор, красный 5161AS (5611AH) --> https://hcomp.ru/shop/5161as/
 *
 *\authors        PivnevNikolay.
 *\сode debugging PivnevNikolay.
 *
 *\brief
 */
//************************************************************************
#include <Arduino.h>

typedef uint8_t u8;

u8 CW[10] = {0x3F, 0X6, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x7, 0x7F, 0x67};

void setup() {
  systick_config();
  rcu_periph_clock_enable(RCU_GPIOA);
  gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 
  | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);
}

void loop() {
  for (u8 i = 0; i < 10; i++) {
    gpio_port_write (GPIOA, *(CW+i));
    delay(400);
  }
}
/****************************** End of file *****************************/
