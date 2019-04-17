# cximage功能简介 - DoubleLi - 博客园






CxImage是一个可以用于MFC 的C++图像处理类库类，它可以打开，保存，显示，转换各种常见格式的图像文件，比如BMP, JPEG, GIF, PNG, TIFF, MNG, ICO, PCX, TGA, WMF, WBMP, JBG, J2K 等格式的文件。可以实现BMP<->JPG，PNG <>TIFF格式等等的互相转换。

  CxImage不仅可以实现图像文件的类型转换，还可以实现在内存图像数据的类型转换，并且使用很方便。除了文件格式的相互转换，它还提供了很多很多图像后处理的功能，比如图像模糊，锐化等等，功能非常强大和实用。



**示例: 如何转换一种格式到另外一种格式 **





**[cpp]**[view plain](http://blog.csdn.net/augusdi/article/details/8680040#)[copy](http://blog.csdn.net/augusdi/article/details/8680040#)



- CxImage  image;  
- // bmp -> jpg   
- image.Load( " image.bmp " , CXIMAGE_FORMAT_BMP);  
- if (image.IsValid()){  
- if ( ! image.IsGrayScale()) image.IncreaseBpp( 24 );  
-     image.SetJpegQuality( 80 );  
-     image.Save( " image.jpg " ,CXIMAGE_FORMAT_JPG);  
- }  
- // png -> tif   
- image.Load( " image.png " , CXIMAGE_FORMAT_PNG);  
- if (image.IsValid()){  
-     image.Save( " image.tif " ,CXIMAGE_FORMAT_TIF);  
- }   






**  如何从资源中加载图像**



**[cpp]**[view plain](http://blog.csdn.net/augusdi/article/details/8680040#)[copy](http://blog.csdn.net/augusdi/article/details/8680040#)



- // Load the resource IDR_PNG1 from the PNG resource type   
- CxImage * newImage = new CxImage();  
- newImage -> LoadResource(FindResource(NULL,MAKEINTRESOURCE(IDR_PNG1),  
- " PNG " ),CXIMAGE_FORMAT_PNG);  
- 或者  
- // Load the resource IDR_JPG1 from DLL   
- CxImage * newImage = new CxImage();  
- HINSTANCE hdll = LoadLibrary( " imagelib.dll " );  
- if (hdll){  
- HRSRC hres = FindResource(hdll,MAKEINTRESOURCE(IDR_JPG1), " JPG " );  
-     newImage -> LoadResource(hres,CXIMAGE_FORMAT_JPG,hdll);  
-     FreeLibrary(hdll);  
- }  
- 或者  
- // Load a bitmap resource;   
- HBITMAP bitmap = ::LoadBitmap(AfxGetInstanceHandle(),  
-                            MAKEINTRESOURCE(IDB_BITMAP1)));  
- CxImage * newImage = new CxImage();  
- newImage -> CreateFromHBITMAP(bitmap);   



**如何解码[内存](http://product.it168.com/list/b/0205_1.shtml)中的图像 **






**[cpp]**[view plain](http://blog.csdn.net/augusdi/article/details/8680040#)[copy](http://blog.csdn.net/augusdi/article/details/8680040#)



- CxImage image((BYTE * )buffer,size,image_type);  
- 或者  
- CxMemFile memfile((BYTE * )buffer,size);  
- CxImage image( & memfile,image_type);  
- 或者  
- CxMemFile memfile((BYTE * )buffer,size);  
- CxImage * image = new CxImage();  
- image -> Decode( & memfile,type);   



** 如何对内存中的图像编码 **








**[cpp]**[view plain](http://blog.csdn.net/augusdi/article/details/8680040#)[copy](http://blog.csdn.net/augusdi/article/details/8680040#)



- long size = 0 ;  
- BYTE * buffer = 0 ;  
- image.Encode(buffer,size,image_type);  
- ...  
- image.FreeMemory(buffer);  
- 或者  
- CxMemFile memfile;  
- memfile.Open();  
- image.Encode( & memfile,image_type);  
- BYTE * buffer = memfile.GetBuffer();  
- long size = memfile.Size();  
- ...  
- image.FreeMemory(buffer);   







**  如何创建一副多页的TIFF**



**[cpp]**[view plain](http://blog.csdn.net/augusdi/article/details/8680040#)[copy](http://blog.csdn.net/augusdi/article/details/8680040#)



- CxImage * pimage[ 3 ];  
- pimage[ 0 ] =& image1;  
- pimage[ 1 ] =& image2;  
- pimage[ 2 ] =& image3;  
- FILE * hFile;  
- hFile = fopen( " multipage.tif " , " w+b " );  
- CxImageTIF multiimage;  
- multiimage.Encode(hFile,pimage, 3 );  
- fclose(hFile);  
- 或者  
- FILE * hFile;  
- hFile = fopen( " c://multi.tif " , " w+b " );  
- CxImageTIF image;  
- image.Load( " c://1.tif " ,CXIMAGE_FORMAT_TIF);  
- image.Encode(hFile, true );  
- image.Load( " c://2.bmp " ,CXIMAGE_FORMAT_BMP);  
- image.Encode(hFile, true );  
- image.Load( " c://3.png " ,CXIMAGE_FORMAT_PNG);  
- image.Encode(hFile);  
- fclose(hFile);   





**如何复制和粘贴图像**





**[cpp]**[view plain](http://blog.csdn.net/augusdi/article/details/8680040#)[copy](http://blog.csdn.net/augusdi/article/details/8680040#)



- // 复制(copy)   
- HANDLE hDIB = image -> CopyToHandle();  
- if (::OpenClipboard(AfxGetApp() -> m_pMainWnd -> GetSafeHwnd())) {  
- if (::EmptyClipboard()) {  
- if (::SetClipboardData(CF_DIB,hDIB) == NULL ) {  
-             AfxMessageBox( " Unable to set Clipboard data " );  
- }    }    }  
- CloseClipboard();  
- // 粘贴(paste)   
- HANDLE hBitmap = NULL;  
- CxImage * newima = new CxImage();  
- if (OpenClipboard()) hBitmap = GetClipboardData(CF_DIB);  
- if (hBitmap) newima -> CreateFromHANDLE(hBitmap);  
- CloseClipboard();   






**  如何在图片框控件(Picture Box)中显示图像 **



**[cpp]**[view plain](http://blog.csdn.net/augusdi/article/details/8680040#)[copy](http://blog.csdn.net/augusdi/article/details/8680040#)



- HBITMAP m_bitmap = NULL;  
- CxImage image( " myfile.png " , CXIMAGE_FORMAT_PNG);  
- ...  
- CDC * hdc = m_picture.GetDC();  
- HBITMAP m_bitmap = image.MakeBitmap(hdc -> m_hDC);  
- HBITMAP hOldBmp = m_picture.SetBitmap(m_bitmap);  
- if (hOldBmp) DeleteObject(hOldBmp);  
- if (hdc -> m_hDC) m_picture.ReleaseDC(hdc);  
- ...  
- if (m_bitmap) DeleteObject(m_bitmap);   





Cimage合并图片就是这么简单： 



**[cpp]**[view plain](http://blog.csdn.net/augusdi/article/details/8680040#)[copy](http://blog.csdn.net/augusdi/article/details/8680040#)



- CxImage t_img1,t_img2,t_img3;   
- 
- int t_h1,t_w1,t_h2,t_w2,t_h3,t_w3,t_bpp;   
- 
-       t_img1.Load("F://1.jpg");   
- 
-       t_img2.Load("F://2.jpg");   
- 
-       t_h1=t_img1.GetHeight();   
- 
-       t_w1=t_img1.GetWidth();   
- 
-       t_h2=t_img2.GetHeight();   
- 
-       t_w2=t_img2.GetWidth();   
- 
-       t_h3=t_h1;   
- 
-       t_w3=t_w1+t_w2;   
- 
-       t_bpp=t_img1.GetBpp();   
- 
-       t_img3.Create(t_w3,t_h3,t_bpp);   
- 
-       t_img3.MixFrom(t_img1,0,0);   
- 
-       t_img3.MixFrom(t_img2,t_w1,0);   
- 
-       t_img3.Save("f://3.jpg",CXIMAGE_FORMAT_JPG);   





**译后序：**

  关于图像处理库，我相信大家有一个共同的疑问：这么多图像处理库，我该如何选择？在CSDN的blog中有这样一段文字，比较透彻地回答了这个问题，感谢作者的透彻解释：

  "CxImage类库是一个优秀的图像操作类库。它可以快捷地存取、显示、转换各种图像。有的读者可能说，有那么多优秀的图形库，如OpenIL,FreeImage, PaintLib等等，它们可谓是功能强大，齐全，没必要用其它的类库。但我要说，这些类库基本上没有免费的，使用这些类库，你要被这样那样的许可协议所束缚。

  在这点上，CxImage类库是完全免费的。另外，在使用上述类库时，你会遇到重重麻烦。因为它们大部分是平台无关的，且用[C语言](http://lib.csdn.net/base/c)写成，有的还夹杂着基本的C++ wrapper和成堆德编译选项的声明需要你去处理。而CxImage类库在这方面做得很好。还有让我最看好的，就是作者完全公开了源代码。相对于那些封装好的图形库和GDI+来说，这一点使我们可以进一步学习各种编解码技术，而不再浮于各种技术的表面。"











