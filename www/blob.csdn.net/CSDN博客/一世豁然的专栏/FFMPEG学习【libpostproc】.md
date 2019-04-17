# FFMPEG学习【libpostproc】 - 一世豁然的专栏 - CSDN博客





2017年07月30日 10:32:38[一世豁然](https://me.csdn.net/Explorer_day)阅读数：1422








视频后处理库。





一、宏


|#define|[PP_QUALITY_MAX](http://ffmpeg.org/doxygen/trunk/group__lpp.html#gaa73885b5143a0d9bbaa0aa84b101a844)   6|
|----|----|
|| |
|#define|[PP_CPU_CAPS_MMX](http://ffmpeg.org/doxygen/trunk/group__lpp.html#ga58d0adec5c30bc7af74ef51e6a054655)   0x80000000|
|| |
|#define|[PP_CPU_CAPS_MMX2](http://ffmpeg.org/doxygen/trunk/group__lpp.html#ga58f7363e4251a37e4ddab21d95004d79)   0x20000000|
|| |
|#define|[PP_CPU_CAPS_3DNOW](http://ffmpeg.org/doxygen/trunk/group__lpp.html#ga0d7ea04970a084254fb9be4072c89bac)   0x40000000|
|| |
|#define|[PP_CPU_CAPS_ALTIVEC](http://ffmpeg.org/doxygen/trunk/group__lpp.html#ga3ca9c37132bf613044a981f5f0b49430)   0x10000000|
|| |
|#define|[PP_CPU_CAPS_AUTO](http://ffmpeg.org/doxygen/trunk/group__lpp.html#ga75dbe51b6414ad8e19eb2109985998ff)   0x00080000|
|| |
|#define|[PP_FORMAT](http://ffmpeg.org/doxygen/trunk/group__lpp.html#gac5ca6116dc08939f4332966036605763)   0x00000008|
|| |
|#define|[PP_FORMAT_420](http://ffmpeg.org/doxygen/trunk/group__lpp.html#ga8a589c15a9599d9c5f82f1951d5ef2f6)   (0x00000011|PP_FORMAT)|
|| |
|#define|[PP_FORMAT_422](http://ffmpeg.org/doxygen/trunk/group__lpp.html#ga93183c4dc8b06635e40c127e7f91bdd1)   (0x00000001|PP_FORMAT)|
|| |
|#define|[PP_FORMAT_411](http://ffmpeg.org/doxygen/trunk/group__lpp.html#gadf6e9121f608ccc203dd665f07d20f72)   (0x00000002|PP_FORMAT)|
|| |
|#define|[PP_FORMAT_444](http://ffmpeg.org/doxygen/trunk/group__lpp.html#gab96266f8dfad15c76ecad18a5ece3f13)   (0x00000000|PP_FORMAT)|
|| |
|#define|[PP_FORMAT_440](http://ffmpeg.org/doxygen/trunk/group__lpp.html#gae51854fd69dfeee5816f2283dd78dabd)   (0x00000010|PP_FORMAT)|
|| |
|#define|[PP_PICT_TYPE_QP2](http://ffmpeg.org/doxygen/trunk/group__lpp.html#ga8546d84b7dd3257439845f6ee4adba54)   0x00000010|

MPEG2风格的QScale。







二、类型定义


|typedef [void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[pp_context](http://ffmpeg.org/doxygen/trunk/group__lpp.html#ga0bbbccc0c988dfe2359f66e74933c51d)|
|----|----|
|| |
|typedef [void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[pp_mode](http://ffmpeg.org/doxygen/trunk/group__lpp.html#gab32903357ef0a647034c2f8f1a3aad0c)|
|| |
|typedef [pp_context](http://ffmpeg.org/doxygen/trunk/group__lpp.html#ga0bbbccc0c988dfe2359f66e74933c51d)|[pp_context_t](http://ffmpeg.org/doxygen/trunk/group__lpp.html#ga83b95c16f306b97f65e2caae33b5dfbc)|
|| |
|typedef [pp_mode](http://ffmpeg.org/doxygen/trunk/group__lpp.html#gab32903357ef0a647034c2f8f1a3aad0c)|[pp_mode_t](http://ffmpeg.org/doxygen/trunk/group__lpp.html#ga070953f8ff4441b3d9a86020e0aa541c)|





三、函数


|unsigned|[postproc_version](http://ffmpeg.org/doxygen/trunk/group__lpp.html#ga55be0317e8ec07c4d363589d1ec1eb81) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac))|
|----|----|

返回LIBPOSTPROC_VERSION_INT常量。





|const char *|[postproc_configuration](http://ffmpeg.org/doxygen/trunk/group__lpp.html#ga0f12bac7729c4c7fb51a2b727378db17) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac))|
|----|----|

返回libpostproc构建时配置。





|const char *|[postproc_license](http://ffmpeg.org/doxygen/trunk/group__lpp.html#ga2f60d4db2552da4b26c19f204a5f48c7) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac))|
|----|----|

返回libpostproc许可证。





|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[pp_postprocess](http://ffmpeg.org/doxygen/trunk/group__lpp.html#gad1f7efe31a2e03c7aca6db8dcaca0f2e) (const [uint8_t](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) *[src](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#a67caf60f8ec76e0adc7b693bfb651b66)[3], const [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) srcStride[3], [uint8_t](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) *dst[3], const [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) dstStride[3], [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) horizontalSize, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) verticalSize, const int8_t *QP_store, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) QP_stride, [pp_mode](http://ffmpeg.org/doxygen/trunk/group__lpp.html#gab32903357ef0a647034c2f8f1a3aad0c)*[mode](http://ffmpeg.org/doxygen/trunk/ebur128_8h.html#a1a6b6fb557d8d37d59700faf4e4c9167), [pp_context](http://ffmpeg.org/doxygen/trunk/group__lpp.html#ga0bbbccc0c988dfe2359f66e74933c51d) *ppContext, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) pict_type)|
|----|----|



|[pp_mode](http://ffmpeg.org/doxygen/trunk/group__lpp.html#gab32903357ef0a647034c2f8f1a3aad0c) *|[pp_get_mode_by_name_and_quality](http://ffmpeg.org/doxygen/trunk/group__lpp.html#gaaf27937128a07c2ca777235078fa420e) (const char *[name](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a09d548d751fcf0b9b67ea230712ae06c), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) quality)|
|----|----|

如果发生错误，返回一个pp_mode或NULL。

参数：name：在命令行上的“-pp”之后的字符串

   quality：一个从0到PP_QUALITY_MAX的数字





|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[pp_free_mode](http://ffmpeg.org/doxygen/trunk/group__lpp.html#gae12780b334051c3deab9f85aab6833ce) ([pp_mode](http://ffmpeg.org/doxygen/trunk/group__lpp.html#gab32903357ef0a647034c2f8f1a3aad0c) *[mode](http://ffmpeg.org/doxygen/trunk/ebur128_8h.html#a1a6b6fb557d8d37d59700faf4e4c9167))|
|----|----|



|[pp_context](http://ffmpeg.org/doxygen/trunk/group__lpp.html#ga0bbbccc0c988dfe2359f66e74933c51d) *|[pp_get_context](http://ffmpeg.org/doxygen/trunk/group__lpp.html#ga31fa719a4a7b99c5b0c5fd5bc1fc7b20) ([int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[width](http://ffmpeg.org/doxygen/trunk/tests_2utils_8c.html#a2474a5474cbff19523a51eb1de01cda4), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[height](http://ffmpeg.org/doxygen/trunk/tests_2utils_8c.html#ad12fc34ce789bce6c8a05d8a17138534), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[flags](http://ffmpeg.org/doxygen/trunk/log_8c.html#ac8bf36fe0577cba66bccda3a6f7e80a4))|
|----|----|



|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[pp_free_context](http://ffmpeg.org/doxygen/trunk/group__lpp.html#ga6cb6f1dca5f1f91fd86a8038e87ea37d) ([pp_context](http://ffmpeg.org/doxygen/trunk/group__lpp.html#ga0bbbccc0c988dfe2359f66e74933c51d) *ppContext)|
|----|----|





三、变量


|const char *const|[pp_help](http://ffmpeg.org/doxygen/trunk/group__lpp.html#ga08865dfff25499a0bcffaadac47d63d7)|
|----|----|

一个简单的帮助文本



