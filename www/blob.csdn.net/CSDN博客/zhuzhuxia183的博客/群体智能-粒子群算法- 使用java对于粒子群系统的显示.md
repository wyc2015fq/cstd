# 群体智能-粒子群算法- 使用java对于粒子群系统的显示 - zhuzhuxia183的博客 - CSDN博客





2017年07月19日 18:28:13[喵小姐的邻居小小朱](https://me.csdn.net/zhuzhuxia183)阅读数：786










本篇短文讨论的是小编对于粒子群系统的一些理解，使用的语言是java代码，然用java中图形库加上线程的相关

知识， 实现的一个蜜蜂采寻花朵的一个效果。

首先希望各位读者在阅读之前，先注意下这段。（ps如果能力很强，可以忽略）因为虽然本篇博客中实现的一些

相关功能很少，但是还是使用了不少的知识点，为了方便新手上路的话，我将他们一一列举出来：1.java图形库JFra-

me类，然后使用Graphics画图，当然由于考虑画面的流畅性，后面使用了双缓冲。2.使用了线程，来对于我们的图

画进行显示。3.整体的话，因为使用java写的，所以整体的代码遵循面向对象思想。

好了，上面扯了很多的废话，现在开始，我们切入整题。我们今天讨论的主题其实就是一个比较有趣的算法。

粒子群算法。好吧，如果你没有听过的话，应该是很正常的吧，因为小编也是刚刚才了解过的。所以，不要怕。接下

小编就来谈谈自己是怎么理解的吧。

首先呢？我是百度了下 粒子群算法 这个名词，发现的话。额额额额，发现不是很好理解。不过还是有一些不错

的博客的。比如：[点击打开链接](http://blog.csdn.net/niuyongjie/article/details/1569671)大家可以进去看看，其实博客里面都讲的十分的详细了，可是把，由于本人的理解能

着实有限，所以的话，只能说，讲的似乎懂了。![尴尬](http://static.blog.csdn.net/xheditor/xheditor_emot/default/awkward.gif)

![](https://img-blog.csdn.net/20170719190849710?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1emh1eGlhMTgz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





所以，我得想想办法啊。然后的话，我就想看看，就没有什么比较简单的demo实例吗？我好看看代码，便于

理解嘛。很幸运，然后找到一篇。就是这篇。[点击打开链接](http://blog.csdn.net/yangbaoli1992/article/details/53001856)，我当时直接把代码全部抄着码了一遍，边抄边想，便是

开始有所感悟了。![](https://img-blog.csdn.net/20170719191813256?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1emh1eGlhMTgz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
















其实大家看到上面，我建议亲可以先去你的Eclipse（java的编辑器）上面敲遍了。再来看小编的杂文，好啦，我默认

亲已经码了一遍了哈。然后，我来谈谈自己对于这个算法的一些理解。为了方便讲解，我先提出一个问题：

现在有这么一群蜜蜂哈，他们想要找寻花朵来采蜜哈，但是他们可能无法像我们一样，可以直接看到花朵的具体的位置，然后径直地飞过去，但是呢？他们可以：1.每只蜜蜂可以通过花粉的强弱来感受花朵距离他们的远近 2.每一只蜜蜂呢，都可以记住在自己飞过的所有地方中感受花粉最强烈的那个点，即是具体花朵最近的那个点。3然后蜜蜂这个群体之间是可以进行通信的，所以呢，在任何一个时刻，他们都可以感知到是哪个位置的蜜蜂距离花朵最近。

哈哈，现在，我想告诉你，其实这个算法你可以认为是来解决这类问题的。好吧，如果对于数学公式很敏感的同学，应该开始有点思绪了。如果还不是很懂的同学，其实也没有必要灰心，可以对于上面的代码多敲几遍，可能就懂了呢？对吧。

 下面，我用粒子群算法来对以上题目进行解答：就是说，通过自己历史状态中的最佳位置和自己现在的位置的距离来作为速度改变影响因素之一，同时全局最佳的位置与自己现在的位置的距离也是作为速度改变影响因素之一。

然后呢？同时我们的现在位置=之前位置+速度×时间；好吧，放大招，如果在考试上，核心得分点就是：

![](https://img-blog.csdn.net/20170719202529296?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1emh1eGlhMTgz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


基本上，理解部分，到此位置吧。然后呢？我们来谈谈关于使用Java实现的蜜蜂采蜜效果。废话先不多说，先祭图吧。![](https://img-blog.csdn.net/20170719203455422?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1emh1eGlhMTgz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

左击画面上的某个花朵位置，模拟那个地方花朵散发出花粉，然后蜜蜂根据上面的算法会聚集靠近。

![](https://img-blog.csdn.net/20170719203808904?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1emh1eGlhMTgz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


当然了，为了方便演示，还弄了一个打破他们平衡的操作，就是右击的时候，就会出现一块火石，然后使他们混乱，之后的话，我们可以再次左击图片上某个花朵的位置，再次使他们聚集，当然除此之外，我还设置了，当他们靠近目标足够近后，便开始在小范围内改变速度。

好了，今天似乎看来就只能先介绍到这里了，具体的代码介绍以及源码的话，我会在下一篇博客中详谈。各位，加油！
























