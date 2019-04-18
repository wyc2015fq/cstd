# FFMPEG学习【ffmpeg工具】 - 一世豁然的专栏 - CSDN博客





2017年08月05日 14:00:46[一世豁然](https://me.csdn.net/Explorer_day)阅读数：2237








一、概要


`ffmpeg [global_options] {[input_file_options] -i input_url} ... {[output_file_options] output_url} ...`











二、描述

**ffmpeg**是一个非常快的视频和音频转换器，也可以从现场音频/视频源获取。 它还可以在任意采样率之间进行转换，并使用高质量的多相滤波器即时调整视频大小。




**ffmpeg**从-i选项指定的任意数量的输入“文件”（可以是常规文件，管道，网络流，抓取设备等）读取，并写入任意数量的输出“文件”，其中 由纯输出url指定。 在命令行上发现的任何不能被解释为选项的内容都被认为是输出url。




原则上，每个输入或输出网址可以包含任何数量的不同类型的流（视频/音频/字幕/附件/数据）。 流的允许数量和/或类型可能受到容器格式的限制。 选择流从哪个输入到哪个输出是自动完成的或使用-map选项指定（参见流选择一章）。




要在选项中引用输入文件，您必须使用其索引（基于0）。 例如。 第一个输入文件为0，第二个为1等。类似地，文件中的流由其索引引用。 例如。 2：3指第三个输入文件中的第四个流。 另请参阅Stream说明符一章。




通常，选项将应用于下一个指定的文件。 因此，顺序很重要，您可以在命令行上多次使用相同的选项。 然后将每个事件应用于下一个输入或输出文件。 此规则的例外是全局选项（例如详细程度级别），其首先应该指定。




不要混合输入和输出文件 - 首先指定所有输入文件，然后指定所有输出文件。 也不要混合属于不同文件的选项。 所有选项仅适用于下一个输入或输出文件，并在文件之间复位。


1、要将输出文件的视频比特率设置为64 kbit / s：


`ffmpeg -i input.avi -b:v 64k -bufsize 64k output.avi`2、要强制输出文件的帧速率为24 fps：




`ffmpeg -i input.avi -r 24 output.avi`3、强制输入文件的帧速率（仅对原始格式有效）为1 fps，输出文件的帧速率为24 fps：




`ffmpeg -r 1 -i input.m2v -r 24 output.avi`

原始输入文件可能需要格式化选项。








三、详细说明

ffmpeg中的每个输出的代码转换过程可以通过下图来描述：



_______              ______________
|       |            |              |
| input |  demuxer   | encoded data |   decoder
| file  | ---------> | packets      | -----+
|_______|            |______________|      |
                                           v
                                       _________
                                      |         |
                                      | decoded |
                                      | frames  |
                                      |_________|
 ________             ______________       |
|        |           |              |      |
| output | <-------- | encoded data | <----+
| file   |   muxer   | packets      |   encoder
|________|           |______________|



**ffmpeg**调用libavformat库（包含解复用器）来读取输入文件，并从中获取包含编码数据的数据包。 当有多个输入文件时，**ffmpeg**尝试通过跟踪任何活动输入流上的最低时间戳来保持同步。






然后将编码的数据包传递给解码器（除非为流选择了streamcopy，请参阅进一步说明）。 解码器产生未经压缩的帧（原始视频/ PCM音频/ ...），可以通过过滤进一步处理（见下一节）。 滤波后，帧被传送到编码器，对编码器进行编码并输出编码的数据包。 最后，将这些传递给muxer，它将编码的数据包写入输出文件。








一）、过滤

在编码之前，ffmpeg可以使用libavfilter库中的过滤器处理原始音频和视频帧。 几个链式过滤器形成一个过滤器图。 ffmpeg区分两种类型的过滤器：简单而复杂。





1、简单的过滤器

简单的滤光片是具有相同类型的正好一个输入和输出的滤光片。 在上图中，可以通过简单地在解码和编码之间插入一个附加步骤来表示它们：



 _________                        ______________
|         |                      |              |
| decoded |                      | encoded data |
| frames  |\                   _ | packets      |
|_________| \                  /||______________|
             \   __________   /
  simple     _\||          | /  encoder
  filtergraph   | filtered |/
                | frames   |
                |__________|




简单的过滤器配置了每个流过滤器选项（分别使用-vf和-af别名的视频和音频别名）。 视频的简单过滤器可以像这样查找：




 _______        _____________        _______        ________
|       |      |             |      |       |      |        |
| input | ---> | deinterlace | ---> | scale | ---> | output |
|_______|      |_____________|      |_______|      |________|



请注意，某些过滤器更改帧属性，但不更改帧内容。 例如。 上述示例中的fps过滤器会更改帧数，但不会触摸帧内容。 另一个例子是setpts过滤器，它只设置时间戳，否则传递帧不变。





2、复杂滤波器

复杂滤波器是不能被简单地应用于一个流的线性处理链的那些。 例如，当图形具有多个输入和/或输出，或当输出流类型与输入不同时，就是这种情况。 它们可以用下面的图表示：



 _________
|         |
| input 0 |\                    __________
|_________| \                  |          |
             \   _________    /| output 0 |
              \ |         |  / |__________|
 _________     \| complex | /
|         |     |         |/
| input 1 |---->| filter  |\
|_________|     |         | \   __________
               /| graph   |  \ |          |
              / |         |   \| output 1 |
 _________   /  |_________|    |__________|
|         | /
| input 2 |/
|_________|




复杂的过滤器配置为-filter_complex选项。 请注意，此选项是全局的，因为复杂的过滤器本质上不能明确地与单个流或文件相关联。






-lavfi选项等同于-filter_complex。





复杂滤波器的一个简单的例子是覆盖滤波器，其具有两个视频输入和一个视频输出，其中包含覆盖在另一个视频上的一个视频。 它的音频对应是amix过滤器。








二）、流拷贝

流复制是通过将复制参数提供给-codec选项而选择的模式。 它使ffmpeg省略了指定流的解码和编码步骤，因此它只解析和复用。 更改容器格式或修改容器级元数据是非常有用的。 在这种情况下，上图将简化为：



 _______              ______________            ________
|       |            |              |          |        |
| input |  demuxer   | encoded data |  muxer   | output |
| file  | ---------> | packets      | -------> | file   |
|_______|            |______________|          |________|




由于没有解码或编码，它非常快，没有质量损失。 但是，由于很多因素，在某些情况下可能无法正常工作。 应用过滤器显然也是不可能的，因为过滤器对未压缩的数据工作。












四、流选择

默认情况下，ffmpeg只包含输入文件中存在的每种类型的一个流（视频，音频，字幕），并将它们添加到每个输出文件。 它基于以下标准选择每个的“最佳”：对于视频，它是具有最高分辨率的流，对于音频，它是具有最多频道的流，对于字幕，它是第一个字幕流。 在相同类型的几个流量相等的情况下，选择具有最低索引的流。





您可以使用-vn / -an / -sn / -dn选项来禁用某些默认值。 对于完全手动控制，请使用-map选项，它禁用刚才描述的默认值。











五、选项

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


使用此索引匹配流。 例如。 -threads：1 4将第二个流的线程数设置为4。





***stream_type[:stream_index]***


stream_type是以下之一：视频为'v'或'V'，音频'a'，字幕's''，数据'd'和附件't'。 'v'匹配所有视频流，'V'仅匹配未附加图片，视频缩略图或封面艺术的视频流。 如果给出stream_index，则它匹配此类型的流号stream_index。 否则，它将匹配此类型的所有流。





***p:program_id[:stream_index]***


如果给出了stream_index，那么它将使用id program_id与程序中具有数字stream_index的流进行匹配。 否则，它会匹配程序中的所有流。





***#stream_id或i：stream_id***


通过流ID匹配流（例如MPEG-TS容器中的PID）。





***m:key[:value]***


使用具有指定值的元数据标签键匹配流。 如果没有给出值，则将包含给定标签的流与任何值进行匹配。





***u***

匹配具有可用配置的流，必须定义编解码器，并且必须存在视频维度或音频采样率等基本信息。





请注意，在ffmpeg中，元数据的匹配将仅适用于输入文件。








二）、一般选项

这些选项在ff *工具之间共享。





***-L***

显示证书



***-h, -?, -help, --help [arg]***


显示帮助。 可以指定可选参数来打印有关特定项目的帮助。 如果没有指定参数，则只显示基本（非高级）工具选项。


arg参数的可能值为：


***long***：打印高级工具选项以及基本工具选项。

***full***：打印选项的完整列表，包括编码器，解码器，分离器，复用器，过滤器等的共享和私有选项。

***decoder=decoder_name***：打印名为decode_name的解码器的详细信息。 使用-decoders选项获取所有解码器的列表。


***encoder=encoder_name***：打印有关编码器的编码器名称的详细信息。 使用-encoders选项获取所有编码器的列表。


***demuxer=demuxer_name***：打印有关Demuxer_name的复制器的详细信息。 使用-formats选项来获取所有分割器和多路复用器的列表。


***muxer=muxer_name***：打印有关muxer_name的muxer的详细信息。 使用-formats选项获取所有muxers和demuxers的列表。


***filter=filter_name***：打印有关过滤器名称filter_name的详细信息。





***-version***


显示版本




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





***-layouts***


显示频道名称和标准频道布局。





***-colors***


显示可识别的颜色名称。





-sources device[,opt1=val1[,opt2=val2]...]


显示输入设备的自动检测源。 某些设备可能会提供不能自动检测的依赖于系统的源名称。 返回的列表不能被假定为始终完成。



`ffmpeg -sources pulse,server=192.168.0.4`






-sinks device[,opt1=val1[,opt2=val2]...]

显示输出设备的自动检测的接收器。 某些设备可能会提供不能自动检测的系统相关的接收器名称。 返回的列表不能被假定为始终完成。



`ffmpeg -sinks pulse,server=192.168.0.4```-loglevel [repeat+]loglevel | -v [repeat+]loglevel



设置库使用的日志记录级别。 添加“repeat +”表示重复的日志输出不应该被压缩到第一行，而“Last message repeat n times”行将被省略。 “重复”也可以单独使用。 如果单独使用“重复”，并且没有先前的日志级别，则将使用默认日志级别。 如果给出了多个loglevel参数，使用'repeat'不会改变loglevel。 loglevel是一个字符串或包含以下值之一的数字：
***‘quiet, -8’***

根本不显示; 安静。

***‘panic, 0’***

只显示可能导致进程崩溃的致命错误，例如断言失败。 这目前不用于任何东西。

***‘fatal, 8’***

只显示致命错误。 这些都是错误之后，过程绝对不能继续。

***‘error, 16’***

显示所有错误，包括可以从中恢复的错误。

***‘warning, 24’***

显示所有警告和错误。 将显示与可能不正确或意外事件相关的任何消息。

***‘info, 32’***

在处理过程中显示信息。 这是除了警告和错误之外。 这是默认值。

***‘verbose, 40’***

与信息相同，除了更详细。与信息相同，除了更详细。

‘***debug, 48’***

显示所有内容，包括调试信息。

***‘trace, 56’***

默认情况下，程序记录到stderr。 如果终端支持着色，则使用颜色来标记错误和警告。 可以禁用日志着色设置环境变量AV_LOG_FORCE_NOCOLOR或NO_COLOR，

或者可以强制设置环境变量AV_LOG_FORCE_COLOR。 环境变量NO_COLOR的使用已被弃用，将在将来的FFmpeg版本中被删除。




-report


将完整的命令行和控制台输出转储到当前目录中名为program-YYYYMMDD-HHMMSS.log的文件中。 此文件可用于错误报告。 它也意味着-loglevel verbose。

将环境变量FFREPORT设置为任何值具有相同的效果。 如果值为'：' - 分离的key =值序列，这些选项将影响报告; 如果选项值包含特殊字符或选项分隔符“：”（请参阅ffmpeg-utils手册中的“引用和转义”部分），则必须转义选项值。


可以识别以下选项：

file：设置要用于报告的文件名; ％p被扩展为程序的名称，％t被展开为时间戳，%%被扩展为普通的％


level：使用数值设置日志详细程度级别（请参见-loglevel）。

例如，要使用日志级别32（日志级别信息的别名）将报告输出到名为ffreport.log的文件：

FFREPORT=file=ffreport.log:level=32 ffmpeg -i input output
解析环境变量的错误并不致命，也不会在报表中显示。




-hide_banner


禁止打印横幅。


所有FFmpeg工具通常会显示版权声明，构建选项和库版本。 此选项可用于禁止打印此信息。





-cpuflags flags (global)


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




-opencl_bench


此选项用于对所有可用的OpenCL设备进行基准测试并打印结果。 仅当使用--enable-opencl编译FFmpeg时，此选项才可用。


当FFmpeg配置为--enable-opencl时，通过-opencl_options设置全局OpenCL上下文的选项。 有关支持的选项的完整列表，请参阅ffmpeg-utils手册中的“OpenCL Options”一节。

除此之外，这些选项包括能够选择特定的平台和设备来运行OpenCL代码。 默认情况下，FFmpeg将在第一个平台的第一个设备上运行。 虽然全局OpenCL上下文的选项为用户选择所选择的OpenCL设备提供了灵活性，但大多数用户可能希望为其系统选择最快的OpenCL设备。


此选项通过识别用户系统的适当设备来帮助选择最有效的配置。 所有内置基准测试都运行在所有OpenCL设备上，并对每个设备进行性能测试。 结果列表中的设备将根据其性

能进行排序，其中首先列出最快的设备。 用户随后可以使用通过-opencl_options认为最合适的设备调用ffmpeg，以获得OpenCL加速代码的最佳性能。


使用最快的OpenCL设备的典型用法包括以下步骤。


运行命令：



`ffmpeg -opencl_bench`

记下列表中第一个即最快设备的平台ID（pidx）和设备ID（didx）。 使用以下命令选择平台和设备：




`ffmpeg -opencl_options platform_idx=pidx:device_idx=didx ...`

-opencl_options options (global)



设置OpenCL环境选项。 仅当使用--enable-opencl编译FFmpeg时，此选项才可用。


选项必须是以“：”分隔的key = value选项对的列表。 有关支持的选项列表，请参阅ffmpeg-utils手册中的“OpenCL Options”一节。








三）、AVOption选项

这些选项由libavformat，libavdevice和libavcodec库直接提供。 要查看可用的AVOptions列表，请使用-help选项。 它们分为两类：


generic


可以为任何容器，编解码器或设备设置这些选项。 通用选项在容器/设备的AVFormatContext选项下以及编解码器的AVCodecContext选项下列出。





private


这些选项特定于给定的容器，设备或编解码器。 私有选项列在其相应的容器/设备/编解码器下。





例如，要将ID3v2.3标题而不是默认ID3v2.4写入MP3文件，请使用MP3 muxer的id3v2_version私有选项：



`ffmpeg -i input.flac -id3v2_version 3 out.mp3`

所有编解码器AVOptions都是每个流，因此应该附加一个流说明符。



注意：-nooption语法不能用于布尔AVOptions，请使用-option 0 / -option 1。


注意：通过将v / a / s添加到选项名称来指定每个流AVOptions的旧的未记录方式现在已经过时，将很快被删除。








四）、主要选项

***-f fmt (input/output)***


强制输入或输出文件格式。 通常会自动检测格式的输入文件，并从输出文件的文件扩展名中猜出，因此在大多数情况下不需要此选项。





***-i url (input)***


输入文件url





***-y (global)***


覆盖输出文件而不要求。





***-n (global)***


不要覆盖输出文件，如果指定的输出文件已经存在，则立即退出。





***-stream_loop number (input)***


输入流的设置次数应循环。 循环0表示无循环，循环-1表示无限循环。





***-c[:stream_specifier] codec (input/output,per-stream)-codec[:stream_specifier] codec (input/output,per-stream)***


在一个或多个流中选择一个编码器（在输出文件之前使用时）或解码器（在输入文件之前使用时）。 编解码器是解码器/编码器的名称或特殊值复制（仅输出），以指示流不被重新编码。


例如：


`ffmpeg -i INPUT -map 0 -c:v libx264 -c:a copy OUTPUT`使用libx264对所有视频流进行编码，并复制所有音频流。

对于每个流，应用最后一个匹配的c选项




`ffmpeg -i INPUT -map 0 -c copy -c:v:1 libx264 -c:a:137 libvorbis OUTPUT`将复制除第二个视频之外的所有流，这些视频将使用libx264进行编码，第138个音频将以libvorbis编码。




***-t duration (input/output)***


当用作输入选项（在-i之前）时，在此输入文件中寻找定位。 请注意，在大多数格式中，不可能完全寻求，所以ffmpeg将寻找到位置之前最接近的搜索点。 当代码转换和-accurate_seek被启用（默认）时，搜索点和位置之间的这个额外的段将被解码和丢弃。 当做流复制或使用-noaccurate_seek时，它将被保留。


当用作输出选项（在输出url之前）时，解码，但丢弃输入，直到时间戳达到位置。


位置必须是持续时间规范，请参阅（ffmpeg-utils）ffmpeg-utils（1）手册中的“持续时间”部分。





***-sseof position (input/output)***


像-ss选项，但相对于“文件的结尾”。 这是负值在文件中较早，0在EOF。





***-itsoffset offset（input）***


设置输入时间偏移量。


偏移量必须是持续时间规范，请参阅（ffmpeg-utils）ffmpeg-utils（1）手册中的持续时间部分。


偏移量被添加到输入文件的时间戳。 指定正偏移意味着相应的流被延迟了偏移量中指定的持续时间。





***-timestamp date (output)***


在容器中设置录制时间戳。


日期必须是日期规范，请参阅（ffmpeg-utils）ffmpeg-utils（1）手册中的Date部分。





***-metadata[:metadata_specifier] key=value (output,per-metadata)***


设置元数据键/值对。


可以使用可选的metadata_specifier来设置流，章节或程序上的元数据。 有关详细信息，请参阅-map_metadata文档。


此选项将覆盖使用-map_metadata设置的元数据。 也可以使用空值删除元数据。


例如，在输出文件中设置标题：



`ffmpeg -i in.avi -metadata title="my title" out.flv`要设置第一个音频流的语言：
`ffmpeg -i INPUT -metadata:s:a:0 language=eng OUTPUT`
***-disposition[:stream_specifier] value (output,per-stream)***



设置流的配置。


此选项将覆盖从输入流复制的配置。 也可以通过将其设置为0来删除。


以下处置得到承认：


default

dub

original

comment

lyrics

karaoke

forced

hearing_impaired

visual_impaired

clean_effects

captions

descriptions

metadata


例如：使第二个音频流成为默认流：


`ffmpeg -i in.mkv -disposition:a:1 default out.mkv`要使第二个字幕流成为默认流，并从第一个字幕流中删除默认配置：
`ffmpeg -i INPUT -disposition:s:0 0 -disposition:s:1 default OUTPUT`***-program [title=title:][program_num=program_num:]st=stream[:st=stream...] (output)***



创建一个具有指定标题的程序，program_num并将指定的流添加到该程序中。





***-target type (output)***


指定目标文件类型（vcd，svcd，dvd，dv，dv50）。 类型可能以pal-，ntsc-或film-为前缀，以使用相应的标准。 然后自动设置所有格式选项（比特率，编解码器，缓冲区大小）。 你可以输入：



`ffmpeg -i myfile.avi -target vcd /tmp/vcd.mpg`不过，您可以指定其他选项，只要您知道它们与标准不冲突，如下所示：
`ffmpeg -i myfile.avi -target vcd -bf 2 /tmp/vcd.mpg`
***-dframes number (output)***



设置要输出的数据帧数。 这是对于_frame：d的过时别名，您应该使用它。





***-frames[:stream_specifier] framecount (output,per-stream)***


在framecount框架之后停止写入流。





***-q[:stream_specifier] q (output,per-stream)-qscale[:stream_specifier] q (output,per-stream)***


使用固定质量标尺（VBR）。 q / qscale的含义取决于编解码。 如果使用qscale而没有stream_specifier，那么它只适用于视频流，这是为了保持与以前的行为的兼容性，并且指定相同的编解码器特定值到2个不同的编解码器，音频和视频通常不是什么意思当没有stream_specifier 用来。





***-filter[:stream_specifier] filtergraph (output,per-stream)***


创建由过滤器指定的过滤器，并使用它过滤流。


过滤器是要应用于流的过滤器的描述，并且必须具有相同类型流的单个输入和单个输出。 在过滤器中，输入与标签相关联，输出到标签输出。 有关filtergraph语法的更多信息，请参阅ffmpeg-filters手册。


如果要创建具有多个输入和/或输出的滤波器，请参阅-filter_complex选项。





***-filter_script[:stream_specifier] filename (output,per-stream)***


此选项与过滤器类似，唯一的区别是它的参数是要从中读取过滤器描述的文件的名称。





***-filter_threads nb_threads (global)***


定义用于处理过滤器管道的线程数。 每个管道将产生一个线程池，这个线程可以用于并行处理。 默认值是可用CPU的数量。





***-pre[:stream_specifier] preset_name (output,per-stream)***


指定匹配流的预设。





***-stats (global)***


打印编码进度/统计。默认情况下，要明确禁用它，您需要指定-nostat。





***-progress url (global)***


将程序友好的进度信息发送到URL。


进度信息大约每秒写入编码过程结束。它由“key = value”行组成。键仅包含字母数字字符。进度信息序列的最后一个关键是始终是“进度”。





***-stdin***


在标准输入上启用交互。默认为开，除非使用标准输入作为输入。要明确禁用交互，您需要指定-nostdin。


禁用标准输入上的交互是有用的，例如，如果ffmpeg在后台进程组中。 ffmpeg ... </ dev / null可以大致相同的结果，但它需要一个shell。





***-debug_ts (global)***


打印时间戳信息。默认情况下关闭。此选项对于测试和调试目的非常有用，输出格式可能会从一个版本更改为另一个版本，因此不应该由便携式脚本使用。


另见选项-fdebug ts。





***-attach filename (output)***


向输出文件添加附件。 这是由Matroska的几种格式支持的。 用于渲染字幕的字体。 附件实现为特定类型的流，因此此选项将向文件添加新流。 这样就可以以通常的方式在此流上使用每个流选项。 使用此选项创建的附件流将在所有其他流（即使用-map或自动映射创建的流）之后创建。


请注意，对于Matroska，您还必须设置mimetype元数据标签：



`ffmpeg -i INPUT -attach DejaVuSans.ttf -metadata：s：2 mimetype = application / x-truetype-font out.mkv`（假设附件流在输出文件中将是第三个）。




***-dump_attachment [：stream_specifier] filename（输入，每个流）***


将匹配的附件流解压缩到名为filename的文件中。 如果文件名为空，则将使用文件名元数据标签的值。


例如。 将第一个附件提取到名为“out.ttf”的文件中：



`ffmpeg -dump_attachment:t:0 out.ttf -i INPUT`将所有附件提取到文件名标签确定的文件中：
`ffmpeg -dump_attachment:t "" -i INPUT`技术说明 - 附件实现为编解码器extradata，因此该选项实际上可用于从任何流中提取extradata，而不仅仅是附件。




***-noautorotate***


根据文件元数据禁用自动旋转视频。








五）、视频选项

-vframes number (output)


设置要输出的视频帧数。这是_frame：v的过时别名，您应该使用它。





-r[:stream_specifier] fps (input/output,per-stream)


设置帧速率（Hz值，分数或缩写）。


作为输入选项，忽略存储在文件中的任何时间戳，并以假定为不变帧速率的帧生成时间戳。这与用于某些输入格式（如image2或v4l2）（以前在FFmpeg的旧版本中相同）的-framerate选项不同。如果使用-framerate而不是输入选项-r。


作为输出选项，重复或删除输入帧以实现不变输出帧速率fps。



-s[:stream_specifier] size (input/output,per-stream)


设置框架尺寸。


作为输入选项，这是video_size私有选项的快捷方式，由某些分辨率识别，帧大小不存储在文件中或可配置 - 例如。原始视频或视频抓取。


作为输出选项，它将缩放视频过滤器插入相应过滤器的末尾。请直接使用缩放滤镜将其插入到开头或其他位置。


格式为'wxh'（默认 - 与源相同）。





-aspect[:stream_specifier] aspect (output,per-stream)


设置由aspect指定的视频显示宽高比。


aspect可以是浮点数字符串，也可以是num：den形式的字符串，其中num和den是宽高比的分子和分母。例如“4：3”，“16：9”，“1.3333”和“1.7777”是有效的参数值。


如果与-vcodec副本一起使用，它将影响存储在容器级别的宽高比，但不影响存储在编码帧中的宽高比（如果存在）。





-vn (output)


禁用视频录制。





-vcodec codec (output)


设置视频编解码器。这是-codec的别名：v。





-pass[:stream_specifier] n (output,per-stream)


选择通行证号码（1或2）。它用于进行双程视频编码。视频的统计信息将记录在第一次记录到日志文件中（另见选项-passlogfile），而在第二次记录中，该日志文件用于以精确请求的比特率生成视频。通过1，您可能只是停用音频并将输出设置为null，Windows和Unix的示例：




```
ffmpeg -i foo.mov -c:v libxvid -pass 1 -an -f rawvideo -y NUL
ffmpeg -i foo.mov -c:v libxvid -pass 1 -an -f rawvideo -y /dev/null
```


-passlogfile[:stream_specifier] prefix (output,per-stream)



将双向记录文件名前缀设置为前缀，默认文件名前缀为“ffmpeg2pass”。 完整的文件名将为PREFIX-N.log，其中N是特定于输出流的数字





-vf filtergraph (output)


创建由过滤器指定的过滤器，并使用它过滤流。


这是-filter的别名：v，请参阅-filter选项。








六）、高级视频选项

***-pix_fmt[:stream_specifier] format (input/output,per-stream)***


设置像素格式。使用-pix_fmts显示所有支持的像素格式。如果选择的像素格式无法选择，ffmpeg会打印一个警告，并选择编码器支持的最佳像素格式。如果pix_fmt以+为前缀，则如果无法选择所请求的像素格式，则ffmpeg会退出并出现错误，并且禁用滤镜内的自动转换。如果pix_fmt是单个+，ffmpeg将选择与输入（或图形输出）相同的像素格式，并禁用自动转换。





***-sws_flags flags (input/output)***


设置SwScaler标志。





***-vdt n***


舍弃阈值





***-rc_override[:stream_specifier] override (output,per-stream)***


特定间隔的速率控制覆盖，格式为“int，int，int”列表，用斜杠分隔。两个第一个值是开始和结束帧数，最后一个是量化器使用如果为正，或质量因子如果为负。





***-ilme***


在编码器中强制交错支持（仅MPEG-2和MPEG-4）。如果您的输入文件是隔行扫描，并且要保持最小损失的隔行格式，请使用此选项。另一种方法是用--deinterlace去隔行输入流，但是去隔行引入损耗​​。





***-psnr***


计算压缩帧的PSNR。





***-vstats***


将视频编码统计信息转储到vstats_HHMMSS.log。





***-vstats_file file***


将视频编码统计信息转储成档。





***-vstats_version file***


指定要使用的vstats格式的版本。默认值为2。


version = 1 :



`frame= %5d q= %2.1f PSNR= %6.2f f_size= %6d s_size= %8.0fkB time= %0.3f br= %7.1fkbits/s avg_br= %7.1fkbits/s`version > 1:




`out= %2d st= %2d frame= %5d q= %2.1f PSNR= %6.2f f_size= %6d s_size= %8.0fkB time= %0.3f br= %7.1fkbits/s avg_br= %7.1fkbits/s`

***-top[:stream_specifier] n (output,per-stream)***



首先= 1 / bottom = 0 / auto = -1字段





***-dc precision***


Intra_dc_precision。





***-vtag fourcc/tag (output)***


强制视频标签/ 4cc。 这是一个别名为-tag：v。





***-qphist (global)***


显示QP直方图





***-vbsf bitstream_filter***


已弃用，请参阅-bsf





***-force_key_frames[:stream_specifier] time[,time...] (output,per-stream)-force_key_frames[:stream_specifier] expr:expr (output,per-stream)***


强制指定时间戳的关键帧，更精确地在每个指定时间之后的第一帧。


如果参数前缀为expr：，则字符串expr将被解释为一个表达式，并为每个框架进行求值。如果评估不为零，则强制关键帧。


如果其中一个时代是“第八章”，它将扩展到文件中所有章节开始的时间，以三角形为单位，以秒为单位表示。此选项可用于确保查找点存在于输出文件中的章标记或任何其他指定位置。


例如，要在5分钟之前插入一个关键帧，再加上每个章节开始前0.1秒的关键帧：



`-force_key_frames 0:05:00,chapters-0.1`expr中的表达式可以包含以下常量：

n



当前处理帧的数量，从0开始


n_forced

强制帧数



prev_forced_n

以前的强制帧的数量，当没有关键帧被强制时是NAN



prev_forced_t

以前的强制帧的时间，当没有关键帧被迫时，它是NAN



Ť

当前处理帧的时间





例如，每5秒钟强制一次关键帧，您可以指定：



`-force_key_frames expr:gte(t,n_forced*5)`在最后一个强制时间后5秒钟强制一个关键帧，从第二个13开始：
`-force_key_frames expr:if(isnan(prev_forced_t),gte(t,13),gte(t,prev_forced_t+5))`请注意，强制太多的关键帧对于某些编码器的前瞻算法是非常有害的：使用固定GOP选项或类似的方法将更有效。




***-copyinkf[:stream_specifier] (output,per-stream)***


当进行流拷贝时，也复制一些非关键帧。





***-init_hw_device type[=name][:device[,key=value...]]***


使用给定的设备参数初始化名为name的类型类型的新硬件设备。如果没有指定名称，它将接收默认名称“type％d”。


设备的含义和以下参数取决于设备类型：





CUDA

设备是CUDA设备的编号。



DXVA2

设备是Direct3D 9显示适配器的编号。



VAAPI

设备是X11显示名称或DRM渲染节点。如果未指定，将尝试打开默认的X11显示（$ DISPLAY），然后打开第一个DRM渲染节点（/ dev / dri / renderD128）。



VDPAU

设备是X11显示名称。如果未指定，将尝试打开默认的X11显示（$ DISPLAY）。



QSV

设备在“MFX_IMPL_ *”中选择一个值。允许的值是：

auto

SW

HW

auto_any

HW_ANY

HW2

HW3

HW4

如果未指定，则使用'auto_any'。 （请注意，通过创建平台适当的子设备（'dxva2'或'vaapi'），然后从中导出QSV设备，可以更容易地实现QSV的所需结果。）





***-init_hw_device type[=name]@source***


初始化名为name的类型类型的新硬件设备，从名称来源的现有设备中导出。





***-init_hw_device list***


列出此版本的ffmpeg支持的所有硬件设备类型。





***-filter_hw_device name***


将所有名称的硬件设备传递给任何过滤器图形中的所有过滤器。 这可以用于将设备上传到hwupload过滤器，或使用hwmap过滤器进行映射的设备。 当需要硬件设备时，其他过滤器也可以使用该参数。 请注意，这通常仅在输入尚未在硬件框架中时才需要 - 如果是，则过滤器将从接收到的帧的上下文导出所需的设备作为输入。


这是一个全局设置，所以所有过滤器都将收到相同的设备。





***-hwaccel[:stream_specifier] hwaccel (input,per-stream)***


使用硬件加速来解码匹配的流。 hwaccel的允许值为：


none

不要使用任何硬件加速（默认）。



auto

自动选择硬件加速方式。



VDA

使用苹果VDA硬件加速。



VDPAU

使用VDPAU（Video Decode和Presentation API for Unix）硬件加速。



DXVA2

使用DXVA2（DirectX视频加速）硬件加速。



VAAPI

使用VAAPI（Video Acceleration API）硬件加速。



QSV

使用英特尔QuickSync视频加速进行视频转码。





与大多数其他值不同，此选项不启用加速解码（每当选择qsv解码器时都会自动使用），但加速转码，而无需将帧复制到系统内存中。


为了工作，解码器和编码器都必须支持QSV加速度，并且不能使用滤波器。


如果选择的hwaccel不可用或不被选择的解码器支持，则此选项不起作用。


请注意，大多数加速方法都用于播放，并且不会比现代CPU上的软件解码速度更快。此外，ffmpeg通常需要将解码的帧从GPU内存复制到系统内存中，从而导致进一步的性能损失。因此，此选项主要用于测试。





***-hwaccel_device[:stream_specifier] hwaccel_device (input,per-stream)***


选择要用于硬件加速的设备。


此选项只有在指定了-hwaccel选项时才有意义。 它可以引用名称使用-init_hw_device创建的现有设备，也可以创建一个新设备，就像'-init_hw_device'类型：hwaccel_device之前一样。





***-hwaccels***


列出此构建ffmpeg支持的所有硬件加速方法。








七）、音频选项

***-aframes number (output)***


设置要输出的音频帧数。对于_frame：a而言，这是一个过时的别名，您应该使用它。

***-ar[:stream_specifier] freq (input/output,per-stream)***


设置音频采样频率。对于输出流，它默认设置为相应输入流的频率。对于输入流，此选项仅适用于音频抓取设备和原始解复用器，并映射到相应的解复用器选项





***-aq q (output)***


设置音频质量（编解码专用，VBR）。这是-q：a的别名。




***-ac[:stream_specifier] channels (input/output,per-stream)***


设置音频通道的数量。对于输出流，默认设置为输入音频通道的数量。对于输入流，此选项仅适用于音频抓取设备和原始解复用器，并映射到相应的解复用器选项。





***-an (output)***


禁用录音。





***-acodec codec (input/output)***


设置音频编解码器。这是-codec的别名：a。





***-sample_fmt[:stream_specifier] sample_fmt (output,per-stream)***


设置音频样本格式。使用-sample_fmts获取支持的样本格式列表。





***-af filtergraph (output)***


创建由过滤器指定的过滤器，并使用它过滤流。


这是-filter的别名：a，请参阅-filter选项。








八）、高级音频选项

***-atag fourcc/tag (output)***


强制音频标签/ 4cc。 这是-tag的别名：a。





***-absf bitstream_filter***


已弃用，请参阅-bsf





***-guess_layout_max channels (input,per-stream)***


如果某些输入通道布局未知，请尝试仅在最多指定数量的通道时进行猜测。 例如，2告诉ffmpeg将1个声道识别为单声道，2个声道作为立体声识别，而不是6声道。 默认是总是试图猜测。 使用0禁用所有猜测。








九）、字幕选项

***-scodec codec (input/output)***


设置字幕编解码器。 这是-codec：s的别名。





***-sn (output)***


禁用字幕录制。





***-sbsf bitstream_filter***


已弃用，请参阅-bsf








十）、高级字幕选项

***-fix_sub_duration***


修复字幕持续时间。 对于每个字幕，等待相同流中的下一个数据包，并调整第一个数据包的持续时间以避免重叠。 这对于一些字幕编解码器，特别是DVB字幕是必要的，因为原始分组中的持续时间仅仅是粗略估计，并且结束实际上被空字幕帧标记。 在必要时不能使用此选项可能导致由于非单调时间戳而导致的夸张的持续时间或复用失败。


请注意，此选项将延迟所有数据的输出，直到下一个字幕数据包被解码为止：它可能会增加内存消耗和延迟。





***-canvas_size size***


设置用于渲染字幕的画布的大小。








十一）、高级选项

-map [-]input_file_id[:stream_specifier][?][,sync_file_id[:stream_specifier]] | [linklabel] (output)


指定一个或多个输入流作为输出文件的源。 每个输入流由输入文件索引input_file_id和输入文件中的输入流索引input_stream_id标识。 两个索引从0开始。如果指定，sync_file_id：stream_specifier将哪个输入流用作呈现同步引用。


命令行上的第一个map选项指定输出流0的源，第二个map选项指定输出流1的源。


流标识符前面的一个字符创建一个“负”映射。 它禁用已经创建的映射的匹配流。


尾随？ 在流索引将允许地图是可选的：如果地图匹配没有流，地图将被忽略，而不是失败。 注意，如果使用无效的输入文件索引，地图仍然会失败; 例如地图是指不存在的输入。


另一种[linklabel]形式将映射来自复杂过滤器图形的输出（参见-filter_complex选项）到输出文件。 linklabel必须对应于图中定义的输出链接标签。


例如，将所有流从第一个输入文件映射到输出



`ffmpeg -i INPUT -map 0 output`例如，如果在第一个输入文件中有两个音频流，则这些流由“0：0”和“0：1”标识。 您可以使用-map来选择要在输出文件中放置哪些流。 例如：
`ffmpeg -i INPUT -map 0:1 out.wav`将以“0：1”标识的INPUT中的输入流映射到out.wav中的（单个）输出流。

例如，要从输入文件a.mov（由标识符“0：2”指定）中选择具有索引2的流，以及从输入b.mov（由标识符“1：6指定”）的索引6的流， 并将它们复制到输出文件out.mov：


`ffmpeg -i a.mov -i b.mov -c copy -map 0:2 -map 1:6 out.mov`从输入文件中选择所有视频和第三个音频流：
`ffmpeg -i INPUT -map 0:v -map 0:a:2 OUTPUT`要映射除第二个音频之外的所有流，请使用负映射


`ffmpeg -i INPUT -map 0 -map -0:a:1 OUTPUT`要从第一个输入映射视频和音频流，并使用尾随？，如果第一个输入中没有音频流，则忽略音频映射：
`ffmpeg -i INPUT -map 0:v -map 0:a? OUTPUT`选择英文音频流：
`ffmpeg -i INPUT -map 0:m:language:eng OUTPUT`请注意，使用此选项将禁用此输出文件的默认映射。


-ignore_unknown


忽略未知类型的输入流，而不是尝试复制此类流时失败。





-copy_unknown


允许复制未知类型的输入流，而不是尝试复制此类流时出现故障。





-map_channel [input_file_id.stream_specifier.channel_id|-1][:output_file_id.stream_specifier]


将音频通道从给定输入映射到输出。 如果output_file_id.stream_specifier未设置，音频通道将映射到所有音频流。


使用“-1”而不是input_file_id.stream_specifier.channel_id将映射静音通道。


例如，假设INPUT是立体声音频文件，您可以使用以下命令切换两个音频通道：



`ffmpeg -i INPUT -map_channel 0.0.1 -map_channel 0.0.0 OUTPUT`如果您要静音第一个频道并保持第二个频道：
`ffmpeg -i INPUT -map_channel -1 -map_channel 0.0.1 OUTPUT`“-map_channel”选项的顺序指定输出流中通道的顺序。 从映射的通道数量猜出输出通道布局（单声道如果一个“-map_channel”，立体声如果两个等）。 如果输入和输出通道布局不匹配（例如两个“-map_channel”选项和“-ac 6”），则使用“-map”组合“-map_channel”可以使通道增益级别更新。

您还可以将输入的每个通道提取到特定输出; 以下命令将INPUT音频流（文件0，流0）的两个通道提取到相应的OUTPUT_CH0和OUTPUT_CH1输出：


`ffmpeg -i INPUT -map_channel 0.0.0 OUTPUT_CH0 -map_channel 0.0.1 OUTPUT_CH1`以下示例将立体声输入的通道分成两个独立的流，将其放入相同的输出文件中：
`ffmpeg -i stereo.wav -map 0:0 -map 0:0 -map_channel 0.0.0:0.0 -map_channel 0.0.1:0.1 -y out.ogg`请注意，当前每个输出流只能包含来自单个输入流的通道; 您不能使用“-map_channel”来选择包含在不同流中的多个输入音频通道（来自相同或不同的文件），并将它们合并到单个输出流中。 因此，目前不可能将两个单独的单声道流转换成单个立体声流。 然而，将立体声流分割成两个单声道单声道流是可能的。

如果您需要此功能，可能的解决方法是使用amer ge ge filter。。。。 例如，如果您需要将一个媒体（这里是input.mkv）与2个单声道音频流合并为一个单声道音频流（并保留视频流），则可以使用以下命令：


`ffmpeg -i input.mkv -filter_complex "[0:1] [0:2] amerge" -c:a pcm_s16le -c:v copy output.mkv`

-map_metadata[:metadata_spec_out] infile[:metadata_spec_in] (output,per-metadata)

从infile设置下一个输出文件的元数据信息。 请注意，这些是文件索引（从零开始），而不是文件名。 可选的metadata_spec_in / out参数指定要复制的元数据。 元数据说明符可以具有以下形式：


g:全局元数据，即适用于整个文件的元数据

s[:stream_spec]:每流元数据。 stream_spec是Stream说明符一章中描述的流说明符。 在输入元数据说明符中，第一个匹配流被复制。 在输出元数据说明符中，将复制所有匹配的流。

c:chapter_index:每章元数据。 chapter_index是基于零的章索引。

p:program_index:每程序元数据。 program_index是基于零的程序索引。

如果省略了元数据说明符，则默认为全局。


默认情况下，从第一个输入文件复制全局元数据，每个流和每章元数据与流/章节一起复制。 通过创建相关类型的任何映射来禁用这些默认映射。 可以使用负文件索引来创建仅禁用自动复制的虚拟映射。


例如，将元数据从输入文件的第一个流复制到输出文件的全局元数据：



`ffmpeg -i in.ogg -map_metadata 0:s:0 out.mp3`要做相反的，即将全局元数据复制到所有音频流：
`ffmpeg -i in.mkv -map_metadata:s:a 0:g out.mkv`请注意，在此示例中，简单的0也将起作用，因为默认情况下假定全局元数据。


-map_chapters input_file_index (output)


将具有索引input_file_index的输入文件的章节复制到下一个输出文件。如果没有指定章节映射，则从第一个输入文件中复制至少一章。使用负文件索引来禁用任何章节复制。





-benchmark (global)


在编码结束时显示基准测试信息。显示CPU使用时间和最大内存消耗。所有系统不支持最大内存消耗，如果不支持，它通常会显示为0。





-benchmark_all (global)


在编码期间显示基准信息。显示用于各种步骤（音频/视频编码/解码）的CPU时间。





-timelimit duration (global)


ffmpeg运行持续时间秒后退出。





-dump (global)


将每个输入数据包转储到stderr。





-hex (global)


当转储数据包时，也会转储有效载荷。





-re (input)


以原始帧速率读取输入。主要用于模拟抓取设备或实时输入流（例如，从文件读取时）。不应与实际抓取设备或实时输入流（可能导致数据包丢失）一起使用。默认情况下，ffmpeg会尽可能快地读取输入。该选项将使输入读取速度降低到输入的本机帧速率。它对于实时输出（例如直播）很有用。





-loop_input


循环输入流。目前它只适用于图像流。此选项用于自动FFserver测试。此选项已弃用，请使用-loop 1。





-loop_output number_of_times


重复循环输出，支持循环播放，如GIF动画（0将无限循环输出）。此选项已弃用，请使用-loop。





-vsync parameter


视频同步方式 为了兼容性，可以将旧值指定为数字。 新添加的值必须始终指定为字符串。


0, passthrough:每个帧都以其时间戳从分解器传递到复用器。

1, cfr帧将被复制和丢弃，以实现所要求的恒定帧速率。

2, vfr:帧通过其时间戳或丢弃，以防止2帧具有相同的时间戳。

drop:作为传递，但是破坏所有时间戳，使得复用器基于帧速率产生新的时间戳。

-1, auto:根据复用器功能选择1到2。 这是默认方法。

请注意，在此之后，复制器可以进一步修改时间戳。 例如，在启用格式选项avoid_negative_ts的情况下。

使用-map可以从哪个流中选择应该采用的时间戳。 您可以保留视频或音频不变，并将剩余的流同步到未更改的流。




-frame_drop_threshold parameter


帧丢弃阈值，其指定在丢弃视频帧之前可以留下多少。在帧速率单位中，1.0是一帧。默认值为-1.1。一个可能的用法是在有噪声的时间戳的情况下避免帧格式，或者在精确时间戳的情况下增加帧丢帧精度。





-async samples_per_second


音频同步方式“拉伸/挤压”音频流以匹配时间戳，该参数是音频改变的每秒最大采样数。 - 同步1是一种特殊情况，其中只有音频流的开始被校正，没有任何更新。


请注意，在此之后，复制器可以进一步修改时间戳。例如，在启用格式选项avoid_negative_ts的情况下。


此选项已被弃用。改为使用issample音频滤镜。





-copyts


不要处理输入时间戳，而是保持其值而不尝试清理它们。具体而言，请勿删除初始起始时间偏移值。


请注意，根据vsync选项或特定的muxer处理（例如，如果启用格式选项avoid_negative_ts），即使选择此选项，输出时间戳也可能与输入时间戳不一致。





-start_at_zero


当与副本一起使用时，转移输入时间戳，以便它们从零开始。


这意味着使用例如-ss 50将使输出时间戳从50秒开始，而不管输入文件何时启动。





-copytb mode


指定流复制时如何设置编码器时基。 mode是一个整数值，可以采用以下值之一：


1:使用解码器时基。

   时基从相应的输入解复用器复制到输出编码器。 有时需要避免在以可变帧速率复制视频流时，不会单调增加时间戳。

0:使用解码器时基。

   时基从相应的输入解码器复制到输出编码器。

-1:尝试自动进行选择，以生成一个理智的输出。

默认值为-1。




-enc_time_base[:stream_specifier] timebase (output,per-stream)


设置编码器时基。 时基是浮点数，可以采用以下值之一：


0:根据媒体类型分配默认值。

   对于视频 - 使用1帧率，音频使用1 /采样率。

-1:尽可能使用输入流时基。

    如果输入流不可用，将使用默认的时基。

>0:使用提供的号码作为时基。

     该字段可以以两个整数（例如1:24,1:48000）或浮点数（例如0.04166,2.08333e-5）的比率来提供，

默认值为0。




-shortest (output)


当最短输入流结束时完成编码。





-dts_delta_threshold


时间戳不连续差值阈值。





-muxdelay seconds (input)


设置最大解码解码延迟。





-muxpreload seconds (input)


设置初始解复用解码延迟。





-streamid output-stream-index:new-value (output)


为输出流分配一个新的stream-id值。 此选项应在其应用的输出文件名之前指定。 对于存在多个输出文件的情况，可以将流分配重新分配给不同的值。


例如，要为输出mpegts文件将流0 PID设置为33，流1 PID设置为36，



`ffmpeg -i inurl -streamid 0:33 -streamid 1:36 out.ts`

-bsf[:stream_specifier] bitstream_filters (output,per-stream)

设置匹配流的比特流过滤器。 bits_filters是逗号分隔的比特流筛选器列表。 使用-bsfs选项获取比特流筛选器列表。



`ffmpeg -i h264.mp4 -c:v copy -bsf:v h264_mp4toannexb -an out.h264``ffmpeg -i file.mov -an -vn -bsf:s mov2textsub -c:s copy -f rawvideo sub.txt`

-tag[:stream_specifier] codec_tag (input/output,per-stream)

强制一个标签/ fourcc来匹配流。




-timecode hh:mm:ssSEPff


指定写入的时间码。 SEP是“：”，用于非丢弃时间码，“;”（或'。'）用于删除。



`ffmpeg -i input.mpg -timecode 01:02:03.04 -r 30000/1001 -s ntsc output.mpg`

-filter_complex filtergraph (global)

定义一个复杂的滤波器，即具有任意数量的输入和/或输出的滤波器。 对于简单的图形 - 具有一个输入和一个相同类型的输出的图 - 请参阅-filter选项。 过滤器是过滤器的描述，如ffmpeg过滤器手册的“过滤器语法”部分所述。


输入链接标签必须使用[file_index：stream_specifier]语法（即与映射使用相同）来引用输入流。 如果stream_specifier匹配多个流，则将使用第一个流。 未标记的输入将连接到匹配类型的第一个未使用的输入流。


输出链接标签参考-map。 未标记的输出被添加到第一个输出文件。


请注意，使用此选项可以仅使用没有正常输入文件的lavfi源。


例如，将图像叠加在视频上




```
ffmpeg -i video.mkv -i image.png -filter_complex '[0:v][1:v]overlay[out]' -map
'[out]' out.mkv
```
这里[0：v]是指第一个输入文件中的第一个视频流，它连接到叠加滤波器的第一个（主）输入。 类似地，第二输入中的第一视频流被链接到覆盖的第二（覆盖）输入。

假设每个输入文件中只有一个视频流，我们可以省略输入标签，所以上面相当于



```
ffmpeg -i video.mkv -i image.png -filter_complex 'overlay[out]' -map
'[out]' out.mkv
```
此外，我们可以省略输出标签，并且过滤器图形中的单个输出将自动添加到输出文件中，因此我们可以简单地写
`ffmpeg -i video.mkv -i image.png -filter_complex 'overlay' out.mkv`使用lavfi颜色源生成5秒的纯红色视频：
`ffmpeg -filter_complex 'color=c=red' -t 5 out.mkv`

-filter_complex_threads nb_threads (global)

定义使用多少个线程来处理filter_complex图。与filter_threads类似，但仅用于-filter_complex图。默认值是可用CPU的数量。





-lavfi filtergraph (global)


定义一个复杂的滤波器，即具有任意数量的输入和/或输出的滤波器。相当于-filter_complex。





-filter_complex_script filename (global)


此选项与-filter_complex类似，唯一的区别是它的参数是要从中读取复杂过滤器描述的文件的名称。





-accurate_seek (input)


此选项可使用-ss选项启用或禁用输入文件中的准确搜索。默认情况下启用，所以在转码时寻求准确。使用-noaccurate_seek禁用它，这可能是有用的。当复制一些流和转码其他流。





-seek_timestamp (input)


此选项通过-ss选项启用或禁用输入文件中的时间戳搜索。默认情况下禁用。如果启用，则将-ss选项的参数视为实际时间戳，并且不会被文件的开始时间偏移。这仅适用于不从时间戳记0开始的文件，例如传输流。





-thread_queue_size size (input)


此选项设置从文件或设备读取时排队数据包的最大数量。对于低延迟/高速率的实时流，如果不及时读取数据包，则可能丢弃数据包;提高这个值可以避免。





-override_ffserver (global)


覆盖ffserver的输入规范。使用此选项，您可以将任何输入流映射到ffserver，并从ffmpeg控制编码的许多方面。如果没有这个选项，ffmpeg将向ffserver发送ffserver请求的内容。


该选项适用于需要无法指定为ffserver但可以进行ffmpeg功能的情况。





-sdp_file file (global)


打印输出流文件的sdp信息。这允许在至少一个输出不是rtp流时转储sdp信息。 （需要至少一种输出格式为rtp）。





-discard (input)


允许在分解器处丢弃流的特定流或帧。 并不是所有的demuxers都支持这一点。


none:舍弃无框架

default:默认，它丢弃没有帧。

noref:舍弃所有非参考帧。

nokey:丢弃除关键帧之外的所有帧。

all:舍弃所有帧




-abort_on flags (global)


停止和中止在各种条件。 以下标志可用：


empty_output:没有数据包被传递给muxer，输出为空。




-xerror (global)


停止并退出错误





-max_muxing_queue_size packets (output,per-stream)


当转码音频和/或视频流时，ffmpeg将不会开始写入输出，直到它为每个这样的流有一个数据包。 在等待这种情况发生时，其他流的数据包被缓冲。 此选项为匹配的输出流设置此缓冲区的大小（以数据包为单位）。


此选项的默认值应足够大，大多数用途，因此只有在确定需要时才可以触摸此选项。





作为一个特殊的例外，您可以使用位图字幕流作为输入：它将被转换为与文件中最大视频大小相同的视频，如果没有视频，则将转换为720x576。 请注意，这是一个实验和临时解决方案。 一旦libavfilter正确支持字幕，它将被删除。


例如，为了在以MPEG-TS格式存储的DVB-T录音之上硬编码字幕，将字幕延迟1秒：




```
ffmpeg -i input.ts -filter_complex \
  '[#0x2ef] setpts=PTS+1/TB [sub] ; [#0x2d0] [sub] overlay' \
  -sn -map '#0x2dc' output.mkv
```
（0x2d0，0x2dc和0x2ef分别是视频，音频和字幕流的MPEG-TS PID; 0：0，0：3和0：7也将工作）





十二）、预设文件

一个预设文件包含一个选项=值对序列，每行一个，指定一系列在命令行上指定的选项。 以哈希（'＃'）字符开头的行将被忽略，用于提供注释。 检查FFmpeg源代码树中的预设目录中的示例。


有两种类型的预设文件：ffpreset和avpreset文件。





1、ffpreset文件

fpreset文件使用vpre，apre，spre和fpre选项指定。 fpre选项将预设的文件名替换为预设名称作为输入，可用于任何类型的编解码器。 对于vpre，apre和spre选项，预设文件中指定的选项将应用于与预设选项相同类型的当前选定的编解码器。


传递给vpre，apre和spre预设选项的参数根据以下规则识别要使用的预设文件：


首先，ffmpeg在目录$ FFMPEG_DATADIR（如果设置）和$ HOME / .ffmpeg的目录中搜索名为arg.ffpreset的文件，并在配置时定义的datadir（通常为PREFIX / share / ffmpeg）或ffpresets文件夹中 可执行的win32，按顺序。 例如，如果参数是libvpx-1080p，它将搜索文件libvpx-1080p.ffpreset。


如果没有找到这样的文件，那么ffmpeg将在上述目录中搜索名为codec_name-arg.ffpreset的文件，其中，codec_name是要应用预设文件选项的编解码器的名称。 例如，如果您使用-vcodec libvpx选择视频编解码器，并使用-vpre 1080p，则会搜索libvpx-1080p.ffpreset文件。





2、avpreset文件

使用pre选项指定avpreset文件。 它们类似于ffpreset文件，但它们只允许编码器特定的选项。 因此，不能使用指定编码器的选项=值对。


当指定pre选项时，ffmpeg将在$ AVCONV_DATADIR（如果设置）和$ HOME / .avconv目录中查找文件。后缀为.avpreset，并在配置时定义的datadir（通常为PREFIX / share / ffmpeg） ， 以该顺序。


首先，ffmpeg在上述目录中搜索名为codec_name-arg.avpreset的文件，其中，codec_name是要应用预设文件选项的编解码器的名称。 例如，如果您使用-vcodec libvpx选择视频编解码器并使用-pre 1080p，则会搜索libvpx-1080p.avpreset文件。


如果没有找到这样的文件，那么ffmpeg将在同一目录中搜索名为arg.avpreset的文件。








六、实例

一）、视频和音频抓取

如果您指定输入格式和设备，则ffmpeg可以直接抓取视频和音频。



`ffmpeg -f oss -i /dev/dsp -f video4linux2 -i /dev/video0 /tmp/out.mpg`或者使用ALSA音频源（单声道输入，卡ID 1）而不是OSS：
`ffmpeg -f alsa -ac 1 -i hw:1 -f video4linux2 -i /dev/video0 /tmp/out.mpg`请注意，您必须在任何电视浏览器（如Gerd Knorr的xawtv）启动ffmpeg之前激活正确的视频源和通道。 您还必须使用标准混音器正确设置录音水平。





二）、X11抓取

用ffmpeg通过X11显示



`ffmpeg -f x11grab -video_size cif -framerate 25 -i :0.0 /tmp/out.mpg`0.0是您的X11服务器的显示屏号，与DISPLAY环境变量相同。
`ffmpeg -f x11grab -video_size cif -framerate 25 -i :0.0+10,20 /tmp/out.mpg`0.0是您的X11服务器的显示屏号，与DISPLAY环境变量相同。 10是x偏移，20是y偏移的抓取。





三）、视频和音频文件格式转换

任何支持的文件格式和协议都可以作为ffmpeg的输入：


例子：

1、您可以使用YUV文件作为输入：


`ffmpeg -i /tmp/test%d.Y /tmp/out.mpg`它将使用文件：

```
/tmp/test0.Y, /tmp/test0.U, /tmp/test0.V,
/tmp/test1.Y, /tmp/test1.U, /tmp/test1.V, etc...
```
Y文件使用U和V文件分辨率的两倍。 它们是原始文件，没有标题。 它们可以由所有体面的视频解码器生成。 如果ffmpeg无法猜出，则必须使用-s选项指定图像的大小。


2、您可以从原始的YUV420P文件输入：


`ffmpeg -i /tmp/test.yuv /tmp/out.avi`test.yuv是一个包含原始YUV平面数据的文件。 每个框架由Y平面和U和V平面以半垂直和水平分辨率组成。


3、您可以输出到原始的YUV420P文件：


`ffmpeg -i mydivx.avi hugefile.yuv`

4、您可以设置多个输入文件和输出文件：


`ffmpeg -i /tmp/a.wav -s 640x480 -i /tmp/a.yuv /tmp/a.mpg`将音频文件a.wav和原始YUV视频文件a.yuv转换为MPEG文件a.mpg。


5、您还可以同时进行音频和视频转换：


`ffmpeg -i /tmp/a.wav -ar 22050 /tmp/a.mp2`将a.wav以22050Hz采样率转换为MPEG音频。


6、您可以同时编码为多种格式，并定义从输入流到输出流的映射：


`ffmpeg -i /tmp/a.wav -map 0:a -b:a 64k /tmp/a.mp2 -map 0:a -b:a 128k /tmp/b.mp2`将a.wav转换为64位的a.mp2和128 kbits的b.mp2。 '-map file：index'按照输出流的定义顺序指定每个输出流使用哪个输入流。


7、您可以转码解密的VOB：


`ffmpeg -i snatch_1.vob -f avi -c:v mpeg4 -b:v 800k -g 300 -bf 2 -c:a libmp3lame -b:a 128k snatch.avi`这是一个典型的DVD翻录示例; 输入是一个VOB文件，输出一个AVI文件与MPEG-4视频和MP3音频。 请注意，在此命令中，我们使用B帧，因此MPEG-4流是DivX5兼容的，GOP大小是300，这意味着29.97fps输入视频每10秒钟一帧内帧。 此外，音频流是MP3编码的，因此您需要通过传递--enable-libmp3lame来配置来启用LAME支持。 该映射对于DVD转码特别有用，以获得所需的音频语言。

注意：要查看支持的输入格式，请使用ffmpeg -demuxers。




8、您可以从视频中提取图像，或从许多图像创建视频：

从视频中提取图像：



`ffmpeg -i foo.avi -r 1 -s WxH -f image2 foo-%03d.jpeg`这将从视频中每秒提取一个视频帧，并将它们输出到名为foo-001.jpeg，foo-002.jpeg等的文件中。图像将被重新缩放以适应新的WxH值。

如果要仅提取有限数量的帧，则可以将以上命令与-frames：v或-t选项结合使用，或与-ss组合使用，以便从某个时间点开始提取。

用于从许多图像创建视频：



`ffmpeg -f image2 -framerate 12 -i foo-%03d.jpeg -s WxH foo.avi`语法foo％03d.jpeg指定使用由填充零的三位数组成的十进制数来表示序列号。 它与C printf函数支持相同的语法，但只适用于接受正常整数的格式。

导入图像序列时，-i还支持通过选择image2特定的-pattern_type glob选项来内部扩展类似shell的通配符模式（globbing）。

例如，创建与glob模式匹配的文件名的视频foo - *。jpeg：



`ffmpeg -f image2 -pattern_type glob -framerate 12 -i 'foo-*.jpeg' -s WxH foo.avi`

9、您可以在输出中放入相同类型的许多流：


`ffmpeg -i test1.avi -i test2.avi -map 1：1 -map 1：0 -map 0：1 -map 0：0 -c copy -y test12.nut`结果输出文件test12.nut将以相反的顺序包含输入文件中的前四个流。




10、强制CBR视频输出：

`ffmpeg -i myfile.avi -b 4000k -minrate 4000k -maxrate 4000k -bufsize 1835k out.m2v`

11、四个选项lmin，lmax，mblmin和mblmax使用“lambda”单位，但您可以使用QP2LAMBDA常量轻松从“q”单位转换：


`ffmpeg -i src.ext -lmax 21*QP2LAMBDA dst.ext`




