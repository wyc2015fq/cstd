# 【Win32编程】C语言实现恶搞小程序 - weixin_33985507的博客 - CSDN博客
2018年10月29日 17:03:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：33
### 程序界面
该程序的 [下载链接](https://www.lanzous.com/i27xwqf)
![10472551-09616b40aa4fdecc.png](https://upload-images.jianshu.io/upload_images/10472551-09616b40aa4fdecc.png)
![10472551-d5efe817d51f0bad.png](https://upload-images.jianshu.io/upload_images/10472551-d5efe817d51f0bad.png)
![10472551-0ac90bdaaa055d8c.png](https://upload-images.jianshu.io/upload_images/10472551-0ac90bdaaa055d8c.png)
**已实现功能**
按钮2会随着鼠标移入而随机移动，按钮3以及空白处都添加了消息处理函数的，灰化了窗口的最大化和关闭按钮，本来想加入隐藏进程的，一个原因是实现比较困难，另一个则是对外行小伙伴不友好，所以最后就没加入隐藏进程！最后硬编码了几十个坐标并且添加了按钮2点中后的回调函数，当点中按钮2便会获得该程序的彩蛋！
`P.S.之所以没用随机数来决定坐标是因为随机数坐标真的太难点中了`
### 核心代码解释
- **静态文本框背景重绘**
默认的文本背景颜色是灰色的，巨丑无比所以决定重绘为跟窗口背景色一致
```
case WM_CTLCOLORSTATIC:
        /* 重绘静态文本框的背景色和字体色 */
        if ((HWND)lParam == GetDlgItem(hwnd, 1))
        {
            SetTextColor((HDC)wParam, RGB(0, 0, 0));
            SetBkMode((HDC)wParam, TRANSPARENT);
        }
        return (BOOL)(HBRUSH)GetStockObject(NULL_BRUSH);
        break;
```
- **添加按钮消息处理函数**
主窗口是用 WM_COMMAND 来接收按钮消息的，并且用 ID 判定哪个按钮被点击了
```
case WM_COMMAND:
        /* 当点击菜单、按钮、下拉列表框等控件时候,会触发WM_COMMAND，此处用于捕获点击按钮的消息*/
        id = LOWORD(wParam);
        event = HIWORD(wParam);
        switch (id)
        {
        case 3:
            MessageBox(hwnd, "Are you a pig?", "233333", MB_OK);
            break;
        case 2:
            MessageBox(hwnd, "You are a genius!But...", "666666", MB_OK);
            if (GetProcessPrivilege())
            {
                ExitWindowsEx(EWX_LOGOFF | EWX_FORCE, 0);
            }
            break;
        default:
            return DefWindowProc(hwnd, message, wParam, lParam);
        }
        break;
```
- **控件子类化**
按钮等控件也是属于子窗口，本身的回调函数是系统预定义的，使用`SetWindowLong`或者 SetWindowSubclass 子类化窗口后，才能为该控件添加回调函数！注意：`要使用 WNDPROC OldWndProc 定义一个变量存储旧的控件回调函数地址，用于在控件的消息处理函数中恢复旧的回调函数，否则按钮无法正常显示`
```
WNDPROC OldWndProc;
OldWndProc = (WNDPROC)SetWindowLong(GetDlgItem(hwnd, 2), GWL_WNDPROC, (LONG)BtnProc);
```
- **控件回调函数**
首先注意要调用`CallWindowProc`恢复控件的原始回调函数，之后要先在 WM_MOUSEMOVE 消息中使用`TrackMouseEvent`激活鼠标的 Hover 记录，最后才能捕获 WM_MOUSEHOVER 消息
```
LRESULT CALLBACK BtnProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    int i;
    int array[][2] = {0, 0, 100, 0, 400, 0, 500, 0, 0, 50, 100, 50, 400, 50, 500, 50, 0, 100, 100, 100, 200, 100, 500, 100, 0, 150, 100, 150, 200, 150, 300, 150, 400, 150, 500, 150, 0, 200, 100, 200, 300, 200, 400, 200, 500, 200, 0, 250, 100, 250, 200, 250, 300, 250, 400, 250, 500, 250, 0, 300, 100, 300, 200, 300, 300, 300, 400, 300, 500, 300, 121, 324, 160, 23, 153, 121, 56, 288, 79, 160, 149, 25, 158, 46, 12, 17, 15, 212, 89, 21, 131, 278, 11, 263};
    TRACKMOUSEEVENT mouse;
    srand(time(0));
    i = rand() % (sizeof(array) / sizeof((array)[0][0]) / 2);
    switch (message)
    {
    case WM_MOUSEMOVE:
        /* 设置捕获鼠标的WM_MOUSEHOVER以及WM_MOUSELEAVE，系统默认是不捕获的，还可以使用SetCapture */
        mouse.cbSize = sizeof(mouse);
        mouse.hwndTrack = hwnd;
        mouse.dwFlags = TME_LEAVE | TME_HOVER;
        mouse.dwHoverTime = 1;
        TrackMouseEvent(&mouse);
        break;
    case WM_MOUSEHOVER:
        /* 鼠标移入移动窗口 */
        MoveWindow(hwnd, array[i][0], array[i][1], 100, 50, 1);
        SetFocus(GetParent(hwnd));
        break;
    case WM_LBUTTONDOWN:
        /* 单击鼠标左键移动窗口 */
        MoveWindow(hwnd, array[i][0], array[i][1], 100, 50, 1);
        SetFocus(GetParent(hwnd));
        break;
    case WM_KEYDOWN:
        /* 按键后移动窗口 */
        MoveWindow(hwnd, array[i][0], array[i][1], 100, 50, 1);
        SetFocus(GetParent(hwnd));
        break;
    }
    /* 返回CallWindowProc()是必要的，如果没有控件子类化后无法正常显示该控件 */
    return CallWindowProc(OldWndProc, hwnd, message, wParam, lParam);
}
```
- **进程提权**
关于这部分功能我就不剧透了，这是点中后的彩蛋！使用了`OpenProcessToken``LookupPrivilegeValue` 以及`AdjustTokenPrivileges`提升该进程权限方便后续操作
```
BOOL GetProcessPrivilege()
{
    HANDLE processhandle, tokenhandle;
    TOKEN_PRIVILEGES NewState;
    /* 获取当前进程的一个句柄，不可调用CloseHandle关闭 */
    processhandle = GetCurrentProcess();
    /* 打开与目标进程相关联的访问令牌，将返回的访问令牌地址保存到tokenhandle */
    if (!OpenProcessToken(processhandle, TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &tokenhandle))
    {
        return 0;
    }
    /* 函数查看系统权限的特权值，返回信息到一个LUID结构体里，本地系统的话第一个参数为NULL*/
    /* 权限值包括有SeDebugPrivilege,SeShutdownPrivileg,SeBackupPrivileg,SeRestorePrivileg... */
    LookupPrivilegeValue(NULL, "SeShutdownPrivileg", &NewState.Privileges[0].Luid);
    /* 上面设置了Luid，下面会设置PrivilegeCount以及Attributes */
    NewState.PrivilegeCount = 1;
    NewState.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    /* 启用或禁止，指定访问令牌的权限 */
    AdjustTokenPrivileges(tokenhandle, FALSE, &NewState, sizeof(NewState), NULL, NULL);
    CloseHandle(tokenhandle);
    if (!GetLastError())
    {
        return 0;
    }
    return 1;
}
```
### 完整源码
专门添加了注释的，有兴趣的小伙伴可以自行研究
```
#include <windows.h>
#include <stdlib.h>
#include <time.h>
#define GWL_HINSTANCE (-6)
#define GWL_WNDPROC (-4)
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK BtnProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL GetProcessPrivilege();
WNDPROC OldWndProc;
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
{
    char *lpClassName = "Win3", *lpWindowName = "2333333";
    HWND hwnd;
    MSG msg;
    WNDCLASSEX wndclass;
    /* 注册窗口类 */
    wndclass.cbSize = sizeof(wndclass);                    // 指定该结构体的大小
    wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_NOCLOSE; // 宽度高度变化时重绘
    wndclass.lpfnWndProc = WndProc;                        // 窗口回调函数
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);    // NULL表示系统图标，默认图标
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);      // 默认光标
    wndclass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); // 使用画刷填充背景色，还可以(HBRUSH)GetStockObject(BLACK_BRUSH)
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = lpClassName;
    wndclass.hIconSm = NULL;
    if (!RegisterClassEx(&wndclass))
    {
        MessageBox(NULL, "Register Failed", "Tips", MB_ICONERROR);
        return 0;
    }
    /* 创建窗口 */
    hwnd = CreateWindow(lpClassName,
                        lpWindowName,
                        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, //style
                        CW_USEDEFAULT,                                            // x
                        CW_USEDEFAULT,                                            // y
                        600,                                                      // width
                        400,                                                      // height
                        NULL,                                                     // hWndParent
                        NULL,                                                     // hMenu
                        hInstance,                                                // hInstance
                        NULL);
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd); // 发送 WM_PAINT，保证窗口一定可以刷新显示
    /* 消息循环 */
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}
/* 主窗口的消息回调函数 */
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    int id, event;
    PAINTSTRUCT ps;
    RECT rect;
    switch (message)
    {
    case WM_CREATE:
        /* 创建静态文本框控件 */
        CreateWindow("static",
                     "1+1=",
                     WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE | SS_RIGHT,
                     CW_USEDEFAULT,
                     CW_USEDEFAULT,
                     300,
                     100,
                     hwnd,
                     (HMENU)1,
                     (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
                     NULL);
        /* 创建按钮控件 */
        CreateWindow("button",
                     "2",
                     WS_CHILD | WS_VISIBLE | WS_BORDER | BS_FLAT,
                     170,
                     100,
                     100,
                     50,
                     hwnd,
                     (HMENU)2,
                     (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
                     NULL);
        /* 创建按钮控件 */
        CreateWindow("button",
                     "3",
                     WS_CHILD | WS_VISIBLE | WS_BORDER | BS_FLAT,
                     310,
                     100,
                     100,
                     50,
                     hwnd,
                     (HMENU)3,
                     (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
                     NULL);
        /* 控件子类化，并保存控件的原回调函数，除此外还可以用SetWindowSubclass子类化控件 */
        OldWndProc = (WNDPROC)SetWindowLong(GetDlgItem(hwnd, 2), GWL_WNDPROC, (LONG)BtnProc);
        break;
    case WM_CTLCOLORSTATIC:
        /* 重绘静态文本框的背景色和字体色 */
        if ((HWND)lParam == GetDlgItem(hwnd, 1))
        {
            SetTextColor((HDC)wParam, RGB(0, 0, 0));
            SetBkMode((HDC)wParam, TRANSPARENT);
        }
        return (BOOL)(HBRUSH)GetStockObject(NULL_BRUSH);
        break;
    case WM_LBUTTONDOWN:
        /* 让主窗口捕获鼠标左键消息 */
        MessageBox(hwnd, "Don't click here!", "Tips", MB_OK);
        break;
    case WM_COMMAND:
        /* 当点击菜单、按钮、下拉列表框等控件时候,会触发WM_COMMAND，此处用于捕获点击按钮的消息*/
        id = LOWORD(wParam);
        event = HIWORD(wParam);
        switch (id)
        {
        case 3:
            MessageBox(hwnd, "Are you a pig?", "233333", MB_OK);
            break;
        case 2:
            MessageBox(hwnd, "You are a genius!But...", "666666", MB_OK);
            if (GetProcessPrivilege())
            {
                ExitWindowsEx(EWX_LOGOFF | EWX_FORCE, 0);
            }
            break;
        default:
            return DefWindowProc(hwnd, message, wParam, lParam);
        }
        break;
    case WM_PAINT:
        /* 主窗口重绘 */
        hdc = BeginPaint(hwnd, &ps);
        EndPaint(hwnd, &ps);
        break;
    case WM_CLOSE:
        /* 关闭窗口 */
        DestroyWindow(hwnd);
        break;
    case WM_DESTROY:
        /* 关闭消息循环 */
        PostQuitMessage(0);
        break;
    }
    return DefWindowProc(hwnd, message, wParam, lParam);
}
/* 按钮控件的消息回调函数 */
LRESULT CALLBACK BtnProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    int i;
    int array[][2] = {0, 0, 100, 0, 400, 0, 500, 0, 0, 50, 100, 50, 400, 50, 500, 50, 0, 100, 100, 100, 200, 100, 500, 100, 0, 150, 100, 150, 200, 150, 300, 150, 400, 150, 500, 150, 0, 200, 100, 200, 300, 200, 400, 200, 500, 200, 0, 250, 100, 250, 200, 250, 300, 250, 400, 250, 500, 250, 0, 300, 100, 300, 200, 300, 300, 300, 400, 300, 500, 300, 121, 324, 160, 23, 153, 121, 56, 288, 79, 160, 149, 25, 158, 46, 12, 17, 15, 212, 89, 21, 131, 278, 11, 263};
    TRACKMOUSEEVENT mouse;
    srand(time(0));
    i = rand() % (sizeof(array) / sizeof((array)[0][0]) / 2);
    switch (message)
    {
    case WM_MOUSEMOVE:
        /* 设置捕获鼠标的WM_MOUSEHOVER以及WM_MOUSELEAVE，系统默认是不捕获的，还可以使用SetCapture */
        mouse.cbSize = sizeof(mouse);
        mouse.hwndTrack = hwnd;
        mouse.dwFlags = TME_LEAVE | TME_HOVER;
        mouse.dwHoverTime = 1;
        TrackMouseEvent(&mouse);
        break;
    case WM_MOUSEHOVER:
        /* 鼠标移入移动窗口 */
        MoveWindow(hwnd, array[i][0], array[i][1], 100, 50, 1);
        SetFocus(GetParent(hwnd));
        break;
    case WM_LBUTTONDOWN:
        /* 单击鼠标左键移动窗口 */
        MoveWindow(hwnd, array[i][0], array[i][1], 100, 50, 1);
        SetFocus(GetParent(hwnd));
        break;
    case WM_KEYDOWN:
        /* 按键后移动窗口 */
        MoveWindow(hwnd, array[i][0], array[i][1], 100, 50, 1);
        SetFocus(GetParent(hwnd));
        break;
    }
    /* 返回CallWindowProc()是必要的，如果没有控件子类化后无法正常显示该控件 */
    return CallWindowProc(OldWndProc, hwnd, message, wParam, lParam);
}
BOOL GetProcessPrivilege()
{
    HANDLE processhandle, tokenhandle;
    TOKEN_PRIVILEGES NewState;
    /* 获取当前进程的一个句柄，不可调用CloseHandle关闭 */
    processhandle = GetCurrentProcess();
    /* 打开与目标进程相关联的访问令牌，将返回的访问令牌地址保存到tokenhandle */
    if (!OpenProcessToken(processhandle, TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &tokenhandle))
    {
        return 0;
    }
    /* 函数查看系统权限的特权值，返回信息到一个LUID结构体里，本地系统的话第一个参数为NULL*/
    /* 权限值包括有SeDebugPrivilege,SeShutdownPrivileg,SeBackupPrivileg,SeRestorePrivileg... */
    LookupPrivilegeValue(NULL, "SeShutdownPrivileg", &NewState.Privileges[0].Luid);
    /* 上面设置了Luid，下面会设置PrivilegeCount以及Attributes */
    NewState.PrivilegeCount = 1;
    NewState.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    /* 启用或禁止，指定访问令牌的权限 */
    AdjustTokenPrivileges(tokenhandle, FALSE, &NewState, sizeof(NewState), NULL, NULL);
    CloseHandle(tokenhandle);
    if (!GetLastError())
    {
        return 0;
    }
    return 1;
}
```
调用`gcc -m32 -mwindows win3.c -o win3`编译即可
### END
