
/**-----------------------------------------------------------------------
 \date  14.02.2024
 *
 | ** DHT22               ** |
 | ** Код написан в IDE SEGGER Embedded Studio for ARM 7.32a    ** |
 | ** J-Link Software and Documentation pack V7.90 [2023-08-02] ** |
 *
 *   BluePill+
 *   GD32F303CCT6   
 *   ------------          _________
 *  |            |        |         |
 *  |            |        |  DHT22  |
 *  |            |        |         |
 *  |            |        |_________|
 *  |            |           | | |
 *  |        PB.5| ---->  ---| | |
 *  |            |             | |
 *  |      +3.3V | ---->    ---| |
 *  |            |               |
 *  |        GND | ---->      ---|
 *
 *  DHT22 - это цифровой датчик влажности и температуры на микросхеме AM2302.
 *  Датчик предназначен для измерения с высокой точностью, температуры и влажности окружающей среды.
 *  Датчик DHT22 выполнен в виде модуля и содержит всю необходимую обвязку.
 *
 *  Вывод информации:
 *  =================================
 *  Sample DHT22 with RAW bits...
 *  Sample RAW Bits: 0 0 0 0  0 0 0 1  0 0 1 0  0 0 1 1  0 0 0 0  0 0 0 0  1 1 1 0  1 0 0 1  0 0 0 0  1 1 0 1  
 *  Sample OK: Val = 23.02  Val = 29.02 RH
 *
 *
 *  Библиотека: https://github.com/winlinvip/SimpleDHT/tree/master
 */
/****************************************************************************************/
#include "gd32f30x.h"
#include "stdbool.h"
#include "systick.h"
#include <stdio.h>
#include <string.h>

#define SYSTICK_LOAD (SystemCoreClock / 1000000)
#define SYSTICK_DELAY_CALIB (SYSTICK_LOAD >> 1)

#define SimpleDHTErrDuration(err) ((err & 0xff00) >> 8)
#define SimpleDHTErrCode(err) (err & 0x00ff)

int integer_number;
uint32_t fractional_number;
uint8_t sign_number;
volatile uint32_t gd_ticks;
/****************************************************************************************/
#define simpleDHTCombileError(t, err) ((t << 8) & 0xff00) | (err & 0x00ff)
#define SimpleDHTErrSuccess 0
#define SimpleDHTErrStartLow 16
#define SimpleDHTErrStartHigh 17
#define SimpleDHTErrDataLow 18
#define SimpleDHTErrDataRead 19
#define SimpleDHTErrDataEOF 20
#define SimpleDHTErrDataChecksum 21
/****************************************************************************************/
uint32_t levelTimeout = 500000; // 500ms
float temperature,humidity;
uint8_t data[40] = {0};
int err = SimpleDHTErrSuccess;
/****************************************************************************************/
int read2(float *ptemperature, float *phumidity, uint8_t pdata[40]);
int parse(uint8_t data[40], short *ptemperature, short *phumidity);
uint8_t bits2byte(uint8_t data[8]);
long levelTime(uint8_t level);
int sample(uint8_t data[40]);
void Float_transform(float value, uint8_t width, uint8_t *sign_number, int *integer_number, uint32_t *fractional_number);
/****************************************************************************************/
uint32_t micros(void);
uint32_t getCurrentMicros(void);
void delayMicroseconds(uint32_t us);
void delay(uint32_t ms);
uint32_t getCurrentMillis(void);
/****************************************************************************************/
void SysTick_Handler(void) {
  gd_ticks++;
}
/****************************************************************************************/
int main(void) {
  rcu_periph_clock_enable(RCU_GPIOB);
  systick_config();

  while (1) {
    if ((err = read2(&temperature, &humidity, data)) != SimpleDHTErrSuccess) {
      printf("Read DHT22 failed, err=");
      // printf(SimpleDHTErrCode(err));
      printf(",");
      // printf(SimpleDHTErrDuration(err));
      delay(2000);
      // return;
    }
    printf("=================================\n");
    printf("Sample DHT22 with RAW bits...\n");
    read2(&temperature, &humidity, data);
    printf("Sample RAW Bits: ");
    for (int i = 0; i < 40; i++) {
      printf("%d ", data[i]);
      if (i > 0 && ((i + 1) % 4) == 0) {
        printf(" ");
      }
    }
    printf("\n");
    printf("Sample OK: ");
    Float_transform(temperature, 2, &sign_number, &integer_number, &fractional_number);
    printf("Val = %d.%.02  ", integer_number, fractional_number);
    Float_transform(humidity, 2, &sign_number, &integer_number, &fractional_number);
    printf("Val = %d.%.02", integer_number, fractional_number);
    printf(" RH%\r\n");
    delay(2500);
  }
}
/****************************************************************************************/
int read2(float *ptemperature, float *phumidity, uint8_t pdata[40]) {
  int ret = SimpleDHTErrSuccess;
  uint8_t data[40] = {0};
  if ((ret = sample(data)) != SimpleDHTErrSuccess) {
    return ret;
  }
  short temperature = 0;
  short humidity = 0;
  if ((ret = parse(data, &temperature, &humidity)) != SimpleDHTErrSuccess) {
    return ret;
  }
  if (pdata) {
    memcpy(pdata, data, 40);
  }
  if (ptemperature) {
    *ptemperature = (float)((temperature & 0x8000 ? -1 : 1) * (temperature & 0x7FFF)) / 10.0;
  }
  if (phumidity) {
    *phumidity = (float)humidity / 10.0;
  }
  return ret;
}
/****************************************************************************************/
int parse(uint8_t data[40], short *ptemperature, short *phumidity) {
  short humidity = bits2byte(data);
  short humidity2 = bits2byte(data + 8);
  short temperature = bits2byte(data + 16);
  short temperature2 = bits2byte(data + 24);
  uint8_t check = bits2byte(data + 32);
  uint8_t expect = (uint8_t)humidity + (uint8_t)humidity2 + (uint8_t)temperature + (uint8_t)temperature2;
  if (check != expect) {
    return SimpleDHTErrDataChecksum;
  }
  *ptemperature = temperature << 8 | temperature2;
  *phumidity = humidity << 8 | humidity2;
  return SimpleDHTErrSuccess;
}
/****************************************************************************************/
uint8_t bits2byte(uint8_t data[8]) {
  uint8_t v = 0;
  for (int i = 0; i < 8; i++) {
    v += data[i] << (7 - i);
  }
  return v;
}
/****************************************************************************************/
int sample(uint8_t data[40]) {
  memset(data, 0, 40);
  gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_2MHZ, GPIO_PIN_5);
  gpio_bit_reset(GPIOB, GPIO_PIN_5);
  delayMicroseconds(1000);
  gpio_bit_set(GPIOB, GPIO_PIN_5);
  gpio_init(GPIOB, GPIO_MODE_IPU, GPIO_OSPEED_2MHZ, GPIO_PIN_5);
  delayMicroseconds(40);

  long t = 0;

  if ((t = levelTime(0)) < 30) {
    return simpleDHTCombileError(t, SimpleDHTErrStartLow);
  }
  if ((t = levelTime(1)) < 50) {
    return simpleDHTCombileError(t, SimpleDHTErrStartHigh);
  }

  for (int j = 0; j < 40; j++) {
    t = levelTime(0);
    if (t < 24) {
      return simpleDHTCombileError(t, SimpleDHTErrDataLow);
    }
    t = levelTime(1);
    if (t < 11) {
      return simpleDHTCombileError(t, SimpleDHTErrDataRead);
    }
    data[j] = (t > 40 ? 1 : 0);
  }
  t = levelTime(0);
  if (t < 24) {
    return simpleDHTCombileError(t, SimpleDHTErrDataEOF);
  }
  return SimpleDHTErrSuccess;
}
/****************************************************************************************/
long levelTime(uint8_t level) {
  uint8_t firstWait = 10;
  uint8_t interval = 6;
  unsigned long time_start = micros();
  long time = 0;
  bool loop = true;
  for (int i = 0; loop; i++) {
    if (time < 0 || time > levelTimeout) {
      return -1;
    }
    if (i == 0) {
      if (firstWait > 0) {
        delayMicroseconds(firstWait);
      }
    } else if (interval > 0) {
      delayMicroseconds(interval);
    }
    time = micros() - time_start;
    loop = (gpio_input_bit_get(GPIOB, GPIO_PIN_5) == level);
  }
  return time;
}
/****************************************************************************************/
uint32_t micros(void) {
  return getCurrentMicros();
}
/****************************************************************************************/
uint32_t getCurrentMicros(void) {
  uint32_t ms = gd_ticks;
  uint32_t systick_value = SysTick->VAL;
  uint32_t systick_load = SysTick->LOAD + 1;
  uint32_t us = ((systick_load - systick_value) * 1000) / systick_load;
  return (ms * 1000 + us);
}
/****************************************************************************************/
void delayMicroseconds(uint32_t us) {
  do {
    uint32_t start = SysTick->VAL;
    uint32_t ticks = (us * SYSTICK_LOAD) - SYSTICK_DELAY_CALIB;
    while ((start - SysTick->VAL) < ticks)
      ;
  } while (0);
}
/****************************************************************************************/
void delay(uint32_t ms) {
  if (ms != 0) {
    uint32_t start = getCurrentMillis();
    do {
      __NOP();
    } while (getCurrentMillis() - start < ms);
  }
}
/****************************************************************************************/
uint32_t getCurrentMillis(void) {
  return gd_ticks;
}
/****************************************************************************************/
void Float_transform(float value, uint8_t width, uint8_t *sign_number, int *integer_number, uint32_t *fractional_number) {
  uint8_t i;
  uint32_t rounding_2 = 1;
  float rounding = 0.5f;
  *sign_number = 0;
  if (*((uint32_t *)&value) & 0x80000000) {
    *((uint32_t *)&value) &= 0x7FFFFFFF;
    *sign_number = 1;
  }
  for (i = 0; i < width; i++) {
    rounding_2 = rounding_2 * 10;
  }
  rounding = rounding / rounding_2;
  *integer_number = (int)((float)value + rounding);
  *fractional_number = (((float)value + rounding) * rounding_2) - (*integer_number * rounding_2);
}
/************************************ End of file ***************************************/

