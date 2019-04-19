# C程序调用shell脚本共有三种方法 - sxf_123456的博客 - CSDN博客
2017年04月25日 16:13:45[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：405
C程序调用shell脚本共有三种方法：
1、system()   #直接放入shell脚本路径
在c_call_shell.c文件
  1#include <stdio.h>
  2 #include <stdlib.h>
  3 #include <string.h>
  4 #include <unistd.h>
  5
  6 void c_call_shell(char *shell_path)
  7 {
  8  char str[]="/usr/sxf/MFS/4_25/test/shell.sh";
  9  system(shell_path);
 10  system(str);
 11 }
 12
 13 int main(int argc,char* argv[])
 14 {
 15  c_call_shell(argv[1]);
 16 }
shell.sh程序
 #!/bin/sh
  2 #shell.sh
  3
  4 echo "hello,world"
2、popen(char *command,char *type)
type为r或w
command 为shell脚本路径
popen()会调用fork()产生子进程，子进程调用/bin/sh -c来执行参数command指令。参数type可使用“r”代表读，“w”代表写。
popen()会建立管道连到子进程的标准输出设备或标准输入设备，然后返回一个文件指针，所有对文件指针的操作都可以用，除了fclose()外。
c_print_shell.c 
     #include <stdio.h>
  2 int main(int argc,char *argv[])
  3 {
  4   FILE *fp=NULL;
  5   char buffer[80];
  6   fp=popen("/usr/sxf/MFS/4_25/test/shell.sh","r");
  7   fgets(buffer,sizeof(buffer),fp);
  8   printf("%s\n",buffer);
  9   pclose(fp);
 10
 11   return 0;
 12 }
shell.sh
 #!/bin/sh
  2 #shell.sh
  3
  4 echo "hello,world"
