# VC设置createwindow方法生成的form, button的bitmap背景图 - sandro_zhang的专栏 - CSDN博客
2011年09月30日 15:37:26[sandro_zhang](https://me.csdn.net/sandro_zhang)阅读数：3342
前两天做了一个notify window的提示窗口，现整理纠结出来的主要方法。
1，设置window form的背景图(IDI_NOTIFY_BG为resource.rc文件中定义的bitmap文件)
```cpp
MSG msg;
        HWND hWnd;
        wstring szTitle = L"Test";            
        wstring szWindowClass = L"Test";
        WNDCLASSEX wcex={0};
        wcex.cbSize = sizeof(WNDCLASSEX);      
        wcex.style            = CS_HREDRAW | CS_VREDRAW;
        wcex.lpfnWndProc    = (WNDPROC)NotifyWndProc;      
        wcex.hInstance        = 0;     
        HBITMAP hbmp;
        hbmp = LoadBitmap((HINSTANCE)GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_NOTIFY_BG));
        wcex.hbrBackground    = (HBRUSH)CreatePatternBrush(hbmp); 
        wcex.lpszClassName    = szWindowClass.c_str();    
        wcex.hIcon            =0;     
        wcex.hCursor        =LoadCursor(NULL, IDC_ARROW);   
        wcex.lpszMenuName    =0;    
        wcex.hIconSm        =0;    
        RegisterClassEx(&wcex);
        hWnd = CreateWindow(szWindowClass.c_str(), szTitle.c_str(), WS_POPUP,
          (scrWidth - 205 ),(scrHeight - tbheight - 105), 200, 100, g_hMainWnd, NULL, 0, NULL);
```
2，设置button的背景图
```cpp
HWND hbtn = CreateWindow(TEXT("BUTTON"), TEXT(""), 
                                    WS_CHILD | WS_VISIBLE | BS_BITMAP | BS_FLAT , 
                                     180, 0, 19, 
                                    17, hWnd, 
                                     (HMENU) 0x8801, // BUTTON ID
                                     NULL, NULL );
            HBITMAP hbmp1;
            hbmp1 = LoadBitmap((HINSTANCE)GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_NOTIFY_CLOSEBTN));
            ::SendMessage(hbtn,   BM_SETIMAGE,   (WPARAM)IMAGE_BITMAP,   LPARAM(hbmp1));
```
一些细节要注意到，比如BUTTON中BS_BITMAP和SENDMESSAGE的 IMAGE_BITMAP的这种标记。
