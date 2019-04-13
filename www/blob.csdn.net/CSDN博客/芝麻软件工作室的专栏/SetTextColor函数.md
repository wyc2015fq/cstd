
# SetTextColor函数 -  芝麻软件工作室的专栏 - CSDN博客


2012年05月08日 10:49:30[seven-soft](https://me.csdn.net/softn)阅读数：246标签：[winapi																](https://so.csdn.net/so/search/s.do?q=winapi&t=blog)[杀毒软件																](https://so.csdn.net/so/search/s.do?q=杀毒软件&t=blog)[windows																](https://so.csdn.net/so/search/s.do?q=windows&t=blog)[sap																](https://so.csdn.net/so/search/s.do?q=sap&t=blog)[交通																](https://so.csdn.net/so/search/s.do?q=交通&t=blog)[微软																](https://so.csdn.net/so/search/s.do?q=微软&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=交通&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=sap&t=blog)个人分类：[WINDOWS API																](https://blog.csdn.net/softn/article/category/1130113)
[
																								](https://so.csdn.net/so/search/s.do?q=sap&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=windows&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=windows&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=杀毒软件&t=blog)
[
	](https://so.csdn.net/so/search/s.do?q=winapi&t=blog)
世界是多姿多彩的，色彩是不可以缺少的。在软件开发里，不同的字符颜色往往用来区分不同的数据类型，比如严重的警告，就是使用红色，当然这也是跟交通里红绿灯是一样的道理。最根本的原因，还是人类对自然的选择。毕竟人眼对不同的颜色作出了不同的选择。由于红色光波穿透性比较好，所以就形成以红色来警告的信号。说来也怪，人类的身体不管是什么颜色，流着的血一定是红色的。在软件开发里，要把输出字符的颜色符合现在人位的习惯，不要背离它，否则就不是“以人为本”的科学理念，如果开发出不是“以人为本”的软件是没有客户使用的。在Windows里，微软是选择以蓝色为基调的配色方案，深蓝色的XP界面比WIN2000界面，让人感觉到耳目一新的感觉，深深地把年轻人吸引住了。看到国内杀毒软件，很多是选择绿色为基调，其实就是绿色也就是代表了安全的选择。可见，在软件领域里颜色是非常重要的，面对不同的客户要选择不同的颜色。比如SAP软件，选择是黄色为基调，比较有特色。
函数SetTextColor声明如下：
WINGDIAPI COLORREF WINAPI SetTextColor(__in HDC hdc, __in COLORREF color);
**hdc**是当前设备的句柄。
**color**是设置当前设备字符输出颜色。
调用这个函数的例子如下：
\#001 //
\#002 //界面显示输出.
\#003 //
\#004 //
\#005 //
\#006 void CCaiWinMsg::OnDraw(HDC hDC)
\#007 {
\#008  //
\#009  std::wstring strShow(_T("C++窗口类的实现,2007-08-13"));
\#010  TextOut(hDC,10,10,strShow.c_str(),(int)strShow.length());
\#011
\#012  //设置输出字符串的颜色.
\#013  COLORREF crOld = SetTextColor(hDC,RGB(255,0,0));
\#014  TextOut(hDC,10,30,strShow.c_str(),(int)strShow.length());
\#015
\#016  SetTextColor(hDC,RGB(0,255,0));
\#017  TextOut(hDC,10,50,strShow.c_str(),(int)strShow.length());
\#018
\#019  SetTextColor(hDC,RGB(0,0,255));
\#020  TextOut(hDC,10,70,strShow.c_str(),(int)strShow.length());
\#021
\#022  //
\#023  SetTextColor(hDC,crOld);
\#024 }
第13行设置字符的颜色为红色。
第16行设置字符的颜色为绿色。
第19行设置字符的颜色为蓝色。
第23行恢复原来的颜色，这个一定要记得做，否则后面显示会出错。
![](https://p-blog.csdn.net/images/p_blog_csdn_net/caimouse/71665/o_%E5%AD%97%E4%BD%93%E9%A2%9C%E8%89%B2.JPG)


