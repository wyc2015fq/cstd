# 科学网—I LOVE U的数学表达 - 陈奎孚的博文




# I LOVE U的数学表达                           

已有 2107 次阅读2017-11-27 16:46|个人分类:[未分类](http://blog.sciencenet.cn/home.php?mod=space&uid=510768&do=blog&classid=149217&view=me)|系统分类:[科普集锦](http://blog.sciencenet.cn/home.php?mod=space&do=blog&view=all&uid=510768&catid=7)



    古典的理科生相对呆一些，想表达❤，但又羞于直说，于是便网传这样的图片

![](home.php?mod=attachment&filename=image001.gif&id=129990)

上述图形作为《公式图形不烦恼》课程例题，肯定是蛮有趣的(浪漫不浪浪不知道，老师是古典型的)。作为课程例题，拓展一下，我们来画I LOVE U.

  I 可用一条竖直线表示，它不是常规函数的图形，但其参数方程非常简单，即![](home.php?mod=attachment&filename=image002.gif&id=129992)。用代码：ParametricPlot[{1,t},{t,0,2}]可以画出来

![](home.php?mod=attachment&filename=ILOVEU1.jpg&id=129993)

注意在上述代码中a取成具体数1了。Mathematica最霸气的是可进行“纯”代数的公式推导，所涉的代数符号不用取具体的数值。然而对图形而言，每个点是需要确定的几何位置的，以便在屏幕上表现出来，所以在画图前，所有符号的数值都必须是确定的，故而这里取a=1。至于t,代码只给了范围，没给具体值，这是因为软件自己会按一定规则选定t的值。



另外数学坐标轴(Axes)会破坏艺术的感觉，也应去掉，但古典理科生虽装但想表现，所以把最好把曲线的方程标(PlotLabel)上去。实现上述要求的代码是：C0=ParametricPlot[{1,t},{t,0,2},Axes->False,PlotLabel->x==1],效果是

![](home.php?mod=attachment&filename=ILOVEU2.jpg&id=129997)

“L”在网传的图片中用1/x,这非常适合用Plot来画，因为这是个真正的函数。不过有点美中不足的是：KFC觉得1/x曲线拐弯的曲率半径太大了，所以用1/x^9来实现，代码为C1=Plot[1/x^9,{x,0.01,2},Axes->False,PlotLabel->“1/x^9”],效果如下

![](home.php?mod=attachment&filename=ILOVEU3.jpg&id=129998)

注意代码的左边有"C1="，这是把图形赋给了C1。这样,随后需要此图，直接引用C1即可(不用重画)了。画"I"的代码中"C0="也是这个道理。




    "O"是封闭的曲线，不是标准的函数，可以用圆曲线来表示。对于圆曲线，极坐标画法最方便了，代码为C2=PolarPlot[1,{q,0,2Pi}, Axes->False,PlotLabel->x^2+y^2==1]，效果如下



![](home.php?mod=attachment&filename=ILOVEU4.jpg&id=130001)




  "V"最容易实现，用|x|即可，这也是函数，适合Plot画，代码为：C3=Plot[Abs[x],{x,-2,2 },Axes->False,PlotLabel->Abs[x]],效果如下

![](home.php?mod=attachment&filename=ILOVEU5.jpg&id=130002)

“E”的实现稍微复杂一些，网传形式为隐函数式。画这种关系式的方法是等值线图，即ContourPlot,代码为C4=ContourPlot[x==-Abs[Sin[y*3]],{x,-1.5,0.5},{y,-Pi/3,Pi/3},Axes->False,Frame->False,PlotLabel->x==Abs[Sin[3y]]]，效果如下

![](home.php?mod=attachment&filename=ILOVEU6.jpg&id=130003)

因为等值线图缺省有外面的方框，所以这里强行通过Frame->False把外框去掉。




    "U"用Tan[x^2]来实现，代码为：C5=Plot[Tan[x^2],{x,-1.2,1.2}, Axes®None,PlotLabel®Tan[x^2]]。

![](home.php?mod=attachment&filename=ILOVEU9.jpg&id=130004)

上面把6个字母画出来。6个字母同时显示用Show函数，代码为Show[{C0,C1,C2,C3,C4,C5}],效果如下

![](home.php?mod=attachment&filename=ILOVEU7.jpg&id=130005)

这显然不是我们希望的结果。

我们是要一字排开的ILOVEU. Mathematica的实现方式用GraphicsGrid函数，代码为GraphicsGrid[{{C0, C1, C2, C3, C4, C5}}]，效果如下

![](home.php?mod=attachment&filename=ILOVEU8.jpg&id=130006)




    这么做下来，成就感有了，但情感在哪里呢？

       想看更多的精彩数学曲线，请手机扫二维码关注我课程《公式图形不烦恼》的微信公众号吧！

![](home.php?mod=attachment&filename=%B9%AB%D6%DA%BA%C5%B6%FE%CE%AC%C2%EB.jpg&id=130008)


转载本文请联系原作者获取授权，同时请注明本文来自陈奎孚科学网博客。
链接地址：[http://blog.sciencenet.cn/blog-510768-1087098.html](http://blog.sciencenet.cn/blog-510768-1087098.html)

上一篇：[你相信这是条数学曲线吗？](blog-510768-1086935.html)
下一篇：[“正能量”的“正”字是否冗余？](blog-510768-1087319.html)


