# 处理WM_NCPAINT消息  - ljx0305的专栏 - CSDN博客
2009年11月27日 15:34:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：4524
 处理WM_NCPAINT消息
    处理WM_NCPAINT消息可以实现自绘窗口边框和标题栏。     手头有个窗口需要自绘边框，于是处理了WM_NCPAINT消息，结果程序初运行时可以正常绘制，一旦最小化或者调整窗口位置后就看不到自绘的边框了。找了半天发现犯了个愚蠢的错误。在获取窗口矩形后未做处理便开始了绘图操作，现在加上平移操作，使要绘制的矩形左上角坐标归零后一切正常。     窗口包含WS_BORDER和WS_THICKFRAME风格，边框大小一般为4。     代码如下： view plaincopy to clipboardprint?
if( uMsg == WM_NCPAINT )   
{   
    HDC hdc;   
    hdc = GetWindowDC(hWnd);   
    // Paint into this DC   
    RECT rcWin;   
    GetWindowRect(hWnd, &rcWin);   
    OffsetRect( &rcWin, -rcWin.left, -rcWin.top );   
    for(int i=0; i<4; i++)   
    {   
        FrameRect(hdc, &rcWin, m_hbr[i]);   
        InflateRect(&rcWin, -1, -1);           
    }   
    ReleaseDC(hWnd, hdc);   
    return 0;   
}  
  if( uMsg == WM_NCPAINT )
  {
   HDC hdc;
   hdc = GetWindowDC(hWnd);
   // Paint into this DC
   RECT rcWin;
   GetWindowRect(hWnd, &rcWin);
   OffsetRect( &rcWin, -rcWin.left, -rcWin.top );
   for(int i=0; i<4; i++)
   {
    FrameRect(hdc, &rcWin, m_hbr[i]);
    InflateRect(&rcWin, -1, -1);  
   }
   ReleaseDC(hWnd, hdc);
   return 0;
  }    其中m_hbr是HBRUSH类型的数组，含4个元素，用CreateSolidBrush函数做了初始化。处理完WM_NCPAINT消息后返回0，阻止系统调用DefWindowProc继续处理。 
引用：[http://blog.csdn.net/foruok/archive/2008/06/23/2579013.aspx](http://blog.csdn.net/foruok/archive/2008/06/23/2579013.aspx)
