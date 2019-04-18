# MFC CImageList 详解 - L_Andy的专栏 - CSDN博客

2013年10月10日 09:36:47[卡哥](https://me.csdn.net/L_Andy)阅读数：1471


                
图像列表控件（CImageList）是相同大小图像的一个集合，每个集合中均以0为图像的索引序号基数，（可以把这些图标看成是以数组方式存储的），图像列表通常由大图标或位图构成，其中包含透明位图模式（实际上，所有的再一个图像列表中的图标都被存储在一个屏幕设备的位图中）。可以利用windows API来绘制、建立和删除图像，并能实现增加、删除、替换和拖动图像邓操作。图像列表控件提供了控制图像列表额基本方法，这些方法在Windows 95及以后版本才能实现。该控件是不可见的，通常与其他如CListBox、CComboBox、CComboBoxEx以及CTabCtrl一起使用，为他们提供图标资源。

    一、图像控件的对象结构

  1 图像控件的数据成员

    m_hImageList连接图像对象的控件句柄

  2 图像控件的建立方法

    CImageList& ImageList建立图像控件对象结构，Create初始化图像列表并绑定对象，图像控件的建立方法如下

    BOOL Create（int cx，int cy，UINT nFlags，int nInitial，int nGrow）；

    BOOL Create（UINT nBitmapID，int cx，int nGrow，COLORREF crMask）；

    BOOL Create（LPCTSTR lpszBitmapID，int cx，int nGrow，COLORREF crMask）；

    BOOL Create（CImageList& imageList1，int nImage1，CImageList& imagelist2，int nImage2，int dx，int dy）；

    其中各项参数的含义为：cx定义图像的宽度，单位为像素；cy定义图像的高度，单位为像素；nFlags确定建立图像列表的类型，可以是以下值的组合：ILC_COLOR、ILC_COLOR4、ILC_COLOR8、ILC_COLOR16、ILC_COLOR24、ILC_COLOR32、 ILC_COLORDDB和ILS_MASK；nInitial用来确定图像列表包含的图像数量；nGrow用来确定图像列表可控制的图像数量。

    nBitmapID用来确定图像列表联系的位图标志值；crMask表示颜色屏蔽位；lpszBitmapID用来确定包含位图资源额标识串；imagellist1指向图像列表控件对象的一个指针；nImage1图像列表1中包含的图像数量；imagelist2指向图像列表控件对象的一个指针。

    添加图像的两种方法

    通常可以向CImageList对象中添加位图和图标资源，其资源可以是程序中资源文件中的资源，也可以直接从外部文件调入。

    （1）从程序中资源文件中添加图标和位图图像如下所示

// 创建一个CImageList类的指针变量

CImageList* pImageList;

pImageList = new CImageList();

pImageList->Create(32, 32, ILC_COLOR32, 0, 4);

// 添加图标，图标可以随图像列表大小自动拉伸

pImageList->Add(AfxGetApp()->LoadIcon(IDI_ICON1));

pImageList->Add(AfxGetApp()->LoadIcon(IDI_ICON2));

// 添加位图，并使黑色为透明色,如果图像列表大小和位图大小不一致可能会导致天添加失败

CBitmap bmp;

bmp.LoadBitmap(IDB_BITMAP1)

pImageList->Add(&bmp, RGB(0, 0, 0));

bmp.DeleteObject();

bmp.LoadBitmap(IDB_BITMAP2)

pImageList->Add(&bmp, RGB(0, 0, 0));

    （2）从外部文件中添加图标和位图图像

HBITMAP hBitmap;

CBitmap *pBitmap;

CString FilePathName = "e:\TIME.bmp";

pBitmap = new   CBitmap;

// 从文件导入位图

hBitmap = (HBITMAP)LoadImage(AfxGetInstanceHandle(),FilePathName,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);

pBitmap->Attach(hBitmap);

m_pImageList->Add(pBitmap, RGB(0,0,0));

    这样一个图像列表控件就创建好了，并且已经添加了图像，接下来就可以在其它控件中使用其中的图像。通常需要调用SetImageList(m_pImageList)将当前图像列表设置为引用图像源。

  3 图像控件的属性类

  图像控件的属性类包括返回m_hImageList控制句柄GetSafeHandle，取得图像列表中的图像数量GetImageCount，设置图像列表的背景颜色SetBkColor，取得图像列表的背景颜色GetBkColor。

  4 图像控件的操作方法

  图像控件的操作方法包括将一个图像列表绑定到一个对象上Attach、将对象上的图像列表解除绑定并返回句柄Detach、删除一个图像列表DeleteImageList、将一个图像增加到图像列表中Add和将一个图像从图像列表中删除Remove等。

另外一种加载BMP位图的方法是：

CImageList* m_pImageList = new CImageList();

HBITMAP hBitmap;

   CBitmap* pBitmap;

   CString strFileName;

   strFileName.Format("%s\\res\\root.bmp", szPath);

   hBitmap=(HBITMAP)::LoadImage(::AfxGetInstanceHandle(),       (LPCTSTR)strFileName,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);

   pBitmap = new CBitmap;

   pBitmap->Attach(hBitmap);

   m_pImageList->Add(pBitmap,RGB(0,0,0));

   delete pBitmap;

二、图像控件的应用技巧

  对于图像控件，同样不能单独使用，必须与列表控件、树控件和标签控件相互结合使用，下面分别介绍其具体应用技巧。

  1 图像控件在列表控件中的应用技巧

    1.1设置图像控件CListCtrl::SetImageList的调用格式如下：

      CImageList* SetImageList(CImageList* pImageList，int nImageList);

      其返回值是指向前一个图像列表控件的一个指针，如果不存在前一个图像列表则为NULL；其中参数pImageList是指向图像列表的标识，nImageList是图像列表的类型，可以是如下值：LVSIL_NORMAL用大图标方式进行图像列表；LVSIL_SAMLL用小图标方式进行图像列表；LVSIL_STATE以图像状态进行图像列表。

    1.2取得图像控件CListCtrl::GetImageList的调用格式如下：

      CImageList* GetImageList（int nImageList）const；

      其返回值为指向图像列表控件的指针，其中nImageList用来确定取得返回值的图像列表的值，其取值与设置图像列表函数相同。

CImageList Cil1,Cil2; //定义大小图标像列表

CVCLISTApp *pApp=(CVCLISTApp *)AfxGetApp();//取得列表控制程序

Cil1.Create(32,32,TRUE,2,2);//建立32位图像控制

Cil1.Add(pApp->LoadIcon(IDI_GJ));//增加选中状态图像

Cil1.Add(pApp->LoadIcon(IDI_XS));//增加非选中状态图像

Cil2.Create(16,16,TRUE,2,2); //建立16位图像控制

Cil2.Add(pApp->LoadIcon(IDI_GJ));//增加选中状态图像

Cil2.Add(pApp->LoadIcon(IDI_XS));//增加非选中状态图像

m_ListCtrl.SetImageList(&Cil1,LVSIL_NORMAL);//设置大图标控制

m_ListCtrl.SetImageList(&Cil2,LVSIL_SMALL);//设置小图标控制

  2 图像控件在树控件中的应用技巧

    2.1设置图像控件CTreeCtrl::SetImageList的调用格式如下：

      CImageList* SetImageList（CImageList* pImageList，int nImageListType）；

      其返回值为指向前一个图像列表的指针，否则为NULL；参数pImageList为指向图像列表的标识，如果pImageList为NULL则所有的图像都将从树控件中被清除；nImageListType为图像列表设置的类型，可以是如下值之一：TVSIL_NORMAL设置正常图像列表，其中包括选中和非选中两种图标；TVSIL_STATE设置图像列表状态，指用户自定义状态。

    2.2取得图像控件CTreeCtrl::GetImageList的调用格式如下：

      CImageList* GetImageList（UINT nImage）；

      如果调用成功则返回图像列表控件指针，否则为NULL；nImage为取得返回值的图像列表类型，其取值和取得图像列表控件完全相同。

CImageList Cil1,Cil2;//定义大小图标像列表

CVCTREEApp *pApp=(CVCTREEApp *)AfxGetApp();//获取应用程序指针

Cil1.Create(16,16,ILC_COLOR,2,2);//建立图像控制

Cil1.Add(pApp->LoadIcon(IDI_PM));//增加选中状态图像

Cil1.Add(pApp->LoadIcon(IDI_CJ));//增加非选中状态图像

m_TreeCtrl.SetImageList(&Cil1,TVSIL_NORMAL);//设置图像控制列表

然后在树控制的结构定义中进行如下设置：

TCItem.item.iImage=0; //设置未选中图像索引号

TCItem.item.iSelectedImage=1;//设置选中时图像引号

MSDN中

  由于图像列表不是窗口，它们本身并不是控件；但它们和几种不同类型的控件一起是以，包括列表控件（CListCtrl）、树控件（CTreeCtrl）和选项卡控件（CTabCtrl）。图像列表中的所有图像包含在屏幕设备格式的单个宽位图中。图像列表也可能包括单色位图，该位图包含用于透明地绘制图像（图标样式）的屏蔽。CImageList提供的成员函数使您得以绘制图像，创建和销毁图像列表，添加和移除图像，替换图像，合并图像，以及拖动图像。

  图像列表的类型

  有两种类型的图像列表（CImageList）：非屏蔽的和屏蔽的。非屏蔽的图像列表由包含一个或多个图像的色位图构成。屏蔽的图像列表由相同大小的两个位图构成。第一个是包含图像的色位图，第二个是包含一系列屏蔽的单色位图（一个屏蔽对应第一个位图中的一个图像）。

  Create成员函数的一个重载曹永一个标志指示图像列表是否是屏蔽的。（其他重载创建屏蔽的图像列表）。

  绘制非屏蔽图像时，只是简单地将其复制到目标DC。绘制屏蔽图像时，图像位和屏蔽位组合，一般在位图中产生显露目标DC背景色的透明区域。绘制屏蔽图像时可以指定几种绘图样式。例如，可以指定图像进行抖色处理以指示选定的对象。

  使用图像列表

  构造CImageList对象，调用其Create函数的一个重载来创建图像列表并将图像列表附加到CImageList对象；

  如果在创建图像列表时没有添加图像，则调用Add或Read成员函数向图像列表添加图像；

  通过调用控件的适当成员函数，将图像列表与此控件相关联，或者您自己使用图像列表的Draw成员函数绘制图像列表中的图像；

  或许允许用户是以图像列表的内置拖动支持拖动图像。

  注意：如果图像列表是用new运算符创建的，则在操作完CImageList对象后必须销毁它。

  操作图像列表

  Replace成员函数用新的图像替换图像列表（CImageList）中的图像。如果需要动态增加图像列表中的图像数，该函数也有用。 SetImageCount函数动态更改存储在图像列表中的图像数。如果增加图像列表的大小，请调用Replace向新图像增加图像。如果减少图像列表的大小，则释放超过新大小的图像。

  Remove成员函数移除图像列表中的图像。Copy成员函数可以在图像列表内复制或交换图像。该函数使您得以指示是否应将源图像复制到目标索引，还是应交换源图像和目标图像。

  若要通过合并两个图像列表来创建新的图像列表，请使用Create成员函数的适当重载。该Create重载合并现有的图像列表的第一个图像，并将所得的图像存储在新的图像列表对象中。通过在第一个图像上透明地绘制第二个图像来创建新图像。新图像的屏蔽是对两个现有图像的屏蔽位执行逻辑OR元算的结果。

  该操作一直重复到所有图像都合并并添加到新的图像列表中。

  可以调用Write成员函数将图像信息写入存档，并通过调用Read成员函数读回。

  GetSafeHandle、Attach和Detach成员函数使您得以操作附加到CImageList对象的图像列表句柄，而DeleteImageList成员函数删除图像列表但不销毁CImageList对象。

  绘制图像列表中的图像

  若要绘制图像，请使用CImageList::Draw成员函数。您将需要指定指向DC的指针，要绘制的图像的索引，DC中绘制图像的位置，以及一组指示绘图样式的标志。

  指定ILD_TRANSPARENT样式后，Draw是以一个两步进程来绘制屏蔽的图像。


