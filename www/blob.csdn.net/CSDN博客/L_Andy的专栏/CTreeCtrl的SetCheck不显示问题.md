# CTreeCtrl的SetCheck不显示问题 - L_Andy的专栏 - CSDN博客

2014年11月10日 10:56:07[卡哥](https://me.csdn.net/L_Andy)阅读数：1225



一、问题的提出

CTreeCtrl有个属性TVS_HASBUTTONS，如果创建控件的时候加上了这个属性，则在每个节点的左侧

都有一个按钮，用来表示节点的选择状态。通过两个函数SetCheck / GetCheck来设置和获取指定

节点的选择状态。

但是奇怪的是，在对话框中按照常规的方法使用了SetCheck，最后CTreeCtrl并没有显示节点被选

中，下面是测试例子：

   1、用wizard创建一个对话框工程，并且在上面放置一个CTreeCtrl控件。

   2、设置CTreeCtrl的属性，"More Styles"里面选中"Check Boxes"，给它加上复选框。

   3、对话框初始化的时候，给CTreeCtrl控件添加节点，并设置选中。代码如下：

BOOL CTestCheckDlg::OnInitDialog()

{

 CDialog::OnInitDialog();

。。。

 // TODO: Add extra initialization here

 HTREEITEM hRoot = m_Tree.InsertItem("Root");

 m_Tree.SetCheck(hRoot);

 m_Tree.InsertItem("Child1", hRoot);

 m_Tree.InsertItem("Child2", hRoot);

 m_Tree.Expand(hRoot, TVE_EXPAND );

 return TRUE;

}

但是，对话框运行以后，树控件的根节点并没有显示被选中。用GetCheck测试，返回FALSE。但是

如果在OnInitDialog里面SetCheck以后，立刻用GetCheck测试，发现返回的却是TRUE。

进一步测试可以发现，对话框显示以后，任何时候再次使用SetCheck都没有问题了。

二、问题分析

由于无法直接监视树控件根节点状态的变化，为了弄清楚树控件究竟是什么时候修改了我们设置的

check状态，我在其他消息响应函数中进一步用GetCheck检查，发现对话框在第一次OnPaint的时候

，树控件根节点的check状态还是选中的，但紧接着下个消息来到后，它的状态就发生了改变。我

思来想去肯定是树控件本身的问题。可能是它第一次画自己的时候，修改了每个节点的选择状态。

但是它为何这么做？我开始猜测和它的图表列表有关系。为了验证这个想法，加入下面的代码：

 CImageList* pStateIcon = m_Tree.GetImageList(TVSIL_STATE);

 int nItem = (pStateIcon == NULL)?(0):(pStateIcon->GetImageCount());

 CString str;

 str.Format("Count of State Icon: %d\n", nItem);

 TRACE(str);

果然不出所料，测试结果发现，在OnInitDialog里面，树控件虽然添加了数据，但是它的State Icon

并没有加载，ImageList是空的。一旦它显示自己后，ImageList就不是NULL了，里面的图标数目是3。

也就是说它使用了3个图标。

显然是树控件在创建自己的时候，并没有加载所需的图标列表，而是在显示的时候，发现需要后，它才

加载，并且重新复位了每个节点的选择状态。它之所以这么做，我想可能是出于效率方面的考虑。也就

是说，如果用户没有添加TVS_HASBUTTONS，那么它就不需要这个图标列表了。

三、解决问题

既然知道了问题的所在，那么鉴于在对话框初始化的时候，设置树控件的数据和选择状态是一个常规的

做法，我就不打算修改这些代码的位置，而采用提前给它加载图标列表的方法。方法如下：

   1、在资源中添加一个位图资源IDB_BITMAP1，尺寸拉伸成48*16。（长48，高16，共三个图标）

   2、在位图上面画三个图标，每个大小都是16*16，第二个是X，第三个是选择（对勾）。至于第一个

      的用途，还不清楚。

   3、在对话框的头文件中，添加一个CImageList对象：

      CImageList m_ImageList;

   4、在对话框初始化函数中，添加如下代码：

 m_ImageList.Create(IDB_BITMAP1, 16,3,RGB(255,255,255));

 m_Tree.SetImageList(&m_ImageList, TVSIL_STATE);

运行以后，发现问题已经解决。只不过树控件的状态图标已经换成了自己的，可以弄得更好看一些。

以上代码的测试环境[win98 / vc6.0]

2006/2/17 iwaswzq


/////////////////////////////////////////////////////

添加下面两行代码就可以了
//////////////begin////////////
m_Tree.ModifyStyle( TVS_CHECKBOXES, 0 );
m_Tee.ModifyStyle( 0, TVS_CHECKBOXES );
/////////////////end///////////
HTREEITEM hRoot = m_Tree.InsertItem("Root");
 m_Tree.SetCheck(hRoot);
 m_Tree.InsertItem("Child1", hRoot);
 m_Tree.InsertItem("Child2", hRoot);
 m_Tree.Expand(hRoot, TVE_EXPAND );

