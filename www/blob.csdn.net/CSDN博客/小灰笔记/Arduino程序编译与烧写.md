# Arduino程序编译与烧写 - 小灰笔记 - CSDN博客





2017年12月24日 13:54:32[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：2529








       买树莓派的时候知道了这个小板子，一同买了。买来太久，一直没有玩过。近段时间倒是花了点时间把手头的几个开发板学习、探索了一下。现在轮到Arduino了。

       其实，比较简单的入门就是安装一下开发环境烧写个程序了。这个完成后，接下来的折腾就顺理成章了。相应的环境搭建主要是就两部分：1，串口驱动的安装；2，Arduino IDE的安装。串口驱动的安装是比较通用的一个，Arduino的安装则需要去其官网上下载。

       IDE有安装版本，也有绿色版本。我觉得安装版麻烦，就直接下载了免安装版本。比较好的一点是软件打开后已经默认是中文版本，看起来是增加了区域语言的识别。

![](https://img-blog.csdn.net/20171224135208254?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




       通过上面的菜单操作可以导入一个示例程序工程。

![](https://img-blog.csdn.net/20171224135232897?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




       以上是导入例程之后的样子，画出来的图标中，对号是编译启动功能而箭头是编译并下载。在此之前，需要先选择串口配置。

![](https://img-blog.csdn.net/20171224135311233?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




       通过如上界面，选择相应的串口。比较人性化的是，串口中连接到Arduino的串口端口会有相应的文字提示。选项的上面是开发板的选择，我自己用的ArduinoUno。

       好了，接下来就是编译烧写了。简化的方法就是直接点击向右的箭头了。

![](https://img-blog.csdn.net/20171224135356424?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




       编译以及下载（IDE中叫做上传），在下面的框条中会有显示。从上面看，我的程序已经烧写成功。

       按下板子上的复位键，LED等三次闪烁后代表复位成功。接下来即可看到LED的规律闪烁。



