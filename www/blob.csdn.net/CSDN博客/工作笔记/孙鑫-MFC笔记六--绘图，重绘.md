# 孙鑫-MFC笔记六--绘图，重绘 - 工作笔记 - CSDN博客





2012年05月07日 14:37:19[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：10732








Windows颜色对话框功能的添加：

MFC提供了CColorDialog类，方便创建颜色对话框。

CColorDialog dlg；

dlg.DoModal();

默认为黑色。即首参为值为0.

保存用户所选的颜色，我们用CHOOSECOLOR结构体，用户选择颜色后，其成员rgbResult便保存了用户选择。CColorDialog有一个私用变量m_cc，便是该结构体类型对象。

这样，并不能改变用户修改后的默认颜色。要实现该功能，用CC_RGBINIT标记。设置 成员变量m_cc成员变量FLAGS成员。但不能给它直接赋值，采用或运算。CC_FULLOPEN，让对话框完整展开。

字体对话框：

CFontDialog，创建字体对话框。其构造函数，是缺省的，可以不指定值。方法与颜色对话框一样。

该类有个CHOOSEFONT结构体类型的成员变量m_cf.

CHOOSEFONT结构体中的成员lpLogFont是指向逻辑字体（LOGFONT）的指针。

LOGFONT结构体类型：

　typedef struct tagLOGFONT 



　　{ // lf LONG lfHeight; //字体高度



　　LONG lfWidth;  //字体宽度



　　LONG lfEscapement; 



　　LONG lfOrientation; 



　　LONG lfWeight; 



　　BYTE lfItalic; 



　　BYTE lfUnderline; 



　　BYTE lfStrikeOut; 



　　BYTE lfCharSet; 



　　BYTE lfOutPrecision; 



　　BYTE lfClipPrecision; 



　　BYTE lfQuality; 



　　BYTE lfPitchAndFamily; 



　　TCHAR lfFaceName[LF_FACESIZE];  //字体名字



　　} LOGFONT;



CFONT font

font.CreatFont(.....)



CreatFontIndirect(const LOGFONT* lplogfont)

要通过判断，防止CFONT发生重复关联。如果已经关联，要释放先前的字体资源。

if(m_font.m_hObject)

   m_font.DeleteObject();

示例对话框功能：



用户在编辑框内输入内容时，编辑框捕获到这个消息，发送EN-CHANE消息给父窗口，即对话框。因此，要给父窗口类，添加捕获消息响应函数。

在消息响应函数中，首先让窗口无效，下次wm_pain消息发送后，窗口重绘。

GetWindRect。得到的是屏幕对应坐标。因此需要转换坐标。用ScreenToClient函数。

UpDateData更新。





改变对话框和空间的背景以及文本颜色

WM_CTLCOLOR

响应函数

**afx_msg HBRUSH OnCtlColor(CDC *pDC,CWnd *pWnd,UINT nCtlColor);**

**pDC当前绘制控件DC的指针。**

**pWnd  当前绘制控件指针**           参数**nCtlColor**用于指定控件的类型,可以是:

           .**CTLCOLOR_BTN**                按钮控件

           .**CTLCOLOR_DLG**                对话框

           .**CTLCOLOR**_**EDIT**               编辑框

           .**CTLCOLOR_LISTBOX**            列表控件

           .**CTLCOLOR_MSGBOX**             消息控件

           .**CTLCOLOR_SCROLLBAR** 滚动条控件

           .**CTLCOLOR_STATIC**             静态控件

1.首先在自己需要设置界面的对话框上点击右键-->建立类向导-->加入WM_CTLCOLOR消息-->自动生成OnCtlColor（）函数，此函数可以对本对话框的控件的界面外观做修饰，用法如下：

将类向导产生的函数做如下修改：
 HBRUSH CDialogColor::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
HBRUSH hbr = CDialog::OnCtlColor(pDC,pWnd, nCtlColor);
// TODO: Change any attributes of theDC here
//设置显示字体
CFont * cFont=new CFont;
cFont->CreateFont(16,0,0,0,FW_SEMIBOLD,FALSE,FALSE,0, 
ANSI_CHARSET,OUT_DEFAULT_PRECIS,
CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,
DEFAULT_PITCH&FF_SWISS,"Arial");
//对特定的控件做修改
switch(nCtlColor)
{
case CTLCOLOR_STATIC: //对所有静态文本控件的设置
{
pDC->SetBkMode(TRANSPARENT); 
//设置背景为透明
pDC->SetTextColor(RGB(255,255,0)); //设置字体颜色
pWnd->SetFont(cFont); //设置字体
HBRUSH B = CreateSolidBrush(RGB(125,125,255)); 
//创建画刷
return (HBRUSH) B; //返回画刷句柄
}
case CTLCOLOR_EDIT: //对所有编辑框的设置
{
pDC->SetBkMode(TRANSPARENT); 
pDC->SetTextColor(RGB(255,255,0)); 
pWnd->SetFont(cFont); 
HBRUSH B = CreateSolidBrush(RGB(125,125,255)); 
return (HBRUSH) B; 
}
default:
return CDialog::OnCtlColor(pDC,pWnd, nCtlColor);
}
}


注：case的类别有以下几种：

CTLCOLOR_BTN 按钮控件

CTLCOLOR_DLG 对话框

CTLCOLOR_EDIT 编辑框

CTLCOLOR_LISTBOX 列表框

CTLCOLOR_MSGBOX 消息框

CTLCOLOR_SCROLLBAR 滚动条

CTLCOLOR_STATIC 静态文本


按钮控件背景和字体的改变

无法用上述方式修改。

要用CButtom类成员虚函数 DramItem函数。

因此，需要自己写一个按钮类，派生于CButton类。然后，用该类关联按钮控件、



位图显示：

1 创建位图

2 创建兼容DC

3 位图选入兼容DC

4 兼容DC复制到实际DC



BitBlt复制是1：1复制。

StretchBlt 实现压缩拉伸功能、该函数需要源位图的宽度和高度。我们通过位图类成员函数，getBitmap。得到位图的信息。



#### 滚动条实现：



坐标空间：


nMicrosoft Windows下的程序运用坐标空间和转换来对图形输出进行缩放、旋转、平移、斜切和反射。

n一个坐标空间是一个平面的空间，通过使用两个相互垂直并且长度相等的轴来定位二维对象。



nWin32应用程序设计接口(API)使用四种坐标空间：世界坐标系空间、页面空间、设备空间、和物理设备空间。应用程序运用世界坐标系空间对图形输出进行旋转、斜切或者反射。

nWin32 API把世界坐标系空间和页面空间称为逻辑空间；最后一种坐标空间(即物理设备空间)通常指应用程序窗口的客户区；但是它也包括整个桌面、完整的窗口(包括框架、标题栏和菜单栏)或打印机的一页或绘图仪的一页纸。物理设备的尺寸随显示器、打印机或绘图仪所设置的尺寸而变化。



转换：





n如要在物理设备上绘制输出，Windows把一个矩形区域从一个坐标空间拷贝到(或映射到)另一个坐标空间，直至最终完整的输出呈现在物理设备上（通常是屏幕或打印机） 。

n如果该应用程序调用了SetWorldTransform函数，那么映射就从应用程序的世界坐标系空间开始；否则，映射在页面空间中进行。在Windows把矩形区域的每一点从一个空间拷贝到另一个空间时，它采用了一种被称作转换的算法，转换是把对象从一个坐标空间拷贝到另一个坐标空间时改变(或转变)这一对象的大小、方位、和形态，尽管转换把对象看成一个整体，但它也作用于对象中的每一点或每条线。



世界坐标系空间 ->  页面空间 -> 设备空间 -> 物理设备空间（显示客户区）





n页面空间到设备空间的转换是原Windows接口的一部分。这种转换确定与一特定设备描述表相关的所有图形输出的映射方式。

n所谓映射方式是指确定用于绘图操作的单位大小的一种量度转换。映射方式是一种影响几乎任何客户区绘图的设备环境属性。另外还有四种设备环境属性：窗口原点、视口原点、窗口范围和视口范围，这四种属性与映射方式密切相关。





n页面空间到设备空间的转换所用的是两个矩形的宽与高的比率，其中页面空间中的矩形被称为窗口，设备空间中的矩形被称为视口，Windows把窗口原点映射到视口原点，把窗口范围映射到视口范围，就完成了这种转换







n设备空间到物理空间的转换有几个独特之处：它只限于平移，并由Windows的窗口管理部分控制，这种转换的唯一用途是确保设备空间的原点被映射到物理设备上的适当点上。没有函数能设置这种转换，也没有函数可以获取有关数据。




默认转换




n一旦应用程序建立了设备描述表，并立即开始调用GDI绘图或输出函数，则运用默认页面空间到设备空间的转换和设备空间到客户区的转换(在应用程序调用SetWorldTransform函数之前，不会出现世界坐标空间到页面空间的转换)。

n 默认页面空间到设备空间的转换结果是一对一的映射；即页面空间上给出的一点映射到设备空间的一个点。正如前文讲到的，这种转换没有以矩阵指定，而是通过把视口宽除以窗口宽，把视口高除以窗口高而得出的。在默认的情况下，视口尺寸为1x1个象素，窗口尺寸为1x1页单位。

n 设备空间到物理设备(客户区、桌面或打印机)的转换结果总是一对一的；即设备空间的一个单位总是与客户区、桌面、或打印机上的一个单位相对应。这一转换的唯一用途是平移。无论窗口移到桌面的什么位置，它永远确保输出能够正确无误地出现在窗口上。

n默认转换的一个独特之处是设备空间和应用程序窗口的y轴方向。在默认的状态下，y轴正向朝下，负y方向朝上。



逻辑坐标。设备坐标。





n几乎在所有的GDI函数中使用的坐标值都是采用的逻辑单位。Windows必须将逻辑单位转换为“设备单位”，即像素。这种转换是由映射方式、窗口和视口的原点以及窗口和视口的范围所控制的。

nWindows对所有的消息(如WM_SIZE、WM_MOUSEMOVE、WM_LBUTTONDOWN、WM_LBUTTONUP)，所有的非GDI函数和一些GDI函数(例如GetDeviceCaps函数)，永远使用设备坐标。

n“窗口”是基于逻辑坐标的，逻辑坐标可以是象素、毫米、英寸等单位；“视口”是基于设备坐标(象素)的。通常，视口和客户区是相同的。

n缺省的映射模式为MM_TEXT。在这种映射模式下，逻辑单位和设备单位相同。 





n窗口(逻辑)坐标转换为视口(设备)坐标的两个公式：

xViewport=(xWindow-xWinOrg)*             +xViewOrg 



yViewport=(yWindow-yWinOrg)*             +yViewOrg 



n视口(设备)坐标转换为窗口(逻辑)坐标的两个公式：

xWindow=(xViewPort-xViewOrg)*             +xWinOrg 



yWindow=(yViewPort-yViewOrg)*             +yWinOrg 





nCDC中提供了两个成员函数函数SetViewportOrg和SetWindowOrg，用来改变视口和窗口的原点。

n如果将视口原点设置为(xViewOrg,yViewOrg)，则逻辑点(0,0)就会被映射为设备点(xViewOrg,yViewOrg)。如果将窗口原点改变为(xWinOrg,yWinOrg)，则逻辑点(xWinOrg,yWinOrg)将会被映射为设备点(0,0)，即左上角。

n不管对窗口和视口原点作什么改变，设备点(0,0)始终是客户区的左上角。





**基于滚动类的滚动条实现方法：**

**窗口创建之后，调用SetScrollSizes函数，完成滚动条的初始化。**

**该函数在OnInitUpdate函数中调用。首先。给对话框类重载一个虚函数OnInitialUpdate。该函数在窗口完成创建之后，第一个被调用，即，在ondraw之前调用该函数。在该函数内，添加SetScrollSize函数，完成滚动条的初始化。**

**然后将所有的CView类修改为CScrollView类。**



**解决图形错位的方法：**

**说明：**


当我们在窗口中点击鼠标左键的时候，得到的是设备坐标(680,390)，在MM_TEXT的映射模式下，逻辑坐标和设备坐标是相等的，所以我们利用集合类保存的这个点的坐标是以象素为单位，坐标值为(680,390)。在调用OnDraw函数前，在OnPaint函数中调用了OnPrepareDC函数，调整了显示上下文的属性，将视口的原点设置为了(0,-150)，这样的话，窗口的原点，也就是逻辑坐标(0,0)将被映射为设备坐标(0,-150)，在画线的时候，因为GDI的函数使用的是逻辑坐标，而图形在显示的时候，Windows需要将逻辑坐标转化为设备坐标，因此，原先保存的坐标点(680,390)（在GDI函数中，作为逻辑坐标使用），根据转换公式xViewport
 = xWindow-xWinOrg+xViewOrg 和yViewport = yWindow-yWinOrg+yViewOrg，得到设备点的x坐标为680-0+0=680，设备点的y坐标为390-0+(-150)=240，于是我们看到图形在原先显示地方的上方出现了。



**解决方式**：



n首先我们在绘制图形之后，在保存坐标点之前，调用OnPrepareDC函数，调整显示上下文的属性，将视口的原点设置为(0,-150)，这样的话，窗口的原点，也就是逻辑坐标(0,0)将被映射为设备坐标(0,-150)，然后我们调用DPtoLP函数将设备坐标(680,390)转换为逻辑坐标，根据设备坐标转换为逻辑坐标的公式：

xWindow = xViewport-xViewOrg+xWinOrg，

yWindow = yViewport-yViewOrg+yWinOrg，得到逻辑点的x坐标为680-0+0=680，y坐标为390-(-150)+0=540，将逻辑坐标(680,540)保存起来，在窗口重绘时，会先调用OnPrepareDC函数，调整显示上下文的属性，将视口的原点设置为了(0,-150)，然后GDI函数用逻辑坐标点(680,540)绘制图形，被Windows转换为设备坐标点(680,390)，和原先显示图形时的设备点是一样的，当然图形就还在原先的地方显示出来。






**OnPrepareDC会随时根据滚动窗口的位置来调整视口的原点。**



**实际上，就是点击后，先调整，转换，保存，然后输出时，调整，转换，绘图。**



**另外两种图形的保存和重绘方法：**

**1 元文件**

** 实际是一些了绘制图形的命令，**

** 步骤：**

** 1 利用CMetaFileDC构造函数，构造一个DC对象。然后调用该类成员函数Create，创建一个设备上下文。并与创建对象关联。**

**2 给以创建的DC发送GDI命令、**

**3 调用Close函数，关闭元文件，返回元文件句柄。**

**4 以元文件句柄为参数，调用playMetaFile 播放**

**5 释放资源 DeleteMetaFile **



**保存图形。用在元文件DC创建后，释放资源前用元文件DC播放先前元文件。、**



**保存元文件 CopyMetaFile（HMETAFILE “c.wmf”）**

**打开 GetMetaFile（“c.wmf”）** 返回HMETAFILE



方式2：

兼容DC,实际就是双缓存。

双缓存:就是在内存中准备一块区域,把要显示的位图都加载到内存中,然后调用BitBlt函数,把内存设备复制到显示设备上,这样就可以防止闪屏了。


一、闪烁问题。

当后面一幅图像以很快的速度画在前面一幅图像上时，在后面图像显示前，你可以很快看到前面那一个图像，这样的现象就是闪烁

   程序出现闪烁可以由多种形式造成，最常见的原因是窗口大小发生改变时，其内容重画造成闪烁。 


    仅仅画一次 

这是一个黄金法则,在任何计算机(Windows或者你使用的任何操作系统)上处理画法逻辑都需要遵循，即永远不要将同一像素画两次。一个懒惰的程序员常常不愿意在画法逻辑上投入过多精力,而是采用简单的处理逻辑。要避免闪烁,就需要确保不会出现重复绘制的情况发生。现在，WIndows和计算机还是很笨的，除非你给他们指令，否则他们不会做任何事情。如果闪烁的现象发生，那是因为你的程序刻意地多绘制了屏幕的某些区域造成的. 这个现象可能是因为一些明确的命令，或者一些被你忽视了的地方。如果程序有闪烁的现象出现，你需要你知道如何找到好的方案去解决这个问题。



WM_ERASEBKGND 

通常，首先需要怀疑的是WM_ERASEBKGND消息。当一个窗口的背景需要被擦除时，这个消息会被发送。这是因为窗口的绘画通常经历了两个过程 


WM_ERASEBKGND: 清除背景 

WM_PAINT: 在上面绘制内容 

这两个过程让窗体在绘制内容时变得很简单，即：每次当收到WM_PAINT消息时，你知道已经有了一个新画布等待去绘制。然而，画窗口两次（一次是通过WM_ERASEBKGND画背景，另外一次是WM_PAINT）将会导致窗口出现比较糟糕的闪烁现象。只要看看标准的编辑框-打开Windows的写字板并改变窗口大小，就可以看到那种闪烁的效果。



那么，如何避免窗口背景的重刷呢？有如下两种方法： 

1 设置窗口背景刷子为NULL（当注册Windows类时，设置WNDCLASS结构中的hbrBackground成员为零） 

2 在WM_ERASEBKGND消息处理时 返回非零值 
以上任何一种方法都可以阻止WM_ERASEBKGND 消息去清除窗口。其中，第二个方案的通常可以以如下代码实现： 


case WM_ERASEBKGND: 

return 1; 


当你标记窗口内容无效并试图更新时，还有如下办法可以防止WM_ERASEBKGND消息：InvalidateRect函数的最后一个参数可以指明在下一次窗口重画时，是否窗口的部分背景会被重刷。将该参数置为False可以防止当窗口需要重画时系统发出WM_ERASEBKGND消息。



InvalidateRect(hwnd, &rect, FALSE); 


不该画的时候一定不要画 

有一个比较普遍的现象：即使窗口中只有一个小的部分发生了改变，往往所有的部分都会被重画。比如，经常地，当窗口大小被改变时，一些（不是所有）的程序会重画所有的窗口。通常，这是个是不必要的，这是因为当窗口大小被改变时，经常是之前窗口的内容是不变的，仅仅是改变大小造成的一个小的边界区域需要重画。此时，没有必要重画所有区域。如果在这里多注意，多考虑，就可以使用好的算法以使得一次只有最小的部分被画。



系统中每个窗口都有更新区域。这个区域描述了窗口中变得无效需要重画的地方。如果一个窗口仅仅其需要更新的区域，不多绘制其他地方，那么窗口的绘制效果将会非常快。 


有几种方法可以获得窗口的更新区域。通过GetUpdateRgn 函数可以获得准确的更新区域，这个函数返回的结果可以使矩形的区域也可以是非矩形的区域。通过GetUpdateRect 函数可以获得需要更新的最小矩形区域。通常使用矩形的更新区域比较容易。第三个方法是在BeginPaint/EndPaint中得到PAINTSTRUCT 结构，从而得到准确的更新区域信息。



一个常规的画法函数是这样的： 


PAINTSTRUCT ps; 

HDC hdc; 

case WM_PAINT: 

hdc = BeginPaint(hwnd, &ps); 

// do painting 

EndPaint(hwnd, &ps); 

return 0; 


BeginPaint函数初始化PS（PAINTSTRUCT）结构，其中，成员rcPaint是一个RECT结构，描述了包含了需要更新的最小矩形区域（就像GetWindowRect函数）。


如果仅仅在这个矩形区域上绘制窗口，速度上绘有很好地提高。 


现在，当使用BeginPaint/EndPaint时Windows会自动剪切掉画在更新区域外面的部分。这意味着，你没有机会画到更新区域以外的地方。可能你会认为，如果是这样的话，花功夫确保代码不试图画到更新区域外是没有意义的，反正没有画出任何东西来。然而，你仍然可以避免不必要的API调用和相关计算，所以，我认为放一些精力在如何工作地更快上是绝对值得的。



如果还是不能解决 

有些时候，当你花了很多努力去考虑非常好的画法时，发现窗口还是会被全部刷新。这通常是由两个Window 类的属性造成的：CS_VREDRAW 和 CS_HREDRAW。如果有其中一个标志被设置时，那么当窗口水平或者竖直方向有大小被改变时，其内容每次都会被重新刷新。所有，你需要关掉这两个标志，解决的唯一的方式是在创建窗体和窗体类被注册时，确保这两个属性不被设置。



WNDCLASSEX wc; 

wc.cbSize = sizeof(wc); 

wc.style = 0; /* CS_VREDRAW | CS_HREDRAW; */ 

... 

RegisterClassEx(&wc); 


上面的例子描述了当窗体类被注册时，这两个属性不被设置的实现方法。 

有一点需要注意：如果主窗体有了这两个属性，即使子窗体没有重画标志，会导致所有子窗体在其大小被改变时会被重绘。可以通过以下方式避免这个情况发生： 


剪切子窗体 

有时，闪烁的原因是因为当重画时，父窗体没有剪切其子窗体区域。这样的结果导致，整个父窗口内容被重画，而子窗体又被显示在了上面（造成闪烁）。这个可以通过在父窗体上设置WS_CLIPCHILDREN 来解决。当这个标志被设置时，被子窗体占据的任何区域将会被排除在更新区域外。因此，即使你尝试在子窗体所在的位置上绘制（父窗口的内容），BeginPaint中的剪切区域也会阻止其绘制效果。


双缓冲和内存设备描述表（Memory Device Context, 简称Memory-DC） 

常见的彻底避免闪烁的方法是使用双缓冲。其基本的思路是：将窗体的内容画在屏幕外的一个缓冲区内，然后，将该缓冲区的内容再传递到屏幕上（使用BilBlt函数）。这是一个非常好的减少闪烁的方法，但是经常被滥用，特别是当程序员并不真正地理解如何有效地绘制窗口时。

    按照我个人的理解哦，当我做五子棋棋盘的时候，背景色设置为棕黄色，在制定区域绘制20X15的小格子作为棋盘。当每次点重置时，就是清空棋子时候，或者更改窗体大小和移动窗体时，都会产生闪烁。为什么这样子呢？就是当窗体重绘的时候，背景图画了一遍，棋盘又画了一遍，这期间有时间差，导致很不一致，给人视觉上的闪烁，让客户很不舒服，这是程序员需要注意也必须要解决的一个问题！于是就引进了双缓存技术。什么是双缓存呢？它的原理是将所有图像合成到一起 然后一下显示出来 移动刷新的时候也是一张一张的往整个客户区覆盖 这样就不存在了重叠刷新的问题。如果是控件闪烁.可以利用获取控件DC绘制在父窗体底部减小色差的方法
 应用函数为GetDC() 。在我的程序中，个人认为是这样引用双缓存的，当窗体收到重绘消息时，首先调用OnEraseBkgnd(CDC* pDC) 返回一个true，将客户区的背景色搽除！再调用onpaint()函数进行重绘，但是在重绘的过程中得注意，利用双缓存技术。先获得一个绘图设备CDC，然后再将图画在位图上，当然你重绘的背景也要画在这个位图上~~~最后就是将画好的位图按顺序的显示在客户区上~~感觉就像跟幻灯片一样一幅幅的出现吧，并且不让人的眼球发觉正在重绘，也没有闪烁！当然，这里面我还碰到一个问题，就客户区，重绘区问题解决了，非客户区的问题出现了，非客户区的背景是父窗口的颜色，与客户区的不一致，当把客户区扩大到整个窗口的时候，又会把非客户区的控件覆盖！解决办法和原因后面再说~~~

    通常我们编写双缓存的代码的时候，有经典的写法:

HDC hdcMem; 

HBITMAP hbmMem; 

HANDLE hOld; 

PAINTSTRUCT ps; 

HDC hdc; 

.... 

case WM_PAINT: 

// Get DC for window 

hdc = BeginPaint(hwnd, &ps); 

// Create an off-screen DC for double-buffering 

hdcMem = CreateCompatibleDC(hdc); 

hbmMem = CreateCompatibleBitmap(hdc, win_width, win_height); 

hOld = SelectObject(hdcMem, hbmMem); 

// Draw into hdcMem 

// Transfer the off-screen DC to the screen 

BitBlt(hdc, 0, 0, win_width, win_height, hdcMem, 0, 0, SRCCOPY); 

// Free-up the off-screen DC 

SelectObject(hdcMem, hOld); 

DeleteObject(hbmMem); 

DeleteDC (hdcMem); 

EndPaint(hwnd, &ps); 

return 0;

我的做法是：

BOOL CGI_PaltFormDlg::OnEraseBkgnd(CDC* pDC) 

{

// TODO: Add your message handler code here and/or call default


//return CDialog::OnEraseBkgnd(pDC); 

return true;

}

void CGI_PaltFormDlg::OnPaint() 

{

   CPaintDC dc(this); // device context for painting

        SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);


   // Center icon in client rectangle

   int cxIcon = GetSystemMetrics(SM_CXICON);

   int cyIcon = GetSystemMetrics(SM_CYICON);

   CRect rect;

   GetClientRect(&rect);


   dc.FillSolidRect(rect,RGB(255,128,0)); 


   int x = (rect.Width() - cxIcon + 1) / 2;

   int y = (rect.Height() - cyIcon + 1) / 2;


   // Draw the icon

   dc.DrawIcon(x, y, m_hIcon);

        CDC *pDC=this->GetDC();

   int red=0,green=0,blue=0;

   int width=2;


   CDC MenmDC;

   CBitmap MemBitmap;

   MenmDC.CreateCompatibleDC(NULL);

   MemBitmap.CreateCompatibleBitmap(pDC,950,750);


   CBitmap *pOldBit=MenmDC.SelectObject(&MemBitmap);

   MenmDC.FillSolidRect(0,0,950,750,RGB(255,128,0));


   int tem=0;

   //画横线

   for (int i=0;i<16;i++)

   {

    MenmDC.MoveTo(100,100+tem);

    MenmDC.LineTo(900,100+tem);

    tem=40+tem;

   }

   //画竖线

   tem=0;

   for(i=0;i<21;i++)

   {

    MenmDC.MoveTo(100+tem,100);

    MenmDC.LineTo(100+tem,700);

    tem=40+tem; 

   } 

   pDC->BitBlt(0,0,1400,900,&MenmDC,0,0,SRCCOPY);

   MemBitmap.DeleteObject();

     MenmDC.DeleteDC();

   ReleaseDC(pDC);

}

上面是我画棋盘的语句，只供参考而已！

二、对话框背景覆盖控件问题。

    上面已经提及到我遇到的问题。为了达到两全其美的效果，竟让闪屏问题解决了，又让我的控件不收影响，我想了一个很笨的办法！首先确定客户区的范围，我确定的是只双缓存重绘棋盘部分，矩形区域为（0，0，950，750），这里面我重绘了背景色和棋盘！然后再让非重绘区背景色设置一遍就是和控件一起重绘，说不重绘其实也是重绘的，只是不一致的重绘而已！

CPaintDC dc(this); // device context for painting

CRect rect;

GetClientRect(&rect);

dc.FillSolidRect(rect,RGB(255,128,0));

当然这里面还有个小问题，就是在onpain()函数里面最好把对话框控件和非缓存重绘区的背景重绘的代码放在缓存区重绘代码的前面，像我上面代码说的!

通过这样的设置，我的问题解决了！哈哈，皆大欢喜....



























