# OLE控件在Direct3D中的渲染方法 - 逍遥剑客 - CSDN博客
2008年03月15日 21:05:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：4600标签：[direct3d																[windows																[浏览器																[null																[图形																[游戏](https://so.csdn.net/so/search/s.do?q=游戏&t=blog)](https://so.csdn.net/so/search/s.do?q=图形&t=blog)](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=浏览器&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)](https://so.csdn.net/so/search/s.do?q=direct3d&t=blog)
个人分类：[图形技术](https://blog.csdn.net/xoyojank/article/category/337624)
Windows上的图形绘制是基于GDI的, 而Direct3D并不是, 所以, 要在3D窗口中显示一些Windows中的控件会有很多问题
那么, 有什么办法让GDI绘制的内容在3D中显示出来?反正都是图像, 总有办法实现的嘛!
前段时间在研究浏览器在游戏中的嵌入, 基本的思路就是在后台打开一个浏览窗口, 然后把它显示的内容拷贝到一张纹理上, 再把纹理在D3D中绘制出来, 至于事件处理就要另做文章了.
所以, 其它的Windows里的GDI绘制的东西都可以这样来实现!
最初我是GetDC, 然后GetPixel逐像素拷贝, 慢得我想死.....
后来发现了BitBlt这一速度很快的复制方法, 才有了实用价值:
1. 取得控件的DC: GetDC(hWnd)
2. 取得Texture的DC: IDirect3DSurface9::GetDC
3. 用BitBlt拷贝过去
BOOL BitBlt(
  HDC hdcDest, // handle to destination DC
int nXDest,  // x-coord of destination upper-left corner
int nYDest,  // y-coord of destination upper-left corner
int nWidth,  // width of destination rectangle
int nHeight, // height of destination rectangle
  HDC hdcSrc,  // handle to source DC
int nXSrc,   // x-coordinate of source upper-left corner
int nYSrc,   // y-coordinate of source upper-left corner
  DWORD dwRop  // raster operation code
);
如果是OLE控件那就更简单啦:
WINOLEAPI OleDraw( 
  IUnknown * pUnk,    //Pointer to the view object to be drawn
  DWORD dwAspect,     //How the object is to be represented
  HDC hdcDraw,        //Device context on which to draw
  LPCRECT lprcBounds  //Pointer to the rectangle in which the object 
// is drawn
);
比如我有一个IWebBrowser2的指针, 想把它显示的内容拷贝到纹理上, 可以这么干:
    IDirect3DSurface9* pSurface = NULL;
this->mTexture->GetSurfaceLevel(0, &pSurface);
if (NULL != pSurface)
    {
        HDC hdcTexture;
        HRESULT hr = pSurface->GetDC(&hdcTexture);
if(FAILED(hr)) return;
        ::SetMapMode(hdcTexture, MM_TEXT);
        ::OleDraw(pBrowser, DVASPECT_CONTENT, hdcTexture, &rect);
        pSurface->ReleaseDC(hdcTexture);
        pSurface->Release();
    }
Show一下:
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/DXWebBrowser.JPG)
不光是浏览器啦, 任何OLE控件都可以, 可以发挥你的想像力:
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/DxWebFlashPlayer.JPG)
