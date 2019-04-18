# MFC多标签页对话框 - wishchinYang的专栏 - CSDN博客
2013年10月29日 14:03:44[wishchin](https://me.csdn.net/wishchin)阅读数：2252
原文链接（有修改）：[http://blog.sina.com.cn/s/blog_6a1cdb3f0101llcw.html](http://blog.sina.com.cn/s/blog_6a1cdb3f0101llcw.html)
1.新建一个MFC工程
取名PageSheet，选择Dialog based，然后Finish。
2.在对话框资源中插入两个对话框IDD_DIALOG1、IDD_DIALOG2，作为两个子页。
3.为这两个对话框增加两个类，分别为CPage1和CPage2，其基类选择CPropertyPage。
4.增加一个属性页类CMySheet, 其基类选择CPropertySheet。 
5.在类CMySheet中增加两个成员变量m_page1，类型为CPage1，m_page2，类型为CPage2。
如：
```cpp
public:
 CPage1 m_page1;
 CPage2 m_page2;
```
6.在类CMySheet的构造函数中添加如下两句，增加两个页。
如：
```cpp
CMySheet::CMySheet()
{
 AddPage(&m_page1);
 AddPage(&m_page2);
}
```
ps：CMySheet类原先的两个带参构造函数，可以删除，改成以上所示无参构造函数。
7.在类CPageSheetApp的初始化函数InitInstance()中，将原来声明的对象去掉。改为属性页类对象声明。
如：
```cpp
//CPageSheetDlg dlg;
  CMySheet dlg;
```
8.添加头文件。
①“PageSheet.cpp”文件中添加头文件“MySheet.h”
②“MySheet.h”中添加头文件“CPage1.h”和“CPage2.h”
9.为属性页类添加初始化函数。
如：
```cpp
BOOL CMySheet::OnInitDialog()
{
 //属性页对话框初始化
 BOOL bResult =CPropertySheet::OnInitDialog();
 //设置窗口标题
 SetWindowText("Page Sheet");
 //去除多余的控件（确定、取消、应用、帮助）
 CWnd *pWnd = GetDlgItem(IDOK);
 pWnd->ShowWindow(SW_HIDE);
 pWnd = GetDlgItem(ID_APPLY_NOW);
 pWnd->ShowWindow(SW_HIDE);
 pWnd = GetDlgItem(IDHELP);
 pWnd->ShowWindow(SW_HIDE);
 pWnd = GetDlgItem(IDCANCEL);
 pWnd->ShowWindow(SW_HIDE);
 RECT  rc; 
 //调整属性页对话框的大小 
 GetWindowRect (&rc); 
 MoveWindow(rc.left,rc.top,rc.right-rc.left,rc.bottom-rc.top-30); //调整窗口 
 return bResult;
}
```
10.可以将多余的对话框、文件删除。
对话框：程序初始对话框IDD_PAGESHEET_DIALOG
文件：CPageSheetDlg.h ，CPageSheetDlg.cpp
