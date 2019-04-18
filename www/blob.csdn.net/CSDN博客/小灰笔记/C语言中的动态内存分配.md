# C语言中的动态内存分配 - 小灰笔记 - CSDN博客





2017年08月08日 23:28:37[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：197
个人分类：[C																[嵌入式](https://blog.csdn.net/grey_csdn/article/category/6639472)](https://blog.csdn.net/grey_csdn/article/category/6639462)








       C语言中的动态内存分配应该是基础的C语言知识之一，印象中大学的时候这是让我考试头疼的地方。工作这么多年，跟C语言不断打交道。说真的，这个东西我依然不是很熟悉。因为我现在工作的行业领域中这东西还真就是用到的不多！

       今天算是对基础知识的回顾，对这个功能进行小结一下。其实，在标准C中用到的就是两个函数，一个是malloc，还有一个是free。分别是对内存的分配以及用完之后的内存回收。

       Malloc的使用方式是：指针名=（数据类型*）malloc（长度），一段示范代码如下：

#include<stdio.h>

#include<unistd.h>

#include<string.h>

#include"stdlib.h"



int file_exists(const char *dirpath, const char *filename)

{

       size_t len = 1+ strlen (dirpath) + 1+strlen (filename);

       char * filepath = (char *) malloc (len);

       int result;

       if (filepath == 0)

       perror ("malloc");

       sprintf (filepath, "%s/%s",dirpath, filename);

       /* Invert the return status of access tobehave like a boolean. */

       result = 1+ access (filepath, F_OK);

       free (filepath);

       return result;

}



int main(void)

{

       if(file_exists("./","mem.c"))

       {

              printf("fileexists!\n");

       }

       return 0;

}

       这里面使用malloc分配了能够容纳两个字符串拼接后的结果的内存空间，然后最后用于存储两个字符串的拼接结果。在内存中的内容使用结束之后，使用free进行了内存的释放。说道字符串的拼接，看起来我之前的一个示范代码写的有一点问题。但是，不考虑末尾的结束符号居然也获得了我想要的结果，这也是让我觉得很奇怪的。其实，当初写代码的时候倒是真考虑到了这一段，只是没有去落实以及测试。

       字符串拼接完成后，新得字符串是带有完整目录的文件名称。整个代码的功能是用于实现文件存在性的检测。代码的编译以及执行结果如下：

E:\01_workSpace\02_programme_language\01_clang\2017\08\08>gccmem.c



E:\01_workSpace\02_programme_language\01_clang\2017\08\08>a

file exists!




