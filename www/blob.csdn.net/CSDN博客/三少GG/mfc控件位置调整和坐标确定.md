# mfc控件位置调整和坐标确定 - 三少GG - CSDN博客
2012年05月03日 13:58:37[三少GG](https://me.csdn.net/scut1135)阅读数：2495标签：[mfc																[windows																[2010																[测试																[游戏																[工作](https://so.csdn.net/so/search/s.do?q=工作&t=blog)](https://so.csdn.net/so/search/s.do?q=游戏&t=blog)](https://so.csdn.net/so/search/s.do?q=测试&t=blog)](https://so.csdn.net/so/search/s.do?q=2010&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)](https://so.csdn.net/so/search/s.do?q=mfc&t=blog)
个人分类：[重构C/C++](https://blog.csdn.net/scut1135/article/category/621651)
### [http://my.oschina.net/u/131904/blog/39804](http://my.oschina.net/u/131904/blog/39804)
### 在mfc工程中控件或者窗口位置的调整是经常遇到的，特别是基于对话框的工程。位置的调整包括坐标、长度和宽度的变化，一般在窗口类的OnSize函数中实现。控件位置的调整涉及的函数有：GetWindowRect()、ScreenToClient()、GetClientRect()、MoveWindow()或SetWindowPos()，功能意义如下：
### GetWindowRect()：获得窗口在屏幕上的矩形坐标，调整控件位置时必须首先获得该屏幕坐标；
### ScreenToClient()：转换屏幕坐标到客户区坐标，如果为子窗口，转换后坐标为相对于父窗口的坐标，若为独立窗口，转换后客户区左上坐标为（0，0）；调整子窗口时这一步也是必须的；
### GetClientRect()：获得窗口客户区坐标，左上坐标永远为（0，0）；
### MoveWindow()：调整控件到指定位置；
### SetWindowPos()：调整控件的位置，该函数使用更灵活，多用于只修改控件位置而大小不变或只修改大小而位置不变的情况：
### 控件位置调整涉及的参变量有：主窗口的cx、cy坐标、参考点坐标。相对于调整位置时调用的函数，变量稍许复杂些。cx、cy坐标为主窗口的宽和高，有OnSize的参数给出，为窗口控件调整提供了变化的范围，所有的控件为了能够正常显示都不能超出这个范围。其实在开发过程中较难和重要的是参考控件的选择，其位置相对于主窗口来说必须好确定。常用的策略：
### 1．  选择主窗口上位置不随窗口大小变化的控件为参考；
### 2．  选择主窗口上控件的宽或者高固定的控件；
### 3．  选择与主窗口满足固定坐标关系的控件 ；
### 4．  选择主窗口上控件位置或者高宽容易确定的控件。
### 以上四种策略可在实际开发中作为参考!不管遇到什么的情况，一定要清楚：选择一个
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
MFC如何获取控件在对话框上的位置坐标
2010-09-13 20:20
本来希望的效果是在对话框上设置两个picture控件，分别显示两幅图像，然后将两幅图像中的相似部分利用一条直线连接起来。要实现这样的效果需要知道相似位置在这两幅图中的坐标以及这两个控件在对话框上的坐标，然后通过加减运算就可以得到图像上的相似区域在对话框的坐标，直接将这两个坐标用直线连接就可以了。                                                                                
为此，如何获得控件在对话框上的坐标是关键问题。编写了如下的测试小程序，目的是将两个picture控件中的点用直线连接起来，比较直观的是picture控件的四个角，所以程序中是将控件的拐角连接起来。首先在对话框上并排放置两个同样大小的picture控件，将他们的标识分别设成IDC_LEFT和IDC_RIGHT，然后添加两个编辑框用于显示picture控件的大小，给这两个编辑框添加相应的数据成员m_row和m_colume。添加一个按钮用于连接picture控件中的点，为这个按钮添加成员函数OnMatch()
 。
void CControlDlg::OnMatch() 
{
// TODO: Add your control notification handler code here
CRect rectL,rectR;
GetDlgItem(IDC_LEFT)->GetWindowRect(&rectL);//获取控件相对于屏幕的位置
ScreenToClient(rectL);//转化为对话框上的相对位置
GetDlgItem(IDC_RIGHT)->GetWindowRect(&rectR);//获取控件相对于屏幕的位置
ScreenToClient(rectR);//转化为对话框上的相对位置
m_row=rectL.bottom-rectL.top;
m_colume=rectL.right-rectL.left;
UpdateData(FALSE);
CClientDC dc(this);
dc.MoveTo(rectL.left,rectL.top);
dc.LineTo(rectR.right,rectR.bottom);
dc.MoveTo(rectL.right,rectL.top);
dc.LineTo(rectR.left,rectR.bottom);
dc.MoveTo(rectL.left+m_colume/2,rectL.top+m_row/2);//连接两个控件中心点
dc.LineTo(rectR.left+m_colume/2,rectR.top+m_row/2);
}
![](http://hiphotos.baidu.com/%C3%A8%D1%DB%D3%E3/pic/item/04d27d7cda98987d0dd7da46.jpg)
MFC中，如何获得对话框控件相对于父窗口（对话框窗口）的位置
创建者: nottoobad
最后修改: 2010-11-29 21:07:54
状态: 公开
标签:[mfc](http://www.jtben.com/list-files?userTagId=169831)
在MFC中，如何获得对话框控件相对于父窗口（对话框窗口）的位置:
CRect r;
pWnd->GetWindowRect(&r);
这样获得的r是控件相对于屏幕的坐标，然后用ScreenToClient(&r)就可以获得控件相对于父窗口的坐标。如果用GetClientRect(&r)的话，r.left和r.top始终是0,得到的并不是实际坐标。
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
以用GetSystemMetrics函数可以获取系统分辨率，但这只是其功能之一，GetSystemMetrics函数只有一个参数，称之为「索引」，这个索引有75个标识符，通过设置不同的标识符就可以获取系统分辨率、窗体显示区域的宽度和高度、滚动条的宽度和高度。
为了使使GetSystemMetrics的功能，我们以获取系统分辨率为例，并将其中的两个值用TextOut输出到窗体中。
第一步：用GetSystemMetrics获取屏幕的宽度和高度
- **int**x, y;
- x = GetSystemMetrics(SM_CXSCREEN);//屏幕宽度
- y = GetSystemMetrics(SM_CYSCREEN);//屏幕高度
**获取窗体显示区域大小**
已我现在的了解，获取窗体显示区域大小有三种方法。
第一种方法：使用GetSystemMetrics函数
- GetSystemMetrics(SM_CXFULLSCREEN);//获取最大化窗体的显示区域宽度
- GetSystemMetrics(SM_CYFULLSCREEN);//获取最大化窗体的显示区域高度
下面是GetSystemMetrics函数参数nIndex的定义：
SM_ARRANGE 返回是否预备最小化.
SM_CLEANBOOT 返回系统启动方式:
    0 正常启动
    1 安全模式启动
    2 网络安全模式启动
SM_CMOUSEBUTTONS 返回值为系统支持的鼠标键数，返回0，则系统中没有安装鼠标。
SM_CXBORDER,
SM_CYBORDER 返回以相素值为单位的Windows窗口边框的宽度和高度，如果Windows的为3D形态，则
等同于SM_CXEDGE参数
SM_CXCURSOR,
SM_CYCURSOR 返回以相素值为单位的标准光标的宽度和高度
SM_CXDLGFRAME,
SM_CYDLGFRAME 等同与SM_CXFIXEDFRAME and SM_CYFIXEDFRAME
SM_CXDOUBLECLK,
SM_CYDOUBLECLK 以相素值为单位的双击有效的矩形区域
SM_CXEDGE,SM_CYEDGE 以相素值为单位的3D边框的宽度和高度
SM_CXFIXEDFRAME,
SM_CYFIXEDFRAME 围绕具有标题但无法改变尺寸的窗口（通常是一些对话框）的边框的厚度
SM_CXFRAME,SM_CYFRAME 等同于SM_CXSIZEFRAME and SM_CYSIZEFRAME
SM_CXFULLSCREEN,
SM_CYFULLSCREEN 全屏幕窗口的窗口区域的宽度和高度
SM_CXHSCROLL,
SM_CYHSCROLL 水平滚动条的高度和水平滚动条上箭头的宽度
SM_CXHTHUMB 以相素为单位的水平滚动条上的滑动块宽度
SM_CXICON,SM_CYICON 系统缺省的图标的高度和宽度（一般为32*32）
SM_CXICONSPACING,
SM_CYICONSPACING 以大图标方式查看Item时图标之间的间距，这个距离总是大于等于
SM_CXICON and SM_CYICON.
SM_CXMAXIMIZED,
SM_CYMAXIMIZED 处于顶层的最大化窗口的缺省尺寸
SM_CXMAXTRACK,
SM_CYMAXTRACK 具有可改变尺寸边框和标题栏的窗口的缺省最大尺寸，如果窗口大于这个
尺寸，窗口是不可移动的。
SM_CXMENUCHECK,
SM_CYMENUCHECK 以相素为单位计算的菜单选中标记位图的尺寸
SM_CXMENUSIZE,
SM_CYMENUSIZE 以相素计算的菜单栏按钮的尺寸
SM_CXMIN,SM_CYMIN 窗口所能达到的最小尺寸
SM_CXMINIMIZED,
SM_CYMINIMIZED 正常的最小化窗口的尺寸
SM_CXMINTRACK,
SM_CYMINTRACK 最小跟踪距离，当使用者拖动窗口移动距离小于这个值，窗口不会移动。
SM_CXSCREEN,
SM_CYSCREEN 以相素为单位计算的屏幕尺寸。
SM_CXSIZE,SM_CYSIZE 以相素计算的标题栏按钮的尺寸
SM_CXSIZEFRAME,
SM_CYSIZEFRAME 围绕可改变大小的窗口的边框的厚度
SM_CXSMICON,
SM_CYSMICON 以相素计算的小图标的尺寸，小图标一般出现在窗口标题栏上。
M_CXVSCROLL,
SM_CYVSCROLL 以相素计算的垂直滚动条的宽度和垂直滚动条上箭头的高度
SM_CYCAPTION 以相素计算的普通窗口标题的高度
SM_CYMENU 以相素计算的单个菜单条的高度
SM_CYSMCAPTION 以相素计算的窗口小标题栏的高度
SM_CYVTHUMB 以相素计算的垂直滚动条中滚动块的高度
SM_DBCSENABLED 如果为TRUE或不为0的值表明系统安装了双字节版本的USER.EXE,为FALSE或0则不是。
SM_DEBUG 如果为TRUE或不为0的值表明系统安装了debug版本的USER.EXE,为FALSE或0则不是。
SM_MENUDROPALIGNMENT 如果为TRUE或不为0的值下拉菜单是右对齐的否则是左对齐的。
SM_MOUSEPRESENT 如果为TRUE或不为0的值则安装了鼠标，否则没有安装。
SM_MOUSEWHEELPRESENT 如果为TRUE或不为0的值则安装了滚轮鼠标，否则没有安装。(Windows NT only)
SM_SWAPBUTTON 如果为TRUE或不为0的值则鼠标左右键交换，否则没有。
## Invalidate（TRUE）的闪屏问题
(2010-05-03 02:58:24)
![](http://simg.sinajs.cn/blog7style/images/common/sg_trans.gif)转载▼
|标签：### [杂谈](http://search.sina.com.cn/?c=blog&q=%D4%D3%CC%B8&by=tag)|分类：[MFC小游戏](http://blog.sina.com.cn/s/articlelist_1627005024_1_1.html)|
    在使用**Invalidate(TRUE)**进行窗口重绘时，总是会遇到闪屏的问题。
    一开始以为是绘图速度过慢照成的，但在对绘图时间做了一个测试之后发现，**即使整个绘图过程只持续了几个毫秒，还是会看见很明显的闪烁****，所以时间并不是造成闪烁的决定性因素**。
    那到底是什么原因呢？现在来看看Invalidate(TRUE)都干了些什么。其实，它只是间接向消息队列添加了**WM_ERASEBKGND**和WM_PAINT两个消息。但是，如果使用Invalidate(FALSE)的话，则只有WM_PAINT消息产生，这时是不会有任何闪烁的。
    现在看来，闪烁似乎是由WM_ERASEBKGND消息产生的，事实上，的确与它有关。那WM_ERASEBKGND有干了什么呢？WM_ERASEBKGND消息由OnEraseBkgnd()消息处理函数响应，它的作用就是**重绘客户区背景**。我们可以通过向工程里添加WM_ERASEBKGND这个消息，然后在重写的消息处理函数中将返回语句修改为**return TRUE**来屏蔽这一功能，这样做的好处是这时不会重绘背景了，坏处是这时背景也不会被擦出来。
    好像还没有说到真实原因，其实真正的原因就隐含在其中。现在来做一个实验，分别尝试一下快速的眨眼和慢速的眨眼，你会发现快速眨眼时我们会感觉眼前的黑色一闪而过，而慢速眨眼时，则会觉得整个过程是连续的，没有什么异样。其实闪烁也就是这么回事，即**多张不连续图像的快速切换**。这里有三个条件，**多张和快速和不连续**，而且需要**同时具备**才会发生闪烁。如果只是两张，只会感觉到突变，还谈不上闪烁；如果频率慢的话，也相当于两张图像的情况了；最后如果是连续图像的话，那就像是看电影，平稳的过渡也不会让人觉得不适。
    知道了这些，接下来就可以做决策了。
**解决方案**：
    使用Invalidate(FALSE),添加WM_ERASEBKGND消息处理函数或者局部刷新三者选其一，都是可以解决问题的。它们的都是通过除去图像不连续这一因素来达到目的的。
    另外，要说的是GDI的BitBlt()函数是及其高效的，一次操作所需要的时间只有几到十几个微秒，所以我们可以放心的使用它，而不用担心任何效率问题。不过相对于BitBlt()来说StretchBlt()就要慢的多，大概是几十倍的差别。
    还有就是一般的绘图工作都是先绘制在一个缓冲区上，然后再一次拷贝到屏幕上。
    有时，当我们需要利用闪烁的效果的话，也是可以通过多张图像的快速切换来做到，在这里我们也将两张图像的重复切换理解为多张图像。
