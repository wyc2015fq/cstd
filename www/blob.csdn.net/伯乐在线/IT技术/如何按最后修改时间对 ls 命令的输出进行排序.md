# 如何按最后修改时间对 ls 命令的输出进行排序 - 文章 - 伯乐在线
原文出处： [Aaron Kili](http://www.tecmint.com/sort-ls-output-by-last-modified-date-and-time/)   译文出处：[zky001](https://linux.cn/article-7984-1.html)
Linux 用户常常做的一个事情是，是在命令行[列出目录内容](http://www.tecmint.com/file-and-directory-management-in-linux/)。
我们已经知道，[ls](http://www.tecmint.com/15-basic-ls-command-examples-in-linux/) 和 [dir](http://www.tecmint.com/linux-dir-command-usage-with-examples/) 是两个可用在列出目录内容的 Linux 命令，前者是更受欢迎的，在大多数情况下，是用户的首选。
我们列出目录内容时，可以按照不同的标准进行排序，例如文件名、修改时间、添加时间、版本或者文件大小。可以通过指定一个特别的参数来使用这些文件的属性进行排序。
在这个简洁的 [ls 命令指导](http://www.tecmint.com/tag/linux-ls-command/)中，我们将看看如何通过上次修改时间（日期和时分秒）[排序 ls 命令的输出结果](http://www.tecmint.com/sort-command-linux/) 。
让我们由执行一些基本的 [ls 命令](http://www.tecmint.com/15-basic-ls-command-examples-in-linux/)开始。
### Linux 基本 ls 命令
1、 不带任何参数运行 ls 命令将列出当前工作目录的内容。


```
$ ls
```
![List Content of Working Directory](http://jbcdn2.b0.upaiyun.com/2017/01/cb2cba24c6b4c925961d1c9206ddd804.png)
*列出工作目录的内容*
2、要列出任何目录的内容，例如 /etc 目录使用如下命令：


```
$ ls /etc
```
![List Contents of Directory](http://jbcdn2.b0.upaiyun.com/2017/01/a7ea651606b12a334e656c3fce120d45.png)
*列出工作目录 /etc 的内容*
3、一个目录总是包含一些隐藏的文件（至少有两个），因此，要展示目录中的所有文件，使用`-a`或`-all`标志：


```
$ ls  -a
```
![List Hidden Files in Directory](http://jbcdn2.b0.upaiyun.com/2017/01/987432450bb7f6519f2f26a6a34c36bd.png)
*列出工作目录的隐藏文件*
4、你还可以打印输出的每一个文件的详细信息，例如文件权限、链接数、所有者名称和组所有者、文件大小、最后修改的时间和文件/目录名称。
这是由`-l`选项来设置，这意味着一个如下面的屏幕截图般的长长的列表格式。


```
$ ls -l
```
![Long List Directory Contents](http://jbcdn2.b0.upaiyun.com/2017/01/76d3ae777d1644d97ae8c506615024ab.png)
*长列表目录内容*
### 基于日期和基于时刻排序文件
5、要在目录中列出文件并[对最后修改日期和时间进行排序](http://www.tecmint.com/find-and-sort-files-modification-date-and-time-in-linux/)，在下面的命令中使用`-t`选项：


```
$ ls -lt
```
![Sort ls Output by Date and Time](http://jbcdn2.b0.upaiyun.com/2017/01/afe4ed84c3ee1ed6dcd7ab590bf3132b.png)
*按日期和时间排序ls输出内容*
6、如果你想要一个基于日期和时间的逆向排序文件，你可以使用`-r`选项来工作，像这样：


```
$ ls -ltr
```
![Sort ls Output Reverse by Date and Time](http://jbcdn2.b0.upaiyun.com/2017/01/5ed196af4a21aea6ddf4a8db867572b1.png)
*按日期和时间排序的逆向输出*
我们将在这里结束，但是，[ls 命令](http://www.tecmint.com/tag/linux-ls-command/)还有更多的使用信息和选项，因此，应该特别注意它或看看其它指南，比如《[每一个用户应该知道 ls 的命令技巧](http://www.tecmint.com/linux-ls-command-tricks/)》或《[使用排序命令](http://www.tecmint.com/linux-sort-command-examples/)》。
