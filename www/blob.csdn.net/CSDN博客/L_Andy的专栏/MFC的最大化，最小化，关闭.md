# MFC的最大化，最小化，关闭 - L_Andy的专栏 - CSDN博客

2013年09月12日 17:10:45[卡哥](https://me.csdn.net/L_Andy)阅读数：7457


最大化、最小化和关闭按钮是窗口中最主要的元素。首先要说明，说他们是按钮其实是不准确的，按钮是一种窗口，而这三个组件根本就不是窗口，而是一个窗口常见的组成部分。出于习惯的原因，这里还是称呼他们为按钮。
这三个按钮是否显示似乎只能在窗口创建是决定。首先，要显示这三个按钮，窗口必须有标题条和系统菜单，没有标题条和系统菜单的窗口是不能显示这三个按钮的，也就是说，要想有这三个按钮，创建窗口时必须有WS_CAPTION和WS_SYSMENU风格。当窗口创建时有风格WS_MAXIMIZEBOX时，窗口显示最大化按钮，当有风格WS_MINIMIZEBOX时，窗口显示最小化按钮，而只要有标题条并且有系统菜单，关闭按钮就必然会显示出来。

对这三个按钮的操作往往都是系统菜单来进行的，这也解释了为什么要显示这三个按钮必须有系统菜单。一般而言，当一个窗口有最大化按钮时，在系统菜单里也会有对应的菜单项，对这个菜单项的操作一般都能反馈到这各按钮中。其中，最大化按钮在系统菜单中的ID为SC_MAXIMIZE，最小化按钮为SC_MINIMIZE，而关闭按钮为SC_CLOSE。当窗口处于最大化状态时，系统菜单SC_MAXIMIZE则变会并被禁止，而此时最大化按钮变为恢复按钮，ID对应为系统菜单中的SC_RESTORE。

变灰和使能

一般从功能上来讲，界面元素有使能(Enabled)和禁止(Disabled)两种状态，当界面元素处于使能状态时，用户可以使用它，否则用户不能使用它。而从外观的颜色角度讲，界面元素有灰色状态(Grayed）和普通状态两种状态，一般对于使能的界面元素，不应该是灰色的，而对于禁止的界面元素，则应该是灰色的。

这三个按钮也是如此。让人奇怪的是，虽然这三个按钮通过系统菜单操作，它的行为似乎并不和普通菜单项不完全一致。使能、禁止、变灰一个菜单项的操作都是通过EnableMenuItem，一般常见的使能一个菜单项ID_MY_MENU的代码为：

       EnableMenuItem(hMenu,ID_MY_MENU,MF_BYCOMMAND|MF_ENABLED);

                          或者用CMenu的成员函数

       pMenu->EnableMenuItem(ID_MY_MENU,MF_BYCOMMAND|MF_ENABLED);

而禁止并同时使它变灰的代码则为：

       EnableMenuItem(hMenu,ID_MY_MENU,MF_BYCOMMAND|MF_GRAYED|MF_DISABLED);

                          或者用CMenu的成员函数

       pMenu->EnableMenuItem(ID_MY_MENU,MF_BYCOMMAND|MF_GRAYED|MF_DISABLED);

注意：虽然从一般的视觉效果上变灰和禁止是一起起作用的，实际上在内部实现上，这两者并没有捆绑在一起，只有同时设置MF_GRAYED|MF_DISABLED，两个功能才同时作用。否则如果只使用MF_GRAYED，则只变灰不禁止，如果只使用MF_DISABLED，则只禁止不变灰。

这段代码对于菜单项总是很有效的，对关闭按钮也有作用，但是对于最大化和最小化按钮，则完全没有作用。对系统菜单应用上述代码后，系统菜单内的对应项的状态已经正确转变，但是窗口这两个按钮的功能和外观都未变化。

为了禁止并变灰两个按钮中的一个，唯一的方法是从系统菜单中删除这个菜单项，代码如下：

         CMenu* pSysMenu = GetSystemMenu(FALSE);

         ASSERT_VALID(pSysMenu);

         if(NULL == pSysMenu)

         {

                return;

          }

          pSysMenu->DeleteMenu(SC_MINIMIZE,MF_BYCOMMAND);

为了使能一个按钮，则需要向系统菜单中添加这个按钮对应的项，代码为：

         CMenu* pSysMenu = GetSystemMenu(FALSE);

         ASSERT_VALID(pSysMenu);

         if(NULL == pSysMenu)

         {

                return;

          }

          pSysMenu->AppendMenu(MF_STRING,SC_MINIMIZE,_T("最小化(&N)"));

消息处理

对于这三个消息的响应，也一样通过系统菜单的途径来处理的。系统菜单中的所有项都是通过消息WM_SYSCOMMAND来处理的，这个消息的wParam包含了需要处理的消息类型，其含义如下表：值含义

SC_CLOSE 关闭按钮

SC_MAXIMIZE 最大化按钮

SC_MINIMIZE 最小化按钮

SC_RESTORE 恢复按钮

这里只列出了和三个按钮的相关说明，其他值的说明请读者阅读MSDN。

为了自己截获这三个按钮的处理，需要处理WM_SYSCOMMAND消息。下面的代码示例就是处理SC_MINIMIZE消息并隐藏对话框到任务栏的常见操作。

void CTdDlg::OnSysCommand(UINT nID, LPARAM lParam)

{

if ((nID & 0xFFF0) == IDM_ABOUTBOX)

{

CAboutDlg dlgAbout;

dlgAbout.DoModal();

}

else if(nID == SC_MINIMIZE)

{

NOTIFYICONDATA nid;

nid.cbSize = sizeof(nid);

nid.hIcon = m_hIcon;

nid.hWnd   = GetSafeHwnd();

GetWindowText(nid.szTip ,sizeof(nid.szTip ));

nid.uCallbackMessage = WM_NOTIFY_ICON_MESSAGE;

nid.uFlags = NIF_MESSAGE|NIF_TIP|NIF_ICON;

nid.uID = 1;

ShowWindow(SW_HIDE);

Shell_NotifyIcon(NIM_ADD,&nid);

}

else

{

   CDialog::OnSysCommand(nID, lParam);

}

}

对于对话框的SC_CLOSE按钮，缺省调用OnCancel，用户也可以仿照上面代码截获并不调用OnCancel

