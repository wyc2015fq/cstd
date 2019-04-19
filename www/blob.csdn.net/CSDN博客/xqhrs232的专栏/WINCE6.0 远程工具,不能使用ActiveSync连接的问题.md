# WINCE6.0 远程工具,不能使用ActiveSync连接的问题 - xqhrs232的专栏 - CSDN博客
2013年11月01日 11:07:18[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1110
原文地址::[http://www.cnblogs.com/gmh915/archive/2010/10/28/1945398.html](http://www.cnblogs.com/gmh915/archive/2010/10/28/1945398.html)
相关文章
1、[WINCE6.0 远程工具,不能使用ActiveSync连接的问题](http://blog.csdn.net/michaelcao1980/article/details/8594863)----[http://blog.csdn.net/michaelcao1980/article/details/8594863](http://blog.csdn.net/michaelcao1980/article/details/8594863)
2、WinCE6.0使用远程工具监控和远程线程管理Using the Remote Tools & the Remote Process----[http://hi.baidu.com/dragoniye2008/item/1db2598bf6a14e58850fab29](http://hi.baidu.com/dragoniye2008/item/1db2598bf6a14e58850fab29)
现象：“VS2005->Target->Remote Tools”打开的远程工具，当选用ActiveSync作为连接方式时，会出现如下错误：“The Microsoft ActiveSync reported the following error: Unable to load device side components”
    解决:
    微软的解决方法:
找不到 "C:\Program 
Files\Common Files\Microsoft Shared\Windows CE Tools\Platman\target\wce600\armV4". 下面有关的库和工具
所以应该添加这个目录文件："C:\Program Files\Common Files\Microsoft Shared\Windows CE 
Tools\Platman\target\wce600\armV4"
然后把"C:\Program Files\Common Files\Microsoft Shared\Windows CE 
Tools\Platman\target\wce600\armV4i"下所用的内容复制到armV4下面。
最后随便打开一个remote tools ，添加一个设备，transport 和 startup都选用activesync. 连接就好了！
[](http://blog.csdn.net/GaryLiu1104/archive/2010/04/14/5482594.aspx)
注意是把armV4i下面的东西复制到armV4下，，上面的c盘符是在安装目录盘符下，但我由于之前在f盘下装了wince5.0，后面又在e盘下安装了wince6.0，在e盘下找不到上面这个路径，而是在f盘下找到了，修改后，一切正常能用了。

//=====================================================================
备注::
1>这样就不用去用Start /程序/Microsoft Visual Studio 2005/visual studio remote tools/下的那些工具了，可以直接用在VS2005 IDE下Target/Remote Tools/下的工具
2>这样的话Performance monitor/System information/Kernel Tracker/Call Profiler等工具也可以用了，这样就可以监视OS的内存动态情况了

