# 编译libevent源代码(Windows) - DoubleLi - 博客园






学习笔记，只是记录本次成功用libevent源代码进行编译。环境为MinGW+VS2008+Msys。

#### **0.下载libevent库**

　　http://libevent.org/ 下载stable稳定版的库。

#### **1.编译**

　　一开始我用MinGW进行编译的，但是总是出现问题。后来参考了这个博客：http://m.blog.csdn.net/blog/bojie5744/39698599 ，把vs的运行环境包含进来，然后再进行编译

call "C:\Program Files\Microsoft Visual Studio 9.0\VC\vcvarsall.bat"

　　./configure ; make ; make install;

　　如果不install也可以，libevent头文件在include里面，而生成的库在.libs里面，由于使用的是vs的运行环境。所以libevent源代码编译后的库后缀为lib，而非以前的a文件。复制 libevent.lib;libevent_core.lib;libevent_extra.lib;(最后给出我编译后的文件，方便没有vs编译环境的人下载使用。) 如果为了以后方便，可以把include目录里文件复制到开发环境的include里面，而.libs里面的三个文件也可以拷贝到mingw的lib文件夹里面。

#### **2.libevent测试代码**

```
![复制代码](https://common.cnblogs.com/images/copycode.gif)

1 #include <winsock2.h>
2 #include <event2/event.h>
3 #include <event2/event_struct.h> 
4 
5 int main(int argc, char **argv)
6 {
7     event timeout;
8     return (0);
9 }

![复制代码](https://common.cnblogs.com/images/copycode.gif)
```

　　编译选项为

　　g++ test.cpp -Iinclude -L. -levent -lws2_32

　　注意测试的编译还是不通过的。要把libevent.lib 文件名改为event.lib就可以通过了，具体为什么就还不清楚。至于为什么MS系的lib库和mingw的的a库文件有什么区别，在Windows下mingw高级版本，两者是没有区别的，都是可以调用使用的。

#### **3.libevent自带的http服务器源码**(在sample中的http-server.c)
`![](https://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif) View Code`
　　g++ http-server.c -Iinclude -L. -levent -lws2_32

![](https://images0.cnblogs.com/blog2015/485067/201506/031940090703869.png)

　　这样就可以当作服务器了。



　　libevent编译后的库下载地址: http://files.cnblogs.com/files/wunaozai/libevent.zip

　　from: http://www.cnblogs.com/wunaozai/p/4550084.html 









