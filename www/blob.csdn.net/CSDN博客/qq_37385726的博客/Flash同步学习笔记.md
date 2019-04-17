# Flash同步学习笔记~ - qq_37385726的博客 - CSDN博客





2017年05月08日 17:41:19[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：588








**一：新建一个fla文件**

1.选择 Actionscripe2.0---点击确定=========就建好了一个空白的fla文件

2.点击文件---点击新建---选择模板=========就建好了一个模板fla文件




**二：在制作flash动画前的首要步骤**

首先要确定动画的尺寸大小以及背景颜色等，以方便后期制作。

方法：

菜单栏的修改---文档========弹出文档设置的对话框就可以去设置fla文件的尺寸大小及背景颜色，完成制作动画的首要步骤。

（也可以在文档设置的对话框修改帧频【每秒播放的帧数】）




**三：如何精确舞台中的图像所置的位置**

1.使用网格：![](https://img-blog.csdn.net/20170508174031614?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzczODU3MjY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

选择视图---网格---显示网格=====舞台上就会出现网格

选择视图---网格---编辑网格=====就会弹出对话框可以修改网格的颜色，宽度，高度，贴近度(这个要勾选了贴紧至网格后才有用)

2.使用标尺：

选择视图---标尺=======舞台上就会出现标尺

3.使用辅助线：

选择视图---辅助线（有显示辅助线，锁定辅助线，编辑辅助线，删除辅助线这几个功能）




使用以上这三种方式可以实现图像的精确定位。




**四：如何导入图片**

文件---导入---导入到舞台=========就可以将图片导入到舞台上了

（这里有一个小点要提醒的是，不是选择文件里的打开，打开是针对于打开fla文件而言的，要选择导入哦）




**五：如何改变导入图片的大小方向**

说完了如何导入图片，自然就会谈到如何改变导入图片的大小和方向了。

选择选取工具那一栏的![](https://img-blog.csdn.net/20170508174528840?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzczODU3MjY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)任意变形工具，即可完成图片大小和方向的变换




**六：导入图片转矢量图**

我们知道我们导入的图片的格式一般是JPG等，但是flash的绘图是矢量形式的，这也就会导致你导入后直接在图片上进行绘画会产生全部画到了图片后面的情况，所以我们必须先将导入的图片转成矢量图。

右键图片---分离=======即将图片转成矢量图了




**七：怎么将颜色设置成渐变色**

①

![](https://img-blog.csdn.net/20170508202350791?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzczODU3MjY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)属性面板上的颜色-----选择线性渐变就可以自己去设置渐变色了~





如果你的属性面板上没有这个选项，你也可以选择菜单栏的窗口----勾上颜色====就会出现在属性面板上了




②也可以直接选择样本里已有的渐变色（还有径向渐变可以选哦）

![](https://img-blog.csdn.net/20170508203447603?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzczODU3MjY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





**八：绘图工具介绍（部分）**

1.刷子工具

选择刷子工具后，有四种模式可以选择：

1）标准绘画：也是默认的绘图模式，对所有区域都有效。

2）颜料填充：只对填色区域有效，对图形中的线条不产生影响。（高亮的这部分字就是颜料填充与标准绘画之间的区别）

3）后面绘画：只对图形后面的空白区域有效，不影响原图形。

4）颜料选择：必须先用选择工具选区一个区域作为颜料选择的选中区，然后再使用刷子的时候就只会填充这一个选中的区域。

5）内部绘画：刷子的笔触一定要落在图片内，然后再画，只对图片内部有效，不影响外面。




详细带图的介绍可以看看这篇文章：[flash刷子工具的使用方法](http://www.jb51.net/flash/44519.html)




2.线条工具

①选择线条工具后在属性面板可以选择接合方式：尖角、圆角、斜角。

后面有尖角![](https://img-blog.csdn.net/20170508181035283?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzczODU3MjY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)可以用于设置尖角接合的清晰度（1~60），越大越清晰。

②按住shift键可以画水平直线或垂直直线（这个画正交的功能铅笔也可以用，对于矩形就画成正方形，对于椭圆就画成圆）




3.喷涂刷工具

可以看看这篇文章：[喷涂刷工具](http://article.pchome.net/content-1266440.html)




4.铅笔工具

一定要和刷子工具区分开来，刷子工具适用于填色的，而不是用于在空白舞台上画画的，画画应该用铅笔工具。

使用铅笔工具在图片上画画时，不会对图片本身有影响。




5.颜料桶工具

①工具栏里面可以选择颜料桶工具的填充样式：不封闭空隙、封闭小空隙、封闭中空隙、封闭大空隙。

②锁定填充是针对于渐变色来说的：[填充锁定工具怎么用](https://zhidao.baidu.com/question/745408345896927492.html)




6.墨水瓶工具

①它是针对于图形的轮廓来说的（油漆桶是针对于图形的内部填充来说的），可以通过墨水瓶工具去改变图形的轮廓大小颜色等。

[墨水瓶工具如何使用](http://jingyan.baidu.com/article/73c3ce28d0b0afe50343d927.html)


但是自己去尝试过之后，会发现画一个椭圆去改椭圆的边线，只要点击铅笔工具再调节也能够做到，那他们两者到底有什么区别呢？

请看下面这个回答：[墨水瓶和铅笔工具的区别](https://zhidao.baidu.com/question/267103207595853645.html)

其实就是墨水瓶工具可以给图片加边框，但是铅笔不可以，请看效果：

![](https://img-blog.csdn.net/20170508200125912?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzczODU3MjY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





7.矩形区域，基本矩形区域，椭圆区域，基本椭圆区域，多边形区域，多角星形区域

这些东西看工具栏下方和属性面板就很简单。

重点说一下矩形（椭圆）区域和基本矩形（椭圆）区域的差别：[矩形（椭圆）区域和基本椭圆（矩形）区域的差别](https://zhidao.baidu.com/question/195680108.html)

再来谈一下个人理解：

①我们点击矩形区域和基本矩形区域会发现在工具栏上有一个地方有不同——就是有没有对象绘制这个按钮。这个说明什么？对象绘制就是将区域内部和区域的边界连成一个整体，而基本矩形区域没有这个按钮，因为它本身就已经自动选择了将区域内部和区域的边界连成一个整体，但是矩形区域有这个按钮，因为它需要人工去选择是否连成一个整体。

②并且尝试之后还发现如果选了基本矩形区域或者是对象绘制，选取工具是不可以对其造型进行编辑的。




**九：对象选取工具**

1.钢笔工具（嘻嘻嘻，这个可以自行去百度^~^）

就说一小点：按住Alt键可以将钢笔转换成转换描点工具




2.关于选取工具

![](https://img-blog.csdn.net/20170508210920103?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzczODU3MjY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


![](https://img-blog.csdn.net/20170508211037337?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzczODU3MjY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


![](https://img-blog.csdn.net/20170508211129950?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzczODU3MjY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





3.关于部分选取工具

![](https://img-blog.csdn.net/20170508211245436?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzczODU3MjY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


![](https://img-blog.csdn.net/20170508211601403?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzczODU3MjY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)控 制  点


4.关于套索工具

![](https://img-blog.csdn.net/20170508211758482?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzczODU3MjY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center) 套索工具下面有魔棒~~




**十一：变形工具**

①任意变形工具

工具栏下方有贴紧至对象，旋转，缩放，扭曲，封套几个模式可以选。

个人觉得，选择贴紧至对象就好了，像旋转，缩放，这个不选也可以用。

！！！！！！！！！！！！！！！！！！！！！！！！

但是这里有一个问题可能会出现：就是你会发现你的扭曲和封套点不动····

这里就要知道，是因为你导入的图，还没有分离，没有转成矢量图。

②渐变变形工具

（长按任意变形工具就可以弹出来选择了）




**十二：橡皮擦工具**

（工具栏下面有模式可以选~~）















