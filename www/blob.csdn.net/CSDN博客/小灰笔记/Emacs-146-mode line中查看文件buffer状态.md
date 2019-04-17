# Emacs-146-mode line中查看文件buffer状态 - 小灰笔记 - CSDN博客





2018年03月15日 20:47:48[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：158
个人分类：[Emacs](https://blog.csdn.net/grey_csdn/article/category/7131378)









![](https://img-blog.csdn.net/20180315204719796)

上面是一个文件刚刚读入到buffer的时候显示状态，两个横线代表文件没有变化。

![](https://img-blog.csdn.net/20180315204726215)

上面进行了插入行的操作，Buffer发生了变化，因此状态时**。**代表的就是发生了变化没保存，这个让我有点联想到了Simulink的状态，那里用的符号也是*，只是只有一个。

如果是%*的话说明文件Buffer是只读状态，不过这个目前没有实现这种状态，还不熟悉什么时候会出现。

![](https://img-blog.csdn.net/20180315204740181)

如果是%%，说明是其他的状态。比如，在diredmode中，这个显示就是%%。




