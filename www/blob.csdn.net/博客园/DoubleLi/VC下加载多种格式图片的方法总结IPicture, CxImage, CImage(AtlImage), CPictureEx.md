# VC下加载多种格式图片的方法总结IPicture, CxImage, CImage(AtlImage), CPictureEx - DoubleLi - 博客园






尽管VC有提供相应的API和类来操作bmp位图、图标和（增强）元文件，但却不支持jpg、gif和png等格式的图片，而这几种格式却是常常要用到的。这里我给大家介绍两种办法来操作这些格式的图片。

1.用API OleLoadPicture来加载JPG、GIF格式的图片（注：不支持PNG格式，另外GIF只能加载第一帧，且不支持透明）

OleLoadPicture 函数实际上创建了一个IPicture类型的COM接口对象，然后我们可以通过这个COM接口来操作图片（实际上你也可以用API OleCreatePictureIndirect来加载图片，不过相比而言OleLoadPicture函数简化了基于流的IPicture对象的创建），下面是示例代码：（注：由于只是用来示例，代码中省去了出错情况的处理）



**[cpp]**[view plain](http://blog.csdn.net/augusdi/article/details/8680070#)[copy](http://blog.csdn.net/augusdi/article/details/8680070#)



- <pre class="cpp" name="code">#include <olectl.h></pre>/*     *如下代码段实现的功能是从指定的路径中读取图片，并显示出来     */ void DisplayImage(HDC hDC, LPCTSTR szImagePath) {             HANDLE hFile=CreateFile(szImagePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL); //从指定的路径szImagePath中读取文件句柄             DWORD dwFileSize=GetFileSize(hFile, NULL); //获得图片文件的大小，用来分配全局内存             HGLOBAL hImageMemory=GlobalAlloc(GMEM_MOVEABLE, dwFileSize); //给图片分配全局内存             void *pImageMemory=GlobalLock(hImageMemory); //锁定内存             DWORD dwReadedSize; //保存实际读取的文件大小             ReadFile(hFile, pImageMemory, dwFileSize, &dwReadedSize, NULL); //读取图片到全局内存当中             GlobalUnlock(hImageMemory); //解锁内存             CloseHandle(hFile); //关闭文件句柄                  IStream *pIStream;//创建一个IStream接口指针，用来保存图片流             IPicture *pIPicture;//创建一个IPicture接口指针，表示图片对象             CreateStreamOnHGlobal(hImageMemory, false, &pIStream); //用全局内存初使化IStream接口指针             OleLoadPicture(pIStream, 0, false, IID_IPicture, (LPVOID*)&(pIPicture));//用OleLoadPicture获得IPicture接口指针                  //得到IPicture COM接口对象后，你就可以进行获得图片信息、显示图片等操作             OLE_XSIZE_HIMETRIC hmWidth;             OLE_YSIZE_HIMETRIC hmHeight;             pIPicture->get_Width(&hmWidth); //用接口方法获得图片的宽和高             pIPicture->get_Height(&hmHeight);             pIPicture->Render(hDC,0,0,100,100,0,hmHeight,hmWidth,-hmHeight,NULL); //在指定的DC上绘出图片                  GlobalFree(hImageMemory); //释放全局内存             pIStream->Release(); //释放pIStream             pIPicture->Release(); //释放pIPicture     }   



2.利用第三方的开发库来操作图片

这 里我向大家推荐一个库CxImage。 CxImage里面包含了许多的类，可以用来加载、保存、显示和变换图片，而且支持许多的图片格式，包括BMP、 JPEG、 GIF、 PNG、 TIFF、 MNG、 ICO、 PCX、 TGA、 WMF、 WBMP、 JBG、 J2K等。另外CxImage也支持Alpha通道，动画帧等许多功能，而且它还是开源免费的。CxImage的当前的版本是v6.00， 介绍和下载可以访问：[http://www.codeproject.com/KB/graphics/cximage.aspx](http://www.codeproject.com/KB/graphics/cximage.aspx)。CxImage的用法十分简单，示例如下（省去出错处理）：   



**[cpp]**[view plain](http://blog.csdn.net/augusdi/article/details/8680070#)[copy](http://blog.csdn.net/augusdi/article/details/8680070#)



- void DisplayImage(HDC hDC, CString fileName)   
- {   
-             CString fileExt; //图片的扩展名   
- int len = fileName.GetLength();   
- for(int i=len-1; i>=0; i--) //得到图片的扩展名   
-             {   
- if(fileName[ i ] == '.')   
-                     {   
-                             fileExt=fileName.Mid(i+1);   
- break;   
-                     }   
-             }   
-             fileExt.MakeLower(); //将扩展名转为小写   
- if(fileExt != _T(""))   
-             {   
- //创建CxImage对象，其中静态方法CxImage::GetTypeIdFromName用来根据扩展名获得图片格式的ID代表   
-                     CxImage image(fileName,CxImage::GetTypeIdFromName(fileExt));   
- if(image.IsValid())   
-                     {   
-                             image.Draw(hDC);   
-                             image.Destroy();   
-                     }   
-             }   
- }   




3 提供一中更简单的方法

VC MFC 提供的 API LoadBitmap / LoadImage 类 CBitmap 等都只能操作 BMP 位图，图标。对于其他常用的 JPG / JPEG / GIF / PNG 格式，它无能为力。VC 下怎样才能加载各种非 BMP 格式的图片呢？ 下面介绍一种最简单的办法。用 CImage 类的 Load 函数加载图片，之后用 Detach 取得 HBITMAP 句柄。取得图片的HBITMAP 句柄后就可以像操作 BMP 图片一样处理 JPG / JPEG / GIF / PNG 格式的图片了。具体代码如下： 



**[cpp]**[view plain](http://blog.csdn.net/augusdi/article/details/8680070#)[copy](http://blog.csdn.net/augusdi/article/details/8680070#)



- #include <atlimage.h>   
- 
- CImage img;   
- 
- HRESULT ret = img.Load(filename ); // filename 是要加载的文件名（包含路径）   
- 
- HBITMAP bitmap = img.Detach();   
- 
- //像操作 BMP 图片一样处理图片  




但这些网上的方法还是有些问题，比如gif不能动态的显示

下面说一下详细步骤吧：

 1。下载 PictureEx.h和PictureEx.cpp两个文件

把这两个文件放在工程的文件夹里面，然后在将这两个文件添加到工程里面去，这样你的工程里就多了一个类了：CPictureEx

2.将你要加载的GIF图片添加到项目文件夹里，这里我命名为："inter.gif"

3。在试图类的头文件里添加：  



**[cpp]**[view plain](http://blog.csdn.net/augusdi/article/details/8680070#)[copy](http://blog.csdn.net/augusdi/article/details/8680070#)



- #include "PictureEx.h"  



定义一个对象： 



**[cpp]**[view plain](http://blog.csdn.net/augusdi/article/details/8680070#)[copy](http://blog.csdn.net/augusdi/article/details/8680070#)



- CPictureEx m_GifPic;  



4.在视图类的OnCreate中创建 CPictureEx 对象并加载图片：



**[cpp]**[view plain](http://blog.csdn.net/augusdi/article/details/8680070#)[copy](http://blog.csdn.net/augusdi/article/details/8680070#)



- <pre class="cpp" name="code">m_GifPic.Create(NULL,WS_CHILD | WS_VISIBLE |SS_ENHMETAFILE,CRect(50,50,100,100),this,1234);  
- m_GifPic.Load(_T("inter.gif"));  
- m_GifPic.ShowWindow(SW_HIDE);//SW_SHOW  
- </pre>  




注意：这一步骤不要在OnDraw里面实现，否则会出现错误，我一开始时一直有问题就是这个原因，还有load必须在movewindow（下一步的函数）之前，否则不会显示图片，还有就是load也可以放到ondraw里面去，但是那么做的话速度明显不行了。

5。在ondraw里改变窗口位置并显示图片



**[cpp]**[view plain](http://blog.csdn.net/augusdi/article/details/8680070#)[copy](http://blog.csdn.net/augusdi/article/details/8680070#)



- m_GifPic.Create(NULL,WS_CHILD | WS_VISIBLE |SS_ENHMETAFILE,CRect(50,50,100,100),this,1234);  
- m_GifPic.Load(_T("inter.gif"));  
- m_GifPic.ShowWindow(SW_HIDE);//SW_SHOW  
- CRect rc =CRect(100,400,150,450);  
- m_GifPic.MoveWindow(&rc,true);  
- m_GifPic.Draw();  
- m_GifPic.ShowWindow(SW_SHOW);  


这里还有一个基于对话框加载GIF图片的例子，添加了图片链接和鼠标变换的功能。

下面是详细的编程过程：

1. 新建项目：在VC6中用MFC新建一个基于对话框的GifDemo应用程序，接受所有缺省选项即可；

2.在项目中插入文件：把PictureEx.h，PictureEx.cpp文件copy 到项目文件夹下，Project->Add to Project->Files中选上PictureEx.h，PictureEx.cpp， Insert;

3.加入图片控件：从对话框控件中把Picture Control（图片控件)拖入主对话框中，修改其属性：ID：IDC_GIF，TYPE：Rectangle，其余接受缺省选项。再在ClassWiard中为IDF_GIF加入CSatic控制变量m_GifPic, 注意看一下，GifDemoDlg.h中是否加上了#include "PictureEx.h"（由ClassWiard加入）。然后将CSatic m_GifPic;更改成CPictureEx m_GifPic;

4.加载动画文件：先将要加载的动画文件放到 res 资源文件夹下，再将其Import进项目中，由于MFC只支持256BMP文件的图片，因此，我们要新建一个图片类型："GIF",我在这里将我网站的宣传图片roaring.gif放进去 （希望大家多支持），并将其ID修改成：IDR_GIFROARING。

____________________________________

import(导入）gif动画的详细过程：
在resourceview窗口中，单击鼠标右键，在出现的环境菜单中选择“import...”命令，会出现“import resource”选择文件对话框，文件类型选择“所有文件（*.*）”，open as 选项为"auto",再选择动画文件所在目录，选上要载入的动画文件 roaring.gif,再单击 import,由于gif动画类型不是vc默认的文件类型，这时会出现"custom resource type"对话框，键入“"gif"”，再单击ok，然后再修改其id。
________________________________________________________________

5.在程序的适当位置添入加载代码： 这里，我们在CGifDemoDlg::OnInitDialog()函数中加入如下代码：



**[cpp]**[view plain](http://blog.csdn.net/augusdi/article/details/8680070#)[copy](http://blog.csdn.net/augusdi/article/details/8680070#)



- // TODO: Add extra initialization here  
- if (m_GifPic.Load(MAKEINTRESOURCE(IDR_GIFROARING),_T("Gif")))  
- m_GifPic.Draw();  



如果仅仅把动画载入，到这就可以了，运行一下，应该看看您的的成果了。

下面附带说说如何将这幅动画制作成超链接，以后，咱们也可以宣传自已的公司、网站或产品了。

6.利用ClassWiard加入一个LButtonDown鼠标左键消息处理函数CGifDemoDlg::OnLButtonDown(UINT nFlags, CPoint point)，  添入如下代码：



**[cpp]**[view plain](http://blog.csdn.net/augusdi/article/details/8680070#)[copy](http://blog.csdn.net/augusdi/article/details/8680070#)



- void CGifDemoDlg::OnLButtonDown(UINT nFlags, CPoint point)   
- {  
- // TODO: Add your message handler code here and/or call default  
- CRect rect;  
- m_GifPic.GetWindowRect(&rect);  
- ScreenToClient(&rect);  
- 
- if (rect.PtInRect(point))  
- ShellExecute(AfxGetMainWnd()->m_hWnd,_T("open"),  
- _T("http://roaringwind.best.163.com"),_T(""),NULL,0);  
- 
- 
- CDialog::OnLButtonDown(nFlags, point);  
- }  


我在这儿将我主页的地址放上了，运行，点击动画图片就能进入我的站点的了。当然要是能象所有的超链接一样，能将鼠标变成手形，就更好了。

7.改变鼠标形状：将一个鼠标文件放在res文件夹中，IMPORT，ID：IDC_CURSOR1，利用ClassWiard加入一个WM_SETCURSOR消息处理函数CGifDemoDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)，  添入如下代码：



**[cpp]**[view plain](http://blog.csdn.net/augusdi/article/details/8680070#)[copy](http://blog.csdn.net/augusdi/article/details/8680070#)



- BOOL CGifDemoDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)   
- {  
- // TODO: Add your message handler code here and/or call default  
- CRect rect;  
- m_GifPic.GetWindowRect(&rect);  
- ScreenToClient(&rect);  
- 
- CPoint point;  
- GetCursorPos(&point);  
- ScreenToClient(&point);  
- 
- if (rect.PtInRect(point) && m_hCursor)  
- {  
- SetCursor(m_hCursor);  
- return TRUE;  
- };  
- 
- 
- return CDialog::OnSetCursor(pWnd, nHitTest, message);  
- }  

![](http://static.blog.csdn.net/images/save_snippets.png)












