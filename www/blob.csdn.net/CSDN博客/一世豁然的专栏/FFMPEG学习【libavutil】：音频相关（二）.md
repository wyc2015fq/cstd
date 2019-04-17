# FFMPEG学习【libavutil】：音频相关（二） - 一世豁然的专栏 - CSDN博客





2017年07月29日 10:05:46[一世豁然](https://me.csdn.net/Explorer_day)阅读数：350








一、Audio channel layouts

一）、宏


|#define|[AV_CH_LAYOUT_MONO](http://ffmpeg.org/doxygen/trunk/group__channel__mask__c.html#ga53d6da2dcd56f5f87c7afd8b33fa15ba)   ([AV_CH_FRONT_CENTER](http://ffmpeg.org/doxygen/trunk/group__channel__masks.html#ga88f2868b232a89fd11ad9d6c39d502a1))|
|----|----|


|#define|[AV_CH_LAYOUT_STEREO](http://ffmpeg.org/doxygen/trunk/group__channel__mask__c.html#gabc6d6651bba254cd0fa2c42a57228e65)   ([AV_CH_FRONT_LEFT](http://ffmpeg.org/doxygen/trunk/group__channel__masks.html#gaf9c5619f74c99ea8a8606719c01fe7df)|[AV_CH_FRONT_RIGHT](http://ffmpeg.org/doxygen/trunk/group__channel__masks.html#ga394b4b2c87790178bf7b683b45320f31))|
|----|----|


|#define|[AV_CH_LAYOUT_STEREO](http://ffmpeg.org/doxygen/trunk/group__channel__mask__c.html#gabc6d6651bba254cd0fa2c42a57228e65)   ([AV_CH_FRONT_LEFT](http://ffmpeg.org/doxygen/trunk/group__channel__masks.html#gaf9c5619f74c99ea8a8606719c01fe7df)|[AV_CH_FRONT_RIGHT](http://ffmpeg.org/doxygen/trunk/group__channel__masks.html#ga394b4b2c87790178bf7b683b45320f31))|
|----|----|

|#define|[AV_CH_LAYOUT_2POINT1](http://ffmpeg.org/doxygen/trunk/group__channel__mask__c.html#ga03f59e18b3598aa0cab9ed675c287a36)   ([AV_CH_LAYOUT_STEREO](http://ffmpeg.org/doxygen/trunk/group__channel__mask__c.html#gabc6d6651bba254cd0fa2c42a57228e65)|[AV_CH_LOW_FREQUENCY](http://ffmpeg.org/doxygen/trunk/group__channel__masks.html#gab19b66e345cf1f9d50ffc26c28862bcc))|
|----|----|
|| |
|#define|[AV_CH_LAYOUT_2_1](http://ffmpeg.org/doxygen/trunk/group__channel__mask__c.html#gad0badd9c9984bc74290f26c2edfdfe44)   ([AV_CH_LAYOUT_STEREO](http://ffmpeg.org/doxygen/trunk/group__channel__mask__c.html#gabc6d6651bba254cd0fa2c42a57228e65)|[AV_CH_BACK_CENTER](http://ffmpeg.org/doxygen/trunk/group__channel__masks.html#ga68650a364e52edb62d5984dddec4f935))|
|| |
|#define|[AV_CH_LAYOUT_SURROUND](http://ffmpeg.org/doxygen/trunk/group__channel__mask__c.html#ga625aec7f2285dc35e3ed801e91889564)   ([AV_CH_LAYOUT_STEREO](http://ffmpeg.org/doxygen/trunk/group__channel__mask__c.html#gabc6d6651bba254cd0fa2c42a57228e65)|[AV_CH_FRONT_CENTER](http://ffmpeg.org/doxygen/trunk/group__channel__masks.html#ga88f2868b232a89fd11ad9d6c39d502a1))|
|| |
|#define|[AV_CH_LAYOUT_3POINT1](http://ffmpeg.org/doxygen/trunk/group__channel__mask__c.html#gae3fc8d83d40cfbd4910b16d526beb294)   ([AV_CH_LAYOUT_SURROUND](http://ffmpeg.org/doxygen/trunk/group__channel__mask__c.html#ga625aec7f2285dc35e3ed801e91889564)|[AV_CH_LOW_FREQUENCY](http://ffmpeg.org/doxygen/trunk/group__channel__masks.html#gab19b66e345cf1f9d50ffc26c28862bcc))|
|| |
|#define|[AV_CH_LAYOUT_4POINT0](http://ffmpeg.org/doxygen/trunk/group__channel__mask__c.html#ga0bcbe41f2d3214b0a3fd2698ebe0caf9)   ([AV_CH_LAYOUT_SURROUND](http://ffmpeg.org/doxygen/trunk/group__channel__mask__c.html#ga625aec7f2285dc35e3ed801e91889564)|[AV_CH_BACK_CENTER](http://ffmpeg.org/doxygen/trunk/group__channel__masks.html#ga68650a364e52edb62d5984dddec4f935))|
|| |
|#define|[AV_CH_LAYOUT_4POINT1](http://ffmpeg.org/doxygen/trunk/group__channel__mask__c.html#gacc48c498c05b3b3c2549f715ba4f1d1b)   ([AV_CH_LAYOUT_4POINT0](http://ffmpeg.org/doxygen/trunk/group__channel__mask__c.html#ga0bcbe41f2d3214b0a3fd2698ebe0caf9)|[AV_CH_LOW_FREQUENCY](http://ffmpeg.org/doxygen/trunk/group__channel__masks.html#gab19b66e345cf1f9d50ffc26c28862bcc))|
|| |
|#define|[AV_CH_LAYOUT_2_2](http://ffmpeg.org/doxygen/trunk/group__channel__mask__c.html#ga5c25ee8b976d0cb15e0b6d2cf923d19d)   ([AV_CH_LAYOUT_STEREO](http://ffmpeg.org/doxygen/trunk/group__channel__mask__c.html#gabc6d6651bba254cd0fa2c42a57228e65)|[AV_CH_SIDE_LEFT](http://ffmpeg.org/doxygen/trunk/group__channel__masks.html#ga02e55f35249dfa57a2e4edfd3acfec4f)|[AV_CH_SIDE_RIGHT](http://ffmpeg.org/doxygen/trunk/group__channel__masks.html#ga947b60ab70c428ebb07c8b9c87023b39))|
|| |
|#define|[AV_CH_LAYOUT_QUAD](http://ffmpeg.org/doxygen/trunk/group__channel__mask__c.html#ga0982ce29eced939dfcfb5b52416810ae)   ([AV_CH_LAYOUT_STEREO](http://ffmpeg.org/doxygen/trunk/group__channel__mask__c.html#gabc6d6651bba254cd0fa2c42a57228e65)|[AV_CH_BACK_LEFT](http://ffmpeg.org/doxygen/trunk/group__channel__masks.html#gaa3cae8c6d0eaab3dc3ce178f6af81ae2)|[AV_CH_BACK_RIGHT](http://ffmpeg.org/doxygen/trunk/group__channel__masks.html#ga7a7ab3bb418bf9672dcdf6554346e5eb))|
|| |
|#define|[AV_CH_LAYOUT_5POINT0](http://ffmpeg.org/doxygen/trunk/group__channel__mask__c.html#ga10eddec555bcf849efce5158b0cbdb50)   ([AV_CH_LAYOUT_SURROUND](http://ffmpeg.org/doxygen/trunk/group__channel__mask__c.html#ga625aec7f2285dc35e3ed801e91889564)|[AV_CH_SIDE_LEFT](http://ffmpeg.org/doxygen/trunk/group__channel__masks.html#ga02e55f35249dfa57a2e4edfd3acfec4f)|[AV_CH_SIDE_RIGHT](http://ffmpeg.org/doxygen/trunk/group__channel__masks.html#ga947b60ab70c428ebb07c8b9c87023b39))|
|| |
|#define|[AV_CH_LAYOUT_5POINT1](http://ffmpeg.org/doxygen/trunk/group__channel__mask__c.html#gac36a042dc21866f9043ce930f300d0df)   ([AV_CH_LAYOUT_5POINT0](http://ffmpeg.org/doxygen/trunk/group__channel__mask__c.html#ga10eddec555bcf849efce5158b0cbdb50)|[AV_CH_LOW_FREQUENCY](http://ffmpeg.org/doxygen/trunk/group__channel__masks.html#gab19b66e345cf1f9d50ffc26c28862bcc))|
|| |
|#define|[AV_CH_LAYOUT_5POINT0_BACK](http://ffmpeg.org/doxygen/trunk/group__channel__mask__c.html#gab057e9fd36b7e929d000d663badbafc6)   ([AV_CH_LAYOUT_SURROUND](http://ffmpeg.org/doxygen/trunk/group__channel__mask__c.html#ga625aec7f2285dc35e3ed801e91889564)|[AV_CH_BACK_LEFT](http://ffmpeg.org/doxygen/trunk/group__channel__masks.html#gaa3cae8c6d0eaab3dc3ce178f6af81ae2)|[AV_CH_BACK_RIGHT](http://ffmpeg.org/doxygen/trunk/group__channel__masks.html#ga7a7ab3bb418bf9672dcdf6554346e5eb))|
|| |
|#define|[AV_CH_LAYOUT_5POINT1_BACK](http://ffmpeg.org/doxygen/trunk/group__channel__mask__c.html#gae9da5d6069701241a22f3af19b152662)   ([AV_CH_LAYOUT_5POINT0_BACK](http://ffmpeg.org/doxygen/trunk/group__channel__mask__c.html#gab057e9fd36b7e929d000d663badbafc6)|[AV_CH_LOW_FREQUENCY](http://ffmpeg.org/doxygen/trunk/group__channel__masks.html#gab19b66e345cf1f9d50ffc26c28862bcc))|
|| |
|#define|[AV_CH_LAYOUT_6POINT0](http://ffmpeg.org/doxygen/trunk/group__channel__mask__c.html#ga0fbea70d397140a524b55018128fca77)   ([AV_CH_LAYOUT_5POINT0](http://ffmpeg.org/doxygen/trunk/group__channel__mask__c.html#ga10eddec555bcf849efce5158b0cbdb50)|[AV_CH_BACK_CENTER](http://ffmpeg.org/doxygen/trunk/group__channel__masks.html#ga68650a364e52edb62d5984dddec4f935))|
|| |
|#define|[AV_CH_LAYOUT_6POINT0_FRONT](http://ffmpeg.org/doxygen/trunk/group__channel__mask__c.html#ga735985d89e38896a0cb9e5a1e96cb9b8)   ([AV_CH_LAYOUT_2_2](http://ffmpeg.org/doxygen/trunk/group__channel__mask__c.html#ga5c25ee8b976d0cb15e0b6d2cf923d19d)|[AV_CH_FRONT_LEFT_OF_CENTER](http://ffmpeg.org/doxygen/trunk/group__channel__masks.html#ga6c9ea2ea60044bfe844174300b237386)|[AV_CH_FRONT_RIGHT_OF_CENTER](http://ffmpeg.org/doxygen/trunk/group__channel__masks.html#ga558dc25b0ce327d2547d3cc9e4bd6a4e))|
|| |
|#define|[AV_CH_LAYOUT_HEXAGONAL](http://ffmpeg.org/doxygen/trunk/group__channel__mask__c.html#ga44e719835fe17c5af5e619083918d1ca)   ([AV_CH_LAYOUT_5POINT0_BACK](http://ffmpeg.org/doxygen/trunk/group__channel__mask__c.html#gab057e9fd36b7e929d000d663badbafc6)|[AV_CH_BACK_CENTER](http://ffmpeg.org/doxygen/trunk/group__channel__masks.html#ga68650a364e52edb62d5984dddec4f935))|
|| |
|#define|[AV_CH_LAYOUT_6POINT1](http://ffmpeg.org/doxygen/trunk/group__channel__mask__c.html#ga0b63b18f49c294bfbc22afa8b59b6685)   ([AV_CH_LAYOUT_5POINT1](http://ffmpeg.org/doxygen/trunk/group__channel__mask__c.html#gac36a042dc21866f9043ce930f300d0df)|[AV_CH_BACK_CENTER](http://ffmpeg.org/doxygen/trunk/group__channel__masks.html#ga68650a364e52edb62d5984dddec4f935))|
|| |
|#define|[AV_CH_LAYOUT_6POINT1_BACK](http://ffmpeg.org/doxygen/trunk/group__channel__mask__c.html#gaaf1920f827888aeb99540f3ee5a78cea)   ([AV_CH_LAYOUT_5POINT1_BACK](http://ffmpeg.org/doxygen/trunk/group__channel__mask__c.html#gae9da5d6069701241a22f3af19b152662)|[AV_CH_BACK_CENTER](http://ffmpeg.org/doxygen/trunk/group__channel__masks.html#ga68650a364e52edb62d5984dddec4f935))|
|| |
|#define|[AV_CH_LAYOUT_6POINT1_FRONT](http://ffmpeg.org/doxygen/trunk/group__channel__mask__c.html#ga934ec29833f0cbc4a1fe8f97cc914664)   ([AV_CH_LAYOUT_6POINT0_FRONT](http://ffmpeg.org/doxygen/trunk/group__channel__mask__c.html#ga735985d89e38896a0cb9e5a1e96cb9b8)|[AV_CH_LOW_FREQUENCY](http://ffmpeg.org/doxygen/trunk/group__channel__masks.html#gab19b66e345cf1f9d50ffc26c28862bcc))|
|| |
|#define|[AV_CH_LAYOUT_7POINT0](http://ffmpeg.org/doxygen/trunk/group__channel__mask__c.html#gab843d4e87bc747c7a81f082589023f98)   ([AV_CH_LAYOUT_5POINT0](http://ffmpeg.org/doxygen/trunk/group__channel__mask__c.html#ga10eddec555bcf849efce5158b0cbdb50)|[AV_CH_BACK_LEFT](http://ffmpeg.org/doxygen/trunk/group__channel__masks.html#gaa3cae8c6d0eaab3dc3ce178f6af81ae2)|[AV_CH_BACK_RIGHT](http://ffmpeg.org/doxygen/trunk/group__channel__masks.html#ga7a7ab3bb418bf9672dcdf6554346e5eb))|
|| |
|#define|[AV_CH_LAYOUT_7POINT0_FRONT](http://ffmpeg.org/doxygen/trunk/group__channel__mask__c.html#gaa4c21d7abb460730295f99c7f0098259)   ([AV_CH_LAYOUT_5POINT0](http://ffmpeg.org/doxygen/trunk/group__channel__mask__c.html#ga10eddec555bcf849efce5158b0cbdb50)|[AV_CH_FRONT_LEFT_OF_CENTER](http://ffmpeg.org/doxygen/trunk/group__channel__masks.html#ga6c9ea2ea60044bfe844174300b237386)|[AV_CH_FRONT_RIGHT_OF_CENTER](http://ffmpeg.org/doxygen/trunk/group__channel__masks.html#ga558dc25b0ce327d2547d3cc9e4bd6a4e))|
|| |
|#define|[AV_CH_LAYOUT_7POINT1](http://ffmpeg.org/doxygen/trunk/group__channel__mask__c.html#ga2006ce652a95bad74291db52fa346464)   ([AV_CH_LAYOUT_5POINT1](http://ffmpeg.org/doxygen/trunk/group__channel__mask__c.html#gac36a042dc21866f9043ce930f300d0df)|[AV_CH_BACK_LEFT](http://ffmpeg.org/doxygen/trunk/group__channel__masks.html#gaa3cae8c6d0eaab3dc3ce178f6af81ae2)|[AV_CH_BACK_RIGHT](http://ffmpeg.org/doxygen/trunk/group__channel__masks.html#ga7a7ab3bb418bf9672dcdf6554346e5eb))|
|| |
|#define|[AV_CH_LAYOUT_7POINT1_WIDE](http://ffmpeg.org/doxygen/trunk/group__channel__mask__c.html#ga883478469aa86f5c3f57ce864a12745b)   ([AV_CH_LAYOUT_5POINT1](http://ffmpeg.org/doxygen/trunk/group__channel__mask__c.html#gac36a042dc21866f9043ce930f300d0df)|[AV_CH_FRONT_LEFT_OF_CENTER](http://ffmpeg.org/doxygen/trunk/group__channel__masks.html#ga6c9ea2ea60044bfe844174300b237386)|[AV_CH_FRONT_RIGHT_OF_CENTER](http://ffmpeg.org/doxygen/trunk/group__channel__masks.html#ga558dc25b0ce327d2547d3cc9e4bd6a4e))|
|| |
|#define|[AV_CH_LAYOUT_7POINT1_WIDE_BACK](http://ffmpeg.org/doxygen/trunk/group__channel__mask__c.html#ga9702d40a7d1f6e2b00e02d8ecb83bd63)   ([AV_CH_LAYOUT_5POINT1_BACK](http://ffmpeg.org/doxygen/trunk/group__channel__mask__c.html#gae9da5d6069701241a22f3af19b152662)|[AV_CH_FRONT_LEFT_OF_CENTER](http://ffmpeg.org/doxygen/trunk/group__channel__masks.html#ga6c9ea2ea60044bfe844174300b237386)|[AV_CH_FRONT_RIGHT_OF_CENTER](http://ffmpeg.org/doxygen/trunk/group__channel__masks.html#ga558dc25b0ce327d2547d3cc9e4bd6a4e))|
|| |
|#define|[AV_CH_LAYOUT_OCTAGONAL](http://ffmpeg.org/doxygen/trunk/group__channel__mask__c.html#gaa760931c084e33a254fca038b7de8dae)   ([AV_CH_LAYOUT_5POINT0](http://ffmpeg.org/doxygen/trunk/group__channel__mask__c.html#ga10eddec555bcf849efce5158b0cbdb50)|[AV_CH_BACK_LEFT](http://ffmpeg.org/doxygen/trunk/group__channel__masks.html#gaa3cae8c6d0eaab3dc3ce178f6af81ae2)|[AV_CH_BACK_CENTER](http://ffmpeg.org/doxygen/trunk/group__channel__masks.html#ga68650a364e52edb62d5984dddec4f935)|[AV_CH_BACK_RIGHT](http://ffmpeg.org/doxygen/trunk/group__channel__masks.html#ga7a7ab3bb418bf9672dcdf6554346e5eb))|
|| |
|#define|[AV_CH_LAYOUT_HEXADECAGONAL](http://ffmpeg.org/doxygen/trunk/group__channel__mask__c.html#gafdbfe62b38dd3c6b1f89023f7aa6994c)   ([AV_CH_LAYOUT_OCTAGONAL](http://ffmpeg.org/doxygen/trunk/group__channel__mask__c.html#gaa760931c084e33a254fca038b7de8dae)|[AV_CH_WIDE_LEFT](http://ffmpeg.org/doxygen/trunk/group__channel__masks.html#ga11e273c4872335063b5059cca52218c0)|[AV_CH_WIDE_RIGHT](http://ffmpeg.org/doxygen/trunk/group__channel__masks.html#ga2c7ff738c3481d2891d052f5bca55248)|[AV_CH_TOP_BACK_LEFT](http://ffmpeg.org/doxygen/trunk/group__channel__masks.html#gaac082f244f37f9c5d8e4f7bf96cc76dc)|[AV_CH_TOP_BACK_RIGHT](http://ffmpeg.org/doxygen/trunk/group__channel__masks.html#ga8fa88c3c74339f4218a3e70286fdbdaa)|[AV_CH_TOP_BACK_CENTER](http://ffmpeg.org/doxygen/trunk/group__channel__masks.html#ga439a8664c452f9782b460f61a3908451)|[AV_CH_TOP_FRONT_CENTER](http://ffmpeg.org/doxygen/trunk/group__channel__masks.html#ga6b558fe85e67383a5b04e56b97b145d0)|[AV_CH_TOP_FRONT_LEFT](http://ffmpeg.org/doxygen/trunk/group__channel__masks.html#ga352bec66d2e1bef67efe5b7f0a4207f3)|[AV_CH_TOP_FRONT_RIGHT](http://ffmpeg.org/doxygen/trunk/group__channel__masks.html#gabce29ea1d7e2d433263b2cbf28232360))|
|| |
|#define|[AV_CH_LAYOUT_STEREO_DOWNMIX](http://ffmpeg.org/doxygen/trunk/group__channel__mask__c.html#ga3004e784b29fd907a0d4aebeca0e6933)   ([AV_CH_STEREO_LEFT](http://ffmpeg.org/doxygen/trunk/group__channel__masks.html#gaea8f62dcaa4774307a8fa9b52172c688)|[AV_CH_STEREO_RIGHT](http://ffmpeg.org/doxygen/trunk/group__channel__masks.html#gab68e7edb9182b1b22898b960707143c1))|





二）、枚举


|#define|[AV_CH_LAYOUT_STEREO](http://ffmpeg.org/doxygen/trunk/group__channel__mask__c.html#gabc6d6651bba254cd0fa2c42a57228e65)   ([AV_CH_FRONT_LEFT](http://ffmpeg.org/doxygen/trunk/group__channel__masks.html#gaf9c5619f74c99ea8a8606719c01fe7df)|[AV_CH_FRONT_RIGHT](http://ffmpeg.org/doxygen/trunk/group__channel__masks.html#ga394b4b2c87790178bf7b683b45320f31))|
|----|----|

|enum|[AVMatrixEncoding](http://ffmpeg.org/doxygen/trunk/group__channel__mask__c.html#gabd576ea664e3ac5d31f3362064fe13e6) { [AV_MATRIX_ENCODING_NONE](http://ffmpeg.org/doxygen/trunk/group__channel__mask__c.html#ggabd576ea664e3ac5d31f3362064fe13e6a04179fd7b3339556603020ae88e02793), [AV_MATRIX_ENCODING_DOLBY](http://ffmpeg.org/doxygen/trunk/group__channel__mask__c.html#ggabd576ea664e3ac5d31f3362064fe13e6a0578f715ed49986a6e4c40884c174dbd), [AV_MATRIX_ENCODING_DPLII](http://ffmpeg.org/doxygen/trunk/group__channel__mask__c.html#ggabd576ea664e3ac5d31f3362064fe13e6a068d30f3ce95f82781b0052bf86bc10e), [AV_MATRIX_ENCODING_DPLIIX](http://ffmpeg.org/doxygen/trunk/group__channel__mask__c.html#ggabd576ea664e3ac5d31f3362064fe13e6a03124652d3e9efb6ce687971c7abbfa1), [AV_MATRIX_ENCODING_DPLIIZ](http://ffmpeg.org/doxygen/trunk/group__channel__mask__c.html#ggabd576ea664e3ac5d31f3362064fe13e6a927893de2c83ddb117fb89dcc579e363), [AV_MATRIX_ENCODING_DOLBYEX](http://ffmpeg.org/doxygen/trunk/group__channel__mask__c.html#ggabd576ea664e3ac5d31f3362064fe13e6aa155505a7fb9f2b45778b1d7776dcfb7), [AV_MATRIX_ENCODING_DOLBYHEADPHONE](http://ffmpeg.org/doxygen/trunk/group__channel__mask__c.html#ggabd576ea664e3ac5d31f3362064fe13e6ad9089736c1dd449617a2805d37e502d3), [AV_MATRIX_ENCODING_NB](http://ffmpeg.org/doxygen/trunk/group__channel__mask__c.html#ggabd576ea664e3ac5d31f3362064fe13e6a3a3acaf62825a3b21c7631ddf1dcc648)}|
|----|----|





三）、函数


|uint64_t|[av_get_channel_layout](http://ffmpeg.org/doxygen/trunk/group__channel__mask__c.html#ga3e8a7ab3d4c1ac98b902ab6b00741343) (const char *[name](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a09d548d751fcf0b9b67ea230712ae06c))|
|----|----|

返回与名称匹配的通道布局ID，如果未找到匹配，则返回0。



名称可以是以下一个或几个符号，用“+”或“|”分隔：


1、通常的通道布局（单声道，立体声，4.0，四，5.0，5.0（侧），5.1,5.1（侧），7.1,7.1（宽），下混合）的名称;

2、单个通道的名称（FL，FR，FC，LFE，BL，BR，FLC，FRC，BC，SL，SR，TC，TFL，TFC，TFR，TBL，TBC，TBR，DL，DR）

3、多个通道，以十进制，后跟'c'，产生该通道数的默认通道布局（查看[av_get_default_channel_layout](http://ffmpeg.org/doxygen/trunk/group__channel__mask__c.html#gacb84f3e93a583e1f84a5283162a606a2)）

4、一个通道布局掩码，十六进制以“0x”开头（见AV_CH_ *宏）。

示例：“stereo + FC”=“2c + FC”=“2c + 1c”=“0x7”






|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_get_extended_channel_layout](http://ffmpeg.org/doxygen/trunk/group__channel__mask__c.html#gad2336dae5569ae7fe509a9ea12df6a60) (const char *[name](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a09d548d751fcf0b9b67ea230712ae06c), uint64_t *channel_layout, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) *[nb_channels](http://ffmpeg.org/doxygen/trunk/channel__layout_8c.html#ac8404c077a0560a06a73cfec0697c99a))|
|----|----|

根据指定的名称返回通道布局和通道数。



这个函数类似于（查看[av_get_channel_layout](http://ffmpeg.org/doxygen/trunk/group__channel__mask__c.html#ga3e8a7ab3d4c1ac98b902ab6b00741343)），但也可以解析未知的通道布局规格。


参数：index：通道布局规格字符串

   layout：解析的频道布局（如果未知，则为0）

   name：通道数量

返回：成功时为0，如果解析失败，则为AVERROR（EINVAL）。





|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_get_channel_layout_string](http://ffmpeg.org/doxygen/trunk/group__channel__mask__c.html#ga99d0b5bb80534d13a6cc96336cf9076a) (char *[buf](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#a5bc5fa69bee375df074734a2c4858604), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) buf_size, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[nb_channels](http://ffmpeg.org/doxygen/trunk/channel__layout_8c.html#ac8404c077a0560a06a73cfec0697c99a), uint64_t channel_layout)|
|----|----|

返回一个通道布局的描述。

如果nb_channels为<= 0，则从channel_layout中猜出。


参数：buf:在这里放置包含通道布局的字符串

   buf_size：缓冲区大小（以字节为单位）





|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_bprint_channel_layout](http://ffmpeg.org/doxygen/trunk/group__channel__mask__c.html#ga48cdbfac1bbb45835eae8bcdc9877b0e) (struct AVBPrint *bp, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[nb_channels](http://ffmpeg.org/doxygen/trunk/channel__layout_8c.html#ac8404c077a0560a06a73cfec0697c99a), uint64_t channel_layout)|
|----|----|

将通道布局的描述附加到打印缓冲区。





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_get_channel_layout_nb_channels](http://ffmpeg.org/doxygen/trunk/group__channel__mask__c.html#gac95619abeb32e4a3daa18e3ff3419380) (uint64_t channel_layout)|
|----|----|

返回通道布局中的通道数。





|int64_t|[av_get_default_channel_layout](http://ffmpeg.org/doxygen/trunk/group__channel__mask__c.html#gacb84f3e93a583e1f84a5283162a606a2) ([int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[nb_channels](http://ffmpeg.org/doxygen/trunk/channel__layout_8c.html#ac8404c077a0560a06a73cfec0697c99a))|
|----|----|

返回给定数量通道的默认通道布局。





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_get_channel_layout_channel_index](http://ffmpeg.org/doxygen/trunk/group__channel__mask__c.html#ga0669978493902b5ae723e386013da3ce) (uint64_t channel_layout, uint64_t [channel](http://ffmpeg.org/doxygen/trunk/ebur128_8h.html#af4463779bb9b119822c94419a10d31c0))|
|----|----|

获取channel_layout中的通道索引。

参数：channel：一个通道布局，描述channel_layout中必须存在的一个通道。

返回：channel_layout中的频道索引成功，错误的AVERROR为负。





|uint64_t|[av_channel_layout_extract_channel](http://ffmpeg.org/doxygen/trunk/group__channel__mask__c.html#gaf492843360fb4ecee6b5ec4166b04fcd) (uint64_t channel_layout, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[index](http://ffmpeg.org/doxygen/trunk/mxfenc_8c.html#a750b5d744c39a06bfb13e6eb010e35d0))|
|----|----|

在channel_layout中获取给定索引的通道。





|const char *|[av_get_channel_name](http://ffmpeg.org/doxygen/trunk/group__channel__mask__c.html#gaf9a5e23a9bbf5909f2dc84b25d78e35e) (uint64_t [channel](http://ffmpeg.org/doxygen/trunk/ebur128_8h.html#af4463779bb9b119822c94419a10d31c0))|
|----|----|

获取给定通道的名称。

返回：通道名称成功，NULL错误。





|const char *|[av_get_channel_description](http://ffmpeg.org/doxygen/trunk/group__channel__mask__c.html#gade4b74b99e3fb954b88b0cb8eddb785a) (uint64_t [channel](http://ffmpeg.org/doxygen/trunk/ebur128_8h.html#af4463779bb9b119822c94419a10d31c0))|
|----|----|

获取给定通道的描述。

参数：channel：具有单个通道的通道布局

返回：通道描述成功，NULL错误





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_get_standard_channel_layout](http://ffmpeg.org/doxygen/trunk/group__channel__mask__c.html#ga00447bfae43d690f1f549a9e2521bf85) (unsigned [index](http://ffmpeg.org/doxygen/trunk/mxfenc_8c.html#a750b5d744c39a06bfb13e6eb010e35d0), uint64_t *[layout](http://ffmpeg.org/doxygen/trunk/channel__layout_8c.html#a5fb8d8660239f03323dae7e7c1699f29), const char **[name](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a09d548d751fcf0b9b67ea230712ae06c))|
|----|----|

获取标准通道布局的值和名称。

参数：index：索引在内部列表中，从0开始

   layout：通道布局面具

   name：布局名称



返回：如果布局存在，则为0，如果索引超出限制，则为0







二、Audio downmix metadata

一）、数据结构

此结构描述与下混程序相关的可选元数据。


所有字段由解码器设置为音频比特流（如果存在）中指示的值，否则为“理智”默认值。



|struct|[AVDownmixInfo](http://ffmpeg.org/doxygen/trunk/structAVDownmixInfo.html){enum AVDownmixType preferred_downmix_type；//母版工程师优选的下混类型。double center_mix_level;//表示正常下混时中心通道的标称电平的绝对比例因子。double center_mix_level_ltrt;//表示在Lt / Rt兼容下混合期间中心通道的标称电平的绝对比例因子。double surround_mix_level;//表示正常缩混期间环绕声道的标称电平的绝对比例因子。double surround_mix_level_ltrt;//表示在Lt / Rt兼容下混合期间环绕声道的标称电平的绝对比例因子。double lfe_mix_level;//表示在下混期间将LFE数据混入L / R通道的电平的绝对比例因子。}|
|----|----|





二）、宏


|enum|[AVDownmixType](http://ffmpeg.org/doxygen/trunk/group__downmix__info.html#ga3e3654fedbe3e5485f00ea09396c3521) { [AV_DOWNMIX_TYPE_UNKNOWN](http://ffmpeg.org/doxygen/trunk/group__downmix__info.html#gga3e3654fedbe3e5485f00ea09396c3521a1184863af0dcd15f6851f2db10fa355c), [AV_DOWNMIX_TYPE_LORO](http://ffmpeg.org/doxygen/trunk/group__downmix__info.html#gga3e3654fedbe3e5485f00ea09396c3521a63e63739b6ea34a5ec073ad6ead25f8e), [AV_DOWNMIX_TYPE_LTRT](http://ffmpeg.org/doxygen/trunk/group__downmix__info.html#gga3e3654fedbe3e5485f00ea09396c3521a3737f8fe65fe8b805b256efa8d1328ef), [AV_DOWNMIX_TYPE_DPLII](http://ffmpeg.org/doxygen/trunk/group__downmix__info.html#gga3e3654fedbe3e5485f00ea09396c3521ac92fde94fbc0aa50b93071d995a36fca), [AV_DOWNMIX_TYPE_NB](http://ffmpeg.org/doxygen/trunk/group__downmix__info.html#gga3e3654fedbe3e5485f00ea09396c3521ae8e1a1a755fccdddddbc9680694af182)}|
|----|----|





三）、函数


|[AVDownmixInfo](http://ffmpeg.org/doxygen/trunk/structAVDownmixInfo.html) *|[av_downmix_info_update_side_data](http://ffmpeg.org/doxygen/trunk/group__downmix__info.html#gadedd619e3fd3bb7732b917bfafba97a8) ([AVFrame](http://ffmpeg.org/doxygen/trunk/structAVFrame.html) *[frame](http://ffmpeg.org/doxygen/trunk/extract__mvs_8c.html#ad7d33d579a8d4241a5e643e39287a209))|
|----|----|

获取帧的AV_FRAME_DATA_DOWNMIX_INFO边数据进行编辑。

如果侧面数据不存在，则创建它并将其添加到框架中。


参数：frame：要获取或创建副数据的帧。

返回：要由调用者编辑的AVDownmixInfo结构，如果结构不能被分配，则为NULL。





