# Linux 如何查看一个进程的堆栈 - DoubleLi - 博客园






### 有两种方法：第一种：pstack 进程ID

### 第二种，使用gdb 然后attach 进程ID，然后再使用命令 thread apply all bt

### 第三种：strace -f -p pid  该方法和pstack类似


第四中：gcore pid ，输出core文件，gdb cmd corefile

### 两种方法都可以列出进程所有的线程的当前的调用栈。不过，使用gdb的方法，还可以查看某些信息，例如局部变量，指针等。不过，如果只看调用栈的话，pstack还是很方便的。


转自：http://blog.csdn.net/jimmyleeee/article/details/4651158

pstack不是一个链接程序，所以不需要其他库的支持，这个是centos5.8 的pstack：![](http://blog.chinaunix.net/image/default/fj.png)pstack.rar
copy过去就可以直接用了。









