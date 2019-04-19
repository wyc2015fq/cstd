# Tab不同子对话框间函数互相调用 - xqhrs232的专栏 - CSDN博客
2019年01月29日 18:09:45[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：54
个人分类：[C/C++/VC++/VS](https://blog.csdn.net/xqhrs232/article/category/906934)
原文地址::[https://blog.csdn.net/phenixyf/article/details/50378712](https://blog.csdn.net/phenixyf/article/details/50378712)
相关文章
1、MFC tab控件不同页面间的变量传递和函数调用----[https://blog.csdn.net/qyl_qiyalu/article/details/48732651](https://blog.csdn.net/qyl_qiyalu/article/details/48732651)
主对话框类为CMyDialog，Tab控件的两个子对话框类分别为CList1和CList2。
CList1中有成员函数GetChipaddress(); 在CList2中需要调用CList1的GetChipaddress函数。
第一步：按Tab使用加载两个子对话框，在CMyDialog中添加两个子对话框成员变量
class CMyDialog: public CdialogEx
{
....
public:
CList1 m_List1;
CList2 m_List2;
...
}
第二步：在CList2.cpp文件中添加CMyDialog的头文件，#include "MyDialog.h"
第三步：在CList2需要位置用如下语句调用CList1的GetChipaddress函数
CMyDialog * pWnd = (CMyDialog*)(GetParent()) -> GetParent();
pWnd -> m_List1.GetChipaddress();
注：
第一个GetParent()获取的是tab control指针，再通过tab control获取主对话框指针；
得到主对话框的指针后，再通过主对话框指针调用子对话框变量，进而调用子对话框成员函数。
