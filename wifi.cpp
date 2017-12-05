#include<Arduino.h>
#include"wifi.h"
#include"edp.c"
#include"readSensor.h"
#include"Voice.h"
#include"Infrared.h"
#include"Switch.h"
#include<ArduinoJson.h>
#include"type.c"
#define KEY  "****************************"     //APIkey
#define ID   "*******"                          //设备ID
#define PUSH_ID NULL

unsigned char pkt_type;

Sensor_Class      Sensor1;
Voice_Class       Voice2;
Infrared_Class    Infrared2;
Switch_Class      Switch2;
edp_pkt *pkt;

extern int tem, hum,flag1, flag2,flag3,flag4;
int edp_flag ;                                                                          //onenet连接状态
int temperature, temperature_low_day1, temperature_high_day1, temperature_low_day2,
    temperature_high_day2, temperature_low_day3, temperature_high_day3;                 //实时温度，最低、最高温度
int code, code_day1, code_night1, code_day2, code_night2, code_day3, code_night3;       //实时天气，白天、夜晚天气
int wind_scale_day1, wind_scale_day2, wind_scale_day3;                                  //风力等级
String wind_direction_day1, wind_direction_day2, wind_direction_day3;                   //风向
String date1, date2, date3;                                                             //日期
String data, data_day1, data_day2, data_day3, suggestion;
String dressing, flu, sport, uv;                                                        //生活指数
bool doCmdOk(String data, char *keyword)
{
  bool result = false;
  if (data != "")   //对于tcp连接命令，直接等待第二次回复
  {
    Serial1.println(data);  //发送AT指令
    Serial.print("SEND: ");
    Serial.println(data);
  }
  if (data == "AT")   //检查模块存在
    delay(100);
  else
    while (!Serial1.available());  // 等待模块回复
  delay(100);
  if (Serial1.find(keyword))   //返回值判断
  {
    Serial.println("do cmd OK");
    result = true;
  }
  else
  {
    Serial.println("do cmd ERROR");
    if (data == "AT")
      Serial1.print("+++");//退出透传
    result = false;
  }
  while (Serial1.available()) Serial1.read();   //清空串口接收缓存
  delay(100); //指令时间间隔
  return result;
}
void rcvDebug(unsigned char *rcv, int len)
{
  int i;

  Serial.print("rcv len: ");
  Serial.println(len, DEC);
  for (i = 0; i < len; i++)
  {
    Serial.print(rcv[i], HEX);
    Serial.print(" ");
  }
  Serial.println("");
}

bool readEdpPkt(edp_pkt *p)
{
  int tmp;
  if ((tmp = Serial1.readBytes(p->data + p->len, sizeof(p->data))) > 0 )
  {
    rcvDebug(p->data + p->len, tmp);
    p->len += tmp;
  }
  return true;
}

/*
   packetSend
   将待发数据发送至串口，并释放到动态分配的内存
*/
void packetSend(edp_pkt* pkt)
{
  if (pkt != NULL)
  {
    Serial1.write(pkt->data, pkt->len);    //串口发送
    Serial1.flush();
    free(pkt);              //回收内存
  }
}



bool WiFi_Class::INIT()
{
  Serial1.begin(115200);
  while (!doCmdOk("AT", "OK"));
  while (!doCmdOk("AT+CWMODE=3", "OK"));
  Serial.println("WiFi Init OK");
}

bool WiFi_Class::Connect()
{
  while (!doCmdOk("AT+CWJAP=\"****\",\"********\"", "OK"));    //wifi密码
  Serial.println("Connected");
}

void WiFi_Class::Weather()
{
  edp_flag = 0;                      //将连接状态改为0，避免没有连上onenet发送数据

  /*
     获取实时天气信息
  */

  while (!doCmdOk("AT+CIPMODE=1", "OK"));
  while (!doCmdOk("AT+CIPSTART=\"TCP\",\"116.62.81.138\",80", "CONNECT"));
  while (!doCmdOk("AT+CIPSEND", ">"));

  Serial1.println("GET /v3/weather/now.json?key=9l3kb1dgowaobqi4&location=zhengzhou&language=en&unit=c");
  String httpData = Serial1.readString();
  //Serial.println(httpData);
  data = httpData.substring((httpData.indexOf("\"now\":") + 6), httpData.indexOf(",\"last"));
  DynamicJsonBuffer jsonBuffer0;
  JsonObject& root0 = jsonBuffer0.parseObject(data);
  temperature = root0[String("temperature")];
  code = root0[String("code")];
  delay(100);

  /*
     获取第一天全天天气信息
  */

  while (!doCmdOk("AT", "OK"));
  while (!doCmdOk("AT+CIPSTART=\"TCP\",\"116.62.81.138\",80", "CONNECT"));
  while (!doCmdOk("AT+CIPSEND", ">"));
  Serial1.println("GET /v3/weather/daily.json?key=9l3kb1dgowaobqi4&location=zhengzhou&language=en&unit=c&start=0&days=1");
  httpData = Serial1.readString();
  //Serial.println(httpData);
  data_day1 = httpData.substring((httpData.indexOf("\"daily\"") + 9), httpData.indexOf("],\"last"));
  DynamicJsonBuffer jsonBuffer1;
  JsonObject& root1 = jsonBuffer1.parseObject(data_day1);
  date1 = root1[("date")].as<String>();
  temperature_low_day1 = root1[String("low")];
  temperature_high_day1 = root1[String("high")];
  code_day1 = root1[String("code_day")];
  code_night1 = root1[String("code_night")];
  wind_direction_day1 = root1[("wind_direction")].as<String>();
  wind_scale_day1 = root1[String("wind_scale")];
  delay(100);
  /*
     第二天
  */
  while (!doCmdOk("AT", "OK"));
  while (!doCmdOk("AT+CIPSTART=\"TCP\",\"116.62.81.138\",80", "CONNECT"));
  while (!doCmdOk("AT+CIPSEND", ">"));
  Serial1.println("GET /v3/weather/daily.json?key=9l3kb1dgowaobqi4&location=zhengzhou&language=en&unit=c&start=1&days=1");
  httpData = Serial1.readString();
  //Serial.println(httpData);
  data_day2 = httpData.substring((httpData.indexOf("\"daily\"") + 9), httpData.indexOf("],\"last"));
  DynamicJsonBuffer jsonBuffer2;
  JsonObject& root2 = jsonBuffer2.parseObject(data_day2);
  date2 = root2[("date")].as<String>();
  temperature_low_day2 = root2[String("low")];
  temperature_high_day2 = root2[String("high")];
  code_day2 = root2[String("code_day")];
  code_night2 = root2[String("code_night")];
  wind_direction_day2 = root2[("wind_direction")].as<String>();
  wind_scale_day2 = root2[String("wind_scale")];
  delay(100);
  /*
     第三天
  */
  while (!doCmdOk("AT", "OK"));
  while (!doCmdOk("AT+CIPSTART=\"TCP\",\"116.62.81.138\",80", "CONNECT"));
  while (!doCmdOk("AT+CIPSEND", ">"));
  Serial1.println("GET /v3/weather/daily.json?key=9l3kb1dgowaobqi4&location=zhengzhou&language=en&unit=c&start=2&days=1");
  httpData = Serial1.readString();
  data_day3 = httpData.substring((httpData.indexOf("\"daily\"") + 9), httpData.indexOf("],\"last"));
  DynamicJsonBuffer jsonBuffer3;
  JsonObject& root3 = jsonBuffer3.parseObject(data_day3);
  date3 = root3[("date")].as<String>();
  temperature_low_day3 = root3[String("low")];
  temperature_high_day3 = root3[String("high")];
  code_day3 = root3[String("code_day")];
  code_night3 = root3[String("code_night")];
  wind_direction_day3 = root3[("wind_direction")].as<String>();
  wind_scale_day3 = root3[String("wind_scale")];
  delay(100);

  /*
     获取生活指数
  */
  while (!doCmdOk("AT", "OK"));
  while (!doCmdOk("AT+CIPSTART=\"TCP\",\"116.62.81.138\",80", "CONNECT"));
  while (!doCmdOk("AT+CIPSEND", ">"));
  Serial1.println("GET /v3/life/suggestion.json?key=9l3kb1dgowaobqi4&location=zhengzhou&language=en");
  httpData = Serial1.readString();
  String suggestion3 = httpData.substring((httpData.indexOf("\"uv\"") + 5), (httpData.indexOf("\"last") - 2));
  DynamicJsonBuffer jsonBuffer7;
  JsonObject& root7 = jsonBuffer7.parseObject(suggestion3);
  uv = root7[("brief")].as<String>();
  Serial.println(uv);

  suggestion = httpData.substring((httpData.indexOf("\"dressing\"") + 11), (httpData.indexOf("\"},\"flu") + 2));
  DynamicJsonBuffer jsonBuffer4;
  JsonObject& root4 = jsonBuffer4.parseObject(suggestion);
  dressing = root4[("brief")].as<String>();
  Serial.println(dressing);

  String suggestion2 = httpData.substring((httpData.indexOf("\"sport\"") + 8), (httpData.indexOf("\"},\"travel") + 2));
  DynamicJsonBuffer jsonBuffer6;
  JsonObject& root6 = jsonBuffer6.parseObject(suggestion2);
  sport = root6[("brief")].as<String>();
  Serial.println(sport);

  suggestion = httpData.substring((httpData.indexOf("\"flu\"") + 6), (httpData.indexOf("\"},\"sport") + 2));
  DynamicJsonBuffer jsonBuffer5;
  JsonObject& root5 = jsonBuffer5.parseObject(suggestion);
  flu = root5[("brief")].as<String>();
  Serial.println(flu);
}

void WiFi_Class::SendData()
{
  char* data = "";
  if (edp_flag)
  {
    sprintf(data, "%d", tem);
    packetSend(packetDataSaveTrans(PUSH_ID, "temperature", data ));
    delay(100);
    sprintf(data, "%d", hum);
    packetSend(packetDataSaveTrans(PUSH_ID, "humidity", data));
    delay(100);
    sprintf(data, "%d", Sensor1.GET_MQ2());
    packetSend(packetDataSaveTrans(PUSH_ID, "smoke", data ));
    delay(100);
    sprintf(data, "%d", Sensor1.GET_LIGHT());
    packetSend(packetDataSaveTrans(PUSH_ID, "light", data));
    delay(100);
    sprintf(data, "%d", Sensor1.GET_FIRE());
    packetSend(packetDataSaveTrans(PUSH_ID, "fire", data));
    delay(100);
    sprintf(data, "%d", Sensor1.GET_SR501());
    packetSend(packetDataSaveTrans(PUSH_ID, "SR501", data));
    Serial.println("SEND OK");
  }
  else
  {
    ConnectToOnenet();              //重新连接平台
  }
}

bool WiFi_Class::ConnectToOnenet()
{
  edp_pkt rcv_pkt;
  unsigned char pkt_type;
  int i, tmp;
  while (!doCmdOk("AT", "OK"));
  while (!doCmdOk("AT+CIPSTART=\"TCP\",\"183.230.40.39\",876", "CONNECT"));
  while (!doCmdOk("AT+CIPMODE=1", "OK"));           //透传模式
  while (!doCmdOk("AT+CIPSEND", ">"));              //开始发送
  edp_flag = 0;
  if (!edp_flag)
  {
    while (Serial1.available()) Serial1.read(); //清空串口接收缓存
    packetSend(packetConnect(ID, KEY));             //发送EPD连接包
    while (!Serial1.available());                 //等待EDP连接应答
    if ((tmp = Serial1.readBytes(rcv_pkt.data, sizeof(rcv_pkt.data))) > 0 )
    {
      rcvDebug(rcv_pkt.data, tmp);
      if (rcv_pkt.data[0] == 0x20 && rcv_pkt.data[2] == 0x00 && rcv_pkt.data[3] == 0x00)
      {
        edp_flag = 1;
        Serial.println("EDP connected.");
      }
      else
        Serial.println("EDP connect error.");
    }
    packetClear(&rcv_pkt);
  }
}

void WiFi_Class::Control()
{
  edp_pkt rcv_pkt;
  while (Serial1.available())
  {
    readEdpPkt(&rcv_pkt);
    if (isEdpPkt(&rcv_pkt))
    {
      pkt_type = rcv_pkt.data[0];
      switch (pkt_type)
      {
        case CMDREQ:
          char edp_command[50];
          char edp_cmd_id[40];
          long id_len, cmd_len, rm_len;
          char datastr[20];
          char val[10];
          memset(edp_command, 0, sizeof(edp_command));
          memset(edp_cmd_id, 0, sizeof(edp_cmd_id));
          edpCommandReqParse(&rcv_pkt, edp_cmd_id, edp_command, &rm_len, &id_len, &cmd_len);  //解析数据
          sscanf(edp_command, "%[^:]:%s", datastr, val);              //格式化读取
          if (strcmp(datastr, "SwitchLED") == 0)                      //判断是哪个指令
          {
            if (atoi(val) == 1)
            {
              Switch2.Control(9, 1);                  // 打开LED
              Voice2.play(10, 7);
            }
            if (atoi(val) == 0)
            {
              Switch2.Control(9, 0);                  // 关闭LED
              Voice2.play(10, 8);
            }
          }
          if (strcmp(datastr, "SwitchPIN") == 0)
          {
            if (atoi(val) == 1)
            {
              Switch2.Control(10, 1);                   // 打开
              Voice2.play(10, 10);
            }
            if (atoi(val) == 0)
            {
              Switch2.Control(10, 0);                    // 关闭
              Voice2.play(10, 9);
            }
          }
          if (strcmp(datastr, "Conditioner") == 0)  //   远程控制空调
          {
            switch (atoi(val))
            {
              case 24:
                Infrared2.Send();
            }
          }
          if (strcmp(datastr, "Control1") == 0)  //   回家模式
          {
            if (atoi(val) == 1)
            {
              flag1 = 0;
              Voice2.play(10, 5);
              flag2 = 1;                       //重置，让下一次开启时播放提示语音
              flag3 = 1;
              flag4 = 0;
              //Infrared2.Send()               //这里可以选择是否自动开启空调
            }
          }
          if (strcmp(datastr, "Control2") == 0)  //  离家模式
          {
            if (atoi(val) == 1)
            {
              flag1 = 1;
            }
          }
          Serial.print(datastr); Serial.println(val);
          packetSend(packetDataSaveTrans(NULL, datastr, val)); //将新数据值上传至数据流
          break;
        default:
          Serial.print("unknown type: ");
          Serial.println(pkt_type, HEX);
          break;
      }
    }
  }
  if (rcv_pkt.len > 0)
    packetClear(&rcv_pkt);
  delay(150);
}



void WiFi_Class::SendAlarm()
{
  packetSend(packetDataSaveTrans(PUSH_ID, "alarm", "1"));
}

