/*
   继电器的初始化和控制
*/
#include<Arduino.h>
#include"Switch.h"

int SwitchLED_flag, Switch_flag;
void Switch_Class::GPIO_INIT(int SwitchPIN1, int SwitchPIN2)
{
  pinMode(SwitchPIN1, OUTPUT);
  pinMode(SwitchPIN2, OUTPUT);
  Serial.println("Switch GPIO Init OK");
}

void Switch_Class::Control(int SwitchPIN, int value)
{
  digitalWrite(SwitchPIN, value);
  if (SwitchPIN == 9)
    SwitchLED_flag = value;
  if (SwitchPIN == 10)
    Switch_flag = value;
}

