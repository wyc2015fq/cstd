
#ifndef _LOADDLL_H_
#define _LOADDLL_H_

#include <windows.h>
/*
LOADDLL_BEGIN()
*/
static HINSTANCE hInsts[ 100 ] = {0};
static unsigned hInstslen = 0;

static void unloadalldll(void)
{
  unsigned i;

  for (i = 0; i < hInstslen; ++i) {
    if (NULL != hInsts[ i ]) {
      FreeLibrary(hInsts[ i ]);
      hInsts[ i ] = NULL;
    }
  }

  hInstslen = 0;
}

#define LOADDLL_BEGIN(_DLLNAME) do { HINSTANCE _hInst = LoadLibrary(_DLLNAME); if(_hInst) { hInsts[hInstslen++] = _hInst;
#define GETFUN(_FUNNAME)  GetProcAddress(_hInst, _FUNNAME)
#define GETDLLFUN(_FUNNAME, _FTYPE)  ((_FTYPE)GetProcAddress(_hInst, _FUNNAME))
#define LOADDLL_END()  atexit(unloadalldll);  } } while(0)


#if 0
// 直接载入内存中的DLL

　　你可能不希望在发布程序时附带上一个外部的 DLL，因为可能会有些用户在无意中把 DLL 删除了而造成 EXE 不能正确运行，也有可能该 DLL 会被别人拿去使用，也有可能，此 DLL 会成为破解者破解你的程序的突破口。无论出于何种原因，如果你想把一个 DLL 合并到一个 EXE 中的话，本文向你介绍这种方法。

Win32 程序调用 DLL 的机制

　　 Win32 EXE 在调用一个外部 DLL 中的函数时，首先要调用 LoadLibary 函数来载入此 DLL 到程序的进程地址空间。 如果 LoadLibary 载入此 DLL 成功，将返回一个该 DLL 的句柄。 这个句柄实际上就是该 DLL 在内存中的起始地址。 在载入 DLL 成功后，还必须调用 GetProcAddress 函数来获取要调用的函数的地址。然后再根据该地址来调用这个函数。
根据上述原理，我们可以把一个 DLL 作为资源文件放到 EXE 文件中，在程序运行时，分配一块内存，然后将此资源复制到该分配的内存中，并根据该内存地址计算得到相关的导出函数地址，然后，当我们需要调用某一函数时，可以用该函数在内存中的地址来调用它。
程序实现。
　　首先，把要合并的 DLL 作为资源加入到项目的资源文件中，然后在程序运行时，从资源中载入该资源，以得到该 DLL 在内存中的位置：
LPVOID sRawDll; // 资源文件在内存中的地址
HRSRC hRes;
HMODULE hLibrary;
HGLOBAL hResourceLoaded;
char lib_name[MAX_PATH];
GetModuleFileName(hInstance, lib_name, MAX_PATH);  // 得到运行程序的名字
hLibrary = LoadLibrary(lib_name);                  // 就象载入一个 DLL 一样载入运行程序到内存中

if (NULL != hLibrary)
{
  // 得到指定的资源文件在内存中的位置
  hRes = FindResource(hLibrary, MAKEINTRESOURCE(IDR_DATA1), RT_RCDATA);

  if (NULL != hRes) {
    // 将资源文件载入内存
    hResourceLoaded = LoadResource(hLibrary, hRes);

    if (NULL != hResourceLoaded) {
      // 得到资源文件大小
      SizeofResource(hLibrary, hRes);

      // 锁定资源以得到它在内存中的地址
      sRawDll = (LPVOID)LockResource(hResourceLoaded);
    }
  }
  else {
    return 1;
  }

  FreeLibrary(hLibrary);
}
else
{
  return 1;
}

然后，从资源中载入 DLL 到内存函数 LoadPbDllFromMemory 将载入 DLL 到内存中， 该函数有两个参数，第一个参数是指向 DLL 资源在内存中的地址的指针，也就是前面代码中的 LockResource 函数的返回值。第二个参数是一个空的指针，如果函数 LoadPbDllFromMemory 运行成功，该指针将指向重新组合后的内存中的 DLL 的起始地址。该函数还有一个功能就是如果运行成功，它将手动地用 DLL_PROCESS_ATTACH 参数调用 DLL 的入口函数 DllMain 来初始化该 DLL。除此之外，它还会手动地载入合并的 DLL 的入口表中导入的 DLL 并调整它们在内存中的相对地址。以下是该函数代码:

DWORD LoadPbDllFromMemory(LPVOID lpRawDll, LPVOID lpImageDll)
{
  SYSTEM_INFO sSysInfo;
  PIMAGE_DOS_HEADER dosHeader;
  PIMAGE_NT_HEADERS pNTHeader;
  PIMAGE_SECTION_HEADER section;
  PIMAGE_IMPORT_DESCRIPTOR pImportDesc;
  PIMAGE_IMPORT_BY_NAME pOrdinalName;
  PIMAGE_BASE_RELOCATION baseReloc;
  PDWORD lpLink;
  unsigned char Protection[4096];
  HINSTANCE hDll;
  WORD i;
  DWORD ImagePages, fOldProtect, j, MaxLen, HdrLen, Addr1, Addr2, Pg, Pg1, Pg2;
  char* sDllName;
  int HeaderSize;
  DWORD NeededMemory;
  int ii;
  DWORD OrgAddr, NewAddr, Size;
  DWORD importsStartRVA;
  PBYTE pImageBase;

  if (NULL == lpRawDll) {
    return 1 ;
  }

  dosHeader = (PIMAGE_DOS_HEADER)lpRawDll;

  // Is this the MZ header?
  if ((TRUE == IsBadReadPtr(dosHeader, sizeof(IMAGE_DOS_HEADER))) ||
      (IMAGE_DOS_SIGNATURE != dosHeader->e_magic)) {
    return 2;
  }

#define MakePtr(cast, ptr, addvalue)   (cast)((DWORD)(ptr)+(DWORD)(addvalue))

  // Get the PE header.
  pNTHeader = MakePtr(PIMAGE_NT_HEADERS, dosHeader, dosHeader->e_lfanew);

  // Is this a real PE image?
  if ((TRUE == IsBadReadPtr(pNTHeader, sizeof(IMAGE_NT_HEADERS))) ||
      (IMAGE_NT_SIGNATURE != pNTHeader->Signature)) {
    return 3 ;
  }

  if ((pNTHeader->FileHeader.SizeOfOptionalHeader !=
      sizeof(pNTHeader->OptionalHeader)) ||
      (pNTHeader->OptionalHeader.Magic != IMAGE_NT_OPTIONAL_HDR32_MAGIC)) {
    return 4;
  }

  if (pNTHeader->FileHeader.NumberOfSections < 1) {
    return 5;
  }

  section = IMAGE_FIRST_SECTION(pNTHeader);
  HeaderSize = sizeof(IMAGE_SECTION_HEADER);

  // 节头长度
  HdrLen = (DWORD)section - (DWORD)dosHeader +
      HeaderSize * pNTHeader->FileHeader.NumberOfSections;

  // 找出最大的节的长度,此节一般是代码所在的节(.text 节)
  MaxLen = HdrLen;
  ii = 0;

  for (i = 0; i < (DWORD)pNTHeader->FileHeader.NumberOfSections; i++) { // find MaxLen
    if (MaxLen < section[i].VirtualAddress + section[i].SizeOfRawData) {
      MaxLen = section[i].VirtualAddress + section[i].SizeOfRawData;
    }

    if (strcmp((const char*)section[i].Name, ".rsrc") == 0) {
      ii = i;
    }
  }

  GetSystemInfo(&sSysInfo);
  ImagePages = MaxLen / sSysInfo.dwPageSize;

  if (MaxLen % sSysInfo.dwPageSize) {
    ImagePages++;
  }

  // 分配所需的内存
  NeededMemory = ImagePages * sSysInfo.dwPageSize;
  lpImageDll = VirtualAlloc(NULL, NeededMemory, MEM_COMMIT, PAGE_EXECUTE_READWRITE);

  if (lpImageDll == NULL) {
    return 6;  // 分配内存失败
  }

  MoveMemory(lpImageDll, lpRawDll, HdrLen);   // 复制节头

  OrgAddr = 0;
  NewAddr = 0;
  Size = 0;

  // 复制 .text 节数据
  for (i = 0; i < pNTHeader->FileHeader.NumberOfSections; i++) {
    OrgAddr = (DWORD)lpImageDll + (DWORD)section[i].VirtualAddress;
    NewAddr = (DWORD)lpRawDll + (DWORD)section[i].PointerToRawData;
    Size = (DWORD)section[i].SizeOfRawData;
    MoveMemory((void*)OrgAddr, (void*)NewAddr, Size);
  }

  // 把指针指向新的 DLL 映像
  dosHeader = (PIMAGE_DOS_HEADER) lpImageDll; // Switch to new image
  pNTHeader = (PIMAGE_NT_HEADERS)((DWORD)dosHeader + dosHeader->e_lfanew);
  section = (PIMAGE_SECTION_HEADER)((DWORD)pNTHeader + sizeof(IMAGE_NT_HEADERS));
  pImageBase = (PBYTE)dosHeader;

#define IsNT() (TRUE)

  if ((ii != 0) && (IsNT() == TRUE)) {
    section[ii].VirtualAddress = section[ii].VirtualAddress + (DWORD)lpRawDll;
    section[ii].PointerToRawData = section[ii].PointerToRawData + (DWORD)lpRawDll;
  }


  // Look up where the imports section is (normally in the .idata section)
  // but not necessarily so. Therefore, grab the RVA from the data dir.
  importsStartRVA = GetImgDirEntryRVA(pNTHeader, IMAGE_DIRECTORY_ENTRY_IMPORT);

  if (!importsStartRVA) {
    VirtualFree(dosHeader, 0, MEM_RELEASE);
    return 7;
  }

  pImportDesc = (PIMAGE_IMPORT_DESCRIPTOR) pNTHeader->
      OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress;

  if (pImportDesc != 0) {
    pImportDesc = (PIMAGE_IMPORT_DESCRIPTOR)((DWORD)pImportDesc + (DWORD)dosHeader);
  }
  else {
    VirtualFree(dosHeader, 0, MEM_RELEASE);
    return 8;
  }

  while (1) { // 处理各入口表中的 DLL
    // 检查是否遇到了空的 IMAGE_IMPORT_DESCRIPTOR
    if ((pImportDesc->TimeDateStamp == 0) && (pImportDesc->Name == 0)) {
      break;
    }

    // 从磁盘载入必须的 Dll,
    // 注意,载入的 DLL 是合并的 DLL 的入口表中的 DLL,
    // 不是合并到 EXE 中的 DLL
    sDllName = (char*)(pImportDesc->Name + (DWORD)pImageBase);
    hDll = GetModuleHandle(sDllName);

    if (hDll == 0) {
      hDll = LoadLibrary(sDllName);
    }

    if (hDll == 0) {
      MessageBox(NULL, "Can''t find required Dll",
          "Error in LoadPbDllFromMemory()", 0);
      VirtualFree(dosHeader, 0, MEM_RELEASE);
      return 9;
    }

    DWORD* lpFuncNameRef = (DWORD*)(pImportDesc->OriginalFirstThunk +
        (DWORD)dosHeader);
    DWORD* lpFuncAddr = (DWORD*)(pImportDesc->FirstThunk +
        (DWORD)dosHeader);

    while (*lpFuncNameRef != 0) {
      pOrdinalName = (PIMAGE_IMPORT_BY_NAME)(*lpFuncNameRef +
          (DWORD)dosHeader);
      DWORD pIMAGE_ORDINAL_FLAG = 0x80000000;

      if (*lpFuncNameRef & pIMAGE_ORDINAL_FLAG)
        *lpFuncAddr = (DWORD) GetProcAddress(hDll,
            (const char*)(*lpFuncNameRef & 0xFFFF));
      else
        *lpFuncAddr = (DWORD) GetProcAddress(hDll,
            (const char*)pOrdinalName->Name);

      if (lpFuncAddr == 0) {
        VirtualFree(dosHeader, 0, MEM_RELEASE);
        return 10;// Can''t GetProcAddress
      }

      lpFuncAddr++;
      lpFuncNameRef++;
    }

    pImportDesc++;
  }

  DWORD TpOffset;
  baseReloc = (PIMAGE_BASE_RELOCATION)((DWORD)pNTHeader->
      　　　　 OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress);

  if (baseReloc != 0) {
    baseReloc = (PIMAGE_BASE_RELOCATION)((DWORD)baseReloc + (DWORD)dosHeader);

    while (baseReloc->VirtualAddress != 0) {
      PWORD lpTypeOffset = (PWORD)((DWORD)baseReloc +
          sizeof(IMAGE_BASE_RELOCATION));

      while (lpTypeOffset < (PWORD)((DWORD)baseReloc +
          (DWORD)baseReloc->SizeOfBlock)) {
        TpOffset = *lpTypeOffset & 0xF000;

        if (TpOffset == 0x3000) {
          lpLink = (PDWORD)((DWORD)dosHeader +
              　　　　　　　　　　　　　　　　　　　　　　             baseReloc->VirtualAddress +
              (*lpTypeOffset & 0xFFF));
          *lpLink = (DWORD)dosHeader +
              (*lpLink) - pNTHeader->OptionalHeader.ImageBase;
        }
        else {
          if (TpOffset != 0) {
            VirtualFree(dosHeader, 0, MEM_RELEASE);
            return 10;
          }
        }

        lpTypeOffset++;
      }

      baseReloc = (PIMAGE_BASE_RELOCATION)((DWORD)baseReloc +
          (DWORD)baseReloc->SizeOfBlock);
    }
  }

  // 取得原始的内存状态
  memset(Protection, 0, 4096);

  for (i = 0; i <= pNTHeader->FileHeader.NumberOfSections; i++) {
    if (i == pNTHeader->FileHeader.NumberOfSections) {
      Addr1 = 0;
      Addr2 = HdrLen;
      j = 0x60000000;
    }
    else {
      Addr1 = section[i].VirtualAddress;
      Addr2 = section[i].SizeOfRawData;
      j = section[i].Characteristics;
    }

    Addr2 += Addr1 - 1;

    Pg1 = Addr1 / sSysInfo.dwPageSize;
    Pg2 = Addr2 / sSysInfo.dwPageSize;

    for (Pg = Pg1 ; Pg <= Pg2; Pg++) {
      if (j & 0x20000000) {
        Protection[Pg] |= 1;  // Execute
      }

      if (j & 0x40000000) {
        Protection[Pg] |= 2;  // Read
      }

      if (j & 0x80000000) {
        Protection[Pg] |= 4;  // Write
      }
    }
  }

  // 恢复原始的内存状态
  Addr1 = (DWORD)dosHeader;

  for (Pg = 0 ; Pg <= ImagePages; Pg++) {
    switch (Protection[Pg]) {
    case 2:
      fOldProtect = PAGE_READONLY;
      break;

    case 3:
      fOldProtect = PAGE_EXECUTE_READ;
      break;

    case 6:
      fOldProtect = PAGE_READWRITE;
      break;

    default:
      // Ignore strange combinations
      fOldProtect = PAGE_EXECUTE_READWRITE;
      break;
    }

    if (fOldProtect != PAGE_EXECUTE_READWRITE) {
      if (VirtualProtect((void*)Addr1,
          sSysInfo.dwPageSize,
          fOldProtect,
          &fOldProtect) == 0) {
        VirtualFree(dosHeader, 0, MEM_RELEASE);
        return 11;
      }
    }

    Addr1 += sSysInfo.dwPageSize;
  }

  EntryPoint = (LPENTRYPOINT)((DWORD)pNTHeader->OptionalHeader.AddressOfEntryPoint +
      (DWORD)dosHeader);
  LPVOID lpReserved = 0;
  EntryPoint((HINSTANCE)dosHeader, DLL_PROCESS_ATTACH, lpReserved);
  lpImageDll2 = lpImageDll;
  return 0;
}
// 一但 DLL 被正确地载入到内存中，我们就可以通过自定义函数 GetProcAddressDirectly 来获取某函数在内存中的地址，
/// 并根据该地址来调用该函数，该函数也有两个参数，第一个参数是指向载入到内存中的 DLL 的起始地址的指针，
// 第二个是要调用的函数的函数名。以下是 GetProcAddressDirectly 函数代码:
DWORD GetProcAddressDirectly(PIMAGE_DOS_HEADER dosHeader, char* FuncName)
{
  PIMAGE_NT_HEADERS pNTHeader;
  PIMAGE_EXPORT_DIRECTORY pExportDir;
  PWORD lpNameOrdinals;
  LPDWORD lpFunctions;
  DWORD* lpName;
  char* lpExpFuncName;
  DWORD i;
  DWORD j;
  char* lpFuncName;
  DWORD exportsStartRVA, exportsEndRVA;

  if (dosHeader->e_magic != IMAGE_DOS_SIGNATURE) {
    return 0;
  }

  pNTHeader = (PIMAGE_NT_HEADERS)((DWORD)dosHeader + dosHeader->e_lfanew);

  if (pNTHeader->Signature != IMAGE_NT_SIGNATURE) {
    return 0;
  }

  if ((pNTHeader->FileHeader.SizeOfOptionalHeader != sizeof(pNTHeader->OptionalHeader)) ||
      (pNTHeader->OptionalHeader.Magic != IMAGE_NT_OPTIONAL_HDR32_MAGIC)) {
    return 0;
  }

  pImageBase = (PBYTE)dosHeader;

  // Make pointers to 32 and 64 bit versions of the header.
  pNTHeader = MakePtr(PIMAGE_NT_HEADERS, dosHeader, dosHeader->e_lfanew);

  exportsStartRVA = GetImgDirEntryRVA(pNTHeader, IMAGE_DIRECTORY_ENTRY_EXPORT);
  exportsEndRVA = exportsStartRVA +
      GetImgDirEntrySize(pNTHeader, IMAGE_DIRECTORY_ENTRY_EXPORT);

  // Get the IMAGE_SECTION_HEADER that contains the exports. This is
  // usually the .edata section, but doesn''t have to be.
  PIMAGE_SECTION_HEADER header;
  header = GetEnclosingSectionHeader(exportsStartRVA, pNTHeader);

  if (!header) {
    return 0;
  }

  INT delta;
  delta = (INT)(header->VirtualAddress - header->PointerToRawData);
  pExportDir = (PIMAGE_EXPORT_DIRECTORY)GetPtrFromRVA(exportsStartRVA,
      pNTHeader, pImageBase);


  pExportDir = (PIMAGE_EXPORT_DIRECTORY)(pNTHeader->
      OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);

  if (pExportDir == 0) {
    MessageBox(NULL, "Error in GetProcAddressDirectly()", 0, 0);
    return 0;
  }

  pExportDir = (PIMAGE_EXPORT_DIRECTORY)((DWORD)pExportDir + (DWORD)dosHeader);
  lpNameOrdinals = (PWORD)((DWORD)pExportDir->AddressOfNameOrdinals + (DWORD)dosHeader);
  lpName = (LPDWORD)(pExportDir->AddressOfNames + (DWORD)dosHeader);
  lpFunctions = (LPDWORD)(pExportDir->AddressOfFunctions + (DWORD)dosHeader);
  lpFuncName = FuncName;

  if (HIWORD(lpFuncName) != 0) {
    for (i = 0; i <= pExportDir->NumberOfFunctions - 1; i++) {
      DWORD entryPointRVA = *lpFunctions;

      // Skip over gaps in exported function
      if (entryPointRVA == 0) {
        continue;
      }

      for (j = 0; j <= pExportDir->NumberOfNames - 1; j++) {
        if (lpNameOrdinals[j] == i) {
          lpExpFuncName = (char*)(lpName[j] +
              (DWORD)dosHeader);

          if (strcmp((char*)lpExpFuncName, (char*)FuncName) == 0)
            return (DWORD)(lpFunctions[i] +
                (DWORD)dosHeader);
        }
      }
    }
  }
  else {
    for (i = 0 ; i <= pExportDir->NumberOfFunctions - 1; i++) {
      if (lpFuncName == (char*)(pExportDir->Base + i)) {
        if (lpFunctions[i]) return (unsigned long)(lpFunctions[i] +
              dosHeader);
      }
    }
  }

  return 0;
}


// 在调用完函数后，不要忘记用 UnloadPbDllFromMemory 来从内存中移去 DLL 以释放分配的内存，该函数还会用 DLL_PROCESS_DETACH 参数调用 DLL 的入口函数 DllMain 来从调用进程的地址空间卸载该 DLL。 以下是 UnloadPbDllFromMemory 函数代码:
DWORD UnloadPbDllFromMemory(PIMAGE_DOS_HEADER dosHeader)
{
  PIMAGE_NT_HEADERS pNTHeader;
  pNTHeader = (PIMAGE_NT_HEADERS)((DWORD)dosHeader + (DWORD)dosHeader->e_lfanew);
  EntryPoint = (LPENTRYPOINT)(pNTHeader->OptionalHeader.AddressOfEntryPoint +
      (DWORD)dosHeader);
  EntryPoint((HINSTANCE)dosHeader, DLL_PROCESS_DETACH, 0);
  return VirtualFree(dosHeader, 0, MEM_RELEASE);
}

关于示例代码的说明
　　在本文附带的示例代码中，合并了一个名为 hardware.dll 的动态连接库，该动态连接库是一个获取系统硬件信息的库文件，其中包括了以下函数:

getmac     取得网卡 MAC
VolumeNumber   取得硬盘卷标
changeres            改变屏幕分辩率
IsDiskInDrive        检查软驱中是否插有盘
DPGetDefaultPrinter  取得默认的打印机名
DPSetDefaultPrinter  设置默认的打印机
getserial            取得硬盘的出厂序列号
getmetric            取得显示分辩率
PrintStringDirect    直接向打印机发送一个串
vfpbeep              让 PC 喇叭发声
getcpuid             取得 CPU ID
getbios              取得主板 BIOS ID
　　在示例代码中，只调用了其中一个函数 getbios 来获取主板 BIOS ID， 这里说明一下，该函数实际上好象只能检测 AWARD 主板的 BIOS， 也就是说它是读取的是系统内存 0x000fex71 处的值。因为其它牌子的主板 BIOS 的位置稍有不同，但在程序中没有进行这方面的处理，所以在读其它牌子的主板 BIOS 时可能会有些问题(读出的内容可能不正确)。关于此 DLL 的内容和实现，也许我会在另一篇文章中论及。
　

局限
　　在我进行测试时，发现对于有些含有资源的 DLL，在 9x 平台下可能会有问题。
题外话
　　另外，其它一些本文未提及的非主要的函数，请自行参见源代码中的注释。
再，本文涉及 PE 文件格式方面的知识，它们已经超出了本文的范围，具体信息可参见 MSDM 中的:

Peering Inside the PE:
A Tour of the Win32 Portable Executable File Format 一文和
Microsoft Portable Executable and Common Object File Format Specification 一文
特别感谢卢春明（Aming）在我编写本文时所作的一些技术方面的建议和指导

#endif





#endif // _LOADDLL_H_
