# Emacs-166-查看minibuffer中的历史输入记录 - 小灰笔记 - CSDN博客





2018年03月19日 20:40:00[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：98








       查看emacs minibuffer中的历史记录，其实有个专门的历史buffer可以访问。访问的方式跟我们shell终端中访问历史命令的方式十分相近。

       有两个方式可以进行：

1.     使用上下按键，这个跟命令终端中是一致的；

2.     M-p向上访问查看、M-n向下访问查看。

按键效果：

![](https://img-blog.csdn.net/20180319203907637)

组合键效果：

![](https://img-blog.csdn.net/20180319203915391)

       查看一下按键绑定的功能：

![](https://img-blog.csdn.net/20180319203922955)


![](https://img-blog.csdn.net/20180319203942996)


       从上面的结果看，似乎两者还是有一定差异。应该说上下箭头的功能可能更多一点，而组合键的方式更加精准一点。




