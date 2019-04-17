# 程序优化：算法对上SIMD+OMP(2) - 战斗蜗牛的专栏 - CSDN博客





2014年04月05日 03:23:02[vbskj](https://me.csdn.net/vbskj)阅读数：1970









传统方法+OpenMP


使用OpenMP对传统方法进行优化。感谢OpenMP，实现多线程优化方便多了!多核时代，OpenMP将成为我们的利器!

float test_Normal_OMP_Filter()

{

    BYTE* buf = (BYTE *)malloc(1024 * 1024 * sizeof(int));  //分配内存

    BYTE* ptr = buf; // 内存指针

    //背景色，用于和前景色进行溶合

int background_R = 0xF8, background_G = 0xF8, background_B = 0xF8;

int dr, dg, db; // 过滤比值分量



    BEGIN_PERF() // 计时开始


    // 内外层都使用默认的OMP，指定不同的线程数会得出特定于机器的结果

#pragma omp parallel for

for ( int h = 0; h < 1024; h++ )

    {

        //内层的线程

#pragma omp parallel for

for (int w = 0; w < 1024; w++ )

        {

            // 具体操作过程与传统方法一样

if ( (*(int *)ptr & 0x00070707) != 0 )

            {

                dr = (int)*(ptr + 2) << 8;

                dg = (int)*(ptr + 1) << 8;

                db = (int)*(ptr + 0) << 8;



                *(ptr + 2) = ((int)*(ptr + 2) * (65535 - dr) + background_R * dr) >> 16;

                *(ptr + 1) = ((int)*(ptr + 1) * (65535 - dg) + background_R * dg) >> 16;

                *(ptr + 0) = ((int)*(ptr + 0) * (65535 - db) + background_R * db) >> 16;

            }



            ptr += 4;

        }

    }



    END_PERF() // 结束计时



    free(buf); // 释放内存



return GET_PERF();  //返回计时结果

}

使用OpenMP这个利器，即使使用默认设定，也能得到比非OMP方法快17%-23%的结果。看来，多核时代OpenMP必将大行其道!



