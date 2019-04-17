# 程序优化：算法对上SIMD+OMP(3) - 战斗蜗牛的专栏 - CSDN博客





2014年04月05日 03:23:29[vbskj](https://me.csdn.net/vbskj)阅读数：1192








**使用Intrinsics方法，实现SIMD处理**

     使用Intrinsics，可以利用硬件的SIMD指令进行处理。MMX，SSE，SSE2（AMD已经通过交叉授权取得该技术）看起来真有那么美好么？让我们拭目以待。

     在处理中同样使用了定点数技术。



float test_SIMD_Filter()

{

// 模拟申请X1024 32bpp的图像内存

__m128i* buf = (__m128i *)_mm_malloc(1024 * 1024 * sizeof(int),16);

// 0x0000FFFF0000FFFF0000FFFF0000FFFF 用于进行计算过滤比值的65535

__m128i _65535 = _mm_set1_epi32(0xFFFF);

// 背景色，假设为0xFFF8F8F8

__m128i pixel_bg = _mm_set_epi32(0x000000FF, 0x000000F8, 0x000000F8, 0x000000F8);

// 用于滤掉颜色中的ALPHA分量

__m128i noalpha_mask = _mm_set_epi32(0x000000FF, 0x000000FF, 0x0000000FF,0x0000000FF);

// 小于这个颜色的值将被滤掉

__m128i filter_val = _mm_set_epi32(0x00000000, 0x00000008, 0x000000008, 0x00000008);

__m128i* ptr = buf; // 图像数据指针



//先初始化数据

for ( int h = 0; h < 1024; h++ )    // 按行循环

    {

for ( int w = 0; w < 1024 / 4; w++ ) // 一次中四个点(4X32 = 128)

        {

// 将4个像素设为比较有特色的数值

            *(__m128i *)ptr = _mm_set_epi32(0xFF112233, 0xFF445566, 0xFF778899, 0xFFAABBCC);



            ptr++; // 下四个点

        }

    }



    ptr = buf; // 开始模拟处理



    BEGIN_PERF() // 开始计数



for ( int h = 0; h < 1024; h++ ) // 还是按行循环

    {

for ( int w = 0; w < 1024 / 4; w++ ) // 一次四个点

        {

__m128i pixel = *ptr; // 4个像素：0xFF112233 0xFF445566 0xFF778899 0xFFAABBCC

            // 取出前两个像素，成为－＞00FF, 0011, 0022, 0033, 00FF, 0044, 0055, 0066

__m128i pixel_1234 = _mm_unpacklo_epi8(pixel, _mm_setzero_si128());

//取出前后两个像素，成为－＞00FF, 0077, 0088, 0099, 00FF, 00AA, 00BB, 00CC

__m128i pixel_5678 = _mm_unpackhi_epi8(pixel, _mm_setzero_si128());

//因为涉及到32位乘法，所以还需要将像素的颜色分量扩展成32位格式

//->00000000 00000011 00000022 00000033 第一个像素

__m128i pixel_12 = _mm_unpacklo_epi8(pixel_1234, _mm_setzero_si128());

//->00000000 00000044 00000055 00000066 第二个像素

__m128i pixel_34 = _mm_unpackhi_epi8(pixel_1234, _mm_setzero_si128());

//->00000000 00000077 00000088 00000099　第三个像素

__m128i pixel_56 = _mm_unpacklo_epi8(pixel_5678, _mm_setzero_si128());

//->00000000 000000AA 000000BB 000000CC 第四个像素

__m128i pixel_78 = _mm_unpackhi_epi8(pixel_5678, _mm_setzero_si128());

//先减，然后比较是否为0，类似于传统方法中的减法和＆操作

__m128i cmp_res = _mm_cmplt_epi32(_mm_sub_epi32(noalpha_mask, pixel_12), filter_val);

__m128i delta, bg; // 过滤比值，背景色



         　//先判断第一个像素

if ( _mm_cvtsi128_si32(cmp_res) != 0 ) // 为了比较，需要转换成整数

            {

// 计算过滤比值

                delta = _mm_slli_epi32(pixel_12, 8);



// 分别计算位的低，高位，然后再或（高位要左移位），得到过滤后的背景色

                bg = _mm_or_si128( _mm_mullo_epi16(pixel_bg, delta),

_mm_slli_epi32(_mm_mulhi_epu16(pixel_bg, delta), 16));



                delta = _mm_sub_epi32(_65535, delta); // 65535-比例为原像素的比例



// 与计算背景过滤的方法相同

                pixel_12 = _mm_or_si128(_mm_mullo_epi16(pixel_12, delta),

_mm_slli_epi32(_mm_mulhi_epu16(pixel_12, delta), 16));



                pixel_12 = _mm_srli_epi32(pixel_12, 16); // 从定点数还原

            }



             //第三个像素

             ....



             // 第四个像素略

            ......

            //最后还要将数据拼装回去

            // 第1,2个像素

pixel_12 = _mm_packs_epi32(pixel_12, pixel_34);

// 第3,4个像素

pixel_56 = _mm_packs_epi32(pixel_56, pixel_78);

//写回

            *ptr++ = _mm_packs_epi16(pixel_12, pixel_56);

        }

    }



    END_PERF() // 停止计时



    _mm_free(buf); // 释放内存



return GET_PERF(); // 返回结果

}

使用SIMD，一次过处理4个像素，貌似很快的说，但各种扩展操作抵消了性能增长，因此速度大幅落后于传统算法!



