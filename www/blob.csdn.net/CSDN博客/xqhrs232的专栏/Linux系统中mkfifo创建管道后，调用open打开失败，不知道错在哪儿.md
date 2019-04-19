# Linux系统中mkfifo创建管道后，调用open打开失败，不知道错在哪儿 - xqhrs232的专栏 - CSDN博客
2016年12月14日 11:32:26[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：2055
原文地址::[https://zhidao.baidu.com/question/582458579522299805.html](https://zhidao.baidu.com/question/582458579522299805.html)
相关文章
1、linux下为什么创建管道成功,打开管道却失败呢----[https://zhidao.baidu.com/question/454079361.html](https://zhidao.baidu.com/question/454079361.html)
进程间通信之[FIFO](https://www.baidu.com/s?wd=FIFO&tn=44039180_cpr&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1YYuWDsuHb1m1RvuW6smvmL0ZwV5Hcvrjm3rH6sPfKWUMw85HfYnjn4nH6sgvPsT6KdThsqpZwYTjCEQLGCpyw9Uz4Bmy-bIi4WUvYETgN-TLwGUv3EPH6zPjR3PHT4PHcznWb4rj0d)，在阻塞模式下，只有当读和写模式都打开时才返回，否则一直阻塞；
非阻塞模式下，当读端没打开，则打开写端无效，返回错误。
建议你读一读UNP卷2。
- 
创建 [fifo](https://www.baidu.com/s?wd=fifo&tn=44039180_cpr&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1YYuWDsuHb1m1RvuW6smvmL0ZwV5Hcvrjm3rH6sPfKWUMw85HfYnjn4nH6sgvPsT6KdThsqpZwYTjCEQLGCpyw9Uz4Bmy-bIi4WUvYETgN-TLwGUv3EPH6zPjR3PHT4PHcznWb4rj0d) 时，写
 0777 (8进制的 777) 
- 
你用了 nonblock 方式去打开[fifo](https://www.baidu.com/s?wd=fifo&tn=44039180_cpr&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1YYuWDsuHb1m1RvuW6smvmL0ZwV5Hcvrjm3rH6sPfKWUMw85HfYnjn4nH6sgvPsT6KdThsqpZwYTjCEQLGCpyw9Uz4Bmy-bIi4WUvYETgN-TLwGUv3EPH6zPjR3PHT4PHcznWb4rj0d)的写端，此时fifo的读端没有人open，所以一定会返回失败。必须先有人以读的方式打开fifo后，才能以nonblock方式打开写。或者去掉
 O_NONBLOCK 参数。
//==============================================================================================
代码贴出来看看呢？
打开管道失败，一般要注意打开的方式，一般不能以读写方式打开，要么只读打开，要么只写打开，否则会出错。
如果以O_NONBLOCK 非阻塞方式打开的话，管道的另一端也要同时要打开。
read.c
#include<sys/types.h>
#include<sys/stat.h>
#include<errno.h>
#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h> 
//fifo所在目录
#define FIFO_SERVER "/tmp/myfifo"
int main(int argc,char** argv)
{
       char r_buf[100];//用来存放管道读出数据
       int fd;
       int nread;
写不下了啊。
追答
你试试这种方法呢，我是用的这个，一直很好。
写端：
1，建立管道：
mkfifo(FIFO_FILE,0777);
2，打开管道：
fd = open("/tmp/myfifo", O_WRONLY);
3，写数据：
write(fd ,“Love Linux!”,11);
 
读端：
1，打开管道：
fd = open("/tmp/myfifo", O_RDONLY);
2，读数据：
read(fd ,r_buf,11);
