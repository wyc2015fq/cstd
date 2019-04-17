# 在C/C++代码中使用SSE等指令集的指令(4)SSE指令集Intrinsic函数使用 - 战斗蜗牛的专栏 - CSDN博客





2014年04月05日 03:13:45[vbskj](https://me.csdn.net/vbskj)阅读数：2040
个人分类：[SSE并行](https://blog.csdn.net/vbskj/article/category/2169499)










在[http://blog.csdn.net/gengshenghong/article/details/7008682](http://blog.csdn.net/gengshenghong/article/details/7008682)里面列举了一些手册，其中Intel Intrinsic Guide可以查询到所有的Intrinsic函数、对应的汇编指令以及如何使用等，所以，接下来就不全部进行分析，以下只分析其中一部分，从而了解如何在C/C++代码中使用这些高级指令集的基本方法，至于更多的指令的使用，查询手册就很容易理解了。


说明：下面用到的指令可能只涉及到SSE指令集，而不是SSE系列（SSE、SSE2、SSE3、SSSE3、SSE4.1、SSE4.2等）指令集。另外，下面的例子都是和SSE中浮点运算相关的指令（前面说到过，SSE新增的指令分为4类的，浮点运算只是一种一个主要的部分）。


（1）FP Load/Store/Set


使用SSE指令，首先要了解这一类用于进行初始化加载数据以及将暂存器的数据保存到内存相关的指令，我们知道，大多数SSE指令是使用的xmm0到xmm8的暂存器，那么使用之前，就需要将数据从内存加载到这些暂存器。


1. load系列，用于加载数据，从内存到暂存器。



- __m128 _mm_load_ss (float *p)  
- __m128 _mm_load_ps (float *p)  
- __m128 _mm_load1_ps (float *p)  
- __m128 _mm_loadh_pi (__m128 a, __m64 *p)  
- __m128 _mm_loadl_pi (__m128 a, __m64 *p)  
- __m128 _mm_loadr_ps (float *p)  
- __m128 _mm_loadu_ps (float *p)  

上面是从手册查询到的load系列的函数。其中，



_mm_load_ss用于scalar的加载，所以，加载一个单精度浮点数到暂存器的低字节，其它三个字节清0，（r0 := *p, r1 := r2 := r3 := 0.0）。


_mm_load_ps用于packed的加载（下面的都是用于packed的），要求p的地址是16字节对齐，否则读取的结果会出错，（r0 := p[0], r1 := p[1], r2 := p[2], r3 := p[3]）。



_mm_load1_ps表示将p地址的值，加载到暂存器的四个字节，需要多条指令完成，所以，从性能考虑，在内层循环不要使用这类指令。（r0 := r1 := r2 := r3 := *p）。



_mm_loadh_pi和_mm_loadl_pi分别用于从两个参数高底字节等组合加载。具体参考手册。



_mm_loadr_ps表示以_mm_load_ps反向的顺序加载，需要多条指令完成，当然，也要求地址是16字节对齐。（r0 := p[3], r1 := p[2], r2 := p[1], r3 := p[0]）。



_mm_loadu_ps和_mm_load_ps一样的加载，但是不要求地址是16字节对齐，对应指令为movups。



2. set系列，用于加载数据，大部分需要多条指令完成，但是可能不需要16字节对齐。



- __m128 _mm_set_ss (float w)  
- __m128 _mm_set_ps (float z, float y, float x, float w)  
- __m128 _mm_set1_ps (float w)  
- __m128 _mm_setr_ps (float z, float y, float x, float w)  
- __m128 _mm_setzero_ps ()  





这一系列函数主要是类似于load的操作，但是可能会调用多条指令去完成，方便的是可能不需要考虑对齐的问题。


_mm_set_ss对应于_mm_load_ss的功能，不需要字节对齐，需要多条指令。（r0 = w, r1 = r2 = r3 = 0.0）



_mm_set_ps对应于_mm_load_ps的功能，参数是四个单独的单精度浮点数，所以也不需要字节对齐，需要多条指令。（r0=w, r1 = x, r2 = y, r3 = z，注意顺序）



_mm_set1_ps对应于_mm_load1_ps的功能，不需要字节对齐，需要多条指令。（r0 = r1 = r2 = r3 = w）



_mm_setzero_ps是清0操作，只需要一条指令。（r0 = r1 = r2 = r3 = 0.0）



3. store系列，用于将计算结果等SSE暂存器的数据保存到内存中。



- void _mm_store_ss (float *p, __m128 a)  
- void _mm_store_ps (float *p, __m128 a)  
- void _mm_store1_ps (float *p, __m128 a)  
- void _mm_storeh_pi (__m64 *p, __m128 a)  
- void _mm_storel_pi (__m64 *p, __m128 a)  
- void _mm_storer_ps (float *p, __m128 a)  
- void _mm_storeu_ps (float *p, __m128 a)  
- void _mm_stream_ps (float *p, __m128 a)  

这一系列函数和load系列函数的功能对应，基本上都是一个反向的过程。



_mm_store_ss：一条指令，*p = a0



_mm_store_ps：一条指令，p[i] = a[i]。



_mm_store1_ps：多条指令，p[i] = a0。



_mm_storeh_pi，_mm_storel_pi：值保存其高位或低位。


_mm_storer_ps：反向，多条指令。



_mm_storeu_ps：一条指令，p[i] = a[i]，不要求16字节对齐。



_mm_stream_ps：直接写入内存，不改变cache的数据。



（2）算术指令


SSE提供了大量的浮点运算指令，包括加法、减法、乘法、除法、开方、最大值、最小值、近似求倒数、求开方的倒数等等，可见SSE指令的强大之处。那么在了解了上面的数据加载和数据保存的指令之后，使用这些算术指令就很容易了，下面以加法为例。


SSE中浮点加法的指令有：



- __m128 _mm_add_ss (__m128 a, __m128 b)  
- __m128 _mm_add_ps (__m128 a, __m128 b)  

其中，_mm_add_ss表示scalar执行模式，_mm_add_ps表示packed执行模式。



一般而言，使用SSE指令写代码，步骤为：使用load/set函数将数据从内存加载到SSE暂存器；使用相关SSE指令完成计算等；使用store系列函数将结果从暂存器保存到内存，供后面使用。


下面是一个完成加法的例子：



- #include <intrin.h>
- 
- int main(int argc, char* argv[])  
- {  
- float op1[4] = {1.0, 2.0, 3.0, 4.0};  
- float op2[4] = {1.0, 2.0, 3.0, 4.0};  
- float result[4];  
- 
-     __m128  a;  
-     __m128  b;  
-     __m128  c;  
- 
- // Load
-     a = _mm_loadu_ps(op1);  
-     b = _mm_loadu_ps(op2);  
- 
- // Calculate
-     c = _mm_add_ps(a, b);   // c = a + b
- 
- // Store
-     _mm_storeu_ps(result, c);  
- 
- /*      // Using the __m128 union to get the result.
-     printf("0: %lf\n", c.m128_f32[0]);
-     printf("1: %lf\n", c.m128_f32[1]);
-     printf("2: %lf\n", c.m128_f32[2]);
-     printf("3: %lf\n", c.m128_f32[3]);
-     */
-     printf("0: %lf\n", result[0]);  
-     printf("1: %lf\n", result[1]);  
-     printf("2: %lf\n", result[2]);  
-     printf("3: %lf\n", result[3]);  
- 
- return 0;  
- }  

这个例子，前面已经写过类似的加法例子，注释中的printf部分是利用__m128这个数据类型来获取相关的值，这个类型是一个union类型，具体定义可以参考相关头文件，但是，对于实际使用，有时候这个值是一个中间值，需要后面计算使用，就得使用store了，效率更高。上面使用的是_mm_loadu_ps和_mm_storeu_ps，不要求字节对齐，如果使用_mm_load_ps和_mm_store_ps，会发现程序会崩溃或得不到正确结果。下面是指定字节对齐后的一种实现方法：




- #include <intrin.h>
- 
- int main(int argc, char* argv[])  
- {  
- __declspec(align(16)) float op1[4] = {1.0, 2.0, 3.0, 4.0};  
- __declspec(align(16)) float op2[4] = {1.0, 2.0, 3.0, 4.0};  
-     _MM_ALIGN16 float result[4];        // A macro, same as "__declspec(align(16))" 
- 
-     __m128  a;  
-     __m128  b;  
-     __m128  c;  
- 
- // Load
-     a = _mm_load_ps(op1);  
-     b = _mm_load_ps(op2);  
- 
- // Calculate
-     c = _mm_add_ps(a, b);   // c = a + b
- 
- // Store
-     _mm_store_ps(result, c);  
- 
- /*      // Using the __m128 union to get the result.
-     printf("0: %lf\n", c.m128_f32[0]);
-     printf("1: %lf\n", c.m128_f32[1]);
-     printf("2: %lf\n", c.m128_f32[2]);
-     printf("3: %lf\n", c.m128_f32[3]);
-     */
-     printf("0: %lf\n", result[0]);  
-     printf("1: %lf\n", result[1]);  
-     printf("2: %lf\n", result[2]);  
-     printf("3: %lf\n", result[3]);  
- 
- return 0;  
- }  

（3）其它指令



除了上面的算术指令之后，SSE还有一些其它浮点处理相关的指令，比如浮点比较、数据转换、逻辑运算等，其使用都是类似的，所以就不一一分析了。重点是要掌握load/set/store系列函数，这样才能很容易的使用其他相关运算处理指令。


（4）其他指令集


了解了SSE指令集的这些函数的使用，其它指令集也就能很容易的知道如何使用了，上面提到的Intel Intrinsic Guide就包括了所有的Intel处理器的指令集的Intrinsic函数查询，包括MMX、SSE、SSE2、SSE3、SSSE3、SSE4.1、SSE4.2、AVX等。



