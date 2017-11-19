/*
   语音模块
*/
#ifndef Voice_H
#define Voice_H
class Voice_Class
{
  public:
    void INIT();                    //初始化语音模块
    void play(int, int);            //播放指定目录下的音频
    void playTemHum();              //播放室内温湿度
    void playWeather();             //播放天气
    void playSuggestion();          //播放外出建议
    void setvolume(int);            //设置音量大小
    int  getvolume();               //获取音量大小
    void VolumeUP();                //音量增加
    void VolumeReduce();            //音量减少
};
#endif
