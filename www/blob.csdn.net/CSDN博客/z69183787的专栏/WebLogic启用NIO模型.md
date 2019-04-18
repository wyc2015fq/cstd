# WebLogic启用NIO模型 - z69183787的专栏 - CSDN博客
2014年03月22日 14:24:34[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2799
JDK 1.4之后引入了NIO（非阻塞的New IO），而WebLogic默认则会自动选择本地代码IO（Native IO）或Java IO。可通过如下参数指定您想使用的Muxer实现为NIO：
增加至 setDomainEnv 中 
set JAVA_OPTIONS=%JAVA_OPTIONS%
set JAVA_OPTIONS=%JAVA_OPTIONS% -Dweblogic.MuxerClass=weblogic.socket.NIOSocketMuxer
行出
-Dweblogic.MuxerClass=weblogic.socket.NIOSocketMuxer
注意这样做可能会有风险存在。
下图显示了几种WebLogic中的Muxer实现类：
![image](http://www.beansoft.biz/wp-content/uploads/2012/09/image.png)
