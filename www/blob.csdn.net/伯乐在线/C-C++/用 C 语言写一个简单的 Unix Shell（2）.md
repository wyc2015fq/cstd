# 用 C 语言写一个简单的 Unix Shell（2） - 文章 - 伯乐在线
本文由 [伯乐在线](http://blog.jobbole.com) - [效楚](http://www.jobbole.com/members/qdoverture) 翻译，[Panblack](http://www.jobbole.com/members/upanblack) 校稿。未经许可，禁止转载！
英文出处：[indradhanush.github.io](https://indradhanush.github.io/blog/writing-a-unix-shell-part-2/)。欢迎加入[翻译组](https://github.com/jobbole/translation-project)。
[在第一部分中我们讨论了 `fork` 系统调用以及它的注意事项](http://blog.jobbole.com/111738/)。在本文中，我们将研究怎样执行命令。
### 这里将介绍 `exec` 函数家族。即以下函数：
- `execl`
- `execv`
- `execle`
- `execve`
- `execlp`
- `execvp`
为了满足需要，我们将使用 `execvp`，它的签名看起来像这样：

C
```
int execvp(const char *file, char *const argv[]);
```
函数名中的 `vp` 表明：它接受一个文件名，将在系统 $PATH 变量中搜索此文件名，它还接受将要执行的一组参数。
你可以阅读 [exec 的 man 页面](https://linux.die.net/man/3/exec) 以得到其它函数的更多信息。
让我们看一下以下代码，它执行命令 `ls -l -h -a`：
[execvp.c](https://indradhanush.github.io/code/shell-part-2/execvp.c)

C
```
#include <unistd.h>
int main() {
    char *argv[] = {"ls", "-l", "-h", "-a", NULL};
    execvp(argv[0], argv);
    return 0;
}
```
关于 `execvp` 函数，有几点需要注意：
- 第一个参数是命令名。
- 第二个参数由命令名和传递给命令自身的参数组成。并且它必须以 `NULL` 结束。
- 它将当前进程的映像交换为被执行的命令的映像，后面再展开说明。
如果你编译并执行上面的代码，你会看到类似于下面的输出：


```
total 32
drwxr-xr-x  5 dhanush  staff   170B Jun 11 11:32 .
drwxr-xr-x  4 dhanush  staff   136B Jun 11 11:30 ..
-rwxr-xr-x  1 dhanush  staff   8.7K Jun 11 11:32 a.out
drwxr-xr-x  3 dhanush  staff   102B Jun 11 11:32 a.out.dSYM
-rw-r--r--  1 dhanush  staff   130B Jun 11 11:32
```
它和你在你的主 shell 中手动执行`ls -l -h -a`的结果完全相同。
既然我们能执行命令了，我们需要使用在[第一部分](http://blog.jobbole.com/111738/)中学到的`fork` 系统调用构建有用的东西。事实上我们要做到以下这些：
- 当用户输入时接受命令。
- 调用 `fork` 以创建一个子进程。
- 在子进程中执行命令，同时父进程等待命令完成。
- 回到第一步。
我们看看下面的函数，它接收一个字符串作为`输入`。我们使用库函数 `strtok` 以`空格`分割该字符串，然后返回一个字符串数组，数组也用 `NULL`来终结。

C
```
include <stdlib.h>
#include <string.h>
char **get_input(char *input) {
    char **command = malloc(8 * sizeof(char *));
    char *separator = " ";
    char *parsed;
    int index = 0;
    parsed = strtok(input, separator);
    while (parsed != NULL) {
        command[index] = parsed;
        index++;
        parsed = strtok(NULL, separator);
    }
    command[index] = NULL;
    return command;
}
```
如果该函数的`输入`是字符串 `"ls -l -h -a"`，那么函数将会创建这样形式的一个数组：`["ls", "-l", "-h", "-a", NULL]`，并且返回指向此队列的指针。
现在，我们在`主`函数中调用 `readline` 来读取用户的输入，并将它传给我们刚刚在上面定义的 `get_input`。一旦输入被解析，我们在子进程中调用 `fork` 和 `execvp`。在研究代码以前，看一下下面的图片，先理解 `execvp` 的含义：
![](https://indradhanush.github.io/images/shell-part-2/execvp.jpg)
当 `fork` 命令完成后，子进程是父进程的一份精确的拷贝。然而，当我们调用 `execvp` 时，它将当前程序替换为在参数中传递给它的程序。这意味着，虽然进程的当前文本、数据、堆栈段被替换了，进程 id 仍保持不变，但程序完全被覆盖了。如果调用成功了，那么 `execvp` 将不会返回，并且子进程中在这之后的任何代码都不会被执行。这里是`主`函数：

C
```
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <readline/readline.h>
#include <unistd.h>
#include <sys/wait.h>
int main() {
    char **command;
    char *input;
    pid_t child_pid;
    int stat_loc;
    while (1) {
        input = readline("unixsh> ");
        command = get_input(input);
        child_pid = fork();
        if (child_pid == 0) {
            /* Never returns if the call is successful */
            execvp(command[0], command);
            printf("This won't be printed if execvp is successuln");
        } else {
            waitpid(child_pid, &stat_loc, WUNTRACED);
        }
        free(input);
        free(command);
    }
    return 0;
}
```
全部代码可在[此处](https://indradhanush.github.io/code/shell-part-2/shell.c)的单个文件中获取。如果你用 `gcc -g -lreadline shell.c` 编译它，并执行二进制文件，你会得到一个最小的可工作 shell，你可以用它来运行系统命令，比如 `pwd` 和 `ls -lha`：


```
unixsh> pwd
/Users/dhanush/github.com/indradhanush.github.io/code/shell-part-2
unixsh> ls -lha
total 28K
drwxr-xr-x 6 root root  204 Jun 11 18:27 .
drwxr-xr-x 3 root root 4.0K Jun 11 16:50 ..
-rwxr-xr-x 1 root root  16K Jun 11 18:27 a.out
drwxr-xr-x 3 root root  102 Jun 11 15:32 a.out.dSYM
-rw-r--r-- 1 root root  130 Jun 11 15:38 execvp.c
-rw-r--r-- 1 root root  997 Jun 11 18:25 shell.c
unixsh>
```
注意：`fork` 只有在用户输入命令后才被调用，这意味着接受用户输入的用户提示符是父进程。
## 错误处理
到目前为止，我们一直假设我们的命令总会完美的运行，还没有处理错误。所以我们要对 [shell.c](https://indradhanush.github.io/code/shell-part-2/shell.c)做一点改动：
- **fork** – 如果操作系统内存耗尽或是进程数量已经到了允许的最大值，子进程就无法创建，会返回 -1。我们在代码里加上以下内容：

C
```
...
    while (1) {
        input = readline("unixsh> ");
        command = get_input(input);
        child_pid = fork();
        if (child_pid < 0) {
            perror("Fork failed");
            exit(1);
        }
    ...
```
- **execvp** – 就像上面解释过的，被成功调用后它不会返回。然而，如果执行失败它会返回 -1。同样地，我们修改 `execvp` 调用：
- 

C
```
...
        if (execvp(command[0], command) < 0) {
            perror(command[0]);
            exit(1);
        }
...
```
注意：虽然`fork`之后的`exit`调用终止整个程序，但`execvp`之后的`exit` 调用只会终止子进程，因为这段代码只属于子进程。
- **malloc** – 如果操作系统内存耗尽，它就会失败。在这种情况下，我们应该退出程序：

C
```
char **get_input(char *input) {
char **command = malloc(8 * sizeof(char *));
if (command == NULL) {
perror("malloc failed");
exit(1);
}
...
```
- **动态内存分配** – 目前我们的命令缓冲区只分配了8个块。如果我们输入的命令超过8个单词，命令就无法像预期的那样工作。这么做是为了让例子便于理解，如何解决这个问题留给读者作为一个练习。
上面带有错误处理的代码可在[这里](https://indradhanush.github.io/code/shell-part-2/shell_with_error_handling.c)获取。
## 内建命令
如果你试着执行 `cd` 命令，你会得到这样的错误：


```
cd: No such file or directory
```
我们的 shell 现在还不能识别`cd`命令。这背后的原因是：cd不是`ls`或`pwd`这样的系统程序。让我们后退一步，暂时假设`cd` 也是一个系统程序。你认为执行流程会是什么样？在继续阅读之前，你可能想要思考一下。
流程是这样的：
- 用户输入 `cd /`。
- shell对当前进程作 `fork`，并在子进程中执行命令。
- 在成功调用后，子进程退出，控制权还给父进程。
- 父进程的当前工作目录没有改变，因为命令是在子进程中执行的。因此，`cd` 命令虽然成功了，但并没有产生我们想要的结果。
因此，要支持 `cd`，我们必须自己实现它。我们也需要确保，如果用户输入的命令是 `cd`（或属于预定义的内建命令），我们根本不要 `fork` 进程。相反地，我们将执行我们对 `cd`（或任何其它内建命令）的实现，并继续等待用户的下一次输入。``，幸运的是我们可以利用 `chdir` 函数调用，它用起来很简单。它接受路径作为参数，如果成功则返回0，失败则返回 -1。我们定义函数：

C
```
int cd(char *path) {
        return chdir(path);
    }
```
并且在我们的`主`函数中为它加入一个检查：

C
```
while (1) {
        input = readline("unixsh> ");
        command = get_input(input);
            if (strcmp(command[0], "cd") == 0) {
            if (cd(command[1]) < 0) {
                perror(command[1]);
            }
                /* Skip the fork */
            continue;
        }
    ...
```
带有以上更改的代码可从[这里](https://indradhanush.github.io/code/shell-part-2/shell_with_builtin.c)获取，如果你编译并执行它，你将能运行 `cd` 命令。这里是一个示例输出：

Shell
```
unixsh> pwd
/Users/dhanush/github.com/indradhanush.github.io/code/shell-part-2
unixsh> cd /
unixsh> pwd
/
unixsh>
```
第二部分到此结束。这篇博客帖文中的所有代码示例可在[这里](https://github.com/indradhanush/indradhanush.github.io/tree/master/code/shell-part-2/)获取。在[下一篇博客帖文](https://indradhanush.github.io/blog/writing-a-unix-shell-part-3/)中，我们将探讨信号的主题以及实现对用户中断（Ctrl-C）的处理。敬请期待。
## 致谢
谢谢 [Dominic Spadacene](https://github.com/domspad)和我搭档写这篇文章，谢谢 [Saul Pwanson](https://github.com/saulpw)帮我解决怪异的内存泄漏，当时代码貌似完全不能工作了。
**更新**：Saul提到：按照惯例，用 `< 0` 来检查错误比用 `== -1` 更好，因为有些 API 可能返回除了 `-1` 以外的负值，`<0` 有助于应对那些情况。我已经据此更新了本帖文和代码示例。
**参考资料**
- [UNIX 环境高级编程 – Stephens & Rago](http://amzn.to/2sc0p8o)
- [Linux 系统编程 – Robert Love](http://amzn.to/2sc3TYr)
- [Linux man 页面](https://linux.die.net/man/)
> 
**打赏支持我翻译更多好文章，谢谢！**
[打赏译者](#rewardbox)
#### 打赏支持我翻译更多好文章，谢谢！
任选一种支付方式
![](http://jbcdn2.b0.upaiyun.com/2017/06/8204a635b9f0335c5b2e27498c3d5c97.jpg)![](http://jbcdn2.b0.upaiyun.com/2017/06/0fbc02d33dc26bbfebc27f81bea9cdf9.jpg)
