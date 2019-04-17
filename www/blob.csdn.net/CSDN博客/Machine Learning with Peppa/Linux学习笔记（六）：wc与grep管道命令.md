# Linux学习笔记（六）：wc与grep管道命令 - Machine Learning with Peppa - CSDN博客





2018年03月14日 22:09:12[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：143
所属专栏：[Linux私房菜](https://blog.csdn.net/column/details/20153.html)









Linux系统中grep命令是一种强大的文本搜索工具，它能使用正则表达式搜索文本，并把匹 配的行打印出来。grep全称是Global Regular Expression Print，表示全局正则表达式版本，它的使用权限是所有用户。



### wc
1.命令格式：

wc [选项]文件...
2.命令功能：

统计指定文件中的字节数、字数、行数，并将统计结果显示输出。该命令统计指定文件中的字节数、字数、行数。如果没有给出文件名，则从标准输入读取。wc同时也给出所指定文件的总统计数

3.命令参数：
-c 统计字节数。
-l 统计行数。
-m 统计字符数。这个标志不能与 -c 标志一起使用。
-w 统计字数。一个字被定义为由空白、跳格或换行字符分隔的字符串。
-L 打印最长行的长度。
--help 显示帮助信息

--version 显示版本信息





### grep  Global search REgular expression and Print out the line
作用：文本搜索工具，根据用户指定的‘模式（过滤条件）’对目标文本逐行进行匹配检查；打印匹配到的行；‘模式’：由正则表达式的元字符及文本字符所编写出的过滤条件。

grep [OPTIONS] PATTERN [FILE...]
grep [OPTIONS] [-e PATTERN | -f FILE] [FILE...]


### 常用选项：
        -i:忽略字符大小写
        -o:仅显示匹配到的字符串本身
        -v:显示不能被模式匹配到的行
        -E:支持扩展正则表达式元字符
        -q:静默模式，匹配不显示
        -A#:after,显示匹配条件所在行的后#行
        -B#:before,显示匹配条件所在行的前#行
        -C#:context,显示匹配条件所在行的前后#行
        -n：显示匹配的行号（用的较少）
        -c: 统计匹配的行数（用的较少）



例子: 在文件a中统计 hello 出现的行数:
`grep hello a | wc -l`
在文件a中统计hello出现的次数:
`grep -o hello a | wc -l`
grep命令和wc命令 这种可以在命令行中通过 | 互相链接 ,作为输入的命令原来有同一的名字 叫做管道命令



![image](https://images.cnblogs.com/cnblogs_com/chengmo/WindowsLiveWriter/linuxshellpipe_ACF7/image_thumb.png)



　　1、管道命令只处理前一个命令正确输出，不处理错误输出

　　2、管道命令右边命令，必须能够接收标准输入流命令才行。




