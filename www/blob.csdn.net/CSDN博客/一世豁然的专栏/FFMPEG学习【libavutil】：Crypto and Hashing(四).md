# FFMPEG学习【libavutil】：Crypto and Hashing(四) - 一世豁然的专栏 - CSDN博客





2017年07月05日 09:45:54[一世豁然](https://me.csdn.net/Explorer_day)阅读数：256标签：[ffmpeg](https://so.csdn.net/so/search/s.do?q=ffmpeg&t=blog)
个人分类：[ffmpeg](https://blog.csdn.net/Explorer_day/article/category/6289310)









一、TWOFISH

一）、函数


|struct [AVTWOFISH](http://ffmpeg.org/doxygen/trunk/structAVTWOFISH.html) *|[av_twofish_alloc](http://ffmpeg.org/doxygen/trunk/group__lavu__twofish.html#gae57ffa35989216cc82eb47e68700e2ea) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac))|
|----|----|

分配AVTWOFISH上下文要释放结构体：av_free（ptr）





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_twofish_init](http://ffmpeg.org/doxygen/trunk/group__lavu__twofish.html#gaef4cdbc95118287ee4f170f7493dc948) (struct [AVTWOFISH](http://ffmpeg.org/doxygen/trunk/structAVTWOFISH.html) *[ctx](http://ffmpeg.org/doxygen/trunk/tests_2movenc_8c.html#a93cc7296f13f507c1fd7abd0db36b898), const [uint8_t](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) *key, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) key_bits)|
|----|----|

初始化AVTWOFISH上下文。

参数：ctx：AVTWOFISH上下文

    key：大小范围从1到32字节的密钥用于加密/解密

key_bits：密钥数：128,192,256如果小于所需的数，则用零填充到最近的有效值; 如果key_bits为128/192/256，返回值为0，否则为-1，否则为1





|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_twofish_crypt](http://ffmpeg.org/doxygen/trunk/group__lavu__twofish.html#ga2a9bae3c6491601acc809dc939691c38) (struct [AVTWOFISH](http://ffmpeg.org/doxygen/trunk/structAVTWOFISH.html) *[ctx](http://ffmpeg.org/doxygen/trunk/tests_2movenc_8c.html#a93cc7296f13f507c1fd7abd0db36b898), [uint8_t](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) *dst, const [uint8_t](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) *[src](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#a67caf60f8ec76e0adc7b693bfb651b66), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[count](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a619bc20e8198de3bd3f3d7fc34de66b2), [uint8_t](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) *iv, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) decrypt)|
|----|----|

使用先前初始化的上下文加密或解密缓冲区。

参数：ctx：AVTWOFISH上下文

    dst：目标数组，可以等于src

    src：源数组，可以等于dst

    count：16字节块的数量iv CBC模式的初始化向量，ECB模式为NULL


decrypt：0加密，1解密







二）、变量


|const [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_twofish_size](http://ffmpeg.org/doxygen/trunk/group__lavu__twofish.html#gaa3974c485c936db768316fb77e26b7c5)|
|----|----|





二、XTEA

一）、数据结构


|struct|[AVXTEA{uint32_t key [16]；}](http://ffmpeg.org/doxygen/trunk/structAVXTEA.html)|
|----|----|





二）、函数


|[AVXTEA](http://ffmpeg.org/doxygen/trunk/structAVXTEA.html) *|[av_xtea_alloc](http://ffmpeg.org/doxygen/trunk/group__lavu__xtea.html#gad53e078d8b3ce347ccc95590db416ace) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac))|
|----|----|

分配AVXTEA上下文。





|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_xtea_init](http://ffmpeg.org/doxygen/trunk/group__lavu__xtea.html#ga015fbc7f1062f11900dcf1c6352f2156) (struct [AVXTEA](http://ffmpeg.org/doxygen/trunk/structAVXTEA.html) *[ctx](http://ffmpeg.org/doxygen/trunk/tests_2movenc_8c.html#a93cc7296f13f507c1fd7abd0db36b898), const [uint8_t](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) key[16])|
|----|----|

初始化AVXTEA上下文。

参数：ctx：AVXTEA上下文

key[16]：用于加密/解密的16个字节的密钥，被解释为大字节32位数字





|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_xtea_le_init](http://ffmpeg.org/doxygen/trunk/group__lavu__xtea.html#ga95fcfa52ff3a9974ca6fdae6a0e6a618) (struct [AVXTEA](http://ffmpeg.org/doxygen/trunk/structAVXTEA.html) *[ctx](http://ffmpeg.org/doxygen/trunk/tests_2movenc_8c.html#a93cc7296f13f507c1fd7abd0db36b898), const [uint8_t](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) key[16])|
|----|----|

初始化AVXTEA上下文。



参数：ctx：AVXTEA上下文

key[16]：用于加密/解密的16个字节的密钥，被解释为小字节32位数字





|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_xtea_crypt](http://ffmpeg.org/doxygen/trunk/group__lavu__xtea.html#gaeaf213c9d74ba6d6b3769bda4ce283fc) (struct [AVXTEA](http://ffmpeg.org/doxygen/trunk/structAVXTEA.html) *[ctx](http://ffmpeg.org/doxygen/trunk/tests_2movenc_8c.html#a93cc7296f13f507c1fd7abd0db36b898), [uint8_t](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) *dst, const [uint8_t](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) *[src](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#a67caf60f8ec76e0adc7b693bfb651b66), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[count](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a619bc20e8198de3bd3f3d7fc34de66b2), [uint8_t](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) *iv, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) decrypt)|
|----|----|

使用先前初始化的上下文，以大尾数格式加密或解密缓冲区。

参数：ctx：AVXTEA上下文

    dst：目标数组，可以等于src

    src：源数组，可以等于dst

    count：8字节块数

    iv：CBC模式的初始化向量，如果NULL，则使用ECB

decrypt：0 for encryption, 1 for decryption





|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_xtea_le_crypt](http://ffmpeg.org/doxygen/trunk/group__lavu__xtea.html#gaec55b57021e2b6fd48f9e12a0a3a2724) (struct [AVXTEA](http://ffmpeg.org/doxygen/trunk/structAVXTEA.html) *[ctx](http://ffmpeg.org/doxygen/trunk/tests_2movenc_8c.html#a93cc7296f13f507c1fd7abd0db36b898), [uint8_t](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) *dst, const [uint8_t](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) *[src](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#a67caf60f8ec76e0adc7b693bfb651b66), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[count](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a619bc20e8198de3bd3f3d7fc34de66b2), [uint8_t](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) *iv, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) decrypt)|
|----|----|

使用先前初始化的上下文来加密或解密缓冲区，小端点格式。



参数：ctx：AVXTEA上下文

    dst：目标数组，可以等于src

    src：源数组，可以等于dst

    count：8字节块数

    iv：CBC模式的初始化向量，如果NULL，则使用ECB

decrypt：0 for encryption, 1 for decryption




