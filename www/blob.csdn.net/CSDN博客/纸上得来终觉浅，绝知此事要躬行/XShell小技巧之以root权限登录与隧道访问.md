# XShell小技巧之以root权限登录与隧道访问 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年11月09日 18:25:16[boonya](https://me.csdn.net/boonya)阅读数：632








这里记录一下两个小技巧：以root登录与隧道访问。

### XShell以root权限方式登录

需要设置登录脚本：sudo su  这个命令会要求我们输入root账号的密码

![](https://img-blog.csdnimg.cn/20181109181640246.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==,size_16,color_FFFFFF,t_70)

我们还需要添加一个密码：

![](https://img-blog.csdnimg.cn/20181109181834161.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==,size_16,color_FFFFFF,t_70)

确定，连上之后默认就是管理员了。

![](https://img-blog.csdnimg.cn/20181109182016539.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==,size_16,color_FFFFFF,t_70)

### 开启隧道访问

隧道访问适合局域网SSH，也适合远程SSH。

![](https://img-blog.csdnimg.cn/2018110918231574.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==,size_16,color_FFFFFF,t_70)

这样设置之后，我们就可以在本地访问远程服务了，就像跟本地一样，比如：http://localhost:1935/stat ,它实际对应的是外网服务地址的IP和端口服务。



