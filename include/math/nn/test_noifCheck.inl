////////////////////////////////////////////////////////////
// noifCheck.c : 检查验证：用位掩码代替分支
// Author: zyl910
// Blog: http://www.cnblogs.com/zyl910
// URL: http://www.cnblogs.com/zyl910/archive/2012/03/12/noifopex1.html
// Version: V1.0
// Updata: 2012-03-12
////////////////////////////////////////////////////////////

#include <Windows.h>
#include <stdio.h>

// 用位掩码做饱和处理.用求负生成掩码
#define LIMITSU_FAST(n, bits) ( (n) & -((n) >= 0) | -((n) >= (1<<(bits))) )
#define LIMITSU_SAFE(n, bits) ( (LIMITSU_FAST(n, bits)) & ((1<<(bits)) - 1) )
#define LIMITSU_BYTE(n) ((BYTE)(LIMITSU_FAST(n, 8)))

// 用位掩码做饱和处理.用带符号右移生成掩码
//#define LIMITSW_FAST(n, bits) ( (n) & ~((signed short)(n) >> 15) | ((signed short)((1<<(bits)) - 1 - (n)) >> 15) )
#define LIMITSW_FAST(n, bits) ( ( (n) | ((signed short)((1<<(bits)) - 1 - (n)) >> 15) ) & ~((signed short)(n) >> 15) )
#define LIMITSW_SAFE(n, bits) ( (LIMITSW_FAST(n, bits)) & ((1<<(bits)) - 1) )
#define LIMITSW_BYTE(n) ((BYTE)(LIMITSW_FAST(n, 8)))

signed short	buf[0x10000];	// 将数值放在数组中，避免编译器过度优化

int main(int argc, char* argv[])
{
	int i;	// 循环变量（32位）
	signed short n;	// 当前数值
	signed short m;	// 临时变量
	BYTE	by0;	// 用if分支做饱和处理
	BYTE	by1;	// 用位掩码做饱和处理.用求负生成掩码
	BYTE	by2;	// 用位掩码做饱和处理.用带符号右移生成掩码

	//printf("Hello World!\n");
	printf("== noifCheck ==\n");

	// 初始化buf
	for(i=0; i<0x10000; ++i)
	{
		buf[i] = (signed short)(i - 0x8000);
		//printf("%d\n", buf[i]);
	}

	// 检查 “<0”处理
	printf("[Test: less0]\n");
	for(i=0; i<0x8100; ++i)	// [-32768, 255]
	//for(i=0x7FFE; i<=0x8002; ++i)	// [-2, 2]
	{
		// 加载数值
		n = buf[i];

		// 用if分支做饱和处理
		m = n;
		if (m < 0) m = 0;
		by0 = (BYTE)m;

		// 用位掩码做饱和处理.用求负生成掩码
		by1 = (BYTE)(n & -(n >= 0));
		if (by1 != by0)	printf("[Error] 1.1 neg: [%d] %d!=%d\n", n, by0, by1);	// 验证

		// 用位掩码做饱和处理.用带符号右移生成掩码
		by2 = (BYTE)(n & ~((signed short)n >> 15));
		if (by2 != by0)	printf("[Error] 1.2 sar: [%d] %d!=%d\n", n, by0, by2);	// 验证
	}

	// 检查 “>255”处理
	printf("[Test: great255]\n");
	for(i=0x8000; i<0x10000; ++i)	// [0, 32767]
	//for(i=0x80FE; i<=0x8102; ++i)	// [254, 258]
	{
		// 加载数值
		n = buf[i];

		// 用if分支做饱和处理
		m = n;
		if (m > 255) m = 255;
		by0 = (BYTE)m;

		// 用位掩码做饱和处理.用求负生成掩码
		by1 = (BYTE)(n | -(n >= 256) );
		if (by1 != by0)	printf("[Error] 2.1 neg: [%d] %d!=%d\n", n, by0, by1);	// 验证

		// 用位掩码做饱和处理.用带符号右移生成掩码
		by2 = (BYTE)(n | ((signed short)(255-n) >> 15));
		if (by2 != by0)	printf("[Error] 2.2 sar: [%d] %d!=%d\n", n, by0, by2);	// 验证
	}

	// 检查 饱和处理
	printf("[Test: saturation]\n");
	for(i=0; i<0x10000; ++i)	// [-32768, 32767]
	//for(i=0x7FFE; i<=0x8102; ++i)	// [-2, 258]
	{
		// 加载数值
		n = buf[i];

		// 用if分支做饱和处理
		m = n;
		if (m < 0) m = 0;
		else if (m > 255) m = 255;
		by0 = (BYTE)m;

		// 用位掩码做饱和处理.用求负生成掩码
		by1 = LIMITSU_BYTE(n);
		if (by1 != by0)	printf("[Error] 3.1 neg: [%d] %d!=%d\n", n, by0, by1);	// 验证

		// 用位掩码做饱和处理.用求负生成掩码
		by2 = LIMITSW_BYTE(n);
		if (by2 != by0)	printf("[Error] 3.2 sar: [%d] %d!=%d\n", n, by0, by2);	// 验证
	}

	return 0;
}

