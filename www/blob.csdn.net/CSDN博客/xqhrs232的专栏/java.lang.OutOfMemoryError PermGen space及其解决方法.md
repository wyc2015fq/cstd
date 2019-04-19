# java.lang.OutOfMemoryError: PermGen space及其解决方法 - xqhrs232的专栏 - CSDN博客
2018年11月30日 15:53:46[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：66
个人分类：[JavaWeb技术](https://blog.csdn.net/xqhrs232/article/category/8231541)
原文地址::[https://blog.csdn.net/figo0423/article/details/64440779](https://blog.csdn.net/figo0423/article/details/64440779)
相关文章
1、[java.lang.OutOfMemoryError: PermGen space有效解决方法](https://www.cnblogs.com/learnapi/p/7641591.html)----[https://www.cnblogs.com/learnapi/archive/2017/10/09/7641591.html](https://www.cnblogs.com/learnapi/archive/2017/10/09/7641591.html)
今天换了个tomcat6.0.51的时候，两个项目同时debug启动，就报这个错误了：java.lang.OutOfMemoryError: PermGen space
        网上找了下答案主要是这样的解决方案：
        手动设置MaxPermSize大小
        修改TOMCAT_HOME/bin/catalina.bat（Linux下为catalina.sh），
        在“echo "Using CATALINA_BASE: $CATALINA_BASE"”上面加入以下行：
        set JAVA_OPTS=%JAVA_OPTS% -server -XX:PermSize=128M -XX:MaxPermSize=512m
         catalina.sh下为：
         JAVA_OPTS="$JAVA_OPTS -server -XX:PermSize=128M -XX:MaxPermSize=512m"
         参考文章：http://blog.csdn.net/fengyie007/article/details/1780375
         加上了没有解决，最后看到某人的评论解决了问题，非常感谢~
          设置tomcat的时候，应该双击server的配置，加上以下配置。
       -Xms256M -Xmx512M -XX:PermSize=256m -XX:MaxPermSize=512m
   点击open launch configuration
--------------------- 
作者：志飞 
来源：CSDN 
原文：https://blog.csdn.net/figo0423/article/details/64440779 
版权声明：本文为博主原创文章，转载请附上博文链接！
