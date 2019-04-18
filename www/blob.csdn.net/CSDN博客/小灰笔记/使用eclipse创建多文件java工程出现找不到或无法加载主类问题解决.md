# 使用eclipse创建多文件java工程出现找不到或无法加载主类问题解决 - 小灰笔记 - CSDN博客





2017年02月20日 00:43:06[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：1138








![](https://img-blog.csdn.net/20170220004236209?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




上面的这个问题在没有使用eclipse IDE的时候就已经遇到过，也就是上午的事情。我暂时把这个问题归类为“包管理”出现了问题，就是package使用出现了问题。不过，上午尝试解决的时候也是凭借尝试以及运气解决的。出现这个问题其实也是我想换用eclipse的一个原因，我觉得IDE毕竟还是能够把部分问题化繁为简。没想到，第一次尝试又遇到了这个问题。

经过多次尝试，最终问题得以解决。我学习的时候参考的是慕课网上的视频教程，仔细对比了一下我们两个人的操作差异，发现正确的方式应该是在包上右击鼠标创建新的类。而我操作的错误在于右击鼠标的位置确实工程目录结构上的src文件夹。

重新按照正确的方式进行了操作，同时使用了新的一个IDE技能，也就是在创建类的时候同时创建了main函数。这只需要在IDE的弹出框中勾选相应的参数。

![](https://img-blog.csdn.net/20170220004826233?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





代码编辑完后运行结果：

![](https://img-blog.csdn.net/20170220005026674?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




期待的运行结果出现！



