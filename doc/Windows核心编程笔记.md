# Windows核心编程笔记



# （一） 错误处理

​    在Windows编程中我们调用各种Windows API函数来使用系统提供的各种基础支持，这些API函数并不是总是都能够预期完成所规定的工作的，有些时候会因为各种原因导致函数调用失败，比如传给函数的参数不正确，调用函数的权限不够，请求的资源系统暂时分配不了等等。

   大多时候根据函数的返回值，可以判断函数是否失败

![img](https://img-blog.csdn.net/20170105005705626?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2FuZ3BlbmdrNzc4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



​    Windows除了使用返回值标识函数执行情况，外还引入了错误代码机制。返回值指出函数已发生一个错误。要查看具体是什么错误，就应该使用错误代码。如当我们调用函数打开一个文件，如果打开失败，可能出现多种原因。一是：文件不存在，另一种是该文件被其他程序互斥使用。在这两种情况下都导致文件打开失败，返回值仅仅标识打开失败，但我们不知道原因。这时错误代码就派上用场了。


     Windows将错误代码存储于独立于线程的，线程局部存储区中。顾名思义，这个区域是每个线程独有的其他线程无法更改。它存储着一些线程独有的东西，在第二十一章有详细的介绍。在函数执行后，就可以通过调用GetLastError()来获得错误代码。因此这个错误代码永远反映的是上一个函数的执行情况，也就是说它的值是由上一个函数设置的。因此，当调用一个API函数后应该在调用其他API之前使用GetLastError'来获取具体的错误代码（避免被其他API改写Error值），进而判定失败的原因。



​      有些时候从API函数的返回值来判定为成功，但是函数的成功可能缘于不同的原因，例如使用CreateEvent创建一个具名的事件对象时，函数成存在两种情况:1、对象实际创建成功2、打开了已存在的同名事件对象。这时我们可以调用GetLastError根据返回值是不是ERROR_ALREADY_EXISTS来判定是不是打开了同名的对象。



​     GetLastError函数的原型如下

```cpp
DWORD WINAPI GetLastError(void);
```



他的返回值是一个DWORD类型，在Windows SDK的winerror.h文件中为这些表示错误的DWORD值定义了宏和注释，格式如下

```cpp
//
// MessageId: ERROR_SUCCESS
//
// MessageText:
//
// The operation completed successfully.
//
#define ERROR_SUCCESS                    0L
 
#define NO_ERROR 0L                                                 // dderror
#define SEC_E_OK                         ((HRESULT)0x00000000L)
 
//
// MessageId: ERROR_INVALID_FUNCTION
//
// MessageText:
//
// Incorrect function.
//
#define ERROR_INVALID_FUNCTION           1L    // dderror
 
//
// MessageId: ERROR_FILE_NOT_FOUND
//
// MessageText:
//
// The system cannot find the file specified.
//
#define ERROR_FILE_NOT_FOUND             2L
 
//
// MessageId: ERROR_PATH_NOT_FOUND
//
// MessageText:
//
// The system cannot find the path specified.
//
#define ERROR_PATH_NOT_FOUND             3L
 
//
// MessageId: ERROR_TOO_MANY_OPEN_FILES
//
// MessageText:
//
// The system cannot open the file.
```



​    第一行是错误代码的宏定义  每一个DWORD对应一个宏标记，方面理解错误代码的函数，这些宏都以ERROR_开头，宏下面的第三行是对错误代码的含义注释

​    在我们自己编写的函数中可以使用这种机制，我们的函数可能因为N中原因导致失败，我们可以再让函数返回失败的值得同时为每一种失败原因设置同Windows AP一样的错误码，Windows提供了让我们来设置错误码的系统函数  SetLastError,





```cpp
void WINAPI SetLastError(
  __in  DWORD dwErrCode
);
```



我们应该尽量使用WinError.h中现有的错误码，只要可以很好的反应想报告的错我。如果没有合适的可以定义自己错误码，32位的错误码被分为了不同的字段，其结构如下

![img](https://img-blog.csdn.net/20170105020127220?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2FuZ3BlbmdrNzc4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



可以定义自己的错误代码宏，来方便的完成错误的定义

例如  我们定义一个错误码表示在一个获取网页的函数中域名不正确，可以这样来做



```cpp
#define    FACILITY_HTTP    257
#define     COVERT_HTTP(x)    (x<=0?x:((x&0x0000FFFF)|((FACILITY_HTTP<<16)|0xA0000000)))

#define    ERROR_DOMAIN_NO_EXISTS COVERT_HTTP(1L)
```



   另外Windwos提供了一个API函数FormatMessage函数来获取相应错误码的 详细描述 具体使用参见MSDN





```cpp
DWORD WINAPI FormatMessage(
  __in      DWORD dwFlags,
  __in_opt  LPCVOID lpSource,
  __in      DWORD dwMessageId,
  __in      DWORD dwLanguageId,
  __out     LPTSTR lpBuffer,
  __in      DWORD nSize,
  __in_opt  va_list *Arguments
);
```

QT实现了下Error Lookup程序





```cpp
#include "dialog.h"
#include "ui_dialog.h"
#include <qstring.h>
Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    ui->edit_ErrorCode->setValidator(new QIntValidator(0,65536,this));
    hlocal =  (LPSTR)VirtualAlloc(NULL,256,MEM_COMMIT | MEM_RESERVE,PAGE_READWRITE);
}
 
Dialog::~Dialog()
{
    delete ui;
}
 
 
void Dialog::on_btn_LookUp_pressed()
 
{
    QString str = ui->edit_ErrorCode->text();
    DWORD systemLocale = MAKELANGID(LANG_NEUTRAL,SUBLANG_NEUTRAL);
    BOOL bRet = FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS \
        | FORMAT_MESSAGE_MAX_WIDTH_MASK,NULL,str.toLong(),0,(LPSTR)hlocal,256,NULL);
    if(!bRet)
    {
        HMODULE hDll = LoadLibraryExA("netmsg.dll",NULL,DONT_RESOLVE_DLL_REFERENCES);
        if(hDll != NULL)
        {
             bRet = FormatMessageA(FORMAT_MESSAGE_FROM_HMODULE | FORMAT_MESSAGE_IGNORE_INSERTS \
                | FORMAT_MESSAGE_ALLOCATE_BUFFER,hDll,str.toLong(),systemLocale,hlocal,256,NULL);
            FreeLibrary(hDll);
        }
    }
    QString strMsg;
    if(!bRet)
    {
        memset(hlocal,0,256);
        sprintf(hlocal,"无法找到关于%d的描述",str.toLong());
        strMsg = QString::fromUtf8(hlocal);
    }
    else
    {
        strMsg = QString::fromLocal8Bit(hlocal);
    }
 
    ui->textEdit->clear();
    ui->textEdit->append(strMsg);
    memset(hlocal,0,256);
}
```









# （二） Widnwos下的字符处理



这一章作者首先介绍了字符编码，以及Windwos为什么使用Uincode 并使用UTF-16编码，关于字符集和字符编码参见<http://blog.csdn.net/wangpengk7788/article/details/54075917>



微软的编译器内建了一个wchar_t的数据类型，它表示UTF-16编码的字符，编译器需要打开 /Zc:wchar_t开关才可以使用该类型，现在的VS默认是打开的，定义Unicode字符和字符串

```cpp
wchar_t   c= L"A";
whcar_t   buffer[100] = L"A String0";
```

微软为区别C/C++为字符类型做了一些扩展定义，在WINNT.H文件中



```cpp
#ifndef _MAC
typedef wchar_t WCHAR;    // wc,   16-bit UNICODE character
#else
// some Macintosh compilers don't define wchar_t in a convenient location, or define it as a char
typedef unsigned short WCHAR;    // wc,   16-bit UNICODE character
#endif
 
typedef WCHAR *PWCHAR, *LPWCH, *PWCH;
typedef CONST WCHAR *LPCWCH, *PCWCH;
 
typedef __nullterminated WCHAR *NWPSTR, *LPWSTR, *PWSTR;
typedef __nullterminated PWSTR *PZPWSTR;
typedef __nullterminated CONST PWSTR *PCZPWSTR;
typedef __nullterminated WCHAR UNALIGNED *LPUWSTR, *PUWSTR;
typedef __nullterminated CONST WCHAR *LPCWSTR, *PCWSTR;
typedef __nullterminated PCWSTR *PZPCWSTR;
typedef __nullterminated CONST WCHAR UNALIGNED *LPCUWSTR, *PCUWSTR;
 
typedef __nullnullterminated WCHAR *PZZWSTR;
typedef __nullnullterminated CONST WCHAR *PCZZWSTR;
typedef __nullnullterminated WCHAR UNALIGNED *PUZZWSTR;
typedef __nullnullterminated CONST WCHAR UNALIGNED *PCUZZWSTR;
 
typedef __possibly_notnullterminated WCHAR *PNZWCH;
typedef __possibly_notnullterminated CONST WCHAR *PCNZWCH;
typedef __possibly_notnullterminated WCHAR UNALIGNED *PUNZWCH;
typedef __possibly_notnullterminated CONST WCHAR UNALIGNED *PCUNZWCH;
 
#if _WIN32_WINNT >= 0x0600 || (defined(__cplusplus) && defined(WINDOWS_ENABLE_CPLUSPLUS))
 
typedef CONST WCHAR *LPCWCHAR, *PCWCHAR;
typedef CONST WCHAR UNALIGNED *LPCUWCHAR, *PCUWCHAR;

typedef char CHAR;
typedef CHAR *PCHAR, *LPCH, *PCH;
typedef CONST CHAR *LPCCH, *PCCH;
 
typedef __nullterminated CHAR *NPSTR, *LPSTR, *PSTR;
typedef __nullterminated PSTR *PZPSTR;
typedef __nullterminated CONST PSTR *PCZPSTR;
typedef __nullterminated CONST CHAR *LPCSTR, *PCSTR;
typedef __nullterminated PCSTR *PZPCSTR;
 
typedef __nullnullterminated CHAR *PZZSTR;
typedef __nullnullterminated CONST CHAR *PCZZSTR;
 
typedef __possibly_notnullterminated CHAR *PNZCH;
typedef __possibly_notnullterminated CONST CHAR *PCNZCH;
```

在MSDN和VS中对含义字符串参数的API，均使用重新定义的类型来描述，如CreateFile





```cpp
HANDLE WINAPI CreateFile(
  __in      LPCTSTR lpFileName,
  __in      DWORD dwDesiredAccess,
  __in      DWORD dwShareMode,
  __in_opt  LPSECURITY_ATTRIBUTES lpSecurityAttributes,
  __in      DWORD dwCreationDisposition,
  __in      DWORD dwFlagsAndAttributes,
  __in_opt  HANDLE hTemplateFile
);
```

为了在Windwos下编程使代码可维护性增强，应该坚持并统一使用Windows数据类型，比较MSDN文档中的描述都使用这些类型。



另外VS中还定义了这样的宏





```cpp
#ifdef  UNICODE                 
typedef WCHAR TCHAR, *PTCHAR;
#define __TEXT(quote) L##quote   
   
#else         
 
typedef char TCHAR, *PTCHAR;
#define __TEXT(quote) quote  
#endif

#define TEXT(quote)  __TEXT(quote)
```

这样我们编写





```cpp
TCHAR c = TEXT('A');
TCHAR Buffer[100] = TEXT("A String");
```



无论在ANSI还是Unicode模式下 都可以编译通过







Windows的API函数如果有字符串参数的，都存在两个版本，例如CreateFile函数在winbase.h中是这样定义的

 

```cpp
#define CreateFile  CreateFileW
#else
#define CreateFile  CreateFileA
#endif // !UNICODE
```

​      这样的API函数有两个版本分别对应ANSI和Unicode模式，他们都由系统的DLL文件导出，在内部实现中，后缀是A的函数申请了一块内存然后将ANSI字符串转化成Unicode字符串保存到该块Buffer,然后以该Buffer的地址作为参数，调用后缀是W的函数，并在函数返回时释放该Buffer,只所以这样做是因为Windwos的内核实现中都是使用Unicode字符编码的，因此我们在写Windows应用程序时应首先考虑使用Unicode宽字符集，来避免调用A后缀的函数带来的资源开销。



在使用C运行库时，每个字符串相关函数同样存在两个版本，但是与Windows API函数不同，每个版本的函数都是独立实现的。在旧的C标准库函数中，对字符串修改的函数如strcpy  wcscpy因为没有指定缓冲区长度，很可能造成缓冲区溢出，因此在编程时不要使用这些存在安全隐患的C标准库函数是一个好的习惯。



Windows为这些不安全的C标准库函数提供了对应的安全版本，声明在StrSage.h文件中，这些函数都在原函数后加上_s后缀来做为新的函数名，如strcpy_s，这些新版本的函数，会检查传入的参数是否有效，缓冲区是否能容纳结果，如果检查失败，都会设置局部于线程的C运行时变量errno,并返回errno_t指示函数失败，同时C运行库运行我们设置一个自己的回调函数，在检查失败是调用该函数，可以这样定义该函数



```cpp
void invalid_parameter_handler(const wchar_t *, const wchar_t *, const wchar_t *, unsigned int, uintptr_t); 
```



然后调用_set_invalid_parameter_handler来注册这个回调函数，这里其实是设置一个全局函数指针，参见<http://blog.csdn.net/wangpengk7788/article/details/53938305>

​     这些函数的使用可以参考MSDN，除此之外C运行库还提供可一些函数，在处理字符串是提供更多地控制，如用自己填充，复制时截断，

![img](https://img-blog.csdn.net/20170105041712017?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2FuZ3BlbmdrNzc4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



使用参见MSDN   另外Windwos在kernerl32.dll中也导出了一些自出了一些字符串处理函数，作者介绍和向我们推荐了 CompareString(Ex)函数，使用一并参考MSDN吧



![img](https://img-blog.csdn.net/20170105042416183?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2FuZ3BlbmdrNzc4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)







最后是用于Unicode 和 ANSI 字符之间转换的函数 MultiByteToWideChar  和 WideCharToMultiByte 使用可以百度 或者 MSDN  











# （三） 内核对象与句柄



在写Windwos应用程序的时候HANDLE这个类型作为参数和返回值函数我们经常用到，这些函数有个共同的特点就是需要用到一个内核对象，这些内核对象有Job、Directory（对象目录中的目录）、SymbolLink（符号链接），Section（内存映射文件）、Port（LPC端口）、IoCompletion（Io完成端口）、File（并非专指磁盘文件）、同步对象（Mutex、Event、Semaphore、Timer）、Key（注册表中的键）、Token（用户/组令牌）、Process、Thread、Pipe、Mailslot、Debug（调试端口）等.





内核对象本质上是由系统内核维护的一个个结构体，它们大多用来标识某一资源的属性，用面向对象的方式来看，他们相当于没有成员函数的类，之所以没有是因为系统内核是用C语言来写的，但是抽象来他们又是面向对象的，每种内核对象对应着一个系列的内核函数，这些内核函数就相当于类的成员函数，他们接受一个对象的This指针做为参数，在这里这个This指针就是句柄，与C++This指针不同的是这指针不是一个内存地址，而是一个数组的索引，这个数组就是句柄表，Windows以进程为单来维护每个句柄表，句柄表里存放着每一个进程打来了的内核对象的地址，当我们调用一个HANDLE为返回值的函数（如CreateFile）内核种会创建一个相应的内核对象，并把这个对象地址挂入进程的句柄表内，返回它在表内的索引值，当我们使用一个以HANDLE为参数的函数时(如ReadFile)，在内核中传给对应内核函数(如NtCreateFile)，内核函数(调用ObReferenceObjectByHandle)获取本进程的（EPROCESS）在这个结构中找到举报表地址，根据HANDLE索引值获取内核对象的地址。







1、对一些需要在内核中构建全局对象来实现的功能（每个进程地址空间的内核部分是相同的）需要构建一个内核对象（一个结构体）来支持，因此完成这部分工作的API需要用到内核对象

2、位了内核对象的安全性，以及用户态来标识内核数据的可以操作性，（用户态代码不能访问内核地址空间），WInodws为每个进程引入了句柄表，句柄表是一个数组，每项存放一个内核对象的地址，以及一个访问掩码来标识需要的权限，以及一个标志用来表示是否有效。

3、API使用句柄表的索引，即HANDLE来标识内核对象，完成所需要的工作。

4、内核对象都是全局的，并不属于某一个进程，每个内核对象都有一个引用计数器，只有在引用为0时，系统内核才会销毁该对象。因此在打开和使用完成后需要CloseHandle 递减引用值。

5、没一个需要创建内核对象的API都有一个参数LPSECURITY_ATTRIBUTES ，

```cpp
typedef struct _SECURITY_ATTRIBUTES {
  DWORD  nLength;
  LPVOID lpSecurityDescriptor;
  BOOL   bInheritHandle;   标识是否该对象可以内子进程继承
} SECURITY
```



## **共享内核对象的三种方式**



###     跨进程共享内核对象方法之一：使用对象句柄继承

​    只有进程之间属于父子关系时才可以使用对象句柄继承。当父进程创建一个内核对象时，父进程必须向系统指出它希望这个对象的句柄是可继承的。为了创建可继承句柄父进程必须分配并初始化一个SECURITY_ATTRIBUTES结构，并将这个结构的地址传递给Create*函数。如：

```cpp
SECURITY_ATTRIBUTES sa;
sa.nLength=sizeof(sa);
sa.lpSecurityDescriptor=NULL；//使用默认安全性。
sa.bInheritHandle=TRUE；//是此句柄可以继承。
HANDLE mutex=CeattMutex(&sa,FALSE,NULL);
```

​    以上代码初始化了一个SECURITY_ATTRIBUTES结构，表明使用默认安全性来创建此对象，且返回的对象时可继承的。


​    句柄表的每个记录中还有一个指明该句柄是否可继承的标志位，如果在创建内核对象的时候将NULL作为PSECURITY_ATTRIBUTES的参数传入，则返回的句柄是不可继承的，标志位为0。

下一步是由父进程创建子进程，这是通过CreateProcess实现的，此函数第四章会详细介绍，此处仅仅注意bInheritHandles参数。如果在创建进程时，此参数被设为false，则表明不希望子进程继承父进程句柄表中的可继承句柄。如为true，则表明希望子进程继承父进程句柄表中的可继承句柄。注意只有可继承句柄才可以被继承。

新创建的进程句柄表为空，由于我们希望它继承父进程句柄表，此时系统会遍历父进程句柄表，对它的每一个项进行检查，将所有的可继承的句柄的项全部复制到子进程的句柄表中。在子进程的句柄表中，复制项的位置与它在父进程句柄表中的位置是完全一样的，这是非常重要的。它意味着在父进程和子进程中，对一个内核对象进行标识的句柄是完全一样的。除了复制句柄表，系统还会递增每个可继承句柄的使用计数。为了销毁内核对象，父进程和子进程必须都不再使用才可以。这可以通过CloseHandle和进程终止来实现。注意：句柄进程仅仅发生在进程刚被创建时，如果此后父进程又创建了新的内核对象，那么此时子进程不会继承这些新创建的内核对象句柄。

​    如果父进程创建了一个内核对象，得到一个不可继承的句柄，但是后来父进程又希望后来创建的子进程继承它，这怎么办呢？这可以通过使用SetHandleInformation修改内核对象句柄的继承标志 。它需三个参数，第一个标识了一个有效句柄，第二个标识想更改哪些标识。第三个标识指出想把它设成什么。这个标识可以是

```cpp
HANDLE_FLAG_INHERI，//打开句柄继承标识。
HANDLE_FLAG_PROTECT_FROM_CLOSE//不允许关闭句柄。
```

​    GetHandleInformation可以用来返回指定句柄的当前标识。

​    GetHandleInformation可以用来返回指定句柄的当前标识。

###     跨进程共享内核对象方法之二：命名对象


​    许多对象都可以进行命名，但并不是全部。因此该方法有一定局限性。有些创建内核对象的函数都有一个指定内核对象名称的参数，如果传入NULL，则会创建一个匿名的内核对象。如果不为NULL，则应该传入一个一'\0'结尾的字符串。所有这些命名对象共享一个名字空间。即使它们类型不同，如果已存在同名对象，创建就会失败。

​    一旦一个命名的内核对象被创建，其他进程（不仅仅是子进程）可以通过调用Open*或是Create*函数来访问它。当使用Create*函数时，系统会检查是否存在一个传给此函数的名字，如果确实存在一个这样的对象，内核执行安全检查，验证调用者是否有足够的安全权限。如果是，系统就会在此进程的句柄表中查找空白记录项，并将其初始化为指向已存在的命名的内核对象。两个进程的句柄不一定相同，这没有任何影响。由于内核对象被再一次引用，所以其引用计数会被递增。

​    为了防止在创建一个命名对象时，仅仅打开了一个现有的而不是新建的，可以在创建后调用GetLastError获得详细信息。

​    使用Open*函数可以打开已存在的命名内核对象，如果没有找到这个名称的内核对象将返回NULL。如果找到这个名称的内核对象，但类型不同，函数仍返回NULL。只有当名称相同且类型相同的情况下系统才会进一步检查访问权限。如果有权访问，系统就会更新此进程的句柄表，并递增内核对象的引用计数。在Open*函数中也可以指定此句柄的继承性。

​    Open*和Create*的区别：如果对象不存在，Create*会创建它，Open*将会调用失败。

​    我们经常使用命名的内核对象来防止运行一个程序的多个实例。可以在main函数中建立一个命名对象，返回后调用GetLastError如果GetLastError返回ERROR_ALREADY_EXISTS表明此程序的另一个实例在运行。

​     关于终端服务命名空间不再介绍，只需知道它是为了防止命名内核对象命名冲突而设计的。以后有需要的可以仔细研究下。

###     跨进程共享内核对象方法之三：复制对象句柄

​    实现该方法使用的是Duplicatehandle函数。

```cpp
bool DuplicateHandle(
   HANDLE hSourceProcessHandle,
HANDLE hSourceHandle,
HANDLE hTargetProcessHandle,
PHANDLE phTargethandle
DWORD ddwDesiredAccess,
BOOL bInheritHandle,
DWORD dwOptions
);
```

这个函数的功能就是获得进程句柄表的一个记录项，然后在另一个进程中创建这个记录项的副本。第一个和第三个参数分别标识源进程和目标进程内核对象句柄。第二个参数标识要复制的内核对象句柄，它可以指向任何类型的内核对象。第四个参数是一个目标句柄的地址，用来接收复制到的HANDLE值。

函数将源进程中的句柄信息复制到目标进程所标识的句柄表中。第五第六个参数用以指定此内核对象句柄在目标进程句柄表中应该使用何种访问掩码和继承标志。

dwOption参数可以是DUPLICATE_SAME_ACCESS和DUPLICATE_CLOSE_SOURCE任一个。如果是DUPLICATE_SAME_ACCESS标志，将向DuplicateHandle函数表明我们希望目标句柄拥有与源进程句柄一样的访问掩码，此时会忽略dwDesiredAccess。如果是DUPLICATE_CLOSE_SOURCE标志，会关闭源进程的句柄，此时将一个内核对象从一个进程复制到另一个进程，但是内核对象的使用计数不受影响。

​     GetCurrentProcess可以返回当前进程的句柄，但是它是一个伪句柄。其值为-1，GetCurrentThread返回的也是伪句柄其值为-2，它们并不在句柄表中而仅仅代表当前进程和当前线程。
​     这一章很抽象，原来学习的时候读了很多遍也不是很明白，后来干脆跳过去了，一段时间的学习之后再回来看看，发现竟然非常简单。所以有时候学习不能钻牛角尖该跳过就跳过。随着学习的深入，你所站的高度、看问题的角度都会不一样。理解起来也会更容易！！！











# （四） 进程相关





1、Windwos程序分为GUI程序和Console程序，通过设置VS的链接器开关/SUBSYSTEM:XX来设置，/SUBSYSTEM：CONSOLE控制台程序，基于命令行，/SUBSYSTEM：

WINDOWS GUI界面程序，基于窗口。



2、使用VS编写Windows程序会在程序的开始是C/C++运行库启动函数，它初始化了一些全局变量，参见<http://blog.csdn.net/wangpengk7788/article/details/53914526>



3、实例句柄，实例句柄的本质是一个模块被加载到进程地址空间中的基地址，例如，通常我们写的EXE程序在运行时候被加载到（32位环境下）以0x00400000开始的地方，DLL文件被加载到0x00100000开始地址的地方，这些基地址就是实例句柄，EXE文件的实例句柄是HINSTANCE通过main函数的参数传递进来，  DLL文件的是HMOUDLE在DllMain中做为参数传递进来，他们在本质上是一样的因此可以互相转换



```cpp
HMODULE WINAPI GetModuleHandle(
  __in_opt  LPCTSTR lpModuleName
);
```

该函数可以检查某一个名字的模块是否被加载到了进程中，如果已加载返回加载的基地址HMODULE，如果给该API传递NULL参数，返回的是主调进程的可执行文件的基地址





4、在_tWinMain中进程的前一个实例句柄，是为了兼容16位系统，已废弃，我们可以不为其制定参数名，并用UNREFERENCE_PARAMETER宏消除编译器警告



5、可以调用GetCommandLine函数获取进程的命令行参数，C运行库的代码会将去掉可执行文件路径以后的命令参数传递给WinMain函数。

在Shell32.dll中导出的CommandLineToArgvW函数可以将命令行参数转换为，参数数组的形式

```cpp
#include <windows.h>
#include <stdio.h>
#include <shellapi.h>
 
int __cdecl main()
{
   LPWSTR *szArglist;
   int nArgs;
   int i;
 
   szArglist = CommandLineToArgvW(GetCommandLineW(), &nArgs);
   if( NULL == szArglist )
   {
      wprintf(L"CommandLineToArgvW failed\n");
      return 0;
   }
   else for( i=0; i<nArgs; i++) printf("%d: %ws\n", i, szArglist[i]);
 
// Free memory allocated for CommandLineToArgvW arguments.
 
   LocalFree(szArglist);
 
   return(1);
}
 
```







6、每个进程都有一个与它相关联的环境块。用以定义工作环境、保存有用信息，使系统获得相关设置。应用程序经常利用环境变量让用户精细其行为。用户创建一个环境变量并进行初始化，此后应用程序运行时会正在环境块中查找变量，如果找到变量就会解析变量的值，并调整自己的行为。它所占用的内存是在进程地址空间内分配的。同样调用GetEnvironmentStrings函数可以获得完整的环境块。通常子进程会继承一组环境变量，这些环境变量和父进程的环境变量相同，父进程可以控制那些环境变量允许子进程继承。注意子进程继承的仅仅是父进程环境变量的副本，它们不共享同一个环境块。GetEnvironmentVariable函数可以用来判断一个环境变量是否存在。
在下图中我们可以看到形如%USERPROFILE%的字符串，它表示两个%之间的这部分内容是一个可替换的变量。该变量在环境变量中已经被定义。


    可以使用SetEnvironmentVariable来添加、删除或修改一个变量。
Windows不建议使用入口函数的参数来访问命令行或是环境变量，而应该使用以上介绍的各种函数。应该将它们当做只读变量，不要对它们进行修改。
     在多处理器的系统中，可以强迫线程在某个cpu上运行，这成为处理器关联性。子进程继承了其父进程的关联性。
     如果不提供完整的路径名，windows函数就会在当前驱动器的当前目录查找文件和目录。如：调用CreateFile打开一个文件时，如果仅指定文件名，系统将在当前驱动器和目录查找该文件。

系统在内部跟踪记录这一个进程当前驱动器和目录，这些信息是以进程为单位来维护的，如果该进程的一个线程更改了当前驱动器和目录，则只影响本进程的所有线程。

     一个线程可以使用GetCurrentDirectory和SetCurrentDirectory来获得和设置当前驱动器和目录。子进程的当前目录默认为每个驱动器的根目录。如果父进程希望子进程继承它的当前目录，就必须在生成子进程之前，添加环境变量。

使用GetVersionEx可以获得window系统的版本号。







7、进程的错误模式，每个进程都关联的了一组标志，这些标志的作用是让系统知道进程是如何响应严重错误的。

Value Meaning 
0 Use the system default, which is to display all error dialog boxes.

SEM_FAILCRITICALERRORS
0x0001 The system does not display the critical-error-handler message box. Instead, the system sends the error to the calling process.
                    系统不显示错误对话框，相反，系统返回错误代码给主调进程
SEM_NOALIGNMENTFAULTEXCEPT
0x0004 The system automatically fixes memory alignment faults and makes them invisible to the application. It does this for the calling process and any descendant processes. This feature is only supported by certain processor architectures. For more information, see the Remarks section. 
系统自动修复内存对齐，并使应用程序看不到这些错误
After this value is set for a process, subsequent attempts to clear the value are ignored.

SEM_NOGPFAULTERRORBOX
0x0002 The system does not display the general-protection-fault message box. This flag should be set only by debugging applications that handle general protection (GP) faults themselves with an exception handler.

系统不显示常规保护错误对话框，这个标志应该由调试中的程序来设置，该调试程序用一个异常处理例程来处理这些常规错误。

SEM_NOOPENFILEERRORBOX
0x8000 The system does not display a message box when it fails to find a file. Instead, the error is returned to the calling process.
                 系统在查找文件失败市不显示错误对话框



8、当调用CreateFile来打开一个文件是，提供的是一个不完整的路径名，那么系统会在当期驱动器和目录中查找，系统在内部跟踪记录着每一个进程的当前驱动器和目录。



```cpp
DWORD WINAPI GetCurrentDirectory(
  __in   DWORD nBufferLength,
  __out  LPTSTR lpBuffer
);
```

驱动器的当前目录保存在环境变量块中。







```cpp
DWORD WINAPI GetFullPathName(
  __in   LPCTSTR lpFileName,
  __in   DWORD nBufferLength,
  __out  LPTSTR lpBuffer,
  __out  LPTSTR *lpFilePart
);
```





9、 GetVersion和GetVersionEx获取系统版本信息，参见MSDN







**CreateProcess 创建进程的API**





```cpp
BOOL WINAPI CreateProcess(
  __in_opt     LPCTSTR lpApplicationName,
  __inout_opt  LPTSTR lpCommandLine,
  __in_opt     LPSECURITY_ATTRIBUTES lpProcessAttributes,
  __in_opt     LPSECURITY_ATTRIBUTES lpThreadAttributes,
  __in         BOOL bInheritHandles,
  __in         DWORD dwCreationFlags,
  __in_opt     LPVOID lpEnvironment,
  __in_opt     LPCTSTR lpCurrentDirectory,
  __in         LPSTARTUPINFO lpStartupInfo,
  __out        LPPROCESS_INFORMATION lpProcessInformation
);
```



一个线程调用CreateProcess时，系统将创建一个进程内核对象（一个存在于内核的结构体对象实例，系统用它来管理进程，该结构存放于进行相关的一些信息），并将引起计数设置为，然后系统为新创建的进程分配进程虚拟地址空间，并将可执行文件（EXE和其所依赖的DLL）的代码和数据加载到进程的地址空间中。

系统为新进程的主线程创建一个线程内核对象（同样是一个内核结构体的对象实例，系统用来管理线程，该结构存放于线程相关的一些信息），并将引用计数设置为1，这个主线程一个开始就会运行，线程地起始地址就是EXE文件PE结构中的入口点对应的虚拟内存地址，如果是VS编写的EXE，那么入口点地址就是C/C++运行时库的启动代码，它在初始化工作完成后，调用我们程序的Main函数。



参数：



1、lpCommandLine，字符串类型，作为新进程的命令行参数，在实现中系统会修改这个传入的字符串，因此应该避免使用放在PE静态区的字符串做为此参数的实参。

可以使用该参数指定一个完整的命令行，这样函数会以该参数来创建一个新进程，系统会检查字符串的第一个标记，并假定次标识使我们想运行的可以执行文件的名称，然后按照以下顺序搜索可执行文件

（1）主调进程EXE文件所在的目录

（2）主调进程的当前目录

（3）Windows目录

（4）PATH环境变量中列出的目录

如果包含的是一个完整的绝对路径则不会进程搜索，只要lpApplicationName参数为NULL就会发生这样的情况。



2、lpApplicationName，如果该参数不是一个完整路径，则在当期目录中查找，否则应该指定一个完整的路径名，该参数必须指定可执行文件的扩展名如.exe

当该参数不为空时lpCommandLine做为新进程的参数。

3、lpProcessAttributes,和lpThreadAttributes  bInheritHandles      关于返回的进程句柄是否可以被 再次创建的进程 继承，不做讨论



4、dwCreationFlags  ，参见MSDN，比较重要的有，CREATE_SUSPENDED  创建后挂起主线程，DEBUG_PROCESS创建一个用于调试的进程



5、lpEnvironment，为新进程指定环境变量块，如果为NULL则继承父进程的环境变量，



6、lpCurrentDirectory ，如果为NULL ，新进程使用跟父进程一样的当前驱动器和目录。



7、lpStartupInfo，STARTUPINFO 或者 STARTUPINFOEX 结构体指针，用来设置创建新的新进程的一些信息，可以再新进程中调用GetStartupInfo来获取该结构。



```cpp
typedef struct _STARTUPINFO {
  DWORD  cb;  结构体大小
  LPTSTR lpReserved;  保留
  LPTSTR lpDesktop;   在哪个桌面启动程序，如果桌面存在则于进程关联，不存在则创建桌面，如果为NULL与当前桌面关联
  LPTSTR lpTitle;      控制窗口标题，为NULL则 窗口标题为EXE的名字
  DWORD  dwX;          窗口位置的X坐标
  DWORD  dwY; 		窗口位置的Y坐标
  DWORD  dwXSize;	窗口的宽度
  DWORD  dwYSize;	窗口的高度    这两个在创建窗口时使用CW_USERDEFAULE时起作用
  DWORD  dwXCountChars;
  DWORD  dwYCountChars;	控制台窗口的高度和宽度，用字符数表示	
  DWORD  dwFillAttribute;	控制台的文本和背景色	
  DWORD  dwFlags;          
  WORD   wShowWindow;          在dwFlags为 STARTF_USESHOWWINDOWS是起作用，在调用ShowWindows时用SW_SHOWDEFAULET作参数时 用该值标识显示模式
  WORD   cbReserved2;
  LPBYTE lpReserved2;          保留，在C运行库初始化时有用到
  HANDLE hStdInput;            控制台的输入缓冲区句柄
  HANDLE hStdOutput;           缓冲区句柄	
  HANDLE hStdError;            缓冲区句柄
} STARTUPINFO, *LPSTARTUPINFO;
```



dwFlags 指示哪些结构中的内容起作用，参见MSDN







```cpp
typedef struct _STARTUPINFOEX {
  STARTUPINFO                 StartupInfo;
  PPROC_THREAD_ATTRIBUTE_LIST lpAttributeList;
} STARTUPINFOEX
```

第二项是一个指针  可已用InitializeProcThreadAttributeList来申请

```cpp
OOL WINAPI InitializeProcThreadAttributeList(
  __out_opt   LPPROC_THREAD_ATTRIBUTE_LIST lpAttributeList,   在返回中填充该指针
  __in        DWORD dwAttributeCount,  个数
  __reserved  DWORD dwFlags,  保留的，为0
  __inout     PSIZE_T lpSize    返回实际需要的字节数
);
```

在申请好以后用UpdateProcThreadAttribute填空内容







```cpp
BOOL WINAPI UpdateProcThreadAttribute(
  __inout    LPPROC_THREAD_ATTRIBUTE_LIST lpAttributeList,
  __in       DWORD dwFlags,  保留 为NULL
  __in       DWORD_PTR Attribute,
  __in       PVOID lpValue,   值  保留直到DeleteProcThreadAttributeList释放
  __in       SIZE_T cbSize,  值得大小
  __out_opt  PVOID lpPreviousValue, 保留 为NULL
  __in_opt   PSIZE_T lpReturnSize  保留 为NULL
);
```

其中 Attribute  用来指示 lpValue实际需要指向的数据的类型，书上介绍了两种 ，其他的参考MSDN



PROC_THREAD_ATTRIBUTE_HANDLE_LIST    lpValue指向一组句柄（子进程只能继承这组句柄），该组句柄必须是可继承的，而且不能是伪句柄





PROC_THREAD_ATTRIBUTE_PARENT_PROCESS      lpValue指向一个进程句柄，那么新的进程以该句柄标识的进程为父进程，同时新进程从该进程继承 句柄、处理器关联性、优先级、配额、用户令牌、关联的作业。’



在使用完这个申请到的空间之后 调用 DeleteProcThreadAttributeList  释放资源。



8、lpProcessInformation，PROCESS_INFORMATION结构体指针，CreateProcess函数返回过程中该结构被填充。

```cpp
typedef struct _PROCESS_INFORMATION {
  HANDLE hProcess;   //填充为新进程的句柄
  HANDLE hThread;    //填充为新进程的主线程句柄
  DWORD  dwProcessId;//新进程ID
  DWORD  dwThreadId; //主线程ID
} PROCESS_INFORMATION, *LPPROCESS_INFORMATION;
```

在不对子进程进行操作的情况下，经过关闭获取到进程和线程句柄。




**结束进程的4中方式**

1、没有创建新线程的情况下，主线程的入口函数返回

2、进程中的某一个线程调用ExitProcess

3、任何进程中的任何线程调用TerminateProcess，传递给它需要关心的进程的句柄，前提是需要有足够的权限。

4、进程中的所有线程“自然死亡”，线程执行完成退出





主线程的入口函数返回，应该做一下工作：该线程创建的所有对象由对象的析构函数销毁，操作系统释放线程栈使用的内存，系统将退出代码设为入口函数的返回值，系统递减进程内核对象的使用计数。

应避免使用TerminateProcess  ExitProcess  因为这两个函数在不通知进程中任何线程做退出工作的情况下，结束进程。





**当一个进程终止时系统做一下操作：**  

（一）终止进程中的所有线程

（二）释放所有用户对象，GDI对象，关闭所有内核对象（如果该内核对象没有在其他进程中被打开）

（三）进程的退出代码从STILL_CATIVE变为传给TerminateProcess  ExitProcess 的退出码

（四）进程内核对象变为触发状态

（五）进程内核对象使用计数递减1



进程内核对象生命周期可能长于进程，如果一个进程内核对象被其他进程打开，那么及时进程终止，进程地址空间被释放，进程内核对象仍然存在，直到内核对象引用计数为0

我们可以调用GetExitCodeProcess来获取进程的退出码，如果退出码是STILL_ACTIVE表示进程还在运行，否则表示进程已终止



```cpp
BOOL WINAPI GetExitCodeProcess(
  __in   HANDLE hProcess,
  __out  LPDWORD lpExitCode
);
```



**UAC 用户账户控制**



在VISTA及之后的系统中引入了UAC，系统会使用一个经过筛选后的令牌与每个由资源管理创建的进程关联在一起，使用这个筛选后的令牌没有访问受限制的资源的权限，必须选择以管理员身份运行，如果程序的可执行文件中嵌入了RT_MAINIFEST资源，

```xml
<trustInfo xmlns="urn:schemas-microsoft-com:asm.v3">
    <security>
        <requestedPrivileges>
            <requestedExecutionLevel
                level="highestAvailable"              //需要执行的权限
                uiAccess="false"
            />
        </requestedPrivileges>
    </security>
</trustInfo>
</assembly>
```



level="highestAvailable"     以当前最高权限运行，如果系统以管理员身份登录运行弹出提升权限对话框，如果以普通用户身份登录，程序以标准权限启动

level="requireAdministrator"  程序必须以管理员权限启动，弹出提升权限对话框

 level="isInvoker"  程序使用与主调进程同样的权限运行



使用 ShellExecuteEx 要求提升子进程权限





```cpp
typedef struct _SHELLEXECUTEINFO {
  DWORD     cbSize;
  ULONG     fMask;
  HWND      hwnd;
  LPCTSTR   lpVerb;
  LPCTSTR   lpFile;
  LPCTSTR   lpParameters;
  LPCTSTR   lpDirectory;
  int       nShow;
  HINSTANCE hInstApp;
  LPVOID    lpIDList;
  LPCTSTR   lpClass;
  HKEY      hkeyClass;
  DWORD     dwHotKey;
  union {
    HANDLE hIcon;
    HANDLE hMonitor;
  } DUMMYUNIONNAME;
  HANDLE    hProcess;
} SHELLEXECUTEINFO, *LPSHELLEXECUTEINFO;
```

如果 lpverb为runas  lpFile为可执行文件路径，弹出提升权限对话框，要求以管理员身份运行程序







```cpp
#include <Windows.h>
#include <ShellAPI.h>
 
int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	SHELLEXECUTEINFO sei = {0};
	sei.cbSize = sizeof(SHELLEXECUTEINFO);
	sei.lpVerb = TEXT("runas");
	sei.lpFile = TEXT("notepad.exe");
	sei.nShow = SW_SHOWNORMAL;
	if (!ShellExecuteEx(&sei))
	{
		DWORD erCode = GetLastError();
		if (erCode == ERROR_CANCELLED)
		{
			MessageBox(NULL,TEXT("User selected cancel!"),TEXT("Notes!"),NULL);
		}
		else
		{
			if (erCode == ERROR_FILE_NOT_FOUND)
			{
				MessageBox(NULL,TEXT("File to  executed don't found"),TEXT("Notes!"),NULL);
			}
		}
 
	}
 
 
	return 0;
```





使用GetTokenInformation获取进程当前令牌信息。





```cpp
BOOL WINAPI GetTokenInformation(
  __in       HANDLE TokenHandle,
  __in       TOKEN_INFORMATION_CLASS TokenInformationClass,
  __out_opt  LPVOID TokenInformation,
  __in       DWORD TokenInformationLength,
  __out      PDWORD ReturnLength
);
```

查看进程是否处于被提升权限下



```cpp
 BOOL GetProcessElevation(TOKEN_ELEVATION_TYPE * pElevationType,BOOL * pIsAdmin)
{
	DWORD dwLength;
	HANDLE hToken;
	BOOL ret= FALSE;
	if(!OpenProcessToken(GetCurrentProcess(),TOKEN_QUERY,&hToken))
	{
		return ret;
	}
	if (GetTokenInformation(hToken,TokenElevationType,pElevationType,sizeof(TOKEN_ELEVATION_TYPE),&dwLength))
	{
		BYTE adminSID[SECURITY_MAX_SID_SIZE];
		DWORD dwSize = sizeof(adminSID);
		CreateWellKnownSid(WinBuiltinAdministratorsSid,NULL,&adminSID,&dwSize);
		if (*pElevationType == TokenElevationTypeLimited)
		{
			HANDLE hLinkedToken;
			GetTokenInformation(hToken,TokenLinkedToken,&hLinkedToken,sizeof(HANDLE),&dwLength);
			if (CheckTokenMembership(hLinkedToken,&adminSID,pIsAdmin))
			{
				ret = TRUE;
			}
		}
		else
		{
			*pIsAdmin = IsUserAnAdmin();
			ret = TRUE;
		}
 
	}
	CloseHandle(hToken);
 
	return ret;
	
}
```











# （五） Job与进程的关系





Job可以被理解为进程的容器，而它又不是单纯的容器，按照书本上所讲的，Job是一个沙盒，它可以为在它里面运行的进程增加一系列的限制。包括每一个进程或者整个Job能够使用的User mode CPU时间，每一个进程或者整个Job最多能使用的内存，Job内的进程能否访问到Job外的用户对象（例如窗口，画刷），能否退出Windows，能否访问剪切板等等。当限制设定之后，我们就可以创建一个进程，并将它放置到Job之中。





需要注意的是：

1、Job对象即使引用数到了0也不会立刻释放，它会等到Job内所有的进程都结束了再释放，但是，在这种情况下，Job的名称将会失效，不能再通过Job的名称和Job的句柄来向Job中增加新的进程。



2、Job可以设置当前可运行进程最大数量。当超过这个最大数量时，任何新进程都将被立刻终止（Terminate）。



3、Job可以在同一进程优先级下设定调度的微调值（SchedulingClass）。微调值高的Job中的线程比微调值低的获得更多的CPU时间。



4、Job可以限制对用户对象以及Windows界面的访问，例如不许退出窗口，不能访问剪切板等。这意味着Job内的进程无法获取到Job外进程的对象，比如HWND，但是Job外的进程可以获得Job内进程的对象。



5、对Job设定了OBOBJECT_SECURITY_LIMIT_INFORMATION后，该设定不能被取消。





6、新建的进程最好使用CREATE_SUSPEND Flag，这样新进程启动后在加入到Job之前它都无法运行，从而避免新进程可以逃出沙盒。在加入到Job之后，继续运行进程即可。



7、当一个进程被加入到Job中后，没有特殊的说明，那么该进程的所有子进程都将被纳入到Job里。当然，通过JOB_OBJECT_LIMIT_BREAKAWAY_OK或者JOB_OBJECT_LIMIT_SILENT_BREAKAWAY_OK。Job内进程的子进程就可以不被自动纳入到Job中，两者区别在于，后者将所有子进程自动的赶出Job，而前者的子进程需要在CreateProcess时指定CREATE_BREAKAWAY_FROM_JOB。



8、如果Job被强制关闭，那么Job内的所有进程都将被终止（Terminate）



9、当Job所指定的CPU时间耗光后，Job HANDLE将处于Signaled状态，Wait系列函数就可以响应这个事件。





此外，任何Job中重要的事件，例如新进程的产生和终止，进程CPU时间的耗尽等等都可以通过完成端口的形式获得通知。（第五章第五小节）

\---------------------------------------------------

Job相关API介绍



1、CreateJobObject,  创建一个具名的Job对象



```html
HANDLE WINAPI CreateJobObject(
  __in_opt  LPSECURITY_ATTRIBUTES lpJobAttributes,
  __in_opt  LPCTSTR lpName
);
```



该API创建一个Job内核对象，可以为对象的名字前加上"Global\" or "Local\" 来指定名字是全局的还是局部的，该对象名字和其他类型的具名对象同在一个命名空间，如果名字冲突，会导致函数失败。



Job的所有工作机制都是围绕这个内核对象展开的，在内核中所有对Job包含的进程的限制，都是通过获取进程的内核对象中挂接的Job对象，根据Job对象的属性来限制进程所能够完成的操作，Job内核对象在WRK中是这样定义的：



```html
typedef struct _EJOB {
    KEVENT Event;
 
    //
    // All jobs are chained together via this list.
    // Protected by the global lock PspJobListLock
    //
 
    LIST_ENTRY JobLinks;//创建一个Job对象的时候，会将其挂入系统Job对象链表中，
 
    //
    // All processes within this job. Processes are removed from this
    // list at last dereference. Safe object referencing needs to be done.
    // Protected by the joblock.
    //
 
    LIST_ENTRY ProcessListHead; //进程链表头，AssignProcessToJobObject时，或者Job中的进程创建子进程，把进程内核对象挂入该表
    ERESOURCE JobLock;
 
    //
    // Accounting Info
    //
 
    LARGE_INTEGER TotalUserTime;
    LARGE_INTEGER TotalKernelTime;
    LARGE_INTEGER ThisPeriodTotalUserTime;
    LARGE_INTEGER ThisPeriodTotalKernelTime;   //统计已使用的用户和内核时间
    ULONG TotalPageFaultCount;   //统计页错误次数
    ULONG TotalProcesses;         //统计曾属于Job的进程数
    ULONG ActiveProcesses; //目前活动的进程数
    ULONG TotalTerminatedProcesses;//超出时间，终止了的进程数
 
    //
    // Limitable Attributes
    //
 
    LARGE_INTEGER PerProcessUserTimeLimit;  进程时间限制
    LARGE_INTEGER PerJobUserTimeLimit;   Job时间限制
    ULONG LimitFlags;          限制标志哪个起作用
    SIZE_T MinimumWorkingSetSize;
    SIZE_T MaximumWorkingSetSize;   //工作集
    ULONG ActiveProcessLimit;   //最大活动进程限制
    KAFFINITY Affinity;  //CPU集限制
    UCHAR PriorityClass;         //优先级
                            //对应JOBOBJECT_BASIC_LIMIT_INFORMATION
    //
    // UI restrictions
    //
 
    ULONG UIRestrictionsClass;   对应JOBOBJECT_BASIC_UI_RESTRICTIONS
 
    //
    // Security Limitations:  write once, read always
    //
 
    ULONG SecurityLimitFlags;
    PACCESS_TOKEN Token;
    PPS_JOB_TOKEN_FILTER Filter;                     对应JOBOBJECT_SECURITY_LIMIT_INFORMATION 
                                   
    //
    // End Of Job Time Limit
    //
 
    ULONG EndOfJobTimeAction;    对应JOBOBJECT_END_OF_JOB_TIME_INFORMATION
    PVOID CompletionPort;
    PVOID CompletionKey;    //对应JOBOBJECT_ASSOCIATE_COMPLETION_PORT
    ULONG SessionId;
 
    ULONG SchedulingClass;
 
    ULONGLONG ReadOperationCount;
    ULONGLONG WriteOperationCount;
    ULONGLONG OtherOperationCount;
    ULONGLONG ReadTransferCount;
    ULONGLONG WriteTransferCount;
    ULONGLONG OtherTransferCount;
 
    //
    // Extended Limits
    //
 
    IO_COUNTERS IoInfo;         // not used yet
    SIZE_T ProcessMemoryLimit;
    SIZE_T JobMemoryLimit;
    SIZE_T PeakProcessMemoryUsed;
    SIZE_T PeakJobMemoryUsed;
    SIZE_T CurrentJobMemoryUsed;                          对应JOBOBJECT_EXTENDED_LIMIT_INFORMATION
 
    KGUARDED_MUTEX MemoryLimitsLock;
 
    //
    // List of jobs in a job set. Processes within a job in a job set
    // can create processes in the same or higher members of the jobset.
    // Protected by the global lock PspJobListLock
    //
 
    LIST_ENTRY JobSetLinks;
 
    //
    // Member level for this job in the jobset.
    //
 
    ULONG MemberLevel;
 
    //
    // This job has had its last handle closed.
    //
 
#define PS_JOB_FLAGS_CLOSE_DONE 0x1UL
 
    ULONG JobFlags;
} EJOB;
typedef EJOB *PEJOB;
```

2、AssignProcessToJobObject







```html
BOOL WINAPI AssignProcessToJobObject(
  __in  HANDLE hJob,
  __in  HANDLE hProcess
);
```



将一个进程与Job关联，进程的行为将受到Job中的限制，在MSDN的描述中有几项需要注意





- 如果Job设置了时间限制并且时间已经耗尽，调用该函数失败进程也被终止，如果剩余的时间将要超出（剩余的时间小于调用该函数的所消耗的时间）进程终止，JOB将受到一个通知，如果Job设置了进程数限制且已经满了，函数失败进程终止
- Job设置的内存限制，在调用该函数前对进程是不起作用的
- 如果Job设置了安全限制，进程不能够满足，该函数调用失败，在将进程加入Job前应该以挂起的方式创建进程
- 已经存在于一个Job中的进程，调用该函数失败，进程只能同时存在于一个Job中
- 处于Job中的进程，在创建子进程的时候如果没有指定CREATE_BREAKAWAY_FROM_JOB 标志，那么子进程也处于该Job
- 在开启UAC的系统中，没有提示权限的进程会被默认加入一个 兼容性Job，该Job是一个系统的，如不想这样使用CREATE_BREAKAWAY_FROM_JOB 创建进程，或者脱离UAC的控制



3、SetInformationJobObject



```html
BOOL WINAPI SetInformationJobObject(
  __in  HANDLE hJob,
  __in  JOBOBJECTINFOCLASS JobObjectInfoClass, //枚举型型标识使用哪类限制
  __in  LPVOID lpJobObjectInfo,     //根据JobObjectInfoClass  ，使用对应的结构
  __in  DWORD cbJobObjectInfoLength //lpJobObjectInfo的大小
);
```

JOBOBJECTINFOCLASS 枚举值及所对应的lpJobObjectInfo指向的结构体





JobObjectAssociateCompletionPortInformation      The lpJobObjectInfo parameter is a pointer to a JOBOBJECT_ASSOCIATE_COMPLETION_PORT structure. 

 

JobObjectBasicLimitInformation     The lpJobObjectInfo parameter is a pointer to a JOBOBJECT_BASIC_LIMIT_INFORMATION structure. 

 

JobObjectBasicUIRestrictions     The lpJobObjectInfo parameter is a pointer to a JOBOBJECT_BASIC_UI_RESTRICTIONS structure. 

 

JobObjectEndOfJobTimeInformation     The lpJobObjectInfo parameter is a pointer to a JOBOBJECT_END_OF_JOB_TIME_INFORMATION structure. 

 

JobObjectExtendedLimitInformation     The lpJobObjectInfo parameter is a pointer to a JOBOBJECT_EXTENDED_LIMIT_INFORMATION structure. 

 

JobObjectSecurityLimitInformation    This flag is not supported. Applications must set security limitations individually for each process.

JOBOBJECT_BASIC_LIMIT_INFORMATION  基本的限制（包括每个进程的时间,Job时间，工作集，CPU集，调度优先级）具体参见MSDN

JOBOBJECT_EXTENDED_LIMIT_INFORMATION  在BASIC的基础上加了对内存的限制  参见MSDN

JOBOBJECT_BASIC_UI_RESTRICTIONS     对UI资源的限制，包括（创建桌面，设置显示，关机，原子表，获取窗口句柄，获取剪切板，设置系统参数）参见MSDN



MSDN中关于该函数有这样的描述



You must set security limitations individually for each process associated with a job object, rather than setting them for the job object itself. For information, see Process Security and Access Rights.  必须为每个进程单独的设置安全限制




4、 QueryInformationJobObject



```cpp
BOOL WINAPI QueryInformationJobObject(
  __in_opt   HANDLE hJob,
  __in       JOBOBJECTINFOCLASS JobObjectInfoClass,
  __out      LPVOID lpJobObjectInfo,
  __in       DWORD cbJobObjectInfoLength,
  __out_opt  LPDWORD lpReturnLength
);
```

 改函数获取关于Job的限制信息，如果想要对限制的某一项单独设置，可以先用该函数获取之后，修改某项再传给SetInformationJobObject



如果为函数第二个参数传递JobObjectBasicAccountingInformation    对应的lpJobObjectInfo  应该为JOBOBJECT_BASIC_ACCOUNTING_INFORMATION


如果传递JobObjectBasicAndIoAccountingInformation  对应的lpJobObjectInfo  应该为JOBOBJECT_BASIC_AND_IO_ACCOUNTING_INFORMATION



如果传递JobObjectBasicProcessIdList     对应的lpJobObjectInfo  应该为 JOBOBJECT_BASIC_PROCESS_ID_LIST

具体参见MSDN







# （六） 线程



现代操作系统都是多任务的，操作系统在不同的时间片内把CPU及硬件资源分配给不同的任务，在Windows系统中每一个任务就是一个线程，它是CPU执行调度的最小单元，而进程是线程的容器，为线程提供虚拟地址空间。

多线程环境可以让一个进程即一个应用程序将所要完成的任务进程分配，例如一个浏览器使用一个界面线程来完成与用户的交互，一个线程负责和服务器进行数据的传输，一个线程负责HTML代码的解析，一个线程负责脚本的解析等等。多线程的好处是显而易见的，我们可以的边看电影边聊天等等都是多线程的体现。然而并不是所有的需求都使用多线程来完成是最好的，多线程会带来新的需要处理的问题，例如对全局资源的访问可能造成数据破坏，Windwos的消息机制是基于线程的，因此两个线程创建各自的窗口只能捕获各自窗口的消息，因此需要合理的使用多线程编程，线程同步在多线编程中变得如此的重要。



CreateThread函数





```cpp
HANDLE CreateThread(
  __in   SEC_ATTRS SecurityAttributes,
  __in   ULONG StackSize,
  __in   SEC_THREAD_START StartFunction, //线程开始地址，通常是一个函数指针
  __in   PVOID ThreadParameter,  //传给新线程的参数
  __in   ULONG CreationFlags,
  __out  PULONG ThreadId
);
```

StackSize 为新线程分配的栈大小，如果为NULL，使用连接器 /stack 选项指定的大小， 应该为栈指定大小 来防止BUG



**线程的终止：**

1、使用ExitThread  ，系统清理线程所使用的资源，释放线程栈空间，但是不会释放线程中申请的堆空间，不会调用对象的析构函数,

2、TerminateThread ，不释放栈空间，不释放堆空间，不调用析构函数，函数返回并不保证线程已终止





当线程终止运行时：



- 线程拥有的用户对象，窗口，钩子被销毁
- 线程的退出代码从STILL_ACTIVE变成传给退出函数的退出码
- 线程内核对象变为触发状态
- 如果线程是进程中的最后一个活动线程，则进程终止
- 线程内核对象引用计数-1

**线程内幕：**



系统创建一个线程的时候，创建一个线程内核对象，系统通过线程内核对象管理线程，该对象开始时引入计数为2，返回线程内核对象句柄，一旦创建线程内核对象就为其分配栈内存，将新线程的参数 线程入口地址 先后放入栈，初始化线程上下文，CONTEXT结构，SP寄存器指向放入栈的线程入口地址，IP 指令指针 放入RtlUserThreadStart的地址，该函数位置NTDLL.DLL中。



![img](https://img-blog.csdn.net/20170109120344687?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2FuZ3BlbmdrNzc4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)







![img](https://img-blog.csdn.net/20170109120410484?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2FuZ3BlbmdrNzc4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





在线程被内核调度使用CPU就是从这里开始执行的，栈上已经放好了传过来的参数，然后这个函数会构建一个SEH结构异常，通过栈上的新线程地址和参数 执行新线程的代码，线程完成返回时，返回值传给ExitThread,线程终止，如果线程发生异常没有被线程内的异常处理程序消化掉，那么线程最外层的这个异常处理程序被调用，结束进程。





**使用C/C++运行库的 _beginthreadex**



如果程序是使用C/C++ 且使用微软的VS平台，那么应该使用_beginthreadex创建线程  使用_endthreadex 退出线程

这里可以参考<http://blog.csdn.net/wangpengk7788/article/details/53958387>





伪进程线程句柄



GetCurrentProcess();

GetCurrentThread();



这两个函数返回的是伪句柄，不增加内核对象的引用计数，将需要把伪句柄转换为真句柄是  需要使用DuplicateHandle  函数









# （七） 线程调度 优先级 关联性

2017年01月09日 14:15:28 [wangpengk7788](https://me.csdn.net/wangpengk7788) 阅读数：2482



 版权声明：本文为博主原创文章，未经博主允许不得转载。	https://blog.csdn.net/wangpengk7788/article/details/54287966

首先线程与系统时钟间隔参见<http://blog.csdn.net/wangpengk7788/article/details/54287947>

在抢占式多任务操作系统中，线程的运行是有限制的，系统会调度一个线程在一个时间块内占用CPU，在时间到了之后将线程的上下文(CONTEXT结构，保存线程切换前的CPU个寄存器的值）保存到线程内核对象中，从另一个可调度线程的CONTEXT中获取属于它的CPU各寄存器的值，设置CPU寄存器，执行该线程，这样重复着在不同的线程之间切换，保证每个可调度线程能得到执行时间。

​        在我使用的64位WIN7中  GetSystemTimeAdjustment

![img](https://img-blog.csdn.net/20170109132015330?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2FuZ3BlbmdrNzc4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





DWORD SuspendThread(HANDLE hThread   );  返回线程上次挂起的次数

挂起一个线程，一个线程最多可以挂起， 一个线程最多可以挂起MAXIMUM_SUSPEND_COUNT  （127）次， 被挂起的线程不参与线程调度

DWORD ResumeThread(HANDLE hThread   ); 返回上次挂起次数

递减线程内核对象中 线程挂起计数，只有在挂起减到0时，线程才能恢复运行



进程不能挂起，除了在调试器处理WaitForDebugEvent时，被调试的进程所有线程会挂起。



**Sleep函数**



1、调用Sleep函数使线程挂起，线程放弃剩余的时间片

2、Sleep设置的挂起时间只是近似值，不保证会准时醒来

3、可以传入INFINITE线程永久挂起

4、传入0表示放弃当前使用的剩余时间片



SwitchToThread   让出时间片，可能调度给比自己优先级低的线程





可以在一段代码的之前使用GetThreadTime获取线程执行的时间，取前后时间的差 做为该段代码的执行时耗。



在VISTA中为线程分配CPU时间的方式发生了变化 ，在新的系统中使用处理器时间戳计时器TSC，在线程被调度程序暂停时，将计算此时TSC的值与线程开始时的TSC值之间的差，然后在线程执行时间上加上该差值，不计算中断时间。我们可以使用ReadTimeStampCounter宏来获取TSC的值。



GetThreadContext 获取线程的上下文，这里CONTEXT结构对熟悉汇编的看起来会很亲切。Windwos 还是提供了SetThreadContext函数，这是很强大的对于喜欢做一些比较另类和猥琐的事情的人，比如可以创建一个挂起的进程比如系统的计算器，把另一个执行文件，比如系统的记事本程序自己加载到这个挂起的进程空间中去，然后改变主线程的CONTEXT，设置成记事本程序主线程入口所需要的环境，那么就可以借尸还魂了。在任务管理器里看到的是一个计算器程序，实际运行的代码是记事本的。





**线程的优先级**

在Windows中线程的优先级为0-31，优先级高的线程优先被调度，只有在优先级高的线程都处于不可调度状态时，才递减调度其次优先级的线程，当较高优先级的线程占用了CPU时间，致使较低优先级的线程无法运行时成为饥饿，在多处理器上饥饿的情况发生的可能性很小，系统大多数线程都是不可调度的，例如线程调用了GetMessage等待获取消息，如果这时消息队列中没有消息，系统会暂停这个线程，并将CPU分配给另一个等待调度的线程。

在较低优先级的线程被调度时，如果有较高优先级的线程已经处于可调度状态，系统会暂停较低优先级的线程，将CPU分给高优先级的线程，Windows的线程调度是抢占掠夺式的。

Windwos的进程优先级分为6个等级，可以通过SetPriorityClass来设置



```cpp
BOOL WINAPI SetPriorityClass(
  __in  HANDLE hProcess,
  __in  DWORD dwPriorityClass
);
```



dwPriorityClass对应的标志 实际优先级值

| 优先级             | 标志                        | 优先级值 |
| ------------------ | --------------------------- | -------- |
| idle   (低)        | IDLE_PRIORITY_CLASS         | 4        |
| Below （低于标准） | BELOW_NORMAL_PRIORITY_CLASS |          |
| normal （标准）    | NORMAL_PRIORITY_CLASS       | 7或9     |
| Above （高于标准） | ABOVE_NORMAL_PRIORITY_CLASS |          |
| high （高）        | HIGH_PRIORITY_CLASS         | 13       |
| realtime （实时)   | REALTIME_PRIORITY_CLASS     | 24       |



Windwos的线程有6个相对于进程的优先级，相对优先级使用 SetThreadPriority来设置





```cpp
BOOL WINAPI SetThreadPriority(
  __in  HANDLE hThread,
  __in  int nPriority
);
```

nPriority对应的标志 在进程优先级的基础上调整



| 线程优先级等级  | 标志                          | 优先级值                                         |
| --------------- | ----------------------------- | ------------------------------------------------ |
| idle （最低）   | THREAD_PRIORITY_IDLE          | 如果进程优先级为realtime则调整为16，其它情况为1  |
| LOWEST 低       | THREAD_PRIORITY_LOWEST        | -2（在原有基础上-2）                             |
| BELOW 低于标准  | THREAD_PRIORITY_BELOW_NORMAL  | -1（在原有基础上-1）                             |
| NORMAL（标准)   | THREAD_PRIORITY_NORMAL        | 不变(取进程优先级值)                             |
| ABOVE 高于标准  | THREAD_PRIORITY_ABOVE_NORMAL  | +1（在原有基础上+1）                             |
| HIGHEST（高）   | THREAD_PRIORITY_HIGHEST       | +2（在原有基础上+2）                             |
| CRITICAL（最高) | THREAD_PRIORITY_TIME_CRITICAL | 如果进程优先级为realtime则调整为31，其它情况为15 |



这些进程优先级和线程相对优先级在Windwos不同版本 所对应的优先级值得映射不有变化的。





当一个进程中的线程为了响应某种IO事件，比如窗口消息，磁盘读取，获取网络包，线程会在这时被系统动态提升为NORMAL级，且只能提升1~15级的线程。在线程提升优先级后的每个时间片内优先级会被递减，直到恢复原来的优先级。





可以使用SetProcessPriority 来设置是否允许系统动态提升线程优先级。使用GetProcessPriority来获取线程是否开启被提升。





**调度IO请求优先级**



在设置进程和线程优先级时可以为 这两个函数传入  对应的THREAD_MODE_BACKGROUND_BEGIN   PROCESS_MODE_BACKGROUND_BEGIN  让线程或进程进入低IO优先级状态，传入THREAD_MODE_BACKGROUND_END     PROCESS_MODE_BACKGROUND_END 进入NORMAL级状态。







**CPU关联性**





我们可以使用下满两个函数设置线程或进程运行于哪个CPU之上。dwProcessAffinityMask 用二进制位做掩码，比如3  代码运行在CPU0和CPU1上，5表示在CPU2和CPU0上



```cpp
BOOL WINAPI SetProcessAffinityMask(
  __in  HANDLE hProcess,
  __in  DWORD_PTR dwProcessAffinityMask
);
DWORD_PTR WINAPI SetThreadAffinityMask(
  __in  HANDLE hThread,
  __in  DWORD_PTR dwThreadAffinityMask
);
```











# （八）用户模式下的线程同步

2017年01月10日 07:06:19 [wangpengk7788](https://me.csdn.net/wangpengk7788) 阅读数：268



版权声明：本文为博主原创文章，未经博主允许不得转载。	https://blog.csdn.net/wangpengk7788/article/details/54301732



```cpp
Long a;  
DWORD WINAPI ThreadFunc2(PVOID pvParam)  
{  
   InterlockedExchangeAdd(&a,1);  
   Return 0;  
}  
```

第一部分  原子访问 





引述自<http://blog.csdn.net/ithzhang/article/details/8291027>

 系统中的线程必须访问系统资源，如堆、串口、文件、窗口以及其他资源。如果一个线程独占了对某个资源的访问，其他线程就无法完成工作。我们也必须限制线程在任何时刻都能访问任何资源。比如在一个线程读内存时要限制其他线程对此块内存进行写入。


      线程之间的通信很重要，尤其是在以下两种情况下：
      1：需要让多个线程同时访问一个共享资源，同时不能破坏资源的完整性。
      2：一个线程需要通知其他线程某项任务已经完成。


      线程同步包括许多方面，windows提供了许多基础设施使线程同步变得容易。
                                      用户模式下的线程同步：方法一，原子访问
      线程同步的一大部分与原子访问有关。所谓原子访问，指的是一个线程在访问某个资源的同时能够保证没有其他线程会在同一时刻访问统一资源。


    比如有全局变量a=0；有两个线程同时对一全局变量进行a++操作，然后返回结果。那么最后a是多少呢？让我们从汇编代码上分析一下。
    A++编译器会编译成两行代码：

```asm
Mov eax ,[a]  
Inc eax  
Mov [a],eax  
```

​     如果一切正常的情况下,两个线程顺序执行：

```asm
线程A
Mov eax ,[a]  
Inc eax  
Mov [a],eax  
 
线程B
Mov eax ,[a]  
Inc eax  
Mov [a],eax  
```

​       a的值是2，这是毫无疑义的。

​       但是由于windows是抢占式操作系统，系统可能在任何时刻暂停执行一个线程。基于此，前面的代码可能会按下面的顺序执行：





```asm
线程A
Mov eax,[a]  
Inc eax  
线程B
Mov eax,[a]  
Inc eax  
Mov [a],eax  
 
线程B
Mov [a],eax  
```

​      如果按照这种顺序执行，a的值最终应该是1，而不是2。这看起来很不可思议。但是由于我们无法对windows的调度进行控制，在这样的一个环境编程会让程序员崩溃。

​      为了解决这个问题，我们需要一种方法能够保证对一个值的递增操作是原子操作。Interlocked系列函数提供了我们需要的解决方案。所有这些函数会以原子方式来操控一个值。

```cpp
LONG InterlockedExchangeAdd(  
  PLONG volatile plAddend,  
  LONG lIncrement);  
LONGLONG InterlockedExchangeAdd64(  
  PLONGLONG volatile pllAddend,  
  LONGLONG llIncrement);  
```

​      为上述两个函数传入一个长整形变量的地址和一个增量值，函数就会保证递增操作是以原子方式进行的。

​      前面的代码可以改成下面的代码：

​       改成上述代码后，对a的递增都是以原子方式进行。也就可以保证a的最终结果为2。如果只想以原子方式给一个值加1的话，可以使用InterlockedIncrement函数。

​       这些Interlock系列函数的工作机制取决于代码运行的cpu平台。如果是x86平台，那么Interlock函数会在总线上维持一个硬件信号，这个信号会阻止其他cpu访问同一个内存地址。

​      我们并不需要理解Interlock函数是如何工作的。无论编译器如何生成代码，无论机器上装了多少cpu，这些函数都能保证对值的修改是以原子方式进行的。

Interlock系列函数执行速度极快。通常只需要几个cpu周期，而且不需要在用户模式和内核模式进行切换。

使用这些函数也可以做减法，只要传入负值即可。

​       如果需要以原子方式执行变量交换可以调用：

```cpp
LONG InterlockedExchange(  
   PLONG volatile  plTarget,  
   LONG lValue);  
LONGLONG InterlockedExchange64(  
   PLONGLONG volatile  plTarget,  
   LONGLONG lValue);  
PVOID InterlockedPointer(  
   PVOID *volatile ppvTarget，  
   PVOID pvValue);  
```

​       InterloackedExchange和InterlockedExchangePointer会把第一个参数所指向的内存地址的当前值以原子方式替换为第二个参数指定的值

​      对于32位应用程序来说，它们都是替换的32位的值。而对于64位的应用程序InterlockedExchangePointer是替换的64位的值。因为此时指针也是64位的。

​       这两个函数都返回原来的值，在实现旋转锁时，InterlockedExchange函数非常有用。

```cpp
Bool use=false;  
Void func()  
{   
   While(InterlockedExchange(&use,true)==true)  
     Sleep(0);  
//........  
InterlockedExchange(&use,false);  
}  
```

​      While循环不停的执行，把use的值设为true并检查原来的值是否为true。如果原来的值为false，则说明资源尚未被使用。于是将其设为使用中。如果原来的值是true则表明有其他线程正在使用该资源，调用sleep放弃该时间片的调度。

​      这里假定所有使用旋转锁的线程都是以相同的优先级运行的。但是对于单cpu的系统来说使用旋转锁是没有意义的。此处在检测到资源被占用时使用了sleep，可以睡眠一定随机的时间，这在一定程度上缓解了这种状况。

​      旋转锁假定被保护的资源只会被占用一段时间，与切换到内核模式然后等待相比，这种情况下以循环方式进行等待的效率会更高。许多开发人员会指定循环的次数，如果届时仍然无法访问资源，那么线程会切换到内核模式，并一直等待到资源可供使用为止。这就是关键段的使用方式。

​      在多处理器上使用旋转锁很有用，这是因为当一个线程在一个cpu上运行时，另一个线程可以再另一个cpu上循环等待。再次提醒，在单cpu的系统上循环锁毫无意义。

​      当cpu从内存中读取一个字节的时候，它并不是从内存中读取一个字节，而是取回一个高速缓存行。高速缓存行可能是32字节或是64字节。这取决于cpu。高速缓存行存在的目的是为了提高性能。这是根据程序的局部性原理。如果所有字节都在高速缓存行内那么cpu就不需要访问内存总线。





​      对于多处理器环境，高速缓存使得对内存的更新变得更加困难：

​       1：cpu1读取一个字节，这使得该字节以及与它相邻的字节被读取到cpu1的高速缓存中。

​       2：cpu2读取到同一字节，这使得该字节被读到cpu2的高速缓存中。

​       3：cpu1对内存中的这个字节进行修改，这使得该字节被写入到cpu1的告诉缓存中，并没有写回内存。

​       4：cpu2再次读取到同一字节。由于该字节已经在cpu2的高速缓存中，因此cpu2不需要在访问内存。但cpu2无法看到该字节在内存中的新值。



​      上述情况非常糟糕。但是cpu芯片的设计者早就考虑到了这种情况。当一个cpu修改了高速缓存行的一个字节时，机器中的其他cpu会收到通知，并将自己的高速缓存行作废。因此在刚才的情形中，cpu2的高速缓存就作废了。Cpu1必须将它的高速缓存写回内存，cpu2必须重新访问内存来填满它的高速缓存行。我们可以看到虽然高速缓存可以提高性能，但是在多处理器机器上它同样能够损伤性能。



​      为了提高性能，我们应该根据高速缓存的大小来将应用程序的数据组织在一起，将数据与缓存行的边界对齐。并把只读数据与可读写数据分别存放。

​      使用GetLogicalProcessorInformaiton函数可以获得cpu高速缓存行的大小。我们可以使用_declspec(align(#))指示符来对字段对齐加以控制。说了那么多的措施，其实最好的方法就是始终让一个cpu访问数据或只让一个线程访问数据就可以完全避免高速缓存行的问题。



​    如果我们只需要以原子方式修改一个值，那么Interlock系列函数非常好用。但是大多数情况下我们需要处理的数据结构往往要比一个简单的32位值或64位值复杂的多。为了能够以原子的方式来访问复杂的数据结构，我们可以使用windows使用的其他特性。



​      使用旋转锁不停的访问是非常糟糕的一种方式，最好的情况下就是当线程想要访问共享资源时，线程必须调用操作系统的一个函数，告诉操作系统自己等待什么资源。如果此时资源不可用，此线程将会进入等待状态，不可被调度。如果操作系统检测到资源已经可以使用了，系统就会将此线程设为可调度状态并访问此资源。



​       volatile关键字告诉编译器这个变量可能会被应用程序之外的其他东西修改，不要对该变量做任何形式的优化，而是始终从内存中读取此值。如果不对一个变量加上volatile限定符，编译器可能会对C++代码进行优化，如它会将变量值载入到寄存器中，以后的操作都是对寄存器中的值进行操作，并不会多次访问内存。对一个结构加上volatile限定符等于给结构中的所有成员都加入volatile限定符。





**第二部分 关键代码段**



关键段 （critical section）是一小段代码，它在执行之前需要独占对一些共享资源的访问权。这种方式可以让多行代码以“原子方式”来对资源进行操控。即，代码知道除了当前线程之外，没有任何线程会同时访问该资源。当然，系统仍然可以暂停当前线程去调度其它线程。但是，在当前线程离开关键段之前，系统是不会去调度任何想要访问同一资源的其它线程的。


    一般情况下，我们会将CRITICAL_SECTION结构作为全局变量来分配，这样进程中的所有线程就能够非常方便地通过变量名来访问这些结构。在使用 CRIICAL_SECTION的时候，只有两个必要条件：第一条件是所有想要访问资源的线程必须知道用来保护资源的CRITICAL_SECTION结构的地址（我们可以通过自己喜欢的任何方式来把这个地址传给各个线程）。第二个条件是在任何线程试图访问被保护的资源之前，必须对 CRITICAL_SECTION结构的内部成员进行初始化。


下面这个函数用来对结构进行初始化：



```cpp
VOID InitializeCriticalSection(PCRITICAL_SECTION pcs);
BOOL WINAPI InitializeCriticalSectionAndSpinCount(  out  LPCRITICAL_SECTION lpCriticalSection,  in   DWORD dwSpinCount);
```





当知道线程不再需要访问共享资源的时候，我们应该调用下面的函数来清理CRITICAL_SECTION结构：   VOID DeleteCriticalSection(PCRITICAL_SECTION pcs); 

然后我们在以下两个函数之间访问共享资源：   VOID EnterCriticalSection(PCRITICAL_SECTION pcs);     。。。共享资源的访问。。。   VOID LeaveCriticalSection(PCRITICAL_SECTION pcs); 



分析下 关键段是如何工作的，这里分析的代码来自React OS

CRITICAL_SECTION结构是这样的



```cpp
typedef struct _CRITICAL_SECTION {
	PCRITICAL_SECTION_DEBUG DebugInfo;
	LONG LockCount;   
	LONG RecursionCount;
	HANDLE OwningThread;
	HANDLE LockSemaphore;
	ULONG_PTR SpinCount;
} CRITICAL_SECTION
```



初始化过程



```cpp
BOOL
WINAPI
InitializeCriticalSectionAndSpinCount(OUT LPCRITICAL_SECTION lpCriticalSection,
                                      IN DWORD dwSpinCount)     带旋转锁的初始化
{
    NTSTATUS Status;
 
 
    /* Initialize the critical section */
    Status = RtlInitializeCriticalSectionAndSpinCount((PVOID)lpCriticalSection,
                                                      dwSpinCount);
    if (!NT_SUCCESS(Status))
    {
        /* Set failure code */
        BaseSetLastNTError(Status);
        return FALSE;
    }
 
 
    /* Success */
    return TRUE;
}
VOID
WINAPI
InitializeCriticalSection(OUT LPCRITICAL_SECTION lpCriticalSection)   不带旋转锁的初始化
{
    NTSTATUS Status;
 
    /* Initialize the critical section and raise an exception if we failed */
    Status = RtlInitializeCriticalSection((PVOID)lpCriticalSection);
    if (!NT_SUCCESS(Status)) RtlRaiseStatus(Status);
}
 
 
NTSTATUS
NTAPI
RtlInitializeCriticalSection(PRTL_CRITICAL_SECTION CriticalSection)
{
    /* Call the Main Function */
    return RtlInitializeCriticalSectionAndSpinCount(CriticalSection, 0);
}
 
 
 
NTSTATUS
NTAPI
RtlInitializeCriticalSectionAndSpinCount(PRTL_CRITICAL_SECTION CriticalSection,
                                         ULONG SpinCoun)t                              两种初始化都由这一个函数完成
                                         
                                         {
    PRTL_CRITICAL_SECTION_DEBUG CritcalSectionDebugData;
 
    /* First things first, set up the Object */
    DPRINT("Initializing Critical Section: %p\n", CriticalSection);
    CriticalSection->LockCount = -1;                                               注意这里是-1
    CriticalSection->RecursionCount = 0;
    CriticalSection->OwningThread = 0;
    CriticalSection->SpinCount = (NtCurrentPeb()->NumberOfProcessors > 1) ? SpinCount : 0;    //如果CPU是单核的使用0次循环等待，如果不是使用传入的
    CriticalSection->LockSemaphore = 0;
 
    /* Allocate the Debug Data */
    CritcalSectionDebugData = RtlpAllocateDebugInfo();            //申请一块用于调试数据的堆空间 如果申请失败，函数返回STATUS_NO_MEMORY，因此在内存不足的
    DPRINT("Allocated Debug Data: %p inside Process: %p\n",         的机器上可能造成初始化的失败。参见InitializeCriticalSection的MSDN说明
           CritcalSectionDebugData,
           NtCurrentTeb()->ClientId.UniqueProcess);
 
    if (!CritcalSectionDebugData)
    {
        /* This is bad! */
        DPRINT1("Couldn't allocate Debug Data for: %p\n", CriticalSection);
        return STATUS_NO_MEMORY;
    }
 
    /* Set it up */
    CritcalSectionDebugData->Type = RTL_CRITSECT_TYPE;
    CritcalSectionDebugData->ContentionCount = 0;
    CritcalSectionDebugData->EntryCount = 0;
    CritcalSectionDebugData->CriticalSection = CriticalSection;
    CritcalSectionDebugData->Flags = 0;
    CriticalSection->DebugInfo = CritcalSectionDebugData;           初始化调试数据
 
    /*
     * Add it to the List of Critical Sections owned by the process.
     * If we've initialized the Lock, then use it. If not, then probably
     * this is the lock initialization itself, so insert it directly.
     */
    if ((CriticalSection != &RtlCriticalSectionLock) && (RtlpCritSectInitialized))
    {
        DPRINT("Securely Inserting into ProcessLocks: %p, %p, %p\n",
               &CritcalSectionDebugData->ProcessLocksList,
               CriticalSection,
               &RtlCriticalSectionList);
 
        /* Protect List */
        RtlEnterCriticalSection(&RtlCriticalSectionLock); //在一个时间内只能有一个线程，将初始化的关键段对象加入到进程的 关键段对象链表中，
 
        /* Add this one */
        InsertTailList(&RtlCriticalSectionList, &CritcalSectionDebugData->ProcessLocksList);
 
        /* Unprotect */
        RtlLeaveCriticalSection(&RtlCriticalSectionLock);
    }
    else
    {
        DPRINT("Inserting into ProcessLocks: %p, %p, %p\n",
               &CritcalSectionDebugData->ProcessLocksList,
               CriticalSection,
               &RtlCriticalSectionList);
 
        /* Add it directly */
        InsertTailList(&RtlCriticalSectionList, &CritcalSectionDebugData->ProcessLocksList);
    }
 
    return STATUS_SUCCESS;
}
```

  在这里 除了 CriticalSection->LockCount = -1;      CriticalSection->SpinCount = (NtCurrentPeb()->NumberOfProcessors > 1) ? SpinCount : 0;  设置了循环次数

   

```cpp
CritcalSectionDebugData->Type = RTL_CRITSECT_TYPE;
CritcalSectionDebugData->CriticalSection = CriticalSection;
CriticalSection->DebugInfo = CritcalSectionDebugData;
```

设置了调试数据结构  其他的都是初始化成了0



  if ((CriticalSection != &RtlCriticalSectionLock) && (RtlpCritSectInitialized))

这里引用到了三个全局静态变量   RtlCriticalSectionLock是一个关键段对象的指针，该对象用来在线程把初始化好的关键段对象加入进程关键段对象链表中的时候做线程同步使用，RtlpCritSectInitialized 一个BOOL型的全局变量，表示是否已经初始化了 RtlCriticalSectionLock



RtlCriticalSectionList   则是一个全局的双向链表的表头，该链表用来存放进程所有的已初始化的 关键段对象

static LIST_ENTRY RtlCriticalSectionList;



链表的节点结构

```cpp
typedef struct _LIST_ENTRY
{
    struct _LIST_ENTRY *Flink;
    struct _LIST_ENTRY *Blink;
} LIST_ENTRY,*PLIST_ENTRY;
```



关键段调试信息结构



```cpp
typedef struct _RTL_CRITICAL_SECTION_DEBUG {
  WORD Type;
  WORD CreatorBackTraceIndex;
  struct _RTL_CRITICAL_SECTION *CriticalSection;
  LIST_ENTRY ProcessLocksList;
  DWORD EntryCount;
  DWORD ContentionCount;
  DWORD Flags;
  WORD CreatorBackTraceIndexHigh;
  WORD SpareWORD;
}
```

这样就可以根据表头找到对应CritcalSectionDebugData->ProcessLocksList   然后根据它在结构体内的地址使用偏移量定位其他数据，PLIST_ENTRY+4 就是关键段对象的指针 struct _RTL_CRITICAL_SECTION *CriticalSection;









**EnterCriticalSection  做了些什么**



它调用了  RtlEnterCriticalSection

```cpp
NTSTATUS
NTAPI
RtlEnterCriticalSection(PRTL_CRITICAL_SECTION CriticalSection)
{
    HANDLE Thread = (HANDLE)NtCurrentTeb()->ClientId.UniqueThread;
    if (InterlockedIncrement(&CriticalSection->LockCount) != 0)  如果+1之后锁定次数不为0，   刚初始化完的是-1 这里循环不执行，如果是有线程在占用执行循环
    {
 
        if (Thread == CriticalSection->OwningThread)  如果是本线程占用
        {
 
            CriticalSection->RecursionCount++;     资源引用+1
            return STATUS_SUCCESS;
        }
 
 
        RtlpWaitForCriticalSection(CriticalSection); //执行等待
    }
 
    CriticalSection->OwningThread = Thread;   将拥有线程设置为本线程
    CriticalSection->RecursionCount = 1;       资源引用设置为1
    return STATUS_SUCCESS;
}
```



在来看看RtlpWaitForCriticalSection



创建事件内核对象



```cpp
NTAPI
RtlpCreateCriticalSectionSem(PRTL_CRITICAL_SECTION CriticalSection)
{
    HANDLE hEvent = CriticalSection->LockSemaphore;
    HANDLE hNewEvent;
    NTSTATUS Status;
 
    if (!hEvent)
{
        Status = NtCreateEvent(&hNewEvent, EVENT_ALL_ACCESS,NULL, SynchronizationEvent,FALSE);
        if (!NT_SUCCESS(Status))
        {hNewEvent = INVALID_HANDLE_VALUE;
        }
        if (InterlockedCompareExchangePointer((PVOID*)&CriticalSection->LockSemaphore,(PVOID)hNewEvent,NULL) != NULL)
        {
            if (hEvent != INVALID_HANDLE_VALUE)
            {
                DPRINT("Closing already created event: %p\n", hNewEvent);
                NtClose(hNewEvent);
            }
        }
    }
 
    return;
}
```

RtlpWaitForCriticalSection  执行等待





```cpp
NTSTATUS
NTAPI
RtlpWaitForCriticalSection(PRTL_CRITICAL_SECTION CriticalSection)
{
    NTSTATUS Status;
    EXCEPTION_RECORD ExceptionRecord;
    BOOLEAN LastChance = FALSE;
 
    /* Do we have an Event yet? */
    if (!CriticalSection->LockSemaphore)   
    {
        RtlpCreateCriticalSectionSem(CriticalSection);             如果事件内核对象没有创建，用上面的函数创建它
    }
 
    if (CriticalSection->DebugInfo)
        CriticalSection->DebugInfo->EntryCount++;                    增加等待线程的个数
 
 
    if (LdrpShutdownInProgress &&
        LdrpShutdownThreadId == NtCurrentTeb()->RealClientId.UniqueThread)
    {
        DPRINT("Forcing ownership of critical section %p\n", CriticalSection);
        CriticalSection->LockCount = 0;
        return STATUS_SUCCESS;
    }                                       如果进程正在关闭，解锁
 
    for (;;)
    {
        if (CriticalSection->DebugInfo)
            CriticalSection->DebugInfo->ContentionCount++;
 
        if (CriticalSection->LockSemaphore == INVALID_HANDLE_VALUE)  如果创建事件对象失败 使用系统的keyed event等待
        {
            Status = NtWaitForKeyedEvent(NULL,
                                         CriticalSection,
                                         FALSE,
                                         &RtlpTimeout);
        }
        else
        {
            Status = NtWaitForSingleObject(CriticalSection->LockSemaphore,   使用内核事件对象 等待
                                           FALSE,
                                           &RtlpTimeout);                      
        }                                                                         RtlpTimeout是一个全局的等待时间，书上说是3天
 
        if (Status == STATUS_TIMEOUT)
        {
            /* Is this the 2nd time we've timed out? */
            if (LastChance)
            {
                ERROR_DBGBREAK("Deadlock: 0x%p\n", CriticalSection);
 
                /* Yes it is, we are raising an exception */
                ExceptionRecord.ExceptionCode    = STATUS_POSSIBLE_DEADLOCK;
                ExceptionRecord.ExceptionFlags   = 0;
                ExceptionRecord.ExceptionRecord  = NULL;
                ExceptionRecord.ExceptionAddress = RtlRaiseException;
                ExceptionRecord.NumberParameters = 1;
                ExceptionRecord.ExceptionInformation[0] = (ULONG_PTR)CriticalSection;
                RtlRaiseException(&ExceptionRecord);                                       
            }
 
            /* One more try */
            LastChance = TRUE;                                                      如果时间超时抛出异常
        }
        else
        {
            /* If we are here, everything went fine */
            return STATUS_SUCCESS;                                                  有信号了返回
        }
    }
}
```



这里React OS  在用事件对象等待之前，没有使用旋转锁 循环尝试获取执行权，毕竟它不是真正的Windwos







再看 LeaveCriticalSection  对应的是 RtlLeaveCriticalSection(PRTL_CRITICAL_SECTION CriticalSection)





```cpp
NTSTATUS
NTAPI
RtlLeaveCriticalSection(PRTL_CRITICAL_SECTION CriticalSection)
{
#if DBG
    HANDLE Thread = (HANDLE)NtCurrentTeb()->ClientId.UniqueThread;
    if (Thread != CriticalSection->OwningThread)
    {
       return STATUS_INVALID_PARAMETER;    //如果本线程没有处于拥有关键段对象的状态，调用无效。
    }
#endif
    if (--CriticalSection->RecursionCount) //如果线程多次调用EnterCriticalSection，就需要多次调用LeaveCriticalSection
    {
        InterlockedDecrement(&CriticalSection->LockCount);      每次LockCount减1
 
    }
    else
    {                                                              //在LockCount减去为 -1时 表示可以让其他线程拥有而结束等待了
        CriticalSection->OwningThread = 0;
        if (-1 != InterlockedDecrement(&CriticalSection->LockCount))
        {
            RtlpUnWaitCriticalSection(CriticalSection);           //结束等待
        }
    }
    return STATUS_SUCCESS;
}
```





```cpp
RtlpUnWaitCriticalSection(PRTL_CRITICAL_SECTION CriticalSection)
{
    NTSTATUS Status;
 
   *******************************这里省略一些检查不贴了
    if (CriticalSection->LockSemaphore == INVALID_HANDLE_VALUE)
    {
        /* Release keyed event */
        Status = NtReleaseKeyedEvent(NULL, CriticalSection, FALSE, &RtlpTimeout);
    }
    else
    {
        /* Set the event */
        Status = NtSetEvent(CriticalSection->LockSemaphore, NULL);
    }
                                      重置事件状态为有信号
   *******************************************// 省略返回信息
}
```

总结：

1、初始化时将，LockCount设置为-1，SpinCount如果是单CPU设置为0，如果不是单CPU  调用InitializeCriticalSection设置为0，调用InitializeCriticalSectionAndSpinCount设置为参数指定的值。

2、每次调用EnterCriticalSection将LockCount +1，如果给其加1后为0表示拥有对象函数返回  并使RecursionCoun为1，OwningThread设置为本线程

​      如果LockCount +1后不为0，表示对象被其他线程拥有，进入等待函数，使用旋转锁和事件对象等待至到其他线程放弃对象拥有权。

​      注意在线程在拥有对象后重复调用 EnterCriticalSection时，RecursionCoun和LockCount 同时+1，RecursionCoun总是大于LockCount 一个数。

3、在调用LeaveCriticalSection 时，首先会检查是否本线程拥有对象，如果不是直接返回。

递减RecursionCoun和LockCount   知道前者等于0后者等于-1，然后调用事件重置函数 将事件设置为有信号状态，其他等待线程有个被唤醒。

4、DebugInfo 记录了统计信息。





最后 还有DeleteCriticalSection   和  TryEnterCriticalSection  也贴一下





```cpp
RtlDeleteCriticalSection(PRTL_CRITICAL_SECTION CriticalSection)
{
    NTSTATUS Status = STATUS_SUCCESS;
    if (CriticalSection->LockSemaphore)
    {
        Status = NtClose(CriticalSection->LockSemaphore);  //关闭事件对象
    }
    RtlEnterCriticalSection(&RtlCriticalSectionLock);
 
    if (CriticalSection->DebugInfo)
    {
        RemoveEntryList(&CriticalSection->DebugInfo->ProcessLocksList);
    }
 
    RtlLeaveCriticalSection(&RtlCriticalSectionLock);        //从进程关键段对象链表上移除该对象
 
    if (CriticalSection->DebugInfo)
    {
        RtlpFreeDebugInfo(CriticalSection->DebugInfo);   //释放调试信息用的堆内存
    }
    RtlZeroMemory(CriticalSection, sizeof(RTL_CRITICAL_SECTION));//将对象内存全部置0
    return Status;
}
```







```cpp
BOOLEAN
NTAPI
RtlTryEnterCriticalSection(PRTL_CRITICAL_SECTION CriticalSection)
{
    if (InterlockedCompareExchange(&CriticalSection->LockCount, 0, -1) == -1)  如果LockCount等于-1  将其设置为0
    {
        CriticalSection->OwningThread =  NtCurrentTeb()->ClientId.UniqueThread;
        CriticalSection->RecursionCount = 1;                                       设置该线程拥有对象，并返回TRUE
        return TRUE;
    }
    else if (CriticalSection->OwningThread == NtCurrentTeb()->ClientId.UniqueThread)//如果线程已经拥有对象RecursionCoun和LockCount都+1 返回TRUE
    {
        InterlockedIncrement(&CriticalSection->LockCount);   
        CriticalSection->RecursionCount++;        
        return TRUE;                                                     
    }
    return FALSE;    //不然返回FALSE
}
```

这里可以看到每个TryEnterCriticalSection 都必须  对应一个LeaveCriticalSection











# （八）用户模式下的线程同步 SRWLock剖析

2017年01月12日 09:28:02 [wangpengk7788](https://me.csdn.net/wangpengk7788) 阅读数：1858



 版权声明：本文为博主原创文章，未经博主允许不得转载。	https://blog.csdn.net/wangpengk7788/article/details/54374680

在VISTA及之后的系统中，引入了 SRWLock 用户用户模式的线程同步，MSDN中是这样描述的。





![img](https://img-blog.csdn.net/20170112083647286?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2FuZ3BlbmdrNzc4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



SRWLock  轻量级的读写锁，它与临界区对象的不同在于，它分为两个模式来访问共享资源。并假设有两种类型的线程同时工作，一种用在读取共享资源，通常又称为消费线程，另一种用来对共享的资源进程写入操作，通常又称为生产线程。



共享模式下每个读取线程可以读取共享的数据，不受任何限制，它们可以同时读取共享数据。



独占模式下只运行一个写入线程访问共享数据，其他读/写线程都会被堵塞。 直到该线程释放了读写锁。



在多线程的情况下，生产线程和消费线程是无序的，SRWLock是一个指针，它的优势在于可以快速的更新锁的状态，劣势在这个指针可以存放有限的信息，因此SRWLock不能够递归获取，此外一个线程处于共享模式的时候，不能讲自身升级为独占模式。





初始化读写锁很简单



```cpp
VOID WINAPI InitializeSRWLock(  PSRWLOCK SRWLock);
```

在Winbase.h文件中





```cpp
typedef RTL_SRWLOCK SRWLOCK, *PSRWLOCK;
typedef struct _RTL_SRWLOCK {
  PVOID Ptr;
} RTL_SRWLOCK, *PRTL_SRWLOCK;
```

可见读写锁确实只是一个指针变量。





使用读写锁：



在消费线程中

**AcquireSRWLockShared**

*******读取操作

[**ReleaseSRWLockShared**](http://127.0.0.1:47873/help/1-2536/ms.help?method=page&id=AFEFD9F2-7FD4-4CBA-9A6F-1F9DA614DCEC&product=VS&productVersion=100&topicVersion=85&locale=ZH-CN&topicLocale=EN-US)





在生产线程中

**AcquireSRWLockExclusive**

*******写操作

**ReleaseSRWLockExclusive**







这个读写锁和关键段一样方便使用，作者经过测试发现读写锁的性能较关键有很大的提升，这是因为读写锁是基于原子访问的，关键段是基于事件内核对象的，从用户模式到内核模式的切换占用了大量的时钟周期。





**剖析SRWLock,以React OS下对读写锁的实现代码来看其工作原理。**



在实现中SRWLock 的这个指针变量是这样来标识信息的



![img](https://img-blog.csdn.net/20170112100534827?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2FuZ3BlbmdrNzc4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





该指针的低4位被用于4个不同的标志，它们有其对应的宏定义





```cpp
#define RTL_SRWLOCK_OWNED_BIT   0
#define RTL_SRWLOCK_CONTENDED_BIT   1
#define RTL_SRWLOCK_SHARED_BIT  2
#define RTL_SRWLOCK_CONTENTION_LOCK_BIT 3
#define RTL_SRWLOCK_OWNED   (1 << RTL_SRWLOCK_OWNED_BIT)                                     //0位为1 表示有线程正在读/写共享资源
#define RTL_SRWLOCK_CONTENDED   (1 << RTL_SRWLOCK_CONTENDED_BIT)                             //1位为1  表示一个或者多个生产线程在等待独占资源
#define RTL_SRWLOCK_SHARED  (1 << RTL_SRWLOCK_SHARED_BIT)                                    //2位为1  表示一个或者多个消费线程在等待读取资源
#define RTL_SRWLOCK_CONTENTION_LOCK (1 << RTL_SRWLOCK_CONTENTION_LOCK_BIT)                   //3位为1   标识有一个线程在获取WAITBLOCK结构指针
#define RTL_SRWLOCK_MASK    (RTL_SRWLOCK_OWNED | RTL_SRWLOCK_CONTENDED | \
                             RTL_SRWLOCK_SHARED | RTL_SRWLOCK_CONTENTION_LOCK)
#define RTL_SRWLOCK_BITS    4
```

SRWLock 的高28位是一个指针，为什么只有28位呢，是因为它要指向对象的对齐方式是16，即地址的最低4位都是0，比如都是这样的0x00124710,0x00124720,0x00124850,   当需要获取指针所指向的对象时，只要将低4位全部置0，就可以得到对象的地址。





SRWLock 在同时有一个以上线程读写资源的时候，它指向一个结构体链表。



在有其他线程在读/资源的时候，一个线程调用AcquireSRWLockExclusive或者AcquireSRWLockShared  时会将一个在栈上构建的结构体挂入SRWLock 所指向的链表，这样每个将要读/写资源的线程都会在栈上构建这么一个结构体，并将结构体挂入链表中。

该结构体是这样定义的





```cpp
typedef struct _RTLP_SRWLOCK_WAITBLOCK
{
 
    LONG SharedCount; //有多少线程 在等待读取
    volatile struct _RTLP_SRWLOCK_WAITBLOCK *Last;    
    volatile struct _RTLP_SRWLOCK_WAITBLOCK *Next;   //链表节指针
 
    union
    {
        LONG Wake;                                   //非0表示可以被唤醒，0表示继续睡眠
        struct
        {
            PRTLP_SRWLOCK_SHARED_WAKE SharedWakeChain;   //需要被唤醒的消费线程链表
            PRTLP_SRWLOCK_SHARED_WAKE LastSharedWake;    //上一个被唤醒的消费线程
        };
    };
    BOOLEAN Exclusive;                         //1表示该结构体对象由生产线程构建在栈上，0表示结构体由消费线程构建在栈上
} volatile RTLP_SRWLOCK_WAITBLOCK, *PRTLP_SRWLOCK_WAITBLOCK;
```





下面是单向链表结构 ，代表每个需要被唤醒的消费线程

```cpp
typedef struct _RTLP_SRWLOCK_SHARED_WAKE
{
    LONG Wake;   //唤醒标志，非0唤醒，0睡眠
    volatile struct _RTLP_SRWLOCK_SHARED_WAKE *Next;
} volatile RTLP_SRWLOCK_SHARED_WAKE, *PRTLP_SRWLOCK_SHARED_WAKE;
```







初始化读写锁，只是简单的将指针置为0





```cpp
NTAPI
RtlInitializeSRWLock(OUT PRTL_SRWLOCK SRWLock)
{
    SRWLock->Ptr = NULL;
}
```





**AcquireSRWLockExclusive**



它对应的函数是RtlAcquireSRWLockExclusive(IN OUT PRTL_SRWLOCK SRWLock)， 由于这个函数的代码比较长，这里先贴出大体结构，然后分部分注释。





```cpp
NTAPI
RtlAcquireSRWLockExclusive(IN OUT PRTL_SRWLOCK SRWLock)
{
    __ALIGNED(16) RTLP_SRWLOCK_WAITBLOCK StackWaitBlock;    //构建于栈上的 对齐为16字节的 等待块
    PRTLP_SRWLOCK_WAITBLOCK First, Last;
 
    if (InterlockedBitTestAndSetPointer(&SRWLock->Ptr,
                                        RTL_SRWLOCK_OWNED_BIT))   //如果有其他线程在访问资源，进入循环，该原子访问函数返回之前的位值
    {
        LONG_PTR CurrentValue, NewValue;
 
        while (1)
        {
            CurrentValue = *(volatile LONG_PTR *)&SRWLock->Ptr;
 
            if (CurrentValue & RTL_SRWLOCK_SHARED)
            {
                    if (CurrentValue & RTL_SRWLOCK_CONTENDED)
                {
                    goto AddWaitBlock;
                }    
                else
                {
                }               //Part1, 如果有线程在读取资源，且没有其他生产线程在独占资源
            }
            else
            {
                if (CurrentValue & RTL_SRWLOCK_OWNED)
                {
 
                    if (CurrentValue & RTL_SRWLOCK_CONTENDED)
                    {
AddWaitBlock:
                                 //Part2,如果有其他线程在等待独占资源                    
                    }
                    else
                    {
				//Part3,如果有线程独占资源，且没有其他线程在等待资源
                    }
                }
                else
                {
                    if (!InterlockedBitTestAndSetPointer(&SRWLock->Ptr,
                                                         RTL_SRWLOCK_OWNED_BIT))
                    {
                        break;
                    }
                }
            }
 
            YieldProcessor();     执行空指令
        }
    }
}
 
```

最后一个函数  只是执行一个NOP指令，只是用来做延时而已，



​    \#define YieldProcessor() __asm__ __volatile__("nop");







**Part1**



```cpp
                if (CurrentValue & RTL_SRWLOCK_CONTENDED)
                {
                    goto AddWaitBlock;
                }
                else
                {
                    StackWaitBlock.Exclusive = TRUE;                               //标识结构体位于生产线程的栈上
                    StackWaitBlock.SharedCount = (LONG)(CurrentValue >> RTL_SRWLOCK_BITS);  //目前的高28位标识多少个线程在等待读取
                    StackWaitBlock.Next = NULL;
                    StackWaitBlock.Last = &StackWaitBlock;
                    StackWaitBlock.Wake = 0;             //初始化这个栈上的结构体
                    NewValue = (ULONG_PTR)&StackWaitBlock | RTL_SRWLOCK_SHARED | RTL_SRWLOCK_CONTENDED | RTL_SRWLOCK_OWNED;//设置新指针为该栈上结构地址
																并设置标志位
                    if ((LONG_PTR)InterlockedCompareExchangePointer(&SRWLock->Ptr,
                                                                    (PVOID)NewValue,
                                                                    (PVOID)CurrentValue) == CurrentValue) 更换指针为新的值
                    {
                        RtlpAcquireSRWLockExclusiveWait(SRWLock,
                                                        &StackWaitBlock);    进入生产线程的等待函数
                        break;
                    }
```



看下这个生产线程的等待函数





```cpp
NTAPI
RtlpAcquireSRWLockExclusiveWait(IN OUT PRTL_SRWLOCK SRWLock,
                                IN PRTLP_SRWLOCK_WAITBLOCK WaitBlock)
{
    LONG_PTR CurrentValue;
 
    while (1)
    {
        CurrentValue = *(volatile LONG_PTR *)&SRWLock->Ptr;
        if (!(CurrentValue & RTL_SRWLOCK_SHARED))
        {
            if (CurrentValue & RTL_SRWLOCK_CONTENDED)
            {
                if (WaitBlock->Wake != 0)
                {
                    break;
                }
            }
            else
            {
                break;
            }
        }
 
        YieldProcessor();                         //只有在没有线程在读取，没有其他生产线程在独占，或者独占的线程将该线程的WAKE标志设为非0，时退出死循环
    }
}
```



该函数保证了没个生产线程以独占的模式运行，其他生产线程只能在这里死循环，只要被唤醒，或者独占标志位被清除。





Part2:





```cpp
AddWaitBlock:
                        StackWaitBlock.Exclusive = TRUE;
                        StackWaitBlock.SharedCount = 0;
                        StackWaitBlock.Next = NULL;
                        StackWaitBlock.Last = &StackWaitBlock;
                        StackWaitBlock.Wake = 0;                             初始化结构体
 
                        First = RtlpAcquireWaitBlockLock(SRWLock);           根据28位的指针去掉标志位，获取指向的链表表头，
                        if (First != NULL)
                        {
                            Last = First->Last;
                            Last->Next = &StackWaitBlock;
                            First->Last = &StackWaitBlock;           //将该线程栈上的结构体挂入链表的最后面，将表头的Last指向最后这个挂入的
 
                            RtlpReleaseWaitBlockLock(SRWLock);
 
                            RtlpAcquireSRWLockExclusiveWait(SRWLock,
                                                            &StackWaitBlock);  //进入等待循环
 
                            break;
                        }
```

RtlpAcquireWaitBlockLock  揭示了 如何通过28位的指针工作







```cpp
NTAPI
RtlpAcquireWaitBlockLock(IN OUT PRTL_SRWLOCK SRWLock)
{
    LONG_PTR PrevValue;
    PRTLP_SRWLOCK_WAITBLOCK WaitBlock;
 
    while (1)
    {
        PrevValue = InterlockedOrPointer(&SRWLock->Ptr,
                                         RTL_SRWLOCK_CONTENTION_LOCK); //低位 第3位如果是1表示，有其他线程在调用该函数根据指针获取链表头
										这里保证了 只有一个线程可以访问链表，其他需要访问的要等待
        if (!(PrevValue & RTL_SRWLOCK_CONTENTION_LOCK))
            break;
 
        YieldProcessor();
    }
 
    if (!(PrevValue & RTL_SRWLOCK_CONTENDED) ||
        (PrevValue & ~RTL_SRWLOCK_MASK) == 0)
    {
        RtlpReleaseWaitBlockLock(SRWLock);   //如果现在没有处于独占模式 或者 高28位 为0   ，错误 释放RTL_SRWLOCK_CONTENTION_LOCK标志位
        return NULL;
    }
 
    WaitBlock = (PRTLP_SRWLOCK_WAITBLOCK)(PrevValue & ~RTL_SRWLOCK_MASK);   将指针与0x11111110  与操作， 将低4位标志清除，构成实际地址
 
    return WaitBlock;
}

NTAPI
RtlpReleaseWaitBlockLock(IN OUT PRTL_SRWLOCK SRWLock)
{
    InterlockedAndPointer(&SRWLock->Ptr,
                          ~RTL_SRWLOCK_CONTENTION_LOCK);     只是简单将该标志位设为0
}
```



**Part3:**





```cpp
                        StackWaitBlock.Exclusive = TRUE;
                        StackWaitBlock.SharedCount = 0;
                        StackWaitBlock.Next = NULL;
                        StackWaitBlock.Last = &StackWaitBlock;
                        StackWaitBlock.Wake = 0;  初始化结构体
 
                        NewValue = (ULONG_PTR)&StackWaitBlock | RTL_SRWLOCK_OWNED | RTL_SRWLOCK_CONTENDED;   将结构体地址构成新的指针
                        if ((LONG_PTR)InterlockedCompareExchangePointer(&SRWLock->Ptr,
                                                                        (PVOID)NewValue,
                                                                        (PVOID)CurrentValue) == CurrentValue)  替换原来的指针
                        {
                            RtlpAcquireSRWLockExclusiveWait(SRWLock,
                                                            &StackWaitBlock);   进入等待
                            break;
                        }
```



第一部分 和第三不分 代码差不多的，唯一的区别就是 第一不分多设置了一个标志位 RTL_SRWLOCK_SHARED，它们都是初始化栈上的结构体 并将结构体的地址加上标志位，然后替换为指针的值。



AcquireSRWLockExclusive  总结：



1、当有线程还在读取时，但是没有被其它生产线程独占， 那么挂入栈等待块，设置标志 （读取  独占 拥有），进入等待，读取线程全部Release时 线程等待结束，以独占模式访问资源

2、有其他生产线程在等待独占时，将栈块挂入SRWLock指针所指向链表的末尾，进入等待，在前面所有已挂入的等待都Release时，线程才结束等待 以独占模式访问资源。

3、如果有线程在独占，但是没有其他在等待独占的，那么挂入栈等待块，设置标志 （独占 拥有），进入等待，读取线程全部Release时 线程等待结束，以独占模式访问资源











**ReleaseSRWLockExclusive**





```cpp
NTAPI
RtlReleaseSRWLockExclusive(IN OUT PRTL_SRWLOCK SRWLock)
{
    LONG_PTR CurrentValue, NewValue;
    PRTLP_SRWLOCK_WAITBLOCK WaitBlock;
 
    while (1)
    {
        CurrentValue = *(volatile LONG_PTR *)&SRWLock->Ptr;
 
        if (!(CurrentValue & RTL_SRWLOCK_OWNED))   //此时如果不处于拥有状态，抛出异常
        {
            RtlRaiseStatus(STATUS_RESOURCE_NOT_OWNED);
        }
 
        if (!(CurrentValue & RTL_SRWLOCK_SHARED))  //必须不处于读取状态
        {
            if (CurrentValue & RTL_SRWLOCK_CONTENDED) 
            {
                WaitBlock = RtlpAcquireWaitBlockLock(SRWLock);        
                if (WaitBlock != NULL)
                {
                    RtlpReleaseWaitBlockLockExclusive(SRWLock,
                                                      WaitBlock);           如果有等待独占的线程，调用该函数，将指针指向的链表头传给它
 
                    break;
                }
            }
            else
            {
 
                ASSERT(!(CurrentValue & ~RTL_SRWLOCK_OWNED));
 
                NewValue = 0;
                if ((LONG_PTR)InterlockedCompareExchangePointer(&SRWLock->Ptr,
                                                                (PVOID)NewValue,
                                                                (PVOID)CurrentValue) == CurrentValue)  //如果没有等待独占的，指针置为0
                {
                    break;
                }
            }
        }
        else
        {
            RtlRaiseStatus(STATUS_RESOURCE_NOT_OWNED);             如果处于读取状态 抛出异常
        }
 
        YieldProcessor();
    }
}
```







RtlpReleaseWaitBlockLockExclusive：



```cpp
NTAPI
RtlpReleaseWaitBlockLockExclusive(IN OUT PRTL_SRWLOCK SRWLock,
                                  IN PRTLP_SRWLOCK_WAITBLOCK FirstWaitBlock)
{
    PRTLP_SRWLOCK_WAITBLOCK Next;
    LONG_PTR NewValue;
 
 
    Next = FirstWaitBlock->Next;
    if (Next != NULL)  如果还有其他在等待的线程
    {
        NewValue = (LONG_PTR)Next | RTL_SRWLOCK_OWNED | RTL_SRWLOCK_CONTENDED;
        if (!FirstWaitBlock->Exclusive)        如果第一个等待的线程是读取线程
        {
            ASSERT(Next->Exclusive);
            Next->SharedCount = FirstWaitBlock->SharedCount;  //复制读取线程计数
 
            NewValue |= RTL_SRWLOCK_SHARED;
        }
 
        Next->Last = FirstWaitBlock->Last;    将Last指向链尾的对象
    }
    else     //如果只有一个在等待的线程
    {
        if (FirstWaitBlock->Exclusive)  //如果该线程是生产线程，简单设置一个拥有位
            NewValue = RTL_SRWLOCK_OWNED;
        else
        {                           //如果该线程是消费线程，设置高位为线程计数，低位为标志 拥有  和  读取
            ASSERT(FirstWaitBlock->SharedCount > 0);
 
            NewValue = ((LONG_PTR)FirstWaitBlock->SharedCount << RTL_SRWLOCK_BITS) |
                       RTL_SRWLOCK_SHARED | RTL_SRWLOCK_OWNED;
        }
    }
 
    (void)InterlockedExchangePointer(&SRWLock->Ptr, (PVOID)NewValue);  设置新的指针
 
    if (FirstWaitBlock->Exclusive)
    {
        (void)InterlockedOr(&FirstWaitBlock->Wake,     将第一个等待的线程唤醒，如果它是生产线程的话。
                            TRUE);
    }
    else   //如果第一个等待唤醒的是一个消费线程的等待块，根据SharedWakeChain链表， 唤醒每一个等待的消费线程。
    {
        PRTLP_SRWLOCK_SHARED_WAKE WakeChain, NextWake;
        WakeChain = FirstWaitBlock->SharedWakeChain;
        do
        {
            NextWake = WakeChain->Next;
 
            (void)InterlockedOr((PLONG)&WakeChain->Wake,
                                TRUE);
 
            WakeChain = NextWake;
        } while (WakeChain != NULL);
    }
}
 
```









# （九）使用内核对象进行线程同步

2017年01月13日 04:52:10 [wangpengk7788](https://me.csdn.net/wangpengk7788) 阅读数：332



 版权声明：本文为博主原创文章，未经博主允许不得转载。	https://blog.csdn.net/wangpengk7788/article/details/54397199

上一章作者介绍了用户模式下的线程同步，虽然它们有很好的性能，但是他们功能有限并不是能够胜任实际工作中出现的复杂情况，以及特殊的要求，于是Windwos提供了一些用于线程同步的内核对象，它们根据不同的应用场景而设计，满足不同的环境下的线程同步的要求，因为是基于内核对象来完成同步，因此线程同步时可以跨进程的。



这些内核对象包括，事件，可等待计时器，信号量，互斥量。



在具体介绍每个线程同步内核对象之前，需要了解两个基础的用于等待内核对象的函数WaitForSingleObjectEx 和  WaitForMultipleObjects.





```cpp
DWORD WINAPI WaitForSingleObject(
  __in  HANDLE hHandle,  //内核对象句柄
  __in  DWORD dwMilliseconds  //等待的时间、单位：毫秒  如果指定（INFINITE）表示无信号一直等待。
);
```

返回值  



WAIT_ABANDONED   （只在对象为互斥量类型时用到， 表示上一个线程因为错误，线程异常终止，内核对象才变为了有信号的）

WAIT_OBJECT_0  （内核对象有信号）

WAIT_TIMEOUT    （等待时间到了）

WAIT_FAILED  （参数错误，函数调用失败）




```cpp
DWORD WINAPI WaitForMultipleObjects(
  __in  DWORD nCount,  （多少个要等待的对象）
  __in  const HANDLE *lpHandles,  内核对象句柄数组
  __in  BOOL bWaitAll,  （是否等待所有对象有信号才返回）
  __in  DWORD dwMilliseconds  (等待时间）
);
```



该API函数等待一个或者一组线程同步内核对象，由 bWaitAll指定等待方式， bWaitAll == 1在所有对象处于有信号状态返回， bWaitAll=0 只有其中一个处于有信号状态就返回



返回值







WAIT_OBJECT_0 to (WAIT_OBJECT_0 + nCount– 1)      (WAIT_OBJECT_0+N)表示第N个对象处于有信号状态在bWaitAll为1时，如果bWaitAll为0    WAIT_OBJECT_0表示所有对象处于有信号状态，成功返回。

WAIT_ABANDONED_0 to (WAIT_ABANDONED_0 + nCount– 1)  （如果bWaitAll为1，WAIT_ABANDONED_0表示句柄数组中至少有一个是互斥量对象的句柄，如果bWaitAll为0，那么WAIT_ABANDONED_0+N表示句柄数组中第N个是一个互斥量句柄，上一个线程没有释如果放互斥量的情况下异常终止）


WAIT_TIMEOUT   （同WaitForSingleObject）

WAIT_FAILED    （WaitForSingleObject）





（一）事件内核对象

事件内核对象在上一章的用户模式的同步对象 CriticalSection 中已经提到，CriticalSection只是用了它的自动重置模式，自动重置模式在WaitForSingleObject获取到有信号后自动将对象设置为无信号状态并返回。

临界区对象在EnterCriticalSection中调用执WaitForSingleObjec行等待，在LeaveCriticalSection中调用SetEvent将事件对象重置为有信号状态。



此外事件内核对象可以使用手动重置模式，在手动模式下，WaitForSingleObject并改变其状态，需要手动调用SetEvent将对象置为有信号状态，或者调用ResetEvent将对象置为无信号状态，在将对象置为有信号状态时，所有使用WaitXX执行等待的线程都可以返回WAIT_OBJECT_0 得到调度。





API：



```cpp
HANDLE WINAPI CreateEvent(
  __in_opt  LPSECURITY_ATTRIBUTES lpEventAttributes,
  __in      BOOL bManualReset, //TRUE自动模式 FALSE手动模式
  __in      BOOL bInitialState,//初始值是否是有信号的，TRUE （是），FALSE(不是)
  __in_opt  LPCTSTR lpName
);

BOOL WINAPI SetEvent(
  __in  HANDLE hEvent
);

BOOL WINAPI ResetEvent(
  __in  HANDLE hEvent
);
```





适应：在自动模式下与 临界区对象  互斥量 SRWLock的互斥模式  功能是一样的，都保证对共享资源的访问在同一时间只运行一个线程独占。

区别  ：临界区对象在使用内核机制前有个一个旋转锁等待循环，如果在确定对共享资源独占期间占用很少的CPU周期，应该使用临界区对象。

​              如果区分写共享资源和读共享资源的线程，即存在生产和消费两种类型的线程，使用SRWLock

如果要进行进程间的同步，或者在独占共享资源之后的操作可能占用很多的CPU周期，如等待用户输入并把用户的输入写入共享资源，应该使用事件或者互斥量，这两个内核同步对象，使线程在无信号状态不参与调度，不占用CPU，而不是SRWLock那样循环的使用原子访问和NOP



​                 互斥量和事件对象的区别很小，互斥量允许一个在异常终止后系统自己是否它对对象的占用，从而避免死锁。事件对象则多了一个手动模式，允许多个线程同时得到执行





（二）可等待的计时器



可等待计时器，类似于SetTimer,它设置一个定时器，并可以设置周期循环触发的时间，在时间到期时所有等待线程可以得到执行（手动模式），或者一个等待线程被唤醒（自动模式）。同时它还可以设置一个APC回调函数，在时间到期时将ACP挂入 SetWaitableTimer 主调线程的APC队列中。





API:



```cpp
HANDLE WINAPI CreateWaitableTimer(
  __in_opt  LPSECURITY_ATTRIBUTES lpTimerAttributes,
  __in      BOOL bManualReset, //自动还是手动模式
  __in_opt  LPCTSTR lpTimerName   //内核对象名字
);
```





```cpp
BOOL WINAPI SetWaitableTimer(
  __in      HANDLE hTimer,
  __in      const LARGE_INTEGER *pDueTime,   //触发的时间，如果为负数表示相对时间，单位纳秒
  __in      LONG lPeriod,                    //触发后多久循环触发一次一次
  __in_opt  PTIMERAPCROUTINE pfnCompletionRoutine,  //APC回调函数地址
  __in_opt  LPVOID lpArgToCompletionRoutine,        //回调函数参数
  __in      BOOL fResume                        //是否唤醒睡眠中的系统，利用电源管理
);
```

取消对象的活动状态，不再触发。





```cpp
BOOL WINAPI CancelWaitableTimer(
  __in  HANDLE hTimer
);
```



测试代码 使用自动重置模式，10秒后触发，之后每秒触发一次

```cpp
#include <iostream>
#include <stdio.h>
#include <windows.h>
#include <thread>
#include <winbase.h>
 
 
 
//using namespace std;
using std::cout;
using std::endl;
 
 
HANDLE hTimer = NULL;
volatile LONG g_IsPassed = 0;
 
void ThreadForTimer(int pParameter)
{
    //int ID  = *static_cast<int*>(pParameter);
    int ID = pParameter;
    InterlockedAdd(&g_IsPassed,1);
    while(1)
    {
        cout.flush();
        if(WaitForSingleObject(hTimer,INFINITE) == WAIT_OBJECT_0)
        {
            cout<<"Thread "<<ID<<" has obtained the single Time:"<<GetTickCount()<<endl;
        }
        else
        {
            cout<<"Thread exception occurred"<<endl;
        }
    }
 
 
}
 
int main(int argc, char *argv[])
{
    LARGE_INTEGER liDueTime;
 
    liDueTime.QuadPart = -100000000L;
 
    // Create an unnamed waitable timer.
    hTimer = CreateWaitableTimer(NULL, FALSE, NULL);
    if (NULL == hTimer)
    {
        printf("CreateWaitableTimer failed (%d)\n", GetLastError());
        return 1;
    }
 
    printf("Waiting for 10 seconds   Time:%d \n ",GetTickCount());
 
 
    if (!SetWaitableTimer(hTimer, &liDueTime, 1000, NULL, NULL, 0))
    {
        printf("SetWaitableTimer failed (%d)\n", GetLastError());
        return 2;
    }
 
    for(int i = 0; i != 5; ++i)
    {
        new std::thread(ThreadForTimer,i);
        while(1)
        {
            if(InterlockedCompareExchange(&g_IsPassed,0,1) == 1)  break;
            YieldProcessor();
        }
    }
    SleepEx(INFINITE,TRUE);
 
    return 0;
 
 
}
```



运行结果



![img](https://img-blog.csdn.net/20170113072703744?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2FuZ3BlbmdrNzc4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





  hTimer = CreateWaitableTimer(NULL, FALSE, NULL);   第二个参数改为TRUE   手动模式后

![img](https://img-blog.csdn.net/20170113073002620?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2FuZ3BlbmdrNzc4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





每个线程一直都处于可调度状态





可以等待计数器内核对象的适用情况非常的明确，当需要一个定时器在某和时间点唤醒等待线程，或者按一定时间循环唤醒时。





（三）信号量内核对象

信号量内核对象用来对资源进行计数，在需要对资源使用做限制的时候信号量非常有用，在内部它存放了一个当前可用资源数的变量，和一个最大资源数的变量，每个等待线程使用等待函数时,'如果可以资源数大于0，将其值减一并返回，如果可以资源数已经为0则等待函数进入睡眠，可以通过ReleaseSemaphore增加可以用资源数量，对计数访问修改都是使用的原子操作，保证了计数在多线程下不被破坏。



API:





```cpp
HANDLE WINAPI CreateSemaphore(
  __in_opt  LPSECURITY_ATTRIBUTES lpSemaphoreAttributes,
  __in      LONG lInitialCount,  可用数量 
  __in      LONG lMaximumCount,  最大数量
  __in_opt  LPCTSTR lpName
);
```







```cpp
BOOL WINAPI ReleaseSemaphore(
  __in       HANDLE hSemaphore,
  __in       LONG lReleaseCount,  增加多少
    __out_opt  LPLONG lpPreviousCount
);
```

测试代码：





```cpp
#include <iostream>
#include <stdio.h>
#include <windows.h>
#include <thread>
#include <winbase.h>
#include <string>
#include <random>
#include <vector>
 
 
//using namespace std;
using std::cout;
using std::endl;
 
HANDLE g_hSemaphore;
HANDLE g_hEvent;
std::vector<int> vtrBuffer;
 
void ThreadForSemaphoreInc()
{
    std::random_device rd;
    while(1)
    {
        if(WaitForSingleObject(g_hEvent,INFINITE) == WAIT_OBJECT_0)
        {
            for(int i = 0; i !=10; ++i)
            {
               vtrBuffer.push_back(rd()%100);
            }
            ReleaseSemaphore(g_hSemaphore,10,NULL);
            SetEvent(g_hEvent);
 
        }
        cout<<"The buffer has been full"<<endl<<endl;
        Sleep(5*1000);
 
    }
 
}
void ThreadForSemaphoreDec()
{
    HANDLE handles[2] = {g_hEvent,g_hSemaphore};
    while(1)
    {
        if(WaitForMultipleObjects(2,handles,TRUE,INFINITE) == WAIT_OBJECT_0)
        {
            cout<<"Decreasing the buffer,the popuped value is "<<vtrBuffer.back()<<endl;
            vtrBuffer.pop_back();
            SetEvent(g_hEvent);
        }
 
    }
}
 
int main(int argc, char *argv[])
{
    g_hEvent = CreateEvent(NULL,FALSE,TRUE,NULL);
    if(!g_hEvent) cout<<"CreateEvent failed and the error code is "<<GetLastError()<<endl;
 
    g_hSemaphore = CreateSemaphore(NULL,0,10,NULL);
    if(!g_hEvent) cout<<"CreateSemaphore failed and the error code is "<<GetLastError()<<endl;
 
    new std::thread(ThreadForSemaphoreInc);
 
    new std::thread(ThreadForSemaphoreDec);
    Sleep(1000*1000);
 
 
    return 0;
}
```

运行结果





![img](https://img-blog.csdn.net/20170113125639135?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2FuZ3BlbmdrNzc4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





（四）互斥量对象



互斥量内核对象用来确保一个线程独占对一个资源的访问。互斥量对象包含一个使用计数，线程ID，和一个递归计数。互斥量与关键段的行为完全相同，但互斥量是内核对象，而关键段是用户模式下的同步对象。


互斥量规则：


（1）如果线程ID为0（无效线程ID），那么该互斥量不为任何线程占用，它处于触发在状态


（2）如果线程ID为非零值，那么有一个线程已经占用了该互斥量，它处于未触发在状态


（3）与其他内核对象不同，操作系统对互斥量进行了特殊处理，允许它们违反一些常规的规则。



 为了获得对被保护资源的访问权，线程要调用等待函数并传入互斥量句柄。在内部，等待函数会检查线程ID是否为0，如果为0，等待线程将互斥量对象线程ID设为当前线程

ID，递归计数为1。否则，主调线程将会被挂起。当其他线程完成对保护资源的互斥访问，释放对互斥量的占有时，互斥量的线程ID被设为0，原来被挂起的线程变为可调度状态，并将互斥量对象对象ID设为此线程ID，递归计数为1。

前面一直提到递归计数，却没有解释它的意思。当线程试图等待一个未触发的互斥量对象，此时通常处于等待状态。但是系统会检查想要获得互斥量的线程的线程ID与互斥量对象内部记录的线程ID是否相同。如果相同，那么系统会让线程保持可调度状态，即使该互斥量尚未触发。每次线程等待成功一个互斥量，互斥对象的递归计数就会被设为1。因此，使递归对象大于1 的唯一途径是让线程多次等待同一个互斥量。
当目前占有互斥量的线程不再需要访问互斥资源时，它必须调用ReleaseMutex来释放互斥量。





API:





```cpp
HANDLE WINAPI CreateMutex(
  __in_opt  LPSECURITY_ATTRIBUTES lpMutexAttributes,
  __in      BOOL bInitialOwner,
  __in_opt  LPCTSTR lpName
);
```





```cpp
BOOL WINAPI ReleaseMutex(
  __in  HANDLE hMutex
);
```



（五）各种同步对象的概括



![img](https://img-blog.csdn.net/20170113130333336?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2FuZ3BlbmdrNzc4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![img](https://img-blog.csdn.net/20170113130351211?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2FuZ3BlbmdrNzc4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)











每种同步内核对象都有个OpenXX版本的函数，用来打开已经创建过的对象，如OpenMutex,OpenEvent,



每种创建同步内核对象的函数都有一个对应的后缀为Ex,的版本  如CreateSemaphoreEx，CreateEventEx，  带后缀的版本多了一个__in      DWORD dwDesiredAccess参数，用来设置创建的对象可以用什么访问权限来使用。



每种同步内核对象都是可以具名的，所有的内核对象名字同在一个命名空间内，如果要创建的对象名字已存在，创建函数失败，GetLastError 返回ERROR_ALREADY_EXISTS



每种同步内核对象在不使用的时候都要调用CloseHandle关闭，不然会造成内核对象泄露。



每种同步内核对象都可以拥有不同进程间的线程同步，可以使用DuplicationHandle，句柄继承，打开同名对象的方式，让两个进程中各自的线程引用同一个内核对象来完成同步工作。









（六）其他等待函数



```cpp
DWORD WaitForInputIdle(
  HANDLE hProcess,       // handle to process
  DWORD dwMilliseconds   // time-out interval
);
```

用于等待一个进程，第一次创建窗口。









```cpp
DWORD WINAPI MsgWaitForMultipleObjects(
  __in  DWORD nCount,
  __in  const HANDLE *pHandles,
  __in  BOOL bWaitAll,
  __in  DWORD dwMilliseconds,
  __in  DWORD dwWakeMask
);
```

线程在等待内核对象变为有信号的同时，如果有窗口消息到来，线程也可以变为可调度状态。









```cpp
BOOL WaitForDebugEvent(
  LPDEBUG_EVENT lpDebugEvent,  // debug event information
  DWORD dwMilliseconds         // time-out value
);
```

被调试进程有调试事件时触发，否则一直等待









```cpp
DWORD WINAPI SignalObjectAndWait(
  __in  HANDLE hObjectToSignal,
  __in  HANDLE hObjectToWaitOn,
  __in  DWORD dwMilliseconds,
  __in  BOOL bAlertable
);
```

将一个对象置为有信号的同时，等待另一个对象，原子操作在内部。





# （十）同步IO 与 异步IO




有两种类型的文件IO同步：同步文件IO和异步文件IO。在同步文件IO中，线程启动一个IO操作然后就立即进入等待状态，直到IO操作完成后才醒来继续执行。而异步文件IO方式中，线程发送一个IO请求到内核，然后继续处理其他的事情，内核完成IO请求后，将会通知线程IO操作完成了。


如果IO请求需要大量时间执行的话，异步文件IO方式可以显著提高效率，因为在线程等待的这段时间内，CPU将会调度其他线程进行执行，如果没有其他线程需要执行的话，这段时间将会浪费掉（可能会调度操作系统的零页线程）。如果IO请求操作很快，用异步IO方式反而还低效，还不如用同步IO方式。
同步IO在同一时刻只允许一个IO操作，也就是说对于同一个文件句柄的IO操作是序列化的，即使使用两个线程也不能同时对同一个文件句柄同时发出读写操作。重叠IO允许一个或多个线程同时发出IO请求。
异步IO在请求完成时，通过将文件句柄设为有信号状态来通知应用程序，或者应用程序通过GetOverlappedResult察看IO请求是否完成，也可以通过一个事件对象来通知应用程序，或者使用IOCP。
简单的说“同步在编程里，一般是指某个IO操作执行完后，才可以执行后面的操作。异步则是，将某个操作给系统，主线程去忙别的事情，等内核完成操作后通知主线程异步操作已经完成。



I、Windows同步I/O与异步I/O
一、  同步I/O和异步I/O

同步I/O，对于同一个I/O对象句柄在同一时刻只允许一个I/O操作，其原理如下图所示：


  显然，当内核真正处理I/O的时间段（T2~T4），用户线程是处于等待状态的，如果这个时间段比较段的话，没有什么影响；倘若这个时间段很长的话，线程就会长时间处于挂起状态。事实上，该线程完全可以利用这段时间用处理其他事务。

  异步I/O恰好可以解决同步I/O中的问题，而且支持对同一个I/O对象的并行处理，其原理如下图所示：


   异步IO在调用IO操作后函数立即返回继续线程的其他操作。

   异步I/O在I/O请求完成时，如何通知线程对完成的IO操作进行相应呢，大体有四种方式。

（1）、使用另外的线程等待设备对象变为有信号状态，

（2）、使用另外线程等待OverLapped中的Event事件触发。

（3）、使用异步过程调用调用（APC）设置通知回调函数。

（4）、使用IOCP完成端口




二、使用重叠I/O实现异步I/O

  同一个线程可以对多个I/O对象进行I/O操作，不同的线程也可以对同一个I/O对象进行操作，在我的理解中，重叠的命名就是这么来的。

  在使用重叠I/O时，线程需要创建OVERLAPPED结构以供I/O处理。在使用异步IO的时候需要传递该结构体的指针给访问IO的API，如ReadFile.

```
typedef struct _OVERLAPPED {
  ULONG_PTR Internal;  //用于返回错误码
  ULONG_PTR InternalHigh;//用于返回IO完成的字节数
  union {
    struct {
      DWORD Offset;
      DWORD OffsetHigh;
    } ;
    PVOID  Pointer; //文件指针
  } ;
  HANDLE    hEvent;  //事件对象
} OVERLAPPED, *LPOVERLAPPED;
```






  由于在进行I/O请求后会立即返回，但有时用户线程需要知道I/O当前的执行情况，此时就可以使用GetOverlappedResult。如果该函数的bWait参数为true,那么改函数就会阻塞线程直到目标I/O处理完成为止；如果bWait为false，那么就会立即返回，如果此时的I/O尚未完，调用GetLastError就会返回ERROR_IO_INCOMPLETE。

代码示例一：

 代码:

```
DWORD   nReadByte ;
BYTE   bBuf[BUF_SIZE] ;
OVERLAPPED ov = { 0, 0, 0, 0, NULL } ;  // hEvent = NULL ;
HANDLE hFile = CreateFile ( ……, FILE_FLAG_OVERLAPPED, …… ) ;
ReadFile ( hFile, bBuf, sizeof(bBuf), &nReadByte, &ov ) ;
// 由于此时hEvent=NULL，所以同步对象为hFile,下面两句的效果一样
WaitForSingleObject ( hFile, INFINITE ) ;
//GetOverlappedResult ( hFile, &ov, &nRead, TRUE ) ;
```



这段代码在调用ReadFile后会立即返回，但在随后的WaitForSingleObject或者GetOverlappedResult中阻塞，利用同步对象hFile进行同步。

  这段代码在这里可以实现正常的异步I/O，但存在一个问题，倘若现在需要对hFile句柄进行多个I/O操作，就会出现问题。见下面这段代码。

代码示例二：

 代码:

```
DWORD   nReadByte ;
BYTE   bBuf1[BUF_SIZE],bBuf2[BUF_SIZE],bBuf3[BUF_SIZE] ;
OVERLAPPED ov1 = { 0, 0, 0, 0, NULL } ;  
OVERLAPPED ov2 = { 0, 0, 0, 0, NULL } ;  
OVERLAPPED ov3 = { 0, 0, 0, 0, NULL } ;  
HANDLE hFile = CreateFile ( ……, FILE_FLAG_OVERLAPPED, …… ) ;
ReadFile ( hFile, bBuf1, sizeof(bBuf1), &nReadByte, &ov1 ) ;
ReadFile ( hFile, bBuf2, sizeof(bBuf2), &nReadByte, &ov2 ) ;
ReadFile ( hFile, bBuf3, sizeof(bBuf3), &nReadByte, &ov3 ) ;
//假设三个I/O处理的时间比较长，到这里还没有结束
GetOverlappedResult ( hFile, &ov1, &nRead, TRUE ) ;
```



  这里对于hFile有三个重叠的I/O操作，但他们的同步对象却都为hFile。使用GetOverlappedResult进行等待操作，这里看似在等待第一个I/O处理的完成，其实只要有任何一个I/O处理完成，该函数就会返回，相当于忽略了其他两个I/O操作的结果。

  其实，这里有一个很重要的原则：对于一个重叠句柄上有多于一个I/O操作的时候，应该使用事件对象而不是文件句柄来实现同步。正确的实现见示例三。

代码示例三：

 代码:

```
DWORD   nReadByte ;
BYTE   bBuf1[BUF_SIZE],bBuf2[BUF_SIZE],bBuf3[BUF_SIZE] ;
HANDLE  hEvent1 = CreateEvent ( NULL, FALSE, FALSE, NULL ) ; 
HANDLE  hEvent2 = CreateEvent ( NULL, FALSE, FALSE, NULL ) ;
HANDLE  hEvent3 = CreateEvent ( NULL, FALSE, FALSE, NULL ) ;
OVERLAPPED ov1 = { 0, 0, 0, 0, hEvent1 } ;  
OVERLAPPED ov2 = { 0, 0, 0, 0, hEvent2 } ;  
OVERLAPPED ov3 = { 0, 0, 0, 0, hEvent3 } ;  
HANDLE hFile = CreateFile ( ……, FILE_FLAG_OVERLAPPED, …… ) ;
ReadFile ( hFile, bBuf1, sizeof(bBuf1), &nReadByte, &ov1 ) ;
ReadFile ( hFile, bBuf2, sizeof(bBuf2), &nReadByte, &ov2 ) ;
ReadFile ( hFile, bBuf3, sizeof(bBuf3), &nReadByte, &ov3 ) ;
//此时3个I/O操作的同步对象分别为hEvent1,hEvent2,hEvent3
GetOverlappedResult ( hFile, &ov1, &nRead, TRUE ) ;
```



  这样，这个GetOverlappedResult就可以实现对第一个I/O处理的等待
关于重叠I/O的就讨论到这里，关于重叠I/O的实际应用，可以参考《Windows系统编程之进程通信》其中的命名管道实例。
http://bbs.pediy.com/showthread.PHP?s=&threadid=26252

三、  使用异步过程调用实现异步I/O

异步过程调用（APC），即在特定的上下文中异步的执行一个调用。在异步I/O中可以使用APC，即让操作系统的IO系统在完成异步I/O后立即调用你的程序。（在有些资料中，把异步I/O中的APC称为“完成例程”，感觉这个名称比较贴切，下文就以“完成例程”来表述。另外通常APC是作为线程同步这一块的内容，这里尽量淡化这个概念以免混淆。关于APC的详细内容到线程同步时再介绍 ）

这里需要注意三点：
（1）  APC总是在调用线程中被调用；
（2）  当执行APC时，调用线程会进入可变等待状态；
（3）  线程需要使用扩展I/O系列函数，例如ReadFileEx,WriteFileEx, 另外可变等待函数也是必须的（至少下面其中之一）：
WaitForSingleObjectEx
WaitForMultipleObjectEx
SleepEx
SignalObjectAndWait
MsgWaitForMultipleObjectsEx

  在使用ReadFileEx,WriteFileEx时，重叠结构OVERLAPPED中的hEvent成员并非一定要指定，因为系统会忽略它。当多个IO操作共用同一个完成例程时，可以使用hEvent来携带序号等信息，用于区别不同的I/O操作，因为该重叠结构会传递给完成例程。如果多个IO操作使用的完成例程都不相同时，则直接把hEvent设置为NULL就可以了。

在系统调用完成例程有两个条件：
（1）  I/O操作必须完成
（2）  调用线程处于可变等待状态

对于第一个条件比较容易，显然完成例程只有在I/O操作完成时才调用；至于第二个条件就需要进行认为的控制，通过使用可变等待函数，让调用线程处于可变等待状态，这样就可以执行完成例程了。这里可以通过调节调用可变等待函数的时机来控制完成例程的执行，即可以确保完成例程不会被过早的执行。

当线程具有多个完成例程时，就会形成一个队列。使用可变等待函数使线程进入可变等待状态时有一个表示超时值的参数，如果使用INFINITE，那么只有所有排队的完成例程被执行或者句柄获得信号时该等待函数才返回。

上面已经对利用完成例程实现异步I/O的一些比较重要的细节进行的简洁的阐述，接下来就以一个实例来说明完成例程的具体实现过程。



实例一：使用完成例程的异步I/O示例

1、  设计目标
体会完成例程的异步I/O实现原理及过程。

2、  问题的分析与设计
设计流程图如下：

示图说明：
  三个IO操作分别是IO_A, IO_B, IO_C, 他们的完成例程分别是APC_A, APC_B, APC_C。IO_A, IO_B是两个很短的IO操作，IO_C是一个比较费时的IO操作。
3、  详细设计（关键代码如下,具体参见附件中的源代码CompletionRoutine）

 代码:

```
VOID WINAPI APC_A ( DWORD dwError, DWORD cbTransferred, LPOVERLAPPED lpo )
{
  pTempInfo.push_back ( "执行IO_A的完成例程" ) ;
}
VOID WINAPI APC_B ( DWORD dwError, DWORD cbTransferred, LPOVERLAPPED lpo )
{
  pTempInfo.push_back ( "执行IO_B的完成例程" ) ;
}
VOID WINAPI APC_C ( DWORD dwError, DWORD cbTransferred, LPOVERLAPPED lpo )
{
  pTempInfo.push_back ( "执行IO_C的完成例程" ) ;
}

void CCompletionRoutineDlg::OnTest() 
{
  // TODO: Add your control notification handler code here
  HANDLE    hFile_A, hFile_B, hFile_C ;
  OVERLAPPED  ov_A = {0}, ov_B = {0}, ov_C = {0} ;

# define C_SIZE 1024 * 1024 * 32

  string  szText_A = "Sample A !" ;
  string  szText_B = "Sampel B !" ;
  string  szText_C ;
  szText_C.resize ( C_SIZE ) ;
  memset ( &(szText_C[0]), 0x40, C_SIZE ) ;

  pTempInfo.clear () ;

  hFile_A = CreateFile ( "A.txt", GENERIC_WRITE, 0, NULL, /
              CREATE_ALWAYS, FILE_FLAG_OVERLAPPED, NULL ) ;
  hFile_B = CreateFile ( "B.txt", GENERIC_WRITE, 0, NULL, /
              CREATE_ALWAYS, FILE_FLAG_OVERLAPPED, NULL ) ;
  hFile_C = CreateFile ( "C.txt", GENERIC_WRITE, 0, NULL, /
              CREATE_ALWAYS, FILE_FLAG_OVERLAPPED, NULL ) ;

  WriteFileEx ( hFile_A, &(szText_A[0]), szText_A.length(), &ov_A, APC_A ) ;
  pTempInfo.push_back ( "启动IO_A, 并立即返回" ) ;

  WriteFileEx ( hFile_B, &(szText_B[0]), szText_B.length(), &ov_B, APC_B ) ;
  pTempInfo.push_back ( "启动IO_B, 并立即返回" ) ;

  WriteFileEx ( hFile_C, &(szText_C[0]), szText_C.size(), &ov_C, APC_C ) ;
  pTempInfo.push_back ( "启动IO_C, 并立即返回" ) ;

  pTempInfo.push_back ( "进入可变等待状态" ) ;
  SleepEx ( 1, true ) ;
  pTempInfo.push_back ( "结束可变等待状态" ) ;

  pTempInfo.push_back ( "进入可变等待状态" ) ;
  SleepEx ( 10000, true ) ;
  pTempInfo.push_back ( "结束可变等待状态" ) ;

  CloseHandle ( hFile_A ) ;
  CloseHandle ( hFile_B ) ;
  CloseHandle ( hFile_C ) ;

  m_ListBox.ResetContent () ;

  list<string>::iterator p ;
  for ( p = pTempInfo.begin(); p != pTempInfo.end(); p++ )
  {
    m_ListBox.AddString ( p->data() ) ;
  }

  DeleteFile ( "A.txt" ) ;
  DeleteFile ( "B.txt" ) ;
  DeleteFile ( "C.txt" ) ;
}
```

四、使用完成端口（IOCP）

四、使用完成端口（IOCP）

实例二、使用IOCP的异步I/O示例
1、设计目标
体会完成端口的异步I/O实现原理及过程。

2、  问题的分析与设计



说明：
  每个客户端与一个管道进行交互，而在交互过程中I/O操作结束后产生的完成包就会进入“I/O完成包队列”。完成端口的线程队列中的线程使用GetQueuedCompletionStatus来检测“I/O完成包队列”中是否有完成包信息。 
3、详细设计（关键代码如下）

代码:

 

```


UINT ServerThread ( LPVOID lpParameter )

{

  ……

  while ( true )

  {


GetQueuedCompletionStatus ( pMyDlg->hCompletionPort, &cbTrans, &dwCompletionKey, &lpov, INFINITE ) ;

if ( dwCompletionKey == -1 )

  break ;

// 读取管道信息

// 响应管道信息（写入）


  }

  return 0 ;

}

 

void CMyDlg::OnStart() 

{

  // 创建完成端口

  hCompletionPort = CreateIoCompletionPort ( INVALID_HANDLE_VALUE, NULL, 0, nMaxThread ) ;

 

  CString lpPipeName = "////.//Pipe//NamedPipe" ;

  for ( UINT i = 0; i < nMaxPipe; i++ )

  {

    // 创建命名管道
    
    PipeInst[i].hPipe =  CreateNamedPipe ( lpPipeName, PIPE_ACCESS_DUPLEX|FILE_FLAG_OVERLAPPED, /
    
          PIPE_TYPE_BYTE|PIPE_READMODE_BYTE|PIPE_WAIT, nMaxPipe, 0, 0, INFINITE, NULL ) ;
    
    ……
    
    // 把命名管道与完成端口关联起来
    
    HANDLE hRet = CreateIoCompletionPort ( PipeInst[i].hPipe, hCompletionPort, i, nMaxThread ) ;
    
    ……
    
    // 等待连接
    
    ConnectNamedPipe ( PipeInst[i].hPipe, &(PipeInst[i].ov) ) ;

  }

  // 创建线程

  for ( i = 0; i < nMaxThread; i++ )

  {

    hThread[i] = AfxBeginThread ( ServerThread, NULL, THREAD_PRIORITY_NORMAL ) ;

  }

  ……

}

void CMyDlg::OnStop() 

{

  for ( UINT i = 0; i < nMaxThread; i++ )

  {

    // 用来唤醒线程的虚假I/O完成包
    
    PostQueuedCompletionStatus ( hCompletionPort, 0, -1, NULL ) ;
    
    CloseHandle ( hThread[i] ) ;

  }

  for ( i = 0; i < nMaxPipe; i++ )

  {

    DisconnectNamedPipe ( PipeInst[i].hPipe ) ;
    
    CloseHandle ( PipeInst[i].hPipe ) ;

  }

  ……

}
```


4、心得体会
  上面这个例子是关于完成端口的简单应用。可以这样来理解完成端口，它与三种资源相关分别是管道、I/O完成包队列、线程队列，它的作用是协调这三种资源。

 

参考：http://www.pediy.com/bbshtml/bbs8/pediy8-709.htm

 

II、Linux同步I/O与异步I/O

linux中SIO（同步IO）和AIO（异步IO）机制
1.read/write:
对于read操作来说，它是同步的。也就是说只有当申请读的内容真正存放到buffer中后（user mode的buffer），read函数才返回。在此期间，它会因为等待IO完成而被阻塞。研究过源码的朋友应该知道，这个阻塞发生在两个地方：一是read操作刚刚发起，kernel会检查其它进程的need_sched标志，如果有其它进程需要调度，主动阻塞read操作，这个时候其实I/O操作还没有发起。二是I/O操作发起后，调用lock_page对已经加锁的页面申请锁，这时由于页面已经加锁，所以加锁操作被阻塞，从而read操作阻塞，直到I/O操作完成后页面被解锁，read操作继续执行。所以说read是同步的，其阻塞的原因如上。
对于write操作通常是异步的。因为linux中有page cache机制，所有的写操作实际上是把文件对应的page cache中的相应页设置为dirty，然后write操作返回。这个时候对文件的修改并没有真正写到磁盘上去。所以说write是异步的，这种方式下write不会被阻塞。如果设置了O_SYNC标志的文件，write操作再返回前会把修改的页flush到磁盘上去，发起真正的I/O请求，这种模式下会阻塞。
2.Direct I/O
linux支持Direct I/O, 以O_DIRCET标志打开的文件，在read和write的时候会绕开page cache，直接使用user mode的buffer做为I/O操作的buffer。这种情况下的read和write直接发起I/O操作，都是同步的，并会被阻塞。
3.AIO
目前大多数的linux用的AIO是基于2.4内核中的patch，使用librt库中的接口。这种方式实现很简单，就是一个父进程clone出子进程帮其做I/O，完成后通过signal或者callback通知父进程。用户看来是AIO，实质还是SIO。linux kernel中AIO的实现概念类似，只不过是以一组kernel thread去做的。这些kernel thread做I/O的时候使用的是和Direct I/O相同的方式。
4.mmap()
抛开它中讲vm_area和page cache映射在一起的机制不说。真正发起I/O时和read、write使用的是相同的机制，同步阻塞。







# （十一）Windwos线程池

2017年01月16日 20:05:58 [wangpengk7788](https://me.csdn.net/wangpengk7788) 阅读数：601



 版权声明：本文为博主原创文章，未经博主允许不得转载。	https://blog.csdn.net/wangpengk7788/article/details/54577016



Mircosoft从Windows2000引入线程池API，并在Vista后对线程池重新构架，引入新的线程池API。以下所有线程池函数均适用于Vista以后的版本。

　　用Windows提供的线程池函数有以下几个好处：1，不必要用CreateThread创建线程；2，不必要管理自己线程；3，Windows封装好的线程池，效率高，性能优越。

### 1 异步方式调用函数

　　这种方式和我们用CreateThread创建线程的用法差不多，给定一个线程函数模板实现功能，然后API去调用这些线程函数。简单的线程函数模板如下：



```cpp
VOID CALLBACK SimpleCallback(
  [in, out]            PTP_CALLBACK_INSTANCE Instance,
  [in, out, optional]  PVOID Context
);
```



 　　第一个参数暂不解释，第二个参数是传给毁掉函数的任意值。

　　对应的线程池API函数为TrySubmitThreadpoolCallback，函数定义如下：



```cpp
BOOL WINAPI TrySubmitThreadpoolCallback(
  __in          PTP_SIMPLE_CALLBACK pfns,
  __in_out_opt  PVOID pv,
  __in_opt      PTP_CALLBACK_ENVIRON pcbe
);
```



 　　第一个参数是线程函数，第二个参数是给线程函数传的值（Context），第三个参数用作对线程池定制。返回值为TRUE表示线程后续将启动，FALSE表示调用失败。一个小例子为：



```cpp
#include <iostream>
#include <windows.h>
#include <winbase.h>
#include <strsafe.h>
using namespace std;
 
 
extern "C" BOOL WINAPI TrySubmitThreadpoolCallback(
   PTP_SIMPLE_CALLBACK  pfns,
     PVOID                pv,
    PTP_CALLBACK_ENVIRON pcbe
    );
 
 
void CALLBACK MyProc(PTP_CALLBACK_INSTANCE instance,PVOID pContext)
{
    SYSTEMTIME tm;
    GetLocalTime(&tm);
    char strText[MAX_PATH];
    StringCchPrintfA(strText,MAX_PATH,"MyProc is runing within thread ID is %d ,current time %d:%d"   \
                     ,GetCurrentThreadId(),tm.wHour,tm.wMinute);
    cout<<strText<<endl;
 
}
 
int main(int argc, char *argv[])
{
 
    cout<<"Call the TrySubmitThreadpoolCallback function to execute myproc,"   \
                  "thread ID of caller is "<<GetCurrentThreadId()<<endl;
    TrySubmitThreadpoolCallback(MyProc,NULL,NULL);
 
    SleepEx(INFINITE,FALSE);
    return 0;
}
```





 

 　　利用TrySubmitThreadpoolCallback函数有可能失败，这时线程将不会启动。为了确保线程能够启动过，必须显示创建一个工作项对象，知道把工作项提交到线程池中。创建工作项函数如下：



```cpp
PTP_WORK WINAPI CreateThreadpoolWork(
  __in         PTP_WORK_CALLBACK pfnwk,
  __inout_opt  PVOID pv,
  __in_opt     PTP_CALLBACK_ENVIRON pcbe
);
```



　　第一个参数是线程函数，第二个参数是给线程函数传的值（Context），第三个参数用作对线程池定制。这个函数使用的县曾函数模板是：



```cpp
VOID CALLBACK WorkCallback(
  __inout      PTP_CALLBACK_INSTANCE Instance,
  __inout_opt  PVOID Context,
  __inout      PTP_WORK Work
);
```



　　创建工作项后，需要提交工作项给线程池，让线程池分配线程处理该项。提交工作项请求的函数是SubmitThreadpoolWork，定义如下：



```cpp
VOID WINAPI SubmitThreadpoolWork(
  __inout  PTP_WORK pwk
);
```



　　等待工作项完成的函数是WaitForThreadpoolWorkCallbacks，定义如下：



```cpp
VOID WINAPI WaitForThreadpoolWorkCallbacks(
  __inout  PTP_WORK pwk,
  __in     BOOL fCancelPendingCallbacks
);
```



　　　该函数的第二个参数，如果传值为TRUE，试图取消提交的工作项。如果工作项已启动，则等待；FALSE，当前线程挂起，直到工作项完成。

　　　取消工作项的函数是CloseThreadpoolWork，定义如下：



```cpp
VOID WINAPI CloseThreadpoolWork(
  __inout  PTP_WORK pwk
);
```



　　一个简单的例子为：



```cpp
 1 VOID CALLBACK WorkThread1(PTP_CALLBACK_INSTANCE Instance,PVOID Context,PTP_WORK Work)
 2 {
 3     for (int i=0;i<5;i++)
 4     {
 5         AfxMessageBox(_T("this is a WorkCallback fun"));
 6         Sleep(1000);
 7     }
 8 }
 9 
10 int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
11 {
12     ...
13     PTP_WORK pw = CreateThreadpoolWork(WorkThread1,NULL,NULL);
14     SubmitThreadpoolWork(pw);
15     //给线程足够多的时间启动
16     Sleep(2000);
17     //TRUE，试图取消提交的工作项。如果工作项已启动，则等待；
18     //FALSE，当前线程挂起，直到工作项完成
19     WaitForThreadpoolWorkCallbacks(pw,TRUE);
20     CloseThreadpoolWork(pw);
21     ...
22 }
```



 

### 2 每隔一段时间调用一次线程函数

　　先给出时间间隔线程函数模板

```cpp
1 VOID CALLBACK TimerCallback(
2   [in, out]            PTP_CALLBACK_INSTANCE Instance,
3   [in, out, optional]  PVOID Context,
4   [in, out]            PTP_TIMER Timer
5 );
```

　　CreateThreadpoolTimer创建时间间隔计时器对象

```cpp
1 PTP_TIMER WINAPI CreateThreadpoolTimer(
2   __in          PTP_TIMER_CALLBACK pfnti,
3   __in_out_opt  PVOID pv,
4   __in_opt      PTP_CALLBACK_ENVIRON pcbe
5 );
```

　　这个函数的参数和创建工作项的参数差不多。创建完毕后，调用SetThreadpoolTimer注册计时器。

![复制代码](http://common.cnblogs.com/images/copycode.gif)

```cpp
1 VOID WINAPI SetThreadpoolTimer(
2   __in_out      PTP_TIMER pti,
3   __in_opt      PFILETIME pftDueTime,
4   __in          DWORD msPeriod,
5   __in_opt      DWORD msWindowLength
6 );
```

![复制代码](http://common.cnblogs.com/images/copycode.gif)

　　第一个参数是CreateThreadpoolTimer的返回值，第二个参数是第一次调用的开始时间，如果传值为NULL，表示停止调用。-1，立即开始。其他负值，表示调用SetThreadpoolTimer的相对时间。正值，表示，以1600年1月1日开始的绝对时间。第三个参数是调用时间间隔，0表示只调用一次。第4个参数用来个调用时间间隔增加随机性，比如第3个参数传递5，第4个参数传递2，表示在5-7毫秒内随机时间内调用回调函数。这样可以避免多个线程在同一时间调用上的冲突。

　　查看计时器是否被设置IsThreadpoolTimerSet

```cpp
1 BOOL WINAPI IsThreadpoolTimerSet(
2   __in_out      PTP_TIMER pti
3 );
```

　　等待计时器完成WaitForThreadpoolTimerCallbacks

```cpp
1 VOID WINAPI WaitForThreadpoolTimerCallbacks(
2   __in_out      PTP_TIMER pti,
3   __in          BOOL fCancelPendingCallbacks
4 );
```

　　关闭计时器CloseThreadpoolTimer

```cpp
1 VOID WINAPI CloseThreadpoolTimer(
2   __in_out      PTP_TIMER pti
3 );
```

　　一个小例子：



```cpp
#define _WIN32_WINNT 0x0600
#include <iostream>
#include <windows.h>
#include <winbase.h>
#include <strsafe.h>
#include <threadpoolapiset.h>
using namespace std;
 
 
 
 
void CALLBACK MyProc(PTP_CALLBACK_INSTANCE instance,PVOID pContext,PTP_WORK Work)
{
    SYSTEMTIME tm;
    GetLocalTime(&tm);
    char strText[MAX_PATH];
    StringCchPrintfA(strText,MAX_PATH,"MyProc is runing within thread ID is %d ,current time %d:%d"   \
                     ,GetCurrentThreadId(),tm.wHour,tm.wMinute);
    cout<<strText<<endl;
 
}
 
 
 
int main(int argc, char *argv[])
{
 
    PTP_WORK pWork =  CreateThreadpoolWork(MyProc,NULL,NULL);
    cout<<"thread ID of caller is "<<GetCurrentThreadId()<<endl;
    for(int i= 0; i !=5 ;++i)
    {
            SubmitThreadpoolWork(pWork);
    }
 
    WaitForThreadpoolWorkCallbacks(pWork,FALSE);
    CloseThreadpoolWork(pWork);
 
    return 0;
}
```





###  3 内核对象触发时调用一个函数

　　内核对象触发线程函数模板



```cpp
VOID CALLBACK WaitCallback(
  __inout      PTP_CALLBACK_INSTANCE Instance,
  __inout_opt  PVOID Context,
  __inout      PTP_WAIT Wait,
  __in         TP_WAIT_RESULT WaitResult
);
```



　　CreateThreadpoolWait创建线程池等待对象



```cpp
PTP_WAIT WINAPI CreateThreadpoolWait(
  __in         PTP_WAIT_CALLBACK pfnwa,
  __inout_opt  PVOID pv,
  __in_opt     PTP_CALLBACK_ENVIRON pcbe
);
```



 　　将内核对象绑定到线程池



```cpp
VOID WINAPI SetThreadpoolWait(
  __inout   PTP_WAIT pwa,
  __in_opt  HANDLE h,
  __in_opt  PFILETIME pftTimeout
);
```



　　第一个参数是CreateThreadpoolWait的返回值，第二个参数是内核对象句柄，第三个参数是表示线程池要花多长时间等待内核对象被触发，0表示不等待，NULL表示无限等待，负值表示相对时间，正值表示绝对时间。

　　　WaitCallback最后一个参数表示回调函数被调用的原因。如果WaitResult是WAIT_OBJECT_0，表示SetThreadpoolWait的内核对象在超时之前被触发；WAIT_TIMEOUT表示超时；WAIT_ABANDONED_0表示内核对象是一个互斥信号量，且别抛弃。

　　等待完成

```cpp
 VOID WINAPI WaitForThreadpoolWaitCallbacks(
  __in_out      PTP_WAIT pwa,
  __in          BOOL fCancelPendingCallbacks
);
```

 　　关闭

```cpp
VOID WINAPI CloseThreadpoolWait(
in_out      PTP_WAIT pwa
);
```

　　小例子：

![复制代码](http://common.cnblogs.com/images/copycode.gif)

```cpp
 1 VOID CALLBACK WorkThread3(PTP_CALLBACK_INSTANCE Instance,PVOID Context,PTP_WAIT Wait,TP_WAIT_RESULT WaitResult)
 2 {
 3     AfxMessageBox(_T("this is a WaitCallback fun"));
 4 }
 5 
 6 int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
 7 {
 8     ...
 9 　　PTP_WAIT pw = CreateThreadpoolWait(WorkThread3,NULL,NULL);
10     HANDLE h = CreateEvent(NULL,TRUE,FALSE,NULL);
11     //关联内核对象
12     SetThreadpoolWait(pw,h,NULL);
13     //0表示不等待，NULL表示无限等待，负值表示相对时间，正值表示绝对时间
14     Sleep(1000);
15     //触发内核对象
16     SetEvent(h);
17     Sleep(1000);
18     WaitForThreadpoolWaitCallbacks(pw,TRUE);
19     CloseThreadpoolWait(pw);
20     CloseHandle(h);
21     ...
22 }
```

![复制代码](http://common.cnblogs.com/images/copycode.gif)

 

### 4 异步IO完成时调用函数

　　回调函数模板原型：

![复制代码](http://common.cnblogs.com/images/copycode.gif)

```cpp
1 VOID CALLBACK IoCompletionCallback(
2   [in, out]            PTP_CALLBACK_INSTANCE Instance,
3   [in, out, optional]  PVOID Context,
4   [in, out, optional]  PVOID Overlapped,
5   [in]                 ULONG IoResult,
6   [in]                 ULONG_PTR NumberOfBytesTransferred,
7   [in, out]            PTP_IO Io
8 );
```

![复制代码](http://common.cnblogs.com/images/copycode.gif)

 

　　创建线程池IO对象

![复制代码](http://common.cnblogs.com/images/copycode.gif)

```cpp
1 PTP_IO WINAPI CreateThreadpoolIo(
2   __in          HANDLE fl,
3   __in          PTP_WIN32_IO_CALLBACK pfnio,
4   __in_out_opt  PVOID pv,
5   __in_opt      PTP_CALLBACK_ENVIRON pcbe
6 );
```

![复制代码](http://common.cnblogs.com/images/copycode.gif)

　　第一个参数是具有OVERLAPPED标识的设备（文件、socket）句柄。其他三项不多说。

　　将线程池IO对象与线程池内部的完成端口关联：

```cpp
1 VOID WINAPI StartThreadpoolIo(
2   __in_out      PTP_IO pio
3 );
```

　　取消或解除与线程池关联：　　

![复制代码](http://common.cnblogs.com/images/copycode.gif)

```cpp
1 VOID WINAPI CancelThreadpoolIo(
2   __in_out      PTP_IO pio
3 );
4 
5 
6 VOID WINAPI CloseThreadpoolIo(
7   __in_out      PTP_IO pio
8 );
```

![复制代码](http://common.cnblogs.com/images/copycode.gif)

 

　　等待线程池内核对象返回

```cpp
1 VOID WINAPI WaitForThreadpoolIoCallbacks(
2   __in_out      PTP_IO pio,
3   __in          BOOL fCancelPendingCallbacks
4 );
```

　　例子：





```cpp
#define _WIN32_WINNT 0x0600
#include <iostream>
#include <windows.h>
#include <winbase.h>
#include <strsafe.h>
#include <threadpoolapiset.h>
using namespace std;
 
typedef struct MYOVERLAPPED
{
    OVERLAPPED overlapped;
    enum{read,write}modle;
    LPVOID pBuffer;
    DWORD dwNumByte;
};
 
 
VOID CALLBACK MyIoCompletionFunc(PTP_CALLBACK_INSTANCE instance,PVOID pContext,PVOID pOverlapped,    \
                                 ULONG IoResult,ULONG NumOfTransferred,PTP_IO Io)
{
    char * strBuffer = (char*) (((MYOVERLAPPED*)pOverlapped)->pBuffer);
    if(IoResult == NO_ERROR)
    {
        cout<<"Reading file completion!!"<<endl<<endl;
        cout<<strBuffer<<endl;
    }
}
 
int main(int argc,char *argv[])
{
    char * str_filename = "C:\\Users\\Administrator\\Desktop\\1.txt";
    HANDLE hFile = CreateFileA(str_filename,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,    \
                               FILE_FLAG_OVERLAPPED|FILE_FLAG_NO_BUFFERING,NULL);
    if(!hFile && GetLastError() == ERROR_FILE_NOT_FOUND)
    {
        cout<<"Failed to found the file \""<<str_filename<<"\" !"<<endl;
    }
    PTP_IO pIo = CreateThreadpoolIo(hFile,MyIoCompletionFunc,NULL,NULL);
    if(pIo)
    {
        StartThreadpoolIo(pIo);
    }
    MYOVERLAPPED overlap;
    ZeroMemory(&overlap,sizeof(MYOVERLAPPED));
    LARGE_INTEGER fileSize;
    GetFileSizeEx(hFile,&fileSize);
    DWORD dwSize = (64-fileSize.QuadPart%64)+fileSize.QuadPart;
    char *strBuffer =  (char *)VirtualAlloc(NULL,dwSize,MEM_COMMIT,PAGE_READWRITE);//new char[fileSize.LowPart];
    overlap.modle = MYOVERLAPPED::read;
    overlap.pBuffer = strBuffer;
    overlap.dwNumByte = fileSize.QuadPart;
 
    DWORD dwReaded;
    if(!ReadFile(hFile,strBuffer,dwSize,&dwReaded,(LPOVERLAPPED)&overlap))
    {
        if(ERROR_IO_PENDING == GetLastError())
        {
            cout<<"Post IRP of reading to the file device"<<endl;
        }
        else
        {
            cout<<"Read file failed!"<<endl;
        }
    }
    Sleep(2000);
    WaitForThreadpoolIoCallbacks(pIo,FALSE);
    SleepEx(INFINITE,FALSE);
 
 
    return 0;
}
```



###  5 回调函数的终止操作

　　回调函数内部可以使用以下几个API，当回调函数返回后，线程池将释放锁。 

![img](http://images2015.cnblogs.com/blog/964672/201612/964672-20161203193517052-835147864.png)

　　此外，还有CallbackMayRunLong函数，通知线程池当前任务运行时间比较长。如果CallbackMayRunLong函数返回TRUE，表示线程池中还有其他线程可用。返回FALSE，表示线程池中无其他任务可用。

### 6 对线程池进行配置

 　　这里说的线程池配置是对函数TrySubmitThreadpoolCallback用到的线程池（其他几种WORK\WAIT\TIMER\IO都用到内核对象，内核对象是不开源的），其他几种用到的都是系统默认的线程池，生命周期和进程一致。

　　创建一个线程池 CreateThreadpool

```cpp
PTP_POOL WINAPI CreateThreadpool(
  PVOID reserved
);
```

 

 　　设置私有线程池的上下线程数量

![复制代码](http://common.cnblogs.com/images/copycode.gif)

```cpp
1 BOOL WINAPI SetThreadpoolThreadMinimum(
2   __in_out      PTP_POOL ptpp,
3   __in          DWORD cthrdMic
4 );
5 
6 VOID WINAPI SetThreadpoolThreadMaximum(
7   __in_out      PTP_POOL ptpp,
8   __in          DWORD cthrdMost
9 );
```

![复制代码](http://common.cnblogs.com/images/copycode.gif)

　　关闭线程池

```cpp
1 VOID WINAPI CloseThreadpool(
2   __in_out      PTP_POOL ptpp
3 );
```





# （十二） 纤程

2017年01月16日 20:19:54 [wangpengk7788](https://me.csdn.net/wangpengk7788) 阅读数：219



版权声明：本文为博主原创文章，未经博主允许不得转载。	https://blog.csdn.net/wangpengk7788/article/details/54577112

纤程（Fiber），是微软加入到Windows中，使得UNIX服务器应用程序更好地移植到Windows中。所以本篇真正没有多少应用价值，只是为了使得笔记更加完整。

　　看完本章，感觉纤程是比线程的更小的一个运行单位。可以把一个线程拆分成多个纤程，然后通过人工转换纤程，从而让各个纤程工作。


　　要知道的是人工的转换，不是系统自动切换。因为线程的实现通过Windows内核完成的，因此Windows可以自动对线程进行调度。但是纤程是通过用户模式的代码来实现的，是程序员自己写的算法，内核不知道纤程的实现方式，而是你自己定义的调度算法，因此纤程是“非抢占”的调度方式。


　　还有要知道就是，一个线程可以包含多个纤程。

　　要使用纤程，首先要做的就是把当前线程转换为纤程：

```
PVOID ConvertThreadToFiber(PVOID pvParam);
```


　　调用这个函数之后，系统为纤程执行环境分配大概200字节的存储空间，这个执行环境有以下内容构成：


1、用户定义的值，由参数pvParam参数指定。


2、结构化异常处理链头。


3、纤程内存栈的最高和最低地址，当线程转换为纤程的时候，这也是线程的内存栈。


4、各种CPU寄存器信息，比如堆栈指针寄存器，指令指针寄存器等等。


 


　　默认情况下，x86系统的CPU的浮点数状态信息在纤程看来不属于CPU寄存器，因此会导致在纤程中执行一些相关的浮点运算会破坏数据。为了克服这个缺点，你需要呼叫ConvertThreadToFiberEx函数（Windows Vista及其以上版本中才有），并且传递FIBER_FLAG_FLOAT_SWITCH给它的第2个参数dwFlags：

```
PVOID ConvertThreadToFiberEx(
   PVOID pvParam,
   DWORD dwFlags);
```


　　当呼叫完上述两个函数之后，你就初始化了一个纤程执行环境，该执行环境与线程的执行环境关联，线程转换为纤程，纤程就在线程的内部运行。ConvertThreadToFiber(Ex)函数实际返回纤程的执行环境的内存地址，你稍后会用到这个地址，但是你不能直接读取或写入这个地址，你应该使用系统提供的纤程函数来对这个地址进行操纵。


　　当你的纤程返回或者呼叫ExitThread的时候，你的纤程也随之结束。


 


　　如果一个线程中只有一个纤程，那么是没有必要将该线程转换为纤程的，只有你打算在同一个线程中再创建一个纤程才有转换的必要。要创建一个纤程，使用CreateFiber函数：

```
PVOID CreateFiber(
   DWORD dwStackSize,     // 创建新的堆栈的大小，0表示默认大小
   PFIBER_START_ROUTINE pfnStartAddress,     // 纤程函数地址
   PVOID pvParam);     // 传递给纤程函数的参数
```

 


　　这个函数创建一个新的堆栈，堆栈的大小由dwStackSize指定。如果传递0给它，就意味着创建一个默认大小的堆栈。


　　如果你打算让一个线程包含多个纤程，而又想花费比较少的空间的话，可以使用CreateFiberEx函数（只有在Windows Vista及其以上版本中才有）：




```
PVOID CreateFiberEx(
   SIZE_T dwStackCommitSize,     // 堆栈初始提交的大小
   SIZE_T dwStackReserveSize,    // 需要保留的虚拟内存的大小
   DWORD dwFlags,     // 创建旗标
   PFIBER_START_ROUTINE pStartAddress,     // 纤程函数指针
   PVOID pvParam);     // 传递给纤程函数的参数
```



 


　　其中，如果传递FIBER_FLAG_FLOAT_SWITCH给dwFlags参数，则表明将浮点信息添加到纤程执行环境。


 


　　当CreateFiber(Ex)函数创建了一个新的堆栈之后，它分配一个新的纤程执行环境结构并初始化之，用户定义的数据通过pvParam参数被保存，新的堆栈的内存空间的最高和最低地址被保存，纤程函数的地址通过pStartAddress参数被保存。


　　纤程函数的格式必须如下定义：

```
VOID WINAPI FiberFunc(PVOID pvParam);
```


　　这个纤程在第一次被调度的时候，纤程函数被调用，其参数pvParam由CreateFiber(Ex)中的pvParam参数指定。在纤程函数中，你可以做你想做的任何事情。


　　像ConvertThreadToFiber(Ex)函数一样，CreateFiber(Ex)也返回纤程执行环境的内存地址，这个内存地址就像句柄一样，直接标识着一个纤程。


　　当你使用CreateFiber(Ex)函数创建一个纤程之后，该纤程不会执行，因为系统不会自动调度它。你必须调用函数SwitchToFiber来告诉系统你想要哪个纤程执行：

```
VOID SwitchToFiber(PVOID pvFiberExecutionContext);
```

 


　　SwitchToFiber函数的参数是一个纤程执行环境的内存地址，该地址由ConverThreadToFiber(Ex)或CreateFiber(Ex)返回。


　　SwitchToFiber函数内部的执行步骤如下：


1、保存当前的CPU寄存器信息，这些信息保存在正在运行的纤程的执行环境中。


2、从将要执行的纤程的执行环境中加载上次保存的CPU寄存器信息。


3、将即将执行的纤程执行环境与线程关联起来，由线程执行指定的纤程。


4、将指令指针设置为保存的值，继续上次的执行。


 


　　SwitchToFiber函数是一个纤程能够被调度的唯一的方法，因此，纤程的调度是由用户完全操纵的。纤程的调度和线程的调度无关。一个线程，包含了正在运行的纤程，仍会被其他线程抢占。当一个线程被调度，而它里面有几个纤程，那么只有被选择的那个纤程才会执行，其他纤程的执行需要调用SwitchToFiber函数。


 


　　最后，如果一个纤程完成了任务，你需要删除它，呼叫DeleteFiber函数，并传递这个纤程的执行环境内存地址：

```
VOID DeleteFiber(PVOID pvFiberExecutionContext);
```

 


　　 该函数首先清除纤程堆栈，然后删除纤程执行环境。但是，如果参数指定的是一个与当前线程关联的纤程，该函数呼叫ExitThread函数，线程结束，其包含的其他纤程也都结束。因此，DeleteFiber函数一般是由一个纤程调用来删除另一个纤程。


　　当所有纤程结束了运行，你需要从纤程转换为线程，呼叫ConvertFiberToThread函数。


 


　　如果你需要在纤程中保存一些数据，可以使用“纤程局部存储”（FLS）的机制。这个机制和“线程局部存储”（TLS）类似。


　　首先，呼叫FlsAlloc函数分配FLS槽来存放数据，这个FLS槽可以被当前进程内所有纤程共同使用，函数有一个参数：一个回调函数指针，这个回调函数会在以下两种情况下被调用：一个纤程被删除；FLS槽通过FlsFree函数被删除。


　　然后，在你呼叫FlsAlloc函数之后，你可以在纤程中使用FlsSetValue函数来保存数据到FLS槽中，同时该函数需要一个DWORD类型的参数，表示一个FLS槽的索引，即在FLS槽的相关地方保存数据。


　　接着，你可以在各个纤程中使用FlsGetValue函数来取得FLS槽中对应的数据，同样需要上面那个FLS槽索引，并返回指向数据的指针。


　　当使用完这些数据之后，你可以使用FlsFree来释放FLS槽。


 


　　如果你想知道你是否正在一个纤程执行环境中运行，可以使用IsThreadAFiber函数，它返回一个BOOL值，指明你是否正在一个纤程中运行。


 


　　一个线程每次只能执行一个纤程，该纤程与这个线程相关联。你可以使用如下函数来得到正在执行的纤程的执行环境内存地址：

```
PVOID GetCurrentFiber();
```

 


　　每个纤程包含用户定义的一个数据，这个数据由CreateFiber(Ex)或ConvertThreadToFiber(Ex)的pvParam参数指定，你可以使用如下函数得到这个数据的指针：


PVOID GetFiberData();
 


　　最后，让我们假设一个线程中有2个纤程，总结一下纤程的用法：


1、使用ConverThreadToFiber(Ex)将当前线程转换到纤程，这是纤程F1


2、定义一个纤程函数，用于创建一个新纤程


3、纤程F1中调用CreateFiber(Ex)函数创建一个新的纤程F2


4、SwitchToFiber函数进行纤程切换，让新创建的纤程F2执行


5、F2纤程函数执行完毕的时候，使用SwitchToFiber转换到F1


6、在纤程F1中调用DeleteFiber来删除纤程F2


7、纤程F1中调用ConverFiberToThread，转换为线程

8、线程结束









# （十三）Windwos内存体系结构

2017年01月19日 22:02:39 [wangpengk7788](https://me.csdn.net/wangpengk7788) 阅读数：1199



 版权声明：本文为博主原创文章，未经博主允许不得转载。	https://blog.csdn.net/wangpengk7788/article/details/54619746

Windows内存管理机制，底层最核心的东西是分页机制。分页机制使每个进程有自己的4G虚拟空间，使我们可以用虚拟线性地址来跑程序。每个进程有自己的工作集，工作集中的数据可以指明虚拟线性地址对应到怎样的物理地址。进程切换的过程也就是工作集切换的过程，如Matt Pietrek所说如果只给出虚拟地址而不给出工作集，那这个地址是无意义的。（见图一）

 

​         在分页机制所形成的线性地址空间里，我们对内存进行进一步划分涉及的概念有堆、栈、自由存储等。对堆进行操作的API有HeapCreate、HeapAlloc等。操纵自由存储的API有VirtualAlloc等。此外内存映射文件使用的也应该算是自由存储的空间。栈则用来存放函数参数和局部变量，随着stack frame的建立和销毁其自动进行增长和缩减。

 

​         说到这里，也许有人会提出疑问：对x86 CPU分段机制是必须的，分页机制是可选的。为什么这里只提到了分页机制。那么我告诉你分段机制仍然存在，一是为了兼容以前的16位程序，二是Windows毕竟要区分ring 0和ring 3两个特权级。用SoftIce看一下GDT(全局描述表)你基本上会看到如下内容：

 

```
GDTbase=80036000 Limit=03FF


0008Code32 Base=00000000 Lim=FFFFFFFF DPL=0 P RE 
//内核态driver代码段

0010Data32 Base=00000000 Lim=FFFFFFFF DPL=0 P RW
//内核态driver的数据段

 

001BCode32 Base=00000000 Lim=FFFFFFFF DPL=3 P RE 
//应用程序的代码段

 

0023Data32 Base=00000000 Lim=FFFFFFFF DPL=3 P RW 
//应用程序的数据段
```

 

这意味着什么呢？

 

我们再看一下线性地址的生成过程（见图一）。从中我们应该可以得出结论，如果segmeng base address为0的话，那么这个段可以看作不存在，因为偏移地址就是最终的线性地址。

 

此外还有两个段存在用于Kernel Processor Control Region和user thread environment block。所以如果你在反汇编时看到MOVECX,FS:[2C]就不必惊讶，怎么这里使用逻辑地址而不是线性地址。在以后涉及异常处理的地方会对此再做说明。

 

![img](https://img-blog.csdn.net/20170119220030834?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2FuZ3BlbmdrNzc4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

 

二、从Stack说开去

 

从我个人的经验看，谈到内存时说堆的文章最多，说stack的最少。我这里反其道而行的原因是stack其实要比堆更重要，可以有不使用堆的程序，但你不可能不使用stack，虽然由于对stack的管理是由编译器确定了的，进而他较少出错。

 

通过链接开关/STACK:reserve[,commit]可以指定进程主线程的stack大小，当你建立其他线程时如果不指定dwStackSize参数，则也将使用/STACK所指定的值。微软说，如果指定较大的commit值将有利于提升程序的速度，我没验证过，但理应如此。通常并不需要对STACK进行什么设定，缺省的情况下将保留1M空间，并提交两个页（8K for x86）。而1M空间对于大多数程序而言是足够的，但为防止stack overflow有三点需要指出一是当需要非常大的空间时最好用全局数组或用VirtualAlloc进行分配，二是引用传递或用指针传递尺寸较大的函数参数（这点恐怕地球人都知道），三是进行深度递归时一定要考虑会不会产生stack溢出，如果有可能，可以采用我在《递归与goto》一文中提到的办法来仿真递归，这时候可以使用堆或自由存储来代替stack。同时结构化异常被用来控制是否为stack提交新的页面。（这部分写的比较简略因为很多人都写过，推荐阅读Jeffery Ritcher《Windows核心编程》第16章）

 

下面我们来看一下stack的使用。

假设我们有这样一个简单之极的函数：

 

 

```c
int __stdcall add_s(int x,inty)
{
     int sum;
     sum=x+y;
     return sum;
}
```

 

这样在调用函数前，通常我们会看到这样的指令。

```asm
mov         eax,dword ptr [ebp-8]
push        eax
mov         ecx,dword ptr [ebp-4]
push        ecx
```

此时把函数参数压入堆栈，而stack指针ESP递减，stack空间减小。

 

在进入函数后，你将会看到如下指令：

```asm
push        ebp
mov         ebp,esp
sub         esp,44h
```

这三句建立stack框架，并减小esp为局部变量预留空间。建立stack框架后，[ebp+*]指向函数参数，[ebp-*]指向局部变量。

 

另外在很多情况下你会看到如下三条指令

```asm
push        ebx
push        esi
push        edi
```

这三句把三个通用寄存器压入堆栈，这样这三个寄存器就可以用来存放一些变量，进而提升运行速度。

很奇怪，我这个函数根本用不到这三个寄存器，可编译器也生成了上述三条指令。

 

对stack中内容的读取，是靠基址指针ebp进行的。所以对应于sum=x+y;一句你会看到

```asm
mov         eax,dword ptr [ebp+8]
add         eax,dword ptr [ebp+0Ch]
mov         dword ptr [ebp-4],eax
```

其中[ebp+8]是x，[ebp+0Ch]是y，记住压栈方向为从右向左，所以y要在x上边。

 

我们再看一下函数退出时的情况：

```asm
pop         edi
pop         esi
pop         ebx
mov         esp,ebp
pop         ebp
ret         8
```

此时恢复stack框架，使esp与刚进入这个函数时相同，ret8使esp再加8，使esp与没调用这个函数的时候一致。如果使用__cdecl调用规则，则由调用方以类似add  esp,8进行清场工作，使stack的大小与未进行函数调用时一致。Stack的使用就这样完全被编译器实现了，只要不溢出就和我们无关，也许也算一种内存的智能管理。最后要补充的两点是：首先stack不像heap会自动扩充，如果你用光了储备，他会准时溢出。其次是不要以为你使用了缺省参数进行链接，你就有1M的stack，看看启动代码你就知道在你拥有stack之前，C Run–Time Library以用去了一小部分stack的空间。



## 2. 保护模式寻址基础知识



接下来就以32位系统为例，介绍保护模式下，内存中一些地址转换相关的寄存机和数据结构。

## 2.1 内存地址概念

逻辑地址 ：在进行[C语言](http://lib.csdn.net/base/c)编程中，能读取变量地址值(&操作)，实际上这个值就是逻辑地址，也可以是通过malloc或是new调用返回的地址。该地址是相对于当前进程数据段的地址，不和绝对物理地址相干。只有在Intel实模式下，逻辑地址才和物理地址相等（因为实模式没有分段或分页机制,CPU不进行自动地址转换）。应用程序员仅需和逻辑地址打交道，而分段和分页机制对一般程序员来说是完全透明的，仅由系统编程人员涉及。应用程序员虽然自己能直接操作内存，那也只能在操作系统给你分配的内存段操作。一个逻辑地址，是由一个段标识符加上一个指定段内相对地址的偏移量，表示为 [段标识符：段内偏移量]。

线性地址 ：是逻辑地址到物理地址变换之间的中间层。程序代码会产生逻辑地址，或说是段中的偏移地址，加上相应段的基地址就生成了一个线性地址。如果启用了分页机制，那么线性地址能再经变换以产生一个物理地址。若没有启用分页机制，那么线性地址直接就是物理地址。Intel 80386的线性地址空间容量为4G（2的32次方即32根地址总线寻址）。

物理地址（Physical Address） 是指出目前CPU外部地址总线上的寻址物理内存的地址信号，是地址变换的最终结果地址。如果启用了分页机制，那么线性地址会使用页目录和页表中的项变换成物理地址。如果没有启用分页机制，那么线性地址就直接成为物理地址了，比如在实模式下。

## 2.2 虚拟地址，线性地址，物理地址关系

对于保护模式下地址之间的转换，对程序员来说是透明的。那么物理内存通过内存管理机制是如何将虚拟地址转换为物理地址的呢？当程序中的指令访问某一个逻辑地址时，CPU首先会根据段寄存器的内容将虚拟地址转化为线性地址。如果CPU发现包含该线性地址的内存页不在物理内存中就会产生缺页异常，该异常的处理程序通过是操作系统的内存管理器例程。内存管理器得到异常报告后会根据异常的状态信息。特别是CR2寄存器中包含的线性地址，将需要的内存页加载到物理内存中。然后异常处理程序返回使处理器重新执行导致页错误异常的指令，这时所需要的内存页已经在物理内存中，所以便不会再导致页错误异常。

## 2.3 段式机制及实例分析

前面说到在线性地址转换为物理地址之前，要先由逻辑地址转换为线性地址。系统采用段式管理机制来实现逻辑地址到线性地址的转换。保护模式下，通过”段选择符+段内偏移”寻址最终的线性地址。

CPU的段机制提供一种手段可以将系统的内存空间划分为一个个较小的受保护的区域，每个区域为一个段。相对32位系统，也就是把4G的逻辑地址空间换分成不同的段。每个段都有自己的起始地址（基地址），边界和访问权限等属性。实现段机制的一个重要数据结构就是段描述符。

下面是个程序实例中显示除了各个段的值：

![3](http://blog.nsfocus.net/wp-content/uploads/2015/08/22.png)

图中给出了代码段CS,堆栈段SS，数据段DS等段寄存器的值；从得到的值可知，SS=DS=ES是相等的，至于为什么有些段的值相等，后面会说到。 以实例中给出的地址0x83e84110 为例，哪里是段描述符，哪里是段内偏移, 又是如何将该逻辑地址转换为线性地址的呢？相信很多人都迫不及待的想知道整个转换过程，接下来就要看看逻辑地址到线性地址详细转换过程。

上面说到段式管理模式下有段选择符+段内偏移寻址定位线性地址，其实际转换过程如下图所示

![4](http://blog.nsfocus.net/wp-content/uploads/2015/08/32.png)

从图中可知，逻辑地址到线性地址的转换，先是通过段选择符从描述符表中找到段描述符，把段描述符和偏移地址相加得到线性地址。也就是说要想得到段描述符需要三个条件：

1. 得到段选择符。
2. 得到段描述符表
3. 从段描述符表中找到段描述符的索引定位段描述符。

前面我们提到了段描述符 + 偏移地址，并没有提段选择符和段描述符表。所以我们要弄清楚这几个观念段选择符，段描述符表，段描述符，以及如何才能得到这几个描述符？

### 2.3.1 段描述符基础知识

从上图可知，通过段选择符要通过段描述符表找到段描述符，那么段描述符表是什么，又是怎么得到段描述符表呢？

在保护模式下，每个内存段就是一个段描述符。其结构如下图所示：

![5](http://blog.nsfocus.net/wp-content/uploads/2015/08/4.png)

图中看出，一个段描述符是一个8字节长的数据结构，用来描述一个段的位置、大小、访问控制和状态等信息。段描述符最基本内容是段基址和边界。段基址以4字节表示（图中可看出3,4,5,8字节）。4字节刚好表示4G线性地址的任意地址（0x00000000-0xffffffff）。段边界20位表示（1,2字节及7字节的低四位）。

### 2.3.2 段描述符表实例解析

在现在多任务系统中，通常会同时存在多个任务，每个任务会有多个段，每个段需要一个段描述符，段描述符在上面一小节已经介绍，因此系统中会有很多段描述符。为了便于管理，需要将描述符保存于段描述符表中，也就是上图画出的段描述符表。IA-32处理器有3中描述符表：GDT,LDT和IDT。

GDT是全局描述符表。一个系统通常只有一个GDT表。GDT表也即是上图中的段描述符表，供系统中所以程序和任务使用。至于LDT和IDT今天不是重点。

那么如何找到GDT表的存放位置呢？系统中提供了GDTR寄存器用了表示GDT表的位置和边界，也就是系统是通过GDTR寄存器找到GDT表的；在32位模式下，其长度是48位，高32位是基地址，低16位是边界；在IA-32e模式下，长度是80位，高64位基地址，低16位边界。

位于GDT表中的第一个表项（0号）的描述符保留不用，成为空描述符。如何查看系统的GDT表位置呢？通过查看GDTR寄存器，如下图所示

![6](http://blog.nsfocus.net/wp-content/uploads/2015/08/5.png)

从上图看出GDT表位置地址是0x8095000,gdtl值看出GDT边界1023，总长度1024字节。前面知道每一项段描述符占8字节。所以总共128个表项。图中第一表项是空描述符。

### 2.3.3 段选择符结构

前面我们介绍了段描述符表和段描述符的格式结构。那么如何通过段选择符找到段描述符呢，段选择符又是什么呢？

段选择符又叫段选择子，是用来在段描述符表中定位需要的段描述符。段选择子格式如下：

![7](http://blog.nsfocus.net/wp-content/uploads/2015/08/6.png)

段选择子占有16位两个字节，其中高13位是段描述在段描述表中的索引。低3位是一些其他的属性，这里不多介绍。使用13位地址，意味着最多可以索引8k=8192个描述符。但是我们知道了上节GDT最多128个表项。

在保护模式下所有的段寄存器（CS,DS,ES,FS,GS）中存放的都是段选择子。

### 2.3.4 逻辑地址到线性地址转换实例解析

已经了解了逻辑地址到虚拟地址到线性地址的转换流程，那就看看在前面图中逻辑地址0x83e84110对应的线性地址是多少？

首先，地址0x83e84110对应的是代码段的一个逻辑地址，地址偏移已经知道，也就是段内偏移知道，通过寄存器EIP得到是0x83e34110。段选择符是CS寄存器CS=0008，其高13位对应的GDT表的索引是1，也就是第二项段描述符（第一项是空描述符）。GDT表的第二项为标红的8个字节

![8](http://blog.nsfocus.net/wp-content/uploads/2015/08/7.png)

通过段描述的3,4，5，8个字节得到段基址。

![9](http://blog.nsfocus.net/wp-content/uploads/2015/08/8.png)

如上图所示第二项段描述符的3,4,5,8字节对应的值为0x00000000。由此我们得到了段机制和段内偏移。最后的线性地址为段基址+段内偏移=0x0+0x83e34110=0x83e34110。

由此我们知道在32系统中逻辑地址就是线性地址。

其实通过观察其他的段选择子会发现，所有段选择子对应的基地址都是0x0，这是因为在32系统保护模式下，使用了平坦内存模型，所用的基地址和边界值都一样。既然基地址都是0,那么也就是线性地址就等于段内偏移=逻辑地址。

总之：

1. 段描述符8字节
2. GDTR是48位
3. 段选择子2个字节。

## 2.4 页式机制及实例分析

前面介绍了由逻辑地址到线性地址的转换过程，那么接下来就要说说地址是如何将线性地址转为物理地址。需要先了解一些相关的数据结构。

前面说到如果CPU发现包含该线性地址的内存页不在物理内存中就会产生缺页异常，该异常的处理程序通过是操作系统的内存管理器例程。内存管理器得到异常报告后会根据异常的状态信息。特别是CR2寄存器中包含的线性地址，将需要的内存页加载到物理内存中。然后异常处理处理返回使处理器重新执行导致页错误异常的指令，这时所需要的内存页已经在物理内存中，所以便不会再导致也错误异常。

32位系统中通过页式管理机制实现线性地址到物理地址的转换，如下图：

![10](http://blog.nsfocus.net/wp-content/uploads/2015/08/9.png)

### 2.4.1 PDE结构及如何查找内存页目录

从上图中我们知道通过寄存器CR3可以找到页目录表。那么CR3又是什么呢？在32系统中CR3存放的页目录的起始地址。CR3寄存器又称为页目录基址寄存器。32位系统中不同应用程序中4G线性地址对物理地址的映射不同，每个应用程序中CR3寄存器也不同。也就是说每个应用程序中页目录基址也是不同的。

从上图知道页目录表用来存放页目录表项（PDE），页目录占一个4kb内存页，每个PDE长度为4字节，所以页目录最多包含1KB。没启用PAE时，有两种PDE，这里我们只讨论使用常见的指向4KB页表的PDE。

![11](http://blog.nsfocus.net/wp-content/uploads/2015/08/10.png)

页目录表项的高20位表示该PDE所指向的起始物理地址的高20位，该起始地址的低12位为0，也就是通过PDE高20位找到页表。由于页表低12位0，所以页表一定是4KB边界对齐。 也就是通过页目录表中的页目录表项来定位使用哪个页表（每一个应用程序有很多页表）。

以启动的calc程序为例，CR3寄存器是DirBase中的值，如下图

![12](http://blog.nsfocus.net/wp-content/uploads/2015/08/111.png)

Calc.exe程序对应的CR3寄存器值为0x2960a000,下面是对应PDT结构

### 2.4.2页表结构解析

页表是用来存放页表表项（PTE）。每一个页表占4KB的内存页，每个PTE占4个字节。所以每个页表最多1024个PTE。其中高20位代表要使用的最终页面的起始物理地址的高20位。所以4KB的内存页也都是4KB边界对齐。

![13](http://blog.nsfocus.net/wp-content/uploads/2015/08/121.png)





**三      进程地址空间**



·        **32|64位的系统|CPU**

​        操作系统运行在硬件CPU上，32位操作系统运行于32位CPU上，64位操作系统运行于64位CPU上；目前没有真正的64位CPU。

32位CPU一次只能操作32位二进制数；位数多CPU设计越复杂，软件设计越简单。

​       软件的进程运行于32位系统上，其寻址位也是32位，能表示的空间是232=4G，范围从0x0000 0000~0xFFFF FFFF。

·        **NULL指针分区**

范围：0x0000 0000~0x0000 FFFF

作用：保护内存非法访问

例子：分配内存时，如果由于某种原因分配不成功，则返回空指针0x0000 0000；当用户继续使用比如改写数据时，系统将因为发生访问违规而退出。

​        那么，为什么需要那么大的区域呢，一个地址值不就行了吗？我在想，是不是因为不让8或16位的程序运行于32位的系统上呢？！因为NULL分区刚好范围是16的进程空间。

·        **独享用户分区**

范围：0x0001 0000~0x7FFE FFFF

作用：进程只能读取或访问这个范围的虚拟地址；超越这个范围的行为都会产生违规退出。

例子：

​        程序的二进制代码中所用的地址大部分将在这个范围，所有exe和dll文件都加载到这个。每个进程将近2G的空间是独享的。

注意：如果在boot.ini上设置了/3G，这个区域的范围从2G扩大为3G：0x0001 0000~0xBFFE FFFF。

·        **共享内核分区**

范围：0x8000 0000~0xFFFF FFFF

作用：这个空间是供操作系统内核代码、设备驱动程序、设备I/O高速缓存、非页面内存池的分配、进程目表和页表等。

例子：

​       这段地址各进程是可以共享的。                                                                                                                                         

注意：如果在boot.ini上设置了/3G，这个区域的范围从2G缩小为1G：0xC000 0000~0xFFFF FFFF。

​       通过以上分析，可以知道，如果系统有n个进程，它所需的虚拟空间是：2G*n+2G (内核只需2G的共享空间)。

 

**地址映射**

·        **区域**

区域指的是上述地址空间中的一片连续地址。区域的大小必须是粒度(64k) 的整数倍，不是的话系统自动处理成整数倍。不同CPU粒度大小是不一样的，大部分都是64K。

区域的状态有：空闲、私有、映射、映像。

在你的应用程序中，申请空间的过程称作保留(预订)，可以用VirtualAlloc；删除空间的过程为释放，可以用VirtualFree。

​        在程序里预订了地址空间以后，你还不可以存取数据，因为你还没有付钱，没有真实的RAM和它关联。

这时候的区域状态是私有；

默认情况下，区域状态是空闲；

当exe或DLL文件被映射进了进程空间后，区域状态变成映像；

当一般数据文件被映射进了进程空间后，区域状态变成映射。

·        **物理存储器**

Windows各系列支持的内存上限是不一样的，从2G到64G不等。理论上32位CPU，硬件上只能支持4G内存的寻址；能支持超过4G的内存只能靠其他技术来弥补。顺便提一下，Windows个人版只能支持最大2G内存，Intel使用Address Windows Extension (AWE) 技术使得寻址范围为236=64G。当然，也得操作系统配合。

​        内存分配的最小单位是4K或8K，一般来说，根据CPU不同而不同，后面你可以看到可以通过系统函数得到区域粒度和页面粒度。

·        **页文件**

页文件是存在硬盘上的系统文件，它的大小可以在系统属性里面设置，它相当于物理内存，所以称为虚拟内存。事实上，它的大小是影响系统快慢的关键所在，如果物理内存不多的情况下。

​       每页的大小和上述所说内存分配的最小单位是一样的，通常是4K或8K。

·        **访问属性**

物理页面的访问属性指的是对页面进行的具体操作：可读、可写、可执行。CPU一般不支持可执行，它认为可读就是可执行。但是，操作系统提供这个可执行的权限。

PAGE_NOACCESS

PAGE_READONLY

PAGE_READWRITE

PAGE_EXECUTE

PAGE_EXECUTE_READ

PAGE_EXECUTE_READWRITE

这6个属性很好理解，第一个是拒绝所有操作，最后一个是接受收有操作；

PAGE_WRITECOPY

PAGE_EXECUTE_WRITECOPY

这两个属性在运行同一个程序的多个实例时非常有用；它使得程序可以共享代码段和数据段。一般情况下，多个进程只读或执行页面，如果要写的话，将会Copy页面到新的页面。通过映射exe文件时设置这两个属性可以达到这个目的。

PAGE_NOCACHE

PAGE_WRITECOMBINE

这两个是开发设备驱动的时候需要的。

PAGE_GUARD

当往页面写入一个字节时，应用程序会收到堆栈溢出通知，在线程堆栈时有用。

·        **映射过程**

进程地址空间的地址是虚拟地址，也就是说，当取到指令时，需要把虚拟地址转化为物理地址才能够存取数据。这个工作通过页目和页表进行。

![img](https://p-blog.csdn.net/images/p_blog_csdn_net/yeming81/process.JPG)

从图中可以看出，页目大小为4K，其中每一项(32位)保存一个页表的物理地址；每个页表大小为4K，其中每一项(32位)保存一个物理页的物理地址，一共有1024个页表。利用这4K+4K*1K=4.4M的空间可以表示进程的1024*1024* (一页4K) =4G的地址空间。

进程空间中的32位地址如下：

![img](https://p-blog.csdn.net/images/p_blog_csdn_net/yeming81/process1.JPG)

高10位用来找到1024个页目项中的一项，取出页表的物理地址后，利用中10位来得到页表项的值，根据这个值得到物理页的地址，由于一页有4K大小，利用低12位得到单元地址，这样就可以访问这个内存单元了。

​        每个进程都有自己的一个页目和页表，那么，刚开始进程是怎么找到页目所在的物理页呢？答案是CPU的CR3寄存器会保存当前进程的页目物理地址。

​        当进程被创建时，同时需要创建页目和页表，一共需要4.4M。在进程的空间中，0xC030 0000~0xC030 0FFF是用来保存页目的4k空间。0xC000 0000~0xC03F FFFF是用来保存页表的4M空间。也就是说程序里面访问这些地址你是可以读取页目和页表的具体值的（要工作在内核方式下）。有一点我不明白的是，页表的空间包含了页目的空间！

​        至于说，页目和页表是保存在物理内存还是页文件中，我觉得，页目比较常用，应该在物理内存的概率大点，页表需要时再从页文件导入物理内存中。

​        页目项和页表项是一个32位的值，当页目项第0位为1时，表明页表已经在物理内存中；当页表项第0位为1时，表明访问的数据已经在内存中。还有很多数据是否已经被改变，是否可读写等标志。另外，当页目项第7位为1时，表明这是一个4M的页面，这值已经是物理页地址，用虚拟地址的低22位作为偏移量。还有很多：数据是否已经被改变、是否可读写等标志。



四、浅谈一下Heap

(鉴于Matt Pietrek在它的《Windows 95系统程式设计大奥秘》对9x系统的heap做了非常详细的讲解，此处涉及的内容将仅限于Win2000)

 

Heap与Stack正好相反，你需要手动来管理每一块内存的申请和释放（在没有垃圾收集机制的情况下），而对于C/C++程序员来说，操作Heap的方式实在是太多了点。下面是几乎所有可以操作堆内存的方法的列表:

malloc/free

new/delete

GlobalAlloc/GlobalFree

LocalAlloc/LocalFree

HeapAlloc/HeapFree

 

其中malloc/free由运行时库提供，new/delete为C++内置的操作符。他们都使用运行时库的自己的堆。运行时库的在2000和win9x下都有自己独立的堆。这也就意味着只要你一启动进程，你将至少有两个堆，一个作为进程缺省，一个给C/C++运行时库。

 

GlobalAlloc/GlobalFree和LocalAlloc/LocalFree现在已失去原有的含义，统统从进程缺省堆中分配内存。

 

HeapAlloc/HeapFree则从指定的堆中分配内存。

 

单就分配内存而言（new/delete还要管构造和析构），所有这些方式最终要归结到一点2000和98下都是是HeapAlloc。所以微软才会强调GlobalAlloc/GlobalFree和LocalAlloc/LocalFree会比较慢，推荐使用HeapAlloc，但由于Global**和Local**具有较简单的使用界面，因此即使在微软所提供的源代码中他们仍被大量使用。必须指出的是HeapAlloc并不在kernel32.dll中拥有自己的实现，而是把所有调用转发到ntdll.RtlAllocateHeap。下面这张从msdn中截取的图（图2），应该有助于我们理解同堆相关的API。

 

堆内部的运作同SGI STL的分配器有些类似，大体上是这样，OS为每个堆维护几个链表，每个链表上存放指定大小范围的区块。当你分配内存时，操作系统根据你所提供的尺寸，先确定从那个链表中进行分配，接下来从那个链表中找到合适的块，并把其线性地址返还给你。如果你所要求的尺寸，在现存的区块中找不到，那么就新分配一块较大的内存（使用VirtualAlloc），再对他进行切割，而后向你返还某一区块的线性地址。这只是一个大致的情形，操作系统总在不停的更新自己的堆算法，以提高堆操作的速度。

堆本身的信息（包括标志位和链表头等）被存放在Heap Header中，而堆句柄正是指向Heap Header的指针，Heap Header的结构没有公开，稍后我们将试着做些分析。非常有趣的是微软一再强调只对toolhelp API有效的HeapID其实就是堆句柄。

 

原来是准备分析一下堆内部的一些结构的，可后来一想这么做实用价值并不是很大，所需力气却不小。因此也就没具体进行操作。但这里把实现监测堆中各种变化的小程序的实现思路公开一下，希望对大家有所帮助。这个小程序非常的简单，主要完成的任务就是枚举进程内所有的堆的变化情况。由于涉及到比较两个链表的不同，这里使用了STL的vector容器和某些算法来减少编码。同时为了使STL的内存使用不对我们要监测的对象产生干扰，我们需要建立自己的分配器，以使用我们单独创建的堆。此外还需要特别注意的一点是由于toolhelp APIHeap32Next在运行过程中不允许对任何堆进行扰动（否则他总返回TRUE）,导致我们只能使用vector，并预先保留足够的空间。（访问堆内部某些信息的另一种方式是使用**HeapWalk**API,看个人喜好了）。

 

程序的运行过程是这样的，先对当前进程中存在的堆进行枚举，并把结果存入一个set类型的变量heapid1，接下来创建自己的堆给分配器使用，并对进程中存在的堆再次进行枚举并把结果存入另一个set类型的变量heapid2，这样就可以调用set_difference求出我们新建堆的ID，再以后列举队内部的信息时将排除这个ID所表示的堆。接下来就可以在两点之间分别把堆内部的信息存入相应的vector，比较这两个vector，就可以得到对应于分配内存操作，堆内部的变化情况了。

 

![img](https://img-blog.csdn.net/20170119220216975?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2FuZ3BlbmdrNzc4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

 







# （十四）探索虚拟内存

2017年01月20日 00:39:38 [wangpengk7788](https://me.csdn.net/wangpengk7788) 阅读数：323



 版权声明：本文为博主原创文章，未经博主允许不得转载。	https://blog.csdn.net/wangpengk7788/article/details/54620381

在编程中对由于每个系统中页面大小，分配粒度等系统参数是不同的，因此在代码中不能将这些值写死，应该通过系统API动态获取这些值。

GetSystemInfo可以获取一些基本的系统信息





```cpp
void WINAPI GetSystemInfo(
  __out  LPSYSTEM_INFO lpSystemInfo
);
```

需要传入一个被写的SYSTEM_INFO结构体





```cpp
typedef struct _SYSTEM_INFO {
  union {
    DWORD  dwOemId;
    struct {
      WORD wProcessorArchitecture;  处理器体系结构
      WORD wReserved;
    } ;
  } ;
  DWORD     dwPageSize;   //页面大小
  LPVOID    lpMinimumApplicationAddress;  //进程最小可用虚拟机地址
  LPVOID    lpMaximumApplicationAddress; //进程私有空间最大可用内存地址
  DWORD_PTR dwActiveProcessorMask;  //活动CPU掩码
  DWORD     dwNumberOfProcessors;  //CPU数量
  DWORD     dwProcessorType;
  DWORD     dwAllocationGranularity;  //分配粒度
  WORD      wProcessorLevel;
  WORD      wProcessorRevision;
} SYSTEM_INFO;
```



测试：



```cpp
#define _WIN32_WINNT 0x0600
#include <iostream>
#include <windows.h>
#include <winbase.h>
#include <Shlwapi.h>
//#include <strsafe.h>
//#include <threadpoolapiset.h>
using namespace std;
 
 
int main(int argc,char *argv[])
{
 
    SYSTEM_INFO info;
    BOOL IsWow64;
    IsWow64Process(GetCurrentProcess(),&IsWow64);
    if(IsWow64)
    {
        GetNativeSystemInfo(&info);
    }
    else
    {
        GetSystemInfo(&info);
    }
 
 
    cout<<"PageSize: "<<info.dwPageSize<<" MiniAddr: "<<info.lpMinimumApplicationAddress<<endl;
    cout<<"MaxAddr: "<<info.lpMaximumApplicationAddress<<" AllocationGranularity: "<<info.dwAllocationGranularity<<endl;
    cout<<"Processors: "<<info.dwNumberOfProcessors<<" ActiveProcessor: "<<info.dwActiveProcessorMask<<endl;
    if(info.wProcessorArchitecture == 9)
    {
        cout<<"ProcessArchitecture: x64 (AMD or Intel)"<<endl;
    }
    else
    {
        cout<<"ProcessorArchitecture: X86"<<endl;
    }
    return 0;
}
```




![img](https://img-blog.csdn.net/20170120000225412?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



如果需要获取CPU的信息 需要使用GetLogicalProcessorInformation, 可以获取CPU的物理数量，核心数量，逻辑数量，以及一二三级缓存的数量。





```c
BOOL WINAPI GetLogicalProcessorInformation(
  __out    PSYSTEM_LOGICAL_PROCESSOR_INFORMATION Buffer,
  __inout  PDWORD ReturnLength
);
```



MSDN给出了示例代码





```cpp
#include <windows.h>
#include <malloc.h>
#include <stdio.h>
#include <tchar.h>
#define    _tprintf  wprintf
typedef BOOL (WINAPI *LPFN_GLPI)(
    PSYSTEM_LOGICAL_PROCESSOR_INFORMATION,
    PDWORD);
 
 
// Helper function to count set bits in the processor mask.
DWORD CountSetBits(ULONG_PTR bitMask)
{
    DWORD LSHIFT = sizeof(ULONG_PTR)*8 - 1;
    DWORD bitSetCount = 0;
    ULONG_PTR bitTest = (ULONG_PTR)1 << LSHIFT;
    DWORD i;
 
    for (i = 0; i <= LSHIFT; ++i)
    {
        bitSetCount += ((bitMask & bitTest)?1:0);
        bitTest/=2;
    }
 
    return bitSetCount;
}
 
int _cdecl _tmain ()
{
    LPFN_GLPI glpi;
    BOOL done = FALSE;
    PSYSTEM_LOGICAL_PROCESSOR_INFORMATION buffer = NULL;
    PSYSTEM_LOGICAL_PROCESSOR_INFORMATION ptr = NULL;
    DWORD returnLength = 0;
    DWORD logicalProcessorCount = 0;
    DWORD numaNodeCount = 0;
    DWORD processorCoreCount = 0;
    DWORD processorL1CacheCount = 0;
    DWORD processorL2CacheCount = 0;
    DWORD processorL3CacheCount = 0;
    DWORD processorPackageCount = 0;
    DWORD byteOffset = 0;
    PCACHE_DESCRIPTOR Cache;
 
    glpi = (LPFN_GLPI) GetProcAddress(
                            GetModuleHandle(TEXT("kernel32")),
                            "GetLogicalProcessorInformation");
    if (NULL == glpi)
    {
        _tprintf(TEXT("\nGetLogicalProcessorInformation is not supported.\n"));
        return (1);
    }
 
    while (!done)
    {
        DWORD rc = glpi(buffer, &returnLength);
 
        if (FALSE == rc)
        {
            if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
            {
                if (buffer)
                    free(buffer);
 
                buffer = (PSYSTEM_LOGICAL_PROCESSOR_INFORMATION)malloc(
                        returnLength);
 
                if (NULL == buffer)
                {
                    _tprintf(TEXT("\nError: Allocation failure\n"));
                    return (2);
                }
            }
            else
            {
                _tprintf(TEXT("\nError %d\n"), GetLastError());
                return (3);
            }
        }
        else
        {
            done = TRUE;
        }
    }
 
    ptr = buffer;
 
    while (byteOffset + sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION) <= returnLength)
    {
        switch (ptr->Relationship)
        {
        case RelationNumaNode:
            // Non-NUMA systems report a single record of this type.
            numaNodeCount++;
            break;
 
        case RelationProcessorCore:
            processorCoreCount++;
 
            // A hyperthreaded core supplies more than one logical processor.
            logicalProcessorCount += CountSetBits(ptr->ProcessorMask);
            break;
 
        case RelationCache:
            // Cache data is in ptr->Cache, one CACHE_DESCRIPTOR structure for each cache.
            Cache = &ptr->Cache;
            if (Cache->Level == 1)
            {
                processorL1CacheCount++;
            }
            else if (Cache->Level == 2)
            {
                processorL2CacheCount++;
            }
            else if (Cache->Level == 3)
            {
                processorL3CacheCount++;
            }
            break;
 
        case RelationProcessorPackage:
            // Logical processors share a physical package.
            processorPackageCount++;
            break;
 
        default:
            _tprintf(TEXT("\nError: Unsupported LOGICAL_PROCESSOR_RELATIONSHIP value.\n"));
            break;
        }
        byteOffset += sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION);
        ptr++;
    }
 
    _tprintf(TEXT("\nGetLogicalProcessorInformation results:\n"));
    _tprintf(TEXT("Number of NUMA nodes: %d\n"),
             numaNodeCount);
    _tprintf(TEXT("Number of physical processor packages: %d\n"),
             processorPackageCount);
    _tprintf(TEXT("Number of processor cores: %d\n"),
             processorCoreCount);
    _tprintf(TEXT("Number of logical processors: %d\n"),
             logicalProcessorCount);
    _tprintf(TEXT("Number of processor L1/L2/L3 caches: %d/%d/%d\n"),
             processorL1CacheCount,
             processorL2CacheCount,
             processorL3CacheCount);
 
    free(buffer);
 
    return 0;
}
```







我的CPU是 I7-4720HQ   运行结果如下



![img](https://img-blog.csdn.net/20170120003437594?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2FuZ3BlbmdrNzc4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



如果需要获取当前系统中关系内存使用情况的信息可以使用GlobalMemoryStatusEx



```cpp
BOOL WINAPI GlobalMemoryStatusEx(
  __inout  LPMEMORYSTATUSEX lpBuffer
);
```



```cpp
typedef struct _MEMORYSTATUSEX {
  DWORD     dwLength;
  DWORD     dwMemoryLoad; //已使用内存数量
  DWORDLONG ullTotalPhys;  //系统物理内存总量
  DWORDLONG ullAvailPhys;  //空闲的物理内存
  DWORDLONG ullTotalPageFile;//页交换文件大小
  DWORDLONG ullAvailPageFile;//空闲的页交换空间
  DWORDLONG ullTotalVirtual;  //进程可使用虚拟机地址空间大小
  DWORDLONG ullAvailVirtual;  //空闲的虚拟地址空间大小
  DWORDLONG ullAvailExtendedVirtual;
} MEMORYSTATUSEX, *LPMEMORYSTATUSEX
```



如果需要获取当前进程的内存使用情况 可以使用GetProcessMemoryInfo



```cpp
BOOL WINAPI GetProcessMemoryInfo(
  __in   HANDLE Process,
  __out  PPROCESS_MEMORY_COUNTERS ppsmemCounters,
  __in   DWORD cb
);

typedef struct _PROCESS_MEMORY_COUNTERS_EX {
  DWORD  cb;
  DWORD  PageFaultCount; //发生的页面错误
  SIZE_T PeakWorkingSetSize;  //使用过的最大工作集
  SIZE_T WorkingSetSize;      //目前的工作集
  SIZE_T QuotaPeakPagedPoolUsage;//使用过的最大分页池大小
  SIZE_T QuotaPagedPoolUsage;  //分页池大小
  SIZE_T QuotaPeakNonPagedPoolUsage;//非分页池使用过的
  SIZE_T QuotaNonPagedPoolUsage;  //非分页池大小
  SIZE_T PagefileUsage; //页交换文件使用大小
  SIZE_T PeakPagefileUsage; //历史页交换文件使用
  SIZE_T PrivateUsage;  //进程运行过程中申请的内存大小
} PROCESS_MEMORY_COUNTERS_EX, *PPROCESS_MEMORY_COUNTERS_EX
```





测试：



```cpp
#include <windows.h>
#include <stdio.h>
#include <Psapi.h>
 
#define DIV (1024*1024)
 
 
#define WIDTH 7
 
int main(int argc, char *argv[])
{
  MEMORYSTATUSEX statex;
  PROCESS_MEMORY_COUNTERS  counter;
  counter.cb = sizeof(counter);
 
  statex.dwLength = sizeof (statex);
 
  GlobalMemoryStatusEx (&statex);
  GetProcessMemoryInfo(GetCurrentProcess(),&counter,counter.cb);
  printf ("GlobalMemoryStatusEx : \r\n\r\n");
  printf ("There is  %*ld percent of memory in use.\n",
          WIDTH, statex.dwMemoryLoad);
  printf ("There are %*I64d total Mbytes of physical memory.\n",
          WIDTH, statex.ullTotalPhys/DIV);
  printf ("There are %*I64d free Mbytes of physical memory.\n",
          WIDTH, statex.ullAvailPhys/DIV);
  printf ("There are %*I64d total Mbytes of paging file.\n",
          WIDTH, statex.ullTotalPageFile/DIV);
  printf ("There are %*I64d free Mbytes of paging file.\n",
          WIDTH, statex.ullAvailPageFile/DIV);
  printf ("There are %*I64d total Mbytes of virtual memory.\n",
          WIDTH, statex.ullTotalVirtual/DIV);
  printf ("There are %*I64d free Mbytes of virtual memory.\n",
          WIDTH, statex.ullAvailVirtual/DIV);
 
  printf ("There are %*I64d free Mbytes of extended memory.\r\n\r\n",
          WIDTH, statex.ullAvailExtendedVirtual);
 
 
 
  /************************************************************************/
#define DIV 1024
  printf ("GetProcessMemoryInfo : \r\n\r\n");
 
  printf ("There are   %*I64d KBbyte of page fault \n",
          WIDTH, counter.PageFaultCount/DIV);
  printf ("There are %*I64d Kbytes of historic working memory\n",
          WIDTH, counter.PeakWorkingSetSize/DIV);
  printf ("There are %*I64d Kbytes of working memory.\n",
          WIDTH, counter.WorkingSetSize/DIV);
  printf ("There are %*I64d Kbytes of historic page pool.\n",
          WIDTH, counter.QuotaPeakPagedPoolUsage/DIV);
  printf ("There are %*I64d Kbytes of page pool.\n",
          WIDTH, counter.QuotaPagedPoolUsage/DIV);
  printf ("There are %*I64d Kbytes of historic no-pageed pool.\n",
          WIDTH, counter.QuotaPeakNonPagedPoolUsage/DIV);
  printf ("There are %*I64d Kbytes of no-pageed pool.\n",
          WIDTH, counter.QuotaNonPagedPoolUsage/DIV);
 
  printf ("There are %*I64d Kbytes of historic page file in using.\n",
          WIDTH, counter.PeakPagefileUsage);
  printf ("There are %*I64d Kbytes of page file in using.\n",
          WIDTH, counter.PagefileUsage);
 
  return 0;
}
```



![img](https://img-blog.csdn.net/20170120135623821?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2FuZ3BlbmdrNzc4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)







如果要查询进程虚拟地址空间的某个地址所属的块信息 可以使用VirtualQueryEx



```cpp
SIZE_T WINAPI VirtualQueryEx(
  __in      HANDLE hProcess,
  __in_opt  LPCVOID lpAddress,
  __out     PMEMORY_BASIC_INFORMATION lpBuffer,
  __in      SIZE_T dwLength
);
```





```cpp
typedef struct _MEMORY_BASIC_INFORMATION {
  PVOID  BaseAddress;  //区域基地址
  PVOID  AllocationBase;//使用VirtualAlloc分配的基地址
  DWORD  AllocationProtect; //保护属性
  SIZE_T RegionSize;    //区域大小
  DWORD  State;     //页属性
  DWORD  Protect;  //区域属性
  DWORD  Type;  //区域类型
} MEMORY_BASIC_INFORMATION, *PMEMORY_BASIC_INFORMATION;
```

详细参见 随书代码中的VMMap







# （十五）应用程序中使用虚拟内存

2017年02月05日 17:09:08 [wangpengk7788](https://me.csdn.net/wangpengk7788) 阅读数：197



 版权声明：本文为博主原创文章，未经博主允许不得转载。	https://blog.csdn.net/wangpengk7788/article/details/54881374



windows 提供了三种机制来对内存进行操作



1）虚拟内存，最适合用来管理大型对象数组 或大型结构数组

2）内存映射文件，最适合用来管理大型数据流（通常是文件），以及在同一机器上运行的多进程之间共享数据。

3）堆，适合用来管理大量的小型对象。





windows提供了一些用来操控虚拟内存的函数，我们可以通过这些函数直接预定地址空间区域，给区域调拨（来自页交换文件的 ）物理存储器。以及根据自己的需要来设置页面的保护属性。





```cpp
LPVOID WINAPI VirtualAlloc(
  __in_opt  LPVOID lpAddress,
  __in      SIZE_T dwSize,
  __in      DWORD flAllocationType,
  __in      DWORD flProtect
);
```





\1. 预订地址空间区域

virtualalloc  地址传入null，等于告诉系统自动找一块闲置区域但是不保证从上到下分配或从下到上分配

如果打算预订一块区域并且用很长时间，那么我们希望系统从尽可能高的内存分配地址，避免引起内存碎片

需要给flAllocationType 指定MEM_RESERVE | MEM_TOP_DOWN



在预定地址空间时最小的分配粒度是64KB，同时大小为系统定义的页面小大的倍数。

可以测试来验证





```cpp
int main(int argc, char **agrv)
{
    LPVOID pAddr = VirtualAlloc(NULL,1024,MEM_RESERVE,PAGE_READWRITE);
    VirtualAlloc(pAddr,1024,MEM_COMMIT,PAGE_READWRITE);
 
    int *pValue = reinterpret_cast<int*> (pAddr+2048);
    *pValue = 100;
 
    if( !(reinterpret_cast<DWORD>(pAddr)%(1024*64)) )
    {
        cout<<"Allocation granularity is 64 KB"<<endl;
    }
    return 0;
}
```





\2. 给区域调拨物理存储器

系统会从页交换文件或物理内存中调拨存储器给区域

需要给flAllocationType 指定MEM_COMMIT





\3. 取或则同时预定和调拨

   给flAllocationType 指定MEM_COMMIT | MEM_RESERVE



\4. 何时调拨物理存储器

虚拟内存，既能方便享受数组方法所带来的快速，又能节省存储器，就像使用链表一样

（1）预定足够大的空间，只预定不调拨不会消耗物理存储器

（2） 在对数组元素访问时使用SEH异常，在异常处理时调拨物理内存

（3）设置..结构成员



\5. 撤销调拨物理存储器及释放





```cpp
BOOL WINAPI VirtualFree(
  __in  LPVOID lpAddress,
  __in  SIZE_T dwSize,
  __in  DWORD dwFreeType
);
```





当释放整个区域时 dwFreeType 设置为MEM_RELEASE,lpAddress设置为区域的起始地址，dwSize设置为0,



当释放区域中已调拨的某部分页面时，dwFreeType设置为 MEM_DECOMMIT,撤销已调拨物理内存也是基于页面粒度的



6.改变保护属性



```cpp
BOOL WINAPI VirtualProtect(
  __in   LPVOID lpAddress,
  __in   SIZE_T dwSize,
  __in   DWORD flNewProtect,
  __out  PDWORD lpflOldProtect
);
```

同样设置保护属性的时候也使用系统定义的页面粒度





7.重置物理存储器的内容



当我们对一些已经调拨物理存储的页面的内容不在需要的时候，可以通过给VirtualAlloc的flAllocationType指定为MEM_RESET,来将页面设置为重置，这样当系统需要空闲页面时候可以将这些页面调拨给需要的进程，而不将内容倒换进磁盘分页文件中，当重新写入或者读取这部分地址的时候，这些地址被映射成新的页面。



通过设置页面重置，可以有效提高应用程序的性能。



\8. 地址窗口扩展



虽然Windows的32位版本可以支持多达128GB物理内存，但每个32位用户进程在默认情况下只有2GB虚拟地址空间（在Boot.ini中使用/3GB和/USERVA开关时，此限制值可配置为3GB）。

为了允许一个32位进程分配和访问更多的物理内存，突破这一受限地址空间所能表达的内存范围，Windows提供了一组函数，称为地址窗口扩展（AWE , Address  Windowing  Extensions）。例如，在一个具有8GB物理内存Windows  2000  Advanced  Server系统上，一个数据库服务器应用程序有可能使用AWE来分配和使用6GB内存作为数据库缓存。




AWE函数分配和使用内存：


1.分配所要使用的物理内存。
2.创建一个虚拟地址空间区域，把它当作一个窗口来映射物理内存的多个
3.将物理内存的视图映射到窗口中。


一个应用程序想要分配物理内存，可以调用Windows函数
AllocateUserPages(这个函数要求“lock pages in memory[将页面锁在内存中]”用户权限)。然后，应用程序使用VirtualAlloc函数以及MEM_PHYSICAL标志，在进程地址空间的私有部分创建一个窗口，该窗口被映射至原先分配
的物理内存中的全部或一部分。之后，AWE分配的内存几乎可以与所有的Windows API一起使用。

如果一个应用程序在它的地址空间中创建一个256MB的窗口，并且分配4GB的物理内存（当然系统物理内存要超过4GB），此应用程序就可以使用MapUserPhysicalPages或MapUserPhysicalPagesScatter函数，将4GB物理内存映射到256MB窗口中，从而访问此4GB物理内存中的任何部分。应用程序的虚拟地址空间大小决定了该应用程序通过一次映射可以访问的物理内存的数量。




Windows的所有版本都有AWE功能，而且不管一个系统有多少物理内存，AWE功能都可以使用。当然，在超过2GB物理内存的系统上，AWE才是最有用的。




另一种用途是出于安全性考虑：AWE内存永远不会被换出，AWE内存中的数据不会在页面文件中有拷贝，从而避免坏蛋将系统重新引导到另一个操作系统中，通过检查页面文件来获取数据。


对于通过AWE函数分配和映射的内存，有以下限制：

1.进程之间不能共享页面；
2.同样的物理页面不能被映射至同一进程的多个虚拟地址；
3.老版本的Windows中，对页面的保护仅限于读/写；在Windows Server 




2003 Service Pack1及以后的版本中，也支持不可访问和只读模式。



下面是MSDN提高的示例代码：







```cpp
#define _WIN32_WINNT 0x0600
#include <windows.h>
#include <stdio.h>
 
 
 
#define MEMORY_REQUESTED 1024*1024 // request a megabyte
 
BOOL
LoggedSetLockPagesPrivilege ( HANDLE hProcess,
                              BOOL bEnable);
 
int _cdecl main()
{
  BOOL bResult;                   // generic Boolean value
  ULONG_PTR NumberOfPages;        // number of pages to request
  ULONG_PTR NumberOfPagesInitial; // initial number of pages requested
  ULONG_PTR *aPFNs;               // page info; holds opaque data
  PVOID lpMemReserved;            // AWE window
  SYSTEM_INFO sSysInfo;           // useful system information
  int PFNArraySize;               // memory to request for PFN array
 
  GetSystemInfo(&sSysInfo);  // fill the system information structure
 
  printf("This computer has page size %d.\n", sSysInfo.dwPageSize);
 
  // Calculate the number of pages of memory to request.
 
  NumberOfPages = MEMORY_REQUESTED/sSysInfo.dwPageSize;
  printf ("Requesting %d pages of memory.\n", NumberOfPages);
 
  // Calculate the size of the user PFN array.
 
  PFNArraySize = NumberOfPages * sizeof (ULONG_PTR);
 
  printf ("Requesting a PFN array of %d bytes.\n", PFNArraySize);
 
  aPFNs = (ULONG_PTR *) HeapAlloc(GetProcessHeap(), 0, PFNArraySize);
 
  if (aPFNs == NULL)
  {
    printf ("Failed to allocate on heap.\n");
    return 0;
  }
 
  // Enable the privilege.
 
  if( ! LoggedSetLockPagesPrivilege( GetCurrentProcess(), TRUE ) )
  {
    return 0;
  }
 
  // Allocate the physical memory.
 
  NumberOfPagesInitial = NumberOfPages;
  bResult = AllocateUserPhysicalPages( GetCurrentProcess(),
                                       &NumberOfPages,
                                       aPFNs );
 
  if( bResult != TRUE )
  {
    printf("Cannot allocate physical pages (%u)\n", GetLastError() );
    return 0;
  }
 
  if( NumberOfPagesInitial != NumberOfPages )
  {
    printf("Allocated only %p pages.\n", NumberOfPages );
    return 0;
  }
 
  // Reserve the virtual memory.
 
  lpMemReserved = VirtualAlloc( NULL,
                                MEMORY_REQUESTED,
                                MEM_RESERVE | MEM_PHYSICAL,
                                PAGE_READWRITE );
 
  if( lpMemReserved == NULL )
  {
    printf("Cannot reserve memory.\n");
    return 0;
  }
 
  // Map the physical memory into the window.
 
  bResult = MapUserPhysicalPages( lpMemReserved,
                                  NumberOfPages,
                                  aPFNs );
 
  if( bResult != TRUE )
  {
    printf("MapUserPhysicalPages failed (%u)\n", GetLastError() );
    return 0;
  }
 
  // unmap
 
  bResult = MapUserPhysicalPages( lpMemReserved,
                                  NumberOfPages,
                                  NULL );
 
  if( bResult != TRUE )
  {
    printf("MapUserPhysicalPages failed (%u)\n", GetLastError() );
    return 0;
  }
 
  // Free the physical pages.
 
  bResult = FreeUserPhysicalPages( GetCurrentProcess(),
                                   &NumberOfPages,
                                   aPFNs );
 
  if( bResult != TRUE )
  {
    printf("Cannot free physical pages, error %u.\n", GetLastError());
    return 0;
  }
 
  // Free virtual memory.
 
  bResult = VirtualFree( lpMemReserved,
                         0,
                         MEM_RELEASE );
 
  // Release the aPFNs array.
 
  bResult = HeapFree(GetProcessHeap(), 0, aPFNs);
 
  if( bResult != TRUE )
  {
      printf("Call to HeapFree has failed (%u)\n", GetLastError() );
  }
 
}
 
/*****************************************************************
   LoggedSetLockPagesPrivilege: a function to obtain or
   release the privilege of locking physical pages.
   Inputs:
       HANDLE hProcess: Handle for the process for which the
       privilege is needed
       BOOL bEnable: Enable (TRUE) or disable?
   Return value: TRUE indicates success, FALSE failure.
*****************************************************************/
BOOL
LoggedSetLockPagesPrivilege ( HANDLE hProcess,
                              BOOL bEnable)
{
  struct {
    DWORD Count;
    LUID_AND_ATTRIBUTES Privilege [1];
  } Info;
 
  HANDLE Token;
  BOOL Result;
 
  // Open the token.
 
  Result = OpenProcessToken ( hProcess,
                              TOKEN_ADJUST_PRIVILEGES,
                              & Token);
 
  if( Result != TRUE )
  {
    printf( "Cannot open process token.\n" );
    return FALSE;
  }
 
  // Enable or disable?
 
  Info.Count = 1;
  if( bEnable )
  {
    Info.Privilege[0].Attributes = SE_PRIVILEGE_ENABLED;
  }
  else
  {
    Info.Privilege[0].Attributes = 0;
  }
 
  // Get the LUID.
 
  Result = LookupPrivilegeValue ( NULL,
                                  SE_LOCK_MEMORY_NAME,
                                  &(Info.Privilege[0].Luid));
 
  if( Result != TRUE )
  {
    printf( "Cannot get privilege for %s.\n", SE_LOCK_MEMORY_NAME );
    return FALSE;
  }
 
  // Adjust the privilege.
 
  Result = AdjustTokenPrivileges ( Token, FALSE,
                                   (PTOKEN_PRIVILEGES) &Info,
                                   0, NULL, NULL);
 
  // Check the result.
 
  if( Result != TRUE )
  {
    printf ("Cannot adjust token privileges (%u)\n", GetLastError() );
    return FALSE;
  }
  else
  {
    if( GetLastError() != ERROR_SUCCESS )
    {
      printf ("Cannot enable the SE_LOCK_MEMORY_NAME privilege; ");
      printf ("please check the local policy.\n");
      return FALSE;
    }
  }
 
  CloseHandle( Token );
 
  return TRUE;
}
```







# （十六）内存映射文件







**原理**



首先，“映射”这个词，就和数学课上说的“一一映射”是一个意思，就是建立一种一一对应关系，在这里主要是只 硬盘上文件 的位置与进程 逻辑地址空间中 一块大小相同的区域之间的一一对应，如图1中过程1所示。这种对应关系纯属是逻辑上的概念，物理上是不存在的，原因是进程的逻辑地址空间本身就是不存在 的。在内存映射的过程中，并没有实际的数据拷贝，文件没有被载入内存，只是逻辑上被放入了内存，具体到代码，就是建立并初始化了相关的[数据结构](http://lib.csdn.net/base/datastructure) （struct address_space），这个过程有系统调用mmap()实现，所以建立内存映射的效率很高。

 ![img](http://hi.csdn.net/attachment/201009/17/0_1284702135HcPp.gif)

图1.内存映射原理  



 

 

 

既然建立内存映射没有进行实际的数据拷贝，那么进程又怎么能最终直接通过内存操作访问到硬盘上的文件呢？那就要看内存映射之后的几个相关的过程了。

 

mmap()会 返回一个指针ptr，它指向进程逻辑地址空间中的一个地址，这样以后，进程无需再调用read或write对文件进行读写，而只需要通过ptr就能够操作 文件。但是ptr所指向的是一个逻辑地址，要操作其中的数据，必须通过MMU将逻辑地址转换成物理地址，如图1中过程2所示。这个过程与内存映射无关。

 

前 面讲过，建立内存映射并没有实际拷贝数据，这时，MMU在地址映射表中是无法找到与ptr相对应的物理地址的，也就是MMU失败，将产生一个缺页中断，缺 页中断的中断响应函数会在swap中寻找相对应的页面，如果找不到（也就是该文件从来没有被读入内存的情况），则会通过mmap()建立的映射关系，从硬 盘上将文件读取到物理内存中，如图1中过程3所示。这个过程与内存映射无关。

 

如果在拷贝数据时，发现物理内存不够用，则会通过虚拟内存机制（swap）将暂时不用的物理页面交换到硬盘上，如图1中过程4所示。这个过程也与内存映射无关。

 

 

**效率** 

 



从 代码层面上看，从硬盘上将文件读入内存，都要经过文件系统进行数据拷贝，并且数据拷贝操作是由文件系统和硬件驱动实现的，理论上来说，拷贝数据的效率是一 样的。但是通过内存映射的方法访问硬盘上的文件，效率要比read和write系统调用高，这是为什么呢？原因是read()是系统调用，其中进行了数据 拷贝，它首先将文件内容从硬盘拷贝到内核空间的一个缓冲区，如图2中过程1，然后再将这些数据拷贝到用户空间，如图2中过程2，在这个过程中，实际上完成 了两次数据拷贝 ；而mmap()也是系统调用，如前所述，mmap()中没有进行数据拷贝，真正的数据拷贝是在缺页中断处理时进行的，由于mmap()将文件直接映射到用户空间，所以中断处理函数根据这个映射关系，直接将文件从硬盘拷贝到用户空间，只进行了 一次数据拷贝 。因此，内存映射的效率要比read/write效率高。

 

 

![img](http://hi.csdn.net/attachment/201009/17/0_1284702221pRyi.gif)

图2.read系统调用原理







### 内存映射文件可以用于3个不同的目的



• 系统使用内存映射文件，以便加载和执行. exe和DLL文件。这可以大大节省页文件空间和应用程序启动运行所需的时间。

• 可以使用内存映射文件来访问磁盘上的数据文件。这使你可以不必对文件执行I/O操作，并且可以不必对文件内容进行缓存。

• 可以使用内存映射文件，使同一台计算机上运行的多个进程能够相互之间共享数据。Windows确实提供了其他一些方法，以便在进程之间进行数据通信，但是这些方法都是使用内存映射文件来实现的，这使得内存映射文件成为单个计算机上的多个进程互相进行通信的最有效的方法。

### 使用内存映射数据文件 

若要使用内存映射文件，必须执行下列操作步骤：

1) 创建或打开一个文件内核对象，该对象用于标识磁盘上你想用作内存映射文件的文件。

2) 创建一个文件映射内核对象，告诉系统该文件的大小和你打算如何访问该文件。

3) 让系统将文件映射对象的全部或一部分映射到你的进程地址空间中。

当完成对内存映射文件的使用时，必须执行下面这些步骤将它清除：

1) 告诉系统从你的进程的地址空间中撤消文件映射内核对象的映像。

2) 关闭文件映射内核对象。

3) 关闭文件内核对象。

 

下面将详细介绍这些操作步骤。

**步骤1：创建或打开文件内核对象**





```cpp
PVOID MapViewOfFile(
   HANDLE hFileMappingObject,
   DWORD dwDesiredAccess,
   DWORD dwFileOffsetHigh,
   DWORD dwFileOffsetLow,
   SIZE_T dwNumberOfBytesToMap);
```



dwDesiredAccess的值

| 值                           | 含义                                                      |
| ---------------------------- | --------------------------------------------------------- |
| 0                            | 不能读取或写入文件的内容。当只想获得文件的属性时，请设定0 |
| GENERIC_READ                 | 可以从文件中读取数据                                      |
| GENERIC_WRITE                | 可以将数据写入文件                                        |
| GENERIC_READ \|GENERIC_WRITE | 可以从文件中读取数据，也可以将数据写入文件                |

dwShareMode 的值

| 值                                 | 含义                                        |
| ---------------------------------- | ------------------------------------------- |
| 0                                  | 打开文件的任何尝试均将失败                  |
| FILE_SHARE_READ                    | 使用GENERIC_WRITE打开文件的其他尝试将会失败 |
| FILE_SHARE_WRITE                   | 使用GENERIC_READ打开文件的其他尝试将会失败  |
| FILE_SHARE_READ FILE_SHARE_WRITE\| | 打开文件的其他尝试将会取得成功              |

 

**步骤2：创建一个文件映射内核对象**

调用CreateFileMapping函数告诉系统，文件映射对象需要多少物理存储器。

```c
HANDLE CreateFileMapping(
   HANDLE hFile,
   PSECURITY_ATTRIBUTES psa,
   DWORD fdwProtect,
   DWORD dwMaximumSizeHigh,
   DWORD dwMaximumSizeLow,
   PCTSTR pszName);
```

 

第一个参数：hFile用于标识你想要映射到进程地址空间中的文件句柄。该句柄由前面调用的CreateFile函数返回。

第二个参数：psa参数是指向文件映射内核对象的SECURITY_ATTRIBUTES结构的指针，通常传递的值是NULL（它提供默认的安全特性，返回的句柄是不能继承的）。

第三个参数：fdwProtect参数使你能够设定这些保护属性。大多数情况下，可以设定下表列出的3个保护属性之一。

使用fdwProtect 参数设定的部分保护属性

| 保护属性       | 含义                                                         |
| -------------- | ------------------------------------------------------------ |
| PAGE_READONLY  | 当文件映射对象被映射时，可以读取文件的数据。必须已经将GENERIC_READ传递给CreateFile函数 |
| PAGE_READWRITE | 当文件映射对象被映射时，可以读取和写入文件的数据。必须已经将GENERIC_READ \| GENERIC_WRITE传递给Creat eFile |
| PAGE_WRITECOPY | 当文件映射对象被映射时，可以读取和写入文件的数据。如果写入数据，会导致页面的私有拷贝得以创建。必须已经将GENERIC_READ或GENERIC_WRITE传递给CreateFile |

 

除了上面的页面保护属性外，还有4个节保护属性

 

节的第一个保护属性是SEC_NOCACHE，它告诉系统，没有将文件的任何内存映射页面放入高速缓存。因此，当将数据写入该文件时，系统将更加经常地更新磁盘上的文件数据。供设备驱动程序开发人员使用的，应用程序通常不使用。

节的第二个保护属性是SEC_IMAGE，它告诉系统，你映射的文件是个可移植的可执行（PE）文件映像。当系统将该文件映射到你的进程的地址空间中时，系统要查看文件的内容，以确定将哪些保护属性赋予文件映像的各个页面。例如， PE文件的代码节（ . text）通常用PAGE_ EXECUTE_READ属性进行映射， 而PE 文件的数据节( .data) 则通常用PAGE_READW RITE属性进行映射。如果设定的属性是S E C _ I M A G E，则告诉系统进行文件映像的映射，并设置相应的页面保护属性。

最后两个保护属性是SEC_RESERVE和SEC_COMMIT，它们是两个互斥属性。只有当创建由系统的页文件支持的文件映射对象时，这两个标志才有意义。SEC_COMMIT标志能使CreateFileMapping从系统的页文件中提交存储器。如果两个标志都不设定，其结果也一样。

第四和五个参数：dwMaximumSizeHigh和dwMaximumSizeLow这两个参数将告诉系统该文件的最大字节数

最后一个参数是pszName： 它是个以0结尾的字符串，用于给该文件映射对象赋予一个名字。该名字用于与其他进程共享文件映射对象。

 

**步骤3：将文件数据映射到进程的地址空间**

将文件的数据作为映射到该区域的物理存储器进行提交。

```c
PVOID MapViewOfFile(
   HANDLE hFileMappingObject,
   DWORD dwDesiredAccess,
   DWORD dwFileOffsetHigh,
   DWORD dwFileOffsetLow,
   SIZE_T dwNumberOfBytesToMap);
```

 

第一个参数： **h**FileMappingObject用于标识文件映射对象的句柄，该句柄是前面调用CreateFileMapping或OpenFileMapping函数返回的。

 

第二个参数：dwDesiredAccess用于标识如何访问该数据。可以设定下表所列的4个值中的一个。

| 值                   | 含义                                                         |
| -------------------- | ------------------------------------------------------------ |
| FILE_MAP_WRITE       | 可以读取和写入文件数据。CreateFileMapping函数必须通过传递PAGE_READWRITE标志来调用 |
| FILE_MAP_READ        | 可以读取文件数据。CreateFileMapping函数可以通过传递下列任何一个保护属性来调用：PAGE_READONLY、PAGE_ READWRITE或PAGE_WRITECOPY |
| FILE_MAP_ALL_ACCES S | 与FILE_MAP_WRITE相同                                         |
| FILE_MAP_COPY        | 可以读取和写入文件数据。如果写入文件数据，可以创建一个页面的私有拷贝。在Windows 2000中，CreateileMapping函数可以用PAGE_READONLY、PAGE_READWRITE或PAGE_WRITECOPY等保护属性中的任何一个来调用。在Windows 98中，CreateFileMapping必须用PAGE_WRITECOPY来调用 |

 

（一个文件映射到你的进程的地址空间中时，你不必一次性地映射整个文件。相反，可以只将文件的一小部分映射到地址空间。被映射到进程的地址空间的这部分文件称为一个视图。）

 

第三四个参数：dwFileOfsetHigh和dwFileOfsetLow参数。指定哪个字节应该作为视图中的第一个字节来映射。

第五个参数：dwNumberOfBytesToMap有多少字节要映射到地址空间。如果设定的值是0，那么系统将设法把从文件中的指定位移开始到整个文件的结尾的视图映射到地址空间。

 

**步骤4：从进程的地址空间中撤消文件数据的映像**

当不再需要保留映射到进程地址空间区域中的文件数据时，可以通过调用下面的函数将它释放：

```c
BOOL UnmapViewOfFile(PVOID pvBaseAddress);
```

参数：pvBaseAddress由MapViewOfFile函数返回。

注意：如果没有调用这个函数，那么在进程终止运行前，保留的区域就不会被释放。每当调用MapViewOfFile时，系统总是在你的进程地址空间中保留一个新区域，而以前保留的所有区域将不被释放。

为了提高速度，系统将文件的数据页面进行高速缓存，并且在对文件的映射视图进行操作时不立即更新文件的磁盘映像。如果需要确保你的更新被写入磁盘，可以强制系统将修改过的数据的一部分或全部重新写入磁盘映像中，方法是调用FlushViewOfFile函数：

```c
BOOL FlushViewOfFile(
   PVOID pvAddress,
   SIZE_T dwNumberOfBytesToFlush);
```

第一个参数是包含在内存映射文件中的视图的一个字节的地址。该函数将你在这里传递的地址圆整为一个页面边界值。

第二个参数用于指明你想要刷新的字节数。系统将把这个数字向上圆整，使得字节总数是页面的整数。如果你调用FlushViewOfFile函数并且不修改任何数据，那么该函数只是返回，而不将任何信息写入磁盘。

 

**步骤5和步骤6：关闭文件映射对象和文件对象**

用CloseHandle函数关闭相应的对象。

在代码开始运行时关闭这些对象：

```c
HANDLE hFile = CreateFile(...);
HANDLE hFileMapping = CreateFileMapping(hFile, ...);
CloseHandle(hFile);
PVOID pvFile = MapViewOfFile(hFileMapping, ...);
CloseHandle(hFileMapping);
 
// Use the memory-mapped file.
 
UnmapViewOfFile(pvFile);
```

 







# （十七） 堆





**1.堆**

​                **虚拟内存**主要用于**分配连续的内存**，**如给大数组分配内存**。

​                **堆**主要用于**分配小块内存**，**如链表，树**等就适合用堆来分配内存。

​                优点在于：在堆中分配内存时不像虚拟内存要求那么多，如分配粒度、页边界等。它没有这些要求，**可分配任意大小的内存**。

​                缺点在于：相对于其它内存分配机制，它**分配和释放内存的速度较慢**，而且失去了对内存commit和decommit的控制。

​                **一个进程中可以有多个堆。除默认堆外**，其它堆都可以在进程生存期间**创建或销毁**

​                进程的**默认堆**：**用户不能销毁进程默认堆**，因为在进程开始执行前，默认堆已经建立，**进程结束后它会被自动销毁**。

​                **堆中**的内存**分配操作是串行的**，当有两个线程同时要在同一个堆中分配内存时，系统会**先让一个线程执行分配内存操作，待其毕，再让第二个线程做分配内存操作**。

 

**2.创建其它堆的原因**

​                **模块保护**：把**不同的类型数据放在不同的堆中**，可以防止对其中一种数据进行写操作发生内存溢出时覆盖其它类数据的内容。

​                              如：有一个链表和一棵树，若将它们的内存都放在同一个堆中，那么当对链表的内存操作有写溢出时可能会覆盖树中的内容

​                                    把链表和树分别放在不同的堆中就可以保证链表的写溢出操作不会影响树中的数据。

​                **有效的内存管理**：在堆中仅**分配相同大小的对象有利用提高内存使用率，避免产生内存碎片**。

​                              如：有10字节大小的堆，其中有两个2字节的对象，两个3字节的对象。此时内存已满。当释放两个不连续2字节的对象时

​                              此时有4字节的空间可用，但由于不连续，就产生了内存碎片。 当需要3字节的连续空间时就无法提供足够的内存。 

​                **本地存储**： 把可能被同时访问的数据**连续放在一起(同一页中)，减少换页操作和页的访问次数，有利于提高效率**

​                **避免线程同步开销**：当多线程同时对同一堆进行操作时，堆函数需要执行额外的代码来同步线程的访问，这些额外执行的代码会对程序的性能造成影响。

​                                         当我们**为新建堆时可以告诉系统，该堆只会被一个线程访问，这样堆函数就不会执行额外的代码，**从而提高了系统的性能。

​                                         此时由用户来控制堆的线程安全，可以用互拆量等对线程操作堆的操作进行同步 

 

​                **快速释放内存**：为某些数据结构指定专用堆，允许我们**对整个堆进行释放操作**，而无需一块块地释放堆中的内存。

​         

**3.如何创建额外的堆**   

​               若要分配大量内存，如1M或更多，建议用VirtualAlloc来分配内存而不是在堆中分配内存 

​               **创建堆函数** HANDLE**HeatCreate**(DWORD, fwdOptions

​                                               SIZE_T dwInitialSize,

​                                               SIZE_T dwMaximumSize) 

​               在创建堆时尽量**不要**用fwdOptions,设为**HEAP_NO_SERIALIZE**，**除非**以下三种情况：

​                                （1）进程中**只有一个线程**

​                                （2）多线程中确保**只有一个线程会对该堆进行操**作

​                                （3）多线程中用户通过互斥机制来同步不同线程对堆的操作

​               dwInitialSize是堆在**最开始是commit的内存大小**(页大小4KB的倍数)

​               dwMaximumSize是堆**可以增长的最大值**，若将其**设为0，那么堆可以增长直至用完所有内存**。

 

**4.堆中分配内存的步骤**              

​              (1)**遍历已分配内存和未分配内存的内存块的链表**

​              (2)**找到一块足够大的内存模块**

​              (3)在找到的内存模块上**分配内存，并将其标记为已分配**

​              (4)在**内存链表中加入新的条目**

**5.Common API:**

​              HeapCreate             HeapSize HeapReAlloc

​              HeapFree                HeapDestroy                           GetProcessHeaps 取得堆的句柄

​              HeapValidate           HeapCompact

​              HeapLock                HeapUnlock

​              HeapWalk

 

 

 

 

堆非常适合分配大量的小型数据。它是用来管理链表和树的最佳方式。但是它分配和释放内存块的速度比虚拟内存和内存映射文件要慢，而且也无法再对物理存储器的调拨和撤销调拨进行直接控制。

一个进程同时可以有多个堆，进程在整个生命周期内可以创建和销毁这些堆。但是，默认的堆是在进程开始运行之前由系统自动创建的，在进程终止后会自动销毁。我们无法销毁进程的默认堆。**每个堆都有一个用来标识自己的堆句柄**，所有分配和释放内存块的堆函数都会在参数中用到这个堆句柄。

 

我们可以调用如下函数来得到进程默认堆得句柄：

*HANDLE WINAPI GetProcessHeap(void);*

 

#### 创建额外的堆

```c
HANDLE WINAPI HeapCreate(
  __in          DWORD flOptions,
  __in          SIZE_T dwInitialSize,
  __in          SIZE_T dwMaximumSize
);
```

*flOptions*参数用来表示**对堆的操作该如何进行**。可以指定**0， HEAP_NO_SERIALIZE， HEAP_GENERATE_EXCEPTIONS， HEAP_CREATE_ENABLE_EXECUTE。**

**HEAP_NO_SERIALIZE**标志使得**多个线程可以同时访问一个堆**，这使得**堆中的数据可能会遭到破坏，因此应该避免使用**。

**HEAP_GENERATE_EXCEPTIONS**标志告诉系统，每当在堆中分配或者重新分配内存块失败的时候，抛出一个异常。

**HEAP_CREATE_ENABLE_EXECUTE**标志告诉系统，我们**想在堆中存放可执行代码**。如果不设置这个标志，那么当我们试图在来自堆的内存块中执行代码时，系统会抛出EXCEPTION_ACCESS_VIOLATION异常。

 

*dwInitialSize*参数表示一开始**要调拨给堆的字节数**。如果需要，HeapCreate会**把这个值向上取整到CPU页面大小的整数倍**。

 

*dwMaximumSize*参数表示**堆能增长到的最大大小**。如果*dwMaximumSize* 大于0，那么创建的堆会有一个最大大小。

如果*dwMaximumSize*为0，那么创建的堆**将是可增长的，直到用尽所有的物理存储器为止**。

 

 

#### 从堆中分配内存块

```c
LPVOID WINAPI HeapAlloc(
  __in          HANDLE hHeap,
  __in          DWORD dwFlags,
  __in          SIZE_T dwBytes
);
```

 

#### 调整内存块的大小

```c
LPVOID WINAPI HeapReAlloc(
  __in          HANDLE hHeap,
  __in          DWORD dwFlags,
  __in          LPVOID lpMem,
  __in          SIZE_T dwBytes
);
```

*dwFlags*参数可以是**HEAP_GENERATE_EXCEPTIONS，HEAP_NO_SERIALIZE，HEAP_ZERO_MEMORY， HEAP_REALLOC_IN_PLACE_ONLY。**

HEAP_ZERO_MEMORY**：**这个标志**只有在增大内存块的大小时才有用**。在这种情况下，内存中的**额外字节会被清零**。**减小内存块的大小时，这个标志不起任何作用**。

**HEAP_REALLOC_IN_PLACE_ONLY****：**在增大内存块的时候，HeapReAlloc可能会**在堆内部移动内存块**，而这个标志用来告诉HeapReAlloc**不要移动内存块。**

如果HeapReAlloc函数**能在不移动内存块的前提下就能让它增大，那么函数会返回原来的内存块地址**。另一方面，**如果HeapReAlloc必须移动内存块的地址，那么函数将返回一个指向一块更大内存块的新地址**。

如果一个**内存块是链表或者树的一部分，那么需要指定这个标志**。因为链表或者树的**其他节点可能包含指向当前节点的指针**，把**节点移动到堆中其他的地方会破坏链表或树的完整性。**

 

*lpMem*和*dwBytes*参数分别用来指定想要调整大小的内存块的原始地址以及内存块的新大小，以字节为单位。

 

#### 获得内存块的大小

分配一块内存后，可以调用如下函数来得到这块内存的实际大小：

```c
SIZE_T WINAPI HeapSize(
  __in          HANDLE hHeap,
  __in          DWORD dwFlags,
  __in          LPCVOID lpMem
);
```

参数*hHeap*用来**标识堆**。

参数*dwFlags*可以**是0或者HEAP_NO_SERIALIZE**。

参数*lpMem***标识内存块快的地址**。

 

#### 释放内存块

```c
BOOL WINAPI HeapFree(
  __in          HANDLE hHeap,
  __in          DWORD dwFlags,
  __in          LPVOID lpMem
);
```

 

#### 销毁堆

```c
BOOL WINAPI HeapDestroy(
  __in          HANDLE hHeap
);
```

调用这个函数的时候，系统会**释放堆中包含的所有内存块**，同时**系统会收回堆所占用的物理存储器和地址空间区域**。

 

#### 其他堆函数

1.ToolHelp函数允许我们**枚举进程中的堆以及堆中分配的内存块**。

Heap32First，Heap32Next， Heap32ListFirst， Heap32ListNext。

 

2**.得到进程中所有堆的句柄**

```c
DWORD WINAPI GetProcessHeaps(
  __in          DWORD NumberOfHeaps,
  __out         PHANDLE ProcessHeaps
);
```

用法如下：

```c
HANDLE hHeaps[25];
DWORD dwHeaps = GetProcessHeaps(25, hHeaps);
If(dwHeaps > 25)
{
         //大于25个句柄
}
else
{
         //hHeaps[0]
}
```

注意：函数所返回的句柄数组中也包括了进程的默认堆句柄。

 

3**.验证堆的完整性**

```c
BOOL WINAPI HeapValidate(
  __in          HANDLE hHeap,
  __in          DWORD dwFlags,
  __in          LPCVOID lpMem
);
```

通常调用这个函数的时候， 我们会传入一个堆句柄，和一个标志0，并传NULL给lpMem。

该函数**会遍历堆中的各个内存块，确保没有任何一块内存被破坏**。

 

4.为了**让堆中闲置的内存块能重新接合在一起**，并**撤销调拨给堆中闲置内存块的存储器**，可以调用如下函数：

```c
SIZE_T WINAPI HeapCompact(
  __in          HANDLE hHeap,
  __in          DWORD dwFlags
);
```

一般来说，我们会传0 给dwFlags参数。







\1.         堆适合分配小内存块，不需要按分配粒度或者页大小对齐。堆在最初只是预定了一块区域，在客户分配时将预定的区域提交，在客户释放后可能反提交。

\2.         关于默认堆：GetProcessHeap返回，用户模式代码无法销毁它，在进程结束后由系统销毁。进程可以通过链接选项“/HEAP:reserve[,commit]”来设置默认堆大小。因为默认堆属于进程，所以在DLL中不应设置该链接选项。Windows的ANSI版API向Unicode版转化的时候从默认堆分配字符串缓存，LocalAlloc、GlobalAlloc也从默认堆分配内存。默认堆对外界访问进行了同步，即没有使用HEAP_NO_SERIALIZE标记。

\3.         使用独立堆的一些好处：（1）写堆内存出错后，不会影响其他堆的数据。（2）对特定类型数据使用独立堆的话，由于分配块大小相同，具有速度快、无碎片的优点。（3）相关数据使用独立的堆，在访问这些数据时访问的页面更集中，减少PageFault。（4）对特定线程上的逻辑结构使用独立堆，不必加锁，提高性能。

\4.         HeapCreate：参数fdwOption，如果在创建堆的时候指定了部分标志（如HEAP_NO_SERIALIZE标志等），以后每次访问堆这些标志都生效；如果创建的时候没有指定，那后续的每次访问可以单独指定标志。 HEAP_NO_SERIALIZE-访问堆的时候不加锁。HEAP_GENERATE_EXCEPTIONS-分配内存失败的时候抛出异常，默认行为是返回NULL。HEAP_CREATE_ENABLE_EXECUTE-可以在堆内存上放置代码来执行。参数dwInitalSize-初始堆大小。参数dwMaximumSize-如果非0，表示如果堆内存使用量达到这个值后再分配会失败；为0，表示堆会自动增大，直到内存用尽。

\5.         HeapAlloc、HeapSize、HeapFree、HeapDestroy，容易理解。

\6.         HeapReAlloc：HEAP_ZERO_MEMORY-增大内存时，增加的字节初始化为0。HEAP_REALLOC_IN_PLACE_ONLY-要求不移动起始地址的情况下改变大小，需要增大时如果当前位置剩余空间不足会返回NULL。

\7.         **HeapSetInformation**：标记HeapEnableTerminationOnCorruption**-Visita****以上使用**。默认情况下，堆内存被破坏后只在调试器中触发一个断言然后继续执行，这个标记允许发现堆破坏就抛出异常。该标记影响进程中所有堆，无法清空标记。标记HeapCompatibilityInformation-值为2的时候，表示启用低碎片堆（lowfragmentation heap）[算法](http://lib.csdn.net/base/datastructure)，启用该算法的堆针对内存碎片问题优化有更好的性能。

\8.         Heap32ListFirst、Heap32ListNext-**遍历快照（CreateToolhelp32Snapshot）中的堆**。Heap32First、Heap32Next-遍历指定堆中的块。GetProcessHeaps-获得包括默认堆在内的所有堆句柄。HeapValidate-检查指定堆中所有块或者单个块的有效性。HeapCompact-将堆中闲置块合并，并反提交。HeapLock、HeapUnlock-锁定堆。HeapWalk-遍历指定堆中的块，建议先锁堆。





# （十七） DLL高级技术





20.1 DLL模块的显式载入和符号链接

**20.1.1 显式载入DLL模块**

（1）构建DLL时，如果至少导出一个函数/变量，那么链接器会同时生成一个.lib文件，但这个文件只是在隐式链接DLL时使用（显示链接时并没有用到这文件）

（2）显式载入DLL的函数：LoadLibrary(Ex)

| 参数                  | 含义                                                         |
| --------------------- | ------------------------------------------------------------ |
| pCTSTR pszDllPathName | LoadLibrary只有这个参数。函数会根据第19章介绍的搜索算法在用户的计算机中对DLL文件进行定位，并映射到进程的地址空间。 |
| HANDLE hFile          | 该参数为将来扩充所保留的，这里必须为NULL                     |
| DWORD dwFlags         | 可为0或下列标志的组合①DONT_RESOLVE_DLL_REFERENCES：只将该DLL映射到进程地址空间，但不调用DllMain函数及不检查该DLL导入段中的其他额外DLL，这也意味着不自动载入额外的DLL。（一般应避免使用该标志，因为代码所依赖的DLL可能尚未被载入！）②LOAD_LIBRARY_AS_DATAFILE：将DLL作为数据文件映射到进程。（一般用在一个DLL只包含资源而没有函数时或想用一个EXE文件中包含的资源时可用这个标志，**而且载入EXE时必须使用这个标志**）③LOAD_LIBRARY_AS_DATAFILE_EXCLUSIVE：与②标志相似，唯一不同的是以独占方式来打开这个DLL文件，以防止其他程序对其修改。④LOAD_LIBRARY_AS_IMAGE_RESOURCE：与②标志相似，但不同之处在于当系统载入DLL的时候，会对相对虚拟地址（RVA）进行修复。这样RVA就可以直接使用，而不必再根据DLL载入的内存地址来转换了。（当需要对DLL进行遍历其PE段时，这个标志特别有用）⑤LOAD_IGNORE_CODE_AUTHZ_LEVEL：用来关闭UAC对代码在执行过程中可以拥有的特权加以控制。⑥LOAD_WITH_ALTERED_SEARCH_PATH：用来改变LoadLibrary对DLL文件进行定位所使用的搜索算法。 A、如果pszDllPathName不包含“\”字符，会使用标准搜索路径算法 B、如果pszDllPathName包含“\”会因全路径（网络共享路径）或相对路径而有所不同。（见课本P557） C、可以调用SetDllDirectory改变搜索算法，搜索指定的目录路径。（具体顺序为:EXE所在目录→SetDlldirectory设置的文件夹→Windows系统目录→16位Windows系统目录→Windows目录→PATH列出的目录。 |
| 返回值                | HMODULE类型，等价于HINSTANCE，表示DLL被映射到的虚拟内存地址。当返回NULL，表示映射失败，可进一步调用GetLastError |

（3）混用LoadLibrary和LoadLibraryEx加载同一个DLL可能带来的问题

【情况1】：不会出现问题，此时hDll1=hDll2=hDll3

```c
HMODULE hDll1 = LoadLibrary(TEXT("MyLibrary.dll"));

HMODULE hDll2 = LoadLibraryEx(TEXT("MyLibrary.dll"), NULL,LOAD_LIBRARY_AS_IMAGE_RESOURCE);

HMODULE hDll3 = LoadLibraryEx(TEXT("MyLibrary.dll"), NULL,LOAD_LIBRARY_AS_DATAFILE);
```

【情况2】：将上面的调用顺序改变一下，则hDll1≠hDll2≠hDll3，说明DLL被多次映射到进程的地址空间。

```c
HMODULE hDll1 = LoadLibraryEx(TEXT("MyLibrary.dll"), NULL,LOAD_LIBRARY_AS_DATAFILE);

HMODULE hDll2 = LoadLibraryEx(TEXT("MyLibrary.dll"), NULL,LOAD_LIBRARY_AS_IMAGE_RESOURCE);

HMODULE hDll3 = LoadLibrary(TEXT("MyLibrary.dll"));
```

【分析原因】当LoadLibraryEx时（使用LOAD_LIBRARY_AS_DATAFILE, LOAD_LIBRARY_AS_DATAFILE_EXCLUSIVE, or LOAD_LIBRARY_AS_IMAGE_RESOURCE标志），系统会检测该DLL是否被LoadLibrary（或未使用上述标志LoadLibraryEx）进来，如果己经被载入过，那么函数会返回空间中DLL原先被映射的地址。如果DLL未被载入，那么DLL会将这个DLL载入，但会认为是个未完全载入的DLL，如果这时再载入时会被多次的映射到进程的地址空间，从而产生不同的地址。

**20.1.2 显式卸载DLL模块**

（1）BOOL FreeLibrary(HMODULE hInstDll);

（2）VOID FreeLibraryAndExitThread(hInstDll,dwExitCode);

　　①**函数的内部实现（在Kernel32.dll中）：**

```c
VOID FreeLibraryAndExitThread(HMODULE hInstDll,DWORD dwExitCode){
     FreeLibrary(hInstDll);
     ExitThread(dwExitCode);//调用该行指令在kernel32.dll
}
```

　　②为什么需要FreeLibraryAndExitThread函数？

　　A、假设在A这个DLL里创建一个线程，当该线程完成一些工作后，先调用FreeLibrary再调ExitThread来撤销对DLL的映射并终止线程，这里会出现一个严重的问题。因为FreeLibrary会立即从进程的地址空间撤销对DLL的映射。当FreeLibrary返回时，线程会试图调用ExitThread，而这行代码本来在DLL里的，这个DLL己经不存在了，这时线程会试图执行不存在的代码，将引发访问违规，并导致整个进程被终止。

　　B、但如果调用FreeLibraryAndExitThread，由于函数内部会调用FreeLibrary和ExitThread，而这两个函数是在Kernel32.dll的内部调用（而不是A这个DLL）。所以当撤销了A这个DLL后，这个线程可以继续执行ExitThread，只不过当ExitThread时线程不再返回A这个DLL里了，所以也不会出错。

**（3）DLL的使用计数问题**

　　①当LoadLibrary(Ex)时使用计数递增，第一个Load时使用计数为1.如果同一个进程的一个线程再调用LoadLibrary时，系统不会再次进行映射而是将使用计数递增。

　　②FreeLibrary或FreeLibraryAndExitThread使计数递减，但计数递减到0时，系统会这个DLL从进程的地址空间中撤销映射。

　　③**系统为每个进程的每个DLL维护一个使用计数**。如进程A和B都加载了MyLib.dll，那么这个DLL会被映射进两个进程的地址空间，但**该DLL在进程A和B中的使用计数都是1。**如果后来进程B的一个线程再次LoadLibrary这个DLL，则进程B中这个DLL的使用计数为2，但进程A中仍为1。

**（4）检测DLL的两个函数**

　　①检测DLL是否被映射：HMODULE GetModuleHandle(PCTSTR pszModuleName);

　　　　A、返回NULL时表示未被映射。

　　　　B、如果传NULL参数时，会返回应用程序的EXE文件的句柄。

　　②获得DLl/EXE的全路径名：GetModuleFileName(hInstModule,pszPathName,cchPath);其中的hInstModule为DLL或EXE的句柄。

**20.1.3 显示链接到导出符号——**获得函数地址：GetProcAddress

| 参数                | 描述                                                         |
| ------------------- | ------------------------------------------------------------ |
| HMODULE hInstDll    | DLL的句柄，即先前调用LoadLibrary(Ex)或GetModuleHandle时的返回值。 |
| PCSTR pszSymbolName | 函数的名称或序号，注意这里的类型是PCSTR，而不是PCTSTR，说明这个函数只接受ANSI的字符串。 |
| 返回值              | FARPROC，要获得的函数的地址，须转为该函数原型的指针。一般用typedef来声明要获取的函数原型的指针 |

**20.2 DLL的入口点函数**



```c
BOOL WINAPI DllMain(HINSTANCE hInstDll, DWORD fdwReason, PVOID fImpLoad) {

   switch (fdwReason) {
      case DLL_PROCESS_ATTACH:
         // DLL第1次被映射到进程的地址空间时
         break;

      case DLL_THREAD_ATTACH:
         // 创建一个线程时
         break;

      case DLL_THREAD_DETACH:
         // 线程终止时
         break;

      case DLL_PROCESS_DETACH:
         // 进程撤销一个DLL映射时
         break;
   }

   return(TRUE); // 该返回值只在 DLL_PROCESS_ATTACH通知时有用，用来表示DLL的初始化是否成功，如果return FALSE表示加载DLL失败，如果系统会终止整个进程或撤销对该DLL的映射。其他通知时，系统将忽略这个返回值。
}
```



**20.2.1 DLL_PROCESS_ATTACH通知**

**![img](http://images2015.cnblogs.com/blog/772759/201511/772759-20151127222244093-1303373366.png)** 

（1）**只有当DLL第1次被映射到进程地址空间时，才会发送该通知**。如果**以后**一个线程再调用LoadLibrary(Ex)来载入这个DLL，则**只会递增该DLL的使用计数**，但不会再发该通知。

（2）在该通知里，一般用来执行与进程相关的初始化，如创建DLL中一般函数要使用的堆。

（3）当DllMain处理DLL_PROCESS_ATTACH通知时，**返回值**用来表示DLL的初始化是否成功。如果return FALSE表示加载DLL失败，系统**会终止整个进程（这种情况发生在刚创建进程时）**或**撤销对该DLL的映射（这种情况发生在显式调用LoadLibrary(Ex)时）**。

（4）**这个通知由进程中的某个线程来调用的**。**如果是刚创建新的进程时，则由主线程调用**。**如果某个线程调用LoadLibrary第1次显式载入这个DLL时**，则**由这个线程来调用执行**这个通知，然后线程继续正常执行其他任务，如果return FALSE表示初始化失败，系统会撤销对DLL的映射并让LoadLibrary(Ex)返回NULL。

**20.2.2 DLL_PROCESS_DETACH通知**

![img](http://images2015.cnblogs.com/blog/772759/201511/772759-20151127222354077-843532230.png) 

（1）当系统将一个DLL从进程的地址空间中撤销映射时，发送该通知。（注意：**在处理DLL_PROCESS_ATTACH时如果的返回FALSE时**，那么就不会收到DLL_PROCESS_**DETACH**通知。）

（2）如果在处理DLL_PROCESS_ATTACH时返回FALSE，则DllMain就不会收到DLL_PROCESS_DETACH通知。

（3）如果是**因调用ExitProcess而导致撤销DLL映射**，则**调用ExitProcess函数的线程**将**负责执行DllMain函数的代码**。

（4）如果是**因线程调用了FreeLibrary**（或FreeLibraryAndExitThread）而撤销Dll映射，**该线程将执行DllMain函数中的代码**。该线程会直到处理完DLL_PROCESS_DETACH完才从FreeLibrary中返回。因此，如果该通知时死循环，则阻碍线程的终止，只有当每个DLL都处理完该通知后，操作系统才会真正地终止进程。

（5）如果**某个线程调用TerminateProcess来终止进程，则系统不会发送DLL_PORCESS_DETACH通知**。这意味着Dll没有机会执行一些清理代码的操作。因此，不到万不得己，应避免使用TerminateProcess函数。

**20.2.3 DLL_THREAD_ATTACH通知**

（1）当进程创建一个线程时，系统会向当前映射到该进程地址空间中的**所有DLL发送DLL_THREAD_ATTACH通知**。告诉这些DLL执行一些与线程相关的初始化。新创建的线程负责执行所有DLL中DllMain函数中相关的代码。只有当所有DLL完成了对该通知的处理后，新线程才会开始执行它的线程函数。

（2）当一个新的DLL映射到进程地址空间时，进程中己经有的线程不会是不会收到DLL_THREAD_ATTACH通知的。（**即只有在创建新线程时，己经被映射到进程地址空间中的DLL才会收到这个通知**）

（3）因创建进程时，何任被映射到进程地址空间中的DLL都会收到DLL_PROCESS_ATTACH通知，并由主线程负责执行，这里就可以执行一些相关的初始化工作，所以系统不会让主线程用DLL_THREAD_ATTACH来调用DllMain函数**（即主线程只接收DLL_PROCESS_ATTACH通知，而不接收DLL_THREAD_ATTACH通知）**。

**20.2.4 DLL_THREAD_DETACH通知**

（1）当线程函数返回后，系统会调用ExitThread来终止线程，但**在终止前，这个线程会用DLL_THREAD_DETACH去调用所有己映射DLL的DllMain函数**。告诉DLL执行与线程相关的清理操作（如C/C++运行库在这里可释放多线程应用程序的数据块）。

（2）如果该通知里有死循环，将妨碍线程的终止。只有当每个DLL都处理完DLL_THREAD_DETACH通知后，操作系统才会真正的终止线程。

（3）如果某个线程调用了TerminateThread来终止线程，那会系统将不会发送DLL_THREAD_DETACH通知给线程。这意味着DLL没有机会执行任何清理操作。

（4）如果在撤销一个DLL映射时，还有其他线程(正在运行)，系统不会发送DLL_THREAD_DETACH给这些线程。（即这些线程不会用DLL_THREAD_DETACH来调用这个DLL的DllMain）

【注意】上面的规则可能会出现一个情况：当进程中的一个线程调用LoadLibrary来载入一个DLL时，系统会用DLL_PROCESS_ATTACH来调用该DLL的DllMain（但该线程不会得到DLL_THREAD_ATTACH通知）。接着，这个载入DLL的线程退出，这时该线程会收到DLL_THREAD_DETACH通知。由于这个原因，当进行与线程相关的清理里必须极为小心。一般调用LoadLibrary与调用FreeLibrary的线程应该是同一个线程。











**20.3 延迟载入DLL**

**20.3.1延迟载入的目的**

（1）如果应用程序使用了多个DLL，那么它的初始化可能比慢，因为加载程序要将所有必需的DLL映射到进程的地址空间。→利用延迟加载可将载入过程延伸到执行过程时

（2）如果我们的代码调用的操作系统的一个新函数，但程序又试图在老版本的操作系统运行。这时程序会被终止。这时可以有两种解决方法：一种是判断利用GetVersionEx操作系统，在老系统中使用旧函数而不使用新函数。另一种是通过延迟载入，通过SEH来捕获异

**20.3.2延迟载入技术**

（1）延迟载入是**针对隐式链接DLL的**

（2）一个导出了字段（如即全局变量）的DLL是无法延迟载入的

（3）Kernel32.dll模块是无法延迟载入的，因为必须载入该模块才能调用LoadLibrary和GetProcAddress。

（4）不应在DllMain入口函数中调用一个延迟载入的函数，这可能导致程序崩溃

**20.3.3 使用方法及相关说明**

 **（1）使用方法**

　　①常规建立DLL和可执行模块

　　②链接可执行模块时，添加延迟加载开关

　　A.为了延迟加载Dll，还需要在解决方案的该项目“属性”->“配置属性”->“链接器”->“输入”->“延迟加载的Dll”中输入MyDll.dll（注意/DelayLoad:MyDll.dll这个开关不能用#pragma comment(linker, "/DelayLoad:MyDll.dll")来设置。

　　B.增加/Lib:DelayImp.lib开关：这可以用#include <delayimp.h>和#pragma comment(lib, "Delayimp.lib")。这个开关告诉链接器将delayimp中的__delayLoadHelper2函数嵌入到我们的可执行文件中。

　　C.如果需要手动卸载Dll，则需在可选“链接器”→“高级”中指定“卸载延迟加载的DLL”中输入“MyDll.dll”。但要注意两点：一是卸载时**只能调用__FUnloadDelayLoadedDll2(PCSTR szDll)函数**，而不能调用FreeLibrary。二是**该卸载操作是可选的**，不是必需的，只有在需要手动卸载Dll时才设置。

 **（2）/Lib:DelayImp.lib**此时链接器将执行下列的事项

　　①将MyDll.dll从.exe的导入段去除，这样操作系统就不会隐式载入该DLL

　　②在.exe中嵌 入一个新的延迟载入段(Delay Import Section,称为.didata)表示要从MyDll.dll中导入哪些函数。

　　③对延迟载入函数的调用会跳转到__delayLoadHelper2函数，来完成对延迟载入函数的解析。

**（3）其他说明**

　　①应用程序对延迟载入函数的调用实际上会调用__delayLoadHelper2函数，该函数会引用那个特殊的延迟载入段，并用LoadLibrary和GetProcAddress得到延迟载入函数的地址，然后修改对该函数的调用，这样以后将直接调用该延迟载入函数。

　　②同一个DLL中的其他函数仍然必须在第一次被调用的时修复，即其他函数第1次调用时仍然会LoadLibrary+GetProcAddess并修复函数地址。

 【Export/ImportDelay程序】演示延迟载入Dll





**20.4 函数转发器**

（1）函数转发器原理（下图是利用Dependency Walker打开Kernel32.dll得到）

![img](http://images2015.cnblogs.com/blog/772759/201512/772759-20151221225434968-1458035637.png) 

　　①图中CloseThreadpool*等4个函数转发到NTDLL中相应的函数中去了，但我们调用CloseThreadpool*等函数时，exe会被动态地链接到Kernel32.dll。当执行exe时，加载程序会发现被转发的函数实际上在NTDLL.dll中，然后它会将NTDLL.dll模块一并载入。

　　②当我们调用CloseThreadpool*函数时，那么调用GetProcAddress先在Kernel32的导出段是查找，并发现CloseThreadpool*是一个转发器函数，于是它会递归调用GetProcessAddress，在NTDLL  的导出段中查找相应的函数。

（2）**实现自己的函数转发器**

   **#pragma comment(linker,"/export:MyFunc =OtherDll.OtherFunc")**

   //即正在编译的Dll输出一个名为MyFunc的函数，但实际上这个函数在另一个叫OtherDll.dll模块中，函数名为OtherFunc.

**20.5 己知的DLL**

（1）操作系统对某些DLL进行了特殊处理，这些Dll被称为己知的Dll。在载入它们的时候，总是从"%SystemRoot%\System32"目录下查找。这些Dll被记录在注册表中如下的位置

![img](http://images2015.cnblogs.com/blog/772759/201512/772759-20151221225448718-2044444065.png) 

（2）当LoadLibrary(TEXT("A_Dll"))时，系统会用正常的搜索规则来定位这个Dll。但如果调用LoadLibrary(TEXT("A_Dll.dll"))时，系统会先将扩展名.dll去掉，然后在注册表查找名称为"A_Dll"的一项，并将载入该项后面“数据”项里指向的Dll（注意，这个Dll会在"%SystemRoot%\System32"目录里查找），如果载入不成功，会返回NULL，GetLastError将返回(ERROR_FILE_NOT_FOUND).

 **20.6 DLL重定向**

（1）早期Windows为了文件共享，将多个应用程序共享的所有模块都放在Windows系统目录中，但会出现一个严重问题，因为安装程序会用老版本的文件覆盖这个目录中的文件，从而妨碍其他应用程序的正常运行。

（2）从Windows2000开始，新增了DLL重定向特性，使得应用程序首先从应用程序的目录中载入模块，只有当加载程序无法找到这个文件时，才会在其他目录中搜索。

（3）为了强制加载程序先检查应用程序的目录，可以在应用程序目录中，建一个文件名为AppName.local的文件（此处的AppName如MyApp.exe），内容无关紧要。

（4）LoadLibrary(Ex)在内部做了修改，来检查这个文件是否存在，如果应用程序目录中存在这个文件，便载入这个目录中的模块。如果不存在该.local文件，则工作方式与以往相同。

（5）由于安全性缘故，该特性默认是关闭的，因为它会使系统从应用程序的文件夹中载为伪造的系统DLL，而不是从Windows的系统文件夹中载入真正的系统DLL。为打开这个特性，可以HKLM\Software\Microsoft\WindowsNT\CurrentVesion\Image File Execution Options注册表项中增一个项名为DevOverrideEnable的DWORD型项，并将值设为1。

**20.7 模块的基地址重定位**

**（1）基址重定位的原因**

```c
int g_x;

void Func(){
    g_x = 5;  //该行很重要，当编译和链接该行成会生成Mov [0xXXXXXXXX],5之类的代码
}
```

　　①一般EXE的首选基地址为0x00400000，DLL为0x1000000。当运行exe时，加载程序为进程创建一个虚拟地址空间，并将exe映射到0x00400000处，g_x变量的变成0x00414540之类的固定地址。而当该这些代码是位于DLL模块时，这个地址将被固定为类似0x10014540之类的地址（前提是该DLL被加载到首选地址处）。

　　②对于EXE来说，会被加载到首选基地址处而不会造成冲突。但DLL则不一样，如果有两个DLL的首选基地址都是0x10000000。加载程序会将第1个DLL加载到首选地址上，但会对第2个DLL模块进行重定位（如加载到0x20000000）,则g_x的地址会被修改为0x20014540，即汇编代码为Mov [0x20014540],5。

**（2）EXE或（Dll）重定位的缺点**

　　①当链接器在构建模块时，会将**重定位段**(relocation section)嵌入在生成的文件中。重定位表中的有用数据是那些需要重定位机器码所使用到的内存地址的偏移量。

　　②如果加载程序无法将模块载入到它的首选基地址，**那么系统会遍历重定位段中的所有条目**，对每一个条目，加载程序会先找到机器指令的那个存储页面。然后将模块的首选基地址减去实际映射地址，将这个差值加到机器指令使用的地址上。因为加载程序必须遍历重定位段并修改模块中的大量代码，这个过程会牺牲程序的启动时间。

　　③当加载程序写入模块的代码页面中时，**系统的写时复制会强制这些页面以系统的页交换文件为后备存储器**。因为页交换文件是所有模块(如EXE或DLL)的代码页面的后备存储器，所以这也会损害性能，减少可供系统中所有进程使用的存储器的数量。

**（3）指定DLL基地址的方法**

　　①方法1：在“配置属性”→“链接器”→“高级”→“基址”输入如0x20000000之类。同时“固定基址”这项选项“是（/Fixed）”。注意，为了少地址空间碎片，应该总是先从高内存地址开始载入DLL，然后再到低内存地址。

　　②方法2：

　　A.创建一个文件文件，在其中按照下述语法指定每个DLL的基地址和大小（大小可选）

​     　　**key  address [size]  ;comment**

​    　　其中key是一个字母数字组成的字符串，不区分大小写；通常是Dll的名称，但不必非是；只要和在/BASE中指定一样就行。Address是十六进制或十进制表示的基址。Size是可选的，一般为最大DLL的size。

　　B.将这个文本文件放到链接器可以搜到的地方

　　C.在动态库项目的基址选项中指定@filename,key格式的命令，其中@是固定前缀，filename就是刚才的文本文件，可以指定完整路径名，key就是文本文件中指定的key

【CustomDLL程序】演示修改写DLL入口点函数及用方法2指定基地址

 ![img](http://images2015.cnblogs.com/blog/772759/201512/772759-20151225230105781-1674176324.png)









# （十七） 线程局部储存

2017年02月07日 23:43:41 [wangpengk7788](https://me.csdn.net/wangpengk7788) 阅读数：2152



21.1 动态TLS

**21.1.1 为什么要使用线程局部存储**

　　编写多线程程序的时候都希望存储一些线程私有的数据，我们知道，属于每个线程私有的数据包括线程的栈和当前的寄存器，但是这两种存储都是非常不可靠的，栈会在每个函数退出和进入的时候被改变，而寄存器更是少得可怜。假设我们要在线程中使用一个全局变量，但希望这个全局变量是线程私有的，而不是所有线程共享的，该怎么办呢？这时候就须要用到线程局部存储（TLS，Thread Local Storage）这个机制了。

**21.1.2 动态TLS**![img](http://images2015.cnblogs.com/blog/772759/201602/772759-20160201105748054-1730811992.png) 

（1）每个进程都有一组正在使用标志，共TLS_MINIMUM_AVAILABLE个。每个标志可以被设为FREE或INUSE，表示该TLS元素是否正在使用。（注意这组标志属进程所有）

（2）当系统创建一个线程的时候，会为该线程分配与线程关联的、属于线程自己的PVOID型数组（共有TLS_MINIMUM_AVAILBALE个元素），数组中的每个PVOID可以保存任意值。

**21.1.3 使用动态TLS**

（1）调用TlsAlloc函数

　　①该函数会检索系统进程中的位标志并找到一个FREE标志，然后将该标志从FREE改为INUSE，并返回该标志在位数组中的索引，通常将该索引保存在一个全局变量中，因为这个值会在整个进程范围内（而不是线程范围内）使用。

　　②如果TlsAlloc无法在列表中找到一个FREE标志，会返回TLS_OUT_OF_INDEXES。

　　③以上就是TlsAlloc99%的工作，剩1%的工作就是在函数返回之前，会遍历进程中的每个线程，并根据新分配的索引，在每个线程的Tls数组中把对应的素素设为0（具体原因请看后面的分析）。

（2）调用TlsSetValue(dwTlsIndex,pvTlsValue)将一个值放到线程的数组中

　　①该函数把pvTlsValue所标志的一个PVOID值放到线程的数组中,dwTlsIndex指定了在数组中的具体位置（由TlsAlloc得到）

　　②当一个线程调用TlsSetValue的时候，会修改自己的数组。但它无法修改另一个线程的TLS数组中的值。

（3）从线程的数组中取回一个值：PVOID TlsGetValue(dwTlsIndex)

　　①与TlsSetValue相似，TlsGetValue中会查看属于调用线程的数组

（4）释放己经预订的TLS元素：TlsFree(dwTlsIndex)

　　①该函数会将进程内的位标志数组对应的INUSE标志重设回FREE

　　②同时该函数还会将所有线程中该元素的内容设为0。

　　③试图释放一个尚未分配的TLS元素将导致错误

**21.1.4编写类似_tcstok_s函数**



```c
DWORD g_dwTlsIndex; //假设这个全局变量是通过TlsAlloc函数来初始化的

void MyFunction(PSOMESTRUCT PSomeStruct){

    if (pSomeStruct != NULL){
        //调用者正在启用该函数，就像strok函数第一次传入非NULL，
        //以后传为NULL

        //检查是否己经为数据分配存储空间
        if (TlsGetValue(g_dwTlsIndex) == NULL)
            //线程第一次调用该函数时，该空间尚未分配
            //TlsAlloc函数返回之前，会将进程中所有线程的g_dwTlsIndex元素
            //清零，以保证这句代码不会出错非空的现象！

            //通过TLS能保证分配的空间只与调用线程相关联
            TlsSetValue(g_dwTlsIndex,  
                        HeapAlloc(GetProcessHeap(), 0, sizeof(*pSomeStruct));
        }

        //将传入的pSomeStruct数据保存刚才那个只与调用线程相关的存储空间中
        memcpy(TlsGetValue(g_dwTlsIndex), pSomeStruct, sizeof(*pSomeStruct));
    } else{ 
        //调用者己经第二次（或以上）调用该函数，会传入NULL参数
 
        //取出数据
        pSomeStruct = (PSOMESTRUCT)TlsGetValue(g_dwTlsIndex);

        //以下可以开始pSomeStruct这个数据了。
        ...
    }
}
```



**21.2 静态TLS**

（1）静态TLS变量的声明

　　①__thread int number;//GCC使用__thread关键字声明

　　②__declspec(thread) int number; //MSVC使用__declspec(thread)声明

（2）Windows中静态TLS的实现原理

　　①对于Windows系统来说，正常情况下一个全局变量或静态变量会被放到".data"或".bss"段中，但当我们使用__declspec(thread)定义一个线程私有变量的时候，编译器会把这些变量放到PE文件的".tls"段中。

　　②当系统启动一个新的线程时，它会从进程的堆中分配一块足够大小的空间，然后把".tls"段中的内容复制到这块空间中，于是每个线程都有自己独立的一个".tls"副本。所以对于用__declspec(thread)定义的同一个变量，它们在不同线程中的地址都是不一样的。

　　③对于一个TLS变量来说，它有可能是一个C++的全局对象，那么每个线程在启动时不仅仅是复制".tls"的内容那么简单，还需要把这些TLS对象初始化，必须逐个地调用它们的全局构造函数，而且当线程退出时，还要逐个地将它们析构，正如普通的全局对象在进程启动和退出时都要构造、析构一样。

　　④Windows PE文件的结构中有个叫数据目录的结构。它总共有16个元素，其中有一元素下标为IMAGE_DIRECT_ENTRY_TLS，这个元素中保存的地址和长度就是TLS表（IMAGE_TLS_DIRECTORY结构）的地址和长度。TLS表中保存了所有TLS变量的构造函数和析构函数的地址，Windows系统就是根据TLS表中的内容，在每次线程启动或退出时对TLS变量进行构造和析构。TLS表本身往往位于PE文件的".rdata"段中。

　　⑤另外一个问题是，既然同一个TLS变量对于每个线程来说它们的地址都不一样，那么线程是如何访问这些变量的呢？其实对于每个Windows线程来说，系统都会建立一个关于线程信息的结构，叫做线程环境块（TEB，Thread Environment Block）。这个结构里面保存的是线程的堆栈地址、线程ID等相关信息，其中有一个域是一个TLS数组，它在TEB中的偏移是0x2C。对于每个线程来说，x86的FS段寄存器所指的段就是该线程的TEB，于是要得到一个线程的TLS数组的地址就可以通过FS:[0x2C]访问到。

　　⑥这个TLS数组对于每个线程来说大小是固定的，一般有64个元素。而TLS数组的第一个元素就是指向该线程的".tls"副本的地址。于是要得到一个TLS的变量地址的步骤为：首先通过FS:[0x2C]得到TLS数组的地址，然后根据TLS数组的地址得到".tls"副本的地址，然后加上变量在".tls"段中的偏移即该TLS变量在线程中的地址。

【DllTls】演示线程局部存储的例子

![img](http://images2015.cnblogs.com/blog/772759/201602/772759-20160201155519163-217582814.png)

1、动态链接库端的代码：



```c
/************************************************************************
Module: DllTls.h
************************************************************************/
#pragma  once
#include <windows.h>

#ifdef DLLTLS_EXPORT
//DLLTLS_EXPORT必须在Dll源文件包含该头件前被定义
#define DLLTLSAPI extern "C" __declspec(dllexport)
//本例中所有的函数和变量都会被导出
#else
#define DLLTLSAPI extern "C" __declspec(dllimport)
#endif

//定义要导出的函数的原型
DLLTLSAPI VOID* GetThreadBuf();
DLLTLSAPI UINT GetThreadBufSize();
```





```c
#include <tchar.h>
#include <windows.h>
#include <strsafe.h>
#include <locale.h>

//在这个DLL源文件定义要导出的函数和变量
#define DLLTLS_EXPORT   //这个源文件中须定义这个宏，以告诉编译器函数要
//__declspect(dllexport)，这个宏须在包含MyLib.h
//之前被定义

#include "DllTls.h"

#define GRS_ALLOC(sz)    HeapAlloc(GetProcessHeap(),0,sz)
#define GRS_CALLOC(sz)   HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,sz)
#define GRS_SAFEFREE(p)  if(NULL!=p){HeapFree(GetProcessHeap(),0,p);p=NULL;}

static DWORD g_dwTLS = 0;
static UINT g_nBufSize = 256;

BOOL APIENTRY DllMain(HANDLE hDllHandle, DWORD dwReason, LPVOID lpreserved){
    switch (dwReason)
    {
    case DLL_PROCESS_ATTACH:
        {
            _tsetlocale(LC_ALL, _T("chs"));
            g_dwTLS = TlsAlloc();
            if (TLS_OUT_OF_INDEXES == g_dwTLS){
                _tprintf(_T("为进程[ID:0x%X]分配TLS索引失败！\n"),
                         GetCurrentProcessId());
                return FALSE;
            }

            _tprintf(_T("为进程[ID:0x%X]分配TLS索引:%u！\n"),
                     GetCurrentProcessId(),g_dwTLS);
        }
        break;

    case DLL_THREAD_ATTACH:
        {
            PVOID pThdData = GRS_CALLOC(g_nBufSize);

            if (!TlsSetValue(g_dwTLS,pThdData)){
                _tprintf(_T("为线程[ID:0x%X]设置TLS索引[%u]变量[0x%08X]失败！\n"),
                         GetCurrentThreadId(), g_dwTLS,pThdData);
                GRS_SAFEFREE(pThdData);
                return FALSE;
            }

            _tprintf(_T("为线程[ID:0x%X]设置TLS索引[%u]变量[0x%08X]！\n"),
                     GetCurrentThreadId(), g_dwTLS, pThdData);
        }
        break;

    case DLL_THREAD_DETACH:
        {
            PVOID pThdData = TlsGetValue(g_dwTLS);

            if (!pThdData){
                _tprintf(_T("为线程[ID:0x%X]获取TLS索引[%u]变量[0x%08X]失败！\n"),
                         GetCurrentThreadId(), g_dwTLS, pThdData);
                return FALSE;
            }

            _tprintf(_T("为线程[ID:0x%X]获取TLS索引[%u]变量[0x%08X]并销毁！\n"),
                     GetCurrentThreadId(), g_dwTLS, pThdData);
            GRS_SAFEFREE(pThdData);
        }
        break;

    case DLL_PROCESS_DETACH:
        {
            _tprintf(_T("释放进程[ID:0x%X]TLS索引[%u]\n"),
                 GetCurrentProcessId(), g_dwTLS);
            TlsFree(g_dwTLS);
        }
        break;
    }

    return TRUE;
}

VOID* GetThreadBuf()
{
    return TlsGetValue(g_dwTLS);
}

UINT GetThreadBufSize(){
    return g_nBufSize;
}
```



2、测试程序



```cpp
#include <windows.h>
#include <tchar.h>
#include <strsafe.h>
#include <locale.h>
#include "../../Chap21/21_DLLTls/DllTls.h"

#pragma comment(lib,"../../Debug/21_DLLTls.lib")

#define GRS_CREATETHREAD(Fun,Param) CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)Fun,Param,0,NULL)

DWORD WINAPI ThreadProc(LPVOID pvParam){
    TCHAR* pTlsBuf = (TCHAR*)GetThreadBuf();
    UINT nSize = GetThreadBufSize();
    if (NULL !=pTlsBuf && 0 != nSize){
        _tprintf(_T("线程[0x%X]取得缓冲区[地址:0x%08X 大小(字节):%u]，写入数据\n"),
                 GetCurrentThreadId(),pTlsBuf,nSize);
        StringCchPrintf(pTlsBuf, nSize / sizeof(TCHAR), _T("ID：0x%X"), GetCurrentThreadId());
        Sleep(1000);
        _tprintf(_T("线程[0x%X]取得缓冲区[地址:0x%08X 大小(字节):%u]，写入的数据为[%s]\n"),
                 GetCurrentThreadId(), pTlsBuf, nSize,pTlsBuf);
    }
    _tprintf(_T("线程[0x%X]退出\n"),GetCurrentThreadId());
    return 0;
}

#define THREADCNT  2
int _tmain(){

    _tsetlocale(LC_ALL, _T("chs"));

    HANDLE phThread[THREADCNT] = {};
    for (int i = 0; i < THREADCNT; i++){
        phThread[i] = GRS_CREATETHREAD(ThreadProc, NULL);
    }

    WaitForMultipleObjects(THREADCNT, phThread, TRUE, INFINITE);
    for (int i = 0; i < THREADCNT;i++){
        CloseHandle(phThread[i]);
    }
    _tsystem(_T("PAUSE"));
    return 0;
}
```









# （十八） SEH结构化异常 一

2017年02月08日 17:59:06 [wangpengk7788](https://me.csdn.net/wangpengk7788) 阅读数：462



23.1 基础知识

**23.1.1 Windows下的软件异常**

（1）中断和异常

　　①中断是由外部硬件设备或异步事件产生的

　　②异常是由内部事件产生的，可分为故障、陷阱和终止三类。

（2）两种异常处理机制：SEH和VEH（WindowsXP以上新引进）

（3）结构化异常处理（SEH）是Windows操作系统提供的强大异常处理功能。而Visual C++中的__try{}/__finally{}和__try{}/__except{}结构本质上是对Windows提供的SEH的封装。

**23.1.2 SEH的分类**

（1）Per-Thread类型SEH（也称为**线程异常处理**），用来监视某线程代码是否发生异常。

（2）Final类型SEH（也称为进程异常处理、筛选器或**顶层异常处理**），用于监视整个进程中所有线程是否发生异常。在整个进程中，该类型异常处理过程只有一个，可通过SetUnhandledExceptionFilter设置。

**23.1.3 SEH相关的数据结构**

**（1） 线程信息块TIB**（Thread Information Block或TEB）

![复制代码](http://common.cnblogs.com/images/copycode.gif)

```c
typedef struct _NT_TIB {
     struct _EXCEPTION_REGISTRATION_RECORD *ExceptionList; //异常的链表

     PVOID StackBase;
     PVOID StackLimit;
     PVOID SubSystemTib;

     union {
         PVOID FiberData;
         DWORD Version;
     };
 
     PVOID ArbitraryUserPointer;
     struct _NT_TIB *Self;
} NT_TIB;
```

![复制代码](http://common.cnblogs.com/images/copycode.gif)

​    **Fs:[0]总是指向当前线程的TIB，其中0偏移的指向线程的异常链表，即ExceptionList是指向异常处理链表（EXCEPTION_REGISTRATION结构）的一个指针。**

**（2）EXCEPTION_REGISTRATION结构**

```
typedef struct _EXCEPTION_REGISTRATION_RECORD {
     struct _EXCEPTION_REGISTRATION_RECORD *Prev; //指向前一个EXCEPTION_REGISTRATION的指针
     PEXCEPTION_ROUTINE Handler; //当前异常处理回调函数的地址
} EXCEPTION_REGISTRATION_RECORD;
```

（3）EXCEPTION_RECORD结构

![复制代码](http://common.cnblogs.com/images/copycode.gif)

```c
typedef struct _EXCEPTION_RECORD {
    DWORD    ExceptionCode;  //异常码，以STATUS_或EXCEPTION_开头，可自定义。（sehdef.inc）
    DWORD ExceptionFlags;    //异常标志。0可修复；1不可修复；2正在展开，不要试图修复
    struct _EXCEPTION_RECORD *ExceptionRecord; //指向嵌套的异常结构，通常是异常中又引发异常
    PVOID ExceptionAddress;  //异常发生的地址
    DWORD NumberParameters;  //下面ExceptionInformation所含有的dword数目
    ULONG_PTR ExceptionInformation[EXCEPTION_MAXIMUM_PARAMETERS]; //附加消息，如读或写冲突
} EXCEPTION_RECORD;
```

![复制代码](http://common.cnblogs.com/images/copycode.gif)

（4）CONTEXT结构（可通过GetThreadContext和SetThreadContext来读写）

![复制代码](http://common.cnblogs.com/images/copycode.gif)

```c
typedef struct _CONTEXT {
     DWORD ContextFlags; //用来表示该结构中的哪些域有效
     DWORD   Dr0, Dr2, Dr3, Dr4, Dr5, Dr6, Dr7; //调试寄存器
     FLOATING_SAVE_AREA FloatSave; //浮点寄存器区
     DWORD   SegGs, SegFs, SegEs, Seg Ds; //段寄存器
     DWORD   Edi, Esi, Ebx, Edx, Ecx, Eax; //通用寄存器组
     DWORD   Ebp, Eip, SegCs, EFlags, Esp, SegSs; //控制寄存器组

     //扩展寄存器，只有特定的处理器才有
     BYTE    ExtendedRegisters[MAXIMUM_SUPPORTED_EXTENSION];
} CONTEXT;
```

![复制代码](http://common.cnblogs.com/images/copycode.gif)

（5）EXCEPTION_POINTERS结构（传递给顶层型异常处理回调函数的参数）

```c
typedef struct _EXCEPTION_POINTERS {
    PEXCEPTION_RECORD ExceptionRecord; //指向ExceptionRecord的指针
    PCONTEXT ContextRecord; //指向Context结构的指针
} EXCEPTION_POINTERS, *PEXCEPTION_POINTERS;
```

23.1.4 异常处理相关API

（1）SetErrorMode：控制错误模式，如是否出现错误对话框

（2）SetUnhandledExceptionFilter：设定顶层异常处理回调函数

（3）RaiseException：用于引发异常，可指定自己的异常代码及相关信息

（4）GetThreadContext/SetThreadContext：获取或设置线程环境

（5）RtlUnwind：栈展开操作（后面会详细介绍）

**23.1.5 顶层异常处理**

（1）顶层异常处理是进程相关的，只要没有由线程处理过程或调试器处理掉，最终均要交由顶层异常回调函数处理

（2）注册顶层异常处理：SetUnhandledExceptionFilter，每个进程只能注册一个。返回值为前一个异常回调函数的地址。

**（3）回调函数**

![复制代码](http://common.cnblogs.com/images/copycode.gif)

```c
LONG UnhandledExceptionFilter(STRUCT _EXCEPTION_POINTERS *ExceptionInfo){

   //三种返回值，决定系统下一步的动作
   //1.EXCEPTION_EXECUTE_HANDLE(1):表示异常己被处理。程序可以优雅结束
   //2.EXCEPTION_CONTINUE_SEARCH(0)：表示顶层异常处理不能处理，需交给其他异常处理过程
   //3.EXCEPTION_CONTINUE_EXECUTION(-1):表示顶层异常过程处理了异常，且程序应从原异常发生的指令重新继续执行一下。（可以通过改变CONTEXT内容来达到改变程序执行环境）
}
```

![复制代码](http://common.cnblogs.com/images/copycode.gif)

23.1.6 线程异常处理（局部的，仅仅监视进程中某特定线程是否发生异常）

（1）线程异常处理特点

　　①Windows系统为每个线程单独提供了一种异常处理的方法，当一个线程出现错误时，操作系统调用用户定义的一系列回调函数，在这些回调函数中，可以进行修复错误或其它的一些操作，最后的返回值告系统系统下一步的动作（如继续搜索异常处理程序或终止程序等）。

　　②SEH是基于线程的，使用SEH可以为每个线程设置不同的异常处理程序（回调函数）而且可以为每个线程设置多个异常处理程序。

　　③ 由于SEH使用了与硬件平台相关的数据指针，所以不同硬件平台使用SHE的方法有所不同。

（2）回调函数原型

![复制代码](http://common.cnblogs.com/images/copycode.gif)

```c
EXCEPTION_DISPOSITION __cdecl _except_handler(

     struct _EXCEPTION_RECORD *ExceptionRecord,//指向包含异常信息的EXCEPTION_RECORD结构
     void* EstablisherFrame,//指向该异常相关的EXCEPTION_REGISTRATION结构
     struct _CONTEXT *ContextRecord,//指向线程环境CONTEXT结构的指针
     void* DispatcherContext){ //该域暂无意义

     ……

    //4种返回值及含义
    //1.ExceptionContinueExecution(0)：回调函数处理了异常，可以从异常发生的指令处重新执行。
    //2.ExceptionContinueSearch(1)：回调函数不能处理该异常，需要要SEH链中的其他回调函数处理。
    //3.ExceptionNestedException(2)：回调函数在执行中又发生了新的异常，即发生了嵌套异常
    //4.ExceptionCollidedUnwind(3)：发生了嵌套的展开操作

    return …
}
```



（3）线程异常处理的注册

```
push _exception_handler //异常回调函数_exception_handler的地址，即handler
push fs:[0]             //保存前一个异常回调函数的地址，即prev
mov fs:[0],esp          //安装新的EXCEPTION_REGISTRATION结构（两个成员:prev,handler）。
                        //此时栈顶分别是prev和handler，为新的EXCEPTION_REGISTRATION结
                        //构，mov fs:[0],esp，就可以让fs:[0]指向该指构。
```

（4）异常回调函数的调用过程

![img](http://images2015.cnblogs.com/blog/772759/201602/772759-20160221194707655-53764841.png) 

　　①线程信息块（TIB），永远放在fs段选择器指定的数据段的0偏移处，即fs:[0]的地方就是TIB结构。对不同的线程fs寄存器的内容有所有不同，**但fs:[0]都是指向当前线程的TIB结构体**，所以fs:[0]是一个EXCEPTION_REGISTRATION结构体的指针。

　　②当异常发生时，系统从fs:[0]指向的内存地址处取出ExceptionList字段，然后从ExceptionList字段指向的EXCEPTION_REGISTRATION结构中取出handler字段，并根据其中的地址去调用异常处理程序（回调函数）。

【MySeh程序】安装线程异常处理程序

![img](http://images2015.cnblogs.com/blog/772759/201602/772759-20160221194851045-811197148.png)

![复制代码](http://common.cnblogs.com/images/copycode.gif)

```c
/************************************************************************
MYSEH - Matt Pietrek 1997
Microsoft Systems Journal,January 1997
cl.exe myseh.cpp /link -SAFESEH:NO
用IDE编译时须将项目的“链接器”→“高级”→“映像具有安全异常处理程序”设为SAFESEH:NO（Debug或Release版要分别设置）
用命令行CL MYSHE.CPP编译里可参考下面的MySeh.bat批处理的设置。
************************************************************************/
//MySeh.bat
//set PATH = %PATH%; E:\读书笔记\Windows核心编程\SEH\SEH; D:\VisualStudio\VC\bin
//set include = C:\Program Files\Windows Kits\8.1\Include\um; C:\Program Files\Windows Kits\8.1\Include\shared; D:\VisualStudio\VC\include
//set lib = C:\Program Files\Windows Kits\8.1\Lib\winv6.3\um\x86; D:\VisualStudio\VC\lib
//cl.exe E : \读书笔记\Windows核心编程\SEH\SEH\myseh.cpp / link - SAFESEH:NO
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

DWORD scratch;

EXCEPTION_DISPOSITION __cdecl _except_handler(struct _EXCEPTION_RECORD *ExceptionRecord,
                                              void* EstablisherFrame,
                                              struct _CONTEXT *ContextRecord,
                                              void* DispatcherContext){

    //指明是我们让流程转到我们的异常处理程序的
    printf("进入Seh异常处理程序！\n");

    //改变CONTEXT结构体EAX的值，以便它指向可以成功进行写操作的位置
    ContextRecord->Eax = (DWORD)&scratch;

    printf("异常处理完毕！\n");

    //告诉操作系统重新执行出错的指令
    return ExceptionContinueExecution;
}

int main(){

    DWORD handler = (DWORD)_except_handler;

    printf("安装seh异常回调函数!\n");
    __asm{
    
        //创建EXCEPTION_REGISTRATION结构：
        push handler  //handler函数的地址
        push fs:[0]   //保存前一个handler函数地址,即EXCEPTION_REGISTRATION结构的prev

        mov fs:[0],ESP  //安装新的EXCEPTION_REGISTRATION结构

    }
    printf("安装完毕，向0地址写入数据，引发异常!\n");
    __asm
    {
        xor eax, eax                    //EAX = 0
        mov dword ptr[eax], 1234h    //写EAX指向的内存从而故意引发一个异常!
    }
    printf("写入数据完毕，scratch=0x%x!\n",scratch);
    printf("卸载seh自定义的异常回调函数!\n");
    __asm{
        // 移去我们的EXECEPTION_REGISTRATION结构
        mov eax, [ESP]    // 此时栈顶为prev，后进先出原理。获取前一个EXECEPTION_REGISTRATION结构的指针
        mov fs:[0], EAX // 安装前一个结构
        add esp, 8       // 将我们的EXECEPTION_REGISTRATION弹出堆栈
    }

    system("Pause");
    return 0;
}
```



（5）SEH链及异常的传递（通知调试器→SEH链→顶层异常处理→系统默认处理）

![img](http://images2015.cnblogs.com/blog/772759/201602/772759-20160221203646889-1754647384.png) 

（1）系统查看产生异常的进程是否被正在被调试，如果正在被调试，那么向调试器发送EXCEPTION_DEBUG_EVENT事件。

（2）如果进程没有没有被调试或者调试器不去处理这个异常，那么系统检查异常所处的线程并在这个线程环境中查看fs:[0]来确定是否安装SEH异常处理回调函数，如果有则调用它。

（3）回调函数尝试处理这个异常，如果可以正确处理的话，则修正错误并将返回值设置为ExceptionContinueExecution，这时系统将结束整个查找过程。

（4）如果回调函数返回ExceptionContinueSearch，相当于告诉系统它无法处理这个异常，系统将根据SEH链中的prev字段得到前一个回调函数地址并重复步骤3，直至链中的某个回调函数返回ExceptionContinueExection为止，查找结束。

（5）如果到了SEH链的尾部却没有一个回调函数愿意处理这个异常，那么系统会再被检查进程是否正在被调试，如果被调试的话，则再一次通知调试器。

（6）如果调试器还是不去处理这个异常或进程没有被调试，那么系统检查有没有Final型的异常处理回调函数，如果有，就去调用它，当这个回调函数返回时，系统会根据这个函数的返回值做相应的动作。

（7）如果没有安装Final型回调函数，系统直接调用默认的异常处理程序终止进程，但在终止之前，系统再次调用发生异常的线程中的所有异常处理过程，目的是让线程异常处理过程获得最后清理未释放资源的机会，其后程序终止。

 【MySEH2】SEH链及异常传递

　　①当使用SEH时，如果一个函数有一个异常处理程序但它却不处理某个异常，这个函数就有可能非正常退出。例如在MYSEH2中 HomeGrownFrame函数就不处理异常。由于在链表中后面的某个异常处理程序（这里是main函数中的）处理了这个异常，因此出错指令后面的 **printf就永远不会执行，即第52行**。

　　②比较开头输出的两行。第一次异常标志是0，而第二次是2。这就是**堆栈展开（Unwinding）**。实际上，当一个异常处理回调函数拒绝处理某个异常时，它会被再一次调用。

![img](http://images2015.cnblogs.com/blog/772759/201602/772759-20160221212603780-941518700.png)



```c
/************************************************************************
MYSEH2 - Matt pietrek 1997
Microsoft Systems Journal,january 1997
File: MySEH2.cpp
用IDE编译时须将项目的“链接器”→“高级”→“映像具有安全异常处理程序”设为SAFESEH:NO（Debug或Release版要分别设置）
************************************************************************/
#include <windows.h>
#include <stdio.h>

//异常处理回调函数
EXCEPTION_DISPOSITION __cdecl _except_handler(
       struct _EXCEPTION_RECORD *ExceptionRecord,
       void * EstablisherFrame,
       struct _CONTEXT *ContextRecord,
       void * DispatcherContext){
    //显示异常信息
    printf("HomeGrown异常处理回调函数：异常码(%08X) 标志（%X）", 
           ExceptionRecord->ExceptionCode,ExceptionRecord->ExceptionFlags);
    if (ExceptionRecord->ExceptionFlags & 1)
        printf(" EH_NONCONTINUABLE");

    if (ExceptionRecord->ExceptionFlags & 2)
        printf(" EH_UNWINDING");

    if (ExceptionRecord->ExceptionFlags & 4)
        printf(" EH_EXIT_UNWIND");

    if (ExceptionRecord->ExceptionFlags & 8) //
        printf(" EH_STACK_INVALID");

    if (ExceptionRecord->ExceptionFlags & 0x10) //
        printf(" EH_NESTED_CALL");
    
    printf("\n");

    //我们不想处理这个异常，让其它SEH链的其他回调函数去处理
    return ExceptionContinueSearch;
}

void HomeGrownFrame(void){
    DWORD  handler = (DWORD)_except_handler;

    __asm{
        //创建EXCEPTION_REGISTRATION结构
        push handler //handler函数的地址
        push FS :[0] //保存前一个handler地址
        mov FS :[0],ESP //安装新的EXCEPTION_REGISTRATION结构
    }

    *(PDWORD)0 = 0; //向地址0数据，从而引发一个异常

    printf("_except_handle函数不处理异常，此句永远不会被执行！\n");

    __asm{
        //移去我个的EXCEPTION_REGISTRATION结构
        mov eax, [ESP]   //获取前一个结构
        mov FS : [0],eax //安装前一个结构
        add esp,8        //把我们的EXCEPTION_REGISTRATION结构弹出
    }
}

int main(){
    //为了简化代码，使用编译器层面的异常处理，即__try{} __excetp{}
    __try{
        HomeGrownFrame();
    }
    __except (EXCEPTION_EXECUTE_HANDLER){  //这里处理了异常，不再向外层传递
        printf("主函数main中的异常处理过程\n");
    }

    return 0;
}
```



 **23.1.7 异常处理的堆栈展开（Stack Unwind）**

**（1）什么是展开操作**

　　①当发生异常时，系统遍历EXCEPTION_REGISTRATION结构链表，从链表头，直到它找到一个处理这个异常的处理程序。一旦找到，系统就再次重新遍历这个链表，直到处理这个异常的节点为止（即返回ExceptionContinueExecution节点）。在这第二次遍历中，系统将再次调用每个异常处理函数。关键的区别是，在第二次调用中，异常标志被设置为2。这个值被定义 为EH_UNWINDING

　　②注意展开操作是发生在出现异常时，这个异常回调函数**拒绝处理**的时候**（即返回ExceptionContinueSearch）**。这里系统会**从链表**头**开始**遍历（因异常的嵌套，可理解为由内层向外层），所以各异常回调函数会第1次被依次调用，直到找到同意处理的节点。然后，再重新从链表头开始（即由内向外）第2次调用以前那些曾经不处理异常的节点，直到同意处理的那个异常的节点为止。

　　③当一个异常处理程序拒绝处理某个异常时，它实际上也就无权决定流程最终将从何处恢复。只有**处理某个异常的异常处理程序才能决定待所有异常处理代码执行完毕之后流程最终将从何处恢复**。即，当异常已经被处理完毕，并且所有前面的异常帧都已经被展开之后，流程从处理异常的那个回调函数决定的地方开始继续执行。

　　④展开操作完成后，**同意处理异常的回调函数**也必须负责把Fs:[0]恢复到处理这个异常的EXCEPTION_REGISTRATION上，即展开操作导致堆栈上处理异常的帧以下的堆栈区域上的所有内容都被移除了，这个异常处理也就成了SEH链表的第1个节点。

**（2）为什么要进行堆栈展开**

　　①第1个原因是**告知回调函数将被卸掉**，以让被卸载的回调函数有机会进行一些**清理未释放资源的机会**。因为一个函数发生异常时，执行流程通常不会从这个函数正常退出。所以可以导致资源未被正确释放（如C++类的对象析构函数没被调用等）。

　　②第2个原因是如果不进行堆栈展开，可能会引发未知的错误。（见《软件加密技术内幕》第132-134页）。

**（3）如何展开：**RtlUnwind(lpLastStackFrame,lpCodelabel,lpExceptionRecord,dwRet);

　　①lpLastStackFrame：当遍历到这个帧时就停止展开异常帧。为NULL时表示展开所有回调函数。

　　②lpCodeLabel:指向该函数返回的位置。如果指定为NULL，表示函数使用正常的方式返回。

　　③lpExceptionRecord：指定一个EXCPETION_RECORD结构。这个结构将在展开操作的时候被传给每一个被调用的回调函数，一般建议使用NULL来让系统自动生成代表展开操作的EXCEPTION_RECORD结构。

　　④dwRet一般不被使用。

【注意】：

　　①MySEH2程序中我们并没有在main函数中的__try{}的 __except{}中调用RtlUnwind，是因为编译器在生成__try{}/__except{}时，己经帮我们做好了。否则如果是利用Windows本身的SEH来写的话，应该在返回ExceptionContinueExecution的回调函数中调用RtlUnwind来展开。

　　②RtlUnwind这个函数并不像其他API函数会保存esi、edi和ebx等寄存器，在函数返回的时候，这些寄存器的值也可能会被改变。如果程序用到了这些寄存器的话，要自己保存和恢复它们。









# （十八） SEH结构化异常 二

2017年02月08日 19:45:04 [wangpengk7788](https://me.csdn.net/wangpengk7788) 阅读数：339



**23.2 编译器层面对系统SEH机制的封装**

**23.2.1 扩展的EXCEPTION_REGISTRATION级相关结构：VC_EXCEPTION_REGISTRATION**

**（1）VC_EXCEPTION_REGISTRATION结构**

![复制代码](http://common.cnblogs.com/images/copycode.gif)

```
 struct VC_EXCEPTION_REGISTRATION
{
     VC_EXCEPTION_REGISTRATION* prev;    //前一个结构体的指针
     FARPROC                    handler; //永远指向_exception_handler4回调函数
     scopetable_entry*          scopetable;//指向scpoetable数组的指针
     int                        _index; //有的书上也叫tryLevel。scopetable项的当前项
     DWORD                      _ebp;   //当前ebp值，用于访问各成员
}
```

![复制代码](http://common.cnblogs.com/images/copycode.gif)

（2）scopetable_entry结构体

```
struct scopetable_entry
{
     DWORD     prev_entryindex;//指向前一个scopetable_entry在scopetable中的索引
     FARPROC   lpfnFilter; //对应于__except块后小括号内的过滤函数；__finally时为NULL
     FARPROC   lpfnHandler;//__exception或__finally后花括号｛｝内的代码地址。
}
```

（3）VC异常帧堆栈布局及VC默认的异常处理

![img](http://images2015.cnblogs.com/blog/772759/201602/772759-20160227232036409-1943336833.png) 

**23.2.2 数据结构组织**

![img](http://images2015.cnblogs.com/blog/772759/201602/772759-20160227232100237-1976461002.png) 

（1）每个函数只注册一个VC_EXCEPTION_REGISTRATION结构（也叫异常帧，如图中的有5个Frame，即有5个函数调用）。可见，该SEH异常链从链表头部到链表尾共有5节点，分对应于5个异常处理帧。但需注意的是，通过**VC安装的节点为VC_EXCEPTION_REGISTRATION结构**，图中有3个，对应的回调函数为**VCSHE!_exception_handler4(0x00E0178)**，而**系统安装的是EXCEPTION_REGISTRATION结构的帧**，位于链表尾部最后的两个节点，对应的**回调函数分别为ntdll!_exception_handler4（0x77B44FF3）和0x77B50CF5**。

（2）**VC为每个函数内**的所有__try块**建立一个scopetable表**，其中每个__try块对应于scopetable中的一项。（用scopetable_entry结构体来表示这个__try项，结构里分别用lpfnFilter和lpfnHandler来表示__except/__finally的过滤函数和处理函数，其中_finally没有过滤函数，只有异常处理函数）。

（3）若有__try块嵌套，则在scopetable_entry结构里的prev_entryindex或指明，多层嵌套形成单向链表。

（4）对于VC的异常处理，其每个异常帧的回调处理函数都统一设为_except_handler4。每进入一个try块里，编译器会将VC_EXCEPTION_REGISTRATION中tryLevel赋值为相应的值。一旦该try块异常发生，系统会**先从VC_EXCEPTION_REGISTRATION的handler域中找到_exception_handler4函数（C运行时库函数）**，然后根据当前tryLevel的值找到scopetable表中这个__try块相应的过滤函数和处理函数对异常进行相应的处理。

（5）与_except块不同，**_finally块**的lpfnFilter为NULL，即**没有过滤函数**。

**23.2.3 _exception_handler4函数的执行流程**

![img](http://images2015.cnblogs.com/blog/772759/201602/772759-20160227232125034-1220342340.png) 

（1）异常发生时，根据index找到scopetable项，并调用lfpnFilter。如果过滤函数lpfnFilter**返回EXCEPTION_EXECUTE_HANDLER**，则执行**全局展开之后调用lpfnHandler函数**。如果过滤函数lpfnFilter返回**EXCEPTION_CONTINUE_EXECUTION**，则_except_handler4**简单地返回EXCEPTION_CONTINUE_EXECUTION,交由系统恢复线程的执行**。

（2）如果lpfnFilter返回**EXCEPTION_CONTINUE_SEARCH**时，此时_except_handler4查看previndex是否是0xFFFFFFFE,若是则_except_handler4返回ExceptionContinueSearch让系统继续遍历外层SEH链或由系统直接处理。否则_except_handler4根据previndex找到相应的过滤函数，根据其返回值重复上面的动作。直到异常被处理或previndex为0xFFFFFFFE为止。

 【VcSEH程序】演示多层嵌套try块的调用

![img](http://images2015.cnblogs.com/blog/772759/201602/772759-20160227232554768-697596900.png)

![复制代码](http://common.cnblogs.com/images/copycode.gif)

```
/************************************************************************
Module :ExceptFrameInfo.h
Notices:Copyright(c) Microsoft System Journal,February 1997,Matt Pietrek
MSVC 2005之后的编译器开启/GS选项仍可能会回滚到SEH3。不过，CRT的代码总是使用SEH4。
************************************************************************/
#pragma  once

#include <windows.h>
#include <stdio.h>

//-------------------------------------------------------------------
// 本程序仅适用于Visual C++，它使用的数据结构是特定于Visual C++的
//-------------------------------------------------------------------
#ifndef _MSC_VER
#error Visual C++ Required (Visual C++ specific information is displayed)
#endif

/////////////////////////////////结构定义////////////////////////////////
//操作系统定义的基本异常帧

struct EXCEPTION_REGISTRATION
{
    EXCEPTION_REGISTRATION* prev;
    FARPROC     handler;
};

//VC++扩展异常帧指向的数据结构
struct scopetable_entry
{
    DWORD previousTryLevel;
    FARPROC  lpfnFilter;  //过滤函数
    FARPROC  lpfnHandler; //异常处理程序实体的地址
};

//VC++使用的扩展异常帧
struct VC_EXCEPTION_REGISTRATION :EXCEPTION_REGISTRATION
{
    scopetable_entry* scopetable;
    int  trylevel;
    int _ebp;
};

//////////////////////////////////////////////////////////////////////////
//原型声明
//__except_handler3是Visual C++运行时库函数，我们想打印出它的地址，但是它的原型
//并没有出现在任何头文件中，所以需要自己声明它。
extern "C" DWORD __security_cookie;
extern "C" int _except_handler4(PEXCEPTION_RECORD,
                                EXCEPTION_REGISTRATION*,
                                PCONTEXT,
                                PEXCEPTION_RECORD);

//////////////////////////////////////////////////////////////////////////
//显示一个异常帧及其相应的scopetable的信息
void ShowSEHFrame(VC_EXCEPTION_REGISTRATION* pVCExcReg){
    BOOL bVcExceptionHandler4 = pVCExcReg->handler == (FARPROC)_except_handler4; //VC的_except_handler4函数
    if (bVcExceptionHandler4){ //VC的_except_handler4函数
        printf("Frame:%08X Handler:%08X prev:%08X Scopetable:%08X\n",
               pVCExcReg, pVCExcReg->handler, pVCExcReg->prev, (DWORD)pVCExcReg->scopetable^__security_cookie);
    } else{
        printf("Frame:%08X Handler:%08X prev:%08X\n",
               pVCExcReg, pVCExcReg->handler, pVCExcReg->prev);
    }


    DWORD  iAddr = (DWORD)pVCExcReg->scopetable ^ __security_cookie;
    //iAddr = 0x77090928;//在我的系统中，这个值为offset ntdll!ResCSegmentValidateHeader+0x118e (77090928)

    //Scopetable前16个字节几SecurityCookie相关的字段，后面才是scopetable_entry项
    scopetable_entry* pScopeTableEntry = (scopetable_entry*)(iAddr + 16);

    for (int i = 0; i <= pVCExcReg->trylevel; i++){

        if (bVcExceptionHandler4){ //VC的_except_handler4函数

            printf("    scopetable[%u] PreTryLevel:%08X filter:%08X __except:%08X\n",
                   i, pScopeTableEntry->previousTryLevel,
                   pScopeTableEntry->lpfnFilter,
                   pScopeTableEntry->lpfnHandler);
        }

        pScopeTableEntry++;
    }
    printf("\n");
}

//////////////////////////////////////////////////////////////////////////
//遍历异常帧的链表，按顺序显示它们的信息
void WalkSEHFrames(void){
    VC_EXCEPTION_REGISTRATION* pVCExcReg;

    //打印出_except_handler4函数的位置
    printf("_except_handler4 is at address:%08X\n", _except_handler4);
    printf("\n");

    //从FS:[0]处获取指向链表头的指针
    __asm mov eax, FS:[0]
        __asm mov[pVCExcReg], EAX

    //遍历异常帧的链表。0xFFFFFFFF标志着链表的结尾
    while (0xFFFFFFFF != (unsigned)pVCExcReg){
        ShowSEHFrame(pVCExcReg);
        pVCExcReg = (VC_EXCEPTION_REGISTRATION*)(pVCExcReg->prev);
    }
}
```

![复制代码](http://common.cnblogs.com/images/copycode.gif)

//VcSEH.cpp

![复制代码](http://common.cnblogs.com/images/copycode.gif)

```
#include <windows.h>
#include <stdio.h>
#include "ExceptionFrameInfo.h"

void test(void){
    int i = 0;
    //A块
    __try{//第1层
        __try{ //第2层
            __try{ //第3层    
                __try{ //第4层
                     i++;
                }__finally {//第4层
                }
            }__except (EXCEPTION_CONTINUE_SEARCH){//第3层
                //这里不会被执行
            }
        }__except (EXCEPTION_CONTINUE_SEARCH){//第2层
            //这里不会被执行
        }
    }
    __except (EXCEPTION_EXECUTE_HANDLER){ //第1层
        //该try块及内层发生异常时，这里被执行
    }

    //B块
    __try{
        WalkSEHFrames();
    }__except (EXCEPTION_CONTINUE_SEARCH){ 
    }
}

int main(){
    __try{
        test();
    }__except (EXCEPTION_EXECUTE_HANDLER){

    }
    return 0;
}
```

![复制代码](http://common.cnblogs.com/images/copycode.gif)

**23.2.4 小结：异常处理流程及全局展开**

**![img](http://images2015.cnblogs.com/blog/772759/201602/772759-20160227232158674-1561336569.png)**

【GlobalUnwind程序】全局展开

![img](http://images2015.cnblogs.com/blog/772759/201602/772759-20160228112837987-412441686.png)

![复制代码](http://common.cnblogs.com/images/copycode.gif)

```c
#include <windows.h>
#include <stdio.h>

int MyExceptionFilter(int tryLevel,DWORD dwErrorCode){
    printf("第%d层的过滤函数被执行,错误码:%d！\n", tryLevel, dwErrorCode);

    //最外层里返回同意处理；否则，继续查找。
    return (tryLevel == 1) ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH;
}

void test(void){
    int i = 0;
    //A块
    __try{//第1层
        __try{ //第2层
            __try{ //第3层    
                __try{ //第4层
                    RaiseException(999, 0, 0, NULL); //抛出一个异常
                }
                __finally {//第4层
                    printf("最4层的_finally块被执行!\n");
                }
            }
            __except (MyExceptionFilter(3, GetExceptionCode())){//第3层
                //这里不会被执行
                printf("最3层的_except块被执行!\n");
            }
        }
        __except (MyExceptionFilter(2, GetExceptionCode())){//第2层
            //这里不会被执行
            printf("最2层的_except块被执行!\n");
        }
    }
    __except (MyExceptionFilter(1, GetExceptionCode())){ //第1层
        //该try块及内层发生异常时，这里被执行
        printf("最1层的_except块被执行!\n");
    }

    //B块
    __try{

    }
    __except (EXCEPTION_CONTINUE_SEARCH){
        printf("B块中的_except被执行!\n");
    }
}

int main(){
    __try{
        test(); //test()函数内部会处理异常，所以main中的_except块后的代码不会被执行！
    }
    __except (EXCEPTION_EXECUTE_HANDLER){
        printf("main中的_except被执行!\n");

    }
    system("pause");
    return 0;
}
```

![复制代码](http://common.cnblogs.com/images/copycode.gif)

【参考文献】

深入解析结构化异常处理(SEH)

http://www.cppblog.com/weiym/archive/2015/02/27/209884.html

http://blog.csdn.net/bad_sheep/article/details/5803649

http://blog.csdn.net/yuzl32/article/details/5383542

http://www.mouseos.com/windows/index.html

Windows系统程序设计之结构化异常处理

http://bbs.pediy.com/showthread.php?threadid=32222

《软件加密技术内幕》，看雪学院





# （十八） SEH结构化异常 三

2017年02月08日 19:47:06 [wangpengk7788](https://me.csdn.net/wangpengk7788) 阅读数：224



**23.3 终止处理程序**

**23.3.1 程序的结构**

（1）框架



```cpp
__try{

   //被保护的代码块
       ……
}
__finally{

    //终止处理

}
```

![复制代码](http://common.cnblogs.com/images/copycode.gif)

（2）__try/__finally的特点

　　①finally块总是保证，无论__try块中的代码有无异常，finally块总是被调用执行。

　　②try块后面只能跟一个finally块或except块，要跟多个时只能用嵌套，但__finally块不可以再嵌套SEH块，except块中可以嵌套SEH块。

　　③利用try/finally可以使代码的逻辑更清楚，在try块中完成正常的逻辑，finally块中完成清理工作，使代码可读性更强，更容易维护。

（3）__finally块

　　①当指令从__try块底部**自然流出**时，会执行finally块

　　②**局部展开**时：从try块中提前退出（由goto、longjump、continue、break、return等语句引发）将程序控制流强制转入finally块，这时就会进行局部展开（但ExitProcess、ExitThread、TerminateProcess、TerminateThread等原因导致的提前离开除外，因为这会直接终止线/进程，而不能展开）。说白了，局部展开就是将__finally块的代码提前到上述那几种语句之前执行。

　　③**全局展开**时也会引发finally块的执行。**从Vista开始，须显示保护try/finally框架，以确保抛出异常时finally会被执行**。即try/finally块外面的某层要使用try/except块保护且except中的过滤函数要返回EXCEPTION_EXECUTE_HANDLER。Vista以前的Windows，会在线程的入口点处用try/except加以保护，但Vista为了提高Windows错误报告（WER）记录的可靠性，将这个入口点的异常过滤程序返回为EXCEPTION_CONTINUE_SEARCH，最后进程会被终止，从而导致finally块没有机会被执行。（关于全局展开见第24章的相关的部分）。

　　④如果异常发生在异常过滤程序里，终止处理程序也不会被执行。

　　⑤finally块被执行的原因总是由以上三种情况之一引起。可以调用AbnormalTermination函数来查看原因。该函数是个内联函数，当正常流出时会返回FALSE，局部或全局展开时返回TRUE。

**23.3.2 __leave关键字**

　　①该**关键字只能用在try/finally框架中**，它会导致代码执行控制流跳转到到try块的结尾，也可以认为是try后的闭花括号处。

　　②这种情况下，代码执行是正常从try块进入finally，所以**不会进行局部展开**。

　　③但一般需定义一个布尔变量，指令离开try块时，函数执行的结果是成功还是失败，然后在finally块中可根据这个（或这些）变量以决定资源是否需要释放。

23.3.3 局部展开实例分析

![复制代码](http://common.cnblogs.com/images/copycode.gif)

```
//1、Funcenstein2:return引起的局部展开
DWORD Funcenstein2(){
    DWORD dwTemp;

    //1.处理一些其他事情
    //...

    _try{
        //2.
        dwTemp = 5;

        //返回一个新的值。因执行return会提前结束try块会导致
        //局部展开，此时会先保存dwTemp，然后局部展开，执行finally，
        //最后返回try块来执行return。即，将finally提前到return之前执行！
        return (dwTemp);
    }
        __finally{
        //3.清除工作，这里简单输出一行文本
        printf("Finally块被执行！\n");
    }

    //继续处理——以下这些代码将永远不会被执行
    dwTemp = 9;
    printf("dwTemp = %d\n", dwTemp);
}

//2、Funcenstein3:goto引起的局部展开
DWORD Funcenstein3(){
    DWORD dwTemp;

    //1.处理一些其他事情
    //...

    _try{
        //2.
        dwTemp = 5;

        //试图利用goto跳过finally。这也会导致局部展开。
        //当遇到goto时，会先局部展开，先执行finally，
        //最后返回try块来执行goto语句。即，将finally提前到goto之前执行！
        goto ReturnValue;
    }
        __finally{
        //3.清除工作，这里简单输出一行文本
        printf("Finally块被执行！\n");
    }

    //继续处理——以下这些代码将永远不会被执行
    dwTemp = 9; //该行会被跳过

ReturnValue:
    printf("dwTemp = %d\n", dwTemp);
    return (dwTemp);
}

//3.FuncaDoodleDoo：Continue、Break引起的局部展开
DWORD FuncaDoodleDoo(){
    DWORD dwTemp = 0;
    while (dwTemp <10){
        __try{
            //第2次循环时，dwTemp会等于2。由于continue会导致局部展开，
            //所以在执行continue前先执行finally块，dwTemp增到3
            //然后continue，进行第3次循环
            if (dwTemp == 2)
                continue;

            //第3次循环，这时dwTemp等于3。由于break会导致提前离开try块
            //所以进行局部展开，在break前先调用finally块，因此dwTemp增
            //到4，然后break跳出循环，所以finally块外面的dwTemp++没被执行
            if (dwTemp == 3)
                break;
        }
        __finally{
            dwTemp++;
        }

        dwTemp++;
    }
    dwTemp += 0;
    return (dwTemp);
}

//4.Funcenstein4函数：finally块内执行return
DWORD Funcenstein4(){
    DWORD dwTemp;

    //1.处理一些其他事情
    //...

    _try{
        //2.
        dwTemp = 5;

        //返回一个新的值。因执行return会提前结束try块会导致
        //局部展开，此时会先保存dwTemp，然后局部展开，执行finally，
        //最后返回try块来执行return。即，将finally提前到return之前执行！
        return (dwTemp);
    }
        __finally{
        //3.清除工作，这里简单输出一行文本
        printf("Finally块被执行！\n");
        //return (103); //在finally块中提前return这种行为是未定义行为。
                        //VC编译器直接报错。但有些编译器会让通过。
                        //此时在局部展开时，因try块时将返回值复制到一个
                        //临时变量，在等待这里的finally块返回。但因finally
                        //里面又要return，所以会将103写入那个临时变量。然后
                        //函数退出！注意，不再回到try块里了。                        
    }

    //继续处理——以下这些代码将永远不会被执行
    dwTemp = 9;
    printf("dwTemp = %d\n", dwTemp);
    return (dwTemp);
}
```

![复制代码](http://common.cnblogs.com/images/copycode.gif)

【SEHTerm程序】

![img](http://images2015.cnblogs.com/blog/772759/201602/772759-20160229212831392-95256001.png)![img](http://images2015.cnblogs.com/blog/772759/201602/772759-20160229212842064-437752618.png)![img](http://images2015.cnblogs.com/blog/772759/201602/772759-20160229212850955-1647809241.png)![img](http://images2015.cnblogs.com/blog/772759/201602/772759-20160229212904767-171269330.png)

 //SEHTerm.cpp

![复制代码](http://common.cnblogs.com/images/copycode.gif)

```
/************************************************************************
Module： SEHTerm.cpp
Notices：Copyright(c) 2008 Jeffrey Richter & Christophe Nasarre
************************************************************************/

//#include "../../CommonFiles/CmnHdr.h"
#include <windows.h>
#include <tchar.h>

//////////////////////////////////////////////////////////////////////////
//判断操作系统是否是Vista及以上版本
BOOL IsWindowsVistaAbove(){
    //第4章的代码
    //准备一个OSVERSIONINFOEX结构
    OSVERSIONINFOEX osver = { 0 };
    osver.dwOSVersionInfoSize = sizeof(osver);
    osver.dwMajorVersion = 6;
    osver.dwMinorVersion = 0;
    osver.dwPlatformId = VER_PLATFORM_WIN32_NT;

    //准备条件掩码
    DWORDLONG dwlConditionMask = 0; //必须初始化为0
    VER_SET_CONDITION(dwlConditionMask, VER_MAJORVERSION, VER_GREATER_EQUAL); //Vista及以上
    VER_SET_CONDITION(dwlConditionMask, VER_MINORVERSION, VER_EQUAL);
    VER_SET_CONDITION(dwlConditionMask, VER_PLATFORMID, VER_EQUAL);

    //测试,返回TRUE时表示正好是Vista，否则不是Vista
    return VerifyVersionInfo(&osver, VER_MAJORVERSION | VER_MINORVERSION |
                             VER_PLATFORMID, dwlConditionMask);
}

//////////////////////////////////////////////////////////////////////////
void TriggerException(){
    __try{
        int n;

        int n = MessageBox(NULL, TEXT("执行非法内存访问吗？"),
                           TEXT("SEHTerm：在try块内"), MB_YESNO);
        if (n == IDYES){
            *(PBYTE)NULL = 5; //这将引发非法访问内存        
        }
    }
    __finally{
        PCTSTR psz = AbnormalTermination() ?TEXT("异常结束") : TEXT("正常结束");
        MessageBox(NULL, psz, TEXT("SEHTerm：在finally块内"), MB_OK);
    }

    MessageBox(NULL, TEXT("函数正常结束！"), 
               TEXT("SEHTerm：在finally块之后"), MB_OK);
}

//////////////////////////////////////////////////////////////////////////
int WINAPI _tWinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nShowCmd){
    
    //在Vista里，如果过滤函数返回EXCEPTION_EXECUTE_HANDLER时，会引发
    //全局展开。如果有一个未处理异常发生，进程会简单地结束从而导致
    //finally块不会被执行。

    if (IsWindowsVistaAbove()){
        DWORD n = MessageBox(NULL, TEXT("要用try/except保护吗？"),
                             TEXT("SEHTerm：工作流"), MB_YESNO);
        //DWORD n = IDYES;
        if (n == IDYES){ //代码要保护
            __try{
                TriggerException();
            }
            __except (EXCEPTION_EXECUTE_HANDLER){
                //因为代码被保护，系统对话框不会显示
                //所以弹出一个消息框
                MessageBox(NULL, TEXT("进程异常结束"),
                           TEXT("main函数try/except处理器"), MB_OK);

                //进程退出码
                return (-1); 
            }
        } else{  //代码不保护
            TriggerException();
        }
    } else{
        TriggerException();//其中系统默认是保护的。
    }

    MessageBox(NULL, TEXT("进程正常结束"), 
               TEXT("SEHTerm：即将退出主线程！"),MB_OK);
    return 0;
}
```

![复制代码](http://common.cnblogs.com/images/copycode.gif)

//resource.h

![复制代码](http://common.cnblogs.com/images/copycode.gif)

```
//{{NO_DEPENDENCIES}}
// Microsoft Visual C++ 生成的包含文件。
// 供 23_SEHTerm.rc 使用
//
#define IDI_SEHTERM                     101

// Next default values for new objects
// 
#ifdef APSTUDIO_INVOKED
#ifndef APSTUDIO_READONLY_SYMBOLS
#define _APS_NEXT_RESOURCE_VALUE        102
#define _APS_NEXT_COMMAND_VALUE         40001
#define _APS_NEXT_CONTROL_VALUE         1001
#define _APS_NEXT_SYMED_VALUE           101
#endif
#endif
```

![复制代码](http://common.cnblogs.com/images/copycode.gif)

//SEHTerm.rc

![复制代码](http://common.cnblogs.com/images/copycode.gif)

```c
// Microsoft Visual C++ generated resource script.
//
#include "resource.h"

#define APSTUDIO_READONLY_SYMBOLS
/////////////////////////////////////////////////////////////////////////////
//
// Generated from the TEXTINCLUDE 2 resource.
//
#include "winres.h"

/////////////////////////////////////////////////////////////////////////////
#undef APSTUDIO_READONLY_SYMBOLS

/////////////////////////////////////////////////////////////////////////////
// 中文(简体，中国) resources

#if !defined(AFX_RESOURCE_DLL) || defined(AFX_TARG_CHS)
LANGUAGE LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED

#ifdef APSTUDIO_INVOKED
/////////////////////////////////////////////////////////////////////////////
//
// TEXTINCLUDE
//

1 TEXTINCLUDE 
BEGIN
    "resource.h\0"
END

2 TEXTINCLUDE 
BEGIN
    "#include ""winres.h""\r\n"
    "\0"
END

3 TEXTINCLUDE 
BEGIN
    "\r\n"
    "\0"
END

#endif    // APSTUDIO_INVOKED


/////////////////////////////////////////////////////////////////////////////
//
// Icon
//

// Icon with lowest ID value placed first to ensure application icon
// remains consistent on all systems.
IDI_SEHTERM               ICON                    "SEHTerm.ico"
#endif    // 中文(简体，中国) resources
/////////////////////////////////////////////////////////////////////////////



#ifndef APSTUDIO_INVOKED
/////////////////////////////////////////////////////////////////////////////
//
// Generated from the TEXTINCLUDE 3 resource.
//


/////////////////////////////////////////////////////////////////////////////
#endif    // not APSTUDIO_INVOKED
```

![复制代码](http://common.cnblogs.com/images/copycode.gif)

 









# （十九） SEH结构化异常处理_未处理异常及向量化异常

2017年02月13日 19:49:11 [wangpengk7788](https://me.csdn.net/wangpengk7788) 阅读数：1588



UnhandledExceptionFilter函数详解

 **BaseProcessStart伪代码（Kernel32内部）**



```
void BaseProcessStart(PVOID lpfnEntryPoint) //参数为线程函数的入口地址
{
     DWORD retValue;
     DWORD currentESP;
     DWORD exceptionCode;
     currentESP = ESP; 

     //lpfnEntryPoint被try/except封装着，这是系统安装的默认的异常处理程序，也是SEH链上最后一个异常处理程序
     __try
     {
         NtSetInformationThread(GetCurrentThread(),
                                    ThreadQuerySetWin32StartAddress,
                                    &lpfnEntryPoint,
                                    sizeof(lpfnEntryPoint));

         retValue = lpfnEntryPoint();
         ExitThread(retValue); //如果异常，线程从这里退出！
     }
     __except (   //过滤器表达式代码
                exceptionCode = GetExceptionInformation(),
                UnhandledExceptionFilter(GetExceptionInformation())) //出现异常会调用Unhandled...这个函数，该函数内部会调用
                                                                     //用户通过SetUnhandledFilter设置的全局异常处理函数。                     
     {
         //如果UnhandledExceptionFilter返回EXCEPTION_EXECUTE_HANDLER，则会控制流会执行到这里
         ESP = currentESP;

         if (!_BaseRunningInServerProcess) //普通进程，则退出进程
              ExitProcess(exceptionCode);
         else // 线程是作为服务来运行的，只退出线程并不终止整个服务
              ExitThread(exceptionCode);
     }
}
```

![复制代码](http://common.cnblogs.com/images/copycode.gif)

（1）如果异常过滤程序返回EXCEPTION_CONTINUE_SEARCH时，系统会继续向外层寻找异常过滤程序。但如果每个异常过滤程序都返回EXCEPTION_CONTINUE_SEARCH时，会未到遇处理异常。

（2）调用SetUnhandledExceptionFilter安装用户提供的全局**（顶层）异常过滤回调函数**（为所有线程共享）。**如果顶层异常回调函数返回EXCEPTION_EXECUTE_HANDLER或EXCEPTION_CONTINUE_SEARCH则直接传递给UnhandledExceptionFilter函数，UnhandledExceptionFilter根据这个返回值判断是终止进程还是重新执行异常代码**。如果**顶层异常回调函数返回EXCEPTION_CONTINUE_SEARCH**，则接下来的要发生的事情就比较复杂（可**参考后面的《UnhandledExceptionFilter内部工作流程**》）

（3）SetUnhandledExceptionFilter返回值为上次安装的异常过滤程序的地址。**如果使用C/C++运行库，则会默认安装一个__CxxUnhandledExceptionFilter过滤程序**。该函数**首先检查异常是不是C++异常**，如果是则在结束时执行abort函数（该函数内部调用了UnhandledExceptionFilter函数，注意这可能会造成循环调用，因为UnhandledExceptionFilter内部调用了我们安装的全局异常过滤函数_CxxUnhandledExceptionFilter，而这个函数的内部又调用UnhandledExceptionFilter，为了防止无限递归调用，_CxxUnhandledExceptionFilter在调用UnhandledExceptionFilter之前会调用SetUnhandledExceptionFilter(NULL)）。**如果不是C++异常则返回EXCEPTION_CONTINUE_SEARCH**。所以当我们调用SetUnhandled*函数，返回的地址为_CxxUnhandledExceptionFilter的地址。

（4）注意，**在我们的顶层异常过滤函数里，在返回EXCEPTION_CONTINUE_SEARCH前，不应调用之前的全局异常过滤函数**（即我们通过SetUnhandledExceptionFilter的返回值取得的那个函数）。因为如果这个函数是在某个动态库里，那它随时都可能被卸载了。

（5）如果SetUnhandledExceptionFilter(NULL)，则取消我们设置的全局异常过滤函数。

【UnhandledExceptionFilter程序】演示设置顶层异常过滤函数

![img](http://images2015.cnblogs.com/blog/772759/201603/772759-20160306094735237-698076932.png)

![复制代码](http://common.cnblogs.com/images/copycode.gif)

```c
#include <tchar.h>
#include <windows.h>
#include <locale.h>

LONG WINAPI MyUnhandledExceptionFilter(
struct _EXCEPTION_POINTERS *lpTopLevelExceptionFilter)
{
    _tprintf(_T("发生未处理异常\n"));
    _tsystem(_T("PAUSE"));
    return EXCEPTION_EXECUTE_HANDLER; //这样返回，进程将被终止。
}

int _tmain()
{
    SetUnhandledExceptionFilter(MyUnhandledExceptionFilter); //安装用户自定义的未处理异常

    _tsetlocale(LC_ALL, _T("chs"));
    __try{
        //SetErrorMode(SEM_NOGPFAULTERRORBOX);

        *(int*)0 = 5;//引发异常
    }
    __except (EXCEPTION_CONTINUE_SEARCH){ //这里返回EXCEPTION_CONTINUE_SEARCH,异常就会到达MyUnhandled*
        
    }
    
    _tsystem(_T("PAUSE"));//这行不会被执行！
    return 0;
}
```

![复制代码](http://common.cnblogs.com/images/copycode.gif)

**25.1.2 UnhandledExceptionFilter内部工作流程**

　　①判断是否因为对资源进行写入操作引发的异常。如果是，将资源的只读属性改为可写入，并返回EXCEPTION_CONTINUE_EXECUTION以允许失败的指令再次执行。

　　②确定进程是否被调试。如果被调试，就返回EXCEPTION_CONTINUE_SEARCH给调试器，通知调试器定位异常指令，并告知我们出了什么样的异常。

　　③调用我们设置的顶层异常过滤函数（如果存在的话）。如果顶层过滤函数返回EXCEPT_EXECUTE_HANDLER或EXCEPTION_CONTINUE_EXECUTION，将直接传递给UnhandledExceptionFilter，由它将返回值给系统。如果返回EXCEPT_CONTINUE_SEARCH，则跳到第④步。

　　④再次将未处理异常报告给调试器

　　⑤终止进程：如果线程调用SetErrorMode并设置SEM_NOGPFAULTERRORBOX标志，那么UnhandledExceptFilter会返回EXCEPTION_EXECUTE_HANDLER，在未处理异常的情况下进行全局展开并执行未执行的finally块，然后进程终止。

如果没有调用SetErrorMode函数，UnhandledExceptionFilter会返回EXCEPTION_CONTINUE_SEARCH。于是系统内核得到程序控制，它将通过ALPC（高级本地过程调用）机制将异常通知给WerSvc（Windows错误报告专用服务），然后ALPC先阻塞自己的线程，直到WerSvc执行完毕。

　　⑥UnhandledExceptionFilter与WER的交互

![img](http://images2015.cnblogs.com/blog/772759/201603/772759-20160306001230518-671759401.png)      ![img](http://images2015.cnblogs.com/blog/772759/201603/772759-20160306001239237-88976077.png)

当WerSvc接到通知时，会先调用CreateProcess来启动WerFault.exe，然后 WerSvc会等待这个新进程的结束。而WerFault.exe会向我们创建上面的两个对话框以报告错误的发生。当第1个对话框出现时，可以选择“取消”来终止我们的应用程序，否则过一会儿，会弹出第2个对话框，如果我们选择“关闭程序”，则WerFault.exe会调用TerminateProcess来结束我们的应用程序。如果选择“调试”，WerFault.exe会创建一个子进程（调试器），让他附着在出错的程序上进行“即时调试”

**25.2 即时调试**

（1）默认调试器：HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\AeDebug子项下有一个名为Debugger的值，系统通过个值找到调试器。

（2）WerFault.exe会给这个调试器传入两个参数：要调试的进程ID和继承过来的事件句柄（这个句柄由WerSvc服务创建用于通知被调试进程调试也结束）

（3）通过将调试器附着到被调试进程，可以查看全局、局部和静态变量的值，也可以设置断点，检查函数调用树等调试工作。

【Spreadsheet程序】通过SEH向预订的地址空间稀疏调拨存储器

 ![img](http://images2015.cnblogs.com/blog/772759/201603/772759-20160308230116054-1053650065.png)

![复制代码](http://common.cnblogs.com/images/copycode.gif)

```
/************************************************************************
Module： Spreadsheet.cpp
Notices：Copyright(c) 2008 Jeffrey Richter & Christophe Nasarre
************************************************************************/
#include "../../CommonFiles/CmnHdr.h"
#include "resource.h"
#include "VMArray.h"
#include <tchar.h>
#include <strsafe.h>

//////////////////////////////////////////////////////////////////////////
HWND g_hWnd; //全局的窗口句柄，SEH报告中会用到

const int g_nNumRows = 256;
const int g_nNumCols = 1024;

//声明单个单元格内容的结构体，每个单元格大小为1024字节
typedef struct{
    DWORD dwValue;
    BYTE bDummy[1020];
}CELL,*PCELL;

//声明全个电子表格的数据
////SPREADSHEET类型为一个数组类型，元素类型为CELL及g_nNumRows行g_nNumCols列。
//判读时，可去掉typedef来看。
typedef CELL SPREADSHEET[g_nNumRows][g_nNumCols];
typedef SPREADSHEET* PSPREADSHEET;

//////////////////////////////////////////////////////////////////////////
//一个电子表格是一个二维数组的CELLs
class CVMSpreadsheet :public CVMArray<CELL>{
public:
    CVMSpreadsheet() :CVMArray<CELL>(g_nNumRows*g_nNumCols){}

private:
    LONG OnAccessViolation(PVOID pvAddressTouched, BOOL bAttemptedRead, PEXCEPTION_POINTERS pep, BOOL bRetryUntilSuccessful);
};

//////////////////////////////////////////////////////////////////////////
LONG CVMSpreadsheet::OnAccessViolation(PVOID pvAddressTouched, BOOL bAttemptedRead, PEXCEPTION_POINTERS pep, BOOL bRetryUntilSuccessful){
    
    TCHAR sz[200];
    StringCchPrintf(sz, _countof(sz), TEXT("非法访问：试图在0x%8X进行%s操作！"),pvAddressTouched,
                    bAttemptedRead ? TEXT("读取") : TEXT("写入"));

    SetDlgItemText(g_hWnd, IDC_LOG, sz);

    LONG lDispostion = EXCEPTION_EXECUTE_HANDLER;

    //只有写入操作发生异常时才会提交物理存储器，读取操作则不会
    if (!bAttemptedRead){
        //返回基类的返回值
        lDispostion = CVMArray<CELL>::OnAccessViolation(pvAddressTouched,
                                                        bAttemptedRead, pep, bRetryUntilSuccessful);
    }
    return (lDispostion);
}

//////////////////////////////////////////////////////////////////////////
//产生一个全局CVMSpreadsheet对象
static CVMSpreadsheet g_ssObject;

//创建一个全局指针，指向电子表格的区域
SPREADSHEET& g_ss = *(PSPREADSHEET)(PCELL)g_ssObject;

//////////////////////////////////////////////////////////////////////////
BOOL Dlg_OnInitDialog(HWND hWnd, HWND hWndFocus, LPARAM lParam){

    chSETDLGICONS(hWnd, IDI_SPREADSHEET);

    g_hWnd = hWnd; //保存句柄（For SEH错误报告）

    //设置对话框上面控件的默认值
    Edit_LimitText(GetDlgItem(hWnd, IDC_ROW), 3);
    Edit_LimitText(GetDlgItem(hWnd, IDC_COLUMN), 4);
    Edit_LimitText(GetDlgItem(hWnd, IDC_VALUE), 7);

    SetDlgItemInt(hWnd, IDC_ROW, 100, FALSE);
    SetDlgItemInt(hWnd, IDC_COLUMN, 100, FALSE);
    SetDlgItemInt(hWnd, IDC_VALUE, 12345, FALSE);

    return (TRUE);
}

//////////////////////////////////////////////////////////////////////////
void Dlg_OnCommand(HWND hWnd, int id, HWND hwndCtrl, UINT codeNotity){
    int nRow, nCol;
    
    switch (id)
    {
    case IDCANCEL:
        EndDialog(hWnd, id);
        break;

    case IDC_ROW:
        //用户修改了行数，更新UI
        nRow = GetDlgItemInt(hWnd, IDC_ROW, NULL, FALSE);
        EnableWindow(GetDlgItem(hWnd, IDC_READCELL), chINRANGE(0, nRow, g_nNumRows - 1));
        EnableWindow(GetDlgItem(hWnd, IDC_WRITECELL), chINRANGE(0, nRow, g_nNumRows - 1));
        break;

    case IDC_COLUMN:
        //用户修改了行数，更新UI
        nCol = GetDlgItemInt(hWnd, IDC_COLUMN, NULL, FALSE);
        EnableWindow(GetDlgItem(hWnd, IDC_READCELL), chINRANGE(0, nCol, g_nNumCols - 1));
        EnableWindow(GetDlgItem(hWnd, IDC_WRITECELL), chINRANGE(0, nCol, g_nNumCols - 1));
        break;

    case IDC_READCELL:
        //尝试从用户选择的单元格中读取一个数据
        SetDlgItemText(hWnd, IDC_LOG, TEXT("没有发生异常！"));
        nRow = GetDlgItemInt(hWnd, IDC_ROW, NULL, FALSE);
        nCol = GetDlgItemInt(hWnd, IDC_COLUMN, NULL, FALSE);
        __try{
            SetDlgItemInt(hWnd, IDC_VALUE, g_ss[nRow][nCol].dwValue, FALSE);
        }
        //ExceptionFilter返回EXECUTION_CONTINUE_EXECUTE或EXCEPTION_EXECUTE_HANDLER
        //如果提交成功，返回前者；失败，返回后者
        __except (g_ssObject.ExceptionFilter(GetExceptionInformation(),FALSE)){
            //单元格不支持存储，里面不含内容
            SetDlgItemText(hWnd, IDC_VALUE, TEXT(""));

        }
        break;

    case IDC_WRITECELL:
        //尝试向用户选择的单元格中写入数据
        SetDlgItemText(g_hWnd, IDC_LOG, TEXT("没有发生异常！"));
        nRow = GetDlgItemInt(hWnd, IDC_ROW, NULL, FALSE);
        nCol = GetDlgItemInt(hWnd, IDC_COLUMN, NULL, FALSE);


        //假如单元格不支持存储，将抛出非法内存访问，这时将导致自动提交存储器
        //这里不设置try/except，则异常会让全局异常（未处理）过滤函数捕获
        g_ss[nRow][nCol].dwValue = GetDlgItemInt(hWnd, IDC_VALUE, NULL, FALSE);

        break;
    }
}

//////////////////////////////////////////////////////////////////////////
INT_PTR WINAPI Dlg_Proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
    switch (uMsg)
    {
        chHANDLE_DLGMSG(hWnd, WM_INITDIALOG, Dlg_OnInitDialog);
        chHANDLE_DLGMSG(hWnd, WM_COMMAND, Dlg_OnCommand);
    }
    return (FALSE);
}

//////////////////////////////////////////////////////////////////////////
int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nShowCmd){
    DialogBox(hInstance, MAKEINTRESOURCE(IDD_SPREADSHEET), NULL, Dlg_Proc);
    return (0);
}
```

![复制代码](http://common.cnblogs.com/images/copycode.gif)

//VMArray.h

![复制代码](http://common.cnblogs.com/images/copycode.gif)

```
/************************************************************************
 Module： VMArray.h
 Notices：Copyright(c) 2008 Jeffrey Richter & Christophe Nasarre
 ************************************************************************/
#pragma  once
#include "../../CommonFiles/CmnHdr.h"
#include <tchar.h>


#ifndef _M_IX86
#error "The following code only works for x86!"
#endif

//////////////////////////////////////////////////////////////////////////
//注意：这个C++类是线程不安全的。不能在多线程下同时创建和销毁该类的实例

//但是一旦创建，多线程可同时访问不同的CVMArray对象，也可以通过自己同步的
//方法在多线程下访问同一个CVMArray对象
//////////////////////////////////////////////////////////////////////////

template <class TYPE>
class CVMArray{
public:
    //为数组各元素预订稀疏的地址空间
    CVMArray(DWORD dwreservElements);

    //释放
    virtual ~CVMArray();

    //允许访问数组中的一个元素
    operator TYPE*(){ return (m_pArray); }
    operator const TYPE*()const { return (m_pArray); }

    //若提交失败，可以被优雅的处理
    LONG ExceptionFilter(PEXCEPTION_POINTERS pep, BOOL bRetryUntilSuccessful = FALSE);

protected:
    //虚函数，当非法访问内存时，可优雅的处理
    virtual LONG OnAccessViolation(PVOID pvAddressTouched, BOOL bAttemptedRead,
                                   PEXCEPTION_POINTERS pep, BOOL bRetryUntilSuccessful);
private:
    static CVMArray* sm_pHead;  //第一个CVMArray对象
    CVMArray* m_pNext;          //下一个VCMArray对象
    TYPE* m_pArray;  //指向一个预订的区域数组
    DWORD m_cbReserve; //预订的数组空间的大小

private:
    //访问前一个未处理异常过滤函数
    static PTOP_LEVEL_EXCEPTION_FILTER sm_pfnUnhandledExceptionFilterPrev;

    //当这个类发生异常，调用我们自己的全局异常过滤函数（VS2005以后微软让
    // 对CRT （C 运行时库）的一些与安全相关的代码做了些改动,使得许多错误
    //都不能在SetUnhandledExceptionFilter 捕获到。
    static LONG WINAPI MyUnhandledExceptionFilter(PEXCEPTION_POINTERS pep);
    void DisableSetUnhandledExceptionFilter();//使SetUnhandledExceptionFilter函数失效

    //为了达到设置全局异常过滤函数的目的，用向量化
    //AddVectoredContinueHandler来达到设置全局异常过滤函数相同的功能
    //static LONG WINAPI LastVEHandler(PEXCEPTION_POINTERS pep);
    //static PVOID sm_pVEH;


};

//////////////////////////////////////////////////////////////////////////
//向量化异常过滤函数句柄
//template <class TYPE>
//PVOID CVMArray<TYPE>::sm_pVEH = NULL;

//CVMArray对象链表的头
template <class TYPE>
CVMArray<TYPE>* CVMArray<TYPE>::sm_pHead = NULL;

//前一个全局异常过滤函数
template <class TYPE>
PTOP_LEVEL_EXCEPTION_FILTER CVMArray<TYPE>::sm_pfnUnhandledExceptionFilterPrev;


//////////////////////////////////////////////////////////////////////////
template <class TYPE>
CVMArray<TYPE>::CVMArray(DWORD dwreservElements){
    if (sm_pHead == NULL){
        //在创建第1个对象前，安装我们的全局异常过滤函数
        sm_pfnUnhandledExceptionFilterPrev =
            SetUnhandledExceptionFilter(MyUnhandledExceptionFilter);
        DisableSetUnhandledExceptionFilter();//使SetUnhandledExceptionFilter失效

        //sm_pVEH = AddVectoredContinueHandler(0, LastVEHandler);
    }

    m_pNext = sm_pHead;  //下一次节点初始化为链表头部
    sm_pHead = this;     //本对象为链表头

    m_cbReserve = sizeof(TYPE)*dwreservElements;

    //预订整个数组大小的一块区域
    m_pArray = (TYPE*)VirtualAlloc(NULL, m_cbReserve, MEM_RESERVE | MEM_TOP_DOWN,
                                   PAGE_READWRITE);
    chASSERT(m_pArray != NULL);
}

//////////////////////////////////////////////////////////////////////////
template <class TYPE>
CVMArray<TYPE>::~CVMArray(){
    //释放数组所占的空间
    VirtualFree(m_pArray, 0, MEM_RELEASE);

    //删除链表
    CVMArray* p = sm_pHead;
    if (p == this){
        sm_pHead = p->m_pNext; //删除链表头
    }else{
        BOOL bFound = FALSE;

        //遍历链头，并修复指针
        for (; !bFound && (p->m_pNext != NULL);p= p->m_pNext){
            if (p->m_pNext == this){
                p->m_pNext = p->m_pNext->m_pNext;
                bFound = TRUE;
                break;
            }
        }
        chASSERT(bFound);
    }

    //if (sm_pVEH != NULL)
    //    RemoveVectoredExceptionHandler(sm_pVEH);
}

//////////////////////////////////////////////////////////////////////////
//当非法访问时，默认的异常处理
template <class TYPE>
LONG CVMArray<TYPE>::OnAccessViolation(PVOID pvAddressTouched, BOOL bAttemptedRead, PEXCEPTION_POINTERS pep, BOOL bRetryUntilSuccessful)
{
    BOOL bCommittedStorage = FALSE; //假定提交失败
    do{
        //提交存储器
        bCommittedStorage = (NULL != VirtualAlloc(pvAddressTouched,
            sizeof(TYPE),MEM_COMMIT,PAGE_READWRITE));

        //假如无法提交而我们又试图重试，提醒用户释放内存
        if (!bCommittedStorage && bRetryUntilSuccessful)
            MessageBox(NULL, TEXT("请关闭一些其他应用，然后按“确定”！"),
            TEXT("内存空间不足"),MB_ICONWARNING | MB_OK);

    } while (!bCommittedStorage && bRetryUntilSuccessful);

    //当提交存储器，重新执行出错代码。否则执行异常处理程序
    return (bCommittedStorage 
                ? EXCEPTION_CONTINUE_EXECUTION : EXCEPTION_EXECUTE_HANDLER);
}

//////////////////////////////////////////////////////////////////////////
//过滤函数被关联到单一的CVMArray对象
template <class TYPE>
LONG CVMArray<TYPE>::ExceptionFilter(PEXCEPTION_POINTERS pep, BOOL bRetryUntilSuccessful /* = FALSE */){
    //默认，提交给其它过滤函数处理（这是一个安全的选择）
    LONG lDispostion = EXCEPTION_CONTINUE_SEARCH;

    //只修改非法访问内存的异常
    if (pep->ExceptionRecord->ExceptionCode != EXCEPTION_ACCESS_VIOLATION)
        return (lDispostion);

    //获取试图访问的地址，以及读或写异常
    //对于EXCEPTION_ACCESS_VIOLATION异常，ExceptionInformation[0]指出非法访问的类型
    //0表示线程试图读取不能访问的数据；1表示写入不能访问的数据
    PVOID pvAddrTouched = (PVOID)pep->ExceptionRecord->ExceptionInformation[1];//非法访问的地址
    BOOL bAttempedRead = (pep->ExceptionRecord->ExceptionInformation[0] == 0); //非法访问的类型

    //如果试图访问的地址在VMArray的预订的地址空间内
    if ((m_pArray <=pvAddrTouched) && (pvAddrTouched<((PBYTE)m_pArray + m_cbReserve))){
        //访问这个数组，并尝试解决问题
        lDispostion = OnAccessViolation(pvAddrTouched, bAttempedRead, pep, bRetryUntilSuccessful);
    }
    return (lDispostion);
}

//////////////////////////////////////////////////////////////////////////
//template <class TYPE>
//LONG  CVMArray<TYPE>::LastVEHandler(PEXCEPTION_POINTERS pep){
//    //默认为让其他过滤器处理
//    LONG lDispostion = EXCEPTION_CONTINUE_SEARCH;
//
//    MessageBox(NULL, TEXT("发生未处理异常"), TEXT("提示"), MB_OK);
//
//    //只修改非法访问内存
//    if (pep->ExceptionRecord->ExceptionCode == EXCEPTION_ACCESS_VIOLATION){
//        //遍历所有链表节点
//        for (CVMArray* p = sm_pHead; p != NULL; p = p->m_pNext){
//            //询问该节点是否可以修复错误
//            //注意：这个错误必须被修复，否进程会被终止
//            lDispostion = p->ExceptionFilter(pep, TRUE);
//
//            //如果修复了错误，就停止循环
//            if (lDispostion != EXCEPTION_CONTINUE_SEARCH)
//                break;
//        }
//    }
//    return (lDispostion);
//}

//新版本的CRT 实现在异常处理中强制删除所有应用程序先前设置的捕获函数，如下所示：
///* Make sure any filter already in place is deleted. */
//SetUnhandledExceptionFilter(NULL);
//UnhandledExceptionFilter(&ExceptionPointers);
//解决方法是拦截CRT 调用SetUnhandledExceptionFilter 函数，使之无效
template <class TYPE>
void CVMArray<TYPE>::DisableSetUnhandledExceptionFilter()
{
    void *addr = (void*)GetProcAddress(LoadLibrary(_T("kernel32.dll")),
                                       "SetUnhandledExceptionFilter");
    if (addr)
    {
        unsigned char code[16];
        int size = 0;
        code[size++] = 0x33;
        code[size++] = 0xC0;
        code[size++] = 0xC2;
        code[size++] = 0x04;
        code[size++] = 0x00;

        DWORD dwOldFlag, dwTempFlag;
        VirtualProtect(addr, size, PAGE_READWRITE, &dwOldFlag);
        WriteProcessMemory(GetCurrentProcess(), addr, code, size, NULL);
        VirtualProtect(addr, size, dwOldFlag, &dwTempFlag);
    }
}

//////////////////////////////////////////////////////////////////////////
//全局异常过滤函数，为所有CVMArray对象共用
//这个未处理异常很有必须，如果用户忘记用try/except来处理此类对象发生的异常，可以在这里
//进行最后的处理！
template <class TYPE>
LONG WINAPI  CVMArray<TYPE>::MyUnhandledExceptionFilter(PEXCEPTION_POINTERS pep){
    //默认为让其他过滤器处理
    LONG lDispostion = EXCEPTION_CONTINUE_SEARCH;

    //只修改非法访问内存
    if (pep->ExceptionRecord->ExceptionCode == EXCEPTION_ACCESS_VIOLATION){
        //遍历所有链表节点
        for (CVMArray* p = sm_pHead; p != NULL;p=p->m_pNext){
            //询问该节点是否可以修复错误
            //注意：这个错误必须被修复，否进程会被终止
            lDispostion = p->ExceptionFilter(pep, TRUE);

            //如果修复了错误，就停止循环
            if (lDispostion != EXCEPTION_CONTINUE_SEARCH)
                break;
        }    
    }

    //如果节点修复错误，试图调用前一个异常处理来处理
    if (lDispostion == EXCEPTION_CONTINUE_SEARCH)
        lDispostion = sm_pfnUnhandledExceptionFilterPrev(pep);

    return (lDispostion);
}

///////////////////////////////////文件结束///////////////////////////////
```

![复制代码](http://common.cnblogs.com/images/copycode.gif)

//resource.h

![复制代码](http://common.cnblogs.com/images/copycode.gif)

```
//{{NO_DEPENDENCIES}}
// Microsoft Visual C++ 生成的包含文件。
// 供 25_Spreadsheet.rc 使用
//
#define IDD_SPREADSHEET                 1
#define IDC_LOG                         101
#define IDI_SPREADSHEET                 102
#define IDI_ICON1                       102
#define IDC_ROW                         1001
#define IDC_COLUMN                      1002
#define IDC_COLUMN2                     1003
#define IDC_VALUE                       1003
#define IDC_READCELL                    1004
#define IDC_WRITECELL                   1005

// Next default values for new objects
// 
#ifdef APSTUDIO_INVOKED
#ifndef APSTUDIO_READONLY_SYMBOLS
#define _APS_NEXT_RESOURCE_VALUE        103
#define _APS_NEXT_COMMAND_VALUE         40001
#define _APS_NEXT_CONTROL_VALUE         1001
#define _APS_NEXT_SYMED_VALUE           101
#endif
#endif
```

![复制代码](http://common.cnblogs.com/images/copycode.gif)

//Spreadsheet.rc

![复制代码](http://common.cnblogs.com/images/copycode.gif)

```c
// Microsoft Visual C++ generated resource script.
//
#include "resource.h"

#define APSTUDIO_READONLY_SYMBOLS
/////////////////////////////////////////////////////////////////////////////
//
// Generated from the TEXTINCLUDE 2 resource.
//
#include "winres.h"

/////////////////////////////////////////////////////////////////////////////
#undef APSTUDIO_READONLY_SYMBOLS

/////////////////////////////////////////////////////////////////////////////
// 中文(简体，中国) resources

#if !defined(AFX_RESOURCE_DLL) || defined(AFX_TARG_CHS)
LANGUAGE LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED

#ifdef APSTUDIO_INVOKED
/////////////////////////////////////////////////////////////////////////////
//
// TEXTINCLUDE
//

1 TEXTINCLUDE 
BEGIN
    "resource.h\0"
END

2 TEXTINCLUDE 
BEGIN
    "#include ""winres.h""\r\n"
    "\0"
END

3 TEXTINCLUDE 
BEGIN
    "\r\n"
    "\0"
END

#endif    // APSTUDIO_INVOKED


/////////////////////////////////////////////////////////////////////////////
//
// Dialog
//

IDD_SPREADSHEET DIALOGEX 18, 18, 164, 165
STYLE DS_SETFONT | DS_CENTER | WS_MINIMIZEBOX | WS_VISIBLE | WS_CAPTION | WS_SYSMENU
CAPTION "Spreadsheet"
FONT 10, "宋体", 400, 0, 0x86
BEGIN
    LTEXT           "单元格大小:\n行数:\n列数:\n总大小:",IDC_STATIC,4,4,56,36
    LTEXT           "1024 字节\n256\n1024\n256 MB (268,435,456 字节)",IDC_STATIC,49,4,104,36
    LTEXT           "行 (0-255):",IDC_STATIC,4,45,50,8
    EDITTEXT        IDC_ROW,60,41,40,14,ES_AUTOHSCROLL | ES_NUMBER
    LTEXT           "列(0-1023):",IDC_STATIC,4,65,54,8
    EDITTEXT        IDC_COLUMN,60,61,40,14,ES_AUTOHSCROLL | ES_NUMBER
    PUSHBUTTON      "&读取单元格",IDC_READCELL,104,61,54,14
    LTEXT           "值:",IDC_STATIC,4,85,21,8
    EDITTEXT        IDC_VALUE,60,81,40,14,ES_AUTOHSCROLL | ES_NUMBER
    PUSHBUTTON      "写入单元格",IDC_WRITECELL,104,81,54,14
    LTEXT           "操作日志:",IDC_STATIC,4,100,48,8
    EDITTEXT        IDC_LOG,4,110,156,52,ES_MULTILINE | ES_AUTOHSCROLL | ES_READONLY
END


/////////////////////////////////////////////////////////////////////////////
//
// DESIGNINFO
//

#ifdef APSTUDIO_INVOKED
GUIDELINES DESIGNINFO
BEGIN
    IDD_SPREADSHEET, DIALOG
    BEGIN
    END
END
#endif    // APSTUDIO_INVOKED


/////////////////////////////////////////////////////////////////////////////
//
// Icon
//

// Icon with lowest ID value placed first to ensure application icon
// remains consistent on all systems.
IDI_SPREADSHEET         ICON                    "Spreadsheet.ico"
#endif    // 中文(简体，中国) resources
/////////////////////////////////////////////////////////////////////////////



#ifndef APSTUDIO_INVOKED
/////////////////////////////////////////////////////////////////////////////
//
// Generated from the TEXTINCLUDE 3 resource.
//


/////////////////////////////////////////////////////////////////////////////
#endif    // not APSTUDIO_INVOKED
```

![复制代码](http://common.cnblogs.com/images/copycode.gif)

**25.3 向量化异常和继续处理程序**

**25.3.1向量化异常（vectored exception handing，VEH）——在SEH前被调用**

　　①对于多层嵌套的SEH来说，外层的__try_except语句块可能没有机会处理被内层嵌套拦截的异常。对于一般软件来说，这不是太大的问题，但是当内层嵌套的软件是第三方的库函数，并且内部以不友好的方式处理了异常，比如：异常退出进程了事，这对整个程序将造成很不利的影响。

　　②此时可以利用向量化异常处理，在正常的SEH之前以合适的方式拦截和处理异常。

　　③当异常发生时，系统在执行SEH过滤程序之前，会先依次调用VEH列表中的每个VEH异常处理函数。

（2）注册VEH异常处理程序:AddVectoredExceptionHandler(bFirstInTheList,pfnHandler)

　　①参数bFirstInTheList为0表示pfnHandler被添加到列表尾端，非0在列表头部。

　　②pfnHandler：异常处理函数，如果返回EXCEPTION_CONTINUE_SEARCH，则重新执行导致异常的指令，如果返回EXCEPTION_CONTINUE_SEARCH表示让VEH链表中的其他函数去处理异常，如果所有函数都返回EXCEPTION_CONTINUE_SEARCH，SEH过滤函数就会被执行。

（3）删除VEH异常处理函数：RemoveVectoredExceptionHandler(pHandler)，其中pHandler这个句柄为AddVectoredExceptionHandler的返回值。

**25.3.2 继续处理程序：——用于实现程序的诊断和跟踪**

（1）安装：PVOID AddVectoredContinueHandler(bFirstInTheList,pfnHandler);

　　①参数bFirstInTheList为0，表示安装在继续处理程序列头的尾部，非0在头部。

　　②通过该函数安装的异常处理程序是在SetUnhandledExceptionFilter安装的异常处理程序返回EXCEPTION_CONTINUE_SEARCH**之后**才被调用。

　　③如果pfnHandler函数返回EXCEPTION_CONTINUE_EXECUTION重新执行导致异常的指令，EXCEPTION_CONTINUE_SEARCH让系统执行它后面的异常处理程序。

（2）删除：RemoveVectoredContinueHandler(pHandler);

 【VectoredExceptionFilter】演示向量化异常过滤函数的调用

![img](http://images2015.cnblogs.com/blog/772759/201603/772759-20160306111513690-339394186.png)

![复制代码](http://common.cnblogs.com/images/copycode.gif)

```
#include <windows.h>
#include <tchar.h>
#include <locale.h>

int g_iVal = 0;

//VEH1异常过滤函数
LONG CALLBACK VEH1(struct _EXCEPTION_POINTERS* pEP){
    _tprintf(_T("VEH1\n"));
    return EXCEPTION_CONTINUE_SEARCH;
}

//VEH2异常过滤函数
LONG CALLBACK VEH2(struct _EXCEPTION_POINTERS* pEP){
    _tprintf(_T("VEH2\n"));
    //以下的注释，可以取消以观察不同的输出结果
    //if ((EXCEPTION_INT_DIVIDE_BY_ZERO == pEP->ExceptionRecord->ExceptionCode)){
    //    g_iVal = 25;
    //    return EXCEPTION_CONTINUE_EXECUTION;
    //}
    return EXCEPTION_CONTINUE_SEARCH;
}

//VEH3异常过滤函数
LONG CALLBACK VEH3(struct _EXCEPTION_POINTERS* pEP){
    _tprintf(_T("VEH3\n"));
    return EXCEPTION_CONTINUE_SEARCH;
}

//SEH异常过滤函数
LONG SEHFilter(PEXCEPTION_POINTERS pEP){
    _tprintf(_T("SEH\n"));
    if ((EXCEPTION_INT_DIVIDE_BY_ZERO == pEP->ExceptionRecord->ExceptionCode)){
        g_iVal = 34;
        return EXCEPTION_CONTINUE_EXECUTION;
    }
    return EXCEPTION_CONTINUE_SEARCH;
}

void Fun1(int iVal){
    __try{
        _tprintf(_T("Fun1 g_iVal = %d iVal = %d\n"), g_iVal, iVal);
        iVal /= g_iVal; //这里发生异常，VEH异常会先被调用！
        _tprintf(_T("Fun1 g_iVal = %d iVal = %d\n"), g_iVal, iVal);
    }
    __except (EXCEPTION_EXECUTE_HANDLER){
        _tprintf(_T("Func1 _except块执行，程序将退出！\n"));
        _tsystem(_T("PAUSE"));
        ExitProcess(1);
    }
}

int _tmain(){
    _tsetlocale(LC_ALL, _T("chs"));

    PVOID pVEH1 = AddVectoredExceptionHandler(0, VEH1);//安装到VEH链表尾部
    PVOID pVEH2 = AddVectoredExceptionHandler(0, VEH2);//安装到VEH链表尾部
    PVOID pVEH3 = AddVectoredExceptionHandler(0, VEH3);//安装到VEH链表尾部

    __try{
        Fun1(g_iVal);
    }
    __except (SEHFilter(GetExceptionInformation())){
        _tprintf(_T("main _except excuted!\n"));
    }

    RemoveVectoredExceptionHandler(pVEH1);
    RemoveVectoredExceptionHandler(pVEH2);
    RemoveVectoredExceptionHandler(pVEH3);
    _tsetlocale(LC_ALL, _T("chs"));

    _tsystem(_T("PAUSE"));
    return 0;
} 
```

![复制代码](http://common.cnblogs.com/images/copycode.gif)

**25.4 C++异常与结构化异常的比较**

（1）框架的差别

```cpp
//C++异常
void ChunkyFunky(){
     try{
         //try块
         ...
         throw 5;
}
     catch (int x){
         //catch块
         ...
     }
     ...
}


//SEH异常
void ChunkyFunky(){
     __try{
         //try块
         //...
         RaiseException(Code = 0xE06D7363,//ASCII的“msc”
                          Flag = EXECEPTION_NONCONTINUABLE,
                          Args = 5);
     }
     __except ((ArgType == Integer)?
               EXCEPTION_EXECUTE_HANDLER:
               EXCEPTION_CONTINUE_SEARCH){
         //Catch块
         ...
     }
     ...
}
```





（2）SEH和C++异常的比较

　　①SEH是操作系统提供的，它在任何语言中都可以使用，而C++异常只有在编写C++代码时才可以使用。

　　②如果开发C++应用程序，应该使用C++异常，而不是SEH异常，因为C++异常是语言的一部分，编译器会自动生成代码来调用C++对象的析构函数，保证对象的释放。

　　③C++异常也是利用操作系统的SEH来实现的，所以在创建一个C++try块时，编译器也会生成一个SEH的__try块。C++的catch语句对应SEH异常过滤程序，catch块中的代码对应SEH __except块中的代码。C++的throw语句也是对RaiseException函数的调用。

　　④C++调用throw抛出异常时，都会自动带EXCEPT_NONCONTINUEABLE标志。这意味着C++不能再次执行错误代码。

　　⑤__except通过比较throw变量的数据类型与C++ catch语句中所用到的变量的数据类型，如果一致，返回EXCEPTION_EXECUTE_HANDLER，让__except块执行。如果不同，返回EXCEPTION_CONTINUE_SEARCH继续向搜索外层的__try/__except。

**25.5 异常与调试器**

（1）首次机会通知和最后机会通知：

当某个线程抛出异常里，操作系统会马上通知调试器（如果调试器己经附着），这个通知被称为“首次机会通知（First-Chance Notification）”调试器将响应这个通知，促使线程寻找异常过滤程序。如果所有的异常过滤程序都返回EXCEPTION_CONTINUE_SEARCH，操作系统会给调试器一个“最后机会通知（Last-Chance Notification）”

（2）每个解决方案（.sln），可以从主菜单“调试”→“异常(x)…”找出“异常”对话框。每个异常代码为32位，如果**勾选“引发”**表示每当被调试线程引发异常时，调试器都会收到首次机会通知，此时**异常刚刚发生，线程还没有得到机会执行异常过滤程序**。调试器会弹出相应的对话框让我们选择操作，我们可以选择调试，然后在代码里设置断点，查看变量的值或线程的函数调用栈，**如果不勾选“引发”这项**，则**不会弹出对话框**，但**调试器收到通知时，会在输出窗口中显示一行文字**，以表示它收到了这个异常通知，**然后允许线程寻找异常过滤程序**。 “用户未处理的”表示异常过滤函数返回EXCEPTION_CONTINUE_SEARCH，调试器收到最后机会通知，也会弹出相应的对话框让用户来选择操作。

 ![img](http://images2015.cnblogs.com/blog/772759/201603/772759-20160306104201971-1306220460.png)

（3）可以自定义软件异常：只需单击“添加”按钮，然后输入异常名称和异常代码（不能与己有的重复）











# （二十） 窗口与消息

2017年02月13日 19:50:43 [wangpengk7788](https://me.csdn.net/wangpengk7788) 阅读数：852



 线程的消息队列

（1）Windows用户对象（User Object）

　　①类型：图标、光标、窗口类、菜单、加速键表等

　　②当一个线程创建某个对象时，则该对象归这个线程的进程所有，当进程结束时，如果用户没有明确删除这个对象，则操作系统会自动删除这个对象。

　　③**窗口和钩子(hook)这两种用户对象**，它们分别由建立窗口和安装钩子的**线程所拥有（注意，不是进程）**。如果一个线程建立一个窗口或安装一个钩子。然后线程结束，操作系统会自动删除窗口或卸载钩子。

　　④窗口及线程的拥有关系使得建立窗口的线程必须为它的窗口处理所有消息。这意味着如果线程建立了一个窗口（或调用一个与图形用户界面有关的函数），系统将对它分配一个消息队列，用来向窗口派送（dispatch）消息。

（2）THREADINFO的内部数据结构（注意它与某一线程相关联）

![img](http://images2015.cnblogs.com/blog/772759/201603/772759-20160319181344553-791571230.png) 

**27.2 将消息投递到线程的消息队列**

**27.2.1 BOOL PostMessage(hwnd,uMsg,wParam,lParam);**

（1）调用该函数时，系统先确定是哪一个线程建立了hwnd窗口，然后分配一块内存，将消息参数存储在这块内存，然后把这块内存增加到对应线程的Posted-Message队列中。

（2）添加到队列后，PostMessage还设置了Wake Flags变量的QS_POSTMESSAGE唤醒位。

（3）消息投递完会函数立即返回。

**27.2.2 BOOL PostThreadMessage(dwThreadId,uMsg,wParam,lParam)**

（1）dwThreadId为目标线程id，当消息被投递出去后，MSG结构体中的hwnd会被自动设为NULL。

（2）目标线程的主消息循环中，当GetMessage（或PeekMessage）获取一条消息后，先检查hwnd是否为NULL，如果是就对这个消息进行一行特殊处理而不发往某个窗口过程，所以就不调用DispatchMessage将消息分派出去。

（3）与PostMessage一样，PostThreadMessage也是立即返回。调用线程无法知道消息是否被处理。

（4）获取创建窗口的线程ID：DWORD GetWindowThreadProcessID(hwnd,pdwProcessId)的返回值即为线程ID。

**27.2.3 void PostQuitMessage(int nExitCode)**

（1）终止消息循环。与PostThreadMessage(GetCurrentThreadId(),WM_QUIT,nExitCode,0)类似（但不相同）。PostQuitMessage并不真正投递消息到队列中去（即并不发送WM_QUIT消息，原因见27.4.3《从线程的队列中提取消息的算法》），而只是设置QS_QUIT标志位和nExitCode退出码。

（2）因为该函数只是设置QS_QUIT标志位和nExitCode，调用总是会成功，所以返回值为void。

**27.3 向窗口发送消息**

**27.3.1 LRESULT SendMessage(hwnd,uMsg,wParam,lParam)；**

（1）只有消息被处理后，SendMessage才会返回。

（2）如果调用SendMessage的线程向该线程所建立的一个窗口发送消息，则SendMessage工作过程很简单，只是直接调用了指定的窗口过程，将其作为一个子函数调用而己。当窗口过程处理完消息后，其返回值直接返回给SendMessage，SendMessage再将其返回给调用线程。

（3）**如果调用SendMessage向其他线程（包含其他进程中的线程）时，其过程较复杂，因为Windows要求窗口过程须由创建该窗口的线程来调用（而不能是其他线程），其工作过程如下：**

　　①当调用SendMessage时，函数会将这条消息**添加到接收线程的“Send-Message”队列**中，这将导致**开启接收线程的QS_SENDMESSAGE标志位**。

　　②如果此时**接收线程正在执行“其他代码”**，那么**这条消息不会被立即处理**。当接收线程开始等待消息时，系统会**先检查QS_SENDMESSAGE标志位是否被设置**。如果被设置，系统会从“Send-Message”队列中找到第一条消息（队列中可能有很多条消息，因为可能有多个线程在同一时间调用SendMessage向这个队列添加消息）。（注意，前面说的“其他代码”不包含调用GetMessage、PeekMeesage或WaitMessage等函数，因为这些函数会让接收线程取出Send-Message队列中的消息并开始处理）

　　③当接收线程从“Send-Message”队列中取出消息并调用相应的窗口过程后，**处理一条消息后，GetMessage并不返回**，而是再转去处理该队列的消息，直到“send-queue”队列中没有其他消息了，**系统就会关闭QS_SENDMESSAGE标志位**。在**接收线程处理消息期间，调用SendMessage的线程（也被称为“调用线程”或“发送线程”）被设置为“空闲”以便等待接收线程发送一条应答消息到自己的“Reply-Message”队列**。当接收线程处理完消息后，会把窗口过程的返回值**投递到发送线程的“Reply-Message”队列**（注意，这个返回值会作为SendMessage的返回值，返回给发送线程），并**唤醒发送线程**。然后发送线程继续正常执行。

　　④当发送线程等待SendMessage返回期间，他通常被设成“空闲”，但他也被允许执行一项任务：假如另一个线程（B）发送一个消息给当前这个线程调用SendMessage的线程（A），A线程必须立即去处理这个消息，而不必调用GetMessage、PeekMessage或WaitMessage等函数。为什么要立即处理？比如A线程SendMessage给B，这时A挂起等待B线程处理完毕，在这期间B也SendMessage给了A，如果A不被立即唤醒去处理B发送过来的消息，两个进程就会进入互相等待的死锁状态）

（4）Windows采用这种方法来处理线程间的SendMessage会导致发送线程挂起，如果这时接收线程出现一个Bug而导致死循环，这时发送线程也就无法被唤醒。这就意味着一个进程的Bug，可能影响到另一个进程。（可以调用以下四个函数来解决这个问题）

**27.3.2 SendMessageTimeOut函数**

| 参数             | 含义                                                         |
| ---------------- | ------------------------------------------------------------ |
| HWND hwnd        |                                                              |
| UINT uMsg        |                                                              |
| WPARAM wParam    |                                                              |
| LPARAM lParam    | 前四个参数与SendMessage含义一样                              |
| UINT fuFlags     | 标志：可以是以下几个的组合①SMTO_NORMAL（0）：不使用任何其他标志，或取消以下几个标志或其组合。②SMTO_ABORTIFHUNG：如果接收线程被挂起时，立即返回③SMTO_BLOCK：一个线程在等待SendMessage*返回时可以被中断，以便处理另一个发送来的消息。使用SMTO_BLOCK标志阻止系统允许这种中断，但可能会造成死锁（原因见SendMessage部分的分析）。④SMTO_NOTIMEOUTIFNOTHUNG：如果接收线程没有被挂起时，不考虑时间限定值。 |
| UINT uTimeOut    | 等待其他线程应答我们发送的消息的时间最大值，单位ms           |
| PDWORD pdwResult | 处理消息的结果。                                             |
| 返回值           | 成功或失败，可用GetLastError获取更多的信息                   |
|                  | 备注：如果调用SendMessageTimeOut向调用线程所建立的窗口发送一个消息，系统只是调用这个窗口的过程，并将返回值赋给pdwResult。因为所有的处理都必须发生在一个线程里，调用该函数之后出现的代码必须等消息被处理完之后才会被执行。 |

**27.3.3 SendMessageCallBack(hwnd,uMsg,wParam,lParam,pfnResultCallBack,dwData);**

（1）调用SendMessageCallBack时，一条**消息会被添加到接收线程的“Send-Message”队列**，然后调用线程立即返回。当**接收线程处理完消息后，会向发送线程的“Reply-Message”队列发送一条应答消息**。然后系统**合适的时候通知发送线程去执行pfnResultCallBack指定的回调函数**。

（2）回调函数的原型：VOID CALLBACK ResultCallBack(hwnd,uMsg,dwData,dwResult); 当调用SendMessageCallBack时，会其将自己的dwData参数直接传给回调函数，dwResult处理消息的窗口过程返回的结果。

（3）在线程间发送消息时，**SendMessageCallBack会立即返回，但回调函数并不立即执行**。即使接收线程处理完消息后，回调函数也不一定立即执行，因为接收线程只是发送个应答给发送线程，告知他处理完了消息。至于发送线程什么时候调用这个回调函数，由发送线程说了算，一般是当发送线程调用GetMessage、PeekMessage、WaitMessage时，消息从“Reply-Message”队列被取出时，回调函数才会被执行。

（4）SendMessageCallBack还有**另一种用法**。Windows提供了一种**广播消息**的方法，可以用系统中所有的重叠（Overlapped）窗口广播消息。虽然可以通过SendMessage，并向hwnd传递HWND_BROADCAST(-1)，但这种方法的广播，其返回值只是一个LRESULT值，并不能查看每个重叠窗口的窗口过程处理后的返回结果。但如果调用SendMessageCallBack，对每个窗口过程处理会后，回调函数都会被调用一次，因此就可以通过dwResult查看返回结果。

（5）如果SendMessageCallBack向一个由调用线程所建立的窗口发送一个消息时，系统会立即调用窗口过程，并且在消息被处理之后，系统再调用回调函数。当回调函数返回之后，系统从调用SendMessageCallBack之后的代码开始执行。

**27.7.4 BOOL SendNotifyMessage(hwnd,uMsg,wParam,lParam);**

（1）SendNotifyMessage将一条消息添加到接收线程的“Send-Message”队列中并立即返回，这有点像PostMessage，**但他与PostMessage有两点不同：**

　　①如果向其他线程的窗口发送消息时， **SendNotifyMessage是向接收线程的“Send-Message”添加消息的**，而**PostMessage是向接收线程的“Posted-Message”队列添加消息的**。在系统的消息机制中， **“Send-Message”队列中的消息总是比“Posted-Message”队列中的消息被优先处理**。

　　②其次，当向调用线程自己创建的窗口发送消息时，SendNotifyMessage的工作就像SendMessage一样，直接调用窗口过程，同时等待窗口过程处理完才返回。

（2）很多消息只是用于通知的目的，用于告知应用程序某个状态发生了改变，所以发送线程没有必要等待接收线程处理完毕才返回。（如系统向窗口发送WM_SIZE、WM_MOVE等，操作系统没有必要停下来等用户处理了这些消息再继续。但有些消息，系统必须等待，如操作系统向一个窗口发送WM_CREATE，必须等待，如果窗口过程返回-1，则系统不建立这个窗口。

**27.3.5 BOOL ReplyMessage(LRESULT lResult)**

（1）接收线程可以在窗口过程还没处理完消息的情况下，**提前**向通过**SendMessage**发送消息的线程的“Reply-Message”添加一个应答消息，这**会唤醒发送线程**。

（2）接收线程把lResult作为消息处理结果传递给发送线程。**当调用ReplyMessage后**，发送线程被提前唤醒。当接收线程真正从窗口过程中返回时，**系统将忽略这个返回值，即不再向发送线程回复本应在窗口过程正常结束才发送的应答消息**。

（3）**ReplyMessage必须在接收消息的窗口过程中被调用**，而不能由某个调用Send*的线程调用，因为他是用来回复调用SendMessage的线程。前面讨论过的3个**SendMessage\*函数会立即返回。**而**SendMessage函数的返回，可以由窗口过程的实现者通过调用ReplyMessage来控制。**

（4）如果消息不是通过SendMessage发送的，或者消息由同一个线程发送，ReplyMessage不起作用。这也是返回值指出的，如果在处理线程间的消息发送时调用了ReplyMessage返回TRUE。处理线程内的消息发送时调用ReplyMessage会返回FALSE。

（5）可以调用InSendMessage(Ex)来确定是线程间的消息发送还是线程内的发送消息。如果是线程间发送的会返回TRUE，线程内Send或Post的会返回FALSE。

【Sending a Message示例】

![复制代码](http://common.cnblogs.com/images/copycode.gif)

```c
//WM_USER+5是由其他线程发送过来的消息。如果是其他进程发送过来的消息，那个进程里要先调用RegisterWindowMessage注册为全局的消息类型
case WM_USER + 5:
    ……//处理些其他事情
    if (InSendMessage())    //是否是线程间发送的消息，只有线程间的才能Reply
        ReplyMessage(TRUE); //该消息己经处理了差不多了，SendMessage的线程可以被唤醒了，因为后面我还要做些其他事情，比如//弹出对话框（DialogBox），你就不要傻等下去了。注意：在哪里调用ReplyMessage就可以在那个什么//通知系统去唤醒SendMessage线程

    DialogBox(hInst, "MyDialogBox", hwndMain, (DLGPROC) MyDlgProc);

break;
```



 





# （二十） 窗口与消息2

2017年02月13日 20:20:49 [wangpengk7788](https://me.csdn.net/wangpengk7788) 阅读数：632



**唤醒一个线程**

**线程的挂起与唤醒**

（1）当线程调用GetMessage或WaitMessage，而消息队列中又没有消息出现时，线程会被挂起。

（2）当消息被“Post”（也可以是线程间的“Send”）到消息队列时，相应的Wake标志位会被设置，以表明该线程可被调度。

 **查询队列的状态：DWORD GetQueueStatus(UINT fuFlags)**

（1）fuFlags参数，可以是下列标志或组合，表示要查询的哪个（些）标志位的状态

| 要查询的标志位    | 队列中的消息                                                 |
| ----------------- | ------------------------------------------------------------ |
| QS_KEY            | 如果有一条WM_KEYUP、WM_KEYDOWN、WM_SYSKEYUP或WM_SYSKEYDOWN在消息队列中，则该标志被设置。 |
| QS_MOUSEMOVE      | 一条WM_MOUSEMOVE在“Input -Queue”队列中（只要队列中存在一个未处理的WM_MOUSEMOVE消息，这个标志被设置，当最后一条WM_MOUSEMOV消息被从队列删除时，标志被关闭）。 |
| QS_MOUSEBUTTON    | 一条鼠标消息（如WM_?BUTTON*消息等）                          |
| QS_MOUSE          | 相当于QS_MOUSEMOVE｜QS_MOUSEBUTTON的组合                     |
| QS_INPUT          | 相当于QS_MOUSE｜QS_KEY的组合                                 |
| QS_PAINT          | 一条WM_PAINT消息（该线程创建的窗口中，只要窗口存在无效区时，该位被设置。只有当线程建立的所有窗口都有效时，这个标志才关闭）。 |
| QS_TIMER          | 一条WM_TIMER消息（当定时器报时，QS_TIMER被设置，当GetMessage返回WM_TIMER消息后，该标志被关闭） |
| QS_HOTKEY         | 一条WM_HOTKEY消息                                            |
| QS_POSTMESSAGE    | post的消息（不同于硬件输入事件）。如果队列在期望的消息过滤范围内没有post的消息，该标志被关闭。只要Posted-Message队列中有一条消息，该标志就被设置。 |
| QS_ALLPOSTMESSAGE | Post的消息（不同于硬件输入事件）。如果Posted-Message队列中完全没有消息，这个标志被清除。 |
| QS_ALLEVENTS      | 相当于QS_INPUT｜QS_POSTMESSAGE\|QS_TIMER\|QS_PAINT｜QS_HOTKEY |
| QS_QUIT           | 调用了PostQuitMessage函数，该标志被设置，但系统并不向线程添加WM_QUIT消息。（这个标志是未公开的，只供系统内部使用，GetQueueStatus不返回这个标志的状态） |
| QS_SENDMESSAGE    | 由另一个线程发送的消息，即Send-Message队列中有消息时被设置，没有时被清除。 |

（2）返回值：

　　①高位字表示队列里**当前消息的类型，即如果调用GetMessage将取出来的那条消息**。（如WM_SIZE、WM_DESTROY等）。但这与fuFlags指定的查询状态有关。它总是返回是所想要的标志集的子集。例如，对下面的调用：BOOL fPaintMsgWaiting = HIWORD(GetQueueStatus(QS_TIMER)) & QS_PAINT; fPaintMsgWaiting的值总是FALSE，不论队列中是否有一个WM_PAINT消息在等待，因为GetQueueStatus的参数中没有将QS_PAINT指定为一个标志。

　　②低位字表示**下个消息的类型**，即上次调用GetQueuestatus，GetMessage或PeekMessBge以来加入队列并仍然在队列里的消息的类型。

　　③QS_标志出现在返回值里，并不保证以后调用函数GetMessage或PeekMessage会返回一个消息。GetMesssge和PeekMesssge执行某些内部过滤会导致消息被内部处理。因此，GetQueueStatus的返回值只能被看作是否调用GetMessage或PeekMessage的提示。

 **从线程的队列中提取消息的算法**

![img](http://images2015.cnblogs.com/blog/772759/201603/772759-20160319182231834-1486453457.png) 

**（1）调用GetMessage（或PeekMessage）获取下一条消息的算法过程**

　　①如果**QS_SENDMESSAGE标志被设置**，系统向相应的窗口发送消息。Get/PeekMessage函数在内部进行这种处理，并且在窗口过程处理完消息之后**不返回到线程**。相反，Get/PeekMessage函数会等待其他消息以便处理（可以是任何的消息，包括本线程或其他线程send或post过来的消息）。

　　②系统查看“posted-message”队列，如果“posted-message”队列有消息，Get/PeekMessage会填充MSG结构体，然后返回。在线程的消息循环中通常会调用DispatchMessage，将这个消息分派到相应的窗口过程中去处理。

　　③如果QS_QUIT标志被设置，Get/PeekMessage会返回一条WM_QUIT消息（在消息的wParam参数中指出了退出代码），并关闭QS_QUIT标志。

　　④如果在线程的虚拟“Input-Queue”有消息，Get/PeekMessage会返回硬件输入消息（如键盘或鼠标消息）。

　　⑤如果QS_PAINT标志被设置，Get/PeekMessage会返回一条WM_PAINT消息给相应的窗口。

　　⑥如果QS_TIMER标志被设置，Get/PeekMessage会返回一条WM_TIMER消息。

**（2）为什么 “posted-message”队列在“虚拟Input-Queue”队列之前被检查？**

　　①由于某个硬件事件可能引起其他的软件事件，所以系统要等用户读取下一个硬件事件之前，先处理这些软件事件。

　　②比如，调用TranslateMessage函数。这个函数会检查是否有一个WM_KEYDOWN（或WM_SYSKEYDOW硬件消息）从“Input-Queue”队列中被取出。如果有，系统会检查虚键信息能否转化为等价的字符，当可以转换时TranslateMessage会调用PostMessage将一个WM_CHAR（或WM_SYSCHAR）的软件消息投递到“posted-message”队列。下次调用GetMessage时，系统首先会检查“posted-message”队列，将这个WM_CHAR（或WM_SYSCHAR）消息取出。直到这个队列为空，再去检查“input-queue”队列。所以才会生成WM_KEYDOWN→WM_CHAR→WM_KEYUP这样的消息序列。

**（3）为什么QS_QUIT的检测在检查“post-message”队列之后。原因有两个：**

使用QS_QUIT标志可以让线程在结束消息循环前，处理完所有“posted-message”队列中的消息，比如下面代码：

```c
case WM_CLOSE:

   PostQuitMessage(0);//虽然比后面的PostMessage更早调用，但不会更早退出！
   PostMessage(hwnd,WM_USER,0,0);// 会先取出“posted-message”队列中的
                                 //WM_USER，队列空时再检查QS_QUIT标志。
   Break;
```

**（4）为什么PostQuitMessage只设置QS_QUIT而不将WM_QUIT消息投递到“posted-message”队列？**

　　①调用PostQuitMessage类似于（但不相同）调用PostThreadMessage(,WM_QUIT,…)。后者会**将消息添加到“posted-message”队列的尾端**，并使消息在系统检查“input-queue”前被处理。但**前者只会设置QS_QUIT标志位，而不会将WM_QUIT消息放入“posted-queue”队列**。因为当低内存时，post一个消息到“posted-message”可能会失败，如果一个程序想退出，即使是低内存时也应该被允许，但postQuitMessage函数调用不会失败（其返回值为void），因为它只改变QS_QUIT标志。

　　②尽管**PostQuitMessage不会向消息队列投递WM_QUIT消息**，但当系统检测到QS_QUIT标志被设置时，会先填充MSG结构体,将uMsg字段设为WM_QUIT，然后设置GetMessage的返回值为FALSE。由于系统内部自动填充了这个带有WM_QUIT信息的MSG结构体，让人感觉GetMessage好象从posted-message队列取出了一条WM_QUIT消息。但实际上这条消息并不是从“posted-message”队列中取出的，而是系统伪造的一个MSG结构体。

（5）WM_PAINT优先级低于键盘消息，WM_TIMER的优先级比WM_PAINT更低（防止用定器时在短时间内重复的WM_PAINT）。

（6）Get/PeekMessage只检查唤醒标志和调用线程的消息队列。这意味**不能从其他线程的消息队列中取得消息，包括同一进程内的其他线程的消息**。

**利用内核对象或队列状态标志唤醒线程（与UI相关或无关的线程）**

（１）当内核对象触发或某种消息到达时唤醒线程，既可以兼顾让线程运行一个长时间的操作时，又可以响应界面操作，防止界面出现一种“假死”的现象。

（２）MsgWaitForMultipleObjects（Ex）函数

| 参数                 | 描述                                                         |
| -------------------- | ------------------------------------------------------------ |
| DWORD nCount         | 要等待的句柄的数量                                           |
| PHANDLE phObjects    | 内核对象的句柄数组                                           |
| BOOL fWaitAll        | 该参数MsgWaitForMultipleObjects才有的。TRUE表示所有事件（内核对象和指定消息）触发；FALSE表示内核对象或消息任一被触发时，线程被唤醒。 |
| DWORD dwMilliseconds | 要等待的毫秒数                                               |
| DWORD dwWakeMask     | 何时触发事件，即要等待哪些消息到达，与GetQueueStatus的参数一致。带QS_前缀的一个或多个常数。 |
| DWORD dwFlags        | 该参数MsgWaitForMultipleObjectsEx才有。可以是以下标志的组合，如果不要指定任何标志，可以设为0①MWMO_WAITALL：等待所有内核对象触发且指定消息到达。如果不使用该标志，则只有任一内核对象触发或消息到达，线程就被唤醒。②MWMO_ALERTABLE：让线程处于可警告状态③MWMO_INPUTAVAILABLE：只要队列中有指定的消息就被唤醒（不一定要等指定的、新的消息到达才唤醒） |
| 返回值               | 内核对象：WAIT_OBJECT_0～WAIT_OBJECT_nCount-1消息触发：WAIT_OBJECT_nCount超时：WAIT_TIMEOUT…… |

（3）使用MsgWaitForMultipleObjects(Ex)的几个注意点

　　①nCount的最大值为MAXIMUM_WAIT_OBJECTS-1，即63。

　　②如果fWaitAll为FALSE，当任一内核对象或指定的消息到达时，函数会立即返回。如果fWaitAll为TRUE时，函数必须等到所有内核对象被触发且指定的消息到达时才返回。

　　③默认下，当调用这两个函数时，它们都会检查是否有指定的新的消息的到来，比如现在队列是有两个键盘消息，如果现在用带QS_INPUT标志的参数调用MsgWaitForMultipleOjbects(Ex)函数，线程会被唤醒。当第1个键盘消息被取出并处理完后，如果继续调用MsgWaitForMultipleOjbects(Ex)函数，则线程会挂起。此时虽然队列中还有一个键盘消息，但他不是新的。为了解决这个问题，在MsgWaitForMultipleOjbectsEx函数中增加了一个MWMO_INPUTAVAILABLE标志，只要队列中有指定消息，函数就立即返回，但这个标志只能在MsgWaitForMultipleOjbectsEx中使用。

【使用MsgWaitForMultipleOjbectsEx的消息循环】

![复制代码](http://common.cnblogs.com/images/copycode.gif)

```c
BOOL fQuit = FALSE; //是否要退出消息循环

while(!fQuit){
     //当内核对象触发或UI消息到达时唤醒线程
     DWORD dwResult = MsgWaitForMultipleObjectsEx(1,&hEvent,
                               INFINITE,QS_ALLEVENTS,//所有事件
                               MWMO_INPUTAVAILABLE); //只有消息队列还有消息，就返回
    
     switch(dwResult)
     {
     case WAIT_OBJECT_0://内核对象被触发

         break;       
     case WAIT_OBJECT_0+1://消息出现在消息队列中
         //分派消息
         MSG msg;
         //与GetMessage不同，PeekMessage的返回值表示有无获取到消息。
         //GetMessage:如果函数取得WM_QUIT之外的其他消息，返回非零值.
         //           如果函数取得WM_QUIT消息，返回值是零
         //PeekMessage:TRUE表示获取到一条消息。FALSE表示队列中无消息
         while (PeekMessage(&MSG,NULL,0,0,PM_REMOVE))
         {
              if (msg.message == WM_QUIT)
              {
                   fQuit = TRUE;//退出消息循环
              }else{
                   //翻译和分派消息
                   TranslateMessage(&msg);
                   DispatchMessage(&msg);
              }
         }//队列己空
         break;
     }
}//循环结束
```

![复制代码](http://common.cnblogs.com/images/copycode.gif)

 **利用窗口消息在进程之间传送数据**

**WM_SETTEXT/WM_GETTEXT消息的发送过程**

**（1）WM_SETTEXT消息的处理过程**

　　①当调用SendMessage时，函数会检查是否要发送WM_SETTEXT消息。如果是，就将以零结尾的字符串从调用进程的地址空间放入到一个内存映像文件（可在进程间共享）。

　　②然后再发送消息到共他进程的线程。

　　③当接收线程己准备好处理WM_SETTEXT消息时，它在自己的地址空间中找到上述内存映射文件（该文件包含要发送的文本），并让lParam指向那个文本，然后消息被分派到指定的窗口过程去进行处理。

　　④处理完消息之后，内存映像文件被删除。

**（2）WM_GETTEXT消息的处理过程**

　　①SendMessage时，系统检测到WM_GETTEXT消息时，实际上会发送两个消息。首先系统向那个窗口发送WM_GETTEXTLENGTH消息，以获得文本的长度。

　　②系统利用这个长度创建一个内存映像文件，用于在两个进程间共享。然后再发送消息来填充它。

　　③然后转到调用SendMessage的进程，从内存映像文件中复制文本到指定的缓冲区（由lParam参数指定）

　　④最后SendMessage函数返回。

**WM_COPYDATA消息发送过程**

（1）COPYDATASTRUCT结构体：

```
typedef struct tagCOPYDATASTRUCT
{
    ULONG_PTR dwData;//备用的数据项，可以放任何值
    DWORD cbData;    //要向另一个进程发送的字节数
    PVOID lpData;    //指向要发送的第1个字节（地址在发送进程的地址空间中）
} COPYDATASTRUCT;
```

（2）WM_COPYDATA消息的处理过程

　　①当SendMessage看到要发送一个WM_COPYDATA消息时，会建立一个内存映像文件，大小是cbData字节。

　　②从发送进程的地址空间中将数据复制到这个内存映像文件。

　　③然后向目标窗口发送消息

　　④接收线程在处理这个消息时，lParam参数己指定接收进程地址空间中的一个COPYDATASTRUCT结构体。这个结构的lpData成员指向了接收进程地址空间中的共享内存映像文件的视图。

（3）使用WM_COPYDATA消息，应注意的3个重要问题。

　　①只能SendMessage这个消息，而不能PostMessage。因为在接收消息的窗口过程处理完消息后，系统必须释放内存映像文件。但因PostMessage是立即返回的，它不知道这个消息何时被处理完，也就不能释放这个内存块。

　　②系统从另外的进程地址空间中复制数据要花费一些时间。所以不应让发送程序中运行的其他线程在SendMessage返回前修改这个内存块。

　　③WM_COPYDATA消息，可以实现16位到32位之间的通信，也能实现32位到64位之间的通信。但Win98以下没有WM_COPYDATA消息和COPYDATASTRUCT结构本的定义，须参考msdn自己定义。

【27_CopyData程序】演示如何使用WM_COPYDATA消息从一个程序向另一个程序发送一个数据块。

 ![img](http://images2015.cnblogs.com/blog/772759/201603/772759-20160319183741474-2024221717.jpg)

![复制代码](http://common.cnblogs.com/images/copycode.gif)

```c
/************************************************************************
Module: CopyData.cpp
Notices: Copyright(c) 2008 Jeffrey Ritcher & Christophe Nasarre
************************************************************************/

#include "../../CommonFiles/CmnHdr.h"
#include "resource.h"
#include <malloc.h>
#include <tchar.h>

//////////////////////////////////////////////////////////////////////////
BOOL Dlg_OnCopyData(HWND hWnd,HWND hWndFrom,PCOPYDATASTRUCT pcds){
    Edit_SetText(GetDlgItem(hWnd,pcds->dwData?IDC_DATA2:IDC_DATA1),
        (PTSTR)pcds->lpData);
    return (TRUE);
}

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
BOOL Dlg_OnInitDialog(HWND hWnd,HWND hWndFocus,LPARAM lParam){
    chSETDLGICONS(hWnd,IDI_COPYDATA);

    //初始化编辑框
    Edit_SetText(GetDlgItem(hWnd,IDC_DATA1),TEXT("测试数据"));
    Edit_SetText(GetDlgItem(hWnd,IDC_DATA2),TEXT("其他测试数据"));
    return (TRUE);
}

//////////////////////////////////////////////////////////////////////////
void Dlg_OnCommand(HWND hWnd,int id,HWND hWndCtrl,UINT codeNotify){
    switch(id){
    case IDCANCEL:
        EndDialog(hWnd,id);
        break;

    case IDC_COPYDATA1:
    case IDC_COPYDATA2:
        if (codeNotify !=BN_CLICKED)
           break;

        //获取相应的文本框
        HWND hWndEdit = GetDlgItem(hWnd,
            (id == IDC_COPYDATA1)?IDC_DATA1:IDC_DATA2);

        //准备一个COPYDATASTRUCT结构体，其内容会被Copy到
        //一个内存映像文件中
        COPYDATASTRUCT cds;

        //指示哪个数据域的内容要被发送（0=ID_DATA1，1=ID_DATA2)
        cds.dwData =(DWORD)((id == IDC_COPYDATA1)?0:1);

        //获取文本的长度(字节）
        cds.cbData = (Edit_GetTextLength(hWndEdit)+1)*sizeof(TCHAR);

        //在栈中分配一个内容以保存字符串内容
        cds.lpData = _alloca(cds.cbData);

        //将编辑框里的字符串存到cbs.lpData中
        Edit_GetText(hWndEdit,(PTSTR)cds.lpData,cds.cbData);

        //获取应用程序的标题
        TCHAR szCaption[100];
        GetWindowText(hWnd,szCaption,sizeof(szCaption)/sizeof(szCaption[0]));

        //枚举所有具有相同标题的顶层窗口
        HWND hWndT = NULL;
        do 
        {
            hWndT = FindWindowEx(NULL,hWndT,NULL,szCaption);
            if (hWndT != NULL)
            {
                FORWARD_WM_COPYDATA(hWndT,hWnd,&cds,SendMessage);
            }
        } while (hWndT != NULL);
        break;
    }
}

//////////////////////////////////////////////////////////////////////////
INT_PTR WINAPI Dlg_Proc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam){
    
    switch(uMsg){
        chHANDLE_DLGMSG(hWnd,WM_INITDIALOG,Dlg_OnInitDialog);
        chHANDLE_DLGMSG(hWnd,WM_COMMAND,Dlg_OnCommand);
        chHANDLE_DLGMSG(hWnd,WM_COPYDATA,Dlg_OnCopyData);
    }
    
    return (FALSE);
}

//////////////////////////////////////////////////////////////////////////
int WINAPI _tWinMain(HINSTANCE hInstExe,HINSTANCE hPrev,PTSTR lpCmdLIne,int nShowCmd){
    DialogBox(hInstExe,MAKEINTRESOURCE(IDD_COPYDATA),NULL,Dlg_Proc);
    return 0;
}
```

![复制代码](http://common.cnblogs.com/images/copycode.gif)

 

//resource.h

![复制代码](http://common.cnblogs.com/images/copycode.gif)

```
//{{NO_DEPENDENCIES}}
// Microsoft Visual C++ 生成的包含文件。
// 供 27_CopyData.rc 使用
//
#define IDD_COPYDATA                    101
#define IDC_DATA1                       1001
#define IDC_DATA2                       1002
#define IDC_COPYDATA1                   1003
#define IDC_COPYDATA2                   1004
#define IDI_COPYDATA                    1005

// Next default values for new objects
// 
#ifdef APSTUDIO_INVOKED
#ifndef APSTUDIO_READONLY_SYMBOLS
#define _APS_NEXT_RESOURCE_VALUE        103
#define _APS_NEXT_COMMAND_VALUE         40001
#define _APS_NEXT_CONTROL_VALUE         1004
#define _APS_NEXT_SYMED_VALUE           101
#endif
#endif
```

![复制代码](http://common.cnblogs.com/images/copycode.gif)

//27_CopyData.rc

![复制代码](http://common.cnblogs.com/images/copycode.gif)

```
// Microsoft Visual C++ generated resource script.
//
#include "resource.h"

#define APSTUDIO_READONLY_SYMBOLS
/////////////////////////////////////////////////////////////////////////////
//
// Generated from the TEXTINCLUDE 2 resource.
//
#include "afxres.h"

/////////////////////////////////////////////////////////////////////////////
#undef APSTUDIO_READONLY_SYMBOLS

/////////////////////////////////////////////////////////////////////////////
// 中文(简体，中国) resources

#if !defined(AFX_RESOURCE_DLL) || defined(AFX_TARG_CHS)
LANGUAGE LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED

#ifdef APSTUDIO_INVOKED
/////////////////////////////////////////////////////////////////////////////
//
// TEXTINCLUDE
//

1 TEXTINCLUDE 
BEGIN
    "resource.h\0"
END

2 TEXTINCLUDE 
BEGIN
    "#include ""afxres.h""\r\n"
    "\0"
END

3 TEXTINCLUDE 
BEGIN
    "\r\n"
    "\0"
END

#endif    // APSTUDIO_INVOKED


/////////////////////////////////////////////////////////////////////////////
//
// Dialog
//

IDD_COPYDATA DIALOGEX 0, 0, 239, 45
STYLE DS_SETFONT | DS_CENTER | WS_MINIMIZEBOX | WS_VISIBLE | WS_CAPTION | WS_SYSMENU
CAPTION "CopyData应用程序"
FONT 9, "宋体", 400, 0, 0x86
BEGIN
    LTEXT           "数据1：",IDC_STATIC,13,9,29,8
    LTEXT           "数据2：",IDC_STATIC,12,26,29,8
    EDITTEXT        IDC_DATA1,42,6,63,14,ES_AUTOHSCROLL
    EDITTEXT        IDC_DATA2,42,23,63,14,ES_AUTOHSCROLL
    PUSHBUTTON      "将数据1发送到其他窗口",IDC_COPYDATA1,115,6,110,14
    PUSHBUTTON      "将数据2发送到其他窗口",IDC_COPYDATA2,115,23,110,14
END


/////////////////////////////////////////////////////////////////////////////
//
// DESIGNINFO
//

#ifdef APSTUDIO_INVOKED
GUIDELINES DESIGNINFO
BEGIN
    IDD_COPYDATA, DIALOG
    BEGIN
        LEFTMARGIN, 7
        RIGHTMARGIN, 232
        TOPMARGIN, 3
        BOTTOMMARGIN, 40
    END
END
#endif    // APSTUDIO_INVOKED


/////////////////////////////////////////////////////////////////////////////
//
// Icon
//

// Icon with lowest ID value placed first to ensure application icon
// remains consistent on all systems.
IDI_COPYDATA            ICON                    "CopyData.ico"
#endif    // 中文(简体，中国) resources
/////////////////////////////////////////////////////////////////////////////



#ifndef APSTUDIO_INVOKED
/////////////////////////////////////////////////////////////////////////////
//
// Generated from the TEXTINCLUDE 3 resource.
//


/////////////////////////////////////////////////////////////////////////////
#endif    // not APSTUDIO_INVOKED
```

![复制代码](http://common.cnblogs.com/images/copycode.gif)

**27.6 Windows如何处理ANSI/Unicode字符串**

**27.6.1 问题的由来**

（1）对于某些消息（如WM_SETTEXT），**窗口过程可能要求**传入ANSI或Unicode字符串。这取决于注册窗口类时所使用的函数。如果调用**RegisterClassA**，系统认为**窗口过程要求**所有字符串和字符都属于**ANSI**。而用**RegisterClassW**注册窗口类时，则系统认为窗口过程需要**Unicode字符串**。

（2）IsWindowsUnicode(hwnd)可用来判断窗口过程需要使用哪种字符（串）类型

（3）如果要将一个ANSI字符串发送WM_SETTEXT给一个要求Unicode串的窗口过程，则系统在发送消息之前，会自动地转换字符串而无法我们干预。如果将Unicode字符串发送WM_SETTEXT给一个要求ANSI串的窗口过程，就需要我们自己来转换。

**27.6.2 窗口子类化可能出现的问题（假设父类的窗口过程要求Unicode）**

**（1）子类窗口过程要求的字符类型：**

　　调用SetWindowLongPtrA/W并将nIndex设为GCLP_WNDPROC，dwNewLong设为子类窗口过程的地址。如果调用**SetWindowLongPtrA表示子类的窗口过程要求是ANSI字符串**（可以调用IsWindowUnicode来判断子窗口过程验证是否是ANSI类型的）。相反如果调用SetWindowLongPtrW，则表示子类的窗口过程要求Unicode字符串。

**（2）子类窗口过程中如何知道父类的窗口过程使用的是哪种类型？**

　　当调用SetWindowLongPtr来派生子类时，系统会检查子类与父类**窗口过程要求的类型**是否一致。**如果一致，SetWindowLongPtr就直接返回原窗口过程（父类）的地址**，如果**不一致**，SetWindowLongPtr**返回一个内部子系统数据结构的句柄**，**这个数据结构包含原窗口过程的地址及一个用来指示父窗口过程要求ANSI或Unicode的数值**。

**（3）子类窗口过程的设计**

　　因SetWindowLongPtr的返回值可能是原窗口过程，也可能是系统内部一个数据结构，所以**子类中不能直接通过这个返回值来调用原窗口过程**,而**应该用CallWindowProc**，因为这个函数会查看传入的地址是一个窗口过程的地址还是某个内部数据结构的地址。如果传入的是一个窗口过程，则调用原先（父类）的窗口过程，不需要执行字符转换。否则，根据内部数据结构指示的类型进行转换，再调用原窗口过程。

【子类窗口过程的设计】

![复制代码](http://common.cnblogs.com/images/copycode.gif)

```
  //子类化
  WNDPROC wndprocPrev = SetWindowLongPtr(hwnd,GWL_WNDPROC,SubClassWndProc);
  ……

  //子类窗口过程
  LRESULT SubClassWndProc(hwnd,uMsg,wParam,lParam){
  …… //子类处理一些自己感兴趣的消息
  //对不感兴趣的消息，调用父类窗口过程处理
  //return CallWindowsProcA(wndprocPrev,hwnd,uMsg,wParam,lParam); //子类使用ANSI时
  return   CallWindowsProcW(wndprocPrev,hwnd,uMsg,wParam,lParam); //子类Unicode时
  
  //以下调用父类窗口过程的方法是错误的，因为wndprocPrev可能是原窗口过程，也可能是系统
  //内部的数据结构
  //return wndprocPrev(hwnd,uMsg,wParam,lParam);//错误，直接使用SetWindowLongPtr返回值。
}
```

![复制代码](http://common.cnblogs.com/images/copycode.gif)

 









# （二十一） 硬件输入模型和局部输入状态

2017年02月13日 20:38:55 [wangpengk7788](https://me.csdn.net/wangpengk7788) 阅读数：633



28.1 原始输入线程（RIT）

**（1）图解硬件输入模型**

![img](http://images2015.cnblogs.com/blog/772759/201603/772759-20160319190528599-40096726.png) 

　　①当操作系统初始化时会创建一个**原始输入线程（RIT）**和**系统硬件消息队列（SHIQ）**，这两者是系统硬件输入模型的核心。当SHIQ队列有硬件（如鼠标或键盘）消息时，RIT被唤醒，并将事件添加到用户线程的VIQ队列。

　　②任何时刻，只能有一个用户线程与RIT连接，该线程被称为**前景线程**。相对于其他线程创建的窗口，前景线程创建的窗口主要当前正在与用户交互的窗口。

　　③RIT如何知道要往哪个线程增加硬件输入消息？

　　　　A.如果是鼠标消息，RIT会调用检查当前鼠标所在的窗口，并调用GetWidowThreadProcessId找出创建该窗口的线程，然后向该线程添加消息。

　　　　B.如果是键盘事件，**RIT只向前景线程添加键盘消息**

　　④RIT如何切换与不同线程的连接？

　　　　A.当进程创建一个子进程，而子进程的线程里创建了一个窗口时，这个子进程的线程成为前景线程。

　　　　B.RIT负责Alt+Tab、Alt+Esc和Ctrl+Alt+Del键的处理，用户可以通过这些按键的组合来激活窗口并将创建该窗口的线程连接到RIT。（注意这些按键组合在RIT内部处理，所以我们无法拦截或丢弃它们）。Windows提供了一些激活窗口的函数（见后面）

**（2）硬件输入模型的健壮性**

　　①假设当前前景线程为线程B，此时RIT将消息分派给线程B的VIQ队列。如果此时线程B遇到一个死循环或死锁。由于线程B仍与RIT相连，所以系统的硬件消息仍会被添加到线程B的队列中。

　　②当用户按住Alt+Tab键时，由于Alt+Tab组合键由RIT线程内部处理，所以用户可以激活其他线程的窗口（如窗口A1），这里线程A将连接到RIT。因此，保证了当线程B出了问题时，不会影响到其他线程的执行。

**28.2 局部输入状态**

　　每个线程都有自己的THREADINFO结构，通过使用THREADINFO结构可以控制虚拟输入队列和一组状态信息，从而可以让线程使用虚拟输入队列和状态信息变量来实现对“窗口焦点、鼠标捕获”的管理信息有各自不同的处理，以防止对其他线程的影响。主要有两类：

　　①管理与键盘输入及焦点窗口有关的信息：如哪个窗口拥有键盘焦点、哪个窗口是活动的、键盘上哪些键被按下。

　　②管理与鼠标和光标有关的信息：哪个窗口捕获鼠标、鼠标的形状、鼠标的可见性

**28.2.1 键盘输入与焦点**

（1）焦点窗口与活动窗口的区别

　　**线****程内部**会维护当前自己的活动窗口（Active Window）和焦点窗口（Focus Window），焦点窗口其实只是窗口的一个属性，其实就是“焦点状态”是窗口的一个属性，而**焦点窗口的顶层窗口就是活动窗口**。比如，一个对话框中有一个按钮，当按钮获得焦点的时候，那此按钮就是焦点窗口，则包含此按钮的对话框就是活动窗口，若出现窗口嵌套的情况，则最根的那个窗口才是活动窗口。

　　**焦点窗口只是一个局部的概念**，并不是所有的焦点窗口都可以获得键盘事件。只有前景线程的焦点窗口才能从系统队列中得到键盘事件(所以要SetFocus())，而**前景线程中的活动窗口是前景窗口**。在任何时刻系统中都只可能有一个被激活的窗口，这就是前景窗口。

（2）窗口间的焦点切换

![img](http://images2015.cnblogs.com/blog/772759/201603/772759-20160319190636599-355855249.png) 

　　①RIT是将键盘输入放到线程的虚拟输入队列（VIQ），**而不是某个窗口**。当线程GetMessage时，键盘事件被**从VIQ中删除**，并被分派到**当前的焦点窗口**。

　　②要让不同窗口接受键盘输入，须做两件事：一是让将接受线程与RIT连接。二是在该线程局部输入状态变量中记录要成为焦点的窗口。而SetFocus不能同时完成这两件事，只能做后面的一件。

　　③假设**当前线程1与RIT连接**，当调用SetFocus，然后传入参数hwndA、hwndB、hwndC则焦点会在这三个窗口之间切换。但如果传入的是hwndD、hwndE、hwndF中的任何一个都会失败，实际上SetFocus什么事情都不做。因为当前线程B还没与RIT相连接。

　　④仍然**假设线程1与RIT正连接着**，如果线程2调用SetFocus，然后传入hwndE。此时，线程2的局部输入状态变量会被更新以反映这一变化，在以后线程2连接到RIT时，键盘事件会被分派到窗口E中。但要注意的是，虽然在线程2还未连接RIT之前，窗口E还不能接收到键盘消息，但它会收到WM_SETFOCUS消息，如果是按钮，其表面会绘出虚线框。

　　⑤当焦点切换时，失去焦点的窗口会收到WM_KILLFOCUS消息。如果接收焦点的窗口与失去窗口的焦点属于不同线程创建的，那么失去焦点的窗口会更新局部输入状态变量，以表明该线程现在没有焦点窗口（调用GetFocus会返回NULL）。

（3）设置焦点窗口函数的比较

　　①SetForegroundWindow 和 SetActiveWindow的区别

　　SetActiveWindow(hWnd)函数，改变的是一个线程的局部状态变量，将活动窗口置为hWnd。如果**当前线程是背景线程，则只改变局部状态变量**。如果是**前景线程，则该窗口会被置为前景窗口**。但要注意的是这个函数**不能够跨线程调用**（也就是说不能够改变另外一个线程的局部变量），即如果hWnd为其他线程的窗口，则调用线程的局部状态变量将被设为NULL，所以GetActivWindow会返回NULL。

　　SetWindowPos、BringWindowToTop（该函数内部调用了SetWindowPos）**可以跨线程或进程调用**，函数会改变窗口的Z序，激活状态和焦点。**如果调用线程未连接到RIT则什么也不做**。**如果调用线程己连接到RIT**，则系统会激活hWnd窗口（其他线程也可以）。这也就意味着如果hWnd是其他线程创建的窗口，则会**同时将这个线程连接到RIT**，并改变其局部输入状态来反应激活窗口的变化。

　　SetForegroundWindow将窗口设为前景窗口， Windows为了防止突然的一个窗口跳至屏幕的Foreground，所以如果**调用线程是背景线程，则产生的将是任务栏闪烁效果，表示不当前前景进程（正连接RIT的进程）不允许该窗口置于它的前面而成为前景窗口，我们可以手动到任务栏去激活这个窗口**。而**BringWindowToTop和SetWindowPos （TOP）在没有连接到RIT的时候则干脆不起效果**。但是需要注意的是SetWindowPos（BOTTOM）还是有效果的（因为不违反Windows的这个约束）。

　　②但我们调用AllowSetForegroundWindow(dwProcessId)，表示允许dwProcessId进程弹出的窗口置于调用线程的窗口之上。当传入参数ASFW_ANY表示允许任何进程，如果这时调用线程为RIT，而其他线程要通过SetForegroundWindow来置顶窗口时，只会在任务栏中闪烁提示，而不会真正被置顶。

　　③LockSetForegroundWindow函数，如果调用线程调用该函数并传入LSFW_LOCK参数，则当该线程为前景线程时，任何其他线程调用SetForegroundWindow函数都将失败。这可以防止当前线程的前景窗口被其他线程的窗口给挡住。传入参数LSFW_UNLOCK时则解锁这种阻止。当用户按Alt键或用户显式将一个窗口变为前景窗口时，系统会自动解锁这种阻止，以防止一个应用程序总是霸占桌面。

（4）键盘状态——比较GetKeyState和GetAsyncKeyState函数

| 函数                           | 描述                                                         |
| ------------------------------ | ------------------------------------------------------------ |
| GetKeyState(int nVirtKey)      | ①线程局部输入状态包含一个同步键状态数组，这个数组为线程私有。②获得**最近那次从消息队列中删除键盘消息时的按键状态，**是从线程私有的同步键状态数组中获取到的。③nVirtKey指出要检查键的虚键代码，结果的高位为1时表示按下，0为释放 |
| GetAsyncKeyState(int nVirtKey) | ①该函数从异步键状态数组中获取按键的状态，这个数组是所有线程所共享的，函数检查当前实时的键盘状态。②nVirtKey指出要检查键的虚键代码。结果的高位为1时表示按下，0为释放。③如果调用线程不是当前焦点窗口的创建线程，则函数总是返回0. |

**28.2.2 鼠标光标管理**

**（1）ShowCursor(BOOL bShow):**

　　①只影响调用线程的光标状态。

　　②调用ShowCursor(FALSE)多少次来隐藏，就要ShowCursor(TRUE)多少次才能显示出来。

**（2）ClipCursor(CONST RECT\* prc);**

　　将鼠标限制在prc指定的范围内。但当异步激活事件发生（如用户激活另一个窗口）、调用SetForegroundWindow或用户按了Ctrl+Esc时，系统会停止鼠标剪裁。

**（3）SetCapture(HWND hWnd)**

　　①当调用SetCapture时，RIT会将所有消息发送给调用线程的VIQ队列，并把所有消息都发送给hWnd窗口。同时设置局部输入状态，以反映是哪个窗口被捕获。调用ReleaseCapture将释放捕获

　　②当鼠标按住时调用SetCapture，这里进行的是系统范围内的捕获。不管鼠标移到桌面上的哪个位置，鼠标消息都被发往hWnd窗口。**如果此时释放鼠标按键时**，RIT会检测到这个动作，此时如果鼠标位于其他线程创建的窗口时，**RIT会将鼠标消息发给鼠标光标之下的窗口，而不是hWnd。如果鼠标位于调用线程创建的任何窗口时**，这里系统会认为鼠标捕获仍然有效，所以只要鼠标位于调用线程所创建的任何一个窗口中，**鼠标消息都会被发往hWnd窗口**。换一句话讲，如果用户释放鼠标时，鼠标捕获不再是系统全局的捕获，而是线程局部的一种捕获。

　　③如果用户试图去激活另一个线程的窗口时，系统会自动向调用SetCapture线程发送鼠标按下和释放的消息。然后系统会更新调用线程的局部输入状态，以将捕获窗口设为NULL。

**（4）SetCursor(HCURSOR hCursor);——设置光标的形状**

　　①改变光标形状，并设置线程局部输入状态变量以更新鼠标的形状信息

　　②当鼠标在窗口中移动时（前提是未设置鼠标捕获），窗口会收到WM_SETFOCUS消息，这时可以调用SetCursor函数来设置鼠标的形状。

**28.3 让多个线程共享某个线程的虚拟输入队列和局部输入状态变量**

**（1） AttachingThreadInput(IdAttach,IdAttachTo,fAttach);**

| 参数                                                         | 说明                                                         |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| IdAttach                                                     | 参数为不再使用虚拟输入队列和局部输入变量的线程Id             |
| IdAttachTo                                                   | 参数要共享虚拟输入队列和局部输入变量的线程Id                 |
| fAttach                                                      | TRUE时表示要挂接线程以共享，FALSE表示分离线程的VIQ和局部输入变量。 |
| 备注：可多次调用，以让多个线程共享同一个VIQ和局部输入状态变量 |                                                              |

**（2）举例说明：AttachThreadInput(idThreadA,idThreadB,TRUE);**

![img](http://images2015.cnblogs.com/blog/772759/201603/772759-20160319190742490-2004147316.png) 

　　①所有将输入窗口A1、B1、B2的硬件输入事件都将被添加到线程B的虚拟输入队列。在分离之前，线程A的虚拟输入队列不再接收输入事件。

　　②当**两个线程共享虚拟输入队列**时，也会**共享同一套局部输入状态变量**。但是**会使用各自的posted-message、send-message、reply-message队列及唤醒标志位**。

　　③当线程共享单一个VIQ队列时，会严重影响程序的健壮性。当一个线程接收一个击键消息并挂起，另一个线程就不能接收任何的输入。

**（3）使用AttachThreadInput的场合**

　　①在少数场合下，系统会显式地将两个线程挂起在一起。如线程安装了日志记录或日志回放钩子。在钩子卸载时会自动将两个线程分离。如果线程安装日志记录钩子，它等于告诉系统当发生硬件输入事件时，它都应该被通知。由于用户的输入必须被按相同的顺序记录下来，所以系统会共享一个VIQ以让所有的输入处理都能被同步起来。

　　②当应用程序创建了两个线程，第1个线程创建了一个对话框，当创建完成后。第2个线程调用CreateWindow，使用WS_CHILD并将对话框的句柄传给函数以便创建一个对话框的子窗口。系统会调用AttachThreadInput让第1个线程与第2个线程共对话框线程的VIQ，这个动作可以强制所有对话框所有的子窗口（包括第1个线程创建和其他线程创建的窗口）输入都可以同步起来。

【28_LISLab程序】检查线程局部输入状态

![img](http://images2015.cnblogs.com/blog/772759/201603/772759-20160319211057178-810879299.jpg)

　　①GetFocus、GetActiveWindow、GetForegroundWindow、GetCapture是从调用线程自己的局部输入状态中获取信息的

　　②当激活记事本时（属不同线程间的切换），LISLab会收到WM_KILLFOCUS，从而会认为自己没有“焦点窗口”及“激活窗口”

　　③SetFocus、SetActiveWindow会返回之前的“焦点窗口”或“激活窗口”的句柄，并显示在“先前窗口”标签中。

　　④可以选中列表框的“记事本”，然后在“AttachToNotepad”和“DetachFromNotepad”下分别观察SetFocus等几个函数的操作结果。

　　⑤鼠标捕获实验：观察鼠标按住和释放鼠标时的不同

//LISLab.cpp

![复制代码](http://common.cnblogs.com/images/copycode.gif)

```
/************************************************************************
Module： LISLab.cpp
Notices：Copyright(c) 2008 Jeffrey Richter & Christophe Nasarre
************************************************************************/

#include "../../CommonFiles/CmnHdr.h"
#include "resource.h"
#include <tchar.h>

//////////////////////////////////////////////////////////////////////////
#define TIMER_DELAY  (500)  //半秒
UINT_PTR  g_uTimerId = 1;
int       g_nEventId = 0; //所选择的函数（如SetFocus：0，SetActiveWindow：1等）
DWORD     g_dwEventTime = 0;//事件发生以来的毫秒数
HWND      g_hwndSubject = NULL; //当前窗口列表中选中的窗口
HWND      g_hwndNotepad = NULL; //记事本的句柄

//////////////////////////////////////////////////////////////////////////
void CalcWndText(HWND hwnd, PTSTR szBuf, int nLen){
    TCHAR szClass[50], szCaption[50], szBufT[150];

    if (hwnd == (HWND)NULL){
        _tcscpy_s(szBuf, nLen,TEXT("(无窗口)"));
        return;
    }

    if (!IsWindow(hwnd)){
        _tcscpy_s(szBuf,nLen, TEXT("(无效窗口)"));
        return;
    }

    GetClassName(hwnd, szClass, chDIMOF(szClass));
    GetWindowText(hwnd, szCaption, chDIMOF(szCaption));

    wsprintf(szBufT, TEXT("[%s] %s"), (PTSTR)szClass,
             (*szCaption ==0)?TEXT("(无标题)"):(PTSTR)szCaption);

    _tcsncpy_s(szBuf,nLen, szBufT, nLen - 1);
    szBuf[nLen - 1] = 0; //强制0字符串结束
}

//////////////////////////////////////////////////////////////////////////
//为了最大限度为减少对栈的使用，这里创建一个局部变量WALKWINDOWTREEDATA，并
//将其指针传给WalkWindowTreeRecurse递归函数。

//在WalkWindowTreeRecurse递归函数中使用的数据
typedef struct{
    HWND hwndLB;       //要输出信息到指定的列表框
    HWND hwndParent;   //父句柄
    int nLevel;        //递归深度
    int nIndex;        //列表框项的索引
    TCHAR szBuf[100];  //输出缓冲区
    int  iBuf;         //szBuf的索引值
}WALKWINDOWTREEDATA,*PWALKWINDOWTREEDATA;

//////////////////////////////////////////////////////////////////////////
void WalkWindowTreeRecurse(PWALKWINDOWTREEDATA pWWT){
    if (!IsWindow(pWWT->hwndParent)) //父窗口是否存在
        return;

    pWWT->nLevel++;
    const int nIndexAmount = 2;

    //格式化输出，为了直观，每级比上一级缩进2个字符
    for (pWWT->iBuf = 0; pWWT->iBuf < pWWT->nLevel*nIndexAmount; pWWT->iBuf++)
        pWWT->szBuf[pWWT->iBuf] = TEXT(' ');

    //获取窗体类名及标题
    CalcWndText(pWWT->hwndParent, &pWWT->szBuf[pWWT->iBuf],
                chDIMOF(pWWT->szBuf)-pWWT->iBuf);

    pWWT->nIndex = ListBox_AddString(pWWT->hwndLB, pWWT->szBuf);
    ListBox_SetItemData(pWWT->hwndLB, pWWT->nIndex, pWWT->hwndParent);

    HWND hwndChild = GetFirstChild(pWWT->hwndParent);
    while (hwndChild != NULL){
        pWWT->hwndParent = hwndChild;
        WalkWindowTreeRecurse(pWWT);
        hwndChild = GetNextSibling(hwndChild);
    }
    pWWT->nLevel--;
}
//////////////////////////////////////////////////////////////////////////
void WalkWindowTree(HWND hwndLB, HWND hwndParent){
    WALKWINDOWTREEDATA WWT;
    WWT.hwndLB = hwndLB;
    WWT.hwndParent = hwndParent;
    WWT.nLevel = -1;
    WalkWindowTreeRecurse(&WWT);
}

//////////////////////////////////////////////////////////////////////////
BOOL Dlg_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam){

    chSETDLGICONS(hwnd, IDI_LISLAB);

    //关联一个向上箭头的鼠标到对话框的客户区
    SetClassLongPtr(hwnd, GCLP_HCURSOR, (LONG_PTR)LoadCursor(NULL,IDC_UPARROW));

    g_uTimerId = SetTimer(hwnd, g_uTimerId, TIMER_DELAY, NULL);

    HWND hWndT = GetDlgItem(hwnd, IDC_WNDFUNC);
    ComboBox_AddString(hWndT, TEXT("SetFocus"));
    ComboBox_AddString(hWndT, TEXT("SetActiveWindow"));
    ComboBox_AddString(hWndT, TEXT("SetForegroundWnd"));
    ComboBox_AddString(hWndT, TEXT("BringWindowToTop"));
    ComboBox_AddString(hWndT, TEXT("SetWindowPos-TOP"));
    ComboBox_AddString(hWndT, TEXT("SetWindowPos-BTM"));
    ComboBox_SetCurSel(hWndT, 0);
    
    //为列表填充所有的窗口信息（本窗口及记事本下的所有窗口）
    hWndT = GetDlgItem(hwnd, IDC_WNDS);
    ListBox_AddString(hWndT, TEXT("--->本对话框<---"));
    ListBox_SetItemData(hWndT, 0, hwnd);
    ListBox_SetCurSel(hWndT, 0);

    //添加记事本下的窗口
    g_hwndNotepad = FindWindow(TEXT("NotePad"), NULL);
    if (g_hwndNotepad == NULL){
        //记事本程序还没启动;启动它
        STARTUPINFO si = { sizeof(si)};
        PROCESS_INFORMATION  pi;
        TCHAR szCommandLine[] = TEXT("Notepad");
        if (CreateProcess(NULL,szCommandLine,NULL,NULL,FALSE,0,
            NULL,NULL,&si,&pi)){

            //等待记事本创建它所有的窗口
            WaitForInputIdle(pi.hProcess, INFINITE);
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
            g_hwndNotepad = FindWindow(TEXT("Notepad"), NULL);
        }
    }

    WalkWindowTree(hWndT, g_hwndNotepad);

    return (TRUE);
}

//////////////////////////////////////////////////////////////////////////
void Dlg_OnCommand(HWND hwnd, int id, HWND hwndCtrl, UINT codeNotity){
    HWND hWndT;
    switch (id)
    {
    case IDCANCEL:
        if (g_uTimerId != 0)
            KillTimer(hwnd, g_uTimerId);
        EndDialog(hwnd, id);
        break;

    case IDC_HIDECURSOR:
        ShowCursor(FALSE);
        break;

    case IDC_SHOWCURSOR:
        ShowCursor(TRUE);
        break;

    case IDC_INFINITELOOP:
        SetCursor(LoadCursor(NULL, IDC_NO));
        for (;;);
        break;

    case IDC_FUNCSTART: //“延时”按钮
        g_dwEventTime = GetTickCount() + 1000 * GetDlgItemInt(hwnd, IDC_DELAY, NULL, FALSE); //IDC_DELAY指定了秒数
        hWndT = GetDlgItem(hwnd, IDC_WNDS);//在“窗口”列表框
        g_hwndSubject = (HWND)//选中的、要操作的窗口
            ListBox_GetItemData(hWndT, ListBox_GetCurSel(hWndT));
        g_nEventId = ComboBox_GetCurSel(GetDlgItem(hwnd, IDC_WNDFUNC));
        SetWindowText(GetDlgItem(hwnd, IDC_EVENTPENDING), TEXT("Pending"));        
        break;

    case IDC_REMOVECLIPRECT:
        ClipCursor(NULL);
        break;

    case IDC_SETCLIPRECT:
        RECT rc;
        SetRect(&rc, 0, 0, GetSystemMetrics(SM_CXSCREEN) / 2,
                GetSystemMetrics(SM_CYSCREEN) / 2);
        ClipCursor(&rc);
        break;

    case IDC_THREADATTACH:
        AttachThreadInput(GetWindowThreadProcessId(g_hwndNotepad, NULL), 
            GetCurrentThreadId(),TRUE);
        break;

    case IDC_THREADDETACH:
        AttachThreadInput(GetWindowThreadProcessId(g_hwndNotepad, NULL),
                          GetCurrentThreadId(), FALSE);
        break;
    }
}

//////////////////////////////////////////////////////////////////////////
void AddStr(HWND hwndLB, PCTSTR szBuf){
    int nIndex;
    do{
        nIndex = ListBox_AddString(hwndLB, szBuf);
        if (nIndex == LB_ERR)
            ListBox_DeleteString(hwndLB, 0);

    } while (nIndex == LB_ERR);

    ListBox_SetCurSel(hwndLB, nIndex);
}

//////////////////////////////////////////////////////////////////////////

void Dlg_OnMouseMove(HWND hwnd, int x, int y, UINT KeyFlags){
    TCHAR szBuf[100];
    wsprintf(szBuf, TEXT("捕获=%s,消息=MouseMove, x=%5d,y=%5d"),
             (GetCapture() == NULL) ? TEXT("否") : TEXT("是"), x, y);
    AddStr(GetDlgItem(hwnd, IDC_MOUSEMSGS), szBuf);
}

//////////////////////////////////////////////////////////////////////////
int Dlg_OnLButtonDown(HWND hwnd, BOOL fDoubleClick,int x,int y,UINT KeyFlags){
    TCHAR szBuf[100];
    wsprintf(szBuf, TEXT("捕获=%s,消息=LButtonDown, DblClk=%s,x=%5d,y=%5d"),
             (GetCapture() == NULL) ? TEXT("否") : TEXT("是"),
             fDoubleClick ? TEXT("是"):TEXT("否"), x, y);
    AddStr(GetDlgItem(hwnd, IDC_MOUSEMSGS), szBuf);

    return (0);
}

//////////////////////////////////////////////////////////////////////////
void Dlg_OnLButtonUp(HWND hwnd, int x, int y, UINT KeyFlags){
    TCHAR szBuf[100];
    wsprintf(szBuf, TEXT("捕获=%s,消息=LButtonUp,x=%5d,y=%5d"),
             (GetCapture() == NULL) ? TEXT("否") : TEXT("是"), x, y);
    AddStr(GetDlgItem(hwnd, IDC_MOUSEMSGS), szBuf);
}

//////////////////////////////////////////////////////////////////////////
int Dlg_OnRButtonDown(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT KeyFlags){
    TCHAR szBuf[100];
    wsprintf(szBuf, TEXT("捕获=%s,消息=RButtonDown, DblClk=%s,x=%5d,y=%5d"),
             (GetCapture() == NULL) ? TEXT("否") : TEXT("是"),
             fDoubleClick ? TEXT("是") : TEXT("否"), x, y);
    AddStr(GetDlgItem(hwnd, IDC_MOUSEMSGS), szBuf);

    if (!fDoubleClick)
        SetCapture(hwnd);
    else ReleaseCapture();

    return (0);
}

//////////////////////////////////////////////////////////////////////////
void Dlg_OnRButtonUp(HWND hwnd, int x, int y, UINT KeyFlags){
    TCHAR szBuf[100];
    wsprintf(szBuf, TEXT("捕获=%s,消息=RButtonUp,x=%5d,y=%5d"),
             (GetCapture() == NULL) ? TEXT("否") : TEXT("是"), x, y);
    AddStr(GetDlgItem(hwnd, IDC_MOUSEMSGS), szBuf);
}

//////////////////////////////////////////////////////////////////////////
void Dlg_OnTimer(HWND hwnd, UINT id){ //每秒刷新2次
    TCHAR szBuf[100];
    CalcWndText(GetFocus(), szBuf, chDIMOF(szBuf));
    SetWindowText(GetDlgItem(hwnd, IDC_WNDFOCUS), szBuf);

    CalcWndText(GetCapture(), szBuf, chDIMOF(szBuf));
    SetWindowText(GetDlgItem(hwnd, IDC_WNDCAPTURE), szBuf);

    CalcWndText(GetActiveWindow(), szBuf, chDIMOF(szBuf));
    SetWindowText(GetDlgItem(hwnd, IDC_WNDACTIVE), szBuf);

    CalcWndText(GetForegroundWindow(), szBuf, chDIMOF(szBuf));
    SetWindowText(GetDlgItem(hwnd, IDC_WNDFOREGROUND), szBuf);

    RECT rc;
    GetClipCursor(&rc);
    wsprintf(szBuf, TEXT("左=%d,顶=%d,右=%d,底=%d"), 
             rc.left,rc.top,rc.right,rc.bottom);
    SetWindowText(GetDlgItem(hwnd, IDC_CLIPCURSOR), szBuf);

    //延时的时间未到或未输入延时数（秒），则直接返回
    if ((g_dwEventTime == 0) || (GetTickCount() < g_dwEventTime))
        return;

    HWND hWndT;
    switch (g_nEventId)
    {
    case 0: //SetFocus
        g_hwndSubject = SetFocus(g_hwndSubject);
        break;

    case 1: //SetActiveWindow
        g_hwndSubject = SetActiveWindow(g_hwndSubject);
        break;

    case 2: //SetForegroundWindow
        hWndT = GetForegroundWindow();
        SetForegroundWindow(g_hwndSubject);
        g_hwndSubject = hWndT;
        break;

    case 3: //BringWindowToTopWindow
        BringWindowToTop(g_hwndSubject);
        break;

    case 4: //SetWindowPos w/HWND_TOP
        SetWindowPos(g_hwndSubject, HWND_TOP, 0, 0, 0, 0, 
                     SWP_NOMOVE | SWP_NOSIZE);
        g_hwndSubject = (HWND)1;
        break;

    case 5: //SetWindowPos w/HWND_BOTTOM
        SetWindowPos(g_hwndSubject, HWND_BOTTOM, 0, 0, 0, 0,
                     SWP_NOMOVE | SWP_NOSIZE);
        g_hwndSubject = (HWND)1;
        break;
    }

    if (g_hwndSubject == (HWND)1){
        SetWindowText(GetDlgItem(hwnd, IDC_PREVWND), TEXT("不能判别."));
    } else{
        CalcWndText(g_hwndSubject, szBuf, chDIMOF(szBuf));
        SetWindowText(GetDlgItem(hwnd, IDC_PREVWND), szBuf);
    }
    g_hwndSubject = NULL; g_nEventId = 0; g_dwEventTime = 0;
    SetWindowText(GetDlgItem(hwnd, IDC_EVENTPENDING), TEXT("执行"));
}

//////////////////////////////////////////////////////////////////////////
INT_PTR WINAPI Dlg_Proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
    switch (uMsg)
    {
        chHANDLE_DLGMSG(hWnd, WM_INITDIALOG,   Dlg_OnInitDialog);
        chHANDLE_DLGMSG(hWnd, WM_COMMAND,       Dlg_OnCommand);

        chHANDLE_DLGMSG(hWnd, WM_MOUSEMOVE,    Dlg_OnMouseMove);

        chHANDLE_DLGMSG(hWnd, WM_LBUTTONDOWN,   Dlg_OnLButtonDown);
        chHANDLE_DLGMSG(hWnd, WM_LBUTTONDBLCLK, Dlg_OnLButtonDown);
        chHANDLE_DLGMSG(hWnd, WM_LBUTTONUP,     Dlg_OnLButtonUp);

        chHANDLE_DLGMSG(hWnd, WM_RBUTTONDOWN,   Dlg_OnRButtonDown);
        chHANDLE_DLGMSG(hWnd, WM_RBUTTONDBLCLK, Dlg_OnRButtonDown);
        chHANDLE_DLGMSG(hWnd, WM_RBUTTONUP,     Dlg_OnRButtonUp);

        chHANDLE_DLGMSG(hWnd, WM_TIMER,         Dlg_OnTimer);
    }
    
    return (FALSE);
}

//////////////////////////////////////////////////////////////////////////
int WINAPI _tWinMain(HINSTANCE hInstExe, HINSTANCE, PTSTR pszCmdLine, int){
    DialogBox(hInstExe, MAKEINTRESOURCE(IDD_LISLAB), NULL, Dlg_Proc);
    return 0;
}
/////////////////////////////文件结束/////////////////////////////////////
```

![复制代码](http://common.cnblogs.com/images/copycode.gif)

//resouce.h

![img](http://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif) View Code

//28_LISLab.rc

![img](http://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif) View Code

【28_LISWatch程序】用来监视“焦点窗口”、“活动窗口”和“鼠标捕获窗口”信息

　　①主线程会被Attach到前景线程或指定的线程。如果Attact到前景线程则监视所有线程，否则监视指定的线程。

　　②当监视指定线程时，如“计算器”程序，则“线程”标签上会为显示计算器的主线程ID。如果此时去激活另一个程序，如“记事本”，则将无法获取记事本的“焦点窗口”等信息，因为现在仍被Attach到“计算器”的线程时，所以无法获得非“计算器”线程的局部输入状态变量的信息。

![img](http://images2015.cnblogs.com/blog/772759/201603/772759-20160319232047381-1734024757.jpg)        ![img](http://images2015.cnblogs.com/blog/772759/201603/772759-20160319232543787-17346597.jpg)

//LISWatch.cpp

![复制代码](http://common.cnblogs.com/images/copycode.gif)

```
/************************************************************************
Module： LISWatch.cpp
Notices：Copyright(c) 2008 Jeffrey Richter
************************************************************************/

#include "../../CommonFiles/CmnHdr.h"
#include "resource.h"
#include <tchar.h>


//////////////////////////////////////////////////////////////////////////
#define TIMER_DELAY (500)  //半秒
UINT_PTR  g_uTimerId = 1;
DWORD  g_dwThreadIdAttchTo = 0; //0：System-wide；非0：指定的线程


//////////////////////////////////////////////////////////////////////////
static void CalcWndText(HWND hwnd, PTSTR szBuf, int nLen){
    if (hwnd == (HWND)NULL) {
        lstrcpy(szBuf, TEXT("(无窗口)"));
        return;
    }

    if (!IsWindow(hwnd)){
        lstrcpy(szBuf, TEXT("(无效窗口)"));
        return;
    }

    TCHAR szClass[50], szCaption[50], szBufT[150];
    GetClassName(hwnd, szClass, chDIMOF(szClass));
    GetWindowText(hwnd, szCaption, chDIMOF(szCaption));
    wsprintf(szBufT, TEXT("[%s] %s"), (PTSTR)szClass, 
             (szCaption[0]==0)? (PTSTR)TEXT("(无标题)"):
             (PTSTR)szCaption);
    _tcsncpy_s(szBuf, nLen, szBufT, nLen - 1);
    szBuf[nLen - 1] = 0; //强制0字符串结束
}

//////////////////////////////////////////////////////////////////////////
BOOL Dlg_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam){

    chSETDLGICONS(hwnd, IDI_LISWATCH);

    //周期性的更新内容
    g_uTimerId = SetTimer(hwnd, g_uTimerId, TIMER_DELAY, NULL);

    //使我们的窗口置顶
    SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

    return (TRUE);
}

//////////////////////////////////////////////////////////////////////////
void Dlg_OnCommand(HWND hwnd, int id, HWND hwndCtrl, UINT codeNotity){
    switch (id)
    {
    case IDCANCEL:
        KillTimer(hwnd, g_uTimerId);
        EndDialog(hwnd, id);
        break;
    }
}



//////////////////////////////////////////////////////////////////////////
void Dlg_OnTimer(HWND hwnd, UINT id){
    TCHAR szBuf[100] = TEXT("全系统范围"); //System-wide
    HWND  hwndForeground = GetForegroundWindow(); //获取前景窗口
    DWORD  dwThreadIdAttachTo = g_dwThreadIdAttchTo;

    if (dwThreadIdAttachTo == 0){ //全系统范围
        //如果监视全系统范围的局部输入状态，则将我们的线程
        //Attatch到前景线程
        dwThreadIdAttachTo = GetWindowThreadProcessId(hwndForeground, NULL);
        AttachThreadInput(GetCurrentThreadId(), dwThreadIdAttachTo, TRUE);
    } else {
        wsprintf(szBuf, TEXT("0x%08x"), dwThreadIdAttachTo);
    }

    SetWindowText(GetDlgItem(hwnd, IDC_THREADID), szBuf);

    CalcWndText(GetFocus(), szBuf, chDIMOF(szBuf));
    SetWindowText(GetDlgItem(hwnd, IDC_WNDFOCUS), szBuf);

    CalcWndText(GetActiveWindow(), szBuf, chDIMOF(szBuf));
    SetWindowText(GetDlgItem(hwnd, IDC_WNDACTIVE), szBuf);

    CalcWndText(GetCapture(), szBuf, chDIMOF(szBuf));
    SetWindowText(GetDlgItem(hwnd, IDC_WNDCAPTURE), szBuf);

    CalcWndText(GetForegroundWindow(), szBuf, chDIMOF(szBuf));
    SetWindowText(GetDlgItem(hwnd, IDC_WNDFOREGRND), szBuf);

    if (g_dwThreadIdAttchTo == 0){
        //如果监视全系统范围的局部输入状态，则将本线程与前景线程分离
        AttachThreadInput(GetCurrentThreadId(), dwThreadIdAttachTo, FALSE);
    }
}

//////////////////////////////////////////////////////////////////////////
void Dlg_OnRButtonDown(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT KeyFlags){
    chMB("要监视指定线程，可在主窗口中单击鼠标左键，并在指定的窗口中释放左键.\n"
         "要监视所有线程，可在主窗口中双击鼠标左键.");
}

//////////////////////////////////////////////////////////////////////////
void Dlg_OnLButtonDown(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT KeyFlags){
    //如果我们己Attatch到某个线程，则分离它
    if (g_dwThreadIdAttchTo !=0){
        AttachThreadInput(GetCurrentThreadId(), g_dwThreadIdAttchTo, FALSE);
    }

    //设置鼠标捕获，并改变鼠标形状
    SetCapture(hwnd);
    //HCURSOR hCur = LoadCursor(GetModuleHandle(NULL), MAKEINTRESOURCE(IDC_EYES));
    SetCursor(LoadCursor(GetModuleHandle(NULL), MAKEINTRESOURCE(IDC_EYES)));
}

//////////////////////////////////////////////////////////////////////////
void Dlg_OnLButtonUp(HWND hwnd, int x, int y, UINT KeyFlags){
    if (GetCapture() == hwnd){
        //如果我们设置了鼠标捕获，则获取鼠标下面的窗口的线程Id
        POINT pt;
        pt.x = LOWORD(GetMessagePos());
        pt.y = HIWORD(GetMessagePos());
        ReleaseCapture();

        g_dwThreadIdAttchTo = GetWindowThreadProcessId(
            ChildWindowFromPointEx(GetDesktopWindow(),pt,CWP_SKIPINVISIBLE),NULL);

        if (g_dwThreadIdAttchTo == GetCurrentThreadId()){
            //如果鼠标是在我们的窗口上释放的，则仍监视所有的线程
            g_dwThreadIdAttchTo = 0;
        } else{
            //鼠标是在其他线程创建的窗口上释放，则监视该窗口所属的线程
            AttachThreadInput(GetCurrentThreadId(), g_dwThreadIdAttchTo, TRUE);
        }
    }
}

//////////////////////////////////////////////////////////////////////////
INT_PTR  WINAPI  Dlg_Proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
    
    switch (uMsg)
    {
        chHANDLE_DLGMSG(hWnd, WM_INITDIALOG,    Dlg_OnInitDialog);
        chHANDLE_DLGMSG(hWnd, WM_COMMAND,        Dlg_OnCommand);
        chHANDLE_DLGMSG(hWnd, WM_TIMER,            Dlg_OnTimer);
        chHANDLE_DLGMSG(hWnd, WM_RBUTTONDOWN,    Dlg_OnRButtonDown);
        chHANDLE_DLGMSG(hWnd, WM_LBUTTONDOWN,   Dlg_OnLButtonDown);
        chHANDLE_DLGMSG(hWnd, WM_LBUTTONUP,     Dlg_OnLButtonUp);
    }
    
    return FALSE;
}

//////////////////////////////////////////////////////////////////////////
int WINAPI _tWinMain(HINSTANCE hInstExe, HINSTANCE, PTSTR pszCmdLine, int){
    DialogBox(hInstExe, MAKEINTRESOURCE(IDD_LISWATCH), NULL, Dlg_Proc);
    return (0);
}
```

![复制代码](http://common.cnblogs.com/images/copycode.gif)

//resource.h

 View Code

//28_LISWatch.rc

 View Code

 