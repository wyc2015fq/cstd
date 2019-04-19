# floating point exception || LD_LIBRARY_PATH - 三少GG - CSDN博客
2013年11月10日 16:41:28[三少GG](https://me.csdn.net/scut1135)阅读数：1749

## Floating point exception 问题解决 GCC版本问题引发的floating point exception
现象：
同一个程序在一台高版本Linux上运行时没有问题，而在另一台低版本机器上运行报Floating Point Exception时，那**么这极有可能是由高版本gcc链接造成的**。
原因：
高版本的gcc在链接时采用了新的哈希技术来提高动态链接的速度，这在低版本中是不支持的。因此会发生这个错误。
解决方案：
在链接的时候添加选项-Wl,--hash-style=sysv
例如 gcc -Wl,--hash-type=sysv -o test test.c
-----------------------------------------------------------
[问题] RedHat AS5 编译的可执行程序拷贝到 RedHat AS4 上运行, 直接报Floating point exception.
[原因] glibc 不兼容导致. 详细原因如下引用.
[引用 来自 http://docs.fedoraproject.org/ ]
Fedora 开发者对 ELF 格式中，提供动态链接的符号的 .hash 段做了变动。新的 .gnu.hash 段，使用 gcc 的 --hash-style=gnu 参数来生成，提供与过去的 hash 段相同的功能，但是带来了 50% 的动态链接性能提升。使用新的 hash 函数生成的可执行程序和库与旧的 glibc 以及动态链接器不兼容。要使用过去的 hash 规则，与过去基于 glibc 的版本兼容，可以传递 --hash-style=sysv 选项。
*****************************************************************************************************************************************************
LD_LIBRARY_PATH
在 Linux 下，如果你写好了自己的动态链接库，需要在其它程序里调用，则需要让这些程序能找到这些动态链接库。如果设置不对，会出现类似如下的错误：
test: error while loading shared libraries: libexampleso.so.0: cannot open shared object file: No such file or directory
这是因为**没有把动态链接库的安装路径（例如说是 /usr/local/lib ）放到变量[LD_LIBRARY_PATH](http://baike.baidu.com/view/1270749.htm) 里。**
这时，可以用命令 **export** 来临时测试确认是不是这个问题：
exportLD_LIBRARY_PATH=/usr/local/lib
在终端里运行上面这行命令，再运行这个可执行文件，如果运行正常就说明是这个问题。
接下来的问题是：以上做法，只是临时设置变量 **LD_LIBRARY_PATH** ，下次开机，一切设置将不复存在；如何把这个值持续写到 LD_LIBRARY_PATH 里呢？
我们可以在 **~/.bashrc** 或者 **~/.bash_profile** 中加入 export 语句，前者在每次登陆和每次打开 shell 都读取一次，后者只在登陆时读取一次。我的习惯是加到
**~/.bashrc** 中，在该文件的未尾，可采用如下语句来使设置生效：
exportLD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib
**修改完后，记得关掉当前终端并重新打开一个新的终端，从而使上面的配置生效。**
