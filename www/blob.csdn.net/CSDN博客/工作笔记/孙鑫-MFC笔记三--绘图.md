# 孙鑫-MFC笔记三--绘图 - 工作笔记 - CSDN博客





2012年05月02日 22:35:16[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：5753








一、 消息映射机制　　

　　在左边View类处点右键，在出现的菜单里点击“Add Windows Message Handler”，在出现的对话框里选择“WM_LBUTTONDOWN”，添加消息映射函数。


　　回到原文件，我们将看到三处进行了修改：


　　1， 在头文件(View.h)中声明消息响应函数原型。

　　//{{AFX_MSG(CDrawView) //注释宏

　　afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

　　//}}AFX_MSG //注释宏

　　afx_msg宏表示声明的是一个消息响应函数。


　　2， 在源文件（View.cpp)中进行消息映射。


　　BEGIN_MESSAGE_MAP(CDrawView, CView)

　　 //{{AFX_MSG_MAP(CDrawView)

　　 ON_WM_LBUTTONDOWN()

　　 //}}AFX_MSG_MAP


　　在宏BEGIN_MESSAGE_MAP()与END_MESSAGE_MAP()之间进行消息映射。

　　宏ON_WM_LBUTTONDOWN()把消息WM_LBUTTONDOWN与它的响应函数OnLButtonDown（）相关联。这样一旦有消息的产生，就会自动调用相关联的消息响应函数去处理。


　　宏ON_WM_LBUTTONDOWN()定义如下：

　　 #define ON_WM_LBUTTONDOWN() 

　　{ WM_LBUTTONDOWN, 0, 0, 0,

　　AfxSig_vwp, (AFX_PMSG)(AFX_PMSGW)(void

　　(AFX_MSG_CALL CWnd::*)(UINT, CPoint))&OnLButtonDown },


　　3， 源文件中进行消息响应函数处理。

　　void CDrawView::OnLButtonDown(UINT nFlags, CPoint point)

　　{

　　 // TODO: Add your message handler code here and/or call default

　　 CView::OnLButtonDown(nFlags, point);

　　}

　　说明：


　　可见当增加一个消息响应处理，在以上三处进行了修改。可在消息响应函数里添加消息处理代码完成对消息的响应、处理。




　　消息响应的可能方式：


　　1）在基类中针对每种消息做一个虚函数，当子类对消息响应时候，只要在子类中重写这个虚函数即可。缺点：MFC类派生层次很多，如果在基类对每个消息进行虚函数处理，那么从基类派生的每个子类都将背负一个庞大的虚表，这样浪费内存，故MFC没有采取这中方式而采取消息映射方式。


　　 2）消息映射方式：MFC在后台维护了一个句柄和C++对象指针对照表，当收到一个消息后，通过消息结构里资源句柄（查对照表）就可找到与它对应的一个 C++对象指针，然后把这个指针传给基类，基类利用这个指针调用WindowProc()函数对消息进行处理，WindowProc()函数中调用 OnWndMsg()函数，真正的消息路由及处理是由OnWndMsg()函数完成的。由于WindowProc()和OnWndMsg()都是虚函数，而且是用派生类对象指针调用的，由多态性知最终调用子类的。在OnWndMsg()函数处理的时候，根据消息种类去查找消息映射，判断所发的消息有没有响应函数，具体方式是到相关的头文件和源文件中寻找消息响应函数声明（从注释宏//{{AFX_MSG(CDrawView)...
 //}}AFX_MSG之间寻找），消息映射（从宏BEGIN_MESSAGE_MAP(...)....END_MESSAGE_MAP()之间寻找），最终找到对应的消息处理函数。当然，如果子类中没有对消息进行处理，则消息交由基类处理。


　　说明：


　　virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);


　　virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);



二、 绘图DC的获取

　　说明：在View类添加WM_LBUTTONDOWN和WM_LBUTTONUP的消息处理函数，我们将用来演示各种DC的获取，以及显示效果。

　　View类添加全局变量CPoint m_ptOrigin用来存储左键按下点坐标。下面集中来关注OnLButtonUp中的绘图程序和效果。

　　以下语句添加于OnLButtonUp函数中，可以查看不同的效果。


　　1， 使用SDK获取DC句柄： 全局函数方法

　　HDC hdc;

　　hdc=::GetDc(m_hWnd);//获取DC句柄

　　MoveToEx(hdc,m_ptOrigin.x,m_ptOrigin.y,NULL);

　　LineTo(hdc,point.x,point.y);

　　::ReleaseDC(m_hWnd,hdc);//释放DC


　　2， 利用CDC类指针和CWin类成员函数获取DC。这里的GetDc，ReleaseDc是CWnd成员函数。CDC封装了作图相关的所有操作。

　　CDC *pDC=GetDC();

　　pDC->MoveTo(m_ptOrigin);

　　pDC->LineTo(point);

　　ReleaseDC(pDC);


　　3， 利用CClientDC对象。（CClientDC类从CDC类派生来的）构造函数和析构函数，包含了DC的获取与释放、

　　CClientDC dc(this); //需要CWnd的一个指针。用this。

　　dc.MoveTo(m_ptOrigin);

　　dc.LineTo(point);


　　4， 利用CWindowDC对象。（CWindowDC类从CDC类派生来的）

　　CWindowDC dc(this);//

　　dc.MoveTo(m_ptOrigin);

　　dc.LineTo(point);


        CClientDC获取this指针，只能在视类的客户区画图，即视类本身，因为视类没有非客户区。

        获取父类指针，则在框架类的客户区，即菜单栏以下都可以画图，非客户区，即标题栏，菜单栏，不能画图。

       而对于CWindowDC，this指针下，无区别，父类指针时，可以在整个窗口区域，包含框架类的客户区和非客户区。

       同时，CWindowDc，还可以在桌面区域内画线。　　

　　可以分别试验画线效果。


　　6， 利用画笔改变线条颜色和类型：

　　CPen pen(PS_DOT,1,RGB(0,255,0));//构造画笔对象，选用虚线PS_DASH时，线宽必须小于1才有效。

　　CClientDC dc(this);CPen *pOldPen=dc.SelectObject(&pen);//将画笔选入DC.该函数返回的是先前的画笔。

　　dc.MoveTo(m_ptOrigin);

　　dc.LineTo(point);

　　dc.SelectObject(pOldPen);//恢复先前的画笔


　　7， 使用画刷（通常利用画刷去填充矩形区域）：

　　使用单色画刷

　　CBrush brush(RGB(255,0,0));//构造画刷对象

　　CClientDC dc(this);

　　dc.FillRect(CRect(m_ptOrigin,point),&brush);//用指定的画刷去填充矩形区域


　　使用位图画刷

　　CBitmap bitmap;//构造位图对象（使用前需要初试化）

　　bitmap.LoadBitmap(IDB_BITMAP1);//初试化位图对象

　　CBrush brush(&bitmap);//构造位图画刷

　　CClientDC dc(this);

　　dc.FillRect(CRect(m_ptOrigin,point),&brush);//用指定的位图画刷去填充矩形区域


　　使用透明画刷

　　CBrush *pBrush=CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));//获取透明画刷对象指针

　　CClientDC dc(this);

　　CBrush *pOldBrush=dc.SelectObject(pBrush);//将透明画刷选入DC

　　dc.Rectangle(CRect(m_ptOrigin,point));

　　dc.SelectObject(pOldBrush);//释放透明画刷



8， 注意点：

　　1）静态方法不属于某一个具体对象，而属于类本身，在类加载的时候就已经为类静态方法分配了代码去，故可用CBrush::FromHandle()形式调用。

　　2）静态方法中，不能引用非静态的数据成员和方法。

　　3）静态数据成员需要在类外单独做初始化，形式如： 变量类型 类名::变量名=初始值;







