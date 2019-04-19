# PL/SQLDeveloper配置使用说明 - xqhrs232的专栏 - CSDN博客
2017年10月18日 08:46:24[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：322
个人分类：[SQLServer/Oracle数据库技术](https://blog.csdn.net/xqhrs232/article/category/906969)
原文地址::[https://www.2cto.com/database/201509/442590.html](https://www.2cto.com/database/201509/442590.html)
PL/SQL Developer是一个集成开发环境，专门面向[Oracle](https://www.2cto.com/database/Oracle/)[数据库](https://www.2cto.com/database/)存储程序单元的开发。如今，有越来越多的商业逻辑和应用逻辑转向了Oracle
 Server，因此，PL/SQL[编程](https://www.2cto.com/kf)也成了整个开发过程的一个重要组成部分。PL/SQL Developer侧重于易用性、代码品质和生产力，充分发挥Oracle应用[程序开发](https://www.2cto.com/kf)过程中的主要优势。
**以下测试环境为 windows 7 32位。**
PL/SQL Developer绿色版 下载链接：https://pan.baidu.com/s/1hqwDToo
Oracle9i客户端精简版下载链接：https://pan.baidu.com/s/1mgL8pXu
**详细配置说明：**
1）解压 PLSQLDeveloper.zip，此为绿色包，解压即可用：
![\](https://www.2cto.com/uploadfile/Collfiles/20150915/201509151002481.jpg)
2）进入解压后的文件夹，plsqldev.exe 即为其可执行程序，暂时不能使用，还得相应配置，请看后面介绍：
![\](https://www.2cto.com/uploadfile/Collfiles/20150915/201509151002492.jpg)
3）解压 Oracle9i客户端精简版.rar ：
![\](https://www.2cto.com/uploadfile/Collfiles/20150915/201509151002493.jpg)
4）解压后，双击安装其中的安装包，默认只能安装在 C 盘：
![\](https://www.2cto.com/uploadfile/Collfiles/20150915/201509151002494.jpg)
5）安装成功，C 盘多出一个Oracle文件夹：
![\](https://www.2cto.com/uploadfile/Collfiles/20150915/201509151002495.jpg)
6）给 oracle 客户端设置 Path 环境变量：
a)先拷贝需要设置环境变量的那个路径：
![\](https://www.2cto.com/uploadfile/Collfiles/20150915/201509151002496.jpg)
b)右击电脑 -> 属性 -> 高级[系统](https://www.2cto.com/os/)设置 -> 高级 -> 环境变量：
![\](https://www.2cto.com/uploadfile/Collfiles/20150915/201509151002497.jpg)
![\](https://www.2cto.com/uploadfile/Collfiles/20150915/201509151002498.jpg)
![\](https://www.2cto.com/uploadfile/Collfiles/20150915/201509151002499.jpg)
![\](https://www.2cto.com/uploadfile/Collfiles/20150915/2015091510024910.jpg)
Path 开头添加如下路径，路径和路径之间以分号分割：
![\](https://www.2cto.com/uploadfile/Collfiles/20150915/2015091510024911.jpg)
7)配置 PL/SQL Developer，打开时会跳出一个登陆窗口，取消即可，然后跳进一个 Developer 界面（没有登录）：
![\](https://www.2cto.com/uploadfile/Collfiles/20150915/2015091510024912.png)
![\](https://www.2cto.com/uploadfile/Collfiles/20150915/2015091510025013.jpg)
8）工具 -> 首选项：
> 
> 
Oracle 主目录名 -> C:\Oracle\ora90\BIN
> 
OCI 库 -> C:\Oracle\ora90\BIN\oci.dll
![\](https://www.2cto.com/uploadfile/Collfiles/20150915/2015091510025014.jpg)
![\](https://www.2cto.com/uploadfile/Collfiles/20150915/2015091510025015.jpg)
9）应用 -> 确定 -> 重新运行软件，发现比之前的登录界面多了一个选项“连接为”：
![\](https://www.2cto.com/uploadfile/Collfiles/20150915/2015091510025016.jpg)
10）添加数据库服务器信息，在 tnsnames.ora 配置文件，添加服务器信息：
![\](https://www.2cto.com/uploadfile/Collfiles/20150915/2015091510025017.jpg)
11）本示例中，在 tnsnames.ora 的最后添加如下内容：
[?](https://www.2cto.com/database/201509/442590.html#)
```
```
ORA90
 =
```
`  `
```
(DESCRIPTION
 =
```
`    `
```
(ADDRESS_LIST
 =
```
`      `
```
(ADDRESS
 = (PROTOCOL = TCP)(HOST = 192.168.161.90)(PORT = 1521))
```
`    ``)`
`    `
```
(CONNECT_DATA
 =
```
`      `
```
(SERVICE_NAME
 = ora11)
```
`    ``)`
`  ``)`
```
![加载中...](http://statics.2cto.com/images/s_nopic.gif)
12）登录数据库服务器，填写登录信息：
![\](https://www.2cto.com/uploadfile/Collfiles/20150915/2015091510025018.jpg)
13）成功登录后的操作界面：
![\](https://www.2cto.com/uploadfile/Collfiles/20150915/2015091510025019.jpg)
![\](https://www.2cto.com/uploadfile/Collfiles/20150915/2015091510025020.jpg)

