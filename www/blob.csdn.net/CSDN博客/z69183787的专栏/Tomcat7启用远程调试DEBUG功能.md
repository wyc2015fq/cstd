# Tomcat7启用远程调试DEBUG功能 - z69183787的专栏 - CSDN博客
2018年10月15日 17:29:36[OkidoGreen](https://me.csdn.net/z69183787)阅读数：87
[https://blog.csdn.net/yztezhl/article/details/79026404](https://blog.csdn.net/yztezhl/article/details/79026404)
# 前言
工作上可能用的上这个功能，因为将项目放到服务器（测试，生产还是别开的好）后，有些问题需要DEBUG，没有断点非常不方便啊。
而且网上很多资料说的乱七八糟，图不对版的（自己贴的图和你自己写的东西都不一样！严重误导别人！！！）
自己踩完坑后MARK一下
# Tomcat
**Windows下**
进入目录下的bin目录，编辑打开**startup.bat**
在前面添加：SET CATALINA_OPTS=-server -Xdebug -Xnoagent -Djava.compiler=NONE -Xrunjdwp:transport=dt_socket,server=y,suspend=n,address=8000
完成一行，不要断开。
![](https://images2017.cnblogs.com/blog/1176890/201708/1176890-20170821161714168-174021555.png)
**Linux下（CentOS）**
**在catalina.sh中的首行添加：**
CATALINA_OPTS="-Xdebug  -Xrunjdwp:transport=dt_socket,address=8000,server=y,suspend=n"
![](https://images2017.cnblogs.com/blog/1176890/201709/1176890-20170925201800901-180660824.png)
**启动~看是否弹出信息(一闪而过）**
**![](https://images2017.cnblogs.com/blog/1176890/201708/1176890-20170821161946121-728362794.png)**
参数说明：
-Xdebug                              ： 启用调试模式
-Xrunjdwp<sub-options>  : 加载JVM的JPDA参考实现库
transport=dt_socket           ：Socket连接，可选dt_shmem 通过共享内存的方式连接到调试服务器
address=8000                     ：调试服务器监听的端口
server=y                             ： 是否是服务器端，n为客户端
suspend=n                         ： 启动过程是否加载暂停，y为启动时暂停，方便调试启动过程
```
CATALINA_OPTS="$CATALINA_OPTS -Xdebug -Xrunjdwp:transport=dt_socket,address=8787,server=y,suspend=n"
```
# **Eclipse**
 在Run下找到Debug Configurations
中文改成对应的信息。
![](https://images2017.cnblogs.com/blog/1176890/201708/1176890-20170821162140386-1826580773.png)
然后点击**Debug**
![](https://images2017.cnblogs.com/blog/1176890/201708/1176890-20170821162254793-1249423802.png)
之后设置断点什么的就和本地差不多了
