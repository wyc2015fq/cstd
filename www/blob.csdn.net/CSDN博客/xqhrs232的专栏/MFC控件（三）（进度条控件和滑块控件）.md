# MFC控件（三）（进度条控件和滑块控件） - xqhrs232的专栏 - CSDN博客
2014年05月19日 14:56:02[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：998
原文地址::[http://blog.csdn.net/muzi9_17/article/details/6385162](http://blog.csdn.net/muzi9_17/article/details/6385162)
相关文章
1、[[VC/MFC]滑动动条控件Slider的用法](http://www.cppblog.com/deane/articles/82060.html)----[http://www.cppblog.com/deane/articles/82060.html](http://www.cppblog.com/deane/articles/82060.html)
进度条控件
使用进度条显示操作进度：
进度条控件封装在CProgressCtrl类中。
1、
SetRange和SetRange32方法用来设置进度条显示范围。
语法格式：
void SetRange(short nLower, short nUpper);
void SetRange32(int nLower, int nUpper);
nLower:最小值，默认0；
nUpper:最大值。
SetRange方法设置进度条范围在短整型范围，SetRange32可在整数范围。
2、
设置进度条进度：SetPos方法
int SetPos（int nPos）;
nPos:表示设置的新进度。返回值为之前的进度。
3、
获得进度条进度：GetPos方法
int GetPos();
返回值为当前显示进度。
示例：
![](http://hi.csdn.net/attachment/201105/2/0_1304345906MQPQ.gif)
静态文本控件变量：CStatic  m_Number;
进度条控件变量：CProgressCtrl  m_Progress;
按钮单击事件：OnStart
void CProgressDemoDlg::OnStart()
{
    const int nMaxValue = 99999;
    m_Progress.SetRange32(0,nMaxValue);
    for(int i = 0; i<nMaxValue; i++)
    {
       m_Progress.SetPos(i);
       char szNumber[20] = {0};
       itoa(i, szNumber, 10);
       m_Number.SetWindowText(szNumber);
    }
}
注：
itoa方法：将整数转化为字符串
第三个参数:表示进制。（上例为十进制）
滑块控件：
与进度条控件使用类似：
SetRange设置范围
SetPos设置进度
GetPos获得当前进度
使用示例：
![](http://hi.csdn.net/attachment/201105/2/0_1304345947swXW.gif)
picture控件：框架类型
关联变量：CStatic  m_Area;
文本框控件关联变量：
UINT  m_nRvalue;
UINT  m_nGvalue;
UINT  m_nBvalue;
进度条控件关联变量：
CSliderCtrl  m_RSlider;
CSliderCtrl  m_GSlider;
CSliderCtrl  m_BSlider;
1、为对话框类添加成员变量：COLORREF  m_cColor;
2、对话框初始化时：
m_RSlider.SetRange(0,255);
m_GSlider.SetRange(0,255);
m_BSlider.SetRange(0,255);
m_cColor = RGB(0,0,0);
3、为对话框类添加WM_HSCROLL消息响应函数，进度条滑动时就会响应。
（若是竖直进度条，则添加WM_VSCROLL响应函数）
void CPickColorDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    if (nSBCode == SB_THUMBTRACK)
    {
       if (pScrollBar->m_hWnd == m_RSlider.m_hWnd)
       {
           m_nRValue = nPos;
           UpdateData(FALSE);
       }
       if (pScrollBar->m_hWnd == m_GSlider.m_hWnd)
       {
           m_nGValue = nPos;
           UpdateData(FALSE);
       }
       if (pScrollBar->m_hWnd == m_BSlider.m_hWnd)
       {
           m_nBValue = nPos;
           UpdateData(FALSE);
       }
    }
    m_clColor = RGB(m_nRValue, m_nGValue, m_nBValue);
    UpdateArea();
    CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}
4、上面提到的UpdateArea()函数，用于更新图片控件区域的颜色显示
需要在对话框类中添加成员函数：
public: void UpdateArea()
代码如下：
void CPickColorDlg::UpdateArea()
{
    CDC* pDC = m_Area.GetDC();
    CRect clientRC;
    m_Area.GetClientRect(clientRC);
    pDC->FillRect(clientRC,&CBrush(m_cColor));
    m_Area.ReleaseDC(pDC);
}
5、OnPaint()中也要添加UpdateArea();
6、为文本框控件添加EN_CHANGE消息响应函数。（当更改文本框内容时，滑块和Picture控件要做出相应响应）
代码如下：
void CPickColorDlg::OnChangeBvalue()
{
    UpdateData(TRUE);
    m_BSlider.SetPos(m_nBValue);
    UpdateArea();
}
void CPickColorDlg::OnChangeRvalue()
{
    UpdateData(TRUE);
    m_RSlider.SetPos(m_nRValue);
    UpdateArea();
}
void CPickColorDlg::OnChangeGvalue()
{
    UpdateData(TRUE);
    m_GSlider.SetPos(m_nGValue);
    UpdateArea();
}
