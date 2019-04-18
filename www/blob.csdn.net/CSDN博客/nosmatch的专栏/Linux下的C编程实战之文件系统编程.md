# Linux下的C编程实战之文件系统编程 - nosmatch的专栏 - CSDN博客
2011年07月26日 23:21:16[nosmatch](https://me.csdn.net/HDUTigerkin)阅读数：432标签：[编程																[linux																[c																[stream																[file																[语言](https://so.csdn.net/so/search/s.do?q=语言&t=blog)](https://so.csdn.net/so/search/s.do?q=file&t=blog)](https://so.csdn.net/so/search/s.do?q=stream&t=blog)](https://so.csdn.net/so/search/s.do?q=c&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)](https://so.csdn.net/so/search/s.do?q=编程&t=blog)
**1.Linux文件系统**
[**Linux**](http://search.yesky.com/search.do?stype=0&wd=linux)支持多种文件系统，如ext、ext2、minix、iso9660、msdos、fat、vfat、nfs等。在这些具体文件系统的上层，Linux提供了虚拟文件系统（VFS）来统一它们的行为，虚拟文件系统为不同的文件系统与内核的通信提供了一致的接口。
　　在Linux平台下对文件编程可以使用两类函数：（1）Linux操作系统文件API；（2）C语言I/O库函数。 前者依赖于Linux系统调用，后者实际上与操作系统是独立的，因为在任何操作系统下，使用C语言I/O库函数操作文件的方法都是相同的。本章将对这两种方法进行实例讲解。
**2.Linux文件API**
　　Linux的文件操作API涉及到创建、打开、读写和关闭文件。
　　创建
int creat(const char *filename, mode_t mode);
　　参数mode指定新建文件的存取权限，它同umask一起决定文件的最终权限（mode&umask），其中umask代表了文件在创建时需要去掉的一些存取权限。umask可通过系统调用umask()来改变：
int umask(int newmask);
　　该调用将umask设置为newmask，然后返回旧的umask，它只影响读、写和执行权限。
　　打开
int open(const char *pathname, int flags); 
int open(const char *pathname, int flags, mode_t mode);
　　open函数有两个形式，其中pathname是我们要打开的文件名(包含路径名称，缺省是认为在当前路径下面)，flags可以去下面的一个值或者是几个值的组合：
|标志|含义|
|----|----|
|O_RDONLY|以只读的方式打开文件|
|O_WRONLY|以只写的方式打开文件|
|O_RDWR|以读写的方式打开文件|
|O_APPEND|以追加的方式打开文件|
|O_CREAT|创建一个文件|
|O_EXEC|如果使用了O_CREAT而且文件已经存在，就会发生一个错误|
|O_NOBLOCK|以非阻塞的方式打开一个文件|
|O_TRUNC|如果文件已经存在，则删除文件的内容|
|||
　　O_RDONLY、O_WRONLY、O_RDWR三个标志只能使用任意的一个。
　　如果使用了O_CREATE标志，则使用的函数是int open(const char *pathname,int flags,mode_t mode); 这个时候我们还要指定mode标志，用来表示文件的访问权限。mode可以是以下情况的组合：
|标志|含义|
|----|----|
|S_IRUSR|用户可以读|
|S_IWUSR|用户可以写|
|S_IXUSR|用户可以执行|
|S_IRWXU|用户可以读、写、执行|
|S_IRGRP|组可以读|
|S_IWGRP|组可以写|
|S_IXGRP|组可以执行|
|S_IRWXG|组可以读写执行|
|S_IROTH|其他人可以读|
|S_IWOTH|其他人可以写|
|S_IXOTH|其他人可以执行|
|S_IRWXO|其他人可以读、写、执行|
|S_ISUID|设置用户执行ID|
|S_ISGID|设置组的执行ID|
　　除了可以通过上述宏进行“或”逻辑产生标志以外，我们也可以自己用数字来表示，Linux总共用5个数字来表示文件的各种权限：第一位表示设置用户ID；第二位表示设置组ID；第三位表示用户自己的权限位；第四位表示组的权限；最后一位表示其他人的权限。每个数字可以取1(执行权限)、2(写权限)、4(读权限)、0(无)或者是这些值的和。例如，要创建一个用户可读、可写、可执行，但是组没有权限，其他人可以读、可以执行的文件，并设置用户ID位。那么，我们应该使用的模式是1(设置用户ID)、0(不设置组ID)、7(1+2+4，读、写、执行)、0(没有权限)、5(1+4，读、执行)即10705： 
open("test", O_CREAT, 10705);
　　上述语句等价于：
open("test", O_CREAT, S_IRWXU | S_IROTH | S_IXOTH | S_ISUID );
　　如果文件打开成功，open函数会返回一个文件描述符，以后对该文件的所有操作就可以通过对这个文件描述符进行操作来实现。
**读写**
　　在文件打开以后，我们才可对文件进行读写了，Linux中提供文件读写的系统调用是read、write函数：
int read(int fd, const void *buf, size_t length);
int write(int fd, const void *buf, size_t length);
　　其中参数buf为指向缓冲区的指针，length为缓冲区的大小（以字节为单位）。函数read()实现从文件描述符fd所指定的文件中读取length个字节到buf所指向的缓冲区中，返回值为实际读取的字节数。函数write实现将把length个字节从buf指向的缓冲区中写到文件描述符fd所指向的文件中，返回值为实际写入的字节数。
　　以O_CREAT为标志的open实际上实现了文件创建的功能，因此，下面的函数等同creat()函数：
int open(pathname, O_CREAT | O_WRONLY | O_TRUNC, mode);
**定位**
　　对于随机文件，我们可以随机的指定位置读写，使用如下函数进行定位：
int lseek(int fd, offset_t offset, int whence);
　　lseek()将文件读写指针相对whence移动offset个字节。操作成功时，返回文件指针相对于文件头的位置。参数whence可使用下述值：
　　SEEK_SET：相对文件开头
　　SEEK_CUR：相对文件读写指针的当前位置
　　SEEK_END：相对文件末尾
　　offset可取负值，例如下述调用可将文件指针相对当前位置向前移动5个字节：
lseek(fd, -5, SEEK_CUR);
　　由于lseek函数的返回值为文件指针相对于文件头的位置，因此下列调用的返回值就是文件的长度：
lseek(fd, 0, SEEK_END);
　　关闭
　　当我们操作完成以后，我们要关闭文件了，只要调用close就可以了，其中fd是我们要关闭的文件描述符： 
int close(int fd);
　　例程：编写一个程序，在当前目录下创建用户可读写文件“hello.txt”，在其中写入“Hello, software weekly”，关闭该文件。再次打开该文件，读取其中的内容并输出在屏幕上。
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#define LENGTH 100
main()
{
　int fd, len;
　char str[LENGTH]; 
　fd = open("hello.txt", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR); /* 创建并打开文件 */
　if (fd) 
　{
　　write(fd, "Hello, Software Weekly", strlen("Hello, software weekly")); /* 写入　Hello, software weekly字符串 */
　　close(fd);
　}
　fd = open("hello.txt", O_RDWR);
　len = read(fd, str, LENGTH); /* 读取文件内容 */
　str[len] = '\0';
　printf("%s\n", str);
　close(fd);
}
　　编译并运行，执行结果如下图：
![](http://hi.csdn.net/attachment/201107/26/0_1311693758KIH2.gif)
　　3.C语言库函数
　　C库函数的文件操作实际上是独立于具体的操作系统平台的，不管是在DOS、Windows、Linux还是在VxWorks中都是这些函数：
**创建和打开**
FILE *fopen(const char *path, const char *mode);
　　fopen()实现打开指定文件[filename](http://www.mydown.com/soft/utilitie/systems/198/433698.shtml)，其中的mode为打开模式，C语言中支持的打开模式如下表：
|标志|含义|
|----|----|
|r, rb|以只读方式打开|
|w, wb|以只写方式打开。如果文件不存在，则创建该文件，否则文件被截断|
|a, ab|以追加方式打开。如果文件不存在，则创建该文件|
|r+, r+b, rb+|以读写方式打开|
|w+, w+b, wh+|以读写方式打开。如果文件不存在时，创建新文件，否则文件被截断|
|a+, a+b, ab+|以读和追加方式打开。如果文件不存在，创建新文件|
　　其中b用于区分二进制文件和文本文件，这一点在DOS、Windows系统中是有区分的，但Linux不区分二进制文件和文本文件。
　　读写
　　C库函数支持以字符、字符串等为单位，支持按照某中格式进行文件的读写，这一组函数为：
int fgetc(FILE *stream);
int fputc(int c, FILE *stream);
char *fgets(char *s, int n, FILE *stream);
int fputs(const char *s, FILE *stream);
int fprintf(FILE *stream, const char *format, ...);
int fscanf (FILE *stream, const char *format, ...);
size_t fread(void *ptr, size_t size, size_t n, FILE *stream);
size_t fwrite (const void *ptr, size_t size, size_t n, FILE *stream);
　　fread()实现从流stream中读取加n个字段，每个字段为size字节，并将读取的字段放入ptr所指的字符数组中，返回实际已读取的字段数。在读取的字段数小于num时，可能是在函数调用时出现错误，也可能是读到文件的结尾。所以要通过调用feof()和ferror()来判断。
　　write()实现从缓冲区ptr所指的数组中把n个字段写到流stream中，每个字段长为size个字节，返回实际写入的字段数。
　　另外，C库函数还提供了读写过程中的定位能力，这些函数包括
int fgetpos(FILE *stream, fpos_t *pos);
int fsetpos(FILE *stream, const fpos_t *pos);
int fseek(FILE *stream, long offset, int whence); 
等。
　　关闭
　　利用C库函数关闭文件依然是很简单的操作：
int fclose (FILE *stream);
　　例程：将第2节中的例程用C库函数来实现。
#include <stdio.h>
#define LENGTH 100
main()
{
　FILE *fd;
　char str[LENGTH];
　fd = fopen("hello.txt", "w+"); /* 创建并打开文件 */
　if (fd)
　{
　　fputs("Hello, Software Weekly", fd); /* 写入Hello, software weekly字符串 */
　　fclose(fd);
　}
　fd = fopen("hello.txt", "r");
　fgets(str, LENGTH, fd); /* 读取文件内容 */
　printf("%s\n", str);
　fclose(fd);
}
**4.小结**
　　Linux提供的虚拟文件系统为多种文件系统提供了统一的接口，Linux的文件编程有两种途径：基于Linux系统调用；基于C库函数。这两种编程所涉及到文件操作有新建、打开、读写和关闭，对随机文件还可以定位。本章对这两种编程方法都给出了具体的实例。
