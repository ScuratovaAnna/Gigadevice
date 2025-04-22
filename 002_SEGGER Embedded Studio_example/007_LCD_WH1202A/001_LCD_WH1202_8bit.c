/**-----------------------------------------------------------------------
 *
 | ** WH1202A 8 бит 2 строки                                    ** |
 | ** Код написан в IDE SEGGER Embedded Studio for ARM 7.32a    ** |
 | ** J-Link Software and Documentation pack V7.90 [2023-08-02] ** |
 *   
 *   GD32F303CCT6            WH1202A
 *   ------------         -------------
 *  |            |       |
 *  |        PB.5| ----> |RS
 *  |        PB.6| ----> |E
 *  |            |       |
 *  |        PA.0| ----> |DB0
 *  |        PA.1| ----> |DB1
 *  |        PA.2| ----> |DB2
 *  |        PA.3| ----> |DB3
 *  |        PA.4| ----> |DB4
 *  |        PA.5| ----> |DB5
 *  |        PA.6| ----> |DB6
 *  |        PA.7| ----> |DB7
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

#define e1 gpio_bit_set(GPIOB, GPIO_PIN_5);
#define e0 gpio_bit_reset(GPIOB, GPIO_PIN_5);
#define rs1 gpio_bit_set(GPIOB, GPIO_PIN_6);
#define rs0 gpio_bit_reset(GPIOB, GPIO_PIN_6);

// volatile int counter = -99;
volatile int counter = 0;

void lcd_com(uint8_t p);
void lcd_dat(unsigned char p);
void lcd_init_8bit(void);
void lcd_SetPos(uint8_t x, uint8_t y);
void lcd_print(const char *s);
void lcd_string(char *p, uint8_t x, uint8_t y);
void LCD_PrintNumber(int num);
void LCD_PrintHex(uint8_t num);
void lcd_printBinary(uint8_t num);
void lcds_creen_left(uint8_t n);
void lcds_creen_right(uint8_t n);

int main(void) {
  systick_config();
  /* enable the peripherals clock */
  rcu_periph_clock_enable(RCU_GPIOA);
  rcu_periph_clock_enable(RCU_GPIOB);
  /* GPIOA output */
  gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);
  /* GPIOA output */
  gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_5 | GPIO_PIN_6);

  lcd_init_8bit(); // Инициализация дисплея
  lcd_SetPos(0, 0);
  lcd_print("Hello..");
  lcd_SetPos(2, 1);
  lcd_string("HI BABY", 1, 1);
  delay_1ms(100);
  lcds_creen_right(5);//Подвигаем туда
  lcds_creen_left(5); //Подвигаем сюда 
  delay_1ms(50);
  lcd_com(0x01);      //clear the screen
  lcd_SetPos(0, 0);
  lcd_print("Binary : ");
  for (uint8_t i = 0; i < 255; i++) {
    lcd_SetPos(0, 1);
    lcd_printBinary(i);
    delay_1ms(150);
  }
  lcd_com(0x01);      //clear the screen
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
      lcd_com(0x01); //clear the screen
      lcd_SetPos(0, 0);
      lcd_print("Dec:  Hex: ");
    }
  }
}
/******************************************************************************
*                  Функция записи команды в LCD                               *
*******************************************************************************/
void lcd_com(uint8_t p) {
  rs0;                  // RS = 0 будет передана команда
  e1;                   // EN = 1 (начало записи команды в LCD)
  GPIO_OCTL(GPIOA) = p; // Вывод команды на шину PA0-PA7 LCD
  delay_1ms(1);         // Длительность сигнала EN
  e0;                   // EN = 0 (конец записи команды в LCD)
  delay_1ms(1);         // Пауза для выполнения команды
}
/******************************************************************************
*                  Функция вывода одного символа (char)                       *
*******************************************************************************/
void lcd_write_Char(unsigned char p) {
  rs1;                  // RS = 1 будут переданы пользовательские данные
  e1;                   // EN = 1 (начало записи команды в LCD)
  GPIO_OCTL(GPIOA) = p; // Вывод команды на шину PA0-PA7 LCD
  delay_1ms(1);         // Длительность сигнала EN
  e0;                   // EN = 0 (конец записи команды в LCD)
  delay_1ms(1);         // Пауза для выполнения команды
}
/******************************************************************************
*                   Функция инициализации LCD                                 *
*******************************************************************************/
void lcd_init_8bit(void) {
  rs0;
  e0;
  gpio_port_write(GPIOA, 0x00); // Лог. нули на выходе
  lcd_com(0x08);                // Полное выключение дисплея
  lcd_com(0x38);                // 8 бит 2 строки
  lcd_com(0x01);                // Очистка дисплея
  lcd_com(0x06);                // Сдвиг курсора вправо
  lcd_com(0x0C);                // Курсор невидим
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
    lcd_write_Char(s[i]);
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
    lcd_write_Char(*p++);
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
    lcd_write_Char((num & (1 << i)) ? '1' : '0');
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