
# CreateRectRgn和CombineRgn函数 -  芝麻软件工作室的专栏 - CSDN博客


2012年05月07日 11:04:35[seven-soft](https://me.csdn.net/softn)阅读数：239标签：[winapi																](https://so.csdn.net/so/search/s.do?q=winapi&t=blog)[windows																](https://so.csdn.net/so/search/s.do?q=windows&t=blog)[图形																](https://so.csdn.net/so/search/s.do?q=图形&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=windows&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=winapi&t=blog)个人分类：[WINDOWS API																](https://blog.csdn.net/softn/article/category/1130113)
[
																								](https://so.csdn.net/so/search/s.do?q=winapi&t=blog)


创新是永恒的追求。当大家习惯Windows的界面时，又想自己开发的软件耳目一新的感觉，那么就得要改变窗口的形状，比如心形的窗口，总之是不规则的窗口。这时就需要使用到叫做区域的技术。区域就是把不同的形状的图形进行组合，然后可以填充它，或者在它那里显示。比如MSN左边的TAB按钮，应就是区域的运用就可以实现它了。
函数CreateRectRgn声明如下：
WINGDIAPI HRGN    WINAPI CreateRectRgn( __in int x1, __in int y1, __in int x2, __in int y2);
**x1**和**y1**是区域左上角的X轴和Y轴坐标。
**x2**和**y2**是区域右下角的X轴和Y轴坐标。
**返回值是创建的区域。**
函数CombineRgn声明如下：
WINGDIAPI int     WINAPI CombineRgn( __in_opt HRGN hrgnDst, __in_opt HRGN hrgnSrc1, __in_opt HRGN hrgnSrc2, __in int iMode);
**hrgnDst**是组合的区域。
**hrgnSrc1**是想组合的第一个区域。
**hrgnSrc2**是想组合的第二个区域。
**iMode**是区域的组合方式，比如相与，相或，异或等等。
调用这个函数的例子如下：
\#001 //
\#002 //界面显示输出.
\#003 //
\#004 //
\#005 //
\#006 void CCaiWinMsg::OnDraw(HDC hDC)
\#007 {
\#008  //创建两个方形区域。
\#009  HRGN rgnRect1 = CreateRectRgn(10,10,100,100);
\#010  HRGN rgnRect2 = CreateRectRgn(50,50,200,200);
\#011
\#012  //合并两个区域。
\#013  CombineRgn(rgnRect1,rgnRect1,rgnRect2,RGN_XOR);
\#014
\#015  //创建画刷。
\#016  HBRUSH hbrush = CreateSolidBrush(RGB(0, 0, 0));
\#017
\#018  //填充区域。
\#019  FillRgn(hDC,rgnRect1,hbrush);
\#020
\#021  //删除画刷.
\#022  DeleteObject(hbrush);
\#023
\#024  //删除创建的两个区域.
\#025  DeleteObject(rgnRect1);
\#026  DeleteObject(rgnRect2);
\#027
\#028 }


