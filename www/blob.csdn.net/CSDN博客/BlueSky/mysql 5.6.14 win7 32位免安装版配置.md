# mysql 5.6.14 win7 32位免安装版配置 - BlueSky - CSDN博客
2015年01月27日 22:01:05[ASIA_kobe](https://me.csdn.net/ASIA_kobe)阅读数：838
个人分类：[java编程](https://blog.csdn.net/ASIA_kobe/article/category/2863525)

下载安装mysql用了很长时间，这篇文章还是有很好地指导性。
1.下载mysql 5.6.14；下载地址:http://cdn.mysql.com/Downloads/MySQL-5.6/mysql-5.6.14-win32.zip
2.解压到自定义目录，我的是D:\ProgramFiles\mysql5614win32\bin
3.复制根目录下的my-default.ini,改名为my.ini,my.ini用下面内容替换
[client]
port=3306
default-character-set=utf8
#客户端字符类型，与服务端一致就行，建议utf8
[mysqld]
port=3306
character_set_server=utf8
#服务端字符类型，建议utf8
basedir=D:\ProgramFiles\mysql5614win32
#解压根目录
datadir=D:\ProgramFiles\mysql5614win32\data
#解压根目录\data
sql_mode=NO_ENGINE_SUBSTITUTION,STRICT_TRANS_TABLES
[WinMySQLAdmin]
D:\ProgramFiles\mysql5614win32\bin\mysqld.exe 
#解压根目录\bin\mysqld.exe
4.环境变量配置
你的解压目录 
MYSQL_HOME  D:\ProgramFiles\mysql5614win32
path后面加上%MYSQL_HOME%\bin;
5.找到C:\Windows\System32\cmd.exe,右键-以管理员身份运行(很重要，否则有权限问题，会报错：Install/Remove of the Service Denied!)，进入mysql解压目录bin子目录下，输入mysqld install MySQL --defaults-file="你的解压根目录\my.ini",提示：
Service successfully installed.
6.启动、停止、移出MYSQL服务
  启动MYSQL服务：net start mysql
  停止MYSQL服务：net stop mysql
  移出mysql服务：mysqld-nt --remove
7.修改root密码：
运行中输入cmd ，跳到安装目录/bin下,
D:\ProgramFiles\mysql5614win32\bin>mysql -uroot
D:\ProgramFiles\mysql5614win32\bin>show databases; 
D:\ProgramFiles\mysql5614win32\bin>use mysql;
D:\ProgramFiles\mysql5614win32\bin>UPDATE user SET password=PASSWORD("root") WHERE user='root';
D:\ProgramFiles\mysql5614win32\bin>FLUSH PRIVILEGES; 
D:\ProgramFiles\mysql5614win32\bin>QUIT
FLUSH PRIVILEGES:强制让MySQL重新加载权限，即刻生效
