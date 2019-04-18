# 对程序错误的处理——Windows核心编程学习手札之一 - 专注于数据挖掘算法研究和应用 - CSDN博客





2008年11月24日 15:24:00[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2336








# 对程序错误的处理

### ——Windows核心编程学习手札之一

函数被调用执行时，先检验传递给它的各个参数的有效性，后执行任务。函数执行中若因参数无效或因某种原因导致无法正常完成函数任务，那么操作系统会返回一个值，以提示函数运行失败。这个思想可归纳为：函数错误提示的透明，同样适用于个人编写的函数，在函数执行任务的每个关键点设置错误提示并返回。对所返回的错误代码或提示对于正确定位和调试函数失败点是非常有用的。

Microsoft给出了一个所有可能的错误代码列表，并为每个错误代码分配一个32位的号码。Windows函数常用的返回值类型有VOID/BOOL/HANDLE/PVOID/LONG/DWORD，基于此，个人编写的程序也可以维护一张自己的错误代码列表，并设定返回类型。

当函数检测到错误时，会使用线程本地存储器（thread-local storage）机制，将相应的错误代码号码与调用的线程关联起来，这使线程能够互相独立地运行，而不影响各自的错误代码。Windows函数返回时，其返回值就能指明一个错误已经发生。若要确定发生的错误是什么，可调用函数：

DWORD GetLastError()；

该函数返回线程的32位错误代码。有了32位错误代码的号码，便可以将该号码转换成更有用的信息。WinError.h头文件包含了Microsoft定义的错误代码列表。当函数运行失败时，应立即调用GetLastError函数，因为如果调用另一个Windows函数，它的值很可能被改写。

有些函数可能返回了正确的代码，但并不表明函数实际运行成功，其中有很多原因。如，创建指明的事件内核对象之所以成功，是因为实际上已经创建了该对象或已经存在带有相同名字的事件内核对象。

程序调试中，对线程的最后错误代码进行提取和分析是非常有用的。在Microsoft Visual studio6.0中，Microsoft的调试程序可以配置Watch窗口，以便始终显示线程的最后错误代码及其英文描述。操作是在Watch窗口中的一行输入“@err,hr”即可。Visual studio还提供一个实用程序Error Lookup可以将错误代码的号码转换成相应的文本描述。个人在用Microsoft Visual C++6.0开发的单元调试中经常用到Watch窗口和Error Lookup来查看最后错误代码，当然Watch窗口的作用还不仅仅是查看错误代码及其描述。

同时Windows提供一个函数FormatMessage可以将错误代码转换成它的文字描述。

DWORD FormatMessage(

DWORD dwFlags,

LPCVOID pSource,

DWORD dwMessageID,

DWORD dwLanguageID,

PTSTR pszBuffer,

DWORD nSize,

va_list *Arguments);

FormatMessage函数能够检测出用户首选的语言，并返回相应的文本。对于Microsoft是否应该建立一个主控列表，以显示每个Windows函数可能返回的所有错误代码，可从下面两方面来否定：一是创建新系统版本时，建立和维护该主控列表太困难；二是函数在内部调用另一函数，而这一函数同样内部调用其他函数，如此类推，其中任何一个函数运行失败，调用函数都可能恢复并执行工作，而Microsoft需要跟踪每个函数的运行路径，并建立所有可能的错误代码列表。

实际上我们在使用Win32的开发环境中，调用Windows函数可以利用GetLastError函数来知道所调用的Windows函数错误所指，而对自己的函数则需要自己编写错误代码，或说维护自己的错误代码列表，以便与Windows函数一样可以定位错误。换句话说，可以将Windows函数告诉调用者指明发生的错误的机制用于自己的函数体系。一方面可以利用Windows已有的错误代码，在某个函数可能出现错误的地方设置最后错误代码并返回，可采用Windows函数：

VOID SetLastError(DWORD dwErrCode);

那么可以用WinError.h中代码，只要该代码可能说明错误思想并让调用者理解即可，如想反映句柄无效，可以设置错误代码为6，错误代码是32位的数字。


另一方面你认为有必要维护自己的错误列表，或WinError.h中的错误不足以反映你程序的错误性质，可以创建自己的代码，Microsoft规定，该公司建立的所有错误代码在第29位的信息位使用0，用户自己创建的使1，可以确保自己的错误代码与Microsoft不冲突。具体错误代码域可如下：



位31~30292827~1615~0


内容严重性Microsoft/客户保留设备代码异常代码

含义0=成功0=Microsoft公司必须是0由Microsoft由Microsoft

公司定义公司定义

定义的代码

1=供参考1=客户定义代码

2=警告


3=错误


这里模拟了Microsoft Visual C6.0中Error Lookup程序，利用MFC基于对话框的程序实现，主要实现代码如下：

<?xml:namespace prefix = o ns = "urn:schemas-microsoft-com:office:office" />



void CErrorShowDlg::OnBtnLookUp() 

{

// TODO: Add your control notification handler code here

//Get the error code

BOOL bRetErrorCode;

DWORD dwErrorCode=GetDlgItemInt(IDC_EdtErrorCode,&bRetErrorCode,FALSE);

if(!bRetErrorCode)//无值

{

AfxMessageBox("输入错误的代码！");

return;

}

//Get the error code's textual description

HLOCAL hLocal=NULL;

BOOL bRetErrorText=FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,

NULL,dwErrorCode,MAKELANGID(LANG_ENGLISH,SUBLANG_ENGLISH_US),

(LPTSTR)&hLocal,0,NULL);

if(hLocal != NULL)

{

SetDlgItemText(IDC_EdtErrorShow,(LPCTSTR)LocalLock(hLocal));

LocalFree(hLocal);

}else

SetDlgItemText(IDC_EdtErrorShow,TEXT("Error Number not found!"));

}





如非<?xml:namespace prefix = st1 ns = "urn:schemas-microsoft-com:office:smarttags" />2008-11-22



