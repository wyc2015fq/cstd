# [转] MFC CListCtrl 使用介绍  - ljx0305的专栏 - CSDN博客
2010年08月12日 16:24:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：641标签：[mfc																[listview																[report																[imagelist																[数据结构																[报表](https://so.csdn.net/so/search/s.do?q=报表&t=blog)](https://so.csdn.net/so/search/s.do?q=数据结构&t=blog)](https://so.csdn.net/so/search/s.do?q=imagelist&t=blog)](https://so.csdn.net/so/search/s.do?q=report&t=blog)](https://so.csdn.net/so/search/s.do?q=listview&t=blog)](https://so.csdn.net/so/search/s.do?q=mfc&t=blog)
个人分类：[MFC																[VC](https://blog.csdn.net/ljx0305/article/category/401790)](https://blog.csdn.net/ljx0305/article/category/403913)
列表控件可以看作是功能增强的ListBox，它提供了四种风格，而且可以同时显示一列的多中属性值。MFC中使用CListCtrl类来封装列表控件的各种操作。通过调用
BOOL Create( DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID );创建一个窗口，dwStyle中可以使用以下一些列表控件的专用风格： 
- LVS_ICON LVS_SMALLICON LVS_LIST LVS_REPORT 这四种风格决定控件的外观，同时只可以选择其中一种，分别对应：大图标显示，小图标显示，列表显示，详细报表显示 
- LVS_EDITLABELS 结点的显示字符可以被编辑，对于报表风格来讲可编辑的只为第一列。 
- LVS_SHOWSELALWAYS 在失去焦点时也显示当前选中的结点 
- LVS_SINGLESEL 同时只能选中列表中一项 
首先你需要设置列表控件所使用的ImageList，如果你使用大图标显示风格，你就需要以如下形式调用： 
CImageList* SetImageList( CImageList* pImageList, LVSIL_NORMAL); 
如果使用其它三种风格显示而不想显示图标你可以不进行任何设置，否则需要以如下形式调用： 
CImageList* SetImageList( CImageList* pImageList, LVSIL_SMALL); 
通过调用int InsertItem( int nItem, LPCTSTR lpszItem );可以在列表控件中nItem指明位置插入一项，lpszItem为显示字符。除LVS_REPORT风格外其他三种风格都只需要直接调用 InsertItem就可以了，但如果使用报表风格就必须先设置列表控件中的列信息。
通过调用int InsertColumn( int nCol, LPCTSTR lpszColumnHeading, int nFormat , int nWidth, int nSubItem);可以插入列。iCol为列的位置，从零开始，lpszColumnHeading为显示的列名，nFormat为显示对齐方式， nWidth为显示宽度，nSubItem为分配给该列的列索引。 
在有多列的列表控件中就需要为每一项指明其在每一列中的显示字符，通过调用 
BOOL SetItemText( int nItem, int nSubItem, LPTSTR lpszText );可以设置每列的显示字符。nItem为设置的项的位置，nSubItem为列位置，lpszText为显示字符。下面的代码演示了如何设置多列并插入数据： 
m_list.SetImageList(&m_listSmall,LVSIL_SMALL);//设置ImageList
m_list.InsertColumn(0,"Col 1",LVCFMT_LEFT,300,0);//设置列
m_list.InsertColumn(1,"Col 2",LVCFMT_LEFT,300,1);
m_list.InsertColumn(2,"Col 3",LVCFMT_LEFT,300,2);
m_list.InsertItem(0,"Item 1_1");//插入行
m_list.SetItemText(0,1,"Item 1_2");//设置该行的不同列的显示字符
m_list.SetItemText(0,2,"Item 1_3");
此外CListCtrl还提供了一些函数用于得到/修改控件的状态。 
COLORREF GetTextColor( )/BOOL SetTextColor( COLORREF cr );用于得到/设置显示的字符颜色。 
COLORREF GetTextBkColor( )/BOOL SetTextBkColor( COLORREF cr );用于得到/设置显示的背景颜色。 
void SetItemCount( int iCount );用于得到添加进列表中项的数量。 
BOOL DeleteItem(int nItem);用于删除某一项，BOOL DeleteAllItems( );将删除所有项。 
BOOL SetBkImage(HBITMAP hbm, BOOL fTile , int xOffsetPercent, int yOffsetPercent);用于设置背景位图。 
CString GetItemText( int nItem, int nSubItem );用于得到某项的显示字符。 
列表控件的消息映射同样使用ON_NOTIFY宏，形式如同：ON_NOTIFY( wNotifyCode, id, memberFxn )，wNotifyCode为通知代码，id为产生该消息的窗口ID，memberFxn为处理函数，函数的原型如同void OnXXXList(NMHDR* pNMHDR, LRESULT* pResult)，其中pNMHDR为一数据结构，在具体使用时需要转换成其他类型的结构。对于列表控件可能取值和对应的数据结构为： 
- LVN_BEGINLABELEDIT 在开始某项编辑字符时发送，所用结构：NMLVDISPINFO 
- LVN_ENDLABELEDIT 在结束某项编辑字符时发送，所用结构：NMLVDISPINFO 
- LVN_GETDISPINFO 在需要得到某项信息时发送，（如得到某项的显示字符）所用结构：NMLVDISPINFO 
关于ON_NOTIFY有很多内容，将在以后的内容中进行详细讲解。 
**关于动态提供结点所显示的字符：**首先你在项时需要指明lpszItem参数为： LPSTR_TEXTCALLBACK。在控件显示该结点时会通过发送TVN_GETDISPINFO来取得所需要的字符，在处理该消息时先将参数 pNMHDR转换为LPNMLVDISPINFO，然后填充其中item.pszText。通过item中的iItem,iSubItem可以知道当前显示的为那一项。下面的代码演示了这种方法： 
char szOut[8][3]={"No.1","No.2","No.3"};//添加结点
m_list.InsertItem(LPSTR_TEXTCALLBACK,![](http://www.cppblog.com/Images/dot.gif))
m_list.InsertItem(LPSTR_TEXTCALLBACK,![](http://www.cppblog.com/Images/dot.gif))
//处理消息
void CParentWnd::OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult)
{ 
    LV_DISPINFO* pLVDI = (LV_DISPINFO*)pNMHDR; 
    pLVDI->item.pszText=szOut[pTVDI->item.iItem];//通过iItem得到需要显示的字符在数组中的位置 
*pResult =0;
}
**关于编辑某项的显示字符：**（在报表风格中只对第一列有效）首先需要设置列表控件的 LVS_EDITLABELS风格，在开始编辑时该控件将会发送LVN_BEGINLABELEDIT，你可以通过在处理函数中返回TRUE来取消接下来的编辑，在编辑完成后会发送LVN_ENDLABELEDIT，在处理该消息时需要将参数pNMHDR转换为LPNMLVDISPINFO，然后通过其中的item.pszText得到编辑后的字符，并重置显示字符。如果编辑在中途中取消该变量为NULL。下面的代码说明如何处理这些消息： 
//处理消息 LVN_BEGINLABELEDIT
void CParentWnd::OnBeginEditList(NMHDR* pNMHDR, LRESULT* pResult)
{
    LV_DISPINFO* pLVDI = (LV_DISPINFO*)pNMHDR; 
if(pLVDI->item.iItem==0);//判断是否取消该操作
*pResult =1; 
else*pResult =0；
}
//处理消息 LVN_BEGINLABELEDIT
void CParentWnd::OnBeginEditList(NMHDR* pNMHDR, LRESULT* pResult)
{
    LV_DISPINFO* pLVDI = (LV_DISPINFO*)pNMHDR; 
if(pLVDI->item.pszText==NULL);//判断是否已经取消取消编辑  
    m_list.SetItemText(pLVDI->item.iItem,0,pLVDI->pszText);//重置显示字符 
*pResult =0;
}
上面讲述的方法所进行的消息映射必须在父窗口中进行（同样WM_NOTIFY的所有消息都需要在父窗口中处理）。 
**如何得到当前选中项位置：**在列表控件中没有一个类似于ListBox中GetCurSel()的函数，但是可以通过调用GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);得到选中项位置。
下面是一些例子
作者：lixiaosan
时间：04/06/2006
以下未经说明，listctrl默认view 风格为report
相关类及处理函数
MFC：CListCtrl类
SDK：以 “ListView_”开头的一些宏。如 ListView_InsertColumn
--------------------------------------------------------------------------------
**1. CListCtrl 风格**
      LVS_ICON: 为每个item显示大图标
      LVS_SMALLICON: 为每个item显示小图标
      LVS_LIST: 显示一列带有小图标的item
      LVS_REPORT: 显示item详细资料
      直观的理解：windows资源管理器，“查看”标签下的“大图标，小图标，列表，详细资料”
--------------------------------------------------------------------------------
**2. 设置listctrl 风格及扩展风格**      LONG lStyle;
      lStyle. = GetWindowLong(m_list.m_hWnd, GWL_STYLE);//获取当前窗口style
      lStyle. &= ~LVS_TYPEMASK; //清除显示方式位
      lStyle.|= LVS_REPORT; //设置style
      SetWindowLong(m_list.m_hWnd, GWL_STYLE, lStyle);//设置style
      DWORD dwStyle. = m_list.GetExtendedStyle();
      dwStyle.|= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl）
      dwStyle.|= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl）
      dwStyle.|= LVS_EX_CHECKBOXES;//item前生成checkbox控件
      m_list.SetExtendedStyle(dwStyle); //设置扩展风格
      注：listview的style请查阅msdn
[http://msdn.microsoft.com/library/default.asp?url=/library/en-us/wceshellui5/html/wce50lrflistviewstyles.asp](http://msdn.microsoft.com/library/default.asp?url=/library/en-us/wceshellui5/html/wce50lrflistviewstyles.asp)
--------------------------------------------------------------------------------
**3. 插入数据**
      m_list.InsertColumn( 0, "ID", LVCFMT_LEFT, 40 );//插入列
      m_list.InsertColumn( 1, "NAME", LVCFMT_LEFT, 50 );
      int nRow = m_list.InsertItem(0, “11”);//插入行
      m_list.SetItemText(nRow, 1, “jacky”);//设置数据
--------------------------------------------------------------------------------
**4. 一直选中item**
    选中style中的Show selection always，或者在上面第2点中设置LVS_SHOWSELALWAYS
--------------------------------------------------------------------------------
**5. 选中和取消选中一行**
    int nIndex = 0;
    //选中
    m_list.SetItemState(nIndex, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
    //取消选中
    m_list.SetItemState(nIndex, 0, LVIS_SELECTED|LVIS_FOCUSED);
--------------------------------------------------------------------------------
**6. 得到listctrl中所有行的checkbox的状态**
      m_list.SetExtendedStyle(LVS_EX_CHECKBOXES);
      CString str;
      for(int i=0; i<m_list.GetItemCount(); i++)
      {
           if( m_list.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED || m_list.GetCheck(i))
           {
                str.Format(_T("第%d行的checkbox为选中状态"), i);
                AfxMessageBox(str);
           }
      }
--------------------------------------------------------------------------------
**7. 得到listctrl中所有选中行的序号**
      方法一：
      CString str;
      for(int i=0; i<m_list.GetItemCount(); i++)
      {
           if( m_list.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED )
           {
                str.Format(_T("选中了第%d行"), i);
                AfxMessageBox(str);
           }
      }
      方法二：
      POSITION pos = m_list.GetFirstSelectedItemPosition();
      if (pos == NULL)
           TRACE0("No items were selected!/n");
      else
      {
           while (pos)
           {
                int nItem = m_list.GetNextSelectedItem(pos);
                TRACE1("Item %d was selected!/n", nItem);
                // you could do your own processing on nItem here
           }
      }
--------------------------------------------------------------------------------
**8. 得到item的信息**
      TCHAR szBuf[1024];
      LVITEM lvi;
      lvi.iItem = nItemIndex;
      lvi.iSubItem = 0;
      lvi.mask = LVIF_TEXT;
      lvi.pszText = szBuf;
      lvi.cchTextMax = 1024;
      m_list.GetItem(&lvi);
      关于得到设置item的状态，还可以参考msdn文章
      Q173242: Use Masks to Set/Get Item States in CListCtrl
[http://support.microsoft.com/kb/173242/en-us](http://support.microsoft.com/kb/173242/en-us)
--------------------------------------------------------------------------------
**9. 得到listctrl的所有列的header字符串内容**      LVCOLUMN lvcol;
      char str[256];
      int   nColNum;
      CString strColumnName[4];//假如有4列
      nColNum = 0;
      lvcol.mask = LVCF_TEXT;
      lvcol.pszText = str;
      lvcol.cchTextMax = 256;
      while(m_list.GetColumn(nColNum, &lvcol))
      { 
           strColumnName[nColNum] = lvcol.pszText;
           nColNum++;
      }
--------------------------------------------------------------------------------
**10. 使listctrl中一项可见，即滚动滚动条**
    m_list.EnsureVisible(i, FALSE);
--------------------------------------------------------------------------------
**11. 得到listctrl列数**    int nHeadNum = m_list.GetHeaderCtrl()->GetItemCount();
--------------------------------------------------------------------------------
**12. 删除所有列**    方法一：
         while ( m_list.DeleteColumn (0))
       因为你删除了第一列后，后面的列会依次向上移动。
    方法二：
      int nColumns = 4;
      for (int i=nColumns-1; i>=0; i--)
          m_list.DeleteColumn (i);
--------------------------------------------------------------------------------
**13. 得到单击的listctrl的行列号**
      添加listctrl控件的NM_CLICK消息相应函数
      void CTest6Dlg::OnClickList1(NMHDR* pNMHDR, LRESULT* pResult)
      {
           // 方法一：
           /*
           DWORD dwPos = GetMessagePos();
           CPoint point( LOWORD(dwPos), HIWORD(dwPos) );
           m_list.ScreenToClient(&point);
           LVHITTESTINFO lvinfo;
           lvinfo.pt = point;
           lvinfo.flags = LVHT_ABOVE;
           int nItem = m_list.SubItemHitTest(&lvinfo);
           if(nItem != -1)
           {
                CString strtemp;
                strtemp.Format("单击的是第%d行第%d列", lvinfo.iItem, lvinfo.iSubItem);
                AfxMessageBox(strtemp);
           }
          */
          // 方法二:
          /*
           NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
           if(pNMListView->iItem != -1)
           {
                CString strtemp;
                strtemp.Format("单击的是第%d行第%d列",
                                pNMListView->iItem, pNMListView->iSubItem);
                AfxMessageBox(strtemp);
           }
          */
           *pResult = 0;
      }
--------------------------------------------------------------------------------
**14. 判断是否点击在listctrl的checkbox上**
      添加listctrl控件的NM_CLICK消息相应函数
      void CTest6Dlg::OnClickList1(NMHDR* pNMHDR, LRESULT* pResult)
      {
           DWORD dwPos = GetMessagePos();
           CPoint point( LOWORD(dwPos), HIWORD(dwPos) );
           m_list.ScreenToClient(&point);
           LVHITTESTINFO lvinfo;
           lvinfo.pt = point;
           lvinfo.flags = LVHT_ABOVE;
           UINT nFlag;
           int nItem = m_list.HitTest(point, &nFlag);
           //判断是否点在checkbox上
           if(nFlag == LVHT_ONITEMSTATEICON)
           {
                AfxMessageBox("点在listctrl的checkbox上");
           } 
           *pResult = 0;
      }
--------------------------------------------------------------------------------
**15. 右键点击listctrl的item弹出菜单**
      添加listctrl控件的NM_RCLICK消息相应函数
      void CTest6Dlg::OnRclickList1(NMHDR* pNMHDR, LRESULT* pResult)
      {
           NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
           if(pNMListView->iItem != -1)
           {
                DWORD dwPos = GetMessagePos();
                CPoint point( LOWORD(dwPos), HIWORD(dwPos) );
                CMenu menu;
                VERIFY( menu.LoadMenu( IDR_MENU1 ) );
                CMenu* popup = menu.GetSubMenu(0);
                ASSERT( popup != NULL );
                popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this );
           } 
           *pResult = 0;
}
--------------------------------------------------------------------------------
**16. item切换焦点时(包括用键盘和鼠标切换item时)，状态的一些变化顺序**
      添加listctrl控件的LVN_ITEMCHANGED消息相应函数
      void CTest6Dlg::OnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult)
      {
           NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
           // TODO: Add your control notification handler code here
           CString sTemp;
           if((pNMListView->uOldState & LVIS_FOCUSED) == LVIS_FOCUSED && 
            (pNMListView->uNewState & LVIS_FOCUSED) == 0)
           {
                sTemp.Format("%d losted focus",pNMListView->iItem);
           }
           else if((pNMListView->uOldState & LVIS_FOCUSED) == 0 &&
               (pNMListView->uNewState & LVIS_FOCUSED) == LVIS_FOCUSED)
           {
                sTemp.Format("%d got focus",pNMListView->iItem);
           } 
           if((pNMListView->uOldState & LVIS_SELECTED) == LVIS_SELECTED &&
            (pNMListView->uNewState & LVIS_SELECTED) == 0)
           {
                sTemp.Format("%d losted selected",pNMListView->iItem);
           }
           else if((pNMListView->uOldState & LVIS_SELECTED) == 0 &&
            (pNMListView->uNewState & LVIS_SELECTED) == LVIS_SELECTED)
           {
                sTemp.Format("%d got selected",pNMListView->iItem);
           }
           *pResult = 0;
      }
--------------------------------------------------------------------------------
**17. 得到另一个进程里的listctrl控件的item内容**
[http://www.codeproject.com/threads/int64_memsteal.asp](http://www.codeproject.com/threads/int64_memsteal.asp)
--------------------------------------------------------------------------------
**18. 选中listview中的item**
Q131284: How To Select a Listview Item Programmatically
[http://support.microsoft.com/kb/131284/en-us](http://support.microsoft.com/kb/131284/en-us)
--------------------------------------------------------------------------------
**19. 如何在CListView中使用CListCtrl的派生类**
[http://www.codeguru.com/cpp/controls/listview/introduction/article.php/c919/](http://www.codeguru.com/cpp/controls/listview/introduction/article.php/c919/)
--------------------------------------------------------------------------------
**20. listctrl的subitem添加图标**
      m_list.SetExtendedStyle(LVS_EX_SUBITEMIMAGES);
      m_list.SetItem(..); //具体参数请参考msdn
--------------------------------------------------------------------------------
**21. 在CListCtrl显示文件，并根据文件类型来显示图标**
      网上找到的代码，share
      BOOL CTest6Dlg::OnInitDialog()
      {
           CDialog::OnInitDialog();
           HIMAGELIST himlSmall;
           HIMAGELIST himlLarge;
           SHFILEINFO sfi;
           char cSysDir[MAX_PATH];
           CString strBuf;
           memset(cSysDir, 0, MAX_PATH);
           GetWindowsDirectory(cSysDir, MAX_PATH);
           strBuf = cSysDir;
           sprintf(cSysDir, "%s", strBuf.Left(strBuf.Find("//")+1));
           himlSmall = (HIMAGELIST)SHGetFileInfo ((LPCSTR)cSysDir, 
                      0, 
                      &sfi, 
                      sizeof(SHFILEINFO), 
                      SHGFI_SYSICONINDEX | SHGFI_SMALLICON );
           himlLarge = (HIMAGELIST)SHGetFileInfo((LPCSTR)cSysDir, 
                      0, 
                      &sfi, 
                      sizeof(SHFILEINFO), 
                      SHGFI_SYSICONINDEX | SHGFI_LARGEICON);
           if (himlSmall && himlLarge)
           {
                ::SendMessage(m_list.m_hWnd, LVM_SETIMAGELIST,
                             (WPARAM)LVSIL_SMALL, (LPARAM)himlSmall);
                ::SendMessage(m_list.m_hWnd, LVM_SETIMAGELIST,
                             (WPARAM)LVSIL_NORMAL, (LPARAM)himlLarge);
           }
           return TRUE; // return TRUE unless you set the focus to a control
      }
      void CTest6Dlg::AddFiles(LPCTSTR lpszFileName, BOOL bAddToDocument)
      {
           int nIcon = GetIconIndex(lpszFileName, FALSE, FALSE);
           CString strSize;
           CFileFind filefind;
           // get file size
           if (filefind.FindFile(lpszFileName))
           {
                filefind.FindNextFile();
                strSize.Format("%d", filefind.GetLength());
           }
           else
                strSize = "0";
           // split path and filename
           CString strFileName = lpszFileName;
           CString strPath;
           int nPos = strFileName.ReverseFind('//');
           if (nPos != -1)
           {
                strPath = strFileName.Left(nPos);
                strFileName = strFileName.Mid(nPos + 1);
           }
           // insert to list
           int nItem = m_list.GetItemCount();
           m_list.InsertItem(nItem, strFileName, nIcon);
           m_list.SetItemText(nItem, 1, strSize);
           m_list.SetItemText(nItem, 2, strFileName.Right(3));
           m_list.SetItemText(nItem, 3, strPath);
      }
      int CTest6Dlg::GetIconIndex(LPCTSTR lpszPath, BOOL bIsDir, BOOL bSelected)
      {
           SHFILEINFO sfi;
           memset(&sfi, 0, sizeof(sfi));
           if (bIsDir)
           {
            SHGetFileInfo(lpszPath, 
                         FILE_ATTRIBUTE_DIRECTORY, 
                         &sfi, 
                         sizeof(sfi), 
                         SHGFI_SMALLICON | SHGFI_SYSICONINDEX |
                         SHGFI_USEFILEATTRIBUTES |(bSelected ? SHGFI_OPENICON : 0)); 
            return sfi.iIcon;
           }
           else
           {
            SHGetFileInfo (lpszPath, 
                         FILE_ATTRIBUTE_NORMAL, 
                         &sfi, 
                         sizeof(sfi), 
                         SHGFI_SMALLICON | SHGFI_SYSICONINDEX | 
                         SHGFI_USEFILEATTRIBUTES | (bSelected ? SHGFI_OPENICON : 0));
            return   sfi.iIcon;
           }
           return -1;
      }
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/KataDoc360/archive/2009/05/18/4195356.aspx](http://blog.csdn.net/KataDoc360/archive/2009/05/18/4195356.aspx)
