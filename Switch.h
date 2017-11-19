/*
   继电器模块
*/
#ifndef Switch_H
#define Switch_H

class Switch_Class
{
  public:
    void GPIO_INIT(int, int); //初始化继电器接口
    void Control(int, int) ;  //控制继电器
};
#endif
