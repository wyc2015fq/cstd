
# SelectObject和DeleteObject函数 -  芝麻软件工作室的专栏 - CSDN博客


2012年05月04日 17:03:41[seven-soft](https://me.csdn.net/softn)阅读数：292标签：[winapi																](https://so.csdn.net/so/search/s.do?q=winapi&t=blog)[windows																](https://so.csdn.net/so/search/s.do?q=windows&t=blog)[c																](https://so.csdn.net/so/search/s.do?q=c&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=windows&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=winapi&t=blog)个人分类：[WINDOWS API																](https://blog.csdn.net/softn/article/category/1130113)
[
																								](https://so.csdn.net/so/search/s.do?q=winapi&t=blog)


Windows显示设备的属性，共有下面几种：位图、画刷、字体、画笔、区域。如果要设置它们到当前设备里，就需要使用SelectObject函数，比如上面介绍的字体设置，就会用到这个函数。当你创建一个位图时，这时Windows就会在内存里分配一块内存空间，用来保存位图的数据。当你创建字体时，也会分配一块内存空间保存字体。如果程序只是分配，而不去删除，就会造成内存使用越来越多，最后导到Windows这幢大楼倒下来。如果你忘记删除它，就造成了内存泄漏。因此，当你创建显示设备资源时，一定要记得删除它们啊，否则运行你的程序越长，就导致系统不稳定。记得使用DeleteObject函数去删除它们，把占用的内存释放回去给系统。
函数SelectObject和DeleteObject声明如下：
WINGDIAPI HGDIOBJ WINAPI SelectObject(__in HDC hdc, __in HGDIOBJ h);
WINGDIAPI BOOL WINAPI DeleteObject( __in HGDIOBJ ho);
**hDC**是当前设备的句柄。
**h****，****ho**是设备对象，其实它就是内存的地址。
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
\#015  rcText.left = 10;
\#016  rcText.top = 10;
\#017  rcText.right = 300;
\#018  rcText.bottom = 80;
\#019
**\#020  //****创建黑色的画刷****,**
**\#021  HBRUSH hbrush = CreateSolidBrush(RGB(0, 0, 0));**
\#022
\#023  //用黑色的画刷填充四边形的颜色.
\#024  FillRect(hDC,&rcText,hbrush);
\#025
**\#026  //****删除画刷****.**
**\#027  DeleteObject(hbrush);**
\#028
\#029
\#030  rcText.left = 10;
\#031  rcText.top = 10;
\#032  rcText.right = 300;
\#033  rcText.bottom = 40;
\#034
\#035  //显示字符串在四边形的中间位置.
\#036  DrawText(hDC,strShow.c_str(),(int)strShow.length(),&rcText,
\#037         DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
\#038
\#039
\#040  rcText.left = 10;
\#041  rcText.top = 40;
\#042  rcText.right = 300;
\#043  rcText.bottom = 80;
\#044  //设置透明背景
\#045  int nOldMode = SetBkMode(hDC,TRANSPARENT);
\#046
\#047  //设置新字体.
\#048  HGDIOBJ hOldFont = SelectObject(hDC,GetFont());
\#049
\#050  //显示字符串在四边形的中间位置.
\#051  DrawText(hDC,strShow.c_str(),(int)strShow.length(),&rcText,
\#052         DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
\#053
\#054  //恢复原来的字体.
**\#055  HGDIOBJ hFont = SelectObject(hDC,hOldFont);**
**\#056  DeleteObject(hFont);**
\#057
\#058  //恢复原来的模式.
\#059  SetBkMode(hDC,nOldMode);
\#060
\#061  //恢复原来的颜色.
\#062  SetTextColor(hDC,crOld);
\#063 }


