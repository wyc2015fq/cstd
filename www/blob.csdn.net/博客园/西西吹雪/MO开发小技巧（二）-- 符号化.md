
# MO开发小技巧（二）-- 符号化 - 西西吹雪 - 博客园
# [编程　&　生活      --       西西吹雪](https://www.cnblogs.com/watsonyin/)
开心工作，快乐生活  (原创文章，转载请注明出处)

### 导航
[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/watsonyin/)
[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)
[联系](https://msg.cnblogs.com/send/%E8%A5%BF%E8%A5%BF%E5%90%B9%E9%9B%AA)[订阅](https://www.cnblogs.com/watsonyin/rss)![订阅](//www.cnblogs.com/images/xml.gif)[管理](https://i.cnblogs.com/)
统计
随笔 -		88文章 -		10评论 -		1793引用 -		18
公告

[MO开发小技巧（二）-- 符号化](https://www.cnblogs.com/watsonyin/archive/2005/03/24/125035.html)
这一篇讲一下如何通过MO实现符号化，请多多指教
一、先看一下鄙人完成的程序的界面：
![](https://images.cnblogs.com/cnblogs_com/watsonyin/theme.JPG)[   ](https://images.cnblogs.com/cnblogs_com/watsonyin/theme.JPG)
图一、符号化（专题图Thematic）设置界面
![](https://images.cnblogs.com/cnblogs_com/watsonyin/theme2.JPG)[   ](https://images.cnblogs.com/cnblogs_com/watsonyin/theme2.JPG)
图二、面状符号选择面板
![](https://images.cnblogs.com/cnblogs_com/watsonyin/theme3.JPG)[ ](https://images.cnblogs.com/cnblogs_com/watsonyin/theme3.JPG)
图三、 线状符号选择面板
可以看到，整个符号化界面是模仿ArcView 来完成的。

二、符号化实现的基本思路
实现符号化的思路很多，下面介绍一下本人实现的思路。 〕
1、点状符号化：采用字体来完成即可。
制作字体的方法：建议采用CoralDraw完成。
2、线状符号化：
打开MO自带的例子CustomSym，先仔细看一下代码，搞清楚其实现的思路。
MO在符号化方面提供了一个接口叫ICustom，该接口有三个需要实现的接口函数：SetupDC，Draw和ResetDC。很容易理解，三个函数分别实现以下功能：
SetupDC： 熟悉GDI编程的朋友都知道，Windows界面中所看到的一切都是通过函数（本质是都是调API）绘制在一个DC上的。DC就相当于作画的画布。SetupDC的作用就是在作画之前先作好准备工作。这些准备工作包括绘制的画笔（Pen）或画刷（Brush），以及画布的相关信息。
该函数的原型是：Public Sub SetupDC(hdc As Long, dpi As Double, baseSym As Object)
其中hdc就是画布的句柄，dpi为画布的分辨率，baseSym是一个Symbol对象，定义了画笔的风格，如Symbol.Color为画笔的颜色，Symbol.Size为画笔的宽度。当然只要你愿意，也可以不用它，在这里，完全可以自由发挥。
Draw：到了真正绘制的时候了，这个函数中的代码，决定了整个符号化所表现出来的效果。
该函数的原型是：Public Sub Draw(hdc As Long, points As Long, partCounts As Long, numParts As Long)
该函数有四个参数，第一个同样是画布的句柄，后面三个定义了要绘制的数据。其中points函数给出了所有要绘制的线的组成点的集合的首地址；partcount也是指向了一个数组，它定义了每一条线的点的个数； numParts告诉我们这么多点是由多少了断开的线所组成。细心的朋友会发现，这三个参数与一个API函数Polypolyline的参数如出一辙！ 确实如此，如果我们在定义的画笔之后，直接调用polypolyline(points,partCounts,numParts)，执行，你会发现数据的显示已经变成了你所定义的样式。这本身提供了一种符号化的方法，即定义画笔的方式。该方法实现思路简单，运行速度快，但难以做到随心所欲的绘制，很难完全满足现实需要。
在CustomSym例子中，用到了一个库叫mohelper，其功能是将参数中给出的点集得新组合整理，变成一个个实实在在线对象（MultiShape）。有了这些线，绘制就容易多了。
下面看一看例子中给出的Draw函数的代码：
Public Sub Draw(hdc As Long, points As Long, partCounts As Long, numParts As Long)
Dim pl As New MoHelper.MultiShape ' new polyline
pl.Init points, partCounts, numParts
Dim l As MoHelper.Shape   ' line pointer
Dim i As Long
For i = 0 To numParts - 1
Set l = pl.Part(i)   ' point to next line
'Debug.Print "count: " + CStr(i) + ":" + CStr(l.Count)
PolylineL hdc, l.Pointer, l.Count
DrawTies hdc, l
Next i
End Sub
思路很明显，就是逐条绘制集合中的第一条线。我采用的也是同样的做法。只不过，为了实现自定义线型，需要抽象更多的东西，其中最重要的一个是LineUnit，即绘制单元，下面称为线单元。当然，这也是建立在一个假设之上，即所有的线型都是以某一长度的单元作为模板重复绘制的（事实上不一定如此，但很少，对于这种情况，可以单独出来考虑）。所以要实现绘制功能，第一步就是将一条线按长度划分成若干个单元，这个工作并不复杂。事实上，在CustomSym例子中DrawTies函数已经实现了这个功能。
所以现在问题的关键变成了如保定义和绘制第一个线单元。
一个线单元由若干固定要素构成。由于绘制一条线的过程，就是重复绘制一个线单元的过程，每个线单元都有一个“中心线”，它决定了这个线单元弯曲的形状和走向。现在假设我们把这个中心线拉直，那么这个线单元中所有要素的相对位置也是固定不变的。现在我们假设每个要素为一个层，那么一个线单元就是由多个相对位置的层构成。由此我们可以设计一个LineUnit类，用于保存所有这些要素的形状以及位置信息。在绘制时，再根据中心线的位置，确定每个要素的实际位置。并将其绘制在hdc上。
实际应用中包含多少种要素，怎么样划分这些要素，方式多种多样，请朋友们自己思考完成。
ResetDC函数 ：即绘制完毕后，进行善后处理工作。主要是进行资源清理和状态回复。
函数原型是：Public Sub ResetDC(hdc As Long)
3、面状符号化：
面状符号化也是通过ICustom接口实现的。
实现的思路也有很多种。这里只介绍一下本人的实现方法－－采用图片填充。该方法思路简单，实现方便，速度也很快。图片如何填充呢？在Windows API编程中，填充采用刷子（Brush）来进行的，而这个刷子又可以通过位图来定义，这就为面状符号化提供了一个最简单的方法。
三、符号化中碰到的实际问题
1、怎么制符号库
符号化要实现，必须可以自定义符号，制作自己的符号库。基本思路是以记录的形式定义符号的结构，然后将这些数据依次写入文件中。对于线状符号，每条记录等长，读写比较方便。对于面状符号，由于采用图片，图片可能不一样， 这时，可同时建立一个索引文件，记录每条记录的长度以及在文件中的位置，以便于进行检索。
2、面状填充前景色的背景色的实现
图片采用pf1bit格式存储，这样文件大小非常小，而且图片只有黑白两色，这样在使用前方便进行预处理。可以将黑色的像素处理成前景色，白色的像素处理背景色。
3、将自定义的符号化显示到任意画布的任意区域中。
经过上述思路，虽然实现了符号化，但怎样将符号的样式显示在指定的位置，从而实现符号面板的制作呢？其实很简单，上面讲到，实现符号化实际就是实现了ICustom接口的三个函数：SetUpDC,Draw和ResetDC，其实我们完全可以利用这三个函数将符号显示在任意区域。具体方法请朋友们自己思考。





posted on2005-03-24 17:48[西西吹雪](https://www.cnblogs.com/watsonyin/)阅读(...) 评论(...)[编辑](https://i.cnblogs.com/EditPosts.aspx?postid=125035)[收藏](#)


[刷新评论](javascript:void(0);)[刷新页面](#)[返回顶部](#top)






Powered by:
博客园
Copyright © 西西吹雪
