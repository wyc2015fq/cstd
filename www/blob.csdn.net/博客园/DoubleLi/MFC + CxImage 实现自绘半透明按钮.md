# MFC + CxImage 实现自绘半透明按钮 - DoubleLi - 博客园






btn.h





**[cpp]**[view plain](http://blog.csdn.net/augusdi/article/details/8680064#)[copy](http://blog.csdn.net/augusdi/article/details/8680064#)



- #pragma once  
- 
- // CBtn  
- #include "ximage/ximage.h"  
- 
- class CBtn : public CButton  
- {  
-     DECLARE_DYNAMIC(CBtn)  
- private:  
-     CxImage * m_pImgNormal;  
-     CxImage * m_pImgOver;  
-     CxImage * m_pImgDown;  
-     CxImage * m_pImgFocus;  
-     CxImage * m_pImgDisable;  
- 
- BOOL m_bTracking;  
- BOOL m_bMouseOver;  
- BOOL m_bLBtnDown;  
- public:  
-     CBtn(UINT imgID_LbtnDown,UINT imgID_Over,UINT imgID_Normal,UINT imgID_Disabled=0,UINT imgID_Focus=0);  
- virtual ~CBtn();  
- 
- protected:  
-     DECLARE_MESSAGE_MAP()  
- public:  
- virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);  
-     afx_msg void OnDestroy();  
-     afx_msg BOOL OnEraseBkgnd(CDC* pDC);  
-     afx_msg void OnMouseMove(UINT nFlags, CPoint point);  
-     afx_msg void OnMouseLeave();  
-     afx_msg void OnMouseHover(UINT nFlags, CPoint point);  
- protected:  
- virtual BOOL PreCreateWindow(CREATESTRUCT& cs);  
- public:  
- void PaintParent();  
- virtual BOOL Create(LPCTSTR lpszCaption, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);  
-     afx_msg void OnLButtonDown(UINT nFlags, CPoint point);  
-     afx_msg void OnLButtonUp(UINT nFlags, CPoint point);  
-     afx_msg void OnBnDoubleclicked();  
-     afx_msg void OnKillFocus(CWnd* pNewWnd);  
- };  



btn.cpp









**[cpp]**[view plain](http://blog.csdn.net/augusdi/article/details/8680064#)[copy](http://blog.csdn.net/augusdi/article/details/8680064#)



- // Btn.cpp : 实现文件     
- //     
- 
- #include "stdafx.h"     
- #include "Btn.h"     
- 
- 
- // CBtn  
- IMPLEMENT_DYNAMIC(CBtn, CButton)    
- 
- CBtn::CBtn(UINT imgID_LbtnDown,UINT imgID_Over,UINT imgID_Normal,UINT imgID_Disabled,UINT imgID_Focus):    
-       m_pImgNormal(NULL)    
-     , m_pImgOver(NULL)    
-     , m_pImgDown(NULL)    
-     , m_pImgFocus(NULL)    
-     , m_pImgDisable(NULL)    
-     , m_bTracking(FALSE)    
-     , m_bMouseOver(FALSE)    
-     , m_bLBtnDown(FALSE)    
- {    
- if(imgID_Normal > 0)    
-     {    
- if(!m_pImgNormal)    
-             m_pImgNormal = new CxImage(CXIMAGE_FORMAT_PNG);    
-         m_pImgNormal ->LoadResource(FindResource(NULL,MAKEINTRESOURCE(imgID_Normal),L"PNG"),CXIMAGE_FORMAT_PNG);    
-     }    
- if(imgID_Focus > 0)    
-     {    
- if(!m_pImgFocus)    
-             m_pImgFocus = new CxImage(CXIMAGE_FORMAT_PNG);    
-         m_pImgFocus ->LoadResource(FindResource(NULL,MAKEINTRESOURCE(imgID_Focus),L"PNG"),CXIMAGE_FORMAT_PNG);    
-     }    
- if( imgID_LbtnDown>0 )    
-     {    
- if(!m_pImgDown)    
-             m_pImgDown = new CxImage(CXIMAGE_FORMAT_PNG);    
-         m_pImgDown ->LoadResource(FindResource(NULL,MAKEINTRESOURCE(imgID_LbtnDown),L"PNG"),CXIMAGE_FORMAT_PNG);    
-     }    
- if( imgID_Disabled > 0 )    
-     {    
- if(!m_pImgDisable)    
-             m_pImgDisable = new CxImage(CXIMAGE_FORMAT_PNG);    
-         m_pImgDisable ->LoadResource(FindResource(NULL,MAKEINTRESOURCE(imgID_Disabled),L"PNG"),CXIMAGE_FORMAT_PNG);    
-     }    
- if( imgID_Over > 0)    
-     {    
- if(!m_pImgOver)    
-             m_pImgOver = new CxImage(CXIMAGE_FORMAT_PNG);    
-         m_pImgOver ->LoadResource(FindResource(NULL,MAKEINTRESOURCE(imgID_Over),L"PNG"),CXIMAGE_FORMAT_PNG);    
-     }    
- }    
- 
- CBtn::~CBtn()    
- {    
- }    
- 
- BEGIN_MESSAGE_MAP(CBtn, CWnd)    
-     ON_WM_DESTROY()    
-     ON_WM_ERASEBKGND()    
-     ON_WM_MOUSEMOVE()    
-     ON_WM_MOUSELEAVE()    
-     ON_WM_MOUSEHOVER()    
-     ON_WM_LBUTTONDOWN()    
-     ON_WM_LBUTTONUP()    
-     ON_CONTROL_REFLECT(BN_DOUBLECLICKED, &CBtn::OnBnDoubleclicked)    
-     ON_WM_KILLFOCUS()    
- END_MESSAGE_MAP()    
- 
- 
- // CBtn 消息处理程序     
- void CBtn::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)    
- {    
-     CDC ButtonDC;     
-     CRect rc;    
- 
-     ButtonDC.Attach(lpDrawItemStruct->hDC); //得到用于绘制按钮的DC      
-     rc=lpDrawItemStruct->rcItem; //获取按钮所占的矩形大小     
- 
- UINT state = lpDrawItemStruct->itemState; //获取按钮目前所处的状态，根据不同的状态绘制不同的按钮      
- 
- if(state&ODS_DISABLED && m_pImgDisable)     
-     {     
-         m_pImgDisable ->Draw(ButtonDC.GetSafeHdc(),rc);    
-     }    
- else    
-     {    
- 
- if( m_bLBtnDown && m_pImgDown)    
-         {    
-             m_pImgDown ->Draw(ButtonDC.GetSafeHdc(),rc);    
-         }    
- else if(m_bMouseOver && m_pImgOver)    
-         {    
-             m_pImgOver ->Draw(ButtonDC.GetSafeHdc(),rc);    
-         }    
- else if(state&ODS_FOCUS && m_pImgFocus)     
-         {     
-             m_pImgFocus ->Draw(ButtonDC.GetSafeHdc(),rc);    
-         }    
- else if(m_pImgNormal)    
-         {    
-             m_pImgNormal ->Draw(ButtonDC.GetSafeHdc(),rc);    
-         }    
-     }    
- }    
- 
- void CBtn::OnDestroy()    
- {    
-     CButton::OnDestroy();    
- 
- // TODO: 在此处添加消息处理程序代码     
- if(m_pImgNormal)    
-     {    
-         m_pImgNormal ->Destroy();    
- delete m_pImgNormal;    
-         m_pImgNormal = NULL;    
-     }    
- if(m_pImgOver)    
-     {    
-         m_pImgOver ->Destroy();    
- delete m_pImgOver;    
-         m_pImgOver = NULL;    
-     }    
- if(m_pImgDown)    
-     {    
-         m_pImgDown ->Destroy();    
- delete m_pImgDown;    
-         m_pImgDown = NULL;    
-     }    
- if(m_pImgFocus)    
-     {    
-         m_pImgFocus ->Destroy();    
- delete m_pImgFocus;    
-         m_pImgFocus = NULL;    
-     }    
- if(m_pImgDisable)    
-     {    
-         m_pImgDisable ->Destroy();    
- delete m_pImgDisable;    
-         m_pImgDisable = NULL;    
-     }    
- }    
- 
- BOOL CBtn::OnEraseBkgnd(CDC* pDC)    
- {    
- // TODO: 在此添加消息处理程序代码和/或调用默认值     
- return TRUE;    
- //return CButton::OnEraseBkgnd(pDC);     
- }    
- 
- BOOL CBtn::PreCreateWindow(CREATESTRUCT& cs)    
- {    
- // TODO: 在此添加专用代码和/或调用基类     
-     cs.style |= BS_OWNERDRAW;    
- 
- return CButton::PreCreateWindow(cs);    
- }    
- 
- void CBtn::OnMouseMove(UINT nFlags, CPoint point)    
- {    
- // TODO: 在此添加消息处理程序代码和/或调用默认值     
-     m_bMouseOver = TRUE;    
- if (!m_bTracking)    
-     {    
-         TRACKMOUSEEVENT   tme;     
-         tme.cbSize      =   sizeof(TRACKMOUSEEVENT);     
-         tme.dwFlags     =   TME_LEAVE|TME_HOVER; //     
-         tme.hwndTrack   =   GetSafeHwnd();     
-         tme.dwHoverTime =   8;     
-         _TrackMouseEvent(&tme);    
-         m_bTracking = TRUE;    
-     }    
-     CButton::OnMouseMove(nFlags, point);    
-     ::SetCursor(AfxGetApp() ->LoadCursor(IDC_CURSOR_HAND));    
- }    
- 
- void CBtn::OnMouseLeave()    
- {    
- // TODO: 在此添加消息处理程序代码和/或调用默认值     
-     m_bMouseOver    =   FALSE;     
-     m_bTracking     =   FALSE;      
-     m_bLBtnDown     =   FALSE;    
-     PaintParent();    
-     CButton::OnMouseLeave();    
- }    
- 
- void CBtn::OnMouseHover(UINT nFlags, CPoint point)    
- {    
-     m_bMouseOver = TRUE;    
-     PaintParent();    
-     ::SetCursor(AfxGetApp() ->LoadCursor(IDC_CURSOR_HAND));    
- }    
- 
- void CBtn::PaintParent()    
- {    
-     CRect   rect;     
-     GetWindowRect(&rect);     
-     GetParent()-> ScreenToClient(&rect);     
-     GetParent()-> InvalidateRect(&rect);    
- }    
- 
- BOOL CBtn::Create(LPCTSTR lpszCaption, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)    
- {    
- BOOL OK=CButton::Create(lpszCaption, dwStyle, rect, pParentWnd, nID);    
-     ModifyStyleEx(0, WS_EX_TRANSPARENT);// WS_EX_LAYERED||WS_EX_TRANSPARENT       
- return OK;    
- }    
- 
- void CBtn::OnLButtonDown(UINT nFlags, CPoint point)    
- {    
-     m_bLBtnDown = TRUE;    
-     PaintParent();    
-     CButton::OnLButtonDown(nFlags, point);    
-     ::SetCursor(AfxGetApp() ->LoadCursor(IDC_CURSOR_HAND));    
- }    
- 
- void CBtn::OnLButtonUp(UINT nFlags, CPoint point)    
- {    
-     m_bLBtnDown = FALSE;    
-     PaintParent();    
-     CButton::OnLButtonUp(nFlags, point);    
-     ::SetCursor(AfxGetApp() ->LoadCursor(IDC_CURSOR_HAND));    
- }    
- 
- void CBtn::OnBnDoubleclicked()    
- {    
- // TODO: 在此添加控件通知处理程序代码     
-     ::SetCursor(AfxGetApp() ->LoadCursor(IDC_CURSOR_HAND));    
- }    
- 
- void CBtn::OnKillFocus(CWnd* pNewWnd)    
- {    
-     CButton::OnKillFocus(pNewWnd);    
- 
- // TODO: 在此处添加消息处理程序代码     
-     PaintParent();    
- }    








调用



**[cpp]**[view plain](http://blog.csdn.net/augusdi/article/details/8680064#)[copy](http://blog.csdn.net/augusdi/article/details/8680064#)



- ///头文件中添加  
- ...  
- #include "Btn.h"  
- ...  
- enum {emTestBtnId1=8099,emTestBtnId2,emTestBtnId3};  
-     CBtn m_btnTest1,m_btnTest2,m_btnTest3;//test  
- 
- ///在构造函数初始化列表中初始化变量(IDB_PNG_BTN_TEST_*为Png图片资源ID)     
-     , m_btnTest1(IDB_PNG_BTN_TEST_DOWN,  
-             IDB_PNG_BTN_TEST_MOUSEOVER,  
-             IDB_PNG_BTN_TEST_NORMAL,  
-             IDB_PNG_BTN_TEST_DISABLE,  
-             IDB_PNG_BTN_TEST_FOCUS)   
-     , m_btnTest2(IDB_PNG_BTN_TEST_DOWN,  
-             IDB_PNG_BTN_TEST_MOUSEOVER,  
-             IDB_PNG_BTN_TEST_NORMAL,  
-             IDB_PNG_BTN_TEST_DISABLE/*,IDB_PNG_BTN_TEST_FOCUS*/)   
-     , m_btnTest3(IDB_PNG_BTN_TEST_DOWN,  
-             IDB_PNG_BTN_TEST_MOUSEOVER,  
-             IDB_PNG_BTN_TEST_NORMAL,  
-             IDB_PNG_BTN_TEST_DISABLE/*,IDB_PNG_BTN_TEST_FOCUS*/)  
- 
- ////在OnInitDialog()函数中创建按钮  
- //test   
-     m_btnTest1.Create(_T("test"),WS_VISIBLE | WS_CHILD/* |WS_DISABLED*/,CRect(10,10,58,58),this,emTestBtnId1);   
-     m_btnTest2.Create(_T("test"),WS_VISIBLE | WS_CHILD/* |WS_DISABLED*/,CRect(10,60,58,108),this,emTestBtnId2);   
-     m_btnTest3.Create(_T("test"),WS_VISIBLE | WS_CHILD |WS_DISABLED,CRect(10,110,58,158),this,emTestBtnId2);  




效果图



![](http://img.my.csdn.net/uploads/201209/18/1347952351_8182.png)









