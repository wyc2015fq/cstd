# Zeroc Ice 3.6.1 生成 vs2015 c++ 版本库及相关配置 - DoubleLi - 博客园






### 背景：

目前发布的Ice最新版本为3.6.0，使用的vs2013编译的，vs2015版本正在开发，发布只是时间问题；

不过官方实在给力，github上的源码已经支持vs2015编译了。源码版本为3.6.1

## 开发环境：

64位Win7旗舰版 + vs2015企业版

## 正题：

### 1、编译及安装Ice

从github上下载ice3.6.1的源码，按照https://github.com/zeroc-ice/ice/blob/master/cpp/BuildInstructionsWindows.md说明进行编译及安装。

说明：

1）不用手动下载或安装第三方依赖库。官方已经把第三方依赖库集成到Makefile中了，使用nuget直接去下载库文件。

2）默认编译为Debug版本，修改cpp\config\Make.rules.mak文件中 OPTIMIZE = yes （取消注释），则编译为Release版本。

3）默认安装目录为 C:\Ice-3.6.1

4）编译中出现的错误及解决办法：

错误1：下载第三方依赖库时，出错：无法连接到远程服务器。

解决办法：确保网络是正常的，然后多试几次。那四个第三方库，我大概试了5、6次才全部下载下来。



错误2：在Debug和Release版本编译切换后出现 error LNK2038: 检测到“_ITERATOR_DEBUG_LEVEL”的不匹配项

解决办法：执行 nmake /f Makefile.mak clean，清除之前编译生成的文件。

### 2、安装Ice Builder

从官方下载 https://visualstudiogallery.msdn.microsoft.com/1a64e701-63f2-4740-8004-290e6c682ce0并安装

### 3、配置vs2015

1）vs2015界面 -- 工具 -- 选项 -- Projects and Solutions -- Ice Builder，将Ice home directory设为C:\Ice-3.6.1

2）安装Ice Builder后，IceLib宏的值默认为C:\Ice-3.6.1\lib\vc140，而我们之前默认安装的路径C:\Ice-3.6.1\lib下没有vc140目录，因此新建vc140目录并把lib下文件复制一份过去。









