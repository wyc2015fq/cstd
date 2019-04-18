# Linux 信号表 signals - xcw0754 - 博客园
# [Linux 信号表 signals](https://www.cnblogs.com/xcw0754/p/8433493.html)
Linux支持POSIX **reliable signals**和POSIX **real-time signals**。
### 信号处理
进程接受到信号之后会有一些默认的处理方式，如下为5种处理方式：
|type|description|
|----|----|
|Term|Default action is to terminate the process.|
|Ign|Default action is to ignore the signal.|
|Core|Default action is to terminate the process and dump core (see core(5)).|
|Stop|Default action is to stop the process.|
|Cont|Default action is to continue the process if it is currently stopped.|
若想改变signal的默认处理方式，可以看`signal`函数和`sigaction`函数。
### 发送信号
下面是一些**系统调用**或者**库函数**可以用于发送信号给某进程：
|系统调用或库函数|描述|
|----|----|
|raise|Sends a signal to the calling thread.|
|kill|Sends a signal to a specified process, to all members of a specified process group, or to all processes on the system.|
|killpg|Sends a signal to all of the members of a specified process group.|
|pthread_kill|Sends a signal to a specified POSIX thread in the same process as the caller.|
|tgkill|Sends a signal to a specified thread within a specific process. (This is the system call used to implement pthread_kill(3).)|
|sigqueue|Sends a real-time signal with accompanying data to a specified process.|
### 信号类型
Linux支持的信号很多，常用的有如下这些：
|Signal|Value|Action|Comment|
|----|----|----|----|
|SIGHUP|1|Term|Hangup detected on controlling terminal or death of controlling process|
|SIGINT|2|Term|Interrupt from keyboard|
|SIGQUIT|3|Core|Quit from keyboard|
|SIGILL|4|Core|Illegal Instruction|
|SIGABRT|6|Core|Abort signal from abort(3)|
|SIGFPE|8|Core|Floating point exception|
|SIGKILL|9|Term|Kill signal|
|SIGSEGV|11|Core|Invalid memory reference|
|SIGPIPE|13|Term|Broken pipe: write to pipe with no readers|
|SIGALRM|14|Term|Timer signal from alarm(2)|
|SIGTERM|15|Term|Termination signal|
|SIGUSR1|30,10,16|Term|User-defined signal 1|
|SIGUSR2|31,12,17|Term|User-defined signal 2|
|SIGCHLD|20,17,18|Ign|Child stopped or terminated|
|SIGCONT|19,18,25|Cont|Continue if stopped|
|SIGSTOP|17,19,23|Stop|Stop process|
|SIGTSTP|18,20,24|Stop|Stop typed at terminal|
|SIGTTIN|21,21,26|Stop|Terminal input for background process|
|SIGTTOU|22,22,27|Stop|Terminal output for background process|
其中value是可能的取值，因为在不同的系统上它的值不一样，最好使用宏进行操作比较安全，移植性也需要考虑。

