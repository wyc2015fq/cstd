
# CreatePen函数 -  芝麻软件工作室的专栏 - CSDN博客


2012年05月08日 10:38:00[seven-soft](https://me.csdn.net/softn)阅读数：377标签：[winapi																](https://so.csdn.net/so/search/s.do?q=winapi&t=blog)[windows																](https://so.csdn.net/so/search/s.do?q=windows&t=blog)[null																](https://so.csdn.net/so/search/s.do?q=null&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=windows&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=winapi&t=blog)个人分类：[WINDOWS API																](https://blog.csdn.net/softn/article/category/1130113)
[
																								](https://so.csdn.net/so/search/s.do?q=winapi&t=blog)


画画是讲究色彩与线条，不同的地方是采用不同的画笔。上面显示直线，都是采用设备缺省的画笔来画直线。现在就来介绍怎么样创建自己的画笔，比如设置画笔的颜色，画笔的大小。像Windows按钮显示为3D的形状，其实就是用两种颜色画笔分别画相应的线，就生成按钮。而创建画笔就需要使用到CreatePen函数。
函数CreatePen声明如下：
WINGDIAPI HPEN    WINAPI CreatePen( __in int iStyle, __in int cWidth, __in COLORREF color);
**iStyle**是画笔的类型，比如是实线，还是虚线等等。
**cWidth**是线的宽度。
**color**是线的颜色。
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
\#014  //修改直线的颜色,粗细.
\#015  HPEN hPen = CreatePen(PS_SOLID, 10, RGB(0, 255, 0));
\#016
\#017  //设置当前设备的画笔.
\#018  HGDIOBJ hOldPen = SelectObject(hDC,hPen);
\#019
\#020  //从(10,10)到(100,100)画一条直线.
\#021  ptLeftTop.x = 100;
\#022  ptLeftTop.y = 100;
\#023  LineTo(hDC,ptLeftTop.x,ptLeftTop.y);
\#024
\#025  //恢复原来的画笔.
\#026  SelectObject(hDC,hOldPen);
\#027
\#028  //删除创建的画笔.
\#029  DeleteObject(hPen);
\#030
\#031 }


