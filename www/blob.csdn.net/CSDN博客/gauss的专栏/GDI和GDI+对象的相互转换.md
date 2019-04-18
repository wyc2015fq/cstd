# GDI和GDI+对象的相互转换 - gauss的专栏 - CSDN博客
2013年01月12日 09:48:22[gauss](https://me.csdn.net/mathlmx)阅读数：271
个人分类：[图形与图像处理](https://blog.csdn.net/mathlmx/article/category/605907)
当然希望可以使用GDI+来完全代替GDI，再不济也只需要从GDI对象转到GDI+对象。但是就是有那么些工程，又要新技术新特性、又希望快速开发、又不肯丢弃旧版本的已有的功能。没办法，只能缝缝补补，咔哐碙啌...
一、CDC到Graphics只要这样：
`1.``Graphics g(pDC->GetSafeHdc());``//只要能构造CDC句柄的MFC对象都可以`
`2.``//CClientDC、CPaintDC，也都可以`
二、CBitmap到Graphics需要创建一个内存位图设备：
`1.`
```
CDC MemDC;
```
`2.``CBitmap MemBitmap;`
`3.``MemDC.CreateCompatibleDC(pDC);`
`4.``MemBitmap.CreateCompatibleBitmap(pDC,size.cx, size.cy);`
`5.``CBitmap *pOldBit=MemDC.SelectObject(&MemBitmap);`
`6.``Graphics g(MemDC.GetSafeHdc());`
`7.``///… you can work with GDI+ … ///`
或者使用：
`1.``Bitmap::Bitmap(``HBITMAP``,``HPALETTE``)。`
`2.``static``?Bitmap?*FromHBITMAP(``HBITMAP``?hbm,``HPALETTE``?hpal);`
三、想要用Bitmap来打开图片，却又需要用GDI的绘图函数来操作（一般都为历史遗留问题），就需要将Bitmap对象转换到CBitmap，可以这样：
`1.`
```
Bitmap* pPicture =
```
`new``Bitmap(L``"dotLive.jpg"``);`
`2.``HBITMAP``hBitmap;`
`3.``CBitmap* pbm;`
`4.``pPicture->GetHBITMAP(Color(0,0,0),&hBitmap);`
`5.``pbm = CBitmap::FromHandle(aaa1);`
`6.``///… use pbm to do the GDI works ...///`
`7.``pbm->DeleteObject();``//要记得删除GDI对象，否则你会发现你的内存占用…`
四、保存Bitmap，打开很方便，保存就不那么的方便了。Bitmap或者Image打开图像会自动锁定文件，保存时会产生文件被使用之类的错误。一种折衷的办法是，保存时全新创建一个Bitmap（Image）对象，将原来的对象画到新对象上，然后将原来的对象删除以释放文件。保存新对象。不能用clone，它会将锁定的文件资源一块复制过来，则解锁文件需要的释放对象就又多了一个。
`01.`
```
Bitmap* tPic=
```
`new``Bitmap(L``"dotLive.jpg"``);``//锁定文件`
`02.`
```
Bitmap* pPicture =
```
`new``Bitmap(tPic->GetWidth()`
`03.``,tPic->GetHeight()`
`04.``,tPic->GetPixelFormat());`
`05.``Graphics g(pPicture);`
`06.``g.DrawImage(tPic,0,0,m_nWidth,m_nHeight);``//复制过来`
`07.``delete``tPic;``//解锁`
`08.``/// … use the pPicture to access "dotLive.jpg" … ///`
`09.``CLSID tiffClsid;`
`10.``GetEncoderClsid(L``"image/jpeg"``, &tiffClsid);``//这个函数MSDN中有`
`11.``pPicture->Save(L``"dotLive.jpg"``,&tiffClsid);``//保存`
五、Graphics到HDC，想要用Bitmap方便打开图像，又要往图像里写文字作图等。糟糕的是由于历史遗留问题，这些操作都是使用GDI完成的。并且GDI+没有异或绘图模式（实现橡皮筋效果）、没有文字输出底色不透明、字符串需要转换到WChar。咋办呢，可以这样转：
`1.`
```
Bitmap* pPicture =
```
`new``Bitmap(L``"dotLive.jpg"``);``//打开图片`
`2.``Graphics tempG(pPicture );`
`3.``HDC``hdc = tempG.GetHDC ();``//获得GDI句柄`
`4.``CDC* tempDC = CDC::FromHandle(hdc);`
`5.``/// … hurry up, draw what you want … ///`
`6.`
```
tempG.ReleaseHDC(hdc);
```
`//要记得relese，GDI都需要释放，否则资源会被占用而不接受其他操作`
转自：http://www.vckbase.com/index.php/wv/1394
