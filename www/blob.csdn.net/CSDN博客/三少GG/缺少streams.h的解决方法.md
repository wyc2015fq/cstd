# 缺少streams.h的解决方法 - 三少GG - CSDN博客
2012年05月02日 00:24:48[三少GG](https://me.csdn.net/scut1135)阅读数：8656
                
安装完directx sdk后，要将directx sdk的include和lib目录配置到vc的系统目录中，并且放到标准的vc目录之前，以保证编译器能拿到最新版本的源文件。
配置方法为，打开vc，选择菜单项tool、options、directories，在弹出的对话框中的show directories for中选择include files，添加配置如下（假设directx sdk安装在D:\dxsdk目录下）
streams.h这个头文件现在在%\DX9 SDK\Samples\C++\DirectShow\BaseClasses路径下，首先要把这个路径添加到VC的Include Directories下面。
**vs2008中从别处拷贝BaseClasses到Samples\C++\DirectShow\BaseClasses下！！！**
streams.h问题汇总zz
2008-09-04 20:42
1.找不到streams.h的解决方法：
streams.h这个头文件现在在%Platform SDK Root%\Samples\Multimedia\DirectShow\BaseClasses路径下，首先要把这个路径添加到VC的Include Directories下面。
注：需要下载streams.h的朋友们注意了，其实你根本不用下载。
最 令我ft的是DirectShow的library，真不知道M$的人是不是脑子坏掉了，库居然要自己编译。一开始我翻遍了硬盘也没找到 strmbasd.lib，最后在BaseClasses目录下看到一个makefile，才知道是怎么回事。扯远了，打住。打开SDK的command line debug build environment ，进入%Platform SDK Root%\Samples\Multimedia\DirectShow\BaseClasses目录，二话不说nmake，生成一个 XP32_DEBUG目录（我选的是Set
 Windows XP 32-bit Build Environment (Debug)），strmbasd.lib赫然在目，这就是debug版的库，retail版的如法炮制，生成的库在XP32_RETAIL目录下，名 为strmbase.lib。
2.请问下面的问题是怎么回事儿（vc.net下，连接了Strmiids.lib   Quartz.lib   strmbasd.lib   uuid.lib           winmm.lib）：   
是不是需要指定路径阿。。这些库文件在哪个目录阿，   
fatal   error   C1083:   无法打开包含文件:“streams.h”:   No   such   file   or   directory  
回答：
1   build   the   BaseClasses   project   in   DirectSDK\Samples\C++\DirectShow\BaseClasses   
2   add   the   baseclasses   dir   to   your   include   path
