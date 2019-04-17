# gdbserver移植使用 - V__KING__的专栏 - CSDN博客





2017年03月01日 17:47:27[v__king__](https://me.csdn.net/V__KING__)阅读数：936








# gdbserver移植使用

## gdbserver的使用

gdbserver用法其实和gdb是一样的

记住在Ubuntu12.04中默认没有arm-linux-gdb的，必须下载gdb.gz并编译安装。

还要记住一点Ubuntu12.04中默认的gdb和arm-linux-gdb是不一样的。
- linux板子： 
`gdbserver 192.168.1.108:2222 run.exe`
- Ubuntu12.04机子： 
`arm-linux-gdb run.exe`   //如果不加run.exe会没有symbol的 
`target remote 192.168.1.108:2222`
## why要用gdbserver
- 
首先明白gdb是用来干什么 

先试试gdb是用来干什么的，网上资料一大堆。参考：[http://blog.csdn.net/v__king__/article/details/58192272](http://blog.csdn.net/v__king__/article/details/58192272)

- 
gdbserver干什么的 

-某些时候由于模拟环境的限制，调试必须要在目标板上进行。由于嵌入式系统资源比较有限，一般不能在目标板上直接构建GDB的调试环境，这时我们通常采用gdb+gdbserver的远程调试方法：gdbserver在目标板中运行，而gdb则在主机上运行。 

-构建gdb+gdbserver调试环境的在于，要将gdb和gdbserver都编译成适用于目标板的版本。比如我们用x86的主机和ARM目标板，平时在主机上直接调试的时候都使用用于x86调试的gdb，但这个gdb不能用于远程调试中，需要针对ARM平台进行配置之后重新编译才行；而gdbserver要运行在目标板上，则需要用arm-linux-gcc编译才行。
## 如何安装gdbserver 到linux的板子上

参考：[http://www.cnblogs.com/huaping-audio/archive/2009/06/19/1506898.html](http://www.cnblogs.com/huaping-audio/archive/2009/06/19/1506898.html)

1. 保证arm-linux-gcc和gdbserver版本能兼容，能对得上号 

2. 下载gdb 

    下载地址：[http://ftp.gnu.org/gnu/gdb/](http://ftp.gnu.org/gnu/gdb/)

3. 编译gdb或者执行gdbserver 192.168.1.108:2222 run.exe 

    出现错误：`gdbserver libthread_db.so.1: cannot open shared object file: No such file or directory`

    原因: 在linux的板子上没有gdbserver用到的动态库libthread_db.so.1 。 

    解决：我们会想linux板子所用的动态库会在什么地方呢？在交叉编译工具的arm-linux中去找，注意动态库一般是链接ln。将该库cp到linux板子的/lib目录下`cp libthread_db-1.0.so /lib`，然后`ln -s libthread_db-1.0.so libthread_db.so.1`
```
问题：没有termcap的库
原因：后来的Ubuntu没有这个库了
解决：下载termcap.1_3_1.gz，编译安装: ./configue ; make ; sudo make install
```

4. 想办法将gdbserver复制到linux板子上，可以用scp，ftp，tftp等

## 运行调试
- linux板子： 
`gdbserver 192.168.1.108:2222 run.exe`
- Ubuntu12.04机子： 
`arm-linux-gdb run.exe`  //如果不加run.exe会没有symbol的 
`target remote 192.168.1.108:2222`

假如运行的是`arm-linux-gdb` 的话并不会出现错误，但是没有symbol的，也就不能设置断点
## 补充知识

./configure –host –target

build: 执行代码编译的主机，正常的话就是你的主机系统。这个参数一般由config.guess来猜就可以。当然自己指定也可以。 

host: 编译出来的二进制程序所执行的主机，因为绝大多数是如果本机编译，本机执行。所以这个值就等于build。只有交叉编译的时候（也就是本机编译，其他系统机器执行）才会build和host不同。用host指定运行主机。 

target: 这个选项只有在建立交叉编译环境的时候用到，正常编译和交叉编译都不会用到。他用build主机上的编译器，编译一个新的编译器（binutils, gcc,gdb等），这个新的编译器将来编译出来的其他程序将运行在target指定的系统上。








