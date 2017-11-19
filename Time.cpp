#include<Arduino.h>
#include"Time.h"
#include <Wire.h>
#include "DS3231.h"

RTClib RTC;
DS3231 DT;

int year, month, day, hour, minute;
void Time_Class::INIT()
{
  Wire.begin();
  Serial.println("DS3231 Init OK");
}
int Time_Class::getTime()
{
  DateTime now = RTC.now();
  return year = now.year(), month = now.month(), day = now.day(), hour = now.hour(), minute = now.minute();
}
void Time_Class::setTime(int Minute, int Hour, int DoW, int Day, int Month, int Year, bool Mode)
{
  DT.setMinute(Minute);
  DT.setHour(Hour);
  DT.setDoW(DoW);
  DT.setDate(Day);
  DT.setMonth(Month);
  DT.setYear(Year);
  DT.setClockMode(Mode);      //flase为24小时制 true 为12小时制
}

