
# GetSysColor函数 -  芝麻软件工作室的专栏 - CSDN博客


2012年05月07日 11:08:56[seven-soft](https://me.csdn.net/softn)阅读数：237标签：[null																](https://so.csdn.net/so/search/s.do?q=null&t=blog)[api																](https://so.csdn.net/so/search/s.do?q=api&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=null&t=blog)个人分类：[WINDOWS API																](https://blog.csdn.net/softn/article/category/1130113)



当你需要自己显示一个与众不同的按钮时，就需要使用下面的方法来创建。当然这里也是综合地使用前面学习过的知识进行一次综合的练习。演示怎么样使用众多的API函数，这里还可以学会使用GetSysColor函数来获取系统的颜色。
WINUSERAPI
DWORD
WINAPI
GetSysColor(
__in int nIndex);
**nIndex**是系统定义的颜色索引值。
调用这个函数的例子如下：
\#001 //
\#002 //界面显示输出.
\#003 //
\#004 //
\#005 //
\#006 void CCaiWinMsg::OnDraw(HDC hDC)
\#007 {
\#008  //显示一个按钮。
\#009  //设置按钮背景颜色。
\#010  RECT rcText;
\#011  rcText.left = 10;
\#012  rcText.top = 10;
\#013  rcText.right = 100;
\#014  rcText.bottom = 100;
\#015
\#016  HBRUSH hbrush = CreateSolidBrush(GetSysColor(COLOR_3DFACE));
\#017  //用黑色的画刷填充四边形的颜色.
\#018  FillRect(hDC,&rcText,hbrush);
\#019  DeleteObject(hbrush);
\#020
\#021  //
\#022  HPEN hLight = CreatePen(PS_SOLID,1,
\#023         GetSysColor(COLOR_3DHIGHLIGHT));
\#024  HPEN hShdaow = CreatePen(PS_SOLID,1,
\#025         GetSysColor(COLOR_3DDKSHADOW));
\#026
\#027  //
\#028  //移到指定位置.
\#029  POINT ptLeftTop;
\#030  ptLeftTop.x = 20;
\#031  ptLeftTop.y = 20;
\#032  MoveToEx(hDC,ptLeftTop.x,ptLeftTop.y,NULL);
\#033
\#034  //显示白线。
\#035  HGDIOBJ hOldPen = SelectObject(hDC,hLight);
\#036  ptLeftTop.x = 20;
\#037  ptLeftTop.y = 90;
\#038  LineTo(hDC,ptLeftTop.x,ptLeftTop.y);
\#039
\#040  ptLeftTop.x = 20;
\#041  ptLeftTop.y = 20;
\#042  MoveToEx(hDC,ptLeftTop.x,ptLeftTop.y,NULL);
\#043  ptLeftTop.x = 90;
\#044  ptLeftTop.y = 20;
\#045  LineTo(hDC,ptLeftTop.x,ptLeftTop.y);
\#046
\#047  //显示黑色线。
\#048  SelectObject(hDC,hShdaow);
\#049  ptLeftTop.x = 90;
\#050  ptLeftTop.y = 90;
\#051  LineTo(hDC,ptLeftTop.x,ptLeftTop.y);
\#052
\#053  ptLeftTop.x = 20;
\#054  ptLeftTop.y = 90;
\#055  LineTo(hDC,ptLeftTop.x,ptLeftTop.y);
\#056
\#057  //
\#058  SelectObject(hDC,hOldPen);
\#059  DeleteObject(hLight);
\#060  DeleteObject(hShdaow);
\#061
\#062  int nOldMode = SetBkMode(hDC,TRANSPARENT);
\#063  //输出文字。
\#064  std::wstring strShow(_T("按钮"));
\#065  DrawText(hDC,strShow.c_str(),(int)strShow.length(),&rcText,
\#066         DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
\#067
\#068  //
\#069  SetBkMode(hDC,nOldMode);
\#070 }


