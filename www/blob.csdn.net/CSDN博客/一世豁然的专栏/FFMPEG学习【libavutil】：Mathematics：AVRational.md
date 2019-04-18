# FFMPEG学习【libavutil】：Mathematics：AVRational - 一世豁然的专栏 - CSDN博客





2017年07月13日 09:05:30[一世豁然](https://me.csdn.net/Explorer_day)阅读数：247标签：[ffmpeg](https://so.csdn.net/so/search/s.do?q=ffmpeg&t=blog)
个人分类：[ffmpeg](https://blog.csdn.net/Explorer_day/article/category/6289310)









Rationnal数的计算。




虽然rational数字可以表示为浮点数，但是转换过程是一个有损的，浮点操作也是如此。另一方面，FFmpeg的性质要求精确地计算时间戳。这组有理数实用程序充当了一个通用接口，用于操作有理数作为数字和分母的一对。





在AVRational的许多函数中都有后缀q，这是指数学符号“”(q)，它表示所有有理数的集合。





一、数据结构


|struct|[AVRational](http://ffmpeg.org/doxygen/trunk/structAVRational.html){intnum;intden;}|
|----|----|





二、函数


|static [AVRational](http://ffmpeg.org/doxygen/trunk/structAVRational.html)|[av_make_q](http://ffmpeg.org/doxygen/trunk/group__lavu__math__rational.html#ga7778b1d209ceb3e4bf34e40150d3b882) ([int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) num, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) den)|
|----|----|

创建AVRationnal。

对不支持复合文字的编译器有用。

注意：返回值并没有减少。





|static [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_cmp_q](http://ffmpeg.org/doxygen/trunk/group__lavu__math__rational.html#gaa440bf42ede128219a571ae4ce0f90b8) ([AVRational](http://ffmpeg.org/doxygen/trunk/structAVRational.html)[a](http://ffmpeg.org/doxygen/trunk/h264pred__template_8c.html#a4124bc0a9335c27f086f24ba207a4912), [AVRational](http://ffmpeg.org/doxygen/trunk/structAVRational.html)[b](http://ffmpeg.org/doxygen/trunk/vf__curves_8c.html#a0a667482238bcca3ac201c1cd09c5072))|
|----|----|

比较两个rational。

参数：a：第一个有理数

   b：第二个有理数

返回：下列值的其中一个

    0如果a==b

    1如果a>b

     -1如果a<b

     INT_MIN如果其中一个值事0/0




|static double|[av_q2d](http://ffmpeg.org/doxygen/trunk/group__lavu__math__rational.html#ga935dbbf6bde8dfe5fa7ddb1da582eb07) ([AVRational](http://ffmpeg.org/doxygen/trunk/structAVRational.html)[a](http://ffmpeg.org/doxygen/trunk/h264pred__template_8c.html#a4124bc0a9335c27f086f24ba207a4912))|
|----|----|

将AVRationnal转换为double。

参数：a：用来转换的AVRationnal。

返回：一个浮点形式。





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_reduce](http://ffmpeg.org/doxygen/trunk/group__lavu__math__rational.html#ga4fd9c0b8e663cf3d55e89fbc6ce71602) ([int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) *dst_num, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) *dst_den, int64_t num, int64_t den, int64_t max)|
|----|----|

减少一部分。

这对于framerate的计算很有用。

参数：dst_num：目的分子

dst_den：目的分母

   num：源分子

   den：源分母

   max：最大允许dst_num&dst_den的值

返回：如果操作正确，返回1，否则返回0、





|[AVRational](http://ffmpeg.org/doxygen/trunk/structAVRational.html)|[av_mul_q](http://ffmpeg.org/doxygen/trunk/group__lavu__math__rational.html#ga3f9c69432582e2857147bcba3c75dc32) ([AVRational](http://ffmpeg.org/doxygen/trunk/structAVRational.html)[b](http://ffmpeg.org/doxygen/trunk/vf__curves_8c.html#a0a667482238bcca3ac201c1cd09c5072), [AVRational](http://ffmpeg.org/doxygen/trunk/structAVRational.html)[c](http://ffmpeg.org/doxygen/trunk/target__dec__fuzzer_8c.html#acfe27e61edea06412363421f1a013e81)) [av_const](http://ffmpeg.org/doxygen/trunk/attributes_8h.html#a9eaead3cde73464fd66bab4cef9307ad)|
|----|----|

两个rationnal相乘。

参数：b：第一个有理数

   c：第二个有理数

返回：b*c





|[AVRational](http://ffmpeg.org/doxygen/trunk/structAVRational.html)|[av_div_q](http://ffmpeg.org/doxygen/trunk/group__lavu__math__rational.html#gaffa24e7bd38e12dbac540d8b66461f97) ([AVRational](http://ffmpeg.org/doxygen/trunk/structAVRational.html)[b](http://ffmpeg.org/doxygen/trunk/vf__curves_8c.html#a0a667482238bcca3ac201c1cd09c5072), [AVRational](http://ffmpeg.org/doxygen/trunk/structAVRational.html)[c](http://ffmpeg.org/doxygen/trunk/target__dec__fuzzer_8c.html#acfe27e61edea06412363421f1a013e81)) [av_const](http://ffmpeg.org/doxygen/trunk/attributes_8h.html#a9eaead3cde73464fd66bab4cef9307ad)|
|----|----|

使用一个rationnal除以另一个。

参数：b：第一个有理数

   c：第二个有理数

返回：b/c





|[AVRational](http://ffmpeg.org/doxygen/trunk/structAVRational.html)|[av_add_q](http://ffmpeg.org/doxygen/trunk/group__lavu__math__rational.html#ga2eb3a275aabacd8421f140a12bab4a91) ([AVRational](http://ffmpeg.org/doxygen/trunk/structAVRational.html)[b](http://ffmpeg.org/doxygen/trunk/vf__curves_8c.html#a0a667482238bcca3ac201c1cd09c5072), [AVRational](http://ffmpeg.org/doxygen/trunk/structAVRational.html)[c](http://ffmpeg.org/doxygen/trunk/target__dec__fuzzer_8c.html#acfe27e61edea06412363421f1a013e81)) [av_const](http://ffmpeg.org/doxygen/trunk/attributes_8h.html#a9eaead3cde73464fd66bab4cef9307ad)|
|----|----|

把两个rationnal相加。
参数：b：第一个有理数

  c：第二个有理数

返回：b+c



|[AVRational](http://ffmpeg.org/doxygen/trunk/structAVRational.html)|[av_add_q](http://ffmpeg.org/doxygen/trunk/group__lavu__math__rational.html#ga2eb3a275aabacd8421f140a12bab4a91) ([AVRational](http://ffmpeg.org/doxygen/trunk/structAVRational.html)[b](http://ffmpeg.org/doxygen/trunk/vf__curves_8c.html#a0a667482238bcca3ac201c1cd09c5072), [AVRational](http://ffmpeg.org/doxygen/trunk/structAVRational.html)[c](http://ffmpeg.org/doxygen/trunk/target__dec__fuzzer_8c.html#acfe27e61edea06412363421f1a013e81)) [av_const](http://ffmpeg.org/doxygen/trunk/attributes_8h.html#a9eaead3cde73464fd66bab4cef9307ad)|
|----|----|

使用rationnal减去另外一个。

参数：b：第一个有理数

   c：第二个有理数

返回：b-c



|static [av_always_inline](http://ffmpeg.org/doxygen/trunk/attributes_8h.html#a28f50f4c125959216885f01d56dbe866)[AVRational](http://ffmpeg.org/doxygen/trunk/structAVRational.html)|[av_inv_q](http://ffmpeg.org/doxygen/trunk/group__lavu__math__rational.html#gab646f0a7cc30af543ba4a91b37f9de6e) ([AVRational](http://ffmpeg.org/doxygen/trunk/structAVRational.html) q)|
|----|----|

反转一个rationnal。

参数：q：值

返回：1/q



|[AVRational](http://ffmpeg.org/doxygen/trunk/structAVRational.html)|[av_d2q](http://ffmpeg.org/doxygen/trunk/group__lavu__math__rational.html#ga7dfd5ba1eb1edf5845ac32b338de9e76) (double d, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) max) [av_const](http://ffmpeg.org/doxygen/trunk/attributes_8h.html#a9eaead3cde73464fd66bab4cef9307ad)|
|----|----|

将双精度浮点数转换为一个rational。

在无穷大的情况下，返回的值被表示为1、0或-1，这取决于符号。


参数：d：双转换

   max：最大允许的分子和分母

返回：d AVRationnal形式



|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_nearer_q](http://ffmpeg.org/doxygen/trunk/group__lavu__math__rational.html#ga111810c563749cd3081aaa19004f44df) ([AVRational](http://ffmpeg.org/doxygen/trunk/structAVRational.html) q, [AVRational](http://ffmpeg.org/doxygen/trunk/structAVRational.html)[q1](http://ffmpeg.org/doxygen/trunk/twofish_8c.html#a5413050687cd4a7cafafb705aeba1d03), [AVRational](http://ffmpeg.org/doxygen/trunk/structAVRational.html) q2)|
|----|----|

找出两个rationnal中那个更接近rantional。

参数：q：理性的加以比较值

   q1，q2：用来测试的有理数

返回：下列值得一个

    1如果Q1接近Q而不是Q2

    -1如果Q2比Q值接近Q值1

    0如果它们有相同的距离



|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_find_nearest_q_idx](http://ffmpeg.org/doxygen/trunk/group__lavu__math__rational.html#ga63d5ef1c2f77a5fdccb2b3a1c8cc77f5) ([AVRational](http://ffmpeg.org/doxygen/trunk/structAVRational.html) q, const [AVRational](http://ffmpeg.org/doxygen/trunk/structAVRational.html) *q_list)|
|----|----|

在一个给定的参考rational的最接近的rational的列表中找到它的值。

参数：q：参考有理数

   q_list：有理数的终止{0, 0}阵

返回：数组中最近值的索引



|uint32_t|[av_q2intfloat](http://ffmpeg.org/doxygen/trunk/group__lavu__math__rational.html#gafbcd96878344ade957d5bccc6bc705b2) ([AVRational](http://ffmpeg.org/doxygen/trunk/structAVRational.html) q)|
|----|----|

将AVRational转换为以固定点格式表示的IEEE 32位浮点数。

参数：q：用来转换的有理数

返回：等效浮点值，表示为无符号32位整数。

注意：返回值是平台独立的。



