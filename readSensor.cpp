/*
   传感器数据的初始化和读取，其中烟雾和光照强度是读取模拟量
*/

#include <Arduino.h>
#include<dht11.h>
#include"readSensor.h"
#include"Switch.h"

dht11 DHT11;
int i = 0;
void Sensor_Class::GPIO_INIT(int Dht11PIN , int LightPIN, int FirePIN , int SmokePIN , int SR501PIN )
{
  pinMode(Dht11PIN, INPUT);
  pinMode(LightPIN, INPUT);
  pinMode(FirePIN, INPUT);
  pinMode(SmokePIN, INPUT);
  pinMode(SR501PIN, INPUT);
  Serial.println("Sensor GPIO Init OK");
}

int Sensor_Class:: GET_TEM()
{
  int chk = DHT11.read(2);
  if ((DHT11.temperature < 40)&&(DHT11.temperature>=0))
    return DHT11.temperature;
}

int Sensor_Class:: GET_HUM()
{
  int chk = DHT11.read(2);
  if ((DHT11.humidity <= 80)&&(DHT11.humidity>=30))
    return DHT11.humidity;
}

int Sensor_Class::GET_LIGHT()           //光线越强数值越小
{
  //Serial.print("light:");Serial.println(analogRead(1));
  return analogRead(1);
}

int Sensor_Class::GET_FIRE()            //有火为0
{
  //Serial.print("fire:");Serial.println(digitalRead(3));
  return digitalRead(5);
}

int Sensor_Class::GET_MQ2()
{
  //Serial.print("smoke:");Serial.println(analogRead(2));
  return analogRead(2);
}

int Sensor_Class::GET_SR501()           //有人为1
{
  //Serial.print("sr051:");Serial.println(digitalRead(4));
  return digitalRead(4);
}

