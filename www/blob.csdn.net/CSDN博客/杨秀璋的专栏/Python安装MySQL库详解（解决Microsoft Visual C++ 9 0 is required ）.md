# Python安装MySQL库详解（解决Microsoft Visual C++ 9.0 is required ） - 杨秀璋的专栏 - CSDN博客





2018年04月25日 15:16:08[Eastmount](https://me.csdn.net/Eastmount)阅读数：2096
所属专栏：[Python学习系列](https://blog.csdn.net/column/details/eastmount-python.html)









前面我们介绍的Python网络爬虫通常将抓取的数据存储至TXT或CSV文件，而当数据量增加之时，就需要将其存储至本地数据库了。Python访问数据库需要对应的接口程序，我们可以把接口程序理解为Python的一个模块，它提供了数据库客户端的接口供您访问。本文主要讲述Python操作MySQL数据库，通过调用MySQLdb扩展包实现。而在安装过程中会遇到各种各样的错误，这里给出安装MySQL库的详细步骤及错误的解决方法。




**第一步：安装Python程序**

首先安装Python编程环境python-2.7.8.amd64.msi，如下图所示：

![](https://img-blog.csdn.net/2018042508563045)

![](https://img-blog.csdn.net/20180425085653180)


默认安装在C:\python27路径，接下来我需要安装pip工具。



**第二步：安装pip工具**

pip是一个现代的、通用的Python包管理工具，提供了对Python包（Package）的查找、下载、安装及卸载功能。Python可以通过easy_install或者pip命令安装各种各样的包，其中easy_insall提供了“傻瓜式”的在线一键安装模块的方式，而pip是easy_install的改进版，提供更好的提示信息以及下载、卸载Python包等功能，常见用法如下表所示。


![](https://img-blog.csdn.net/20180425090251525)


在官网 http://pypi.python.org/pypi/pip#downloads 可以下载pip软件，这里我直接双击“get-pip.py”文件进行安装，如下图所示，该文件下载地址。


![](https://img-blog.csdn.net/20180425142227330)

安装成功之后，在Python27文件夹下生成一个Scripts文件夹，包含了pip和easy_install工具，如下图所示，我们可以调用"pip install xxx"命令安装扩展包。

![](https://img-blog.csdn.net/20180425142350344)






第三步：安装本地MySQL数据库


接下来安装本地MySQL数据库，这里下载mysql-5.0.96-winx64安装，如下图所示：

![](https://img-blog.csdn.net/20180425142754464)






**第四步：调用pip工具安装MySQL库报错**

接下来调用打开cmd命令行，去到"C:\python27\Scripts"路径安装mysql库。
    C:\Users\yxz>cd..
    C:\Users>cd..
    C:\>cd C:\Python27\Scripts
    C:\Python27\Scripts>**pip install mysql**


![](https://img-blog.csdn.net/20180425143348297)
但此时报错如下：
error: Microsoft Visual C++ 9.0 is required. Get it from http://aka.ms/vcpython27
Command "C:\Python27\python.exe -u -c "import setuptools, tokenize;__file__='c:\\users


![](https://img-blog.csdn.net/2018042514333614)

这是驱动问题，这里我下载VCForPython27.msi软件安装。




**第五步：安装VCForPython27.msi**

双击软件即可安装，如下图所示：


![](https://img-blog.csdn.net/20180425145500700)

安装过程如下图所示：


![](https://img-blog.csdn.net/20180425145539431)



安装成功之后，再调用"pip install mysql"命令安装扩展包，此时报错：
_mysql.c(42) : fatal error C1083: Cannot open include file: 'config-win.h': No such file or directory
error: command 'C:\\Users\\yxz\\AppData\\Local\\Programs\\Common\\Microsoft\\Visual C++ for Python\\9.0\\VC\\Bin\\amd64\\cl.exe' failed with exit status 2


![](https://img-blog.csdn.net/20180425145837305)


其原因是安装MySQL时没有安装C语言库。一方面可以重新运行MySQL的安装程序，选择Modify，把“C Include Files / Lib Files”勾选上，并安装；另一方面，还是驱动问题，安装MySQL-python-1.2.3.win-amd64-py2.7.exe文件即可。




第六步：安装MySQL-python软件

下载一个MySQL-python-1.2.3.win-amd64-py2.7.exe文件进行安装，如下图所示：
官网地址：[https://pypi.python.org/pypi/MySQL-python/](https://pypi.python.org/pypi/MySQL-python/)
下载地址：[http://download.csdn.net/detail/eastmount/9598651](http://download.csdn.net/detail/eastmount/9598651)

![](https://img-blog.csdn.net/2018042515015038)


安装过程如下图所示：

![](https://img-blog.csdn.net/2018042515022376)


![](https://img-blog.csdn.net/20180425150243172)







第七步：pip install mysql安装MySQL库

接下来可以成功安装MySQL数据库，如下图所示：

```python
C:\Python27\Scripts>pip install mysql
Collecting mysql
  Using cached https://files.pythonhosted.org/packages/06/ef/c4efbf2a51fb46aba9be03a973638d9539c9ca10a5259b2cbb1a66133b2e/
mysql-0.0.1.tar.gz
Requirement already satisfied: MySQL-python in c:\python27\lib\site-packages (from mysql) (1.2.3)
Installing collected packages: mysql
  Running setup.py install for mysql ... done
Successfully installed mysql-0.0.1

C:\Python27\Scripts>
```

![](https://img-blog.csdn.net/20180425150515452)

最后测试MySQL库代码，注意导入MySQLdb库，再进行使用。
详见前文：[[python] 专题九.Mysql数据库编程基础知识](https://blog.csdn.net/eastmount/article/details/52156383)

```python
import MySQLdb
 
try:
    conn=MySQLdb.connect(host='localhost',user='root',passwd='123456',port=3306)
    cur=conn.cursor()
    res = cur.execute('show databases')
    print res
    for data in cur.fetchall():
        print '%s' % data
    cur.close()
    conn.close()
except MySQLdb.Error,e:
     print "Mysql Error %d: %s" % (e.args[0], e.args[1])
```

输出结果如下图所示：

![](https://img-blog.csdn.net/201804251509361)







希望这篇文章对您有所帮助，如果有错误或不足之处，还请海涵~同时，这是我回学校写的第一篇文章，加油。
(By:Eastmount 2018-04-25 下午3点半[http://blog.csdn.net/eastmount/](http://blog.csdn.net/eastmount/))









