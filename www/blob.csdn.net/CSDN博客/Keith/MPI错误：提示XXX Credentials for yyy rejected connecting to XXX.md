# MPI错误：提示XXX Credentials for yyy rejected connecting to XXX - Keith - CSDN博客





2014年12月09日 20:47:20[ke1th](https://me.csdn.net/u012436149)阅读数：2656








[MPI错误：提示XXX Credentials for yyy rejected connecting to XXX](http://www.cnblogs.com/zealsoft/archive/2008/05/04/1181944.html)
    这个错误信息是指你所提供的用户名和密码是错误的。在wmregister的对话框中，MPI要求你提供一个用户名（Account）和密码（Password），这个用户名和密码必须是可以登录你的机器的，如果用户名和密码错误就会出现这样的信息。


    在Windows XP下，有时系统显示的用户名并不是实际的用户名。用户名是你在建立用户账号时指定的，如果在“控制面板”的“用户帐户”中更改帐户名称，通常只是改变了显示的名称，而不是实际的用户名。例如：某机器安装时建立了“rr”这个用户，通过“控制面板”的“用户帐户”将帐户名改为“LI Hai”，但是在使用wmregister时，仍然需要使用“rr”这个用户名，而不是“LI Hai”。这一点可以通过选择“控制面板”的“管理工具”中的“计算机管理”这个工具来确认。在“计算机管理”工具中，选择“系统工具 |
 本地用户和组 | 用户”，在其中可以看到用户名称仍然为“rr”，用户的“全名”一栏显示的是“LI Hai”。所以，在Windows XP中，如果出现上面提到的错误，最好使用“计算机管理”这个工具来查看一下真正的用户名。

![](http://images.cnblogs.com/cnblogs_com/zealsoft/useraccount.JPG)
[原址](http://http//www.cnblogs.com/zealsoft/archive/2008/05/04/1181944.html)






