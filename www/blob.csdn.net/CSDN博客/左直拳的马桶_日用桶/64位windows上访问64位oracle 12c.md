# 64位windows上访问64位oracle 12c - 左直拳的马桶_日用桶 - CSDN博客
2016年07月15日 20:05:54[左直拳](https://me.csdn.net/leftfist)阅读数：4501
64位windows上访问64位oracle 12c，这会有啥问题？
没啥问题。问题是，我64位操作系统的机器上装了个oracle 10g。而oracle 10g好像是不区分啥32位、64位的，一律32位。
此前，我在ASP.NET项目中，连接无论是oracle 10g，还是oracle 12c，都没有问题，只须在对应的IIS的应用程序池上开启32位支持即可。为啥要开启32位支持呢？原因是本机的oracle是10g的，32位，它充当了客户端的角色，凡访问oracle，不管外部内部，都要这个客户端支持。
由于IIS的强大，asp.net程序被支持得很好，连接oracle的途径畅通无阻。直到近期我开发一个WIN FORM桌面程序。这个可没有IIS的支持。于是就报错了：
> 
数据连接不成功，请检查该数据库是否已启动尝试加载oracle客户端时引发BadImageFormatException.如果在安装32位Oracle客户端组件的情况下以64位模式运行，将出现此问题。
怎么办呢？难道我要装一个64位的oracle客户端？真麻烦。
依照以前的经验（[.NET访问ORACLE心得：疯狂重启IIS，如有神助](http://blog.csdn.net/leftfist/article/details/7361747)），我决定不装这个客户端，仅到oracle站点下载一个包，然后修改一下系统环境的PATH。
1、首先到oracle官方站点下载64位的 instantclient-basic-windows.x64-12.1.0.2.0.zip。 
里面有几个dll。看上去，.NET是依赖于其中的oci.dll来访问oracle。
2、然后将这个包的路径加到系统环境变量PATH里 
主要是让系统能找到里面的oci.dll。据说系统会自动忽略32位的oci.dll，转而用这个64位的。
3、PATH设置好以后，程序依然报错。靠。查看PATH，生效了，但不知为什么，程序还是报错。一怒之下，重启，问题解决。真贱，不重启都不行。
估计是系统初始化时，会加载这个PATH的？然后加载完就完了，才不管你这个PATH是否已经修改。
