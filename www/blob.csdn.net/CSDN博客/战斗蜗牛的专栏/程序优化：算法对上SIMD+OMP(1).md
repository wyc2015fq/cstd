# 程序优化：算法对上SIMD+OMP(1) - 战斗蜗牛的专栏 - CSDN博客





2014年04月05日 03:22:35[vbskj](https://me.csdn.net/vbskj)阅读数：1059








**传统处理方法**

     传统处理方法纯粹基于C＋＋，通过指针操作进行处理。通常的优化是使用定点数来代替浮点数，这里使用的方法为：

     比值＝(颜色分量值 * 65535) / 255

     转换成位移操作：

     比值＝(颜色分量值 << 16) >> 8

     进行一步简化：

         比值＝颜色分量值 << 8



代码如下：

float test_Normal_Filter()

{

    BYTE* buf = (BYTE *)malloc(1024 * 1024 * sizeof(int)); //分配内存：1024X1024 32bpp

    BYTE* ptr = buf; // 操作指针

    // 背景色，该颜色将与前景色进行溶

int background_R = 0xF8, background_G = 0xF8, background_B = 0xF8; 合

int dr, dg, db; // 颜色过滤比值



    BEGIN_PERF() //记录开始时间



for ( int h = 0; h < 1024; h++ )     // 按行循环

    {

for (int w = 0; w < 1024; w++ )    // 处理行

        {

if ( (*(int *)ptr & 0x00070707) != 0 ) // 如果符合过滤条件，则进行处理

            {

                dr = (int)*(ptr + 2) << 8; // 计算比值：R * 65535 / 255

                dg = (int)*(ptr + 1) << 8;

                db = (int)*(ptr + 0) << 8;



               //依次处理3个颜色分量

                *(ptr + 2) = ((int)*(ptr + 2) * (65535 - dr) + background_R * dr) >> 16;

                *(ptr + 1) = ((int)*(ptr + 1) * (65535 - dg) + background_R * dg) >> 16;

                *(ptr + 0) = ((int)*(ptr + 0) * (65535 - db) + background_R * db) >> 16;

            }



            ptr += 4; // 以4为步进

        }

    }



    END_PERF() // 记录结束时间



    free(buf); // 释放内存



return GET_PERF(); // 返回操作花费的时间

}



在测试中发现，如果不使用OMP，该方法其实是最快的。这样的数据无关处理在形成流水线后，速度那是相当的快!



