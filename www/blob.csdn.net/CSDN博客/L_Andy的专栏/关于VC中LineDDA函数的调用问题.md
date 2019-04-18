# 关于VC中LineDDA函数的调用问题 - L_Andy的专栏 - CSDN博客

2013年12月13日 15:11:49[卡哥](https://me.csdn.net/L_Andy)阅读数：2205


该函数的原型如下： 
BOOL LineDDA(int nXStart, int nYStart, int nXEnd, int nYEnd, LINEDDAPROC lpLineFunc, LPARAM lpData); 
参数说明如下： 
nXStart：起点的X值
 
nYStart：起点的Y值
 
nXEnd：终点的X值 

nYEnd：终点的Y值 

lpLineFunc：回调函数的地址 

lpData：用户自定义参数（这个参数会传给回调函数） 

这个函数和动画其实没什么关系，它的功能就是计算出连接两点的线段上的每一个屏幕像素的坐标，这两个点的坐标已经在函数的前四个参数中给出。
每计算出一个坐标，该函数就会调用第五个参数所指的回调函数，我们可以在回调函数中完成一些简单的操作，以实现动画效果。 
回调函数的原型是： VOID CALLBACK LineDDAProc(int X, int Y, LPARAM lpData); 
前两个参数是点的坐标，第三个参数就是由LineDDA传过来的自定义参数，是由我们自己指定的，传什么都行。 :) 
详解：
实现以复杂线条为基础的图形绘图
在GIS（地理信息系统）类软件设计中经常需要在绘图时使用一些相对固定但又频繁使用的一些用以代表地理状态的符号如河流、铁路、海岸线等等。每一种符号均有其各自的风格，但在不同的位置的具体表示却不尽相同，比如代表铁路的符号是一段黑白相间的细矩形，但有时是平直的，在拐弯时用弯曲的矩形来表示。因此对于上述符号的绘制一般不易用固定的图标去实现，而多采用灵活多变的用函数来直接绘制的方法。显然作为GIS基本符号的图形一般都是相对比较复杂的线条，在MFC提供的基本类库中并未提供可以直接使用的相关函数。即使是在绘图功能比较强大的CDC中也仅仅提供了LineTo（）、SetPixel（）等一些通用的最基本的绘图函数，虽然也可以使用这些基本函数来绘制GIS里的基本符号，但这是效率比较低下的一种办法，这在大量的绘图操作中将会表现的比较明显，因此不宜提倡。本文下面将介绍一种使用Win32 API函数LineDDA来绘制复杂风格线条的方法来解决上述类似问题。
步骤/方法
1
把复杂风格的线条作为基本绘图操作
　　在Windows NT 3.1中首次出现了Win32 API函数LineDDA，用以创建虚线或点划线，以及其他一些更复杂的线条。因此可以从该函数入手来解决以复杂线条作为基本绘图操作的问题。LineDDA函数主要是通过回调机制来实现其功能的，其绘制线条总是被使用当前显示分辨率的缺省转换和映射模式来计算的。如果不使用缺省方式，也可以向其回调函数LineDDAProc传递手工转换的X和Y值。LineDDA是一个32位的图形设备接口库函数调用，从如下所示的函数原形中可以看出其入口参数是一组线条坐标、一个回调函数的地址以及一个指向应用程序定义数据的指针：
　　BOOL LineDDA（ int nXStart， // 线条起点的X坐标
　　int nYStart， // 线条起点的Y坐标
　　int nXEnd， // 线条终点的X坐标
　　int nYEnd， // 线条终点的Y坐标
　　LINEDDAPROC lpLineFunc， // 回调函数的指针
　　LPARAM lpData // 应用程序定义数据的指针）；
2
　　由lpLineFunc指针指向的回调函数将在除终点外的线段的每个点上被调用，显然这里是实现复杂线条算法的最佳地方。该回调函数一般可以定义如下：
　　VOID CALLBACK LineDDAProc（int X， // 被求值点的X坐标
　　int Y， // 被求值点的Y坐标
　　LPARAM lpData // 应用程序定义数据的指针）；
　　在实际调用时一般选当前的设备环境句柄作为应用程序定义数据的指针，该指针会在调用LineDDA函数时将其传送给回调函数LineDDAProc，在回调函数中通过CDC* pDC= （CDC*）lpData；强制转换即可在其中使用当前的设备环境句柄，并通过该句柄进行绘图等工作。
　　下面通过一个具体实例来做进一步的介绍：类似于CDC类库中的绘图函数，当我们把某种较复杂线条作为一个整体元素进行绘图操作时有以下几个元素需要确定：起始点、终止点坐标，绘图时用的颜色等。因此可以初步确定绘制复杂线条的函数采取如下形式：
　　void CLineDDAView::DrawWave(CPoint ptFrom, CPoint ptTo, COLORREF crValue)
　　{
　　g_crValue=crValue; //绘制图形用的颜色
　　CDC* pDC=GetDC(); //获取当前设备环境句柄
　　//通过LineDDA函数调用回调函数Proc以完成复杂线条的绘图工作
　　LineDDA(ptFrom.x,ptFrom.y,ptTo.x,ptTo.y,(LINEDDAPROC)Proc,(long)pDC);
　　ReleaseDC(pDC); //释放申请到的设备环境句柄
　　}
3
　　具体的实质性工作一般都是放在回调函数中进行的，在这里只是象征性的实现一个波浪曲线的绘图工作：
　　VOID CALLBACK Proc(int X, int Y, LPARAM lpData)
　　{
　　//使用lpData传递用于绘图的CDC对象的引用
　　CDC* pDC;
　　pDC = (CDC*)lpData;
　　//该回调函数将在线上每个点处被调用，因此随着X坐标的变化，纵坐标Y+sin(X)
　　//也就呈正弦波形波动，通过CDC类的SetPixel函数将计算出的波浪线上的每一点
　　//都显示出来。
　　pDC->SetPixel(X,Y+sin(X),g_crValue);
　　}
　　在应用时应当根据需求的不同采取不同的线条风格设计算法，甚至可以在回调函数中使用TextOut函数实现文本文字的曲线显示等效果。在使用我们设计的DrawWave函数进行绘图操作时，可以象是使用CDC提供的LineTo等函数一样非常简单的画出一条在CDC类中并未提供的波浪线。
　　
END
总结
1
LineDDA函数为画出复杂线条类型提供了一种解决办法。虽然是一种Win32的解决办法，在MFC中并不直接支持，但作为一个一般的解决办法，它还是十分有用的，而且它能解决一些没有直接解决办法的比较独特的问题。
2
通过对LineDDA函数的挖掘可以设计出适合自己需要的可以作为绘图基本单元的复杂线条。本程序在Windows 98下，以Microsoft Visual C++ 6.0编译通过。

