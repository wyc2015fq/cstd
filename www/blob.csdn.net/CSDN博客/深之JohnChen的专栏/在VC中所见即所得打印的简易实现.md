# 在VC中所见即所得打印的简易实现  - 深之JohnChen的专栏 - CSDN博客

2005年09月06日 11:42:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：2045


                在VC++6.0中用应用程序向导（AppWizard）生成的单文档或多文档程序提供了对打印功能的实现，但遗憾的是如果对自动生成的框架程序不做任何改进，打印出来的文档或图形和屏幕上的显示相比就会特别小。为什么会这样呢？
　　本文对这种现象的原因和MFC的打印机制进行了深入的分析，并提出了一种特别简单的方法，在原有的程序中只需加入几行代码就能解决这一问题，实现所见即所得的打印。 

　　首先，分析MFC的打印机制，把原理弄清楚了，就不难明白现象形成的原因和提出解决办法。MFC应用程序的核心是文档对象以及相关的视图窗口的概念，即CDocument类和CView类的构成和关系，简单地说CDocument类负责数据的生成和储存，CView类负责数据的显示和用户交互。输出到屏幕和输出到打印机都是数据的显示，实质上是一样的，所以打印功能也是由CView类来实现的。 

　　在CView类中由应用程序向导自动生成的源代码提供了一个OnDraw(CDC* pDC)的函数，通过重载这个函数，利用它提供的pDC（设备上下文）指针，可以在屏幕上显示各种图形和数据。CView类的打印是通过OnPrint(CDC* pDC, CPrintInfo* pInfo)这个函数实现的，应用程序向导自动生成的源代码中没有这个函数的框架，而这个函数对打印的实现就是简单地调用OnDraw(CDC* pDC)这个函数，把打印机的设备上下文指针pDC传递给OnDraw(CDC* pDC)函数。 

　　可见CView类对输出到屏幕和输出到打印机的处理都是一样的，只是换了一个设备上下文而已，那么为什么输出到打印机的图像特别小呢？ 

　　这与VC采用的缺省的坐标映射方式MM_TEXT有关，这种方式的好处是用户图形坐标和设备的象素完全一致。但是在屏幕的象素大小为800*600时，每逻辑英寸包含的屏幕象素为96，而打印机的点数却要多好几倍，如当打印机为HP LaserJet 6L时每逻辑英寸包含的打印机点数为600，也就是说打印机的清晰度比屏幕要高得多。 

　　这样的后果就是在屏幕上显示出来的满屏图像在打印出来的纸上却只有一点点大，怎么解决这个问题呢？一种简单的方法就是转换坐标映射方式，使得打印时采用的坐标比例比显示时采用的坐标比例相应地大若干倍，就可以解决这一问题。 

　　下面将给出详细的方法。

　　注意到CView类在进行显示和打印之前都会调用virtual void OnPrepareDC( CDC* pDC, CPrintInfo* pInfo = NULL )这个虚拟成员函数来准备设备上下文，我们可以在CView类中重载这个虚拟成员函数，进行坐标转换。 

　　首先用VC的ClassWizard实现对OnPrepareDC( CDC* pDC, CPrintInfo* pInfo = NULL )函数的重载，ClassWizard生成的源代码如下： 
　　void CTempView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo) 
　　{ // TODO: Add your specialized code here and /or call the base class

　　     CView::OnPrepareDC(pDC, pInfo);

　　} 

　　我们只需在源代码中加入以下几行代码即可，如下： 

　　void CPrintSameView::OnPrepareDC

　　(CDC* pDC, CPrintInfo* pInfo) 

　　{ CView::OnPrepareDC(pDC, pInfo);

　　pDC->SetMapMode(MM_ANISOTROPIC); //转换坐标映射方式 

　　CSize size = CSize(800, 560);

　　pDC->SetWindowExt(size);

　　//确定窗口大小 //得到实际设备每逻辑英寸的象素数量 int xLogPixPerInch = pDC->
GetDeviceCaps(LOGPIXELSX); 

　　int yLogPixPerInch = pDC- >GetDeviceCaps(LOGPIXELSY);

　　//得到设备坐标和逻辑坐标的比例 long xExt = (long)size.cx * xLogPixPerInch/96 ; 

　　long yExt = (long)size.cy * yLogPixPerInch/96 ;

　　pDC->SetViewportExt((int)xExt, (int)yExt);

　　//确定视口大小 }  
　　如上所示，首先将坐标映射方式改变为MM_ANISOTROPIC方式，即各向异性的意思，在这种坐标方式下，X轴和Y轴的逻辑单位可以进行任意的缩放。改变坐标映射方式后，就要确定窗口大小和视口大小，注意窗口大小就是我们在屏幕上所见的尺寸，而视口大小则是实际设备，如打印机等，和显示器设备每逻辑英寸的象素数量比较所得的比例尺寸。通过函数得到显示器和打印机每逻辑英寸的象素数量，然后对视口大小进行相应的缩放，就可以使得屏幕上的显示和打印机的输出是一致的了。 

　　这样，只通过几行简单的代码，我们就实现了所见即所得的打印。 

