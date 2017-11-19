
#ifndef wifi_H
#define wifi_H

class WiFi_Class
{
  public:
    bool INIT();              //设置工作模式
    bool Connect();           //连接WiFi
    void SendData();          //发送数据
    void Weather();           //获取天气信息
    bool ConnectToOnenet();   //连接OneNet服务器
    void Control();           //接收控制信息
    void SendAlarm();         //发送报警数据
};
#endif

