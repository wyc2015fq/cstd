# VC++中把窗体封装到dll中并调用 - xqhrs232的专栏 - CSDN博客
2014年01月14日 10:51:21[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：2174
原文地址::[http://wenku.baidu.com/link?url=LE3ktwKCW1IS09hMebfloHitGGPSHt4I7s6J3Dx0q84_3xeQlZwucYuwGrO-l8y9F8soojjs_mzyTOxgVboMg3asMcljOAigdiRCITdmv1C](http://wenku.baidu.com/link?url=LE3ktwKCW1IS09hMebfloHitGGPSHt4I7s6J3Dx0q84_3xeQlZwucYuwGrO-l8y9F8soojjs_mzyTOxgVboMg3asMcljOAigdiRCITdmv1C)
在DLL中使用资源
现在最常看见的关于DLL的问题就是如何在DLL中使用对话框，这是一个很普遍的关于如何在DLL中使用资源的问题。这里我们从Win32 DLL和MFC DLL两个方面来分析并解决这个问题。
1．Win32 DLL
在Win32 DLL中使用对话框很简单，你只需要在你的DLL中添加对话框资源，而且可以在对话框上面设置你所需要的控件。然后使用DialogBox或者CreateDialog这两个函数（或相同作用的其它函数）来创建对话框，并定义你自己的对话框回调函数处理对话框收到的消息。下面通过一个具体实例来学习如何在Win32 DLL中使用对话框，可以按照以下步骤来完成这个例子：
1）在VC菜单中File->New新建一个命名为UseDlg的Win32 Dynamic-Link Library工程，下一步选择A simple DLL project。
2）在VC菜单中Insert->Resource添加一个ID为IDD_DLG_SHOW的Dialog资源，将此Dialog上的Cancel按钮去掉，仅保留OK按钮。再添加一个ID为IDD_ABOUTBOX的对话框，其Caption为About。保存此资源，将资源文件命名为UseDlg.rc。并将resource.h和UseDlg.rc加入到工程里面。
3）在UseDlg.app中包含resource.h，并添加如下代码：
HINSTANCE hinst = NULL;
HWND hwndDLG = NULL;
BOOL CALLBACK DlgProc(HWND hDlg, UINT message,WPARAM wParam, LPARAM lParam);
BOOL CALLBACK AboutProc(HWND hDlg, UINT message,WPARAM wParam, LPARAM lParam);
extern "C" __declspec(dllexport) void ShowDlg();
BOOL APIENTRY DllMain( HANDLE hModule,
DWORD ul_reason_for_call,
LPVOID lpReserved
)
{
switch(ul_reason_for_call)
{
case DLL_PROCESS_ATTACH:
hinst = (HINSTANCE)hModule;
case DLL_PROCESS_DETACH:
break;
}
return TRUE;
}
extern "C" __declspec(dllexport) void ShowDlg()
{
hwndDLG = CreateDialog(hinst,MAKEINTRESOURCE(IDD_DLG_SHOW),NULL,(DLGPROC)DlgProc);
ShowWindow(hwndDLG, SW_SHOW);
}
BOOL CALLBACK DlgProc(HWND hDlg, UINT message,WPARAM wParam, LPARAM lParam)
{
switch(message)
{
case WM_INITDIALOG:
return TRUE;
case WM_COMMAND:
if(LOWORD(wParam)==IDOK)
DialogBox(hinst,MAKEINTRESOURCE(IDD_ABOUTBOX),hDlg,(DLGPROC)AboutProc);
return TRUE; 
case WM_CLOSE:
DestroyWindow(hDlg);
hwndDLG = NULL;
return TRUE;
}
return FALSE;
}
BOOL CALLBACK AboutProc(HWND hDlg, UINT message,WPARAM wParam, LPARAM lParam)
{
switch(message)
{
case WM_CLOSE:
EndDialog(hDlg,NULL);
hwndDLG = NULL;
return TRUE;
}
return FALSE;
}
4）编译生成UseDlg.dll和UseDlg.lib。
接下来我们建立调用此DLL的应用程序，其步骤如下：
1）在VC菜单中File->New新建一个命名为Use的MFC AppWizard(exe)工程，下一步选择Dialog Based之后点击Finish按钮。
2）在主对话框上面添加一个按钮,www.qqme5.com,之后双击此按钮，会弹出Add Member Function的对话框，直接点击OK进入void CUseDlg::OnButton1()函数。并在此函数内添加一个函数调用：ShowDlg();。
3）紧跟在#include语句后面加上如下代码：
extern "C" __declspec(dllexport) void ShowDlg();
#pragma comment(lib,"debug/UseDlg")
4）将上面UseDlg工程中生成的UseDlg.dll和UseDlg.lib两个文件复制到Use工程的Debug目录内。
5）编译生成Use.exe。
运行Use.exe，点击Button1按钮，可以看到一个名称为Dialog的非模态对话框弹出。点击上面的按钮，可以弹出模态对话框About。运行成功。
让我们来回顾一下在Win32 DLL中使用对话框的过程。
在DLL中，我们定义了两个对话框资源：IDD_DLG_SHOW和IDD_ABOUTBOX，并且导出了函数ShowDlg。在函数ShowDlg之中使用CreateDialog函数创建了非模态对话框IDD_DLG_SHOW，并指定了该对话框的回调函数DlgProc。在DlgProc之中处理了WM_INITDIALOG、WM_COMMAND和WM_CLOSE消息，以响应用户对对话框所做的动作。在处理按钮动作的时候，使用DialogBox函数创建IDD_ABOUTBOX这个模态对话框，指定其回调函数为AboutProc，并且在AboutProc中处理其相应消息。
在EXE中，我们使用隐式链接的方法来调用DLL，并使用DLL中导出的ShowDlg函数来调用DLL中的对话框。
在Win32 DLL中使用对话框就是这么简单，下面让我们来看一下在MFC DLL中如何使用对话框。
2．MFC DLL
在MFC DLL中使用对话框不像Win32 DLL中那么简单，主要是因为MFC程序中存在一个模块状态（Module State）的问题，也就是资源重复的问题。（此处的术语模块是指一个可执行程序，或指其操作不依赖于应用程序的其余部分但使用MFC运行库的共享副本的一个DLL（或一组DLL）。我们所创建的MFC DLL就是这种模块的一个典型实例。）
在每个模块（EXE或DLL）中，都存在一种全局的状态数据，MFC依靠这种全局的状态数据来区分不同的模块，以执行正确的操作。这种数据包括：Windows实例句柄（用于加载资源），指向应用程序当前的CWinApp和CWinThread对象的指针，OLE模块引用计数，以及维护Windows对象句柄与相应的MFC对象实例之间连接的各种映射等。但当应用程序使用多个模块时，每个模块的状态数据不是应用程序范围的。相反，每个模块具有自已的MFC状态数据的私有副本。这种全局的状态数据就叫做MFC模块状态。
模块的状态数据包含在结构中，并且总是可以通过指向该结构的指针使用。当代码在执行时进入了某一个模块时，只有此模块的状态为“当前”或“有效”状态时，MFC才能正确的区分此模块并执行正确的操作。
例如，MFC应用程序可以使用下面代码从资源文件中加载字符串：
CString str;
str.LoadString(IDS_MYSTRING);
使用这种代码非常方便，但它掩盖了这样一个事实：即此程序中IDS_MYSTRING可能不是唯一的标识符。一个程序可以加载多个DLL，某些DLL可能也用IDS_MYSTRING标识符定义了一个资源。MFC怎样知道应该加载哪个资源呢？MFC使用当前模块状态查找资源句柄。如果当前模块不是我们要使用的正确模块，那么就会产生不正确的调用或者错误。
按照MFC库的链接方法，一个MFC DLL有两种使用MFC库的方法：静态链接到MFC的DLL和动态链接到MFC的DLL。下面我们就按照这两种类型的MFC DLL来介绍如何切换当前模块状态以正确的在MFC DLL中使用资源。
1、静态链接到MFC的DLL
静态链接到MFC的规则DLL与MFC库静态链接，则此时MFC库不能共享，所以MFC总是使用它所链接的DLL的模块状态。这样也就不存在管理模块状态的问题。但使用这种方法的缺点是DLL程序将会变大，而且会在程序中留下重复代码。下面给出的例子验证了这一点。本例可以按照以下步骤来完成：
1）在VC菜单中File->New新建一个命名为DLLStatic的MFC AppWizard的工程，下一步选择Regular DLL with MFC statically linked。
2）在工程中添加一个对话框资源，在VC菜单中Insert->Resource添加一个ID为：IDD_ABOUTBOX。保存此资源，将资源文件命名为UseDlg.rc。并将resource.h和UseDlg.rc加入到工程里面,并在resource.h之中将IDD_ABOUTBOX 的数值改为100。
3）在DLLStatic.cpp中定义如下函数：
void ShowDlg()
{
CDialog dlg(IDD_ABOUTBOX);
dlg.DoModal();
}
4）在DLLStatic.def文件中的EXPORTS语句中添加一行：ShowDlg，以导出ShowDlg函数。
; MFCDLLStatic.def : Declares the module parameters for the DLL.
LIBRARY "MFCDLLStatic"
DESCRIPTION 'MFCDLLStatic Windows Dynamic Link Library'
EXPORTS
ShowDlg; //添加这一行
; Explicit exports can go here
5）编译生成DLLStatic.dll和DLLStatic.lib。
继续使用上一节中的Use工程，将前面生成的DLLStatic.dll和DLLStatic.lib两个文件复制到工程的Debug目录内，并将
extern "C" __declspec(dllexport) void ShowDlg();
#pragma comment(lib,"debug/UseDlg")
这两行改为：
void ShowDlg();
#pragma comment(lib,"debug/DLLStatic")
编译并运行Use.exe。点击按钮，可以看到DLLStatic中的模态对话框弹出。
本例中，可以注意到DLL中所定义的About对话框资源与EXE中所定义的About对话框资源ID完全相同,http://youximingzhi.qqq23.com,但是当我们点击Use.exe上面的按钮时，弹出的是DLL中的模态对话框。说明，当使用静态链接到MFC的规则DLL时，不存在管理模块状态的问题。
2、动态链接到MFC的DLL
在讨论关于动态链接到MFC的DLL的模块状态问题之前，先来看一个例子。本例可以通过如下步骤来完成：
1）在VC菜单中File->New新建一个命名为DLLShared的MFC AppWizard的工程，下一步选择Regular DLL using shared MFC DLL。
2）在工程中添加一个对话框资源，其ID为：IDD_ABOUTBOX。并在resource.h之中将IDD_ABOUTBOX 的数值改为100。
3）在DLLShared.cpp中定义如下函数：
void ShowDlg()
{
CDialog dlg(IDD_ABOUTBOX);
dlg.DoModal();
}
4）在DLLShared.def文件中的EXPORTS语句中添加一行：ShowDlg，以导出ShowDlg函数。
5）编译生成DLLShared.dll和DLLShared.lib。
继续使用上面的Use工程，将前面生成的DLLShared.dll和DLLShared.lib两个文件复制到工程的Debug目录内，并将
extern "C" __declspec(dllexport) void ShowDlg();
#pragma comment(lib,"debug/DLLStatic")
这两行改为：
void ShowDlg();
#pragma comment(lib,"debug/DLLShared")
编译并运行Use.exe。点击按钮，这次你看到了什么？对，没错，这次弹出的是Use.exe的关于对话框。将上述例子的DLL类型换成MFC Extension DLL(using shared MFC DLL)也会出现相同的问题。
为什么会出现上面的问题？这是因为在使用了MFC共享库的时候，默认情况下，MFC使用主应用程序的资源句柄来加载资源模板。虽然我们调用的是DLL中的函数来显示DLL中的对话框，并且对应的对话框模板是存储在DLL中的，但MFC仍旧在主应用程序也就是Use.exe中寻找相应的对话框模板。由于在DLL中所定义的对话框资源ID与主应用程序中所定义的关于对话框的资源ID相同，所以MFC就把主应用程序中的关于对话框显示了出来。如果二者不同，则MFC就认为DLL中所定义的对话框资源不存在，dlg.DoModal会返回0，也就是什么都不会显示。
那么如何解决上述问题呢？解决办法就是在适当的时候进行模块状态切换，以保证具有当前状态的模块是我们所需要的模块从而使用正确的资源。MFC提供了下列函数和宏来完成这些工作：
AfxGetStaticModuleState：这是一个函数，其函数原型为：
AFX_MODULE_STATE* AFXAPI AfxGetStaticModuleState( );
此函数在堆栈上构造AFX_MODULE_STATE类的实例pModuleState并对其赋值后将其返回。在AFX_MODULE_STATE类的构造函数中，该类获取指向当前模块状态的指针并将其存储在成员变量中，然后将pModuleState设置为新的有效模块状态。在它的析构函数中，该类将存储在其成员变量中的指针还原为存贮的前一个模块状态。
AFX_MANAGE_STATE：这是一个宏，其原型为：
AFX_MANAGE_STATE( AFX_MODULE_STATE* pModuleState )
该宏用于将pModuleState（指向包含模块全局数据也就是模块状态的AFX_MODULE_STATE结构的指针）设置为当前的即时作用空间中（the remainder of the immediate containing scope）的有效模块状态。在离开包含该宏的作用空间时，前一个有效的模块状态自动还原。
AfxGetResourceHandle：这个函数的原型为：
HINSTANCE AfxGetResourceHandle( );
该函数返回了一个保存了HINSTANCE类型的、应用程序默认所加载资源的模块的句柄。
AfxSetResourceHandle：这个函数的原型为：
void AfxSetResourceHandle( HINSTANCE hInstResource );
该函数将hInstResource所代表的模块设置为具有当前状态的模块。
通过使用上述四个函数或宏就可以正确的在动态链接到MFC的DLL中切换模块状态。接下来我们将通过修改上面出现问题的那个例子来介绍如何使用上述四个函数或宏。先来看看Regular DLL using shared MFC DLL类型：
在上述例子的第三步的ShowDlg函数的第一条语句前加上如下语句（要确保该语句在函数实现的第一行）：
AFX_MANAGE_STATE(AfxGetStaticModuleState());
之后重新编译生成DLLShared.dll和DLLShared.lib，并将这两个文件重新拷贝到Use工程的Debug目录内。这次编译生成Use.exe并运行，点击按钮，可以看到弹出的时我们在DLL中所加入的那个对话框，而不再是Use.exe的关于对话框了。
通过上面的讲解，相信你已经知道该语句的作用了。在函数ShowDlg的第一行加上这么一句后，每次调用DLL的应用程序使用该函数的时候，MFC库都会自动切换当前模块状态，这样就保证了资源读取的正确性。
AFX_MANAGE_STATE(AfxGetStaticModuleState());是自动切换当前模块状态，也可以通过使用AfxGetResourceHandle和AfxSetResourceHandle来手动切换当前模块状态。具体使用方法如下：
在上述例子的第三步的ShowDlg函数的第一条语句前加上如下语句（要确保该语句在函数实现的第一行）：
HINSTANCE save_hInstance = AfxGetResourceHandle();
AfxSetResourceHandle(theApp.m_hInstance);
在调用对话框成功之后，也就是dlg.DoModal();之后，添加：
AfxSetResourceHandle(save_hInstance);
这种方法在进入ShowDlg函数之后，通过AfxGetResourceHandle来获得并保存当前状态模块的句柄。然后获得DLL模块的句柄theApp.m_hInstance（当然，也可以使用GetModuleHandle函数来获得DLL模块的句柄），并使用AfxSetResourceHandle函数来将其设置为当前状态状态。最后在调用对话框成功之后再用恢复AfxSetResourceHandle资源句柄，将当前模块状态恢复。
这样做有些麻烦，但是有一点好处是可以在完成使用资源的任务之后就可以立即恢复资源句柄。而AFX_MANAGE_STATE(AfxGetStaticModuleState());的方法只能等函数的作用空间结束之后才恢复资源句柄。由于可执行文件必须重画工具条等原因，因此建议只要有可能就必须恢复资源句柄，否则可能会遇到许多问题。比如说，如果用户移动DLL的对话框，而此时资源句柄仍然为DLL的资源，那么程序就会崩溃。最好的恢复句柄的时机在对话框响应WM_INITDIALOG消息的时候，因为这时对话框的模板等已经读出了。
对于MFC Extension DLL(using shared MFC DLL)类型的MFC DLL，切换当前模块状态的方法与Regular DLL using shared MFC DLL类型的MFC DLL所使用的方法很相似，这里不再举例实现。二者不同的地方如下：
在MFC扩展DLL中使用AFX_MANAGE_STATE(AfxGetStaticModuleState());时，会产生如下错误：
mfcs42d.lib(dllmodul.obj) : error LNK2005: __pRawDllMain already defined in dllextend.obj
mfcs42d.lib(dllmodul.obj) : error LNK2005: _DllMain@12 already defined in dllextend.obj
mfcs42d.lib(dllmodul.obj) : error LNK2005: __pRawDllMain already defined in dllextend.obj
因此在MFC扩展DLL中需要将AFX_MANAGE_STATE(AfxGetStaticModuleState());换成AFX_MANAGE_STATE(AfxGetAppModuleState());才能正确切换当前模块状态。
在MFC扩展DLL中使用AfxGetResourceHandle和AfxSetResourceHandle的方法与在Regular DLL using shared MFC DLL类型的MFC DLL中所使用的方法相同。并且，DLL模块的句柄可以通过MFC提供的DlgextentDLL这个结构的hModule成员来获得。即使用AfxSetResourceHandle(DlgextentDLL.hModule);语句。
当然，对于动态链接到MFC的DLL，也可以在调用该DLL的MFC应用程序中使用AfxGetResourceHandle和AfxSetResourceHandle两个函数来切换当前状态模块。该DLL模块的句柄可以用GetModuleHandle函数来获得。在此不再赘述。
