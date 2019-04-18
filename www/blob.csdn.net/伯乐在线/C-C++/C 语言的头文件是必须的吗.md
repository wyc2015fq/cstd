# C 语言的头文件是必须的吗 - 文章 - 伯乐在线
原文出处： [Bean_lee](http://blog.chinaunix.net/uid-24774106-id-3291005.html)
N年前学习C语言开始，就被老师教导，要记得包含头文件。自己也养成了二话不说就#include的习惯。从来没有静下信来想想，C语言必需要有头文件吗？头文件到底起到什么的作用。
最近一段时间做了一些静态库和动态库相关的东西，一些内容在上一篇博文中，阅读上一篇博文的看官可以看出，我的静态库和动态库都没有对应的头文件，可是我的应用程序调用了库，没有包含头文件，一样是正常地编译执行。意识到这个问题的时候，我还有一阵恐慌，不知道怎么解释这个现象。因为我们常规都会包含头文件，比如调用多线程库，都会 #include 。
对于这个问题，我又做了一些实验算是把这个问题有个初步的理解。当然这个题目起的有点标题党，这么大的题目不是我这种菜鸟能够驾驭的了的，但是我还是想把自己的理解说以下，欢迎各位路过的高手批评指点。
我写了一个测试程序，里面没有包含任何的头文件。


```
int f() 
{
        while(1)
        { 
                printf("hello\n");
                sleep(5);
        } 
}
int main()
{
        creat("testfile",0777);
        // printf("hello world\n");
        unsigned long tid = 0;
        int ret = pthread_create(&tid,0,&f,0); 
        if(ret)
        { 
                printf("pthread_create failed \n");
        } 
        else
        { 
                printf("tid = %lu\n",tid);
                pthread_join(tid,0);
        } 
        return 0;
}
```
这个测试程序中调用了printf,sleep,creat，pthread_create,he pthread_join,按照惯例，我们是需要头文件的，需要的头文件如下：


```
//for printf
#include<stdio>
// for sleep
#include<unistd.h>
//for create
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
//for pthread_create ,pthread_join
#include <pthread.h>
```
现在我们观察，能否编译通过：


```
root@libin:~/program/C/testlib/head# gcc -o test test.c
test.c: In function ‘f’:
test.c:6: warning: incompatible implicit declaration of built-in function ‘printf’
test.c: In function ‘main’:
test.c:20: warning: incompatible implicit declaration of built-in function ‘printf’
test.c:24: warning: incompatible implicit declaration of built-in function ‘printf’
/tmp/cc2Rt0UO.o: In function `main':
test.c:(.text+0x65): undefined reference to `pthread_create'
test.c:(.text+0xa6): undefined reference to `pthread_join'
collect2: ld returned 1 exit status
```
这是预料之中的事情，因为，没有找到pthread_create 和pthread_join两个函数的定义。这是因为链接时，没有指定libpthread.so。加上-lpthread自然能够编过。 有的看官就问了，为什么creat sleep 和printf为什么不报找不到函数定义。那是因为这些函数都在libc库中，而libc库不需要显式指定，默认就包换了libc的动态库。


```
root@libin:~/program/C/testlib/head# ll
总用量 12
drwxr-xr-x 2 root root 4096 2012-07-28 10:41 ./
drwxr-xr-x 5 root root 4096 2012-07-27 19:05 ../
-rw-r--r-- 1 root root 405 2012-07-28 10:40 test.c
root@libin:~/program/C/testlib/head# gcc -Wall -o test test.c -lpthread
test.c: In function ‘f’:
test.c:6: warning: implicit declaration of function ‘printf’
test.c:6: warning: incompatible implicit declaration of built-in function ‘printf’
test.c:7: warning: implicit declaration of function ‘sleep’
test.c: In function ‘main’:
test.c:14: warning: implicit declaration of function ‘creat’
test.c:17: warning: implicit declaration of function ‘pthread_create’
test.c:20: warning: incompatible implicit declaration of built-in function ‘printf’
test.c:24: warning: incompatible implicit declaration of built-in function ‘printf’
test.c:25: warning: implicit declaration of function ‘pthread_join’
root@libin:~/program/C/testlib/head# ll
总用量 20
drwxr-xr-x 2 root root 4096 2012-07-28 10:41 ./
drwxr-xr-x 5 root root 4096 2012-07-27 19:05 ../
-rwxr-xr-x 1 root root 7394 2012-07-28 10:41 test*
-rw-r--r-- 1 root root 405 2012-07-28 10:40 test.c
root@libin:~/program/C/testlib/head# ./test
tid = 3077905264
hello
hello
```
有一些警告，但是毕竟编出了可执行文件test，并能够正确的执行。
我们的实验做完了，没有包含任何头文件，我想做的事情，动态库也帮我做了。这个实验就证明了，头文件并不是必不可少的，没有头文件，只要你清楚函数调用的接口，清楚需要包含那个库，一样可以做你想做的事情。
很颠覆常规是吧，既然没有头文件，我也能干事，头文件存在还有什么意义呢？
我们想一下，如果我写了一个功能很强大的库，（静态库或者动态库，whatever），完全没有头文件，因为函数通通是我写的，我很清楚每个函数的入参个数 类型及返回值，清楚每个函数的功能，所以我可以不需要头文件。但是如果另外一个人来用我的库文件，他就会很挠头，因为没有头文件，他不知道我库里面有那些函数，每个函数入参 个数类型以及返回值。他就没法调用我的库做想做的事情。从这个角度上讲，头文件是描述性的文件，它不涉及功能的实现，就好像一本书，没有任何的目录和章节名，也是OK的，但是读者读起来就很不方便。
除了这一点，我还想到了一点，就是让编译器帮我们做必要的检查。编译器的功能是很强大的，他能帮你做很多的事情。如果你没有包含头文件，函数调用错了，编译器也帮不了你。看下面的例子：
我将sleep函数调用多加了个参数，pthread_create函数少传了个参数，因为我没有头文件，所以编译器帮不了什么。


```
int f() 
{
        while(1)
        { 
                printf("hello\n");
                sleep(5,4);
        } 
}
int main()
{
        creat("testfile",0777);
        // printf("hello world\n");
        unsigned long tid = 0;
        int ret = pthread_create(&tid,0,&f); 
        if(ret)
        { 
                printf("pthread_create failed \n");
        } 
        else
        { 
                printf("tid = %lu\n",tid);
                pthread_join(tid,0);
        } 
        return 0;
}
```


```
root@libin:~/program/C/testlib/head# gcc -Wall -o test test.c -lpthread
test.c: In function ‘f’:
test.c:6: warning: implicit declaration of function ‘printf’
test.c:6: warning: incompatible implicit declaration of built-in function ‘printf’
test.c:7: warning: implicit declaration of function ‘sleep’
test.c: In function ‘main’:
test.c:14: warning: implicit declaration of function ‘creat’
test.c:17: warning: implicit declaration of function ‘pthread_create’
test.c:20: warning: incompatible implicit declaration of built-in function ‘printf’
test.c:24: warning: incompatible implicit declaration of built-in function ‘printf’
test.c:25: warning: implicit declaration of function ‘pthread_join’
```
看到了，编译器gcc爱莫能助，发现不了sleep多了个参数，pthread_create少个参数。这种情况下还是能运行。


```
root@libin:~/program/C/testlib/head# ./test
tid = 3079175024
hello
hello
hello
hello
hello
hello
hello
```
这种情况是很危险的，因为会破坏栈空间，有可能将栈空间踩坏。但是如果我包含了头文件，就不同了，编译器会帮我检查。


```
#include<unistd.h>
#include<pthread.h>
int f() 
{
        while(1)
        { 
                printf("hello\n");
                sleep(5,4);
        } 
}
int main()
{
        creat("testfile",0777);
        // printf("hello world\n");
        unsigned long tid = 0;
        int ret = pthread_create(&tid,0,&f); 
        if(ret)
        { 
                printf("pthread_create failed \n");
        } 
        else
        { 
                printf("tid = %lu\n",tid);
                pthread_join(tid,0);
        } 
        return 0;
}
```
看下编译情况：


```
root@libin:~/program/C/testlib/head# ll
总用量 12
drwxr-xr-x 2 root root 4096 2012-07-28 10:51 ./
drwxr-xr-x 5 root root 4096 2012-07-27 19:05 ../
-rw-r--r-- 1 root root 444 2012-07-28 10:51 test.c
root@libin:~/program/C/testlib/head# gcc -Wall -o test test.c -lpthread
test.c: In function ‘f’:
test.c:8: warning: implicit declaration of function ‘printf’
test.c:8: warning: incompatible implicit declaration of built-in function ‘printf’
test.c:9: error: too many arguments to function ‘sleep’
test.c: In function ‘main’:
test.c:16: warning: implicit declaration of function ‘creat’
test.c:19: warning: passing argument 3 of ‘pthread_create’ from incompatible pointer type
/usr/include/pthread.h:227: note: expected ‘void * (*)(void *)’ but argument is of type ‘int (*)()’
test.c:19: error: too few arguments to function ‘pthread_create’
test.c:22: warning: incompatible implicit declaration of built-in function ‘printf’
test.c:26: warning: incompatible implicit declaration of built-in function ‘printf’
```
有种观点是这样的，如果你的程序可能崩溃，请让它在第一时间第一现场崩溃，这样调试的代价比较低，方便troubleshooting，如果程序已经走到了背离预想轨道的分支，但是还有办法不让程序崩溃，请不要做这种事情，因为，早就背离了轨道，你不去检查，硬让程序继续走下去，如果终于崩溃了，你找不到这个第一现场，你很难调试。
编译检查也是这样，对自己的代码严格检查，让它编译不过也比编译过了埋下隐患要好的多。因为越往后面，troubleshooting的代价也就越大。
