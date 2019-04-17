# FFMPEG学习【libavutil】：String Manipulation - 一世豁然的专栏 - CSDN博客





2017年07月17日 09:13:46[一世豁然](https://me.csdn.net/Explorer_day)阅读数：648








一、宏


|#define|[AV_ESCAPE_FLAG_WHITESPACE](https://ffmpeg.org/doxygen/trunk/group__lavu__string.html#ga84ac09ffcff9c3c42c35db1f8477cc17)   (1 << 0)|
|----|----|

考虑空间特别，甚至在字符串的中间逃避他们。

这相当于将空格字符添加到特殊字符列表中，除了它保证使用完全相同的空格字符列表作为libavutil的其余部分。






|#define|[AV_ESCAPE_FLAG_STRICT](https://ffmpeg.org/doxygen/trunk/group__lavu__string.html#ga24963cc06a1b89194ba8a2fe013a7d7d)   (1 << 1)|
|----|----|

仅转义指定的特殊字符。

没有这个标志，也可以逃避av_get_token（）可能被视为特殊的任何字符，例如单引号。






|#define|[AV_UTF8_FLAG_ACCEPT_INVALID_BIG_CODES](https://ffmpeg.org/doxygen/trunk/group__lavu__string.html#ga61b431e73b72949c839788914a599207)   1|
|----|----|

接受0x10FFFF以上的代码点






|#define|[AV_UTF8_FLAG_ACCEPT_NON_CHARACTERS](https://ffmpeg.org/doxygen/trunk/group__lavu__string.html#gadc5f344d3eafe34e9c3169b1178e137b)   2|
|----|----|

接受非字符 - 0xFFFE和0xFFFF





|#define|[AV_UTF8_FLAG_ACCEPT_SURROGATES](https://ffmpeg.org/doxygen/trunk/group__lavu__string.html#ga39a93e11907ca4b116d8d179cbdb570d)   4|
|----|----|

接受UTF-16代理代码

‘

’

|#define|[AV_UTF8_FLAG_EXCLUDE_XML_INVALID_CONTROL_CODES](https://ffmpeg.org/doxygen/trunk/group__lavu__string.html#gaecaf936b2eb2f3c410869e235650229b)   8|
|----|----|

排除不被XML接受的控制代码





|#define|[AV_UTF8_FLAG_ACCEPT_ALL](https://ffmpeg.org/doxygen/trunk/group__lavu__string.html#ga4825349f5346880c6ac32b0847e9a73a)[AV_UTF8_FLAG_ACCEPT_INVALID_BIG_CODES](https://ffmpeg.org/doxygen/trunk/group__lavu__string.html#ga61b431e73b72949c839788914a599207)|[AV_UTF8_FLAG_ACCEPT_NON_CHARACTERS](https://ffmpeg.org/doxygen/trunk/group__lavu__string.html#gadc5f344d3eafe34e9c3169b1178e137b)|[AV_UTF8_FLAG_ACCEPT_SURROGATES](https://ffmpeg.org/doxygen/trunk/group__lavu__string.html#ga39a93e11907ca4b116d8d179cbdb570d)|
|----|----|
|| |





二、枚举


|enum|[AVEscapeMode](https://ffmpeg.org/doxygen/trunk/group__lavu__string.html#gaef885958293f07153383f99db573ff5a) { [AV_ESCAPE_MODE_AUTO](https://ffmpeg.org/doxygen/trunk/group__lavu__string.html#ggaef885958293f07153383f99db573ff5aa342d7bf60c9a7cb7a886f70f81ac5071), [AV_ESCAPE_MODE_BACKSLASH](https://ffmpeg.org/doxygen/trunk/group__lavu__string.html#ggaef885958293f07153383f99db573ff5aa8b2d26951a9e06d8f7c12977a204eaf9), [AV_ESCAPE_MODE_QUOTE](https://ffmpeg.org/doxygen/trunk/group__lavu__string.html#ggaef885958293f07153383f99db573ff5aa84da6c323003df474c690ccf5d4e5d67) }|
|----|----|





三、函数


|[int](https://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_strstart](https://ffmpeg.org/doxygen/trunk/group__lavu__string.html#ga6135a12628e2b6a63c8d3d9b3a742b06) (const char *str, const char *pfx, const char **ptr)|
|----|----|

如果pfx是str的前缀，返回非零。

如果是，则将* ptr设置为str前面的第一个字符的地址。


参数：str：输入字符

   pfx：前缀测试

   ptr：如果前缀在str中匹配，则更新

返回：如果前缀匹配则为非零，否则为零





|[int](https://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_stristart](https://ffmpeg.org/doxygen/trunk/group__lavu__string.html#gacd29ff1f7e62230a113c88fa10d3f5b9) (const char *str, const char *pfx, const char **ptr)|
|----|----|

如果pfx是独立于大小写的str的前缀，返回非零。

如果是，则将* ptr设置为str前面的第一个字符的地址。


参数：str：输入字符

   pfx：前缀测试

   ptr：如果前缀在str中匹配，则更新

返回：如果前缀匹配则为非零，否则为零






|char *|[av_stristr](https://ffmpeg.org/doxygen/trunk/group__lavu__string.html#ga47e14424edfb96d3f6bf0fca96a76fe5) (const char *haystack, const char *needle)|
|----|----|

在字符串针的字符串干草堆中找到第一个不区分大小写的事件。

在干草堆开始时，零长度的针头针被认为是匹配的。


此函数是标准strstr（）的不区分大小写的版本。


参数：haystack：要搜索的字符串

needle：待搜索字符串

返回：指向haystack中找到的匹配的指针，如果没有匹配，则指向空指针





|char *|[av_strnstr](https://ffmpeg.org/doxygen/trunk/group__lavu__string.html#ga827486f333df7bc9a4e7f9bd212ba472) (const char *haystack, const char *needle, size_t hay_length)|
|----|----|

在字符串haystack中找到字符串针的第一次出现，其中不超过hay_length字符被搜索。

在haystack开始时，零长度的针头针被认为是匹配的。


此函数是标准strstr（）的长度限制版本。


参数：haystack：要搜索的字符串

needle：待搜索字符串

hay_length：要搜索的字符串的长度

返回：指向haystack中找到的匹配的指针，如果没有匹配，则指向空指针





|size_t|[av_strlcpy](https://ffmpeg.org/doxygen/trunk/group__lavu__string.html#ga716447ae73b612bcf3a405b9b4608f6c) (char *dst, const char *[src](https://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#a67caf60f8ec76e0adc7b693bfb651b66), size_t [size](https://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a3d1e3edfcf61ca2d831883e1afbad89e))|
|----|----|

将字符串src复制到dst，但不能超过size-1个字节，并将null终止dst。

此功能与BSD strlcpy（）相同。


参数：dst：目的缓冲区

   src：源字符串

   size：目的缓冲区的大小

返回：源字符串长度

警告：由于返回值是src的长度，src绝对必须是一个正确的0终止的字符串，否则这将读取超出缓冲区的结尾并可能崩溃。





|size_t|[av_strlcat](https://ffmpeg.org/doxygen/trunk/group__lavu__string.html#ga193877ba459faf314c3d285b6082cf73) (char *dst, const char *[src](https://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#a67caf60f8ec76e0adc7b693bfb651b66), size_t [size](https://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a3d1e3edfcf61ca2d831883e1afbad89e))|
|----|----|

将字符串src附加到字符串dst，但总长度不得大于1字节，并且为null终止dst。

此功能类似于BSD strlcat（），但在大小<= strlen（dst）时不同。


参数：dst：目的缓冲区

   src：源字符串

    size：目的缓冲区的大小

返回：src和dst的总长度

警告：由于返回值使用src和dst的长度，这些绝对必须是一个正确的0终止的字符串，否则这将读取超出缓冲区的结尾，并可能会崩溃。





|size_t|[av_strlcatf](https://ffmpeg.org/doxygen/trunk/group__lavu__string.html#gae264e2caf6ad1ca830ce3f2b49abdf50) (char *dst, size_t [size](https://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a3d1e3edfcf61ca2d831883e1afbad89e), const char *[fmt](https://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#ad9a5b33dafc33a26929ac177b964d4ab),...) [av_printf_format](https://ffmpeg.org/doxygen/trunk/rtpproto_8c.html#a592f4a5e13fd709e6e0bc93a3131612d)(3|
|----|----|

根据格式将输出附加到字符串。

不要写出目标缓冲区，并且始终将缓冲区中的终止0。


参数：dst：目标缓冲区（附加输出的字符串）

   size：目标缓冲区的总大小

   fmt：printf兼容的格式字符串，指定如何使用以下参数

返回：如果有足够的空间可用，将生成的字符串的长度





|size_t static size_t|[av_strnlen](https://ffmpeg.org/doxygen/trunk/group__lavu__string.html#ga1cb2a3929575d28a415dc02b917ecc01) (const char *[s](https://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#ab87f55bd0280d90925050a4188c14ab5), size_t [len](https://ffmpeg.org/doxygen/trunk/vorbis__enc__data_8h.html#afed088663f8704004425cdae2120b9b3))|
|----|----|

从头开始获取连续非零字符的数量。

参数：len：要检查字符串的最大字符数，即该函数返回的最大值





|char *|[av_asprintf](https://ffmpeg.org/doxygen/trunk/group__lavu__string.html#ga0a8e3f4a2a3039c441bd0826e4727462) (const char *[fmt](https://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#ad9a5b33dafc33a26929ac177b964d4ab),...) [av_printf_format](https://ffmpeg.org/doxygen/trunk/rtpproto_8c.html#a592f4a5e13fd709e6e0bc93a3131612d)(1|
|----|----|

按照指定格式将参数打印到足够大的自动分配缓冲区中。

它类似于GNU asprintf（）。


参数：fmt：printf兼容的格式字符串，指定如何使用以下参数。

返回：分配的字符串

注意：你必须用av_free（）自己释放字符串。





|char char *|[av_d2str](https://ffmpeg.org/doxygen/trunk/group__lavu__string.html#ga45d0395a448bc186baf081933ea816c9) (double d)|
|----|----|

将数字转换为av_malloced字符串。





|char *|[av_get_token](https://ffmpeg.org/doxygen/trunk/group__lavu__string.html#ga7b438169b430f6edf4f4451d867a9218) (const char **[buf](https://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#a5bc5fa69bee375df074734a2c4858604), const char *term)|
|----|----|

取消对给定的字符串进行过滤，直到非转义的终止字符为止，并返回与未转义字符串对应的令牌。

正常的\和'转义是支持的。 领先和尾随的空白被删除，除非它们以“\”或“封闭在”之间被转义。


参数：buf：要解析的缓冲区，buf将被更新为指向终止字符

   term：终止字符的0个终止的列表

返回：在分配失败的情况下，用户必须av_freed的malloced unescaped字符串为NULL





|char *|[av_strtok](https://ffmpeg.org/doxygen/trunk/group__lavu__string.html#ga3d4270f4ed651c414e2850090b9b77c4) (char *[s](https://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#ab87f55bd0280d90925050a4188c14ab5), const char *delim, char **saveptr)|
|----|----|

将字符串拆分成若干令牌，可以通过连续调用av_strtok（）来访问。

令牌被定义为不属于在delim中指定的集合的字符序列。


在第一次调用av_strtok（）时，s应指向要解析的字符串，并忽略saveptr的值。 在后续调用中，s应该为NULL，并且saveptr应该是从前一次调用开始不变的。


此函数与POSIX.1中定义的strtok_r（）类似。


参数：s：要解析的字符串可能为NULL

   delim：0终止的令牌分隔符列表，必须为非NULL

   saveptr：用户提供的指针指向av_strtok（）所需的存储信息，以继续扫描相同的字符串。 saveptr被更新为指向第一个分隔符之后的下一个字符，如果字符串被终止，则为NULL

返回：找到的令牌，或者当没有找到令牌时为NULL





|static [av_const](https://ffmpeg.org/doxygen/trunk/attributes_8h.html#a9eaead3cde73464fd66bab4cef9307ad)[int](https://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_isdigit](https://ffmpeg.org/doxygen/trunk/group__lavu__string.html#ga54f9d8cb6e7f411fb145dd6ade19e939) ([int](https://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[c](https://ffmpeg.org/doxygen/trunk/target__dec__fuzzer_8c.html#acfe27e61edea06412363421f1a013e81))|
|----|----|

与ASCII isdigit的区域无关转换。





|static [av_const](https://ffmpeg.org/doxygen/trunk/attributes_8h.html#a9eaead3cde73464fd66bab4cef9307ad)[int](https://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_isgraph](https://ffmpeg.org/doxygen/trunk/group__lavu__string.html#ga91dbda7570409baeb62265fa5f425039) ([int](https://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[c](https://ffmpeg.org/doxygen/trunk/target__dec__fuzzer_8c.html#acfe27e61edea06412363421f1a013e81))|
|----|----|

ASCII地图区域设置独立转换。





|static [av_const](https://ffmpeg.org/doxygen/trunk/attributes_8h.html#a9eaead3cde73464fd66bab4cef9307ad)[int](https://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_isspace](https://ffmpeg.org/doxygen/trunk/group__lavu__string.html#ga96490acc8dd8d85f2ee43bc605661ffa) ([int](https://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[c](https://ffmpeg.org/doxygen/trunk/target__dec__fuzzer_8c.html#acfe27e61edea06412363421f1a013e81))|
|----|----|

与ASCII isspace的区域无关转换。





|static [av_const](https://ffmpeg.org/doxygen/trunk/attributes_8h.html#a9eaead3cde73464fd66bab4cef9307ad)[int](https://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_toupper](https://ffmpeg.org/doxygen/trunk/group__lavu__string.html#ga9c20e01cbc94a7c9996a8ce93de046b0) ([int](https://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[c](https://ffmpeg.org/doxygen/trunk/target__dec__fuzzer_8c.html#acfe27e61edea06412363421f1a013e81))|
|----|----|

ASCII字符转换为大写字母区域设置。





|static [av_const](https://ffmpeg.org/doxygen/trunk/attributes_8h.html#a9eaead3cde73464fd66bab4cef9307ad)[int](https://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_tolower](https://ffmpeg.org/doxygen/trunk/group__lavu__string.html#ga4586526cc16bff30163dee592ed2897c) ([int](https://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[c](https://ffmpeg.org/doxygen/trunk/target__dec__fuzzer_8c.html#acfe27e61edea06412363421f1a013e81))|
|----|----|

区域设置独立的ASCII字符转换为小写。





|static [av_const](https://ffmpeg.org/doxygen/trunk/attributes_8h.html#a9eaead3cde73464fd66bab4cef9307ad)[int](https://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_isxdigit](https://ffmpeg.org/doxygen/trunk/group__lavu__string.html#ga644b6231c0c43b9e2925db980f6bfa81) ([int](https://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[c](https://ffmpeg.org/doxygen/trunk/target__dec__fuzzer_8c.html#acfe27e61edea06412363421f1a013e81))|
|----|----|

ASCII区域设置独立转换isxdigit。





|[int](https://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_strcasecmp](https://ffmpeg.org/doxygen/trunk/group__lavu__string.html#ga4c60b7d4ce023a6466bc033e31c5349c) (const char *[a](https://ffmpeg.org/doxygen/trunk/h264pred__template_8c.html#a4124bc0a9335c27f086f24ba207a4912), const char *[b](https://ffmpeg.org/doxygen/trunk/vf__curves_8c.html#a0a667482238bcca3ac201c1cd09c5072))|
|----|----|

不区分大小写区分大小写。

注意：这意味着只有ASCII范围字符不区分大小写





|[int](https://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_strncasecmp](https://ffmpeg.org/doxygen/trunk/group__lavu__string.html#gaea8a7c8c2832c11e753fbfc0d1464970) (const char *[a](https://ffmpeg.org/doxygen/trunk/h264pred__template_8c.html#a4124bc0a9335c27f086f24ba207a4912), const char *[b](https://ffmpeg.org/doxygen/trunk/vf__curves_8c.html#a0a667482238bcca3ac201c1cd09c5072), size_t [n](https://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#a76f11d9a0a47b94f72c2d0e77fb32240))|
|----|----|

不区分大小写区分大小写。

注意：这意味着只有ASCII范围字符不区分大小写





|char *|[av_strireplace](https://ffmpeg.org/doxygen/trunk/group__lavu__string.html#gaf67c5971eb2a00a02286316d36a88d73) (const char *str, const char *[from](https://ffmpeg.org/doxygen/trunk/libavcodec_2webvttdec_8c.html#af9f143e62cdad4afb8a53a21b8b8fcfd), const char *[to](https://ffmpeg.org/doxygen/trunk/libavcodec_2webvttdec_8c.html#a5d467a63df9ff99d33d6749038f6f421))|
|----|----|

与区域设置无关的字符串替换。

注意：这意味着只有ASCII范围字符被替换





|const char *|[av_basename](https://ffmpeg.org/doxygen/trunk/group__lavu__string.html#ga1fe5db82a1f7639434fc01bf83919096) (const char *path)|
|----|----|

线程安全基本名称。

参数：path：路径，在DOS上，\和/都被认为是分隔符。

返回：指向basename子字符串的指针。





|const char *|[av_dirname](https://ffmpeg.org/doxygen/trunk/group__lavu__string.html#gac50facc4ebec4a3a4a06c058a71bc027) (char *path)|
|----|----|

线程安全目录。

路径，在DOS上，\和/都被认为是分隔符。


返回：带有分隔符的路径被字符串终止符或“。”替换。

注意：该功能可能会改变输入字符串。





|[int](https://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_match_name](https://ffmpeg.org/doxygen/trunk/group__lavu__string.html#ga5883fa1071129810bccfad9b3e779371) (const char *[name](https://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a09d548d751fcf0b9b67ea230712ae06c), const char *names)|
|----|----|

以逗号分隔的名称列表匹配名称的实例。

从名称列表的开始到结束检查列表条目，第一个匹配结束进一步处理。 如果以' - '为前缀的条目匹配，则返回0。 “ALL”列表项被认为与所有名称相匹配。


参数：name：要找的名字。

   names：名字列表

返回：1表示匹配，偶表示不匹配





|char *|[av_append_path_component](https://ffmpeg.org/doxygen/trunk/group__lavu__string.html#ga7a948df0bd80a22f3fc4a267c4ef08c1) (const char *path, const char *component)|
|----|----|

将路径组件附加到现有路径。

路径分隔符'/'位于需要的位置之间。 必须使用av_free（）来释放所得到的字符串。


参数：path：基本路径

component:要附加的组件

返回：新路径或NULL错误。





|[av_warn_unused_result](https://ffmpeg.org/doxygen/trunk/attributes_8h.html#a93f17e0f0b53a66e7b5eceb3dff2918f)[int](https://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_escape](https://ffmpeg.org/doxygen/trunk/group__lavu__string.html#ga3c5e00d3f0575f2a3858f837656c08e9) (char **dst, const char *[src](https://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#a67caf60f8ec76e0adc7b693bfb651b66), const char *special_chars, enum [AVEscapeMode](https://ffmpeg.org/doxygen/trunk/group__lavu__string.html#gaef885958293f07153383f99db573ff5a)[mode](https://ffmpeg.org/doxygen/trunk/ebur128_8h.html#a1a6b6fb557d8d37d59700faf4e4c9167), [int](https://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[flags](https://ffmpeg.org/doxygen/trunk/log_8c.html#ac8bf36fe0577cba66bccda3a6f7e80a4))|
|----|----|

在src中转义字符串，并将转义的字符串放在* dst中分配的字符串中，该字符串必须与av_free（）一起释放。

参数：dst：指向分配字符串的指针

   src：字符串要转义，必须是非NULL

special_chars：包含需要转义的特殊字符的字符串可以为NULL

   mode：逃生模式使用，请参阅AV_ESCAPE_MODE_ *宏。 模式的任何未知值将被视为等同于AV_ESCAPE_MODE_BACKSLASH，但是此行为可能会更改，恕不另行通知。

   flags：控制如何转义的标志，请参阅AV_ESCAPE_FLAG_宏

返回：分配的字符串的长度，或者错误的情况下的负错误代码





|[av_warn_unused_result](https://ffmpeg.org/doxygen/trunk/attributes_8h.html#a93f17e0f0b53a66e7b5eceb3dff2918f)[int](https://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_utf8_decode](https://ffmpeg.org/doxygen/trunk/group__lavu__string.html#gac6e5b8d7ba04ac2023e825222c36bee3) ([int32_t](https://ffmpeg.org/doxygen/trunk/libswresample_2audioconvert_8c.html#afd12020da5a235dfcf0c3c748fb5baed) *codep, const [uint8_t](https://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) **bufp, const [uint8_t](https://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) *buf_end, unsigned [int](https://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[flags](https://ffmpeg.org/doxygen/trunk/log_8c.html#ac8bf36fe0577cba66bccda3a6f7e80a4))|
|----|----|

从* buf中的缓冲区中读取和解码单个UTF-8代码点（字符），并更新* buf以指向要解码的下一个字节。

在无效字节序列的情况下，指针将被更新为无效序列后的下一个字节，并且该函数将返回错误代码。


根据指定的标志，解码的代码点不属于有效范围时，功能也将失败。


注意：对于与速度相关的代码，可能会优先考虑使用GET_UTF8（）。

参数：codep：指针用于返回解析的代码，以防成功。 即使在范围检查失败的情况下，也会设置* codep中的值。

bufp：指向地址的第一个字节的序列进行解码，由该函数更新为指向下一个字节后的解码序列

buf_end：指向缓冲区的结尾，指向缓冲区中最后一个的下一个字节。 这用于避免缓冲区覆盖（在缓冲区末尾有未完成的UTF-8序列的情况下）。

    flags：一个AV_UTF8_FLAG_ *标志的集合

返回：> = 0，如果序列成功读取，则在无效序列的情况下为负值





|[int](https://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_match_list](https://ffmpeg.org/doxygen/trunk/group__lavu__string.html#ga9a6eb11f2b4053d6bba95d8da156da2d) (const char *[name](https://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a09d548d751fcf0b9b67ea230712ae06c), const char *list, char separator)|
|----|----|

检查名称是否在列表中。

返回：0，如果没有找到，或者在列表中找到的基于1的索引。



