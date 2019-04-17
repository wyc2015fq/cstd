# FFMPEG学习【libavutil】：Crypto and Hashing(五) - 一世豁然的专栏 - CSDN博客





2017年07月07日 08:17:23[一世豁然](https://me.csdn.net/Explorer_day)阅读数：231标签：[ffmpeg](https://so.csdn.net/so/search/s.do?q=ffmpeg&t=blog)
个人分类：[ffmpeg](https://blog.csdn.net/Explorer_day/article/category/6289310)









哈希功能在多媒体中很有用。





哈希函数广泛应用于多媒体，从错误检查和隐藏到内部回归测试。 libavutil具有可以对FFmpeg和其他多媒体应用程序有用的各种哈希函数的高效实现。





一、Adler-32

一）、函数


|unsigned long|[av_adler32_update](http://ffmpeg.org/doxygen/trunk/group__lavu__adler32.html#gafaf13d08fc7e1bfbf4c4b7f819c984ed) (unsigned long adler, const [uint8_t](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) *[buf](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#a5bc5fa69bee375df074734a2c4858604), unsigned [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[len](http://ffmpeg.org/doxygen/trunk/vorbis__enc__data_8h.html#afed088663f8704004425cdae2120b9b3)) [av_pure](http://ffmpeg.org/doxygen/trunk/attributes_8h.html#adc7fd5bb40d84900a3a30ef3318020c2)|
|----|----|

计算缓冲区的Adler32校验和。

参数：adler：初始校验和值

    buf：指向输入缓冲区的指针

    len：输入缓冲区的大小

返回：更新校验和







二、CRC

一）、类型定义


|typedef uint32_t|[AVCRC](http://ffmpeg.org/doxygen/trunk/group__lavu__crc32.html#gad7b1134679f68f2d4a52edaf1441e38f)|
|----|----|





二）、枚举


|enum|[AVCRCId](http://ffmpeg.org/doxygen/trunk/group__lavu__crc32.html#gab7717a9f1179a5ac8516982f9f2997d2) { [AV_CRC_8_ATM](http://ffmpeg.org/doxygen/trunk/group__lavu__crc32.html#ggab7717a9f1179a5ac8516982f9f2997d2a892e48b7e846baeefd1b55a955f5cc66), [AV_CRC_16_ANSI](http://ffmpeg.org/doxygen/trunk/group__lavu__crc32.html#ggab7717a9f1179a5ac8516982f9f2997d2ae71c422e08cb83214c438ec352799e75), [AV_CRC_16_CCITT](http://ffmpeg.org/doxygen/trunk/group__lavu__crc32.html#ggab7717a9f1179a5ac8516982f9f2997d2a0e915db19602a96b5a83a953fd169a95), [AV_CRC_32_IEEE](http://ffmpeg.org/doxygen/trunk/group__lavu__crc32.html#ggab7717a9f1179a5ac8516982f9f2997d2a2c95e44f51d497669d7c49d58018dea1), [AV_CRC_32_IEEE_LE](http://ffmpeg.org/doxygen/trunk/group__lavu__crc32.html#ggab7717a9f1179a5ac8516982f9f2997d2aa43f268d15303373c90287522a4d4752), [AV_CRC_16_ANSI_LE](http://ffmpeg.org/doxygen/trunk/group__lavu__crc32.html#ggab7717a9f1179a5ac8516982f9f2997d2aee02f172ec3ff10972855bdab591927a), [AV_CRC_24_IEEE](http://ffmpeg.org/doxygen/trunk/group__lavu__crc32.html#ggab7717a9f1179a5ac8516982f9f2997d2a317de029f3dce59427fe4681c5a8af2a) = 12, [AV_CRC_MAX](http://ffmpeg.org/doxygen/trunk/group__lavu__crc32.html#ggab7717a9f1179a5ac8516982f9f2997d2ac72372c57b632a072de910c789ad08ee)}|
|----|----|





三）、函数


|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_crc_init](http://ffmpeg.org/doxygen/trunk/group__lavu__crc32.html#ga27f906ac7e618afdac0be18de678ac3d) ([AVCRC](http://ffmpeg.org/doxygen/trunk/group__lavu__crc32.html#gad7b1134679f68f2d4a52edaf1441e38f) *[ctx](http://ffmpeg.org/doxygen/trunk/tests_2movenc_8c.html#a93cc7296f13f507c1fd7abd0db36b898), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[le](http://ffmpeg.org/doxygen/trunk/crc_8c.html#a15eef4ebd7b79c85b5f0433d809f6a32), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[bits](http://ffmpeg.org/doxygen/trunk/crc_8c.html#a46a6da6b1936191571fd30b2a749f38c), uint32_t [poly](http://ffmpeg.org/doxygen/trunk/crc_8c.html#a0e41a87672d56e3422f4bcbf411c2ff1), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) ctx_size)|
|----|----|

初始化CRC表。

参数：ctx：必须是大小sizeof（AVCRC）* 257或sizeof（AVCRC）* 1024的数组

    le：如果为1，则最低位表示相应多项式（对于多重和实际CRC两者）的最高指数的系数。 如果为0，则必须交换CRC参数和av_crc的结果，如果需要标准表示（在大多数情况下可简化为例如bswap16）：av_bswap32（crc <<（32位））

    bits：CRC的位数

    poly：在没有x **位系数的生成多项式中，在由le指定的表示中

ctx_size：ctx的大小（以字节为单位）

返回：<0表示失败。





|const [AVCRC](http://ffmpeg.org/doxygen/trunk/group__lavu__crc32.html#gad7b1134679f68f2d4a52edaf1441e38f) *|[av_crc_get_table](http://ffmpeg.org/doxygen/trunk/group__lavu__crc32.html#gabb35ea72fdeee2645e4e57cae225ac2b) ([AVCRCId](http://ffmpeg.org/doxygen/trunk/group__lavu__crc32.html#gab7717a9f1179a5ac8516982f9f2997d2) crc_id)|
|----|----|

获取一个初始化的标准CRC表。

参数：crc_id：标准CRC的ID

返回：指向CRC表的指针或失败时为NULL





|uint32_t|[av_crc](http://ffmpeg.org/doxygen/trunk/group__lavu__crc32.html#gae0feb73f66306494315f07de7ab57f80) (const [AVCRC](http://ffmpeg.org/doxygen/trunk/group__lavu__crc32.html#gad7b1134679f68f2d4a52edaf1441e38f) *[ctx](http://ffmpeg.org/doxygen/trunk/tests_2movenc_8c.html#a93cc7296f13f507c1fd7abd0db36b898), uint32_t crc, const [uint8_t](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) *[buffer](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a3667f558219c90437106b544a3ca00b8), size_t [length](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a4b0c8e796f240df86333df9dd6ff4361)) [av_pure](http://ffmpeg.org/doxygen/trunk/attributes_8h.html#adc7fd5bb40d84900a3a30ef3318020c2)|
|----|----|

计算块的CRC。

参数：crc：先前块的CRC（如果有）或CRC的初始值

返回：使用给定块的数据更新CRC







三、通用哈希API

一）、宏


|#define|[AV_HASH_MAX_SIZE](http://ffmpeg.org/doxygen/trunk/group__lavu__hash__generic.html#gaff5173f8837eb3afbd20b52528833d72)   64|
|----|----|

av_hash_get_size（）当前将返回的最大值。

如果您绝对想要或需要为输出缓冲区使用静态分配，则可以使用此选项，并且不支持新添加到libavutil中而不重新编译的哈希值。


警告：添加更大尺寸的新散列，并在增加宏时不会被认为是ABI的变化。 为了防止代码溢出缓冲区，可以使用av_hash_get_size（）动态分配输出缓冲区，也可以在编译期间将哈希API的使用限制在已经在FFmpeg中的哈希值。




二）、函数


|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_hash_alloc](http://ffmpeg.org/doxygen/trunk/group__lavu__hash__generic.html#gad1e018687f3a0e9baf3fb6d3af0dd10b) (struct [AVHashContext](http://ffmpeg.org/doxygen/trunk/structAVHashContext.html) **[ctx](http://ffmpeg.org/doxygen/trunk/tests_2movenc_8c.html#a93cc7296f13f507c1fd7abd0db36b898), const char *[name](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a09d548d751fcf0b9b67ea230712ae06c))|
|----|----|

为名称指定的算法分配哈希上下文。

返回：> = 0成功，否定错误代码失败

注意：在调用此函数后，上下文不被初始化; 你必须调用av_hash_init（）这样做。





|const char *|[av_hash_names](http://ffmpeg.org/doxygen/trunk/group__lavu__hash__generic.html#ga62537bbcf4cae67bbb33f3fa4f4329bf) ([int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) i)|
|----|----|

获取可用哈希算法的名称。

该函数可用于枚举算法


参数：i：散列算法的索引，从0开始

返回：指向静态字符串，如果i超出范围，则为NULL





|const char *|[av_hash_get_name](http://ffmpeg.org/doxygen/trunk/group__lavu__hash__generic.html#ga5952d21d5503ac307fbaa5d05a3d93ca) (const struct [AVHashContext](http://ffmpeg.org/doxygen/trunk/structAVHashContext.html) *[ctx](http://ffmpeg.org/doxygen/trunk/tests_2movenc_8c.html#a93cc7296f13f507c1fd7abd0db36b898))|
|----|----|

获取与给定哈希上下文对应的算法的名称。





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_hash_get_size](http://ffmpeg.org/doxygen/trunk/group__lavu__hash__generic.html#ga8bf8f94e3ebf73f838d017f83352d880) (const struct [AVHashContext](http://ffmpeg.org/doxygen/trunk/structAVHashContext.html) *[ctx](http://ffmpeg.org/doxygen/trunk/tests_2movenc_8c.html#a93cc7296f13f507c1fd7abd0db36b898))|
|----|----|

获取哈希值的大小（以字节为单位）。

此函数当前返回的最大值作为宏AV_HASH_MAX_SIZE可用。


参数：ctx：哈希上下文

返回：哈希值的大小（以字节为单位）





|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_hash_init](http://ffmpeg.org/doxygen/trunk/group__lavu__hash__generic.html#ga84ef872a731eb5c2637ddcd0f0976bca) (struct [AVHashContext](http://ffmpeg.org/doxygen/trunk/structAVHashContext.html) *[ctx](http://ffmpeg.org/doxygen/trunk/tests_2movenc_8c.html#a93cc7296f13f507c1fd7abd0db36b898))|
|----|----|

初始化或重置哈希上下文。

参数：ctx：哈希上下文





|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_hash_update](http://ffmpeg.org/doxygen/trunk/group__lavu__hash__generic.html#gac6a467ff60cb5080b56e818e7b8adb4a) (struct [AVHashContext](http://ffmpeg.org/doxygen/trunk/structAVHashContext.html) *[ctx](http://ffmpeg.org/doxygen/trunk/tests_2movenc_8c.html#a93cc7296f13f507c1fd7abd0db36b898), const [uint8_t](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) *[src](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#a67caf60f8ec76e0adc7b693bfb651b66), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[len](http://ffmpeg.org/doxygen/trunk/vorbis__enc__data_8h.html#afed088663f8704004425cdae2120b9b3))|
|----|----|

使用附加数据更新哈希上下文。

参数：ctx：哈希上下文

    src：要添加到哈希上下文的数据

    len：附加数据的大小





|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_hash_final](http://ffmpeg.org/doxygen/trunk/group__lavu__hash__generic.html#gaeff9a69f576f8f94e33a0b43a08baa70) (struct [AVHashContext](http://ffmpeg.org/doxygen/trunk/structAVHashContext.html) *[ctx](http://ffmpeg.org/doxygen/trunk/tests_2movenc_8c.html#a93cc7296f13f507c1fd7abd0db36b898), [uint8_t](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) *dst)|
|----|----|

完成哈希上下文并计算实际的哈希值。

dst缓冲区的最小大小由av_hash_get_size（）或AV_HASH_MAX_SIZE给出。


如果已经完成更新或最终确定哈希上下文是不安全的。


参数：ctx：哈希上下文

    dst：最终哈希值将被存储的位置





|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_hash_final_bin](http://ffmpeg.org/doxygen/trunk/group__lavu__hash__generic.html#ga65e1a59bf9c2076eda5884e89249af12) (struct [AVHashContext](http://ffmpeg.org/doxygen/trunk/structAVHashContext.html) *[ctx](http://ffmpeg.org/doxygen/trunk/tests_2movenc_8c.html#a93cc7296f13f507c1fd7abd0db36b898), [uint8_t](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) *dst, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[size](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a3d1e3edfcf61ca2d831883e1afbad89e))|
|----|----|

完成哈希上下文并将实际哈希值存储在缓冲区中。

如果已经完成更新或最终确定哈希上下文是不安全的。


如果大小小于哈希大小（由av_hash_get_size（）给出），那么哈希将被截断; 如果大小较大，则缓冲区用0填充。


参数：ctx：哈希上下文

    dst：最终哈希值将被存储的位置

    size：要写入dst的字节数





|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_hash_final_hex](http://ffmpeg.org/doxygen/trunk/group__lavu__hash__generic.html#ga8316ebcbe11caebc88b4c4afc0eef16c) (struct [AVHashContext](http://ffmpeg.org/doxygen/trunk/structAVHashContext.html) *[ctx](http://ffmpeg.org/doxygen/trunk/tests_2movenc_8c.html#a93cc7296f13f507c1fd7abd0db36b898), [uint8_t](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) *dst, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[size](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a3d1e3edfcf61ca2d831883e1afbad89e))|
|----|----|

完成散列上下文并将实际散列值的十六进制表示形式存储为字符串。

如果已经完成更新或最终确定哈希上下文是不安全的。


字符串始终为0终止。


如果大小小于2 * hash_size + 1，其中hash_size是av_hash_get_size（）返回的值，则字符串将被截断。


参数：ctx：哈希上下文


dst：字符串将被存储在哪里

    size：要写入dst的最大字节数





|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_hash_final_b64](http://ffmpeg.org/doxygen/trunk/group__lavu__hash__generic.html#ga2300a29e15fd439bcac2f9bc35b730ca) (struct [AVHashContext](http://ffmpeg.org/doxygen/trunk/structAVHashContext.html) *[ctx](http://ffmpeg.org/doxygen/trunk/tests_2movenc_8c.html#a93cc7296f13f507c1fd7abd0db36b898), [uint8_t](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) *dst, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[size](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a3d1e3edfcf61ca2d831883e1afbad89e))|
|----|----|

完成哈希上下文，并将实际哈希值的Base64表示形式存储为字符串。

如果已经完成更新或最终确定哈希上下文是不安全的。


字符串始终为0终止。


如果大小小于AV_BASE64_SIZE（hash_size），其中hash_size是av_hash_get_size（）返回的值，则字符串将被截断。




参数：ctx：哈希上下文


    dst：最终哈希值将被存储的位置

    size：要写入dst的最大字节数





|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_hash_freep](http://ffmpeg.org/doxygen/trunk/group__lavu__hash__generic.html#ga1e8bac529966e9af25f3608d3a1515ab) (struct [AVHashContext](http://ffmpeg.org/doxygen/trunk/structAVHashContext.html) **[ctx](http://ffmpeg.org/doxygen/trunk/tests_2movenc_8c.html#a93cc7296f13f507c1fd7abd0db36b898))|
|----|----|

自由哈希上下文，并将哈希上下文指针设置为NULL。

参数：ctx：指向哈希上下文的指针







四、MD5

一）、函数


|struct [AVMD5](http://ffmpeg.org/doxygen/trunk/structAVMD5.html) *|[av_md5_alloc](http://ffmpeg.org/doxygen/trunk/group__lavu__md5.html#ga5bf7cd7598f3be750458a53ed5c8ddde) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac))|
|----|----|

分配AVMD5上下文。





|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_md5_init](http://ffmpeg.org/doxygen/trunk/group__lavu__md5.html#ga31b412926441fa8f496a6033c6e918d0) (struct [AVMD5](http://ffmpeg.org/doxygen/trunk/structAVMD5.html) *[ctx](http://ffmpeg.org/doxygen/trunk/tests_2movenc_8c.html#a93cc7296f13f507c1fd7abd0db36b898))|
|----|----|

初始化MD5散列。

参数：ctx：指向函数上下文（大小为av_md5_size）的指针





|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_md5_update](http://ffmpeg.org/doxygen/trunk/group__lavu__md5.html#gaebb2a5a3aa979c9464b676f2d873a4c2) (struct [AVMD5](http://ffmpeg.org/doxygen/trunk/structAVMD5.html) *[ctx](http://ffmpeg.org/doxygen/trunk/tests_2movenc_8c.html#a93cc7296f13f507c1fd7abd0db36b898), const [uint8_t](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) *[src](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#a67caf60f8ec76e0adc7b693bfb651b66), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[len](http://ffmpeg.org/doxygen/trunk/vorbis__enc__data_8h.html#afed088663f8704004425cdae2120b9b3))|
|----|----|

更新哈希值。

参数：ctx：散列函数上下文

    src：输入数据来更新哈希

    len：输入数据的长度





|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_md5_final](http://ffmpeg.org/doxygen/trunk/group__lavu__md5.html#ga855dfc4c59bc1da98635390f8294237c) (struct [AVMD5](http://ffmpeg.org/doxygen/trunk/structAVMD5.html) *[ctx](http://ffmpeg.org/doxygen/trunk/tests_2movenc_8c.html#a93cc7296f13f507c1fd7abd0db36b898), [uint8_t](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) *dst)|
|----|----|

完成散列和输出摘要值。

参数：ctx：散列函数上下文

    dst：存储输出摘要值的缓冲区





|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_md5_sum](http://ffmpeg.org/doxygen/trunk/group__lavu__md5.html#gad28ee17bdad299b9ed9293eaf834ffff) ([uint8_t](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) *dst, const [uint8_t](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) *[src](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#a67caf60f8ec76e0adc7b693bfb651b66), const [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[len](http://ffmpeg.org/doxygen/trunk/vorbis__enc__data_8h.html#afed088663f8704004425cdae2120b9b3))|
|----|----|

哈希数组数组。

参数：dst：将消息写入的输出缓冲区

    src：数据要哈希

    len:数据的长度（以字节为单位）




二）、变量


|const [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_md5_size](http://ffmpeg.org/doxygen/trunk/group__lavu__md5.html#ga758362caf2146c2ef1e433415893d536)|
|----|----|





五、Murmur3

MurmurHash3哈希函数实现。





MurmurHash3是一种非加密散列函数，其发明人Austin Appleby创建了三个不兼容的版本：


1、32位输出

2、32位平台的128位输出

3、64位平台的128位输出




FFmpeg仅实现最后一个变体：为64位平台设计的128位输出。 即使哈希函数是为64位平台而设计的，但现实中的功能也在32位系统上工作，只有降低性能。





通过设计，MurmurHash3需要种子才能操作。 为此，libavutil提供了两个用于哈希启动的功能，一个需要一个种子（av_murmur3_init_seeded（）），一个使用固定的任意整数作为种子，因此不（av_murmur3_init（））。





为了使哈希值相当，您应该为所有调用此哈希函数提供相同的种子 - 如果您自己提供一个。





一）、函数


|struct [AVMurMur3](http://ffmpeg.org/doxygen/trunk/structAVMurMur3.html) *|[av_murmur3_alloc](http://ffmpeg.org/doxygen/trunk/group__lavu__murmur3.html#ga01dee6574ec23bc583bcb8624565adef) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac))|
|----|----|

分配AVMurMur3哈希上下文。

返回：未初始化的哈希上下文或在出现错误的情况下为NULL





|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_murmur3_init_seeded](http://ffmpeg.org/doxygen/trunk/group__lavu__murmur3.html#ga8870f1e1d537790bc224c04e955d357b) (struct [AVMurMur3](http://ffmpeg.org/doxygen/trunk/structAVMurMur3.html) *[c](http://ffmpeg.org/doxygen/trunk/target__dec__fuzzer_8c.html#acfe27e61edea06412363421f1a013e81), uint64_t [seed](http://ffmpeg.org/doxygen/trunk/videogen_8c.html#ae21f357c223957d36046a0d71cc6aed7))|
|----|----|

使用种子初始化或重新初始化AVMurMur3哈希上下文。

参数：c：哈希上下文

    seed：随机种子





|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_murmur3_init](http://ffmpeg.org/doxygen/trunk/group__lavu__murmur3.html#ga403fc40619831d043c444bf1df0b893d) (struct [AVMurMur3](http://ffmpeg.org/doxygen/trunk/structAVMurMur3.html) *[c](http://ffmpeg.org/doxygen/trunk/target__dec__fuzzer_8c.html#acfe27e61edea06412363421f1a013e81))|
|----|----|

初始化或重新初始化AVMurMur3哈希上下文。

相当于具有内置种子的av_murmur3_init_seeded（）。


参数：c：哈希上下文





|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_murmur3_update](http://ffmpeg.org/doxygen/trunk/group__lavu__murmur3.html#ga3428765cacead6bf0603b96eff68c197) (struct [AVMurMur3](http://ffmpeg.org/doxygen/trunk/structAVMurMur3.html) *[c](http://ffmpeg.org/doxygen/trunk/target__dec__fuzzer_8c.html#acfe27e61edea06412363421f1a013e81), const [uint8_t](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) *[src](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#a67caf60f8ec76e0adc7b693bfb651b66), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[len](http://ffmpeg.org/doxygen/trunk/vorbis__enc__data_8h.html#afed088663f8704004425cdae2120b9b3))|
|----|----|

使用新数据更新哈希上下文。

参数：c：哈希上下文

    src：输入数据来更新哈希

    len：从src读取的字节数





|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_murmur3_final](http://ffmpeg.org/doxygen/trunk/group__lavu__murmur3.html#gaf11c74aa0af4c0d6d422eb627b96a0c6) (struct [AVMurMur3](http://ffmpeg.org/doxygen/trunk/structAVMurMur3.html) *[c](http://ffmpeg.org/doxygen/trunk/target__dec__fuzzer_8c.html#acfe27e61edea06412363421f1a013e81), [uint8_t](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) dst[16])|
|----|----|

完成散列和输出摘要值。

参数：c：哈希上下文

    dst：存储输出摘要值的缓冲区







六、RIPEMD

一）、函数


|struct [AVRIPEMD](http://ffmpeg.org/doxygen/trunk/structAVRIPEMD.html) *|[av_ripemd_alloc](http://ffmpeg.org/doxygen/trunk/group__lavu__ripemd.html#gac5bd1677ed92c9330ed348e5657b39e1) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac))|
|----|----|

分配AVRIPEMD上下文。





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_ripemd_init](http://ffmpeg.org/doxygen/trunk/group__lavu__ripemd.html#ga4629925d978ee6975daecfad6921b799) (struct [AVRIPEMD](http://ffmpeg.org/doxygen/trunk/structAVRIPEMD.html) *context, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[bits](http://ffmpeg.org/doxygen/trunk/crc_8c.html#a46a6da6b1936191571fd30b2a749f38c))|
|----|----|

初始化RIPEMD散列。

参数：context：指向函数上下文（大小av_ripemd_size）的指针

    bits：摘要中的位数（128,160,256或320位）

返回：如果初始化成功，则为零，否则为-1





|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_ripemd_update](http://ffmpeg.org/doxygen/trunk/group__lavu__ripemd.html#ga7a87a9fe45d426f9ecfcfeb2eedf93ec) (struct [AVRIPEMD](http://ffmpeg.org/doxygen/trunk/structAVRIPEMD.html) *context, const [uint8_t](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) *[data](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a35090c370ccd7636420cba1acc908df1), unsigned [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[len](http://ffmpeg.org/doxygen/trunk/vorbis__enc__data_8h.html#afed088663f8704004425cdae2120b9b3))|
|----|----|

更新哈希值。

参数：context：散列函数上下文

    data：输入数据来更新哈希

    len：输入数据长度







|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_ripemd_final](http://ffmpeg.org/doxygen/trunk/group__lavu__ripemd.html#ga10a142f6a13c54e0b12e29eb6bcd75be) (struct [AVRIPEMD](http://ffmpeg.org/doxygen/trunk/structAVRIPEMD.html) *context, [uint8_t](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) *digest)|
|----|----|

完成散列和输出摘要值。

参数：context,：散列函数上下文

    digest：存储输出摘要值的缓冲区




二）、变量


|const [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_ripemd_size](http://ffmpeg.org/doxygen/trunk/group__lavu__ripemd.html#gad5e60759f7ece82caf50ea4e8623649c)|
|----|----|





七、SHA

SHA-1和SHA-256（安全散列算法）散列函数实现。





该模块支持以下SHA散列函数：


1、SHA-1：160位

2、SHA-224：224位，作为SHA-2的变体

3、SHA-256：256位，作为SHA-2的变体




一）、函数


|struct [AVSHA](http://ffmpeg.org/doxygen/trunk/structAVSHA.html) *|[av_sha_alloc](http://ffmpeg.org/doxygen/trunk/group__lavu__sha.html#ga58c5632adf76c34993e509b3464496c5) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac))|
|----|----|

分配AVSHA上下文。





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_sha_init](http://ffmpeg.org/doxygen/trunk/group__lavu__sha.html#gaf53a37af454b04d4974861e931593e1a) (struct [AVSHA](http://ffmpeg.org/doxygen/trunk/structAVSHA.html) *context, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[bits](http://ffmpeg.org/doxygen/trunk/crc_8c.html#a46a6da6b1936191571fd30b2a749f38c))|
|----|----|

初始化SHA-1或SHA-2哈希。

参数：context：指向函数上下文（大小为av_sha_size）的指针

    bits：摘要中的位数（SHA-1 - 160位，SHA-2 224或256位）

返回：如果初始化成功，则为零，否则为-1





|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_sha_update](http://ffmpeg.org/doxygen/trunk/group__lavu__sha.html#ga6f6ba69b07c33fdd30dada95e86fe0cb) (struct [AVSHA](http://ffmpeg.org/doxygen/trunk/structAVSHA.html) *[ctx](http://ffmpeg.org/doxygen/trunk/tests_2movenc_8c.html#a93cc7296f13f507c1fd7abd0db36b898), const [uint8_t](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) *[data](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a35090c370ccd7636420cba1acc908df1), unsigned [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[len](http://ffmpeg.org/doxygen/trunk/vorbis__enc__data_8h.html#afed088663f8704004425cdae2120b9b3))|
|----|----|

更新哈希值。

参数：ctx：散列函数上下文

    data：输入数据来更新哈希

    len：输入数据长度





|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_sha_final](http://ffmpeg.org/doxygen/trunk/group__lavu__sha.html#gadc0f2ed085f24f10cacfcdfcfc2e0127) (struct [AVSHA](http://ffmpeg.org/doxygen/trunk/structAVSHA.html) *context, [uint8_t](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) *digest)|
|----|----|

完成散列和输出摘要值。

参数：context：散列函数上下文

    digest：存储输出摘要值的缓冲区




二）、变量


|const [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_sha_size](http://ffmpeg.org/doxygen/trunk/group__lavu__sha.html#ga622be70e89326cc46029bf4f130be825)|
|----|----|





八、SHA-512

SHA-512（安全散列算法）散列函数实现。





该模块支持以下SHA-2散列函数：


1、SHA-512/224：224位

2、SHA-512/256：256位

3、SHA-384：384位

4、SHA-512：512位




一）、函数


|struct [AVSHA512](http://ffmpeg.org/doxygen/trunk/structAVSHA512.html) *|[av_sha512_alloc](http://ffmpeg.org/doxygen/trunk/group__lavu__sha512.html#ga40fcf794737ab4a08be5c4b4bfa95c0e) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac))|
|----|----|

分配AVSHA512上下文。





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_sha512_init](http://ffmpeg.org/doxygen/trunk/group__lavu__sha512.html#ga3cd6e2e5e591440905380dd345058516) (struct [AVSHA512](http://ffmpeg.org/doxygen/trunk/structAVSHA512.html) *context, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[bits](http://ffmpeg.org/doxygen/trunk/crc_8c.html#a46a6da6b1936191571fd30b2a749f38c))|
|----|----|

初始化SHA-2 512散列。

参数：context：指向函数上下文（大小为av_sha512_size）

    bits：摘要中的位数（224,256,384或512位）

返回：如果初始化成功，则为零，否则为-1





|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_sha512_update](http://ffmpeg.org/doxygen/trunk/group__lavu__sha512.html#ga101585440840ee0f17d562e024e8ac49) (struct [AVSHA512](http://ffmpeg.org/doxygen/trunk/structAVSHA512.html) *context, const [uint8_t](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) *[data](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a35090c370ccd7636420cba1acc908df1), unsigned [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[len](http://ffmpeg.org/doxygen/trunk/vorbis__enc__data_8h.html#afed088663f8704004425cdae2120b9b3))|
|----|----|

更新哈希值。

参数：context：散列函数上下文

    data：输入数据来更新哈希

    len：输入数据长度





|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_sha512_final](http://ffmpeg.org/doxygen/trunk/group__lavu__sha512.html#ga445bbd692a0e005d30a09d85f2ffbc69) (struct [AVSHA512](http://ffmpeg.org/doxygen/trunk/structAVSHA512.html) *context, [uint8_t](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) *digest)|
|----|----|

完成散列和输出摘要值。

参数：context：散列函数上下文

    digest：存储输出摘要值的缓冲区




二）变量


|const [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_sha512_size](http://ffmpeg.org/doxygen/trunk/group__lavu__sha512.html#gaa90af4540d2b03a1a122e1d6a8678efe)|
|----|----|




