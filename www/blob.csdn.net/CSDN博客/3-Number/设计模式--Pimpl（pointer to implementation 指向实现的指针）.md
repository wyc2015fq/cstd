
# 设计模式--Pimpl（pointer to implementation 指向实现的指针） - 3-Number - CSDN博客


2017年11月01日 11:50:44[庞叶蒙](https://me.csdn.net/pangyemeng)阅读数：261


# 0x01 缘由
以前学习了一些设计模式，但是实践中使用得较少。用得比较多的是单列模式、代理模式等，这些模式我在其他博文中也有讲解。为什么这些模式没有使用，难道是真的用不到？ 我应该不是。现在重新学习设计模式：设计模式是针对软件设计问题的通用解决方案。从基本的原理、优缺点、什么场景下使用等方面考虑和思考。
设计模式是前辈们提炼和总结的一些设计思路，所以掌握它有利无弊。
# 0x02 介绍
使用Pimpl 惯用法将实现细节从公有头文件中分离出来。当你想给客户设计一个良好的API接口时，你会将大量的实现隐藏，提供给客户的是非常明了的API。
![](https://img-blog.csdn.net/20171101114849592?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGFuZ3llbWVuZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

# 0x03 优缺点
**优点：**
信息隐藏。看下面的例子就可以发现。
降低耦合。头文件的耦合，导致头文件要包含较多文件；
加速编译。
更好的二进制兼容性。
惰性分配。
**缺点：**
难调试、难阅读；
增加对象的创建和分配的开销；
# 0x04 实现
头文件：
```python
#ifndef AUTOTIMER_H_
#define AUTOTIMER_H_
#include <string.h>
class AutoTimer
{
     public:
          explicit AutoTimer(const std::string name);
          ~AutoTimer();
     private:
          class Imple;
          Imple * mimpl;
}；
#endif /* AUTOTIMER_H_ */
```
实现：
```python
#include "autotimer.h"
#include <iostream>
#if _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif
class AutoTimer::Imple
{
     public:
          double GetElapsed() const
          {
              #ifdef _WIN32
              return (GetTickCount() - mSmStartTime) / 1e3;
              #else
              struct timeval mEndTime;
              gettimeofday(&mEndTime, NULL);
              double t1 = mStartTime.tv_usec/1e6 + mStartTime.tv_sec ;
              double t2 = mEndTime.tv_usec/1e6 + mEndTime.tv_sec ;
              return t2 - t1;
              #endif
          }
     std::string mName;
     #ifdef _WIN32
     DWORD mStartTime;
     #else
     struct timeval mStartTime;
     #endif
};
AutoTimer::AutoTimer(const std::string name):mImpl(new AutoTimer::Imple)
{
     mimpl->mName = name;
     #ifdef _WIN32
     mimpl->mStartTime = GetTickCount();
     #else
     gettimeofday(&mimpl->mStartTime, NULL);
     #endif
}
AutoTimer::~AutoTimer()
{
     std::cout << mimpl->mName << ": took" << mimpl->GetElapsed()
              << "secs" << std::endl;
     delete mimpl;
     mimpl = NULL;
}
```

# 0x05 使用场景
给第三方提高API接口。内部各负责模块间提供API接口。
# 0x06 总结
学习到了一种设计对外API的思路。


