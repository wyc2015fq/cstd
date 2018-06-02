////////////////////////////////////////////////////////////
// noifVC2010.cpp : VC2010饱和处理速度测试
// Author: zyl910
// Blog: http://www.cnblogs.com/zyl910
// URL: http://www.cnblogs.com/zyl910/archive/2012/03/28/noifopex4.html
// Version: V1.0
// Updata: 2012-02-28
////////////////////////////////////////////////////////////


#include "stdafx.h"

#include <Windows.h>
#include <stdio.h>
#include <time.h>
#include <conio.h>


// 用位掩码做饱和处理.用求负生成掩码
#define LIMITSU_FAST(n, bits) ( (n) & -((n) >= 0) | -((n) >= (1<<(bits))) )
#define LIMITSU_SAFE(n, bits) ( (LIMITSU_FAST(n, bits)) & ((1<<(bits)) - 1) )
#define LIMITSU_BYTE(n) ((BYTE)(LIMITSU_FAST(n, 8)))

// 用位掩码做饱和处理.用带符号右移生成掩码
#define LIMITSW_FAST(n, bits) ( ( (n) | ((signed short)((1<<(bits)) - 1 - (n)) >> 15) ) & ~((signed short)(n) >> 15) )
#define LIMITSW_SAFE(n, bits) ( (LIMITSW_FAST(n, bits)) & ((1<<(bits)) - 1) )
#define LIMITSW_BYTE(n) ((BYTE)(LIMITSW_FAST(n, 8)))

// 数据规模
#define DATASIZE	16384	// 128KB / (sizeof(signed short) * 4)

// 缓冲区
signed short	bufS[DATASIZE*4];	// 源缓冲区。64位的颜色（4通道，每通道16位）
BYTE	bufD[DATASIZE*4];	// 目标缓冲区。32位的颜色（4通道，每通道8位）

// 测试时的函数类型
typedef void (*TESTPROC)(BYTE* pbufD, const signed short* pbufS, int cnt);

// 用if分支做饱和处理
void f0_if(BYTE* pbufD, const signed short* pbufS, int cnt)
{
	const signed short* pS = pbufS;
	BYTE* pD = pbufD;
	int i;
	for(i=0; i<cnt; ++i)
	{
		// 分别对4个通道做饱和处理
		pD[0] = (pS[0]<0) ? 0 : ( (pS[0]>255) ? 255 : (BYTE)pS[0] );
		pD[1] = (pS[1]<0) ? 0 : ( (pS[1]>255) ? 255 : (BYTE)pS[1] );
		pD[2] = (pS[2]<0) ? 0 : ( (pS[2]>255) ? 255 : (BYTE)pS[2] );
		pD[3] = (pS[3]<0) ? 0 : ( (pS[3]>255) ? 255 : (BYTE)pS[3] );
		// next
		pS += 4;
		pD += 4;
	}
}

// 用min、max饱和处理
void f1_min(BYTE* pbufD, const signed short* pbufS, int cnt)
{
	const signed short* pS = pbufS;
	BYTE* pD = pbufD;
	int i;
	for(i=0; i<cnt; ++i)
	{
		// 分别对4个通道做饱和处理
		pD[0] = min(max(0, pS[0]), 255);
		pD[1] = min(max(0, pS[1]), 255);
		pD[2] = min(max(0, pS[2]), 255);
		pD[3] = min(max(0, pS[3]), 255);
		// next
		pS += 4;
		pD += 4;
	}
}

// 用位掩码做饱和处理.用求负生成掩码
void f2_neg(BYTE* pbufD, const signed short* pbufS, int cnt)
{
	const signed short* pS = pbufS;
	BYTE* pD = pbufD;
	int i;
	for(i=0; i<cnt; ++i)
	{
		// 分别对4个通道做饱和处理
		pD[0] = LIMITSU_BYTE(pS[0]);
		pD[1] = LIMITSU_BYTE(pS[1]);
		pD[2] = LIMITSU_BYTE(pS[2]);
		pD[3] = LIMITSU_BYTE(pS[3]);
		// next
		pS += 4;
		pD += 4;
	}
}

// 用位掩码做饱和处理.用带符号右移生成掩码
void f3_sar(BYTE* pbufD, const signed short* pbufS, int cnt)
{
	const signed short* pS = pbufS;
	BYTE* pD = pbufD;
	int i;
	for(i=0; i<cnt; ++i)
	{
		// 分别对4个通道做饱和处理
		pD[0] = LIMITSW_BYTE(pS[0]);
		pD[1] = LIMITSW_BYTE(pS[1]);
		pD[2] = LIMITSW_BYTE(pS[2]);
		pD[3] = LIMITSW_BYTE(pS[3]);
		// next
		pS += 4;
		pD += 4;
	}
}

// 进行测试
void runTest(_TCHAR* szname, TESTPROC proc)
{
	int i,j;
	DWORD	tm0, tm1;	// 存储时间
	for(i=1; i<=3; ++i)	// 多次测试
	{
		//tm0 = GetTickCount();
		tm0 = timeGetTime();
		// main
		for(j=1; j<=4000; ++j)	// 重复运算几次延长时间，避免计时精度问题
		{
			proc(bufD, bufS, DATASIZE);
		}
		// show
		//tm1 = GetTickCount() - tm0;
		tm1 = timeGetTime() - tm0;
		_tprintf(_T("%s[%d]:\t%u\n"), szname, i, tm1);

	}
}

// 获取程序位数（被编译为多少位的代码）
int GetProgramBits()
{
	return sizeof(int*) * 8;
}

// 安全的取得真实系统信息
VOID SafeGetNativeSystemInfo(__out LPSYSTEM_INFO lpSystemInfo)
{
	if (NULL==lpSystemInfo)	return;
	typedef VOID (WINAPI *LPFN_GetNativeSystemInfo)(LPSYSTEM_INFO lpSystemInfo);
	LPFN_GetNativeSystemInfo fnGetNativeSystemInfo = (LPFN_GetNativeSystemInfo)GetProcAddress( GetModuleHandle(_T("kernel32")), "GetNativeSystemInfo");;
	if (NULL != fnGetNativeSystemInfo)
	{
		fnGetNativeSystemInfo(lpSystemInfo);
	}
	else
	{
		GetSystemInfo(lpSystemInfo);
	}
}

// 获取操作系统位数
int GetSystemBits()
{
	SYSTEM_INFO si;
	SafeGetNativeSystemInfo(&si);
 	if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64 ||
		si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64 )
	{
		return 64;
	}
	return 32;
}

int _tmain(int argc, _TCHAR* argv[])
{
	int i;	// 循环变量

	//printf("Hello World!\n");
	const int nBitCode = GetProgramBits();
	const int nBitSys = GetSystemBits();
	_tprintf(_T("== noif:VC2010(%d) on %dbit =="), nBitCode, nBitSys);

	// 初始化
	srand( (unsigned)time( NULL ) );
	for(i=0; i<DATASIZE*4; ++i)
	{
		bufS[i] = (signed short)((rand()&0x1FF) - 128);	// 使数值在 [-128, 383] 区间
	}

	// 准备开始。可以将将进程优先级设为实时
	if (argc<=1)
	{
		_tprintf(_T("<Press any key to continue>"));
		_getch();
	}
	_tprintf(_T("\n"));

	// 进行测试
	runTest(_T("f0_if"), f0_if);
	runTest(_T("f1_min"), f1_min);
	runTest(_T("f2_neg"), f2_neg);
	runTest(_T("f3_sar"), f3_sar);

	// 结束前提示
	if (argc<=1)
	{
		_tprintf(_T("<Press any key to exit>"));
		_getch();
	}
	_tprintf(_T("\n"));

	return 0;
}

