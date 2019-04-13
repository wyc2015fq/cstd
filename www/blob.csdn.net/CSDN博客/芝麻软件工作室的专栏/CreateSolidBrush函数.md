
# CreateSolidBrush函数 -  芝麻软件工作室的专栏 - CSDN博客


2012年05月08日 10:46:33[seven-soft](https://me.csdn.net/softn)阅读数：276标签：[windows																](https://so.csdn.net/so/search/s.do?q=windows&t=blog)[api																](https://so.csdn.net/so/search/s.do?q=api&t=blog)[c																](https://so.csdn.net/so/search/s.do?q=c&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=api&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=windows&t=blog)个人分类：[WINDOWS API																](https://blog.csdn.net/softn/article/category/1130113)
[
																								](https://so.csdn.net/so/search/s.do?q=windows&t=blog)


当你看到Windows显示的按钮时，背景颜色是灰色的。当你看到缺省的窗口背景时，它是白色的。当你的老板需要你创建一个黑色背景的按钮时，你会怎么样做呢？其实在Windows里先用API函数CreateSolidBrush创建画刷，然后调用FillRect函数来填充背景。这样来，不管你需要什么样的背景，都随心所欲了吧。现在先来搞懂CreateSolidBrush函数，下次再来练习FillRect。
函数CreateSolidBrush声明如下：
WINGDIAPI HBRUSH WINAPI CreateSolidBrush( __in COLORREF color);
**color**是画刷颜色。
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
\#010
\#011  //设置输出字符串的颜色.
\#012  COLORREF crOld = SetTextColor(hDC,RGB(255,0,0));
\#013
\#014  RECT rcText;
\#015  rcText.left = 10;
\#016  rcText.top = 30;
\#017  rcText.right = 300;
\#018  rcText.bottom = 80;
\#019
\#020  //创建黑色的画刷,
**\#021  HBRUSH hbrush = CreateSolidBrush(RGB(0, 0, 0));**
\#022
\#023  //用黑色的画刷填充四边形的颜色.
\#024  FillRect(hDC,&rcText,hbrush);
\#025
\#026  //删除画刷.
\#027  DeleteObject(hbrush);
\#028
\#029  //显示字符串在四边形的中间位置.
\#030  DrawText(hDC,strShow.c_str(),(int)strShow.length(),&rcText,
\#031         DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
\#032
\#033  //恢复原来的颜色.
\#034  SetTextColor(hDC,crOld);
\#035 }
第21行是创建黑色的画刷。它的效果图如下：
![](https://p-blog.csdn.net/images/p_blog_csdn_net/caimouse/71665/o_Brush.JPG)


