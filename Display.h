/*
   屏幕显示
*/
#ifndef Display_H
#define Display_H

class Display_Class
{
  public:
    void INIT();
    void showBegin();         //显示启动界面
    void showWeather();       //显示天气
    void showTemHum();        //显示室内温湿度
    void showWiFiKeyboard();  //显示wifi设置键盘
    void showAddKeyboard();   //显示地址设置键盘
    void showTime();          //显示时钟
    void showSetting();       //显示设置界面
    void showControl();       //显示控制界面
    void Control();           //屏幕控制
    void showData();          //显示传感器的趋势
};
#endif
