# tomcat随windows自动启动 - xqhrs232的专栏 - CSDN博客
2018年11月30日 17:31:31[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：38
个人分类：[JavaWeb技术](https://blog.csdn.net/xqhrs232/article/category/8231541)
原文地址::[https://blog.csdn.net/jinjianghai/article/details/53022118](https://blog.csdn.net/jinjianghai/article/details/53022118)
相关文章
1、[如何在开机时让Tomcat以进程的方式启动](https://www.cnblogs.com/Mr-kevin/p/5586779.html)----[https://www.cnblogs.com/Mr-kevin/p/5586779.html](https://www.cnblogs.com/Mr-kevin/p/5586779.html)
2、tomcat怎么设置开机自动启动----[https://jingyan.baidu.com/article/48b558e3f23f6d7f38c09a0d.html](https://jingyan.baidu.com/article/48b558e3f23f6d7f38c09a0d.html)
一：确保tomcat 在点击bin\startup 文件可以正常启动访问；
二：本机安装有JDK；
三：本机环境变量配置：JAVA_HOME：C:\Java\jdk1.7.0_17;
四：本机Tomcat环境变量配置：CATALINA_HOME：D:\work\apache-tomcat-7.0.72;     
五：找到Tomcat目前中bin目前下的service.bat并编辑；搜索：”JvmMx 256“ 后面加空格 增加：Startup=auto
六：找到service.bat，打开搜索”SERVICE_NAME“ 可以看到服务名称；
七：启动：cmd窗口进入到tomcat的bin目录下：执行命令：service.bat install iatoms-activiti
八：删除：cmd窗口进入到tomcat的bin目录下：执行命令：service.bat remove iatoms-activiti
九：设置开机启动，以后就可以随机启动了，配图如下：
--------------------- 
作者：靳江海 
来源：CSDN 
原文：https://blog.csdn.net/jinjianghai/article/details/53022118 
版权声明：本文为博主原创文章，转载请附上博文链接！
