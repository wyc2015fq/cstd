# 【局部刷新】GetUpdateRect和GetClipBox 获取无效区域的区别 - xqhrs232的专栏 - CSDN博客
2012年09月19日 16:50:39[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1237
原文地址::[http://blog.sina.com.cn/s/blog_4c0cb1c001018e23.html](http://blog.sina.com.cn/s/blog_4c0cb1c001018e23.html)
相关网帖
1.请问 MFC中 GetUpdateRect（）怎么获得无效矩形？----[http://topic.csdn.net/u/20081211/22/0eac573c-f0e4-46fd-ab2f-4e07c058262c.html?seed=3194964&r=60494669#r_60494669](http://topic.csdn.net/u/20081211/22/0eac573c-f0e4-46fd-ab2f-4e07c058262c.html?seed=3194964&r=60494669#r_60494669)
个人在实际使用中的体会(基于WINCE)，不具有权威性。
1、GetUpdateRect是获得需要刷新的无效区域，在调用了InvalidRect后可以用这个函数来获得无效区域，但是如果在之前调用了BeginPaint/EndPaint则取到的无效区域为空，这也是为什么要在WM_PAINT中调用BeginPaint的原因。在需要做局部刷新的时候这个函数很有用。
2、GetClipBox是获得裁剪区域大小，在很多时候最终效果是和GetUpdateRect相同的。但是有两点不太相同的
1)在InvalidateRect触发WM_PAINT后直接调用GetClipBox取得的可显示区域是和GetUpdateRect取得的无效区域是一样的
2)看GetClipBox的返回值(NULLREGION、SIMPLEREGION、COMPLEXREGION、RGN_ERROR)可以得知，第一种和第二种情况和GetUpdateRect相同(无效区域无或有)，而第三种情况在需要进行复杂不规则图形局部刷新时很有用也更有效率，举个简单的例子：在屏幕左上角和右下角各有一个矩形，在InvalidRect后如果不调用UpdateWindow，则系统会取两个矩形的最大化区域(也就是全屏幕)并触发WM_PAINT，此时GetUpdateRect获得的无效区域也就是全屏无效区域，应用程序是不知道实际刷新区域的；GetClipBox尽管也是返回的全屏区域，但是在创建裁剪区(CreateRectRgnIndirect)并选入(SelectClipRgn)后，应用程序尽管也不知道实际刷新区域但最终的效果只会刷新两个角的矩形。关于这一点是在实际开发中得知的，这种情况下SelectClipRgn返回值是COMPLEXREGION，我猜测有可能在CreateRectRgnIndirect的时候系统内部查询了一下实际无效区域再创建并组合剪裁区(这个猜测不知道是不是对的)，或者SelectClipRgn取得是CreateRectRgnIndirect区域和InvalidateRect区域的交集
补充：根据BitBlt在贴图的时候如果是有效区域则跳过来看，应该是GDI的优化机制，这个与region关系不大。所以提倡在WM_PAINT中用BeginPaint来画图的深层次目的就是为了利用GDI的优化机制进行无效区域管理及局部绘图提高效率，而用GetDC主动画图获得的是全屏无效区域，想局部刷新就必须用户自已来管理无效区域或使用裁剪区域功能。GetUpdateRect和GetUpdateRgn获得的数据应该是系统记录的，在BeginPaint后就被reset了
