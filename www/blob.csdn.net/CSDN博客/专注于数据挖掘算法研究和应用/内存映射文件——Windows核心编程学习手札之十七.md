# 内存映射文件——Windows核心编程学习手札之十七 - 专注于数据挖掘算法研究和应用 - CSDN博客





2009年09月04日 10:20:00[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2569








# 内存映射文件

## ——Windows核心编程学习手札之十七

与虚拟内存一样，内存映射文件保留地址空间，并将物理存储器提交给该区域，差别在于所提交的物理存储器是磁盘上有文件存在的空间，而非系统的页文件，一旦文件被映射，就可以访问它，如同整个文件被加载到内存一样。内存映射文件用于三个不同目的：

1）系统使用内存映射文件，以便加载和执行.exe和DLL文件，可以节省页文件空间和应用程序启动运行所需的时间；

2）可以使用内存映射文件来访问磁盘上的数据文件，可以不必对文件执行I/O操作，并且可以不用对文件内容进行缓存；

3）使用内存映射文件，使同一台计算机上运行的多个进程能够相互之间共享数据。

当线程调用CreateProcess时，系统会执行下列步骤：

1）系统找出在调用CreateProcess时设定的.exe文件，如找不到这个文件，进程无法创建，CreateProcess将返回FALSE；

2）系统创建一个新进程内核对象；

3）系统为新进程创建一个私有地址空间；

4）系统保留一个足够大的地址空间区域，用于存放.exe文件，按照默认设置，.exe文件的基地址是0X00400000，也可以在创建应用程序的.exe文件时重载这个地址，方法是在链接应用程序时使用链接程序的/BASE选项。

5）系统知道已保留区域的物理存储器是在磁盘上的.exe文件，而不是在系统的页文件；

当.exe文件被映射到进程的地址空间后，系统将访问.exe文件一个部分，该部分列出了包含.exe文件中的代码要调用的函数的DLL文件，进而系统为每个DLL文件调用LoadLibrary函数。通过LoadLibrary函数加载DLL，系统有如下步骤：

1）系统保留一个足够大的地址空间区域，用于存放DLL文件，区域位置由DLL文件本身决定，默认设置，Microsoft的Visual C++建立的DLL文件基地址是0X10000000，不过，你可以在创建DLL文件重载这个地址，方法是使用链接程序的/BASE选项。Windows提供的所有标准系统DLL都拥有不同的基地址，这样，如果加载到单个地址空间，区域不会重叠；

2）系统可能因为区域为其他.exe或DLL文件占用和区域不够大的原因，无法在该DLL的首选基地址；

3）系统知道支持已保留区域的物理存储器位于磁盘上的DLL文件中，而不是在系统的页文件中；

如果由于某种原因，系统无法映射.exe和所有必要的DLL文件，那系统会通过消息框提示，并且释放进程的地址空间和进程对象。CreateProcess函数将返回FALSE，可通过GetLastError函数了解失败原因。当.exe文件和所有必要DLL文件都成功映射到进程的地址空间后，系统开始执行.exe文件的启动代码，同时，系统将负责所有的分页、缓冲和高速缓存的处理，如.exe文件中的代码需要去访问尚未加载到内存的指令地址，将出现错误，系统发现错误后，将自动将该页代码从该文件的映象加载到一个RAM页面，之后系统将这个RAM页面映射到进程的地址空间中的相应位置，并且让线程继续运行。

知道应用程序实例数代码：

// AppInst.cpp : Defines the entry point for the application.

//



#include "stdafx.h"

#include "resource.h"



#define MAX_LOADSTRING 100



// Global Variables:

HINSTANCE hInst;// current instance

TCHAR szTitle[MAX_LOADSTRING];// The title bar text

TCHAR szWindowClass[MAX_LOADSTRING];// The title bar text



// Foward declarations of functions included in this code module:

ATOMMyRegisterClass(HINSTANCE hInstance);

BOOLInitInstance(HINSTANCE, int);

LRESULT CALLBACKWndProc(HWND, UINT, WPARAM, LPARAM);

LRESULT CALLBACKAbout(HWND, UINT, WPARAM, LPARAM);



//define here for demo

UINT g_uMsgAppInstCountUpdate=INVALID_ATOM;//the system-wide unique window message

//==========================================

//tell the compiler to put this initialized variable in tis own shared section 

//so it is shared by all instances of this application.

#pragma data_seg("Shared")

volatile LONG g_lApplicationInstances=0;

#pragma data_seg()

//tell the linker to make the Shared section readable/writable,and shared.

#pragma comment(linker,"/Section:Shared,RWS")

//=============================================

//end of demo



int APIENTRY WinMain(HINSTANCE hInstance,

HINSTANCE hPrevInstance,

LPSTRlpCmdLine,

intnCmdShow)

{

//get the numberic value of the systemwide window message used to notify 

//all top-level windows when the module's usage count has changed.

g_uMsgAppInstCountUpdate=RegisterWindowMessage(TEXT("MsgAppInstCountUpdate"));

//there is another instance of this application running.

InterlockedExchangeAdd((PLONG)&g_lApplicationInstances,1);

// TODO: Place code here.

MSG msg;

HACCEL hAccelTable;



// Initialize global strings

LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);

LoadString(hInstance, IDC_APPINST, szWindowClass, MAX_LOADSTRING);

MyRegisterClass(hInstance);



// Perform application initialization:

if (!InitInstance (hInstance, nCmdShow)) 

{

return FALSE;

}



hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_APPINST);



// Main message loop:

while (GetMessage(&msg, NULL, 0, 0)) 

{

if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 

{

TranslateMessage(&msg);

DispatchMessage(&msg);

}

}



InterlockedExchangeAdd((PLONG)&g_lApplicationInstances,-1);

//have all other instances update their display.

PostMessage(HWND_BROADCAST,g_uMsgAppInstCountUpdate,0,0);



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

wcex.hIcon= LoadIcon(hInstance, (LPCTSTR)IDI_APPINST);

wcex.hCursor= LoadCursor(NULL, IDC_ARROW);

wcex.hbrBackground= (HBRUSH)(COLOR_WINDOW+1);

wcex.lpszMenuName= (LPCSTR)IDC_APPINST;

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



//Force the static control to be initializede correctly.

PostMessage(HWND_BROADCAST,g_uMsgAppInstCountUpdate,0,0);



return TRUE;

}



//

//FUNCTION: WndProc(HWND, unsigned, WORD, LONG)

//

// PURPOSE:Processes messages for the main window.

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



if(message==g_uMsgAppInstCountUpdate)

{

TCHAR chNum[4];

itoa(g_uMsgAppInstCountUpdate,chNum,4);

strcat(szHello,chNum);

}



switch (message) 

{

case WM_COMMAND:

wmId= LOWORD(wParam); 

wmEvent = HIWORD(wParam); 

// Parse the menu selections:

switch (wmId)

{

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

操作系统使内存能够将一个数据文件映射到进程的地址空间中，方便大量数据的操作。下面以将文件中的所有字节顺序进行倒序，来理解内存映射文件的功能。

步骤一：创建或打开文件内核对象

若要创建或打开一个文件内核对象，调用CreateFile函数：

HANDLE CreateFile(

PCSTR pszFileName,

DWORD dwDesiredAccess,

DWORD dwShareMode,

PSECURITY_ATTRIBUTES psa,

DWORD dwCreationDisposition,

DWORD dwFlagsAndAttributes,

HANDLE hTemplateFile);

参数pszFileName用于指明要创建或打开的文件的名字（包括严格选项路径），参数dwDesiredAccess用于设定如何访问该文件的内容，为0表不能读取或写入文件的内容（用于只想获取文件的属性时）；为GENERIC_READ可以从文件中读取数据；为GENERIC_WRITE可以将数据写入文件；为GENERIC_READ|GENERIC_WRITE可以从文件中读取数据，也可以将数据写入文件。参数dwShareMode告诉系统将如何共享文件，为0表打开文件的任何尝试均将失败；为FILE_SHARE_READ表示使用GENERIC_WRITE打开文件的其他尝试将会失败；为FILE_SHARE_WRITE表示使用GENERIC_READ打开文件的其他尝试将会失败；为FILE_SHARE_READ|FILE_SHARE_WRITE表示打开文件的其他尝试将会取得成功。如果CreateFile函数成功地创建或打开指定的文件，便返回个亿文件内核对象的句柄，否则返回INVALID_HANDLE_VALUE。

步骤二：创建一个文件映射内核对象

调用CreateFile函数，就可将文件映象的物理存储器的位置告诉系统，所传递的路径名用于指明支持文件映象的物理存储器所在磁盘上的确切位置，此时，还需要告诉系统，文件映射对象需要多少物理存储器，需调用CreateFileMapping函数：

HANDLE CreateFileMapping(

HANDLE hFile,

PSECURITY_ATTRIBUTES psa,

DWORD fdwProtect,

DWORD dwMaximumSizeHigh,

DWORD dwMaximumSizeLow,

PCTSTR pszName);

参数hFile用于标识映射到进程地址空间中的文件句柄，是CreateFile返回的值；psa参数是指向文件映射内核对象的SECURITY_ATTRIBUTES结构的指针，通常传递NULL值。系统创建文件映射对象，并将用于标识该对象的句柄返回该调用线程，如系统无法创建文件映射对象，便返回一个NULL句柄值。

步骤三：将文件数据映射到进程的地址空间

当创建了一个文件映射对象后，仍然必须让系统为文件的数据保留一个地址空间区域，并将文件的数据作为映射到该区域的物理存储器进行提交。可通过MapViewOfFile函数来实现：

PVOID MapViewOfFile(

HNADLE hFileMappingObject,

DWORD dwDesiredAccess,

DWORD dwFileOffsetHigh,

DWORD dwFileOffsetLow,

SIZE_T dwNumberOfBytesToMap);

参数hFileMappingObject用于标识文件映射对象的句柄，该句柄上调用CreateFileMapping和OpenFileMapping函数返回的。参数dwDesiredAccess用于标识如何访问该数据。

步骤四：从进程的地址空间中撤消文件数据的映象

当不再需要保留映射到你的进程地址空间区域中的文件数据时，可调用下面函数：

BOOL UnmapViewOfFile(PVOID pvBaseAddress);

参数pvBaseAddress用于设定返回区域的基地址，该值必须与调用MapViewOfFile函数返回的值相同。在进程终止运行前，如没有调用该函数，保留区域将不会被释放。为提高速度，系统将文件的数据页面进行高速缓存，并且在对文件的映射视图进行操作时不立即更新文件的磁盘映象，如果要确保更新被写入磁盘，可调用函数FlushViewOfFile强制系统将修改过的数据的一部分或全部重新写入磁盘映像中：

BOOL FlushViewOfFile(

PVOID pvAddress,

SIZE_T dwNumberOfBytesToFlush);

第一个参数pvAddress包含在内存映射文件中的视图的一个字节的地址，传递地址圆整为一个页面边界值，第二个参数指明要刷新的字节数。

步骤五和步骤六：关闭文件映射对象和文件对象

如不关闭打开了的内核对象，在进程继续运行中会导致资源泄露。关闭文件映射对象和文件对象，需两次调用CloseHandle函数，每个句柄调用一次。

使用内存映射文件在进程之间共享数据。Windows提供各种机制，使应用程序能共享数据和信息，这些机制包括：RPC、COM、OLE、DDE、窗口消息（尤其是WM_COPYDATA）、剪贴板、邮箱、管道和套接字等。在Windows中，单机共享数据的最底层机制是内存映射文件。如果互相进行通信的所有进程都在同一台计算机上，上面提到的所有机制均使用内存映射文件从事，如要求达到较高性能和较小开销，内存映射文件是最佳机制。数据共享方法是通过让两个或多个进程映射同一个文件映射对象的视图来实现的，这意味着它们将共享物理存储器的同一个页面，因此，当一个进程将数据写入一个共享文件映射对象的视图时，其他进程可以立即看到它们视图中的数据变更情况。注意，如果多个进程共享单个文件映射对象，那所有进程必须使用相同的名字来表示该文件映射对象。



