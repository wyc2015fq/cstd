# standard IO library---标准IO库函数定义在stdio.h中 - 三少GG - CSDN博客
2010年05月10日 02:07:00[三少GG](https://me.csdn.net/scut1135)阅读数：1250标签：[io																[stream																[file																[null																[linux																[library](https://so.csdn.net/so/search/s.do?q=library&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=file&t=blog)](https://so.csdn.net/so/search/s.do?q=stream&t=blog)](https://so.csdn.net/so/search/s.do?q=io&t=blog)
个人分类：[重构C/C++](https://blog.csdn.net/scut1135/article/category/621651)
standard IO library
标准IO库函数定义在stdio.h中
fopen
#include <stdio.h>
FILE *fopen(const char *filename, const char *mode);
mode:
"r" "rb"以只读方式打开文件,rb在其他系统中表示以二进制格式打开
"w" "wb"以写方式打开,会清空文件内容
"a" "ab"以附加方式打开
"r+" "rb+" "r+b"读写方式打开
"w+" "wb+" "w+b"读写方式打开,清空文件内容
"a+" "ab+" "a+b"以附加方式打开
在linux系统中并不区分普通文本文件和二进制文件,所以两种方式在linux里可以通用.
返回值:成功返回非空的指向FILE的指针,失败返回NULL,并设置errno
最大打开文件数由 FOPEN_MAX限定
fread,fwrite
#include <stdio.h>
size_t fread(void *ptr, size_t size, size_t nitems, FILE *stream);
size_t fwrite(const void *ptr, size_t size, size_t nitems, FILE *stream);
size表示每个数据的大小,nitems表示处理的数据数量.
返回值:成功返回实际读写的item数量,可以小于指定的nitems数量,如果到达文件末尾或者出现错误则返回比nitems小的值或者0,fread不区分错误和文件末尾,应该通过feof和ferror检测是否发生错误.
fclose
#include <stdio.h>
int fclose(FILE *stream);
会flush数据流,并且关闭底层文件描述符.
返回值:成功返回0,错误返回EOF,并设置errno,产生错误以后的访问包括fclose的行为是未定义的.
fflush
#include <stdio.h>
int fflush(FILE *stream);
如果stream是NULL,fflush将作用于所有文件.
返回值:成功返回0,否则EOF,并设置errno.
fseek
#include <stdio.h>
int fseek(FILE *stream, long int offset, int whence);
其中whence和offset和lseek相同,成功调用fseek将清除文件流的文件结尾标志,并且消除ungetc对此文件流的影响.
返回值:成功返回0,否则返回EOF,并设置errno.
fgetc,getc,getchar
#include <stdio.h>
int fgetc(FILE *stream);
int getc(FILE *stream);
int getchar();
getc通常实现为宏所以不能对其取地址,stream可能在这种情况下消除side effect.getchar相当于 getc(stdin);
返回值:返回下文件流的一个字符转换为int的值,当到达文件结尾或者错误时返回EOF,需要使用ferror和feof检测具体情况.
fputc,putc,putchar
#include <stdio.h>
int fputc(int c, FILE *stream);
int putc(int c, FILE *stream);
int putchar(int c);
fputc向stream中写入字符c.
返回值:成功返回将写入字符转换为int的值,错误返回EOF.
fgets,gets,fputs,puts
#include <stdio.h>
char *fgets(char *s, int n, FILE *stream);
char *gets(char *s);
int fputs(const char *s, FILE *stream);
int puts(const char *s); 
文件流和文件描述符
每一个文件流在底层都对应一个文件描述符.
#include <stdio.h>
int fileno(FILE *stream);
FILE *fdopen(int fd, const char *mode);
fileno检查并返回stream对应的文件描述符.当stream不是一个有效的文件流时返回-1,并设置errno为EBADF.
fdopen把指定的文件描述符和文件流关联起来并返回文件流指针.错误则返回NULL,并设置errno.
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/dts011/archive/2009/02/16/3896280.aspx](http://blog.csdn.net/dts011/archive/2009/02/16/3896280.aspx)
