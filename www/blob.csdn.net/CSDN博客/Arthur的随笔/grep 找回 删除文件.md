# grep 找回 删除文件 - Arthur的随笔 - CSDN博客
2010年09月03日 10:19:00[largetalk](https://me.csdn.net/largetalk)阅读数：774
转自http://coolshell.cn/articles/2822.html
这里给大家介绍一个小技巧用来恢复一些被rm了的文件中的数据。我们知道，rm命令其实并不是真正的从物理上删除文件内容，只过不把文件的
inode回收了，其实文件内容还在硬盘上。所以，如果你不小删除了什么比较重要的程序配置文件的时候，我们完全可以用grep命令在恢复，下面是一个恢
复示例：
[查看源代码](http://coolshell.cn/articles/2822.html#viewSource)
[打印](http://coolshell.cn/articles/2822.html#printSource)[帮助](http://coolshell.cn/articles/2822.html#about)
|`1`|```bashgrep``````bash-a -B 50 -A 60``````bash'some string in the file'``````bash/dev/sda1 > results.txt```|
说明：
- 关于grep的-a意为–binary-files=text，也就是把二进制文件当作文本文件。
- -B和-A的选项就是这段字符串之前几行和之后几行。
- /dev/sda1，就是硬盘设备，
- > results.txt，就是把结果重定向到results.txt文件中。
如果你幸运的话，你就可以看到被恢复的内容了。这正是Unix的简单哲学（详见《[Unix传奇下篇](http://coolshell.cn/articles/2324.html)
》）——** 所有的设备都是文件**
。
