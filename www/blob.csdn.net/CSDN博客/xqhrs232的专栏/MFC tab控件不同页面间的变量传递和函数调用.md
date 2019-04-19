# MFC tab控件不同页面间的变量传递和函数调用 - xqhrs232的专栏 - CSDN博客
2019年01月23日 17:25:09[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：48
个人分类：[C/C++/VC++/VS](https://blog.csdn.net/xqhrs232/article/category/906934)
原文地址::[https://blog.csdn.net/qyl_qiyalu/article/details/48732651](https://blog.csdn.net/qyl_qiyalu/article/details/48732651)
相关文章
1、VC之TAB 控件实例(转)----[https://blog.csdn.net/dehuadeng/article/details/5725183](https://blog.csdn.net/dehuadeng/article/details/5725183)
2、VC++中的Tab Control简单例子----[https://blog.csdn.net/dancewyr/article/details/7672255](https://blog.csdn.net/dancewyr/article/details/7672255)
3、MFC TAB控件的两个子对话框之间的参数传递及调用----[https://blog.csdn.net/hanshuai584044490/article/details/79646540](https://blog.csdn.net/hanshuai584044490/article/details/79646540)
问题描述：MFC程序中tab控价下有三个子页面Page1、Page2、Page3，Page2上有Text控件IDC_EDIT1和成员函数Fun1，现在Page3中想获取Page2上Text控件中输入的值，和调用Page2的成员函数Fun1
方法：
Page3中包含头文件：
#include "Page2.h"
在Page3中获取Page2 Text控件值：
<pre name="code" class="cpp">CTabCtrl *tab=(CTabCtrl*)GetParent();//获取父窗口即tab控件指针
CWnd *cwnd=tab->GetWindow(GW_CHILD);//获取第一个子窗口指针，即Page1
CWnd *cwnd1=cwnd->GetWindow(GW_HWNDNEXT);//获取第一个子窗口后的窗口，即Page2
cwnd1->GetDlgItemText(IDC_EDIT1,str);//获取Page2上Text控件的值
在Page3中调用Page2的成员函数：
CTabCtrl *tab=(CTabCtrl*)GetParent();
CPage2 *page2=(CPage2*)tab->GetWindow(GW_CHILD)->GetWindow(GW_HWNDNEXT);
page2->Fun1();//调用Page2的成员函数
