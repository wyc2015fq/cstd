
# Linux kill, killall, kill -9 - 阳光岛主 - CSDN博客

2013年07月22日 00:16:08[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：12456


**1） 查看进程的方法：**
ps -ef或者ps aux
root     15087  0.0  0.0      0     0 ?        S    23:31   0:00 [kworker/1:1]
root     15219  0.0  0.0      0     0 ?        S    23:36   0:00 [kworker/1:0]
root     15460  0.0  0.0      0     0 ?        S    23:41   0:00 [kworker/1:2]
homer    15572 23.0  5.8 791472 119788 ?       Sl   23:45   0:03 /usr/lib/firefox/firefox
homer    15590  0.1  0.1 273796  3132 ?        Sl   23:45   0:00 /usr/lib/at-spi2-core/at-spi-bus-launcher
homer    15620  0.0  0.0  22360  1268 pts/0    R+   23:45   0:00 ps aux

**2） kill -9**
kill  -s  9  15572
其中，-s 9 制定了传递给进程的信号是９，即强制、尽快终止进程。 15572则是上面ps查到的火狐（firefox）的PID。
简单吧，但有个问题，进程少时ps还无所谓，进程多了，就会觉得痛苦了，无论是ps -ef 还是ps -aux，每次都要在一大串进程信息里面查找到要杀的进程PID，看的眼都花了。
**用法详解：**
kill -9， 这个强大和危险的命令迫使进程在运行时突然终止，进程在结束后不能自我清理。危害是导致系统资源无法正常释放，一般不推荐使用，除非其他办法都无效。
当使用此命令时，一定要通过ps -ef确认没有剩下任何僵尸进程。只能通过终止父进程来消除僵尸进程。如果僵尸进程被init收养，问题就比较严重了。杀死init进程意味着关闭系统。
如果系统中有僵尸进程，并且其父进程是init，而且僵尸进程占用了大量的系统资源，那么就需要在某个时候重启机器以清除进程表了。

**2.1） 改进 1 —— grep**
把ps的查询结果通过管道传给grep，查找包含特定字符串的进程。管道符“|”用来隔开两个命令，管道符左边命令的输出会作为管道符右边命令的输入。
例如：homer@ubuntu:~$ps -aux | grep firefox
homer    15572  1.7  5.0 808644 103260 ?       Sl   23:45   0:07 /usr/lib/firefox/firefox
homer    15735  0.0  0.0  13584   920 pts/0    S+   23:52   0:00 grep --color=auto firefox
直接找到firefox进程PID， 输入：kill  -s  915572

**2.2） 改进 2 —— pgrep**
pgrep的p表明了这个命令是专门用于进程查询的grep
例如：homer@ubuntu:~$pgrep firefox
15572

**2.3） 改进 3 —— pidof**
pidof命令，表示 pid of xx，字面翻译过来就是 xx的PID
例如：homer@ubuntu:~$pidof firefox
15572

**2.4） 改进4 —— grep + awk**
grep firefox 列出firefox进程信息，并awk取第二个field，即PID
例如：homer@ubuntu:~$ps -ef | grep firefox | grep -v grep | awk '{print $2}'
15572

**2.5） 改进 5 —— kill + xargs**
杀死进程的方式有以下几种：
a）**ps**-ef | grep firefox | grep -v grep | awk '{print $2}' |**xargs**kill -s 9
b）**pgrep**firefox | xargs kill -s 9
c）**pidof**firefox | xargs kill -s 9
或
d）kill -s 9 `**ps**-ef | grep firefox | grep -v grep | awk '{print $2}'`
e）kill -s 9 `**pgrep**firefox`
f）kill -s 9 `**pidof**firefox`

**2.6） 改进 6 —— pkill**
pkill 类似于 pgrep， pkill 表示pgrep+kill
例如：pkill firefox

**3） killall**
killall命令， 杀死同一进程组内的所有进程，其允许指定要终止的进程的名称，而非PID
killall和pkill是相似的,不过如果给出的进程名不完整，killall会报错。pkill或者pgrep只要给出进程名的一部分就可以终止进程。
homer@ubuntu:~$killall firefo
firefo: no process found
homer@ubuntu:~$killall firefox
homer@ubuntu:~$killall -9 firefox

**4） kill**
杀死进程最安全的方法是单纯使用kill命令，不加修饰符，不带标志。
例如：\# kill -pid
注释： 标准的kill命令，默认采用信号（signal）号是15，通常都能达到目的，终止有问题的进程，并把进程的资源释放给系统。然而，如果进程启动了子进程，只杀死父进程，子进程仍在运行，因此仍消耗资源。为了防止这些所谓的“僵尸进程”，应确保在杀死父进程之前，先杀死其所有的子进程。

**5） kill -l**
例如： kill -l PID
-l 选项， 告诉kill命令用好像启动进程的用户已注销的方式结束进程。当使用该选项时，kill命令也试图杀死所留下的子进程。但这个命令也不是总能成功--或许仍然需要先手工杀死子进程，然后再杀死父进程。

**6）****kill -HUP**
有时候只想简单的停止和重启进程。
例如：\# kill -HUP PID
该命令让Linux和缓的执行进程关闭，然后立即重启。在配置应用程序的时候，这个命令很方便，在对配置文件修改后需要重启进程时就可以执行此命令。

附录：各种信号及其用途
|Signal|Description|Signal number on Linux x86|
|SIGABRT|Process aborted|6|
|SIGALRM|Signal raised by|alarm|14|
|SIGBUS|Bus error: "access to undefined portion of memory object"|7|
|SIGCHLD|Child process terminated, stopped (or continued*)|17|
|SIGCONT|Continue if stopped|18|
|SIGFPE|Floating point exception: "erroneous arithmetic operation"|8|
|SIGHUP|Hangup|1|
|SIGILL|Illegal instruction|4|
|SIGINT|Interrupt|2|
|SIGKILL|Kill (terminate immediately)|9|
|SIGPIPE|Write to pipe with no one reading|13|
|SIGQUIT|Quit and dump core|3|
|SIGSEGV|Segmentation violation|11|
|SIGSTOP|Stop executing temporarily|19|
|SIGTERM|Termination (request to terminate)|15|
|SIGTSTP|Terminal stop signal|20|
|SIGTTIN|Background process attempting to read from tty ("in")|21|
|SIGTTOU|Background process attempting to write to tty ("out")|22|
|SIGUSR1|User-defined 1|10|
|SIGUSR2|User-defined 2|12|
|SIGPOLL|Pollable event|29|
|SIGPROF|Profiling timer expired|27|
|SIGSYS|Bad|syscall|31|
|SIGTRAP|Trace/breakpoint|trap|5|
|SIGURG|Urgent data available on socket|23|
|SIGVTALRM|Signal raised by timer counting virtual time: "virtual timer expired"|26|
|SIGXCPU|CPU time limit exceeded|24|
|SIGXFSZ|File size limit exceeded|25|



grep GlobalHandlerExceptionResolver . -nr --exclude-dir=".svn" --binary-files=without-match

**参考推荐：**
[linux下杀死进程（kill）的N种方法](http://blog.csdn.net/andy572633/article/details/7211546)
[Linux 杀死进程方法大全(kill,killall)](http://tieba.baidu.com/p/347592186)
[linux杀掉进程,fuser、kill、pkill命令的运用](http://adophper.com/webserver-961.html)



