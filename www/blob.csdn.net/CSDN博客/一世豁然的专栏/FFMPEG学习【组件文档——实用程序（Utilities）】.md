# FFMPEG学习【组件文档——实用程序（Utilities）】 - 一世豁然的专栏 - CSDN博客





2017年08月30日 10:53:40[一世豁然](https://me.csdn.net/Explorer_day)阅读数：457








一、描述

本文档描述了libavutil库提供的一些通用功能和实用程序。








二、语法

本节介绍了FFmpeg库和工具所使用的语法和格式。





一）、引用和转义

FFmpeg采用以下引用和转义机制，除非明确指定。 应用以下规则：


1）、'''和'\'是特殊字符（分别用于引用和转义）。 除了它们之外，还可能有其他特殊字符取决于使用转义和引用的具体语法。

2）、一个特殊的字符通过前缀“\”进行转义。

3）、“'”之间的所有字符都包含在解析的字符串中。 引用字符“'”本身不能引用，所以您可能需要关闭引用并将其退出。

4）、领先和尾随的空格，除非被转义或引用，将从被解析的字符串中删除。




请注意，您可能需要在使用命令行或脚本时添加第二级转义级别，这取决于所采用的shell语言的语法。





在libavutil / avstring.h中定义的函数av_get_token可用于根据上面定义的规则来解析引用或转义的令牌。





FFmpeg源代码树中的工具tools/ ffescape可用于自动引用或转义脚本中的字符串。





1、实例

1）、逃避字符**Crime d'Amour**包含“特殊字符：


`Crime d\'Amour`

2）、上面的字符串包含一个引号，所以'引用时需要转义它：


`'Crime d'\''Amour'`



3）、使用引用包括前导或尾随的空格：


`'  this string starts and ends with whitespaces  '`

4）、转义和引用可以混合在一起：
`' The string '\'string\'' is a string '`

5）、要包含文字'\'，您可以使用转义或引用：


`'c:\foo' can be written as c:\\foo`


二）、日期

接受的语法是：




```
[(YYYY-MM-DD|YYYYMMDD)[T|t| ]]((HH:MM:SS[.m...]]])|(HHMMSS[.m...]]]))[Z]
now
```


如果值为“now”，则需要当前时间。




时间是本地时间，除非附加Z，在这种情况下，它被解释为UTC。 如果未指定年月日零件，则需要当前的年月日。





三）、持续时间

有两种接受的表达时间的语法。



`[-][HH:]MM:SS[.m...]`HH表示小时数，MM最多为2位数的分钟数，SS表示最多2位数的秒数。 最后的m表示SS的十进制值。

或者


`[-]S+[.m...]`S表示秒数，带有可选的小数部分m。

在两个表达式中，可选的“ - ”表示负的持续时间。




1、实例

以下示例都是有效的持续时间：


“55”


55秒

‘12:03:45’


12小时03分45秒


‘23.189’


23.189秒





四）、视频大小

指定源视频的大小，它可以是widthxheight格式的字符串或大小缩写的名称。





确认以下缩写：


‘ntsc’

720x480



‘pal’

720x576



‘qntsc’

352x240



‘qpal’

352x288



‘sntsc’

640x480



‘spal’

768x576



‘film’

352x240



‘ntsc-film’

352x240



‘sqcif’

128x96



‘qcif’

176x144



‘cif’

352x288



‘4cif’

704x576



‘16cif’

1408x1152



‘qqvga’

160x120



‘qvga’

320x240



‘vga’

640x480



‘svga’

800x600



‘xga’

1024x768



‘uxga’

1600x1200



‘qxga’

2048x1536



‘sxga’

1280x1024



‘qsxga’

2560x2048



‘hsxga’

5120x4096



‘wvga’

852x480



‘wxga’

1366x768



‘wsxga’

1600x1024



‘wuxga’

1920x1200



‘woxga’

2560x1600



‘wqsxga’

3200x2048



‘wquxga’

3840x2400



‘whsxga’

6400x4096



‘whuxga’

7680x4800



‘cga’

320x200



‘ega’

640x350



‘hd480’

852x480



‘hd720’

1280x720



‘hd1080’

1920x1080



‘2k’

2048x1080



‘2kflat’

1998x1080



‘2kscope’

2048x858



‘4k’

4096x2160



‘4kflat’

3996x2160



‘4kscope’

4096x1716



‘nhd’

640x360



‘hqvga’

240x160



‘wqvga’

400x240



‘fwqvga’

432x240



‘hvga’

480x320



‘qhd’

960x540



‘2kdci’

2048x1080



‘4kdci’

4096x2160



‘uhd2160’

3840x2160



‘uhd4320’

7680x4320








五）、视频率

指定视频的帧速率，表示为每秒生成的帧数。 它必须是格式为frame_rate_num / frame_rate_den的字符串，整数，浮点数或有效的视频帧速率缩写。





确认以下缩写：


‘ntsc’

30000/1001



‘pal’

25/1



‘qntsc’

30000/1001



‘qpal’

25/1



‘sntsc’

30000/1001



‘spal’

25/1



‘film’

24/1



‘ntsc-film’

24000/1001





六）、比率

一个比率可以表示为一个表达式，也可以用分子形式表示：分母。





请注意，无限（1/0）或负值的比率被认为是有效的，因此如果要排除这些值，则应检查返回的值。





未定义的值可以使用“0：0”字符串表示。








七）、颜色

它可以是下面定义的颜色的名称（不区分大小写的匹配）或[0x |＃] RRGGBB [AA]序列，可能后跟@和表示alpha分量的字符串。





alpha组件可以是由“0x”后跟十六进制数或0.0和1.0之间的十进制数组成的字符串，表示不透明度值（'0x00'或'0.0'表示完全透明，'0xff'或'1.0' 完全不透明）。 如果未指定alpha分量，则假定为'0xff'。





字符串“随机”将导致随机颜色。





以下名称的颜色被识别：


‘AliceBlue’

0xF0F8FF



‘AntiqueWhite’

0xFAEBD7



‘Aqua’

0x00FFFF



‘Aquamarine’

0x7FFFD4



‘Azure’

0xF0FFFF



‘Beige’

0xF5F5DC



‘Bisque’

0xFFE4C4



‘Black’

0x000000



‘BlanchedAlmond’

0xFFEBCD



‘Blue’

0x0000FF



‘BlueViolet’

0x8A2BE2



‘Brown’

0xA52A2A



‘BurlyWood’

0xDEB887



‘CadetBlue’

0x5F9EA0



‘Chartreuse’

0x7FFF00



‘Chocolate’

0xD2691E



‘Coral’

0xFF7F50



‘CornflowerBlue’

0x6495ED



‘Cornsilk’

0xFFF8DC



‘Crimson’

0xDC143C



‘Cyan’

0x00FFFF



‘DarkBlue’

0x00008B



‘DarkCyan’

0x008B8B



‘DarkGoldenRod’

0xB8860B



‘DarkGray’

0xA9A9A9



‘DarkGreen’

0x006400



‘DarkKhaki’

0xBDB76B



‘DarkMagenta’

0x8B008B



‘DarkOliveGreen’

0x556B2F



‘Darkorange’

0xFF8C00



‘DarkOrchid’

0x9932CC



‘DarkRed’

0x8B0000



‘DarkSalmon’

0xE9967A



‘DarkSeaGreen’

0x8FBC8F



‘DarkSlateBlue’

0x483D8B



‘DarkSlateGray’

0x2F4F4F



‘DarkTurquoise’

0x00CED1



‘DarkViolet’

0x9400D3



‘DeepPink’

0xFF1493



‘DeepSkyBlue’

0x00BFFF



‘DimGray’

0x696969



‘DodgerBlue’

0x1E90FF



‘FireBrick’

0xB22222



‘FloralWhite’

0xFFFAF0



‘ForestGreen’

0x228B22



‘Fuchsia’

0xFF00FF



‘Gainsboro’

0xDCDCDC



‘GhostWhite’

0xF8F8FF



‘Gold’

0xFFD700



‘GoldenRod’

0xDAA520



‘Gray’

0x808080



‘Green’

0x008000



‘GreenYellow’

0xADFF2F



‘HoneyDew’

0xF0FFF0



‘HotPink’

0xFF69B4



‘IndianRed’

0xCD5C5C



‘Indigo’

0x4B0082



‘Ivory’

0xFFFFF0



‘Khaki’

0xF0E68C



‘Lavender’

0xE6E6FA



‘LavenderBlush’

0xFFF0F5



‘LawnGreen’

0x7CFC00



‘LemonChiffon’

0xFFFACD



‘LightBlue’

0xADD8E6



‘LightCoral’

0xF08080



‘LightCyan’

0xE0FFFF



‘LightGoldenRodYellow’

0xFAFAD2



‘LightGreen’

0x90EE90



‘LightGrey’

0xD3D3D3



‘LightPink’

0xFFB6C1



‘LightSalmon’

0xFFA07A



‘LightSeaGreen’

0x20B2AA



‘LightSkyBlue’

0x87CEFA



‘LightSlateGray’

0x778899



‘LightSteelBlue’

0xB0C4DE



‘LightYellow’

0xFFFFE0



‘Lime’

0x00FF00



‘LimeGreen’

0x32CD32



‘Linen’

0xFAF0E6



‘Magenta’

0xFF00FF



‘Maroon’

0x800000



‘MediumAquaMarine’

0x66CDAA



‘MediumBlue’

0x0000CD



‘MediumOrchid’

0xBA55D3



‘MediumPurple’

0x9370D8



‘MediumSeaGreen’

0x3CB371



‘MediumSlateBlue’

0x7B68EE



‘MediumSpringGreen’

0x00FA9A



‘MediumTurquoise’

0x48D1CC



‘MediumVioletRed’

0xC71585



‘MidnightBlue’

0x191970



‘MintCream’

0xF5FFFA



‘MistyRose’

0xFFE4E1



‘Moccasin’

0xFFE4B5



‘NavajoWhite’

0xFFDEAD



‘Navy’

0x000080



‘OldLace’

0xFDF5E6



‘Olive’

0x808000



‘OliveDrab’

0x6B8E23



‘Orange’

0xFFA500



‘OrangeRed’

0xFF4500



‘Orchid’

0xDA70D6



‘PaleGoldenRod’

0xEEE8AA



‘PaleGreen’

0x98FB98



‘PaleTurquoise’

0xAFEEEE



‘PaleVioletRed’

0xD87093



‘PapayaWhip’

0xFFEFD5



‘PeachPuff’

0xFFDAB9



‘Peru’

0xCD853F



‘Pink’

0xFFC0CB



‘Plum’

0xDDA0DD



‘PowderBlue’

0xB0E0E6



‘Purple’

0x800080



‘Red’

0xFF0000



‘RosyBrown’

0xBC8F8F



‘RoyalBlue’

0x4169E1



‘SaddleBrown’

0x8B4513



‘Salmon’

0xFA8072



‘SandyBrown’

0xF4A460



‘SeaGreen’

0x2E8B57



‘SeaShell’

0xFFF5EE



‘Sienna’

0xA0522D



‘Silver’

0xC0C0C0



‘SkyBlue’

0x87CEEB



‘SlateBlue’

0x6A5ACD



‘SlateGray’

0x708090



‘Snow’

0xFFFAFA



‘SpringGreen’

0x00FF7F



‘SteelBlue’

0x4682B4



‘Tan’

0xD2B48C



‘Teal’

0x008080



‘Thistle’

0xD8BFD8



‘Tomato’

0xFF6347



‘Turquoise’

0x40E0D0



‘Violet’

0xEE82EE



‘Wheat’

0xF5DEB3



‘White’

0xFFFFFF



‘WhiteSmoke’

0xF5F5F5



‘Yellow’

0xFFFF00



‘YellowGreen’

0x9ACD32








八）、频道布局

频道布局指定多声道音频流中的频道的空间配置。 要指定一个通道布局，FFmpeg使用一个特殊的语法。





单个渠道由id标识，如下表所示：


“FL”

前左



“FR”

右前方



“FC”

前中心



“LFE”

低频率



“BL”

回到左边



“BR”

右后



“FLC”

前左中心



“FRC”

前右中心



'公元前'

后中心



“SL”

左边



“SR”

右边



“TC”

顶级中心



“TFL”

前左前



“TFC”

前排中心



“TFR”

前右前



“TBL”

顶部左后



“TBC”

顶部中心



“TBR”

顶部右后



“DL”

下混



“DR”

下混



“WL”

宽左



“WR”

宽的权利



“SDL”

环绕直接左



“SDR”

环绕直接右



“LFE2”

低频2





可以通过使用以下标识符来指定标准通道布局组合：


“单声道”

FC



'立体声'

FL+ FR



“2.1”

FL+ FR+ LFE



“3.0”

FL+ FR+ FC



“3.0（背面）”

FL+ FR+ BC



“4.0”

FL+ FR+ FC+ BC



“四”

FL+ FR+ BL+ BR



“四（侧）”

FL+ FR+ SL+ SR



“3.1”

FL+ FR+ FC+ LFE



“5.0”

FL+ FR+ FC+ BL+ BR



“5.0（侧）”

FL+ FR+ FC+ SL+ SR



“4.1”

FL+ FR+ FC+ LFE+ BC



“5.1”

FL+ FR+ FC+ LFE+ BL+ BR



“5.1（侧）”

FL+ FR+ FC+ LFE+ SL+ SR



“6.0”

FL+ FR+ FC+ BC+ SL+ SR



“6.0（前）”

FL+ FR+ FLC+ FRC+ SL+ SR



“六角形”

FL+ FR+ FC+ BL+ BR+ BC



“6.1”

FL+ FR+ FC+ LFE+ BC+ SL+ SR



“6.1”

FL+ FR+ FC+ LFE+ BL+ BR+ BC



“6.1（前）”

FL+ FR+ LFE+ FLC+ FRC+ SL+ SR



“7.0”

FL+ FR+ FC+ BL+ BR+ SL+ SR



“7.0（前）”

FL+ FR+ FC+ FLC+ FRC+ SL+ SR



“7.1”

FL+ FR+ FC+ LFE+ BL+ BR+ SL+ SR



“7.1（宽）”

FL+ FR+ FC+ LFE+ BL+ BR+ FLC+ FRC



“7.1（宽侧）”

FL+ FR+ FC+ LFE+ FLC+ FRC+ SL+ SR



“八角”

FL+ FR+ FC+ BL+ BR+ BC+ SL+ SR



“缩混”

DL+ DR





可以将自定义渠道布局指定为由“+”或“|”分隔的术语序列。 每个术语可以是：


1）、标准频道布局的名称（例如“mono”，“stereo”，“4.0”，“quad”，“5.0”等）

2）、单个频道的名称（例如“FL”，“FR”，“FC”，“LFE”等）

3）、多个通道，以十进制，后跟'c'，产生该通道数的默认通道布局（见函数av_get_default_channel_layout）。 请注意，并非所有通道计数都具有默认布局。

4）、多个通道，以十进制，后跟“C”，产生具有指定数量通道的未知通道布局。 请注意，并非所有通道布局规范字符串都支持未知通道布局。

5）、一个通道布局掩码，十六进制以“0x”开头（请参阅libavutil / channel_layout.h中的AV_CH_ *宏。




在libavutil第53版之前，尾部字符“c”指定多个通道是可选的，但现在是必需的，而通道布局掩码也可以指定为十进制数（当且仅当后面没有“c”或 “C”）。





另请参见libavutil / channel_layout.h中定义的函数av_get_channel_layout。








三、表达评估

在评估算术表达式时，FFmpeg使用内部公式求值器，通过libavutil / eval.h接口实现。





表达式可能包含一元的二进制运算符，常量和函数。





两个表达式expr1和expr2可以组合形成另一个表达式“expr1; expr2”。 依次对expr1和expr2进行求值，新表达式计算为expr2的值。





以下二进制运算符可用：+， - ，*，/，^。





以下一元运算符可用：+， - 。





以下功能可用：


ABS（x）的

计算x的绝对值。



ACOS（x）的

计算x的反余弦。



ASIN（x）的

计算x的反正弦。



ATAN（x）的

计算x的反正切。



atan2（x，y）

计算y / x的反正切的主值。



between（x，min，max）

如果x大于或等于min且小于或等于max，则返回1，否则返回0。



bitand（x，y）

bitor（x，y）

在x和y上计算按位和/或运算。


在执行按位操作之前，将x和y的评估结果转换为整数。


请注意，转换为整数和转换回浮点均可能失去精度。 注意大量的意外结果（通常为2 ^ 53及以上）。





ceil（表达式）

将表达式expr的值向上舍入到最接近的整数。例如，“ceil（1.5）”是“2.0”。



clip（x，min，max）

返回在最小和最大值之间剪切的x的值。



COS（x）的

计算x的余弦。



COSH（x）的

计算x的双曲余弦。



eq（x，y）

如果x和y相等，返回1，否则返回0。



EXP（x）的

计算x的指数（基数e，欧拉数）。



floor（expr）

将表达式expr的值向下舍入到最接近的整数。例如，“floor（-1.5）”是“-2.0”。



gauss（x）的

计算x的高斯函数，对应于exp（-x * x / 2）/ sqrt（2 * PI）。



gcd（x，y）

返回x和y的最大公约数。如果x和y都为0或者两者都小于零，则行为是未定义的。



gt（x，y）

如果x大于y，则返回1，否则返回0。



gte（x，y）

如果x大于或等于y，则返回1，否则返回0。



hypot（x，y）

该函数与C名称相同，它返回“sqrt（x * x + y * y）”，具有长度x和y的边的直角三角形的斜边长度，或点（x，y）与原点的距离。



if（x，y）

评估x，如果结果不为零，则返回y的评估结果，否则返回0。



if（x，y，z）

评估x，如果结果非零，返回y的评估结果，否则z的评估结果。



ifnot（x，y）

评估x，如果结果为零，则返回y的评估结果，否则返回0。



ifnot（x，y，z）

评估x，如果结果为零，则返回y的评估结果，否则为z的评估结果。



isinf（x）的

如果x为+/- INFINITY，返回1.0，否则返回0.0。



isnan（x）的

如果x为NAN则返回1.0，否则返回0.0。



LD（VAR）

使用数字var加载内部变量的值，该值先前存储有st（var，expr）。该函数返回加载的值。



lerp（x，y，z）

在x和y之间返回Z量的线性插值。



log(x)

计算x的自然对数。



lt（x，y）

如果x小于y，返回1，否则返回0。



lte（x，y）

如果x小于或等于y，则返回1，否则返回0。



max（x，y）

返回x和y之间的最大值。



min（x，y）

返回x和y之间的最小值。



mod（x，y）

计算x除以y的余数。



not(expr)

如果expr为零，返回1.0，否则返回0.0。



pow（x，y）

计算x升高的y的幂，它等于“（x）^（y）”。



print(t)

print(t, l)

用loglevel l打印表达式t的值。如果未指定l，则使用默认日志级别。返回打印表达式的值。

用日志级别l打印





random(x)

返回0.0到1.0之间的伪随机值。 x是将用于保存种子/状态的内部变量的索引。





root(expr, max)

在0..max间隔内找到一个输入值，其中由参数ld（0）表示的由expr表示的函数为0。


expr中的表达式必须表示连续的函数或结果未定义。


ld（0）用于表示函数输入值，这意味着表达式可以通过ld（0）访问的各种输入值进行多次评估。 当表达式求值为0时，将返回相应的输入值。





round(expr)

将表达式expr的值舍入到最接近的整数。 例如，“round（1.5）”是“2.0”。



sin（x）

计算x的正弦。



sinh（x）的

计算x的双曲正弦。



sqrt(expr)

计算expr的平方根。 这相当于“（expr）^。5”。



squish(x)

计算表达式1 /（1 + exp（4 * x））。



st（var，expr）

将表达式expr的值存储在内部变量中。 var指定要存储值的变量的数量，它的取值范围为0到9.该函数返回存储在内部变量中的值。 注意，变量当前不在表达式之间共享。



tan(x)

计算x的切线。



tanh（x）

计算x的双曲正切值。





taylor(expr, x)

taylor(expr, x, id)


给出表达式为0的函数的ld（id）导数的表达式，计算x的泰勒级数。


当系列不收敛时，结果是未定义的。


ld（id）用于表示expr中的派生顺序，这意味着给定的表达式将通过表达式可以通过ld（id）访问的各种输入值进行多次评估。 如果未指定id，则假定为0。


注意，当您的衍生工具在y而不是0时，可以使用taylor（expr，x-y）。





time（0）

以秒为单位返回当前（wallclock）时间。



TRUNC（expr）

将表达式expr的值向零舍入到最接近的整数。 例如，“trunc（-1.5）”是“-1.0”。



while(cond, expr)

在表达式cond为非零时评估表达式expr，并返回上一个expr评估的值，或者如果cond始终为false则返回NAN。





以下常数可用：


**PI**

单位盘的面积约3.14


**E**

exp（1）（欧拉数），约2.718


**PHI**

黄金比例（1 + sqrt（5））/ 2，约1.618





假设表达式被认为是“true”，如果它具有非零值，请注意：


* 像AND一样工作

+ 像OR一样工作




例如：



`if (A AND B) then C`相当于：
`if(A*B, C)`

在您的C代码中，您可以扩展一元和二进制函数的列表，并定义识别的常量，以便它们可用于您的表达式。




评估者还可以识别国际系统单位前缀。 如果在前缀之后附加“i”，则使用二进制前缀，它们基于1024的幂而不是1000的幂。“B”后缀将该值乘以8，并且可以在单位前缀后附加

或单独使用。 这允许使用例如“KB”，“MiB”，“G”和“B”作为后缀。





可用的国际系统前缀列表如下，指示相应的权力为10和2。


y

10^-24 / 2^-80



z

10^-21 / 2^-70



a

10^-18 / 2^-60



f

10^-15 / 2^-50



p

10^-12 / 2^-40



n

10^-9 / 2^-30



u

10^-6 / 2^-20



m

10^-3 / 2^-10



c

10^-2



d

10^-1



h

10^2



k

10^3 / 2^10



K

10^3 / 2^10



M

10^6 / 2^20



G

10^9 / 2^30



T

10^12 / 2^40



P

10^15 / 2^40



E

10^18 / 2^50



Z

10^21 / 2^60



Y

10^24 / 2^70








四、OpenCL选项

当FFmpeg配置为--enable-opencl时，可以设置全局OpenCL上下文的选项。





支持的选项列表如下：


build_options

设置用于编译注册内核的构建选项。

参见“OpenCL规范版本：1.2第5.6.4节”。



platform_idx

选择平台的索引来运行OpenCL代码。

指定的索引必须是可以使用ffmpeg -opencl_bench或av_opencl_get_device_list（）获取的设备列表中的索引之一。



device_idx

选择用于运行OpenCL代码的设备的索引。

指定的索引必须是可以使用ffmpeg -opencl_bench或av_opencl_get_device_list（）获取的设备列表中的索引之一。




