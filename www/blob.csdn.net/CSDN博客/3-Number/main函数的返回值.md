
# main函数的返回值 - 3-Number - CSDN博客


2017年12月20日 15:24:15[庞叶蒙](https://me.csdn.net/pangyemeng)阅读数：2685


# 0x01 缘由
最近在面试过程中，面试官都会问些C语言容易忽视的地方，如main函数返回值的作用等。这些细节还是得学习下，虽然在实践工作中的使用会较少。
# 0x02 介绍
程序运行过程入口点main函数，main（）函数返回值类型必须是int，这样返回值才能传递给程序激活者（如操作系统）表示程序正常退出。main（int args, char **argv） 参数的传递。参数的处理，一般会调用getopt（）函数处理，但实践中，这仅仅是一部分，不会经常用到的技能点。
# 0x03 代码实践点
\#include <stdio.h>
void main()
{
printf("test\n");
}
gcc -o main mian.c
用gcc编译，无警告；
g++ -o main main.c
错误直接：
![](https://img-blog.csdn.net/20171220152521818)
查看返回值：
如下代码：
例程1：
\#include <stdio.h>
int main(int agrc, char **argv)
{
printf("test\n");
return  1;

}
例程2
\#include <stdio.h>
int main(int agrc, char **argv)
{
printf("test\n");
return -1;
}
![](https://img-blog.csdn.net/20171220152550888)
第二例子返回为255，说明返回一个无符号类型。
通过gcc编译后运行 通过echo 显示 $? （代表上一个命令执行后的退出状态） 得到上一个程序运行后的返回值。
# 0x04 总结
目前还没遇到要利用main函数返回值去做一些实践的情况，了解下。



