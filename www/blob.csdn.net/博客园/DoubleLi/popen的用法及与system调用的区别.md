# popen的用法及与system调用的区别 - DoubleLi - 博客园






首先用man查看下popen的介绍：



# popen(3) - Linux man page

## Name



popen, pclose - pipe stream to or from a process

## Synopsis
**#include <[stdio.h](http://linux.die.net/include/stdio.h)>FILE *popen(const char ****command***, const char ****type***);int pclose(FILE ****stream***);**


popen总是和pclose一起出现被使用的。popen() 创建一个管道，通过fork或者invoke一个子进程,然后执行command。返回值在标准IO流中，由于是在管道之中，因此数据流是单向的，command只能产生stdout或者读取stdin，因此type只有两个值：‘w’或‘r’。r表示command从管道中读取数据流，而w表示command的stdout输出到管道中。command无法同时读取和输出。popen返回该FIFO数据流的指针。

举例用法（http://linux.chinaitlab.com/c/806015.html）:



　　管道读：先创建一个文件test,然后再test文件内写入“Read pipe successfully !”

　　#include “stdio.h”

　　#include “stdlib.h”

　　int main()

　　{

　　FILE *fp;

　　char buf[200] = {0};

　　if((fp = popen(“cat test”, “r”)) == NULL) {

　　perror(“Fail to popen\n”);

　　exit(1);

　　}

　　while(fgets(buf, 200, fp) != NULL) {

　　printf(“%s”, buf);

　　}

　　pclose(fp);

　　return 0;

　　}

　　打印输出： Read pipe successfully !

　　管道读：

　　#include “stdio.h”

　　#include “stdlib.h”

　　int main()

　　{

　　FILE *fp;

　　char buf[200] = {0};

　　if((fp = popen(“cat > test1″, “w”)) == NULL) {

　　perror(“Fail to popen\n”);

　　exit(1);

　　}

　　fwrite(“Read pipe successfully !”, 1, sizeof(“Read pipe successfully !”), fp);

　　pclose(fp);

　　return 0;

　　}

　　执行完毕后，当前目录下多了一个test1文件，打开，里面内容为Read pipe successfully !

---------------------------------------------------------------------------------------------

对于管道读例子已经很清楚，而管道写可能用的地方比较少。而对于写可能更常用的是system函数：

system("cat "Read pipe successfully!" > test1")

可以看出，popen可以控制程序的输入或者输出，而system的功能明显要弱一点，比如无法将ls的结果用到程序中。如果不需要使用到程序的I/O数据流，那么system是最方便的。

而且system函数是C89和C99中标准定义的，可以跨平台使用。而popen是Posix 标准函数，可能在某些平台无法使用（windows应该是可以的吧，没做过测试）。

如果上述两个函数还无法满足你的交互需求，那么可以考虑exec函数组了。









