# 系统托盘图标显示菜单（TrackPopupMenu）无法自动消失解决方法 - sandro_zhang的专栏 - CSDN博客
2011年09月19日 16:39:19[sandro_zhang](https://me.csdn.net/sandro_zhang)阅读数：2518
函数：SetForegroundWindow
需要先设置当前窗口为系统活动窗口，之后，系统就会自动设置响应的操作，比如自动隐藏之类的。
```cpp
HMENU menu = CreatePopupMenu();
AppendMenu(menu, MF_STRING, 2, strExit.c_str() );
::SetForegroundWindow(g_hMainWnd);
UINT cmd = TrackPopupMenu(menu, TPM_RETURNCMD|TPM_RIGHTBUTTON, pt.x, pt.y, 0, g_hMainWnd, NULL);
```
