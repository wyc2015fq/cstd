# 40、[ iOS ] 真机、模拟机判断 - weixin_33985507的博客 - CSDN博客
2016年05月20日 10:49:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：6
```
1、宏定义
//------
#if TARGET_IPHONE_SIMULATOR
#define SIMULATOR 1
#elif TARGET_OS_IPHONE
#define SIMULATOR 0
#endif
//-----使用
    //-----0 真机   1 模拟机 
    if (SIMULATOR==0) {
                
     };
```
