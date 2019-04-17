# Linux 数据流重定向 - Alex_McAvoy的博客 - CSDN博客





2018年07月14日 20:29:29[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：72
个人分类：[Linux 学习笔记](https://blog.csdn.net/u011815404/article/category/7715914)









# 【概述】

1.重定向：将原本输出到标准输出的数据重定向到一个文件中。

2.用于终端显示输出的特殊设备：


|文件描述符|设备文件|说明|
|----|----|----|
|0|/dev/stdin|标准输入|
|1|/dev/stdout|标准输出|
|2|/dev/stderr|标准错误|

3.文件描述符：在形式上是一个非负整数，实际上，是一索引值，指向内核为每一个进程所维护的该进程打开文件的记录表。当程序打开一个现有文件或者创建一个新文件时，内核向进程返回一个文件描述符。

4.与管道区别：管道默认是连接前一个命令的输出到下一个命令的输入，而重定向通常是需要一个文件来建立两个命令的连接。

# 【基本重定向控制符】

1.标准输入的控制



1）作用：将命令重定向到文件中。

2）语法：**命令 < 文件**

3）实例：将文件 test 当做信件的内容，主题名称为 test，送给收信人

![](https://img-blog.csdn.net/20180714173421508?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

2.标准输出的控制



1）将命令的执行结果送至指定的文件中

①语法：**命令 > 文件**

②实例：将执行 ls -l 命令的结果写入文件 test 中

![](https://img-blog.csdn.net/20180714174921628?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



2）将命令执行的结果附加到指定的文件中

①语法：**命令 >> 文件**

②实例：

![](https://img-blog.csdn.net/20180714175508188?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



# 【标准错误重定向】

标准输出和标准错误都被指向伪终端的屏幕显示，所以我们常看到的一个命令的输出通常是同时包含了标准输出和标准错误的结果，例如：

![](https://img-blog.csdn.net/20180714175726341?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

如果我们需要隐藏某些错误或者警告，此时需要使用重定向符号“&”，其格式为：**2>&1**

![](https://img-blog.csdn.net/20180714194034682?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

重定向后，标准输出和标准错误虽然都指向终端屏幕，但实际它们并不一样，标准错误和标准输出实质上都同时重定向到文件中。

**注：关于 & 的使用**

**1）command > filename 2>&1 把标准输出和标准错误一起重定向到一个文件中**

**2）command >> filename 2>&1 把标准输出和标准错误以追加的形式一起重定向到一个文件中**

**3）command <&m 把文件描述符 m 作为标准输入**

**4）command >&m 把标准输出重定向到文件描述符 m 中**

**5）command <&- 关闭标准输入**

**6）**command >&- 关闭标准输出****

# 【tee 命令】

1.作用：用于将数据重定向到文件，另一方面还可以提供一份重定向数据的副本作为后续命令的 stdin，即把数据重定向到给定文件和屏幕上。

![](https://img-blog.csdn.net/20180714194841444?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

2.命令格式：**tee [选项] 文件名**

3.常用选项：


|-a|向文件中重定向时使用追加模式|
|----|----|
|-i|忽略中断（interrupt）信号|

4.实例：在终端打印 stdout 同时重定向到文件中

![](https://img-blog.csdn.net/2018071419521172?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

# 【exec 命令】

exec 命令使用指定的命令替换当前的 Shell，即使用一个进程替换当前进程，或者指定新的重定向。

## 1.永久重定向



1）重定向操作一般都是临时性的，即只对当前命令有效，如果想实现“永久”有效，可以使用 exec 命令来实现。

2）格式：**exec 文件描述符 > 文件名**

3）实例：在子壳 zsh 中使用 exec 替换当前进程的重定向，将标准输出重定向到一个文件，后面执行的命令的输出都将被重定向到文件中，直到你退出当前子 shell，或取消 exec 的重定向

![](https://img-blog.csdn.net/20180714200049831?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



## 2.创建输出文件描述符

1）在 Shell 中有9个文件描述符，Linux 中只默认提供的 0，1，2 号文件描述符，另外我们还可以使用 3-8 号的文件描述符，只是它们默认没有打开而已。 



2）查看当前 Shell 进程中打开的文件描述符
`cd /dev/fd/;ls -Al;cd -`
如下图，可以看到 0-2 号文件描述符默认打开

![](https://img-blog.csdn.net/20180714202509338?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

3）创建新的文件格式符：**exec > 要开启的文件描述符**

![](https://img-blog.csdn.net/20180714202052785?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

## 3.关闭文件描述符

1）格式：exec >&-

2）实例：

![](https://img-blog.csdn.net/20180714202724462?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

# 【完全屏蔽命令的输出】

在 Linux 系统中，/dev/null，或称空设备，是一个特殊的设备文件，它通常被用于丢弃不需要的输出流，或作为用于输入流的空文件，这些操作通常由重定向完成。

读取空设备会立即得到一个 EOF，我们可以利用这个空设备来屏蔽命令的输出。

![](https://img-blog.csdn.net/2018071420341415?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)




