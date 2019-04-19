# WINCE应用的UI实现方案 - xqhrs232的专栏 - CSDN博客
2010年07月08日 15:02:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1428标签：[wince																[ui																[button																[dialog																[mfc																[测试](https://so.csdn.net/so/search/s.do?q=测试&t=blog)](https://so.csdn.net/so/search/s.do?q=mfc&t=blog)](https://so.csdn.net/so/search/s.do?q=dialog&t=blog)](https://so.csdn.net/so/search/s.do?q=button&t=blog)](https://so.csdn.net/so/search/s.do?q=ui&t=blog)](https://so.csdn.net/so/search/s.do?q=wince&t=blog)
个人分类：[DirectUI界面技术](https://blog.csdn.net/xqhrs232/article/category/1243806)
原文地址::[http://space.itpub.net/?uid-16803921-action-viewspace-itemid-503877](http://space.itpub.net/?uid-16803921-action-viewspace-itemid-503877)
一、MFC的硬伤
在接手现在这个项目之前，我对WINDOWS平台上的UI开发还是个白痴，除了MFC，就只知道GDI了。而且居然大言不惭地说用MFC只能画画灰色的对话框和按钮。但不论如何，在[**嵌入式**]()这种对成本极度敏感的项目上，我是不会拍板用MFC的。假设极端情况，定制后的系统是31.8M,我放一个ARMV4I上的MFC DLL进去，大概500多K，那么只有两种选择，要么把32M的FLASH换成64M的——我的上司会把我给砍了，要么把应用层的UI代码全部重写——我的下属会把我给剁了。另一方面，WINCE上的应用软件我看过不少[**开源**]()代码，也接触了一些外包的软件，还真没见过谁用MFC的。网上公论用MFC后会导致程序在不同平台上移植性降低，因为你不能指望别人的平台给你准备好奢侈的MFC。另一方面，多数高手都不屑用。我不是高手，但可以学人家摆谱，于是“不会用”就变成了“不屑用”
 ^_^
二、GDI的痛苦
把整套UI从CreateWindow开始写起，的确很累人。我写了500多行才勉强实现BUTTON类，另一个同事也用了500行左右才实现了TRACK BAR类，而且还未经[**测试**]()，也没有很正式的CODE REVIEW。如果工业[**设计**]()中心多增加几种图样，那么我们就得多些几个基类，然后再赔进去CODE
 REVIEW的时间、测试时间、BUG FIX的时间。不痛苦，那是不可能滴~。
三、GWES的探路，我不是先锋
群众的智慧是无穷的。当我这组同事的思维都受制于我的GDI方案时，从通信部过来协助完成项目的软件工程师从WINCE500的一个应用SAMPLE CODE里把DialogBox函数给抓出来了。我认为自己在定UI实现方案上很失败的一点就是习惯性思维地从eVC里建立DIALOG RESOURCE后，立刻就要去点Class Wizard, 然后就是关联MFC类。而他却画出来的DIALOG和BUTTON后，拿着RESOUCE ID从DialogBox函数建立起UI。并且我又习惯性思维地认为DialogBox并不在STANDARD
 SDK 500里面，但他确实从STANDARDSDK_500里不引用其它LIB和DLL就把DialogBox和BUTTON用起来了，然后过来找我谈论如何把图片叠加在DIALOG和BUTTON上。泪奔一百里~ 我应该去找块豆腐撞死~
四、最后的攻关，GWES API能否成为我们需要的坚实地基
GWES系列API能否实现我们所需的所有UI功能呢？没有人知道，需要评估。刚才起草稿时，我把这些都写在同一篇文章里了。现在觉得还是分篇好些，毕竟主题不同。请继续看中篇：GWES方案上几技术难点的解决
这里谈论的所谓技术难点，其实根本不值一提。只不过[**微软**]()定了一套游戏规则，我们目前不清楚这套游戏规则，花时间去摸索而已。
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
itemAction == 1；  // ODA_DRAWENTIRE
itemState == 16；  // ODS_FOCUS
(2) BUTTON被按下时
itemAction == 2；  // ODA_Select
itemState == 17；  // ODS_FOCUS | ODS_SelectED
(3) BUTTON被点击后松开时
itemAction == 2；  // ODA_Select
itemState == 16；  // ODS_FOCUS
ODA和ODS宏定义数值
/*** from Winuser.h ***/
#define ODT_MENU        1
#define ODT_LISTBOX     2
#define ODT_COMBOBOX    3
#define ODT_BUTTON      4
//action
#define ODA_DRAWENTIRE  0x0001
#define ODA_Select      0x0002
#define ODA_FOCUS       0x0004
//state
#define ODS_SelectED    0x0001
#define ODS_GRAYED      0x0002
#define ODS_DISABLED    0x0004
#define ODS_CHECKED     0x0008
#define ODS_FOCUS       0x0010
在WM_DRAWITEM中有两点要特别注意
(1) 不能在里面用InvalidateRect(lpDIS->hwndItem, lpDIS->rcItem, NULL),这会立即再发一个WM_DRAWITEM消息过来，接着再调InvalidateRect, 进入死循环直至把设备上的内存耗光，导致死机
(2) 对itemAction和itemState作判断时，必须把两者同时都判断了才能确定CLICK状态，单独判断action或单独判断state是不够的，会导致重绘作用在不希望发生的情况下。并且不能简单地作itemAction & ODA_Select这样的位与判断，还必须有排他性，我干脆就用==号了。
参考代码如下
1 BOOL CALLBACK DialogProc (HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
2 {
3     switch(uMsg)
4     {
5         case WM_DRAWITEM:
6         {
7             LPDRAWITEMSTRUCT lpDIS = (LPDRAWITEMSTRUCT)lParam;
8
9             if(lpDIS->CtlID == IDC_BTN_1)
10             {
11
12                 if( (lpDIS->itemAction & ODA_DRAWENTIRE)   //这是初始化时的情况
13                     ||    ( (lpDIS->itemAction == ODA_Select) && (lpDIS->itemState == ODS_FOCUS) ) ) //这是按下后放开时的情况
14                 {
15                     //画上未被按下时的图片
16                 }
17                 else if( (lpDIS->itemAction & ODA_Select) && (lpDIS->itemState == (ODS_FOCUS | ODS_SelectED) ) ) //被按下时的情况
18                 {
19                     //画上被按下时的图片
20                     MessageBeep(MB_OK); //顺便响一声，比较有手感
21                 }
22             }
23             return TRUE;
24         }
25         default:
26             //STUB
27             break;
28     }
29    
30     return FALSE;
31 }
2、DLU和PIXEL的单位转换
本来以为做完BUTTON效果后就OVER了，结果今天傍晚时候遇到一个很恼火的问题。在VC / eVC / VS中画的对话框、按钮等控件时，在IDE右下角显示的 XX * XX单位是DLU (Dialog Unit), 这是根据你设置的对话框字体大小而改变的。这种做法无可厚非。如果把字体改大了，那么DIALOG和BUTTON自然也被“撑”大了，比较灵活。但是我往上叠加的图片是按像素(PIXEL)来算的。最后实现出来，有两个方法。按照国际惯例，当然是先讲笨的方法，MSDN上的作风也是如此。
(1)方法一：我查了下DLU和PIXEL之间的换算关系，有个讲得比较全的网页是[http://support.microsoft.com/default.aspx?scid=kb](http://support.microsoft.com/default.aspx?scid=kb);en-us;145994 (How to caculate dialog box units based
 on the current font in Visual C++) 按照文中的Method Two简单测试了一下
1 BOOL CALLBACK DialogProc (HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
2 {
3     switch(uMsg)
4     {
5         case WM_INITDIALOG:
6         {
7             RECT rc={0,0,4,8};
8
9             MapDialogRect(hDlg, &rc );
10             printf("baseUnitX = %ld/n", rc.right);
11             printf("baseUnitY = %ld/n", rc.bottom);
12
13             //DLU_Weight = pixel_weight * 4 / baseunitX;
14             //DLU_Height = pixel_height * 8 / baseunitY;
15
16             return TRUE;
17         }
18     }
19
20     return FALSE;
21 }
对于我DIALOG中设置的Courier New 10pt字体，
第一步：从代码中得到baseUnitX = 7, baseUnitY = 16,
第二步：套用公式
weight_DLU = weight_pixel * 4 / baseUnitX
height_DLU = height_pixel * 8 / baseUnitY
比如我想建立一个320*240 PIXEL的对话框，那么
weight_DLU = 320 * 4 / 7 = 182.86, 取整为183
height_DLU = 240 * 8 / 16 = 120
第三步：到VC里去拖动对话框的边界，画出183*120的对话框，那么代码运行起来后，通过GetClientRect查一下，的确是得到320*240 pixel的对话框了。
但是这种方法有一个致命的缺点。比如我要画一个60*70的按钮，按照上面的baseUnitX和baseUnitY折算后应该为34.28 * 35,但是我画34*35 DLU时，运行得到59*70 PIXEL窗口；画35*35 DLU时，运行得到61*70 PIXEL窗口，无法恰到好处，这导致了往上叠加60*70 pixel的图片时，按钮边缘会出现不连续的黑点。所以这段文字只能当作DLU和PIXEL的换算关系来玩了，没有任何实际应用价值。
(2) 方法二：正确的方法
其实也很简单。晚饭后突然想到的，用SetWindowPos强制设置window的长宽和左上角坐标。我在WM_DRAWITEM消息的处理中简单试了一下：
SetWindowPos(lpDIS->hwndItem, NULL, 10, 10, 60, 70, SWP_NOZORDER);
设置前是59*70的按钮，设置后就是60*70了，并且叠加图片没有任何问题。OK，整个基于GWES的UI方案至此成型了。后面貌似没有什么大的技术障碍了。
至此，我没有发现GWES方案上还有什么路障了，可以拍板使用这套方案了，和只用GDI写UI相比，软件研发的工作量大概降低了30%左右。当然事情还没有就这样结束，这套方案对我这项目组的意义是很深远的。请看下篇：代码中的一小步，项目进度[**管理**]()上的一大步
当我试验SetWindowPos成功时，我感觉到对我这个应用开发组来说，这是一次革命了。项目进度上的革命。
按照目前的进度安排方式，事业部发布设计需求后各部门的工作状态时这样的：
(1) 软件研发，首先去确定底层接口，比如要调用BSP的哪些DeviceIoControl，要用哪些协议栈，要约定哪些注册表键值，约定各应用的进程间通信。
(2) 工业设计中心, 同步开始设计UI图片。
(3) 测试组，同步开始编写测试例
而三者之中，工业设计中心是最慢的，界面风格需要多次评审和修改，而且主观因素很强，领导说不好看，就得继续改，随便调整一下就是一两天。以我做几个项目的经历来看，往往是软件研发人员和测试都完成第一步了，工业设计中心还没发出切割图，然后大家就傻在那里等资源。等工业设计中心正式发布切割图后，软件研发才开始埋头苦干，这时候测试组又继续闲着，等到出了ALPHA版才开始测试工作。
以我当前这个项目为例，工业设计用两个人花了足足一个月的时间才完成一级界面和二级界面，所以应用组的人也不紧不慢地花了一个月的时间来作底层接口的研究和确定，慢慢地看文档。实际上如果都是该领域的熟手，并且效率够高的话，这些事情最多两周就能做完了。
而如果用了GWES的API，加上SetWindowPos的做法之后，项目进度上的优势是非常明显的：
(1) 软件研发：确定底层接口后，立刻建立起DIALOG和BUTTON，EDITOR等控件，根本不用关心UI最后设计成什么样。重点是上层的数据结构和逻辑，和编写代码对底层接口进行调用测试。UI并不再会成为瓶颈，只要随便拖几个控件出来就行了，坐标和长宽也是随意的，只要把功能做对了。
(2) 工业设计中心：可以慢慢地做图片，一轮一轮地慢慢评审。由于叠加图片的方式已经很明确，并且[**程序员**]()写绘图代码时可以同时指定坐标和长宽，直接修正原型开发时乱拼凑的界面，所以切割图在软件BETA RELEASE前两三天发布就来得及了。
(3) 测试组：由于软件研发可以很快地把界面丑陋、但功能实现好的ALPHA版程序发布，所以测试组可以大大提前手工测试的开始时间点。并且尽早开始BUG反馈。甚至于在UI图片出来之前，就可以改几轮BUG了。在UI图片出来之前完成ALPHA版，并且改过几轮BUG，这种情况在以前是从来不能想象的，应用工程师肯定会说：图片都还没有，怎么写代码？写了也白写，反正还要改。
(4) 图片发布后，每个应用程序最多花两天工夫作图片叠加，而且原先写的代码在图片叠加的工作中完全不用改动。
(2) 由于功能实现的代码段没有因为图片叠加而改动，所以之前测试的BUG仍然全部有效，并且因为图片叠加而产生的高级别BUG可能性很小。
OHYE，事情想象得真美妙。
难道是我年少无知，其实其他公司早就是这么开发应用软件的？我今天造的新词很适合形容现在的心情：泪奔一百里。GO ON~嗯~嗯~一百里啊一百里~
