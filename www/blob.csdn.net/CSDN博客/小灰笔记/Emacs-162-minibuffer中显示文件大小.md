# Emacs-162-minibuffer中显示文件大小 - 小灰笔记 - CSDN博客





2018年03月18日 11:12:59[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：111
个人分类：[Emacs](https://blog.csdn.net/grey_csdn/article/category/7131378)









![](https://img-blog.csdn.net/20180318111238340)

            上面是文档中的相关描述，在执行相应的命令之前，modeline的显示如下：

![](https://img-blog.csdn.net/201803181112444)

            突然发现一个很有意思的事情，不知道什么时候开始我的modeline居然还是显示了Git的信息。我点击查看了一下，确实是我自己维护的一个Git版本管理的信息。不过，这个版本库中的文件似乎全都没有读入到Buffer中。这个确实是一个很有意思的现象，不过后期能够把Git集成到Emacs中一块儿进行管理倒也是不错的。

            执行命令之后：

![](https://img-blog.csdn.net/20180318111249687)

            mode line中显示出了文件大小为1.7k，同时，minibuffer中显式出了执行成功的提示。

            总体来说，这个功能还是有一定使用价值的，能够大致预估现在编辑的对象的内容的多少。不过，于我自己来说真正使用的频率恐怕不会太高。




