# WINCE应用的UI实现方案 —— 中篇：GWES方案上几技术难点的解决 - xqhrs232的专栏 - CSDN博客
2010年07月08日 15:11:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1012标签：[wince																[ui																[button																[dialog																[styles																[properties](https://so.csdn.net/so/search/s.do?q=properties&t=blog)](https://so.csdn.net/so/search/s.do?q=styles&t=blog)](https://so.csdn.net/so/search/s.do?q=dialog&t=blog)](https://so.csdn.net/so/search/s.do?q=button&t=blog)](https://so.csdn.net/so/search/s.do?q=ui&t=blog)](https://so.csdn.net/so/search/s.do?q=wince&t=blog)
个人分类：[DirectUI界面技术](https://blog.csdn.net/xqhrs232/article/category/1243806)
原文地址::[http://chxy85.spaces.live.com/blog/cns!FC72FD5092CCD2F8!344.entry](http://chxy85.spaces.live.com/blog/cns!FC72FD5092CCD2F8!344.entry)

4月24日
#### WINCE应用的UI实现方案 —— 中篇：GWES方案上几技术难点的解决
这里谈论的所谓技术难点，其实根本不值一提。只不过微软定了一套游戏规则，我们目前不清楚这套游戏规则，花时间去摸索而已。
1、BUTTON的动画效果
我们用了GWES里提供的BUTTON类，在WINCE PRODUCT DOCUMENT里的位置是
ms-help://MS.WindowsCE.500/wceshellui5/html/wce50grfButtonReference.htm
里面并没有给出太多的说明，在Button Messages里提到有WM_CTLCOLORBTN消息, 但简单试用后发现和预期效果不符。我乱翻乱点时注意到了eVC在画图时，对BUTTON点右键出的菜单里，打开Properties，里面的Styles页有个复选框"Owner draw", 我就抓住这根稻草，GOOGLE一把，方法就出来了。
当Owner draw属性被勾选时，轮到该BUTTON绘图时，程序就不会跑DefDlgProc去画个灰色突出的效果并把按钮名字写上去，而是给BUTTON的父窗口，也就是DIALOG的PROC发个消息WM_DRAWITEM，并且所带的lParam中有我们需要的所有东西。来个强制转换
LPDRAWITEMSTRUCT lpDIS = (LPDRAWITEMSTRUCT)lParam;
然后根据 lpDIS->itemAction 和 lpDIS->itemState 判断BUTTON当前状态，以决定加载哪幅图片 
DRAWITEMSTRUCT的详细说明请参考 ms-help://MS.WindowsCE.500/wceshellui5/html/wce50lrfdrawitemstruct.htm
我简单试了了一下，
(1) 初始化时
 itemAction == 1；  // ODA_DRAWENTIRE
 itemState == 16；  // ODS_FOCUS
(2) BUTTON被按下时 
 itemAction == 2；  // ODA_SELECT
 itemState == 17；  // ODS_FOCUS | ODS_SELECTED
(3) BUTTON被点击后松开时
 itemAction == 2；  // ODA_SELECT
 itemState == 16；  // ODS_FOCUS
ODA和ODS宏定义数值
 /*** from Winuser.h ***/
 #define ODT_MENU        1
 #define ODT_LISTBOX     2
 #define ODT_COMBOBOX    3
 #define ODT_BUTTON      4
 //action
 #define ODA_DRAWENTIRE  0x0001
 #define ODA_SELECT      0x0002
 #define ODA_FOCUS       0x0004
 //state
 #define ODS_SELECTED    0x0001
 #define ODS_GRAYED      0x0002
 #define ODS_DISABLED    0x0004
 #define ODS_CHECKED     0x0008
 #define ODS_FOCUS       0x0010 
在WM_DRAWITEM中有两点要特别注意
(1) 不能在里面用InvalidateRect(lpDIS->hwndItem, lpDIS->rcItem, NULL),这会立即再发一个WM_DRAWITEM消息过来，接着再调InvalidateRect, 进入死循环直至把设备上的内存耗光，导致死机
(2) 对itemAction和itemState作判断时，必须把两者同时都判断了才能确定CLICK状态，单独判断action或单独判断state是不够的，会导致重绘作用在不希望发生的情况下。并且不能简单地作itemAction & ODA_SELECT这样的位与判断，还必须有排他性，我干脆就用==号了。
参考代码如下
 1BOOL CALLBACK DialogProc (HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
 2{
 3switch(uMsg)
 4    {
 5case WM_DRAWITEM:
 6        {
 7            LPDRAWITEMSTRUCT lpDIS = (LPDRAWITEMSTRUCT)lParam;
 8
 9if(lpDIS->CtlID == IDC_BTN_1)
10            {
11
12if( (lpDIS->itemAction & ODA_DRAWENTIRE)   //这是初始化时的情况
13||    ( (lpDIS->itemAction == ODA_SELECT) && (lpDIS->itemState == ODS_FOCUS) ) ) //这是按下后放开时的情况
14                {
15//画上未被按下时的图片
16                }
17elseif( (lpDIS->itemAction & ODA_SELECT) && (lpDIS->itemState == (ODS_FOCUS | ODS_SELECTED) ) ) //被按下时的情况
18                {
19//画上被按下时的图片
20                    MessageBeep(MB_OK); //顺便响一声，比较有手感
21                }
22            }
23return TRUE;
24        }
25default:
26//STUB
27break;
28    }
29
30return FALSE;
31}
2、DLU和PIXEL的单位转换
本来以为做完BUTTON效果后就OVER了，结果今天傍晚时候遇到一个很恼火的问题。在VC / eVC / VS中画的对话框、按钮等控件时，在IDE右下角显示的 XX * XX单位是DLU (Dialog Unit), 这是根据你设置的对话框字体大小而改变的。这种做法无可厚非。如果把字体改大了，那么DIALOG和BUTTON自然也被“撑”大了，比较灵活。但是我往上叠加的图片是按像素(PIXEL)来算的。最后实现出来，有两个方法。按照国际惯例，当然是先讲笨的方法，MSDN上的作风也是如此。
(1)方法一：我查了下DLU和PIXEL之间的换算关系，有个讲得比较全的网页是[http://support.microsoft.com/default.aspx?scid=kb;en-us;145994](http://support.microsoft.com/default.aspx?scid=kb;en-us;145994)
 (How to caculate dialog box units based on the current font in Visual C++) 按照文中的Method Two简单测试了一下
 1BOOL CALLBACK DialogProc (HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
 2{
 3switch(uMsg)
 4    {
 5case WM_INITDIALOG:
 6        {
 7            RECT rc={0,0,4,8};
 8
 9            MapDialogRect(hDlg, &rc );
10            printf("baseUnitX = %ld/n", rc.right);
11            printf("baseUnitY = %ld/n", rc.bottom);
12
13//DLU_Weight = pixel_weight * 4 / baseunitX;
14//DLU_Height = pixel_height * 8 / baseunitY;
15
16return TRUE;
17        }
18    }
19
20return FALSE;
21}
对于我DIALOG中设置的Courier New 10pt字体，
**第一步**：从代码中得到baseUnitX = 7, baseUnitY = 16,
**第二步**：套用公式
 weight_DLU = weight_pixel * 4 / baseUnitX
 height_DLU = height_pixel * 8 / baseUnitY
 比如我想建立一个320*240 PIXEL的对话框，那么
 weight_DLU = 320 * 4 / 7 = 182.86, 取整为183
 height_DLU = 240 * 8 / 16 = 120
**第三步**：到VC里去拖动对话框的边界，画出183*120的对话框，那么代码运行起来后，通过GetClientRect查一下，的确是得到320*240 pixel的对话框了。
但是这种方法有一个致命的缺点。比如我要画一个60*70的按钮，按照上面的baseUnitX和baseUnitY折算后应该为34.28 * 35,但是我画34*35 DLU时，运行得到59*70 PIXEL窗口；画35*35 DLU时，运行得到61*70 PIXEL窗口，无法恰到好处，这导致了往上叠加60*70 pixel的图片时，按钮边缘会出现不连续的黑点。所以这段文字只能当作DLU和PIXEL的换算关系来玩了，没有任何实际应用价值。
(2) 方法二：正确的方法
 其实也很简单。晚饭后突然想到的，用SetWindowPos强制设置window的长宽和左上角坐标。我在WM_DRAWITEM消息的处理中简单试了一下：
SetWindowPos(lpDIS->hwndItem, NULL, 10, 10, 60, 70, SWP_NOZORDER);
设置前是59*70的按钮，设置后就是60*70了，并且叠加图片没有任何问题。OK，整个基于GWES的UI方案至此成型了。后面貌似没有什么大的技术障碍了。
至此，我没有发现GWES方案上还有什么路障了，可以拍板使用这套方案了，和只用GDI写UI相比，软件研发的工作量大概降低了30%左右。当然事情还没有就这样结束，这套方案对我这项目组的意义是很深远的。请看下篇：代码中的一小步，项目进度管理上的一大步
以下还有网友留言和博主回复，感觉也不错一并发上来： 
有个问题想请教！ 为什么我在WM_DRAWITEM消息中使用StretchBlt画图片，第一次时比实际图片的颜色暗很多，我需要响应自己的按键重新发WM_DRAWITEM图片才变回原来的颜色。 如何才能在第一次显示按键时图片不变暗。
如果我说的不是很清楚，我可以把代码过去。 不知道发到哪里比较好，请留言。 
## [#7楼](http://chxy85.spaces.live.com/walzer/archive/2007/04/08/704996.html#1253532)[楼主]   [回复](http://chxy85.spaces.live.com/walzer/archive/2007/04/08/704996.html#post)[引用](http://chxy85.spaces.live.com/walzer/archive/2007/04/08/704996.html#post)[查看](http://www.cnblogs.com/CommentsByAuthor.aspx?author=Walzer)
2008-07-14 09:09 by [Walzer](http://www.cnblogs.com/walzer/)[](http://space.cnblogs.com/msg/send/Walzer)
@chenxin 
我自己没有用过StretchBlt这个函数，这个运行时比较费时间. 一般都是直接在PS里把图片拉到程序里适合的大小,然后用BitBlt画原始大小上去. 或许你可以先试一下用BitBlt的方法，看看是否是StretchBlt函数的调用方法错误导致的.
然后就是查一下WM_PAINT, WM_ACTIVATE这些消息响应里干了什么, 你说的情况区别就在于WM_PAINT, WM_ACTIVATE等某些消息只有在窗口初始化的时候才发，第二次就只有发WM_DRAWITEM和WM_COMMAND了. 你可以在进入switch(uMsg)之前把消息ID都打印出来, 比较一下窗口初始化时有哪些消息, 而你点BUTTON去重绘后又有哪些消息, 这就是关键点了.
## [#8楼](http://chxy85.spaces.live.com/walzer/archive/2007/04/08/704996.html#1254072)[回复](http://chxy85.spaces.live.com/walzer/archive/2007/04/08/704996.html#post)[引用](http://chxy85.spaces.live.com/walzer/archive/2007/04/08/704996.html#post)
2008-07-14 14:38 by chenxin [未注册用户]
谢谢你的回答, 问题解决了。是StretchBlt中光栅操作代码设定的问题。还是要多看啊! 学习......
## [#9楼](http://chxy85.spaces.live.com/walzer/archive/2007/04/08/704996.html#1255749)[回复](http://chxy85.spaces.live.com/walzer/archive/2007/04/08/704996.html#post)[引用](http://chxy85.spaces.live.com/walzer/archive/2007/04/08/704996.html#post)
2008-07-15 18:58 by chenxin [未注册用户]
还有一个事儿想麻烦搂主 TrackBar控件如何改背景颜色呢? 楼主是否有相关技术的资料。我查了一个下午,都是用类写的。而且据说控件上的滑动块颜色改变不了,难道非让我自己写 TrackBar吗?
## [#10楼](http://chxy85.spaces.live.com/walzer/archive/2007/04/08/704996.html#1260425)[楼主]   [回复](http://chxy85.spaces.live.com/walzer/archive/2007/04/08/704996.html#post)[引用](http://chxy85.spaces.live.com/walzer/archive/2007/04/08/704996.html#post)[查看](http://www.cnblogs.com/CommentsByAuthor.aspx?author=Walzer)
2008-07-18 18:12 by [Walzer](http://www.cnblogs.com/walzer/)[](http://space.cnblogs.com/msg/send/Walzer)
@chenxin 
哈, 你说对了, 想改TrackBar贴图的话最好是整个儿自己重写, 用SetWindowLong(..., GWL_WNDPROC, ...)把TRACKBAR的主处理函数给替换了，然后在自己的消息处理函数里搞, 除了处理WM_PAINT, WM_LBUTTONDOWN, WM_MOUSEMOVE, WM_LBUTTONUP这些所有窗口都会收到的常规消息外，还得处理TRACKBAR特有的TBM_SETRANGE, TBM_SETRANGMAX, TBM_SETRANGEMIN, TBM_GETRANGEMAX,
 TBM_GETRANGEMIN, TBM_GETPOS, TBM_SETPOS等消息. 这样子想画哪儿都行了. 
不用怕, 我重写了整个Trackbar也就900行代码

