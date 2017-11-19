#include<Arduino.h>
#include <EEPROM.h>
#include"Voice.h"
#include"readSensor.h"
 
unsigned int  buf[7]  = {0x7E, 0x04, 0x44, 0x01, 0x00, 0xEF};
unsigned int  buf_sound[5]  = {0x7E, 0x03, 0x31, 0x1E, 0xEF};
unsigned int  buf_cmd[4] = {0x7E, 0x02, 0x01, 0xEF};
extern int temperature, temperature_low_day1, temperature_high_day1;            //实时温度，最低、最高温度
extern int code, code_day1, code_night1;                                        //实时天气，白天、夜晚天气
extern int wind_scale_day1;                                                     //风力等级
extern String wind_direction_day1;                                              //风向
extern String dressing, flu, sport, uv;
extern int tem, hum;
Sensor_Class Sensor2;

void Voice_Class::INIT()
{
  pinMode(13, INPUT);
  Serial2.begin(9600);
  buf_sound[3] = EEPROM.read(0);
    for (int i = 0; i < 5; i++)
      Serial2.write(buf_sound[i]);                   //读取内存中的数据，并设置音量大小
  Serial.println("Voice Init OK");
}

/*播放当前室内温湿度*/
void Voice_Class::playTemHum()
{
  if (tem < 0)
    buf[4] = (100 - tem);
  else
    buf[4] = tem;
  buf[3] = 0x01;
  if (digitalRead(13) == 1)
  {
    for (int i = 0 ; i < 7 ; i++)
      Serial2.write(buf[i]);
  }
  buf[4] = hum;
  buf[3] = 0x02;
  if (digitalRead(13) == 1)
  {
    for (int i = 0 ; i < 7 ; i++)
      Serial2.write(buf[i]);
  }
}

void Voice_Class::playWeather()
{
  buf[3] = 3;
  buf[4] = code;
  if (digitalRead(13) == 1)
  {
    for (int i = 0 ; i < 7 ; i++)
      Serial2.write(buf[i]);
  }
  buf[3] = 4;
  if (temperature_high_day1 < 0)
    buf[4] = (100 - temperature_high_day1);
  else
    buf[4] = temperature_high_day1;
  if (digitalRead(13) == 1)
  {
    for (int i = 0 ; i < 7 ; i++)
      Serial2.write(buf[i]);
  }
  buf[3] = 5;
  if (temperature_low_day1 < 0)
    buf[4] = (100 - temperature_low_day1);
  else
    buf[4] = temperature_low_day1;
  if (digitalRead(13) == 1)
  {
    for (int i = 0 ; i < 7 ; i++)
      Serial2.write(buf[i]);
  }
  buf[3] = 6;
  if (wind_direction_day1.indexOf("CLM") == 0)
    buf[4] = 0;
  if (wind_direction_day1.indexOf("E") == 0)
    buf[4] = 1;
  if (wind_direction_day1.indexOf("S") == 0)
    buf[4] = 2;
  if (wind_direction_day1.indexOf("W") == 0)
    buf[4] = 3;
  if (wind_direction_day1.indexOf("N") == 0)
    buf[4] = 4;
  if (wind_direction_day1.indexOf("ES") == 0 || wind_direction_day1.indexOf("SE") == 0)
    buf[4] = 5;
  if (wind_direction_day1.indexOf("EN") == 0 || wind_direction_day1.indexOf("NE") == 0)
    buf[4] = 6;
  if (wind_direction_day1.indexOf("WS") == 0 || wind_direction_day1.indexOf("SW") == 0)
    buf[4] = 7;
  if (wind_direction_day1.indexOf("WN") == 0 || wind_direction_day1.indexOf("NW") == 0)
    buf[4] = 8;
  if (digitalRead(13) == 1)
  {
    for (int i = 0 ; i < 7 ; i++)
      Serial2.write(buf[i]);
  }
  buf[3] = 7;
  buf[4] = wind_scale_day1;
  if (digitalRead(13) == 1)
  {
    for (int i = 0 ; i < 7 ; i++)
      Serial2.write(buf[i]);
  }
  buf[3] = 11;
  if (temperature < 0)
    buf[4] = 100 - temperature;
  else
    buf[4] = temperature;
  if (digitalRead(13) == 1)
  {
    for (int i = 0 ; i < 7 ; i++)
      Serial2.write(buf[i]);
  }
}

void Voice_Class::playSuggestion()
{
  /*
     室外温度提醒
  */
  buf[3] = 11;
  if (temperature < 0)
    buf[4] = 100 - temperature;
  else
    buf[4] = temperature;
  if (digitalRead(13) == 1)
  {
    for (int i = 0 ; i < 7 ; i++)
      Serial2.write(buf[i]);
  }
  delay(100);
  /*
     天气建议提醒
  */

  if ((uv.indexOf("High") == 0) || ( uv.indexOf("Very High") == 0))
  {
    buf[3] = 8;
    buf[4] = 2;
    if (digitalRead(13) == 1)
    {
      for (int i = 0 ; i < 7 ; i++)
        Serial2.write(buf[i]);
    }
  }
  if ((flu.indexOf("Probably") == 0) || (flu.indexOf("Certainly") == 0))
  {
    buf[3] = 8;
    buf[4] = 6;
    if (digitalRead(13) == 1)
    {
      for (int i = 0 ; i < 7 ; i++)
        Serial2.write(buf[i]);
    }
  }
  if ((dressing.indexOf("Hot") == 0 ) || (dressing.indexOf("Very Hot") == 0))
  {
    buf[3] = 8;
    buf[4] = 5;
    if (digitalRead(13) == 1)
    {
      for (int i = 0 ; i < 7 ; i++)
        Serial2.write(buf[i]);
    }
  }
  if ((dressing.indexOf("Cold") == 0 ) || (dressing.indexOf("Very Cold") == 0))
  {
    buf[3] = 8;
    buf[4] = 4;
    if (digitalRead(13) == 1)
    {
      for (int i = 0 ; i < 7 ; i++)
        Serial2.write(buf[i]);
    }
  }
  if ((sport.indexOf("Good") == 0 ) || (sport.indexOf("Great") == 0))
  {
    buf[3] = 8;
    buf[4] = 0;
    if (digitalRead(13) == 1)
    {
      for (int i = 0 ; i < 7 ; i++)
        Serial2.write(buf[i]);
    }
  }
  if ((sport.indexOf("Not Good") == 0) || (sport.indexOf("Unfavorable") == 0))
  {
    buf[4] = 1;
    buf[3] = 8;
    if (digitalRead(13) == 1)
    {
      for (int i = 0 ; i < 7 ; i++)
        Serial2.write(buf[i]);
    }
  }
  /*
     温差差距提醒
  */
  if (abs(temperature - Sensor2.GET_TEM()) > 8)
  {
    buf[3] = 8;
    buf[4] = 7;
    if (digitalRead(13) == 1)
    {
      for (int i = 0 ; i < 7 ; i++)
        Serial2.write(buf[i]);
    }
  }
  /*
     雨天提醒
  */

  if ((code >= 10 ) && ( code <= 20))
  {
    buf[3] = 3;
    buf[4] = code;
    if (digitalRead(13) == 1)
    {
      for (int i = 0 ; i < 7 ; i++)
        Serial2.write(buf[i]);
    }
    buf[3] = 8;
    buf[4] = 3;
    if (digitalRead(13) == 1)
    {
      for (int i = 0 ; i < 7 ; i++)
        Serial2.write(buf[i]);
    }
  }
  /*
     雪天提醒
  */

  if ((code >= 21 ) && ( code <= 25))
  {
    buf[3] = 3;
    buf[4] = code;
    if (digitalRead(13) == 1)
    {
      for (int i = 0 ; i < 7 ; i++)
        Serial2.write(buf[i]);
    }
    buf[3] = 8;
    buf[4] = 8;
    if (digitalRead(13) == 1)
    {
      for (int i = 0 ; i < 7 ; i++)
        Serial2.write(buf[i]);
    }
  }
}

  void Voice_Class::play(int m, int n)
  {
    buf[3] = m;
    buf[4] = n;
    if (digitalRead(13) == 1)
    {
      for (int i = 0; i < 7; i++)
        Serial2.write(buf[i]);
    }
  }

  void Voice_Class::setvolume(int volume)
  {
    buf_sound[3] = volume;
    for (int i = 0; i < 5; i++)
      Serial2.write(buf_sound[i]);
    buf[3] = 10;
    buf[4] = 20;
    if (digitalRead(13) == 1)       //播放试听音效
    {
      for (int i = 0; i < 7; i++)
        Serial2.write(buf[i]);
    }
    EEPROM.write(0, volume);      //写入内存中
  }

  int Voice_Class::getvolume()
  {
    //  int s;
    //  for (int i = 0; i < 4; i++)
    //    Serial2.write(buf_volume[i]);
    //  while(Serial2.available()>0)
    //    return Serial2.read();

  }

  void Voice_Class::VolumeUP()
  {
    buf_cmd[2] = 5;
    for (int i = 0; i < 5; i++)
      Serial2.write(buf_cmd[i]);
    buf[3] = 10;
    buf[4] = 20;
    if (digitalRead(13) == 1)       //播放试听音效
    {
      for (int i = 0; i < 7; i++)
        Serial2.write(buf[i]);
    }
  }

  void Voice_Class::VolumeReduce()
  {
    buf_cmd[2] = 6;
    for (int i = 0; i < 5; i++)
      Serial2.write(buf_cmd[i]);
    buf[3] = 10;
    buf[4] = 20;
    if (digitalRead(13) == 1)       //播放试听音效
    {
      for (int i = 0; i < 7; i++)
        Serial2.write(buf[i]);
    }
  }

