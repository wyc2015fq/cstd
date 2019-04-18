# MFC——ComBox用法大全 - wishchinYang的专栏 - CSDN博客
2013年11月15日 21:36:57[wishchin](https://me.csdn.net/wishchin)阅读数：1094
（1）：先看第一篇：[CComBox控件使用方法](http://blog.csdn.net/a284812264/article/details/8628218)
**CComBox控件在MFC编程经常使用到。主要涉及到几个功能：**
**1、值的插入，这通常会使用两种方法**
**1）**
int AddString(LPCTSTR lpszString)
** 使用这个方法插入值不大好控制**
**2）**
int InsertString(int nIndex, LPCTSTR lpszString) 可以控制插入值的按照你的顺序。
2、int SetItemData(int nIndex, DWORD_PTR dwItemData) 配合InsertString使用3、int SetCurSel(int nSelect) 设置当前选中的item4、int GetCurSel() 获取选中的item
**5、DWORD_PTR GetItemData(intnIndex)**
**  获取绑定的值**
**6、**
int SelectString(int nStartAfter,LPCTSTR lpszString)
** 这个方法相当的好用，通常从数据库获取的ID都是一个整型值，这就需要转换成需要显示的字符串。这个方法就提供了查找字符串并且设置选中的字符串的功能。**
（2）：再看详细介绍：
有修改！原文链接：[http://blog.csdn.net/wzy198852/article/details/7206990](http://blog.csdn.net/wzy198852/article/details/7206990)
Combo
 Box (组合框)控件很简单，可以节省空间。从用户角度来看，这个控件是由一个文本输入控件和一个下拉菜单组成的。用户可以从一个预先定义的列表里选择一个选项，同时也可以直接在文本框里面输入文本。下面的例子简要说明如何利用 MFC CComboBox Class来操作字符串列表。
1、定义控件对应变量
假定已经创建了一个Dialog，并且从控件工具箱将 Combo Box 控件拖放到上面。打开 Class Wizard，添加控件对应变量，如：CComboBox m_cbExamble;
在后面的代码中会不断使用这个变量。
2、向控件添加 Items
1) 在Combo Box控件属性的Data标签里面添加，一行表示Combo Box下拉列表中的一行。换行用ctrl+回车。（VS2010可以直接使用分号来标定，这只是个约束问题，是个小问题）
2) 利用函数 AddString() 向 Combo Box 控件添加 Items，如：
m_cbExample.AddString(“StringData1”);
m_cbExample.AddString(“StringData2”);
m_cbExample.AddString(“StringData3”);
3) 也可以调用函数 InsertString() 将 Item 插入指定位置 nIndex，如：
m_cbExample.InsertString( nIndex, “StringData” );
3、从控件得到选定的Item
假设在控件列表中已经选定某项，现在要得到被选定项的内容，首先要得到该项的位置，然后得到对应位置的内容。这里会用到两个函数，如：
int nIndex = m_cbExample.GetCurSel();
CString strCBText;
m_cbExample.GetLBText( nIndex, strCBText);
这样，得到的内容就保存在 strCBText 中。
若要选取当前内容，可调用函数GetWindowText(strCBText)。
4、在控件中查找给定Item
这种操作一般用于在程序中动态修改控件中该项的值，可以用函数FindStringExact() 精确匹配，如：
int nIndex = m_cbExample.FindStringExact( nStartAfter, “value to be found”);
nStartAfter指明从哪一行开始查找。如果查找成功，返回的是该项的位置；否则，返回CB_ERR。
也可以选中包含指定字符串的项，如：
int nIndex = m_cbExample.SelectString( nStartAfter, “value to be selected”);
5、删除控件中的Item
该操作可以利用函数DeleteString()，需要指定被删除项的位置，如：
m_cbExample.DeleteString(nIndex);
也可以使用函数ResetContent()，清除目前的所有项，如：
m_cbExample.ResetContent();
6、显示控件中的某项
int nIndex = m_cbExample.GetCurSel(); //当前选中的项
m_cbExample.SetCurSel(nIndex); //设置第nIndex项为显示的内容
7、得到或设置输入框中被选中的字符位置
DWORD GetEditSel( ) /BOOL SetEditSel( int nStartChar, int nEndChar );
BOOL LimitText( int nMaxChars ); 设置输入框中可输入的最大字符数。
8、列表框常用消息映射宏
ON_CBN_DBLCLK 鼠标双击
ON_CBN_DROPDOWN 列表框被弹出
ON_CBN_KILLFOCUS / ON_CBN_SETFOCUS 在输入框失去/得到输入焦点时产生
ON_CBN_SELCHANGE 列表框中选择的行发生改变
ON_CBN_EDITUPDATE 输入框中内容被更新
补充：
一、如何添加/删除Combo Box内容
1，在Combo Box控件属性的Data标签里面添加，一行表示Combo Box下拉列表中的一行。换行用ctrl+回车。
2，在程序初始化时动态添加
如： //控件内容初始化
CString strTemp;
((CComboBox*)GetDlgItem(IDC_COMBO_CF))->ResetContent();//消除现有所有内容
for(int i=1;i<=100;i++)
{
strTemp.Format("%d",i);
((CComboBox*)GetDlgItem(IDC_COMBO_CF))->AddString(strTemp);
}
3，下拉的时候添加
如： CString strTemp;
int iCount=((CComboBox*)GetDlgItem(IDC_COMBO_CF))->GetCount();//取得目前已经有的行数
if(iCount<1)//防止重复多次添加
{
((CComboBox*)GetDlgItem(IDC_COMBO_CF))->ResetContent();
for(int i=1;i<=100;i++)
{
strTemp.Format("%d",i);
((CComboBox*)GetDlgItem(IDC_COMBO_CF))->AddString(strTemp);
}
}
4，删除
DeleteString( UINT nIndex )//删除指定行，
5，插入
InsertString( int nIndex, LPCTSTR lpszItem )//将行插入到指定位置
6，查找
FindString( int nStartAfter, LPCTSTR lpszItem )//可以在当前所有行中查找指定的字符传的位置，nStartAfter指明从那一行开始进行查找。
int SelectString( int nStartAfter, LPCTSTR lpszItem )//可以选中包含指定字符串的行
二、如何控制Combo Box的下拉长度
1，首先要知道两点：一、那就是在设计界面里，点击一下Combo Box的下拉箭头，此时出现的调整框就是Combo Box的下拉调整框。
2，二、属性里有个 No integral height 钩选项，表示最大长度为设计长度，如果实际内容比设计长度多，就出现滚动条，少就以实际长度显示。
三、选择其中的某行
1，选中：
int iPos=((CComboBox*)GetDlgItem(IDC_COMBO_CF))->GetCurSel();//当前选中的行。
2，设置
((CComboBox*)GetDlgItem(IDC_COMBO_CF))->SetCurSel(n)//设置第n行内容为显示的内容。
四、取得Combo Box框内容
1取当前内容
((CComboBox*)GetDlgItem(IDC_COMBO_CF))->GetWindowText(strTemp);
2取其他行内容
((CComboBox*)GetDlgItem(IDC_COMBO_CF))->GetLBText(n,strTemp);
四、获得焦点
通常要判断控件是否获得了焦点，可以用GetFocus()函数
例如：if(GetFocus()==GetDlgItem(IDC_EDIT_VALUE2))//判断焦点是否在编辑框IDC_EDIT_VALUE2内。
但是combobox 的焦点不同，因为它是由edit和listbox两部分组成的
所以获得焦点要用GetParent()：if ((GetFocus()->GetParent())==GetDlgItem(IDC_COMBO_CF))
控制Combo Box的下拉长度
1，首先要知道两点：一、那就是在设计界面里，点击一下Combo Box的下拉箭头，此时出现的调整框就是Combo Box的下拉调整框。
二、属性里有个 No integral height 钩选项，表示最大长度为设计长度，如果实际内容比设计长度多，就出现滚动条，少就以实际长度显示。
VC++ Combo Box/Combo Box Ex控件
　　组合窗口是由一个输入框和一个列表框组成。创建一个组合窗口可以使用成员函数：
BOOL CListBox::Create( LPCTSTR lpszText, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID = 0xffff );
其中dwStyle将指明该窗口的风格，除了子窗口常用的风格WS_CHILD,WS_VISIBLE外，你可以针对列表控件指明专门的风格。
CBS_DROPDOWN 下拉式组合框
CBS_DROPDOWNLIST 下拉式组合框，但是输入框内不能进行输入
CBS_SIMPLE 输入框和列表框同时被显示
LBS_SORT 所有的行按照字母顺序进行排序
由于组合框内包含了列表框，所以列表框的功能都能够使用，如可以利用：
int AddString( LPCTSTR lpszItem )添加行，
int DeleteString( UINT nIndex )删除指定行，
int InsertString( int nIndex, LPCTSTR lpszItem )将行插入到指定位置。
void ResetContent( )可以删除列表框中所有行。
通过调用int GetCount( )得到当前列表框中行的数量。
如果需要得到/设置当前被选中的行的位置，可以调用int GetCurSel( )/int SetCurSel(int iIndex)。通过调用int GetLBText( int nIndex, LPTSTR lpszText )得到列表框内指定行的字符串。
此外通过调用int FindString( int nStartAfter, LPCTSTR lpszItem )可以在当前所有行中查找指定的字符传的位置，nStartAfter指明从那一行开始进行查找。
int SelectString( int nStartAfter, LPCTSTR lpszItem )可以选中包含指定字符串的行。
此外输入框的功能都能够使用，如可以利用：
DWORD GetEditSel( ) /BOOL SetEditSel( int nStartChar, int nEndChar )得到或设置输入框中被选中的字符位置。
BOOL LimitText( int nMaxChars )设置输入框中可输入的最大字符数。
输入框的剪贴板功能Copy,Clear,Cut,Paste动可以使用。
最后介绍一下列表框几种常用的消息映射宏：
ON_CBN_DBLCLK 鼠标双击
ON_CBN_DROPDOWN 列表框被弹出
ON_CBN_KILLFOCUS / ON_CBN_SETFOCUS 在输入框失去/得到输入焦点时产生
ON_CBN_SELCHANGE 列表框中选择的行发生改变
ON_CBN_EDITUPDATE 输入框中内容被更新
使用以上几种消息映射的方法为定义原型如：afx_msg void memberFxn( );的函数，并且定义形式如ON_Notification( id, memberFxn )的消息映射。如果在对话框中使用组合框，Class Wizard会自动列出相关的消息，并能自动产生消息映射代码。
在MFC 4.2中对组合框进行了增强，你可以在组合框中使用ImageList，有一个新的类CComboBoxEx（由CComboBox派生）来实现这一功能。在CComboBoxEx类中添加了一些新的成员函数来实现新的功能：首先你需要调用CImageList* SetImageList( CImageList* pImageList );来设置ImageList，然后调用
int InsertItem( const COMBOBOXEXITEM* pCBItem );来添加行，其中COMBOBOXEXITEM定义如下：
typedef struct { UINT mask; int iItem; LPTSTR pszText; int cchTextMax; int iImage; int iSelectedImage; int iOverlay; 　　　　　 int iIndent; LPARAM lParam;} COMBOBOXEXITEM, *PCOMBOBOXEXITEM;
你需要设置mask=CBEIF_IMAGE　CBEIF_TEXT，并设置iItem为插入位置，设置pszText为显示字符串，设置iImage为显示的图标索引。下面的代码演示了如何进行插入：
/*m_cbeWnd 为已经创建的CComboBox对象
m_list 为CImageList对象IDB_IMG 为16*(16*4)的位图，每个图片为16*16共4个图标*/
m_list.Create(IDB_IMG,16,4,RGB(0,0,0));
m_cbeWnd.SetImageList(&m_list);
COMBOBOXEXITEM insItem;insItem.mask=CBEIF_IMAGE　CBEIF_TEXT;insItem.iItem=0; insItem.iImage=0;insItem.pszText="Line 1";m_cbeWnd.InsertItem(&insItem);insItem.iItem=1;insItem.iImage=1;insItem.pszText="Line 2";m_cbeWnd.InsertItem(&insItem);
通过调用int DeleteItem( int iIndex );来删除行，并指明行的位置。
通过调用BOOL GetItem( COMBOBOXEXITEM* pCBItem )/BOOL SetItem( const COMBOBOXEXITEM* pCBItem );来得到/设置行数据
******************************************************************************************************
tonyswe（tong）回复于 2003-03-10 13:31:10 得分 0 这位朋友可能也是新手. 
是不是加了,运行后看不到选项. 
加上后.要先在combo box的属性里的styles->types下选simple,然后把combo box框拉大, 
也就是你希望的下拉的范围大小. 再回到styles->types下选drop down 就行了.
MFC ComBox ResetContent( )
MFC, ComBox, ResetContent
在 ComBox控件的EditExchange事件中,添加ResetContent( )函数清空列表后无法输入数据,如何解决这个问题
void CTestDlg::OnEditchangeCombo1() 
{
((CComboBox*)GetDlgItem(IDC_COMBO1))->ResetContent();
//其他.....
}  
使ComBox默认有值：
（（CComboBox*）GetDlgItem(IDC_COMBO1)）->AddString("男");
（（CComboBox*）GetDlgItem(IDC_COMBO1)）->AddString("女");
（（CComboBox*）GetDlgItem(IDC_COMBO1)）->SetCurlSel(1);
