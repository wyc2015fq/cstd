////////////////////////////////////////////////////////////
// noifVC6.c : VC6饱和处理速度测试
// Author: zyl910
// Blog: http://www.cnblogs.com/zyl910
// URL: http://www.cnblogs.com/zyl910/archive/2012/03/27/noifopex3.html
// Version: V1.0
// Updata: 2012-03-27
////////////////////////////////////////////////////////////

#include <Windows.h>
#include <stdlib.h>
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
void runTest(char* szname, TESTPROC proc)
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
		printf("%s[%d]:\t%u\n", szname, i, tm1);

	}
}

int main(int argc, char* argv[])
{
	int i;	// 循环变量

	//printf("Hello World!\n");
	printf("== noif:VC6 ==");

	// 初始化
	srand( (unsigned)time( NULL ) );
	for(i=0; i<DATASIZE*4; ++i)
	{
		bufS[i] = (signed short)((rand()&0x1FF) - 128);	// 使数值在 [-128, 383] 区间
	}

	// 准备开始。可以将将进程优先级设为实时
	if (argc<=1)
	{
		printf("<Press any key to continue>");
		getch();
		printf("\n");
	}

	// 进行测试
	runTest("f0_if", f0_if);
	runTest("f1_min", f1_min);
	runTest("f2_neg", f2_neg);
	runTest("f3_sar", f3_sar);

	// 结束前提示
	if (argc<=1)
	{
		printf("<Press any key to exit>");
		getch();
		printf("\n");
	}

	return 0;
}
