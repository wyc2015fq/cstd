# C++中使用MediaInfo库获取视频信息 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年09月22日 16:58:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：38
个人分类：[MediaInfo](https://blog.csdn.net/leixiaohua1020/article/category/1645587)










MediaInfo 用来分析视频和音频文件的编码和内容信息,是一款是自由软件 (免费使用、免费获得源代码）。

我在项目软件中集成了它的DLL，发现真的是非常好用！

下面简单记录一下它的使用方法。

（系统开发平台是VC2010）

1.将下载下来的MediaInfo.dll拷贝到项目里面

2.拷贝MediaInfoDLL.h到项目目录

3.CPP文件中添加头文件和命名空间



```cpp
#include "MediaInfoDLL.h" //Dynamicly-loaded library (.dll or .so)
using namespace MediaInfoDLL;
```

4.使用的时候声明一个MediaInfo对象就可以了



例如，获得视频的宽和高，用Get()：



```cpp
MediaInfo MI;
CString width,height;
MI.Open("test.flv");
width = MI.Get(stream_t::Stream_Video,0,"Width").c_str();
height = MI.Get(stream_t::Stream_Video,0,"Height").c_str();
MI.Close();
```





这里需要注意的是：width，height都是字符串，使用的时候需要转换



获得视频的完整信息，用Inform()：



```cpp
MediaInfo MI;
CString all;
MI.Open("test.flv");
MI.Option("Complete");
all= MI.Inform().c_str();
MI.Close();
```



下载地址：[http://download.csdn.net/detail/leixiaohua1020/6371889](http://download.csdn.net/detail/leixiaohua1020/6371889)






