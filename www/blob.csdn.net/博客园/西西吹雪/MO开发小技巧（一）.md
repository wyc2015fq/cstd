
# MO开发小技巧（一） - 西西吹雪 - 博客园
# [编程　&　生活      --       西西吹雪](https://www.cnblogs.com/watsonyin/)
开心工作，快乐生活  (原创文章，转载请注明出处)

### 导航
[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/watsonyin/)
[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)
[联系](https://msg.cnblogs.com/send/%E8%A5%BF%E8%A5%BF%E5%90%B9%E9%9B%AA)[订阅](https://www.cnblogs.com/watsonyin/rss)![订阅](//www.cnblogs.com/images/xml.gif)[管理](https://i.cnblogs.com/)
统计
随笔 -		88文章 -		10评论 -		1793引用 -		18
公告

[MO开发小技巧（一）](https://www.cnblogs.com/watsonyin/archive/2005/03/23/124116.html)
本人用VB+MO开发近两年，中间积累了一些小经验，对老手可能没用，但对新手可能有一点帮助。下面把它记下来，也算是一个小小的总结。很多东西没想起来，下次更新时补上。 大部分内容只是概述了实现的思路，具体实现的方法，有心者自己领会吧～，不对之处，请指教。
一、出图线宽的问题。
问题描述：MO出图一般使用Output2方法。但该方法有一个问题就是出图后线宽特别细，这是由于打印机分辨率比屏幕分辨高得多的缘故。为了得与屏幕显示相同地线宽，必须在打印前对显示符号(Symbol)对象作相应的调整，执行完毕后再调整回来。
出图代码如下：
Dim dblScale As Double
dblScale = GetDeviceCaps(Printer.hdc, 88) / GetDeviceCaps(Me.hdc, 88)  '即打印机分辨率/屏幕分辨率
ResetLnSize dblScale
On Error GoTo errh
'此处执行打印操作,mpprinter是本人组织开发的专门用于打印出图的控件
mpPrinter.DoPrint()
ResetLnSize 1 / dblScale
'下面是用于缩放线宽的函数
Private Sub ResetLnSize(dblScale As Double)
Dim i As Integer
Dim obj As Object
Dim ly As MapLayer
For i = 0 To Map1.Layers.Count - 1
Set obj = Map1.Layers(i)
If TypeOf obj Is MapObjects2.MapLayer Then
Set ly = obj
If ly.ShapeType <> moShapeTypePoint Then
ly.Symbol.Size = CInt(ly.Symbol.Size * dblScale)
End If
End If
Next
'跟踪层的Event的线粗也要设置，只要设置跟踪层所有的symbol就可以了。
Dim Sym As MapObjects2.Symbol
For i = 0 To Map1.TrackingLayer.SymbolCount - 1
Set Sym = Map1.TrackingLayer.Symbol(i)
If Sym.SymbolType <> moPointSymbol Then
Sym.Size = CInt(Sym.Size * dblScale)
End If
Next i
End Sub

二、指北针。最简单的办法是使用ESRI公司提供的指北针字体ESRI_North。
三、注记的缩放。显示注记一般用LabelRenderer来实现，要实现与地图其它内容一起缩放，只要设置HeightField属性即可。HeightField即高度字段，是一个数值型字段。
四、关于编辑功能
1、新建线或面时的捕捉问题。用MO新建要素的传统方法是用TrackLine或TrackPolygon方法。但该方法有一个致命的弱点就是新建过程中不能进行捕捉，这是由于MO对系统消息进行了屏蔽的缘故。为了能够实现捕捉，只能自己再想办法实现。最简单办法是自己处理MouseDown，MouseMove，MouseUp事件，判断状态，然后在TrackLayer中添加或修改一个GeoEvent来实现，这样就可以在新建要素过程中实现捕捉了。
2、新建线、多边形点个数太多时的闪烁问题。当点的个数很多时，会闪烁比较厉害。为了避免出现这种情况，可以在新建过程中，将加入的点集人为分为若干个点一组，每一组成为一个独立的GeoEvent。但这样处理比较麻烦。
3、编辑回退的问题。首先将编辑进行分类，可以分为节点编辑，新增要素，删除要素三种。更新要素（如移动）可以归结为新增一个要素和删除一个要素，分割（split等)也是如此。然后根据这些类型定义不同的数据结构对其进行保存。保存方式可以保存在内存中，也可以保存为临时的Shape文件中，建议使用后者，因为保存属性比较方便，同时不占用内存，理论上可以不限回退步数。
4、历史版本问题。 要点：第一，需要保存历史数据的图层在入库时要建立一个对应的的历史图层，此时历史图层与现状层数据完全一致，作为历史数据的初始状态；第二，将所有编辑过程归结为删除和新增两种操作，编辑可以归结为删除一个（或几个）要素，同时新增一个（或几个）要素。然后在历史图层中添加两个字段：新增时间和删除时间，现状的数据的删除时间可以定义为9999-01-01。 第三要启用事务编辑，一次编辑过程包含若干个编辑步骤。一次编辑过程可以形成一个版本，也可以归入上一次编辑所属的版本中，在编辑结束时应允许选择。一次编辑过程中的的所有编辑步骤要经过计算处理成两个列表，及新增要素列表和删除要素列表，在事务提交时同时写历史层。第四、版本的合并，清除等操作均可以归结为对历史图层的新建时间和删除时间两个字段的操作。 第五，版本的标识可以建立一个普通table保存，更加人性化。







posted on2005-03-23 14:28[西西吹雪](https://www.cnblogs.com/watsonyin/)阅读(...) 评论(...)[编辑](https://i.cnblogs.com/EditPosts.aspx?postid=124116)[收藏](#)


[刷新评论](javascript:void(0);)[刷新页面](#)[返回顶部](#top)






Powered by:
博客园
Copyright © 西西吹雪
