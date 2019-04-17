# Emacs-047-org模式中自动修正编号 - 小灰笔记 - CSDN博客





2018年01月14日 16:09:23[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：137
个人分类：[Emacs](https://blog.csdn.net/grey_csdn/article/category/7131378)









Emacs-047-org模式中自动修正编号

这个功能感觉有点让人欣喜，确实是个不错的功能。如果出现在像Office这样的软件中兴许不是很细怪，但是出现在了编辑器中确实是让我很意外。

大致实现的功能就是编号的自动修正，修正功能的启动时通过M-回车。

下面通过简单的示例进行说明，加入有如下org文件内容：

![](https://img-blog.csdn.net/20180114160856230?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




编号有点混乱，但是我们一般都想要连续的，如何改成连续的呢？手动修改自然是一个方法，但是还有更简单的方法，那就是光标移动到编号10那一行的最后然后按下前面说到的M-return组合键。完成后的效果如下：

![](https://img-blog.csdn.net/20180114160906856?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




虽说是增加了一行，但是一个空行的处理还是很简单的。



