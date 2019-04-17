# FFMPEG学习【ffplay工具】 - 一世豁然的专栏 - CSDN博客





2017年08月05日 20:29:13[一世豁然](https://me.csdn.net/Explorer_day)阅读数：506








一、概要

ffplay [options] [input_url]








二、描述

FFplay是使用FFmpeg库和SDL库的非常简单便携的媒体播放器。 它主要用作各种FFmpeg API的测试平台。








三、选项

所有数值选项（如果没有另外规定）接受一个表示数字的字符串作为输入，后面可以跟着SI单位前缀之一，例如：'K'，'M'或'G'。





如果将“i”附加到SI单元前缀，则完整前缀将被解释为二进制倍数的单位前缀，其基于1024的幂而不是1000的幂。将“B”附加到SI单元前缀将乘以 值为8.这允许使用例如：'KB'，'MiB'，'G'和'B'作为数字后缀。





不带参数的选项是布尔选项，并将相应的值设置为true。 可以将选项名称前缀为“no”，将其设置为false。 例如使用“-nofoo”会将名称为“foo”的布尔选项设置为false。





一）、流说明符

每个流可以应用一些选项。 比特率或编解码器。 流说明符用于精确指定给定选项所属的流。





流说明符是通常附加到选项名称并由冒号分隔的字符串。 例如。 -codec：a：1 ac3包含与第二个音频流匹配的a：1流说明符。 因此，它将为第二个音频流选择ac3编解码器。





流说明符可以匹配多个流，以便将该选项应用于所有流。 例如。 -b中的流说明符：128k匹配所有音频流。





空的流说明符匹配所有流。 例如，-codec副本或-codec：copy将复制所有的流，而不需要重新编码。





流说明符的可能形式有：


***stream_index***


使用此索引匹配流。例如。 -threads：1 4将第二个流的线程数设置为4。





***stream_type[:stream_index]***


stream_type是以下之一：视频为'v'或'V'，音频'a'，字幕'''，数据'd'和附件't'。 'v'匹配所有视频流，'V'仅匹配未附加图片，视频缩略图或封面艺术的视频流。如果给出stream_index，则它匹配此类型的流号stream_index。否则，它将匹配此类型的所有流。





***p:program_id[:stream_index]***


如果给出了stream_index，那么它将使用id program_id与程序中具有数字stream_index的流进行匹配。否则，它会匹配程序中的所有流。





***#stream_id or i:stream_id***


通过流ID匹配流（例如MPEG-TS容器中的PID）。





***m:key[:value]***


使用具有指定值的元数据标签键匹配流。如果没有给出值，则将包含给定标签的流与任何值进行匹配。





***u***

匹配具有可用配置的流，必须定义编解码器，并且必须存在视频维度或音频采样率等基本信息。





请注意，在ffmpeg中，元数据的匹配将仅适用于输入文件。








二）、一般选项

这些选项在ff *工具之间共享。





***-L***


显示许可证。





***-h, -?, -help, --help [arg]***


显示帮助。可以指定可选参数来打印有关特定项目的帮助。如果没有指定参数，则只显示基本（非高级）工具选项。


参数的可能值为：


long：打印高级工具选项以及基本工具选项。

full：打印选项的完整列表，包括编码器，解码器，分离器，复用器，过滤器等的共享和私有选项。

decoder=decoder_name：打印名为decode_name的解码器的详细信息。使用-decoders选项获取所有解码器的列表。

encoder=encoder_name：打印有关编码器的编码器名称的详细信息。使用-encoders选项获取所有编码器的列表。

demuxer=demuxer_name：打印有关Demuxer_name的复制器的详细信息。使用-formats选项来获取所有分割器和多路复用器的列表。

muxer=muxer_name：打印有关muxer_name的muxer的详细信息。使用-formats选项获取所有muxers和demuxers的列表。

filter=filter_name：打印有关过滤器名称filter_name的详细信息。使用-filters选项来获取所有过滤器的列表。




***-version***


显示版本。





***-formats***


显示可用的格式（包括设备）。





***-demuxers***


显示可用的分离器。





***-muxers***


显示可用的复用器。





***-devices***


显示可用的设备。





***-codecs***


显示libavcodec已知的所有编解码器。





请注意，本文档中使用术语“编解码器”作为更准确称为媒体比特流格式的快捷方式。





***-decoders***


显示可用的解码器。





***-encoders***


显示所有可用的编码器。





***-bsfs***


显示可用的位流过滤器





***-protocols***


显示可用的协议。





***-filters***


显示可用的libavfilter过滤器。





***-pix_fmts***


显示可用的像素格式。





***-sample_fmts***


显示可用的样本格式。





***-colors***


显示识别的颜色名称。





***-sources device[,opt1=val1[,opt2=val2]...]***


显示输入设备的自动检测源。 某些设备可能会提供不能自动检测的依赖于系统的源名称。 返回的列表不能被假定为始终完成。



`ffmpeg -sources pulse,server=192.168.0.4`
***-sinks device[,opt1=val1[,opt2=val2]...]***

显示输出设备的自动检测的接收器。 某些设备可能会提供不能自动检测的系统相关的接收器名称。 返回的列表不能被假定为始终完成。



`ffmpeg -sinks pulse,server=192.168.0.4`
***-loglevel [repeat+]loglevel | -v [repeat+]loglevel***

设置库使用的日志记录级别。 添加“repeat +”表示重复的日志输出不应该被压缩到第一行，而“Last message repeat n times”行将被省略。 “重复”也可以单独使用。 如果单独使用“重复”，并且没有先前的日志级别，则将使用默认日志级别。 如果给出了多个loglevel参数，使用'repeat'不会改变loglevel。 loglevel是一个字符串或包含以下值之一的数字：


‘quiet, -8’：根本不显示; 安静。


‘panic, 0’：只显示可能导致进程崩溃的致命错误，例如断言失败。 这目前不用于任何东西。

‘fatal, 8’：只显示致命错误。 这些都是错误之后，过程绝对不能继续。

‘error, 16’：显示所有错误，包括可以从中恢复的错误。

‘warning, 24’：显示所有警告和错误。 将显示与可能不正确或意外事件相关的任何消息。

‘info, 32’：在处理过程中显示信息。 这是除了警告和错误之外。 这是默认值。

‘verbose, 40’：与信息相同，除了更详细。

‘debug, 48’：显示所有内容，包括调试信息。

‘trace, 56’

默认情况下，程序记录到stderr。 如果终端支持着色，则使用颜色来标记错误和警告。 可以禁用日志着色设置环境变量AV_LOG_FORCE_NOCOLOR或NO_COLOR，

或者可以强制设置环境变量AV_LOG_FORCE_COLOR。 环境变量NO_COLOR的使用已被弃用，将在将来的FFmpeg版本中被删除。




***-report***


将完整的命令行和控制台输出转储到当前目录中名为program-YYYYMMDD-HHMMSS.log的文件中。 此文件可用于错误报告。 它也意味着-loglevel verbose。





将环境变量FFREPORT设置为任何值具有相同的效果。 如果值为'：' - 分离的key =值序列，这些选项将影响报告; 如果选项值包含特殊字符或选项分隔符“：”（请参阅

ffmpeg-utils手册中的“引用和转义”部分），则必须转义选项值。





可以识别以下选项：


file：设置要用于报告的文件名; ％p被扩展为程序的名称，％t被展开为时间戳，%%被扩展为普通的％

level：使用数值设置日志详细程度级别（请参见-loglevel）。




例如，要使用日志级别32（日志级别信息的别名）将报告输出到名为ffreport.log的文件：



`FFREPORT=file=ffreport.log:level=32 ffmpeg -i input output`

解析环境变量的错误并不致命，也不会在报表中显示。




***-hide_banner***


禁止打印横幅。


所有FFmpeg工具通常会显示版权声明，构建选项和库版本。 此选项可用于禁止打印此信息。





***-cpuflags flags (global)***


允许设置和清除cpu标志。 此选项用于测试。 除非你知道你在做什么，否则不要使用它




```
ffmpeg -cpuflags -sse+mmx ...
ffmpeg -cpuflags mmx ...
ffmpeg -cpuflags 0 ...
```


此选项的可能标志是：

‘x86’
‘mmx’
‘mmxext’
‘sse’
‘sse2’
‘sse2slow’
‘sse3’
‘sse3slow’
‘ssse3’
‘atom’
‘sse4.1’
‘sse4.2’
‘avx’
‘avx2’
‘xop’
‘fma3’
‘fma4’
‘3dnow’
‘3dnowext’
‘bmi1’
‘bmi2’
‘cmov’



‘ARM’
‘armv5te’
‘armv6’
‘armv6t2’
‘vfp’
‘vfpv3’
‘neon’
‘setend’



‘AArch64’
‘armv8’
‘vfp’
‘neon’



‘PowerPC’
‘altivec’



‘Specific Processors’
‘pentium2’
‘pentium3’
‘pentium4’
‘k6’
‘k62’
‘athlon’
‘athlonxp’
‘k8’





***-opencl_bench***


此选项用于对所有可用的OpenCL设备进行基准测试并打印结果。 仅当使用--enable-opencl编译FFmpeg时，此选项才可用。





当FFmpeg配置为--enable-opencl时，通过-opencl_options设置全局OpenCL上下文的选项。 有关支持的选项的完整列表，请参阅ffmpeg-utils手册中的“OpenCL Options”一节。

除此之外，这些选项包括能够选择特定的平台和设备来运行OpenCL代码。 默认情况下，FFmpeg将在第一个平台的第一个设备上运行。 虽然全局OpenCL上下文的选项为用

户选择所选择的OpenCL设备提供了灵活性，但大多数用户可能希望为其系统选择最快的OpenCL设备。





此选项通过识别用户系统的适当设备来帮助选择最有效的配置。 所有内置基准测试都运行在所有OpenCL设备上，并对每个设备进行性能测试。 结果列表中的设备将根据其性能

进行排序，其中首先列出最快的设备。 用户随后可以使用通过-opencl_options认为最合适的设备调用ffmpeg，以获得OpenCL加速代码的最佳性能。





使用最快的OpenCL设备的典型用法包括以下步骤。


运行命令：



`ffmpeg -opencl_bench`

记下列表中第一个即最快设备的平台ID（pidx）和设备ID（didx）。 使用以下命令选择平台和设备：

ffmpeg -opencl_options platform_idx=pidx:device_idx=didx ...





***-opencl_options options (global)***


设置OpenCL环境选项。 仅当使用--enable-opencl编译FFmpeg时，此选项才可用。


选项必须是以“：”分隔的key = value选项对的列表。 有关支持的选项列表，请参阅ffmpeg-utils手册中的“OpenCL Options”一节。








三）、音视频选项

这些选项由libavformat，libavdevice和libavcodec库直接提供。 要查看可用的AVOptions列表，请使用-help选项。 它们分为两类：


***generic***


可以为任何容器，编解码器或设备设置这些选项。 通用选项在容器/设备的AVFormatContext选项下以及编解码器的AVCodecContext选项下列出。





***private***


这些选项特定于给定的容器，设备或编解码器。 私有选项列在其相应的容器/设备/编解码器下。





例如，要将ID3v2.3标题而不是默认ID3v2.4写入MP3文件，请使用MP3 muxer的id3v2_version私有选项：



`ffmpeg -i input.flac -id3v2_version 3 out.mp3`

所有编解码器AVOptions都是每个流，因此应该附加一个流说明符。

注意：-nooption语法不能用于布尔AVOptions，请使用-option 0 / -option 1。


注意：通过将v / a / s添加到选项名称来指定每个流AVOptions的旧的未记录方式现在已经过时，将很快被删除。








四）、主要选项

***-x width***


强制显示宽度。




***-y height***


强制显示高度。




***-s size***


设置不包含像原始YUV的帧大小的标题的视频所需的帧大小（WxH或缩写）。此选项已被弃用，有利于私有选项，请尝试使用-video_size。




***-fs***


以全屏模式开始。





***-an***


禁用音频。





***-vn***


禁用视频。





***-sn***


禁用字幕





***-ss pos***


寻求位置请注意，在大多数格式中，不可能精确地查找，所以ffplay将寻求最近的寻找点到位置。


pos必须是时间段规范，请参阅ffmpeg-utils（1）手册中的（ffmpeg-utils）时间段。





***-t duration***


播放音频/视频的持续时间秒。


持续时间必须是持续时间规范，请参阅（ffmpeg-utils）ffmpeg-utils（1）手册中的持续时间部分。





***-bytes***


寻求字节。





***-nodisp***


禁用图形显示。





***-noborder***


无边界的窗口。





***-volume***


设置启动量。 0表示静音，100表示​​没有音量减小或放大。负值被视为0，高于100的值被视为100。





***-f fmt***


强制格式





***-window_title title***


设置窗口标题（默认为输入文件名）。





***-loop number***


循环播放电影<number>次。 0表示永远。





***-showmode mode***


设置显示模式使用。 模式的可用值为：


‘0, video’：显示视频

‘1, waves’：显示音频波

'2，rdft'：使用RDFT（（逆）实数离散傅里叶变换）显示音频带

默认值为“video”，如果视频不存在或无法播放，“rdft”将被自动选择。


您可以通过按键w交互地循环浏览可用的显示模式。





***-vf filtergraph***


创建由过滤器指定的过滤器，并使用它过滤视频流。


滤波器是要应用于流的滤波器的描述，并且必须具有单个视频输入和单个视频输出。 在过滤器中，输入与标签相关联，输出到标签输出。 有关filtergraph语法的更多信息，

请参阅ffmpeg-filters手册。


您可以多次指定此参数，并通过按w键循环显示指定的滤光片以及显示模式。





***-af filtergraph***


滤光片是要应用于输入音频的滤光片的说明。 使用选项“-filters”来显示所有可用的过滤器（包括源和接收器）。





***-i input_url***


读取input_url。








五）、高级选项

***-pix_fmt format***


设置像素格式。此选项已被弃用，有利于私有选项，请尝试使用-pixel_format。





***-stats***


打印多个播放统计信息，特别显示流持续时间，编解码器参数，流中的当前位置和音频/视频同步漂移。默认情况下，要明确禁用它，您需要指定-nostat。





***-fast***


非规范兼容优化。





***-genpts***


生成pts





***-sync type***


将主时钟设置为音频（类型=音频），视频（类型=视频）或外部（type = ext）。默认是音频。主时钟用于控制音视频同步。大多数媒体播放器使用音频作为主时钟，

但在某些情况下（流媒体或高质量广播），有必要改变它。此选项主要用于调试目的。





***-ast audio_stream_specifier***


使用给定的流说明符选择所需的音频流。流说明符在Stream说明符一章中有描述。如果未指定此选项，则在已选择的视频流的程序中选择“最佳”音频流。





***-vst video_stream_specifier***


使用给定的流说明符选择所需的视频流。流说明符在Stream说明符一章中有描述。如果未指定此选项，则选择“最佳”视频流。





***-sst subtitle_stream_specifier***


使用给定的流说明符选择所需的字幕流。流说明符在Stream说明符一章中有描述。如果未指定此选项，则在已选择的视频或音频流的程序中选择“最佳”字幕流。





***-autoexit***


视频播放完毕后退出





***-exitonkeydown***


如果按任何键退出。





***-exitonmousedown***


如果按任何鼠标按钮退出。





***-codec：media_specifier codec_name***


强制由media_specifier标识的流的特定解码器实现，它可以采用值a（音频），v（视频）和s字幕。





***-acodec codec_name***


强制特定的音频解码器。





***-vcodec codec_name***


强制特定的视频解码器。





***scodec codec_name***


强制一个特定的字幕解码器。





***-autorotate***


根据文件元数据自动旋转视频。默认情况下启用，使用-noautorotate禁用它。





***-framedrop***


如果视频不同步，请丢弃视频帧。如果主时钟未设置为视频，则默认为启用。使用此选项为所有主时钟源启用帧丢弃，请使用-noframedrop禁用它。





***-infbuf***


不要限制输入缓冲区大小，尽快从输入读取尽可能多的数据。默认情况下，实时流启用，如果未及时读取数据，则可能会丢弃数据。使用此选项为所有输入启用无限缓冲区，

使用-noinfbuf禁用它。








六）、当播放时

***q, ESC***


放弃。





***f***


切换全屏。





***p, SPC***


暂停。





***m***


切换静音。





***9, 0***


分别减少和增加音量。





***/, ****


分别减少和增加音量。





***a***


在当前程序中循环音频通道。





***v***


循环视频通道。





***t***


在当前程序中循环字幕通道。





***c***


循环程序。





***w***


循环视频过滤器或显示模式。





***s***


一步到下一帧。

暂停如果流尚未暂停，请转到下一个视频帧并暂停。





***left/right***


向后/向前寻求10秒钟。





***down/up***


向后/向前寻求1分钟。




***right mouse click***


寻求对应于宽度分数的文件百分比。




***left mouse double-click***


切换全屏。




