# Unix学习笔记（三） - 彩虹糖的博客 - CSDN博客





2018年05月12日 15:25:57[彩虹糖梦](https://me.csdn.net/caozixuan98724)阅读数：68标签：[Unix](https://so.csdn.net/so/search/s.do?q=Unix&t=blog)
个人分类：[C++																[Unix](https://blog.csdn.net/caozixuan98724/article/category/7630652)](https://blog.csdn.net/caozixuan98724/article/category/6944176)








这一部分的重点在于进程。

# 1. test.c 

**源码：**

```cpp
#include <unistd.h>
#include <stdio.h>

int main(){
    printf("%d\n",getpid());
    printf("%d\n",getppid());
}
```

**输出：**

![](https://img-blog.csdn.net/20180512110324538)


这一段代码十分简单，getpid(),getppid()分别获取当前进程和当前进程父进程的进程id。我们关注的是得到的父进程id：11882代表的究竟是什么？

所以我们执行如下命令：

kill -9 11882


得到如下结果：

![](https://img-blog.csdn.net/20180512110555325)


由此可见，该进程的父进程就是控制台程序。

# 2. main.c

**源码：**

```cpp
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int glob = 6;
char buf[] = "a write to stdout\n";

int
main(int argc, char **argv) {
	int var;
	pid_t pid;

	var = 88;
	if (write(STDOUT_FILENO, buf, sizeof(buf)-1) != sizeof(buf)-1) {
		fprintf(stderr, "%s: write error: %s\n",
				argv[0], strerror(errno));
		exit(1);
	}
	printf("before fork\n");

	if ((pid = fork()) < 0) {
		fprintf(stderr, "%s: fork error: %s\n",
				argv[0], strerror(errno));
		exit(1);
	} else if (pid == 0) { /* child */
                var++;printf("in Child, var is %d\n",var);
                printf("in Child, &var is %p\n",&var);
                char *argv[]={"ls",NULL};
                char *envp[]={NULL};
		execve("../../../../../bin/ls",argv,envp);
		var++;printf("After execve, in Child, var is %d\n",var);
	} else {		/* parent */
		sleep(2);
                printf("in Parent, &var is %p\n",&var);
	}

	printf("pid = %d, glob = %d, var = %d\n", getpid(), glob, var);
	exit(0);
}
```

**输出：**

![](https://img-blog.csdn.net/20180512111416722)


代码分析：

这一段代码稍微复杂一些，重点在于fork()创建新进程的理解。

要注意的是一下几点：

(1) 从输出结果可以看到，execve后面的语句是没有被执行的，execve之后的语句可以被执行当且仅当exeve语句执行失败的时候。

(2) fork()语句执行之后系统会复制一份一模一样的资源给子进程，所以我们看到在子进程中var的值变为了89而回到父进程var的值依然为88。

(3) 既然资源已经被复制，那为什么在父进程和子进程中var的内存地址是一样的呢？

这说明在unix中，每个进程都有各自独立的内存布局，每一个进程都认为自己占有全部的空间，真实的内存地址需要转化才能得到。

(4) sleep(2)是为了保证先执行完子进程，再执行父进程

# 3. entry.c

**源码：**

```cpp
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int foo(void) {
	printf("Foo for the win!\n");
	return 1;
	/* Note: this will cause a segfault, because this function
	 * returns, but there is nothing to return to: the routines set up
	 * by the kernel remain set up for 'main', we just told the linker
	 * to jump into 'foo' at program start.  Compare objdump(1)
	 * output. */
}

int bar(void) {
	printf("bar rules!\n");
	exit(1);
	/* Unlike foo(), this will not cause a segfault, since we are not
	 * returning; we explicitly call exit(3). */
}

int main(int argc, char **argv) {
	printf("Hooray main!\n");
	/* Note that we do explicitly _not_ return an error here, nor call
	 * any of the exit(3) functions.  Your compiler will warn you
	 * about this. */
}
```

这个代码本身没有什么好说的，主要是观察代码执行入口的地址，和关于main函数的一些特性。

执行以下命令：

readelf -h a.out

![](https://img-blog.csdn.net/20180512112746803)


我们可以看到入口地址为0x400470

那么这个地址代表什么呢？

再执行如下命令：

objdump -d a.out|more

![](https://img-blog.csdn.net/20180512143426225)


可以看到，这段程序的入口为<_start>

注意右侧的 callq 40040<_libc_start_main@plt>我们可以看看这个函数

```cpp
glibc/csu/lic-start.c

STATIC  int
LIBC_START_MAIN  (int  (*main)  (int,  char  **,  char  **  MAIN_AUXVEC_DECL),
		int  argc,  char  **argv,
		__typeof  (main)  init,
		void  (*fini)  (void),
		void  (*rtld_fini)  (void),  void  *stack_end)
{
[...]
	result  =  main  (argc,  argv,  __environ  MAIN_AUXVEC_PARAM);


	exit  (result);
}
```

那么，假如我们指定入口函数呢？

输入下面的命令：

gcc -e foo entry.c -o foo

查看输出：

![](https://img-blog.csdn.net/20180512144850328)


可以看到，程序报错了（段吐核也就是内存错误）

报错原因：从源码中我们看到foo程序return了1，但实际上，在这个程序中没有可以接受这个return值的东西。

同理，如果我们把函数原型改为 int foo(int argc, char**argv)，测试，同样可以发现，这几个参数是访问不到的。

因此我们可以得到如下结论，如果想要指定某一个函数作为程序的入口函数，那么这个函数应该满足如下两个条件

A. 该函数不带参数

B. 该函数不带return值（可以用exit(number)进行替代）

# 4. exit-handlers.c

```cpp
#include <stdio.h>
#include <stdlib.h>

int i;

void
my_exit1(void) {
	printf("first exit handler: %d\n", i);
	i++;
}

void my_exit2(void) {
	printf("second exit handler: %d\n", i);
}


int
main(int argc, char **argv) {
	i = 0;
	if (atexit(my_exit2) != 0) {
		perror("can't register my_exit2\n");
		exit(1);
	}

	if (atexit(my_exit1) != 0) {
		perror("can't register my_exit1");
		exit(1);
	}

	if (atexit(my_exit1) != 0) {
		perror("can't register my_exit1");
		exit(1);
	}

	printf("main is done\n");

	return (0);
}
```

**函数输出：**

![](https://img-blog.csdn.net/20180512151305867)


**函数详解：**

int atexit (void (*)(void))

在讲这个函数之前先提一下进程退出的方式：



进程终⽌的⽅式有8种，前5种为正常终⽌，后三种为异常终⽌：
1 从main函数返回；
2 调⽤exit函数；
3 调⽤_exit或_Exit；
4 最后⼀个线程从启动例程返回；
5 最后⼀个线程调⽤pthread_exit；
6 调⽤abort函数；
7 接到⼀个信号并终⽌；

8 最后⼀个线程对取消请求做出响应。




然后再说一下exit()和_exit()的区别：exit函数运行时首先会执行由atexit()函数登记的函数，然后会做一些自身的清理工作，同时刷新所有输出流、关闭所有打开的流并且关闭通过标准I/O函数tmpfile()创建的临时文件，最后调用_exit系统函数。也就是说exit()函数在退出时会调用一些相关函数做善后处理，而_exit()则是直接退出。




int atexit(void (*func)(void));


用来指定在执行exit()函数时还要执行的函数。

atexit也被称为登记函数，重点我们要观察登记函数的登记顺序和调用顺序，可以看到登记顺序和调用顺序正好相反，这个应该是一个堆栈。




