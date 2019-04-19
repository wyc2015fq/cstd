# oracle 的sys 和 system 账号 - 左直拳的马桶_日用桶 - CSDN博客
2017年05月04日 11:16:47[左直拳](https://me.csdn.net/leftfist)阅读数：813
sys 和 system 账号有啥区别？一直以来懵懵懂懂，只想当然的认为就是权限大小不一样。
但是，它们都是管理员？
现在，我知道有一个区别了：
> 
【sys】只能用sysdba身份登录（也许还有sysoper？），在客户端和服务器都可以 
  【system】在客户端只能用normal身份登录；在服务器，normal和sysdba都可以。
另外，normal身份登录，不能带“；”号，否则系统会以为这是连接串的一部分，我靠。我发现在sqlplus外面，真不能随便带这个分号，比如expdp，impdp等等。
其他区别学习笔记： 
**1、最重要的区别，存储的数据的重要性不同**
【sys】 
所有Oracle的数据字典的基表和视图都存放在sys用户中，这些基表和视图对于oracle的运行是至关重要的，由数据库自己维护，任何用户都不能手动更改。
【system】 
用户用于存放次一级的内部数据，如oracle的一些特性或工具的管理信息。
**2、其次的区别，权限的不同**
【sys】 
拥有dba，sysdba，sysoper等角色或权限，是oracle权限最高的用户。登陆em也只能用这两个身份，不能用normal。
【system】 
system用户拥有普通dba角色权限。用户只能用normal身份登陆em，除非你对它授予了sysdba的系统权限或者syspoer系统权限。
system如果正常登录，它其实就是一个普通的dba用户，但是如果以as sysdba登录，其结果实际上它是作为sys用户登录的
**3、sysdba和dba的区别**
sysdba，是管理oracle实例的，它的存在不依赖于整个数据库完全启动，只要实例启动了，它就已经存在，以sysdba身份登陆，装载数据库、打开数据库。
只有数据库打开了，或者说整个数据库完全启动后，dba角色才有了存在的基础。
**4、Sysdba和sysoper两个系统权限区别**
【sysdba】拥有最高的系统权限，登陆后是 sys
【sysoper】主要用来启动、关闭数据库，sysoper 登陆后用户是 public
![这里写图片描述](https://img-blog.csdn.net/20170504111556262?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
参考资料： 
[Oracle sys和system用户、sysdba 和sysoper系统权限、sysdba和dba角色的区别](http://blog.csdn.net/wanghai__/article/details/4791879)
