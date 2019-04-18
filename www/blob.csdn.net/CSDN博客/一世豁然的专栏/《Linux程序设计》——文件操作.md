# 《Linux程序设计》——文件操作 - 一世豁然的专栏 - CSDN博客





2014年09月18日 11:54:11[一世豁然](https://me.csdn.net/Explorer_day)阅读数：919








一、Linux文件结构

 1、文件

   1）、在Linux中，一切（或几乎一切）都是文件。目录是一种特殊的文件。

   2）、Linux中的任何事物都可以用一个文件来表示，或通过特殊的文件文件提供。

   3）、文件除了包含内容以外，它还包括名字和属性——管理信息，包含文件的创建/修改日期和它的访问权限。这些都被保存在inode（节点）中，其中节点还包括文件的长度    


             和文件在磁盘上的存放位置。


 2、目录

  1）、目录是用于保存其他文件的节点和名字的文件。可以用ls -i命令查看节点。用ln命令创建链接。

   2）、删除一个目录，实质上是和删除了该文件对应的目录项，同时指向该文件的链接数减一。

   3）、文件被安排在目录中，目录中可能还包含子目录，这样就构成了文件系统层次结构。




 3、设备与文件

   1）、Linux中比较重要的设备文件有/dev/console、dev/tty和/dev/null。

     I、/dev/console——代表系统控制台，错误信息和诊断信息通常会被发送到这个设备。

     II、dev/tty——如果一个进程有中断控制的话，那么dev/tty就是这个控制终端（键盘和显示器，或键盘和接口）的别名（逻辑设备）。

     III、/dev/null——空设备。所有写向这个设备的输出都将被丢弃，而读这个设备会立即返回一个文件尾标志。

    IV、/dev目录中其他设备包括：硬盘和软盘、通信端口、磁带驱动器、CD-ROM、声卡以及一些代表系统内部工作状态的设备。



 4、设备被分为字符设备和块设备。







二、系统调用和设备驱动程序

 1、设备驱动程序

  1）、操作系统的核心部分——内核，是一组设备驱动程序。它们是一组对系统硬件进行控制的底层接口。




 2、系统调用

  1）、可以对文件和设备进行访问和控制的函数称为系统调用。

  2）、ioctl系统调用

    I、open：打开文件或设备

   II、read：从打开的文件或设备里读取数据。

   III、write：向文件或设备写数据。

   IV、close：关闭设备和文件。

   V、ioctl：把设备信息传递给设备驱动程序。




三、底层文件访问

 1、文件描述符

   1）、0：标准输入

   2）、1：标准输出

   3）、2：标准错误




 2、write系统调用

** #include<unistd.h>**

**   size_t  write(int fildes, const void *buf, size_t nbytes);**



 3、read系统调用

**#include<unistd.h>**

**   size_t  read(int  fildes, const  *buf,  size_t  nbytes);**




 4、open系统调用

**  #include<fcntl.h>**

**   #include<sys/types.h>**

**   #include<sys/stat.h>**

**   int open(const  char  *path,int oflags);**

**   int open(const char *path, int oflags, mode_t mode);**

    1)、oflags参数

        I、O_RDONLY：以只读方式打开。

       II、O_WRONLY：以只写方式打开。

       III、O_RDWR：以读写方式打开。

       IV、O_APPEND：把写入数据追加在文件的末尾。

       V、O_TRUNC：把文件长度设置为零，丢弃已有的内容。

      VI、O_CREAT：如果需要，就按参数mode中给出的访问模式创建文件。

     VII、O_EXCL：与O_CREAT一起使用，确保调用者创建文件。

   2）、当带有O_CREAT标志open调用来创建文件时，你必须有三个参数的open调用。

       I、S_IRUSR：读权限，文件属主。

      II、S_IWUSR：写权限，文件属主。

     III、S_IXUSR：执行权限，文件属主。

     IV、S_IRGRP：读权限，文件所属组。

     V、S_IWGRP：写权限，文件所属组。

     VI、S_IXGRP：执行权限，文件所属组。

    VII、S_IROTH：读权限，其他用户。

   VIII、S_IWOTH：写权限，其他用户。

     IX、S_IXOTH：执行权限，其他用户。

  3）、umask:当文件被访问是，为文件的访问权限设定一个掩码（表3-2）。




 5、close系统调用

**  #include<unistd.h>**

**    int close(int  fildes);**




 6、ioctl系统调用

** #include<unistd.h>**

**    int ioctl(int  fildes, int  cmd, ...);**




 7、其他系统调用

  1）、lseek系统调用

         #include<unistd.h>

        #include<sys/types.h>

        off_t  lseek(int  fildes,  off_t  offset,  int  whence);

       offset参数：

       SEEK_SET:offset是一个绝对位置。

       SEEK_CUR:offset是相对于当前位置的一个相对位置。

       SEEK_END:offset是相对于文件尾的一个相对位置。

  2）、fstat、stat和lstat系统调用

**#include<unistd.h>**

**        #include<sys/stat.h>**

**        #include<sys/types.h>**

**        int fstat(int fildes,  struct stat *buf);**

**       int stat(const char *path,  struct stat *buf);**

**       int lstat(const char *path,  struct stat *buf);**

       I、结构stat成员（P89  表3-4）

      II、st_mode标志（P90）

  3）、dup和dup2系统调用

** #include<unistd.h>**

**        int dup(int  fildes);**

**        int dup2(int fildes,  int filde2)**;







三、库函数

 1、标准I/O库

  1）、fopen函数

**    include<stdio.h>**

**       FILE  *fopen(const  char  *filename,  const  char  *mode);**

       mode参数：

      I、“r”或“rb”：

     II、“w”或“wb”：

    III、“a”或“ab”：

    IV、“r+”或“rb+”或“r+b”：

    VI、“w+”或“wb+”或“w+b”：

    VI、“a+”或“ab+”或“a+b”：

  2）、fread函数

**  #include<stdio.h>**

**       size_t  fread(void  *ptr,  size_t  size,size_t  nitems,  FILE  *stream);**

  3）、fwrite函数

**       #include<stdio.h>**

**       size_t  fwrite(const  void  *ptr,  size_t  size,  size_t  nitems,  FILE  *stream);**

  4）、fclose函数

** #incldue<stdio.h>**

**        int  fclose(FILE  *stream);**

  5）、fflush函数

** #include<stdio.h>**

**        int  fflush(FILE  *stream);**

  6）、fseek函数

** include<stdio.h>**

**        int  fseek(FILE  *stream,  long  int  offset,  int  whence);**

  7）、fgetc、getc和getchar函数

** #include<stdio.h>**

**        int  fgetc(FILE  *stream);**

**        int  getc(FILE  *stream);**

**        int  getchar();**

  8）、fputc、putc和putchar函数

**  #include<stdio.h>**

**       int  fputc(int c, FILE  *stream);**

**       int  putc(int  c, FILE  *stream);**

**       int  putchar(int  c);**

  9）、fgets和gets函数

** #include<stdio.h>**

**       char  *fgets(char  *s,  int  n,  FILE  *stream);**

**       char  *gets(char  *s);**



 2、格式化输入和输出

  1）、printf、fprintf和sprintf函数

**#include<stdio.h>**

**       int  printf(const  char  *format, ...);**

**       int  sprintf(char  *s,  const  char  *foemat, ...);**

**       int  fprintf(FILE  *stream,  const  char  *format, ...);**


  2）、scanf、fscanf和sscanf函数

**  #include<stdio.h>**

**       int  scanf(const  char  *format,...);**

**       int  fscanf(FILE  *stream, const  char  *format,...);**

**       int  sscanf(const  char  *s,  const  char  *format,...);**

  3）、其他流函数

**  fgetpos**：获得文件流当前（读写）位置。

**  fsetpos**:设置文件流当前（读写）位置。

** ftell**:返回文件流当前（读写）位置的偏移量。

**rewind**:重置文件流的读写位置。

**freopen**:重新使用一个文件流。

** setvbuf:**设置文件流的缓冲机制。

** remove**:相当于unlink函数，但如果它的path参数是一个目录的话，其作用就相当于一个rmdir函数。

  4）、文件流错误

**     #include<errno.h>**

**        extern  int  errno;**

**        #include<stdio.h>**

**        int  ferror(FILE  *stream);**

**        int  feof(FILE  *stream);**

**        int  clearerr(FILE  *stream);**

  5）、文件流和文件描述符

**    #include<stdio.h>**

**       int fileno(FILE  *stream);**

**       FILE  *fdopen(int  fildes,  const  char  *mode);**







四、文件和目录的维护

 1、chmod系统调用

**#include<sys/stat.h>**

**    int  chmod(const  char  *path, mode_t  mode);**




 2、chown系统调用

** #include<sys/stat.h>**

**    int  chown(const  char  *path, uid_t  owner,  gid_t  group);**




 3、unlink、link和symlink系统调用

**#incldue<unistd.h>**

**    int  unlink(const  char  *path);**

**    int  link(const  char  *path, const  char  *path2);**

**    int  symlink(const  char  *path1,  const  char  *path2);**




 4、mkdir和rmdir系统调用

** #include<sys/types.h>**

**    #include<sys/stat.h>**

**     int  mkdir(const  char  *path,  mode_t  mode);**

**    #include<unistd.h>**

**     int  rmdir(const  char  *path);**



 5、chdir函数和getcwd函数

** #inlcude<unistd.h>**

**    int  chdir(const  char  *path);**

**    #inlcude<unistd.h>**

**    char  *getced(char  *buf,  size_t  size);**







五、扫描目录

 1、opendir函数

**    #include<sys/types.h>**

**    #include<dirent.h>**

**    DIR  *opendir(const  char  *name);**




 2、readdir函数

**  #include<sys/types.h>**

**    #include<dirent.h>**

**    struct  dirent  *readdir(DIR  *dirp);**




 3、telldir函数

**#include<sys/types.h>**

**    #include<dirent.h>**
**    long  int  telldir(DIR  *dirp);**



 4、seekdir函数

**   #include<sys/types.h>**

**    #include<dirent.h>**
**    void  seekdir(DIR  *drip, long  int  loc);**



 5、close函数

**#include<sys/types.h>**

**    #include<dirent.h>**
**    int  closedir(DIR  *dirp);**






六、错误处理

 1、错误代码的取值和含义

  1）、EPERM:操作不允许。

  2）、ENOENT:文件或目录不存在。

  3）、EINTR:系统调用被中断。

  4）、EIO:I/O错误。

  5）、EBUSY:设备或资源忙。

  6）、EEXIST:文件存在。

  7）、EINVAL:无效参数。

  8）、EMFILE:打开的文件过多。

  9）、ENODEV:设备不存在。

  10）、EISDIR:是一个目录。

  11）、ENOTDIR:不是一个目录。




 2、strerror函数

**    #include<string.h>**

**    char  *strerror(int  errnum);**




 3、perror函数

**    #include<stdio.h>**

**    void  perror(const  char  *s);**







七、/proc文件系统







八、高级主题：fcntl和mmap

 1、fcntl系统调用

** #include<fcntl.h>**

**    int  fcntl(int  fildes, int  cmd);**

**    int  fcntl(int  fildes, int  cmd, long arg);**




 2、mmap函数

**    #include<sys/mman.h>**

**    void  *mmap(void  *addr,  size_t  len,  int  prot, int  flags, int  fildex, off_t  off);**

    1）、prot参数

        I 、PROT_READ:允许读该内存。

        II、 PROT_WRITE:允许写该内存。

        III 、PROT_EXEC:允许执行该内存段。

         IV、PROT_NONE:该内存不能被访问。

   2）、flags参数（P112  表3-7）




 3、msync函数


**    #include<sys/mman.h>**

**      int  msync(void  *addr,  size_t  len, int  flags);**

    1）、flags参数（P112  表3-8）




 4、munmap函数

**  #include<sys/mman.h>**

**     int  munmap(void  *addr,  size_t  len);**




