# MFC中树控件CTreeCtrl的用法 - L_Andy的专栏 - CSDN博客

2013年10月09日 17:37:41[卡哥](https://me.csdn.net/L_Andy)阅读数：1682


                
树形控件可以用于树形的结构，其中有一个根接点(Root)然后下面有许多子结点，而每个子结点上有允许有一个或多个或没有子结点。MFC中使用CTreeCtrl类来封装树形控件的各种操作。通过调用

BOOL Create( DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID );创建一个窗口，dwStyle中可以使用以下一些树形控件的专用风格：

TVS_HASLINES 在父/子结点之间绘制连线

TVS_LINESATROOT 在根/子结点之间绘制连线

TVS_HASBUTTONS 在每一个结点前添加一个按钮，用于表示当前结点是否已被展开

TVS_EDITLABELS 结点的显示字符可以被编辑

TVS_SHOWSELALWAYS 在失去焦点时也显示当前选中的结点

TVS_DISABLEDRAGDROP 不允许Drag/Drop

TVS_NOTOOLTIPS 不使用ToolTip显示结点的显示字符

在树形控件中每一个结点都有一个句柄（HTREEITEM），同时添加结点时必须提供的参数是该结点的父结点句柄，（其中根Root结点只有一个，既不可以添加也不可以删除）利用

HTREEITEM InsertItem( LPCTSTR lpszItem, HTREEITEM hParent = TVI_ROOT, HTREEITEM hInsertAfter = TVI_LAST );可以添加一个结点，pszItem为显示的字符，hParent代表父结点的句柄，当前添加的结点会排在hInsertAfter表示的结点的后面，返回值为当前创建的结点的句柄。下面的代码会建立一个如下形式的树形结构：

+--- Parent1 +--- Child1_1 +--- Child1_2 +--- Child1_3 +--- Parent2 +--- Parent3

HTREEITEM hItem,hSubItem;

hItem = m_tree.InsertItem("Parent1",TVI_ROOT); 在根结点上添加Parent1

hSubItem = m_tree.InsertItem("Child1_1",hItem); //在Parent1上添加一个子结点

hSubItem = m_tree.InsertItem("Child1_2",hItem,hSubItem); //在Parent1上添加一个子结点，排在Child1_1后面

hSubItem = m_tree.InsertItem("Child1_3",hItem,hSubItem);

hItem = m_tree.InsertItem("Parent2",TVI_ROOT,hItem);

hItem = m_tree.InsertItem("Parent3",TVI_ROOT,hItem);

如果你希望在每个结点前添加一个小图标，就必需先调用CImageList* SetImageList( CImageList * pImageList, int nImageListType );指明当前所使用的ImageList，nImageListType为TVSIL_NORMAL。在调用完成后控件中使用图片以设置的 ImageList中图片为准。然后调用

HTREEITEM InsertItem( LPCTSTR lpszItem, int nImage, int nSelectedImage, HTREEITEM hParent = TVI_ROOT, HTREEITEM hInsertAfter = TVI_LAST);添加结点，nImage为结点没被选中时所使用图片序号，nSelectedImage为结点被选中时所使用图片序号。下面的代码演示了ImageList的设置。

m_list.Create(IDB_TREE,16,4,RGB(0,0,0));

m_tree.SetImageList(&m_list,TVSIL_NORMAL);

m_tree.InsertItem("Parent1",0,1); //添加，选中时显示图标1，未选中时显示图标0

此外CTreeCtrl还提供了一些函数用于得到/修改控件的状态。

HTREEITEM GetSelectedItem( );将返回当前选中的结点的句柄。BOOL SelectItem( HTREEITEM hItem );将选中指明结点。

BOOL GetItemImage( HTREEITEM hItem, int& nImage, int& nSelectedImage ) / BOOL SetItemImage( HTREEITEM hItem, int nImage, int nSelectedImage )用于得到/修改某结点所使用图标索引。

CString GetItemText( HTREEITEM hItem ) /BOOL SetItemText( HTREEITEM hItem, LPCTSTR lpszItem );用于得到/修改某一结点的显示字符。

BOOL DeleteItem( HTREEITEM hItem );用于删除某一结点，BOOL DeleteAllItems( );将删除所有结点。

此外如果想遍历树可以使用下面的函数：

HTREEITEM GetRootItem( );得到根结点。

HTREEITEM GetChildItem( HTREEITEM hItem );得到子结点。

HTREEITEM GetPrevSiblingItem/GetNextSiblingItem( HTREEITEM hItem );得到指明结点的上/下一个兄弟结点。

HTREEITEM GetParentItem( HTREEITEM hItem );得到父结点。

树形控件的消息映射使用ON_NOTIFY宏，形式如同：ON_NOTIFY( wNotifyCode, id, memberFxn )，wNotifyCode为通知代码，id为产生该消息的窗口ID，memberFxn为处理函数，函数的原型如同void OnXXXTree(NMHDR* pNMHDR, LRESULT* pResult)，其中pNMHDR为一数据结构，在具体使用时需要转换成其他类型的结构。对于树形控件可能取值和对应的数据结构为：

TVN_SELCHANGED 在所选中的结点发生改变后发送，所用结构：NMTREEVIEW

TVN_ITEMEXPANDED 在某结点被展开后发送，所用结构：NMTREEVIEW

TVN_BEGINLABELEDIT 在开始编辑结点字符时发送，所用结构：NMTVDISPINFO

TVN_ENDLABELEDIT 在结束编辑结点字符时发送，所用结构：NMTVDISPINFO

TVN_GETDISPINFO 在需要得到某结点信息时发送，（如得到结点的显示字符）所用结构：NMTVDISPINFO

关于ON_NOTIFY有很多内容，将在以后的内容中进行详细讲解。

关于动态提供结点所显示的字符：首先你在添加结点时需要指明lpszItem参数为：LPSTR_TEXTCALLBACK。在控件显示该结点时会通过发送 TVN_GETDISPINFO来取得所需要的字符，在处理该消息时先将参数pNMHDR转换为LPNMTVDISPINFO，然后填充其中 item.pszText。但是我们通过什么来知道该结点所对应的信息呢，我的做法是在添加结点后设置其lParam参数，然后在提供信息时利用该参数来查找所对应的信息。下面的代码说明了这种方法：

char szOut[8][3]={"No.1","No.2","No.3"}; //添加结点

HTREEITEM hItem = m_tree.InsertItem(LPSTR_TEXTCALLBACK,...);

m_tree.SetItemData(hItem, 0 );  //SetItemData的功能是把相应的条目与一个数据相关联

hItem = m_tree.InsertItem(LPSTR_TEXTCALLBACK,...)

m_tree.SetItemData(hItem, 1 ); //处理消息 void

CParentWnd::OnGetDispInfoTree(NMHDR* pNMHDR, LRESULT* pResult) { TV_DISPINFO* pTVDI = (TV_DISPINFO*)pNMHDR;

pTVDI->item.pszText=szOut[pTVDI->item.lParam]; //通过lParam得到需要显示的字符在数组中的位置 *pResult = 0; }

关于编辑结点的显示字符：首先需要设置树形控件的TVS_EDITLABELS风格，在开始编辑时该控件将会发送TVN_BEGINLABELEDIT，你可以通过在处理函数中返回TRUE来取消接下来的编辑，在编辑完成后会发送TVN_ENDLABELEDIT，在处理该消息时需要将参数pNMHDR转换为 LPNMTVDISPINFO，然后通过其中的item.pszText得到编辑后的字符，并重置显示字符。如果编辑在中途中取消该变量为NULL。下面的代码说明如何处理这些消息：

//处理消息 TVN_BEGINLABELEDIT void CParentWnd::OnBeginEditTree(NMHDR* pNMHDR, LRESULT* pResult) { TV_DISPINFO* pTVDI = (TV_DISPINFO*)pNMHDR; if(pTVDI->item.lParam==0);//判断是否取消该操作 *pResult = 1; else *pResult = 0; } //处理消息 TVN_BEGINLABELEDIT void CParentWnd::OnBeginEditTree(NMHDR*
 pNMHDR, LRESULT* pResult) { TV_DISPINFO* pTVDI = (TV_DISPINFO*)pNMHDR; if(pTVDI->item.pszText==NULL);//判断是否已经取消取消编辑 m_tree.SetItemText(pTVDI->item.hItem,pTVDI->pszText); //重置显示字符 *pResult = 0; }

上面讲述的方法所进行的消息映射必须在父窗口中进行（同样WM_NOTIFY的所有消息都需要在父窗口中处理）。

1.取得或设定项目的信息.

BOOL CTreeCtrl::GetItem(TV_ITEM* pItem);

BOOL CTreeCtrl::SetItem(TV_ITEM* pItem);

BOOL CTreeCtrl::SetItem(HTREEITEM hItem,UINTnMask,LPCTSTR lpszItem,int Image,int nSelectedImage,UINT nState,UINT nStateMask,LPARAME lParam);

2.取得与设定项目的状态

UINT CTreeCtrl::GetItemState(HTREEITEM hItem,UINT sStateMask)const;

BOOL CTree Ctrl::SetItemState(HTREEITEM hItem,UINT nState,UINT nStateMask);

3.取得与设定项目的图形

BOOL CTreeCtrl::GetItemImage(HTREEITEM hItem,int& nImage,int& nSelectedImage)const;

BOOL CTreeCtrl::SetItemImage(HTREEITEM hItem,int nImage,int nSelectedImage);

4. 取得与设定项目的文本

CString CTreeCtrl::GetItemText(HTREEITEM,hItem)const;

BOOL CTreeCtrl::SetItemText(HTREEITEM hItem,LPCTSTR lpszItem);

5. 查询 CTreeCtrl 中项目的个数

UINT CTreeCtrl::GetCount();

6.查询hItem 的父项目的句柄

HTREEITEM CTreeCtrl::GetparenItem(HTREEITEM hItem);

7.查询hItem是否有子项

BOOL CTreeCtrl::ItemHasChildren(HTREEITEM hItem);

8.取得hItem 第一个子项的句柄

HTREEITEM CTreeCtrl::GetChildItem(HTREEITEM hItem);

9.查询排在hItem前后的兄弟项

HTREEITEM CTreeCtrl::GetPrevSiblingItem(HTREEITEM hItem);

HTREEITEM CTreeCtrl::GetNextSiblingItem(HTREEITEM hItem);

10.取得选中项的句柄 取得根项的句柄

HTREEITEM CTreeCtrl::GetSelectedItem();

HTREEITEM CTreeCtrl::GetRootItem();

下面是树型视的三个结构TVINSERTSTRUCT、TVITEM、NMTREEVIEW

TVINSERTSTRUCT

包含添加新项到树形视控件所使用的信息。这个结构被TVM_INSERTITEM消息使用。这个结构与TV_INSERTSTRUCT结构是一样的，但它已经按当前的命名习惯重命名了。

typedef struct tagTVINSERTSTRUCT {

HTREEITEM hParent;

HTREEITEM hInsertAfter;

#if (_WIN32_IE >= 0x0400)

union

{

TVITEMEX itemex;

TVITEM item;

} DUMMYUNIONNAME;

#else

TVITEM item;

#endif

} TVINSERTSTRUCT, FAR *LPTVINSERTSTRUCT;

成员

hParent

父项的句柄。如果这个成员的值是TVI_ROOT或NULL，这项将被作为树形控件的根插入。

hInsertAfter

插入的新项之后的项的句柄。或是下列值之一： 值意味

TVI_FIRST在列表的开始插入项

TVI_LAST在列表的最后插入项

TVI_ROOT作为一个根项添加

TVI_SORT以字母顺序插入项

itemex

版本4.71。TVITEMEX包含关于项添加的信息。

item

TVITEM包含关于项添加的信息。

需求

  Windows NT/2000：需要Windows NT 3.51或更高版本。

  Windows 95/98：需要Windows 95或更高版本。

  Header：定义在commctrl.h。

TVITEM

指定或接收树形视项的属性。这个结构与TV_ITEM结构一样，但它已经被当前命名协议重新命名了。新的应用程序应该使用这个结构。

typedef struct tagTVITEM{

UINT mask;

HTREEITEM hItem;

UINT state;

UINT stateMask;

LPTSTR pszText;

int cchTextMax;

int iImage;

int iSelectedImage;

int cChildren;

LPARAM lParam;

} TVITEM, FAR *LPTVITEM;

成员

mask

指出其它的结构成员哪些包含有效数据的标记数组。当这个结构被TVM_GETITEM消息使用时，mask成员指出项的属性被取回。这个成员可以是下列值的一个或多个。

TVIF_CHILDRENcChildren成员是有效的。

TVIF_DI_SETITEM树形视控件将保留支持信息并且不重新请求它。当处理TVN_GETDISPINF通知时，这个标记是有效的。

TVIF_HANDLEhItem成员有效。

TVIF_IMAGEiImage成员有效。

TVIF_PARAMlParam成员有效。

TVIF_SELECTEDIMAGEiSelectedImage成员有效。

TVIF_STATEstate和stateMask成员有效。

TVIF_TEXTpszText和cchTextMax成员有效。

hItem

这个函数引用的项。

state

位标记和图像列表索引的设置，指出项的状态。当设置了一个项的状态，stateMask成员指出这个成员的位是有效的。当取加一个项的状态时，这个成员返回stateMask成员指出的位的当前状态。

这个成员的0至7位包含了项的状态标记。关于可能的项状态标记，参见Tree View Control Item States.

覆盖图像覆盖在项的图标图像之上。这个成员的8至11位指定了以1为基准的覆盖图像索引。如果这些位是0，这个项没有覆盖图像。要隔离这些位，使用TVIS_OVERLAYMASK掩码。要在这个成员中设置覆盖图像索引，使用INDEXTOOVERLAYMASK宏。图像列表的覆盖图像是被ImageList_SetOverlayImage函数设置的。

一个状态图像是仅次于指出应用程序定义的状态的项的图标显示的。通过发送TVM_SETIMAGELIST消息来指定一个状态图像列表。要设置一个项的状态图像，在TVITEM结构的stateMask成员中包含TVIS_STATEIMAGEMASK值。结构的state成员的12至15位指定状态图像列表中被绘制图像的索引。

要设置状态图像索引，使用INDEXTOSTATEIMAGEMASK。这个宏把一个索引适当的设置到12至15位上。要指出项没有状态图像，设置索引为0。这意味着在状态图像列表中的图像0不能被作为一个状态图像使用。要隔离state成员的位12至15，使用TVIS_STATEIMAGEMASK掩码。

stateMask

state成员的位是有效的。如果你取回了一个项的状态，设置stateMask成员的位来指出state成员中的这个位被返回。如果你设置了一个项的状态，设置stateMask成员的位来指出state成员的这个位是你想设置的。要设置或取回一个项的覆盖图像的索引，设置TVIS_OVERLAYMASK位。要设置和取回一个项的状态图像索引，设置TVIS_STATEIMAGEMASK位。

pszText

如果这个结构指定了项属性，那么这个成员是指向一个以空字符结束的字符串，包含有项的文本。如果这个成员是值LPSTR_TEXTCALLBACK，那么父窗口为保存名字负责。既然这样，当树形视控件需要显示、保存或编辑项文本时，向父窗口发送TVN_GETDISPINFO通过消息，当项文本改变时，发送TVN_SETDISPINFO通知消息。

如果结构是取回项的属性，这个成员是取回项文本缓冲的地址。

cchTextMax

pszText成员指定缓冲的大小，以字符为单位。如果这个结构被使用来设置项属性，这个成员被忽略。

iImage

当项是在非选择状态中时，是树形控件的图像列表的索引。

如果这个成员是值I_IMAGECALLBACK，父窗口为保存索引负责。既然这样，当树形视控件需要显示这个图像时，向父窗口发送TVN_GETDISPINFO通知消息来获得索引。

iSelectedImage

当项被选择时，是树形控件图像列表的索引。

如果这个成员是值I_IMAGECALLBACK，父窗口为保存索引负责。既然这样，当树形视控件需要显示这个图像时，向父窗口发送TVN_GETDISPINFO通知消息来获得索引。

cChildren

标记指出哪一个项有关联的子项。这个成员可以是下列值之一。

zero这个项没有子项。

one这个项有一个或更多的子项。

I_CHILDRENCALLBACKThe parent window keeps track of whether the item has child items. In this case, when the tree view control needs to display the item, the control sends the parent a TVN_GETDISPINFO notification message
 to determine whether the item has child items.

If the tree view control has the TVS_HASBUTTONS style, it uses this member to determine whether to display the button indicating the presence of child items. You can use this member to force the control to display the button even though the item does not have
 any child items inserted. This allows you to display the button while minimizing the control's memory usage by inserting child items only when the item is visible or expanded.

lParam

与这项相关的32位值。

需要

  Windows NT/2000：需要Windows NT 3.51或更高版本。

  Windows 95/98：需要Windows 95或更高版本。

  Header：定义在commctrl.h。

NMTREEVIEW

包含关于树形视通知消息的信息。这个结构与NM_TREEVIEW结构一样，但它已经用当前的命名规则进行了重命名。

typedef struct tagNMTREEVIEW {

NMHDR hdr; 

UINT action; 

TVITEM itemOld; 

TVITEM itemNew; 

POINT ptDrag; 

} NMTREEVIEW, FAR *LPNMTREEVIEW; 

成员

hdr

NMHDR结构，包含了关于这个通知消息的信息

action

通知指定的动作标记。

itemOld

包含关于旧项状态信息的TVITEM结构。通知消息没有使用它时，这个成员为0。

itemNew

包含关于新项状态信息的TVITEM结构。通知消息没有使用它时，这个成员为0。

ptDrag

包含引起通知消息发送的事件信息的POINT结构。

参见

WM_NOTIFY

