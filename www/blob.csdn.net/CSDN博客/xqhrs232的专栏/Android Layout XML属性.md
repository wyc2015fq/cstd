# Android Layout XML属性 - xqhrs232的专栏 - CSDN博客
2012年03月23日 15:32:15[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：535标签：[layout																[android																[xml																[button																[文档](https://so.csdn.net/so/search/s.do?q=文档&t=blog)](https://so.csdn.net/so/search/s.do?q=button&t=blog)](https://so.csdn.net/so/search/s.do?q=xml&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)](https://so.csdn.net/so/search/s.do?q=layout&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://www.cnblogs.com/playing/archive/2011/04/07/2008620.html](http://www.cnblogs.com/playing/archive/2011/04/07/2008620.html)
相关网帖
1.android布局属性详解----[http://hi.baidu.com/lfcomputer/blog/item/336cd022dcadbce2d7cae290.html](http://hi.baidu.com/lfcomputer/blog/item/336cd022dcadbce2d7cae290.html)
2.Android Layout XML属性----[http://www.189works.com/article-15525-1.html](http://www.189works.com/article-15525-1.html)
3.android布局属性详解----[http://mobile.51cto.com/android-265842.htm](http://mobile.51cto.com/android-265842.htm)

Layout对于迅速的搭建界面和提高界面在不同分辨率的屏幕上的适应性具有很大的作用。这里简要介绍Android的Layout和研究一下它的实现。
Android有Layout：FrameLayout，LinearLayout，TableLayout，RelativeLayout,AbsoluteLayout。
放入Layout中进行排布的View的XML属性：
1.几种Layout中**Item**所共有的XML属性：
- (1)layout_width 
- (2)layout_height 
**注: (1)和(2)用来确定放入Layout中的View的宽度和高度：它们的可能取值为fill_parent，wrap_content或者固定的像素值。**
- (3)layout_marginLeft         --->paddingLeft 
- (4)layout_marginTop         --->paddingTop 
- (5)layout_marginRight       --->paddingRight 
- (6)layout_marginBottom    --->paddingBottom 
- (补)layout_margin              --->padding 
** 注:(3)(4)(5)(6)是放入Layout中的View期望它能够和Layout的边界或者其他View之间能够相距一段距离。**
- (7)layout_gravity 
**注:(7)用来确定View在Layout中的停靠位置。**
**FrameLayout**是最简单的Layout，就只具有这些属性。
**LinearLayout**还会有：
- (8)layout_weight 
****注:** (8)用于在LinearLayout中把所有子View排布之后的剩余空间按照它们的layout_weight分配给各个拥有这个属性的View。**
**TableLayout**的行TableRow是一个横向的（horizontal）的LinearLayout
**RelativeLayout**有16个align相关的XML属性：
- (9 )layout_alignParentBottom    当前控件低端与父控件的低端对齐(重合） 
- (10)layout_alignParentLeft        当前控件左端与父控件的左端对齐(重合） 
- (11)layout_alignParentRight      当前控件右端与父控件的右端对齐(重合） 
- (12)layout_alignParentTop        当前控件上端与父控件的上端对齐(重合） 
- (13)layout_centerHorizontal      当前控件位于父控件的横向中间位置（水平方向上的中间） 
- (14)layout_centerInParent        当前控件位于父控件的纵横向中间位置（垂直方向上的中间） 
- (15)layout_centerVertical          当前控件位于父控件的纵向中间位置（平面上的正中间） 
- (16)layout_above             使当前控件位于给出id控件的上方 
- (17)layout_below             使当前控件位于给出id控件的下方 
- (18)layout_toLeftOf          使当前控件位于给出id控件的左侧 
- (19)layout_toRightOf        使当前控件位于给出id控件的右侧 
- (20)layout_alignBottom     使当前控件与给出id控件的底部部重合**(注意可用和给出id控件来对齐）**
- (21)layout_alignLeft          使当前控件与给出id控件的左边重合 
- (22)layout_alignRight        使当前控件与给出id控件的右边重合 
- (23)layout_alignTop          使当前控件与给出id控件的顶部重合 
- (24)layout_alignBaseline    使当前控件的BaseLine与给出id控件t的BaseLine重合，这个主要用于Label或者其他包含文本的widgets。
** 注:(9)到(24)用来确定RelativeLayout中的View相对于Layout或者Layout中的其他View的位置。**
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
根据Android的文档，Android会对Layou和View嵌套组成的这棵树进行2次遍历，一次是measure调用，用来确定Layout或者View的大小；一次是layout调用，用来确定Layout或者view的位置。当然后来我自己的山寨实现把这2次调用合并到了一起。那就是Layout在排布之前都对自己进行measure一次，然后对View递归调用Layout方法。这样子的大小肯定是确定了的。然后用确定了的大小来使用gravity或者align属性来定位，使用margin来调整位置。
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
补充：
1.android:paddingLeft与android:layout_marginLeft的区别：
      padding margin都是边距的含义，关键问题得明白是什么相对什么的边距．
      padding是控件的内容相对控件的边缘的边距．
      layout_margin是控件边缘相对父空间的边距．
2.android gravity和layout_gravity区别
　　android:gravity　属性是对该view 内容的限定．比如一个button 上面的text.  你可以设置该text 在view的靠左，靠右等位置．该属性就干了这个．
      android:layout_gravity是用来设置该view相对与起父view 的位置．比如一个button 在linearlayout里，你想把该button放在靠左靠右等位置就可以通过该属性设置．
这样就解释了，有什么我们弄个最外布局，然后里面包了几个布局，如果要使这几个布局都靠底，就可以在最外布局的属性里设置androi:gravity="botton"  因为gravity是对里面的内容起作用．

