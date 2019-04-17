# GDB（十）－－调试正在运行的进程 - DoubleLi - 博客园






我编写了一个循环：

    long i;
    for (i = 0; i < 999999; i++) {
        mt.a += 1;
        sleep(1);
    }
把它编译成a.out，并在后台执行它：./a.out &

[1] 2570

然后用命令gdb ./a.out 2570可以附加到这个进程上。被时进程会中断。

或者在GDB里输入attach 2570同样可以附加到进程。

输入bt来查看栈

(gdb) bt
#0  0x008e9416 in __kernel_vsyscall ()
#1  0x003bb900 in nanosleep () from /lib/i386-[linux](http://biancheng.dnbcw.info/linux/)-gnu/libc.so.6
#2  0x003bb71f in sleep () from /lib/i386-[linux](http://biancheng.dnbcw.info/linux/)-gnu/libc.so.6
#3  0x0804845d in main () at ptype_struct.c:22
可以看到程序正中断在系统调用vsyscall上。用frame 3进入main函数的栈框架并打印i的值，

(gdb) frame 3
#3  0x0804845d in main () at ptype_struct.c:22
22            sleep(1);
(gdb) p i
$1 = 153
next可以执行下一行。

(gdb) next
Single stepping until exit from function __kernel_vsyscall,
which has no line number information.
0x003bb900 in nanosleep () from /lib/i386-linux-gnu/libc.so.6

设置i的值。

(gdb) p i = 999999
$3 = 999999

detach可以分离进程。
(gdb) detach
Detaching from program: /home/tommy/tmp/a.out, process 2570
(gdb) q
[1]+  完成                  ./a.out

如果被调试的进程没有调试信息：

$ strip ./a.out 
$ ./a.out &
[1] 2603
tommy:~/tmp$ gdb
gdb> attach 2603

(gdb) bt
#0  0x00b0f416 in __kernel_vsyscall ()
#1  0x00721900 in nanosleep () from /lib/i386-linux-gnu/libc.so.6
#2  0x0072171f in sleep () from /lib/i386-linux-gnu/libc.so.6
#3  0x0804845d in ?? ()
#4  0x0069e113 in __libc_start_main () from /lib/i386-linux-gnu/libc.so.6
#5  0x08048351 in ?? ()
函数名没有被打印。



注意GDB其实可以看作是ptrace系统调用的前端。ptrace专门用来观察和控制另一个进程的执行。观察别的进程可能需要恰当的权限，比如超级用户。不要去调试init进程，不然可能会很伤。我不小心把它杀掉，电脑直接黑屏。

### 相关资料：
- [linux](http://biancheng.dnbcw.info/linux/)｜[GDB（九）－－观察和改变变量](http://biancheng.dnbcw.info/linux/391847.html)
- [linux](http://biancheng.dnbcw.info/linux/)｜[GDB（八）－－启动被调试程序](http://biancheng.dnbcw.info/linux/391848.html)
- [linux](http://biancheng.dnbcw.info/linux/)｜[GDB（六）－－断点](http://biancheng.dnbcw.info/linux/391849.html)
- [linux](http://biancheng.dnbcw.info/linux/)｜[GDB（十）－－调试正在运行的进程](http://biancheng.dnbcw.info/linux/391846.html)
- [linux](http://biancheng.dnbcw.info/linux/)｜[Ubuntu下配置tftp服务和NFS服务](http://biancheng.dnbcw.info/linux/391833.html)
- [linux](http://biancheng.dnbcw.info/linux/)｜[squid日志TCP命中率字段详解](http://biancheng.dnbcw.info/linux/391828.html)
- [linux](http://biancheng.dnbcw.info/linux/)｜[TCP三次握手过程基础知识](http://biancheng.dnbcw.info/linux/391826.html)









