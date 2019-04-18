# Windows屏幕广播编程技术系列一 - ljx0305的专栏 - CSDN博客
2011年04月20日 21:16:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：1209
windows屏幕广播编程技术系列一
作者近期开发了一套屏幕广播教学软件，作者愿与广大的VC++编程爱好者共享其中的各种技术及经验心得，屏幕的抓取与显示仅是其中的第一步和一个技术点，其他技术与细节将在以后的文章中逐步讲解。
许多VC++程序员都遇到过这样的难题，利用CreateCompatibleDC、CreateCompatibleBitmap及BitBlt等API函数能够比较容易地抓取整个屏幕图像，但所抓取屏幕图像不包含当前光标区域。我们偶尔也能从书上看到类似的例子，但不能完全明白其中的具体细节，很难做到灵活应用。
本文将根据作者长期的摸索和在实际相关软件开发中积累的经验，对具体实现细节作出详尽的分析。
本文将给出VC++6.0下实现抓取整个屏幕图像及光标区域的具体步骤及程序代码。闲话少说,让我们来看看具体的实现方法吧!
第一步：通过AppWizard生成一个工程名为GrabScreen的单文档工程，注意在最后一步中选择你的View类（本例中为CGrabScreenView）的派生基类为CscrollView，以便能够滚动显示整个屏幕图像。
第二步：用以下代码修改CGrabScreenView中的OnInitialUpdate函数使视图区的滚动范围为整个屏幕大小。
void CGrabScreenView::OnInitialUpdate()
{
 CScrollView::OnInitialUpdate();
 CSize sizeTotal;
 // TODO: calculate the total size of this view
 sizeTotal.cx =GetSystemMetrics(SM_CXSCREEN);
 sizeTotal.cy =GetSystemMetrics(SM_CYSCREEN);
 SetScrollSizes(MM_TEXT, sizeTotal);
}
第三步：在主菜单条的编辑菜单下添加一子菜单项，ID为IDM_EDIT_SCREEN，标题为“抓取全屏”。通过ClassWizard在CGrabScreenView中添加该菜单项的命令响应函数，采用ClassWizard建议的函数名称OnEditScreen。
第四步：在OnEditScreen函数中实现抓取整个屏幕图像及光标区域。
首先在CgrabScreenView类定义中增加成员变量HDC m_hdcCompatible和HBITMAP m_hbmScreen，分别对应整个屏幕图像及光标区域的兼容设备描述表和兼容位图。然后用下面给出的程序代码获取所需数据。
void CGrabScreenView::OnEditScreen() 
{
 // TODO: Add your command handler code here
 //获取全屏幕窗口的设备描述表
 HDC hdcScreen=::GetDC(NULL);
 //产生全屏幕窗口设备描述表的兼容设备描述表
 m_hdcCompatible=CreateCompatibleDC(hdcScreen);
 //产生全屏幕窗口设备描述表的兼容位图
 HBITMAP m_hbmScreen=CreateCompatibleBitmap(hdcScreen,
   GetDeviceCaps(hdcScreen,HORZRES),GetDeviceCaps(hdcScreen,VERTRES));
 //将兼容位图选入兼容设备描述表
 SelectObject(m_hdcCompatible,m_hbmScreen);
 //将全屏幕窗口位图的象素数据拷贝到兼容设备描述表
 BitBlt(m_hdcCompatible,0,0,GetDeviceCaps(hdcScreen,HORZRES),
   GetDeviceCaps(hdcScreen,VERTRES),hdcScreen,0,0,SRCCOPY);
 //获取当前光标及其位置
 HCURSOR hCursor=GetCursor();
 POINT ptCursor;
 GetCursorPos(&ptCursor);
 //获取光标的图标数据
 ICONINFO IconInfo;
 if (GetIconInfo(hCursor, &IconInfo))
 {
  ptCursor.x -= ((int) IconInfo.xHotspot);
  ptCursor.y -= ((int) IconInfo.yHotspot);
  if (IconInfo.hbmMask != NULL)
   DeleteObject(IconInfo.hbmMask);
  if (IconInfo.hbmColor != NULL)
   DeleteObject(IconInfo.hbmColor);
 }
 //在兼容设备描述表上画出该光标
 DrawIconEx(
  m_hdcCompatible,         // handle to device context 
  ptCursor.x, ptCursor.y,
  hCursor,         // handle to icon to draw 
  0,0,          // width of the icon 
 0,           // index of frame in animated cursor 
  NULL,          // handle to background brush 
  DI_NORMAL | DI_COMPAT      // icon-drawing flags 
  );
 //使窗口无效，调用OnDraw重画窗口 
 Invalidate(); 
}
第五步：在窗口重画函数OnDraw中将m_hdcCompatible中的图像拷贝到CgrabScreenView的设备描述表上，从而在该窗口中抓取的屏幕及光标区域。
void CGrabScreenView::OnDraw(CDC* pDC)
{
 CGrabScreenDoc* pDoc = GetDocument();
 ASSERT_VALID(pDoc);
 // TODO: add draw code for native data here
 //在视图窗口显示全屏幕窗口图像及光标区域
 SelectObject(pDC->m_hDC,m_hbmScreen);
 BitBlt(pDC->m_hDC,0,0,GetSystemMetrics(SM_CXSCREEN),
   GetSystemMetrics(SM_CXSCREEN),m_hdcCompatible,0,0,SRCCOPY);
}
实验心得与体会：
1． 256色及更低色位图由三部分组成。一是位图信息头，定义了位图的大小等信息。二是颜色表，定义了各象素点值对应的红、绿、蓝三色分量。三是位图的象素数据块，包含位图中的每个象素点的数据，它们与位图信息头中的颜色表一起决定了象素的最终显示颜色。对于大于256色的位图，通常由两区域组成，即位图信息头和象素数据块，象素数据块中的每个象素点的数据包含了红、绿、蓝三色分量，具体细节，请参看相关的专业书籍。
2． 每个设备描述表中都包含有一个位图，CreateCompatibleDC返回的兼容设备描述表所包含的位图大小仅为一个象素，并且是单色图像。
3． CreateCompatibleBitmap返回的位图对象只包含相应设备表述表中的位图的位图信息头，不包含颜色表和象素数据块。因此，选入该位图对象的设备描述表不能象选入普通位图对象的设备描述表一样应用，必须在SelectObject函数之后，调用BitBlt将原始设备描述表的颜色表及象素数据块拷贝到兼容设备描述表。
4． 在获取光标图像的程序代码中，GetCursorPos所取回的坐标点是光标的中间点的位置，而DrawIconEx函数所要求的坐标值为光标的左上角坐标，因此，要调用GetIconInfo对坐标点进行转换。
5． 在OnDraw函数的实现代码中，我在BitBlt函数之前调用了SelectObject将屏幕的兼容位图选入CgrabScreenView窗口的设备描述表，有人告诉我，说不用SelectObject这一步，也是一样的效果。我问，如果，你要将一台计算机上的屏幕传送到网上的另外一台计算机上显示，效果还是一样吗？结果是不尽相同的。重要的一点是，BitBlt函数只拷贝颜色表及象素数据块，而不含位图信息头。只是，本例中的CgrabScreenView窗口的设备描述表中所包含的位图对象的位图信息头与m_hbmScreen中的一样，所以，有了以上疑问。
作者：[http://www.it315.org/](http://www.it315.org/)     张孝祥
