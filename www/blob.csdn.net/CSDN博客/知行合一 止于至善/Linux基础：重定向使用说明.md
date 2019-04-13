
# Linux基础：重定向使用说明 - 知行合一 止于至善 - CSDN博客

2019年02月05日 22:07:30[liumiaocn](https://me.csdn.net/liumiaocn)阅读数：772


在Linux或者Unix中经常会使用> < >> << <<< 等多种重定向方式，结合标准错误和标准输出，还会看到2>/dev/null 以及2>&1等常见的方式，在这篇文章中将对常见的重定向使用进行整理和总结。
# 基础知识： 输入输出流
基本输入输出流信息如下所示：
|文件描述符|名称|说明|
|---|---|---|
|0|stdin|标准输入|
|1|stdout|标准输出|
|2|stderr|标准错误|
# 基础知识：常见重定向符号
|符号|说明|
|---|---|
|<|输入重定向|
|>|输出重定向|
|<<|Here Document方式多行标准输入|
|>>|追加方式输出重定向|
# 示例说明：输入重定向<的使用方式
这篇文章中将会以bash的格式对上述常见的重定向的使用方式进行示例说明，首先来看一下输入重定向<的使用方式。
`[root@platform ~]# cat test.txt
hello
world
[root@platform ~]# while read line 
> do
> echo "[info] : $line"
> done <test.txt
[info] : hello
[info] : world
[root@platform ~]#`这个示例说明了如何使用输入重定向从文件中读取输入信息，并予以显示。
# 示例说明：输出重定向
## 标准错误输入重定向说明方式
> 使用示例：命令 2>/dev/null

> 将标准错误重定向到/dev/null，也就是不显示错误信息

> 同样也可以将其连接相关的文件名称，则会创建新的错误输出信息

> 标准输出重定向说明方式

> 使用示例：命令 1>/dev/null 或者 命令 >/dev/null

> 将标准输出重定向到/dev/null，也就是不显示stdout信息

> 同样也可以将其连接相关的文件名称，则会创建新的标准输出信息

> 将信息输出到标准错误

> 使用示例：echo “some information” >&2

> 具体使用方式可以结合如下示例进行理解，这里不再赘述。

> [root@platform ~]\# echo "some information"  |grep some
some information
[root@platform ~]\# echo $?
0
[root@platform ~]\#
> 而一旦使用重定向，可以看到实际的区别

> [root@platform ~]\# echo "some information"  >&2 |grep some
some information
[root@platform ~]\# echo $?
1
[root@platform ~]\#
> 同样，将信息输出到标准输出则可以使用 >&1

> 将标准错误和标准输入一同输出到标准输出

> 先看一下如下示例：

> [root@platform ~]\# ls aaa bbb helloworld.c  2>&1
ls: cannot access aaa: No such file or directory
ls: cannot access bbb: No such file or directory
helloworld.c
[root@platform ~]\#
> 标准错误也输入到标准输出中，但是现在还看不出区别，在这个基础上稍作改动，则会变成比较常见的一个操作，标准输入和输出同时输出到某个文件或者不显示

> [root@platform ~]\# ls aaa bbb helloworld.c  >/dev/null 2>&1
[root@platform ~]\#
> 此处需要注意2点：

> >/dev/null 的位置会起到很重要的作用，需要注意到是在重定向之前还是之后

> bash和其他shell的写法有所不同

> 常见示例：<<的使用方式

> <<常见于here document的使用，用于给命令提供多行参数，比如：

> [root@platform ~]\# grep hello <<EOF
> hello
> world
> EOF
hello
[root@platform ~]\#
> 详见如下Here Document的说明，此处不再赘述。

> [https://blog.csdn.net/liumiaocn/article/details/86715953](https://blog.csdn.net/liumiaocn/article/details/86715953)

> 常见示例：>>的使用

> 追加的使用方式非常常见，比如如下所示

> [root@platform ~]\# echo "hello" >test.txt
[root@platform ~]\# echo "world" >test.txt
[root@platform ~]\# cat test.txt
world
[root@platform ~]\# 
[root@platform ~]\# echo "hello" >test.txt
[root@platform ~]\# echo "world" >>test.txt
[root@platform ~]\# cat test.txt
hello
world
[root@platform ~]\#
> 因为内容非常简单，此处不再说明。

> 常见示例：<<<的使用

> <<<是一个bash中常见的写法，具体使用方式可以根据如下示例进行理解：

> [root@platform ~]\# read var1 var2 <<< "hello world"
[root@platform ~]\# echo $var1
hello
[root@platform ~]\# echo $var2
world
[root@platform ~]\#
> 当然，不使用这种方式，也可以使用另外一种比较难看的方式来实现类似的功能：

> [root@platform ~]\# echo "hello world" | { read var1 var2; echo $var1; echo $var2; }
hello
world
[root@platform ~]\#

