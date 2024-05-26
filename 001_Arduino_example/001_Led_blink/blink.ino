/**-----------------------------------------------------------------------
 *\date  26.05.2024
 *\brief
 *
 * ** Код написан в Arduino 1.8.19 **
 *
 *   GD32F303CCT6          
 *   ------------        
 *  |            |
 *  |            |
 *  |        PB.2| ----> LED 
 *  |            |
 *  |            |
 *
 *
 *\authors        PivnevNikolay.
 *\сode debugging PivnevNikolay.
 *
 *
 ************************************************************************
 *\brief
 */
//******************************  Пример первый **************************
#include <Arduino.h>
#define LED PB2

void setup(){
  pinMode(LED, OUTPUT);
}
void loop(){
  digitalWrite(LED, LOW);
  delay(50);
  digitalWrite(LED, HIGH);
  delay(50);
}
/****************************** End of file *****************************/

//******************************  Пример второй **************************
#include <Arduino.h>

void setup(){
  systick_config();    
  rcu_periph_clock_enable(RCU_GPIOB);
  gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_2);
}

void loop(){
  gpio_bit_set(GPIOB, GPIO_PIN_2); 
  delay(100);
  gpio_bit_reset(GPIOB, GPIO_PIN_2);
  delay(100);
}
/****************************** End of file *****************************/

//******************************  Пример третий **************************
#include <Arduino.h>

void setup() {
  systick_config();
  RCU_REG_VAL(RCU_GPIOB) |= BIT(RCU_BIT_POS(RCU_GPIOB));
  gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_2);
}
void loop() {
  GPIO_BOP(GPIOB) = GPIO_PIN_2;
  Delay_1(150);
  GPIO_BC(GPIOB) = GPIO_PIN_2;
  Delay_1(150);
}

void Delay_1(uint32_t ms)
{
  if (ms != 0) {
    uint32_t start = getCurrentMillis();
    do {
      __NOP();
    } while (getCurrentMillis() - start < ms);
  }
}
/****************************** End of file *****************************/
