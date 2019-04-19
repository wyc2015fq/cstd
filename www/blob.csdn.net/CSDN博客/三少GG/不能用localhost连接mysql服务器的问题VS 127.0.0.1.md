# 不能用localhost连接mysql服务器的问题VS 127.0.0.1 - 三少GG - CSDN博客
2011年02月06日 22:33:00[三少GG](https://me.csdn.net/scut1135)阅读数：3390
自从上次[在WIN7下搭建PHP环境](http://www.xxzh.org/web/Win7-IIS-FastCGI-PHP.html)之后，我就到网上找了些PHP源码来调试，这些PHP都可以完美运行，其中有些程序是需要安装mysql数据库的，我就特地下载并装上了mysql-5.1.51和phpMyAdmin-3.3.7。但是每次安装程序到了配置数据库这一步的时候都连接不上mysql数据库，我可以确定数据库用户名和密码是正确的，而且随便换什么程序安装也是这种情况。
![](http://www.xxzh.org/upload/lianbushang-shujuku.jpg)
    我检查了下mysql并没有发现问题，我的mysql服务器用的是本机地址localhost、默认root和空密码，而且我用phpMyAdmin是可以连接上mysql数据库的，百度多次都没有找到答案，今天想再次测试的时候想到本机地址也可以输入IP即“127.0.0.1”，本来只是抱着试一试的想法，没想到安装成功了。
    我们知道localhost和127.0.0.1都是本机地址的意思，输入localhost，系统也会自动解析到127.0.0.1，而且我的phpMyAdmin也都是用localhost这个地址打开的，说明localhost确实已经解析到了127.0.0.1，但是不知道为什么不能用localhost这个地址来连接mysql数据库。
    因为大多PHP程序安装的默认数据库地址localhost，每次都要去修改成127.0.0.1觉得有点麻烦，既然知道了原理就好办了，我们可以通过修改HOSTS文件来达到我们的目的。首先用记事本打开HOSTS文件“C:/WINDOWS/system32/drivers/etc/hosts”，打开后如图所示在其中加上一条“127.0.0.1 localhost”并保存。如果你是WIN7系统，可以参考下[WIN7下修改HOSTS文件](http://www.xxzh.org/computer/Windows7-hosts.html)这篇文章。
![](http://www.xxzh.org/upload/hosts-localhost.jpg)
    进行完上面的操作以后我们就可以直接用localhost连接mysql服务器了。我不知道有没有人碰过这种问题，还是只有WIN7系统才有这种情况，反正我就是通过这种方法解决的。
