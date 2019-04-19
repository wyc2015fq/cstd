# 一步一步学List Control控件的用法（第一步）----创建第一列 - SHINING的博客 - CSDN博客
2010年03月17日 10:11:00[snsn1984](https://me.csdn.net/snsn1984)阅读数：27346标签：[list																[mfc																[report																[windows																[.net																[工具](https://so.csdn.net/so/search/s.do?q=工具&t=blog)](https://so.csdn.net/so/search/s.do?q=.net&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)](https://so.csdn.net/so/search/s.do?q=report&t=blog)](https://so.csdn.net/so/search/s.do?q=mfc&t=blog)](https://so.csdn.net/so/search/s.do?q=list&t=blog)
个人分类：[VC++/MFC](https://blog.csdn.net/snsn1984/article/category/420804)
第一步：创建一个列
注：开发平台为VS2008.NET。
1、创建一个MFC应用程序，选择基于对话框的。
具体步骤如下：文件à新建à项目àVisual C++àMFCàMFC应用程序，在下方名称中输入你自己想要建立的项目名，此处为ListCtrl。
在MFC应用程序向导中选择引用程序类型，然后在窗口右边应用程序类型中选择基于对话框，然后选择完成。
2、进入项目，可以直接看到建立好的一个对话框，将对话框中部的“TODO：在此放置对话框删除”。
3、从工具箱中拖一个List Contrrol到对话框上，调节大小到自己满意的状态。
4、将控件（一般ID为IDC_LIST1）的View的属性改为改为Report，一般默认为ICON。
5、使用Ctrl+左键双击，打开添加成员变量向导，选择控件变量，控件ID为上一步添加的的List Control的ID（默认为IDC_LIST1），设置访问权限（一般为public），变量类型（CListCtrl）,变量名可以自己设定，此处假设为m_ListCtrl,点击确定。由此可以发现在你的对话框对应的类中多了一个变量。对话框对应的类名为CXXXXDlg，XXX为你的工程名，此处为CListCtrlDlg。
6、在对话框对应的类CListCtrlDlg中的OnInitDialog(void)中的CDialog::OnInitDialog();后便可以添加代码如下：
m_ListCtrl.InsertColumn(0, _T("姓名"), LVCFMT_LEFT, 60，-1);
7、按F5调试程序，你可以发现如下的窗口：
![截图](http://hi.csdn.net/attachment/201003/17/33463_1268792477G6k3.gif)
8、此处由于是在WINDOWS7的系统上的结果，所以可能跟XP下面的视觉效果有点不同。而且大家可以发现，姓名这一栏的宽度是可以调整的。
9、下面我们来看下刚才加进去的那一行代码：
m_ListCtrl.InsertColumn(0, _T("姓名"), LVCFMT_LEFT, 60，-1);
m_ListCtrl就是我们刚刚加入的成员变量，也即是个CListCtrl类的变量，其实也可以理解为ClistCtrl类的一个对象；
InsertColumn();其实为ClistCtrl类的成员函数，具体内容如下：
CListCtrl::InsertColumn
int InsertColumn(int nCol,const LVCOLUMN* pColumn)
int InsertColumn(int nCol,
                 LPCTSTR lpszColumnHeading,
                 int nFormat = LVCFMT_LEFT,
                 int nWidth = -1,
                 int nSubItem = -1
                )
返回值：如果成功，则返回新建列的索引值，否则为-1。
参数：
|nCol|新建列的索引值。|
|----|----|
|pColumn|包含新建列属性的LVCOLUMN结构的地址。|
|lpszcolumnHeading|包含列标题的字符串的地址。|
|nFormat|指定列对齐方式的整数。它为下列值之一：LVCFMT_LEFT，LVCFMT_RIGHT或LVCFMT_CENTER。|
|nWidth|以像素为单位的列宽。如果该参数为-1，那么没有设置列宽。|
|nSubItem|与列相关联的子项的索引。如果该参数为-1，那么没有子项与列相关。|
说明：在列表视图控件中新插入一列。
10、其实在
int InsertColumn(int nCol,
                 LPCTSTR lpszColumnHeading,
                 int nFormat = LVCFMT_LEFT,
                 int nWidth = -1,
                 int nSubItem = -1
                )
中，可以直接传递给函数四个值，最后一个值不用管，也可以达到同样的效果。
