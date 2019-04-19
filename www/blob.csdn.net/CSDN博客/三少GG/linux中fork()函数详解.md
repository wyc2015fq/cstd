# linux中fork()函数详解 - 三少GG - CSDN博客
2013年12月30日 21:46:26[三少GG](https://me.csdn.net/scut1135)阅读数：703
个人分类：[重构C/C++](https://blog.csdn.net/scut1135/article/category/621651)

[http://blog.csdn.net/jason314/article/details/5640969](http://blog.csdn.net/jason314/article/details/5640969)
** 一、fork入门知识**
一个进程，包括代码、数据和分配给进程的资源。fork（）函数通过系统调用创建一个与原来进程几乎完全相同的进程，也就是两个进程可以做完全相同的事，但如果初始参数或者传入的变量不同，两个进程也可以做不同的事。
    一个进程调用fork（）函数后，系统先给新的进程分配资源，例如存储数据和代码的空间。然后把原来的进程的所有值都复制到新的新进程中，只有少数值与原来的进程的值不同。相当于克隆了一个自己。
     我们来看一个例子：
**[cpp]**[view
 plain](http://blog.csdn.net/jason314/article/details/5640969#)[copy](http://blog.csdn.net/jason314/article/details/5640969#)
- /*
-  *  fork_test.c
-  *  version 1
-  *  Created on: 2010-5-29
-  *      Author: wangth
-  */
- #include <unistd.h>
- #include <stdio.h> 
- int main ()   
- {   
-     pid_t fpid; //fpid表示fork函数返回的值
- int count=0;  
-     fpid=fork();   
- if (fpid < 0)   
-         printf("error in fork!");   
- elseif (fpid == 0) {  
-         printf("i am the child process, my process id is %d/n",getpid());   
-         printf("我是爹的儿子/n");//对某些人来说中文看着更直白。
-         count++;  
-     }  
- else {  
-         printf("i am the parent process, my process id is %d/n",getpid());   
-         printf("我是孩子他爹/n");  
-         count++;  
-     }  
-     printf("统计结果是: %d/n",count);  
- return 0;  
- }  
运行结果是：
i am the child process, my process id is 5574
    我是爹的儿子
    统计结果是: 1
    i am the parent process, my process id is 5573
    我是孩子他爹
    统计结果是: 1
    在语句fpid=fork()之前，只有一个进程在执行这段代码，但在这条语句之后，就变成两个进程在执行了，这两个进程的几乎完全相同，将要执行的下一条语句都是if(fpid<0)……
    为什么两个进程的fpid不同呢，这与fork函数的特性有关。fork调用的一个奇妙之处就是它仅仅被调用一次，却能够返回两次，它可能有三种不同的返回值：
    1）在父进程中，fork返回新创建子进程的进程ID；
    2）在子进程中，fork返回0；
    3）如果出现错误，fork返回一个负值；
    在fork函数执行完毕后，如果创建新进程成功，则出现两个进程，一个是子进程，一个是父进程。在子进程中，fork函数返回0，在父进程中，fork返回新创建子进程的进程ID。我们可以通过fork返回的值来判断当前进程是子进程还是父进程。
引用一位网友的话来解释fpid的值为什么在父子进程中不同。“其实就相当于链表，进程形成了链表，父进程的fpid(p 意味point)指向子进程的进程id, 因为子进程没有子进程，所以其fpid为0.
    fork出错可能有两种原因：
    1）当前的进程数已经达到了系统规定的上限，这时errno的值被设置为EAGAIN。
    2）系统内存不足，这时errno的值被设置为ENOMEM。
    创建新进程成功后，系统中出现两个基本完全相同的进程，这两个进程执行没有固定的先后顺序，哪个进程先执行要看系统的进程调度策略。
    每个进程都有一个独特（互不相同）的进程标识符（process ID），可以通过getpid（）函数获得，还有一个记录父进程pid的变量，可以通过getppid（）函数获得变量的值。
 fork执行完毕后，出现两个进程，
![](http://hi.csdn.net/attachment/201006/3/0_1275576795zgzN.gif)
    有人说两个进程的内容完全一样啊，怎么打印的结果不一样啊，那是因为判断条件的原因，上面列举的只是进程的代码和指令，还有变量啊。
    执行完fork后，进程1的变量为count=0，fpid！=0（父进程）。进程2的变量为count=0，fpid=0（子进程），这两个进程的变量都是独立的，存在不同的地址中，不是共用的，这点要注意。可以说，我们就是通过fpid来识别和操作父子进程的。
    还有人可能疑惑为什么不是从#include处开始复制代码的，这是因为fork是把进程当前的情况拷贝一份，执行fork时，进程已经执行完了int count=0;fork只拷贝下一个要执行的代码到新的进程。
**二、fork进阶知识**
    先看一份代码：
**[cpp]**[view
 plain](http://blog.csdn.net/jason314/article/details/5640969#)[copy](http://blog.csdn.net/jason314/article/details/5640969#)
- /*
-  *  fork_test.c
-  *  version 2
-  *  Created on: 2010-5-29
-  *      Author: wangth
-  */
- #include <unistd.h>
- #include <stdio.h>
- int main(void)  
- {  
- int i=0;  
-    printf("i son/pa ppid pid  fpid/n");  
- //ppid指当前进程的父进程pid
- //pid指当前进程的pid,
- //fpid指fork返回给当前进程的值
- for(i=0;i<2;i++){  
-        pid_t fpid=fork();  
- if(fpid==0)  
-            printf("%d child  %4d %4d %4d/n",i,getppid(),getpid(),fpid);  
- else
-            printf("%d parent %4d %4d %4d/n",i,getppid(),getpid(),fpid);  
-    }  
- return 0;  
- }  
    运行结果是：
i son/pa ppid pid  fpid
    0 parent 2043 3224 3225
    0 child  3224 3225    0
    1 parent 2043 3224 3226
    1 parent 3224 3225 3227
    1 child     1 3227    0
    1 child     1 3226    0
    这份代码比较有意思，我们来认真分析一下：
第一步：在父进程中，指令执行到for循环中，i=0，接着执行fork，fork执行完后，系统中出现两个进程，分别是p3224和p3225（后面我都用pxxxx表示进程id为xxxx的进程）。可以看到父进程p3224的父进程是p2043，子进程p3225的父进程正好是p3224。我们用一个链表来表示这个关系：
p2043->p3224->p3225
    第一次fork后，p3224（父进程）的变量为i=0，fpid=3225（fork函数在父进程中返向子进程id），代码内容为：
**[c-sharp]**[view
 plain](http://blog.csdn.net/jason314/article/details/5640969#)[copy](http://blog.csdn.net/jason314/article/details/5640969#)
- for(i=0;i<2;i++){  
-     pid_t fpid=fork();//执行完毕，i=0，fpid=3225
- if(fpid==0)  
-        printf("%d child  %4d %4d %4d/n",i,getppid(),getpid(),fpid);  
- else
-        printf("%d parent %4d %4d %4d/n",i,getppid(),getpid(),fpid);  
- }  
- return 0;  
    p3225（子进程）的变量为i=0，fpid=0（fork函数在子进程中返回0），代码内容为：
**[c-sharp]**[view
 plain](http://blog.csdn.net/jason314/article/details/5640969#)[copy](http://blog.csdn.net/jason314/article/details/5640969#)
- for(i=0;i<2;i++){  
-     pid_t fpid=fork();//执行完毕，i=0，fpid=0
- if(fpid==0)  
-        printf("%d child  %4d %4d %4d/n",i,getppid(),getpid(),fpid);  
- else
-        printf("%d parent %4d %4d %4d/n",i,getppid(),getpid(),fpid);  
- }  
- return 0;  
    所以打印出结果：
0 parent 2043 3224 3225
    0 child  3224 3225    0
第二步：假设父进程p3224先执行，当进入下一个循环时，i=1，接着执行fork，系统中又新增一个进程p3226，对于此时的父进程，p2043->p3224（当前进程）->p3226（被创建的子进程）。
    对于子进程p3225，执行完第一次循环后，i=1，接着执行fork，系统中新增一个进程p3227，对于此进程，p3224->p3225（当前进程）->p3227（被创建的子进程）。从输出可以看到p3225原来是p3224的子进程，现在变成p3227的父进程。父子是相对的，这个大家应该容易理解。只要当前进程执行了fork，该进程就变成了父进程了，就打印出了parent。
    所以打印出结果是：
1 parent 2043 3224 3226
    1 parent 3224 3225 3227
第三步：第二步创建了两个进程p3226，p3227，这两个进程执行完printf函数后就结束了，因为这两个进程无法进入第三次循环，无法fork，该执行return 0;了，其他进程也是如此。
    以下是p3226，p3227打印出的结果：
1 child     1 3227    0
    1 child     1 3226    0
    细心的读者可能注意到p3226，p3227的父进程难道不该是p3224和p3225吗，怎么会是1呢？这里得讲到进程的创建和死亡的过程，在p3224和p3225执行完第二个循环后，main函数就该退出了，也即进程该死亡了，因为它已经做完所有事情了。p3224和p3225死亡后，p3226，p3227就没有父进程了，这在操作系统是不被允许的，所以p3226，p3227的父进程就被置为p1了，p1是永远不会死亡的，至于为什么，这里先不介绍，留到“三、fork高阶知识”讲。
    总结一下，这个程序执行的流程如下：
![](http://hi.csdn.net/attachment/201006/1/0_1275402306yq99.gif)
     这个程序最终产生了3个子进程，执行过6次printf（）函数。
    我们再来看一份代码：
**[cpp]**[view
 plain](http://blog.csdn.net/jason314/article/details/5640969#)[copy](http://blog.csdn.net/jason314/article/details/5640969#)
- /*
-  *  fork_test.c
-  *  version 3
-  *  Created on: 2010-5-29
-  *      Author: wangth
-  */
- #include <unistd.h>
- #include <stdio.h>
- int main(void)  
- {  
- int i=0;  
- for(i=0;i<3;i++){  
-        pid_t fpid=fork();  
- if(fpid==0)  
-            printf("son/n");  
- else
-            printf("father/n");  
-    }  
- return 0;  
- 
- }  
     它的执行结果是：
father
    son
    father
    father
    father
    father
    son
    son
    father
    son
    son
    son
    father
    son 
    这里就不做详细解释了，只做一个大概的分析。
    for        i=0         1           2
              father     father     father
                                        son
                            son       father
                                        son
               son       father     father
                                        son
                            son       father
                                        son
    其中每一行分别代表一个进程的运行打印结果。
    总结一下规律，对于这种N次循环的情况，执行printf函数的次数为2*（1+2+4+……+2N-1）次，创建的子进程数为1+2+4+……+2N-1个。(感谢gao_jiawei网友指出的错误，原本我的结论是“执行printf函数的次数为2*（1+2+4+……+2N）次，创建的子进程数为1+2+4+……+2N ”，这是错的)
网上有人说N次循环产生2*（1+2+4+……+2N）个进程，这个说法是不对的，希望大家需要注意。
    数学推理见[http://202.117.3.13/wordpress/?p=81](http://202.117.3.13/wordpress/?p=81)（该博文的最后）。
    同时，大家如果想测一下一个程序中到底创建了几个子进程，最好的方法就是调用printf函数打印该进程的pid，也即调用printf("%d/n",getpid());或者通过printf("+/n");来判断产生了几个进程。有人想通过调用printf("+");来统计创建了几个进程，这是不妥当的。具体原因我来分析。
    老规矩，大家看一下下面的代码：
**[cpp]**[view
 plain](http://blog.csdn.net/jason314/article/details/5640969#)[copy](http://blog.csdn.net/jason314/article/details/5640969#)
- /*
-  *  fork_test.c
-  *  version 4
-  *  Created on: 2010-5-29
-  *      Author: wangth
-  */
- #include <unistd.h>
- #include <stdio.h>
- int main() {  
-     pid_t fpid;//fpid表示fork函数返回的值
- //printf("fork!");
-     printf("fork!/n");  
-     fpid = fork();  
- if (fpid < 0)  
-         printf("error in fork!");  
- elseif (fpid == 0)  
-         printf("I am the child process, my process id is %d/n", getpid());  
- else
-         printf("I am the parent process, my process id is %d/n", getpid());  
- return 0;  
- }  
   
 执行结果如下：
fork!
    I am the parent process, my process id is 3361
    I am the child process, my process id is 3362 
如果把语句printf("fork!/n");注释掉，执行printf("fork!");
    则新的程序的执行结果是：
fork!I am the parent process, my process id is 3298
    fork!I am the child process, my process id is 3299 
    程序的唯一的区别就在于一个/n回车符号，为什么结果会相差这么大呢？
这就跟printf的缓冲机制有关了，printf某些内容时，操作系统仅仅是把该内容放到了stdout的缓冲队列里了,并没有实际的写到屏幕上。但是,只要看到有/n 则会立即刷新stdout,因此就马上能够打印了。
    运行了printf("fork!")后,“fork!”仅仅被放到了缓冲里,程序运行到fork时缓冲里面的“fork!”  被子进程复制过去了。因此在子进程度stdout缓冲里面就也有了fork! 。所以,你最终看到的会是fork!  被printf了2次！！！！
    而运行printf("fork! /n")后,“fork!”被立即打印到了屏幕上,之后fork到的子进程里的stdout缓冲里不会有fork! 内容。因此你看到的结果会是fork! 被printf了1次！！！！
所以说printf("+");不能正确地反应进程的数量。
    大家看了这么多可能有点疲倦吧，不过我还得贴最后一份代码来进一步分析fork函数。
**[cpp]**[view
 plain](http://blog.csdn.net/jason314/article/details/5640969#)[copy](http://blog.csdn.net/jason314/article/details/5640969#)
- #include <stdio.h>
- #include <unistd.h>
- int main(int argc, char* argv[])  
- {  
-    fork();  
-    fork() && fork() || fork();  
-    fork();  
- return 0;  
- }  
   
 问题是不算main这个进程自身，程序到底创建了多少个进程。
    为了解答这个问题，我们先做一下弊，先用程序验证一下，到此有多少个进程。
**[c-sharp]**[view
 plain](http://blog.csdn.net/jason314/article/details/5640969#)[copy](http://blog.csdn.net/jason314/article/details/5640969#)
- #include <stdio.h>
- int main(int argc, char* argv[])  
- {  
-    fork();  
-    fork() && fork() || fork();  
-    fork();  
-    printf("+/n");  
- }  
    答案是总共20个进程，除去main进程，还有19个进程。
    我们再来仔细分析一下，为什么是还有19个进程。
    第一个fork和最后一个fork肯定是会执行的。
    主要在中间3个fork上，可以画一个图进行描述。
    这里就需要注意&&和||运算符。
    A&&B，如果A=0，就没有必要继续执行&&B了；A非0，就需要继续执行&&B。
    A||B，如果A非0，就没有必要继续执行||B了，A=0，就需要继续执行||B。
    fork()对于父进程和子进程的返回值是不同的，按照上面的A&&B和A||B的分支进行画图，可以得出5个分支。
![](http://hi.csdn.net/attachment/201006/1/0_1275406158t1t0.gif)
     加上前面的fork和最后的fork，总共4*5=20个进程，除去main主进程，就是19个进程了。
**三、fork高阶知识**
        这一块我主要就fork函数讲一下操作系统进程的创建、死亡和调度等。因为时间和精力限制，我先写到这里，下次找个时间我争取把剩下的内容补齐。
**参考资料：**
[http://blog.csdn.net/dog_in_yellow/archive/2008/01/13/2041079.aspx](http://blog.csdn.net/dog_in_yellow/archive/2008/01/13/2041079.aspx)
[http://blog.chinaunix.net/u1/53053/showart_425189.html](http://blog.chinaunix.net/u1/53053/showart_425189.html)
[http://blog.csdn.net/saturnbj/archive/2009/06/19/4282639.aspx](http://blog.csdn.net/saturnbj/archive/2009/06/19/4282639.aspx)
[http://www.cppblog.com/zhangxu/archive/2007/12/02/37640.html](http://www.cppblog.com/zhangxu/archive/2007/12/02/37640.html)
[http://www.qqread.com/linux/2010/03/y491043.html](http://www.qqread.com/linux/2010/03/y491043.html)
[http://www.yuanma.org/data/2009/1103/article_3998.htm](http://www.yuanma.org/data/2009/1103/article_3998.htm)
*******************************************************************************************************************************************************************************************
[http://coolshell.cn/articles/7965.html](http://coolshell.cn/articles/7965.html)
前两天有人问了个关于Unix的fork()系统调用的面试题，这个题正好是我大约十年前找工作时某公司问我的一个题，我觉得比较有趣，写篇文章与大家分享一下。这个题是这样的：
**题目：请问下面的程序一共输出多少个“-”？**
```
```cpp
#include
 <stdio.h>
```
```cpp
#include
 <sys/types.h>
```
```cpp
#include
 <unistd.h>
```
```cpp
int
```
```cpp
main(
```
```cpp
void
```
```cpp
)
```
```cpp
{
```
```cpp
```
```cpp
int
```
```cpp
i;
```
```cpp
```
```cpp
for
```
```cpp
(i=0;
 i<2; i++){
```
```cpp
```
```cpp
fork();
```
```cpp
```
```cpp
printf
```
```cpp
(
```
```cpp
"-"
```
```cpp
);
```
```cpp
```
```cpp
}
```
```cpp
```
```cpp
return
```
```cpp
0;
```
```cpp
}
```
```
如果你对fork()的机制比较熟悉的话，这个题并不难，输出应该是6个“-”，但是，实际上这个程序会很tricky地输出8个“-”。
要讲清这个题，我们首先需要知道fork()系统调用的特性，
- fork()系统调用是Unix下以自身进程创建子进程的系统调用，一次调用，两次返回，如果返回是0，则是子进程，如果返回值>0，则是父进程（返回值是子进程的pid），这是众为周知的。
- 还有一个很重要的东西是，在fork()的调用处，整个父进程空间会原模原样地复制到子进程中，包括指令，变量值，程序调用栈，环境变量，缓冲区，等等。
所以，上面的那个程序为什么会输入8个“-”，这是因为printf(“-”);语句有buffer，所以，对于上述程序，printf(“-”);把“-”放到了缓存中，并没有真正的输出（参看《[C语言的迷题](http://coolshell.cn/articles/945.html)》中的第一题），**在fork的时候，缓存被复制到了子进程空间**，所以，就多了两个，就成了8个，而不是6个。
另外，多说一下，我们知道，Unix下的设备有“[块设备](http://en.wikipedia.org/wiki/Device_file#Block_devices)”和“[字符设备](http://en.wikipedia.org/wiki/Device_file#Character_devices)”的概念，所谓块设备，就是以一块一块的数据存取的设备，字符设备是一次存取一个字符的设备。磁盘、内存都是块设备，字符设备如键盘和串口。**块设备一般都有缓存，而字符设备一般都没有缓存**。
对于上面的问题，我们如果修改一下上面的printf的那条语句为：
```
```cpp
printf
```
```cpp
(
```
```cpp
"-\n"
```
```cpp
);
```
```
或是
```
```cpp
printf
```
```cpp
(
```
```cpp
"-"
```
```cpp
);
```
```cpp
fflush
```
```cpp
(stdout);
```
```
就没有问题了（就是6个“-”了），因为程序遇到“\n”，或是EOF，或是缓中区满，或是文件描述符关闭，或是主动flush，或是程序退出，就会把数据刷出缓冲区。需要注意的是，标准输出是行缓冲，所以遇到“\n”的时候会刷出缓冲区，但对于磁盘这个块设备来说，“\n”并不会引起缓冲区刷出的动作，那是全缓冲，你可以使用setvbuf来设置缓冲区大小，或是用fflush刷缓存。
我估计有些朋友可能对于fork()还不是很了解，那么我们把上面的程序改成下面这样：
```
```cpp
#include
 <stdio.h>
```
```cpp
#include
 <sys/types.h>
```
```cpp
#include
 <unistd.h>
```
```cpp
int
```
```cpp
main(
```
```cpp
void
```
```cpp
)
```
```cpp
{
```
```cpp
```
```cpp
int
```
```cpp
i;
```
```cpp
```
```cpp
for
```
```cpp
(i=0;
 i<2; i++){
```
```cpp
```
```cpp
fork();
```
```cpp
```
```cpp
//注意：下面的printf有“\n”
```
```cpp
```
```cpp
printf
```
```cpp
(
```
```cpp
"ppid=%d,
 pid=%d, i=%d \n"
```
```cpp
,
 getppid(), getpid(), i);
```
```cpp
```
```cpp
}
```
```cpp
```
```cpp
sleep(10);
```
```cpp
//让进程停留十秒，这样我们可以用pstree查看一下进程树
```
```cpp
```
```cpp
return
```
```cpp
0;
```
```cpp
}
```
```
于是，上面这段程序会输出下面的结果，（注：编译出的可执行的程序名为fork）
```
```bash
ppid=8858,
 pid=8518, i=0
```
```bash
ppid=8858,
 pid=8518, i=1
```
```bash
ppid=8518,
 pid=8519, i=0
```
```bash
ppid=8518,
 pid=8519, i=1
```
```bash
ppid=8518,
 pid=8520, i=1
```
```bash
ppid=8519,
 pid=8521, i=1
```
```bash
$
 pstree -p |
```
```bash
grep
```
```bash
fork
```
```bash
|-
```
```bash
bash
```
```bash
(8858)-+-fork(8518)-+-fork(8519)---fork(8521)
```
```bash
|           
 |            `-fork(8520)
```
```
面对这样的图你可能还是看不懂，没事，我好事做到底，画个图给你看看：
![](http://coolshell.cn//wp-content/uploads/2012/07/fork01jpg.jpg)
注意：上图中的我用了几个色彩，相同颜色的是同一个进程。于是，我们的pstree的图示就可以成为下面这个样子：（下图中的颜色与上图对应）
![](http://coolshell.cn//wp-content/uploads/2012/07/fork02.jpg)
这样，对于printf(“-”);这个语句，我们就可以很清楚的知道，哪个子进程复制了父进程标准输出缓中区里的的内容，而导致了多次输出了。（如下图所示，就是我阴影并双边框了那两个子进程）
![](http://coolshell.cn//wp-content/uploads/2012/07/fork03.jpg)
现在你明白了吧。（另，对于图中的我本人拙劣的配色，请见谅!）
（全文完）
********************************************************************************************************************************************************************************
[http://blog.sina.com.cn/s/blog_8dc64660010158r9.html](http://blog.sina.com.cn/s/blog_8dc64660010158r9.html)
## linux下监控进程是否挂掉的一种方法
有的服务器进程都要求持续无间断的工作，但是有时在高并发，长时间的压力下，服务器进程可能会出现一些意想不到的错误，例如常见的内存非法访问的错误，而导致程序直接挂掉。所以，有时候对进程进行实时监控，当发现进程挂掉时，立刻重新启动进程，也是一种可以救急的方式（当然这个只是一种临时救急，并不是根本解决方法）。
实现方式：
使用fork（）创建子进程，子进程用于执行具体功能，主进程只是用于监控子进程，当主进程检测到子进程挂掉后，可以实现立即重新启动子进程。
子进程结束，系统会向主进程发送信号：SIGCHLD，主进程可以通过捕捉该信号，从而检测子进程已经不存在，进而继续下一步操作。如果需要，主进程还可以获得子进程是为何退出的。
源代码例子：
#include
#include
#include
#include
#include
#include
#include
void process_exit(int s)
{
exit(0);
}
void child_fun()
{
printf("child_fun. ppid %d\n",getppid());
char *st = NULL;
strcpy(st, "123");
}
void fork_child()
{
pid_t child_process;
int status;
int signal_num;
wait(&status);//等待子进程中断或终止，释放子进程资源，否则死掉的子进程会变成僵尸进程
//如果子进程是由于某种信号退出的，捕获该信号
if(WIFSIGNALED(status))
signal_num = WTERMSIG(status);
child_process = fork();
if(child_process == 0)
{
printf("fork new child process.\n");
child_fun();
}
}
int main()
{
pid_t child_process;
int i = 0;
while(1)
{
printf("fork new process.\n");
child_process = fork();
if(child_process > 0)
{
while(1)
{
//捕获子进程结束信号
signal(SIGCHLD, fork_child);
signal(SIGTERM, process_exit);
pause();//主进程休眠，当有信号到来时被唤醒。
}
}
else if(child_process == 0)
{
child_fun();
}
}
return 0;
}
僵尸进程的产生：
在fork()/execve()过程中，假设子进程结束时父进程仍存在，而父进程如果没有给子进程收尸，死掉的子进程就变成僵尸进程了。僵尸进程是非常特殊的一种，它已经放弃了几乎所有内存空间，没有任何可执行代码，也不能被调度，仅仅在进程列表中保留一个位置，记载该进程的退出状态等信息供其他进程收集，除此之外，僵尸进程不再占有任何内存空间，它需要它的父进程来为它收尸。僵尸进程，无法正常结束，此时即使是root身份kill-9也不能杀死僵尸进程。补救办法是杀死僵尸进程的父进程(僵尸进程的父进程必然存在)，僵尸进程成为"孤儿进程"，过继给1号进程init，init始终会负责清理僵尸进程。
僵尸进程的危害：
Linux系统对运行的进程数量有限制，如果产生过多的僵尸进程占用了可用的进程号，将会导致新的进程无法生成。这就是僵尸进程对系统的最大危害。
父进程给子进程收尸的方法是：安装SIGCHLD信号处理函数调用wait或waitpid()等待子进程结束。 
****************************************************************************************************************************************************************
[http://blog.csdn.net/lubber__land/article/details/8890328](http://blog.csdn.net/lubber__land/article/details/8890328)
从lighttpd的源码里找到点启发，仿照lighttpd写了一个多进程监控的例子，一个父进程，n个子进程，如果子进程有异常退出的情况，父进程负责监控并重新fork一个子进程。
发SIGTERM信号给父进程，父进程和子进程必须都退出。
废话不说，代码如下：
**[cpp]**[view plain](http://blog.csdn.net/lubber__land/article/details/8890328#)[copy](http://blog.csdn.net/lubber__land/article/details/8890328#)[print](http://blog.csdn.net/lubber__land/article/details/8890328#)[?](http://blog.csdn.net/lubber__land/article/details/8890328#)
- #include <stdio.h>
- #include <iostream>
- #include <string.h>
- #include <string>
- #include <unistd.h>
- #include <signal.h>
- #include <sys/wait.h>
- #include <sys/types.h>
- #include <errno.h>
- #include<locale.h>
- 
- usingnamespace std;
- 
- staticvolatile
sig_atomic_t app_shutdown = 0; 
- 
- staticvoid signal_handler(int sig)
- { 
- switch (sig) 
- { 
- case SIGTERM: 
- app_shutdown = 1; 
- break; 
- } 
- } 
- 
- int main() 
- { 
- struct sigaction act; 
- memset(&act, 0, sizeof(act)); 
- act.sa_handler = SIG_IGN; 
- sigaction(SIGPIPE, &act, NULL); 
- sigaction(SIGUSR1, &act, NULL); 
- act.sa_handler = signal_handler; 
- sigemptyset(&act.sa_mask); 
- act.sa_flags = 0; 
- sigaction(SIGTERM, &act, NULL); 
- 
- /* start watcher and workers */
- int num_childs = 5; 
- if (num_childs > 0) 
- { 
- int child = 0; 
- while (!child && !app_shutdown) 
- { 
- if (num_childs > 0) 
- { 
- switch (fork()) 
- { 
- case -1: 
- return -1; 
- case 0: 
- child = 1; 
- break; 
- default: 
- num_childs--; 
- break; 
- } 
- } 
- else
- { 
- int status; 
- if (-1 != wait(&status)) 
- { 
- /** 
- * one of our workers went away 
- */
- num_childs++; 
- } 
- } 
- } 
- 
- /**
- * for the parent this is the exit-point 
- */
- if (!child) 
- { 
- /** 
- * kill all children too 
- */
- if (app_shutdown) 
- { 
- kill(0, SIGTERM); 
- } 
- 
- return 0; 
- 
- } 
- } 
- /**
- * children process
- */
- while (!app_shutdown) 
- { 
- //do something
- } 
- 
- return 0; 
- } 
```cpp
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <string>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>
#include<locale.h>
using namespace std;
static volatile sig_atomic_t app_shutdown = 0;
static void signal_handler(int sig)
{
	switch (sig)
  {
  	case SIGTERM: 
      app_shutdown = 1;
      break;    
	}
}
int main()
{
  struct sigaction act;
  memset(&act, 0, sizeof(act));
	act.sa_handler = SIG_IGN;
	sigaction(SIGPIPE, &act, NULL);
	sigaction(SIGUSR1, &act, NULL);
	act.sa_handler = signal_handler;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;  
	sigaction(SIGTERM, &act, NULL);
  
/* start watcher and workers */
	int num_childs = 5;
	if (num_childs > 0)
  {
  	int child = 0;
  	while (!child && !app_shutdown) 
    {
  		if (num_childs > 0)
      {
  			switch (fork())
        {
    			case -1:
    				return -1;
    			case 0:
    				child = 1;
    				break;
    			default:
    				num_childs--;
    				break;
  			}
      }
      else 
      {
  			int status;
  			if (-1 != wait(&status))
        {
  				/** 
  				 * one of our workers went away 
  				 */
  				num_childs++;
        }
  		}
  	}
  	/**
  	 * for the parent this is the exit-point 
  	 */
  	if (!child)
    {
  		/** 
  		 * kill all children too 
  		 */
  		if (app_shutdown)
      {
  			kill(0, SIGTERM);
  		}
  		return 0;
  	}
  }
  /**
	 * children process
   */
  while (!app_shutdown)
  {
    //do something
  }  
  return 0;
}
```
退出流程：
如果发SIGTERM信号给父进程，kill -s SIGTERM parent_processid,
父进程的信号处理函数 signal_handler 将 父进程的静态变量app_shutdown 修改1
父进程跳出while循环后，走到 kill all children too 的处理流程，通过kill(0, SIGTERM); 给所有的子进程发SIGTERM信号
然后退出，
子进程的信号处理函数 signal_handler 将 子进程的静态变量app_shutdown 修改1
那么子进程跳出循环后，返回0，也就退出了
（父进程先退出，所以就没有处理僵尸进程的步骤了）
监控流程：
如果有一个子进程退出了，程序走到if (-1 != wait(&status))，那么num_childs++ 后，又会重新fork一个子进程
可以修改signal_handler 函数和增加静态变量，来控制不同信号产生的不同动作

