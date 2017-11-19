#include<Arduino.h>
#include <IRremote.h>
#include"Infrared.h"
#include"Switch.h"
#include"Voice.h"
#include"Display.h"

#define RECV_PIN      11           //接收引脚
#define Key0          16738425     //对应的按键值
#define Key1          16724175
#define Key2          16718055
#define Key3          16743045
#define Key4          16716015
#define Key5          16726215
#define Key6          16734885
#define Key7          16728765
#define Key8          16730805
#define Key9          16732845
#define UP            16748655
#define Reduce        16754775
IRsend irsend;
decode_results results;
IRrecv irrecv(RECV_PIN);
Switch_Class Switch1;
Voice_Class Voice1;
Display_Class Display1;

int flag1, flag2 = 1, flag3=1,flag4;

void Infrared_Class::GPIO_INIT( )
{
  irrecv.enableIRIn();
  Serial.println("Infrared Init OK");
}

void Infrared_Class::Send( )
{
  Serial.println("Infrared Init OK");
}

int Infrared_Class::Control()
{
  if (irrecv.decode(&results))
  {
    if (results.value == Key1 )
      Switch1.Control(9, 1);
    if (results.value == Key2)
      Switch1.Control(9, 0);
    if (results.value == Key3)
    {
      Voice1.playTemHum();
      Display1.showTemHum();
    }
    if (results.value == Key4)
    {
      Display1.showWeather();
      Voice1.playWeather();
    }
    if (results.value == Key5)
    {
      flag1 = 1;
    }
    if (results.value == Key6)
    {
      flag1 = 0;
      Voice1.play(10, 5);
      flag2 = 1;                       //重置，让下一次开启时播放提示语音
      flag3 = 1;
      flag4 = 0;
    }
    if (results.value == Key7)
      Switch1.Control(10, 1);
    if (results.value == Key8)
      Switch1.Control(10, 0);
    if (results.value == Key9)
    {
      //Voice
    }
    if (results.value == UP)
      Voice1.VolumeUP();
    if (results.value == Reduce)
      Voice1.VolumeReduce();
    Serial.println(results.value);
    irrecv.resume();                    // 接收下一次数值
  }
}
