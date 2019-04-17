# vc关于dialog绘图闪屏和多层覆盖的处理方法 - 专注于数据挖掘算法研究和应用 - CSDN博客





2011年07月06日 10:26:49[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：4293








 1.加载背景图，刷新时闪屏：

   用双缓存：

CDC *pDC=GetDC();
  CDC memDC;
  memDC.CreateCompatibleDC(pDC);

  CBitmap bitmap;
  //创建与屏幕一样大小的位图
  bitmap.CreateCompatibleBitmap(pDC,  nClientWidth,  nClientHeight);
  CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);
  CBrush* pBrush = new CBrush(RGB(250,235,215));//古董白
  CRect rect(0,0,nClientWidth,nClientHeight);
  memDC.FillRect(rect, pBrush);
  delete pBrush;

  Draw(&memDC);//需要绘画的内容


  pDC->BitBlt(0, 0, nClientWidth,  nClientHeight,  &memDC,  0,  0,  SRCCOPY); //画图
  memDC.SelectObject(pOldBitmap);
  bitmap.DeleteObject(); 



   Ontimer刷新时，invalidate(false)即可，不会闪屏；



2.多层图叠加前一层被覆盖的处理办法：

   同样需要用双缓存，先在内存画在输出到屏幕，如果是多层图叠加，就多次双缓存，需要在BitBlt()函数或StretchBlt()函数设置其最后一个参数(SRCPAINT/SRCINVERT/SRCAND  /SRCCOPY)为SRCAND进行透明化处理，如果第一层用SRCCOPY，而第二层也用SRCCOPY则会将第一层覆盖，因此第二层应设置为SRCAND来透明解决多层图叠加；

3.刷新图层，控件闪烁的解决办法：

   屏幕上输出的图层上面有生成的控件，如按钮，这样刷屏时控件会闪动，解决办法是在OnInitDialog()上设置ModifyStyle(NULL,WS_CLIPCHILDREN|WS_CLIPSIBLINGS); 这样控件就不会闪动。



