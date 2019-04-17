# Emacs-194-内置的hungry-delete - 小灰笔记 - CSDN博客





2018年03月24日 21:19:28[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：96








       有如下代码：

![](https://img-blog.csdn.net/2018032421191237)

       其中，缩进使用的都是TAB键。

       如果激活hungry-delete，进行退格。那么操作的方式如下：

       C-cBACKSAPCE

       操作之后，光标的位置以及文本内容状态如下：

![](https://img-blog.csdn.net/20180324211922776)

       hungry-delete把光标前所有的空白全都删除了。值得说的一点是，这个操作并不是在任何模式中都有效，上面的这个是在C语言的模式中执行的。我测试过Perl，相应的功能并不能执行，或许是有其他的操作方式也不好说。




