
# MoveToEx和LineTo函数 -  芝麻软件工作室的专栏 - CSDN博客


2012年05月08日 10:40:10[seven-soft](https://me.csdn.net/softn)阅读数：377标签：[图形																](https://so.csdn.net/so/search/s.do?q=图形&t=blog)[winapi																](https://so.csdn.net/so/search/s.do?q=winapi&t=blog)[null																](https://so.csdn.net/so/search/s.do?q=null&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=winapi&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=图形&t=blog)个人分类：[WINDOWS API																](https://blog.csdn.net/softn/article/category/1130113)
[
																								](https://so.csdn.net/so/search/s.do?q=图形&t=blog)


现在的世界流行图形界面，而不是文字，因此在软件开发里，肯定需要画图的，比如简单地画线，画一些比较特别的图形。比如让你画一个走动的时钟，就需要不断地画秒针、分针等等。MoveToEx是用来移动当前画笔的位置，LineTo是用来画直线的函数，其实在计算机图形里的直线显示是使用光栅图形学里的原理。
函数MoveToEx和LineTo声明如下：
WINGDIAPI BOOL WINAPI MoveToEx( __in HDC hdc, __in int x, __in int y, __out_opt LPPOINT lppt);
**hdc**是当前设备的句柄。
**x**是X轴的位置，水平方向，一般原点是在屏幕左上角的位置。
**y**是Y轴的位置，垂直方向。
**lppt**是移动前的坐标位置。
WINGDIAPI BOOL WINAPI LineTo( __in HDC hdc, __in int x, __in int y);
**hdc**是当前设备的句柄。
**x**是X轴的位置，水平方向，一般原点是在屏幕左上角的位置。
**y**是Y轴的位置，垂直方向。
调用这个函数的例子如下：
\#001 //
\#002 //界面显示输出.
\#003 //
\#004 //
\#005 //
\#006 void CCaiWinMsg::OnDraw(HDC hDC)
\#007 {
\#008  //移到指定位置.
\#009  POINT ptLeftTop;
\#010  ptLeftTop.x = 10;
\#011  ptLeftTop.y = 10;
\#012  MoveToEx(hDC,ptLeftTop.x,ptLeftTop.y,NULL);
\#013
\#014  //从(10，10)到(100，100)画一条直线.
\#015  ptLeftTop.x = 100;
\#016  ptLeftTop.y = 100;
\#017  LineTo(hDC,ptLeftTop.x,ptLeftTop.y);
\#018
\#019 }


