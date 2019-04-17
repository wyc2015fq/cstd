# Linux C 编程内存泄露检测工具(一)：mtrace - DoubleLi - 博客园






## 前言

所有使用动态内存分配(dynamic memory allocation)的程序都有机会遇上内存泄露(memory leakage)问题，在Linux里有三种常用工具来检测内存泄露的情況，包括：
- mtrace
- dmalloc
- memwatch

## 1. mtrace

mtrace是三款工具之中是最简单易用的，mtrace是一个C函數，在<mcheck.h>里声明及定义，函数原型为：

    void mtrace(void);



其实mtrace是类似malloc_hook的malloc handler，只不过mtrace的handler function已由系统为你写好，但既然如此，系统又怎么知道你想将malloc/free的记录写在哪里呢？为此，调用mtrace()前要先设置MALLOC_TRACE环境变量：

    #include <stdlib.h>
    ....
    setenv("MALLOC_TRACE", "output_file_name", 1);
    ...



「output_file_name」就是储存检测结果的文件的名称。

但是检测结果的格式是一般人无法理解的，而只要有安装mtrace的话，就会有一名为mtrace的Perl script，在shell输入以下指令：

    mtrace [binary] output_file_name



就会将output_file_name的內容转化成能被理解的语句，例如「No memory leaks」，「0x12345678 Free 10 was never alloc」诸如此类。

例如以下有一函数：(暂且放下single entry single exit的原则)

    #include <stdio.h>
    #include <stdlib.h>
    #include <errno.h>
    #include <mcheck.h>
    int main() {
        char *hello;

        setenv("MALLOC_TRACE", "output", 1);
        mtrace();
        if ((hello = (char *) malloc(sizeof(char))) == NULL) {
            perror("Cannot allocate memory.");
            return -1;
        }

        return 0;
    }



执行后，再用mtrace 将结果输出：

    - 0x08049670 Free 3 was never alloc'd 0x42029acc
    - 0x080496f0 Free 4 was never alloc'd 0x420dc9e9
    - 0x08049708 Free 5 was never alloc'd 0x420dc9f1
    - 0x08049628 Free 6 was never alloc'd 0x42113a22
    - 0x08049640 Free 7 was never alloc'd 0x42113a52
    - 0x08049658 Free 8 was never alloc'd 0x42113a96

    Memory not freed:
    -----------------
       Address     Size     Caller
    0x08049a90      0x1  at 0x80483fe



最后一行标明有一个大小为1 byte的内存尚未释放，大概是指「hello」吧。


    若我们把该段内存释放：

    #include <stdio.h>
    #include <stdlib.h>
    #include <errno.h>
    #include <mcheck.h>
    int main() {
        char *hello;

        setenv("MALLOC_TRACE", "output", 1);
        mtrace();
        if ((hello = (char *) malloc(sizeof(char))) == NULL) {
            perror("Cannot allocate memory.");
            return -1;
        }

        free(hello);
        return 0;
    }



结果如下：

    - 0x080496b0 Free 4 was never alloc'd 0x42029acc
    - 0x08049730 Free 5 was never alloc'd 0x420dc9e9
    - 0x08049748 Free 6 was never alloc'd 0x420dc9f1
    - 0x08049668 Free 7 was never alloc'd 0x42113a22
    - 0x08049680 Free 8 was never alloc'd 0x42113a52
    - 0x08049698 Free 9 was never alloc'd 0x42113a96
    No memory leaks.



mtrace的原理是记录每一对malloc-free的执行，若每一个malloc都有相应的free，则代表没有内存泄露，对于任何非malloc/free情況下所发生的内存泄露问题，mtrace并不能找出来。









