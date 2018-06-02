////////////////////////////////////////////////////////////
// noifVC6s.c : VC6饱和处理速度测试SIMD版（MMX、SSE）
// Author: zyl910
// Blog: http://www.cnblogs.com/zyl910
// URL: http://www.cnblogs.com/zyl910/archive/2012/04/12/noifopex8.html
// Version: V1.0
// Updata: 2012-04-12
////////////////////////////////////////////////////////////

#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <conio.h>

// MMX, SSE, SSE2
#include <emmintrin.h>


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

// 缓冲区。SSE需要按128位对齐
__declspec(align(16)) signed short	bufS[DATASIZE*4];	// 源缓冲区。64位的颜色（4通道，每通道16位）
__declspec(align(16)) BYTE	bufD[DATASIZE*4];	// 目标缓冲区。32位的颜色（4通道，每通道8位）

// 测试时的函数类型
typedef void (*TESTPROC)(BYTE* pbufD, const signed short* pbufS, int cnt);


// http://www.cnblogs.com/zyl910/archive/2012/03/01/checksimd.html
// SSE系列指令集的支持级别. simd_sse_level 函数的返回值。
#define SIMD_SSE_NONE	0	// 不支持
#define SIMD_SSE_1	1	// SSE
#define SIMD_SSE_2	2	// SSE2
#define SIMD_SSE_3	3	// SSE3
#define SIMD_SSE_3S	4	// SSSE3
#define SIMD_SSE_41	5	// SSE4.1
#define SIMD_SSE_42	6	// SSE4.2

const char*	simd_sse_names[] = {
	"None",
	"SSE",
	"SSE2",
	"SSE3",
	"SSSE3",
	"SSE4.1",
	"SSE4.2",
};

// 是否支持MMX指令集
BOOL	simd_mmx()
{
	const DWORD	BIT_DX_MMX = 0x00800000;	// bit 23
	DWORD	v_edx;

	// check processor support
	__try 
	{
		_asm 
		{
			mov eax, 1
			cpuid
			mov v_edx, edx
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		return FALSE;
	}
	if ( v_edx & BIT_DX_MMX )
	{
		// check OS support
		__try 
		{
			_asm
			{
				pxor mm0, mm0	// executing any MMX instruction
				emms
			}
			return TRUE;
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
		}
	}
	return FALSE;
}

// 检测SSE系列指令集的支持级别
int	simd_sse_level()
{
	const DWORD	BIT_D_SSE = 0x02000000;	// bit 25
	const DWORD	BIT_D_SSE2 = 0x04000000;	// bit 26
	const DWORD	BIT_C_SSE3 = 0x00000001;	// bit 0
	const DWORD	BIT_C_SSSE3 = 0x00000100;	// bit 9
	const DWORD	BIT_C_SSE41 = 0x00080000;	// bit 19
	const DWORD	BIT_C_SSE42 = 0x00100000;	// bit 20
	BYTE	rt = SIMD_SSE_NONE;	// result
	DWORD	v_edx;
	DWORD	v_ecx;

	// check processor support
	__try 
	{
		_asm 
		{
			mov eax, 1
			cpuid
			mov v_edx, edx
			mov v_ecx, ecx
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		return SIMD_SSE_NONE;
	}
	if ( v_edx & BIT_D_SSE )
	{
		rt = SIMD_SSE_1;
		if ( v_edx & BIT_D_SSE2 )
		{
			rt = SIMD_SSE_2;
			if ( v_ecx & BIT_C_SSE3 )
			{
				rt = SIMD_SSE_3;
				if ( v_ecx & BIT_C_SSSE3 )
				{
					rt = SIMD_SSE_3S;
					if ( v_ecx & BIT_C_SSE41 )
					{
						rt = SIMD_SSE_41;
						if ( v_ecx & BIT_C_SSE42 )
						{
							rt = SIMD_SSE_42;
						}
					}
				}
			}
		}
	}

	// check OS support
	__try 
	{
		_asm
		{
			xorps xmm0, xmm0	// executing any SSE instruction
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		return SIMD_SSE_NONE;
	}

	return rt;
}


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

// 饱和处理MMX版
void f4_mmx(BYTE* pbufD, const signed short* pbufS, int cnt)
{
	//const signed short* pS = pbufS;
	//BYTE* pD = pbufD;
	const __m64* pS = (const __m64*)pbufS;
	__m64* pD = (__m64*)pbufD;
	int i;
	for(i=0; i<cnt; i+=2)
	{
		// 同时对两个像素做饱和处理。即 将两个64位像素（4通道，每分量为带符号16位） 转为 两个32位像素（每分量为无符号8位）。
		pD[0] = _mm_packs_pu16(pS[0], pS[1]);	// 饱和方式数据打包（带符号16位->无符号8位）。等价于 for(i=0;i<4;++i){ pD[0].uB[i]=SU(pS[0].iW[i]); pD[0].uB[4+i]=SU(pS[1].iW[i]); }
		// next
		pS += 2;
		pD += 1;
	}

	// MMX状态置空
	_mm_empty();
}

// 饱和处理SSE版
void f5_sse(BYTE* pbufD, const signed short* pbufS, int cnt)
{
	//const signed short* pS = pbufS;
	//BYTE* pD = pbufD;
	const __m128i* pS = (const __m128i*)pbufS;
	__m128i* pD = (__m128i*)pbufD;
	int i;
	for(i=0; i<cnt; i+=4)
	{
		// 同时对四个像素做饱和处理。即 将四个64位像素（4通道，每分量为带符号16位） 转为 四个32位像素（每分量为无符号8位）。
		pD[0] = _mm_packus_epi16(pS[0], pS[1]);	// 饱和方式数据打包（带符号16位->无符号8位）。等价于 for(i=0;i<8;++i){ pD[0].uB[i]=SU(pS[0].iW[i]); r.uB[8+i]=SU(pS[1].iW[i]); }
		// next
		pS += 2;
		pD += 1;
	}
}

// 进行测试
void runTest(char* szname, TESTPROC proc)
{
	const int nLoop = 16;	// 使用MMX/SSE指令时速度太快了，只好再多循环几次
	int i,j,k;
	DWORD	tm0, tm1;	// 存储时间
	for(i=1; i<=3; ++i)	// 多次测试
	{
		//tm0 = GetTickCount();
		tm0 = timeGetTime();
		// main
		for(k=1; k<=nLoop; ++k)
		{
			for(j=1; j<=4000; ++j)	// 重复运算几次延长时间，避免计时精度问题
			{
				proc(bufD, bufS, DATASIZE);
			}
		}
		// show
		//tm1 = GetTickCount() - tm0;
		tm1 = timeGetTime() - tm0;
		printf("%s[%d]:\t%.1f\n", szname, i, (double)tm1/nLoop);
		// check
		//if (1==i)
		//{
		//	// 检查结果
		//	for(j=0; j<=16; ++j)
		//	printf("[%d]:\t%d\t%u\n", j, bufS[j], bufD[j]);
		//}

	}
}

int main(int argc, char* argv[])
{
	int i;	// 循环变量

	//printf("Hello World!\n");
	printf("== noif:VC6 SIMD ==");

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
	//runTest("f0_if", f0_if);
	//runTest("f1_min", f1_min);
	//runTest("f2_neg", f2_neg);
	//runTest("f3_sar", f3_sar);
	if (simd_mmx())	runTest("f4_mmx", f4_mmx);
	if (simd_sse_level()>=SIMD_SSE_2)	runTest("f5_sse", f5_sse);

	// 结束前提示
	if (argc<=1)
	{
		printf("<Press any key to exit>");
		getch();
		printf("\n");
	}

	return 0;
}
