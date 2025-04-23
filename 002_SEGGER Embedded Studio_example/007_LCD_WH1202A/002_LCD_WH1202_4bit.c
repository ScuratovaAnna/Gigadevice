/**-----------------------------------------------------------------------
 *
 | ** WH1202A 4 бит 2 строки                                    ** |
 | ** Код написан в IDE SEGGER Embedded Studio for ARM 7.32a    ** |
 | ** J-Link Software and Documentation pack V7.90 [2023-08-02] ** |
 *   
 *   GD32F303CCT6            WH1202A
 *   ------------         -------------
 *  |            |       |
 *  |        PA.0| ----> |RS
 *  |        PA.1| ----> |E
 *  |            |       |
 *  |            |       |
 *  |        PA.2| ----> |DB4
 *  |        PA.3| ----> |DB5
 *  |        PA.4| ----> |DB6
 *  |        PA.5| ----> |DB7
 *  |            |       |
 *  |            |       |
 *  |      +3.3V |       | +5V
 *  |        GND | <---> | GND
 *
 *  вывод RW дисплея по умолчанию притянут к логическому нулю(GND).
 * -------------------------------------------------------------------
 * ccылка на экран :
 * https://www.chipdip.ru/product/wh1202a-ygh-ct
 * Демонстрация десятичного, шестнадцатеричного и двоичного форматов.
 * -------------------------------------------------------------------
 */
/******************************************************************************/
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

volatile int counter = 0;

char s1[] = "Hello, world!";
char s2[] = "GD32F303CCT6";

void lcd_init_4bit(void);
void lcd_com(uint8_t cmd);
void lcd_send_data(uint8_t data);
void lcd_pulse_enable(void);
void lcd_SetPos(uint8_t x, uint8_t y);
void lcd_print(const char *s);
void lcd_string(char *p, uint8_t x, uint8_t y);
void LCD_PrintNumber(int num);
void LCD_PrintHex(uint8_t num);
void lcd_printBinary(uint8_t num);
void lcds_creen_left(uint8_t n);
void lcds_creen_right(uint8_t n);

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

int main(void) {
  systick_config();
  GPIO_Init();
  lcd_init_4bit();
  /* lcd_SetPos(1, 0);
  lcd_print("Hello LCD");
  lcd_SetPos(2, 1);
  lcd_send_data('H');
  lcd_send_data('i');
  lcd_send_data('_');
  lcd_send_data('G');
  lcd_send_data('D');
  lcd_send_data('3');
  lcd_send_data('2');
  delay_1ms(500);
  lcd_com(0x01);
  lcd_string("HI BABY", 1, 1);
  delay_1ms(500);
  lcds_creen_right(5); // Подвигаем туда
  lcds_creen_left(5);  // Подвигаем сюда
  delay_1ms(50);
  lcd_com(0x01); // clear the screen
  lcd_SetPos(0, 0);
  lcd_print("Binary : ");
  for (uint8_t i = 0; i < 255; i++) {
   lcd_SetPos(0, 1);
   lcd_printBinary(i);
   delay_1ms(100);
 }*/
  lcd_SetPos(0, 0);
  lcd_send_data(126);
  lcd_print("Temp:");
  lcd_print("25.6C");
  lcd_send_data(223);
  delay_1ms(2000);
  lcd_com(0x01); // clear the screen
  lcd_SetPos(0, 0);
  for (int i = 0; i < strlen(s1); i++) {
    lcd_send_data(s1[i]);
    delay_1ms(300);
  }
  lcd_SetPos(0, 1);
  for (int i = 0; i < strlen(s2); i++) {
    lcd_send_data(s2[i]);
    delay_1ms(300);
  }
  delay_1ms(1000);
  lcd_com(0x01); // clear the screen
  lcd_SetPos(0, 0);
  lcd_print("Dec:  Hex: ");
  while (1) {
    lcd_SetPos(0, 1);
    LCD_PrintNumber(counter);
    lcd_SetPos(6, 1);
    LCD_PrintHex(counter);
    counter++;
    delay_1ms(100);
    if (counter == 255) {
      counter = 0;
      lcd_com(0x01); // clear the screen
      lcd_SetPos(0, 0);
      lcd_print("Dec:  Hex: ");
    }
  }
}

/******************************************************************************
 *                  Функция вывода одного символа (char)                       *
 *******************************************************************************/
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
/******************************************************************************
 *                  Функция записи команды в LCD                               *
 *******************************************************************************/
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
/******************************************************************************
 *                   Функция инициализации LCD                                 *
 *******************************************************************************/
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
/******************************************************************************
 *                   Функция установки позиции курсора                         *
 *******************************************************************************/
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
/******************************************************************************
 *                   Функция вывода строки на экран LCD                        *
 *******************************************************************************/
void lcd_print(const char *s) {
  unsigned int len = strlen(s);
  unsigned int i = 0;
  for (i = 0; i < len; i++) {
    lcd_send_data(s[i]);
  }
}
/******************************************************************************
 *         Функция вывода строка на экран LCD с заданными координатами         *
 *******************************************************************************/
void lcd_string(char *p, uint8_t x, uint8_t y) // Вывести строку на дисплей X,Y
/// lcd_string("HI BABY",4,1) ; выведется сторка по конкретным координатам
{
  lcd_SetPos(x, y);
  while (*p) {
    lcd_send_data(*p++);
  }
}
/******************************************************************************
 *         Функция вывода на экран LCD целого число типа int                   *
 *******************************************************************************/
void LCD_PrintNumber(int num) {
  char buffer[16];
  snprintf(buffer, sizeof(buffer), "%d", num);
  lcd_print(buffer);
}
/******************************************************************************
 *       Функция вывода целого числа  в шестнадцатеричном формате              *
 *******************************************************************************/
void LCD_PrintHex(uint8_t num) {
  char hex[3];
  sprintf(hex, "%X", num);
  lcd_print(hex);
}
/******************************************************************************
 *       Функция вывода целого числа  в бинарном формате                       *
 *******************************************************************************/
void lcd_printBinary(uint8_t num) {
  for (int i = 7; i >= 0; i--) {
    lcd_send_data((num & (1 << i)) ? '1' : '0');
  }
}

/******************************************************************************
 *                   Функция смещения экрана LCD влево на n символов           *
 *******************************************************************************/
void lcds_creen_left(uint8_t n) {
  for (uint8_t i = 0; i < n; i++) {
    lcd_com(0x18);
    delay_1ms(250);
  }
}
/******************************************************************************
 *                  Функция смещения экрана LCD вправо на n символов           *
 *******************************************************************************/
void lcds_creen_right(uint8_t n) {
  for (uint8_t i = 0; i < n; i++) {
    lcd_com(0x1C);
    delay_1ms(250);
  }
}
/******************************* End of file ***********************************/