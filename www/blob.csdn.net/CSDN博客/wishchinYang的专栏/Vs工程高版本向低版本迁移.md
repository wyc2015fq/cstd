# Vs工程高版本向低版本迁移 - wishchinYang的专栏 - CSDN博客
2016年01月11日 12:21:34[wishchin](https://me.csdn.net/wishchin)阅读数：1469
        使用到一个SLAM程序，一些运行库是基于PCL1.7.2Vs11版本，但是Vs11巨慢，应该把程序迁移到Vs10 下面，遇到打不开的问题。
1.修改方法：
             参考链接：[使用低版本打开高版本VS解决方案](http://blog.csdn.net/g1036583997/article/details/18985375)
       我的做法是：用记事本打开.sln文件，
              把文件里面的 
Microsoft Visual Studio Solution File, Format Version 12.00
# Visual Studio 2012
修改为
Microsoft Visual Studio Solution File, Format Version 11.00
# Visual Studio 2010
      可以打开解决方案。
重新生成解决方案，可以运行解决方案。
2. 一个问题： 遇到.pch文件打不开
       fatal error c1083: 无法打开[预编译](http://www.makaidong.com/search.jspx?q=%E9%A2%84%E7%BC%96%E8%AF%91)头文件：“debug/****.pch”: no such file or  directory
  排错方法：  
        解决方法：修改：**项目->属性->c/c++ ->[预编译](http://www.makaidong.com/search.jspx?q=%E9%A2%84%E7%BC%96%E8%AF%91)头->不使用[预编译](http://www.makaidong.com/search.jspx?q=%E9%A2%84%E7%BC%96%E8%AF%91)头    即可**。
