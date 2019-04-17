# FFMPEG学习【libavutil】：Mathematics - 一世豁然的专栏 - CSDN博客





2017年07月15日 11:20:31[一世豁然](https://me.csdn.net/Explorer_day)阅读数：288








使用时间戳和时基的数学实用程序。




一、模块

[AVRational](https://ffmpeg.org/doxygen/trunk/group__lavu__math__rational.html)


有理数计算







二、宏


|#define|[ff_ctz](https://ffmpeg.org/doxygen/trunk/group__lavu__math.html#gaf596632977edc53628f534e10e3f0141)[ff_ctz_c](https://ffmpeg.org/doxygen/trunk/group__lavu__math.html#gab430f936fa1fdcc5e529d7f1b4417a89)|
|----|----|
|| |
|#define|[ff_ctzll](https://ffmpeg.org/doxygen/trunk/group__lavu__math.html#ga127b9c4c6a32971768e724734dc183f3)[ff_ctzll_c](https://ffmpeg.org/doxygen/trunk/group__lavu__math.html#ga8d62c3b28dda6fca2c95fcfc3bee6213)|
|| |
|#define|[ff_clz](https://ffmpeg.org/doxygen/trunk/group__lavu__math.html#ga92d9a5e6b5e5aa746dca17c90142a82b)[ff_clz_c](https://ffmpeg.org/doxygen/trunk/group__lavu__math.html#gae41794ba6101acd9ab0d323a88d1ddc5)|
|| |
|#define|[av_parity](https://ffmpeg.org/doxygen/trunk/group__lavu__math.html#ga2c4939720b11c27cf01387493edba4e1)   __builtin_parity|





三、枚举


|enum|[AVRounding](https://ffmpeg.org/doxygen/trunk/group__lavu__math.html#ga921d656eaf2c4d6800a734a13af021d0) { [AV_ROUND_ZERO](https://ffmpeg.org/doxygen/trunk/group__lavu__math.html#gga921d656eaf2c4d6800a734a13af021d0a35e291756025351803b97f7522b56eab) = 0, //[AV_ROUND_INF](https://ffmpeg.org/doxygen/trunk/group__lavu__math.html#gga921d656eaf2c4d6800a734a13af021d0a0ac08aede35de739ec5d0c1ba6217126) = 1, [AV_ROUND_DOWN](https://ffmpeg.org/doxygen/trunk/group__lavu__math.html#gga921d656eaf2c4d6800a734a13af021d0a4a4b2c2c77b7e20884440f7e57cfdb98) = 2, [AV_ROUND_UP](https://ffmpeg.org/doxygen/trunk/group__lavu__math.html#gga921d656eaf2c4d6800a734a13af021d0aa1d28e08e2f8b49b256e056f93038c1a) = 3, [AV_ROUND_NEAR_INF](https://ffmpeg.org/doxygen/trunk/group__lavu__math.html#gga921d656eaf2c4d6800a734a13af021d0aea95b00154ff83c740b46376dfa06e11) = 5, [AV_ROUND_PASS_MINMAX](https://ffmpeg.org/doxygen/trunk/group__lavu__math.html#gga921d656eaf2c4d6800a734a13af021d0a7fcb56631d6898b282d6c78ce6c14a43) = 8192 }|
|----|----|

舍入方法。







四、函数


|static [av_always_inline](https://ffmpeg.org/doxygen/trunk/attributes_8h.html#a28f50f4c125959216885f01d56dbe866)[av_const](https://ffmpeg.org/doxygen/trunk/attributes_8h.html#a9eaead3cde73464fd66bab4cef9307ad)[int](https://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[ff_ctz_c](https://ffmpeg.org/doxygen/trunk/group__lavu__math.html#gab430f936fa1fdcc5e529d7f1b4417a89) ([int](https://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) v)|
|----|----|

尾随零位计数。

参数：v：输入值。 如果v为0，结果为未定义。

返回：尾随0位数





|static [av_always_inline](https://ffmpeg.org/doxygen/trunk/attributes_8h.html#a28f50f4c125959216885f01d56dbe866)[av_const](https://ffmpeg.org/doxygen/trunk/attributes_8h.html#a9eaead3cde73464fd66bab4cef9307ad)[int](https://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[ff_ctzll_c](https://ffmpeg.org/doxygen/trunk/group__lavu__math.html#ga8d62c3b28dda6fca2c95fcfc3bee6213) (long long v)|
|----|----|



|static [av_always_inline](https://ffmpeg.org/doxygen/trunk/attributes_8h.html#a28f50f4c125959216885f01d56dbe866)[av_const](https://ffmpeg.org/doxygen/trunk/attributes_8h.html#a9eaead3cde73464fd66bab4cef9307ad) unsigned|[ff_clz_c](https://ffmpeg.org/doxygen/trunk/group__lavu__math.html#gae41794ba6101acd9ab0d323a88d1ddc5) (unsigned x)|
|----|----|


|int64_t [av_const](https://ffmpeg.org/doxygen/trunk/attributes_8h.html#a9eaead3cde73464fd66bab4cef9307ad)|[av_gcd](https://ffmpeg.org/doxygen/trunk/group__lavu__math.html#ga952147c7a40c48a05c39e918b153ba99) (int64_t [a](https://ffmpeg.org/doxygen/trunk/h264pred__template_8c.html#a4124bc0a9335c27f086f24ba207a4912), int64_t [b](https://ffmpeg.org/doxygen/trunk/vf__curves_8c.html#a0a667482238bcca3ac201c1cd09c5072))|
|----|----|

计算两个整数操作数的最大公约数。

参数：a，b：操作数

返回：和b的GCD达到标志; 如果a> = 0且b> = 0，则返回值为> = 0; 如果一个== 0和b == 0，返回0。



|int64_t|[av_rescale](https://ffmpeg.org/doxygen/trunk/group__lavu__math.html#ga3daf97178dd1b08b5e916be381cd33e4) (int64_t [a](https://ffmpeg.org/doxygen/trunk/h264pred__template_8c.html#a4124bc0a9335c27f086f24ba207a4912), int64_t [b](https://ffmpeg.org/doxygen/trunk/vf__curves_8c.html#a0a667482238bcca3ac201c1cd09c5072), int64_t [c](https://ffmpeg.org/doxygen/trunk/target__dec__fuzzer_8c.html#acfe27e61edea06412363421f1a013e81)) [av_const](https://ffmpeg.org/doxygen/trunk/attributes_8h.html#a9eaead3cde73464fd66bab4cef9307ad)|
|----|----|

重新缩放64位整数，舍入到最接近。

该操作在数学上等同于* b / c，但直接写入可以溢出。


此函数等同于AV_ROUND_NEAR_INF的av_rescale_rnd（）。






|int64_t|[av_rescale_rnd](https://ffmpeg.org/doxygen/trunk/group__lavu__math.html#ga82d40664213508918093822461cc597e) (int64_t [a](https://ffmpeg.org/doxygen/trunk/h264pred__template_8c.html#a4124bc0a9335c27f086f24ba207a4912), int64_t [b](https://ffmpeg.org/doxygen/trunk/vf__curves_8c.html#a0a667482238bcca3ac201c1cd09c5072), int64_t [c](https://ffmpeg.org/doxygen/trunk/target__dec__fuzzer_8c.html#acfe27e61edea06412363421f1a013e81), enum [AVRounding](https://ffmpeg.org/doxygen/trunk/group__lavu__math.html#ga921d656eaf2c4d6800a734a13af021d0)[rnd](https://ffmpeg.org/doxygen/trunk/checkasm_8h.html#a4c65fed18b41fb82eea5bceefdeb5b15)) [av_const](https://ffmpeg.org/doxygen/trunk/attributes_8h.html#a9eaead3cde73464fd66bab4cef9307ad)|
|----|----|

使用指定的舍入重新缩放64位整数。

该操作在数学上等同于* b / c，但直接写入可能会溢出，并且不支持不同的舍入方法。






|int64_t|[av_rescale_q](https://ffmpeg.org/doxygen/trunk/group__lavu__math.html#gaf02994a8bbeaa91d4757df179cbe567f) (int64_t [a](https://ffmpeg.org/doxygen/trunk/h264pred__template_8c.html#a4124bc0a9335c27f086f24ba207a4912), [AVRational](https://ffmpeg.org/doxygen/trunk/structAVRational.html) bq, [AVRational](https://ffmpeg.org/doxygen/trunk/structAVRational.html) cq) [av_const](https://ffmpeg.org/doxygen/trunk/attributes_8h.html#a9eaead3cde73464fd66bab4cef9307ad)|
|----|----|

将64位整数重新缩放2个有理数。

该操作在数学上等同于* bq / cq。


此函数等效于AV_ROUND_NEAR_INF的av_rescale_q_rnd（）。






|int64_t|[av_rescale_q_rnd](https://ffmpeg.org/doxygen/trunk/group__lavu__math.html#gab706bfec9bf56534e02ca9564cb968f6) (int64_t [a](https://ffmpeg.org/doxygen/trunk/h264pred__template_8c.html#a4124bc0a9335c27f086f24ba207a4912), [AVRational](https://ffmpeg.org/doxygen/trunk/structAVRational.html) bq, [AVRational](https://ffmpeg.org/doxygen/trunk/structAVRational.html) cq, enum [AVRounding](https://ffmpeg.org/doxygen/trunk/group__lavu__math.html#ga921d656eaf2c4d6800a734a13af021d0)[rnd](https://ffmpeg.org/doxygen/trunk/checkasm_8h.html#a4c65fed18b41fb82eea5bceefdeb5b15)) [av_const](https://ffmpeg.org/doxygen/trunk/attributes_8h.html#a9eaead3cde73464fd66bab4cef9307ad)|
|----|----|

使用指定的舍入方式将64位整数重新缩放2个有理数。

该操作在数学上等同于* bq / cq。






|[int](https://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_compare_ts](https://ffmpeg.org/doxygen/trunk/group__lavu__math.html#ga151744358fff630942b926e67e67c415) (int64_t ts_a, [AVRational](https://ffmpeg.org/doxygen/trunk/structAVRational.html) tb_a, int64_t ts_b, [AVRational](https://ffmpeg.org/doxygen/trunk/structAVRational.html) tb_b)|
|----|----|

在自己的时间基础上比较两个时间戳。

返回：以下值之一：

   -1如果ts_a在ts_b之前

   1，如果ts_a在ts_b之后

    如果它们代表相同的位置，则为0





|int64_t|[av_compare_mod](https://ffmpeg.org/doxygen/trunk/group__lavu__math.html#gaf268a1dde957d04da846e026e2ebe6e8) (uint64_t [a](https://ffmpeg.org/doxygen/trunk/h264pred__template_8c.html#a4124bc0a9335c27f086f24ba207a4912), uint64_t [b](https://ffmpeg.org/doxygen/trunk/vf__curves_8c.html#a0a667482238bcca3ac201c1cd09c5072), uint64_t mod)|
|----|----|

比较两个整数操作数的余数除以公约数。

换句话说，比较整数a和b的最低有效log2（mod）位。




```cpp
av_compare_mod(0x11, 0x02, 0x10) < 0 // since 0x11 % 0x10  (0x1) < 0x02 % 0x10  (0x2)
av_compare_mod(0x11, 0x02, 0x20) > 0 // since 0x11 % 0x20 (0x11) > 0x02 % 0x20 (0x02)
```
参数：a，b：操作数

   mod：除数; 必须是2的倍数

返回：如果％mod <b％mod，则为负值

   一个正值，如果一个％mod> b％mod

   如果一个％mod == b％mod为零





|int64_t|[av_rescale_delta](https://ffmpeg.org/doxygen/trunk/group__lavu__math.html#ga29b7c3d60d68ef678ee1f4adc61a25dc) ([AVRational](https://ffmpeg.org/doxygen/trunk/structAVRational.html) in_tb, int64_t in_ts, [AVRational](https://ffmpeg.org/doxygen/trunk/structAVRational.html) fs_tb, [int](https://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[duration](https://ffmpeg.org/doxygen/trunk/tests_2movenc_8c.html#a1f342e4e3615abc9314684ce6b4819c0), int64_t *last, [AVRational](https://ffmpeg.org/doxygen/trunk/structAVRational.html) out_tb)|
|----|----|

重新缩放时间戳，同时保留已知的持续时间。

该功能被设计为每个音频包进行调用，以将输入时间戳缩放到不同的时基。 与简单的av_rescale_q（）调用相比，此功能对于可能的不一致的帧持续时间是稳健的。


最后一个参数是必须为同一个流的所有后续调用保留的状态变量。 对于第一个调用，* last应初始化为AV_NOPTS_VALUE。


参数：in_tb：输入时间时基

in_ts：输入时间戳

fs_tb：持续时间 。常这比int_tb和out_tb更精细（更大）

last：指向以fs_tb表示的充当状态变量的时间戳的指针

out_tb：输出时基

 
  duration：持续时间直到对该功能的下次呼叫（即当前分组/帧的持续时间）

返回：时间戳表示为out_tb

注意：在此功能的上下文中，“持续时间”是指样本，而不是秒。





|int64_t|[av_add_stable](https://ffmpeg.org/doxygen/trunk/group__lavu__math.html#ga0c4f9ed3f4102125be7451ad4d848a2f) ([AVRational](https://ffmpeg.org/doxygen/trunk/structAVRational.html) ts_tb, int64_t ts, [AVRational](https://ffmpeg.org/doxygen/trunk/structAVRational.html) inc_tb, int64_t inc)|
|----|----|

将值添加到时间戳。

此功能保证当重复添加相同的值时，不会发生舍入错误的累积。


参数：ts_tb：输入时间戳时基

   ts：输入时间戳

inc_tb：inc时间基准

   inc：要添加的值



