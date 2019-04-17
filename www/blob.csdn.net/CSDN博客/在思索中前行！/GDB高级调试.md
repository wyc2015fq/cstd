# GDB高级调试 - 在思索中前行！ - CSDN博客





2014年08月06日 19:53:53[_Tham](https://me.csdn.net/txl16211)阅读数：627标签：[gdb																[调试](https://so.csdn.net/so/search/s.do?q=调试&t=blog)](https://so.csdn.net/so/search/s.do?q=gdb&t=blog)
个人分类：[C/C++](https://blog.csdn.net/txl16211/article/category/2284665)








#### 一、多线程调试


多线程调试可能是问得最多的。其实，重要就是下面几个命令：
- info thread 查看当前进程的线程。
- thread <ID> 切换调试的线程为指定ID的线程。
- break file.c:100 thread all  在file.c文件第100行处为所有经过这里的线程设置断点。
- set scheduler-locking off|on|step，这个是问得最多的。在使用step或者continue命令调试当前被调试线程的时候，其他线程也是同时执行的，怎么只让被调试程序执行呢？通过这个命令就可以实现这个需求。
- off 不锁定任何线程，也就是所有线程都执行，这是默认值。
- on 只有当前被调试程序会执行。
- step 在单步的时候，除了next过一个函数的情况(熟悉情况的人可能知道，这其实是一个设置断点然后continue的行为)以外，只有当前线程会执行。


#### 二、调试宏


这个问题超多。在GDB下，我们无法print宏定义，因为宏是预编译的。但是我们还是有办法来调试宏，这个需要GCC的配合。


在GCC编译程序的时候，加上-ggdb3参数，这样，你就可以调试宏了。


另外，你可以使用下述的GDB的宏调试命令 来查看相关的宏。
- info macro – 你可以查看这个宏在哪些文件里被引用了，以及宏定义是什么样的。
- macro – 你可以查看宏展开的样子。



#### 三、源文件


这个问题问的也是很多的，太多的朋友都说找不到源文件。在这里我想提醒大家做下面的检查：
- 编译程序员是否加上了-g参数以包含debug信息。
- 路径是否设置正确了。使用GDB的directory命令来设置源文件的目录。


下面给一个调试/bin/ls的示例（ubuntu下）



```
```bash
$
 apt-get
```

```bash
source
```

```bash
coreutils
```



```bash
$
```

```bash
sudo
```

```bash
apt-get
```

```bash
install
```

```bash
coreutils-dbgsym
```



```bash
$
 gdb
```

```bash
/bin/ls
```



```bash
GNU
 gdb (GDB) 7.1-ubuntu
```



```bash
(gdb)
 list main
```



```bash
1192
```

```bash
ls
```

```bash
.c:
 No such
```

```bash
file
```

```bash
or
 directory.
```



```bash
in
```

```bash
ls
```

```bash
.c
```



```bash
(gdb)
 directory ~
```

```bash
/src/coreutils-7
```

```bash
.4
```

```bash
/src/
```



```bash
Source
 directories searched:
```

```bash
/home/hchen/src/coreutils-7
```

```bash
.4:$cdir:$cwd
```



```bash
(gdb)
 list main
```



```bash
1192       
 }
```



```bash
1193   
 }
```



```bash
1194
```



```bash
1195   
 int
```



```bash
1196   
 main (int argc, char **argv)
```



```bash
1197   
 {
```



```bash
1198     
 int i;
```



```bash
1199     
 struct pending *thispend;
```



```bash
1200     
 int n_files;
```



```bash
1201
```
```



#### 四、条件断点


条件断点是语法是：break  [where] if [condition]，这种断点真是非常管用。尤其是在一个循环或递归中，或是要监视某个变量。注意，这个设置是在GDB中的，只不过每经过那个断点时GDB会帮你检查一下条件是否满足。

#### 五、命令行参数


有时候，我们需要调试的程序需要有命令行参数，很多朋友都不知道怎么设置调试的程序的命令行参数。其实，有两种方法：
- gdb命令行的 –args 参数
- gdb环境中 set args命令。

#### 六、gdb的变量


有时候，在调试程序时，我们不单单只是查看运行时的变量，我们还可以直接设置程序中的变量，以模拟一些很难在测试中出现的情况，比较一些出错，或是switch的分支语句。使用set命令可以修改程序中的变量。


另外，你知道gdb中也可以有变量吗？就像shell一样，gdb中的变量以$开头，比如你想打印一个数组中的个个元素，你可以这样：



```
```bash
(gdb)
```

```bash
set
```

```bash
$i
 = 0
```





```bash
(gdb)
 p a[$i++]
```





```bash
...
```

```bash
#然后就一路回车下去了
```
```




当然，这里只是给一个示例，表示程序的变量和gdb的变量是可以交互的。

#### 七、x命令


也许，你很喜欢用p命令。所以，当你不知道变量名的时候，你可能会手足无措，因为p命令总是需要一个变量名的。x命令是用来查看内存的，在gdb中 “help x” 你可以查看其帮助。
- x/x 以十六进制输出
- x/d 以十进制输出
- x/c 以单字符输出
- x/i  反汇编 – 通常，我们会使用 `x/10i $ip-20 来查看当前的汇编（$ip是指令寄存器）`
- x/s 以字符串输出

#### 八、command命令


有一些朋友问我如何自动化调试。这里向大家介绍command命令，简单的理解一下，其就是把一组gdb的命令打包，有点像字处理软件的“宏”。下面是一个示例：



```
```bash
(gdb)
```

```bash
break
```

```bash
func
```



```bash
Breakpoint
 1 at 0x3475678:
```

```bash
file
```

```bash
test
```

```bash
.c,
 line 12.
```



```bash
(gdb)
```

```bash
command
```

```bash
1
```



```bash
Type
 commands
```

```bash
for
```

```bash
when
 breakpoint 1 is hit, one per line.
```



```bash
End
 with a line saying just
```

```bash
"end"
```

```bash
.
```



```bash
>print
 arg1
```



```bash
>print
 arg2
```



```bash
>print
 arg3
```



```bash
>end
```



```bash
(gdb)
```
```




当我们的断点到达时，自动执行command中的三个命令，把func的三个参数值打出来。




