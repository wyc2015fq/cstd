# 在C/C++代码中使用SSE等指令集的指令(5)SSE进行加法运算简单的性能测试 - 战斗蜗牛的专栏 - CSDN博客





2014年04月05日 03:16:53[vbskj](https://me.csdn.net/vbskj)阅读数：1706









下面是一个简单的测试SSE指令性能的程序，可以看到明显的性能提升。


（说明：程序中的timing.h使用的是[http://blog.csdn.net/gengshenghong/article/details/6973086](http://blog.csdn.net/gengshenghong/article/details/6973086)中介绍的时间间隔获取方法）



- #define WIN
- #include "timing.h"
- #include <intrin.h>
- #include <stdlib.h>
- #include <math.h>
- 
- #define N 4*100000      // 注意：必须是4的倍数，否则使用SSE指令计算，要进行一些处理，从而保证正确。
- _MM_ALIGN16 float op1[N];  
- _MM_ALIGN16 float op2[N];  
- _MM_ALIGN16 float result1[N];  
- _MM_ALIGN16 float result2[N];  
- 
- void init()  
- {  
- for(int i = 0;i < N; i++)  
-     {  
-         op1[i] = (float)rand()/(float)RAND_MAX;  
-         op2[i] = (float)rand()/(float)RAND_MAX;  
-     }  
- }  
- 
- void checkResult(int debug)  
- {  
- bool isSame = true;  
- for(int i = 0;i < N; i++)  
-     {  
- if (debug)  
-         {  
-             printf("%lf     %lf\n", result1[i], result2[i]);  
-         }  
- else
-         {  
- if (fabs(result1[i] - result2[i]) > 0.000001)  
-             {  
-                 isSame = false;  
- break;  
-             }  
-         }  
-     }  
- if (!debug) {  
- if (isSame)  
-             printf("Result is Same\n");  
- else
-             printf("Result is not same\n");  
-     }  
- }  
- 
- void add1()  
- {  
- for(int i = 0; i < N;i++)  
-         result1[i] = op1[i] + op2[i];  
- }  
- 
- void add2()  
- {  
-     __m128  a;  
-     __m128  b;  
-     __m128  c;  
- 
- for(int i = 0; i < N;i = i + 4)  
-     {  
- // Load
-         a = _mm_load_ps(op1 + i);  
-         b = _mm_load_ps(op2 + i);  
- 
-         c = _mm_add_ps(a, b);   // c = a + b
- 
-         _mm_store_ps(result2 + i, c);  
-     }  
- }  
- 
- int main(int argc, char* argv[])  
- {  
-     init();  
-     srand((unsigned int)time(NULL));  
- 
-     printf("Add a vector:\n");  
-     startTiming();  
-     add1();  
-     stopWithPrintTiming();  
- 
-     printf("\n");  
-     printf("Add a vector with SSE instructions:\n");  
-     startTiming();  
-     add2();  
-     stopWithPrintTiming();  
- 
-     printf("\n");  
-     checkResult(0);  
- 
- return 0;  
- }  

![](http://hi.csdn.net/attachment/201111/25/0_13221936047OiF.gif)

# [](http://blog.csdn.net/gengshenghong/article/details/7011373)



