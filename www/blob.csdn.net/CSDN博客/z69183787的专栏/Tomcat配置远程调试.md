# Tomcat配置远程调试 - z69183787的专栏 - CSDN博客
2018年03月26日 18:36:37[OkidoGreen](https://me.csdn.net/z69183787)阅读数：667
[https://www.cnblogs.com/justuntil/p/7661536.html](https://www.cnblogs.com/justuntil/p/7661536.html)
[https://blog.csdn.net/torrytang/article/details/55001471](https://blog.csdn.net/torrytang/article/details/55001471)
[https://blog.csdn.net/mhmyqn/article/details/49209541](https://blog.csdn.net/mhmyqn/article/details/49209541)
===========方法1================================
tomcat7 ：catalina.bat jpda start,调试端口默认为8000
===========方法2================================ 
1.进入工程目录，运行Ant命令编译;
2.生成Eclipse工程所需的文件(.classpath和.project等);
3.antexpand发布编译生成的war包;
4.从外面启动Tomcat(某些工程并非标准JavaEE工程，无法直接在Server视图下集成服务器);
5.访问自己的成果，汗，此时如不做特殊设置，效果是看到了，可是调试呢？
带着问题学习了下远程调试总结如下：
在做远程调试时，在windows系统和非windows系统下的配置，Tomcat中会有所差别,具体如下：
第一步、配置tomcat
一、在windows系统中：
打开%CATALINE_HOME%/bin下的文件catalina.bat，加入下面这行：
set CATALINA_OPTS=-server -Xdebug -Xnoagent -Djava.compiler=NONE -Xrunjdwp:transport=dt_socket,server=y,suspend=n,address= 8000
其中 address=8000是没被使用的端口号。连接方式有两种，为dt_shmem和dt_socket，分别表示本机调试和远程调试。
二、在非windows系统中：
还需要把% CATALINE_HOME %/bin/startup.sh中的最后一行exec "EXECUTABLE"
 start "$@" 中的start改成jpda start。由于默认的端口是8000，所以如果8000端口已有他用的话，还需在catalina.sh文件中设置：JPDA_ADDRESS=8000。
输入命令startup.sh或者catalina.sh jpda start就可启动tomcat。
第二步、配置eclipse
在Eclipse中选择Run Debug，在弹出对话框中右击Remote Java Application新建一个远程调试项，如下所示：
![](https://images0.cnblogs.com/blog/285763/201308/27103313-5e7ff5c5cabf447382596f08a3e41be8.gif)
在 “Name”输入框中输入远程调试的名称，在“Project”中选择要调试的项目，在“Host”中输入需要远程调试项目的IP，也就是tomcat所在的IP，在“Port”中输入设置的端口号，比如上面设置的8000，然后钩选“Allow termination of remote VM”，点击“Apply”即可。
设置完后就可以开始调试了，大概分一下几步：
1、启动tomcat（远程），如在控制台输出“Listening for transport dt_socket at address: 8000”，即说明在tomcat中设置成功；
2、在本机设置断点;
3、进入上图界面，选择要调试的项，点击“Debug”即可进行远程调试；
4、访问你的测试页面即可看到久违的调试界面。
但每次做上述操作非常烦，不如写个批处理，如取名为Tomcat debug.bat，在这个文件中加入下面几行：
cd %CATALINE_HOME%/bin
set JPDA_ADDRESS=8000
set JPDA_TRANSPORT=dt_socket
set CATALINA_OPTS=-server -Xdebug -Xnoagent -Djava.compiler=NONE -Xrunjdwp:transport=dt_socket,server=y,suspend=n,address=8787
startup
将此脚本保存到tomcat/bin目录下，然后发个快捷方式在桌面，欧啦~~~~
这样需要远程调试时，运行debug.bat即可；不需要远程调试时，还是运行startup.bat文件
==========方法3================================
tomcat远程调试
1.Linxu系统: apach/bin/startup.sh开始处中增加如下内容： 
　　declare -x CATALINA_OPTS="-server -Xdebug -Xnoagent -Djava.compiler=NONE -Xrunjdwp:transport=dt_socket,server=y,suspend=n,address=8788"   
2.Windows系统: apach/bin/startup.bat开始处中增加如下内容： 
```
SET CATALINA_OPTS=-server -Xdebug -Xnoagent -Djava.compiler=NONE -Xrunjdwp:transport=dt_socket,server=y,suspend=n,address=8788
```
