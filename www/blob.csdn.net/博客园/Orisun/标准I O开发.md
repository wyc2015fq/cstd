# 标准I/O开发 - Orisun - 博客园







# [标准I/O开发](https://www.cnblogs.com/zhangchaoyang/articles/1942389.html)





标准I/O操作都是基于流缓冲的，符合ANSIC标准I/O处理。

全缓冲：填满缓存后才执行I/O操作。如malloc就是全缓冲。

行缓冲：遇到换行符时执行I/O操作。如stdin和stdout。

无缓冲：如stderr。



下面介绍的函数全部包含在stdio.h头文件中。

打开文件

FILE *
fopen(const char * path,const char * mode)
		//mode可以取r(读)，w(写)，a(追加)

FILE *
fdopen(int fd,const char * mode)


FILE *
freopen(const char *path,const char * mode,FILE * stream)


成功:指向 FILE 的指针

失败:NULL




关闭文件

int
fclose(FILE * stream)


成功:0


失败:EOF




读写文件

size_t
fread(void * ptr,size_t size,size_t nmemb,FILE * stream)


size_t
fwrite(const void * ptr,size_t size,size_t nmemb,FILE * stream)


size元素的大小

nmemb元素的个数

成功：返回实际读写的数目

失败：返回EOF



字符输入输出

int
getc(FILE * stream)


int
fgetc(FILE * stream)


int
getchar(void)


fgetc是标准库中的一个函数，getc通常被定义为一个宏，实现效率要高一些，getchar来自stdin



int
putc(int c,FILE * stream)


int
fputc(int c,FILE * stream)


int
putchar(int c)




行输入输出

char *
gets(char *s)


char
fgets(char * s,int size,FILE * stream)




int
puts(const char *s)


int
fputs(const char * s,FILE * stream)




格式化输入输出

int
printf(const char *format,...)


int
fprintf(FILE *fp,const char *format,...)


int
sprintf(char *buf,const char *format,...)




int
vprintf(const char *format,va_list arg)


int
vfprintf(FILE *fp,const char *format,va_list arg)


int
vsprintf(char *buf,const char *format,va_list arg)




int
scanf(const char *format,...)


int
fscanf(FILE *fp,const char *format,...)


int
sscanf(char *buf,const char *format,...) 












