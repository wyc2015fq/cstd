
# GetTextMetrics函数 -  芝麻软件工作室的专栏 - CSDN博客


2012年05月08日 10:41:34[seven-soft](https://me.csdn.net/softn)阅读数：233标签：[winapi																](https://so.csdn.net/so/search/s.do?q=winapi&t=blog)[byte																](https://so.csdn.net/so/search/s.do?q=byte&t=blog)[struct																](https://so.csdn.net/so/search/s.do?q=struct&t=blog)[报表																](https://so.csdn.net/so/search/s.do?q=报表&t=blog)[c																](https://so.csdn.net/so/search/s.do?q=c&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=报表&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=struct&t=blog)个人分类：[WINDOWS API																](https://blog.csdn.net/softn/article/category/1130113)
[
																								](https://so.csdn.net/so/search/s.do?q=struct&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=byte&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=byte&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=winapi&t=blog)

在做报表里，经常要把输出的内容进行错落有致的排列，让用户看起来更加舒服。比如使用标题的字体输出后，再使用其小号的字体进行输出。这样就需要知道每种字体的高度，才让两行文字输出不重叠在一起，就需要计算每种字体的高度。这时就需使用GetTextMetrics函数来获取字体的高度。
函数GetTextMetrics声明如下：
\#ifndef NOTEXTMETRIC
WINGDIAPI BOOL WINAPI GetTextMetricsA( __in HDC hdc, __out LPTEXTMETRICA lptm);
WINGDIAPI BOOL WINAPI GetTextMetricsW( __in HDC hdc, __out LPTEXTMETRICW lptm);
\#ifdef UNICODE
\#define GetTextMetrics GetTextMetricsW
\#else
\#define GetTextMetrics GetTextMetricsA
\#endif // !UNICODE
**hdc**是当前设备的句柄。
**lptm**是获取当前字体属性的保存结构。它的结构定义如下：
typedef struct tagTEXTMETRICW
{
LONG tmHeight;
LONG tmAscent;
LONG tmDescent;
LONG tmInternalLeading;
LONG tmExternalLeading;
LONG tmAveCharWidth;
LONG tmMaxCharWidth;
LONG tmWeight;
LONG tmOverhang;
LONG tmDigitizedAspectX;
LONG tmDigitizedAspectY;
WCHAR tmFirstChar;
WCHAR tmLastChar;
WCHAR tmDefaultChar;
WCHAR tmBreakChar;
BYTE tmItalic;
BYTE tmUnderlined;
BYTE tmStruckOut;
BYTE tmPitchAndFamily;
BYTE tmCharSet;
}     TEXTMETRICW;
调用这个函数的例子如下：
\#001 //
\#002 //界面显示输出.
\#003 //
\#004 //
\#005 //
\#006 void CCaiWinMsg::OnDraw(HDC hDC)
\#007 {
\#008  //
\#009  std::wstring strShow(_T("C++窗口类的实现,2007-09-04"));
\#010
\#011  //设置输出字符串的颜色.
\#012  COLORREF crOld = SetTextColor(hDC,RGB(255,0,0));
\#013
\#014  RECT rcText;
\#015
\#016  //设置新字体.
\#017  HGDIOBJ hOldFont = SelectObject(hDC,GetFont());
\#018
\#019  //获取当前字体的高度.
**\#020  TEXTMETRIC tmFont;**
**\#021  if (GetTextMetrics(hDC,&tmFont))**
\#022  {
\#023         rcText.left = 10;
\#024         rcText.top = 40;
\#025         rcText.right = 300;
**\#026        rcText.bottom = rcText.top + tmFont.tmHeight;**
\#027  }
\#028  else
\#029  {
\#030         rcText.left = 10;
\#031         rcText.top = 40;
\#032         rcText.right = 300;
\#033         rcText.bottom = 80;
\#034  }
\#035
\#036  //创建黑色的画刷,
\#037  HBRUSH hbrush = CreateSolidBrush(RGB(0, 0, 0));
\#038
\#039  //用黑色的画刷填充四边形的颜色.
\#040  FillRect(hDC,&rcText,hbrush);
\#041
\#042  //删除画刷.
\#043  DeleteObject(hbrush);
\#044
\#045  //设置透明背景
\#046  int nOldMode = SetBkMode(hDC,TRANSPARENT);
\#047
\#048  //显示字符串在四边形的中间位置.
\#049  DrawText(hDC,strShow.c_str(),(int)strShow.length(),&rcText,
\#050         DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
\#051
\#052  //恢复原来的字体.
\#053  HGDIOBJ hFont = SelectObject(hDC,hOldFont);
\#054  DeleteObject(hFont);
\#055
\#056  //恢复原来的模式.
\#057  SetBkMode(hDC,nOldMode);
\#058
\#059  //恢复原来的颜色.
\#060  SetTextColor(hDC,crOld);
\#061 }
\#062


