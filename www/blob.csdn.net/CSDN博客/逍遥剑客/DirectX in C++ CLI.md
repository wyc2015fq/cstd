# DirectX in C++/CLI - 逍遥剑客 - CSDN博客
2008年11月02日 21:52:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：2981
昨天熟悉了一下怎么使用CLR, 今天就在里面写个3D程序试试
注意, 这个DirectX是Native DirectX, 不是Managed DirectX, 更不是什么XNA-_-
其实很简单嘛....没遇到什么困难就弄出来了
那些控件基本上你想用什么事件/属性找找MSDN就能找出来, 汗~~~真是人性化啊
至于遇到什么问题了的话, 冒似在CLR类嵌入NativeC++的类的时候要用指针
不过这是我的编程习惯.........
因为创建Device时需要一个HWND, 这个可以取Form/Control的Handle属性:
- HWND hWnd = (HWND)this->panelView->Handle.ToPointer()
估计跟N3整合会非常省事, 因为Ptr<>类直接也相当于gc了
图一张:
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/EntryImages/20081102/DirectX_In_CLR.JPG)
滑块控件能控制三角形的转速.....
代码一会儿发上来
