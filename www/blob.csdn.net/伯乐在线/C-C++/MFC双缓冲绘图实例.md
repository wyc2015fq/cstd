# MFC双缓冲绘图实例 - 文章 - 伯乐在线
原文出处： [BattleScars，2016-11-28](http://www.cnblogs.com/battlescars/p/cpp_mfc_drawonbuffer.html#top)
本人之前一直了解双缓冲绘图的基本原理，但是在研究很久之后才大概知道具体的使用过程，本文将详细介绍本人在实际项目中使用双缓冲绘图的案例。
实现功能：主界面显示某张包含人脸的图片，通过dlib detector获取到人脸上的68个关键点，绘制在图片上显示，然后通过鼠标拖动图片上的关键点，调整位置，之后保存。双缓冲主要能够解决拖动关键点时屏幕闪烁的问题，本文主要侧重在双缓冲的实现，其他功能概不介绍。
具体实现：
1.定义全局变量:

```
CDC dc_mem://内存绘制dc
CDC *dc：//绘图dc
vector<CPoint> face：//保存人脸中关键点的坐标
CBitmap bitmap; //内存绘图相关变量
CImage image;
```
2.在OnInitDialog()函数中初始化绘图dc=GetDC();
3.OnEraseBkgnd()函数直接return true;
添加函数afx_msg BOOL OnEraseBkgnd(CDC* pDC);
添加消息ON_WM_ERASEBKGND();
这一步很关键！！！
4.在OnPaint()中调用DrawOnBuffer()绘图

```
void CEditLmDlg::DrawOnBuffer()
{
    CRgn rgn;
    rgn.CreateRectRgn(0, 0, image_width, image_height);
    dc->SelectClipRgn(&rgn);
 
    dc_mem.CreateCompatibleDC(dc);
    bitmap.CreateCompatibleBitmap(dc, edit_rect.Width(), edit_rect.Height());
    CBitmap *pOldBit = dc_mem.SelectObject(&bitmap);
    dc_mem.FillSolidRect(edit_rect, dc->GetBkColor());
 
    dc_mem.SetStretchBltMode(HALFTONE);
    CRect rect(0, 0, image_width, image_height);
    image.Draw(dc_mem.m_hDC, rect);
     
    dc->BitBlt(0, 0, edit_rect.Width(), edit_rect.Height(), &dc_mem, 0, 0,SRCCOPY);
    
    /**将所有的点绘制到dc_mem上*/代码略
 
    dc->SelectClipRgn(NULL);
    dc_mem.DeleteDC();
    bitmap.DeleteObject();
}
```
函数中CreateRectRgn函数设置裁剪区可以保证刷新时只刷新图片的部分，不刷新图片外的其他控件，这样其他控件就不会出现闪烁的情况，另外函数结束时要将裁剪区设置为空。
5.至于拖动关键点的操作需要调用以下三个函数，以及声明对应的三个消息即可，在OnLButtonUp，OnMouseMove中将变化后的点的坐标更新到face中，并调用Invalidate()即可。

```
afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
afx_msg void OnMouseMove(UINT nFlags, CPoint point);
ON_WM_LBUTTONDOWN()
ON_WM_LBUTTONUP()
ON_WM_MOUSEMOVE()
```
综上，双缓冲绘图的关键是怎么把所有的绘图操作放到一个绘图函数中，该过程可能需要很多的全局变量来保存绘图相关数据。
