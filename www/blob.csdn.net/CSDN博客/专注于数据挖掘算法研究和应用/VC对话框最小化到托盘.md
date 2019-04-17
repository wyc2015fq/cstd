# VC对话框最小化到托盘 - 专注于数据挖掘算法研究和应用 - CSDN博客





2010年11月28日 08:40:00[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：3990标签：[delete																[shell																[mfc																[string																[menu																[c](https://so.csdn.net/so/search/s.do?q=c&t=blog)
个人分类：[C/C++](https://blog.csdn.net/fjssharpsword/article/category/6480441)





1.***dlg.h头文件设置：

   1）public:

   void setTray();//设置托盘
   NOTIFYICONDATA nid;//NOTIFYICONDATA结构包含了系统用来处理托盘图标的信息



   2）protected:

   // Generated message map functions
   //{{AFX_MSG(C***Dlg)
   afx_msg LRESULT onShowTask(WPARAM wParam,LPARAM lParam); 

    //}}AFX_MSG



2.***dlg.cpp实现设置：

  1） #define WM_SHOWTASK (WM_USER + 1)  //设置响应消息



  2） BEGIN_MESSAGE_MAP(C***Dlg, CDialog)
   //{{AFX_MSG_MAP(C***Dlg)
  ON_MESSAGE(WM_SHOWTASK,onShowTask)

  //}}AFX_MSG_MAP
  END_MESSAGE_MAP()



  3）最小化菜单响应

    void C***Dlg::OnSysCommand(UINT nID, LPARAM lParam)

   {

         if(nID==SC_MINIMIZE)
        {
             setTray();//最小化到托盘
         }

   }



  4）托盘函数

   void C***Dlg::setTray()
{
 nid.cbSize=(DWORD)sizeof(NOTIFYICONDATA); 
 nid.hWnd=this->m_hWnd; 
 nid.uID=IDR_MAINFRAME; 
 nid.uFlags=NIF_ICON|NIF_MESSAGE|NIF_TIP; 
 nid.uCallbackMessage=WM_SHOWTASK;//自定义的消息名称 
 nid.hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME)); 
 strcpy(nid.szTip,"ClientTracer"); //信息提示条 
 Shell_NotifyIcon(NIM_ADD,&nid); //在托盘区添加图标 
 ShowWindow(SW_HIDE); //隐藏主窗
}



5）消息函数

LRESULT C***Dlg::onShowTask(WPARAM wParam,LPARAM lParam) 
{
 //wParam接收的是图标的ID，而lParam接收的是鼠标的行为 
 if(wParam!=IDR_MAINFRAME) return 1; 
 switch(lParam) 
 { 
  case WM_RBUTTONUP://右键起来时弹出快捷菜单，这里只有一个“关闭” 
  { 
   LPPOINT lpoint=new tagPOINT; 
   ::GetCursorPos(lpoint);//得到鼠标位置 
   CMenu menu; 
   menu.CreatePopupMenu();//声明一个弹出式菜单 
   //增加菜单项“关闭”，点击则发送消息WM_DESTROY给主窗口，将程序结束。 
   menu.AppendMenu(MF_STRING,WM_DESTROY,"退出"); 
   //确定弹出式菜单的位置 
   SetForegroundWindow();//弹出菜单失去焦点后自动消失
   menu.TrackPopupMenu(TPM_LEFTALIGN,lpoint->x,lpoint->y,this); 
   //资源回收 
   HMENU hmenu=menu.Detach(); 
   menu.DestroyMenu(); 
   delete lpoint; 
  } 
  break; 
  case WM_LBUTTONDBLCLK://双击左键的处理 
  { 
   Shell_NotifyIcon(NIM_DELETE,&nid);//删除托盘图标
   ShowWindow(SW_SHOWNORMAL);//显示主窗口
   //this->SetWindowPos(&wndTopMost,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);//使窗口总是在最前面
  } 
  break; 
 } 
 return 0; 
}

6）继承WM_DESTROY：

   void C***Dlg::OnDestroy() 
{
 CDialog::OnDestroy();

 // TODO: Add your message handler code here
 Shell_NotifyIcon(NIM_DELETE,&nid); //退出程序后删除托盘图标
}



3.说明：代码均是添加到现有MFC框架代码！](https://so.csdn.net/so/search/s.do?q=menu&t=blog)](https://so.csdn.net/so/search/s.do?q=string&t=blog)](https://so.csdn.net/so/search/s.do?q=mfc&t=blog)](https://so.csdn.net/so/search/s.do?q=shell&t=blog)](https://so.csdn.net/so/search/s.do?q=delete&t=blog)




