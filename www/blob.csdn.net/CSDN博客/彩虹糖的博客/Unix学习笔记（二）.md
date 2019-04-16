# Unix学习笔记（二） - 彩虹糖的博客 - CSDN博客





2018年04月30日 19:55:21[彩虹糖梦](https://me.csdn.net/caozixuan98724)阅读数：85








# 1. openmax.c

**源码：**

```cpp
#include <sys/resource.h>

#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define OPEN_MAX_GUESS 256

int open_max(void) {
#ifdef OPEN_MAX
        static int openmax = OPEN_MAX;
#else
        static int openmax = 0;
#endif

        if (openmax == 0) {
            fprintf(stderr,"no predefined OPEN_MAX\n");
            errno = 0;
            if ((openmax = sysconf(_SC_OPEN_MAX)) < 0) {
                        if (errno == 0)
                                openmax = OPEN_MAX_GUESS;       /* indeterminate */
                        else
                                fprintf(stderr, "sysconf error for _SC_OPEN_MAX\n");
                }
        }
        else
            printf("Macro OPEN_MAX has been defined!\n");

        return openmax;
}
int main(int argc, char **argv) {
        struct rlimit rlp;
        if (getrlimit(RLIMIT_NOFILE, &rlp) != 0) {
                fprintf(stderr, "Unable to get per process rlimit: %s\n", strerror(errno));
                exit(EXIT_FAILURE);
        }
        printf("This user could open %d file descriptors.\n", open_max());
        printf("This process could open %d file descriptors.\n", (int)rlp.rlim_cur);
        return EXIT_SUCCESS;
}
```

**输出：**

![](https://img-blog.csdn.net/20180430191648854)


**函数详解：**

OPEN_MAX代表当前用户所能打开的最大文件数量，在程序中如果这个值没有被定义，则使用sysconf(_SC_OPEN_MAX)获取当前进程所能打开的最大进程数。

sysconf()函数可以获得cpu个数，最大打开文件数量等关于系统配置的基本信息。具体的参数可以通过man sysconf命令查看。

getrlimit和setrlimit

int getrlimit(int resource, struct rlimit *rlim);


int setrlimit(int resource, const struct rlimit *rlim);

这两个函数可以获取和设定资源使用限制。每种资源都有相关的软硬限制，软限制是内核强加给相应资源的限制值，硬限制是软限制的最大值。描述软硬件资源限制的结构体：

```cpp
struct rlimit {
　　rlim_t rlim_cur;
　　rlim_t rlim_max;
};
```

**补充说明：**

在这段程序中，由于OPEN_MAX的值没有预先设定，所以我们获取到的还是该进程所能打开的最大文件数。在shell中，我们可以通过 ulimit -n <number>来设定这个最大值，可以看到，在执行这个命令之后，两个输出值都变了。

![](https://img-blog.csdn.net/20180430193507913)


# 2. close.c, close2.c

**源码：**

```cpp
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

//file is updated even if close(fd) is not invoked
int main(){
    int fd = open("./test.txt",O_WRONLY|O_CREAT,S_IRUSR | S_IWUSR);
    if(fd<0){
        printf("file open error!\n");
        return 1;
    }
    char *s ="hello,world\n";
    write(fd,s,strlen(s));
    char c;
    printf("press any key to continue......");
    scanf("%c",&c);
    close(fd);
    return 0;
}
```

```cpp
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

// if fclose(fp) is not invoked, the file will not be updated
int main(){
    FILE *fp;
    fp = fopen("./test.txt","w");
    if(!fp){
        printf("file open error!\n");
        return 1;
    }
    char *s ="hello,world\n";
    fprintf(fp,"%s",s);
    char c;
    printf("press any key to continue......");
    scanf("%c",&c);
    fclose(fp);
    return 0;
}
```

**输出：**

![](https://img-blog.csdn.net/20180430194421117)


可以看到test.txt文件被创建，里面被写入了“hello,world”。

**函数详解：**

int open(const char *path, int access,int mode); 

path为要打开的文件路径和名称

access为访问权限：常用的几种O_RDONLY（只读打开），O_WRONLY（只写打开），O_RDWR（读写打开），O_CREAT（如果文件不存在则创建该文件），O_TRUNC（打开一个已存在的文件并将其文件长度设为0，其他属性不变）。其他常量查看相关手册。这几个常量可以通过 | 运算符一起使用。

mode仅在access中有O_CREAT时起作用，用来定义创建文件的属性。具体参数查看手册。

FILE *fopen(char *filename, char *mode);


path为要打开的文件路径和名称。

mode为访问权限：       

        rb  只读方式打开一个二进制文件                         
        w   只写方式打开一个文本文件                           
        wb  只写方式打开一个二进制文件                         
        a   追加方式打开一个文本文件                           
        ab  追加方式打开一个二进制文件                         
        r+  可读可写方式打开一个文本文件                       
        rb+ 可读可写方式打开一个二进制文件                     
        w+  可读可写方式创建一个文本文件                       
        wb+ 可读可写方式生成一个二进制文件                     
        a+  可读可写追加方式打开一个文本文件                   
        ab+ 可读可写方式追加一个二进制文件

**补充说明：**

open和fopen的区别：


前者属于低级IO，后者是高级IO。前者返回一个文件描述符，后者返回一个文件指针。前者无缓冲，后者有缓冲。前者与 read, write 等配合使用， 后者与 fread, fwrite等配合使用。

open的返回值是从3开始的。

0,1,2分别代表标准输入标准输出和标准错误

# 3. openat1.c openat2.c

**源码：**

```cpp
#include <stdio.h>  
#include <sys/stat.h>  
#include <fcntl.h>  
#include <stdlib.h>  
#include <unistd.h>  
  
void creat_at(char *dir_path, char *relative_path)  
{  
	int dir_fd;  
	int fd;  
	int flags;  
	mode_t mode;  
  
	dir_fd = open(dir_path, O_RDONLY);  
	if (dir_fd < 0)   
	{  
		perror("open");  
		exit(EXIT_FAILURE);  
	}  
  
	flags = O_CREAT | O_TRUNC | O_RDWR;  
	mode = 0640;  
	fd = openat(dir_fd, relative_path, flags, mode);  
	if (fd < 0)   
	{  
		perror("openat");  
		exit(EXIT_FAILURE);  
	}  
  
	write(fd, "HELLO\n", 6);  
  
	close(fd);  
	close(dir_fd);  
}  
  
int main()  
{  
	creat_at("./open", "log.txt");  
	return 0;  
}
```


```cpp

```
#include <sys/types.h>  
#include <sys/stat.h>  
#include <fcntl.h>  
#include <dirent.h>  
#include <stdio.h>  
#include <unistd.h>  
  
int main()  
{  
    DIR *dir;  
    int dirfd2;  
    int fd;  
    int n;  
  
    dir = opendir("./open");  
    if(NULL == dir)  
    {  
        perror("open dir error");  
        return -1;  
    }  
    dirfd2 = dirfd(dir);  
    if(-1 == dirfd2)  
    {  
        perror("dirfd error");  
        return -1;  
    }  
  
    fd = openat(dirfd2,"output.log",O_CREAT|O_RDWR|O_TRUNC);  
    if(-1 == fd)  
    {  
        perror("opeat error");  
        return -1;  
    }  
    n = write(fd,"Hello world!\n",15);  
      
    close(fd);  
    closedir(dir);  
  
    return 0;  
  
} 
**输出：**

向当前文件夹下的open文件夹下的log.txt写入HELLO

函数详解：

这两段代码都是体现openat这个函数的用法，在这里一并讲解。

int openat(int dirfd, const char *pathname, int flags , mode_t  mode );


int dirfd是文件夹的文件描述符，可以通过open函数打开文件夹获得。

const char *pathname是文件的相对路径，相对于第一个参数代表的文件夹的路径。

int flags和mode_t mode的属性和上文中open的用法一致，不再赘述。

函数的具体用法：

1. 在第一段代码中的用法。

通过open函数获取文件夹的文件描述符dir_fd，再通过openat函数打开对应文件夹下的对应文件。

2. 在第二段代码中的用法。

通过opendir函数获得一个类型为DIR的结构体，再通过dirfd()函数获取到对应文件夹的文件描述符，最后通过openat函数打开文件。

3.  openat(AT_FDCWD,"log.txt")

AT_FDCWD 这个参数代表当前进程所在的文件夹。


4. openex.c

```cpp
#include <sys/stat.h>

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void
createFile() {
	int fd;

	if ((fd = open("./newfile", O_RDONLY | O_CREAT,
					S_IRUSR | S_IWUSR)) == -1) {
		fprintf(stderr, "Unable to create './newfile': %s\n",
				strerror(errno));
		exit(EXIT_FAILURE);
	}

	printf("'./newfile' created. File descriptor is: %d\n", fd);

	/* We are leaking a file descriptor here to illustrate that they
	 * are increasing as we open more files.  Normally, we would call
	 * close(2) whenever we're done using the decriptor. */
}

void
failExclFileCreation() {
	int fd;

	if ((fd = open("./newfile", O_RDONLY | O_CREAT | O_EXCL,
					S_IRUSR | S_IWUSR)) == -1) {
		fprintf(stderr, "Unable to create './newfile': %s\n",
				strerror(errno));
	}

	/* We expect this to fail! */
	if (close(fd) == -1) {
		fprintf(stderr, "Closing failed: %s\n", strerror(errno));
	}
}


void
failOpenNonexistingFile() {
	int fd;

	if ((fd = open("./nosuchfile", O_RDONLY)) == -1) {
		fprintf(stderr, "Unable to open './nosuchfile': %s\n",
				strerror(errno));
	}

	/* This will fail.  So be it. */
	(void)close(fd);
}

void
openFile() {
	int fd;

	if ((fd = open("./openex.c", O_RDONLY)) == -1) {
		fprintf(stderr, "Unable to open './openex.c': %s\n",
				strerror(errno));
		exit(EXIT_FAILURE);
	}

	printf("'./openex.c' opened. File descriptor is: %d\n", fd);

	(void)close(fd);
	printf("'./openex.c' closed again\n");
}

void
truncateFile() {
	int fd;

	if ((fd = open("./newfile", O_RDONLY | O_TRUNC)) == -1) {
		fprintf(stderr, "Unable to open './newfile': %s\n",
				strerror(errno));
		exit(EXIT_FAILURE);
	}

	printf("'./newfile' opened. File descriptor is: %d\n", fd);
	printf("'./newfile' truncated -- see 'ls -l openex'\n");

	(void)close(fd);
}



int
main(int argc, char **argv) {
	createFile();
	system("ls -l newfile");
	printf("\n");
	sleep(2);

	createFile();
	system("ls -l newfile");
	printf("\n");
	sleep(2);

	failExclFileCreation();
	printf("\n");
	sleep(2);

	openFile();
	printf("\n");
	sleep(2);

	failOpenNonexistingFile();
	printf("\n");
	sleep(2);

	printf("cp openex.c newfile\n");
	system("cp openex.c newfile");
	system("ls -l newfile");
	truncateFile();
	system("ls -l newfile");

	return 0;
}
```

输出：

![](https://img-blog.csdn.net/20180506102758983)


函数详解：

这一段代码再讲open

(1) fd = open("./newfile", O_RDONLY | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR)


这一段代码的关键在于O_EXCL这个常量，代表当O_CREAT存在时，如果要打开的文件已经存在则返回-1报错。

同时，如果要打开一个不存在的文件却没有加O_CREAT参数，则会返回-1报错。

(2) int system(const char* cmdstring);  


system函数在其实现中调用了fork，exec和waitopid，相关的实现已经在simple-shell.c中讲解过了。

通过这个函数我们可以执行shell命令。

**关于类似write(2)，open(2)函数中括号内数字的含义**

1 用户命令， 可由任何人启动的。
2 系统调用， 即由内核提供的函数。
3 例程， 即库函数。
4 设备， 即/dev目录下的特殊文件。
5 文件格式描述， 例如/etc/passwd。
6 游戏， 不用解释啦！
7 杂项， 例如宏命令包、惯例等。
8 系统管理员工具， 只能由root启动。
9 其他（Linux特定的）， 用来存放内核例行程序的文档。
n 新文档， 可能要移到更适合的领域。
o 老文档， 可能会在一段期限内保留。


l 本地文档， 与本特定系统有关的。

# 未完待续






