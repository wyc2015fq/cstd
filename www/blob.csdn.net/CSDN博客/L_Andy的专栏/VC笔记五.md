# VC笔记五 - L_Andy的专栏 - CSDN博客

2013年02月19日 17:13:09[卡哥](https://me.csdn.net/L_Andy)阅读数：1125


vc++技术总结 张明奇(卡哥)

//////////////////////////////////////////////////

AFX_IDW_CONTROLBAR_FIRST,   AFX_IDW_CONTROLBAR_LAST   

指的是空间ID的范围，在MFC中   

AFX_IDW_CONTROLBAR_FIRST   的值为   0xE800   

AFX_IDW_CONTROLBAR_LAST     的值为   oxE8FF      

意思是这个ID范围内的ControlBar都将被Reposition

eg:RepositionBars(AFX_IDW_CONTROLBAR_FIRST,AFX_IDW_CONTROLBAR_LAST,0);

////////////////////////////////////////////////////////////////////////////////

关于TBSTYLE_FLAT 和 TBSTYLE_TRANSPARENT，MSDN上有云：

TBSTYLE_FLAT   

Version 4.70. Creates a flat toolbar. In a flat toolbar, both the toolbar and the buttons are transparent and hot-tracking is enabled. Button text appears under button bitmaps. To prevent repainting problems, this style should be set before the toolbar control
 becomes visible. 

TBSTYLE_TRANSPARENT   

Version 4.71. Creates a transparent toolbar. In a transparent toolbar, the toolbar is transparent but the buttons are not. Button text appears under button bitmaps. To prevent repainting problems, this style should be set before the toolbar control becomes
 visible.

看意思应该是使用TBSTYLE_FLAT 时toolbar 和 buttons都透明，而使用TBSTYLE_TRANSPARENT 时 toolbar透明而而buttons不透明，如果这样理解的话，需要 toolbar透明而buttons不透明时应该使用 TBSTYLE_TRANSPARENT ，需要两者都透明的话，则使用TBSTYLE_FLAT ，两者同时使用是完全没有必要而且会造成意义模糊的。 但事实是这样的的吗？NO。事实是要创建一个透明的工具条，需要同时使用TBSTYLE_FLAT 和 TBSTYLE_TRANSPARENT
 风格，只用TBSTYLE_FLAT风格创建的 toolbar 将包含不透明的 buttons 。这样的话应该是， 使用TBSTYLE_FLAT 时toolbar 透明而buttons不透明(同MSDN对 TBSTYLE_TRANSPARENT 的解释)，而使用 TBSTYLE_TRANSPARENT 时应该是 toolbar不透明而buttons透明，这样的话两者同时使用达到透明的效果就讲的通了。

但是正确的答案究竟是什么呢？还是不能够确定。:(

///////////////////////////////////////////

LVITEM究竟是什么！

Win32SDK里的。

列表视图控件有两个重要的数据结构LVCOLUMN和LVITEM。LVCOLUMN用于定义报表方式下的“列”的结构；LVITEM用于定义“项”的结构。

/////////////////////////////////////////////////////////////////////////

// 设置对话框为可变大小的

ModifyStyle(0, WS_SIZEBOX);

/////////////////////////////////////////////////////////////////

AfxGetMainWnd返回的是什么

主窗口指针

多文档、单文档返回的是CMainFrame指针

对话框的是主对话框的CDialog指针

////////////////////////////////////////////////////////////////////

AfxGetMainWnd()和AfxGetApp()->GetMainWnd()的区别 1、AfxGetMainWnd取得主窗口。AfxGetApp取得当前应用实例。在MSDN里查一下就知道了阿

2、前者得到应用程序的主窗口，例如MainFrm，对话框应用程序的主对话框。后者是取得应用程序类对象（主线程）

3、AfxGetMianWnd() 返回 CWnd* 

AfxGetApp() 返回 CWndApp*

4、AfxGetMainWnd()返回主窗口的指针， 

AfxGetApp()返回线程指针

注意：在使用线程的一定要注意，使用AfxGetApp()->GetMainWnd() 因为。

AfxGetApp()->GetMainWnd()取得的是主窗口句柄,无论在那个线程里调用都是没有问题的,因为它先取得主线程句柄,再取得主线程的活动窗口。AfxGetMainWnd()是取得其窗口句柄，后台线程既然没有窗口，所以无法取得窗口句柄呢

 ////////////////////////////////////////////////////////////////////////

如何copy一个项目的对话框到另一个项目中？

文本->打开->文件->.rc文件，找到你想要的对话框资源那段，直接拷贝到你的工程中的.rc文件中，然后再手动在resource.h中添加资源ID号；

最后复制相关窗体类对应的文件（*.cpp, *.h)文件拷贝至你的工程目录中，然后在VC中将类添加至工程中即可.

////////////////////////////////////////////////////////////////////////////////

如果你加入的（*.cpp,*.h）在文件中是一个单独的文件夹。当你添加到项目中，并不能打开（*.cpp,*.h）

进行如下操作可解决：项目->属性->c/c++/常规。把文件夹的名称拷贝到。附件包含目录的栏中

eg：(*.cpp,*.h)在(Database)文件夹中，只需在：附件包含目录栏中写入(Database).

////////////////////////////////////////////////////////////////////////////////////

std”: 具有该名称的命名空间不存在

当只用using namesp std 时，会报 error C2871: “std”: 具有该名称的命名空间不存在。

包含一个含有std的头文件就不会报错了，比如<iostream>、<functional>、<algorithm>等

//////////////////////////////////////////////////////////////////////////////////////

1 基本解释：extern可以置于变量或者函数前，以标示变量或者函数的定义在别的文件中，提示编译器遇到此变量和函数时在其他模块中寻找其定义。

此外extern也可用来进行链接指定。也就是说extern有两个作用，第一个,当它与"C"一起连用时，如: extern "C" void fun(int a, int b);

则告诉编译器在编译fun这个函数名时按着C的规则去翻译相应的函数名而不是C++的，C++的规则在翻译这个函数名时会把fun这个名字变得面目全非，

可能是[fun@aBc_int_int#%$](mailto:fun@aBc_int_int#%25$)也可能是别的，这要看编译器的"脾气"了(不同的编译器采用的方法不一样)，为什么这么做呢，因为C++支持函数的重载啊，

在这里不去过多的论述这个问题，如果你有兴趣可以去网上搜索，相信你可以得到满意的解释!

第二，当extern不与"C"在一起修饰变量或函数时，如在头文件中: extern int g_Int; 它的作用就是声明函数或全局变量的作用范围的关键字，

其声明的函数和变量可以在本模块活其他模块中使用，记住它是一个声明不是定义!也就是说B模块(编译单元)要是引用模块(编译单元)A中定义的全局变量或函数时，

它只要包含A模块的头文件即可,在编译阶段，模块B虽然找不到该函数或变量，但它不会报错，它会在连接时从模块A生成的目标代码中找到此函数。

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

用例子给你示范 

// 1.cpp int x = 10; 

// 2.cpp 注意没有包含1.cpp 

#include <iostream> 

using namespace std; 

extern int x;

int main () 

{ 

cout << x << endl;

 } 

//则输出10 两个文件同在一个项目（project）中，你不包含某个文件（cpp）而可以用它内部定义的变量，

(里是.pp不是.h, 因为在.h中定义的数据不能在.cpp中用除非这个.cpp包含这个.h文件）

 例： 

// 1.h #include <iostream>

 void print() 

{ 

std::cout << "hello!" << std::endl; 

} 

// 2.cpp 

#include <iostream> 

using namespace std; 

// 以上两句在这个例子中可以不要 

extern void print(); 

int main () 

{ 

print(); 

} 

就会出错因为

1.h中的

void print()；

在不包含它的文件中是不可调用的，

即使在声明了extern 也于事无补，如果你将这个例子中的1.h名字换成1.cpp就对了！ 从这些可以看出来，

extern在这里起的作用是告诉编译器，你这个print()已经在某个.cpp中已经定义了，这里只不过是声明一下有这个东西，然后拿来用一下。

定义只能出现一次，声明却可出现多次，也就是说extern声明可在多个文件中用（包括.h) 还有，你还可以屏蔽extern声明，

如第二个例子中的第二个.cpp文件可以改成 #include <iostream> using namespace std; // 这里以上两句不能省略，

因为，这里extern void print();函数已经不起作用了，在这里调用的而是本文件中定义的void print()函数，

其中用到了cout,endl;他们来源于std::<iostream> 

extern void print(); 

void print() 

{ 

cout << "world!" << endl; 

} 

int main () 

{

 print(); 

} 

// 输出结果为world! 还有一个extern "C"就不用说了，用这个可以允许C++程序中调用C的函数！

////////////////////////////////////////////////////////////////////////////////////

CFormView简介

MFC提供了一个名为CFormView的特殊视图类，我们称其为表单视图．表单视图是指用控件来输入和输出数据的视图，用户可以方便地在表单视图中使用控件．

表单视图具有对话框和滚动视图的特性，它使程序看起来象是一个具有滚动条的对话框．在有些情况下，用表单视图比用普通视图更符合用户的需要，

例如，在向数据库输入数据时，显然用表单的形式可以更习惯些．

用AppWizard可以方便地创建基于表单视图的应用程序，只要在MFC AppWizard对话框的第六步先选择CView，

然后在Base class栏中选择CFormView，AppWizard就会创建一个基于CFormView的应用程序．

　　读者可以按上述方法建立一个名为Test的应用程序．在Test工程的资源中，读者会发现一个ID为IDD_TEST_FORM的对话框模板，

该对话框模板可供用户放置和安排控件．在程序运行时，框架根据该对话框模板创建CFormView对象，并根据模板的信息在表单视图中自动创建控件．

与设计对话框类相类似，用户可以用ClassWizard为表单视图类加入与控件对应的成员变量，可以调用UpdateData在控件和成员变量之间交换数据，

但对控件的初始化工作是在OnInitialUpdate函数而不是在OnInitDialog函数中进行的．

　　基于表单视图的应用程序与基于对话框的应用程序都是在应用程序中直接使用控件，但二者有很多不同之处．

基于对话框的应用程序是用一个对话框来作为程序的主窗口的，因而程序的主窗口的特性与对话框类似，如窗口的大小不能改变，程序没有菜单条、工

具条和状态栏等．基于表单视图的应用程序仍然是基于Doc/View框架结构的，只是视图被换成了表单视图，也就是说，应用程序的窗口可以改变大小，程序

有菜单条、工具条和状态栏，且程序仍然可以Dov/View运行机制来处理文档．

///////////////////////////////////////////////////////////////////////////////

Virtual C++ 中Resources视图的string table作用是什么啊？

给你翻译一下:IDR_MAINFRAME 这个标识符 不仅用来指定应用程序的字符串，

还标识了应用程序的图标、工具条资源和菜单。应用程序向导 生成了这些资源，

而你可以用资源编辑器对它们进行修改维护操作。

就是说 应用程序向导 生成了 以 IDR_MAINFRAME 命名的资源，各个种类都有（书上还漏写了 快捷键 资源）

String Table 是字符串表。 其中是这个程序所要显示的那些文本。包括在状态条里显示的、消息框显示的…… 

在程序中只是加载它们。这样使以后修改起来比较方便。

/////////////////////////////////////////////////////////////////////////////////////

UpdateData(TRUE) 　　

——刷新控件的值到对应的变量。(外部输入值交给内部变量) 　　

即：控件的值—>变量。 　　

UpdateData(FALSE) 　　

—— 拷贝变量值到控件显示。(变量的最终运算结果值交给外部输出显示) 　　

即：变量值—>控件显示

//////////////////////////////////////////////////////////////////////////////////////////

当我们想得到一个窗口对象（CWnd的派生对象）指针的句柄（HWND）时，最安全的方法是使用GetSafeHwnd()函数，

通过下面的例子来看其理由： 　　

CWnd *pwnd = FindWindow(“ExploreWClass”,NULL); //希望找到资源管理器 　　

HWND hwnd = pwnd->m_hwnd; //得到它的HWND 　　

这样的代码当开始得到的pwnd为空的时候就会出现一个“General protection error”,

并关闭应用程序，因为一般不能对一个NULL指针访问其成员，如果用下面的代码： 　　

CWnd *pwnd = FindWindow(“ExploreWClass”,NULL); //希望找到资源管理器

HWND hwnd = pwnd->GetSafeHwnd(); //得到它的HWND

/////////////////////////////////////////////////////////////////////////////////////////

subclassdlgitem

该函数用来子类化一个控件.

Subclass(子类化)是MFC中最常用的窗体技术之一。子类化完成两个工作：一是把窗体类对象attach到一个windows窗体实体中（即把一个窗体的hwnd赋给该类）

另外就是把该类对象的消息加入到消息路由中，使得该类可以捕获消息。

SubclassDlgItem可以把对话框中已有的控件与某个窗口对象动态连接起来，

该窗口对象将接管控件的消息处理，从而使控件具有新的特性。

要在程序中创建新设计的控件，显然不能用自动创建的办法，因为对话框模板对新控件的特性一无所知．

程序可以用手工方法创建控件，在调用派生类的Create函数时，派生类会调用基类的Create函数创建控件．

用Create函数创建控件是一件比较麻烦的工作，程序需要为函数指定一大堆的控件风格以及控件的坐标和ID．特别是控件的坐标，没有经验的程序员很难确切地安排控件的位置和大小，

往往需要反复调整．利用MFC的CWnd::SubclassDlgItem提供的动态连接功能，可以避免Create函数的许多麻烦，该函数大大简化了在对话框中创建派生控件的过程．

大家知道，在用手工方法创建控件时，先要构建一个控件对象，然后再用Create函数在屏幕上创建控件窗口，也就是说，控件的创建工作是由控件对象完成的．

动态连接的思路则不同，SubclassDlgItem可以把对话框中已有的控件与某个窗口对象动态连接起来，该窗口对象将接管控件的消息处理，从而使控件具有新的特性．

SubclassDlgItem函数的声明为

BOOL SubclassDlgItem( UINT nID, CWnd* pParent );　

参数nID是控件的ID，pParent是指向父窗口的指针．若连接成功则函数返回TRUE，否则返回FALSE．

综上所述，要在程序中使用派生控件，应该按下面两步进行：

在对话框模板中放置好基类控件．

在对话框类中嵌入派生控件类的对象．

在OnInitDialog中调用SubclassDlgItem将派生类的控件对象与对话框中的基类控件相连接，则这个基类控件变成了派生控件．

例如，如果要在对话框中使用新设计的编辑框控件，应先在对话框模板中的合适位置放置一个普通的编辑框，然后，在OnInitDialog函数中按下面的方式调用SubclassDlgItem即可：

BOOL CMyDialog::OnInitDialog()

{

CDialog::OnInitDialog();

m_MyEdit.SubclassDlgItem(IDC_MYEDIT, this);

return TRUE;

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

一、区别

HWND是句柄，CWnd是MFC窗体类,CWnd中包含HWND句柄成员对象是m_hWnd.

HWND是Windows系统中对所有窗口的一种标识，即窗口句柄。这是一个SDK概念。   

CWnd是MFC类库中所有窗口类的基类。微软在MFC中将所有窗口的通用操作都封装到了这个类中，如：ShowWindow等等，同时它也封装了窗口句柄即m_hWnd成员。

二、如何得到窗口对象指针CWnd * pCWnd

1,在窗体里，直接用 this->，//this就是本窗体的窗口对象指针

2,主窗体的类对象指针：AfxGetApp()->m_pMainWnd->,  //AfxGetApp()->m_pMainWnd就是主窗体的窗口对象指针

3,pCWnd=GetDlgItem(ID_EDIT_NAME);//GetDlgItem 函数可以得到某个控件的窗口对象指针

三、如何得到窗口句柄 HWND Handle

1,Handle=this->m_hWnd

2,GetDlgItem(ID_EDIT_NAME);

3，Handle=::GetDlgItem(this,IDC_STATIC_MODE); //得到本窗体内某个控件的句柄

四、转换

CWnd * pCWnd;

HWND Handle;

pCWnd=FromHandle(Handle);

Handle=GetSafeHwnd(pCWnd);

补充：

由HWnd得到CWnd*：

CWnd wnd;

HWnd hWnd;

wnd.Attach(hWnd);

通常一个窗口资源已经和一个CWnd类的对象关联起来的，由于一般来说这个类是自己创建的，所以自然知道怎么得到指向这个类的指针。如果没有就创建一个CWnd对象，将这个对象与窗口资源的hWnd句柄关联起来。（如上边的语句）。如果用

static CWnd*   CWnd::FromHandle(HWND   hWnd) ；

则返回值是一个暂时的CWnd对象，并且我们确保返回值为非空，也就是hWnd是有效的。 

static CWnd*   CWnd::FromHandlePermanent(HWND   hWnd)  ；

返回的是一个永久的对象。只有在返回的CWnd在类表里已经存在是返回值为非空。

由CWnd获取HWnd就容易多了，因为它的一个成员m_hWnd就是所对应窗口的句柄。

wnd->m_hWnd。

///////////////////////////////////////////////////////////////

CreateCompatibleDC

1,首先明白DC的含义，Windows不允许程序员直接访问硬件，它对屏幕的操作是通过环境设备,也就是DC来完成的。屏幕上的没一个窗口都对应一个DC,可以把DC想象成一个视频缓冲区，对这这个缓冲区的操作，会表现在这个缓冲区对应的屏幕窗口上。

在窗口的DC之外，可以建立自己的DC，就是说它不对应窗口，这个方法就是CreateCompatibleDC，这个DC就是一个内存缓冲区，通过这个DC你可以把和它兼容的窗口DC保存到这个DC中，就是说你可以通过它在不同的DC之间拷贝数据。例如：你先在这个DC中建立好数据，然后在拷贝到窗口的DC就是完成了这个窗口的刷新。

下面是一个代码段：不长

hDeskTop = GetDesktopWindow();

hDeskTopDC = GetDC(hDeskTop);//桌面窗口DC

hMemDC = CreateCompatibleDC(hDeskTopDC);//这建立的就是与桌面窗口兼容的DC

2, 函数功能：该函数创建一个与指定设备兼容的内存设备上下文环境（DC）。

函数原型：HDC CreateCompatibleDC(HDC hdc)；

参数：

hdc：现有设备上下文环境的句柄，如果该句柄为NULL，该函数创建一个与应用程序的当前显示器兼容的内存设备上下文环境。

返回值：如果成功，则返回内存设备上下文环境的句柄；如果失败，则返回值为NULL。

CreateCompatibleDc函数只适用于支持光栅操作的设备，应用程序可以通过调用GetDeviceCaps函数来确定一个设备是否支持这些操作。

当不再需要内存设备上下文环境时，可调用DeleteDc函数删除它。

用双缓冲的话还要再定义一个位图对象吧，然后用CreateCompatibleBitmap建立一个与屏幕显示兼容的位图，再用SelectObject将位图选入到内存显示设备中，不知道是不是这样

首先给出实现的程序，然后再解释，同样是在OnDraw(CDC *pDC)中：

CDC MemDC; //首先定义一个显示设备对象

CBitmap MemBitmap;//定义一个位图对象

//随后建立与屏幕显示兼容的内存显示设备

MemDC.CreateCompatibleDC(NULL);

//这时还不能绘图，因为没有地方画 ^_^

//下面建立一个与屏幕显示兼容的位图，至于位图的大小嘛，可以用窗口的大小，也可以自己定义（如：有滚动条时就要大于当前窗口的大小，在BitBlt时决定拷贝内存的哪部分到屏幕上）

MemBitmap.CreateCompatibleBitmap(pDC,nWidth,nHeight);

//将位图选入到内存显示设备中

//只有选入了位图的内存显示设备才有地方绘图，画到指定的位图上

CBitmap *pOldBit=MemDC.SelectObject(&MemBitmap);

//先用背景色将位图清除干净，这里我用的是白色作为背景

//你也可以用自己应该用的颜色

MemDC.FillSolidRect(0,0,nWidth,nHeight,RGB(255,255,255));

//绘图

MemDC.MoveTo(……);

MemDC.LineTo(……);

//将内存中的图拷贝到屏幕上进行显示

pDC->BitBlt(0,0,nWidth,nHeight,&MemDC,0,0,SRCCOPY);

//绘图完成后的清理

MemBitmap.DeleteObject();

MemDC.DeleteDC();

gdi在sdk应该是很重要的东西，不过嘛自己的水平实在是不怎么够品。长久以来一直都没有认识到 CreateCompatibleDC  的用途到底在这里，不过还好从知道这个东西数起的200天内，我终于还是晓得一些关于这个函数或者说内存dc存在的意义了。

     在这种情况下，假如你要对屏幕进行比较多的gdi函数操作，如果每一步操作都直接对屏幕dc进行操作，那出现的大多数可能性都是屏幕的闪烁。一个很好的解决方法就是使用内存dc，将这些操作全部先在内存dc上操作，然后依次性在屏幕上进行操作。

  例如：如果你单单使用bitblt在屏幕上拷贝一个图，那可以直接使用屏幕的dc。但是如果你要先设置背景(fillrect)然后再bitblt的话，这就涉及到两个屏幕dc的操作，这样的话屏幕很容易闪烁。

3,CreatCompatibleDC()创建了一个和当前屏幕的DC兼容的内存DC（DC就是设备上下文的意思，设备上下文就是当前的这个窗体的一些属性，譬如说他使用的画刷，画笔等等），在绘制位图的时候，你必须要在内存中建立这样的一个和当前设备的环境兼容的DC，

也就是用这个函数建立，这样你才能把位图加载到这块内存里，然后在利用BitBlt函数将位图从内存复制到屏幕DC上，位图才能显示出来。这里涉及到一个概念：设备相关位图（DDB），设备无关位图（DIB）。GetDC()获取设备指针,

该函数检索一指定窗口的客户区域或整个屏幕的显示设备上下文的句柄.以后可以在GDI函数中用该句柄绘图. 也就是取得设备上下文，供绘图或者其他的行为使用

/////////////////////////////////////////////////////////////////////////////

浅拷贝和深拷贝

位拷贝拷贝的是地址（也叫浅拷贝），而值拷贝则拷贝的是内容（深拷贝）。深拷贝和浅拷贝可以简单理解为：

如果一个类拥有资源，当这个类的对象发生复制过程的时候，资源重新分配，这个过程就是深拷贝，反之，没有重新分配资源，就是浅拷贝。

///////////////////////////////////////////////////////////

时间控件的用法：

CDateTimeCtrl;用百度查CTime可以了解，关于CString和CTime的转换；可以得到相应的解答；

【VC】 CDateTimeCtrl使用说明

1.CDateTimeCtrl 格式

  CDateTimeCtrl 设定为“ShowNone” 为 True

2.Format 初始化

   CDateTimeCtrl m_Date1;

  m_Date1.SetFormat(_T("yyyy'- 'MM'- 'dd"));

3.Format初始化选择确认框为空

   CDateTimeCtrl m_Date1;

   ::SendMessage( this->m_Date1.m_hWnd,(UINT)DTM_SETSYSTEMTIME,GDT_NONE, NULL);

4.Format初始化选择确认框变化是产生的动作

voidPageDialog4::OnDtnDatetimechangeDatetimepicker1(NMHDR *pNMHDR,LRESULT *pResult)

{

 LPNMDATETIMECHANGE pDTChange =reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);

 // TODO: 在此添加控件通知处理程序代码

   if(pDTChange->dwFlags == GDT_NONE)

    {

     this->m_Date2.EnableWindow(FALSE);

    }

    else if (pDTChange->dwFlags == GDT_VALID)

    {

         if( this->m_Date2.IsWindowEnabled() == FALSE)

         {

               this->m_Date2.EnableWindow(TRUE);

               CTime theTime = CTime::GetCurrentTime();

               this->m_Date1.SetTime(&theTime);

               this->m_Date2.SetTime(&theTime);   

            }

   }

   *pResult= 0;

}

5.获取时间

 CTimetimeTime;

 CString szDate;

 CDateTimeCtrl* pCtrl =&this->m_Date1;

 DWORD dwResult =pCtrl->GetTime(timeTime);

 if (dwResult == GDT_VALID)

 {

     szDate = timeTime.Format(TEXT("%Y/%m/%d"));

 }

 6.根据Format格式设定时间

       CTime theTime = CTime::GetCurrentTime();

       this->m_Date1.SetTime(&theTime);

////////////////////////////////////////////////////////////////////////////

CAbout::IDD是相当于CDialog的构造函数CDialog(UINT   nIDTemplate,   CWnd   *pParentWnd)中的第一个参数，

好象从CDialog派生来的类都经常有象CXxxxx::IDD这种形式的使用

///////////////////////////////////////////////////////////////////////////

Visual Studio初始化命令  

此命令会运行几分钟时间，与此同时，Visual Studio 清除设置并将其自身重置到其最初的状态。

1. 关闭所有 Visual Studio实例。 

2. 单击“开始”，选择“运行...”。 

3. 键入“devenv.exe /resetuserdata”。 

此命令会运行几分钟时间，与此同时，Visual Studio 清除设置并将其自身重置到其最初的状态。此时，

您可以打开“任务管理器”来检查 devenv.exe 进程是否仍在运行。在结束运行后，可以重新启动 Visual Studio。

此时会再次出现首次启动对话框，就像在计算机上第一次运行 Visual Studio 一样。

///////////////////////////////////////////////////////////////////////////////

函数功能：调入加速键表。该函数调入指定的加速键表。

函数原型：HACCEL LoadAccelerators（HINSTANCE hlnstance，LPCTSTR lpTableName）；

//////////////////////////////////////////////////////////////////////////////




