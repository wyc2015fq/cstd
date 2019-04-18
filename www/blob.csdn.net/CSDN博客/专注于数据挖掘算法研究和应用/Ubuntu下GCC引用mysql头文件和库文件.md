# Ubuntu下GCC引用mysql头文件和库文件 - 专注于数据挖掘算法研究和应用 - CSDN博客





2011年11月07日 11:29:37[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：4121标签：[mysql																[ubuntu																[gcc																[c](https://so.csdn.net/so/search/s.do?q=c&t=blog)
个人分类：[C/C++](https://blog.csdn.net/fjssharpsword/article/category/6480441)





1.安装mysql-server：

   sudo apt-get install mysql-server-5.1

2.gcc连接mysql的库安装：

   sudo apt-get install libmysqlclient-dev

  安装后，头文件在/usr/include/mysql，库文件在/usr/lib/mysql和/usr/lib下。

3.具体操作方法，参考：[http://wenku.baidu.com/view/ed359c4de518964bcf847cdd.html](http://wenku.baidu.com/view/ed359c4de518964bcf847cdd.html)](https://so.csdn.net/so/search/s.do?q=gcc&t=blog)](https://so.csdn.net/so/search/s.do?q=ubuntu&t=blog)](https://so.csdn.net/so/search/s.do?q=mysql&t=blog)




