# 为初学者准备的 Linux rmdir 命令案例教程 - 文章 - 伯乐在线
原文出处： [HowtoForge](https://www.howtoforge.com/linux-rmdir-command/)   译文出处：[Linux中国/ChenYi](https://linux.cn/article-9357-1.html)
在 Linux 命令行上删除文件和目录，我们已经讨论过 [rm 命令](https://linux.cn/article-9321-1.html) 的使用。然而，这里有另一个相关的命令行工具，专门针对目录的删除。这是所指的工具就是 `rmdir`，本教程中，我们将根据一些简单的实例来讨论它。
### Linux rmdir 命令
见名知意，`rmdir`（remove directory） 命令专门针对删除目录，不过通常目录内空空如也。如下是它的语法：


```
rmdir [OPTION]... DIRECTORY...
```
下面是 man 文档的说明：
> 
如目录为空，则删除之。
在理解此程序如何工作，下面的 Q&A 风格的案例可能会给您带来更好的灵感。
### Q1. rmdir 命令如何工作？
这非常直接，只需将目录名作为输入参数传递给它。
比如：


```
rmdir test-dir
```
![rmdir 命令如何工作](http://jbcdn2.b0.upaiyun.com/2018/02/414dcdcd6a265053ade40d59ab2c27c6.png)
### Q2. 如何让 rmdir 忽略非空目录？
通常情况下，`rmdir` 命令会在您删除非空目录时抛出一个错误。然而，如果需要的话，您可以使用 `--ignore-fail-on-non-empty` 选项，忽略它。
比如说：
![如何让 rmdir 忽视非空目录](http://jbcdn2.b0.upaiyun.com/2018/02/5c33991a1bf3c2babc9ceea7c1d92db1.png)
### Q3. 如何让 rmdir 命令移除父目录？
和 [mkdir](https://linux.cn/article-9356-1.html) 一样，您可以通过要求 `rmdir` 命令在其父目录执行操作。这意味着，您可以一次性删除目录和目录的上级目录。这个功能可以通过 `-p` 命令选项实现。
比如，下面的命令将会删除 “test” 和 “test-dir” 目录。


```
rmdir -p test/test-dir/
```
注意： 欲此操作生效，所有被删除的父目录都不应该包含要被删除的空目录之外的任何目录。
### Q4. rmdir 和 rm -r 有何区别？
如果还有印象，您还可以通过提供 `-r` 选项的 `rm` 命令删除目录。那 `rmdir` 和 `rm -r` 区别何在？`rmdir` 仅对空目录生效 —— 您没有办法使用 rmdir 删除非空目录。
因此，`rmdir` 在删除需要检测目录是否为空的情况下，是一个有效的工具。
### 总结
相信您会认同，`rmdir` 并不是一个难以理解和使用的复杂命令。除此之外，它仅提供少量命令行选项。在此，我们几乎讨论了所有的选项，如果您想掌握的更好，练习在本教程提到的案例即可。关于 `rmdir`，[这里另有 man 文档](https://linux.die.net/man/1/rmdir)，备您不时之需。
