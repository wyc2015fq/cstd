#ifndef __MEMEXEC_H__
#define __MEMEXEC_H__
//直接运行内存中的程序
// Windows的PE加载器在启动程序的时候，会将磁盘上的文件加载到内存，然后做很多操作，如函数导入表重定位，变量预处理之类的。这位仁兄等于是自己写了一个PE加载器。直接将内存中的程序启动。记得以前的“红色代码”病毒也有相同的特性。
// 直接启动内存中的程序相当于加了一个壳，可以把程序加密保存，运行时解密到内存，然后启动，不过对于增加破解难度还要稍微复杂点。否则人家把内存中的进程DUMP出来然后修复导入表就被拖出来了。
#include <windows.h>
typedef IMAGE_SECTION_HEADER(*PIMAGE_SECTION_HEADERS)[1];
// 计算对齐后的大小
unsigned long GetAlignedSize(unsigned long Origin, unsigned long Alignment)
{
  return (Origin + Alignment - 1) / Alignment * Alignment;
}
// 计算加载pe并对齐需要占用多少内存
// 未直接使用OptionalHeader.SizeOfImage作为结果是因为据说有的编译器生成的exe这个值会填0
unsigned long CalcTotalImageSize(PIMAGE_DOS_HEADER MzH, unsigned long FileLen, PIMAGE_NT_HEADERS peH, PIMAGE_SECTION_HEADERS peSecH)
{
  int i;
  unsigned long res;
  // 计算pe头的大小
  res = GetAlignedSize(peH->OptionalHeader.SizeOfHeaders, peH->OptionalHeader.SectionAlignment);
  // 计算所有节的大小
  for (i = 0; i < peH->FileHeader.NumberOfSections; ++i) {
    // 超出文件范围
    if (peSecH[i]->PointerToRawData + peSecH[i]->SizeOfRawData > FileLen) {
      return 0;
    }
    else if (peSecH[i]->VirtualAddress) { //计算对齐后某节的大小
      if (peSecH[i]->Misc.VirtualSize) {
        res = GetAlignedSize(peSecH[i]->VirtualAddress + peSecH[i]->Misc.VirtualSize, peH->OptionalHeader.SectionAlignment);
      }
      else {
        res = GetAlignedSize(peSecH[i]->VirtualAddress + peSecH[i]->SizeOfRawData, peH->OptionalHeader.SectionAlignment);
      }
    }
    else if (peSecH[i]->Misc.VirtualSize < peSecH[i]->SizeOfRawData) {
      res += GetAlignedSize(peSecH[i]->SizeOfRawData, peH->OptionalHeader.SectionAlignment);
    }
    else {
      res += GetAlignedSize(peSecH[i]->Misc.VirtualSize, peH->OptionalHeader.SectionAlignment);
    }// if_else
  }// for
  return res;
}
// 加载pe到内存并对齐所有节
BOOL AlignPEToMem(void* pBuf, long Len, PIMAGE_NT_HEADERS* ppeH, PIMAGE_SECTION_HEADERS* ppeSecH, void** pMem, unsigned long* pImageSize)
{
  PIMAGE_DOS_HEADER SrcMz;// DOS头
  PIMAGE_NT_HEADERS SrcPeH;// PE头
  PIMAGE_SECTION_HEADERS SrcPeSecH;// 节表
  SrcMz = (PIMAGE_DOS_HEADER)pBuf;
  if (Len < sizeof(IMAGE_DOS_HEADER)) {
    return FALSE;
  }
  if (SrcMz->e_magic != IMAGE_DOS_SIGNATURE) {
    return FALSE;
  }
  if (Len < SrcMz->e_lfanew + (long)sizeof(IMAGE_NT_HEADERS)) {
    return FALSE;
  }
  SrcPeH = (PIMAGE_NT_HEADERS)((int)SrcMz + SrcMz->e_lfanew);
  if (SrcPeH->Signature != IMAGE_NT_SIGNATURE) {
    return FALSE;
  }
  if ((SrcPeH->FileHeader.Characteristics & IMAGE_FILE_DLL) ||
      (SrcPeH->FileHeader.Characteristics & IMAGE_FILE_EXECUTABLE_IMAGE == 0) ||
      (SrcPeH->FileHeader.SizeOfOptionalHeader != sizeof(IMAGE_OPTIONAL_HEADER))) {
    return FALSE;
  }
  SrcPeSecH = (PIMAGE_SECTION_HEADERS)((int)SrcPeH + sizeof(IMAGE_NT_HEADERS));
  *pImageSize = CalcTotalImageSize(SrcMz, Len, SrcPeH, SrcPeSecH);
  if (*pImageSize == 0) {
    return FALSE;
  }
  *pMem = VirtualAlloc(NULL, *pImageSize, MEM_COMMIT, PAGE_EXECUTE_READWRITE);  // 分配内存
  if (*pMem != NULL) {
    // 计算需要复制的PE头字节数
    int i;
    void* Pt;
    unsigned long l = SrcPeH->OptionalHeader.SizeOfHeaders;
    for (i = 0; i < SrcPeH->FileHeader.NumberOfSections; ++i) {
      if ((SrcPeSecH[i]->PointerToRawData) && (SrcPeSecH[i]->PointerToRawData < l)) {
        l = SrcPeSecH[i]->PointerToRawData;
      }
    }
    memmove(*pMem, SrcMz, l);
    *ppeH = (PIMAGE_NT_HEADERS)((int) * pMem + ((PIMAGE_DOS_HEADER) * pMem)->e_lfanew);
    *ppeSecH = (PIMAGE_SECTION_HEADERS)((int) * ppeH + sizeof(IMAGE_NT_HEADERS));
    Pt = (void*)((unsigned long)(*pMem) + GetAlignedSize((*ppeH)->OptionalHeader.SizeOfHeaders, (*ppeH)->OptionalHeader.SectionAlignment));
    for (i = 0; i < (*ppeH)->FileHeader.NumberOfSections; ++i) {
      // 定位该节在内存中的位置
      if ((*ppeSecH)[i]->VirtualAddress) {
        Pt = (void*)((unsigned long)(*pMem) + (*ppeSecH)[i]->VirtualAddress);
      }
      if ((*ppeSecH)[i]->SizeOfRawData) {
        // 复制数据到内存
        memmove(Pt, (const void*)((unsigned long)(SrcMz) + (*ppeSecH)[i]->PointerToRawData), (*ppeSecH)[i]->SizeOfRawData);
        if ((*ppeSecH)[i]->Misc.VirtualSize < (*ppeSecH)[i]->SizeOfRawData) {
          Pt = (void*)((unsigned long)Pt + GetAlignedSize((*ppeSecH)[i]->SizeOfRawData, (*ppeH)->OptionalHeader.SectionAlignment));
        }
        else { // pt 定位到下一节开始位置
          Pt = (void*)((unsigned long)Pt + GetAlignedSize((*ppeSecH)[i]->Misc.VirtualSize, (*ppeH)->OptionalHeader.SectionAlignment));
        }
      }
      else {
        Pt = (void*)((unsigned long)Pt + GetAlignedSize((*ppeSecH)[i]->Misc.VirtualSize, (*ppeH)->OptionalHeader.SectionAlignment));
      }
    }
  }
  return TRUE;
}
typedef void* (__stdcall* pfVirtualAllocEx)(unsigned long, void*, unsigned long, unsigned long, unsigned long);
pfVirtualAllocEx MyVirtualAllocEx = NULL;
// 初始化
int CInit1()
{
  if (NULL == MyVirtualAllocEx) {
    MyVirtualAllocEx = (pfVirtualAllocEx)GetProcAddress(GetModuleHandle("Kernel32.dll"), "VirtualAllocEx");
  }
  return 0;
}
BOOL IsNT()
{
  CInit1();
  return MyVirtualAllocEx != NULL;
}
// 生成外壳程序命令行
char* PrepareShellExe(char* CmdParam, unsigned long BaseAddr, unsigned long ImageSize)
{
  if (IsNT()) {
    char* pBuf = (char*)malloc(256);
    memset(pBuf, 0, 256);
    GetModuleFileName(0, pBuf, 256);
    if (strlen(CmdParam) > 0) {
      strcat(pBuf, " ");
      strcat(pBuf, CmdParam);
    }
    return pBuf; // 请记得释放内存;-)
  }
  else {
    // Win98下的处理请参考原文;-)
    // http://community.csdn.net/Expert/topic/4416/4416252.xml?temp=8.709133E-03
    return NULL;
  }
}
// 是否包含可重定向列表
BOOL HasRelocationTable(PIMAGE_NT_HEADERS peH)
{
  return (peH->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress)
      && (peH->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].Size);
}
#pragma pack(push, 1)
typedef struct {
  unsigned long VirtualAddress;
  unsigned long SizeOfBlock;
}* PImageBaseRelocation;
#pragma pack(pop)
// 重定向PE用到的地址
void DoRelocation(PIMAGE_NT_HEADERS peH, void* OldBase, void* NewBase)
{
  unsigned long Delta = (unsigned long)NewBase - peH->OptionalHeader.ImageBase;
  PImageBaseRelocation p = (PImageBaseRelocation)((unsigned long)OldBase
      + peH->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress);
  while (p->VirtualAddress + p->SizeOfBlock) {
    unsigned int i;
    unsigned short* pw = (unsigned short*)((int)p + sizeof(*p));
    for (i = 1; i <= (p->SizeOfBlock - sizeof(*p)) / 2; ++i) {
      if ((*pw) & 0xF000 == 0x3000) {
        unsigned long* t = (unsigned long*)((unsigned long)(OldBase) + p->VirtualAddress + ((*pw) & 0x0FFF));
        *t += Delta;
      }
      ++pw;
    }
    p = (PImageBaseRelocation)pw;
  }
}
// 卸载原外壳占用内存
BOOL UnloadShell(HANDLE ProcHnd, unsigned long BaseAddr)
{
  typedef unsigned long(__stdcall * pfZwUnmapViewOfSection)(unsigned long, unsigned long);
  pfZwUnmapViewOfSection ZwUnmapViewOfSection = NULL;
  BOOL res = FALSE;
  HMODULE m = LoadLibrary("ntdll.dll");
  if (m) {
    ZwUnmapViewOfSection = (pfZwUnmapViewOfSection)GetProcAddress(m, "ZwUnmapViewOfSection");
    if (ZwUnmapViewOfSection) {
      res = (ZwUnmapViewOfSection((unsigned long)ProcHnd, BaseAddr) == 0);
    }
    FreeLibrary(m);
  }
  return res;
}
// 创建外壳进程并获取其基址、大小和当前运行状态
BOOL CreateChild(char* Cmd, CONTEXT* pCtx, HANDLE* pProcHnd, HANDLE* pThrdHnd,
    unsigned long* pProcId, unsigned long* pBaseAddr, unsigned long* pImageSize)
{
  BOOL res;
  STARTUPINFOA si;
  PROCESS_INFORMATION pi;
  unsigned long old;
  MEMORY_BASIC_INFORMATION MemInfo;
  memset(&si, 0, sizeof(si));
  memset(&pi, 0, sizeof(pi));
  si.cb = sizeof(si);
  res = CreateProcess(NULL, Cmd, NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, NULL, &si, &pi); // 以挂起方式运行进程;
  if (res) {
    void* p;
    *pProcHnd = pi.hProcess;
    *pThrdHnd = pi.hThread;
    *pProcId = pi.dwProcessId;
    // 获取外壳进程运行状态，[ctx.Ebx+8]内存处存的是外壳进程的加载基址，ctx.Eax存放有外壳进程的入口地址
    (*pCtx).ContextFlags = CONTEXT_FULL;
    GetThreadContext(*pThrdHnd, pCtx);
    ReadProcessMemory(*pProcHnd, (void*)(pCtx->Ebx + 8), pBaseAddr, sizeof(unsigned long), &old); // 读取加载基址
    p = (void*)(*pBaseAddr);
    // 计算外壳进程占有的内存
    while (VirtualQueryEx(*pProcHnd, p, &MemInfo, sizeof(MemInfo))) {
      if (MemInfo.State = MEM_FREE) {
        break;
      }
      p = (void*)((unsigned long)p + MemInfo.RegionSize);
    }
    *pImageSize = (unsigned long)p - (unsigned long)(*pBaseAddr);
  }
  return res;
}
// 创建外壳进程并用目标进程替换它然后执行
HANDLE AttachPE(char* CmdParam, PIMAGE_NT_HEADERS peH, PIMAGE_SECTION_HEADERS peSecH,
    void* Ptr, unsigned long ImageSize, unsigned long* pProcId)
{
  HANDLE res = INVALID_HANDLE_VALUE;
  CONTEXT Ctx;
  HANDLE Thrd;
  unsigned long Addr, Size;
  char* s = PrepareShellExe(CmdParam, peH->OptionalHeader.ImageBase, ImageSize);
  CInit1();
  if (s == NULL) {
    return res;
  }
  if (CreateChild(s, &Ctx, &res, &Thrd, pProcId, &Addr, &Size)) {
    void* p = NULL;
    unsigned long old;
    if ((peH->OptionalHeader.ImageBase == Addr) && (Size >= ImageSize)) {// 外壳进程可以容纳目标进程并且加载地址一致
      p = (void*)Addr;
      VirtualProtectEx(res, p, Size, PAGE_EXECUTE_READWRITE, &old);
    }
    else if (IsNT()) {
      if (UnloadShell(res, Addr)) { // 卸载外壳进程占有内存
        p = MyVirtualAllocEx((unsigned long)res, (void*)peH->OptionalHeader.ImageBase, ImageSize, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
      }
      if ((p == NULL) && HasRelocationTable(peH)) { // 分配内存失败并且目标进程支持重定向
        p = MyVirtualAllocEx((unsigned long)res, NULL, ImageSize, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
        if (p) {
          DoRelocation(peH, Ptr, p);  // 重定向
        }
      }
    }
    if (p) {
      WriteProcessMemory(res, (void*)(Ctx.Ebx + 8), &p, sizeof(DWORD), &old); // 重置目标进程运行环境中的基址
      peH->OptionalHeader.ImageBase = (unsigned long)p;
      if (WriteProcessMemory(res, p, Ptr, ImageSize, &old)) { // 复制PE数据到目标进程
        Ctx.ContextFlags = CONTEXT_FULL;
        if ((unsigned long)p == Addr) {
          Ctx.Eax = peH->OptionalHeader.ImageBase + peH->OptionalHeader.AddressOfEntryPoint;  // 重置运行环境中的入口地址
        }
        else {
          Ctx.Eax = (unsigned long)p + peH->OptionalHeader.AddressOfEntryPoint;
        }
        SetThreadContext(Thrd, &Ctx);// 更新运行环境
        ResumeThread(Thrd);// 执行
        CloseHandle(Thrd);
      }
      else {// 加载失败,杀掉外壳进程
        TerminateProcess(res, 0);
        CloseHandle(Thrd);
        CloseHandle(res);
        res = INVALID_HANDLE_VALUE;
      }
    }
    else {// 加载失败,杀掉外壳进程
      TerminateProcess(res, 0);
      CloseHandle(Thrd);
      CloseHandle(res);
      res = INVALID_HANDLE_VALUE;
    }
  }
  free(s);
  return res;
}
// 从内存中加载并运行exe
// 参数：
// Buffer: 内存中的exe地址
// Len: 内存中exe占用长度
// CmdParam: 命令行参数(不包含exe文件名的剩余命令行参数）
// ProcessId: 返回的进程Id
// 返回值： 如果成功则返回进程的Handle(ProcessHandle),如果失败则返回INVALID_HANDLE_VALUE
HANDLE MemExecute(void* ABuffer, long Len, char* CmdParam, unsigned long* pProcessId)
{
  HANDLE res = INVALID_HANDLE_VALUE;
  PIMAGE_NT_HEADERS peH;
  PIMAGE_SECTION_HEADERS peSecH;
  void* Ptr;
  unsigned long peSz;
  if (AlignPEToMem(ABuffer, Len, &peH, &peSecH, &Ptr, &peSz)) {
    res = AttachPE(CmdParam, peH, peSecH, Ptr, peSz, pProcessId);
    VirtualFree(Ptr, peSz, MEM_DECOMMIT);
  }
  return res;
}
#include "cfile.h"
#define EXEPATH "C:\\WINDOWS\\notepad.exe"
//#define EXEPATH ".\\Debug\\aaa.exe"
int test_memexec()
{
  DWORD dwFileSize;
  unsigned long ulProcessId = 0;
  LPBYTE pBuf = filemap(EXEPATH, &dwFileSize);
  MemExecute(pBuf, dwFileSize, "C:\\boot.ini", &ulProcessId);
  free(pBuf);
  return 0;
}
#endif // __MEMEXEC_H__

