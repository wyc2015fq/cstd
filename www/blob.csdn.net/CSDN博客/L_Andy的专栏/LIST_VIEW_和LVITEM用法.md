# LIST_VIEW_和LVITEM用法 - L_Andy的专栏 - CSDN博客

2012年11月07日 10:52:42[卡哥](https://me.csdn.net/L_Andy)阅读数：5728


ListCtrl在工作中，常常用到，也常常看到大家发帖问怎么用这个控件，故总结了一下自己的使用经验，以供参考使用。
先注明一下，这里，我们用m_listctrl来表示一个CListCtrl的类对象，
然后这里我们的ListCtrl都是report形式，至于其他的如什么大图标，小图标
的暂时不讲，毕竟report是大众话的使用。其次，我们这里用条款一，条款二
来描述第一点，第二点，这个是参照《Effective C++》的叫法，俺觉得这么
叫比较COOL :)
条款一：设置ListCtrl的风格
在CSDN上常常看到有人问怎么设置风格的，他们ListCtrl的样子是一个列表
，有横条和竖条分界线，然后选中一行，要整一行都选中，而不是只有某一列
被选中，等等，这里给一个比较全面的设置方法。
//获得原有风格
DWORD dwStyle = ::GetWindowLong(m_listctrl.m_hWnd, GWL_STYLE);

dwStyle &= ~(LVS_TYPEMASK);

dwStyle &= ~(LVS_EDITLABELS);

//设置新风格
     SetWindowLong(m_listctrl.m_hWnd, GWL_STYLE, 

dwStyle,|LVS_REPORT|LVS_NOLABELWRAP|LVS_SHOWSELALWAYS);

//设置扩展风格
DWORD styles = 

LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_CHECKBOXES;

ListView_SetExtendedListViewStyleEx(m_listctrl.m_hWnd, styles, 

styles );
其中LVS_EX_FULLROWSELECT　就是前面说得整行选中
LVS_EX_GRIDLINES　网格线（只适用与report风格的listctrl）
LVS_EX_CHECKBOXES　前面加个checkbox

pListCtrl->SetExtendedStyle(   m_listctrl.GetExtendedStyle()

|LVS_EX_SUBITEMIMAGES);

这也是一个很重要的属性，这样的话，可以在列表中加ICON，记得windows的
任务管理器吗，你想做得那样，这个属性也要加哦，这个我以后会讲的～
条款二：加入列头
这是一个比较实质的东西，给列表框分列，然后加上列头
代码说话，来了
TCHAR rgtsz[2][10] = {_T("列头1"), _T("列头2")};
LV_COLUMN lvcolumn;

CRect rect;

m_listctrl.GetWindowRect(&rect);

for(int i=0;i<2;i++)

{
lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT

 | LVCF_WIDTH | LVCF_ORDER;
lvcolumn.fmt = LVCFMT_LEFT;
lvcolumn.pszText = rgtsz[i];
 lvcolumn.iSubItem = i;
lvcolumn.iOrder = i;
 if(i==0)
{
lvcolumn.cx = rect.Width()*3/5 ; 
 }
 else
lvcolumn.cx = rect.Width()*2/5;

m_listctrl.InsertColumn(i, &lvcolumn);
}

这是插入两列的做法，你要插入20列？？随便你，依样画葫芦～～
lvcolumn.mask 中那个mask可以有各种属性，具体去看msdn吧，
条款三：把记录，插入列表框中
int nIndex = m_listctrl.GetItemCount();

   LV_ITEM    lvitemAdd = {0};

   lvitemAdd.mask = LVIF_TEXT;

   lvitemAdd.iItem = nIndex ;

   lvitemAdd.iSubItem = 0;

   lvitemAdd.pszText =_T("毛毛1");;

   if (m_listctrl.InsertItem(&lvitemAdd) != -1)

   { 

    LV_ITEM lvitem = {0};

    lvitem.mask = LVIF_TEXT;

    lvitem.iItem = nIndex ;

    lvitem.iSubItem = 1;

    lvitem.pszText =_T("毛毛2");

    m_listctrl.SetItem(&lvitem);

   }

nIndex 是当前的行数，然后把新的一行，插在最下面，
条款四：给列表中插入图标
在report格式中，也能插入图标
继续代码说话
m_image是个CImageList对象
m_image.Create(16,16, TRUE|ILC_COLOR24, 3, 1);

m_listctrl.SetImageList(&m_image,LVSIL_SMALL);
然后调用CImageList的成员函数int CImageList::Add( HICON hIcon );

把ICON插入到imagelist，
然后在插入记录的时候
lvitemAdd.mask = LVIF_TEXT;　－》　lvitemAdd.mask =

LVIF_TEXT|LVIF_IMAGE

然后添加一个lvitemAdd.iImage = n;

这个n是imagelist中的序号，表示是具体的哪一个图标，list么，呵呵
条款五：　插入记录时使用额外的信息，lParam 的使用
有时候，你想对于某一行，加入一些额外的信息，那么就可以使用这个
lParam 
ｍｓｄｎ是这么描述的Specifies the 32-bit value of the item

我上次是为了在某一行加入一个信息，窗口句柄，然后是这么加的，
int nIndex = m_listctrl.GetItemCount();

   LV_ITEM    lvitemAdd = {0};

   lvitemAdd.mask = LVIF_TEXT|LVIF_IMAGE|LVIF_PARAM;

   lvitemAdd.iItem = nIndex ;

   lvitemAdd.iSubItem = 0;

   lvitemAdd.pszText =_T("毛毛1");;

   lvitemAdd.iImage = n;

   lvitemAdd.lParam = (LPARAM)hwnd;(某个窗口的窗口句柄)

   if (m_listctrl.InsertItem(&lvitemAdd) != -1)

   { 

    LV_ITEM lvitem = {0};

    lvitem.mask = LVIF_TEXT;

    lvitem.iItem = nIndex ;

    lvitem.iSubItem = 1;

    lvitem.pszText =_T("毛毛2");

    m_listctrl.SetItem(&lvitem);

   }

ok，这是一个比较全的例子的，又插ICON，又使用PARAM的
条款六　：　点击列表框，获取选中行信息
响应NM_CLICK消息，如果你有MSDN，可以看到，有专门关于listview的
NM_CLICK的介绍
void CMyDlg::OnItemClick(NMHDR* pNMHDR, LRESULT* pResult)

{

// TODO: Add your control notification handler code here

int nItem = -1;

LPNMITEMACTIVATE lpNMItemActivate = (LPNMITEMACTIVATE)pNMHDR;

if(lpNMItemActivate != NULL)

{

      nItem = lpNMItemActivate->iItem;

}

}
现在nItem就是点击选中那行的index了，有了index，获取那行的信息还难吗
？
懒汉说：难，因为你还没讲，晕，那就继续说
条款七：　根据行的index，获取该行的信息
直接上代码吧
LV_ITEM lvitem = {0};

    lvitem.iItem = nIndex;

    lvitem.iSubItem = 0;

    lvitem.mask = LVIF_TEXT|LVIF_IMAGE|LVIF_PARAM;

m_listctrl.GetItem(&lvitem)
这样，就把nindex,第一列的信息取出来了，包括刚才我们加入的ICON,和那个
额外信息（窗口句柄），
比如我要获取窗口句柄，就可以hwnd = (HWND)lvitem.lParam;

mask 用来指明你想获取那些信息
具体可以查msdn中LVITEM Structure的定义和CListCtrl::GetItem
条款八：用程序选中某一行，使之选中
选中之　
m_listctrl.SetItemState

(nIndex,LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED);
不选中，取消选中之
m_listctrl.SetItemState(nIndex,0,LVIS_SELECTED|LVIS_FOCUSED);
条款九：获取当前所有选中的行（多选）
这个，俺就比较懒了，抄msdn的代码吧，反正很简单
Example

// CListCtrl* pListCtrl = (CListCtrl*) GetDlgItem

(IDC_YOURLISTCONTROL);

ASSERT(pListCtrl != NULL);

POSITION pos = pList->GetFirstSelectedItemPosition();

if (pos == NULL)

    TRACE0("No items were selected!\n");

else

{

   while (pos)

   {

      int nItem = pList->GetNextSelectedItem(pos);

      TRACE1("Item %d was selected!\n", nItem);

      // you could do your own processing on nItem here

   }

}
条款十：删除条款九中选中的行
这个相对前面九个条款是比较麻烦的，因为如果你要删除多行的话。往往要出错
比如，我现在要删除第0行和第1行（列表的行序列是从0开始的）
那么好啊。我来删了
m_listctrl.DeleteItem(0)

         m_listctrl.DeleteItem(1)
恭喜你，错了，我好开心啊　：）
因为你删除第0行以后，下面的行会往上移，那么原来的第1行就变成了第0行，那么你再 m_listctrl.DeleteItem(1)，那么删除的是原来的第２行，真麻烦，
所以，只有从下往上删，才是安全的，先删的，不会影响后面的操作，
m_listctrl.DeleteItem(1)

         m_listctrl.DeleteItem(0)

但有时候，我们也不知道要删除哪些行，只知道要删除选中的那些行，像条款九中的那些
如果我们还是用
POSITION pos = m_listctrl.GetFirstSelectedItemPosition();

if (pos == NULL)

    TRACE0("No items were selected!\n");

else

{

   while (pos)

   {

      int nItem = m_listctrl.GetNextSelectedItem(pos);

m_listctrl.DeleteItem(nItem );

   }

}
你就等着收尸吧
这时候我们就要B4微软了，为虾米木有GetLastselectedItemPosition 和GetPrevSelectedItem

多写一对成员函数会死啊　:(

没办法，办法自己想，这里有个笨办法
POSITION   sSelPos = NULL;

while(sSelPos = m_listctrl.GetFirstSelectedItemPosition())

{

         int nSelItem = -1;

          nSelItem = m_listctrl.GetNextSelectedItem(sSelPos);

          if(nSelItem >= 0 && nSelItem<m_listctrl.GetItemCount())

        {

好了，这个nSelItem 就是我们要的DD

       }

}

GetNextSelectedItem这个函数，看msdn的用法，其实是返回第一个的index，然后走到下一个选中的行去，所以这么做也是安全的，在实际中，俺也是这么做的，测试也通过，没问题的
当然，还有个办法，先通过GetFirstSelectedItemPosition和GetNextSelectedItem

来获取所有的选中行的index，然后把这些index放到一个数组里，然后再从下往上删
唉真麻烦啊，还要不定数组，不说用new在堆上开吧，那么一个vector总是要的吧，麻烦啊
所以我暂时是用上述的办法来删除，也供大家参考，希望能找到更好的办法。
好了，今天就暂时先整理上述十点。listctrl其他高级的用法，网上资料也很多，我也暂时不总结，以后有机会再说吧。
搞了好长时间啊，要睡觉去了，累死了　：）

