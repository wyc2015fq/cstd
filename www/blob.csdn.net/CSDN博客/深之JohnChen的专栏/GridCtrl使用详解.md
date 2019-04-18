# GridCtrl使用详解  - 深之JohnChen的专栏 - CSDN博客

2008年06月19日 00:07:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：11124


**GridCtrl使用详解**

CGridCtrl类主要是通过grid样式显示数据，CGridCtrl的代码与demo请参考：[http://www.codeproject.com/KB/miscctrl/gridctrl.aspx](http://www.codeproject.com/KB/miscctrl/gridctrl.aspx)

**在单文档中的使用方法**步骤一　初始化在CView类的.h头文件中包含文件:     #include "Gridctrl.h" 并且手写加入如下的成员函数:     CGridCtrl * m_pGridCtrl;

步骤二　构造与析构构造函数中:    m_pGridCtrl = NULL; 析构函数中:    if(m_pGridCtrl)        delete m_pGridCtrl;步骤三 如果需要打印功能的话添加同名打印函数代码 在CView类的OnBeginPrinting()函数中添加如下代码: if(m_pGridCtrl)     m_pGridCtrl->OnBeginPrinting(pDC,pInfo); //简单吧,这就是类的好处 其它两个打印函数也一样的做法. 步骤四 在OnInitaUpdate()函数中或者你自己添加的要显示Grid的消息函数中如下初始化:
   //创建非模式对话框
   CDlg *dlg;
   dlg=new CDlg();
   dlg->Create(IDD_Dlg,this); 

   //初始化GridCtrl控件
   if(m_pGridCtrl!=NULL)
   {
   delete m_pGridCtrl;
   m_pGridCtrl=NULL;
   }
   if (m_pGridCtrl == NULL)
   {
  // Create the Gridctrl object
  m_pGridCtrl = new CGridCtrl;
  if (!m_pGridCtrl) return 0;
  // Create the Gridctrl window
  CRect rect;
  GetClientRect(rect);
  m_pGridCtrl->Create(rect, this, 100);
  // fill it up with stuff
  m_pGridCtrl->SetEditable(false);
  m_pGridCtrl->SetTextBkColor(RGB(0xFF, 0xFF, 0xE0)); //黄色背景
  m_pGridCtrl->EnableDragAndDrop(false);
  try {
   m_pGridCtrl->SetRowCount(k); //设置行数为k行
   m_pGridCtrl->SetColumnCount(4);   //k列
   m_pGridCtrl->SetFixedRowCount(1);   //标题行为一行
   m_pGridCtrl->SetFixedColumnCount(1);  //同上
  }
  catch (CMemoryException* e)
  {
   e->ReportError();
   e->Delete();
   return 0;
  }
  //填充列标题
  int row=0;
  for(int col=0;col<4;col++)
  {
         GV_ITEM Item;
   Item.mask = GVIF_TEXT|GVIF_FORMAT;
   Item.row = row;
   Item.col = col;
   if(col==0){
                Item.nFormat = DT_CENTER|DT_WORDBREAK;
    Item.strText.Format(_T("【类别】"),col);
   }
   else if(col==1){
                Item.nFormat = DT_LEFT|DT_WORDBREAK;
       Item.strText.Format(_T("第一列"),col);
   }
   else if(col==2){
    Item.nFormat = DT_LEFT|DT_WORDBREAK;
       Item.strText.Format(_T("第二列"),col);
   }
   m_pGridCtrl->SetItem(&Item);
  }
   // fill rows/cols with text
  for (row = 1; row < k; row++)
   for (col = 0; col < h; col++)
   { 
    GV_ITEM Item;
    Item.mask = GVIF_TEXT|GVIF_FORMAT;
    Item.row = row;
    Item.col = col;
    if (col < 1) {  //行标题头
     Item.nFormat = DT_CENTER|DT_VCENTER
     |DT_SINGLELINE|DT_END_ELLIPSIS
     |DT_NOPREFIX;
     Item.strText.Format(_T("%d"),row);
    } 
    else if(col==1){  //第一列的值
     Item.nFormat = DT_CENTER|DT_VCENTER
     |DT_SINGLELINE|DT_END_ELLIPSIS
     |DT_NOPREFIX;
        str="aa";
         Item.strText.Format(_T("%s"),str);
    }else if(col==2){ //第二列第值
     Item.nFormat = DT_CENTER|DT_VCENTER
     |DT_SINGLELINE|DT_END_ELLIPSIS
     |DT_NOPREFIX;
        CString str;
        str="bb";
         Item.strText.Format(_T("%s"),str);
    }
    m_pGridCtrl->SetItem(&Item);
   }
  m_pGridCtrl->AutoSize();

  //--------------设置行列距------------------
  for(int a=1;a<m;a++)
   m_pGridCtrl->SetRowHeight(a,21);  //设置各行高
        m_pGridCtrl->SetRowHeight(0,24);  //设置0行高
  m_pGridCtrl->SetColumnWidth(1,110); //设置2列宽
  m_pGridCtrl->SetColumnWidth(2,160); //设置3列宽
  m_pGridCtrl->SetColumnWidth(3,100); //设置4列宽
   }
上例取自实际工程,稍有修改! 部分注释: void SetVirtualMode(TRUE)//设为虚模式BOOL SetRowCount(int nRows) //设置总的行数。BOOL SetFixedRowCount(int nFixedRows = 1)//设置固定的行数据BOOL SetColumnCount(int nCols) //设置列数BOOL SetFixedColumnCount(int nFixedCols = 1)//设置固定的列数

步骤五: 添加WM_SIZE消息,调整控件的界面占屏幕大小   if(m_pGridCtrl->GetSafeHWnd())    {       CRect rect;      GetClientRect(rect);      m_pGridCtrl->MoveWindow(rect);    } 

**在对话框中的使用方法**步骤一　创建数据显示表格对话框

在资源管理器中新创建一个对话框，假设为CDlgTestReportBox。 从工具箱中加入Custom Control，就是人头像的那个，将其区域拉伸至要显示数据表格的大小，充满整个对话框。

在CDlgTestReportBox类的头文件中：

#include "GridCtrl.h" 

再定义成员变量：

CGridCtrl* m_pGrid;

添加OnShowWindow()消息处理函数如下：
void CDlgTestReportBox::OnShowWindow(BOOL bShow, UINT nStatus) 
{
    CDialog::OnShowWindow(bShow, nStatus);
    // TODO: Add your message handler code here
    if(m_pGrid!=NULL)
    {
        delete m_pGrid;
        m_pGrid=NULL;
     } 
  if(m_pGrid==NULL)
     {
        m_pGrid=new CGridCtrl;
        CRect rect;
        GetDlgItem(IDC_ReportAera)->GetWindowRect(rect); //得到显示区域
        ScreenToClient(&rect);
        m_pGrid->Create(rect,this,100);
        m_pGrid->SetEditable(false);
        m_pGrid->SetTextBkColor(RGB(0xFF, 0xFF, 0xE0)); //黄色背景
        try 
        {
            m_pGrid->SetRowCount(10); //初始为10行
            m_pGrid->SetColumnCount(11); //初始化为11列
            m_pGrid->SetFixedRowCount(1); //表头为一行
            m_pGrid->SetFixedColumnCount(1); //表头为一列 
        }
        catch (CMemoryException* e)
        {
            e->ReportError();
            e->Delete();
            // return FALSE;
        }
        for (int row = 0; row < m_pGrid->GetRowCount(); row++)
        for (int col = 0; col < m_pGrid->GetColumnCount(); col++)
        { 
            //设置表格显示属性
            GV_ITEM Item; 
            Item.mask = GVIF_TEXT|GVIF_FORMAT;
            Item.row = row;
            Item.col = col;
            if(row==0&&col==0) //第(0，0)格
            {
                Item.nFormat = DT_CENTER|DT_WORDBREAK;
                Item.szText.Format(_T("报表显示"),col);
            }
            else if (row < 1) //设置0行表头显示
            {        
                Item.nFormat = DT_CENTER|DT_WORDBREAK;
                Item.szText.Format(_T(" 项目%d"),col);
            }
            else if (col < 1) //设置0列表头显示
            {
                if(row< m_pGrid->GetRowCount()-4)
                {
                    Item.nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS;
                    Item.szText.Format(_T("第%d次"),row);
                }
            }
            else
            {
                Item.nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS;
                Item.szText.Format(_T(""),2);
            }
            m_pGrid->SetItem(&Item); 
        }
        m_pGrid->Invalidate();
    }
    //--------------设置行列距------------------
    for(int a=0;aGetRowCount();a++)
         m_pGrid->SetRowHeight(a,16); //设置各行高 
         m_pGrid->SetColumnWidth(0,58); //设置0列宽
    for(int b=1;bGetColumnCount();b++)
        m_pGrid->SetColumnWidth(b,59); //设置各列宽
}

步骤二　嵌入上面的对话框 显示数据

在你需要显示数据的对话框上的头文件中，假设为CDlgTest，加入

#include "GridCtrl.h"

CDlgTestReportBox* m_pTestReportBox;

将数据显示对话框放入你的对话框相应位置上，在CDlgTest::OnInitDialog() 中： 

if(!m_pTestReportBox) {      m_pTestReportBox=new CDlgTestReportBox(this); }

m_pTestReportBox->Create(IDD_DlgTestReportBox,this);

//定义区域变量 CRect rectDraw; GetDlgItem(IDC_AeraReport)->GetWindowRect(rectDraw); ScreenToClient(&rectDraw); //动态测试数据显示区域rectDraw

//将对应的对话框放到指定区域 m_pTestReportBox->MoveWindow(rectDraw); m_pTestReportBox->ShowWindow(SW_SHOW);

自定义填充数据的函数：CDlgTest::FillGrid() 如下：
CGridCtrl* pGrid=m_pTestReportBox->m_pGrid;
for (int row = pGrid->GetRowCount()-1; row >= pGrid->GetRowCount()-3; row--)
{
   for (int col = 1; col <= pGrid->GetColumnCount(); col++)
   { 
      GV_ITEM Item;
      Item.mask = GVIF_TEXT|GVIF_FORMAT;
      Item.row = row;
      Item.col = col;
      if(row==pGrid->GetRowCount()-3&&col>0) //平均值
      {
         if(col==10){
         Item.nFormat = DT_CENTER|DT_WORDBREAK;
         Item.szText.Format(_T(" %6.2f "),avjch);
      }
      else{
         Item.nFormat = DT_CENTER|DT_WORDBREAK;
         Item.szText.Format(_T(" %6.2f "),av[col-1]);
      }
   }
   pGrid->SetItem(&Item); //提交数据
   if(row==0||col==0)
   {
      COLORREF clr = RGB(0, 0, 0);
      pGrid->SetItemBkColour(row, col, clr);
      pGrid->SetItemFgColour(row, col, RGB(255,0,0));
   }
  }//循环结束
  pGrid->Invalidate();
} 
DBGRID和一般的GRID的不同之处在于，一般的GRID并不适合显示大的数据量，如果一个表中有上万条记录都要插入到GRID中，这将是一个很慢的过程，并且在GRID中移动滚动条时，它的记录的滚动也是很慢。而DBGRID并不会真正把这些记录的数据全部插入到控件中，当DBGRID的滚动条滚动时，它会根据DBGRID的显示面积的大小和查询得到的总记录数计算出当前应该显示哪些行，然后插入到表格中，这样一来，速度肯定快，而且没有数据量多少的限制。幸运的是，CGridCtrl类已经为我们提供了这种机制，它是采用虚模式实现的。使用这种方式，即使你向这个该控件插入一百万条数据，它并不会真的生成一百万行，而是随着你的滚动条的滚动，计算出在屏幕上要显示的行和列，然后会向你提供一个接口，通过这个接口，你可以在这儿设置你要显示的数据。下面给出使用CGridCtrl控件的虚模式的步骤：

**步骤一　初始化**

在视图的初始化函数里添加如下代码：

void SetVirtualMode(TRUE)　                   设为虚模式

BOOL SetRowCount(int nRows)                   设置总的行数。

BOOL SetFixedRowCount(int nFixedRows = 1)     设置固定的行数据

BOOL SetColumnCount(int nCols)                 设置列数

BOOL SetFixedColumnCount(int nFixedCols = 1)  设置固定的列数

**步骤二　响应消息　显示数据**

我们假设CGridCtrl是放在单文档视图中，而且它关联的变量是m_GridCtrl,利用ClassWizard添加视图的OnNotify响应函数。这个响应函数的写法是固定的，类似下面的代码：

BOOL CGridCtrlTestView::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 

{

if (wParam == (WPARAM)m_Grid.GetDlgCtrlID())

{

*pResult = 1;

GV_DISPINFO *pDispInfo = (GV_DISPINFO*)lParam;

if (GVN_GETDISPINFO  == pDispInfo->hdr.code)

{

//这是添加的函数，在这个函数里设置当前要显示的数据

SetGridItem(pDispInfo);

return TRUE;

}

}

return CGridCtrlTestView::OnNotify(wParam, lParam, pResult);

}

在上面的代码中，SetGridItem(pDispInfo)是添加的函数，在这个函数里我们设置当前要显示的数据。pDispInfo是一个GV_DISPINFO的结构体对象，它包含了每个单元格的信息，如行号，列号，有没有位图，背景色，前景色等。CGRIDCTRL会把当前要显示那个单元格行号，列号传递给我们，我们只要设置里面显示的数据就可以了。如下面是一个显示数据的例子。 

void CGridCtrlTestView::SetGridItem(GV_DISPINFO *pDispInfo)

{

      pDispInfo->item.strText.Format("row%d,col%d",pDispInfo->item.row, pDispInfo->item.col);

}

通过上面的介绍，我们应该已经会使用CGridCtrl虚模式，下面说明一下用CGridCtrl虚模式做DBGRID的原理，大家知道，MFC的CRecordset类支持多种游标机制，如双向游标的，如果我们是用ClassWizard来生成一个CRecordset的派生类的，那么就可以调用函数CRecordset::SetAbsolutePosition（），用这种方式来实现显示真是太简单了，因为在上面的void CGridCtrlTestView::SetGridItem(GV_DISPINFO *pDispInfo)函数中，已经知道要显示哪一行，哪一列的数据。所以只要通过CRecordset::SetAbsolutePosition（pDispInfo->item.row）函数，把游标定位到那一行，然后获取每个字段的数据就可以了。但是使用上面的方法有一个不好的地方在于，我们必须用ClassWizard为每个表从CRecordset派生出新类，这样做很不方便，在VC知识库第六期上面有一篇介绍《单独使用Crecordset》文章，可是上面的CRecordset打开方式只能使用CRecordset::forwardOnly，游标只能向前滚动，我们不能使用CRecordset::SetAbsolutePosition（）函数。而对于ACCESS数据库的访问，VC中MFC类库提供专门的DAO类（CDaoDatabase和CDaoRecordset等），在这里，附带指出VC6.0还不能访问Access2000数据库（可以使用Access2000转换为能识别的低版本）。对于其他类型的数据库的访问，可以使用开放式数据源（ODBC），然后通过DAO访问ODBC也可以。恰恰CDaoRecordset支持双向游标，可以调用函数CDaoRecordset::SetAbsolutePosition（），我们问题就不是就解决了吗。

**步骤三　打印表格数据**

打印显示在表格控件数据非常简单，重载视图函数OnPrint( )、OnBeginPrinting()和OnEndPringting(),分别调用控件的OnPrint(),OnBeginPrinting()和OnEndPrinting。其具体使用请看举例。

