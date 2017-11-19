/*
   时钟模块
*/
#ifndef Time_H
#define Time_H

class Time_Class
{
  public:
    void  INIT();                                     //初始化
    int   getTime();                                  //获取时间
    void  setTime(int, int, int, int, int, int, bool); //设置时间
};
#endif
