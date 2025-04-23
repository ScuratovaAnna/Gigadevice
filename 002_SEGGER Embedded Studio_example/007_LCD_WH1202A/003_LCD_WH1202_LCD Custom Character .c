/**-----------------------------------------------------------------------
 *
 | ** WH1202A 4 бит 2 строки, cвои символы  (createChar)        ** |
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
 * Экран поддерживает создание “своих” дополнительных символов размером 5х7 точек.
 * Можно воспользоваться онлайн-генератором кодов символов:
 * https://maxpromer.github.io/LCD-Character-Creator/
 * Cтатья Arduino и дисплей LCD1602:
 * https://kit.alexgyver.ru/tutorials/lcd1602/
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

uint8_t bell[]     =   {0x4, 0xe, 0xe, 0xe, 0x1f, 0x0, 0x4};
uint8_t note[]     =   {0x2, 0x3, 0x2, 0xe, 0x1e, 0xc, 0x0};
uint8_t clock[]    =   {0x0, 0xe, 0x15, 0x17, 0x11, 0xe, 0x0};
uint8_t heart[]    =   {0x0, 0xa, 0x1f, 0x1f, 0xe, 0x4, 0x0};
uint8_t duck[]     =   {0x0, 0xc, 0x1d, 0xf, 0xf, 0x6, 0x0};
uint8_t check[]    =   {0x0, 0x1, 0x3, 0x16, 0x1c, 0x8, 0x0};
uint8_t cross[]    =   {0x11,0x1B,0x0E,0x04,0x04,0x0E,0x1B,0x11};
uint8_t retarrow[] =   {0x01,0x01,0x01,0x05,0x09,0x1F,0x08,0x04};
uint8_t smile[]    =   {0x00,0x1B,0x1B,0x00,0x04,0x11,0x0E,0x00};
uint8_t sadness[]  =   {0x00,0x1B,0x1B,0x00,0x04,0x00,0x0E,0x11};

uint8_t battery_0[] =  {0x0E,0x1F,0x11,0x11,0x11,0x11,0x11,0x1F};
uint8_t battery_1[] =  {0x0E,0x1F,0x11,0x11,0x11,0x11,0x1F,0x1F};
uint8_t battery_2[] =  {0x0E,0x1F,0x11,0x11,0x11,0x1F,0x1F,0x1F};
uint8_t battery_3[] =  {0x0E,0x1F,0x11,0x11,0x1F,0x1F,0x1F,0x1F};
uint8_t battery_4[] =  {0x0E,0x1F,0x11,0x1F,0x1F,0x1F,0x1F,0x1F};
uint8_t battery_5[] =  {0x0E,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F};

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
void createChar(uint8_t location, uint8_t charmap[]);

void GPIO_Init(void) {
  rcu_periph_clock_enable(RCU_GPIOA);
  gpio_init(LCD_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ,
      LCD_RS | LCD_EN | LCD_D4 | LCD_D5 | LCD_D6 | LCD_D7);
}

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
  createChar(0, bell);
  createChar(1, note);
  createChar(2, clock);
  createChar(3, heart);
  createChar(4, sadness);
  createChar(5, smile);
  createChar(6, cross);
  createChar(7, retarrow);
  lcd_SetPos(0, 0);
  lcd_send_data(0);
  lcd_send_data(1);
  lcd_send_data(2);
  lcd_send_data(3);
  lcd_send_data(4);
  lcd_send_data(5);
  lcd_send_data(6);
  lcd_send_data(7);
  delay_1ms(2000);
  lcd_com(0x01);
  createChar(0, battery_0);
  createChar(1, battery_1);
  createChar(2, battery_2);
  createChar(3, battery_3);
  createChar(4, battery_4);
  createChar(5, battery_5);
  while (1) {
  for (int i = 0; i < 6; i++) {
    lcd_SetPos(0, 0);
    lcd_send_data(i);
    lcd_SetPos(0, 1);
    lcd_print("battery:");
    LCD_PrintNumber(i*20);
    lcd_print("%");
    delay_1ms(1500);
    if (i==5)lcd_com(0x01);
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
 *                                                                             *
 *******************************************************************************/
void createChar(uint8_t location, uint8_t charmap[]) {
	location &= 0x7; // we only have 8 locations 0-7
	lcd_com(0x40 | (location << 3));
	for (int i=0; i<8; i++) {
          lcd_send_data(charmap[i]);
	}
}
/******************************************************************************
 *                                                                             *
 *******************************************************************************/
void LCD_PrintNumber(int num) {
  char buffer[16];
  snprintf(buffer, sizeof(buffer), "%d", num);
  lcd_print(buffer);
}
/******************************* End of file ***********************************/