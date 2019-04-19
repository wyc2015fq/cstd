# C/C++/Qt软件系统统计运行时间方法详解 - fanyun的博客 - CSDN博客
2017年09月30日 21:21:07[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：517
所属专栏：[C/C++/C#开发实战365](https://blog.csdn.net/column/details/c-plus-plus-01.html)
C/C++中有基础库用于实现该功能，功能在time.h头文件中，代码实例如下：
```cpp
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
 
void main()
 {
     double time_Start = (double)clock(); //开始时间
     //操作。。。
     double timr_Finish = (double)clock(); //结束时间
 
     printf("operate time: %.2fms",(time_Finish-time_Start); //输出
 }
```
Qt程序中也可以使用C的方法，当然Qt内部封装了一个时间统计的方法：QTime类
```cpp
#include <QTime>
QTime time;
time.start(); //开始计时，以ms为单位
int time_Diff = time.elapsed(); //返回从上次start()或restart()开始以来的时间差，单位ms
  //以下方法是将ms转为s
float f = time_Diff/1000.0;
QString tr_timeDiff = QString("%1").arg(f); //float->QString
复制代码
其他的很多库都会对时间统计方法进行封装，对于统计运行时间差余以为调用C的方法是最好用的
```
