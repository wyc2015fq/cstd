# 从Native到Web(四), NaCl学习笔记: 物理引擎 - 逍遥剑客 - CSDN博客
2012年11月18日 20:00:29[xoyojank](https://me.csdn.net/xoyojank)阅读数：4468

## 物理引擎的抉择
前面说了, 这一系列是针对c++&3D的, 所以排除Box2D之类的2D引擎.
3D版的, 目前最流行的有3个:
- Havok: 说它是最NB的毫不为过. 但是要钱, 不给钱人家才不鸟你呢. 免费版本只有Win32, 所以排除
- PhysX: 目前占有率最高, 跟游戏开发商合作比较好. NaCl版本没看到有, 所以排除
- Bullet: 开源. 想跨平台, 还是开源的东西比较靠谱, 一切尽在掌握. 已经有前人做了一些NaCl的移植工作了
这个是Bullet的NaCl版本演示: http://bulletphysics.org/nacl/
代码在这里: https://github.com/erwincoumans/experiments.git
依赖的版本比较老了, 所以需要取pepper_15版本:
![](https://img-my.csdn.net/uploads/201211/18/1353240292_3247.png)
看到这个我就想骂娘了. 你说你开个斯巴达就开吧, 干嘛不让我们写程序啊~
不说了, 你们懂的, 会被和谐的
所以用另找一个吧. 在Chrome Store里找到另一个版本: 
![](https://img-my.csdn.net/uploads/201211/19/1353340696_1568.png)
从说明里找到代码: https://github.com/johnmccutchan/NaClAMBase.git
有兴趣的可以看看这个东西的视频讲解: [Native Client Acceleration Modules](http://www.youtube.com/watch?v=jGfwXMhX8P4), HTML5+NaCl
我们这里只关心Bullet怎么在NaCl里使用
## 编译运行
- 从github上Clone下来
![](https://img-my.csdn.net/uploads/201211/20/1353340947_2317.png)
- 打开NaClAMBase/NaClAM.sln
- Configuration里没有PPAPI这个平台, 没法用VS调试, 所以加一种配置
![](https://img-my.csdn.net/uploads/201211/20/1353341823_9753.png)
- 把NaClAMBullet设置为启动工程, 另外几个改成lib编译, 并加入它的依赖
![](https://img-my.csdn.net/uploads/201211/20/1353341856_4610.png)
- 参考NaCl64的配置把include目录设置一下
- 编译发现<sys/time.h>在PPAPI下找不到, 所以写个替代品:
```cpp
#if defined(_MSC_VER) || defined(_WINDOWS_)
#include <windows.h>
   #include <time.h>
#else
   #include <sys/time.h>
#endif 
#if defined(_MSC_VER) || defined(_WINDOWS_)
   inline int gettimeofday(struct timeval* tv, void*) 
   {
      union {
         long long ns100;
         FILETIME ft;
      } now;
     
      GetSystemTimeAsFileTime (&now.ft);
      tv->tv_usec = (long) ((now.ns100 / 10LL) % 1000000LL);
      tv->tv_sec = (long) ((now.ns100 - 116444736000000000LL) / 10000000LL);
     return (0);
   }
#endif
```
- OK, 调试运行:
![](https://img-my.csdn.net/uploads/201211/20/1353341863_7920.jpg)
## 总结
这个DEMO的渲染部分是使用HTML5做的, 页面交互与C++这边的通信则是通过Json, 是一个不错的案例
这种思路比较值得借鉴: 高性能的部分使用NaCl, 表现部分使用Web开发人员更熟悉的HTML5
不过, 想要渲染性能高, 还是把3D部分也用NaCl实现的好
另外, VS2010的属性表配置真是方便, 安装过NaCl的插件后, 在c:\Users\[USERNAME]\AppData\Local\Microsoft\MSBuild\v4.0\下会有NaCl32, NaCl64, PPAPI的配置. 所以, 把现有的C++工程转换成NaCl的版本, 只需要增加对应的配置就可以了, 十分方便
