/*
   红外控制模块
*/
#ifndef InFrared_H
#define Infrared_H

class Infrared_Class
{
  public:
    void  GPIO_INIT();   //接口的初始化
    int   Control();
    void  Send();
};
#endif

