# ORA-00911: invalid character 错误解决 - 左直拳的马桶_日用桶 - CSDN博客
2017年08月24日 09:16:21[左直拳](https://me.csdn.net/leftfist)阅读数：3266
同事遇到了一个奇怪的问题：同样的SQL语句，写在C#里，访问oracle数据库，在他机器执行会报错，而其他机器则不会。
这条语句拿到他本机的PL/SQL developer执行，又没有问题。
![这里写图片描述](https://img-blog.csdn.net/20170824082457400?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
出现这种问题，首先检查SQL语句后面是否带分号”;”。带分号在plsql developer里执行没有问题，但在C#里就不允许。
不过同事的SQL没有分号。但是里面有中文字段。按照错误提示，应该中文编码的问题。
参考网上文章提示
```
控制面板--系统和安全---系统－－高级系统设置－－高级－－环境变量－－系统变量中
变量名：NLS_LANG
变量值：SIMPLIFIED CHINESE_CHINA.ZHS16GBK
把这个变量添加完成以后，把系统重启一下就ok了，重启以后，错误提示消失了。
```
果然错误修正。
这里修正的，是oracle服务器端的编码。如果是修改oracle客户端，需修改注册表。
参考文章： 
[ORA-00911: invalid character 错误解决集锦](http://www.linuxidc.com/Linux/2017-05/144361.htm)
[oracle服务器和客户端字符集的查看和修改](http://blog.csdn.net/dream19881003/article/details/6800056)
