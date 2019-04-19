# HDC与CDC的相互转换 - xqhrs232的专栏 - CSDN博客
2010年08月27日 10:46:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1406
CPEN+HPEN,CFONT+HFONT,CPOINT+POINT,CRECT+RECT的对应关系跟HDC与CDC差不多
 HPEN   hpen;
 CPen  *pCPen=::CPen::FromHandle(hpen); 
//------------------------
 CPen  *pCPen;
 HPEN   hpen=(HPEN)pCPen->GetSafeHandle(); //可以类推
1.获取HDC
方法A----通过窗体获取HDC
    HDC   hdc=::GetDC(m_hWnd);
方法B----通过CDC类获取HDC
    CDC*  pDC;
    HDC   hdc=pDC->m_hDC;
方法C----通过CDC类获取HDC
    CDC*  pDC;
    HDC   hdc=pDC->GetSafeHdc();
//================================================================
2.获取CDC
方法A----通过HDC获取CDC
    HDC  hdc;
    CDC* pDC=::CDC::FromHandle(hdc); 
方法B----通过HDC获取CDC
    CDC* pDC
    HDC  hDC;
    pDC=Attach(hDC);
方法C----直接获取当前设备环境
   CDC *pDC=GetDC();
