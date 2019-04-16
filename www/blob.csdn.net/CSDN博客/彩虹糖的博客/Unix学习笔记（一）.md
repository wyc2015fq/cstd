# Unix学习笔记（一） - 彩虹糖的博客 - CSDN博客





2018年04月30日 09:39:02[彩虹糖梦](https://me.csdn.net/caozixuan98724)阅读数：337








# 1. welcome.c

**源码：**

```cpp
#include <stdio.h>
#include <unistd.h>
int main(int argc, char **argv) {
    printf("Welcome to CS631 Advanced Programming in the UNIX Environment, %s!\n", getlogin());
}
```

**输出：**

![](https://img-blog.csdn.net/20180429154721342)


**函数解析：**

getlogin() 

如果有的话，这个函数返回当前登录名。在 Unix 系统上，它是从 utmp(5) 文件里读取的。如果它返回假，那么用 getpwuid 取代。

**补充说明：**

需要说明的是，utmp文件是二进制文件，在命令行，我们需要用who,w,users等命令访问utmp中的内容。![](https://img-blog.csdn.net/20180429155243274)

可以看到，当前只有一个用户ubuntu登陆。

**问题：**

在linux下以ubuntu用户名登陆，之后使用su命令切换用户，但显示的用户名始终不变。

![](https://img-blog.csdn.net/20180429155955475)


# 2. combine.c

**源码：**

```cpp
#include <stdio.h>
int main(){
    int i=8,j;
    __asm__ ("movl %1,%0" : "=r" (j) : "m" (i));
    printf("%d\n",j);
}
```

**输出：**

![](https://img-blog.csdn.net/20180429160457775)


**函数详解：**

这是gcc内嵌汇编语言的写法，从输出结果我们可以看出，这一段汇编的含义是将i赋值给j。

**补充说明：**

关于汇编使用gcc -S combine.c 可以生成中间的汇编代码combine.s文件。可以通过vim查看一下combine.s


![](https://img-blog.csdn.net/20180429161345942)


# 3. simple-ls.c

**源码：**

```cpp
/*
 * simple-ls.c
 * Extremely low-power ls clone.
 * ./simple-ls .
 */

#include <sys/types.h>

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>

int
main(int argc, char **argv) {

        DIR *dp;
        struct dirent *dirp;

        if (argc != 2) {
                fprintf(stderr, "usage: %s dir_name\n", argv[0]);
                exit(1);
        }

        if ((dp = opendir(argv[1])) == NULL ) {
                fprintf(stderr, "can't open '%s'\n", argv[1]);
                exit(1);
        }

        while ((dirp = readdir(dp)) != NULL )
                printf("%s\n", dirp->d_name);

        closedir(dp);
        return(0);
}
```

**输出：**

![](https://img-blog.csdn.net/20180429161858565)


**函数详解：**

(1) DIR* opendir(const char* path)

获取path子目录下所有文件和文件夹的列表，如果path是一个文件，则返回NULL。

(2) struct dirent* readdir(DIR* dir_handle)


如果有未读取的目录项，返回dirent结构体指针，如果全部读完，则返回NULL。这个结构体的内容如下：

```cpp
struct dirent
{
　　long d_ino; /* inode number 索引节点号 */
　　off_t d_off; /* offset to this dirent 在目录文件中的偏移 */
　　unsigned short d_reclen; /* length of this d_name 文件名长 */
　　unsigned char d_type; /* the type of d_name 文件类型 */
　　char d_name [NAME_MAX+1]; /* file name (null-terminated) 文件名，最长255字符 */
}
```

(3) int closedir(DIR * dir_handle)

关闭打开的目录。

补充说明：

整个代码的逻辑如下：

首先检查参数个数，如果参数个数不对提醒用户这个函数的用法。如果参数正确，则用opendir()检查目标目录是否真的为目录，如果不是则报错。如果是，则通过readdir()一个个读取文件夹下的文件名（上文输出中的.代表当前目录，..代表父目录）。最后通过closedir()关闭文件。

# 4. simple-cat.c

**源码：**

```cpp
/*
 * Stripped down version of 'cat', using unbuffered I/O.
 * ./simple-cat < simple-cat.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#ifndef BUFFSIZE
#define BUFFSIZE 32768
#endif

int main(int argc, char **argv) {
        int n;
        char buf[BUFFSIZE];

        while ((n = read(STDIN_FILENO, buf, BUFFSIZE)) > 0) {
                if (write(STDOUT_FILENO, buf, n) != n) {
                        fprintf(stderr, "write error\n");
                        exit(1);
                }
        }

        if (n < 0) {
                fprintf(stderr, "read error\n");
                exit(1);
        }

        return(0);
}
```

**输出：**

![](https://img-blog.csdn.net/20180429205737414)


**函数详解：**

read()和write()是很重要的函数，这里只是讲解其中的一部分，其他的内容会在后面的代码中慢慢涉及。

ssize_t read(int filedes, void *buf, size_t nbytes)

read的函数的三个参数分别代表文件描述符，读取数据缓存区，要读取的数据字节数。

返回值：若成功返回读到的字节数，若已到文件末尾返回0，若出错则返回-1.

ssize_t write(int filedes, void *buf, size_t nbytes)


write的函数的三个参数分别代表文件描述符，要写入的数据缓存区，要写入的数据字节数。

返回值：若成功返回写入的字节数，若出错则返回-1。

int fprintf( FILE *stream, const char *format, ... )
fprintf()函数根据指定的format(格式)(格式)发送信息(参数)到由stream(流)指定的文件. fprintf()只能和printf()一样工作. fprintf()的返回值是输出的字符数,发生错误时返回一个负值.


**补充说明：**

read和write的文件描述符通常是通过open函数的返回值获得。在这个代码中使用STDIN_FILENO和STDOUT_FILENO来代表标准输入和标准输出，也就是在终端输入和输出。

整个程序最后实现的功能是从终端写入多少数据，就会从终端显示出多少数据。

# 5. simple-shell.c

**源码：**

```cpp
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>
#include <unistd.h>

char *
getinput(char *buffer, size_t buflen) {
	printf("$$ ");
	return fgets(buffer, buflen, stdin);
}

int
main(int argc, char **argv) {
	char buf[1024];
	pid_t pid;
	int status;

	while (getinput(buf, sizeof(buf))) {
		buf[strlen(buf) - 1] = '\0';

		if((pid=fork()) == -1) {
			fprintf(stderr, "shell: can't fork: %s\n",
					strerror(errno));
			continue;
		} else if (pid == 0) {
			/* child */
			execlp(buf, buf, (char *)0);
			fprintf(stderr, "shell: couldn't exec %s: %s\n", buf,
					strerror(errno));
			exit(EX_DATAERR);
		}

		if ((pid=waitpid(pid, &status, 0)) < 0)
			fprintf(stderr, "shell: waitpid error: %s\n",
					strerror(errno));
	}

	exit(EX_OK);
}
```

**输出：**

![](https://img-blog.csdn.net/20180430091334152)


**函数详解：**

这段代码的核心是用于进程控制的三个函数fork，exec，waitpid。（exec函数有7种变体，但经常把它们统称为exec函数）

(1) fgets函数从标准输入一次读取一行，当输入文件结束符（通常是Ctrl+D）作为行的第一个字符时，fgets返回一个null指针，于是循环停止，进程也终止。

(2) buf[strlen(buf) - 1] = '\0' 这行代码表示用null字节替换换行符。这样做是因为execlp函数要求的参数是以null结尾而不是以换行符结束的。


(3) 调用fork创建一个新进程。新进程是调用进程的一个副本。我们称调用进程为父进程，新创建的进程为子进程。fork对父进程返回新的子进程的进程ID（一个非负整数），对子进程则返回0。因为fork创建了一个新进程，所以说它被调用一次（由父进程），但返回两次（分别在子进程和父进程中）。

(4) int execlp(const char * file,const char * arg,……)；


execlp()会从PATH 环境变量所指的目录中查找符合参数file的文件名, 找到后便执行该文件, 然后将第二个以后的参数当做该文件的argv[0]、argv[1]……, 最后一个参数必须用空指针(NULL)作结束。


(5) 子进程调用execlp以执行新程序文件，而父进程希望等待子进程终止，这是通过调用waitpid实现的，其参数指定要等待的进程，waitpid函数返回的是子进程的终止状态。在这个程序中并没有用到这个返回值。

**补充说明：**

至于在这个程序中为什么要fork一个新进程，原因是我们需要用这个子进程来拉起其他的进程（比如ls进程）。如果不创建这个子进程，则执行一条shell命令后程序就会终止，示例如下：

```cpp
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>
#include <unistd.h>

char * getinput(char *buffer, size_t buflen) {
        printf("$$ ");
        return fgets(buffer, buflen, stdin);
}

int main(int argc, char **argv) {
        char buf[1024];
        pid_t pid;
        int status;

        while (getinput(buf, sizeof(buf))) {
                buf[strlen(buf) - 1] = '\0';

                        execlp(buf, buf, (char *)0);
                        fprintf(stderr, "shell: couldn't exec %s: %s\n", buf,
                                        strerror(errno));
                        exit(EX_DATAERR);

        }

        exit(EX_OK);
}
```

![](https://img-blog.csdn.net/2018043009172044)

# 6.simple-shell2.c

**源码：**

```cpp
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>
#include <unistd.h>

static void sig_int(int);

char *
getinput(char *buffer, size_t buflen) {
	printf("$$ ");
	return fgets(buffer, buflen, stdin);
}

int
main(int argc, char **argv) {
	char buf[1024];
	pid_t pid;
	int status;

	if (signal(SIGINT, sig_int) == SIG_ERR) {
		fprintf(stderr, "signal error: %s\n", strerror(errno));
		exit(1);
	}

	while (getinput(buf, sizeof(buf))) {
		buf[strlen(buf) - 1] = '\0';

		if((pid=fork()) == -1) {
			fprintf(stderr, "shell: can't fork: %s\n",
					strerror(errno));
			continue;
		} else if (pid == 0) {
			/* child */
			execlp(buf, buf, (char *)0);
			fprintf(stderr, "shell: couldn't exec %s: %s\n", buf,
					strerror(errno));
			exit(EX_DATAERR);
		}

		if ((pid=waitpid(pid, &status, 0)) < 0)
			fprintf(stderr, "shell: waitpid error: %s\n",
					strerror(errno));
	}

	exit(EX_OK);
}

void
sig_int(int signo) {
	printf("\nCaught SIGINT!\n");
}
```

这一段代码和上一段代码几乎一样，只是多了一个signal机制。

**输出：**

![](https://img-blog.csdn.net/20180430093723851)


**函数详解：**

signal(int sig,sighandler_t handler)


unix的系统调用signal()用于接受一个指定的信号,并可以指定相应的处理方法。


在上段代码中，指定的处理函数是下面的sing_int(int signo)。



