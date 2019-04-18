# CListCtrl使用详解 - ljx0305的专栏 - CSDN博客
2010年08月12日 16:45:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：906标签：[imagelist																[listview																[image																[path																[report																[list](https://so.csdn.net/so/search/s.do?q=list&t=blog)](https://so.csdn.net/so/search/s.do?q=report&t=blog)](https://so.csdn.net/so/search/s.do?q=path&t=blog)](https://so.csdn.net/so/search/s.do?q=image&t=blog)](https://so.csdn.net/so/search/s.do?q=listview&t=blog)](https://so.csdn.net/so/search/s.do?q=imagelist&t=blog)
个人分类：[MFC																[VC](https://blog.csdn.net/ljx0305/article/category/401790)](https://blog.csdn.net/ljx0305/article/category/403913)
1。先来介绍REPORT类型的CListCtrl：
首先使用下面的语句设置CListCtrl的style：
 DWORD SetExtendedStyle( DWORD dwNewStyle );
其中
 LVS_EX_CHECKBOXES 表示添加CheckBox
 LVS_EX_FULLROWSELECT 表示选择整行
 LVS_EX_GRIDLINES 表示添加表格线
如果设置了LVS_EX_CHECKBOXES属性，则可以用
 BOOL GetCheck( int nItem ) const;
来得到某一行是否Checked。
可以先用下面的语句来删除以前的东西：
 for(int k=2;k>=0;k--) //注意要从后往前删，否则出错
  m_ListCtrl.DeleteColumn(k);
 m_ListCtrl.DeleteAllItems();
用下面的语句新建列：
 m_ListCtrl.InsertColumn(0,_T("文件名"),LVCFMT_IMAGE|LVCFMT_LEFT);
 m_ListCtrl.InsertColumn(1,_T("仪器类别"));
 m_ListCtrl.InsertColumn(2,_T("项目类别"));
其中LVCFMT_IMAGE表示可以在第一列加入图标。如果不要图标可以删去。
然后设置列宽：
 for(j=0;j<3;j++)
  m_ListCtrl.SetColumnWidth(j ,100);
以下为列表加入图标，如果不需要图标，可以跳过这一步。注意只在第一次加入，如果多次加入会出错！
先在头文件中加入声明：
 CImageList m_ImageList;
这是必要的，如果在cpp的某个函数中加入由于生命期结束，CImageList自动释放，则效果是列表中看不到图标，只看到一个白方块。
下面生成CImageList，并将其绑定到CListCtrl中，这是CImageList中还没有图标，只是一个容器：
 static int flag=2;
 if(flag==2){//只调用一次SetImageList，否则出错
  m_ImageList.Create(128, 128, ILC_COLORDDB|ILC_MASK, 20, 1); 
  m_ListCtrl.SetImageList(&m_ImageList,LVSIL_SMALL);
 }
 flag=(flag+1)%2;
如果CListCtrl已经用过，曾经加过图标进去，这时就要删除上次放进m_ImageList中的Image
 for(int kk=0;kk<m_ImageList.GetImageCount();kk++)
  m_ImageList.Remove(k);
下面介绍如何向CListCtrl里面加入行，并同时为每一行动态加入图标：
假设m_listRowCount为要加入的行数。
 CBitmap* bitmap;
 bitmap=new CBitmap[m_list1rowCount];
 HBITMAP hbitmap; 
 for(int i = 0; i < m_listRowCount; i++)
 {
  //为每一行插入相应的缩略图
  CFile f;
  CFileException e;  
  if( !f.Open(m_FileName, CFile::modeRead, &e )){ //m_FileName为bmp文件名，由你来定
   hbitmap = (HBITMAP)LoadImage(NULL,path+"blank.bmp",IMAGE_BITMAP,0,0,
    LR_CREATEDIBSECTION|LR_DEFAULTSIZE|LR_LOADFROMFILE);
  }else{
   f.Close();
   hbitmap = (HBITMAP)LoadImage(NULL,bmpFile,IMAGE_BITMAP,0,0,
    LR_CREATEDIBSECTION|LR_DEFAULTSIZE|LR_LOADFROMFILE);
  }
  bitmap[i].Attach(hbitmap);
  m_ImageList.Add(&bitmap[i], RGB(0, 128, 128));
  //插入行
  m_ListCtrl.InsertItem(i,m_FileName,i);
  m_ListCtrl.SetItemText(i,1,type);
  m_ListCtrl.SetItemText(i,2,m_Path);
 }
 //记得删除已经没用的临时文件
 if(m_list1rowCount!=0)
  delete[] bitmap;
2。如果是ICON类型的CListCtrl，则要做一点点改动：
把绑定图标集的代码由
 SetImageList(&m_ImageList,LVSIL_SMALL);
改为
 SetImageList(&m_ImageList,LVSIL_NORMAL);
插入行时只用
 InsertItem(i,mainSet.m_FileName,i);
不用
 SetItemText(i,1,type);
之类的代码。
[附]CListCtrl控件主要事件及LVN_ITEMCHANGED消息和鼠标双击列表项事件的处理:
LVN_BEGINDRAG 鼠标左键正在被触发以便进行拖放操作（当鼠标左键开始拖拽列表视图控件中的项目时产生）
LVN_BEGINRDRAG 鼠标右键正在被触发以便进行拖放操作（当鼠标右键开始拖拽列表视图控件中的项目时产生）
LVN_BEGINLABELEDIT 开始编辑项的文本
LVN_COLUMNCLICK 单击列（当鼠标单击列表视图控件列标题时产生）
NM_CLICK 当鼠标单击列表视图控件时产生
LVN_COLUMNCLICK 单击列
LVN_DELETEALLITEMS 删除所有项
LVN_DELETEITEM 删除某个项
NM_DBLCLK 当鼠标双击列表视图控件时产生
LVN_ENDLABELEDIT 结束对项文本的编辑
LVN_GETDISPINFO 请求需要显示的信息
LVN_GETINFOTIP 请求显示在工具提示窗口内的附加的文本信息
LVN_HOTTRACK 鼠标滑过某个项
LVN_INSERTITEM 当向列表视图控件插入项目时产生
lVN_ITEMACTIVATE 激活某个项
LVN_ITEMCHANGED 某个项已经发生变化
LVN_ITEMCHANGING 某个项目正在发生变化
NM_KILLFOCUS 当视表列图控件失去焦点时产生
LVN_KEYDOWN 某个键被按下
LVN_MARQUEEBEGIN 开始某个边框选择
NM_OUTOFMEMORY 当内存溢出时产生
LVN_ODCACHEHINT 虚拟列表控件的显示区域的内容发生了变化
LVN_ODSTATECHANGED 虚拟列表的控件的某个项或某个范围内的项已经发生变化
LVN_ODFINDITEM 需要拥有者查找一个特定的回调项
NM_RCLICK 当鼠标右键单击列表视图控件时产生
NM_RDBLCLK 当鼠标右键双击列表视图控件时产生
NM_SETFOCUS 当列表视图控件获得焦点时产生
LVN_SETDISPINFO 父窗口必须更新控件为项所维护的信息
注解：
HDN：Header Notify
LVN：ListView control Notify 
NM：Notify Message
TVN：TreeView control Notify
LVN_ITEMCHANGED消息的处理
消息处理函数：
void C***Dlg::OnItemchangedList(NMHDR* pNMHDR, LRESULT* pResult) 
{
NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
if(pNMListView->uChanged==LVIF_STATE)
{
if(pNMListView->uNewState & LVIS_SELECTED)
   {
    //处理内容
    int nItem=pNMListView->iItem;
    CString value[6];
    for(int i=0;i<6;i++)
    {
     value[i]=m_List.GetItemText(nItem,i);
    }
   }
}
*pResult = 0;
}
消息处理函数中的两个if条件句一定要有，否则可能出现多次进入处理内容的问题。
鼠标双击列表项事件的处理
响应NM_DBLCLK消息：
void CDisSalaryDlg::OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult) 
{
NM_LISTVIEW *pNMListView=(NM_LISTVIEW *)pNMHDR;
int nItem=pNMListView->iItem;
if(nItem>=0 && nItem<m_List.GetItemCount())            //判断双击位置是否在有数据的列表项上面
{
    .............
}
*pResult = 0;
}
消息处理函数中的if语句很重要，用于判断双击位置是否在有数据的列表项上面。因为NM_DBLCLK消息是CListCtrl控件（整体）消息，只要是在CListCtrl控件范围之内执行此事件就会产生该消息，而不一定要在有数据的列表项上面。
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/xiaofengsheng/archive/2009/09/12/4546097.aspx](http://blog.csdn.net/xiaofengsheng/archive/2009/09/12/4546097.aspx)
