# 将maven项目部署到linux中的tomcat - 苝花向暖丨楠枝向寒 - CSDN博客

2018年07月29日 13:02:55[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：1029


修改tomcat目录下的conf目录下的 tomcat-users.xml

在最下面加入下面3行内容，前两行是权限，写法固定，最后一行是设置tomcat用户名，以及密码

```java
<role rolename="manager-gui"/>
  <role rolename="manager-script"/>
  <user username="tomcat" password="123456" roles="manager-gui,manager-script"/>
```

打开pom文件，右键 ---> Maven --->Add Plugin 

![](https://img-blog.csdn.net/20180729121556642?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

右键项目，Run as -- > Maven build..       在Goals 中填写 tomcat7:deploy  然后点击OK，项目就部署到了tomcat中

![](https://img-blog.csdn.net/20180729123949704?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

重新部署的命令是 tomcat7:redeploy

Run As 中还有一个 Maven Build  后面不带... 的 ， 点击这个 就会出现曾经执行过的命令，就无需再手动输入了。

测试：

启动tomcat服务器，然后打开浏览器输入ip地址进入tomcat首页

 点击Manager App，输入用户名密码(刚在配置文件中配置的)

![](https://img-blog.csdn.net/20180729125954327?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

如何可以在下面找到我们的项目名，就说明部署成功了。我的项目名是back0330

![](https://img-blog.csdn.net/20180729130120778?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

