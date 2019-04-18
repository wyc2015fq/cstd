# FFMPEG学习【libavutil】：Crypto and Hashing(二) - 一世豁然的专栏 - CSDN博客





2017年07月04日 10:18:44[一世豁然](https://me.csdn.net/Explorer_day)阅读数：179








一、CAST5、

一）、函数


|struct [AVCAST5](http://ffmpeg.org/doxygen/trunk/structAVCAST5.html) *|[av_cast5_alloc](http://ffmpeg.org/doxygen/trunk/group__lavu__cast5.html#gafacaf78689bf9500adc0e15c9a273f30) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac))|
|----|----|

分配AVCAST5上下文要释放结构体：av_free（ptr）






|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_cast5_init](http://ffmpeg.org/doxygen/trunk/group__lavu__cast5.html#ga604b7f58779a1c41c9c6c5a1016ecc72) (struct [AVCAST5](http://ffmpeg.org/doxygen/trunk/structAVCAST5.html) *[ctx](http://ffmpeg.org/doxygen/trunk/tests_2movenc_8c.html#a93cc7296f13f507c1fd7abd0db36b898), const [uint8_t](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) *key, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) key_bits)|
|----|----|

初始化AVCAST5上下文。

参数：ctx：AVCAST5上下文

    key：一个5,6，... 16字节的密钥用于加密/解密

key_bits：密钥数：可能是40,48，...，128

返回：0成功，故障时小于0





|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_cast5_crypt](http://ffmpeg.org/doxygen/trunk/group__lavu__cast5.html#ga2097b4a332bd12e51674788a3008a53e) (struct [AVCAST5](http://ffmpeg.org/doxygen/trunk/structAVCAST5.html) *[ctx](http://ffmpeg.org/doxygen/trunk/tests_2movenc_8c.html#a93cc7296f13f507c1fd7abd0db36b898), [uint8_t](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) *dst, const [uint8_t](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) *[src](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#a67caf60f8ec76e0adc7b693bfb651b66), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[count](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a619bc20e8198de3bd3f3d7fc34de66b2), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) decrypt)|
|----|----|

使用以前初始化的上下文，仅ECB模式加密或解密缓冲区。

参数：ctx：AVCAST5上下文

    dst：目标数组，可以等于src

    src：源数组，可以等于dst

    count：8字节块数

decrypt：0加密，1解密





|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_cast5_crypt2](http://ffmpeg.org/doxygen/trunk/group__lavu__cast5.html#gad7f20dd3446e711c8fcc8c2e57c2c7d8) (struct [AVCAST5](http://ffmpeg.org/doxygen/trunk/structAVCAST5.html) *[ctx](http://ffmpeg.org/doxygen/trunk/tests_2movenc_8c.html#a93cc7296f13f507c1fd7abd0db36b898), [uint8_t](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) *dst, const [uint8_t](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) *[src](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#a67caf60f8ec76e0adc7b693bfb651b66), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[count](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a619bc20e8198de3bd3f3d7fc34de66b2), [uint8_t](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) *iv, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) decrypt)|
|----|----|

使用先前初始化的上下文加密或解密缓冲区。



参数：ctx：AVCAST5上下文

    dst：目标数组，可以等于src

    src：源数组，可以等于dst

    count：8字节块数

    iv：CBC模式的初始化向量，ECB模式为NULL

decrypt：0加密，1解密








二）、变量


|const [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_cast5_size](http://ffmpeg.org/doxygen/trunk/group__lavu__cast5.html#ga00f0cc05aeee06257fb27eb6247d8092)|
|----|----|








二、DES

一）、数据结构


|struct|[AVDES](http://ffmpeg.org/doxygen/trunk/structAVDES.html){uint64_t round_keys [3] [16]；int triple_des；}|
|----|----|





二）、函数


|[AVDES](http://ffmpeg.org/doxygen/trunk/structAVDES.html) *|[av_des_alloc](http://ffmpeg.org/doxygen/trunk/group__lavu__des.html#gabe865f33a24b647fa5b7852c98387d4c) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac))|
|----|----|

分配AVDES上下文。





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_des_init](http://ffmpeg.org/doxygen/trunk/group__lavu__des.html#gae8ebc22ff06f4624f4ff9a60b974eb27) (struct [AVDES](http://ffmpeg.org/doxygen/trunk/structAVDES.html) *d, const [uint8_t](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) *key, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) key_bits, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) decrypt)|
|----|----|

初始化AVDES上下文。

参数：key_bits：必须是64或192

decrypt：0加密/ CBC-MAC，1解密

返回：成功为零，否则为负值





|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_des_crypt](http://ffmpeg.org/doxygen/trunk/group__lavu__des.html#gaf728cddab7963246ec519c8c123b5412) (struct [AVDES](http://ffmpeg.org/doxygen/trunk/structAVDES.html) *d, [uint8_t](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) *dst, const [uint8_t](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) *[src](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#a67caf60f8ec76e0adc7b693bfb651b66), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[count](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a619bc20e8198de3bd3f3d7fc34de66b2), [uint8_t](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) *iv, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) decrypt)|
|----|----|

使用DES算法加密/解密。

参数：count：8字节块数

    dst：目标数组，可以等于src，必须是8字节对齐

    src：源数组，可以等于dst，必须是8字节对齐，可能是NULL

    iv：用于CBC模式的初始化向量，如果为NULL则ECB将被使用，必须是8字节对齐

decrypt： 0加密，1解密








|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_des_mac](http://ffmpeg.org/doxygen/trunk/group__lavu__des.html#gada639d868e744c18622f4e89b1e5833e) (struct [AVDES](http://ffmpeg.org/doxygen/trunk/structAVDES.html) *d, [uint8_t](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) *dst, const [uint8_t](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) *[src](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#a67caf60f8ec76e0adc7b693bfb651b66), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[count](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a619bc20e8198de3bd3f3d7fc34de66b2))|
|----|----|

使用DES算法计算CBC-MAC。

参数：dst：目标数组，可以等于src，必须是8字节对齐

    count：8字节块数

    src：源数组，可以等于dst，必须是8字节对齐，可能是NULL







三、HMAC

一）、宏


|enum|[AVHMACType](http://ffmpeg.org/doxygen/trunk/group__lavu__hmac.html#gaec34f18155d20236c9f39961bb58f7d4) { [AV_HMAC_MD5](http://ffmpeg.org/doxygen/trunk/group__lavu__hmac.html#ggaec34f18155d20236c9f39961bb58f7d4a063ecd7e758d4abcd438aba0b9708c7a), [AV_HMAC_SHA1](http://ffmpeg.org/doxygen/trunk/group__lavu__hmac.html#ggaec34f18155d20236c9f39961bb58f7d4a421f7f38b635e8d5ef1cc45d1867d5a7), [AV_HMAC_SHA224](http://ffmpeg.org/doxygen/trunk/group__lavu__hmac.html#ggaec34f18155d20236c9f39961bb58f7d4a283c3d421f8279ec3530d0bd0cb56313), [AV_HMAC_SHA256](http://ffmpeg.org/doxygen/trunk/group__lavu__hmac.html#ggaec34f18155d20236c9f39961bb58f7d4a2f0a1a712c6e85b5149baa0780dcda4c), [AV_HMAC_SHA384](http://ffmpeg.org/doxygen/trunk/group__lavu__hmac.html#ggaec34f18155d20236c9f39961bb58f7d4a268dfcdccbe89e48a662989087728ff7) = 12, [AV_HMAC_SHA512](http://ffmpeg.org/doxygen/trunk/group__lavu__hmac.html#ggaec34f18155d20236c9f39961bb58f7d4a811a3754bdc32d9e258c6504759a6608)}|
|----|----|


二）、函数


|[AVHMAC](http://ffmpeg.org/doxygen/trunk/structAVHMAC.html) *|[av_hmac_alloc](http://ffmpeg.org/doxygen/trunk/group__lavu__hmac.html#gaae6685287d6645eb1be86525ecee17ae) (enum [AVHMACType](http://ffmpeg.org/doxygen/trunk/group__lavu__hmac.html#gaec34f18155d20236c9f39961bb58f7d4)[type](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a7d05960f4f1c1b11f3177dc963a45d86))|
|----|----|

分配AVHMAC上下文。

参数：type：用于HMAC的哈希函数。





|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_hmac_free](http://ffmpeg.org/doxygen/trunk/group__lavu__hmac.html#gaaa2951b832fdc7abed35a9112f7f1a02) ([AVHMAC](http://ffmpeg.org/doxygen/trunk/structAVHMAC.html) *[ctx](http://ffmpeg.org/doxygen/trunk/tests_2movenc_8c.html#a93cc7296f13f507c1fd7abd0db36b898))|
|----|----|

释放AVHMAC上下文。

参数：ctx：用来释放的上下文，可能为NULL





|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_hmac_init](http://ffmpeg.org/doxygen/trunk/group__lavu__hmac.html#gae91a0c1f832c269006164b29df7e9aa8) ([AVHMAC](http://ffmpeg.org/doxygen/trunk/structAVHMAC.html) *[ctx](http://ffmpeg.org/doxygen/trunk/tests_2movenc_8c.html#a93cc7296f13f507c1fd7abd0db36b898), const [uint8_t](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) *key, unsigned [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) keylen)|
|----|----|

使用身份验证密钥初始化AVHMAC上下文。

参数：ctx：HMAC上下文

    key：验证密钥

    keylen：密钥的长度，以字节为单位





|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_hmac_update](http://ffmpeg.org/doxygen/trunk/group__lavu__hmac.html#ga4c3307cc9b1f331a15e999f8dacb90de) ([AVHMAC](http://ffmpeg.org/doxygen/trunk/structAVHMAC.html) *[ctx](http://ffmpeg.org/doxygen/trunk/tests_2movenc_8c.html#a93cc7296f13f507c1fd7abd0db36b898), const [uint8_t](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) *[data](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a35090c370ccd7636420cba1acc908df1), unsigned [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[len](http://ffmpeg.org/doxygen/trunk/vorbis__enc__data_8h.html#afed088663f8704004425cdae2120b9b3))|
|----|----|

与HMAC的哈希数据。

参数：ctx：HMAC上下文

    data：哈希的数据

    len：数据的长度（以字节为单位）





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_hmac_final](http://ffmpeg.org/doxygen/trunk/group__lavu__hmac.html#ga02f0b343d9045500beef1f6b09c650cd) ([AVHMAC](http://ffmpeg.org/doxygen/trunk/structAVHMAC.html) *[ctx](http://ffmpeg.org/doxygen/trunk/tests_2movenc_8c.html#a93cc7296f13f507c1fd7abd0db36b898), [uint8_t](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) *[out](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae26fe5221615fb0a46f7688b1c268f69), unsigned [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) outlen)|
|----|----|

完成哈希和输出HMAC摘要。

参数：ctx：HMAC上下文

    out：将消息写入的输出缓冲区

    outlen：out缓冲区的长度，以字节为单位

返回：写出的字节数或负错误代码。





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_hmac_calc](http://ffmpeg.org/doxygen/trunk/group__lavu__hmac.html#ga22d17ebca8556c49be5c22e706de1ab3) ([AVHMAC](http://ffmpeg.org/doxygen/trunk/structAVHMAC.html) *[ctx](http://ffmpeg.org/doxygen/trunk/tests_2movenc_8c.html#a93cc7296f13f507c1fd7abd0db36b898), const [uint8_t](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) *[data](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a35090c370ccd7636420cba1acc908df1), unsigned [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[len](http://ffmpeg.org/doxygen/trunk/vorbis__enc__data_8h.html#afed088663f8704004425cdae2120b9b3), const [uint8_t](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) *key, unsigned [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) keylen, [uint8_t](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) *[out](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae26fe5221615fb0a46f7688b1c268f69), unsigned [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) outlen)|
|----|----|

用一个键来排列一组数据。

参数：ctx：HMAC上下文


    data：哈希的数据

    len：数据的长度（以字节为单位）

    key：验证密钥

    keylen：密钥的长度，以字节为单位

    out：将消息写入的输出缓冲区

    outlen：out缓冲区的长度，以字节为单位

返回：写出的字节数或负错误代码。



