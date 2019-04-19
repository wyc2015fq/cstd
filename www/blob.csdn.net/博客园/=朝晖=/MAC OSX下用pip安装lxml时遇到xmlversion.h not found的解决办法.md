# MAC OSX下用pip安装lxml时遇到xmlversion.h not found的解决办法 - =朝晖= - 博客园
# [MAC OSX下用pip安装lxml时遇到xmlversion.h not found的解决办法](https://www.cnblogs.com/dhcn/p/7120871.html)
http://blog.csdn[.NET](http://lib.csdn.net/base/dotnet)/wave_1102/article/details/37730589
今天在Mac下用pip安装lxml，总是报如下错误：
```
etree_defs.h:9:10: fatal error: 'libxml/xmlversion.h' file not found
```
参考http://lxml.de/installation.html的安装文档，用如下命令安装问题依旧。
```
STATIC_DEPS=true sudo pip install lxml
```
搜了几个方法都不行，无意间搜到如下文章：http://blog.marchtea.com/archives/91，如法炮制，增加C_INCLUDE_PATH指定到XCode MacOSX SDK中libxml路径，问题搞定。Mark一下。
```
sudo C_INCLUDE_PATH=/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/usr/include/libxml2:/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/usr/include/libxml2/libxml:/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/usr/include pip install lxml
```
上面这个指令并不能原班照抄着用，其中MacOSX版本号部分得改成自己系统的版本号，或者到那个父目录里面看一下。

