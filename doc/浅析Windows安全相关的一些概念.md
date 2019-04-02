# 浅析Windows安全相关的一些概念

2017年01月07日 15:32:10 [wangpengk7788](https://me.csdn.net/wangpengk7788) 阅读数：289



转摘自http://www.cppblog.com/weiym/archive/2013/08/25/202751.html?opt=admin





## Session

我们平常所说的Session是指一次终端登录, 这里的终端登录是指要有自己的显示器和鼠标键盘等， 它包括本地登录和远程登录。在XP时代每次终端登录才会创建一个Session，但是在Vista后所有的服务程序都运行在Session 0， 其他终端会依次运行在session 1, session 2...

## **Logon Session**

登录Session是指不同帐号的登录，它包括System登录， 网络登录及活动交互登录等。 我们在任务管理器里可以看到各种进程运行在不同的帐号下，比如System， Local Service， xxx Account等， 这些帐号有不同的权限。这里要注意区分上面的终端登录Session， 每个终端登录Session内有好几个Logon Session.

## **Window Station**

按MSDN的说法，一个Window Station管理一个剪贴板(Clipboard)，一个原子表(Atom Table)和一组桌面(Desktop)。为什么要有Window Station这个概念？ 实际上每个Window Station对应一个Logon Session， 也就是说通过Window Station， 把不同的帐号进行隔离，防止他们相互影响， 试想其他人在你机器上执行一个DCOM对象，如果没有Window Station隔离，他可以直接操作你的桌面了。一个终端登录Session可以有多个Window Station，但只能有一个可交互的活动Window Station， 也就是Winsta0. 

## **Desktop**



每个Window Station可以创建多个Desktop， 我们平时和3个Desktop打交道比较多(WinLogon, Disconnect, Default), 他们分别代表登录桌面，屏保桌面和我们工作的桌面。我们也可以自己通过CreateDesktop创建桌面， 并通过SwitchDesktop进行切换。

## **Sid**

Sid表示Security Identifier, 它是一串唯一标志符， 它可以表示代表一个帐号， 一个用户组或是一次用户登录等， 具体可以参考

这里

## **Token**

Token和进程相关联， 每个进程创建时都会根据Logon Session权限由LSA（Local Security Authority)分配一个Token(如果CreaeProcess时自己指定了Token,  LSA会用该Token， 否则就用父进程Token的一份拷贝,由大部分进程是由Explorer.exe创建， 所以我们大部分时候都复制了explorer.exe的Token)， 里面含有该进程的安全信息，包括用户帐号， 组信息， 权限信息和默认安全描述符(Security Descriptor)等， 我们可以通过GetTokenInformation查询某个Token的详细情况。具体可以参考[这里](http://msdn.microsoft.com/en-us/library/windows/desktop/aa374909(v=vs.85).aspx)

## **DACL和SACL**

DACL(discretionary access control list)用来标志某个安全对象允许被哪些对象访问。SACL(system access control list )用来记录某个安全对象被访问的情况。具体可以参考

这里

## **Security Descriptor**

每个安全对象在创建时都可以指定一个安全描述符(Security Descriptor), 如果没有指定就用进程默认的， 该描述符指定了哪些对象可以访问该安全对象。大部分情况下我们都是传NULL， 也就是用该进程Token中默认的。具体可以参考[这里](http://msdn.microsoft.com/en-us/library/windows/desktop/aa379563(v=vs.85).aspx)

## Integrity level



这是UAC提供的新特性， 强制完整性控制(Mandatory Integrity Control), 它标志某进程的安全性级别， 安全级别的高低很大程度和该标志相关联。

下面的图表示了Session， Window Station和Desktop的关系：

![img](http://www.cppblog.com/images/cppblog_com/weiym/session_ws.png)

下面的图表示当某程序试图访问某个安全对象时， 系统是如何检测的：系统会检测Object的DACL列表， 根据当前进程的Token，判断当前进程（线程）是否允许访问该Object。

![img](http://www.cppblog.com/images/cppblog_com/weiym/dacl_check.png)

**我们用Process Explorer查看某个进程的属性时， Security页的信息如何理解？** 

![img](http://www.cppblog.com/images/cppblog_com/weiym/tk_se.png)

User和SID项表示创建该进程的用户情况，可以通过GetTokenInformation, 将第二个参数指定成TokenInformationClass来查询。



Session项上面提到过了，表示终端登录session ID，可以通过GetTokenInformation, 将第二个参数指定成TokenSessionId来查询， 也可以通过 API ProcessIDToSessionID来获取



Logon Session表示Logon Session的authority id, 可以通过GetTokenInformation， 将第二个参数指定成TokenStatistics来查询。系统登录的Logon Session id是999(0x3E7), 这里要区分还有一个概念是Logon Session SID， 他们是不同的概念， 前者某种程度上反映了Logon Session的类型， 后则是某次登陆的标志（SID）。



Virtualized是Vista之后才有的概念，表示该程序是否启用了UAC virtualization, 对于没有指定manifest的老程序会使用数据重定向机制。可以通过GetTokenInformation的TokenVirtualizationAllowed/TokenVirtualizationEnabled来查询。



Group是指该用户所在的用户组。我们可以看到尽管我们的用户在Administrators组里，但是上面却显示是Deny的，为什么？因为在Vista之后， UAC打开时， 除非我们显式的Run As Admin， 否则我们的程序都默认运行在标准用户权限下。同时我们注意到上面还有Mandatory label\Medium Mandatory Level项，表示该程序运行的完整性级别， 它包括Untrust， Low， Medium， Hight， System等， 级别越低，权限也就越低。我们可以通过GetTokenInformation的TokenIntegrityLevel来进行查询。



Privilege表示该进程的权限， 我们可以看到好多权限默认是Disabled, 实际上我们可以通过AdjustTokenPrivileges进行提升。 我们可以通过GetTokenInformation的TokenPrivileges进行查询。



## Kernel Object， User Object， GDI Object的使用范围



Kernel Object可以跨进程使用， 如果指定成Global， 还可以跨session.  XP时代即使不指定成Global， 服务程序和普通应用程序也可以通过Kernel Object通讯，但是Vista之后就不行了， 因为他们在不同的Session了。

User Object可以跨进程使用， 但是User Object的使用范围是Window Station， 它不能垮Window Station， 更别说跨session了。我们看不到服务程序弹出的界面， 那是因为服务程序和我们的桌面运行在不同的Window Station， 除非你指定“允许服务程序与桌面交互”， 显式让服务程序运行在活动桌面的Window Station (WinStat0) 。

GDI Object只有在创建它的进程里有效。

**怎样以管理员的身份运行某个程序？**

其实就是右键Run as Admin, UAC打开时会有确认窗口。

::ShellExecute(0, L"runas",L"C:\\Windows\\Notepad.exe",0,0,SW_SHOWNORMAL);

**如何判断当前进程是否运行在管理员账号下？**



这里包含2个概念 一个是运行程序的账号是管理员账号， 另外一个是当前运行的环境是管理员环境。

我们下面的Am_I_In_Admin_Group(TRUE)相当于Windows API IsUserAnAdmin()

//如果

bCheckAdminMode是TRUE， 则除了检测Admin账号外，还检测是真的运行在Admin环境， 否则只是检测Admin账号。



```cpp
BOOL Am_I_In_Admin_Group(BOOL bCheckAdminMode /*= FALSE*/)
{
BOOL   fAdmin;
HANDLE  hThread;
TOKEN_GROUPS *ptg = NULL;
DWORD  cbTokenGroups;
DWORD  dwGroup;
PSID   psidAdmin;
SID_IDENTIFIER_AUTHORITY SystemSidAuthority= SECURITY_NT_AUTHORITY;
if ( !OpenThreadToken ( GetCurrentThread(), TOKEN_QUERY, FALSE, &hThread))
{
if ( GetLastError() == ERROR_NO_TOKEN)
{
if (! OpenProcessToken ( GetCurrentProcess(), TOKEN_QUERY, 
&hThread))
return ( FALSE);
}
else 
return ( FALSE);
}
if ( GetTokenInformation ( hThread, TokenGroups, NULL, 0, &cbTokenGroups))
return ( FALSE);
if ( GetLastError() != ERROR_INSUFFICIENT_BUFFER)
return ( FALSE);
if ( ! ( ptg= (TOKEN_GROUPS*)_alloca ( cbTokenGroups))) 
return ( FALSE);
if ( !GetTokenInformation ( hThread, TokenGroups, ptg, cbTokenGroups,
&cbTokenGroups) )
return ( FALSE);
if ( ! AllocateAndInitializeSid ( &SystemSidAuthority, 2, 
SECURITY_BUILTIN_DOMAIN_RID, 
DOMAIN_ALIAS_RID_ADMINS,
0, 0, 0, 0, 0, 0, &psidAdmin) )
return ( FALSE);
fAdmin= FALSE;
for ( dwGroup= 0; dwGroup < ptg->GroupCount; dwGroup++)
{
if ( EqualSid ( ptg->Groups[dwGroup].Sid, psidAdmin))
{
if(bCheckAdminMode)
{
if((ptg->Groups[dwGroup].Attributes) & SE_GROUP_ENABLED)
{
fAdmin = TRUE;
}
}
else
{
fAdmin = TRUE;
}
break;
}
}
FreeSid ( psidAdmin);
return ( fAdmin);
}
```



**如何提升权限？**

注意只有原来是Disable的权限才可以提成Enable， 如果原来就没有这个权限， 是提

不上去的。

```cpp
BOOL EnablePrivilege(HANDLE hToken, LPCTSTR lpszPrivilegeName)
{
    TOKEN_PRIVILEGES tkp = {0};
    BOOL bRet = LookupPrivilegeValue( NULL, lpszPrivilegeName, &tkp.Privileges[0].Luid );
    if(!bRet) return FALSE;

    tkp.PrivilegeCount = 1;
    tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    bRet = AdjustTokenPrivileges( hToken, FALSE, &tkp, sizeof(tkp), NULL, NULL );

    return bRet;
}
```

## 如何判断用户的进程完整性级别？

该信息包含在Integrity  Level的SID里，通过GetTokenInformation， 第二个参数设置成TokenIntegrityLevel，大概代码如下， 详细可以参考后面下载的源代码。

```cpp
void CIntegrityLevel::Print(std::wostream& os) const
{
    SID* pSid = (SID*)m_pIntegrity->Label.Sid;
    DWORD rid = pSid->SubAuthority[0];

    LPCTSTR lpszIntegrity = L"Unknown";
    switch (rid)
    {
    case SECURITY_MANDATORY_UNTRUSTED_RID:
        {
            lpszIntegrity = L"Untrusted";
            break;
        }
    case SECURITY_MANDATORY_LOW_RID:
        {
            lpszIntegrity = L"Low";
            break;
        }
    case SECURITY_MANDATORY_MEDIUM_RID:
        {
            lpszIntegrity = L"Medium";
            break;
        }

    case SECURITY_MANDATORY_MEDIUM_PLUS_RID:
        {
            lpszIntegrity = L"Medium +";
            break;
        }
    case SECURITY_MANDATORY_HIGH_RID:
        {
            lpszIntegrity = L"High";
            break;
        }
    case SECURITY_MANDATORY_SYSTEM_RID:
        {
            lpszIntegrity = L"System";
            break;
        }
    default:
        {
            lpszIntegrity = L"XXXXX";
        }
    }

    os << L"Integrity: " << lpszIntegrity << endl;
}
```

**如何指定程序默认启动运行的级别？**



在VC里配置Manifest文件。

asInvoker：默认选项，新的进程将简单地继承其父进程的访问令牌

highestAvailable：应用程序会选择该用户允许范围内尽可能高的权限。对于标准用户来说，该选项与asInvoker一样，而对于管理员来说，这就意味着请求Admin令牌。

requireAdministrator：应用程序需要Admin令牌。运行该程序时，标准用户将要输入管理员的用户名和密码，而管理员则要在弹出的确认对话框中进行确认。



上面只是我自己的一些理解和总结， 由于不是专门搞安全相关的， 

如果有不正确的地方， 欢迎指正。



部分资料：

 Designing Applications to Run at a Low Integrity Level

注，这是部分测试代码：

[MySecurityTest](http://www.cppblog.com/Files/weiym/MySecurityTest.rar)