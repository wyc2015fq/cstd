# 浅析windows安全策略 - 深之JohnChen的专栏 - CSDN博客

2010年04月27日 11:15:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：4678


理论：
本篇我们将通过一个例子，来大致了解下windows的安全策略。

实现windows安全性的安全组件和数据库有：
1。安全引用监视器**[Security reference monitor (简称SRM)](http://hi.baidu.com/combojiang/blog/item/gloss.html#gloss00_270)**   如图，这是位于核心态windows执行体中的一个组件。它负责： 
1）定义访问令牌数据结构来表示一个安全环境。
2）执行对象的安全访问检查
3）管理特权（用户权限)
4) 生成所有的结果安全审计消息。
![](http://hi.csdn.net/attachment/201004/27/0_1272338123Ror6.gif)

2。本地安全权威子系统**[Local security authority subsystem (简称Lsass)](http://hi.baidu.com/combojiang/blog/item/gloss.html#gloss00_160)**这是一个运行/windows/system32/Lsass.exe文件的用户模式进程。它负责：
    1）本地系统安全策略（比如：允许哪些用户登录到本地机器上，口令策略，授予用户和用户组的特权，以及系统安全审计设置
    2）用户认证
    3）发送安全审计消息到事件日志。

在此进程加载了4个策略相关的dll模块，他们是：
netlogon.dll
ntdsapi.dll
samsrv.dll
lsasrv.dll
这四个模块对应于上图中的Netlogon ,Active Directory , LSA server,SAM server.

Netlogon （网络登陆服务），这是一个windows服务，它建立起与域控制器之间的安全通道，对用户和服务进行身份验证。它将用户的凭然后返回用户据传递给域控制器，的域安全标识符和用户权 限。这通常称为 pass-through 身份验证。

Active Directory （活动目录），包括两个方面：目录和与目录相关的服务。目录是存储各种对象的一个物理上的容器，从静态的角度来理解这活动目录与我们以前所结识的“目录”和“文 件夹”没有本质区别，仅仅是一个对象，是一实体；而目录服务是使目录中所有信息和资源发挥作用的服务，活动目录是一个分布式的目录服务，信息可以分散在多 台不同的计算机上，保证用户能够快速访问，因为多台机上有相同的信息，所以在信息容氏方面具有很强的控制能力，正因如此，不管用户从何处访问或信息处在何 处，都对用户提供统一的视图。域是由一组计算机和与他们相关联的安全组构成的，每个安全组被当作单个实体来管理。活动目录存储了有关该域中的对象的信息，这样的对象包括用户、组和计算机。域用户和组的口令信息和特权也被存储在活动目录中。关于活动目录的介绍祥见[http://windows.chinaitlab.com/domain/37480.html](http://windows.chinaitlab.com/domain/37480.html)

SAM (安全帐户管理器服务），它负责管理一个数据库，该数据库包含了本地机器上定义的用户名和组。

LSA （本地安全权威服务），它实现了Lsass子系统的绝大部分功能。

SRM使用一个称为令牌的对象来标识一个进程或线程地安全环境。安全环境中包括的信息描述了与该进程或者线程相关联的特权、帐户和组。
lkd> dt _token
nt!_TOKEN
   +0x000 TokenSource      : _TOKEN_SOURCE
   +0x010 TokenId          : _LUID
   +0x018 AuthenticationId : _LUID
   +0x020 ParentTokenId    : _LUID
   +0x028 ExpirationTime   : _LARGE_INTEGER
   +0x030 TokenLock        : Ptr32 _ERESOURCE
   +0x038 AuditPolicy      : _SEP_AUDIT_POLICY
   +0x040 ModifiedId       : _LUID
   +0x048 SessionId        : Uint4B
   +0x04c UserAndGroupCount : Uint4B
   +0x050 RestrictedSidCount : Uint4B
+0x054 PrivilegeCount   : Uint4B
   +0x058 VariableLength   : Uint4B
   +0x05c DynamicCharged   : Uint4B
   +0x060 DynamicAvailable : Uint4B
   +0x064 DefaultOwnerIndex : Uint4B
   +0x068 UserAndGroups    : Ptr32 _SID_AND_ATTRIBUTES
   +0x06c RestrictedSids   : Ptr32 _SID_AND_ATTRIBUTES
   +0x070 PrimaryGroup     : Ptr32 Void
+0x074 Privileges       : Ptr32 _LUID_AND_ATTRIBUTES
   +0x078 DynamicPart      : Ptr32 Uint4B
   +0x07c DefaultDacl      : Ptr32 _ACL
   +0x080 TokenType        : _TOKEN_TYPE
   +0x084 ImpersonationLevel : _SECURITY_IMPERSONATION_LEVEL
   +0x088 TokenFlags       : Uint4B
   +0x08c TokenInUse       : UChar
   +0x090 ProxyData        : Ptr32 _SECURITY_TOKEN_PROXY_DATA
   +0x094 AuditData        : Ptr32 _SECURITY_TOKEN_AUDIT_DATA
   +0x098 OriginatingLogonSession : _LUID
   +0x0a0 VariablePart     : Uint4B

SRM运行在内核模式下，而Lsass运行在用户模式下，他们利用LPC进行通讯。在系统初始化过程中，SRM创建了一个名为SeRmCommandPort的端口，Lsass连接到此端口上。当Lsass进程启动的时候，它创建一个名为SeLsaCommandPort的LPC端口。SRM连接到此端口上，从而建立起两者之间的私有通讯。
SRM创建了一个共享内存区，用于传递超过256字节长的消息，他在连接调用中把该共享内存区的句柄传递过去。在系统初始化过程中，一旦SRM与Lsass相互连接起来，他们就不再监听各自的连接端口，因此，后来的用户进程无法连接到这些端口，从而避免一些恶意的破坏。

3. 在SDK中，我们使用下面的三个函数来打开令牌，修改权限。
OpenProcessToken，LookupPrivilegeValue，AdjustTokenPrivileges。
每一个进程，线程都有自己的令牌，线程的令牌通常跟所属进程一样。
前面我们讲过 SRM使用一个称为令牌的对象来标识一个进程或线程地安全环境，令牌中的Privileges决定该令牌的线程或进程可以做哪些事情。 如果想让一个进程有更大的权限去做一些事情，我们就可以通过给进程提权的方式来做到。

步骤如下：

1）首先要获得进程访问令牌的句柄，这可以通过**OpenProcessToken**得到，函数的原型如下：
BOOL    **OpenProcessToken**(
          HANDLE    ProcessHandle,    //要修改访问权限的进程句柄
          DWORD    DesiredAccess,        //指定你要进行的操作类型
          PHANDLE    TokenHandle       //返回的访问令牌指针
    )；
通过这个函数我们就可以得到当前进程的访问令牌的句柄（指定函数的第一个参数为GetCurrentProcess()就可以了）。

2）获取权限对应的LUID
根据权限结构体，如下所示:
lkd> dt _LUID_AND_ATTRIBUTES
ntdll!_LUID_AND_ATTRIBUTES
   +0x000 Luid             : _LUID
   +0x008 Attributes       : Uint4B

从上面令牌对象的结构，我们看到。
+0x074 Privileges       : Ptr32 _LUID_AND_ATTRIBUTES   表示一个权限数组。
+0x054 PrivilegeCount   : Uint4B                                        表示了权限数组元素的个数。

对应于SDK中的定义是这样的。
typedef struct _TOKEN_PRIVILEGES {
    DWORD PrivilegeCount;
    LUID_AND_ATTRIBUTES Privileges[ANYSIZE_ARRAY];
} TOKEN_PRIVILEGES, *PTOKEN_PRIVILEGES;

这里把令牌对象中关于权限的两个成员PrivilegeCount和 Privileges合并到了一个结构体TOKEN_PRIVILEGES中。
要对一个任意进程（包括系统安全进程和服务进程）进行指定了写相关的访问权的OpenProcess操作，只要当前进程具有SeDeDebug权限就可以 了。要是一个用户是Administrator或是被给予了相应的权限，就可以具有该权限。可是，就算我们用Administrator帐号对一个系统安 全进程执行OpenProcess(PROCESS_ALL_ACCESS,FALSE,       dwProcessID)还是会遇到“访问拒绝”的错误。什么原因呢？原来在默认的情况下进程的一些访问权限是没有被使能（Enabled）的，所以我们 要做的首先是使能这些权限。例如：我们要给当前进程具有SeDeDebug权限，只要找到Privileges 数组中对应的SeDeDebug权限的那个元素，然后让它使能就可以了。Privileges 数组中的每个元素对应一个结构体如下
typedef struct _LUID_AND_ATTRIBUTES {
    LUID Luid;
    DWORD Attributes;
    } LUID_AND_ATTRIBUTES, * PLUID_AND_ATTRIBUTES;

在windows中权限不是用名称来标识的，而是使用一个LUID来标识的。LUID就是指locally unique identifier，我想GUID大家是比较熟悉的，和GUID的要求保证全局唯一不同，LUID只要保证局部唯一，就是指在系统的每一次运行期间保证 是唯一的就可以了。另外和GUID相同的一点，LUID也是一个64位的值，相信大家都看过GUID那一大串的值，我们要怎么样才能知道一个权限对应的 LUID值是多少呢？这就要用到另外一个API函数LookupPrivilegevalue，其原形如下：
BOOL LookupPrivilegevalue(
LPCTSTR lpSystemName, // system name
LPCTSTR lpName, // privilege name
PLUID lpLuid // locally unique identifier
);
第一个参数是系统的名称，如果是本地系统只要指明为NULL就可以了，第三个参数就是返回LUID的指针，第二个参数就是指明了权限的名称，如“SeDebugPrivilege”。

3) 修改权限，即：将2步骤找到的权限项使能。调用AdjustTokenPrivileges对这个访问令牌进行修改。
AdjustTokenPrivileges的原型如下：
BOOL AdjustTokenPrivileges(
HANDLE TokenHandle, // handle to token
BOOL DisableAllPrivileges, // disabling option
PTOKEN_PRIVILEGES NewState, // privilege information
DWORD BufferLength, // size of buffer
PTOKEN_PRIVILEGES PreviousState, // original state buffer
PDWORD ReturnLength // required buffer size
);
第一个参数是访问令牌的句柄；第二个参数决定是进行权限修改还是Disable所有权限；第三个参数指明要修改的权限，是一个指向 TOKEN_PRIVILEGES结构的指针，该结构包含一个数组，数据组的每个项指明了权限的类型和要进行的操作; 第四个参数是结构PreviousState的长度，如果PreviousState为空，该参数应为NULL；第五个参数也是一个指向 TOKEN_PRIVILEGES结构的指针，存放修改前的访问权限的信息，可空；最后一个参数为实际PreviousState结构返回的大小。

ring3下的这几个函数的访问流程如下：
**1）OpenProcessToken -〉NtOpenProcessToken ->NtOpenProcessTokenEx->ObOpenObjectByPointer 得到进程的Taken对象2）LookupPrivilegeValueA-〉LookupPrivilegeValueW-〉LsaOpenPolicy-〉LsaLookupPrivilegeValue-〉NdrClientCall2 -〉通过LPC与SRM通讯，返回LUID。。。。3）AdjustTokenPrivileges-〉NtAdjustPrivilegesToken-〉修改****Taken对象中相关的值。。。代码：见光盘test ，直接用了鸡蛋壳转载的代码，相关链接[http://bbs.pediy.com/showthread.php?t=70540](http://bbs.pediy.com/showthread.php?t=70540)**#include <Windows.h>
#include <Ntsecapi.h>
#include <Aclapi.h>
#include <tlhelp32.h>
#pragma comment (lib,"Kernel32.lib")
#pragma comment (lib,"Advapi32.lib")
#pragma comment(linker, "/ENTRY:main")

//------------------ 数据类型声明开始 --------------------//
typedef struct _PROCESS_BASIC_INFORMATION {
     NTSTATUS ExitStatus;
     ULONG PebBaseAddress;
     ULONG_PTR AffinityMask;
     LONG BasePriority;
     ULONG_PTR UniqueProcessId;
     ULONG_PTR InheritedFromUniqueProcessId;
} PROCESS_BASIC_INFORMATION;

typedef PROCESS_BASIC_INFORMATION *PPROCESS_BASIC_INFORMATION;

typedef struct _SYSTEM_HANDLE_INFORMATION
{
     ULONG             ProcessId;
     UCHAR             ObjectTypeNumber;
     UCHAR             Flags;
     USHORT             Handle;
     PVOID             Object;
     ACCESS_MASK         GrantedAccess;
} SYSTEM_HANDLE_INFORMATION, *PSYSTEM_HANDLE_INFORMATION;

typedef struct _SYSTEM_MODULE_INFORMATION 
{
    ULONG Reserved[2];
    PVOID Base;
    ULONG Size;
    ULONG Flags;
    USHORT Index;
    USHORT Unknown;
    USHORT LoadCount;
    USHORT ModuleNameOffset;
    CHAR ImageName[256];
} SYSTEM_MODULE_INFORMATION, *PSYSTEM_MODULE_INFORMATION;

typedef struct _OBJECT_ATTRIBUTES 
{
    ULONG Length;
    HANDLE RootDirectory;
    PUNICODE_STRING ObjectName;
    ULONG Attributes;
    PVOID SecurityDescriptor;
    PVOID SecurityQualityOfService;
} OBJECT_ATTRIBUTES, *POBJECT_ATTRIBUTES;

typedef enum _SECTION_INHERIT 
{
    ViewShare = 1,
    ViewUnmap = 2
} SECTION_INHERIT;

typedef struct _MY_PROCESS_INFO 
{
    ULONG PID;
    ULONG KPEB;
    ULONG CR3;
    CHAR Name[16];
    ULONG Reserved;
} MY_PROCESS_INFO, *PMY_PROCESS_INFO;

typedef struct _CLIENT_ID 
{
     HANDLE UniqueProcess;
     HANDLE UniqueThread;
} CLIENT_ID;
typedef CLIENT_ID *PCLIENT_ID;

typedef long NTSTATUS;

//------------------ 数据类型声明结束 --------------------//

//--------------------- 预定义开始 -----------------------//
#define NT_SUCCESS(Status)         ((NTSTATUS)(Status) >= 0)
#define STATUS_SUCCESS              0x00000000
#define STATUS_UNSUCCESSFUL         0xC0000001
#define STATUS_NOT_IMPLEMENTED      0xC0000002
#define STATUS_INFO_LENGTH_MISMATCH 0xC0000004
#define STATUS_INVALID_PARAMETER    0xC000000D
#define STATUS_ACCESS_DENIED        0xC0000022
#define STATUS_BUFFER_TOO_SMALL     0xC0000023
#define OBJ_KERNEL_HANDLE           0x00000200
#define SystemModuleInformation     11
#define SystemHandleInformation     0x10

#define InitializeObjectAttributes( p, n, a, r, s ) { (p)->Length = sizeof( OBJECT_ATTRIBUTES );(p)->RootDirectory = r;                 (p)->Attributes = a;                     (p)->ObjectName = n;                       (p)->SecurityDescriptor = s;                 (p)->SecurityQualityOfService = NULL;         }
//--------------------- 预定义结束 -----------------------//

//------------------ Native API声明开始 ------------------//

typedef DWORD (_stdcall *XXXZwQuerySystemInformation)(
                ULONG SystemInformationClass,
                PVOID SystemInformation,
                ULONG SystemInformationLength,
                PULONG ReturnLength
                );

typedef DWORD (_stdcall *XXXZwOpenProcess)(
              OUT PHANDLE             ProcessHandle,
              IN ACCESS_MASK           AccessMask,
              IN POBJECT_ATTRIBUTES   ObjectAttributes,
              IN PCLIENT_ID           ClientId
              );

typedef DWORD (_stdcall *XXXZwAllocateVirtualMemory)(
               IN HANDLE               ProcessHandle,
               IN OUT PVOID             *BaseAddress,
               IN ULONG                 ZeroBits,
               IN OUT PULONG           RegionSize,
               IN ULONG                 AllocationType,
               IN ULONG                 Protect
               );

typedef DWORD (_stdcall *XXXZwDuplicateObject)(
               IN HANDLE               SourceProcessHandle,
               IN PHANDLE               SourceHandle,
               IN HANDLE               TargetProcessHandle,
               OUT PHANDLE             TargetHandle,
               IN ACCESS_MASK           DesiredAccess OPTIONAL,
               IN BOOLEAN               InheritHandle,
               IN ULONG                 Options
               );

typedef DWORD (_stdcall *XXXZwQueryInformationProcess)(
                 IN HANDLE               ProcessHandle,
                 IN PVOID          ProcessInformationClass,
                 OUT PVOID               ProcessInformation,
                 IN ULONG                 ProcessInformationLength,
                 OUT PULONG               ReturnLength
                 );

typedef DWORD (_stdcall *XXXZwProtectVirtualMemory)(

              IN HANDLE               ProcessHandle,
              IN OUT PVOID             *BaseAddress,
              IN OUT PULONG           NumberOfBytesToProtect,
              IN ULONG                 NewAccessProtection,
              OUT PULONG               OldAccessProtection
              );

typedef DWORD (_stdcall *XXXZwWriteVirtualMemory)(
               IN HANDLE               ProcessHandle,
               IN PVOID                 BaseAddress,
               IN PVOID                 Buffer,
               IN ULONG                 NumberOfBytesToWrite,
               OUT PULONG               NumberOfBytesWritten OPTIONAL
               );

typedef DWORD (_stdcall *XXXZwClose)(
           IN HANDLE               ObjectHandle
           );

typedef DWORD (_stdcall *XXXZwFreeVirtualMemory)(

              IN HANDLE               ProcessHandle,
              IN PVOID                 *BaseAddress,
              IN OUT PULONG           RegionSize,
              IN ULONG                 FreeType
              );

//------------------ Native API声明结束 ------------------//

//------------------ 程序正式开始 ------------------//

DWORD GetPidByName(char *szName)
{
    HANDLE hProcessSnap = INVALID_HANDLE_VALUE;
    PROCESSENTRY32 pe32={0};
    DWORD dwRet=0;

    hProcessSnap =CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if(hProcessSnap == INVALID_HANDLE_VALUE)return 0;

    pe32.dwSize = sizeof(PROCESSENTRY32);
    if(Process32First(hProcessSnap, &pe32))
    {
       do
       {
            if(lstrcmpi(szName,pe32.szExeFile)==0)
            {
                dwRet=pe32.th32ProcessID;
                break;
            }
       }while (Process32Next(hProcessSnap,&pe32));
    }
    else 
        return 0;

    if(hProcessSnap !=INVALID_HANDLE_VALUE)
        CloseHandle(hProcessSnap);

    return dwRet;
}

void KillIce(ULONG dwProcessId)
{
    HMODULE hNTDLL = LoadLibrary ("ntdll.dll");
    HANDLE      ph, h_dup;
    ULONG      bytesIO;
    PVOID      buf;
    ULONG         i;
    CLIENT_ID     cid1;
    OBJECT_ATTRIBUTES     attr;
    HANDLE         csrss_id;
    //   HANDLE     SnapShotHandle;
    PROCESS_BASIC_INFORMATION     pbi;
    PVOID         p0, p1;
    ULONG         sz, oldp;
    ULONG         NumOfHandle;
    PSYSTEM_HANDLE_INFORMATION     h_info; 

    csrss_id = (HANDLE)GetPidByName("csrss.exe");
    attr.Length = sizeof(OBJECT_ATTRIBUTES);
    attr.RootDirectory = 0;
    attr.ObjectName = 0;
    attr.Attributes = 0;
    attr.SecurityDescriptor = 0;
    attr.SecurityQualityOfService = 0;

    cid1.UniqueProcess = csrss_id;
    cid1.UniqueThread = 0;
    XXXZwOpenProcess ZwOpenProcess;
    ZwOpenProcess = (XXXZwOpenProcess)GetProcAddress( GetModuleHandle("ntdll.dll"), "ZwOpenProcess");
    ZwOpenProcess(&ph, PROCESS_ALL_ACCESS, &attr, &cid1);

    bytesIO = 0x400000;
    buf = 0;
    XXXZwAllocateVirtualMemory ZwAllocateVirtualMemory;
    ZwAllocateVirtualMemory = (XXXZwAllocateVirtualMemory)GetProcAddress(GetModuleHandle("ntdll.dll"), "ZwAllocateVirtualMemory");
    ZwAllocateVirtualMemory(GetCurrentProcess(), &buf, 0, &bytesIO, MEM_COMMIT, PAGE_READWRITE);

    XXXZwQuerySystemInformation ZwQuerySystemInformation;
    ZwQuerySystemInformation = (XXXZwQuerySystemInformation)GetProcAddress( GetModuleHandle("ntdll.dll"), "ZwQuerySystemInformation");
    ZwQuerySystemInformation(SystemHandleInformation, buf, 0x400000, &bytesIO);
    NumOfHandle = (ULONG)buf;
    h_info = ( PSYSTEM_HANDLE_INFORMATION )((ULONG)buf+4);

    for (i= 0 ; i<NumOfHandle; i++, h_info++)
    {   
       if ((h_info->ProcessId == (ULONG)csrss_id)&&(h_info->ObjectTypeNumber == 5))
       {
            XXXZwDuplicateObject ZwDuplicateObject;
            ZwDuplicateObject = (XXXZwDuplicateObject)GetProcAddress(GetModuleHandle("ntdll.dll"), "ZwDuplicateObject");
            XXXZwQueryInformationProcess ZwQueryInformationProcess;
            ZwQueryInformationProcess = (XXXZwQueryInformationProcess)GetProcAddress(GetModuleHandle("ntdll.dll"), "ZwQueryInformationProcess");
            if (ZwDuplicateObject(ph, (PHANDLE)h_info->Handle, (HANDLE)-1, &h_dup,
                                       0, 0, DUPLICATE_SAME_ACCESS) == STATUS_SUCCESS)
                 ZwQueryInformationProcess(h_dup, 0, &pbi, sizeof(pbi), &bytesIO);
            if (pbi.UniqueProcessId == dwProcessId)
            {
                MessageBox(0, "目标已确定!", "OK", MB_OK);
                XXXZwProtectVirtualMemory ZwProtectVirtualMemory;
                ZwProtectVirtualMemory = (XXXZwProtectVirtualMemory)GetProcAddress(GetModuleHandle("ntdll.dll"), "ZwProtectVirtualMemory");
                XXXZwWriteVirtualMemory ZwWriteVirtualMemory;
                ZwWriteVirtualMemory = (XXXZwWriteVirtualMemory)GetProcAddress(GetModuleHandle("ntdll.dll"), "ZwWriteVirtualMemory");
                XXXZwClose ZwClose;
                ZwClose = (XXXZwClose)GetProcAddress(GetModuleHandle("ZwClose"), "ZwClose");
                for (i = 0x1000; i<0x80000000; i = i + 0x1000)
                {
                    p0 = (PVOID)i;
                    p1 = p0;
                    sz = 0x1000;
                    if (ZwProtectVirtualMemory(h_dup, &p1, &sz, PAGE_EXECUTE_READWRITE, &oldp) == STATUS_SUCCESS)
                    {              
                           ZwWriteVirtualMemory(h_dup, p0, buf, 0x1000, &oldp);
                    }          
                }
                MessageBox(0, "任务已完成！","OK", 0);
                ZwClose(h_dup);     
                break;
            }
       }
    }

    bytesIO = 0;
    XXXZwFreeVirtualMemory ZwFreeVirtualMemory;
    ZwFreeVirtualMemory = (XXXZwFreeVirtualMemory)GetProcAddress(GetModuleHandle("ntdll.dll"),   "ZwFreeVirtualMemory");
    ZwFreeVirtualMemory(GetCurrentProcess(), &buf, &bytesIO, MEM_RELEASE);

    FreeLibrary(hNTDLL);   

}

BOOL EnablePrivilege(HANDLE hToken,LPCTSTR szPrivName,BOOL fEnable)
{
    TOKEN_PRIVILEGES tp;
    tp.PrivilegeCount = 1;
    LookupPrivilegeValue(NULL,szPrivName,&tp.Privileges[0].Luid);

    tp.Privileges[0].Attributes = fEnable ? SE_PRIVILEGE_ENABLED:0;
    AdjustTokenPrivileges(hToken,FALSE,&tp,sizeof(tp),NULL,NULL);
    return((GetLastError() == ERROR_SUCCESS));
}

void main()
{    
     ULONG Pid;
     HANDLE hToken;
     OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES,&hToken);
     EnablePrivilege(hToken,SE_DEBUG_NAME,TRUE);
     if (Pid = GetPidByName("calc.exe"))
     {
         KillIce(Pid);
     }     
     ExitProcess(0);
}

分析： 以下是windows NT中定义的权限：
#define SE_CREATE_TOKEN_NAME              TEXT("SeCreateTokenPrivilege")
#define SE_ASSIGNPRIMARYTOKEN_NAME        TEXT("SeAssignPrimaryTokenPrivilege")
#define SE_LOCK_MEMORY_NAME               TEXT("SeLockMemoryPrivilege")
#define SE_INCREASE_QUOTA_NAME            TEXT("SeIncreaseQuotaPrivilege")
#define SE_UNSOLICITED_INPUT_NAME         TEXT("SeUnsolicitedInputPrivilege")
#define SE_MACHINE_ACCOUNT_NAME           TEXT("SeMachineAccountPrivilege")
#define SE_TCB_NAME                       TEXT("SeTcbPrivilege")
#define SE_SECURITY_NAME                  TEXT("SeSecurityPrivilege")
#define SE_TAKE_OWNERSHIP_NAME            TEXT("SeTakeOwnershipPrivilege")
#define SE_LOAD_DRIVER_NAME               TEXT("SeLoadDriverPrivilege")
#define SE_SYSTEM_PROFILE_NAME            TEXT("SeSystemProfilePrivilege")
#define SE_SYSTEMTIME_NAME                TEXT("SeSystemtimePrivilege")
#define SE_PROF_SINGLE_PROCESS_NAME       TEXT("SeProfileSingleProcessPrivilege")
#define SE_INC_BASE_PRIORITY_NAME         TEXT("SeIncreaseBasePriorityPrivilege")
#define SE_CREATE_PAGEFILE_NAME           TEXT("SeCreatePagefilePrivilege")
#define SE_CREATE_PERMANENT_NAME          TEXT("SeCreatePermanentPrivilege")
#define SE_BACKUP_NAME                    TEXT("SeBackupPrivilege")
#define SE_RESTORE_NAME                   TEXT("SeRestorePrivilege")
#define SE_SHUTDOWN_NAME                  TEXT("SeShutdownPrivilege")
#define SE_DEBUG_NAME                     TEXT("SeDebugPrivilege")
#define SE_AUDIT_NAME                     TEXT("SeAuditPrivilege")
#define SE_SYSTEM_ENVIRONMENT_NAME        TEXT("SeSystemEnvironmentPrivilege")
#define SE_CHANGE_NOTIFY_NAME             TEXT("SeChangeNotifyPrivilege")
#define SE_REMOTE_SHUTDOWN_NAME           TEXT("SeRemoteShutdownPrivilege")
#define SE_UNDOCK_NAME                    TEXT("SeUndockPrivilege")
#define SE_SYNC_AGENT_NAME                TEXT("SeSyncAgentPrivilege")
#define SE_ENABLE_DELEGATION_NAME         TEXT("SeEnableDelegationPrivilege")
#define SE_MANAGE_VOLUME_NAME             TEXT("SeManageVolumePrivilege")
#define SE_IMPERSONATE_NAME               TEXT("SeImpersonatePrivilege")
#define SE_CREATE_GLOBAL_NAME             TEXT("SeCreateGlobalPrivilege")

其中的SE_DEBUG_NAME 权限是这样描述的，如果调用者有这个特权的话，则进程管理器允许通过NtOpenProcess访问任何一个进程，而不考虑该进程的安全描述符是否允许。
代码如下：
     OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES,&hToken);
     EnablePrivilege(hToken,SE_DEBUG_NAME,TRUE);

其中：EnablePrivilege函数实现了提权。
BOOL EnablePrivilege(HANDLE hToken,LPCTSTR szPrivName,BOOL fEnable)
{
    TOKEN_PRIVILEGES tp;
    //设置特权数组的元素个数
    tp.PrivilegeCount = 1;

    // 取得szPrivName对应的[LUID](本地唯一的标识符)值
    LookupPrivilegeValue(NULL,szPrivName,&tp.Privileges[0].Luid);

    //设置[LUID]的属性值
    tp.Privileges[0].Attributes = fEnable ? SE_PRIVILEGE_ENABLED:0;

    // 为当前进程取得关闭系统的特权
    AdjustTokenPrivileges(hToken,FALSE,&tp,sizeof(tp),NULL,NULL);
    return((GetLastError() == ERROR_SUCCESS));
}

由于本例的作用是采用内存清0的方式关闭目标进程的。所以当前进程需要先有这个权限才行。
下面GetPidByName函数的作用是根据进程文件名，获取进程的ID。这个ID用于后续打开的进程。

DWORD GetPidByName(char *szName)
{
    HANDLE hProcessSnap = INVALID_HANDLE_VALUE;
    PROCESSENTRY32 pe32={0};
    DWORD dwRet=0;

    hProcessSnap =CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if(hProcessSnap == INVALID_HANDLE_VALUE)return 0;

    pe32.dwSize = sizeof(PROCESSENTRY32);
    if(Process32First(hProcessSnap, &pe32))
    {
       do
       {
            if(lstrcmpi(szName,pe32.szExeFile)==0)
            {
                dwRet=pe32.th32ProcessID;
                break;
            }
       }while (Process32Next(hProcessSnap,&pe32));
    }
    else 
        return 0;

    if(hProcessSnap !=INVALID_HANDLE_VALUE)
        CloseHandle(hProcessSnap);

    return dwRet;
}

这里用到了3个关键的函数：**CreateToolhelp32Snapshot**()，Process32First()和Process32Next()。下面给出了关于这三个函数的原形和参数说明;

HANDLE WINAPI **CreateToolhelp32Snapshot**(
DWORD dwFlags, //系统快照要查看的信息类型
DWORD th32ProcessID      //值0表示当前进程
);
BOOL WINAPI Process32First(
HANDLE hSnapshot,        //**CreateToolhelp32Snapshot**()创建的快照句柄
LPPRO[CES](http://smb.zol.com.cn/topic/589883.html)SENTRY32 lppe //指向进程入口结构
);
BOOL WINAPI Process32Next(
HANDLE hSnapshot,        //这里参数同Process32First
LPPROCESSENTRY32 lppe //同上
);

首先使用**CreateToolhelp32Snapshot**()创建系统快照句柄（hProcessSnap 是我们声明用来保存创建的快照句柄）
hProcessSnap =CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
然后调用Process32First()获得系统快照中的第一个进程信息。
if(Process32First(hProcessSnap, &pe32))
接着用一个循环调用来遍历系统中所有运行的进程。一旦找到就返回进程的ID。
           if(lstrcmpi(szName,pe32.szExeFile)==0)
            {
                dwRet=pe32.th32ProcessID;
                break;
            }

后面的代码工作原理是：
首先了解下内核对象及句柄
内核对象是由系统分配的一块内存，必须有系统来维护和访问。windows 系统内核的对象管理器，管理着所有的内核对象，象进程、线程、文件、文件映射、事件、互斥量、信号量、管道、邮槽等都是内核对象。
内核对象是系统级的，独立于进程。也就是说，任何一个进程创建了一个内核对象，理论上讲，其他进程都应该能访问，只要进程获得该内核对象的句柄。事实上，NT就是通过对句柄的种种 限制，来达到安全的目的。 在windows系统中，每一个进程都维护一内核对象句柄表。对象句柄是一个索引，指向与进程相关的句柄表中的表项。一个进程的句柄表包含了所有已被进程打开的那些对象的指针。

我们可以使用**ZwQuerySystemInformation**函数获取所有核心句柄表，这个句柄表，包含了所有的内核对象的句柄。从遍历出来的结果看，它并不能遍历出所有的进程来。我们通过如下代码，测试输出结果。
由于进程的内核对象类型是5，因此我们可以用这样的过滤条件，打印出所有遍历到的进程信息。

    char bufjiang[100] = {0};
   wsprintf(bufjiang,"ProcessID = %d,ObjectType = %d, handle = %#x, object = %#x/n",
            h_info->ProcessId,h_info->ObjectTypeNumber,h_info->Handle,h_info->Object );
        if(h_info->ObjectTypeNumber == 5)
            OutputDebugString(bufjiang);

所以折中的办法就是，由于csrss.exe是Win32子系统进程，在CSRSS.EXE 进程中保留有一份完整的系统所有进程/线程句柄，因此，我们可以通过csrss.exe中的句柄表，来找出所有运行的进程的句柄来。

于是需要打开CSRSS.EXE 进程，获取其进程ID,
csrss_id = (HANDLE)GetPidByName("csrss.exe");
......
ZwOpenProcess(&ph, PROCESS_ALL_ACCESS, &attr, &cid1);

然后，根据下面的判断过滤内核对象类型是5过滤的情况。

if ((h_info->ProcessId == (ULONG)csrss_id)&&(h_info->ObjectTypeNumber == 5))
       {

然后从**CSRSS.EXE **进程中的系统进程/线程**表**中使用ZwDuplicateObject复制出一份找到的进程句柄，
if (ZwDuplicateObject(ph, (PHANDLE)h_info->Handle, (HANDLE)-1, &h_dup,
                                       0, 0, DUPLICATE_SAME_ACCESS) == STATUS_SUCCESS)

然后，有了进程句柄，我们就可以获取进程的信息。
 ZwQueryInformationProcess(h_dup, 0, &pbi, sizeof(pbi), &bytesIO);
得到的进程信息，对应如下结构。
typedef struct _PROCESS_BASIC_INFORMATION {
     NTSTATUS ExitStatus;
     ULONG PebBaseAddress;
     ULONG_PTR AffinityMask;
     LONG BasePriority;
     ULONG_PTR UniqueProcessId;
     ULONG_PTR InheritedFromUniqueProcessId;
} PROCESS_BASIC_INFORMATION;
接下来，我们继续判断这个进程是不是我们要关闭的目标进程。
if (pbi.UniqueProcessId == dwProcessId)
            {
找到目标进程后，我们就可以用0填充进程的用户内存空间了。
for (i = 0x1000; i<0x80000000; i = i + 0x1000)
                {
                    p0 = (PVOID)i;
                    p1 = p0;
                    sz = 0x1000;
                    if (ZwProtectVirtualMemory(h_dup, &p1, &sz, PAGE_EXECUTE_READWRITE, &oldp) == STATUS_SUCCESS)
                    {              
                           ZwWriteVirtualMemory(h_dup, p0, buf, 0x1000, &oldp);
                    }          
                }

其中 ：    ZwProtectVirtualMemory函数的作用是修改内存块的属性为可读可写可执行。
            ZwWriteVirtualMemory函数的作用是写内存块。这里在内存块中写入0。

最后附上：内核对象类型的定义，上面用到的进程对象类型5 就是OB_TYPE_PROCESS。
#define OB_TYPE_TYPE                    1
#define OB_TYPE_DIRECTORY               2
#define OB_TYPE_SYMBOLIC_LINK           3
#define OB_TYPE_TOKEN                   4
#define OB_TYPE_PROCESS                 5
#define OB_TYPE_THREAD                  6
#define OB_TYPE_EVENT                   7
#define OB_TYPE_EVENT_PAIR              8
#define OB_TYPE_MUTANT                  9
#define OB_TYPE_SEMAPHORE               10
#define OB_TYPE_TIMER                   11
#define OB_TYPE_PROFILE                 12
#define OB_TYPE_WINDOW_STATION          13
#define OB_TYPE_DESKTOP                 14
#define OB_TYPE_SECTION                 15
#define OB_TYPE_KEY                     16
#define OB_TYPE_PORT                    17
#define OB_TYPE_ADAPTER                 18
#define OB_TYPE_CONTROLLER              19
#define OB_TYPE_DEVICE                  20
#define OB_TYPE_DRIVER                  21
#define OB_TYPE_IO_COMPLETION           22
#define OB_TYPE_FILE                    23

文章来源于:http://hi.baidu.com/combojiang/blog/item/7c32ff2dc1ce7932359bf732.html


