# Emacs-059-Dired模式使用 - 小灰笔记 - CSDN博客





2018年01月16日 00:12:19[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：69
个人分类：[Emacs](https://blog.csdn.net/grey_csdn/article/category/7131378)









Emacs-059-Dired模式使用

    这个功能说起来不陌生，其实使用命令行模式的时候参数是一个目录的时候启动的就是一个相同或者是相似的功能。除此之外，VIM中也有一个与之显示的功能。

    启动方式C-x d，之后输入一个需要查看的目录。具体的效果如下：

![](https://img-blog.csdn.net/20180116001233976?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


    接下来，可以进行目录级别的操作。常用的操作如下：

1. 创建目录：+

2. 创建文件：C-x C-f，继而输入文件名称

3. 拷贝文件：C

4. 删除文件：D

5. 取消标记：u

6. 目录的切换与一般操作系统的切换相似

7. 刷新：g

8. 执行所有标记：x，这个似乎除了在删除的时候有用，其他的在我的机器上都没有起作用。

9. 取消所有标记：U

10. 压缩：c

11. 标记删除：d

12. 执行所有标记的删除：x

    此外，对于目录操作中的递归删除改为默认的话可以增加如下配置：





