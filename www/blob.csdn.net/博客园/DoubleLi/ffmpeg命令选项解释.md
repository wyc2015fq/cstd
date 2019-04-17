# ffmpeg命令选项解释 - DoubleLi - 博客园






ffmpeg作为媒体文件处理软件，基本用法如下:

ffmpeg -i INPUTfile [OPTIONS] OUTPUTfile

输入输出文件通常就是待处理的多媒体文件了。可以是纯粹的音频文件，纯粹的视频文件，或者混合的。ffmpeg主持绝大部分的常见音频,

视频格式，象常见的各mpeg,AVI封装的DIVX和Xvid等等，具体的格式支持列表可以使用ffmpeg -formats查看或直接查阅文档。

另外，由于[Linux](http://lib.csdn.net/base/linux)把设备视为文件爱你，因此，-i 选项后可以跟设备名。比如DV，视频卡，光驱或者其它的各类设备。输出的内容通过:

Options调整，其主要的选择如下：

-vcodec视频流编码方式

-b 视频流帧码率（默认只有200k，一般都需要手动设置，具体的数值视codec选择而定）

-r 视频流帧率（一般说来PAL制式通常用25,NTSC制式通常用29）

-s 视频解析度（分辨率，也要视codec和你的需要而定。另：具体写法使用“数字x数字”的形式）

-t 处理持续时间

-acodec 音频流编码方式

-ab 音频流码率（默认是同源文件码率，也需要视codec而定）

-ar 音频流采样率（大多数情况下使用44100和48000,分别对应PAL制式和NTSC制式，根据需要选择）

-vn 屏蔽视频流

-an 屏蔽音频流

-author 设置媒体文件的作者

-title 设置媒体文件的题目

-f 强制使用某种格式

- target type 使用预置的格式转换（可以转换dvd，vcd或svcd）



除此之外还有些更高级的选择，如设定vbr，或设定high quality，或者设定vbr的buff和max/min码率，像一般我们自用的dvd抓轨，DV转vcd，dvd，网上下载的电影

转成vcd或dvd都不一定需要用到它们。



常用命令选择说明

-fromats 现实可用的格式

-f fmt强迫采用格式fmt

-l filename 输出文件

-y 覆盖输出文件

-t duration 设置记录时间hh:mm:ss[.xxx]格式的记录时间也支持（截图需要）

-ss position 搜索到指定的时间[-]hh:mm:ss[.xxx]的格式也支持

-title string 设置标题

-author string 设置作者

-copyright string 设置版权

-comment string 设置评论

-target type 设置目标文件类型（vcd,svcd,dvd）,所有的格式选项（比特率，编解码以及缓冲区大小）自动设置，只需要输入如下的就可以了:ffmpeg -i myfile.avi -target vcd /tmp/vcd.mpg

-hq 激活高质量设置



-b bitrate 设置比特率，缺省200kb/s

-r fps设置帧频，缺省25

-s size 设置帧大小，格式为WXH，缺省160X128.

-aspect aspect 设置横纵比4：316：9或1.3333 1.7777

-croptop/botton/left/right size 设置顶部切除带大小，像素单位

-padtop/botton/left/right size 设置顶部补齐的大小，像素单位

-padcolor color 设置补齐条颜色（hex,6个16进制的数,红：绿：蓝排列，比如000000代表黑色）

-vn 不做视频记录

-bt tolerance 设置视频码率容忍度kbit/s

-maxrate bitrate 设置最大视频码率容忍度

-minrate bitrate设置最小视频码率容忍度

-bufsize size 设置码率控制缓冲大小

-vcodec codec 强制使用codec编码方式，如果用copy便是原始编码数据必须被拷贝



-ab bitrate 设置音频码率

-ar freq 设置音频采样率

-ac channels 设置通道，缺省为1

-an 不使能音频记录

-acodec codec 使用codec编解码





-vd device 设置视频捕获设备，比如/dev/video o

-vc channel 设置视频捕获通道DV1394专用

-tvstd standard 设置电视标准NTSC PAL（SECAM）

-dv1394 设置DV1394捕获

-av device 设置音频设备 比如/dev/dsp



-map file:stream 设置输入流映射

-debug 打印特定调试信息

-benchmark 为基准[测试](http://lib.csdn.net/base/softwaretest)加入时间

-hex 倾到每一个输入包

-bitexact 仅使用位精确[算法](http://lib.csdn.net/base/datastructure)，用于编解码测试

-ps size设置包大小，以bits为单位

-re 以本地帧频读数据，主要用于模拟捕获设备

-loop 循环输入流。只工作于图像流，用于ffserver测试









