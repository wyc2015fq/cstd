# MFC和OpenCV结合 - wishchinYang的专栏 - CSDN博客
2013年07月17日 12:47:58[wishchin](https://me.csdn.net/wishchin)阅读数：1426
最重要的一点：如何把OpenCV的框嵌入MFC的ID..
把cv显示框嵌入 pic控件
```python
cvNamedWindow("kalman");  
    HWND hWnd = (HWND) cvGetWindowHandle("kalman");
    HWND hParent = ::GetParent(hWnd);
    ::SetParent(hWnd, GetDlgItem(IDC_SHOWPIC)->m_hWnd);
    ::ShowWindow(hParent, SW_HIDE);
```
