# 一步一步学List Control控件的用法（第二步）----创建第一行 - SHINING的博客 - CSDN博客
2010年03月17日 11:18:00[snsn1984](https://me.csdn.net/snsn1984)阅读数：15104
第二步创建第一行
1、在上一节中我们创建了一个列，那我们可以按照同样的方法多创建几个列。在
m_ListCtrl.InsertColumn(0, _T("姓名"), LVCFMT_LEFT, 90);
代码后边添入下列三行代码：
m_ListCtrl.InsertColumn(1,_T("性别"),LVCFMT_LEFT,40);
m_ListCtrl.InsertColumn(2,_T("联系方式"),LVCFMT_LEFT,160);
m_ListCtrl.InsertColumn(3,_T("备注"),LVCFMT_LEFT,180);
大家可以发现，只有第一个参数和最后一个参数不同，这两个参数的作用在上一节我们也介绍过了。第一个参数是新建列的索引值，是从0开始的，也就是说第一列的索引值是0，第二列的索引值是1，第三列的索引值是2，以此类推，是不是很像数组的下表啊？
2、大家可以运行下看看结果，会出现4个列，列名分别为：姓名，性别，联系方式，备注。
3、既然我们已经创建好了列，那我们来创建行吧。
4、首先要知道List Control在显示为Report样式时，主项为第一项，后面紧跟着的为子项。主项和子项的数据写入不同，这将在后面提到。所以说，在我们的例子当中姓名就是主项了，而其余的项就都是子项了。
5、在我们刚才添加的代码后边继续添加代码：
m_ListCtrl.InsertItem(0,_T("王二小"));
这里我们来看下这个函数的定义：
int InsertItem(const LVITEM* pItem)
int InsertItem(int nItem,LPCTSTR lpszItem)
int InsertItem(int nItem, LPCTSTR lpszItem,int nImage)
int InsertItem(UINT nMask,
               int nItem,
               LPCTSTR lpszItem,
               UINT nState,
               UINT nStateMask,
               int nImage,
               LPARAM lParam
              )
返回值：
如果成功，则返回新建列的索引值，否则为-1。
参数：
|pItem|指向指定项属性LVITEM结构的指针，如联机文档“平台SDK”中所述。|
|----|----|
|nItem|被插入项的索引值。|
|lpszItem|包含项标签的字符串的地址，或当项为回调项时，该变量为LPSTR_TEXT CALLBACK。要了解更多有关回调项的信息，请参阅CListCtrl::GetCall backMask.|
|nImage|项图象的索引值，或当项为回调项时，该变量为I_IMAGECALLBACK。要了解更多有关回调项的信息，请参阅CListCtrl::GetCallbackMask|
|nMask|参数nMask指定了哪个项属性作为参数传递是有效的。它可以是联机文档“平台SDK” 中LVITEM结构所描述的一个或多个掩码值。有效的数据能够通过位与运算来组合。|
|nState|指示项的状态、状态图象及轮廓图象。请参阅联机文档“平台SDK”中的LVITEM来获取更多信息，并参阅有效标志列表中的列表视图项状态。|
|nStateMask|指示状态成员中的哪一位将被获取或修改。请参阅联机文档“平台SDK”中的LVITEM来获取更多信息。|
|nImage|图象列表之内项的图象的索引。|
|lParam|与项相关联的应用指定的32位值。如果该参数被指定，那么必须设置nMask的属性为LVIF_PARAM。|
说明：向列表视图控件中新插入一项。
很显然，我们采用的是第二个函数，具有三个参数的函数。
6、我们这行代码的作用就是，插入主项：王二小，即在第一行第一列姓名中添加了王二小。
7、那我们要如何插入子项呢？我们继续添加代码如下：
m_ListCtrl.SetItemText(0,1,_T("男"));
我们先来看下这个函数的定义：
BOOL SetItemText(int nItem,int nSubItem,LPTSTR lpszText)
返回值：如果成功，则返回非零值，否则为0。
参数：
|nItem|要设定文本项的索引值。|
|----|----|
|nSubItem|子项的索引，若为零，则设置项标签。|
|lpszText|指向包含新项文本字符串的指针。|
说明：改变列表视图项或子项的文本。
这个就很好理解了，参数很少，这个就是为了添加子项。由于是子项，需要确定添加的位置，所以肯定需要二维的坐标来确定自己的位置，所以需要两个索引。类似于二维数组的下标。在我们的函数参数中，0，1，表示第1行第2列，而男就是要添加的内容了。
8、我们按照这个方法继续添加代码如下：
m_ListCtrl.SetItemText(0,2,_T("15888888888"));
m_ListCtrl.SetItemText(0,3,_T("儿童团"));
9、调试程序，我们将可以看到具有完整的一行数据的结果。如下图：
![第一行](http://hi.csdn.net/attachment/201003/17/33463_1268796014tnnN.gif)
10、同样，我们可以采取同样的方法，创建添加多行数据。
