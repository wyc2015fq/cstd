# Emacs-187-创建名称中带有空白的文件 - 小灰笔记 - CSDN博客





2018年03月24日 21:12:24[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：57
个人分类：[Emacs](https://blog.csdn.net/grey_csdn/article/category/7131378)









       在新文件创建前，dired查看原始目录信息如下：

![](https://img-blog.csdn.net/20180324211203694)

       接下来创建一个名称为“a  1.txt”的文本文件。

       创建的方式其实跟普通命名的文件没有任何差异，直接执行如下命令：

       C-xC-f

       在输入文件名称的时候也并不需要进行特殊的处理，直接输入文件名称即可，空格直接输入空格。如下：

![](https://img-blog.csdn.net/20180324211211625)

       RET，之后保存buffer完成创建。创建完成后，dired查看目录信息如下：

![](https://img-blog.csdn.net/20180324211218756)




