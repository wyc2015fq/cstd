# FFMPEG学习【libavutil】：Crypto and Hashing(一) - 一世豁然的专栏 - CSDN博客





2017年07月03日 11:06:47[一世豁然](https://me.csdn.net/Explorer_day)阅读数：389








一、AES

一）、函数


|struct [AVAES](http://ffmpeg.org/doxygen/trunk/structAVAES.html) *|[av_aes_alloc](http://ffmpeg.org/doxygen/trunk/group__lavu__aes.html#ga82fd4287891dc9fe98ecfe26e1a66bbb) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac))|
|----|----|

分配AVAES上下文。





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_aes_init](http://ffmpeg.org/doxygen/trunk/group__lavu__aes.html#ga54975ad71f0eab2287e3e94d561d0aa4) (struct [AVAES](http://ffmpeg.org/doxygen/trunk/structAVAES.html) *[a](http://ffmpeg.org/doxygen/trunk/h264pred__template_8c.html#a4124bc0a9335c27f086f24ba207a4912), const [uint8_t](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) *key, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) key_bits, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) decrypt)|
|----|----|

初始化AVAES上下文。

参数：key_bits：128，192或256

decrypt：0加密，1解密





|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_aes_crypt](http://ffmpeg.org/doxygen/trunk/group__lavu__aes.html#gae2572febc1bdf0b6dcb8b313da69e06f) (struct [AVAES](http://ffmpeg.org/doxygen/trunk/structAVAES.html) *[a](http://ffmpeg.org/doxygen/trunk/h264pred__template_8c.html#a4124bc0a9335c27f086f24ba207a4912), [uint8_t](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) *dst, const [uint8_t](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) *[src](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#a67caf60f8ec76e0adc7b693bfb651b66), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[count](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a619bc20e8198de3bd3f3d7fc34de66b2), [uint8_t](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) *iv, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) decrypt)|
|----|----|

使用先前初始化的上下文加密或解密缓冲区。

参数：count：16字节块数

     dst：目标数组，可以等于src

     src：源数组，可以等于dst

     iv：CBC模式的初始化向量，如果NULL，则使用ECB

decrypt::0加密，1解密




二）、变量


|const [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_aes_size](http://ffmpeg.org/doxygen/trunk/group__lavu__aes.html#ga33e4afb2c0eb4d7ead8a5b0a7fed4142)|
|----|----|





二、Base64

一）、宏


|#define|[AV_BASE64_DECODE_SIZE](http://ffmpeg.org/doxygen/trunk/group__lavu__base64.html#ga91e6771f201fdb7bfaeb4ccb95f1b252)(x)   ((x) * 3LL / 4)|
|----|----|

计算以长度x解码到数据缓冲区的base64字符串所需的字节输出大小。





|#define|[AV_BASE64_SIZE](http://ffmpeg.org/doxygen/trunk/group__lavu__base64.html#gaa79f39ca3f737b662ae6711d2cbecd20)(x)   (((x)+2) / 3 * 4 + 1)|
|----|----|

计算base64编码x字节所需的输出大小为空终止字符串。




二）、函数


|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_base64_decode](http://ffmpeg.org/doxygen/trunk/group__lavu__base64.html#gac8837266968329642080627f3aec9ab8) ([uint8_t](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) *[out](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae26fe5221615fb0a46f7688b1c268f69), const char *[in](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#a374bb0bba03cab664bd3187c3b5d09dd), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[out_size](http://ffmpeg.org/doxygen/trunk/tests_2movenc_8c.html#a6b2e93f2f2db9a65b19a2487c6b1f78b))|
|----|----|

解码base64编码的字符串。

参数：out：缓冲区用于解码数据

    in： 以空值终止的输入字符串

    out_size:输出缓冲区的大小（以字节为单位）必须至少为in的长度的3/4，即AV_BASE64_DECODE_SIZE（strlen（in））

返回：写入的字节数，或者在输入无效的情况下为负值





|char *|[av_base64_encode](http://ffmpeg.org/doxygen/trunk/group__lavu__base64.html#ga67f87e333f9ff84ec19374e0ad46de13) (char *[out](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae26fe5221615fb0a46f7688b1c268f69), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[out_size](http://ffmpeg.org/doxygen/trunk/tests_2movenc_8c.html#a6b2e93f2f2db9a65b19a2487c6b1f78b), const [uint8_t](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) *[in](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#a374bb0bba03cab664bd3187c3b5d09dd), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) in_size)|
|----|----|

将数据编码到base64并进行空终止。

参数：out：编码数据的缓冲区

    out_size:输出缓冲区（包括空终止符）的字节大小必须至少为AV_BASE64_SIZE（in_size）

    in:输入缓冲区包含要编码的数据

    in_size：在缓冲区中以字节为单位的大小

返回：出错或出现错误时为NULL







三、Blowfish

一）、结构体

struct  AVBlowfish{


|uint32_t|[p](http://ffmpeg.org/doxygen/trunk/structAVBlowfish.html#a8ceacf7fa493d6822c08aa465462606f) [[AV_BF_ROUNDS](http://ffmpeg.org/doxygen/trunk/group__lavu__blowfish.html#ga9ed9747b44cc2737a74a37f80e5cc269)+2]；|
|----|----|


|uint32_t|[s](http://ffmpeg.org/doxygen/trunk/structAVBlowfish.html#abf6f8773f7f25b320439df6a75cd909c) [4][256]|
|----|----|

}




二）、宏


|#define|[AV_BF_ROUNDS](http://ffmpeg.org/doxygen/trunk/group__lavu__blowfish.html#ga9ed9747b44cc2737a74a37f80e5cc269)   16|
|----|----|


三）、函数


|[AVBlowfish](http://ffmpeg.org/doxygen/trunk/structAVBlowfish.html) *|[av_blowfish_alloc](http://ffmpeg.org/doxygen/trunk/group__lavu__blowfish.html#gae049fd4df63f65ad4f8e196a16da07f1) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac))|
|----|----|

分配AVBlowfish上下文。





|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_blowfish_init](http://ffmpeg.org/doxygen/trunk/group__lavu__blowfish.html#gabdeab79ba02586b6c06240eeb3472837) (struct [AVBlowfish](http://ffmpeg.org/doxygen/trunk/structAVBlowfish.html) *[ctx](http://ffmpeg.org/doxygen/trunk/tests_2movenc_8c.html#a93cc7296f13f507c1fd7abd0db36b898), const [uint8_t](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) *key, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) key_len)|
|----|----|

初始化AVBlowfish上下文。

参数：ctx：一个AVBlowfish上下文

    key：密匙

key_len：密匙长度








|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_blowfish_crypt_ecb](http://ffmpeg.org/doxygen/trunk/group__lavu__blowfish.html#ga7b57de27a9aa26b67ac1f0d67fea781b) (struct [AVBlowfish](http://ffmpeg.org/doxygen/trunk/structAVBlowfish.html) *[ctx](http://ffmpeg.org/doxygen/trunk/tests_2movenc_8c.html#a93cc7296f13f507c1fd7abd0db36b898), uint32_t *xl, uint32_t *xr, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) decrypt)|
|----|----|

使用先前初始化的上下文加密或解密缓冲区。

参数：ctx：一个AVBlowfish上下文

    xl：剩下四个字节一半的输入要加密

    xr：右侧四位半字节的输入要加密

decrypt：0加密，1解密





|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_blowfish_crypt](http://ffmpeg.org/doxygen/trunk/group__lavu__blowfish.html#gaad9e1113615df7f3e883ad48e7ee764b) (struct [AVBlowfish](http://ffmpeg.org/doxygen/trunk/structAVBlowfish.html) *[ctx](http://ffmpeg.org/doxygen/trunk/tests_2movenc_8c.html#a93cc7296f13f507c1fd7abd0db36b898), [uint8_t](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) *dst, const [uint8_t](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) *[src](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#a67caf60f8ec76e0adc7b693bfb651b66), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[count](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a619bc20e8198de3bd3f3d7fc34de66b2), [uint8_t](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) *iv, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) decrypt)|
|----|----|

使用先前初始化的上下文加密或解密缓冲区。

参数：ctx：一个AVBlowfish上下文

    dst：目标数组，可以等于src

    src：源数组，可以等于dst

    count：8字节块数

    iv：CBC模式的初始化向量，如果使用NULL ECB

decrypt：0加密，1解密







四、CAMELLIA

一）、函数


|struct [AVCAMELLIA](http://ffmpeg.org/doxygen/trunk/structAVCAMELLIA.html) *|[av_camellia_alloc](http://ffmpeg.org/doxygen/trunk/group__lavu__camellia.html#ga5a91307222e9671fb838743364391fea) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac))|
|----|----|

分配AVCAMELLIA上下文要释放结构体：av_free（ptr）





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_camellia_init](http://ffmpeg.org/doxygen/trunk/group__lavu__camellia.html#gaf7f182d8ab55c60e69ac2cf0413a8dbe) (struct [AVCAMELLIA](http://ffmpeg.org/doxygen/trunk/structAVCAMELLIA.html) *[ctx](http://ffmpeg.org/doxygen/trunk/tests_2movenc_8c.html#a93cc7296f13f507c1fd7abd0db36b898), const [uint8_t](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) *key, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) key_bits)|
|----|----|

初始化AVCAMELLIA上下文。

参数：ctx：AVCAMELLIA上下文

    key：用于加密/解密的16,24,32字节的密钥

key_bits：密钥数：可能是128,192,256





|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_camellia_crypt](http://ffmpeg.org/doxygen/trunk/group__lavu__camellia.html#ga2928427ed68f873d17ed59e21bb28911) (struct [AVCAMELLIA](http://ffmpeg.org/doxygen/trunk/structAVCAMELLIA.html) *[ctx](http://ffmpeg.org/doxygen/trunk/tests_2movenc_8c.html#a93cc7296f13f507c1fd7abd0db36b898), [uint8_t](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) *dst, const [uint8_t](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) *[src](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#a67caf60f8ec76e0adc7b693bfb651b66), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[count](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a619bc20e8198de3bd3f3d7fc34de66b2), [uint8_t](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) *iv, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) decrypt)|
|----|----|

使用先前初始化的上下文加密或解密缓冲区。

参数：ctx：AVCAMELLIA上下文

    dst：目标数组，可以等于src

    src：源数组，可以等于dst

    count：16字节块的数量iv CBC模式的初始化向量，ECB模式为NULL

decrypt：0加密，1解密




二）、变量


|const [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_camellia_size](http://ffmpeg.org/doxygen/trunk/group__lavu__camellia.html#ga148900bf024608d457ddd7fbb4f7ebc2)|
|----|----|




