# WM_PAINT介绍 及OnPaint()函数的作用原理 - xqhrs232的专栏 - CSDN博客
2012年09月19日 16:51:50[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：794标签：[c](https://so.csdn.net/so/search/s.do?q=c&t=blog)
个人分类：[C/C++/VC++/VS](https://blog.csdn.net/xqhrs232/article/category/906934)
原文地址::[http://blog.sina.com.cn/s/blog_4b3c1f950100dmhm.html](http://blog.sina.com.cn/s/blog_4b3c1f950100dmhm.html)
*  WM_PAINT是窗口每次重绘都会产生的一个消息。
*  MFC中 OnPaint()是对这个消息的反应函数（mfc
 的 CWnd::OnPaint() 没做什么，只是丢给系统处理，下面就是介绍它在系统中的处理流程）
**关于 WM_PAINT 消息**
系统会在多个不同的时机发送WM_PAINT消息：当第一次创建一个窗口时，当改变窗口的大小时，当把窗口从另一个窗口背后移出时，当最大化或最小化窗口时，等等，这些动作都是由系统管理的，应用只是被动地接收该消息，在消息处理函数中进行绘制操作；大多数的时候应用也需要能够主动引发窗口中的绘制操作，比如当窗口显示的数据改变的时候，这一般是通过InvalidateRect和InvalidateRgn函数来完成的。
InvalidateRect和
 InvalidateRgn把指定的区域加到窗口的Update Region（Update Region：Windows窗口更新区域/无效区域）中，当应用的消息队列没有其他消息时，如果窗口的Update Region不为空时，系统就会自动产生WM_PAINT消息。
   像这种通过InvalidateRect和InvalidateRgn来使窗口区域无效，依赖于系统在合适的时机发送WM_PAINT消息的机制实际上是一种异步工作方式，也就是说，在无效化窗口区域和发送WM_PAINT消息之间是有延迟的；有时候这种延迟并不是我们希望的，这时我们当然可以在无效化窗口区域后利用SendMessage
 发送一条WM_PAINT消息来强制立即重画，但不如使用Windows GDI为我们提供的更方便和强大的函数：UpdateWindow和RedrawWindow。UpdateWindow会检查窗口的Update
 Region，当其不为空时才发送WM_PAINT消息；RedrawWindow则给我们更多的控制：是否重画非客户区和背景，是否总是发送 WM_PAINT消息而不管Update Region是否为空等。
    系统为什么不在调用Invalidate时发送WM_PAINT消息呢？又为什么非要等应用消息队列为空时才发送WM_PAINT消息呢？这是因为系统把在窗口中的绘制操作当作一种低优先级的操作，于是尽可能地推后做。不过这样也有利于提高绘制的效率：两个WM_PAINT消息之间通过 InvalidateRect和InvaliateRgn使之失效的区域就会被累加起来，然后在一个WM_PAINT消息中一次得到更新，不仅能避免多次重复地更新同一区域，也优化了应用的更新操作。
无效区域的坐标并不附带在WM_PAINT消息的参数中.实际上，Windows为每个窗口维护一个“绘图信息结构”，无效区域的坐标就在其中，每当消息循环空的时候，如果Windows发现存在一个无效区域，就会放入一个WM_PAINT消息。那么“绘图信息结构”怎么获取呢？BeginPaint函数的第二个参数就是一个绘图信息结构的缓冲区地址，windows会在这里返回绘图信息结构，结构中包含了无效区域的位置和大小，绘图信息结构的定义如下：
typedef　struct　tagPAINTSTRUCT　{......}PAINTSTRUCT；
**系统OnPaint()的内部处理流程**
**一 ：**
   先执行OnEraseBkgnd，擦除背景（如果想自绘控件，这个函数直接return TRUE就可以了，这样就不会擦除背景，不会闪）
**OnEraseBkGnd与OnPaint的区别与联系**
在OnEraseBkGnd中,如果你不调用原来缺省的OnEraseBkGnd只是重画背景则不会有闪烁.而在OnPaint里面,由于它隐含的调用了OnEraseBkGnd,而你又没有处理OnEraseBkGnd
 函数,这时就和窗口缺省的背景刷相关了.缺省的 OnEraseBkGnd操作使用窗口的缺省背景刷刷新背景(一般情况下是白刷),而随后你又自己重画背景造成屏幕闪动.
OnEraseBkGnd不是每次都会被调用的.如果你调用Invalidate的时候参数为TRUE,那么在OnPaint里面隐含调用BeginPaint的时候就产生WM_ERASEBKGND消息,如果参数是FALSE,则不会重刷背景.
**ZYP解释：**void Invalidate( BOOL bErase = TRUE ); 该函数的作用是使整个窗口客户区无效。窗口的客户区无效意味着需要重绘，参数bErase为TRUE时，重绘区域内的背景将被重绘即擦除，否则，背景将保持不变。调用Invalidate等函数后窗口不会立即重绘，这是由于WM_PAINT消息的优先级很低，它需要等消息队列中的其它消息发送完后才能被处理。
OnPaint里面会调用BeginPaint函数自动设置显示设备内容的剪切区域而排除任何更新区域外的区域更新区域。如果更新区域被标记为可擦除的，BeginPaint发送一个WM_ERASEBKGND消息给窗口。WM_ERASEBKGND消息的响应函数既是OnEraseBkGnd（）
所以解决方法有三个半:
1.用OnEraseBkGnd实现,不要调用原来的OnEraseBkGnd函数.
2.用OnPaint实现,同时重载OnEraseBkGnd,其中直接返回.
3.用OnPaint实现,创建窗口时设置背景刷为空
4.用OnPaint实现,但是要求刷新时用Invalidate(FALSE)这样
的函数.(不过这种情况下,窗口覆盖等造成的刷新还是要闪一
下,所以不是彻底的解决方法)
都挺简单的.
|在MFC中 任何一個window元件的繪圖 都是放在這兩個member function中在設定上 OnEraseBkgnd()是用來畫底圖的 而OnPaint()是用來畫主要物件的舉例說明 一個按鈕是灰色的 上面還有文字則OnEraseBkgnd()所做的事就是把按鈕畫成灰色而OnPaint()所做的事 就是畫上文字既然這兩個member function都是用來畫出元件的那為何還要分OnPaint() 與 OnEraseBkgnd() 呢其實OnPaint() 與 OnEraseBkgnd() 特性是有差的1. OnEraseBkgnd()的要求是快速在裡面的繪圖程式最好是不要太耗時間因為每當window元件有任何小變動都會馬上呼叫OnEraseBkgnd()2. OnPaint() 是只有在程式有空閒的時候才會被呼叫3. OnEraseBkgnd() 是在 OnPaint() 之前呼叫的所以 OnPaint()被呼叫一次之前 可能會呼叫OnEraseBkgnd()好幾次如果我們是一個在做圖形化使用者介面的人常會需要把一張美美的圖片設為我們dialog的底圖把繪圖的程式碼放在OnPaint() 之中 可能會常碰到一些問題比方說拖曳一個視窗在我們做的dialog上面一直移動則dialog會變成灰色 直到動作停止才恢復這是因為每次需要重繪的時候 程式都會馬上呼叫OnEraseBkgnd()OnEraseBkgnd()就把dialog畫成灰色而只有動作停止之後 程式才會呼叫OnPaint() 這時才會把我們要畫的底圖貼上去這個問題的解法 比較差點的方法是把OnEraseBkgnd() 改寫成不做事的function如下所示BOOL CMyDlg::OnEraseBkgnd(CDC* pDC){return TRUE;}以上本來是會呼叫CDialog::OnEraseBkgnd() 但是如果我們不呼叫的話程式便不會畫上灰色的底色了Q:基于对话框的程序中如何重载OnEraseBkGnd()函数A:这是一个消息WM_ERASEBKWND  在CLASS   WIZARD中    选择CLASSINFO页面    在MESSAGEFILTER中的选项设在WINDOW就可以看到这个消息了.比較好的做法是直接將繪圖的程式從OnPaint()移到OnEraseBkgnd()來做如下所示// m_bmpBKGND 為一CBitmap物件 且事先早已載入我們的底圖// 底圖的大小與我們的視窗client大小一致BOOL CMyDlg::OnEraseBkgnd(CDC* pDC){CRect rc;GetUpdateRect(&rc);CDC srcDC;srcDC.CreateCompatibleDC(pDC);srcDC.SelectObject(m_bmpBKGND);pDC->BitBlt(rc.left,rc.top,rc.GetWidth(),rc.GetHeight(),&srcDC,rc.left,rc.top,SRCCOPY);return TRUE;}特別要注意的是 取得重畫大小是使用GetUpdateRect() 而不是GetClientRect()如果使用GetClientRect() 會把不該重畫的地方重畫来自:http://hi.baidu.com/����_�ֵ�/blog/item/2f6d3b10b6c622fac2ce79a5.html|BOOL CMyDlg::OnEraseBkgnd(CDC* pDC){return TRUE;}|// m_bmpBKGND 為一CBitmap物件 且事先早已載入我們的底圖// 底圖的大小與我們的視窗client大小一致BOOL CMyDlg::OnEraseBkgnd(CDC* pDC){CRect rc;GetUpdateRect(&rc);CDC srcDC;srcDC.CreateCompatibleDC(pDC);srcDC.SelectObject(m_bmpBKGND);pDC->BitBlt(rc.left,rc.top,rc.GetWidth(),rc.GetHeight(),&srcDC,rc.left,rc.top,SRCCOPY);return TRUE;}|
|----|----|----|
|BOOL CMyDlg::OnEraseBkgnd(CDC* pDC){return TRUE;}| | |
|// m_bmpBKGND 為一CBitmap物件 且事先早已載入我們的底圖// 底圖的大小與我們的視窗client大小一致BOOL CMyDlg::OnEraseBkgnd(CDC* pDC){CRect rc;GetUpdateRect(&rc);CDC srcDC;srcDC.CreateCompatibleDC(pDC);srcDC.SelectObject(m_bmpBKGND);pDC->BitBlt(rc.left,rc.top,rc.GetWidth(),rc.GetHeight(),&srcDC,rc.left,rc.top,SRCCOPY);return TRUE;}| | |
特別要注意的是 取得重畫大小是使用GetUpdateRect() 而不是GetClientRect()
如果使用GetClientRect() 會把不該重畫的地方重畫
来自:http://hi.baidu.com/����_�ֵ�/blog/item/2f6d3b10b6c622fac2ce79a5.html
**二 ：**
     系统的Onpaint中调用了OnDraw，但如果我们自己继承了一个OnPaint函数又没有显式调用OnDraw，则OnDraw就不会被调用,OnInitialUpdate在OnDraw之前，是窗口被创建以后调用的第一个函数。
**MFC中OnDraw与OnPaint的区别**
在OnPaint中调用OnDraw，一般来说，用户自己的绘图代码应放在OnDraw中。
OnPaint()是CWnd的类成员，负责响应WM_PAINT消息。OnDraw()是CVIEW的成员函数，没有响应消息的功能.当视图变得无效时（包括大小的改变，移动，被遮盖等等），Windows发送WM_PAINT消息。该视图的OnPaint 处理函数通过创建CPaintDC类的DC对象来响应该消息并调用视图的OnDraw成员函数.OnPaint最后也要调用OnDraw,因此一般在OnDraw函数中进行绘制。
The WM_PAINT message is sent when the UpdateWindow or RedrawWindow member function is called.
在OnPaint中，将调用BeginPaint，用来获得客户区的显示设备环境，并以此调用GDI函数执行绘图操作。在绘图操作完成后，将调用EndPaint以释放显示设备环境。而OnDraw在BeginPaint与EndPaint间被调用。（一个应用程序除了响应WM_PAINT消息外，不应该调用BeginPaint。每次调用BeginPaint都应该有相应的EndPaint函数。）
1) 在mfc结构里OnPaint是CWnd的成员函数. OnDraw是CView的成员函数.
2) OnPaint()调用OnDraw()，OnPrint也会调用OnDraw()，所以OnDraw()是显示和打印的共同操作。
OnPaint是WM_PAINT消息引发的重绘消息处理函数，在OnPaint中会调用OnDraw来进行绘图。OnPaint中首先构造一个CPaintDC类得实例，然后一这个实例为参数来调用虚函数OnPrepareDC来进行一些绘制前的一些处理，比设置映射模式，最后调用OnDraw。而OnDraw和OnPrepareDC不是消息处理函数。所以在不是因为重绘消息所引发的OnPaint导致OnDraw被调用时，比如在OnLButtonDown等消息处理函数中绘图时，要先自己调用OnPrepareDC。
至于CPaintDC和CClientDC根本是两回事情 CPaintDC是一个设备环境类，在OnPaint中作为参数传递给OnPrepareDC来作设备环境的设置。真正和CClientDC具有可比性的是CWindowDC，他们一个是描述客户区域，一个是描述整个屏幕。
如果是对CVIEW或从CVIEW类派生的窗口绘图时应该用OnDraw。
**OnDraw()****和OnPaint**()有什么区别呢？
首先：我们先要明确CView类派生自CWnd类。而OnPaint()是CWnd的类成员，同时负责响应WM_PAINT消息。OnDraw()是CVIEW的成员函数，并且没有响应消息的功能。这就是为什么你用VC成的程序代码时，在视图类只有OnDraw没有OnPaint的原因。而在基于对话框的程序中，只有OnPaint。
其次：我们在第《每天跟我学MFC》3的开始部分已经说到了。要想在屏幕上绘图或显示图形，首先需要建立设备环境DC。其实DC是一个数据结构，它包含输出设备（不单指你17寸的纯屏显示器，还包括打印机之类的输出设备）的绘图属性的描述。MFC提供了CPaintDC类和CWindwoDC类来实时的响应，而CPaintDC支持重画。当视图变得无效时（包括大小的改变，移动，被遮盖等等），Windows 将 WM_PAINT 消息发送给它。该视图的OnPaint 处理函数通过创建 CPaintDC 类的DC对象来响应该消息并调用视图的
 OnDraw 成员函数。通常我们不必编写重写的 OnPaint 处理成员函数。
///CView默认的标准的重画函数
void CView::OnPaint() //见VIEWCORE.CPP
{
CPaintDC
 dc(this);
OnPrepareDC(&dc)；
OnDraw(&dc); //调用了OnDraw
}
///CView默认的标准的OnPrint函数
void CView::OnPrint(CDC* pDC, CPrintInfo*)
{
ASSERT_VALID(pDC);
OnDraw(pDC); // Call Draw
}
既然OnPaint最后也要调用OnDraw,因此我们一般会在OnDraw函数中进行绘制。下面是一个典型的程序。
///视图中的绘图代码首先检索指向文档的指针，然后通过DC进行绘图调用。
void CMyView::OnDraw( CDC* pDC )
{
CMyDoc* pDoc = GetDocument();
CString s = pDoc->GetData();
GetClientRect( &rect ); // Returns a CString CRect rect;
pDC->SetTextAlign( TA_BASELINE | TA_CENTER );
pDC->TextOut( rect.right / 2, rect.bottom / 2, s, s.GetLength() );
}
最后：现在大家明白这哥俩之间的关系了吧。因此我们一般用OnPaint维护窗口的客户区（例如我们的窗口客户区加一个背景图片），用OnDraw维护视图的客户区（例如我们通过鼠标在视图中画图）。当然你也可以不按照上面规律来，只要达到目的并且没有问题，怎么干都成。补充：我们还可以利用Invalidate(),ValidateRgn(),ValidateRect()函数强制的重画窗口，具体的请参考MSDN吧。
OnDraw中可以绘制用户区域。OnPaint中只是当窗口无效时重绘不会保留CClientDC绘制的内容。
这两个函数有区别也有联系：
1、区别：OnDraw是一个纯虚函数，定义为virtual void OnDraw( CDC* pDC ) = 0;　而OnPaint是一个消息响应函数，它响应了WM＿PANIT消息，也是是窗口重绘消息。
2、联系：我们一般在视类中作图的时候，往往不直接响应WM＿PANIT消息，而是重载OnDraw纯虚函数，这是因为在CVIEW类中的WM＿PANIT消息响应函数中调用了OnDraw函数，如果在CMYVIEW类中响应了WM＿PAINT消息，不显式地调用OnDraw函数的话，是不会在窗口重绘的时候调用OnDraw函数的。
应用程序中几乎所有的绘图都在视图的 OnDraw 成员函数中发生，必须在视图类中重写该成员函数。（鼠标绘图是个特例，这在通过视图解释用户输入中讨论。）
OnDraw
 重写：
通过调用您提供的文档成员函数获取数据。
通过调用框架传递给 OnDraw 的设备上下文对象的成员函数来显示数据。
当文档的数据以某种方式更改后，必须重绘视图以反映该更改。默认的 OnUpdate 实现使视图的整个工作区无效。当视图变得无效时，Windows 将 WM_PAINT 消息发送给它。该视图的 OnPaint 处理函数通过创建 CPaintDC 类的设备上下文对象来响应该消息并调用视图的 OnDraw 成员函数。
当没有添加WM_PAINT消息处理时,窗口重绘时,由OnDraw来进行消息响应...当添加WM_PAINT消息处理时,窗口重绘时,WM_PAINT消息被投递,由OnPaint来进行消息响应.这时就不能隐式调用OnDraw了.必须显式调用( CDC
 *pDC=GetDC(); OnDraw(pDC); )..
隐式调用:当由OnPaint来进行消息响应时,系统自动调用CView::OnDraw(&pDC).
想象一下，窗口显示的内容和打印的内容是差不多的，所以，一般情况下，统一由OnDraw来画。窗口前景需要刷新时，系统会会调用到OnPaint，而OnPaint一般情况下是对DC作一些初始化操作后，调用OnDraw()。
**总结：**
OnEraseBkGnd()，是窗口背景需要刷新时由系统调用的。明显的一个例子是设置窗口的背景颜色（你可以把这放在OnPaint中去做，但是会使产生闪烁的现象）。 
至于怎么界定背景和前景，那要具体问题具体分析了，一般情况下，你还是很容易区别的吧。
的确，OnPaint()用来响应WM_PAINT消息，视类的OnPaint()内部根据是打印还是屏幕绘制分别以不同的参数调用OnDraw()虚函数。所以在OnDraw()里你可以区别对待打印和屏幕绘制。
其实，MFC在进行打印前后还做了很多工作，调用了很多虚函数，比如OnPreparePrint()等。
PS.
OnCtlColor()是当窗口的控件需要绘制时发生的，它将绘制窗口的控件。
**另外OnInitialUpdate**
视图窗口完全建立后第一个被框架调用的函数。框架在第一次调用OnDraw前会调用OnInitialUpdate，因此OnInitialUpdate是设置滚动视图的逻辑尺寸和映射模式的最合适的地方。
