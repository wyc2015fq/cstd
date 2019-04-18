# VC++笔记八 - L_Andy的专栏 - CSDN博客

2013年11月27日 10:08:01[卡哥](https://me.csdn.net/L_Andy)阅读数：828
个人分类：[VC++笔记](https://blog.csdn.net/L_Andy/article/category/1925951)



张明奇(卡哥)

VC中的关于删除一个文件的问题

C语言的话可以定义一个FILE指针,比如如下的代码段

FILE *fp;

fp = fopen( "C:\\temp.jpg", "r" );

如果文件存在的话就可以正常打开,会返回一个非0的文件指针给fp,而如果文件不存在就会返回NULL给fp,所以只要判断fp的值就可以知道文件是否存在了

但是如果要对文件进行删除操作的话不要忘了把刚才打开的文件关掉,不然的话会因为文件正在被使用而不能删除的,按照上面的代码,可以写成如下形式:

FILE *fp;

fp = fopen( "C:\\temp.jpg", "r" );

if( fp )

{

fclose( fp );

DeleteFile("c:\\temp.jpg");

}

这样就可以了

可以问一下么,你用的是什么编译器?代码应该是没有问题的啊,或者你用

system( "del c:\\temp.jpg" );把那个删除文件的语句替换掉试一下,不过这个语句需要有以下头文件引用语句:

#include<stdlib.h>

>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
直接使用标准c库函数remove

返回0表示删除成功，否则失败

原型：

#include <stdio.h>

int remove ( const char *filename );

使用：

int main()

{

if(remove("c:\\xxx.jpg") == 0) {

printf("OK");

} else

puts("error");

}

>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

if(FileExists("c:\windows\aa.dat")) //判断文件是否存在

{ 

}

////////////////////////////////////////////////////
vs2010下release版本调试设置

设置在Release模式下调试的方法：

1.工程项目上右键 -> 属性

2.c++ -> 常规 -〉调试信息格式    选  程序数据库(/Zi)或(/ZI), 注意：如果是库的话，只能(Zi)

3.c++ -> 优化 -〉优化            选  禁止（/Od）

4.连接器 -〉调试 -〉生成调试信息 选  是 （/DEBUG）

//////////////////////////////////////////////////////

如何判断鼠标左键在一个按钮上一直按下？

重载你的ＡＰＰ类的虚函数PreTranslateMessage(...).用一下方式处理.

BOOL CXXXXApp::PreTranslateMessage(MSG* pMsg) 

{

// TODO: Add your specialized code here and/or call the base class

if ( WM_LBUTTONDOWN   == pMsg->message && pMsg->hwnd == /*你的BUTTON的窗口句柄*/)

         {

             // 设置你的BUTTON按下标志. 

         }

if ( WM_LBUTTONUP   == pMsg->message && pMsg->hwnd == /*你的BUTTON的窗口句柄*/)

         {

             //清除你的BUTTON按下标志

         }

return CWinApp::PreTranslateMessage(pMsg);

}

///////////////////////////////////////
OnMouseMove 怎么判断鼠标移动到按钮上的

void CGameClientDlg::OnMouseMove(UINT nFlags, CPoint point) 

{ 

// TODO: 在此添加消息处理程序代码和/或调用默认值 

if(point.x> 777 && point.x <799 && point.y> 1 && point.y <22)//此区域是关闭键 

{ 

m_mb_close.state = BUTTON_STATE_STOP;

} 

CDialog::OnMouseMove(nFlags, point); 

} 

------解决方案--------------------

BOOL CWnd::PreTranslateMessage(MSG* pMsg) 

{ 

// TODO: Add your specialized code here and/or call the base class 

if(pMsg-> hwnd== m_button.m_hWnd &&pMsg-> message==WM_MOUSEMOVE) 

{ 

int pos=0; 

} 

return CWnd::PreTranslateMessage(pMsg); 

} 

------解决方案--------------------

// Get the current mouse position 

CPoint pointMouse; 

GetCursorPos(&pointMouse); 

// Transform into client coordinates 

ScreenToClient(&pointMouse); 

CRect rect; 

yourButtom.GetClientRect(&rect); 

if (rect.PtInRect(point)) 

{ 

// mouse is over button 

}


# 界面处理中怎样设置默认按钮

重载PreTranslateMessage函数，截获回车键的消息，当截获到的时候，触发你的函数。 

  BOOL   CxxxDlg::PreTranslateMessage(MSG*   pMsg)   

  { 

    if(pMsg->message==WM_KEYDOWN   &&   pMsg->wParam==VK_ENTER)   

    {   

      YourButtonFunc()   //你的按钮函数

                       //这样，在你的程序中，按回车，就会执行该函数 

      }   

    return   CDialog::PreTranslateMessage(pMsg);  

  }

一下是例子：

BOOL CBrowsBar::PreTranslateMessage(MSG* pMsg)

{

    // TODO: 在此添加专用代码和/或调用基类

    if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_ESCAPE))

    {

        return FALSE;

    }

    if ((pMsg->message == WM_LBUTTONDOWN) || (pMsg->message == WM_LBUTTONUP) || (pMsg->message == WM_MOUSEMOVE))

    {

        m_pToolTipCtrl.RelayEvent(pMsg);

    }

    if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_RETURN))

    {

        int nControlID = GetFocus()->GetDlgCtrlID();

        if (IDC_EDIT_SEARCH == nControlID)

        {

            OnBnClickedButtonSearch();

        }

    }

 if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_DOWN))

 {

  int nControlID = GetFocus()->GetDlgCtrlID();

  if (IDC_EDIT_COMBO == nControlID)

  {

   m_comboURL.SetFocus();

   m_comboURL.SetCurSel(0);

  }

 }

 if (pMsg->message == WM_MOUSEWHEEL)

 {

  CRect rect;

  ::GetWindowRect(GetDlgItem(IDD_VSEEBROWSBAR_COMBO)->GetSafeHwnd(),&rect);

  rect.bottom += 800;

  CPoint pt1;

  GetCursorPos(&pt1);

  if(rect.PtInRect(pt1)) 

  {

   ::SendMessage(GetDlgItem(IDD_VSEEBROWSBAR_COMBO)->GetSafeHwnd(), WM_MOUSEWHEEL, pMsg->wParam, pMsg->lParam);

  } 

 }

 if (pMsg->message == WM_KEYDOWN)

 {

  int nControlID = GetFocus()->GetDlgCtrlID();

  if (IDC_EDIT_COMBO == nControlID)

  {

   m_acComboURL.SetAutoComp(TRUE);

  }

  //OnKeyDown(pMsg->wParam, HIWORD(pMsg->lParam), LOWORD(pMsg->lParam));

 }

    return CDialog::PreTranslateMessage(pMsg);

}


