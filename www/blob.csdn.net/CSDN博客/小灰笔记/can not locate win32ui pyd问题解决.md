# can not locate win32ui.pyd问题解决 - 小灰笔记 - CSDN博客





2017年05月14日 23:29:04[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：1239








       使用Pythonwin的时候遇到了can not locate win32ui.pyd的问题，还是排查了好长时间。具体的问题如下面的截图;




![](https://img-blog.csdn.net/20170514232845429?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




       在网络上寻找，没有找到解决方案。后来，突然发现原来自己机器上安装的Python软件居然是32位的！而pythonwin则是64位的。终于找到了因由所在，动态链接库实现的CPU类型不一致。

       重新安装了32位的pythonwin，运行不再出问题。

       如此问题只是告一段落，但是并不是最终的结果。毕竟，64位的机器没必要放弃自己的长处使用32位的软件。接下来还是换成64位软件吧！



