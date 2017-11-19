#include "readSensor.h"
#include "Switch.h"
#include "Infrared.h"
#include "WiFi.h"
#include "Voice.h"
#include "Display.h"
#include "Time.h"
#include "system.c"
//#include<MsTimer2.h>

Sensor_Class Sensor;
Switch_Class Switch;
Infrared_Class Infrared;
Voice_Class Voice;
WiFi_Class wifi;
Display_Class Display;
Time_Class Time;
/*
   flag1、flag2、flag3在Infrared.c 中定义
*/
extern int flag1, flag2, flag3, flag4;
int tem, hum;
unsigned long time_SendData, time_now, time_Refresh;
void setup()
{
//  MsTimer2::set(500,  Refresh);
//  MsTimer2::start();
  Serial.begin(9600);                                                  //设置波特率
  /*
     系统接口初始化工作
  */
  Display.INIT();
  Display.showBegin();
  Sensor.GPIO_INIT(Dht11PIN , LightPIN, FirePIN , SmokePIN , SR501PIN); //接口初始化
  Switch.GPIO_INIT(SwitchLED, SwitchPIN);
  Infrared.GPIO_INIT();
  Voice.INIT();
  Time.INIT();
  delay(1000);
  Display.showTime();
  /*
     网络部分初始化
  */
  wifi.INIT();
  wifi.Weather();
  wifi.ConnectToOnenet();
  time_Refresh = time_SendData = millis();
  Voice.play(9, 0);
}
void loop()
{
  Refresh();
  Send_data();
  autoControl();
  Infrared.Control();
  wifi.Control();
  Display.Control();
  goOut();
  
  
}



/*
   根据传感器检测数据自动调节一些设备或发出报警
*/
void autoControl()
{
  tem = Sensor.GET_TEM(), hum = Sensor.GET_HUM();
  if (Sensor.GET_MQ2() > 150)           //可燃气体浓度读取值大于100时，播放报警语音
  {
    Voice.play(10, 0);
    delay(100);
  }
  if (Sensor.GET_FIRE() == 0)           //检测有火焰时播放报警语音
  {
    Voice.play(10, 1);
    delay(100);
  }
  if (Sensor.GET_LIGHT() > 1000)        //光照强度读取值大于1000时自动打开灯光（光线越强读取的模拟值越小）
  {
    Switch.Control(9, 1);
    Voice.play(10, 2);
    delay(100);
  }
  if (Sensor.GET_LIGHT() < 750)         //光照强度读取值小于750时自动关闭灯光
  {
    Switch.Control(9, 0);
    Voice.play(10, 3);
    delay(100);
  }
}

/*
   设置离家模式（设置成功后检测有人出门后就启动防盗功能：
   1：语音报警
   2：向平台发送数据，激活触发器
   3：可通过遥控器关闭报警语音
*/
void goOut()
{
  if (flag1)                  //通过遥控器或远程开启关闭功能
  {
    if (flag2)                 //flag2的作用只是为了只播放一遍提示语音
    {
      Voice.play(10, 4);
      flag2 = 0;
    }
    if (flag3)
    {
      if (Sensor.GET_SR501())
      {
        delay(2000);
        if (Sensor.GET_SR501())
        {
          flag3 = 0;
          flag4 = 1;
          Switch.Control(9, 0);
          Switch.Control(10, 0);
          Voice.play(10, 12);
          Voice.playSuggestion();
        }
      }
    }
    if (flag4)
    {
      if (Sensor.GET_SR501())
      {
        Voice.play(10, 6);
        wifi.SendAlarm();
      }
    }
  }
}

void Refresh()                                  //刷新时钟显示，保持心跳连接
{
    time_now = millis();
    if (time_now - time_Refresh >= 60000)      
    {
      time_Refresh = millis();
      Display.showTime();
      Serial1.write(0xc0);
      Serial1.write(0x00);
    }
}

void Send_data()
{
    time_now = millis();
    if (time_now - time_SendData >= 3000)       //每隔3秒发送一次数据
    {
      time_SendData = millis();
      wifi.SendData();
    }
}

