
#include "cstd.h"

#ifdef _MSC_VER
# if _MSC_VER >= 1700
#  pragma warning(disable:4447) // Disable warning 'main' signature found without threading model
# endif
#endif

#if defined ANDROID || defined __linux__ || defined __FreeBSD__
#  include <unistd.h>
#  include <fcntl.h>
#  include <elf.h>
#if defined ANDROID || defined __linux__
#  include <linux/auxvec.h>
#endif
#endif

#if defined WIN32 || defined _WIN32 || defined WINCE
#ifndef _WIN32_WINNT           // This is needed for the declaration of TryEnterCriticalSection in winbase.h with Visual Studio 2005 (and older?)
  #define _WIN32_WINNT 0x0400  // http://msdn.microsoft.com/en-us/library/ms686857(VS.85).aspx
#endif
#include <windows.h>
#if (_WIN32_WINNT >= 0x0602)
  #include <synchapi.h>
#endif
#undef small
#undef min
#undef max
#undef abs
#include <tchar.h>
#if defined _MSC_VER
  #if _MSC_VER >= 1400
    #include <intrin.h>
  #elif defined _M_IX86
    static void __cpuid(int* cpuid_data, int a)
    {
        __asm
        {
            push ebx
            push edi
            mov edi, cpuid_data
            mov eax, 1
            cpuid
            mov [edi], eax
            mov [edi + 4], ebx
            mov [edi + 8], ecx
            mov [edi + 12], edx
            pop edi
            pop ebx
        }
    }
    static void __cpuidex(int* cpuid_data, int a, int b)
    {
        __asm
        {
            push edi
            mov edi, cpuid_data
            mov eax, 7
            mov ecx, 0
            cpuid
            mov [edi], eax
            mov [edi + 4], ebx
            mov [edi + 8], ecx
            mov [edi + 12], edx
            pop edi
        }
    }
  #endif
#endif

#ifdef WINRT
#include <wrl/client.h>
#ifndef __cplusplus_winrt
#include <windows.storage.h>
#pragma comment(lib, "runtimeobject.lib")
#endif

std::wstring GetTempPathWinRT()
{
#ifdef __cplusplus_winrt
    return std::wstring(Windows::Storage::ApplicationData::Current->TemporaryFolder->Path->Data());
#else
    Microsoft::WRL::ComPtr<ABI::Windows::Storage::IApplicationDataStatics> appdataFactory;
    Microsoft::WRL::ComPtr<ABI::Windows::Storage::IApplicationData> appdataRef;
    Microsoft::WRL::ComPtr<ABI::Windows::Storage::IStorageFolder> storagefolderRef;
    Microsoft::WRL::ComPtr<ABI::Windows::Storage::IStorageItem> storageitemRef;
    HSTRING str;
    HSTRING_HEADER hstrHead;
    std::wstring wstr;
    if (FAILED(WindowsCreateStringReference(RuntimeClass_Windows_Storage_ApplicationData,
                                            (UINT32)wcslen(RuntimeClass_Windows_Storage_ApplicationData), &hstrHead, &str)))
        return wstr;
    if (FAILED(RoGetActivationFactory(str, IID_PPV_ARGS(appdataFactory.ReleaseAndGetAddressOf()))))
        return wstr;
    if (FAILED(appdataFactory->get_Current(appdataRef.ReleaseAndGetAddressOf())))
        return wstr;
    if (FAILED(appdataRef->get_TemporaryFolder(storagefolderRef.ReleaseAndGetAddressOf())))
        return wstr;
    if (FAILED(storagefolderRef.As(&storageitemRef)))
        return wstr;
    str = NULL;
    if (FAILED(storageitemRef->get_Path(&str)))
        return wstr;
    wstr = WindowsGetStringRawBuffer(str, NULL);
    WindowsDeleteString(str);
    return wstr;
#endif
}

std::wstring GetTempFileNameWinRT(std::wstring prefix)
{
    wchar_t guidStr[40];
    GUID g;
    CoCreateGuid(&g);
    wchar_t* mask = L"%08x_%04x_%04x_%02x%02x_%02x%02x%02x%02x%02x%02x";
    swprintf(&guidStr[0], sizeof(guidStr)/sizeof(wchar_t), mask,
             g.Data1, g.Data2, g.Data3, UINT(g.Data4[0]), UINT(g.Data4[1]),
             UINT(g.Data4[2]), UINT(g.Data4[3]), UINT(g.Data4[4]),
             UINT(g.Data4[5]), UINT(g.Data4[6]), UINT(g.Data4[7]));

    return prefix.append(std::wstring(guidStr));
}

#endif
#else
#include <pthread.h>
#include <sys/time.h>
#include <time.h>

#if defined __MACH__ && defined __APPLE__
#include <mach/mach.h>
#include <mach/mach_time.h>
#endif

#endif

#ifdef _OPENMP
#include "omp.h"
#endif

#include <stdarg.h>

#if defined __linux__ || defined __APPLE__ || defined __EMSCRIPTEN__ || defined __FreeBSD__
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#if defined ANDROID
#include <sys/sysconf.h>
#endif
#endif

#ifdef ANDROID
# include <android/log.h>
#endif

/*!
 \return the error description and the context as a text string.
 */

static void cpu_info_init_feature(cpu_info_t* f)
{
    int cpuid_data[4] = { 0, 0, 0, 0 };

#if defined _MSC_VER && (defined _M_IX86 || defined _M_X64)
    __cpuid(cpuid_data, 1);
#elif defined __GNUC__ && (defined __i386__ || defined __x86_64__)
    #ifdef __x86_64__
    asm __volatile__
    (
     "movl $1, %%eax\n\t"
     "cpuid\n\t"
     :[eax]"=a"(cpuid_data[0]),[ebx]"=b"(cpuid_data[1]),[ecx]"=c"(cpuid_data[2]),[edx]"=d"(cpuid_data[3])
     :
     : "cc"
    );
    #else
    asm volatile
    (
     "pushl %%ebx\n\t"
     "movl $1,%%eax\n\t"
     "cpuid\n\t"
     "popl %%ebx\n\t"
     : "=a"(cpuid_data[0]), "=c"(cpuid_data[2]), "=d"(cpuid_data[3])
     :
     : "cc"
    );
    #endif
#endif

    f->x86_family = (cpuid_data[0] >> 8) & 15;
    if( f->x86_family >= 6 )
    {
        f->have[CC_CPU_MMX]    = (cpuid_data[3] & (1 << 23)) != 0;
        f->have[CC_CPU_SSE]    = (cpuid_data[3] & (1<<25)) != 0;
        f->have[CC_CPU_SSE2]   = (cpuid_data[3] & (1<<26)) != 0;
        f->have[CC_CPU_SSE3]   = (cpuid_data[2] & (1<<0)) != 0;
        f->have[CC_CPU_SSSE3]  = (cpuid_data[2] & (1<<9)) != 0;
        f->have[CC_CPU_FMA3]  = (cpuid_data[2] & (1<<12)) != 0;
        f->have[CC_CPU_SSE4_1] = (cpuid_data[2] & (1<<19)) != 0;
        f->have[CC_CPU_SSE4_2] = (cpuid_data[2] & (1<<20)) != 0;
        f->have[CC_CPU_POPCNT] = (cpuid_data[2] & (1<<23)) != 0;
        f->have[CC_CPU_AVX]    = (((cpuid_data[2] & (1<<28)) != 0)&&((cpuid_data[2] & (1<<27)) != 0));//OS uses XSAVE_XRSTORE and CPU support AVX
        f->have[CC_CPU_FP16]   = (cpuid_data[2] & (1<<29)) != 0;

        // make the second call to the cpuid command in order to get
        // information about extended features like AVX2
    #if defined _MSC_VER && (defined _M_IX86 || defined _M_X64)
        __cpuidex(cpuid_data, 7, 0);
    #elif defined __GNUC__ && (defined __i386__ || defined __x86_64__)
        #ifdef __x86_64__
        asm __volatile__
        (
         "movl $7, %%eax\n\t"
         "movl $0, %%ecx\n\t"
         "cpuid\n\t"
         :[eax]"=a"(cpuid_data[0]),[ebx]"=b"(cpuid_data[1]),[ecx]"=c"(cpuid_data[2]),[edx]"=d"(cpuid_data[3])
         :
         : "cc"
        );
        #else
        asm volatile
        (
         "pushl %%ebx\n\t"
         "movl $7,%%eax\n\t"
         "movl $0,%%ecx\n\t"
         "cpuid\n\t"
         "movl %%ebx, %0\n\t"
         "popl %%ebx\n\t"
         : "=r"(cpuid_data[1]), "=c"(cpuid_data[2])
         :
         : "cc"
        );
        #endif
    #endif
        f->have[CC_CPU_AVX2]   = (cpuid_data[1] & (1<<5)) != 0;

        f->have[CC_CPU_AVX_512F]       = (cpuid_data[1] & (1<<16)) != 0;
        f->have[CC_CPU_AVX_512DQ]      = (cpuid_data[1] & (1<<17)) != 0;
        f->have[CC_CPU_AVX_512IFMA512] = (cpuid_data[1] & (1<<21)) != 0;
        f->have[CC_CPU_AVX_512PF]      = (cpuid_data[1] & (1<<26)) != 0;
        f->have[CC_CPU_AVX_512ER]      = (cpuid_data[1] & (1<<27)) != 0;
        f->have[CC_CPU_AVX_512CD]      = (cpuid_data[1] & (1<<28)) != 0;
        f->have[CC_CPU_AVX_512BW]      = (cpuid_data[1] & (1<<30)) != 0;
        f->have[CC_CPU_AVX_512VL]      = (cpuid_data[1] & (1<<31)) != 0;
        f->have[CC_CPU_AVX_512VBMI]    = (cpuid_data[2] &  (1<<1)) != 0;
    }

#if defined ANDROID || defined __linux__
#ifdef __aarch64__
    f->have[CC_CPU_NEON] = true;
    f->have[CC_CPU_FP16] = true;
#elif defined __arm__
    int cpufile = open("/proc/self/auxv", O_RDONLY);

    if (cpufile >= 0)
    {
        Elf32_auxv_t auxv;
        const size_t size_auxv_t = sizeof(auxv);

        while ((size_t)read(cpufile, &auxv, size_auxv_t) == size_auxv_t)
        {
            if (auxv.a_type == AT_HWCAP)
            {
                f->have[CC_CPU_NEON] = (auxv.a_un.a_val & 4096) != 0;
                f->have[CC_CPU_FP16] = (auxv.a_un.a_val & 2) != 0;
                break;
            }
        }

        close(cpufile);
    }
#endif
#elif (defined __clang__ || defined __APPLE__)
#if (defined __ARM_NEON__ || (defined __ARM_NEON && defined __aarch64__))
    f->have[CC_CPU_NEON] = true;
#endif
#if (defined __ARM_FP  && (((__ARM_FP & 0x2) != 0) && defined __ARM_NEON__))
    f->have[CC_CPU_FP16] = true;
#endif
#endif

    return ;
}

// intrinsics
#if defined(__GNUC__)    // GCC
#ifndef __arm__
#include <cpuid.h>
#endif
#elif defined(_MSC_VER)    // MSVC
    #if _MSC_VER >=1400    // VC2005
#include <intrin.h>
    #endif    // #if _MSC_VER >=1400
#else
#error Only supports MSVC or GCC.
#endif    // #if defined(__GNUC__)

static void getcpuidex(int CPUInfo[4], unsigned int InfoType, unsigned int ECXValue)
{
#if defined(__GNUC__)  && (defined __i386__ || defined __x86_64__)
    __cpuid_count(InfoType, ECXValue, CPUInfo[0],CPUInfo[1],CPUInfo[2],CPUInfo[3]);
#elif defined(_MSC_VER)  && (defined _M_IX86 || defined _M_X64)
    #if defined(_WIN64) || _MSC_VER>=1600    // 64位下不支持内联汇编. 1600: VS2010, 据说VC2008 SP1之后才支持__cpuidex.
        __cpuidex((int*)(void*)CPUInfo, (int)InfoType, (int)ECXValue);
    #else
        if (NULL==CPUInfo)    return;
        _asm{
            // load. 读取参数到寄存器.
            mov edi, CPUInfo;    // 准备用edi寻址CPUInfo
            mov eax, InfoType;
            mov ecx, ECXValue;
            // CPUID
            cpuid;
            // save. 将寄存器保存到CPUInfo
            mov    [edi], eax;
            mov    [edi+4], ebx;
            mov    [edi+8], ecx;
            mov    [edi+12], edx;
        }
    #endif
#endif    // #if defined(__GNUC__)
}

static void getcpuid(int CPUInfo[4], unsigned int InfoType)
{
#if defined(__GNUC__) && (defined __i386__ || defined __x86_64__)
    __cpuid(InfoType, CPUInfo[0],CPUInfo[1],CPUInfo[2],CPUInfo[3]);
#elif defined(_MSC_VER)  && (defined _M_IX86 || defined _M_X64)
    #if _MSC_VER>=1400    // VC2005才支持__cpuid
        __cpuid((int*)(void*)CPUInfo, (int)InfoType);
    #else
        getcpuidex(CPUInfo, InfoType, 0);
    #endif
#endif    // #if defined(__GNUC__)
}

// 取得CPU厂商（Vendor）.
//
// result: 成功时返回字符串的长度（一般为12）。失败时返回0.
// pvendor: 接收厂商信息的字符串缓冲区。至少为13字节.
static int cpu_getvendor(char* pvendor)
{
  int dwBuf[4];
  if (NULL==pvendor)    return 0;
  // Function 0: Vendor-ID and Largest Standard Function
  getcpuid(dwBuf, 0);
  // save. 保存到pvendor
  *(int *)&pvendor[0] = dwBuf[1];    // ebx: 前四个字符.
  *(int *)&pvendor[4] = dwBuf[3];    // edx: 中间四个字符.
  *(int *)&pvendor[8] = dwBuf[2];    // ecx: 最后四个字符.
  pvendor[12] = '\0';
  return 12;
}

// 取得CPU商标（Brand）.
//
// result: 成功时返回字符串的长度（一般为48）。失败时返回0.
// pbrand: 接收商标信息的字符串缓冲区。至少为49字节.
static int cpu_getbrand(char* pbrand)
{
  int dwBuf[4];
  if (NULL==pbrand)    return 0;
  // Function 0x80000000: Largest Extended Function Number
  getcpuid(dwBuf, 0x80000000U);
  if (dwBuf[0] < 0x80000004U)    return 0;
  // Function 80000002h,80000003h,80000004h: Processor Brand String
  getcpuid((int *)&pbrand[0], 0x80000002U);    // 前16个字符.
  getcpuid((int *)&pbrand[16], 0x80000003U);    // 中间16个字符.
  getcpuid((int *)&pbrand[32], 0x80000004U);    // 最后16个字符.
  pbrand[48] = '\0';
  return 48;
}

CC_INLINE void queryCacheSizes_intel_direct(cpu_info_t* f)
{
  int abcd[4];
  int cache_id = 0;
  int cache_type = 0;
  f->l1 = f->l2 = f->l3 = 0;
  do {
    abcd[0] = abcd[1] = abcd[2] = abcd[3] = 0;
    getcpuidex(abcd,0x4,cache_id);
    cache_type  = (abcd[0] & 0x0F) >> 0;
    if(cache_type==1||cache_type==3) // data or unified cache
    {
      int cache_level = (abcd[0] & 0xE0) >> 5;  // A[7:5]
      int ways        = (abcd[1] & 0xFFC00000) >> 22; // B[31:22]
      int partitions  = (abcd[1] & 0x003FF000) >> 12; // B[21:12]
      int line_size   = (abcd[1] & 0x00000FFF) >>  0; // B[11:0]
      int sets        = (abcd[2]);                    // C[31:0]

      int cache_size = (ways+1) * (partitions+1) * (line_size+1) * (sets+1);

      switch(cache_level)
      {
        case 1: f->l1 = cache_size; break;
        case 2: f->l2 = cache_size; break;
        case 3: f->l3 = cache_size; break;
        default: break;
      }
    }
    cache_id++;
  } while(cache_type>0 && cache_id<16);
}

CC_INLINE bool cpuid_is_vendor(int abcd[4], const int vendor[3])
{
  return abcd[1]==vendor[0] && abcd[3]==vendor[1] && abcd[2]==vendor[2];
}

CC_INLINE void queryCacheSizes_intel_codes(cpu_info_t* f)
{
  int abcd[4] = {0};
  int i, l1 = 0, l2 = 0, l3 = 0;
  unsigned char * bytes = (unsigned char *)(abcd)+2;
  bool check_for_p2_core2 = false;
  getcpuidex(abcd,0x00000002,0);
  for(i=0; i<14; ++i)
  {
    switch(bytes[i])
    {
      case 0x0A: l1 = 8; break;   // 0Ah   data L1 cache, 8 KB, 2 ways, 32 byte lines
      case 0x0C: l1 = 16; break;  // 0Ch   data L1 cache, 16 KB, 4 ways, 32 byte lines
      case 0x0E: l1 = 24; break;  // 0Eh   data L1 cache, 24 KB, 6 ways, 64 byte lines
      case 0x10: l1 = 16; break;  // 10h   data L1 cache, 16 KB, 4 ways, 32 byte lines (IA-64)
      case 0x15: l1 = 16; break;  // 15h   code L1 cache, 16 KB, 4 ways, 32 byte lines (IA-64)
      case 0x2C: l1 = 32; break;  // 2Ch   data L1 cache, 32 KB, 8 ways, 64 byte lines
      case 0x30: l1 = 32; break;  // 30h   code L1 cache, 32 KB, 8 ways, 64 byte lines
      case 0x60: l1 = 16; break;  // 60h   data L1 cache, 16 KB, 8 ways, 64 byte lines, sectored
      case 0x66: l1 = 8; break;   // 66h   data L1 cache, 8 KB, 4 ways, 64 byte lines, sectored
      case 0x67: l1 = 16; break;  // 67h   data L1 cache, 16 KB, 4 ways, 64 byte lines, sectored
      case 0x68: l1 = 32; break;  // 68h   data L1 cache, 32 KB, 4 ways, 64 byte lines, sectored
      case 0x1A: l2 = 96; break;   // code and data L2 cache, 96 KB, 6 ways, 64 byte lines (IA-64)
      case 0x22: l3 = 512; break;   // code and data L3 cache, 512 KB, 4 ways (!), 64 byte lines, dual-sectored
      case 0x23: l3 = 1024; break;   // code and data L3 cache, 1024 KB, 8 ways, 64 byte lines, dual-sectored
      case 0x25: l3 = 2048; break;   // code and data L3 cache, 2048 KB, 8 ways, 64 byte lines, dual-sectored
      case 0x29: l3 = 4096; break;   // code and data L3 cache, 4096 KB, 8 ways, 64 byte lines, dual-sectored
      case 0x39: l2 = 128; break;   // code and data L2 cache, 128 KB, 4 ways, 64 byte lines, sectored
      case 0x3A: l2 = 192; break;   // code and data L2 cache, 192 KB, 6 ways, 64 byte lines, sectored
      case 0x3B: l2 = 128; break;   // code and data L2 cache, 128 KB, 2 ways, 64 byte lines, sectored
      case 0x3C: l2 = 256; break;   // code and data L2 cache, 256 KB, 4 ways, 64 byte lines, sectored
      case 0x3D: l2 = 384; break;   // code and data L2 cache, 384 KB, 6 ways, 64 byte lines, sectored
      case 0x3E: l2 = 512; break;   // code and data L2 cache, 512 KB, 4 ways, 64 byte lines, sectored
      case 0x40: l2 = 0; break;   // no integrated L2 cache (P6 core) or L3 cache (P4 core)
      case 0x41: l2 = 128; break;   // code and data L2 cache, 128 KB, 4 ways, 32 byte lines
      case 0x42: l2 = 256; break;   // code and data L2 cache, 256 KB, 4 ways, 32 byte lines
      case 0x43: l2 = 512; break;   // code and data L2 cache, 512 KB, 4 ways, 32 byte lines
      case 0x44: l2 = 1024; break;   // code and data L2 cache, 1024 KB, 4 ways, 32 byte lines
      case 0x45: l2 = 2048; break;   // code and data L2 cache, 2048 KB, 4 ways, 32 byte lines
      case 0x46: l3 = 4096; break;   // code and data L3 cache, 4096 KB, 4 ways, 64 byte lines
      case 0x47: l3 = 8192; break;   // code and data L3 cache, 8192 KB, 8 ways, 64 byte lines
      case 0x48: l2 = 3072; break;   // code and data L2 cache, 3072 KB, 12 ways, 64 byte lines
      case 0x49: if(l2!=0) l3 = 4096; else {check_for_p2_core2=true; l3 = l2 = 4096;} break;// code and data L3 cache, 4096 KB, 16 ways, 64 byte lines (P4) or L2 for core2
      case 0x4A: l3 = 6144; break;   // code and data L3 cache, 6144 KB, 12 ways, 64 byte lines
      case 0x4B: l3 = 8192; break;   // code and data L3 cache, 8192 KB, 16 ways, 64 byte lines
      case 0x4C: l3 = 12288; break;   // code and data L3 cache, 12288 KB, 12 ways, 64 byte lines
      case 0x4D: l3 = 16384; break;   // code and data L3 cache, 16384 KB, 16 ways, 64 byte lines
      case 0x4E: l2 = 6144; break;   // code and data L2 cache, 6144 KB, 24 ways, 64 byte lines
      case 0x78: l2 = 1024; break;   // code and data L2 cache, 1024 KB, 4 ways, 64 byte lines
      case 0x79: l2 = 128; break;   // code and data L2 cache, 128 KB, 8 ways, 64 byte lines, dual-sectored
      case 0x7A: l2 = 256; break;   // code and data L2 cache, 256 KB, 8 ways, 64 byte lines, dual-sectored
      case 0x7B: l2 = 512; break;   // code and data L2 cache, 512 KB, 8 ways, 64 byte lines, dual-sectored
      case 0x7C: l2 = 1024; break;   // code and data L2 cache, 1024 KB, 8 ways, 64 byte lines, dual-sectored
      case 0x7D: l2 = 2048; break;   // code and data L2 cache, 2048 KB, 8 ways, 64 byte lines
      case 0x7E: l2 = 256; break;   // code and data L2 cache, 256 KB, 8 ways, 128 byte lines, sect. (IA-64)
      case 0x7F: l2 = 512; break;   // code and data L2 cache, 512 KB, 2 ways, 64 byte lines
      case 0x80: l2 = 512; break;   // code and data L2 cache, 512 KB, 8 ways, 64 byte lines
      case 0x81: l2 = 128; break;   // code and data L2 cache, 128 KB, 8 ways, 32 byte lines
      case 0x82: l2 = 256; break;   // code and data L2 cache, 256 KB, 8 ways, 32 byte lines
      case 0x83: l2 = 512; break;   // code and data L2 cache, 512 KB, 8 ways, 32 byte lines
      case 0x84: l2 = 1024; break;   // code and data L2 cache, 1024 KB, 8 ways, 32 byte lines
      case 0x85: l2 = 2048; break;   // code and data L2 cache, 2048 KB, 8 ways, 32 byte lines
      case 0x86: l2 = 512; break;   // code and data L2 cache, 512 KB, 4 ways, 64 byte lines
      case 0x87: l2 = 1024; break;   // code and data L2 cache, 1024 KB, 8 ways, 64 byte lines
      case 0x88: l3 = 2048; break;   // code and data L3 cache, 2048 KB, 4 ways, 64 byte lines (IA-64)
      case 0x89: l3 = 4096; break;   // code and data L3 cache, 4096 KB, 4 ways, 64 byte lines (IA-64)
      case 0x8A: l3 = 8192; break;   // code and data L3 cache, 8192 KB, 4 ways, 64 byte lines (IA-64)
      case 0x8D: l3 = 3072; break;   // code and data L3 cache, 3072 KB, 12 ways, 128 byte lines (IA-64)

      default: break;
    }
  }
  if(check_for_p2_core2 && l2 == l3)
    l3 = 0;
  f->l1 = l1 * 1024;
  f->l2 = l2 * 1024;
  f->l3 = l3 * 1024;
}

CC_INLINE void queryCacheSizes_intel(cpu_info_t* f, int max_std_funcs)
{
  if(max_std_funcs>=4)
    queryCacheSizes_intel_direct(f);
  else
    queryCacheSizes_intel_codes(f);
}

CC_INLINE void queryCacheSizes_amd(cpu_info_t* f)
{
  int abcd[4];
  abcd[0] = abcd[1] = abcd[2] = abcd[3] = 0;
  getcpuidex(abcd,0x80000005,0);
  f->l1 = (abcd[2] >> 24) * 1024; // C[31:24] = L1 size in KB
  abcd[0] = abcd[1] = abcd[2] = abcd[3] = 0;
  getcpuidex(abcd,0x80000006,0);
  f->l2 = (abcd[2] >> 16) * 1024; // C[31;16] = l2 cache size in KB
  f->l3 = ((abcd[3] & 0xFFFC000) >> 18) * 512 * 1024; // D[31;18] = l3 cache size in 512KB
}

CC_INLINE void queryCacheSizes(cpu_info_t* f)
{
  int max_std_funcs;
  int abcd[4];
  const int GenuineIntel[] = {0x756e6547, 0x49656e69, 0x6c65746e};
  const int AuthenticAMD[] = {0x68747541, 0x69746e65, 0x444d4163};
  const int AMDisbetter_[] = {0x69444d41, 0x74656273, 0x21726574}; // "AMDisbetter!"

  // identify the CPU vendor
  getcpuidex(abcd,0x0,0);
  max_std_funcs = abcd[1];
  if(cpuid_is_vendor(abcd,GenuineIntel))
    queryCacheSizes_intel(f,max_std_funcs);
  else if(cpuid_is_vendor(abcd,AuthenticAMD) || cpuid_is_vendor(abcd,AMDisbetter_))
    queryCacheSizes_amd(f);
  else
    // by default let's use Intel's API
    queryCacheSizes_intel(f,max_std_funcs);

  // here is the list of other vendors:
//   ||cpuid_is_vendor(abcd,"VIA VIA VIA ")
//   ||cpuid_is_vendor(abcd,"CyrixInstead")
//   ||cpuid_is_vendor(abcd,"CentaurHauls")
//   ||cpuid_is_vendor(abcd,"GenuineTMx86")
//   ||cpuid_is_vendor(abcd,"TransmetaCPU")
//   ||cpuid_is_vendor(abcd,"RiseRiseRise")
//   ||cpuid_is_vendor(abcd,"Geode by NSC")
//   ||cpuid_is_vendor(abcd,"SiS SiS SiS ")
//   ||cpuid_is_vendor(abcd,"UMC UMC UMC ")
//   ||cpuid_is_vendor(abcd,"NexGenDriven")
}

cpu_info_t* get_cpu_info()
{
    static cpu_info_t g_cpu_info[1] = {0};
    static int inited = 0;
    if (!inited) {
      cpu_info_init_feature(g_cpu_info);
      queryCacheSizes(g_cpu_info);
    }
    return g_cpu_info;
}


static int64 cGetTickCount(void)
{
#if defined WIN32 || defined _WIN32 || defined WINCE
    LARGE_INTEGER counter;
    QueryPerformanceCounter( &counter );
    return (int64)counter.QuadPart;
#elif defined __linux || defined __linux__
    struct timespec tp;
    clock_gettime(CLOCK_MONOTONIC, &tp);
    return (int64)tp.tv_sec*1000000000 + tp.tv_nsec;
#elif defined __MACH__ && defined __APPLE__
    return (int64)mach_absolute_time();
#else
    struct timeval tv;
    struct timezone tz;
    gettimeofday( &tv, &tz );
    return (int64)tv.tv_sec*1000000 + tv.tv_usec;
#endif
}

static double cGetTickFrequency(void)
{
#if defined WIN32 || defined _WIN32 || defined WINCE
    LARGE_INTEGER freq;
    QueryPerformanceFrequency(&freq);
    return (double)freq.QuadPart;
#elif defined __linux || defined __linux__
    return 1e9;
#elif defined __MACH__ && defined __APPLE__
    static double freq = 0;
    if( freq == 0 )
    {
        mach_timebase_info_data_t sTimebaseInfo;
        mach_timebase_info(&sTimebaseInfo);
        freq = sTimebaseInfo.denom*1e9/sTimebaseInfo.numer;
    }
    return freq;
#else
    return 1e6;
#endif
}

#if defined __GNUC__ && (defined __i386__ || defined __x86_64__ || defined __ppc__)
#if defined(__i386__)

static int64 getCPUTickCount(void)
{
    int64 x;
    __asm__ volatile (".byte 0x0f, 0x31" : "=A" (x));
    return x;
}
#elif defined(__x86_64__)

static int64 getCPUTickCount(void)
{
    unsigned hi, lo;
    __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
    return (int64)lo | ((int64)hi << 32);
}

#elif defined(__ppc__)

static int64 getCPUTickCount(void)
{
    int64 result = 0;
    unsigned upper, lower, tmp;
    __asm__ volatile(
                     "0:                  \n"
                     "\tmftbu   %0           \n"
                     "\tmftb    %1           \n"
                     "\tmftbu   %2           \n"
                     "\tcmpw    %2,%0        \n"
                     "\tbne     0b         \n"
                     : "=r"(upper),"=r"(lower),"=r"(tmp)
                     );
    return lower | ((int64)upper << 32);
}

#else

#error "RDTSC not defined"

#endif

#elif defined _MSC_VER && defined WIN32 && defined _M_IX86 && 0

static int64 getCPUTickCount(void)
{
    __asm _emit 0x0f;
    __asm _emit 0x31;
}

#else

//#ifdef HAVE_IPP
//int64 getCPUTickCount(void)
//{
//    return ippGetCpuClocks();
//}
//#else
static int64 getCPUTickCount(void)
{
    return cGetTickCount();
}
//#endif

#endif

static const char* getBuildInformation()
{
    static char *build_info = __DATE__ " " __TIME__;
    return build_info;
}

static char* format( const char* fmt, ... )
{
    int len, bsize = 1024;
    char* buf = MALLOC(char, bsize);

    for ( ; ; )
    {
        va_list va;
        va_start(va, fmt);
        len = vsnprintf(buf, countof(buf), fmt, va);
        va_end(va);

        if (len < 0 || len >= bsize)
        {
            bsize <<= 1;
            MYREALLOC(buf, MAX(bsize, len + 1));
            continue;
        }
        return buf;
    }
}

static const char* tempfile( const char* suffix, char* tname, int maxlen )
{
    char fname[MAX_PATH];
#ifndef WINRT
    const char *temp_dir = getenv("OPENCC_TEMP_PATH");
#endif
    *tname = 0;

#if defined WIN32 || defined _WIN32
#ifdef WINRT
    RoInitialize(RO_INIT_MULTITHREADED);
    std::wstring temp_dir = GetTempPathWinRT();

    std::wstring temp_file = GetTempFileNameWinRT(L"ocv");
    if (temp_file.empty())
        return String();

    temp_file = temp_dir.append(std::wstring(L"\\")).append(temp_file);
    DeleteFileW(temp_file.c_str());

    char aname[MAX_PATH];
    size_t copied = wcstombs(aname, temp_file.c_str(), MAX_PATH);
    CC_Assert((copied != MAX_PATH) && (copied != (size_t)-1));
    strcpy(fname, aname);
    RoUninitialize();
#else
    {
        char temp_dir2[MAX_PATH] = { 0 };
        char temp_file[MAX_PATH] = { 0 };

        if (temp_dir == 0 || temp_dir[0] == 0)
        {
            GetTempPathA(sizeof(temp_dir2), temp_dir2);
            temp_dir = temp_dir2;
        }
        if(0 == GetTempFileNameA(temp_dir, "ocv", 0, temp_file)) {
            return tname;
        }

        DeleteFileA(temp_file);

        strcpy(fname, temp_file);
    }
#endif
# else
#  ifdef ANDROID
    //char defaultTemplate[] = "/mnt/sdcard/__opencv_temp.XXXXXX";
    char defaultTemplate[] = "/data/local/tmp/__opencv_temp.XXXXXX";
#  else
    char defaultTemplate[] = "/tmp/__opencv_temp.XXXXXX";
#  endif

    if (temp_dir == 0 || temp_dir[0] == 0)
        strcpy(fname, defaultTemplate);
    else
    {
        strcpy(fname, temp_dir);
        char ech = fname[strlen(fname) - 1];
        if(ech != '/' && ech != '\\')
            strcat(fname, "/");
        strcat(fname, "__opencv_temp.XXXXXX");
    }

    const int fd = mkstemp(fname);
    if (fd == -1) return tname;

    close(fd);
    remove(fname);
# endif

    if (suffix)
    {
        if (suffix[0] != '.') {
            snprintf(tname, maxlen, "%s.%s", fname, suffix);
        } else {
            snprintf(tname, maxlen, "%s%s", fname, suffix);
        }
    }
    return tname;
}



