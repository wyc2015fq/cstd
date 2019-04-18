# CListCtrl删除选中行和CListCtrl的用法(详) - L_Andy的专栏 - CSDN博客

2012年11月28日 08:25:06[卡哥](https://me.csdn.net/L_Andy)阅读数：10562



### CListCtrl删除选中行

CListCtrl选中行有两种属性情况，如果设置了Single Selection为TRUE，则只能单选，否则为多选(默认)。

**方法一：**int CListCtrl::GetSelectionMark();

返回值：返回选中索引号，否则返回-1

说明：这个函数可以处理单行的情况，但是有一个缺点，它会保留你上次选中的项,所以当你单击空白的地方时,还会返回上次选中的项。因此在使用快捷键实现删除操作时需要注意这个情况，一般可以配合UINT GetItemState(int nItem,

UINT nMask) const;来判断。

单行：int nItem = m_listCtrl.GetSelectionMark();

m_listCtrl.DeleteItem(nItem);

**方法二：**说明：这个可以处理单行和多行(连续)的情况

POSITION pos = m_listCtrl.GetFirstSelectedItemPosition();

if (pos == NULL)

{

TRACE(_T("No items were selected!/n"));

}

else

{

while (pos)

{

int nItem = m_listCtrl.GetNextSelectedItem(pos);

m_listCtrl.DeleteItem(nItem);

// you could do your own processing on nItem here

}

}

**方法三：**说明：可以处理单行和多行的情况，个人一般用这种方法

while(m_listCtrl.GetNextItem(-1,LVNI_ALL | LVNI_SELECTED) != -1)

{

int nItem = m_listCtrl.GetNextItem(-1,LVNI_ALL | LVNI_SELECTED);

m_listCtrl.DeleteItem(nItem);

}

**方法四：**说明：可以处理单行和多行的情况

for(int i = 0; i<m_listCtrl.GetItemCount(); i++)

{

if(m_listCtrl.GetItemState(i, LVNI_ALL | LVNI_SELECTED) == LVIS_SELECTED)

m_listCtrl.DeleteItem(i);

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

1.创建图形列表并和CListCtrl关联：

 m_image_list.Create(IDB_CALLER2, 16, 10, RGB(192,192, 192));

 m_image_list.SetBkColor( GetSysColor( COLOR_WINDOW ) );

 m_caller_list.SetImageList( &m_image_list, LVSIL_SMALL);

2.为报表添加4列：

  char *szColumn[]={"昵称","IP地址","登陆时间","状态"};

  int widths[]={100,98,70,55};

  LV_COLUMN lvc;

  lvc.mask=LVCF_FMT|LVCF_WIDTH|LVCF_TEXT|LVCF_SUBITEM;

  lvc.fmt=LVCFMT_LEFT;

  for(int i=0;i<4;i++) {//插入各列

   lvc.pszText=szColumn[i];

   lvc.cx=widths[i];

   lvc.iSubItem=i;

   m_caller_list.InsertColumn(i,&lvc);

  }

3.为报表添加两项，以附加方式添加：

 char* data[4];

 data[0]="所有人";

 data[1]="0.0.0.0";

 data[3]="在线";

 data[2]=new char;

 CTime now=CTime::GetCurrentTime();

       CString temp = now.Format("%H:%M:%S");

 data[2]=temp.GetBuffer(1);

 LV_ITEM lvi;

 lvi.mask=LVIF_TEXT|LVIF_IMAGE|LVIF_PARAM;

 lvi.iSubItem=0;

 lvi.pszText=(char *)data[0];

 lvi.iImage = 0;

 lvi.iItem=0;

 m_caller_list.InsertItem(&lvi);

 for (int j=0;j<4;j++) m_caller_list.SetItemText(count,j,data[j]);

 count++;

 lvi.iImage = 1;

 lvi.iItem=count;

 m_caller_list.InsertItem(&lvi);

 data[0]="cherami";

 data[1]="127.0.0.1"; 

 for (int n=0;n<4;n++) m_caller_list.SetItemText(count,n,data[n]);

 count++;

4.设置报表的样式

选中一整行：

m_list_ctrl.SetExtendedStyle(m_list_ctrl.GetExtendedStyle()|LVS_EX_FULLROWSELECT); 

绘制表格：

m_list_ctrl.SetExtendedStyle(m_list_ctrl.GetExtendedStyle()|LVS_EX_GRIDLINES); 

带复选框：

m_list_ctrl.SetExtendedStyle(m_list_ctrl.GetExtendedStyle()|LVS_EX_CHECKBOXES); 

自动切换：

m_list_ctrl.SetExtendedStyle(m_list_ctrl.GetExtendedStyle()|LVS_EX_TRACKSELECT); 

选定一行：

设置CListCtrl的Show selection always选项

SetItemState (iIndex, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED) 

选中一个或多个项目时,会发送LVN_ITEMCHANGED消息，可以使用

GetSelectedCount()方法得到被选定的项的数目。

点击列头的消息响应：

ON_NOTIFY(HDN_ITEMCLICKW, 0, ResponseFunc)

消息，需要自己添加 

或者：

ON_NOTIFY(LVN_COLUMNCLICK, ID_yourCtrl,  ResponseFunc)//向导添加

前者后响应，后者先响应

响应函数：

ResponseFunc(NMHDR *pNMHDR, LRESULT *pResult) 

双击CListCtrl中的ITEM的消息是及消息函数：

ON_NOTIFY(NM_DBLCLK, ID_yourCtrl, ResponseFunc) 

单击ITEM的消息响应：

ON_NOTIFY(NM_CLICK, ID_yourCtrl, ResponseFunc)

ResponseFunc(NMHDR *pNMHDR, LRESULT *pResult) 

HDN_ITEMCLICK    就是Header control Notify message for mouse left click on the Header control!

而HDN_ITEMCLICK是当List View中存在一个Header Contrl时，Header Ctrl通知父窗口List View的！

CListCtrl中的Item被选中触发LBN_SELCHANGE(通过WM_COMMAND)消息！

删除CListCtrl中选定的项：

POSITION pos;

int nIndex;

for(; pos= GetFirstSelectedItemPosition();)

{

nIndex = GetNextSelectedItem(pos);

DeleteItem(nIndex);

}

5.在ListCtrl中进行排序

列表[控件](http://www.programbbs.com/com/)（CListCtrl）的顶部有一排按钮，用户可以通过选择不同的列来对记录进行排序。但是 CListCtrl并没有自动排序的功能，我们需要自己添加一个用于排序的回调函数来比较两个数据的大小，此外还需要响应排序按钮被点击的消息。下面讲述一下具体的做法。

CListCtrl提供了用于排序的函数，函数原型为：BOOL CListCtrl::SortItems( PFNLVCOMPARE pfnCompare, DWORD dwData )。其中第一个参数为全局排序函数的地址，第二个参数为用户数据，你可以根据你的需要传递一个数据或是指针。该函数返回-1代表第一项排应在第二项前面，返回1代表第一项排应在第二项后面，返回0代表两项相等。

用于排序的函数原形为：int CALLBACK ListCompare(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)，其中第三个参数为调用者传递的数据（即调用SortItems时的第二个参数dwData）。第一和第二个参数为用于比较的两项的ItemData，你可以通过DWORD CListCtrl::GetItemData( int nItem )/BOOL CListCtrl::SetItemData( int nItem, DWORD dwData
 )来对每一项的ItemData进行存取。在添加项时选用特定的CListCtrl::InsertItem也可以设置该值。由于你在排序时只能通过该值来确定项的位置所以你应该比较明确的确定该值的含义。

最后一点，我们需要知道什么时候需要排序，实现这点可以在父窗口中对LVN_COLUMNCLICK消息进行处理来实现。 

下面我们看一个例子，这个例子是一个派生类，并支持顺序/倒序两种方式排序。为了简单我对全局数据进行排序，而在实际应用中会有多组需要排序的数据，所以需要通过传递参数的方式来告诉派序函数需要对什么数据进行排序。

//全局数据

struct DEMO_DATA

{

 char szName[20];

 int iAge;

}strAllData[5]={{"王某",30},{"张某",40},{"武某",32},{"陈某",20},{"李某",36}};

//CListCtrl派生类定义

class CSortList : public CListCtrl

{

// Construction

public:

 CSortList();

 BOOL m_fAsc;//是否顺序排序

 int m_nSortedCol;//当前排序的列

protected:

 //{{AFX_MSG(CSortList)

 //}}AFX_MSG

...

};

//父窗口中包含该CListCtrl派生类对象

class CSort_in_list_ctrlDlg : public CDialog

{

// Construction

public:

 CSort_in_list_ctrlDlg(CWnd* pParent = NULL); // standard constructor

// Dialog Data

 //{{AFX_DATA(CSort_in_list_ctrlDlg)

 enum { IDD = IDD_SORT_IN_LIST_CTRL_DIALOG };

 CSortList m_listTest;

 //}}AFX_DATA

}

//在父窗口中定义LVN_COLUMNCLICK消息映射

BEGIN_MESSAGE_MAP(CSort_in_list_ctrlDlg, CDialog)

 //{{AFX_MSG_MAP(CSort_in_list_ctrlDlg)

 ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST1, OnColumnclickList1)

 //}}AFX_MSG_MAP

END_MESSAGE_MAP()

//初始化数据

BOOL CSort_in_list_ctrlDlg::OnInitDialog()

{

 CDialog::OnInitDialog();

 //初始化ListCtrl中数据列表

 m_listTest.InsertColumn(0,"姓名");

 m_listTest.InsertColumn(1,"年龄");

 m_listTest.SetColumnWidth(0,80);

 m_listTest.SetColumnWidth(1,80);

 for(int i=0;i<5;i++)

 {

  m_listTest.InsertItem(i,strAllData[i].szName);

  char szAge[10];

  sprintf(szAge,"%d",strAllData[i].iAge);

  m_listTest.SetItemText(i,1,szAge);

  //设置每项的ItemData为数组中数据的索引

  //在排序函数中通过该ItemData来确定数据

  m_listTest.SetItemData(i,i);

 }

 return TRUE;  // return TRUE  unless you set the focus to a control

}

//处理消息

void CSort_in_list_ctrlDlg::OnColumnclickList1(NMHDR* pNMHDR, LRESULT* pResult) 

{

 NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

 //设置排序方式

 if( pNMListView->iSubItem == m_listTest.m_nSortedCol )

  m_listTest.m_fAsc = !m_listTest.m_fAsc;

 else

 {

  m_listTest.m_fAsc = TRUE;

  m_listTest.m_nSortedCol = pNMListView->iSubItem;

 }

 //调用排序函数

 m_listTest.SortItems( ListCompare, (DWORD)&m_listTest );        

 *pResult = 0;

}

//排序函数实现

int CALLBACK ListCompare(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)

{

 //通过传递的参数来得到CSortList对象指针，从而得到排序方式

 CSortList* pV=(CSortList*)lParamSort;

 //通过ItemData来确定数据

 DEMO_DATA* pInfo1=strAllData+lParam1;

 DEMO_DATA* pInfo2=strAllData+lParam2;

 CString szComp1,szComp2;

 int iCompRes;

 switch(pV->m_nSortedCol)

 {

 case(0):

  //以第一列为根据排序

  szComp1=pInfo1->szName;

  szComp2=pInfo2->szName;

  iCompRes=szComp1.Compare(szComp2);

  break;

 case(1):

  //以第二列为根据排序

  if(pInfo1->iAge == pInfo2->iAge)

   iCompRes = 0;

  else

   iCompRes=(pInfo1->iAge < pInfo2->iAge)?-1:1;

  break;

 default:

  ASSERT(0);

  break;

 }

 //根据当前的排序方式进行调整

 if(pV->m_fAsc)

  return iCompRes;

 else

  return iCompRes*-1;

}

排序最快：

CListCtrl::SortItems

Example

// Sort the item in reverse alphabetical order.

static int CALLBACK 

MyCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)

{

  // lParamSort contains a pointer to the list view control.

  // The lParam of an item is just its index.

  CListCtrl* pListCtrl = (CListCtrl*) lParamSort;

  CString    strItem1 = pListCtrl->GetItemText(lParam1, 0);

  CString    strItem2 = pListCtrl->GetItemText(lParam2, 0);

  return strcmp(strItem2, strItem1);

}

void snip_CListCtrl_SortItems()

{

  // The pointer to my list view control.

  extern CListCtrl* pmyListCtrl;

  // Sort the list view items using my callback procedure.

  pmyListCtrl->SortItems(MyCompareProc, (LPARAM) pmyListCtrl);

}

If you don’t want to allow the users to sort the list by clicking on the header, you can use the style LVS_NOSORTHEADER. However, if you do want to allow sorting, you do not specify the LVS_NOSORTHEADER. The control, though, does not sort the items. You have
 to handle the HDN_ITEMCLICK notification from the header control and process it appropriately. In the code below, we have used the sorting function SortTextItems() developed in a previous section. You may choose to sort the items in a different manner.

Step 1: Add two member variables

Add two member variables to the CListCtrl. The first variable to track which column has been sorted on, if any. The second variable to track if the sort is ascending or descending.

        int nSortedCol; 

        BOOL bSortAscending; 

Step 2: Initialize them in the constructor.

Initialize nSortedCol to -1 to indicate that no column has been sorted on. If the list is initially sorted, then this variable should reflect that.

        nSortedCol = -1; 

        bSortAscending = TRUE; 

Step 3: Add entry in message map to handle HDN_ITEMCLICK

Actually you need to add two entries. For HDN_ITEMCLICKA and HDN_ITEMCLICKW. Do not use the class wizard to add the entry. For one, you need to add two entries whereas the class wizard will allow you only one. Secondly, the class wizard uses the wrong macro
 in the entry. It uses ON_NOTIFY_REFLECT() instead of ON_NOTIFY(). Since the HDN_ITEMCLICK is a notification from the header control to the list view control, it is a direct notification and not a reflected one.

ON_NOTIFY(HDN_ITEMCLICKA, 0, OnHeaderClicked) 

ON_NOTIFY(HDN_ITEMCLICKW, 0, OnHeaderClicked)

 Note that we specify the same function for both the notification. Actually the program will receive one or the other and not both. What notification it receives will depend on the OS. The list view control on Windows 95 will send the ANSI version and the control
 on NT will send the UNICODE version. 

Also, note that the second argument is zero. This value filters for the id of the control and we know that header control id is zero.

Step 4: Write the OnHeaderClicked() function

Here’s where you decide what to do when the user clicks on a column header. The expected behaviour is to sort the list based on the values of the items in that column. In this function we have used the SortTextItems() function developed in a previous section.
 If any of the columns displays numeric or date values, then you would have to provide custom sorting for them.

void CMyListCtrl::OnHeaderClicked(NMHDR* pNMHDR, LRESULT* pResult) 

{

        HD_NOTIFY *phdn = (HD_NOTIFY *) pNMHDR;

        if( phdn->iButton == 0 )

        {

                // User clicked on header using left mouse button

                if( phdn->iItem == nSortedCol )

                        bSortAscending = !bSortAscending;

                else

                        bSortAscending = TRUE;

                nSortedCol = phdn->iItem;

                SortTextItems( nSortedCol, bSortAscending );

        }

        *pResult = 0;

}

让CListCtrl的SubItem也具有编辑功能:

要重载一个文本框，然后在LVN_BEGINLABELEDIT时改变文本框位置。

CInEdit m_InEdit;

    if( ( GetStyle() & LVS_TYPEMASK ) == LVS_REPORT && ( m_nEditSubItem != 0 ) )

    {

        HWND    hwndEdit;

        CRect    rtBound;

        CString strText;

        hwndEdit = (HWND)SendMessage( LVM_GETEDITCONTROL );

        GetSubItemRect( pDispInfo->item.iItem, m_nEditSubItem, LVIR_LABEL, rtBound );

        m_InEdit.SubclassWindow( hwndEdit );

        m_InEdit.m_left = rtBound.left;

        strText = GetItemText( pDispInfo->item.iItem, m_nEditSubItem );

        m_InEdit.SetWindowText( strText );

    }

void CInEdit::OnWindowPosChanging(WINDOWPOS FAR* lpwndpos) 

{

    CRect rtClient;

    lpwndpos->x = m_left;  // m_left在LVN_BEGINLABELEDIT中设置

    CEdit::OnWindowPosChanging(lpwndpos);

    // TODO: Add your message handler code here

}

 ////////////////////////////////////////////////////////////////////CListCtrlZ属性/////////////////////////////////////////////////////////

1、CListCtrl控件概述

CListCtrl控件封装了“列表控件”的功能，它显示了一个关于item（有一个图标和一个标签组成）的集合。除了图标和标签外，每一个item还可以在图标和标签右边以列的方式显示其他信息。该控件可用于windows95/98和window nt 3.51及以后的操作系统上运行的程序的设计。

2、视图类别

       该控件可以按四中方式显示它包含的内容，称为“视图”

1）、Icon view

每一个item以一个实际大小（32*32像素）的下面带标签的图标体现。用户可以拖动item到视图窗口的任一地方。

2）、Small icon view

每一格item以一个小的（16*16像素）、右边带标签的图标体现。用户可以拖动item到视图窗口的任一地方。

3）、List view

每一格item以一个小的（16*16像素）、右边带标签的图标体现。Items在列里面且不可以拖动。

4）、Report view

每一个item显示在自己的行中，附加的信息以列的方式显示在该行的右边。最左边的列包含小的图标和标签，后面并排的列显示应用程序安排的子信息。一个列头控件（CHeadCtrl 类）对每一列进行说明。

3、视图风格

CListCtrl当前的style决定了当前视图的情况。除了标准的风格，还支持大量的扩展风格。下面列出style及其说明：

1）标准风格

**LVS_ALIGNLEFT****：**在小图标和图标视图中，表项（items）向左对齐（如果有10个item，从上往下到底部后再另开一列排列）

**LVS_ALIGNMASK**: 决定控件当前靠齐方式。

**LVS_ALIGNTOP**：在小图标和图标视图中，表项（items）向顶端对其（从如果有10个item，从左往右到右边后在另开一杭排列）。

**LVS_AUTOARRANGE**：图标自动保持小图标和图标视图。

**LVS_EDITLABELS**：Item的文本可以嵌入编辑。父窗口处理通知消息： LVN_ENDLABELEDIT

**LVS_ICON**：指定Icon类别的视图。

**LVS_LIST**：指定list类别的视图。

**LVS_NOCOLUMNHEADER**：在report类别的视图中不显示列头，report类别默认是显示列头的。

**LVS_NOLABELWRAP**：在icon视图中，Item的文本在单行显示。Icon视图下可能多行。

**LVS_NOSCROLL**：屏蔽滚动条，所有item必须在客户区内。该风格与list和report视图不兼容。

**LVS_NOSORTHEADER**：列头没有按钮功能。在report视图中，点击列头不执行操作时候（比如排序），可以使用该风格。

**LVS_OWNERDATA**：指定一个虚拟的ListView, 由用户自己管理Item数据。（see List-View Controls Overview）

Version 4.70. This style specifies a virtual list-view control. For more information about this list control style, see List-View Controls Overview.

**LVS_OWNERDRAWFIXED**：允许自画(仅详细资料模式)

The owner window can paint items in report view. The list-view control sends a WM_DRAWITEM message to paint each item; it does not send separate messages for each subitem. The**iItemData**member of the DRAWITEMSTRUCT
 structure contains the item data for the specified list-view item.

**LVS_REPORT**：该风格决定一个report视图，当对ClistCtrl使用该风格时候，第一列总是左对齐。你不能使用LVCFMNT_RIGHT改变列的对齐方式。

**LVS_SHAREIMAGELISTS**：在视图控件销毁时，不删除与之关联的imagelist控件。该风格可以使多个listctrl使用同样的imagelist。

**LVS_SHOWSELALWAYS**：使listctrl控件一直处于选中状态，即使该控件没有焦点。

**LVS_SINGLESEL**：在同一时间只有一个item可以被选择，默认情况下是可以有多个items被选择的。

**LVS_SMALLICON**：该风格指定小图标视图类别。

**LVS_SORTASCENDING**：item索引基于item的文本升序排列。

**LVS_SORTDESCENDING**：item索引基于item的文本降序排列。

**LVS_TYPEMASK**：决定控件当前窗口风格（是否屏蔽设置的 视图类别如：LVS_ICON）。可以确定当前视图的窗口风格。

**LVS_TYPESTYLEMASK**：决定窗口风格，控制item对齐方式和头显示以及其它行为。

修改风格的例子1：

ModifyStyle(LVS_TYPEMASK,LVS_ICON);//图标

ModifyStyle(LVS_TYPEMASK,LVS_SMALLICON);//小图标

ModifyStyle(LVS_TYPEMASK,LVS_LIST);//列表

修改风格的例子2：

    //GWL_STYLE 获得原有风格

    DWORD dwStyle = ::GetWindowLong(m_report.m_hWnd, GWL_STYLE);

    dwStyle &= ~(LVS_TYPEMASK);

    //设置新风格

    SetWindowLong(m_report.m_hWnd, GWL_STYLE,dwStyle|LVS_REPORT|LVS_NOCOLUMNHEADER

        | LVS_NOLABELWRAP| LVS_EDITLABELS); //

**备注：**

对于LVS_SORTASCENDING和LVS_SORTDESCENDING 风格，表项（item）的索引基于表项的文本分别进行升序和降序排列。由于LVS_LIST和LVS_REPORT风格的视图根据索引以相同的方式排列显示，索引这两个类别的视图的排序结果能立即体现给用户。关于LVS_ICON 和LVS_SMALLICON这两个视图不使用索引决定icons的位置，所以它们排序结果不能体现给用户。

你可以使用LVS_TYPEMASK风格从视图列表类别中获得当前列表的类别。

你可以使用LVS_ALIGNMASK风格获得视图列表当前靠齐方式。

你可以使用LVS_TYPESTYLEMASK风格获得对齐方式（LVS_ALIGNTOP 、LVS_ALIGNLEFT）和头显示和行为（LVS_NOCOLUMNHEADER、LVS_NOSORTHEADER）的的风格。

List-View控件的风格被扩展了，要使用这些新风格，可以通过

LVM_SetExtendedListViewStyle消息或者

ListView_SetExtendedListViewStyle 宏和

ListView_SetExtendedListViewStyleEx宏 中的一个。


