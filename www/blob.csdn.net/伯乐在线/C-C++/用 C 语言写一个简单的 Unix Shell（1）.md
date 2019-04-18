# 用 C 语言写一个简单的 Unix Shell（1） - 文章 - 伯乐在线
本文由 [伯乐在线](http://blog.jobbole.com) - [效楚](http://www.jobbole.com/members/qdoverture) 翻译，[Panblack](http://www.jobbole.com/members/upanblack) 校稿。未经许可，禁止转载！
英文出处：[Indradhanush Gupta](https://indradhanush.github.io/blog/writing-a-unix-shell-part-1/)。欢迎加入[翻译组](https://github.com/jobbole/translation-project)。
【导读】：作者用 C 语言实现了一个简易的unix shell，通过本文可加深对 shell 和 Unix 系统原理的理解。
写 Unix shell 是我正在 RC 研究的一个项目。这是第一部分，后续会有一系列的文章。
**免责声明：我不是编写 shell 这个课题的专家，我是一边自学一边分享我的发现。**
## shell 是什么？
关于这一点已经有很多书面资料，所以对于它的定义我不会探讨太多细节。只用一句话说明：
> 
shell 是允许你与操作系统的核心作交互的一个界面（interface）。
## shell 是怎样工作的？
shell解析用户输入的命令并执行它。为了能做到这一点，shell的工作流程看起来像这样：
- 启动shell
- 等待用户输入
- 解析用户输入
- 执行命令并返回结果
- 回到第 2 步。
但在这整个流程中有一个重要的部分：**进程**。shell是父进程。这是我们的程序的主线程，它等待用户输入。然而，由于以下原因，我们不能在主线程自身中执行命令：
- 一个错误的命令会导致整个shell停止工作。我们要避免此情况。
- 独立的命令应该有他们自己的进程块。这被称为隔离，属于容错（机制）。
## Fork
为了能避免此情况，我们使用系统调用 fork。我曾以为我理解了 fork，直到我用它写了大约4行代码（才发现我没有理解）。
``fork 创建当前进程的一份拷贝。这份拷贝被称为“子进程”，系统中的每个进程都有与它联系在一起的唯一的进程 id（pid）。让我们看以下代码片段：
[*fork.c*](https://indradhanush.github.io/code/shell-part-1/fork.c)


```
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int
main() {
    pid_t child_pid = fork();
        
    // The child process
    if (child_pid == 0) {
        printf("### Child ###nCurrent PID: %d and Child PID: %dn",
               getpid(), child_pid);
    } else {
        printf("### Parent ###nCurrent PID: %d and Child PID: %dn",
               getpid(), child_pid);
    }
 
    return 0;
}
```
fork 系统调用返回两次，每个进程一次。这一开始听起来是反直觉的。但让我们看一下在底层发生了什么。
- 通过调用 fork，我们在程序中创建了一个新的分支。这与传统的 if-else 分支不同。fork 对当前进程创建一份拷贝并从中创建了一个新的进程。最终系统调用返回子进程的进程 id。
- 一旦 fork 调用成功，子进程和父进程（我们的代码的主线程）会同时运行。
为了让你更好理解程序流程，看这个图：
![fork](http://jbcdn2.b0.upaiyun.com/2017/06/309a7a84764a07033c28b6ce5511c45c.jpg)
fork() 创建了一个新的子进程，但与此同时，父进程的执行并没有停止。子进程执行的开始和结束独立于父进程，反之亦然。
更进一步讨论以前，先说明一点：getpid 系统调用返回当前的进程 id。
如果你编译并执行这段代码，会得到类似于下面的输出：


```
### Parent ###
Current PID: 85247 and Child PID: 85248
### Child ###
Current PID: 85248 and Child PID: 0
```
在 ### Parent ### 下面的片段中，当前进程 ID 是 85247，子进程 ID 是 85248。注意，子进程的 pid 比父进程的大，表明子进程是在父进程之后创建的。（更新：正如某人在 [Hacker News](https://news.ycombinator.com/item?id=14439781) 上正确指出的，这并不是确定的，虽然往往是这样。原因在于，操作系统可能回收无用的老进程 id。）
在 ### Child ### 下面的片段中，当前进程 ID 是 85248，这与前面片段中子进程的 pid 相同。然而，这里的子进程 pid 为 0。
实际的数字会随着每一次执行而变化。
你可能在想，我们已经在第 9 行明确的给 child_pid 赋了一个值（译者注：应该是第7行），那么 child_pid 怎么会在同一个执行流程中呈现两个不同的值，这种想法值得原谅。但是，回想一下，调用 fork 创建了一个新进程，这个新进程与当前进程相同。因此，在父进程中，child_pid 是刚创建的子进程的实际值，而子进程本身没有自己的子进程，所以 child_pid 的值为 0。
因此，为了控制哪些代码在子进程中执行，哪些又在父进程中执行，需要我们在 12 到 16 行定义的 if-else 块（译者注：应该是 10 到 16 行）。当 child_pid 为 0 时，代码块将在子进程下执行，而 else 块却会在父进程下执行。这些块被执行的顺序是不确定的，取决于操作系统的调度程序。
## 引入确定性
让我向你介绍系统调用 sleep。引用 linux man 页面的话：
> 
sleep – 暂停执行一段时间
时间间隔以秒为单位。
让我们给父进程，即我们代码中的 else 块，加一个 sleep(1) 调用：
[*sleep_parent.c*](https://indradhanush.github.io/code/shell-part-1/sleep_parent.c)


```
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int
main() {
    pid_t child_pid = fork();
    // The child process
    if (child_pid == 0) {
        printf("### Child ###nCurrent PID: %d and Child PID: %dn",
               getpid(), child_pid);
    } else {
        sleep(1); // Sleep for one second
        printf("### Parent ###nCurrent PID: %d and Child PID: %dn",
               getpid(), child_pid);
    }
 
    return 0;
}
```
当你执行这段代码时，输出将类似这样：


```
### Child ###
Current PID: 89743 and Child PID: 0
```
1秒钟以后，你将看到


```
### Parent ###
Current PID: 89742 and Child PID: 89743
```
每次执行这段代码时你会看到同样的表现。这是因为：我们在父进程中做了一个阻塞性的 sleep 调用，与此同时，操作系统调度程序发现有空闲的 CPU 时间可以给子进程执行。
类似的，如果你反过来，把 sleep(1) 调用加到子进程，也就是我们代码中的 if 块里面，你会发现父进程块立刻输出到控制台上。但你也会发现程序终止了。子进程块的输出被转存到标准输出。看起来是这样：


```
$ gcc -lreadline blog/sleep_child.c -o sleep_child && ./sleep_child
### Parent ###
Current PID: 23011 and Child PID: 23012
$ ### Child ###
Current PID: 23012 and Child PID: 0
```
这段源代码可在 [sleep_child.c](https://indradhanush.github.io/code/shell-part-1/sleep_child.c) 获取。
这是因为父进程在 printf 语句之后无事可做，被终止了。然而，子进程在 sleep 调用处被阻塞了 1 秒钟，之后才执行 printf 语句。
## 正确实现的确定性
然而，使用 sleep 来控制进程的执行流程不是最好的方法，因为你做了一个 n 秒的 sleep 调用：
- 你怎么确保不管你等待的是什么，都会在 n 秒内完成执行呢？
- 不管你等待的是什么，要是它在远远早于 n 秒时就结束了呢？在此情况下你不必要地闲置了。
有一种更好的方法是，使用 wait 系统调用（或一种变体）来代替。我们将使用 waitpid 系统调用。它带有以下参数：
- 你想要程序等待的进程的进程 ID。
- 一个变量，用来保存进程如何终止的相关信息。
- 选项标志，用来定制 waitpid 的行为
[*wait.c*](https://indradhanush.github.io/code/shell-part-1/wait.c)


```
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
int
main() {
    pid_t child_pid;
    pid_t wait_result;
    int stat_loc;
    child_pid = fork();
        
    // The child process
    if (child_pid == 0) {
        printf("### Child ###nCurrent PID: %d and Child PID: %dn",
               getpid(), child_pid);
        sleep(1); // Sleep for one second
    } else {
        wait_result = waitpid(child_pid, &stat_loc, WUNTRACED);
        printf("### Parent ###nCurrent PID: %d and Child PID: %dn",
               getpid(), child_pid);
    }
 
    return 0;
}
```
当你执行这段代码，你会发现子进程块立刻被打印，然后等待很短的一段时间（这里我们在 printf 后面加了 sleep）。父进程等待子进程执行结束，之后就有空执行它自己的命令。
第一部分到此结束。这篇博客中的所有代码示例可以在[这里](https://github.com/indradhanush/indradhanush.github.io/tree/master/code/shell-part-1/)获取。在[下一篇](https://indradhanush.github.io/blog/writing-a-unix-shell-part-2/)中，我们将研究怎样在用户输入时接受命令并执行它。敬请期待。
**致谢**
谢谢 [Saul Pwanson](https://github.com/saulpw) 帮助我理解 fork 的表现方式，谢谢 [Jaseem Abid](https://github.com/jaseemabid) 阅读草稿并提出编辑建议。
**参考资料**
- [EnthusiastiCon – Stefanie Schirmer：《天哪！在 10 分中内构建一个 shell”](https://www.youtube.com/watch?v=k6TTj4C0LF0)》
- [Linux man 页面](https://linux.die.net/man/)
99e05c43dc61c56cbc182203.jpeg”>
> 
**打赏支持我翻译更多好文章，谢谢！**
[打赏译者](#rewardbox)
#### 打赏支持我翻译更多好文章，谢谢！
任选一种支付方式
![](http://jbcdn2.b0.upaiyun.com/2017/06/8204a635b9f0335c5b2e27498c3d5c97.jpg)![](http://jbcdn2.b0.upaiyun.com/2017/06/0fbc02d33dc26bbfebc27f81bea9cdf9.jpg)
