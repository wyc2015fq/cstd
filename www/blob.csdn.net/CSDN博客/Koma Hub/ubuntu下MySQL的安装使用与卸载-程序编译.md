# ubuntu下MySQL的安装使用与卸载-程序编译 - Koma Hub - CSDN博客
2017年12月16日 15:19:47[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：170
**１.安装：[原文链接](http://www.linuxidc.com/Linux/2016-07/133128.htm)**
在ubuntu下安装超级简单：
```
1.sudo apt-get install mysql-server
2.sudo apt-get install mysql-client
3.sudo apt-get install libmysqlclient-dev
```
安装过程中会提示输入密码并确认（记住这个密码）。
检查是否安装成功：
`sudo netstat -tap | grep mysql`我的显示为：（主要看这个LISTEN）：
```
rongtao@rongtao:test$ sudo netstat -tap | grep mysql
[sudo] rongtao 的密码： 
tcp        0      0 localhost:mysql         *:*                     LISTEN      5111/mysqld     
rongtao@rongtao:test$
```
使用：
`mysql -u root -p`-u 表示选择登陆的用户名， -p 表示登陆的用户密码，上面命令输入之后会提示输入密码，此时输入密码就可以登录到mysql。
**2.MySQL程序的编译：[原文链接](http://blog.csdn.net/y396397735/article/details/51891666)**
编写MySQL程序：
```cpp
#include <mysql/mysql.h>
#include <stdio.h>
#include <stdlib.h>
int main()
{
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    char server[] = "localhost";
    char user[] = "root";
    char password[] = "mima";
    char database[] = "mysql";
   
    conn = mysql_init(NULL);
   
    if (!mysql_real_connect(conn, server,user, password, database, 0, NULL, 0))
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(1);
    }
   
    if (mysql_query(conn, "show tables"))
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(1);
    }
   
    res = mysql_use_result(conn);
   
    printf("MySQL Tables in mysql database:\n");
   
    while ((row = mysql_fetch_row(res)) != NULL)
    {
        printf("%s \n", row[0]);
    }
   
    mysql_free_result(res);
    mysql_close(conn);
   
    printf("finish! \n");
    return 0;
}
```
编译四种方法：
```
1. gcc -o test test.c `mysql_config --cflags --libs`
2. gcc -o test test.c -I/usr/include/mysql -L/usr/lib/mysql -lmysqlclient -lz -lm
3. gcc -o test test.c -I /usr/include/mysql -L /usr/lib/mysql -l mysqlclient -lz -lm
4. g++ -o test test.cpp -lmysqlclient
```
**３．MySQL5.7在ubuntu下的彻底卸载[原文链接(不彻底）](http://www.linuxidc.com/Linux/2013-04/82934.htm)**
```
#删除mysql
1.sudo apt-get autoremove --purge mysql-server-5.5
2.sudo apt-get remove mysql-common
#清理残留数据
dpkg -l |grep ^rc|awk '{print $2}' |sudo xargs dpkg -P
```
**清理的比较彻底：[点击打开链接](http://blog.csdn.net/shihongliang1993/article/details/53768301)**
```
sudo apt purge mysql-*
sudo rm -rf /etc/mysql/ /var/lib/mysql
sudo apt autoremove
sudo apt autoclean
```
