
# 本机不安装Oracle客户端，使用PL-SQL Developer连接远程数据库 - anlcy - 博客园






# [本机不安装Oracle客户端，使用PL/SQL Developer连接远程数据库](https://www.cnblogs.com/camilla/p/7112448.html)
**附送PL/SQL Developer11中文版****下载地址**
1、先到[Oracle](http://lib.csdn.net/base/oracle)网站下载Instant Client :
[http://www.oracle.com/technetwork/database/features/instant-client/index-097480.html](http://www.oracle.com/technetwork/database/features/instant-client/index-097480.html)
根据你的[操作系统](http://lib.csdn.net/base/operatingsystem)选择不同的Instant Client版本
下载会是一个压缩文件,解压之后的文件夹叫:instantclient_12_1.放在你喜欢的目录即可.例如:E:\Program Files\PLSQL Developer\instantclient_12_1
2、在E:\Program Files\PLSQL Developer\instantclient_12_1目录下新建目录network,在network目录下再新建admin目录,在admin目录下新建文件tnsnames.ora,
![](http://blog.csdn.net/hr541659660/article/details/51683189)![](http://img.blog.csdn.net/20160615162752223?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
使用文本编辑器打开写入如下内容:
连接名 =
(DESCRIPTION =
(ADDRESS_LIST =
(ADDRESS = (PROTOCOL = TCP)(HOST =192.168.0.132 )(PORT = 1521))
)
(CONNECT_DATA =
(SERVICE_NAME =ORCL )
)
)
第一个黄色字块：表示定义远程服务器的在本地主机名（随意输入）
第二个黄色字块：远程[数据库](http://lib.csdn.net/base/mysql)的IP地址
第三个黄色子块：远程[数据库](http://lib.csdn.net/base/mysql)实例名
**注意几点：**
**SERVICE_NAME写成SID貌似也是可以的**
**所有的字符注意顶格写，不然可能有意想不到问题**

设置以下环境变量
ORACLE_HOME=D:\Oracle\instantclient_11_2
TNS_ADMIN=%ORACLE_HOME%\network\admin
NLS_LANG=SIMPLIFIED CHINESE_CHINA.ZHS16GBK
Path变量最前面加上%ORACLE_HOME%;
或者：
右击我的电脑选”属性”->“高级”->“环境变量”->”系统变量”
新增下面两个变量
TNS_ADMINE:\Program Files\PLSQL Developer\instantclient_12_1\network\admin
NLS_LANG 　　　SIMPLIFIED CHINESE_CHINA.ZHS16GBK
**安装PL/SQL Developer 及配置**下载地址：
[http://allroundautomations.com/](http://allroundautomations.com/)**特别注意，安装时不能安装到带有(x86)的文件夹中**

在PLSQL Developer中设置Oracle_Home和OCI Library：
Tools -> Preferences -> Oracle -> Connection：
Oracle_Home：D:\Oracle\instantclient_11_2
OCI Library：D:\Oracle\instantclient_11_2\oci.dll

PLSQL Developer 延长试用期办法，30天删除一次注册表
删除以下注册表内容：
HKEY_CURRENT_USER\Software\Allround Automations
HKEY_CURRENT_USER\Software\Microsoft\Security

**最后，不想配置环境变量的朋友，可以写个批处理**
可以在PL/SQL Developer所在目录建立一个.bat文件，内容如下（路径参照自己安装的客户端路径）：

@echo off
set ORACLE_HOME=D:\Oracle\instantclient_11_2
set PATH=%ORACLE_HOME%;%PATH%;
set TNS_ADMIN=%ORACLE_HOME%\network\admin
set NLS_LANG=SIMPLIFIED CHINESE_CHINA.ZHS16GBK
start plsqldev.exe
或者：
5、下载并安装PL.SQL.Developer配置应用
配置tools->preferences->connection
[oracle](http://lib.csdn.net/base/oracle)Home
E:\Program Files\PLSQL Developer\instantclient_12_1
OCI library
E:\Program Files\PLSQL Developer\instantclient_12_1\oci.dll
中文的PL/SQL Developer就是
工具-->首选项-->连接
Oracle主目录
E:\Program Files\PLSQL Developer\instantclient_12_1
OCI库
E:\Program Files\PLSQL Developer\instantclient_12_1\oci.dll
![](http://img.blog.csdn.net/20160615163108630?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](http://img.blog.csdn.net/20160615163016210?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


6、关闭PL/SQL Developer,重起Developer.
主机名就会出现在PL/SQL Developer的列表里，输入用户名密码，就可以登录远程oracle 11g数据库了。





