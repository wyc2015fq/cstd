# 【重学C/C++】句柄 - 三少GG - CSDN博客
2011年10月13日 10:49:04[三少GG](https://me.csdn.net/scut1135)阅读数：850
[句     
 柄](http://www.cnblogs.com/barcelong/articles/1263762.html)
出处： 方塘2003.9.22 整理，之所以前面引用了 贾旭滨 先生的这段描述，因为他写的是在精彩！
=== 重庆大学光电工程学院 贾旭滨 对句柄有下面的描述：
  句柄概念在WINDOWS编程中是一个很重要的概念，在许多地方都扮演着重要的角色。但由此而产生的句柄概念也大同小异，比如：<<Microsoft Windows 3 Developer's Workshop>>(Microsoft Press,by Richard Wilton)一书中句柄的概念是：在Windows环境中，句柄是用来标识项目的，这些项目包括：
*.模块(module)
*.任务(task)
*.实例(instance)
*.文件(file)
*.内存块(block of memory)
*.菜单(menu)
*.控制(control)
*.字体(font)
*.资源(resource),包括图标(icon)，光标(cursor)，字符串(string)等
*.GDI对象(GDI object),包括位图(bitmap)，画刷(brush)，元文件（metafile),调色板(palette)，画笔(pen)，区域(region)，以及设备描述表(device context)。
**  WINDOWS程序中并不是用物理地址来标识一个内存块，文件，任务或动态装入模块的，相反的，WINDOWS API给这些项目分配确定的句柄，并将句柄返回给应用程序，然后通过句柄来进行操作。**
  在<<WINDOWS编程短平快>>(南京大学出版社）一书中是这么说的：**句柄是WONDOWS用来标识被应用程序所建立或使用的对象的唯一整数，WINDOWS使用各种各样的句柄标识诸如应用程序实例，窗口，控制，位图，GDI对象等等。**WINDOWS句柄有点象C语言中的文件句柄。
从上面的2个定义中的我们可以看到，句柄是一个标识符，是拿来标识对象或者项目的，它就象我们的姓名一样，每个人都会有一个，不同的人的姓名不一样，但是，也可能有一个名字和你一样的人。从数据类型上来看它只是一个16位的无符号整数。应用程序几乎总是通过调用一个WINDOWS函数来获得一个句柄，之后其他的WINDOWS函数就可以使用该句柄，以引用相应的对象。在WINDOWS编程中会用到大量的句柄，比如：HINSTANCE（实例句柄），HBITMAP（位图句柄），HDC（设备描述表句柄），HICON（图标句柄）等等，这当中还有一个通用的句柄，就是HANDLE，比如下面的语句：
HINSTANCE hInstance；
可以改成：
HANDLE hInstance；
上面的2句语句都是对的。
一个WINDOWS应用程序可以用不同的方法获得一个特定项的句柄。**许多API函数，诸如CreateWindow,GlobalAlloc,OpenFile的返回值都是一个句柄值。**另外，WINDOWS也能通过应用程序的引出函数将一个句柄作为参数传送给应用程序，应用程序一旦获得了一个确定项的句柄，便可在WINDOWS环境下的任何地方对这个句柄进行操作。其实句柄的大量使用已经影响到了每一个WINDOWS的程序设计。
句柄只有当唯一的确定了一个项目的时候，它才开始有意义。句柄对应着项目表中的一项，而只有WINDOWS本身才能直接存取这个表，应用程序只能通过API函数来处理不同的句柄，举个例子来说吧！比如：我们可以为我们的应用程序申请一块内存块，通过调用API函数GlobalAlloc,来返回一个句柄值：
hMem=GlobalAlloc(......);
其实现在hMem的值只是一个索引值，不是物理地址，应用程序还不能直接存取这块内存。这儿还有一个话外题，就是，一般情况下我们在编程的时候，给应用程序分配的内存都是可以移动的或者是可以丢弃的，这样能使有限的内存资源充分利用，所以，在某一个时候我们分配的那块内存的地址是不确定的，因为他是可以移动的，所以得先锁定那块内存块，这儿应用程序需要调用API函数GlobalLock函数来锁定句柄。如下：
lpMem=GlobalLock(hMem);
这样应用程序才能存取这块内存。
我想现在大家已经能对句柄概念有所了解了，我希望我的文章能对大家有所帮助。其实如果你学过SDK编程，那对句柄的概念理解会更好，更深。如果你是直接学VC6的MFC编程的，建议你看一下SDK编程，这会对你大有好处。
=== 有关 句柄或指针的 常用函数
1. 如何获取应用程序的 实例句柄? AfxGetInstanceHandle()
    应用程序的 实例句柄保存在CWinAppIm_hInstance 中,可以这么调用 AfxGetInstancdHandle获得句柄.
      Example: HANDLE hInstance=AfxGetInstanceHandle();
2. 如何通过代码获得应用程序主窗口的 指针? AfxGetMainWnd  GetSafeHwnd() AfxGetAppName()  AfxGetThread
   主窗口的 指针保存在CWinThread::m_pMainWnd中,调用 AfxGetMainWnd实现。
  【例】   AfxGetMainWnd() ->ShowWindow(SW_SHOWMAXMIZED); //使程序最大化.
  【例】此例的主窗口是对话框，下面的代码是在另外一个CFileTreeCtrl 类（子窗）中相关函数实现在主对话框（主窗）中的几个 静态文本框（子窗）中显示路径：
CWnd* m_pCWnd= AfxGetMainWnd(); //得到主窗口指针,通过主窗指针访问其他子窗资源
            //方法一
            m_pCWnd->SetDlgItemText(IDC_STATIC_path,"CWnd* "+m_sCurPath); //在主窗中的子窗（ID：IDC_STATIC_path）中显示字符串
            m_pCWnd->SetDlgItemText(IDC_STATIC_who,"路径显示由FileTreeCtrl类完成：");
            //方法二
            m_pCWnd->SendMessage(STN_CLICKED); //向主窗口发送一个消息,显示任务由主窗完成。
                                               //在主窗的.cpp中有：ON_MESSAGE(STN_CLICKED, OnSTATICpath3)相关描述
           //有的函数必须通过窗口的 句柄 来访问，我们可以使用下面的方法三
           //CWnd::GetSafeHwnd 
           //Returns the window handle for a window
           //HWND GetSafeHwnd( ) const;
           HWND m_hWnd_tree =GetSafeHwnd();//【注】此处得到的只是当前窗口（FileTree类）的句柄
HWND m_hWnd = m_pCWnd->GetSafeHwnd();// 这里才是主窗口的句柄（由主窗指针得到主窗的句柄）
           //BOOL SetWindowText( HWND hWnd, LPCTSTR lpString )
           ::SetWindowText(m_hWnd,"ok2222");//修改主窗口标题
           ::SetDlgItemText(m_hWnd,IDC_STATIC_path2,"HWND: "+m_sCurPath);
  【另】AfxGetThread
      CWinThread* AfxGetThread( );
      Return Value：Pointer to the currently executing thread.
3. 如何在程序中获得其他程序的 图标? AfxGetInstanceHandle()
 
 HINSTANCE AfxGetInstanceHandle( );
   Return Value
An HINSTANCE to the current instance of the application. If called from within a
 DLL linked with the USRDLL version of MFC, an HINSTANCE to the DLL is returned.
   Remarks
This function allows you to retrieve the instance handle of the current application. AfxGetInstanceHandle always
 returns the HINSTANCE of your executable file (.EXE) unless it is called from within a DLL linked
 with the USRDLL version of MFC. In this case, it returns an HINSTANCE to the DLL.
     两种方法:
        (1) SDK函数 SHGetFileInfo 或使用 ExtractIcon获得图标资源的 handle(句柄),
        (2) SDK函数 SHGetFileInfo获得有关文件的 很多信息,如大小图标,属性,类型等.
          Example(1): 在程序窗口左上角显示 NotePad图标.
           void CSampleView: OnDraw(CDC * pDC)
             {
                if( :: SHGetFileInfo(_T("c:\\pwin95\\notepad.exe"),0,
                     &stFileInfo,sizeof(stFileInfo),SHGFI_ICON))
                   {
                     pDC ->DrawIcon(10,10,stFileInfo.hIcon);
                   }
              }
         Example(2):同样功能,Use ExtractIcon Function
          void CSampleView:: OnDraw(CDC *pDC)
              {
                HICON hIcon=:: ExtractIcon(AfxGetInstanceHandle(),_T
                 ("NotePad.exe"),0);
                if (hIcon &&hIcon!=(HICON)-1)
                   pDC->DrawIcon(10,10,hIcon);
              }
   【说明】关于如何得到系统文件的正确路径，象win.ini system32.ini等的路径，各种系统中具体的路径是不一样的。如：
           获得notepad.exe的路径正规上来说用GetWindowsDirectory 函数得到；
           如果是调用 win95下的画笔，应该用访问注册表的方法获得其路径；
           要作成一个比较考究的程序，考虑应该全面点.
   【另】
               
 HINSTANCE AfxGetResourceHandle( );
                Return Value：An HINSTANCE handle where the default resources of the application
 are loaded.
4.   有关取得桌面句柄 GetDesktopWindow()
MSDN中的例子：
// 静态函数CWnd:: GetDesktopWindow 返回桌面窗口的指针。下例说明了MFC
void CFrameWnd::BeginModalState ()
{
  //first count all windows that need to be disabled
  UINT nCount=0;
  HWND hWnd=:: GetWindow (:: GetDesktopWindow (), GW_CHILD);
  while (hWnd!=NULL)
  {
    if (:: IsWindowEnabled (hwnd) &&
        CWnd::FromHandlePermanent (hWnd)!=NULL &&
        AfxIsDescendant (pParent->m_hWnd, hWnd) &&
        :: SendMessage (hWnd, WM_DISABLEMODAL, 0, 0) == 0)
    {
       ++nCount;
    }
    hWnd=:: GetWindow (hWnd, GW_HWNDNEXT);
  }
}
//用户的问题：下面程序取的不是同一程序的句柄,但是GetModuleFileName返回的结果一样请问为什莫 
HWND ChWnd;//子窗口句柄 
HWND hwDesktop=::GetDesktopWindow();//取得桌面句柄 
ChWnd=::GetWindow(hwDesktop,GW_CHILD);//取得桌面子句柄 
CString csTitle,csClass,csTm,mLookstring; 
char szBuffer[255]; 
while(ChWnd!=NULL)//循环取子句柄的同级句柄 
{ 
    if(::IsWindowVisible(ChWnd))//判断是否为可显示窗口 
    { 
         ::GetWindowText(ChWnd,csTitle.GetBuffer(255),254); 
         ::GetClassName(ChWnd,csClass.GetBuffer(255),254); 
         csTitle.ReleaseBuffer();//标题 
         csClass.ReleaseBuffer();//类名 
         csTm.Format("%08X:",ChWnd); 
         if(csTitle=="") 
         { 
               mLookstring=csTm+csClass; 
         }else 
         { 
               mLookstring=csTm+csTitle; 
         } 
         //这里的窗口句柄不是同一个程序吧?（问题所在！）可是为什莫结果一样 
         HINSTANCE hInstance = (HINSTANCE)::GetWindowLong(ChWnd,DWL_USER); 
         ::GetModuleFileName(hInstance, szBuffer, sizeof(szBuffer)); 
         MessageBox(szBuffer,mLookstring); 
    } 
    ChWnd=::GetWindow(ChWnd,GW_HWNDNEXT); 
} 
回答: 
问题在于Win32下GetWindowLong(ChWnd,DWL_USER)总是返回当前程序运行的hInstance，所以你得到的文件名总是一个。所以你要用枚举所有"进程的程序名"来获得程序名。 
=== 再谈句柄于指针的区别
 许多开始学习VC的朋友，最多听说的两个词莫过于指针和句柄了。 
 但是，确经常搞不清他们之间的区别。 
 首先，句柄是一个窗口的标志，也就是所有从CWND类继承下来的，多有句柄这个成员。 
 他能做的，也就是唯一代表一个桌面上的窗口罢了。而指针是一个地址，如果它指向了一个内存中的对象，那么就可以对它进行任意操作了，当然，并不局限于自己的应用程序，你如果能够获得别的应用程序的某个对象的指针，也可以进行操作。然而，如果要获得指针，首先，必须找到那个窗口的句柄，然后用函数FromHandle就可以得到他的指针了。
