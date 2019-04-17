# 孙鑫-MFC笔记九--菜单编程 - 工作笔记 - CSDN博客





2012年05月14日 20:42:48[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：5563








弹出菜单（Pop-up)是不能用来作命令响应的。即下拉等形式。

响应菜单响应命令的顺序是：视类,文档类，框架类，应用程序类。

Windows消息分三类：

标准消息，WM_ (除WM_COMMAND)都是标准消息。从CWnd派生出来的类，都可以接受这类消息。

命令消息，来及菜单，加速键，或者工具栏按钮的消息。WM_COMMAND。从CCmdTarget派生出来的类，都可以接受这类消息。

通告消息，这类消息是由控件产生的消息，这类消息，也是有W_MCOMMAND形式出现。按钮的单击，列表框的选择等均产生此类消息，为的是向其父窗口（通常是对话框）通知事件的发生。

CWnd派生于CCmdTarget类，可以接受任何消息，派生于CCmdTarget类，可以接受通告消息，命令消息，不能接受标准消息。



消息路由：



当点击一个菜单项的时候，最先接受到菜单项消息的是CMainFrame框架类，CMainFrame框架类将会把菜单项消息交给它的子窗口View类，由View类首先进行处理；如果View类检测到没对该菜单项消息做响应，则View类把菜单项消息交由文档类Doc类进行处理；如果Doc类检测到Doc类中也没对该菜单项消息做响应，则Doc类又把该菜单项消息交还给View类，由View类再交还给CMainFrame类处理。如果CMainFrame类查看到CMainFrame类中也没对该消息做响应，则最终交给App类进行处理。



CMenu* GetMenu( ) ;//CWnd::GetMenu得到窗口菜单栏对象指针。

CMenu* GetSubMenu( ) ;//CMenu::GetSubMenu获得指向菜单栏中子菜单对象指针

UINT CheckMenuItem( );//CMenu::CheckMenuItem 给菜单项添加或者移除一个标记

BOOL SetDefaultItem();//CMenu::SetDefaultItem 设置默认菜单项 两种方式，1是索引，2是命令ID

BOOL SetMenuItemBitmaps( );//CMenu::SetMenuItemBitmaps 设置位图标题菜单。

UINT EnableMenuItem();//CMenu::EnableMenuItem使菜单项有效，无效，或变灰。

BOOL SetMenu( CMenu* pMenu );//CWnd::SetMenu在当前窗口上设置新菜单或移除菜单。

HMENU Detach( );//CMenu::Detach Detaches a Windows menu from a CMenu object and returns the handle.

说明：

1）在计算子菜单菜单项的索引的时候，分隔栏符也算索引的。

2）默认菜单项只能有一个。

3）int GetSystemMetrics()获取系统信息度量。可以用它来获取菜单标题的尺寸从而设置位图标题菜单中位图的大小。

4）在MFC中MFC为我们提供了一套命令更新机制，所有菜单项的更新都是由这套机制来完成的。所以要想利用CMenu::EnableMenuItem来自己控制菜单使用或不使用变灰等，必须要在CMainFrame的构造函数中将变量m_bAutoMenuEnable设置为FALSE。



CMenu menu;//定义为局部对象

menu.LoadMenu(IDR_MAINFRAME);

SetMenu(&menu);

menu.Detach();// 这里menu对象作为一个局部对象。使用Detach()从menu对象中分离窗口菜单句柄，从而当menu对象析构的时候窗口菜单资源不随之销毁。

命令更新机制：

菜单项状态的维护是依赖于CN_UPDATE_COMMAND_UI消息，谁捕获CN_UPDATE_COMMAND_UI消息，MFC就在其中创建一个CCmdUI对象。

在后台操作系统发出WM_INITMENUPOPUP消息，然后由MFC的基类如CFrameWnd接管并创建一个CCmdUI对象和第一个菜单项相关联，调用对象成员函数DoUpdate()（注：这个函数在MSDN中没有找到说明）发出CN_UPDATE_COMMAND_UI消息，这条消息带有指向CCmdUI对象的指针。此后同一个CCmdUI对象又设置为与第二个菜单项相关联，这样顺序进行，直到完成所有菜单项。

更新命令UI处理程序仅应用于弹出式菜单项上的项目，不能应用于永久显示的顶级菜单项目。

说明：

1）可以手工或用ClassWizard来给菜单项添加UPDATE_COMMAND_UI消息响应，利用响应函数中传进来的CCmdUI对象指针可完成设置菜单项可使用，不可使用，变灰，设置标记菜单等操作

如果要想让工具栏上的某个图标与菜单项的某个菜单相关联，只需要将图标的ID设置为该菜单项的ID。

工具栏图标的索引记数顺序是：从左至右从0开始，分隔符也算索引号。

 利用向项目中添加VC的POPMENU控件：Project->Add to Project->Components and Controls..

系统增加的内容：A，一个菜单资源；B，在派生View类中增加了OnContextMenu()函数

利用调用TrackPopupMenu函数，手工添加弹出菜单：

1）用资源管理器添加一个菜单资源

2）在鼠标右键消息响应函数中，加载菜单资源，并获得要显示的子菜单指针，并用该指针调用TrackPopupMenu函数便完成任务（但要注意：鼠标响应函数传进来的坐标是客户区坐标，而TrackPopupMenu函数中使用的是屏幕坐标，在调用TrackPopupMenu前要调用ClientToScreen客户区坐标到屏幕坐标的转换）

事例代码：

CMenu menu;

menu.LoadMenu(IDR_MENU1);

CMenu *pPopup=menu.GetSubMenu(0);

ClientToScreen(&point);

pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y,this);

说明：

CWnd::ClientToScreen(..);//将一个坐标点或一个矩形区域坐标转换成屏幕坐标。

CMenu::TrackPopupMenu(..);//在指定位置以指定的方式显示弹出菜单。

CWnd::ScreenToClient(..);

//Converts the screen coordinates of a given point or rectangle on the display to client coordinates.

,当弹出菜单属于框架窗口的时候（可在TrackPopupMenu函数参数中设置），弹出菜单上的消息，在路由的时候，仍然遵循View-DOC-MainFrame-APP的响应顺序。



###  动态添加菜单：

所有的资源对象都有一个数据成员保存了资源的句柄。

CMenu::AppendMenu //在菜单末尾添加一个新的菜单项。

CMenu::CreatePopupMenu //创建一个弹出菜单，并与菜单对象关联。

CMenu::InsertMenu //插入菜单项。

CMenu::GetSubMenu //Retrieves a pointer to a pop-up menu.

CWnd::GetMenu //Retrieves a pointer to the menu for this window.

CMenu::DeleteMenu //Deletes an item from the menu. 



手动给动态菜单项添加响应函数：

在Resource.h中可以添加资源的ID

在头文件中写消息函数原型

在代码文件中添加消息映射和添加消息响应函数

说明：

可以先创建一个临时的菜单项，设置它的ID和动态菜单项的一致，然后对它用向导进行消息响应，然后删除临时菜单。

再在代码文件中把消息映射放到宏外（注意一定要放到宏外面，因为CLASSWIZARD发现菜单删除了，同时要把其宏对里的消息映射也删除掉的）









