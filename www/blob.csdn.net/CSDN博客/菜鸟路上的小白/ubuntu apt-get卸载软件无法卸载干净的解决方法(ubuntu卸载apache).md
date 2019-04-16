# ubuntu apt-get卸载软件无法卸载干净的解决方法(ubuntu卸载apache) - 菜鸟路上的小白 - CSDN博客





置顶2016年09月05日 16:50:09[somnus_小凯](https://me.csdn.net/u012486840)阅读数：5837
个人分类：[linux](https://blog.csdn.net/u012486840/article/category/6382672)









安装过sudo apt-get install xxxx 安装软件后，总是无法卸载干净，这里提供一个方法可以解决这个问题，大家参考使用吧








过sudo apt-get install xxxx 安装软件后，总是无法卸载干净，这里以Apache 为例，提供方法：

首先



sudo apt-get remove apache2



再






sudo apt-get autoremove




最近在Ubuntu下写python，需要看下python的文档，我找了很多关于ubuntu 下查看chm的资料，最后确定安装kchmviewer－nokde， 在安装kchmviewer－nokde时候冲突了，然后想删除某些软件 找了点资料，所以在此记录一下。

我安装kchmviewer－nokde的时候是用的新立得（名字起的很有新意啊，“新的立即就得到了”）来装的。

网上有些关于安装软件的讨论，我在此记录一下，最简单的就是用新立得了。我自己知道的命令行的就apt-get了,其实有个更好的就是aptitude，使用方式和apt-get类似的。

安装的时候：





sudo aptitude install kchmviewer-nokde



然后如果想删除：









sudo aptitude remove kchmviewer-nokde



就可以了。


和apt-get相比的优点：可以自动删除依赖的包，而apt-get实际也有删除的功能，但是删除不干净，而且删除起来也没这么方便。一个命令带来的便利就这里体现了。

和apt-get相比的缺点：用sudo aptitude remove删除的软件必须是用sudo aptitude instal来安装的，就是说他们是成对的

补充一点使用方法：一是从命令行启动 kchmviewer 二是让chm文件自动关联，这个在chm文件右键里选择一下就可以了。选择的名字是 kchmviewer。其实这个命令在

／usr/bin 下可以知道，你还可以看到很多类似的命令。用ls可能太多不方便看，可以用重定向 ls > ~/log.txt 当然你可以用其他shell 比如tail 或more




