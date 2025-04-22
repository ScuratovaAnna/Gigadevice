#include "gd32f30x.h"
#include "systick.h"
#include <stdio.h>
#include <string.h>

#define LCD_RS GPIO_PIN_0
#define LCD_EN GPIO_PIN_1
#define LCD_D4 GPIO_PIN_2
#define LCD_D5 GPIO_PIN_3
#define LCD_D6 GPIO_PIN_4
#define LCD_D7 GPIO_PIN_5
#define LCD_PORT GPIOA

#define e1 gpio_bit_set(GPIOA, LCD_EN);
#define e0 gpio_bit_reset(GPIOA, LCD_EN);
#define rs1 gpio_bit_set(GPIOA, LCD_RS);
#define rs0 gpio_bit_reset(GPIOA, LCD_RS);


void lcd_init_4bit(void);
void lcd_com(uint8_t cmd);
void lcd_send_data(uint8_t data);
void lcd_pulse_enable(void);
void delay_ms(uint32_t ms);
void lcd_SetPos(uint8_t x, uint8_t y);


void GPIO_Init(void) {
  rcu_periph_clock_enable(RCU_GPIOA);
  gpio_init(LCD_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ,
      LCD_RS | LCD_EN | LCD_D4 | LCD_D5 | LCD_D6 | LCD_D7);
}

// Формирование импульса на EN
void lcd_pulse_enable(void) {
  e1;
  delay_1ms(1);
  e0;
  delay_1ms(1);
}

// Пример использования
int main(void) {
  systick_config();
  GPIO_Init();
  lcd_init_4bit();
  lcd_SetPos(2, 1);
  lcd_send_data('H');
  lcd_send_data('i');
  lcd_send_data('_');
  lcd_send_data('G');
  lcd_send_data('D');
  lcd_send_data('3');
  lcd_send_data('2');
  while (1) {
  }
}

// Отправка данных
void lcd_send_data(uint8_t data) {
  gpio_bit_set(LCD_PORT, LCD_RS); // RS = 1 (данные)
  // Старшие 4 бита
  gpio_bit_write(LCD_PORT, LCD_D4, (data & 0x10) ? SET : RESET);
  gpio_bit_write(LCD_PORT, LCD_D5, (data & 0x20) ? SET : RESET);
  gpio_bit_write(LCD_PORT, LCD_D6, (data & 0x40) ? SET : RESET);
  gpio_bit_write(LCD_PORT, LCD_D7, (data & 0x80) ? SET : RESET);
  lcd_pulse_enable();
  // Младшие 4 бита
  gpio_bit_write(LCD_PORT, LCD_D4, (data & 0x01) ? SET : RESET);
  gpio_bit_write(LCD_PORT, LCD_D5, (data & 0x02) ? SET : RESET);
  gpio_bit_write(LCD_PORT, LCD_D6, (data & 0x04) ? SET : RESET);
  gpio_bit_write(LCD_PORT, LCD_D7, (data & 0x08) ? SET : RESET);
  lcd_pulse_enable();
}

// Отправка команды
void lcd_com(uint8_t cmd) {
  rs0; // RS = 0 (команда)
  // Старшие 4 бита
  gpio_bit_write(LCD_PORT, LCD_D4, (cmd & 0x10) ? SET : RESET);
  gpio_bit_write(LCD_PORT, LCD_D5, (cmd & 0x20) ? SET : RESET);
  gpio_bit_write(LCD_PORT, LCD_D6, (cmd & 0x40) ? SET : RESET);
  gpio_bit_write(LCD_PORT, LCD_D7, (cmd & 0x80) ? SET : RESET);
  lcd_pulse_enable();
  // Младшие 4 бита
  gpio_bit_write(LCD_PORT, LCD_D4, (cmd & 0x01) ? SET : RESET);
  gpio_bit_write(LCD_PORT, LCD_D5, (cmd & 0x02) ? SET : RESET);
  gpio_bit_write(LCD_PORT, LCD_D6, (cmd & 0x04) ? SET : RESET);
  gpio_bit_write(LCD_PORT, LCD_D7, (cmd & 0x08) ? SET : RESET);
  lcd_pulse_enable();
}

// Инициализация LCD
void lcd_init_4bit(void) {
  delay_1ms(10); // Ожидание после подачи питания

  // Шаг 1: Установка 4-битного режима
  lcd_com(0x33); // Переход в 8-битный режим (временный)
  lcd_com(0x32); // Переход в 4-битный режим

  // Шаг 2: Основные настройки
  lcd_com(0x28); // 4-бит, 2 строки, шрифт 5x8
  lcd_com(0x0C); // Дисплей включен, курсор выключен
  lcd_com(0x06); // Автоинкремент адреса
  lcd_com(0x01); // Очистка дисплея
  delay_1ms(2);  // Ожидание завершения очистки
}

void lcd_SetPos(uint8_t x, uint8_t y) {
  switch (y) {
  case 0:
    lcd_com(x | 0x80);
    delay_1ms(1);
    break;
  case 1:
    lcd_com((0x40 + x) | 0x80);
    delay_1ms(1);
    break;
  case 2:
    lcd_com((0x14 + x) | 0x80);
    delay_1ms(1);
    break;
  case 3:
    lcd_com((0x54 + x) | 0x80);
    delay_1ms(1);
    break;
  }
}