/*
   传感器
*/
#ifndef readSensor_H
#define readSensor_H

class Sensor_Class
{
  public:
    void GPIO_INIT(int, int, int , int, int); //初始化传感器接口
    int GET_TEM();  //获取温度
    int GET_HUM();  //获取湿度
    int GET_LIGHT();//获取光照强度
    int GET_FIRE(); //监测火焰
    int GET_MQ2();//获取可燃气体浓度
    int GET_SR501();//人体红外模块
};
#endif
