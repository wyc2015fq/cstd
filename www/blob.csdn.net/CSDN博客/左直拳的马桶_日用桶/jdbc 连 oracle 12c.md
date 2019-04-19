# jdbc 连 oracle 12c - 左直拳的马桶_日用桶 - CSDN博客
2016年06月07日 20:52:30[左直拳](https://me.csdn.net/leftfist)阅读数：5208
jdbc 连 oracle 12c，除了连接串要书写正确（如果用PDB，可插拔数据库），必要的JDBC包也是不可或缺的。
比如我，机器本身装了个oracle 10g，然后上面有个java项目，使用jdbc去连服务器的oracle 10g数据库，一点问题没有。
直到今天，去连oracle 12c，报错了。
连接串是没问题的，我之前遇到过。见拙作： 
[Oracle可插拔数据库的jdbc连接串写法](http://blog.csdn.net/leftfist/article/details/50478454)
那么只能是包的问题了。我装的是JDK7，到oracle官网下载了个for jdk7的jdbc包，丢到 java_home 的相关位置，见拙作： 
[WEB服务器安装oracle jdbc](http://blog.csdn.net/leftfist/article/details/49999549)
问题解决。之前这里并没有啥jdbc包，估计是系统自行引用了我本机安装的oracle 10g的相应包。但10g对10g是可以的，更高版本就不行鸟。
