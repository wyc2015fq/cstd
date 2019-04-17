# 实现一个类似360的button - DoubleLi - 博客园






通过改写一个buttonst类，实现360效果的button。

主要可以完成：frame，hover,face效果，并且支持menu,tooltips

1）派生新的类QButton.添加虚函数，设置自绘style





**[cpp]**[view plain](http://blog.csdn.net/augusdi/article/details/8680028#)[copy](http://blog.csdn.net/augusdi/article/details/8680028#)



- void QButton::PreSubclassWindow()   
- {  
- // TODO: Add your specialized code here and/or call the base class  
- UINT nBS;  
- 
- nBS = GetButtonStyle();  
- 
- // Set initial control type  
- m_nTypeStyle = nBS & BS_TYPEMASK;  
- 
- // Check if this is a checkbox  
- if (nBS & BS_CHECKBOX) m_bIsCheckBox = TRUE;  
- 
- // Set initial default state flag  
- if (m_nTypeStyle == BS_DEFPUSHBUTTON)  
- {  
- // Set default state for a default button  
- m_bIsDefault = TRUE;  
- 
- // Adjust style for default button  
- m_nTypeStyle = BS_PUSHBUTTON;  
- } // If  
- 
- // You should not set the Owner Draw before this call  
- // (don't use the resource editor "Owner Draw" or  
- // ModifyStyle(0, BS_OWNERDRAW) before calling PreSubclassWindow() )  
- ASSERT(m_nTypeStyle != BS_OWNERDRAW);  
- 
- // Switch to owner-draw  
- ModifyStyle(BS_TYPEMASK, BS_OWNERDRAW, SWP_FRAMECHANGED);  
- CButton::PreSubclassWindow();  
- }  








2)添加一些控制的变量





**[cpp]**[view plain](http://blog.csdn.net/augusdi/article/details/8680028#)[copy](http://blog.csdn.net/augusdi/article/details/8680028#)



- void QButton::DrawItem(LPDRAWITEMSTRUCT lpDIS)   
- {  
- // TODO: Add your code to draw the specified item  
- CDC* pDC = CDC::FromHandle(lpDIS->hDC);  
- CRect rc = lpDIS->rcItem;  
- CBitmap bitmap;  
- bitmap.CreateCompatibleBitmap(pDC, rc.Width(), rc.Height());  
- CDC memdc;  
- memdc.CreateCompatibleDC(pDC);  
- memdc.SelectObject(bitmap);  
- PaintBk(&memdc);  
- memdc.SelectObject(GetStockObject(NULL_BRUSH));  
- memdc.Rectangle(rc);  
- if(m_bIsCheckBox)  
- {  
- m_bIsPressed = (lpDIS->itemState & ODS_SELECTED) || (m_nCheck != 0);  
- }else  
- {  
- m_bIsPressed = (lpDIS->itemState & ODS_SELECTED);  
- }  
- m_bIsFocused = (lpDIS->itemState & ODS_FOCUS);  
- m_bIsDisabled = (lpDIS->itemState & ODS_DISABLED);  
- if(m_bIsDisabled)  
- {  
- if(m_pImage_disable != NULL)  
- m_pImage_disable->Draw(memdc.m_hDC, rc);  
- }else  
- {  
- if(m_bIsPressed)  
- {  
- if(m_pImage_pressed != NULL){  
- m_pImage_face->Draw(memdc.m_hDC, rc);  
- CRect rc2;  
- rc2 = rc;  
- rc2.OffsetRect(2,2);  
- m_pImage_pressed->Draw(memdc.m_hDC, rc);  
- }  
- }else if(m_bIsHovering)  
- {//draw hover image  
- if(m_pImage_hover != NULL && m_pImage_face != NULL)  
- {  
- m_pImage_face->Draw(memdc.m_hDC, rc);  
- m_pImage_hover->Draw(memdc.m_hDC, rc);  
- }  
- }else  
- {  
- if(m_pImage_face != NULL){  
- m_pImage_face->Draw(memdc.m_hDC, rc);  
- }  
- }  
- }  
- 
- pDC->BitBlt(0, 0, rc.Width(), rc.Height(), &memdc, 0, 0, SRCCOPY);  
- TRACE("m_bIsPressed = %d\n", m_bIsPressed);  
- }  


直接用memdc绘制，frame,hover,face

3)使用cximage来加载png图片资源





**[cpp]**[view plain](http://blog.csdn.net/augusdi/article/details/8680028#)[copy](http://blog.csdn.net/augusdi/article/details/8680028#)



- BOOL QButton::LoadDisablePng(UINT nID, LPCTSTR cType)  
- {  
- if(m_pImage_disable != NULL)  
- delete m_pImage_disable;  
- m_pImage_disable = new CxImage;  
- return m_pImage_disable->LoadResource(FindResource(NULL,MAKEINTRESOURCE(nID),cType), CXIMAGE_FORMAT_PNG);  
- }  



4）添加mouse hover等支持。

5）[测试](http://lib.csdn.net/base/softwaretest)程序





**[cpp]**[view plain](http://blog.csdn.net/augusdi/article/details/8680028#)[copy](http://blog.csdn.net/augusdi/article/details/8680028#)



- OnInitDialog()  
- {  
- m_btn.LoadFacePng(IDR_PNG8,"PNG");   
-   m_btn.LoadPressPng(IDR_PNG10,"PNG");  
- // m_btn.LoadDisablePng(IDR_PNG10,"PNG");  
-   m_btn.LoadHoverPng(IDR_PNG9,"PNG");  
- m_btn.SetTooltipText("Hunter lv Tool tips!", TRUE);  
- m_btn.SetMenu(IDR_MENU1, this->m_hWnd, TRUE);////QButton m_btn  
- }  

![](http://static.blog.csdn.net/images/save_snippets.png)




6)需要自己编译cximage的库，然后添加库文件，头文件就可以直接使用。









