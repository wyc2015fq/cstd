# linux 下链接mysql练习 - writeeee的专栏 - CSDN博客
2016年03月14日 21:17:07[writeeee](https://me.csdn.net/writeeee)阅读数：283
编译命令如下 
*hzz@hzz-pc:~/code/testCode$* g++ -I /usr/include/mysql/ /home/hzz/code/testCode/connect2.c -L /usr/lib/mysql -lmysqlclient -o connection2 
*hzz@hzz-pc:~/code/testCode$*  ./connection2 
```cpp
#include <stdlib.h>
#include <stdio.h>
#include "mysql.h"
int main(int argc,char *arg[])
{
  MYSQL myconnection;
  mysql_init(&myconnection);
  if(mysql_real_connect(&myconnection,"localhost","test","test","testDb1",0,NULL,0))
    {
      printf("connection success");
      mysql_close(&myconnection);
    }
  else
    {
      fprintf(stderr,"connection failed\n");
      if(mysql_errno(&myconnection))
      {
          fprintf(stderr,"Connection error %d %s",mysql_errno(&myconnection),mysql_error(&myconnection));
      }
     }
 return 0;
}
```
