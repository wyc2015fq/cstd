# MYSQL数据库环境搭建 - xqhrs232的专栏 - CSDN博客
2018年10月18日 16:38:44[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：37
个人分类：[JavaWeb技术](https://blog.csdn.net/xqhrs232/article/category/8231541)
原文地址::[https://blog.csdn.net/x83853684/article/details/80688346](https://blog.csdn.net/x83853684/article/details/80688346)
相关文章
1、MySQL服务无法启动—系统错误1067----[https://blog.csdn.net/xiao714041/article/details/79101263](https://blog.csdn.net/xiao714041/article/details/79101263)
2、[如何登录mysql？ cmd怎么连接mysql数据库](https://www.cnblogs.com/longshiyVip/p/4854953.html)----[https://www.cnblogs.com/longshiyVip/p/4854953.html](https://www.cnblogs.com/longshiyVip/p/4854953.html)
3、[Navicat for MySQL下载、安装与破解](https://www.cnblogs.com/da19951208/p/6403607.html)----[https://www.cnblogs.com/da19951208/p/6403607.html](https://www.cnblogs.com/da19951208/p/6403607.html)
4、Navicat for MySQL 安装和破解（完美）----[https://blog.csdn.net/wypersist/article/details/79834490](https://blog.csdn.net/wypersist/article/details/79834490)
5、[Navicat for MySQL](http://www.navicat.com.cn/products/navicat-for-mysql/)----[http://www.navicat.com.cn/products/navicat-for-mysql](http://www.navicat.com.cn/products/navicat-for-mysql)
6、MySQL数据库安装和Navicat for MySQL配合使用----[https://blog.csdn.net/dickyqie/article/details/78748675](https://blog.csdn.net/dickyqie/article/details/78748675)
一、MYSQL下载地址  https://downloads.mysql.com/archives/community/
二、下载解压安装包：将以下载的MySQL压缩包解压到自定义目录下,目录不包含中文
三、修改配置文件：
 将解压目录下默认文件 my-default.ini 拷贝一份，改名 my.ini
   [client]
    port=3306
    default-character-set=utf8
    [mysqld]
    port=3306
    character_set_server=utf8
    basedir= E:\mysql-5.6.34-win32\bin\mysqld.exe
    #解压目录
    datadir= E:\mysql-5.6.34-win32\bin\mysqld.exedata
    #解压目录下data目录
    sql_mode=NO_ENGINE_SUBSTITUTION,STRICT_TRANS_TABLES
    [WinMySQLAdmin]
    E:\mysql-5.6.34-win32\bin\mysqld.exe
红色字体为你解压安装的根目录。
四、添加系统环境变量
1、新建系统变量
点击系统变量下的新建按钮
      输入变量名：MYSQL_HOME
      输入变量值：D:\Program Files\mysql-5.6.11-winx64    这是你自己解压的目录
      #即为mysql的自定义解压目录。
2、系统变量path
点击系统变量，选择path，在路径后面添加 ；%MYSQL_HOME%
五、安装MYSQL
 1）从控制台进入到MySQL解压目录下的 bin 目录下：
    2）输入服务安装命令：
    mysqld install MySQL     安装成功后会提示服务安装成功。
    #移除服务命令为：mysqld remove（不需要的时候删除服务，刚安装时不需要）
六、启动MYSQL服务
方法一：
        启动服务命令为：net start mysql
    方法二：
        打开管理工具 服务，找到MySQL服务。
        通过右键选择启动或者直接点击左边的启动来启动服务。
七、修改数据库密码
  刚安装完成时root账号默认密码为空，此时可以将密码修改为指定的密码。如：123456
    c:>mysql –uroot
    mysql>show databases;
    mysql>use mysql;
    mysql>UPDATE user SET password=PASSWORD("123456") WHERE user='root';
    mysql>FLUSH PRIVILEGES;
    mysql>QUIT
--------------------- 
作者：WAIT Y 
来源：CSDN 
原文：https://blog.csdn.net/x83853684/article/details/80688346 
版权声明：本文为博主原创文章，转载请附上博文链接！
