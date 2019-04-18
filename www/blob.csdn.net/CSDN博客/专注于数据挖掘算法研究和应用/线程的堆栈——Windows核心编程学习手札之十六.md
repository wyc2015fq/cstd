# 线程的堆栈——Windows核心编程学习手札之十六 - 专注于数据挖掘算法研究和应用 - CSDN博客





2009年07月23日 17:17:00[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2794








# 线程的堆栈

## ——Windows核心编程学习手札之十六

系统会在进程的地址空间内保存一些区域，同时也会在进程地址空间内为线程的堆栈保留区域。线程都有自己的堆栈，创建时，系统就保留一个堆栈空间区域，并将相应物理存储器提交给这个保留的区域用于线程堆栈。按照默认设置，系统保留1MB的地址空间并提交两个页面的内存，默认值可以在链接应用程序时设定Microsoft的链接程序的/STACK选项：

/STACK:reserve[,commit]

当创建一个线程的堆栈时，系统就会保留一个链接程序/STACK开关指明的地址空间区域。在调用CreateThread或_beginthreadx函数时，可以重载原先提交的内存数量，这两个函数都有一个参数，可以用来重载原先提交给堆栈地址空间的内存数量，如果设定这个参数为0，那么系统将使用/STACK开关指明的已提交的堆栈大小值。

线程堆栈溢栈例子，超过1MB分配空间，SHE提示出错：

// Demo.cpp : Defines the entry point for the application.

//



#include "stdafx.h"

#include "resource.h"

#include<process.h>



#define MAX_LOADSTRING 100

#define UINT_MAX4

// Global Variables:

HINSTANCE hInst;// current instance

TCHAR szTitle[MAX_LOADSTRING];// The title bar text

TCHAR szWindowClass[MAX_LOADSTRING];// The title bar text



// Foward declarations of functions included in this code module:

ATOMMyRegisterClass(HINSTANCE hInstance);

BOOLInitInstance(HINSTANCE, int);

LRESULT CALLBACKWndProc(HWND, UINT, WPARAM, LPARAM);

LRESULT CALLBACKAbout(HWND, UINT, WPARAM, LPARAM);



//Function:Sum

//An example of calling Sum for uNum=0 through 9

//uNum:0 1 2 3 456789 ...

//Sum: 0 1 3 6 10 15 21 28 36 45 ...

UINT Sum(UINT nNum)

{

// call Sum recursively.

return ( (nNum ==0)?0:(nNum+Sum(nNum-1)) );

}

//end of Sum



//Function:FilterFunc

LONG WINAPI FilterFunc(DWORD dwExceptionCode)

{

return ((dwExceptionCode==STATUS_STACK_OVERFLOW)?

EXCEPTION_EXECUTE_HANDLER:EXCEPTION_CONTINUE_SEARCH);

}

//end of FilterFunc



//Function:SumThreadFunc

//the separate thread that is responsible for calculating the sum.

unsigned intWINAPI SumThreadFunc(PVOID pvParam)

{

//the parameter pvParam,contains the number of integers to sum.

UINT uSumNum=PtrToUlong(pvParam);

//uSum contains the summation of the numbers from 0 through uSumNum.

//if the sum cannot be calculated,a sum of UINT_MAX is returned.

UINT uSum=UINT_MAX;

__try{

uSum=Sum(uSumNum);

}

__except(FilterFunc(GetExceptionCode())){

}

return(uSum);

}

//end of SumThreadFunc

int APIENTRY WinMain(HINSTANCE hInstance,

HINSTANCE hPrevInstance,

LPSTRlpCmdLine,

intnCmdShow)

{

// TODO: Place code here.

MSG msg;

HACCEL hAccelTable;



// Initialize global strings

LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);

LoadString(hInstance, IDC_DEMO, szWindowClass, MAX_LOADSTRING);

MyRegisterClass(hInstance);



// Perform application initialization:

if (!InitInstance (hInstance, nCmdShow)) 

{

return FALSE;

}



hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_DEMO);



// Main message loop:

while (GetMessage(&msg, NULL, 0, 0)) 

{

if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 

{

TranslateMessage(&msg);

DispatchMessage(&msg);

}

}



return msg.wParam;

}







//

//FUNCTION: MyRegisterClass()

//

//PURPOSE: Registers the window class.

//

//COMMENTS:

//

//This function and its usage is only necessary if you want this code

//to be compatible with Win32 systems prior to the 'RegisterClassEx'

//function that was added to Windows 95. It is important to call this function

//so that the application will get 'well formed' small icons associated

//with it.

//

ATOM MyRegisterClass(HINSTANCE hInstance)

{

WNDCLASSEX wcex;



wcex.cbSize = sizeof(WNDCLASSEX); 



wcex.style= CS_HREDRAW | CS_VREDRAW;

wcex.lpfnWndProc= (WNDPROC)WndProc;

wcex.cbClsExtra= 0;

wcex.cbWndExtra= 0;

wcex.hInstance= hInstance;

wcex.hIcon= LoadIcon(hInstance, (LPCTSTR)IDI_DEMO);

wcex.hCursor= LoadCursor(NULL, IDC_ARROW);

wcex.hbrBackground= (HBRUSH)(COLOR_WINDOW+1);

wcex.lpszMenuName= (LPCSTR)IDC_DEMO;

wcex.lpszClassName= szWindowClass;

wcex.hIconSm= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);



return RegisterClassEx(&wcex);

}



//

//FUNCTION: InitInstance(HANDLE, int)

//

//PURPOSE: Saves instance handle and creates main window

//

//COMMENTS:

//

//In this function, we save the instance handle in a global variable and

//create and display the main program window.

//

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)

{

HWND hWnd;



hInst = hInstance; // Store instance handle in our global variable



hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,

CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);



if (!hWnd)

{

return FALSE;

}



ShowWindow(hWnd, nCmdShow);

UpdateWindow(hWnd);



return TRUE;

}



//

//FUNCTION: WndProc(HWND, unsigned, WORD, LONG)

//

//PURPOSE:Processes messages for the main window.

//

//WM_COMMAND- process the application menu

//WM_PAINT- Paint the main window

//WM_DESTROY- post a quit message and return

//

//

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)

{

int wmId, wmEvent;

PAINTSTRUCT ps;

HDC hdc;

TCHAR szHello[MAX_LOADSTRING];

LoadString(hInst, IDS_HELLO, szHello, MAX_LOADSTRING);



switch (message) 

{

case WM_COMMAND:

wmId= LOWORD(wParam); 

wmEvent = HIWORD(wParam); 

// Parse the menu selections:

switch (wmId)

{

case IDM_CALC:

{

UINT uSum=44000;

unsigned dwThreadId;

HANDLE hThread=(HANDLE)_beginthreadex(NULL,0,SumThreadFunc,(PVOID)(UINT_PTR)uSum,0,&dwThreadId);

//wait for the thread to terminate.

WaitForSingleObject(hThread,INFINITE);

//the thread's exit code is the resulting summation.

GetExitCodeThread(hThread,(PDWORD)&uSum);

CloseHandle(hThread);

if(uSum==UINT_MAX)

MessageBox(hWnd,"error","warning",0);

else

MessageBox(hWnd,"Success","warning",0);

}

break;

case IDM_ABOUT:

DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);

break;

case IDM_EXIT:

DestroyWindow(hWnd);

break;

default:

return DefWindowProc(hWnd, message, wParam, lParam);

}

break;

case WM_PAINT:

hdc = BeginPaint(hWnd, &ps);

// TODO: Add any drawing code here...

RECT rt;

GetClientRect(hWnd, &rt);

DrawText(hdc, szHello, strlen(szHello), &rt, DT_CENTER);

EndPaint(hWnd, &ps);

break;

case WM_DESTROY:

PostQuitMessage(0);

break;

default:

return DefWindowProc(hWnd, message, wParam, lParam);

}

return 0;

}



// Mesage handler for about box.

LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)

{

switch (message)

{

case WM_INITDIALOG:

return TRUE;



case WM_COMMAND:

if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 

{

EndDialog(hDlg, LOWORD(wParam));

return TRUE;

}

break;

}

return FALSE;

}





