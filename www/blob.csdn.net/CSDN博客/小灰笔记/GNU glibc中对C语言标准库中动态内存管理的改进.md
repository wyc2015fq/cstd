# GNU  glibc中对C语言标准库中动态内存管理的改进 - 小灰笔记 - CSDN博客





2017年08月08日 23:44:15[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：159
个人分类：[C																[GNU](https://blog.csdn.net/grey_csdn/article/category/7086457)](https://blog.csdn.net/grey_csdn/article/category/6639462)








       先看一段前面展示过的代码：

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

       这段代码的功能是实现文件存在性的检测，其中用到了C语言的动态内存分配与回收。在使用了malloc之后，必须得有一次对free的匹配调用。否则就会造成内存被分配却无法使用的情况。而这种调用上成对儿出现的要求增加了编码的难度，也容易引入一些严重的问题。GNU glibc在某些方面上进行了改进，针对以上代码的修改如下：

#include<stdio.h>

#include<unistd.h>

#include<string.h>

#include"stdlib.h"

#include"malloc.h"



int file_exists(const char *dirpath, const char *filename)

{

       size_t len = 1+ strlen (dirpath) + 1+strlen (filename);

       char * filepath = (char *) alloca (len);

       int result;

       if (filepath == 0)

       perror ("alloca");

       sprintf (filepath, "%s/%s",dirpath, filename);

       /* Invert the return status of access tobehave like a boolean. */

       result = 1+ access (filepath, F_OK);

       /*

              free (filepath);

       */

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

       这样，在需要进行动态内存分配的时候只需要进行内存的分配而不需要关注内存的回收。在相应的内存使用结束之后，内存会自动被回收。代码的编译以及程序的执行结果如下：

E:\01_workSpace\02_programme_language\01_clang\2017\08\08>gccmem_gnu.c



E:\01_workSpace\02_programme_language\01_clang\2017\08\08>a

file exists!

       值得注意的是，虽说是这个功能存在于GNUglibc，但是很多移植后的版本中函数的声明文件可能会有不同，这个还是得根据实际的情况进行修改。




