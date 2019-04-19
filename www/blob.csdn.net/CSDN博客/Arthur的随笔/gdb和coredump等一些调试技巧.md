# gdb和coredump等一些调试技巧 - Arthur的随笔 - CSDN博客
2012年09月03日 14:53:00[largetalk](https://me.csdn.net/largetalk)阅读数：1478标签：[file																[kill																[bt																[gcc																[list																[终端](https://so.csdn.net/so/search/s.do?q=终端&t=blog)](https://so.csdn.net/so/search/s.do?q=list&t=blog)](https://so.csdn.net/so/search/s.do?q=gcc&t=blog)](https://so.csdn.net/so/search/s.do?q=bt&t=blog)](https://so.csdn.net/so/search/s.do?q=kill&t=blog)](https://so.csdn.net/so/search/s.do?q=file&t=blog)
个人分类：[linux](https://blog.csdn.net/largetalk/article/category/727912)
查看一个可执行文件需要加载的库， ldd filename, 如：
```
$ ldd str_hash
	linux-gate.so.1 =>  (0x00de1000)
	libc.so.6 => /lib/i386-linux-gnu/libc.so.6 (0x008ed000)
	/lib/ld-linux.so.2 (0x003f0000)
```
如果想看一个运行中程序的动态加载库，可以先ps找到程序的pid，然后 cat /proc/$pid$/maps
strace: 监听程序的系统调用和信号， 简单用法： 先ps找出程序的pid， 然后 sudo strace -p $pid$
strip: 去掉可执行文件中的符号信息， strip filename
使用gdb和cordump来调试程序：
  1. ulimit -c 查看设置的coredump size大小， 默认为0即不生成corm dump， 使用ulimit -c 4096 或 ulimit -c unlimited 打开coredump， 注意，一但设定一个size之后，再次用ulimit -c变更size时，只能变小，不能变大。否则只有重新开一个终端才能重新设置较大的size
  2. 生成带debug符号信息的可执行文件， 如：gcc str_hash.c -g -o str_hash
  3. gdb ./str_hash core
gdb常用命令：
命令                           解释
break NUM              在指定的行上设置断点。
bt                               显示所有的调用栈帧。该命令可用来显示函数的调用顺序。
clear                          删除设置在特定源文件、特定行上的断点。其用法为：clear FILENAME:NUM。
continue                   继续执行正在调试的程序。该命令用在程序由于处理信号或断点而导致停止运行时。
display EXPR         每次程序停止后显示表达式的值。表达式由程序定义的变量组成。
file FILE                   装载指定的可执行文件进行调试。
help NAME             显示指定命令的帮助信息。
info break                显示当前断点清单，包括到达断点处的次数等。
info files                   显示被调试文件的详细信息。
info func                   显示所有的函数名称。
info local                  显示当函数中的局部变量信息。
info prog                  显示被调试程序的执行状态。
info var                    显示所有的全局和静态变量名称。
kill                            终止正被调试的程序。
list                           显示源代码段。
make                      在不退出 gdb 的情况下运行 make 工具。
next                        在不单步执行进入其他函数的情况下，向前执行一行源代码。
print EXPR            显示表达式 EXPR 的值。

