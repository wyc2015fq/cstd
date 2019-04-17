# 串口给STM32阿波罗开发板下载程序 - 小灰笔记 - CSDN博客





2017年09月04日 00:21:42[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：777








![](https://img-blog.csdn.net/20170904001854886?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

            串口驱动安装好的状态下，把开发板通过USB线连到电脑上就会出现一个模拟串口。从截图看，我的电脑上出现的是COM4。

            大致的硬件连接注意点如下：

![](https://img-blog.csdn.net/20170904001906290?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

            一是画出来的跳线帽都要按照图中的进行连接，还有一个重要的一点就是按下右上角那边画出来的电源按钮。按下按钮之后，右下角的灯会亮。此时，板子通过串口进行供电，也可以进行程序的下载。

            使用FlyMCUE进行下载，配置修改如下：

1，

![](https://img-blog.csdn.net/20170904001926314?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

            需要搜索串口，识别到刚刚虚拟出来的串口。同时修改一下波特率以保证程序能够正常下载。不过，我自己电脑上的搜索确实是没有做，软件自动识别出来了。

2，左下角的配置需要按照截图中修改：

![](https://img-blog.csdn.net/20170904002029639?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


3，选择即将烧写的程序

![](https://img-blog.csdn.net/20170904001940697?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


4，点击开始编程，程序开始烧写同时右侧窗口出现烧写信息。

![](https://img-blog.csdn.net/20170904002050496?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


         从上面看出，烧写没有问题。烧写结束后，开发板的跑马灯开始执行。





