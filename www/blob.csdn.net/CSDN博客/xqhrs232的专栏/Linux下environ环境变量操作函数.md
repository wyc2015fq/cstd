# Linux下environ环境变量操作函数 - xqhrs232的专栏 - CSDN博客
2017年03月07日 15:52:41[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：536
原文地址::[http://www.linuxidc.com/Linux/2014-10/108580.htm](http://www.linuxidc.com/Linux/2014-10/108580.htm)
相关文章
1、[Linux 系统启动文件，bash 启动文件，设置环境变量的位置](http://blog.csdn.net/feng27156/article/details/37505041)----[http://blog.csdn.net/feng27156/article/details/37505041](http://blog.csdn.net/feng27156/article/details/37505041)
**Linux下environ环境变量操作函数**
#include <stdio.h>
int main(int argc,char *argv[],char **envptr)
{
  int i=0;
  for(i=0; envptr[i]!=NULL; i++)
   printf("%s\n",envptr[i]);
  return 0;
}
main函数是程序的入口函数,int main(int argc,char *argv[]);
argc是程序参数的个数,argv保存参数
与下边的程序相同的功能
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
extern char **environ;
int main(int argc, char **argv)
{
  char **env = environ;
  while(*env){
   printf("%s\n",*env);
   env++;
  }
  printf("HOME: %s\n", getenv("HOME"));
  return 0;
}
C程序中环境变量相关的函数如下
#include <stdlib.h>
char *getenv(const char *name);
int setenv(const char *name, const char *value, int overwrite);
int putenv(char *string);
int unsetenv(const char *name);
int clearenv(void);
参考 man 7 environ
environ全局变量保存用户环境,以上函数可以获取、设置、删除、清空环境变量
响应的shell命令如下
echo $PATH 打印PATH环境变量
export PATH=$PATH:./ 设置环境变量
unset PATH 删除环境变量
env 显示所有环境变量
**本文永久更新链接地址**：[http://www.linuxidc.com/Linux/2014-10/108580.htm](http://www.linuxidc.com/Linux/2014-10/108580.htm)
