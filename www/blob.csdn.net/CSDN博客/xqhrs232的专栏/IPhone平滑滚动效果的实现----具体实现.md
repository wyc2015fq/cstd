# IPhone平滑滚动效果的实现----具体实现 - xqhrs232的专栏 - CSDN博客
2010年08月03日 18:40:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：912
原文地址::[http://blog.csdn.net/norains/archive/2010/06/14/5670538.aspx](http://blog.csdn.net/norains/archive/2010/06/14/5670538.aspx)
//=====================================================================
//TITLE:
//    IPhone平滑滚动效果的实现
//AUTHOR:
//    norains
//DATE:
//    Monday 14-June-2010
//Environment:
//    Windows CE 5.0
//=====================================================================
    用过IPhone的人应该都记得，IPhone界面切换时有个很有意思的效果：当前窗口从左边移出，显示窗口从右边进入。这个比WinCE原生的一闪然后方框出现的形态更富有想象力。
    那么，我们能不能在WinCE上模拟出IPhone的这种效果呢？答案当然可以--否则也不会有此篇文章了。:)
    效果的原理不是很复杂，但涉及到一些基础的知识，如果你是WinCE新手，可能理解起来有点吃力。不过没关系，让我们一点点来吧！
    在WinCE里面，任何我们需要绘制的图像，最终还是要归结于HDC。HDC是WinCE的设备句柄，我们简单点认为，它就是一个显示的区域，你往里面写了什么，那么它在屏幕上就会显示相应的内容。它的获取，很简单，大体上有两种方式，一种是通过GetDC或GetWindowDC函数，另一种则是通过BeginPaint获取。前者在大部分情况下都可以使用，除了WM_PAINT消息的响应函数，而这恰好是BeginPaint的发挥效力的地方。
    当然HDC所涉及的方面还远远不止上文所提到的那几句话，但对于理解本篇的内容，已经足矣。
    做过多张图片同时绘制的朋友应该对于"缓存HDC"这一概念不陌生。如果我们直接将所有的图片以此绘制到源HDC中，那么展现在我们面前的将是一张一张图片的逐次显示，造成一种闪烁的假象。这问题出现在程序界面中，是个很糟糕的用户体验。所以我们一般是将所有的图片一次性绘制到一个"缓存HDC"中，然后再一次性地将绘制好的内容输入到源HDC，就避免了闪烁的毛病。
    那"缓存HDC"和我们今天讨论的主题有什么关系呢？其实关系大着呢！甚至可以说，成败的关键就在于这缓存。
    假定我们当前的窗口HDC和切换的窗口的HDC组成两个屏幕大的缓存HDC，然后我们依次从左到右的范围依次绘制，那么映入我们眼帘的不就是平缓的滚动效果了么？
    用文字描述可能有点不太清楚，我们以图片来看看：
    两个不同的图片代表的是不同的窗口，红色的方框指代的是我们的屏幕。当我们从左到右进行绘制时，如果绘制的帧数足够多，在红色框框中看到的，就是平滑的滚动。
    如果以伪代码的形式，那么我们的程序结构就可以如此：
view plaincopy to clipboardprint?
//获取两个窗口的HDC   
 HDC hdcWnd1 = GetRealDC(hWnd1);   
 HDC hdcWnd2 = GetRealDC(hWnd2);   
 //将两个窗口DC组合成一个缓存HDC   
 HDC hdcAll = CombineDC(hdcWnd1,hdcWnd2);   
 //源HDC，即真正绘制到屏幕上的HDC。   
 HDC hdcSource = GetDC(NULL);   
 //如果绘制的起点等于HDC的一半时，意味着已经绘制到最后，直接跳出循环   
 for(DWORD dwPos = 0; dwPos < GetWidthForDC(hdcAll) / 2; ++ dwPos)   
 {   
  //将缓存HDC绘制到源HDC中，以实现平滑滚动的效果   
  BltBit(hdc,   
      0,   
      0,   
      ScreenWidth,   
      ScreenHeight,   
      hdcAll,   
      dwPos,   
      0,   
      SRCCOPY)   
 }   
//获取两个窗口的HDC
 HDC hdcWnd1 = GetRealDC(hWnd1);
 HDC hdcWnd2 = GetRealDC(hWnd2);
 //将两个窗口DC组合成一个缓存HDC
 HDC hdcAll = CombineDC(hdcWnd1,hdcWnd2);
 //源HDC，即真正绘制到屏幕上的HDC。
 HDC hdcSource = GetDC(NULL);
 //如果绘制的起点等于HDC的一半时，意味着已经绘制到最后，直接跳出循环
 for(DWORD dwPos = 0; dwPos < GetWidthForDC(hdcAll) / 2; ++ dwPos)
 {
  //将缓存HDC绘制到源HDC中，以实现平滑滚动的效果
  BltBit(hdc,
      0,
      0,
      ScreenWidth,
      ScreenHeight,
      hdcAll,
      dwPos,
      0,
      SRCCOPY)
 }
    程序的结构很简单，不是么？
    如果你还想更进一步，你还可以通过响应WM_MOUSEMOVE消息，根据其提供的坐标原点，再将上述代码的起始绘制原点更改一下，那么实现IPhone的拖动效果对于你来说也再也不是难事。:)
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/norains/archive/2010/06/14/5670538.aspx](http://blog.csdn.net/norains/archive/2010/06/14/5670538.aspx)
//==========================================================================
具体实现
GDI_WindowMove_Test(void)
{
RETAILMSG(1, (TEXT ("++++GDI_WindowMove_Test( )----/r/n")));
   int i;
   BOOL  bRet;
   BITMAP  bm;
   HBITMAP hSrcoldbmp,hComoldbmp,hComnewbmp;
   HDC   hdcSrc,hdcDest,hdcCom;
   memset(&bm,0,sizeof(bm));
   GetObject(m_hbmp_BackgndBack,sizeof(bm),&bm);
   hdcDest=::GetDC(m_hWnd);
   hdcSrc=CreateCompatibleDC(NULL);
   hdcCom=CreateCompatibleDC(NULL);
   hComnewbmp=CreateCompatibleBitmap(hdcDest,bm.bmWidth*2,bm.bmHeight);//ok
  //hComnewbmp=CreateCompatibleBitmap(hdcCom,bm.bmWidth*2,bm.bmHeight); //no ok
    hComoldbmp=(HBITMAP)SelectObject(hdcCom,hComnewbmp);
 hSrcoldbmp=(HBITMAP)SelectObject(hdcSrc,m_hbmp_BackgndBack);
    DWORD   dwValue;
 dwValue=GetTickCount();
    bRet=BitBlt(hdcCom,0,0,bm.bmWidth,bm.bmHeight,hdcDest,0,0,SRCCOPY);
 RETAILMSG(1, (TEXT ("++++GDI_WindowMove_Test( )----the timer of BitBlt is %d/r/n"),GetTickCount()-dwValue));
    bRet=BitBlt(hdcCom,bm.bmWidth,0,bm.bmWidth,bm.bmHeight,hdcSrc,0,0,SRCCOPY);
   for(i=0;i<=bm.bmWidth;)
   {
     BitBlt(hdcDest,0,0,bm.bmWidth,bm.bmHeight,hdcCom,i,0,SRCCOPY);
  i=i+2;
   }
SelectObject(hdcDest,hSrcoldbmp);
SelectObject(hdcCom,hComoldbmp);
DeleteObject(hComnewbmp);
DeleteDC(hdcSrc);
DeleteDC(hdcDest);
DeleteDC(hdcCom);
}
