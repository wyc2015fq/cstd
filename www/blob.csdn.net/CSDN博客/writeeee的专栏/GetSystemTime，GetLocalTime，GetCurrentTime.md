# GetSystemTime，GetLocalTime，GetCurrentTime - writeeee的专栏 - CSDN博客
2016年05月17日 11:47:56[writeeee](https://me.csdn.net/writeeee)阅读数：296
MFC下，常用的获取时间方式有以下几种三种：GetSystemTime，GetLocalTime，GetCurrentTime。
```
SYSTEMTIME sysTm, locTm;
 GetSystemTime(&sysTm);   //获取格林威治标准时间，与北京时间相差8小时
 GetLocalTime(&locTm);  //获取本时区时间。如中国即为东八区北京时间
 CTime tm = CTime::GetCurrentTime();//获取当前时间与1970年1月1日8am的秒数差。
```
