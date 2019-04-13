
# PLSQL developer 连接不上64位Oracle 解决办法 - anlcy - 博客园






# [PLSQL developer 连接不上64位Oracle 解决办法](https://www.cnblogs.com/camilla/p/7119709.html)
在64位Windows7上安装Oracle后，用PLSQL developer去连接数据库出现报错：
Could not load "……\bin\oci.dll"
OCIDLL forced to……
LoadLibrary（……oci.dll） returned 0
原因：
oci.dll是64位的，32位应用程序PLSQL Developer自然无法加载了，而且PLSQL Developer只支持32位，所以如果非要64位Oracle，我们只有想别的办法解决了；
**解决方法一：**
1、下载instantclient_32位客户端，加压后存放，如D:\ProgramFiles\instantclient
2、拷贝Oracle11.2G的msvcr80.dll和tnsnames.ora到解压的instantclient_32位客户端根目录，它们的位置分别是：
D:\ProgramFiles\Crystal\product\11.2.0\dbhome_1\BIN和D:\ProgramFiles\Crystal\product\11.2.0\dbhome_1\NETWORK\ADMIN
3、设置PLSQL Developer中设置Oracle_Home和OCI Library，
4、在PLSQL Developer目录下创建bat文件，作为PLSQL Developer的启动快捷方式替换PLSQL Developer传统启动方式，bat文件中的内容如下：
@echo off
set path=D:\ProgramFiles\instantclient
set ORACLE_HOME=D:\ProgramFiles\instantclient
set TNS_ADMIN=D:\ProgramFiles\instantclient
set NLS_LANG=AMERICAN_AMERICA.AL32UTF8
start plsqldev.exe
至此设置完成，经实验PLSQL Developer的可以成功连接到Oracle 10g64位





