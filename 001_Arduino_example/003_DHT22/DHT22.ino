
/**-----------------------------------------------------------------------
 \date  10.02.2024
 *
 | ** DHT22               ** |
 | ** Arduino IDE 2.3.4   ** |
 *
 *   BluePill+
 *   GD32F303CCT6   
 *   ------------          _________
 *  |            |        |         |
 *  |            |        |  DHT22  |
 *  |            |        |         |
 *  |            |        |_________|
 *  |            |           | | |
 *  |        PB.6| ---->  ---| | |
 *  |            |             | |
 *  |      +3.3V | ---->    ---| |
 *  |            |               |
 *  |        GND | ---->      ---|
 *
 *  DHT22 - это цифровой датчик влажности и температуры на микросхеме AM2302.
 *  Датчик предназначен для измерения с высокой точностью, температуры и влажности окружающей среды.
 *  Датчик DHT22 выполнен в виде модуля и содержит всю необходимую обвязку.
 * -------------------------------------------------------------------
 * Optimize: "Debug (-Og)"
 * Board part number: "GD32F303CC(Generic)"
 * Upload method:"STLink(SWD)"
 * USB support: "Off"
 * -------------------------------------------------------------------
 * Вывод информации в монитор порта:
 * =================================
 * Sample DHT22 with RAW bits...
 * Sample RAW Bits: 0000 0000 1100 1110 0000 0000 1110 0011 1011 0001 
 * Sample OK: 22.70 *C, 20.60 RH%
 * CPU 120000000 Hz
 *
 * =================================
 * Библиотека: https://github.com/winlinvip/SimpleDHT/tree/master
 */
/****************************************************************************************/
#define simpleDHTCombileError(t, err) ((t << 8) & 0xff00) | (err & 0x00ff)
#define SimpleDHTErrSuccess 0
#define SimpleDHTErrStartLow 16
#define SimpleDHTErrStartHigh 17
#define SimpleDHTErrDataLow 18
#define SimpleDHTErrDataRead 19
#define SimpleDHTErrDataEOF 20
#define SimpleDHTErrDataChecksum 21

//#define SimpleDHTErrDuration(err) ((err&0xff00)>>8)
//#define SimpleDHTErrCode(err) (err&0x00ff)

long levelTimeout = 500000;  // 500ms
float temperature = 0;
float humidity = 0;
byte data[40] = { 0 };
//int err = SimpleDHTErrSuccess;
int pin = PB6;

void setup() {
  Serial.begin(115200);
}

void loop() {
  Serial.println("=================================");
  Serial.println("Sample DHT22 with RAW bits...");
  //byte data[40] = {0};
  /* if ((err = read2(&temperature, &humidity, data)) != SimpleDHTErrSuccess) {
  Serial.print("Read DHT22 failed, err="); Serial.print(SimpleDHTErrCode(err));
  Serial.print(","); Serial.println(SimpleDHTErrDuration(err)); delay(2000);
  return;
  }*/
  read2(&temperature, &humidity, data);

  Serial.print("Sample RAW Bits: ");
  for (int i = 0; i < 40; i++) {
    Serial.print((int)data[i]);
    if (i > 0 && ((i + 1) % 4) == 0) {
      Serial.print(' ');
    }
  }
  Serial.println("");
  Serial.print("Sample OK: ");
  Serial.print((float)temperature);
  Serial.print(" *C, ");
  Serial.print((float)humidity);
  Serial.println(" RH%");
  Serial.print("CPU ");
  Serial.print(SystemCoreClock);
  Serial.println(" Hz\n");
  delay(2500);
}
/****************************************************************************************/
int read2(float* ptemperature, float* phumidity, byte pdata[40]) {
  int ret = SimpleDHTErrSuccess;
  byte data[40] = { 0 };
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
int parse(byte data[40], short* ptemperature, short* phumidity) {
  short humidity = bits2byte(data);
  short humidity2 = bits2byte(data + 8);
  short temperature = bits2byte(data + 16);
  short temperature2 = bits2byte(data + 24);
  byte check = bits2byte(data + 32);
  byte expect = (byte)humidity + (byte)humidity2 + (byte)temperature + (byte)temperature2;
  if (check != expect) {
    return SimpleDHTErrDataChecksum;
  }
  *ptemperature = temperature << 8 | temperature2;
  *phumidity = humidity << 8 | humidity2;
  return SimpleDHTErrSuccess;
}
/****************************************************************************************/
byte bits2byte(byte data[8]) {
  byte v = 0;
  for (int i = 0; i < 8; i++) {
    v += data[i] << (7 - i);
  }
  return v;
}
/****************************************************************************************/
int sample(byte data[40]) {
  memset(data, 0, 40);
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
  delayMicroseconds(1000);
  digitalWrite(pin, HIGH);
  pinMode(pin, INPUT);
  delayMicroseconds(40);

  long t = 0;
  if ((t = levelTime(LOW)) < 30) {
    return simpleDHTCombileError(t, SimpleDHTErrStartLow);
  }
  if ((t = levelTime(HIGH)) < 50) {
    return simpleDHTCombileError(t, SimpleDHTErrStartHigh);
  }

  for (int j = 0; j < 40; j++) {
    t = levelTime(LOW);  // 1.
    if (t < 24) {        // specs says: 50us
      return simpleDHTCombileError(t, SimpleDHTErrDataLow);
    }
    // read a bit
    t = levelTime(HIGH);  // 2.
    if (t < 11) {         // specs say: 26us
      return simpleDHTCombileError(t, SimpleDHTErrDataRead);
    }
    data[j] = (t > 40 ? 1 : 0);  // specs: 22-30us -> 0, 70us -> 1
  }
  t = levelTime(LOW);
  if (t < 24) {
    return simpleDHTCombileError(t, SimpleDHTErrDataEOF);
  }
  return SimpleDHTErrSuccess;
}
/****************************************************************************************/
long levelTime(byte level) {
  int firstWait = 10;
  int interval = 6;
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
    loop = (digitalRead(pin) == level);
  }
  return time;
}
/****************************************************************************************/
/*
// Данный код необходим только для теста датчика DHT22 
#include <SimpleDHT.h>// подключаем библиотеку 

//      for DHT22, 
//      VCC: 5V or 3V
//      GND: GND
//      DATA: PB6
int pinDHT22 = PB6;
SimpleDHT22 dht22(pinDHT22);

void setup() {
  Serial.begin(115200);
}

void loop() {
  // start working...
  Serial.println("=================================");
  Serial.println("Sample DHT22...");
  
  // read without samples.
  // @remark We use read2 to get a float data, such as 10.1*C
  //    if user doesn't care about the accurate data, use read to get a byte data, such as 10*C.
  float temperature = 0;
  float humidity = 0;
  int err = SimpleDHTErrSuccess;
  if ((err = dht22.read2(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
    Serial.print("Read DHT22 failed, err="); Serial.print(SimpleDHTErrCode(err));
    Serial.print(","); Serial.println(SimpleDHTErrDuration(err)); delay(2000);
    return;
  }
  
  Serial.print("Sample OK: ");
  Serial.print((float)temperature); Serial.print(" *C, ");
  Serial.print((float)humidity); Serial.println(" RH%");
  
  // DHT22 sampling rate is 0.5HZ.
  delay(2500);
}*/