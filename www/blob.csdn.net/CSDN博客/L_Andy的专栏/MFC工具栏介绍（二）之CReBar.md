# MFC工具栏介绍（二）之CReBar - L_Andy的专栏 - CSDN博客

2012年10月23日 15:32:11[卡哥](https://me.csdn.net/L_Andy)阅读数：1467


MFC工具栏用法总结[点击打开链接](http://blog.csdn.net/l_andy/article/details/12943171)

需求：

1.在工具栏中添加出按钮以外的控件；

2.当工具栏的宽度小于按钮宽度总和时，隐藏部分按钮，并在工具栏的最右侧显示向右的箭头，当点击这个箭头时，弹出一个菜单，菜单由没有显示出来的按钮组成；

3.工具栏按钮显示tip。

实现：

MFC提供了一个类CReBar，这是一个容器类，可以向这个类中添加工具栏，编辑框，下拉列表等等。

1.

//创建工具栏对象

CToolBar* m_pToolBar = new CToolBar();

m_pToolBar->CreateEx(this);

//给工具栏设置图片：

//IDB_BMP_TOOLBAR为图片资源ID，

//22表示每个图片单元的宽度，CImageList通过这个宽度分割图片

//RGB(255,0,255)表示图片的背景掩码色

CImageList img;

img.Create(IDB_BMP_TOOLBAR,22, 0, RGB(255,0,255));

m_pToolBar->GetToolBarCtrl().SetImageList(&img);

img.Detach();

//设置工具栏三个按钮的图片和文本

m_pToolBar->SetButtons(NULL, 3);

m_pToolBar->SetButtonInfo(0, ID_1, TBSTYLE_BUTTON, 0);

m_pToolBar->SetButtonText(0,L"1");

m_pToolBar->SetButtonInfo(1, ID_2, TBSTYLE_BUTTON, 1);

m_pToolBar->SetButtonText(1,L"1");

m_pToolBar->SetButtonInfo(2, ID_3, TBSTYLE_BUTTON, 2);

m_pToolBar->SetButtonText(2,L"1");

//设置工具栏的属性

m_pToolBar->SetBarStyle(m_pToolBar->GetBarStyle() |

  CBRS_TOOLTIPS | CBRS_FLYBY );

2.

//创建编辑框对象

CEdit* pEdit = new CEdit();

pEdit->Create(WS_CHILD|WS_VISIBLE, CRect(0,0,100,20),this, 0);

3.

//创建CReBar对象

CReBar* m_pReBar = new CChevBar();

m_pReBar->Create(this);

4.

//把工具栏和编辑框绑定到CReBar上

m_pReBar->AddBar(m_pToolBar,NULL,NULL, RBBS_GRIPPERALWAYS|RBBS_FIXEDBMP|RBBS_USECHEVRON);

m_pReBar->AddBar(pEdit,NULL,NULL, RBBS_NOGRIPPER);

5.

//设置工具栏在CReBar中的属性

CRect rectToolBarItem;

m_pToolBar->GetItemRect(0, &rectToolBarItem);

REBARBANDINFO rbbi;

rbbi.cbSize = sizeof(rbbi);

rbbi.fMask = RBBIM_CHILDSIZE | RBBIM_IDEALSIZE | RBBIM_SIZE | RBBIM_ID;

rbbi.cxMinChild = rectToolBarItem.Width();

rbbi.cyMinChild = rectToolBarItem.Height();

rbbi.cx = rbbi.cxIdeal = rectToolBar.Width() * 3 ;

rbbi.wID = 0;

m_pReBar->GetReBarCtrl().SetBandInfo(0, &rbbi);

6.

//最后把工具栏显示出来

m_pReBar->SetWindowPos(NULL, 0, 0 ,260, 32,SWP_SHOWWINDOW);

要使工具栏右边的箭头弹出菜单，还必须响应一个消息

ON_NOTIFY_REFLECT( RBN_CHEVRONPUSHED, OnChevronPushed )

这是一个通知消息，通知父窗口，因此OnChevronPushed消息响应函数由父窗口实现。

消息函数的原型是：

void OnChevronPushed( NMHDR * pNotifyStruct, LRESULT* result )

将NMHDR结构体转换成

NMREBARCHEVRON* pChevron = (NMREBARCHEVRON*) pNotifyStruct;

NMREBARCHEVRON结构体中包含了丰富的信息，通过这个信息创建一个弹出式菜单。

如果工具栏是在资源中创建的，那么设置工具栏每一项的"prompt"属性即可产生tip。（注意：tip字符串应该是A/nB的格式，A用来在任务栏中显示，B用来在TIP窗口显示）

否则，必须响应一个消息

ON_NOTIFY_EX( TTN_NEEDTEXT, 0, SetTipText )

这个消息也有父窗口实现，消息函数的原型是：

bool SetTipText( UINT id, NMHDR * pTTTStruct, LRESULT * pResult )

函数体类似于：

TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pTTTStruct; 

UINT nID =(UINT)pTTTStruct->idFrom; 

switch(nID) 

{

case ID_1:

 pTTT->lpszText=::SysAllocString(L"1");

 break;

case ID_2:

 pTTT->lpszText=::SysAllocString(L"2");

 break;

case ID_3:

 pTTT->lpszText=::SysAllocString(L"3");

 break;

}

pTTT->lpszText就是TIP中要提示的信息。

最后补充一点，工具栏上的按钮右侧可以有一个向下的箭头，当工具栏的按钮属性设置成TBN_DROPDOWN（在SetButtonInfo时候设置），

且工具栏的属性设置成TBSTYLE_EX_DRAWDDARROWS（m_pToolBar->GetToolBarCtrl().SetExtendedStyle(TBSTYLE_EX_DRAWDDARROWS)）的时候会出现。

点击这个向下的箭头会出发消息ON_NOTIFY(TBN_DROPDOWN, AFX_IDW_TOOLBAR, OnDropDown)

OnDropDown函数由父窗口实现，函数原型：

void OnDropDown(NMHDR* pNotifyStruct, LRESULT* pResult)

NMHDR* pNotifyStruct做如下转换：

NMTOOLBAR* pNMToolBar = (NMTOOLBAR*)pNotifyStruct;

NMTOOLBAR同样包含丰富的信息，可以用来创建菜单。

在Visual Studio 2008的安装目录C:/Program Files/Microsoft Visual Studio 9.0/Samples/1033/中有一个压缩包AllVCLanguageSamples.zip，解压缩以后，找到目录C++/MFC/internet/MFCIE/打开这个工程，可以看到一个比较完整的例子。

