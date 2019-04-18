# command>/dev/null 2>&1 - Orisun - 博客园







# [command>/dev/null 2>&1](https://www.cnblogs.com/zhangchaoyang/articles/2005865.html)





/dev/null相当于一个只写文件,

所有写入它的内容都会永远丢失,而尝试从它那儿读取内容则什么也读不到.


/dev/null对于写脚本非常有用:

1.禁止标准输出

cat $filename >/dev/null

#文件内容丢失,而不会输出到标准输出中

2.禁止标准错误

rm $basename 2>/dev/null

#标准错误信息去入"黑洞"

一般的command把内容输出到标准输出(stdout)中,现在使用command>/dev/null则把输出重定向到
/dev/null中,即把输出丢进了"黑洞".

2>&1:其中2是标准错误流(stderr)的句柄,&1表示前一次的文件句柄即

/dev/null,那么

2>&1的意思就是把标准错误流也丢进了"黑洞".

但是注意这里

2>&1并不完全等价于2>

/dev/null.原因如下:


　　首先,**command > file 2>file** 的意思是将命令所产生的标准输出信息,和错误的输出信息送到file 中.**command  > file 2>file** 这样的写法,stdout和stderr都直接送到file中, file会被打开两次,这样stdout和stderr会互相覆盖,这样写相当使用了FD1和FD2两个同时去抢占file 的管道.

      而**command >file 2>&1** 这条命令就将stdout直接送向file, stderr 继承了FD1管道后,再被送往file,此时,file 只被打开了一次,也只使用了一个管道FD1,它包括了stdout和stderr的内容.

      从IO效率上,前一条命令的效率要比后面一条的命令效率要低,所以在编写shell脚本的时候,较多的时候我们会用**command > file 2>&1** 这样的写法.













