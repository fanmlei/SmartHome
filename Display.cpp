#include<Arduino.h>
#include "Display.h"
#include "readSensor.h"
#include "Voice.h"
#include "Time.h"
#include "Switch.h"
#include "readSensor.h"
#include <EEPROM.h>
#include <Wire.h>

extern int tem , hum;                                                                          //室内温湿度
extern int temperature, temperature_low_day1, temperature_high_day1, temperature_low_day2,
       temperature_high_day2, temperature_low_day3, temperature_high_day3;                 //实时温度，最低、最高温度
extern int code, code_day1, code_night1, code_day2, code_night2, code_day3, code_night3;       //实时天气，白天、夜晚天气
extern int wind_scale_day1, wind_scale_day2, wind_scale_day3;                                  //风力等级
extern String wind_direction_day1, wind_direction_day2, wind_direction_day3;                   //风向
extern String date1, date2, date3;                                                             //日期
extern int year, month, day, hour, minute;
extern int SwitchLED_flag, Switch_flag;
extern int flag1, flag2, flag3, flag4;

Voice_Class Voice3;
Time_Class Time1;
Switch_Class Switch3;
Sensor_Class Sensor3;

void sendEnd()  //每次指令发送完之后的结束符
{
  Serial3.write(0xff);
  Serial3.write(0xff);
  Serial3.write(0xff);
}
void Display_Class::INIT()
{
  Serial3.begin(9600);
}

void Display_Class::showBegin()
{
  Serial3.print("page 0");
  sendEnd();
}
/*
   显示天气界面
*/
void Display_Class::showWeather()
{
  /********切换到天气界面********/
  Serial3.print("page 1");
  sendEnd();
  /********切换天气图片*********/
  Serial3.print("picq 0,30,180,210,");
  Serial3.print(code);
  sendEnd();
  /*******实时天气状况*******/
  Serial3.print("va0.val=");
  Serial3.print(code);
  sendEnd();
  Serial3.print("click t2,1");
  sendEnd();
  /********实时温度*******/
  Serial3.print("t0.txt=\"");
  Serial3.print(temperature);
  Serial3.print("\"");
  sendEnd();
  Serial3.print("vis t1,1");
  sendEnd();
  /********滚动显示当天详细天气信息*******/
  Serial3.print("g0.txt=\"\"");
  sendEnd();
  Serial3.print("g0.txt=\"");
  Serial3.print(temperature_low_day1);
  Serial3.print("-");
  Serial3.print(temperature_high_day1);
  Serial3.print("\"");
  sendEnd();
  Serial3.print("va1.val=");
  Serial3.print(code_day1);
  sendEnd();
  Serial3.print("va2.val=");
  Serial3.print(code_night1);
  sendEnd();
  Serial3.print("va3.txt=\"");
  Serial3.print(wind_direction_day1);
  Serial3.print("\"");
  sendEnd();
  Serial3.print("va4.val=");
  Serial3.print(wind_scale_day1);
  sendEnd();
  Serial3.print("click g0,1");
  sendEnd();
  /********第一天日期显示*********/
  Serial3.print("t13.txt=\"");
  Serial3.print(date1);
  Serial3.print("\"");
  sendEnd();
  Serial3.print("vis t14,1");
  sendEnd();
  /********第二天日期显示*********/
  Serial3.print("t5.txt=\"");
  Serial3.print(date2);
  Serial3.print("\"");
  sendEnd();
  /********第三天日期显示*********/
  Serial3.print("t9.txt=\"");
  Serial3.print(date3);
  Serial3.print("\"");
  sendEnd();
  /*********第二天最低最高温度********/
  Serial3.print("t6.txt=\"");
  Serial3.print(temperature_low_day2);
  Serial3.print("-");
  Serial3.print(temperature_high_day2);
  Serial3.print("\"");
  sendEnd();
  Serial3.print("click t6,1");
  sendEnd();
  /********第三天的最低最高温度*******/
  Serial3.print("t10.txt=\"");
  Serial3.print(temperature_low_day3);
  Serial3.print("-");
  Serial3.print(temperature_high_day3);
  Serial3.print("\"");
  sendEnd();
  Serial3.print("click t10,1");
  sendEnd();
  /********第二天天气状况*********/
  Serial3.print("va5.val=");
  Serial3.print(code_day2);
  sendEnd();
  Serial3.print("click t7,1");
  sendEnd();
  Serial3.print("va6.txt=\"");
  Serial3.print(wind_direction_day2);
  Serial3.print("\"");
  sendEnd();
  Serial3.print("va7.val=");
  Serial3.print(wind_scale_day2);
  sendEnd();
  Serial3.print("click t8,1");
  sendEnd();
  /********第三天天气状况********/
  Serial3.print("va8.val=");
  Serial3.print(code_day1);
  sendEnd();
  Serial3.print("click t11,1");
  sendEnd();
  Serial3.print("va9.txt=\"");
  Serial3.print(wind_direction_day3);
  Serial3.print("\"");
  sendEnd();
  Serial3.print("va10.val=");
  Serial3.print(wind_scale_day3);
  sendEnd();
  Serial3.print("click t12,1");
  sendEnd();
}

/*
   显示温湿度界面
*/
void Display_Class::showTemHum()
{

  Serial3.print("page 3");    //切换到温度显示界面
  sendEnd();
  if (tem > 0 && tem <= 7)            //更改温度指针,由于角度值不能为小数，为了减少误差采取分段
  {
    Serial3.print("z0.val=");
    Serial3.print(360 - 7 * tem);
    sendEnd();
  }
  else if (tem > 7 && tem <= 20)
  {
    Serial3.print("z0.val=");
    Serial3.print(7 * (tem - 7));
    sendEnd();
  }
  else if (tem > 20 && tem <= 33)
  {
    Serial3.print("z0.val=");
    Serial3.print(90 + 7 * (tem - 20));
    sendEnd();
  }
  else if (tem > 33)
  {
    Serial3.print("z0.val=");
    Serial3.print(180 + 7 * (tem - 7));
    sendEnd();
  }
  Serial3.print("n0.val=");  //更改码盘下面数字
  Serial3.print(tem);
  sendEnd();
  Serial3.print("click t0,1");
  sendEnd();
  if (hum <= 39)                //更改湿度指针
  {
    Serial3.print("z1.val=");
    Serial3.print(360 - 5 * (hum - 30));
    sendEnd();
  }
  else if (hum > 39 && hum <= 55)
  {
    Serial3.print("z1.val=");
    Serial3.print(5 * (hum - 39));
    sendEnd();
  }
  else if (hum > 55 && hum <= 71)
  {
    Serial3.print("z1.val=");
    Serial3.print(90 + 5 * (hum - 55));
    sendEnd();
  }
  else if (hum > 71)
  {
    Serial3.print("z1.val=");
    Serial3.print(180 + 5 * (hum - 71));
    sendEnd();
  }
  Serial3.print("n1.val=");  //更改码盘下面数字
  Serial3.print(hum);
  sendEnd();
  Serial3.print("click t1,1");
  sendEnd();
  Serial3.print("add 7,3,1");
  sendEnd();
}

/*
   显示键盘页面
*/
void Display_Class::showWiFiKeyboard()
{
  Serial3.print("page 2");
  sendEnd();
}

void Display_Class::showAddKeyboard()
{
  Serial3.print("page 9");
  sendEnd();
}

void Display_Class::showTime()
{
  Time1.getTime();
  Serial3.print("page 4");
  sendEnd();
  Serial3.print("t1.txt=\"");
  Serial3.print(year);
  Serial3.print("-");
  Serial3.print(month);
  Serial3.print("-");
  Serial3.print(day);
  Serial3.print("\"");
  sendEnd();
  Serial3.print("n0.val=");
  Serial3.print(hour);
  sendEnd();
  Serial3.print("n1.val=");
  Serial3.print(minute);
  sendEnd();
}

void Display_Class::showData()
{
  for (int i = 0 ; i < 400; i++)
  {
    Serial3.print("add 1,0,");
    Serial3.print(analogRead(2));
    sendEnd();
    Serial3.print("add 1,1,");
    Serial3.print((analogRead(1) / 6));
    sendEnd();
    Serial3.print("add 1,2,");
    Serial3.print((digitalRead(1) * 100));
    sendEnd();
    Serial3.print("add 1,3,");
    Serial3.print((digitalRead(4) * 50));
    sendEnd();
  }
}
void Display_Class::Control()
{
  while (Serial3.available() > 2)
  {
    String dp = Serial3.readString();  //判断页面属性，刷新对应的页面
    Serial.println(dp);
    if (dp.indexOf("Weather") == 1)
      showWeather();
    if (dp.indexOf("Time") == 1)
      showTime();
    if (dp.indexOf("Tem") == 1)
      showTemHum();
    if (dp.indexOf("Setting") == 1)
    {
      showSetting();
      while (1)
      {
        if (Serial3.available() > 0)
        {
          int volume = Serial3.read();      //通过滑块来控制音量大小
          Voice3.setvolume(volume);      //设置音量大小
          Serial.println(EEPROM.read(0));
          if (volume>50)
            break;
        }
      }
    }
    if (dp.indexOf("Data") == 1)
      showData();
    if (dp.indexOf("Keyboard") == 1)
    {
      while (1)
      {
        if (Serial3.available() > 0)
        {
          dp = Serial3.readString();
          if (dp.indexOf("Menu") == 1)
            break;
        }
      }
    }
    if (dp.indexOf("Control") == 1)
    {
      showControl();
      while (1)
      {
        if (Serial3.available() > 0)
        {
          int Switch = Serial3.read();
          Serial.println(Switch);
          if (Switch == 10)
            Switch3.Control(9, 0);      //关灯
          if (Switch == 11)
            Switch3.Control(9, 1);      //开灯
          if (Switch == 20)
            Switch3.Control(10, 0);     //关闭
          if (Switch == 21)
            Switch3.Control(10, 1);     //打开
          if (Switch == 31)
            flag1 = 1;                   //打开离家模式
          if (Switch == 30)
          {
            flag1 = 0;                   //关闭离家模式
            flag2 = 1;
            flag3 = 1;
            flag4 = 0;
            Voice3.play(10, 5);
          }
          if (Switch > 50)
            break;
        }
      }
    }
  }
}

void Display_Class::showSetting()
{
  Serial3.print("page 6");
  sendEnd();
  Serial3.print("h1.val=");         //更新音量滑块的键值
  Serial3.print(EEPROM.read(1));
  sendEnd();
  Serial3.print("n1.val=");
  Serial3.print(EEPROM.read(1));
  sendEnd();
}

void Display_Class::showControl()
{
  Serial3.print("page 7");
  sendEnd();
  if (SwitchLED_flag == 1)                     //更新按键状态,由于flag和按键状态相反所以分别处理
  {
    Serial3.print("bt0.val=0");
    sendEnd();
  }
  if (SwitchLED_flag == 0)
  {
    Serial3.print("bt0.val=1");
    sendEnd();
  }
  if (Switch_flag == 1)
  {
    Serial3.print("bt1.val=0");
    sendEnd();
  }
  if (Switch_flag == 0)
  {
    Serial3.print("bt1.val=1");
    sendEnd();
  }
  if (SwitchLED_flag == 1)
  {
    Serial3.print("bt0.val=0");
    sendEnd();
  }
  if (flag1 == 0)
  {
    Serial3.print("bt2.val=1");
    sendEnd();
  }
  if (flag1 == 1)
  {
    Serial3.print("bt2.val=0");
    sendEnd();
  }
}

