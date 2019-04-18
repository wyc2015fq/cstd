# IDEA 13 tomcat 进行远程调试 - z69183787的专栏 - CSDN博客
2015年07月06日 12:48:18[OkidoGreen](https://me.csdn.net/z69183787)阅读数：5803
1.设置远程tomcat 的 bin/catalina.sh （linux 系统下，win系统下，就修改catalina.bat）文件;
    找到：CATALINA_OPTS并设置添加如下语句：
CATALINA_OPTS="-Xdebug -Xrunjdwp:transport=dt_socket,address=60222,suspend=n,server=y"
    如果是windows server系统，那就是：
set CATALINA_OPTS="-Xdebug -Xrunjdwp:transport=dt_socket,address=60222,suspend=n,server=y"
设置完成后，启动服务器上的tomcat会出现：Listening for transport dt_socket at address : 60222   说明设置成功。
2.设置IDEA远程tomcat进行调试。
![](http://static.oschina.net/uploads/space/2015/0116/141415_f45b_571282.png)![](http://static.oschina.net/uploads/space/2015/0116/141431_BiIq_571282.png)![](http://static.oschina.net/uploads/space/2015/0116/141448_exUY_571282.png)![](http://static.oschina.net/uploads/space/2015/0116/141500_AXaV_571282.png)
1.　　首先在IDEA中check出来你要调试的工程的代码，假设工程的名字为A。然后在IDEA打开这个工程。
2.　　因为我们用的是Tomcat，所以在IDEA中点击右上角那个“Edit Configurations”按钮，然后在弹出的界面中点击左上角的加号，选择tomcat server->remote 
3.　　在弹出的的界面中填写服务器的ip和工程的端口。
4.　　然后点击那个弹出框的Starup/Connection选项卡，点击debug按钮，可以看到下面的文本框中有一段类似于下面的文字：
```
```java
-Xdebug
 -Xrunjdwp:transport=dt_socket,address=
```
```java
55890
```
```java
,suspend=n,server=y
```
```
5.　　将文本框中的这段话复制，然后登陆自己的远程机器，修改配置，在JAVA_OPTS属性中增加上面的那段话。并重启tomcat
6.　　然后回到自己的idea点击debug就可以加断点调试了。profile什么的都不需要选择。
附上eclispe 的 debug
前提：服务器上代码 和本地代码一样。
1.打开Eclipse，点击左上角的debug按钮，如图： 
![](http://dl.iteye.com/upload/attachment/0067/1585/c783f7e2-69bd-305d-a393-7f36ec3ccec9.jpg)
2.点击上图的红色区域 
![](http://dl.iteye.com/upload/attachment/0067/1591/44994db0-0319-3187-bbde-ed3bf263af34.jpg)
3.双击上图的红色区域 
![](http://dl.iteye.com/upload/attachment/0067/1593/6ecb7ecf-aab5-3fb8-8c19-a3f9a1c1129b.jpg)
4.Project：选择你要debug的项目，Host：输入你的远程IP，Port：远程调用端口。如何知道远程端口，通过进入你的远程虚拟机的命令行，输入jps -v 命令，查找这一行
-Xrunjdwp:transport=dt_socket,server=y,address=2424,suspend=n 
其中2424就是你远程服务器的调用接口。如果找不到，那是因为你的远程虚拟机tomacat或jboss配置没有支持remote debugging，需要在给JVM加上如下的启动参数：
-Xdebug -Xrunjdwp:transport=dt_socket,address=2424,server=y,suspend=n 
adderss由你自己定的。 
5.再一切设置好了后，点击右下角的debug按钮，你就可以像本地debug一样进行远程debug了。
