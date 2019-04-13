
# DrawText函数 -  芝麻软件工作室的专栏 - CSDN博客


2012年05月08日 10:48:27[seven-soft](https://me.csdn.net/softn)阅读数：303标签：[c																](https://so.csdn.net/so/search/s.do?q=c&t=blog)个人分类：[WINDOWS API																](https://blog.csdn.net/softn/article/category/1130113)



DrawText函数与前面介绍的TextOut函数都是文本输出函数，但它们是有区别的。DrawText函数是格式化输出函数，而TextOut函数不具备这样的功能。因而DrawText函数比TextOut函数功能强大，可以让文本输出时左对齐，或者右对齐，或者中间对齐，还可以让文本适应输出矩形内，如果超出时可以截断，或者显示为省略号的方式。DrawText函数在表格方式显示时肯定要使用到的函数。
函数DrawText声明如下：
WINUSERAPI
int
WINAPI
DrawTextA(
__in HDC hdc,
__inout_ecount(cchText) LPCSTR lpchText,
__in int cchText,
__inout LPRECT lprc,
__in UINT format);
WINUSERAPI
int
WINAPI
DrawTextW(
__in HDC hdc,
__inout_ecount(cchText) LPCWSTR lpchText,
__in int cchText,
__inout LPRECT lprc,
__in UINT format);
\#ifdef UNICODE
\#define DrawText DrawTextW
\#else
\#define DrawText DrawTextA
\#endif // !UNICODE
**hdc**是当前设备的句柄。
**lpchText**是输出文本的缓冲区首地址。
**cchText**是输出文本的字符个数。
**lprc**是输出的显示区域。
**format**是用什么格式输出。

调用这个函数的例子如下：
\#001 //
\#002 //界面显示输出.
\#003 //
\#004 //
\#005 //
\#006 void CCaiWinMsg::OnDraw(HDC hDC)
\#007 {
\#008  //
\#009  std::wstring strShow(_T("C++窗口类的实现,2007-08-27"));
\#010  TextOut(hDC,10,10,strShow.c_str(),(int)strShow.length());
\#011
\#012  //设置输出字符串的颜色.
\#013  COLORREF crOld = SetTextColor(hDC,RGB(255,0,0));
\#014
\#015  RECT rcText;
\#016
\#017  //显示不全.
\#018  rcText.left = 10;
\#019  rcText.top = 30;
\#020  rcText.right = 100;
\#021  rcText.bottom = 50;
\#022
**\#023  DrawText(hDC,strShow.c_str(),(int)strShow.length(),&rcText,**
**\#024        DT_LEFT|DT_SINGLELINE|DT_END_ELLIPSIS);**
**\#025**
\#026  //完全显示,左对齐.
\#027   rcText.left = 10;
\#028  rcText.top = 50;
\#029  rcText.right = 300;
\#030  rcText.bottom = 80;
\#031
\#032  DrawText(hDC,strShow.c_str(),(int)strShow.length(),&rcText,
\#033         DT_LEFT|DT_SINGLELINE|DT_END_ELLIPSIS);
\#034
\#035
\#036  SetTextColor(hDC,RGB(0,0,255));
\#037  //完全显示,右对齐.
\#038  rcText.left = 10;
\#039  rcText.top = 80;
\#040  rcText.right = 300;
\#041  rcText.bottom = 110;
\#042
\#043  strShow = _T("A&bcd");
\#044  DrawText(hDC,strShow.c_str(),(int)strShow.length(),&rcText,
\#045         DT_RIGHT|DT_SINGLELINE|DT_END_ELLIPSIS);
\#046
\#047
\#048  //
\#049  SetTextColor(hDC,crOld);
\#050 }


