#include<Arduino.h>
/*
   定义引脚，其中LightPIN和SmokePIN都是读取模拟值，接A1、A2 ，红外接收端
   的引脚只是在这说明，修改请前往Infrared.c
*/
#define LightPIN      1
#define SmokePIN      2
#define Dht11PIN      2
#define SR501PIN      4
#define FirePIN       5
#define SwitchLED     9
#define SwitchPIN     10
#define RECV_PIN      11

struct eeprom_write{
  int Length;
  char data [15];
};

