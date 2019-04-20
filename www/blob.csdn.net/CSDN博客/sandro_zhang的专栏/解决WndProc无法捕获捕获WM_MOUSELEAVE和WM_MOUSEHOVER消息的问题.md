# 解决WndProc无法捕获捕获WM_MOUSELEAVE和WM_MOUSEHOVER消息的问题 - sandro_zhang的专栏 - CSDN博客
2011年09月28日 10:50:38[sandro_zhang](https://me.csdn.net/sandro_zhang)阅读数：1661
1，定义一个全局变量，来标记下是否在Track。
```cpp
BOOL _bMouseTrack=TRUE;
```
2，在回调函数的WM_MOUSEMOVE消息中使用TrackMouseEvent函数
```cpp
case WM_MOUSEMOVE:
            if (_bMouseTrack) 
             {
                  TRACKMOUSEEVENT csTME;
                  csTME.cbSize = sizeof(csTME);
                  csTME.dwFlags = TME_LEAVE|TME_HOVER;
                  csTME.hwndTrack = hWnd;
                  csTME.dwHoverTime = 5;//持续多长算HOVER
                  TrackMouseEvent(&csTME);
                  _bMouseTrack=FALSE; 
             }
            break;
```
3，在WM_MOUSELEAVE消息中，设置为TRUE 即可
```cpp
case WM_MOUSELEAVE:
            _bMouseTrack=TRUE;
            break;
```
全局标记可以不设置也行，我试验也没发现什么因为重复设置而导致的错误。
            
