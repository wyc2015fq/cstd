# VC++基于CXImage库实现缩略图 - DoubleLi - 博客园






一般的图像处理软件都对读入程序的图像文件建一个缩略图的列表，像ACDSee那样。笔者最近在做一个图像处理的项目，处理的原始数据就是图像文件。从项目一开始就想做一个缩略图，但一直苦于技术水平有限，且时间也不允许。最近两天闲暇之余就Google,百度了一下，仔细研究了一下缩略图的实现方法。其实用VC++实现一个缩略图的思想并不复杂。就是分两步：1.创建CListCtrl控件和CImageList图像列表，设置各自的相应属性。2.将你要显示的图像文件的缩略图做好，并制成位图文件加入到CImageList中，然后将每个图像文件设置成一个Item插入到CListCtrl中就完成了缩略的的制作。剩下的你就可以写各个消息的处理方法，以响应你对缩略图的操作。

       说起来简单，但当笔者亲自实现时却遇到了许多的麻烦，最大的麻烦就是如何在内存里制作缩略图，如何将各个图像文件的缩略图制成Bitmap位图资源（因为CImageList只能加入位图资源）。笔者在内存中操作了半天，最后还是没成功。而且最大的问题是在内存中由于调色板的原因，在内存中画好的图像是黑白的。最后我想到了我们这个项目中应用的CXImage图像处理库了。我仔细的读了一下这个库的文档，正所谓“踏破铁鞋无觅处，得来全不费工夫”，我发现可以基于这个库提供的方法，很简单的实现缩略图。原理还是这个原理，只是应用CXImage后，原来的问题一一化解，原来的操作也更简单了。下面就把实现的过程及代码贴出来，以飨读者。

**程序所达到的效果：**

[**![【引用】VC++基于CXImage库实现缩略图 - psunbird - psunbird的博客](http://img.blog.163.com/photo/goY3LgQzn-9aqp-ogJqJZg==/467529936317130098.jpg)**](http://img.blog.163.com/photo/goY3LgQzn-9aqp-ogJqJZg==/467529936317130098.jpg)

**1.创建CListCtrl控件和CImageList图像列表，设置各自的相应属性。对于我的程序我是在图中左面的视图的OnCreate中动态创建的CListCtrl控件和CImageList图像列表的。**





**[cpp]**[view plain](http://blog.csdn.net/augusdi/article/details/8680039#)[copy](http://blog.csdn.net/augusdi/article/details/8680039#)



- int CWorkspaceBar::OnCreate(LPCREATESTRUCT lpCreateStruct)   
- {  
- if (CBCGPDockingControlBar::OnCreate(lpCreateStruct) == -1)  
- return -1;  
- 
-  CRect rectDummy;  
-  rectDummy.SetRectEmpty ();  
- 
- // Create tree windows.  
- //动态生产列表控件和图像列表  
-  m_ListCtl.Create(LVS_ICON|WS_CHILD | WS_VISIBLE|LVS_TYPEMASK,  
-                                  CRect(0,0,150,500),this,100);  
-  m_MyImageList.Create(64,64,ILC_COLOR24, 0, 1);  
- 
- return 0;  
- }  






**2.设计一个左面视图的方法，使我们可以在文档类中调用这个方法为左面视图添加Items,这其中要完成的任务还有在内存中做好每个图像文件的Bitmap(位图资源)。**



代码如下：



**[cpp]**[view plain](http://blog.csdn.net/augusdi/article/details/8680039#)[copy](http://blog.csdn.net/augusdi/article/details/8680039#)



- void CWorkspaceBar::InitialListCtrl(int imageCount,CString *pImageNameList)  
- //根据读入的文档数据初始化列表控件  
- 
- //传入参数imageCount是图像文件的总数，pImageNameList是各个图像文件按顺序的名字列表  
- {  
- //保存文档的数据  
-  m_imageCount=imageCount;  
-  m_pImageNameList=pImageNameList;  
- 
- //应用CXImage在内存中动态生产指定大小的位图  
-  CxImage image;        //声明CXImage对象  
- for (int i=0;i<imageCount;i++)  
-  {  
- //应用CXImage载入图像，本程序是相对路径  
-   image.Load(pImageNameList[i],CXIMAGE_FORMAT_BMP);  
- 
- //应用CXImage将图像变成我们指定的大小即96*96的，前面声明CImageList时就是96*96的  
-   image.Resample(96,96,2);  
- //应用CXImage在内存中生产位图  
-   CDC *pDC = GetDC();  
- HBITMAP hBit=image.MakeBitmap(pDC->GetSafeHdc());  
-   CBitmap bmp;  
-   bmp.Attach(hBit);  
- //将位图加到图像列表中  
-   m_MyImageList.Add(&bmp,RGB(255, 255, 255));  
-   bmp.Detach();  
-  }  
- //将图像列表与列表控件关联起来  
-  m_ListCtl.SetImageList(&m_MyImageList, LVSIL_NORMAL);  
- //设置列表控件各项  
-  LVITEM item;  
-   item.mask=LVIF_TEXT|LVIF_IMAGE;  
-  item.iItem=0;   // 第1项  
-   item.iSubItem=0;        // 主项  
-   item.pszText="";  
-   item.iImage=0;  // 设置关联图像列表中指定图像的索引值  
-   m_ListCtl.InsertItem(&item);  // 添加第1项  
-  m_ListCtl.SetItemText(0,0,pImageNameList[0]);  
- 
- for (i=1;i<imageCount;i++)  
-  {  
-   item.iItem=i;           // 准备添加项  
-    item.iImage=i;  
-    m_ListCtl.InsertItem(&item);  
-   m_ListCtl.SetItemText(i,0,pImageNameList[i]);  
-  }  
- DWORD  dwOldStyle,dwNewStyle;   
-  dwNewStyle = LVS_ICON;  
- HWND  hWndLt=m_ListCtl.GetSafeHwnd();  // 获得列表视图控件的窗口句柄  
-  dwOldStyle=GetWindowLong(hWndLt, GWL_STYLE);  // 获得列表视图控件的窗口风格  
- if((dwOldStyle & LVS_TYPEMASK)!=LVS_ICON)  // 判断当前的显示方式是不是所指定的方式  
-  {  
-   dwOldStyle &=~LVS_TYPEMASK;   // 从旧的窗口风格中屏蔽不必要的信息  
-   dwNewStyle |=dwOldStyle;   // 保持窗口其它风格不变，仅改变显示方式  
-   SetWindowLong(hWndLt, GWL_STYLE, dwNewStyle);  // 改变列表控件窗口风格  
-  }  
- }  




剩下的工作就是写各个消息的处理方法了，包括点击相应缩略图在对应的视图里显示各个图像等等。

**       总结：本程序是基于CXImage影像处理库做的。CXImage是一个很好的图像处理库，在网上可以很容易下到，而且是开源的。笔者认为CXImage要优于GDI、GDI+。本程序在VC++6.0中调试通过，运行良好。**









