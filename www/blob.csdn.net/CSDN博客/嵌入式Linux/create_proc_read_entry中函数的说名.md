
# create_proc_read_entry中函数的说名 - 嵌入式Linux - CSDN博客

2015年11月16日 17:22:32[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：378个人分类：[Linux																](https://blog.csdn.net/weiqifa0/article/category/1388863)


原型：
> struct
> proc_dir_entry
> *
> create_proc_read_entry
> (
> const
> char
> *
> name
> ,
> mode_t
> mode
> ,
> struct
> proc_dir_entry
> *
> base
> ,
> read_proc_t
> *
> read_proc
> ,
> void
> *
> data
> );

说明：
name : 要创建的文件名；
mode : 文件掩码，为 0 则按照系统默认的掩码创建文件。
base : 指定该文件所在的目录，如果为 NULL，则文件被创建在 /proc 根目录下。
read_proc : 实现该文件的 read_proc 函数。也就是说，当我们读取 "name" 这个文件时(如 cat /proc/myproc_name) ，读取请求会通过这个函数发送到驱动模块，然后在函数里处理的数据会写到 myproc_name 文件中。
data : 内核忽略此参数，但会把它当作参数传递给 read_proc 这个自定义函数。
用法：
> struct
> proc_dir_entry
> *
> parent
> ;

> parent
> =
> proc_mkdir
> (
> "myproc"
> ,
> NULL
> );

> create_proc_read_entry
> (
> "scullmem"
> ,
> 0744
> ,
> parent
> ,
> scull_read_procmem
> ,
> NULL
> );

这样，就在 /proc 下创建了 myproc 目录，并在 myproc 目录下创建了一个名为 scullmem 的文件，且这个文件的权限为 0744 :
> \# ll /proc/myproc/scullmem

> -rwxr--r-- 1 root root 0 2010-09-27 20:48 /proc/myproc/scullmem*

上面的 scullmem 后有 1 星号表示此文件可执行，实际上 /proc 下的文件一般都是只读的，这里只是演示权限位。
另外，create_proc_entry() 函数同样用来建立 /proc 文件，但较 create_proc_read_entry() 更为底层一些，它的原型为：
> struct
> proc_dir_entry
> *
> create_proc_entry
> (
> const
> char
> *
> name
> ,
> mode_t
> mode
> ,
> struct
> proc_dir_entry
> *
> parent
> );

其中参数和 create_proc_read_entry() 的一样，分别是 /proc 文件名，掩码，父目录。

