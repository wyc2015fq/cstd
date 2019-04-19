# 同一个电脑安装两个jdk版本----本人测试可行！！！ - xqhrs232的专栏 - CSDN博客
2018年11月27日 16:12:06[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：38
个人分类：[JavaWeb技术](https://blog.csdn.net/xqhrs232/article/category/8231541)
原文地址::[https://blog.csdn.net/yuruixin_china/article/details/53607248](https://blog.csdn.net/yuruixin_china/article/details/53607248)
相关文章
1、一台电脑如何配置多个JDK----[https://blog.csdn.net/qq_26545305/article/details/66472521](https://blog.csdn.net/qq_26545305/article/details/66472521)
2、一台电脑安装两个不同版本jdk，实现不同版本切换----[https://blog.csdn.net/programminging/article/details/80770294](https://blog.csdn.net/programminging/article/details/80770294)
版权声明：保留权利，欢迎转载，知识因为分享而变得更加有意义!    https://blog.csdn.net/yuruixin_china/article/details/53607248
场景：公司项目使用的jdk为1.7，最近不是很忙，找到一个爬虫系统学习。该系统使用到了jdk1.8的特性，所以I need 俩版本，开整！！！
1 . 准备两个版本的jdk我的两个jdk路径为：
D:\jdk1.7.0_80 
D:\Program Files\Java\jdk1.8.0_111
2 . 设置两个子JAVA_HOME，一个总设置两个子JAVA_HOME:
JAVA_HOME7 = D:\jdk1.7.0_80 
JAVA_HOME8 = D:\ProgramFiles\Java\jdk1.8.0_111
此处JAVA_HOME设置即为你更换jdk版本是所要修改的地方
JAVA_HOME = %JAVA_HOME8%
1
3 . 设置path 
添加如下内容(注意添加’;’)
;%JAVA_HOME%\bin;%JAVA_HOME%\jre\bin
1
4 . 添加classpath变量
变量值
%JAVA_HOME%lib\dt.jar;%JAVA_HOME%\lib\tools.jar
1
5 . 查看版本是否更换成功
java -version  
1
javac -version
1
若未成功，请看接下来的6
6 . 未成功解决方案
删除C:\Windows\System32目录下的java.exe,删除即可。 
若java -version和javac -version版本不一致 将%JAVA_HOME%\bin加在PATH变量的头，执行java -version和javac -version，版本已然一致。
--------------------- 
作者：yuruixin_china 
来源：CSDN 
原文：https://blog.csdn.net/yuruixin_china/article/details/53607248 
版权声明：本文为博主原创文章，转载请附上博文链接！
