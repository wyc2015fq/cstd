# Keil创建ARM芯片程序工程 - 小灰笔记 - CSDN博客





2017年09月03日 00:28:22[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：435








            近期接触了ARM芯片，学习的条件从软硬件上来说都不具备。等待公司解决这种问题是很难的，在这方面我基本上放弃了对公司的最后一点幻想。我觉得，如果想要掌握什么技能或者去接触一点比较有意思的东西，还是得考自己去投入、努力。通常，类似的学习基础环境在淘宝上都能够解决。

            买了开发板，又找来了Keil试用版本。接下来基本上就可以开始不断挖掘知识金矿了！从最简单的开始，创建一个简单的工程。

            一般，类似的工作我走出了一条相对熟悉的路线。那就是先把IDE搞定，然后再去考虑命令行实现IDE已经实现的功能的自动化，最后再在IDE的基础上进行扩展。下面简单整理一下使用Keil创建STM32F429的软件工程的方法：

1，启动IDE，在project菜单下选择创建功能：

![](https://img-blog.csdn.net/20170903002544838?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


2，选择工程将要创建的位置，输入工程名称

![](https://img-blog.csdn.net/20170903002554040?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


3，保存后，在弹出来的界面中选择相应的芯片型号之后点击确认

![](https://img-blog.csdn.net/20170903002604808?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


4，在弹出的界面中选择自己即将使用的驱动（这部分其实也可以先跳过，后期配置）

![](https://img-blog.csdn.net/20170903002615267?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


5，这样，一个软件工程就创建完成

![](https://img-blog.csdn.net/20170903002626880?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


6，如果给软件工程在增减一个main函数，那么软件工程就可以进行编译链接了。添加后，编译效果如下：

![](https://img-blog.csdn.net/20170903002637124?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)






