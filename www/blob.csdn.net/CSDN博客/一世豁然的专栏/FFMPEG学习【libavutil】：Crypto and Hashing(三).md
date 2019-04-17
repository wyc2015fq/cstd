# FFMPEG学习【libavutil】：Crypto and Hashing(三) - 一世豁然的专栏 - CSDN博客





2017年07月04日 10:50:43[一世豁然](https://me.csdn.net/Explorer_day)阅读数：371








一、LZO

一)、宏


|#define|[AV_LZO_INPUT_PADDING](http://ffmpeg.org/doxygen/trunk/group__lavu__lzo.html#ga396ab17a8ae4cd3cef02747796f5f8f7)   8|
|----|----|



|#define|[AV_LZO_OUTPUT_PADDING](http://ffmpeg.org/doxygen/trunk/group__lavu__lzo.html#gaece7adef189377f4ab7da464247a9e85)   12|
|----|----|




二）、函数


|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_lzo1x_decode](http://ffmpeg.org/doxygen/trunk/group__lavu__lzo.html#gaafb2f88aa03d1fe059f3c2e174df6142) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *[out](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae26fe5221615fb0a46f7688b1c268f69), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) *outlen, const [void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *[in](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#a374bb0bba03cab664bd3187c3b5d09dd), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) *inlen)|
|----|----|

解码LZO 1x压缩数据。

参数：out：输出缓冲

    outlen：输出缓冲区的大小，剩余的字节数在这里返回

    in：输入缓冲

    inlen：输入缓冲区的大小，此处返回的字节数

返回：0成功，否则上面的错误标志的组合

确保所有缓冲区都被适当填充，必须提供AV_LZO_INPUT_PADDING，必须提供AV_LZO_OUTPUT_PADDING附加字节。





三）、av_lzo1x_decode返回的错误标志


|#define|[AV_LZO_INPUT_DEPLETED](http://ffmpeg.org/doxygen/trunk/group__lavu__lzo.html#ga5f66b9a81147a79b747deb6ccacb3ad5)   1|
|----|----|

解码完成之前达到的输入缓冲区结束





|#define|[AV_LZO_OUTPUT_FULL](http://ffmpeg.org/doxygen/trunk/group__lavu__lzo.html#ga400c6aec6aeca19117fb9ff6f6d30325)   2|
|----|----|

解码数据不适合输出缓冲区





|#define|[AV_LZO_INVALID_BACKPTR](http://ffmpeg.org/doxygen/trunk/group__lavu__lzo.html#ga6acd84bec8181fecb2861f8c7eb57cc1)   4|
|----|----|

对先前解码的数据的引用是错误的





|#define|[AV_LZO_ERROR](http://ffmpeg.org/doxygen/trunk/group__lavu__lzo.html#gad8076b8656b50ac499d0f18bf887ed13)   8|
|----|----|

压缩比特流中的非特定错误







二、RC4

一）、数据结构


|struct|[AVRC4](http://ffmpeg.org/doxygen/trunk/structAVRC4.html){uint8_t state [256]；int  x；int  y；}|
|----|----|


二）、函数


|[AVRC4](http://ffmpeg.org/doxygen/trunk/structAVRC4.html) *|[av_rc4_alloc](http://ffmpeg.org/doxygen/trunk/group__lavu__rc4.html#gac6bb9ef90cc6c55250c3ad0df1324ce6) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac))|
|----|----|

分配AVRC4上下文。





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_rc4_init](http://ffmpeg.org/doxygen/trunk/group__lavu__rc4.html#ga2afd06d2962795cc240cde5c18c0d7f9) (struct [AVRC4](http://ffmpeg.org/doxygen/trunk/structAVRC4.html) *d, const [uint8_t](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) *key, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) key_bits, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) decrypt)|
|----|----|

初始化AVRC4上下文。

参数：key_bits：必须是8的倍数

decrypt：0加密，1解密，目前没有任何效果

返回：成功为零，否则为负值





|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_rc4_crypt](http://ffmpeg.org/doxygen/trunk/group__lavu__rc4.html#gadb5dd2b518df0e71479b52310e16c5ad) (struct [AVRC4](http://ffmpeg.org/doxygen/trunk/structAVRC4.html) *d, [uint8_t](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) *dst, const [uint8_t](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) *[src](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#a67caf60f8ec76e0adc7b693bfb651b66), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[count](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a619bc20e8198de3bd3f3d7fc34de66b2), [uint8_t](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) *iv, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) decrypt)|
|----|----|

使用RC4算法进行加密/解密。

参数：count：字节数

    dst：目标数组，可以等于src

    src：源数组，可以等于dst，可能为NULL

    iv：不用（还）用于RC4，应为NULL

decrypt：0用于加密，1用于解密，不（尚）使用







三、TEA

一）、函数


|struct [AVTEA](http://ffmpeg.org/doxygen/trunk/structAVTEA.html) *|[av_tea_alloc](http://ffmpeg.org/doxygen/trunk/group__lavu__tea.html#gaaf22dd3ac8632b0e15e19cc123a82392) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac))|
|----|----|

分配AVTEA上下文要释放结构体：av_free（ptr）





|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_tea_init](http://ffmpeg.org/doxygen/trunk/group__lavu__tea.html#ga8fe0c9dabc9db1b88480f58e0d598c44) (struct [AVTEA](http://ffmpeg.org/doxygen/trunk/structAVTEA.html) *[ctx](http://ffmpeg.org/doxygen/trunk/tests_2movenc_8c.html#a93cc7296f13f507c1fd7abd0db36b898), const [uint8_t](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) key[16], [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) rounds)|
|----|----|

初始化AVTEA上下文。

参数：ctx：AVTEA上下文

    key：用于加密/解密的16字节的密钥

rounds：TEA的回合次数（64个是“标准”）





|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_tea_crypt](http://ffmpeg.org/doxygen/trunk/group__lavu__tea.html#gaf6a071e471a2a7aabae0ec62dc3ec717) (struct [AVTEA](http://ffmpeg.org/doxygen/trunk/structAVTEA.html) *[ctx](http://ffmpeg.org/doxygen/trunk/tests_2movenc_8c.html#a93cc7296f13f507c1fd7abd0db36b898), [uint8_t](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) *dst, const [uint8_t](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) *[src](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#a67caf60f8ec76e0adc7b693bfb651b66), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[count](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a619bc20e8198de3bd3f3d7fc34de66b2), [uint8_t](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) *iv, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) decrypt)|
|----|----|

使用先前初始化的上下文加密或解密缓冲区。

参数：ctx：AVTEA上下文

    dst：目标数组，可以等于src

    src：源数组，可以等于dst

    count：8字节块数

    iv：CBC模式的初始化向量，如果NULL，则使用ECB

    decrypt：0加密，1解密






二）、变量


|const [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_tea_size](http://ffmpeg.org/doxygen/trunk/group__lavu__tea.html#ga7c9222593c131d944e546df088753f8b)|
|----|----|




