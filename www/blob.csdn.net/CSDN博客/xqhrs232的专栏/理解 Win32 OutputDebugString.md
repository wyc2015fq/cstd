# 理解 Win32 "OutputDebugString" - xqhrs232的专栏 - CSDN博客
2013年03月27日 22:41:24[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1026
原文地址::[http://laokaddk.blog.51cto.com/368606/251050](http://laokaddk.blog.51cto.com/368606/251050)
译：理解 Win32 "OutputDebugString"
坚定的 Win32 开发人员可能对 [OutputDebugString()](http://msdn.microsoft.com/library/en-us/debug/base/outputdebugstring.asp) API
 函数比较熟悉，它可以使你的程序和调试器进行交谈。它要比创建日志文件容易，而且所有“真正的”调试器都能使用它。应用程序和调试器交谈的机制相当简单，而本文将揭示整件事情是如何工作的。
本文首先是由以下事件促使的，我们观察到 OutputDebugString() 在管理员和非管理员用户试图一起工作或游戏时并不总是能可靠地工作（至少在 Win2000 上）。我们怀疑是一些相关的内核对象的权限问题，此间涉略了相当多不得不写下来的信息。
请注意，尽管我们使用了“调试器”这一术语，但不是从调试 API 的意义上来使用的：并没有“单步执行”、“断点”或者“附着到进程”等可以在 MS Visual C 或者一些真正的交互开发环境中找到的东西。从某种意义上来说，任何实现了[协议](http://spaces.msn.com/mmm2005-06-20_17.52/#protocol)的程序都是“调试器”。可能是一个非常小的命令行工具，或者像来自于[SysInternals](http://www.sysinternals.com/) 那帮聪明的家伙们的 [DebugView](http://www.sysinternals.com/ntw2k/freeware/debugview.shtml) 那样的高级货。
### 内容目录
- [应用程序用法](http://spaces.msn.com/mmm2005-06-20_17.52/#appuse)
- [协议](http://spaces.msn.com/mmm2005-06-20_17.52/#protocol)
- [权限问题](http://spaces.msn.com/mmm2005-06-20_17.52/#perms)
- [实现细节](http://spaces.msn.com/mmm2005-06-20_17.52/#impl)
- [胡思乱想](http://spaces.msn.com/mmm2005-06-20_17.52/#random)
- [Unixwiz.net 工具：dbmutex](http://www.unixwiz.net/tools/dbmutex.html)
### 应用程序用法
<windows.h> 文件声明了 OutputDebugString() 函数的两个版本 - 一个用于 ASCII，一个用于 Unicode
 - 不像绝大多数 Win32 API 一样，原始版本是 ASCII。而大多数的 Win32 API 的原始版本是 Unicode。
使用一个 NULL 结尾的字符串缓冲区简单调用 OutputDebugString() 将导致信息出现在调试器中，如果有调试器的话。构建一条信息并发送之的通常用法是：
> sprintf(msgbuf, "Cannot open file %s [err=%ld]\n", fname, GetLastError());
OutputDebugString(msgbuf);
不过在实际环境中我们中的不少人会创建一个前端函数，以允许我们使用 printf 风格的格式化。下面的odprintf() 函数格式化字符串，确保结尾有一个合适的回车换行（删除原来的行结尾），并且发送信息到调试器。
> #include <stdio.h>
#include <stdarg.h>
#include <ctype.h>
void __cdecl odprintf(const char *format, ...)
{
char buf[4096], *p = buf;
va_list args;
va_start(args, format);
p += _vsnprintf(p, sizeof buf - 1, format, args);
va_end(args);
while ( p > buf  &&  isspace(p[-1]) )
*--p = '\0';
*p++ = '\r';
*p++ = '\n';
*p   = '\0';
OutputDebugString(buf);
}
于是在代码中使用它就很简单：
>         ...
odprintf("Cannot open file %s [err=%ld]", fname, GetLastError());
...
我们已经这样使用多年了。
### 协议
在应用程序和调试器之间传递数据是通过一个 4KB 大小的共享内存块完成的，并有一个互斥量和两个事件对象用来保护对他的访问。下面就是相关的四个内核对象：
> |对象名称|对象类型|
|----|----|
|DBWinMutex|Mutex|
|DBWIN_BUFFER|Section （共享内存）|
|DBWIN_BUFFER_READY|Event|
|DBWIN_DATA_READY|Event|
互斥量通常一直保留在系统中，其他三个对象仅当调试器要接收信息才出现。事实上 - 如果一个调试器发现后三个对象已经存在，它会拒绝运行。
当 DBWIN_BUFFER 出现时，会被组织成以下结构。进程 ID 显示信息的来源，字符串数据填充这 4K 的剩余部分。按照约定，信息的末尾总是包括一个 NULL 字节。
> struct dbwin_buffer {
                DWORD   dwProcessId;
                char    data[4096-sizeof(DWORD)];
                };
                
当 OutputDebugString() 被应用调用时，它执行以下步骤。注意在任意位置的错误都将放弃整个事情，调试请求被认为是什么也不做（不会发送字符串）。
- 打开 DBWinMutex 并且等待，直到我们取得了独占访问。
- 映射 DBWIN_BUFFER 段到内存中：如果没有发现，则没有调试器在运行，将忽略整个请求。
- 打开 DBWIN_BUFFER_READY 和 DBWIN_DATA_READY 事件对象。就像共享内存段一样，缺少对象意味着没有可用的调试器。
- 等待 DBWIN_BUFFER_READY 事件对象为有信号状态：表示内存缓冲区不再被占用。大部分时候，这一事件对象一被检查就处于有信号状态，但等待缓冲区就绪不会超过 10 秒（超时将放弃请求）。
- 复制数据直到内存缓冲区中接近 4KB，再保存当前进程 ID。总是放置一个 NULL 字节到字符串结尾。
- 通过设置 DBWIN_DATA_READY 事件对象告诉调试器缓冲区就绪。调试器从那儿取走它。
- 释放互斥量。
- 关闭事件对象和段对象，但保留互斥量的句柄以备后用。
在调试器端会简单一点。互斥量根本不需要，如果事件对象和/或共享内存对象已经存在，则假定其他调试器已经在运行。系统中任意时刻只能存在一个调试器。
- 创建共享内存段以及两个事件对象。如果失败，退出。
- 设置 DBWIN_BUFFER_READY 事件对象，由此应用程序得知缓冲区可用。
- 等待 DBWIN_DATA_READY 事件对象变为有信号状态。
- 从内存缓冲区中提取进程 ID 和 NULL 结尾的字符串。
- 转到步骤 2。
这使我们认为这决不是一种低消耗的发送信息的方法，应用程序的运行速度会受到调试器的左右。
### 权限问题
我们发现 OutputDebugString() 有时不可靠已经好几年了，而且我们十分不解为什么微软这么长时间也没把它搞好。奇怪的是，问题总是围绕着 DBWinMutex 对象出现，这就需要我们察看许可系统以找出为什么会这么麻烦。
互斥量对象会一直存活着直到使用它的最后一个程序关闭其句柄，故而它能在初始创建它的应用程序退出后保留相当长的时间。因为此对象被广泛地共享，所以它必须被赋予明确的许可以允许任何人使用它。事实上，“缺省”许可几乎从不适用，这一问题被计为在 NT 3.51 和 NT 4.0 中[我们观察到的第一个问题](http://support.microsoft.com/default.aspx?scid=kb;en-us;q160603)。
当时的修正方法是使用一个广泛开放的 DACL 创建互斥量，以此来允许任何人访问它，但是看样子在 Win2000 里这些许可被加强了。表面上它看起来是正确的，就像我们在下表中看到的：
> |SYSTEM|MUTEX_ALL_ACCESS|
|----|----|
|Administrators|MUTEX_ALL_ACCESS|
|Everybody|SYNCHRONIZE | READ_CONTROL | MUTEX_QUERY_STATE|
希望发送调试信息的应用只需要等待和获取该互斥量的能力，也即体现为拥有 SYNCHRONIZE 权限。上列的许可对于所有参与的用户都是完全正确的。
不过如果有人观察 [CreateMutex()](http://msdn.microsoft.com/library/en-us/dllproc/base/createmutex.asp) 在对象已经存在时的行为，就会发现奇怪的事情。在这种情况下，Win32
 的表现就好像我们进行了如下调用：
> OpenMutex(MUTEX_ALL_ACCESS, FALSE, "DBWinMutex");
                        
尽管我们确实只需要 SYNCHRONIZE 访问，但它还是假定调用者要做任何事情（MUTEX_ALL_ACCESS）。因为非管理员没有这些权限 - 仅有上列的少许 - 互斥量不能被打开或者获取，于是 OutputDebugString() 不做任何事情就悄悄地返回了。
甚至将所有的软件开发都以管理员来执行也不是一个完整的修正方法：如果存在其他的用户（例如服务）以非管理员运行而许可配置不正确，它们的调试信息将会丢失。
我们感觉真正的修正需要微软为 CreateMutex() 添加一个参数 - 如果对象已经存在时用于隐含的OpenMutex() 调用的访问掩码。也许某天我们会看到一个 CreateMutexEx()，但在此期间我们必须采用另外的方法。代之以，当对象已经存活于内存中时我们将硬性改变其上的许可配置。
这需要调用 [SetKernelObjectSecurity()](http://msdn.microsoft.com/library/en-us/security/security/setkernelobjectsecurity.asp)，下列程序片断展示一个程序如何才能打开互斥量并安装一个新的
 DACL。此 DACL 即使在程序退出后也仍然保持着，只要任一其他程序还维护有它（译者注：应该是指互斥量）的句柄。
> ...
                        // open the mutex that we're going to adjust
                        HANDLE hMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, "DBWinMutex");
                        // create SECURITY_DESCRIPTOR with an explicit, empty DACL
                        // that allows full access to everybody
                        SECURITY_DESCRIPTOR     sd;
                        InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION);
                        SetSecurityDescriptorDacl(
                        &sd,            // addr of SD
                        TRUE,           // TRUE=DACL present
                        NULL,           // ... but it's empty (wide open)
                        FALSE);         // DACL explicitly set, not defaulted
                        // plug in the new DACL
                        SetKernelObjectSecurity(hMutex, DACL_SECURITY_INFORMATION, &sd);
                        ...
                        
这一方法明确地走向了正确的道路，但我们还需要找一个地方来放置此逻辑。把它放在一个一经请求即运行的小程序中是可以的，但是看起来它有可能被中断。我们的办法是写一个 Win32 服务来干这件事情。
[我们的 dbmutex 工具](http://www.unixwiz.net/tools/dbmutex.html)完成的就是这一工作：它在系统引导时启动，打开或者创建互斥量，然后设置对象的安全性以允许广泛的访问。然后休眠直到系统关闭，在此过程中保持互斥量的打开状态。它不消耗任何
 CPU 时间。
### 实现细节
我们花了很多时间使用 [IDA Pro](http://www.datarescue.com/idabase/) 深入到 Windows 2000
 KERNEL32.DLL 的实现中，我们认为，对于它在更精确的基础上到底是如何工作的已经有了良好的掌握。在这儿我们给出 OutputDebugString() 函数的伪代码（我们没有编译过它），以及创建互斥量的函数。
我们故意略去了大多数的错误检查：如果事情变糟了，它将释放所有已分配的资源并退出，就像没有调试器存在一样。目的是展示一般行为而不是对代码的完整的逆向工程。
“setup” 函数 - 名字是我们起的 - 创建互斥量或者在已经存在时打开它。经过一些努力来设置互斥量对象的安全性以使任何人都能用它，尽管我们会看到其实并没有完全正确地得到它。
• [OutputDebugString.txt](http://www.unixwiz.net/techtips/OutputDebugString.txt)
### 胡思乱想
一些人可能会感到这是一个安全性问题，其实并不是。非管理员用户确实拥有适当使用 OutputDebugString()的所有权限，不过由于“请求比所需更多权限”这一常见问题，一个合理的请求因形成了错误的形态而被拒绝了。
但并不像大部分的这种问题那样，这并非是有意的。大多数的错误是开发人员显式请求了更多（如“MUTEX_ALL_ACCESS”），而这次的掩码是由 CreateMutex() 的行为隐含的。这使得如果 Win32 API 不做改动的话更加难于避免。
---
当分析 KERNEL32.DLL 中的 OutputDebugStringA() 时，非管理员如何能够有可能去削弱系统变得明显起来。一旦得到互斥量，一个要发送调试信息的应用会等待 DBWIN_BUFFER_READY 事件对象就绪最多十秒钟，如果超时则放弃。这看起来是一个谨慎的防范措施，如果调试系统忙的话，用以避免被饿死。
但在更早的步骤里，等待互斥量，没有这样的超时设定。如果系统中的任何进程 - 包括非特权进程 - 可以以请求 SYNCHRONIZE 权限打开此互斥量，并且不释放它，所有其他试图获取此互斥量的进程将会无限停止完蛋。
我们的研究表明，所有类型的程序都会发送随意的调试信息（例如，[MusicMatch Jukebox](http://www.musicmatch.com/) 就有一个唠唠叨叨的键盘钩子），这些线程通过很少的几行代码就能停止住。没有必要停止整个程序
 - 可能还有其他的线程 - 但在实际中，开发人员不计划使用 OutputDebugString() 将会是一条拒绝服务之路（译者注：此句没有完全明白，请参看原文）。
---
最奇怪的是，我们发现 OutputDebugString() 并非一个天然的 Unicode 函数。大多数的 Win32 API 具有“真正的”使用了 Unicode 的函数（“W” 版本），如果调用“A”版本的函数则它们自动从 ASCII 转换到 UNICODE。
但是，因为 OutputDebugString 把在内存缓冲区中的数据最终是作为 ASCII 传递到调试器中的，它们具有相反于常规的 A/W 配对。这就暗示了如果要在 Unicode 程序里发送一个快捷信息到调试器，可以通过直接调用 “A” 版本来实现：
> OutputDebugStringA("Got here to place X");


//=============================================================
备注：：
1、这篇文章也提到了OutputDebugString函数的A/W这两个版本的问题！！！
