# Openfire 配置连接SQL SERVER（非默认实例） - 左直拳的马桶_日用桶 - CSDN博客
2014年06月04日 22:00:19[左直拳](https://me.csdn.net/leftfist)阅读数：4145
安装好Openfire之后，紧接着进行配置。
连接数据库的时候遇上问题。
打算用我本机上的一个SQL SERVER做为数据库。但是，我本机装了几个SQL SERVER实例，现在我打算使用的是那个非默认实例。
众所周知，访问SQL SERVER的非默认实例，要指明实例名，用studio management打开的时候，服务器名是类似这样的：(local)\SqlServer2012
但在openfire这里，用jdbc，应该怎么写呢？
摸索了好久，才知道应该这样写：
jdbc:jtds:sqlserver://127.0.0.1:端口;instance=实例名;DatabaseName=数据库名;appName=jive
还有一个前提，就是Sql Server Browser 要打开，可以用SQL SERVER的配置工具进去看看。
======================================================
在WIN7下运行OpenFire，要用管理员身份，否则报错；
第一次运行，要进行设置。
配置数据库之前，要新建数据库，然后运行数据库脚本。数据库脚本在  C:\Program Files\Openfire\resources\database，你是什么数据库就选对应的脚本。根目录下的脚本即可。
