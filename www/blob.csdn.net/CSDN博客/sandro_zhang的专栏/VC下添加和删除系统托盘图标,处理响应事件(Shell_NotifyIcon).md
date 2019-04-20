# VC下添加和删除系统托盘图标,处理响应事件(Shell_NotifyIcon) - sandro_zhang的专栏 - CSDN博客
2011年11月01日 16:08:01[sandro_zhang](https://me.csdn.net/sandro_zhang)阅读数：4497
分步骤如下:
1,定义用户消息
```cpp
#define WM_TRAY_NOTIFY (WM_USER + 8118)
```
2,添加托盘图标
```cpp
NOTIFYICONDATA data;
memset(&data, 0, sizeof(data));
// the basic functions need only V1
#ifdef NOTIFYICONDATA_V1_SIZE
    data.cbSize = NOTIFYICONDATA_V1_SIZE;
#else
    data.cbSize = sizeof(data);
#endif
data.hWnd = hWnd;
data.uCallbackMessage = WM_TRAY_NOTIFY;
data.uID = IDI_MAIN_ICON;
data.hIcon = LoadIcon(g_hInstance, MAKEINTRESOURCE(IDI_MAIN_ICON) );
data.uFlags =  NIF_ICON | NIF_TIP | NIF_MESSAGE  ;
wstring name = notify_config::GetResourceString( IDS_TIP_NAME );
size_t tip_len = min(sizeof(data.szTip)-1, wcslen(name.c_str())*sizeof(wchar_t));
memcpy(data.szTip, name.c_str(), tip_len);
data.szTip[tip_len] = 0;
Shell_NotifyIcon(NIM_ADD, &data);
```
其中hWnd为窗体句柄,处理消息块则在此窗体的回调函数中捕捉消息.
3,消息处理
```cpp
LRESULT CALLBACK MainWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
    switch (uMsg)
    {
        case WM_TRAY_NOTIFY:
        {
            switch( lParam )
            {
                case WM_RBUTTONUP:
                //do something
                break;
            }
        }
        break;
    }
...
}
```
4,删除图标
根据NOTIFYICONDATA的结构
```cpp
typedef struct _NOTIFYICONDATA {
  DWORD cbSize;
  HWND  hWnd;
  UINT  uID;
  UINT  uFlags;
  UINT  uCallbackMessage;
  HICON hIcon;
  TCHAR szTip[64];
  DWORD dwState;
  DWORD dwStateMask;
  TCHAR szInfo[256];
  union {
    UINT uTimeout;
    UINT uVersion;
  };
  TCHAR szInfoTitle[64];
  DWORD dwInfoFlags;
  GUID  guidItem;
  HICON hBalloonIcon;
} NOTIFYICONDATA, *PNOTIFYICONDATA;
```
一个图标是有个别参数来标识的,即 hWnd+uID 或者 guidItem,在此就以上面的为例子,代码如下:
```cpp
NOTIFYICONDATA data;
memset(&data, 0, sizeof(data));
data.hWnd = g_hMainWnd;
data.uID = IDI_MAIN_ICON;
Shell_NotifyIcon(NIM_DELETE, &data);
```
参考:[http://msdn.microsoft.com/en-us/library/bb773352(VS.85).aspx](http://msdn.microsoft.com/en-us/library/bb773352(VS.85).aspx)
