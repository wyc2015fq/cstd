# 半透明AlphaBlend - xqhrs232的专栏 - CSDN博客
2010年07月15日 11:30:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1023标签：[null																[callback																[winapi																[application																[windows																[文档](https://so.csdn.net/so/search/s.do?q=文档&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)](https://so.csdn.net/so/search/s.do?q=application&t=blog)](https://so.csdn.net/so/search/s.do?q=winapi&t=blog)](https://so.csdn.net/so/search/s.do?q=callback&t=blog)](https://so.csdn.net/so/search/s.do?q=null&t=blog)
个人分类：[DirectUI界面技术](https://blog.csdn.net/xqhrs232/article/category/1243806)
原文地址::[http://blog.csdn.net/lsmdiao0812/archive/2008/11/18/3324640.aspx](http://blog.csdn.net/lsmdiao0812/archive/2008/11/18/3324640.aspx)
相关技术文档::[http://www.cnblogs.com/Lisen/archive/2010/01/13/1646517.html](http://www.cnblogs.com/Lisen/archive/2010/01/13/1646517.html)
1 AlphaBlend
    函数功能：该函数用来显示透明或半透明像素的位图。
    函数原型：AlphaBlend(HDC hdcDest,int nXOriginDest,int nYOriginDest,int nWidthDest,int hHeightDest,HDC hdcSrc,int nXOriginSrc,int nYOriginSrc,int nWidthSrc,int nHeightSrc,BLENDFUNCTION blendFunction)；
    参数：
    hdcDest：指向目标设备环境的句柄。
    nXoriginDest：指定目标矩形区域左上角的X轴坐标，按逻辑单位。
    nYOriginDest：指定目标矩形区域左上角的Y轴坐标，按逻辑单位。
    nWidthDest：指定目标矩形区域的宽度，按逻辑单位。
    hHeghtdest：指向目标矩形区域高度的句柄，按逻辑单位。
    hdcSrc：指向源设备环境的句柄。
    nXOriginSrc：指定源矩形区域左上角的X轴坐标，按逻辑单位。
    nYOriginSrc：指定源矩形区域左上角的Y轴坐标，按逻辑单位。
    nWidthSrc：指定源矩形区域的宽度，按逻辑单位。
    nHeightSrc：指定源矩形区域的高度，按逻辑单位。
    blendFunction：指定用于源位图和目标位图使用的alpha混合功能，用于整个源位图的全局alpha值和格式信息。源和目标混合功能当前只限为AC_SRC_OVER。
    返回值：如果函数执行成功，那么返回值为TRUE；如果函数执行失败，那么返回值为FALSE。
    Windows NT：若想获取更多错误信息，请调用GetLastError函数。
    备注：如果源矩形区域与目标矩形区域大小不一样，那么将缩放源位图与目标矩形区域匹配。如果使用SetStretchBltMode函数，那么iStretchMode的值是BLACKONWHITE和WHITEONBLACK，在本函数中，iStretchMode的值自动转换成COLORONCOLOR。目标坐标使用为目标设备环境当前指定的转换方式进行转换。源坐标则使用为源设备环境指定的当前转换方式进行转换。如果源设备环境标识为增强型图元文件设备环境，那么会出错（并且该函数返回FALSE）。如果目标和源位图的色彩格式不同，那么AlphaBlend将源位图转换以匹配目标位图。
    AlphaBlend不支持镜像。如果源或目标区域的宽度或高度为负数，那么调用将失败。
注意添加 #pragma   comment(lib,"Msimg32.lib")库
下面是一个例子：
#include <windows.h> 
#pragma   comment(lib,"Msimg32.lib") 
HINSTANCE g_hIns; 
HWND g_hWnd; 
LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM); 
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int iCmdShow)
{ 
    g_hIns=hInstance; 
    static TCHAR szAppName[]=TEXT("HelloWin"); 
    HWND hWnd; 
    MSG msg; 
    WNDCLASS wndClass; 
    wndClass.style=CS_HREDRAW|CS_VREDRAW; 
    wndClass.lpfnWndProc=WndProc; 
    wndClass.cbClsExtra=0; 
    wndClass.cbWndExtra=0; 
    wndClass.hInstance=hInstance; 
    wndClass.hIcon=LoadIcon(NULL,IDI_APPLICATION); 
    wndClass.hCursor=LoadCursor(NULL,IDC_ARROW); 
    wndClass.hbrBackground=(HBRUSH)GetStockObject(WHITE_BRUSH); 
    wndClass.lpszMenuName=NULL; 
    wndClass.lpszClassName=szAppName; 
    if(!RegisterClass(&wndClass)) 
    { 
        MessageBox(NULL,TEXT("error"),szAppName,MB_ICONERROR|MB_OK); 
        return 0; 
    } 
    hWnd=CreateWindow(szAppName,TEXT("The hello program"),WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,NULL,NULL,hInstance,NULL);
    int iReturn; 
    //也可以放在此 
    ShowWindow(hWnd,iCmdShow); 
    UpdateWindow(hWnd); 
    while(GetMessage(&msg,hWnd,0,0)) 
    { 
        TranslateMessage(&msg); 
        DispatchMessage(&msg); 
    } 
    return msg.wParam; 
} 
LRESULT CALLBACK WndProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam) 
{ 
    static HDC hdc; 
    static HDC hBitmapDC; 
    static HBITMAP hBitmap; 
    static int w=50; 
    static int h=50; 
    static BLENDFUNCTION bf; 
    PAINTSTRUCT ps; 
    switch (message) 
    { 
    case   WM_PAINT : 
        hdc=BeginPaint(hWnd,&ps); 
        hBitmapDC = CreateCompatibleDC(hdc); 
        hBitmap = CreateCompatibleBitmap(hdc,w,h); 
        SelectObject(hBitmapDC,hBitmap); 
        SetTextColor(hBitmapDC,RGB(0,255,0)); 
        TextOut(hBitmapDC,0,0,TEXT("asdf"),4); 
        bf.BlendOp = AC_SRC_OVER; 
        bf.BlendFlags = 0; 
        bf.AlphaFormat = 0; 
        bf.SourceConstantAlpha = 50; 
        AlphaBlend(hdc,0,0,w,h,hBitmapDC,0,0,w,h,bf); 
        EndPaint (hWnd, &ps) ; 
        break; 
    case WM_DESTROY : 
        PostQuitMessage (0) ; 
        break ; 
    default: 
        return DefWindowProc (hWnd, message, wParam, lParam) ; 
    } 
    return 1; 
}
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/lsmdiao0812/archive/2008/11/18/3324640.aspx](http://blog.csdn.net/lsmdiao0812/archive/2008/11/18/3324640.aspx)

