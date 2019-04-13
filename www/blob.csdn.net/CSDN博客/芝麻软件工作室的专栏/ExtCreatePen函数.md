
# ExtCreatePen函数 -  芝麻软件工作室的专栏 - CSDN博客


2012年05月07日 11:10:43[seven-soft](https://me.csdn.net/softn)阅读数：277


使用前面介绍的CreatePen函数来画大于1的直线时，会发现直线两端全是圆角的，有时候需要画得有角，那么这样的函数就不满足需求了，这时就需要使用另一个API函数ExtCreatePen来创建合适的画笔。ExtCreatePen函数可以创建几何画笔，还可以创建装饰用的画笔，装饰的画笔是用来画一些图案使用的，这样就需要快速的算法来实现，显示比几何的画笔在速度上快很多。比如你在画GPS地图时，当需要装饰使用的图案，就可以这种画笔，会明显地提高显示的速度。
函数ExtCreatePen声明如下：
WINGDIAPI HPEN WINAPI ExtCreatePen( __in DWORD iPenStyle,
__in DWORD cWidth,
__in CONST LOGBRUSH *plbrush,
__in DWORD cStyle,
__in_ecount_opt(cStyle) CONST DWORD *pstyle);
**iPenStyle**是画笔的类型。
**cWidth**是画笔的宽度，当创建装饰画笔时宽度一定要设置为1。
**plbrush**是画笔的属性。
**cStyle**是后面自定义样式数组的个数。
**pstyle**是自定义样式数组。
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
\#015  LOGBRUSH lb;
\#016  lb.lbStyle = BS_SOLID;
\#017  lb.lbColor = RGB(0,0,255);
\#018  lb.lbHatch = 0;
\#019
\#020  //创建装饰笔.
\#021  HPEN hPen = ExtCreatePen(PS_COSMETIC | PS_DASH,
\#022         1, &lb, 0, NULL);
\#023
\#024  //设置当前设备的画笔.
\#025  HGDIOBJ hOldPen = SelectObject(hDC,hPen);
\#026
\#027  //从(10,10)到(100,100)画一条直线.
\#028  ptLeftTop.x = 100;
\#029  ptLeftTop.y = 100;
\#030  LineTo(hDC,ptLeftTop.x,ptLeftTop.y);
\#031
\#032  //创建一个端点是平的画笔.
\#033  HPEN hPenGeom = ExtCreatePen(PS_GEOMETRIC | PS_SOLID|
\#034         PS_ENDCAP_FLAT,16, &lb, 0, NULL);
\#035
\#036  SelectObject(hDC,hPenGeom);
\#037  //从(100,100)到(10,100)画一条直线.
\#038  ptLeftTop.x = 10;
\#039  ptLeftTop.y = 100;
\#040  LineTo(hDC,ptLeftTop.x,ptLeftTop.y);
\#041
\#042  //创建一个端点是圆角的画笔.
\#043  HPEN hPenGeomRound = ExtCreatePen(PS_GEOMETRIC | PS_SOLID|
\#044         PS_ENDCAP_ROUND,16, &lb, 0, NULL);
\#045
\#046  SelectObject(hDC,hPenGeomRound);
\#047  //从(10,100)到(10,10)画一条直线.
\#048  ptLeftTop.x = 10;
\#049  ptLeftTop.y = 10;
\#050  LineTo(hDC,ptLeftTop.x,ptLeftTop.y);
\#051
\#052
\#053  //恢复原来的画笔.
\#054  SelectObject(hDC,hOldPen);
\#055
\#056  //删除创建的画笔.
\#057  DeleteObject(hPen);
\#058  DeleteObject(hPenGeom);
\#059  DeleteObject(hPenGeomRound);
\#060
\#061 }


