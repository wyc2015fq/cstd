# Emacs-128-spacemacs中禁用if条件下缩进 - 小灰笔记 - CSDN博客





2018年03月11日 16:38:09[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：126








            使用spacemacs编辑C++代码发现if等结构控制中的代码块会被强制缩进，我自己不是很喜欢这样的缩进风格。之前也调整过，在spacemacs中再次实现一下。

配置如下：

![](https://img-blog.csdn.net/20180311163723755)



            在初始化部分加入321行的配置，禁用子表达式的缩进。之后的代码风格已经没有缩进，效果如下：
![](https://img-blog.csdn.net/20180311163759742)



