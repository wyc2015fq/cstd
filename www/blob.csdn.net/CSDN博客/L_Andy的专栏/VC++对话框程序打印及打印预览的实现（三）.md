# VC++对话框程序打印及打印预览的实现（三） - L_Andy的专栏 - CSDN博客

2012年10月22日 17:38:22[卡哥](https://me.csdn.net/L_Andy)阅读数：2980



　　本文首先介绍了利用MFC提供的文档视图框架来实现一个打印程序，实现打印预览，在此基础上，同时通过对MFC源代码的深入探讨，提出了利用该方法在对话框上用MFC实现打印功能，结果表明，利用MFC实现打印不仅方便，而且功能很强大，能够根据不同的需求很方便的打印出所需要的格式。本文还实现了一个在对话框中利用MFC实现打印功能的一个框架结构，对于使用者只要使用该结构就可以按照自己的要求打印任何内容。

　　关键词：Visual C++ ，MFC，对话框， 打印 ，打印预览

　　引言

　　打印程序的编写在windows程序设计中非常有用，针对不同的用户需要，通常用sdk方式实现打印代码量比较大，而且要对打印流程的底层有非常清楚的了解，需要一个程序员有非常深入的打印方面的知识，利用MFC提供的文档视图结构，不但可以实现一些常用的标准界面元素，把数据的处理的界面的处理分离出来，而且其提供的打印功能更是方便快捷，功能强大。打印程序的编写本质是是一种GDI绘图，只是绘图的对象是在打印机的设备描述表，如果对于屏幕的GDI绘图比较熟悉的读者，相信掌握打印程序的编写应该比较容易。

　　1、文档视图结构的程序的打印程序的编写

　　通常情况下，一个结构组织的比较好的MFC程序是基于文档视图结构的，这一框架结构给我们提供了很多功能，比如菜单，注册表的关联，文件类型的注册，打印功能，只要我们善于发掘，这些都可以为我们所用，但我们现在只关心如何使用MFC提供的结构来实现打印功能。

　　在编写打印程序之前，有必要先介绍一下MFC的框架结构，其中的文档视图结构又是这个框架的重点，我们通过分析MFC实现的视图类的原代码就可以看到一个打印程序的执行流程。读者也可以看侯俊杰的《深入浅出MFC》,上面有关于MFC打印的详细流程解释,下面是MFC的打印的函数的实现，该函数名为OnFilePrint它不是一个虚函数，而是响应缺省的COMMAND消息的处理函数，因为MFC提供了向导生成的菜单和工具栏，关于打印的命令ID为ID_FILE_PRINT ，而在视图类的MessageMap里有这样一项，ON_COMMAND
 (ID_FILE_PRINT, CView::OnFilePrint)，因此实际使用的过程中可以不用原来的ID, 而使用自己的ID如ID_MYPRINT,再在MessageMap里加入ON_COMMAND (ID_MYPRINT, CView::OnFilePrint)即可完成原来一样的功能。ViewPrnt.cpp中有CView的OnFilePrint的函数的具体实现，ViewPrnt.cpp的位置读者自己用windows查找就能找到，这是MFC的源代码，本文把其中的主要代码列出放在下面，直接看下面的分析：

void CView::OnFilePrint()

{

　// get default print info

　if (OnPreparePrinting(&printInfo))

　{

　　if (dlg.DoModal() != IDOK)

　　　return;

　}

　OnBeginPrinting(&dcPrint, &printInfo);

　OnPrepareDC(&dcPrint, &printInfo);

　OnPrint(&dcPrint, &printInfo);

　OnEndPrinting(&dcPrint, &printInfo); // clean up after printing

} 

　　其中加粗的代码行为可以重载的虚函数，根据不同的用户，其内容会不同。对于 OnPreparePrinting() 函数的具体内容必须有 return DoPreparePrinting(pInfo);这是在一个打印过程中最先调用的。当然也可以包含一些其它的打印初始化操作。我们最主要的是要重载三个函数：

OnBeginPrinting();

OnPrint();

OnEndPrinting(); 

　　而以 OnPrint 最为复杂，它是我们要写大量代码实现我们打印功能的地方。对于默认的OnPrint实现是调用CView的OnDraw，也就是和绘制视图类的客户区的内容完全相同的方法来在打印机上绘图。实际中我们在两种地方绘图的内容是完全不同的，可能用户在客户区绘的是一个曲线，而在打印机上要绘制表格和数据。OnPrint(CDC* pDC, CPrintInfo* pInfo)的第二个参数是一个CPrintInfo类型的指针，我们可以从这个指针指向的对象中获得很多信息，如总共的页数，当前的页数，这在打印页眉页脚时可能是很有用的信息。CPrintInfo的定义如下：

struct structCPrintInfo // Printing information structure

{

　CPrintInfo();

　~CPrintInfo();

　CPrintDialog* m_pPD; // pointer to print dialog

　BOOL m_bDocObject; // TRUE if printing by IPrint interface

　BOOL m_bPreview; // TRUE if in preview mode

　BOOL m_bDirect; // TRUE if bypassing Print Dialog

　BOOL m_bContinuePrinting;// set to FALSE to prematurely end printing

　UINT m_nCurPage; // Current page

　UINT m_nNumPreviewPages; // Desired number of preview pages

　CString m_strPageDesc; // Format string for page number display

　LPVOID m_lpUserData; // pointer to user created struct

　CRect m_rectDraw; // rectangle defining current usable page area

　// these only valid if m_bDocObject

　UINT m_nOffsetPage; // offset of first page in combined IPrint job

　DWORD m_dwFlags; // flags passed to IPrint::Print

　void SetMinPage(UINT nMinPage);

　void SetMaxPage(UINT nMaxPage);

　UINT GetMinPage() const;

　UINT GetMaxPage() const;

　UINT GetFromPage() const;

　UINT GetToPage() const;

　UINT GetOffsetPage() const;

};

　　OnBeginPrinting()通常用来设定要打印的总页数，以及一些和页面尺寸有关的初始化工作，在OnBeginPrinting()中设定打印的页数是必要的，默认的页数是只有一页，如果开发人员打印的页数大于1，则必须在此函数中设定打印的页数。然后在OnPrint(CDC* pDC, CPrintInfo* pInfo)中用pInfo-> m_nCurPage获取当前的页码，根据当前的页码打印该页相应的内容。OnEndPrinting用来释放在OnBeginPrinting中申请的资源，如果没有申请，则不需重载该函数。

　　关于打印预览只需要将自己的执行打印预览功能的命令ID和CView::OnFilePrintPreview关联起来就行了，具体方法是在用户的视图类的MessageMap中加入：ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview);

　　其中ID_FILE_PRINT_PREVIEW是默认的ID，开发人员也可以使用自己的ID。其实只要重载了OnPrint函数，在打印和打印预览中就可以重用该函数了。到现在为止，相信读者已经对利用MFC的文档视图结构来实现一个包含打印和打印预览功能的程序有了一个总体的认识了，本文还针对该方法给出了一个示例代码，代码来自Jeff Prosise 的《MFC windows程序设计》，见参考文献[1]。

　　2、没有文档视图结构的程序中利用MFC进行打印程序的编写

　　如果程序不是文档视图结构的，我们要使用MFC来进行打印，则可以通过建立一个虚拟的文档视图结构来进行打印，其实MFC的打印的强大功能是在CView里提供的，而CView类的对象是一个子窗口，它必须是某一个框架窗口的子窗口，而在对话框程序中，我们只需要打印，而不需要显示这个框架窗口和视图。我们以按下按钮"打印"来执行打印程序，例如按钮为ID为IDC_PNT，消息相应函数为OnPnt()，即：

ON_BN_CLICKED(IDC_PNT, OnPnt)；

　　需要在OnPnt中建立一个框架窗口，同时使某个CView类的对象为该窗口的子窗口。因此笔者建立了两个类，一个为框架窗口类CPrintFrame，另一个为CPrintView，具体的内容见示例代码。在新建一个用于打印的虚拟框架窗口时，需要将执行打印的对话框的指针传给框架窗口，这以便在对话框来响应WM_BEGIN_PRINTING和WM_END_PRINTING消息，使对话框可以完成打印的初始化和释放操作。在执行一个打印任务时，将打印的流程交给CView来进行，而这个CView是虚拟的，只是用来完成打印一些操作，其它内容则完全不负责处理，而当要执行CView::OnPrint时，则又将处理的具体内容传回到对话框，而对话框则只需要响应WM_MY_PRINT消息即可：

pFrame->m_pCallerDlg->SendMessage(WM_MY_PRINT,(WPARAM) pDC, (LPARAM) pInfo);

　　使打印的具体处理又传回到对话框中，使开发人员根据具体的需要写WM_MY_PRINT的处理函数就可以实现打印，而CView::OnPrint(CDC* pDC, CPrintInfo* pInfo)的参数也从WM_MY_PRINT的消息参数传出来，在用户的对话框程序中,需要写的代码就很少，主要有以下几个步骤，

　　建立一个CPrintFrame的对象,设该对象的指针为pFrame，并将对话框的指针传给该对象的m_pCallerDlg，即pFrame->m_pCallerDlg = this；

　　调用对象的Create函数创建框架窗口；例如pFrame->Create(NULL,"频谱打印",WS_OVERLAPPEDWINDOW,CRect(0,0,0,0));

　　如果要执行打印，则调用pFrame->m_pView->OnMyPrint(); 

　　如果要执行打印预览，则调用：

pFrame->m_pView->OnMyPrintPreview(); 

　　例如：

void CDlgPrintDlg::OnPrint() //执行打印功能{

　CPrintFrame *pFrame = new CPrintFrame;

　pFrame->m_pCallerDlg = this;

　pFrame->Create(NULL,"Curve Print",WS_OVERLAPPEDWINDOW,CRect(0,0,0,0));

　pFrame->m_pView->OnMyPrint();

}

void CDlgPrintDlg::OnPrintPreview() //执行打印预览功能{

　CPrintFrame *pFrame = new CPrintFrame;

　pFrame->m_pCallerDlg = this;

　pFrame->Create(NULL,"Curve Print 

　Preview",WS_OVERLAPPEDWINDOW,CRect(0,0,0,0));

　pFrame->m_pView->OnMyPrintPreview(); 

} 

　　在对话框中响应 WM_BEGIN_PRINTING， WM_END_PRINTING，WM_MY_PRINT消息，分别完成打印的初始化，释放和具体的打印操作；如在示例程序中添加了三个消息响应函数来执行该功能。

ON_MESSAGE(WM_BEGIN_PRINTING,OnBeginPrinting)

ON_MESSAGE(WM_END_PRINTING,OnEndPrinting)

ON_MESSAGE(WM_MY_PRINT,OnMyPrint)

　　其中OnMyPrint是跟具体要打印什么内容有关的开发人员要重点完成的代码，可以打印表格，图片，数据，只要GDI绘图可以进行的操作在这里都可以完成。由于打印预览的一部分工作在CView类里完成，因此在用户程序中只需要相应WM_MY_PRINT消息就可以执行打印预览的功能，而不需要另外编写打印预览代码。

　　本文提供的CPrintFrame和CPrintView类是连个可重用的类，开发者只需要把这两个类对应的四个文件拷贝到工程文件所在目录中(PrintFrame.h, PringtView.h,PrintFrame.cpp, PrintView.cpp)，并将这四个文件加入工程，并在需要执行打印功能的代码处加入 #include "PrintFrame.h"

#include "PrintView.h"

　　然后按照上述5个步骤进行即可以实现一个功能完整的打印程序，利用上述类实现对话框打印不但节省开发者许多时间，而且功能很强大，能达到很专业的水平，但是该方法有一个缺点，笔者发现如果开发者使用静态连接的MFC库时则会出错，只适用于Use MFC in a Shelled DLL情况，而且必须使程序为Debug版本的。

　　3、示例代码的执行效果

图1 执行打印功能的对话框

　　当按下打印预览后则会产生一个框架窗口，显示打印预览的内容，如图2所示：

图2 打印预览效果图

　　可以在上图的界面上按两页同时对两页预览，如图3所示：

图3 两页同时预览效果图

　　但有一点需要注意，在预览界面上的打印按钮不可用，如果按该"打印"钮则直接等于将预览窗口关掉，因此要执行打印功能必须另外在对话框的界面上放一个打印按钮，如果执行了"打印"功能，则会弹出一个选择打印机的对话框，如图4所示。这个对话框是MFC的打印结构内置的，不可以消除，当用户选择了正确的打印机后则可以打印出具体的内容了。

图4 打印机选择对话框

　　4、 结束语

　　本文从分析MFC的原代码入手，利用MFC的CView类提供的打印和打印预览功能进行了在对话框中的打印和打印预览。利用面向对象的C++写了两个可重用类CPrintFrame和CPrintView，实现在对话框中的打印和打印预览功能，极大的简化了对话框打印程序的编写。

