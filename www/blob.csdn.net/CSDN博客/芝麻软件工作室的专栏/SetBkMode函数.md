
# SetBkMode函数 -  芝麻软件工作室的专栏 - CSDN博客


2012年05月08日 10:45:37[seven-soft](https://me.csdn.net/softn)阅读数：341标签：[winapi																](https://so.csdn.net/so/search/s.do?q=winapi&t=blog)[c																](https://so.csdn.net/so/search/s.do?q=c&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=winapi&t=blog)个人分类：[WINDOWS API																](https://blog.csdn.net/softn/article/category/1130113)



上面已经介绍输出红色的字符串时，发现背景的黑色也变成白色了，这样的输出是破坏背景的。那需要使用什么方法来保持背景不变，而又能输出红色的字符串呢？比如按钮的文字颜色是黑色的，而背景是灰色的。这就需要使用SetBkMode函数来设置DrawText函数的输出方式，显示设备共有两种输出方式：OPAQUE和TRANSPARENT。OPAQUE的方式是用当前背景的画刷的颜色输出显示文字的背景，而TRANSPARENT是使用透明的输出，也就是文字的背景是不改变的。
函数SetBkMode声明如下：
WINGDIAPI int   WINAPI SetBkMode(__in HDC hdc, __in int mode);
**hDC**是当前设备的句柄。
**mode**是要设置的模式。
函数返回值是前一次设置的模式。
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
\#016  rcText.top = 10;
\#017  rcText.right = 300;
\#018  rcText.bottom = 80;
\#019
\#020  //创建黑色的画刷,
\#021  HBRUSH hbrush = CreateSolidBrush(RGB(0, 0, 0));
\#022
\#023  //用黑色的画刷填充四边形的颜色.
\#024  FillRect(hDC,&rcText,hbrush);
\#025
\#026  //删除画刷.
\#027  DeleteObject(hbrush);
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
**\#045  int nOldMode = SetBkMode(hDC,TRANSPARENT);**
\#046
\#047  //显示字符串在四边形的中间位置.
\#048  DrawText(hDC,strShow.c_str(),(int)strShow.length(),&rcText,
\#049         DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
\#050
\#051  //恢复原来的模式.
**\#052  SetBkMode(hDC,nOldMode);**
\#053
\#054  //恢复原来的颜色.
\#055  SetTextColor(hDC,crOld);
\#056 }
本程序运行的效果图如下：
![](https://p-blog.csdn.net/images/p_blog_csdn_net/caimouse/71665/o_SetBkMode.JPG)
第一次显示是使用OPAQUE的方式显示。
第二次显示是使用TRANSPARENT的方式显示。


