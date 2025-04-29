#include "gd32f30x.h"

void gpio_config(void) {
  rcu_periph_clock_enable(RCU_GPIOA);
  rcu_periph_clock_enable(RCU_AF);
/*
 rcu_periph_clock_enable(RCU_AF); --> в микроконтроллерах GD32F30x используется для включения
 тактирования модуля AFIO (Alternate Function I/O). Этот модуль отвечает за управление альтернативными
 функциями GPIO-пинов (например, UART, SPI, PWM и другими).
 Когда это нужно?
 Вызов rcu_periph_clock_enable(RCU_AF) требуется в следующих случаях:
 Настройка альтернативных функций пинов:
 Когда пин GPIO используется для периферии (UART, SPI, I2C, PWM и т.д.).
 Например, для работы USART0 на выводах PA9 (TX) и PA10 (RX).
 Ремаппинг пинов:
 Если необходимо переназначить стандартные пины периферии на другие GPIO (например, использовать USART2 на PD5/PD6 вместо
 PA2/PA3).
 Работа с EXTI (External Interrupts):
 При настройке внешних прерываний через пины GPIO.
 Без включения тактирования AFIO (RCU_AF), регистры управления альтернативными функциями будут недоступны.
 Если пропустить этот шаг, периферия (например, UART или SPI) не сможет работать через выбранные пины, даже
 если они правильно настроены.
*/

  gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_6);
}

void TIMER2_PWM_Init(void) {
  rcu_periph_clock_enable(RCU_TIMER2);

  timer_parameter_struct timer_cfg;
  timer_struct_para_init(&timer_cfg);
  timer_cfg.prescaler = 120 - 1;     // 120 МГц / 120 = 1 МГц
  timer_cfg.period    = 1000 - 1;    // 1 МГц / 1000 = 1 кГц (период ШИМ)
  timer_init(TIMER2, &timer_cfg);

  timer_oc_parameter_struct oc_cfg;
  timer_channel_output_struct_para_init(&oc_cfg);
  oc_cfg.ocpolarity  = TIMER_OC_POLARITY_HIGH;
  oc_cfg.outputstate = TIMER_CCX_ENABLE;
  timer_channel_output_config(TIMER2, TIMER_CH_0, &oc_cfg);

  // Установка коэффициента заполнения (50%)
  timer_channel_output_pulse_value_config(TIMER2, TIMER_CH_0, 500);
  timer_channel_output_mode_config(TIMER2, TIMER_CH_0, TIMER_OC_MODE_PWM1);
/*
 Функция timer_channel_output_mode_config()
 Назначение:
 Функция устанавливает режим работы канала таймера (TIMERx_CHy) для генерации сигналов 
 (PWM, сравнение, захват и т.д.).
 Она определяет, как канал будет реагировать на совпадение значения счетчика (CNT) с регистром сравнения (CCRy).
 TIMER_OC_MODE_TIMING --> timing mode (Выход не изменяется при совпадении (CNT == CCRy).)
 TIMER_OC_MODE_ACTIVE --> set the channel output (Устанавливает выход в активный уровень (OCy = HIGH) при CNT == CCRy.)
 TIMER_OC_MODE_INACTIVE --> clear the channel output (Устанавливает выход в неактивный уровень (OCy = LOW) при CNT == CCRy.)
 TIMER_OC_MODE_TO GGLE --> toggle on match
 TIMER_OC_MODE_LOW --> force low mode (Принудительно низкий уровень (игнорирует сравнение).)
 TIMER_OC_MODE_HIGH --> force high mode (Принудительно высокий уровень (игнорирует сравнение).)
 Фиксируют выход в заданном состоянии, игнорируя CNT. Используются для аварийного отключения сигнала.
 TIMER_OC_MODE_PWM0 --> PWM mode 0
 TIMER_OC_MODE_PWM1 --> PWM mode 1
*/
  timer_enable(TIMER2);
}

int main(void) {
  gpio_config();
  TIMER2_PWM_Init();

  while (1) {
  }
}