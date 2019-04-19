# WinCE5.0下直接写屏操作与函数 CreateDIBSection() 的使用 - xqhrs232的专栏 - CSDN博客
2012年11月03日 23:14:28[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：747
个人分类：[DirectUI界面技术](https://blog.csdn.net/xqhrs232/article/category/1243806)
原文地址::[http://blog.csdn.net/91program/article/details/5144060](http://blog.csdn.net/91program/article/details/5144060)
相关网帖
1.请教WINCE保存为BMP文件的问题----[http://bbs.csdn.net/topics/310113591](http://bbs.csdn.net/topics/310113591)
# 请教WINCE保存为BMP文件的问题！
这段时间在研究直接写屏的方法.
此问题的起因是: 在 iMX31(WinCE 5.0)上, 屏的分辨为: 800*480. 调用 BitBlt() API 函数画一整屏的动作, 竟然需要 120ms 左右; 同样的程序, 在 Prima 上只需要 20ms 以内的时间. 
直接写屏, 需要用到以下两个主要的函数: CreateDC(TEXT("DISPLAY"),......) 和 CreateDIBSection().
1) CreateDC(TEXT("DISPLAY"),......) 的用法比较简单, 参考 MSDN 就可以很容易的搞定.
2) CreateDIBSection() API 函数的使用, 花了我一些时间. 因为以前没有用过它, 所以将其使用的方法记录如下:
  {
    HBITMAP hDIBitmap;
    LPBYTE lpBitmapBits;
    BITMAPINFO DriectBmp; 
    ZeroMemory(&DriectBmp,sizeof(BITMAPINFO));
    DriectBmp.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
    DriectBmp.bmiHeader.biWidth = 800;
    DriectBmp.bmiHeader.biHeight = 480;
    DriectBmp.bmiHeader.biPlanes = 1;
    DriectBmp.bmiHeader.biBitCount = 16;
    HDC dcDirect = CreateCompatibleDC(dcBuf);
    // 建立 hDIBitmap, dcDirect, 和 lpBitmapBits 之间的关系.
    // 通过 DC 操作向 dcDirect 中写入图形数据, 从 lpBitmapBits 中取得内存数据, 进行内存 Copy 操作, 直接写向显存.
    hDIBitmap = CreateDIBSection(dcDirect, (BITMAPINFO *)&DriectBmp, 
      DIB_RGB_COLORS, (void **)&lpBitmapBits, NULL, 0);
    if(NULL != hDIBitmap)
    {
      HGDIOBJ OldBitmap = SelectObject(dcDirect, hDIBitmap);
      BitBlt(dcDirect,0,0,800,480,dcBuf,0,0,SRCCOPY);
      CopyMemory(gpbFrameAddress,lpBitmapBits,800 * 480 * 2);
      SelectObject(dcDirect,OldBitmap);
    }
    DeleteObject(hDIBitmap);
    DeleteDC(dcDirect);
  }
在研究此问题的过程中, 在 CSDN 上找到一个很好的资源, 也记录在自己这里:
http://topic.csdn.net/u/20090629/09/00b25d0d-d461-47dc-88ad-0e2131a5abc7.html
并转载其中关键的代码如下:
#include "stdafx.h"
#include "CopyScreen.h"
#include <windows.h>
#include <commctrl.h>
#include"Jpeg.h"
#define MAX_LOADSTRING 100
// 全局变量:
HINSTANCE            g_hInst;            // 当前实例
HWND                g_hWndMenuBar;        // 菜单栏句柄
// 此代码模块中包含的函数的前向声明:
ATOM            MyRegisterClass(HINSTANCE, LPTSTR);
BOOL            InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPTSTR    lpCmdLine,
                   int       nCmdShow)
{
    MSG msg;
    // 执行应用程序初始化:
    if (!InitInstance(hInstance, nCmdShow)) 
    {
        return FALSE;
    }
    HACCEL hAccelTable;
    hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_COPYSCREEN));
    // 主消息循环:
    while (GetMessage(&msg, NULL, 0, 0)) 
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    return (int) msg.wParam;
}
//
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
//  注释:
//
ATOM MyRegisterClass(HINSTANCE hInstance, LPTSTR szWindowClass)
{
    WNDCLASS wc;
    wc.style         = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc   = WndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInstance;
    wc.hIcon         = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_COPYSCREEN));
    wc.hCursor       = 0;
    wc.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName  = 0;
    wc.lpszClassName = szWindowClass;
    return RegisterClass(&wc);
}
//
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    HWND hWnd;
    TCHAR szTitle[MAX_LOADSTRING];        // 标题栏文本
    TCHAR szWindowClass[MAX_LOADSTRING];    // 主窗口类名
    g_hInst = hInstance; // 将实例句柄存储在全局变量中
    // 在应用程序初始化期间，应调用一次 SHInitExtraControls 以初始化
    // 所有设备特定控件，例如，CAPEDIT 和 SIPPREF。
    SHInitExtraControls();
    LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING); 
    LoadString(hInstance, IDC_COPYSCREEN, szWindowClass, MAX_LOADSTRING);
    //如果它已经在运行，则将焦点置于窗口上，然后退出
    hWnd = FindWindow(szWindowClass, szTitle);    
    if (hWnd) 
    {
        // 将焦点置于最前面的子窗口
        // “| 0x00000001”用于将所有附属窗口置于前台并
        // 激活这些窗口。
        SetForegroundWindow((HWND)((ULONG) hWnd | 0x00000001));
        return 0;
    } 
    if (!MyRegisterClass(hInstance, szWindowClass))
    {
        return FALSE;
    }
    hWnd = CreateWindow(szWindowClass, szTitle, WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
    if (!hWnd)
    {
        return FALSE;
    }
    // 使用 CW_USEDEFAULT 创建主窗口时，将不会考虑菜单栏的高度(如果创建了一个
    // 菜单栏)。因此，我们要在创建窗口后调整其大小
    // 如果菜单栏存在
    if (g_hWndMenuBar)
    {
        RECT rc;
        RECT rcMenuBar;
        GetWindowRect(hWnd, &rc);
        GetWindowRect(g_hWndMenuBar, &rcMenuBar);
        rc.bottom -= (rcMenuBar.bottom - rcMenuBar.top);
        MoveWindow(hWnd, rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top, FALSE);
    }
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    return TRUE;
}
HBITMAP CopyScreenToBitmap(int &nWidth,int &nHeight)
{
    // 屏幕和内存设备描述表
    HDC  hScrDC, hMemDC;      
    // 位图句柄
    HBITMAP  hBitmap, hOldBitmap;    
    // 屏幕分辨率
    int  xScrn, yScrn;         
    //为屏幕创建设备描述表
    hScrDC = CreateDC(_T("DISPLAY"), NULL, NULL, NULL);
    //为屏幕设备描述表创建兼容的内存设备描述表
    hMemDC = CreateCompatibleDC(hScrDC);
    // 获得屏幕分辨率
    xScrn = GetDeviceCaps(hScrDC, HORZRES);
    yScrn = GetDeviceCaps(hScrDC, VERTRES);
    //存储屏幕的长宽
    nWidth = xScrn;
    nHeight = yScrn;
    // 创建一个与屏幕设备描述表兼容的位图
    hBitmap = CreateCompatibleBitmap(hScrDC, xScrn, yScrn);
    // 把新位图选到内存设备描述表中
    hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);
    // 把屏幕设备描述表拷贝到内存设备描述表中
    BitBlt(hMemDC, 0, 0, xScrn,yScrn,hScrDC, 0, 0, SRCCOPY);
    //得到屏幕位图的句柄
    hBitmap = (HBITMAP)SelectObject(hMemDC, hOldBitmap);
    //清除 
    DeleteDC(hScrDC);
    DeleteDC(hMemDC);
    // 返回位图句柄
    return hBitmap;
}
int Save16BitmapToFile(HBITMAP hBitmap,LPCTSTR lpFileName)//将截屏所得保存为16位的图片
{
    HDC hDC=CreateDC(TEXT("DISPLAY"),NULL,NULL,NULL);
    HDC hOffDC=CreateCompatibleDC(hDC);
    SelectObject(hOffDC,hBitmap);
    BITMAP Bitmap;
    GetObject(hBitmap,sizeof(BITMAP),&Bitmap);
    HANDLE fh=CreateFile(lpFileName,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN,NULL);
    if(fh == INVALID_HANDLE_VALUE )
        return FALSE;
    BITMAPFILEHEADER bfh;
    memset(&bfh,0,sizeof(bfh));
    bfh.bfType=0x4D42/*((WORD) ('M' << 8) | 'B')*/;
    bfh.bfSize= sizeof(bfh)+2*Bitmap.bmWidth*Bitmap.bmHeight+sizeof(BITMAPFILEHEADER);
    bfh.bfOffBits=sizeof(BITMAPINFOHEADER)+sizeof(BITMAPFILEHEADER);
    DWORD dwWritten=0;
    WriteFile(fh,&bfh,sizeof(bfh),&dwWritten,NULL);
    BITMAPINFOHEADER bih;
    memset(&bih,0,sizeof(bih));
    bih.biSize=sizeof(bih);
    bih.biWidth=Bitmap.bmWidth;
    bih.biHeight=Bitmap.bmHeight;
    bih.biPlanes=1;
    bih.biBitCount=16;
    if( !WriteFile(fh,&bih,sizeof(bih),&dwWritten,NULL) )
    {
        return FALSE;
    }
    BITMAPINFO bitmapInfo;
    memset((void *)&bitmapInfo,0,sizeof(BITMAPINFO) );
    bitmapInfo.bmiHeader=bih;
    HDC hMemDC=CreateCompatibleDC(hDC);    
    BYTE *m_lpBitBmp=new BYTE[bfh.bfSize-sizeof(BITMAPFILEHEADER)];
    HBITMAP hDibBitmap=CreateDIBSection(hMemDC,&bitmapInfo,DIB_RGB_COLORS,(void **)&m_lpBitBmp,
        NULL,0);
    if(hDibBitmap != 0)
    {
        ::SelectObject(hMemDC,hDibBitmap);
        BitBlt(hMemDC,0,0,Bitmap.bmWidth,Bitmap.bmHeight,hOffDC,0,0,SRCCOPY);
        WriteFile(fh,m_lpBitBmp,bfh.bfSize-sizeof(BITMAPFILEHEADER),&dwWritten,NULL);
    }
    DeleteObject(hDibBitmap);
    DeleteDC(hDC);
    DeleteDC(hMemDC);
    CloseHandle(fh);
    return 1;
}
int Save24BitmapToFile(HBITMAP hBitmap,LPCTSTR lpFileName)//将截屏所得保存为24位的图片
{
    HDC hDC=CreateDC(TEXT("DISPLAY"),NULL,NULL,NULL);
    HDC hOffDC=CreateCompatibleDC(hDC);
    SelectObject(hOffDC,hBitmap);
    BITMAP Bitmap;
    GetObject(hBitmap,sizeof(BITMAP),&Bitmap);
    HANDLE fh=CreateFile(lpFileName,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN,NULL);
    if(fh == INVALID_HANDLE_VALUE )
        return FALSE;
    BITMAPFILEHEADER bfh;
    memset(&bfh,0,sizeof(bfh));
    bfh.bfType=0x4D42/*((WORD) ('M' << 8) | 'B')*/;
    bfh.bfSize= sizeof(bfh)+3*Bitmap.bmWidth*Bitmap.bmHeight+sizeof(BITMAPFILEHEADER);
    bfh.bfOffBits=sizeof(BITMAPINFOHEADER)+sizeof(BITMAPFILEHEADER);
    DWORD dwWritten=0;
    WriteFile(fh,&bfh,sizeof(bfh),&dwWritten,NULL);
    BITMAPINFOHEADER bih;
    memset(&bih,0,sizeof(bih));
    bih.biSize=sizeof(bih);
    bih.biWidth=Bitmap.bmWidth;
    bih.biHeight=Bitmap.bmHeight;
    bih.biPlanes=1;
    bih.biBitCount=24;
    if( !WriteFile(fh,&bih,sizeof(bih),&dwWritten,NULL) )
    {
        return FALSE;
    }
    BITMAPINFO bitmapInfo;
    memset((void *)&bitmapInfo,0,sizeof(BITMAPINFO) );
    bitmapInfo.bmiHeader=bih;
    HDC hMemDC=CreateCompatibleDC(hDC);    
    BYTE *m_lpBitBmp=new BYTE[3*Bitmap.bmWidth*Bitmap.bmHeight];
    HBITMAP hDibBitmap=CreateDIBSection(hMemDC,&bitmapInfo,DIB_RGB_COLORS,(void **)&m_lpBitBmp,
        NULL,0);
    if(hDibBitmap != 0)
    {
        ::SelectObject(hMemDC,hDibBitmap);
        BitBlt(hMemDC,0,0,Bitmap.bmWidth,Bitmap.bmHeight,hOffDC,0,0,SRCCOPY);
        WriteFile(fh,m_lpBitBmp,3*Bitmap.bmWidth*Bitmap.bmHeight,&dwWritten,NULL);
    }
    DeleteObject(hDibBitmap);
    DeleteDC(hDC);
    DeleteDC(hMemDC);
    CloseHandle(fh);
    return 1;
}
/*索引颜色/颜色表 
位图常用的一种压缩方法。从位图图片中选择最有代表性的若干种颜色（通常不超过256种）编制成颜色表，
然后将图片中原有颜色用颜色表的索引来表示。
这样原图片可以被大幅度有损压缩。适合于压缩网页图形等颜色数较少的图形，
不适合压缩照片等色彩丰富的图形。 */
int Save32BitmapToFile(HBITMAP hBitmap,LPCTSTR lpFileName)//将截屏所得保存为32位带颜色表的图片
{
    /*若不使用颜色表，则去掉bih.biCompression=BI_BITFIELDS;这句，然后
    不设置bitmapInfo.bmiColors[0].rgbRed=255;
    bitmapInfo.bmiColors[0].rgbGreen=255;
    bitmapInfo.bmiColors[0].rgbBlue=255;*/
    HDC hDC=CreateDC(TEXT("DISPLAY"),NULL,NULL,NULL);
    HDC hOffDC=CreateCompatibleDC(hDC);
    SelectObject(hOffDC,hBitmap);
    BITMAP Bitmap;
    GetObject(hBitmap,sizeof(BITMAP),&Bitmap);
    HANDLE fh=CreateFile(lpFileName,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN,NULL);
    if(fh == INVALID_HANDLE_VALUE )
        return FALSE;
    BITMAPFILEHEADER bfh;
    memset(&bfh,0,sizeof(bfh));
    bfh.bfType=0x4D42/*((WORD) ('M' << 8) | 'B')*/;
    bfh.bfSize= sizeof(bfh)+4*Bitmap.bmWidth*Bitmap.bmHeight+sizeof(BITMAPFILEHEADER);
    bfh.bfOffBits=sizeof(BITMAPINFOHEADER)+sizeof(BITMAPFILEHEADER);
    DWORD dwWritten=0;
    WriteFile(fh,&bfh,sizeof(bfh),&dwWritten,NULL);
    BITMAPINFOHEADER bih;
    memset(&bih,0,sizeof(bih));
    bih.biSize=sizeof(bih);
    bih.biWidth=Bitmap.bmWidth;
    bih.biHeight=Bitmap.bmHeight;
    bih.biPlanes=1;
    bih.biBitCount=32;
    bih.biCompression=BI_BITFIELDS;
    if( !WriteFile(fh,&bih,sizeof(bih),&dwWritten,NULL) )
    {
        return FALSE;
    }
    BITMAPINFO bitmapInfo;
    memset((void *)&bitmapInfo,0,sizeof(BITMAPINFO) );
    bitmapInfo.bmiHeader=bih;
    bitmapInfo.bmiColors[0].rgbRed=255;
    bitmapInfo.bmiColors[0].rgbGreen=255;
    bitmapInfo.bmiColors[0].rgbBlue=255;
    HDC hMemDC=CreateCompatibleDC(hDC);    
    BYTE *m_lpBitBmp=new BYTE[4*Bitmap.bmWidth*Bitmap.bmHeight];
    HBITMAP hDibBitmap=CreateDIBSection(hMemDC,&bitmapInfo,DIB_RGB_COLORS,(void **)&m_lpBitBmp,
        NULL,0);
    if(hDibBitmap != 0)
    {
        ::SelectObject(hMemDC,hDibBitmap);
        BitBlt(hMemDC,0,0,Bitmap.bmWidth,Bitmap.bmHeight,hOffDC,0,0,SRCCOPY);
        WriteFile(fh,m_lpBitBmp,4*Bitmap.bmWidth*Bitmap.bmHeight,&dwWritten,NULL);
    }
    DeleteObject(hDibBitmap);
    DeleteDC(hDC);
    DeleteDC(hMemDC);
    CloseHandle(fh);
    return 1;
}
int SaveJPEGToFile(HBITMAP hBitmap,LPCSTR lpFileName)//将截屏所得保存为jpeg图片
{
    HDC hDC=CreateDC(TEXT("DISPLAY"),NULL,NULL,NULL);
    HDC hOffDC=CreateCompatibleDC(hDC);
    SelectObject(hOffDC,hBitmap);
    BITMAP Bitmap;
    GetObject(hBitmap,sizeof(BITMAP),&Bitmap);
    HANDLE fh=CreateFile((LPCWSTR)lpFileName,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN,NULL);
    if(fh == INVALID_HANDLE_VALUE )
        return FALSE;
    BITMAPFILEHEADER bfh;
    memset(&bfh,0,sizeof(bfh));
    bfh.bfType=0x4D42/*((WORD) ('M' << 8) | 'B')*/;
    bfh.bfSize= sizeof(bfh)+3*Bitmap.bmWidth*Bitmap.bmHeight+sizeof(BITMAPFILEHEADER);
    bfh.bfOffBits=sizeof(BITMAPINFOHEADER)+sizeof(BITMAPFILEHEADER);
    DWORD dwWritten=0;
//    WriteFile(fh,&bfh,sizeof(bfh),&dwWritten,NULL);
    BITMAPINFOHEADER bih;
    memset(&bih,0,sizeof(bih));
    bih.biSize=sizeof(bih);
    bih.biWidth=Bitmap.bmWidth;
    bih.biHeight=Bitmap.bmHeight;
    bih.biPlanes=1;
    bih.biBitCount=24;
    /*if( !WriteFile(fh,&bih,sizeof(bih),&dwWritten,NULL) )
    {
        return FALSE;
    }*/
    BITMAPINFO bitmapInfo;
    memset((void *)&bitmapInfo,0,sizeof(BITMAPINFO) );
    bitmapInfo.bmiHeader=bih;
    HDC hMemDC=CreateCompatibleDC(hDC);    
    BYTE *m_lpBitBmp=new BYTE[bfh.bfSize-sizeof(BITMAPFILEHEADER)];
    HBITMAP hDibBitmap=CreateDIBSection(hMemDC,&bitmapInfo,DIB_RGB_COLORS,(void **)&m_lpBitBmp,
        NULL,0);
    if(hDibBitmap != 0)
    {
        ::SelectObject(hMemDC,hDibBitmap);
        BitBlt(hMemDC,0,0,Bitmap.bmWidth,Bitmap.bmHeight,hOffDC,0,0,SRCCOPY);
        //WriteFile(fh,m_lpBitBmp,bfh.bfSize-sizeof(BITMAPFILEHEADER),&dwWritten,NULL);
         WriteJPEGFile(lpFileName, m_lpBitBmp,Bitmap.bmWidth,Bitmap.bmHeight,TRUE, 60);
    }
    DeleteObject(hDibBitmap);
    DeleteDC(hDC);
    DeleteDC(hMemDC);
    CloseHandle(fh);
    return 1;
}
//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的: 处理主窗口的消息。
//
//  WM_COMMAND    - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY    - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    int wmId, wmEvent;
    PAINTSTRUCT ps;
    HDC hdc;
    static SHACTIVATEINFO s_sai;
    int iWidth,iHeight;
    HBITMAP hBitmap;
    static int s_cxBegin;
    static int s_cyBegin;
    static int s_cxEnd;
    static int s_cyEnd;
    switch (message) 
    {
        case WM_COMMAND:
            wmId    = LOWORD(wParam); 
            wmEvent = HIWORD(wParam); 
            // 分析菜单选择:
            switch (wmId)
            {
                case IDM_HELP_ABOUT:
                    DialogBox(g_hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, About);
                    break;
                case IDM_OK:
                    SendMessage (hWnd, WM_CLOSE, 0, 0);                
                    break;
                case ID_RGB16:
                    hBitmap=CopyScreenToBitmap(iWidth,iHeight);
                    Save16BitmapToFile(hBitmap,TEXT("//a.bmp"));
                    break;
                case ID_RGB24:
                    hBitmap=CopyScreenToBitmap(iWidth,iHeight);
                    Save24BitmapToFile(hBitmap,TEXT("//b.bmp"));
                    break;
                case ID_RGB32:
                    hBitmap=CopyScreenToBitmap(iWidth,iHeight);
                    Save32BitmapToFile(hBitmap,TEXT("//c.bmp"));
                    break;
                case ID_JPEG:
                    hBitmap=CopyScreenToBitmap(iWidth,iHeight);
                    SaveJPEGToFile(hBitmap,"d.jpg");
                    break;
                default:
                    return DefWindowProc(hWnd, message, wParam, lParam);
            }
            break;
        case WM_CREATE:
            SHMENUBARINFO mbi;
            memset(&mbi, 0, sizeof(SHMENUBARINFO));
            mbi.cbSize     = sizeof(SHMENUBARINFO);
            mbi.hwndParent = hWnd;
            mbi.nToolBarId = IDR_MENU;
            mbi.hInstRes   = g_hInst;
            if (!SHCreateMenuBar(&mbi)) 
            {
                g_hWndMenuBar = NULL;
            }
            else
            {
                g_hWndMenuBar = mbi.hwndMB;
            }
            // 初始化外壳程序激活信息结构
            memset(&s_sai, 0, sizeof (s_sai));
            s_sai.cbSize = sizeof (s_sai);
            break;
        case WM_LBUTTONDOWN:
            HCURSOR  hCursor;
            hCursor=SetCursor( LoadCursor(NULL,MAKEINTRESOURCE(IDC_WAIT) ) );
            s_cxBegin=LOWORD(lParam);
            s_cyBegin=HIWORD(lParam);
            break;
        case WM_LBUTTONUP:
            s_cxEnd=LOWORD(lParam);
            s_cyEnd=HIWORD(lParam);
            SetCursor( LoadCursor( GetModuleHandle(NULL),MAKEINTRESOURCE(IDC_ARROW) ) );
            break;
        case WM_PAINT:
            hdc = BeginPaint(hWnd, &ps);
            // TODO: 在此添加任意绘图代码...
            EndPaint(hWnd, &ps);
            break;
        case WM_DESTROY:
            CommandBar_Destroy(g_hWndMenuBar);
            PostQuitMessage(0);
            break;
        case WM_ACTIVATE:
            // 向外壳程序通知我们的激活消息
            SHHandleWMActivate(hWnd, wParam, lParam, &s_sai, FALSE);
            break;
        case WM_SETTINGCHANGE:
            SHHandleWMSettingChange(hWnd, wParam, lParam, &s_sai);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
