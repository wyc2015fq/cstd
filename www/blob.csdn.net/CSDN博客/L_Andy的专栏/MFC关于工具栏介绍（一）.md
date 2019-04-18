# MFC关于工具栏介绍（一） - L_Andy的专栏 - CSDN博客

2012年10月23日 15:28:59[卡哥](https://me.csdn.net/L_Andy)阅读数：860


           张明奇（卡哥）

一个好的软件，界面非常重要，其中的功能要让客户一目了然。而这些可以借助于工具栏，如在Microsoft Word中的工具栏，相关的操作基本上都可以借助工具栏来完成。下面，我就将我知道的关于VC中关于工具栏的一些东西写写，与大家共享，不对的地方请大家帮忙纠正。

    下面我以Visual Studio.net 2003下的单文档/视图的程序为例说明。

    在VS中，实现工具栏的类是CToolBar, 在新建一个单文档/视图程序的时候，在框架类的OnCreate中，系统会自动创建一个工具条。如下：

 if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||

  !m_wndToolBar.LoadToolBar(IDR_MAINFRAME))

 {

  TRACE0("未能创建工具栏\n");

  return -1;      // 未能创建

 }

方法一：

想要修改或者增加工具栏中的内容，一种方法是进入“资源视图”，然后选择"ToolBar"，然后对工具栏中的内容进行修改或者添加，并为它们添加响应消息。这是最简单，有时候也是最直接有效的方法。

方法二：

如果你想创建一个更加符合自己要求的工具条，而不想被资源视图中的ToolBar所限制的话，那么你可以自己写程序来Create ToolBar. 这样的话，你只需要将上面工程自动生成的那段代码注释掉，然后自己写一个函数，这里取名为CreateExToolBar(); 然后用这个函数代替上面这段代码。

然后加入自己特色的工具条代码，当然，先还是要创建一个工具条，如下：

 if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC))

 {

  return -1;      

 }

注意，不要m_wndToolBar.LoadToolBar(IDR_MAINFRAME)，这样，你就可以在新建的工具条中想干什么就干什么。这里，我们不得提到另外一个类，CToolBarCtrl类，至于它和CToolBar之间的关系与区别，随便在网上搜一下就能找到，这里就不多说了。但是用m_wndToolBar.GetToolBarCtrl()可以由CToolBar对象得到它的CToolBarCtrl对象，这样也在CToolBar对象中也能操作CToolBarCtrl。
 比如设置工具条中的每个按钮的长宽，如下：

m_wndToolBar.GetToolBarCtrl().SetButtonWidth(40,80)

按照我的理解，这两个类的功能可以互补。

当然，你还可以利用CToolBar类的其他方法对你的工具条进行设置，如设置每个Button的文本显示：

m_wndToolBar.SetButtonText(1,"系统消息");

此外，关于工具条的其他属性，可以参考

m_wndToolBar.SetButtonInfo函数来进行设置。

方法三：

加入你还想让你的工具条更漂亮一点，可以在每个按钮上显示一些图标之类的，那么你会涉及到另外一个类：CImageList类，由它的名字可以看出，这是一个图像/图标列表类，它的用法如下：

1.创建一个CImageList对象，如下：

CImageList  img;

img.Create(22,22,ILC_COLOR8|ILC_MASK,2,2);

具体参数意思，请自己参考MSDN或者CSDN。

2. 可有可无的，设置图标的背景颜色

img.SetBkColor(::GetSysColor(COLOR_BTNFACE))

3. 装入图像/图标进入列表

img.Add(AfxGetApp->LoadIcon(ID)

4. 在工具条中设置这个图像列表

m_wndToolBar.GetToolBarCtrl().SetImageList(&img)

这里多说两句，有两个函数，SetImageList和SetHotImageList，其中后面这个函数是用来设置当鼠标放在该button上时，会高亮显示的图像列表。而前面这个是当鼠标没有在它上面时显示的图标。

5. 释放图像列表对象

img.Detach()

现在，就可以用在工具栏中用这个图像列表了。如下：

m_wndToolBar.SetButtonInfo(1, ID, TBSTYLE_BUTTON,1);

第一个1是表示工具栏中第一个空间，TBSTYLE_BUTTON表示工具栏中第一个控件是Button.

后面一个1是表示显示图像列表中的第1幅图。

好了，一切OK，运行一下就可以看到漂亮的工具条了。

方法四：

如果你在工具栏中不光有按钮，还有其他的空间，而且工具栏中按照功能进行分组，是不是很酷呢！

如果上面的方法你都掌握了，那么这里再介绍一个类，CReBar类，就可以轻松搞定了。其实，CReBar类有点像CToolBar, 在CReBar中，可以包含几个Band,每个Band中可以包含不同的各式控件。并且各个Band可以自由的调整大小。

它的用法如下：

1. 首先创建一个CReBar对象

CReBar m_wndReBar;

m_wndReBar.Create(this);

2. 将新建的ToolBar或者其他窗口加入到ReBar中

m_wndReBar.AddBar(&m_wndToolBar)

3. 对CReBar的属性进行一个设置

具体代码如下：

//改变一些属性

 REBARBANDINFO rbbi;

 rbbi.cbSize = sizeof(rbbi);  //这个是必须填

 rbbi.fMask = RBBIM_CHILDSIZE | RBBIM_IDEALSIZE | RBBIM_SIZE|RBBIM_BACKGROUND;

 //工具条的宽度

 rbbi.cxMinChild = rectToolBar.Width();

 //高度

 rbbi.cyMinChild = rectToolBar.Height();

 //下面这行代码是为工具条加入背景位图，请注意上rbbi.fMask中RBBIM_BACKGROUND标志

 rbbi.hbmBack = LoadBitmap(::AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_TOOLBARBKMAP));

 rbbi.cx = rbbi.cxIdeal = rectToolBar.Width() * 10;

 //设置属性

 m_wndReBar.GetReBarCtrl().SetBandInfo(0, &rbbi);

具体的这段代码分析，大家可以参考一个MSDN，多解释还不如自己亲身体会一下。

OK，基本就是这样，具体细节大家可以自己继续往下探讨。

方法五：

有看过中央电视台朝闻快报的应该看过电子白板，就是一个工具条在边上，通过这个工具条可以控制读报的一个常用功能。

并且这个工具条是浮动在上面，可以自由移动的。

这里，再介绍一个方法，就是如何做成像上面描述的这种工具条。

步骤1：添加一个CDialogBar派生类 在资源中添加一个对话框，再采用类向导来添加类，找不到CDialogBar作为基类吧，可以先用CDialog作为基类产生一个，

然后把所以的“CDialog”替换为“CDialogBar”，替换完成了。编译一下，^_^有错误吧！！

请看步骤2。 步骤2：解决编译错误并完善该类 其实错误就是构造函数调用基类时有问题，: CDialogBar(/*CDlgBar::IDD, pParent*/)象这样注释掉就可以了，

添加一个类似OnInitDialog的函数，在CDialogBar中是不存在OnInitDialog的消息的,至少我还不知道,因为初始化是在创建后调用的所以我们就重写

virtual BOOL Create(CWnd* pParentWnd,UINT nIDTemplate,UINT nStyle,UINT nID);这个函数。

注意哦用向导添加的Create函数的参数是不对的喔,看上面。下面是实现代码（很简单的）

 BOOL CDlgXXX::Create(CWnd* pParentWnd,UINT nIDTemplate,UINT nStyle,UINT nID)

 { 

// TODO: Add your specialized code here and/or call the base class 

BOOL bRes= CDialogBar::Create(pParentWnd,nIDTemplate,nStyle,nID );

 InitDialogBar();

//在类中添加一个成员函数就可以了

 return bRes; 

} 

BOOL CDlgXXX::InitDialogBar() 

{ 

UpdateData(FALSE);

//这个一定要啊，这样就会有和CDialog一样的数据交换效果了 

return TRUE; 

}

 步骤3：创建和使用 

if (!m_barAttrib.Create(this,IDD_DLG_COM_ATTRIB, CBRS_RIGHT|CBRS_GRIPPER, XXX))

 { 

TRACE0("Failed to create dialogbar\n");

 return -1;

 }

 m_barAttrib.SetWindowText("部件属性");

 XXX是一个资源id手工直接在资源的.h文件中添加一条，不会，这里就不教了 。工具条的显示和隐藏代码如下，自己慢慢理解吧： 

ShowControlBar(&m_barAttrib, (m_barAttrib.GetStyle() & WS_VISIBLE) == 0, FALSE);

上面代码实现后DoDataExchange也是可以用，给控件添加控件就和CDialog一样的方便咯但是还有一个要注意的是就是控件类对象的添加，我试了一下好像不行，窗口句柄好像总是0的，不能使用。还是使用GetDlgItem(IDC_DRIVER_LIST)来取得控件指针吧。

其他方面的心得 利用DoDataExchange来控制自定义的输入格式控制这里就举一个文本框的例子给文本控件添加完变量后就在DoDataExchange会出现如下代码 DDX_Text(pDX, IDC_COM_VAR, m_strVar);

//系统产生的 DDV_MaxChars(pDX, m_strVar,VAR_MAX_LEN);

//加入长度控制后产生的 DDV_FileNameString(pDX, m_strVar);

//自定义的手工添加的实现见下面

 void CXXX::DDV_FileNameString(CDataExchange *pDX, CString m_strFileName) 

{

 CString strError=_T("\\/:*?\"<>|");

 if(m_strFileName.SpanExcluding(strError) != m_strFileName) 

{

 ::AfxMessageBox(_T("文件名中不能包含"+strError+"字符")); 

pDX->Fail();         //关键是这句执行这句后就会抛出异常下面的语句就不执行了 

} 

} 

       还有几个注意点是 1.只有执行了UpdateData()才会调用DoDataExchange函数若中途 执行了pDX->Fail(); UpdateData()就返回FALSE。 2. DDX_Text(pDX, IDC_COM_VAR, m_strVar);//系统产生的 DDV_MaxChars(pDX, m_strVar,VAR_MAX_LEN);//加入长度控制后产生的 DDV_FileNameString(pDX, m_strVar);//自定义的手工添加的实现见下面如上面几句都是对一个控件的内容的控制，他们必须放在一块，且DDX_Text要放在第一句，这样在界面上就可以正确的指出那个控件的内容有问题，控件会被设置焦点并选中全部内容。好了先写这么多了

