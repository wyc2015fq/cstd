# 一步一步学List Control控件的用法（第三步）----设置风格 - SHINING的博客 - CSDN博客
2010年03月17日 14:57:00[snsn1984](https://me.csdn.net/snsn1984)阅读数：11617
# 第三步设置风格
1、通过第一步和第二步，我们已经创建出了一个有一行并且有四列的内容的REPORT风格的List Control。可是，只是简单的方框中有一行数据而已，我们能不能让它们看起来更漂亮呢？或者我们能不能让它们可以被选取从而让我们可以做一些别的操作呢？当然可以，这一步我们就来看看简单的风格设置。
2、要学习风格设置，我们首先来看一个函数：
CListCtrl::SetExtendedStyle
DWORD SetExtendedStyle(DWORD dwNewStyle)
返回值：
返回列表视图控件使用的以前扩展风格的组合。
参数： 
|dwNewStyle|列表视图控件使用的扩展风格的组合。若希望进一步了解这些风格，请参阅联机文档“平台SDK”中的“扩展列表视图风格”|
说明：
该成员函数实现了Win32宏ListView_SetExtentedListViewStyle的功能，如同联机文档“平台SDK”中所描述的。
对了，这就是我们需要的函数，只要调用它，我们就要可以设置风格了。下面我们一起来试试吧。
3、只是这个函数的参数怎么给呢？只能求助于MSDN了，看看我从MSDN中找到了什么吧。
**LVS_EX_AUTOAUTOARRANGE**
Windows Vista. Automatically arrange icons if no icon positions have been set (Similar to LVS_AUTOARRANGE). 
**LVS_EX_AUTOCHECKSELECT**
**Windows Vista**. Automatically select check boxes on single click. 
**LVS_EX_AUTOSIZECOLUMNS**
**Windows Vista**. Automatically size listview columns. 
**LVS_EX_BORDERSELECT**
**Version 4.71**. Changes border color whan an item is selected, instead of highlighting the item. 
**LVS_EX_CHECKBOXES**
**Version 4.70**. Enables check boxes for items in a list-view control. When set to this style, the control creates and sets a state image list with two images using DrawFrameControl. State image 1 is the unchecked box, and state image 2 is the checked box. Setting the state image to zero removes the check box.
**Version 6.00** Check boxes are visible and functional with all list view modes except the tile view mode introduced in ComCtl32.dll version 6. Clicking a checkbox in tile view mode only selects the item; the state does not change.
You can obtain the state of the check box for a given item with ListView_GetCheckState. To set the check state, use ListView_SetCheckState. If this style is set, the list-view control automatically toggles the check state when the user clicks the check box or presses the space bar.
**LVS_EX_COLUMNOVERFLOW**
Indicates that an overflow button should be displayed in icon/tile view if there is not enough client width to display the complete set of header items. The list-view control sends the LVN_COLUMNOVERFLOWCLICK notification when the overflow button is clicked. This flag is only valid when LVS_EX_HEADERINALLVIEWS is also specified. 
**LVS_EX_COLUMNSNAPPOINTS**
**Windows Vista**. Snap to minimum column width when the user resizes a column. 
**LVS_EX_DOUBLEBUFFER**
**Version 6.00**. Paints via double-buffering, which reduces flicker. This extended style also enables alpha-blended marquee selection on systems where it is supported. 
**LVS_EX_FLATSB**
**Version 4.71**. Enables flat scroll bars in the list view. If you need more control over the appearance of the list view's scroll bars, you should manipulate the list view's scroll bars directly using the Flat Scroll Bar APIs. If the system metrics change, you are responsible for adjusting the scroll bar metrics with FlatSB_SetScrollProp. See Flat Scroll Bars for further details. 
**LVS_EX_FULLROWSELECT**
**Version 4.70**. When an item is selected, the item and all its subitems are highlighted. This style is available only in conjunction with the LVS_REPORT style. 
**LVS_EX_GRIDLINES**
**Version 4.70**. Displays gridlines around items and subitems. This style is available only in conjunction with the LVS_REPORT style. 
**LVS_EX_HEADERDRAGDROP**
**Version 4.70**. Enables drag-and-drop reordering of columns in a list-view control. This style is only available to list-view controls that use the LVS_REPORT style. 
**LVS_EX_HEADERINALLVIEWS**
**Windows Vista**. Show column headers in all view modes. 
**LVS_EX_HIDELABELS**
**Windows Vista**. Hides the labels in icon and small icon view. 
**LVS_EX_INFOTIP**
**Version 4.71**. When a list-view control uses the LVS_EX_INFOTIP style, the LVN_GETINFOTIP notification message is sent to the parent window before displaying an item's ToolTip. 
**LVS_EX_JUSTIFYCOLUMNS**
**Windows Vista**. Icons are lined up in columns that use up the whole view. 
**LVS_EX_LABELTIP**
**Version 5.80**. If a partially hidden label in any list view mode lacks ToolTip text, the list-view control will unfold the label. If this style is not set, the list-view control will unfold partly hidden labels only for the large icon mode. 
**LVS_EX_MULTIWORKAREAS**
**Version 4.71**. If the list-view control has the LVS_AUTOARRANGE style, the control will not autoarrange its icons until one or more work areas are defined (see LVM_SETWORKAREAS). To be effective, this style must be set before any work areas are defined and any items have been added to the control. 
**LVS_EX_ONECLICKACTIVATE**
**Version 4.70**. The list-view control sends an LVN_ITEMACTIVATE notification message to the parent window when the user clicks an item. This style also enables *hot tracking* in the list-view control. Hot tracking means that when the cursor moves over an item, it is highlighted but not selected. See the **Extended List-View Styles** Remarks section for a discussion of item activation. 
**LVS_EX_REGIONAL**
**Version 4.71**. Sets the list view window region to include only the item icons and text using SetWindowRgn. Any area that is not part of an item is excluded from the window region. This style is only available to list-view controls that use the LVS_ICON style. 
**LVS_EX_SIMPLESELECT**
**Version 6.00**. In icon view, moves the state image of the control to the top right of the large icon rendering. In views other than icon view there is no change. When the user changes the state by using the space bar, all selected items cycle over, not the item with the focus. 
**LVS_EX_SINGLEROW**
**Windows Vista**. Not used. 
**LVS_EX_SNAPTOGRID**
**Windows Vista**. In icon view, icons automatically snap into a grid. 
**LVS_EX_SUBITEMIMAGES**
**Version 4.70**. Allows images to be displayed for subitems. This style is available only in conjunction with the LVS_REPORT style. 
**LVS_EX_TRACKSELECT**
**Version 4.70**. Enables hot-track selection in a list-view control. Hot track selection means that an item is automatically selected when the cursor remains over the item for a certain period of time. The delay can be changed from the default system setting with a LVM_SETHOVERTIME message. This style applies to all styles of list-view control. You can check whether hot-track selection is enabled by calling SystemParametersInfo. 
**LVS_EX_TRANSPARENTBKGND**
**Windows Vista**. Background is painted by the parent via WM_PRINTCLIENT. 
**LVS_EX_TRANSPARENTSHADOWTEXT**
**Windows Vista**. Enable shadow text on transparent backgrounds only. 
**LVS_EX_TWOCLICKACTIVATE**
**Version 4.70**. The list-view control sends an **LVN_ITEMACTIVATE** notification message to the parent window when the user double-clicks an item. This style also enables *hot tracking* in the list-view control. Hot tracking means that when the cursor moves over an item, it is highlighted but not selected. See the **Extended List-View Styles** Remarks section for a discussion of item activation. 
**LVS_EX_UNDERLINECOLD**
**Version 4.71**. Causes those non-hot items that may be activated to be displayed with underlined text. This style requires that LVS_EX_TWOCLICKACTIVATE be set also. See the **Extended List-View Styles** Remarks section for a discussion of item activation. 
**LVS_EX_UNDERLINEHOT**
**Version 4.71**. Causes those hot items that may be activated to be displayed with underlined text. This style requires that LVS_EX_ONECLICKACTIVATE or LVS_EX_TWOCLICKACTIVATE also be set. See the **Extended List-View Styles** Remarks section for a discussion of item activation.
这就是可以赋给形参的值。怎么找到呢？
4、我们先在MSDN中查找ClistCtrl：：SetExtendedStyle，找到后，我们发现里面有介绍它的形参dwNewStyle的，并且介绍中有这么一句话：For a descriptive list of these styles, see the Extended List View Styles topic in the Windows SDK.其中Extended List View Styles是可以点击的，我们点击它就得到了我们想要的内容。
5、这么多，看起来好像无从下手的样子，那我们就挑几个常用的开始吧：
**LVS_EX_FULLROWSELECT****选中某行使整行高亮（只适用与****report****风格的****listctrl****）****LVS_EX_GRIDLINES****网格线（只适用与****report****风格的****listctrl****）****LVS_EX_CHECKBOXESitem****前生成****checkbox****控件**
将参数放入刚才介绍的函数中，就可以达到对应的效果了，不信吗？现在就动手试试吧，记住别敲错了哦。
6、如果想几种风格都要怎么办？有过WINDOWS编程经验的人一定马上就知道了，把它们用“|”连接起来。用上述3个做例子：
SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT|LVS_EX_CHECKBOXES)
7、那还等什么，赶快加入到我们前面的代码中去吧。别忘了，加入的代码应该是：
m_ListCtrl.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT|LVS_EX_CHECKBOXES);
8、插入位置在哪里呢？当然还是那个老地方了，和前面两步我们放代码的地方一样。在ClistCtrlDlg：：OnInitDialog(void)中。那是放在我们前两步代码前还是后呢？前后都可以，只是最好放在前面吧，这样有利于提高代码的可读性。
9、好了，那我们来一起看看最后的结果吧。
![设置风格](http://hi.csdn.net/attachment/201003/17/33463_1268809007zB37.gif)
10、还想要其他的风格吗？自己按照列表添加吧。
