# VC 双缓冲绘图 - xqhrs232的专栏 - CSDN博客
2012年06月20日 16:19:15[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：3153标签：[delphi																[integer																[null](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=integer&t=blog)](https://so.csdn.net/so/search/s.do?q=delphi&t=blog)
个人分类：[C/C++/VC++/VS](https://blog.csdn.net/xqhrs232/article/category/906934)
原文地址::[http://www.cppblog.com/erran/archive/2006/11/25/15650.html](http://www.cppblog.com/erran/archive/2006/11/25/15650.html)
相关网帖
1.VC 双缓冲技术 ----[http://blog.csdn.net/wooden_men/article/details/5662305](http://blog.csdn.net/wooden_men/article/details/5662305)
VC 双缓冲绘图
BOOL CGraphDlg::OnDraw(CDC* pDC)
{
       CRect rc;
       GetClientRect(&rc); 
       CDC  MemDC;
       CBitmap  MemBitmap;  
       MemDC.CreateCompatibleDC(NULL);      
       MemBitmap.CreateCompatibleBitmap(pDC, rc.Size().cx,rc.Size().cy);
       CBitmap *pOldBit=MemDC.SelectObject(&MemBitmap);      
       MemDC.BitBlt(0,0,rc.width, rc.height, pDC,0,0,SRCCOPY);
       m_pGraph->SetGraphDC(&MemDC);
       m_pGraph->DoGraphShowEvent();  
       pDC->BitBlt(0,0, rc.Size().cx,rc.Size().cy,&MemDC,0,0,SRCCOPY);  
       MemDC.SelectObject(pOldBit);
       MemBitmap.DeleteObject();
       MemDC.DeleteDC();
}
Delphi 双缓冲绘图：
procedure TBkCoordi.Draw(Canvas : TCanvas);
var
    _bmp : TBitmap;
    _width, _height : integer;
    _top, _left : integer;
begin
    _width := Round(m_width);
    _height := Round(m_height);
    _left := Round(m_topleft.x);
    _top := Round(m_topleft.y);
    _bmp := Tbitmap.Create;
    _bmp.Handle := CreateCompatibleDC(Canvas.Handle);
    _bmp.Width :=  _width;
    _bmp.Height :=  _height;
    BitBlt(_bmp.Canvas.Handle,0,0,_width,_height, Canvas.Handle,_left,_top,SRCCOPY);
    if m_showcross then m_cross.Draw(_bmp.Canvas);
    if @OnDrawing <> nil then OnDrawing(_bmp.Canvas);
    BitBlt(Canvas.Handle,_left,_top,_width,_height, _bmp.Canvas.Handle,0,0,SRCCOPY);
    _bmp.Free;
end;
