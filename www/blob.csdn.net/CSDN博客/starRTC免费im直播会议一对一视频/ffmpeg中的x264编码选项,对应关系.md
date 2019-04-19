# ffmpeg中的x264编码选项,对应关系 - starRTC免费im直播会议一对一视频 - CSDN博客
2017年08月22日 11:18:43[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：637
[http://ffmpeg.org/ffmpeg-all.html#libx264_002c-libx264rgb](http://ffmpeg.org/ffmpeg-all.html#libx264_002c-libx264rgb)
x264 H.264/MPEG-4 AVC encoder wrapper.
This encoder requires the presence of the libx264 headers and library during configuration. You need to explicitly configure the build with --enable-libx264.
libx264 supports an impressive number of features, including 8x8 and 4x4 adaptive spatial transform, adaptive B-frame placement, CAVLC/CABAC entropy coding, interlacing
 (MBAFF), lossless mode, psy optimizations for detail retention (adaptive quantization, psy-RD, psy-trellis).
Many libx264 encoder options are mapped to FFmpeg global codec options,
 while unique encoder options are provided through private options.
 Additionally the x264opts and x264-params private
 options allows one to pass a list of key=value tuples as accepted by the libx264 x264_param_parse function.
The libx264rgb encoder
is the same as libx264, except it accepts packed RGB pixel formats
 as input instead of YUV.
[**17.8.1 Supported Pixel Formats**](http://ffmpeg.org/ffmpeg-all.html#toc-Supported-Pixel-Formats)
x264 supports 8- to 10-bit color spaces. The exact bit depth is controlled at x264’s configure time. FFmpeg only supports one bit depth in one particular build. In other
 words, it is not possible to build one FFmpeg with multiple versions of x264 with different bit depths.
[**17.8.2 Options**](http://ffmpeg.org/ffmpeg-all.html#toc-Options-26)
The following options are supported by the libx264 wrapper. The x264-equivalent
 options or values are listed in parentheses括号 for easy migration.括号里面对应x264的选项
To reduce the duplication of documentation, only the private options and some others requiring special attention are documented here.
For the documentation of the undocumented generic options, see [the
 Codec Options chapter](http://ffmpeg.org/ffmpeg-all.html#codec_002doptions).
To get a more accurate and extensive documentation of the libx264 options, invoke the command x264
 --fullhelp or consult the libx264 documentation.
**b (*****bitrate*****)**
Set bitrate in bits/s. Note that FFmpeg’s b option
 is expressed in bits/s, while x264’s bitrate is
 in kilobits/s.
**bf (*****bframes*****)**
**g (*****keyint*****)**
**qmin (*****qpmin*****)**
Minimum quantizer scale.
**qmax (*****qpmax*****)**
Maximum quantizer scale.
**qdiff (*****qpstep*****)**
Maximum difference between quantizer scales.
**qblur (*****qblur*****)**
Quantizer curve blur
**qcomp (*****qcomp*****)**
Quantizer curve compression factor
**refs (*****ref*****)**
Number of reference frames each P-frame can use. The range is from *0-16*.
**sc_threshold (*****scenecut*****)**
Sets the threshold for the scene change detection.
**trellis (*****trellis*****)**
Performs Trellis quantization to increase efficiency. Enabled by default.
**nr (*****nr*****)**
**me_range (*****merange*****)**
Maximum range of the motion search in pixels.
**me_method (*****me*****)**
Set motion estimation method. Possible values in the decreasing order of speed:
**‘****dia (*****dia*****)****’**
**‘****epzs (*****dia*****)****’**
Diamond search with radius 1 (fastest). ‘epzs’
 is an alias for ‘dia’.
**‘****hex (*****hex*****)****’**
Hexagonal search with radius 2.
**‘****umh (*****umh*****)****’**
Uneven multi-hexagon search.
**‘****esa (*****esa*****)****’**
Exhaustive search.穷举搜索
**‘****tesa (*****tesa*****)****’**
Hadamard exhaustive search (slowest).
**forced-idr**
Normally, when forcing a I-frame type, the encoder can select any type of I-frame. This option forces it to choose an IDR-frame.
**subq (*****subme*****)**
Sub-pixel motion estimation method.
**b_strategy (*****b-adapt*****)**
Adaptive B-frame placement decision algorithm. Use only on first-pass.
**keyint_min (*****min-keyint*****)**
Minimum GOP size.
**coder**
Set entropy encoder. Possible values:
**‘****ac****’**
Enable CABAC.
**‘****vlc****’**
Enable CAVLC
 and disable CABAC. It generates the same effect as x264’s --no-cabac option.
**cmp**
Set full pixel motion estimation comparison algorithm. Possible values:
**‘****chroma****’**
Enable chroma in motion estimation.
**‘****sad****’**
Ignore chroma in motion estimation. It generates the same effect as x264’s --no-chroma-me option.
**threads (*****threads*****)**
Number of encoding threads.
**thread_type**
Set multithreading technique. Possible values:
**‘****slice****’**
Slice-based multithreading. It generates the same effect as x264’s --sliced-threads option.
**‘****frame****’**
Frame-based multithreading.
**flags**
Set encoding flags. It can be used to disable closed GOP and enable
open GOP by setting it to -cgop.
 The result is similar to the behavior of x264’s --open-gop option.
**rc_init_occupancy (*****vbv-init*****)**
**preset (*****preset*****)**
Set the encoding preset.
**tune (*****tune*****)**
Set tuning of the encoding params.
**profile (*****profile*****)**
Set profile restrictions.
**fastfirstpass**
Enable fast settings when encoding first pass, when set to 1. When set to 0, it has the same effect of x264’s --slow-firstpass option.
**crf (*****crf*****)**
Set the quality for constant quality mode.
**crf_max (*****crf-max*****)**
In CRF mode, prevents VBV from lowering quality beyond this point.
**qp (*****qp*****)**
Set constant quantization rate control method parameter.
**aq-mode (*****aq-mode*****)**
Set AQ method. Possible values:
**‘****none (*****0*****)****’**
Disabled.
**‘****variance (*****1*****)****’**
Variance AQ (complexity mask).
**‘****autovariance (*****2*****)****’**
Auto-variance AQ (experimental).
**aq-strength (*****aq-strength*****)**
Set AQ strength, reduce blocking and blurring in flat and textured areas.
**psy**
Use psychovisual optimizations when set to 1. When set to 0, it has the same effect as x264’s --no-psy option.
**psy-rd (*****psy-rd*****)**
Set strength of psychovisual optimization, in *psy-rd*:*psy-trellis* format.
**rc-lookahead (*****rc-lookahead*****)**
Set number of frames to look ahead for frametype and ratecontrol.
**weightb**
Enable weighted prediction for B-frames when set to 1. When set to 0, it has the same effect as x264’s --no-weightb option.
**weightp (*****weightp*****)**
Set weighted prediction method for P-frames. Possible values:
**‘****none (*****0*****)****’**
Disabled
**‘****simple (*****1*****)****’**
Enable only weighted refs
**‘****smart (*****2*****)****’**
Enable both weighted refs and duplicates
**ssim (*****ssim*****)**
Enable calculation and printing SSIM stats after the encoding.
**intra-refresh (*****intra-refresh*****)**
Enable the use of Periodic
定期的Intra Refresh instead of IDR frames when set to 1.
**avcintra-class (*****class*****)**
Configure the encoder to generate AVC-Intra. Valid values are 50,100 and 200
**bluray-compat (*****bluray-compat*****)**
Configure the encoder to be compatible with the bluray standard. It is a shorthand for setting "bluray-compat=1 force-cfr=1".
**b-bias (*****b-bias*****)**
Set the influence on how often B-frames are used.
**b-pyramid (*****b-pyramid*****)**
Set method for keeping of some B-frames as references. Possible values:
**‘****none (*****none*****)****’**
Disabled.
**‘****strict (*****strict*****)****’**
Strictly hierarchical pyramid.
**‘****normal (*****normal*****)****’**
Non-strict (not Blu-ray compatible).
**mixed-refs**
Enable the use of one reference per partition, as opposed to one reference per macroblock when set to 1. When set to 0, it has the same effect as x264’s --no-mixed-refs option.
**8x8dct**
Enable adaptive spatial transform (high profile 8x8 transform) when set to 1. When set to 0, it has the same effect as x264’s --no-8x8dct option.
**fast-pskip**
Enable early SKIP detection on P-frames when set to 1. When set to 0, it has the same effect as x264’s --no-fast-pskip option.
**aud (*****aud*****)**
Enable use of access unit delimiters when set to 1.
**mbtree**
Enable use macroblock tree ratecontrol when set to 1. When set to 0, it has the same effect as x264’s --no-mbtree option.
**deblock (*****deblock*****)**
Set loop filter parameters, in *alpha*:*beta* form.
**cplxblur (*****cplxblur*****)**
Set fluctuations reduction in QP (before curve compression).
**partitions (*****partitions*****)**
Set partitions to consider as a comma-separated list of. Possible values in the list:
**‘****p8x8****’**
8x8 P-frame partition.
**‘****p4x4****’**
4x4 P-frame partition.
**‘****b8x8****’**
4x4 B-frame partition.
**‘****i8x8****’**
8x8 I-frame partition.
**‘****i4x4****’**
4x4 I-frame partition. (Enabling ‘p4x4’
 requires ‘p8x8’ to be enabled. Enabling ‘i8x8’
 requires adaptive spatial transform (8x8dct option) to be
 enabled.)
**‘****none (*****none*****)****’**
Do not consider any partitions.
**‘****all (*****all*****)****’**
Consider every partition.
**direct-pred (*****direct*****)**
Set direct MV prediction mode. Possible values:
**‘****none (*****none*****)****’**
Disable MV prediction.
**‘****spatial (*****spatial*****)****’**
Enable spatial predicting.
**‘****temporal (*****temporal*****)****’**
Enable temporal predicting.
**‘****auto (*****auto*****)****’**
Automatically decided.
**slice-max-size (*****slice-max-size*****)**
Set the limit of the size of each slice in bytes. If not specified but RTP payload size (ps)
 is specified, that is used.
**stats (*****stats*****)**
Set the file name for multi-pass stats.
**nal-hrd (*****nal-hrd*****)**
Set signal HRD information (requires vbv-bufsize to
 be set). Possible values:
**‘****none (*****none*****)****’**
Disable HRD information signaling.
**‘****vbr****(*****vbr*****)****’**
Variable bit rate.
**‘****cbr****(*****cbr*****)****’**
Constant bit rate (not allowed in MP4 container).
**x264opts (N.A.)**
Set any x264 option, see x264 --fullhelp for
 a list.
Argument is a list of *key*=*value* couples
 separated by ":". In *filter* and *psy-rd* options
 that use ":" as a separator themselves, use "," instead. They accept
 it as well since long ago but this is kept undocumented for some reason.
For example to specify libx264 encoding options with ffmpeg:
ffmpeg -i foo.mpg -vcodec libx264 -x264opts keyint=123:min-keyint=20 -an out.mkv
**a53cc *****boolean***
Import closed captions (which must be ATSC compatible format) into output. Only the mpeg2 and h264 decoders provide these. Default is 1 (on).
**x264-params**** (N.A.)**
Override the x264 configuration using a :-separated list of key=value parameters.
This option is functionally
the same as the x264opts,
 but is duplicated for compatibility with the Libav fork.
For example to specify libx264 encoding options with ffmpeg:
ffmpeg -i INPUT -c:v libx264
-x264-params level=30:bframes=0:weightp=0:\cabac=0:ref=1:vbv-maxrate=768:vbv-bufsize=2000:analyse=all:me=umh:\no-fast-pskip=1:subq=6:8x8dct=0:trellis=0
 OUTPUT
Encoding ffpresets for common usages are provided so they can be used with the general presets system (e.g. passing the pre option).
CBR设置
为输出设置CBR,有三个参数必须设置为同一个值。
bitrate(-b option), minimal rate(-minrate), maximal rate(-maxrate)。maximal rate需要设置-bufsize选项。例如设置CBR为0.5Mbit/s。
ffmpeg -i in.avi -b 0.5M -minrate 0.5M -maxrate 0.5M -bufsize 1M
 output.mkv
- qscale的取值可以是0.01-255但实际使用超过50就很糟糕了
- -qscale 使用固定量化因子来量化视频这个是在vbr模式的,前面有提到,越小质量越好,不要超过50,相关的参数还有-qmin --qmax用来设定最大最小可使用的量化值
- -qdiff 指定固定量化器因子允许的最大偏差
qblur 指定量化器模糊系数，可用范围是0.0-1.0越大使得码率在时间上分配的越平均
- -qcomp 指定视频量化器压缩系数，默认0.5
上面文档中括号后面对应的是x264的选项
|FFmpeg option|x264 option|
|----|----|
|-g <frames>|--keyint|
|-b <bits per second>|--bitrate|
|-bufsize <bits>|--vbv-bufsize|
|-maxrate <bits>|--vbv-maxrate|
|-pass <1,2,3>|--pass|
|-crf <float>|--crf|
|-cqp <int>|--qp|
|-bf <int>|--bframes|
|-coder <0,1>|--no-cabac|
|-bframebias <int>|--b-bias|
|-keyint_min <int>|--min-keyint|
|-sc_threshold <int>|--scenecut|
|-deblockalpha <int>-deblockbeta <int>|--deblock|
|-qmin <int>|--qpmin|
|-qmax <int>|--qpmax|
|-qdiff <int>|--qpstep|
|-qcomp <float>|--qcomp|
|-qblur <float>|--qblur|
|-complexityblur <float>|--cplxblur|
|-refs <int>|--ref|
|-directpred <int>|--direct|
|-me_method <epzs,hex,umh,full>|--me|
|-me_range <int>|--merange|
|-subq <int>|--subme|
|-bidir_refine <0,1>|--bime|
|-trellis <0,1,2>|--trellis|
|-nr <int>|--nr|
|-level <int>|--level|
|-bt <bits>|--ratetol = -bt / -b|
|-rc_init_occupancy <bits>|--vbv-init = -rc_init_occupancy / -bufsize|
|-i_qfactor <float>|--ipratio = 1 / -i_qfactor|
|-b_qfactor <float>|--pbratio|
|-chromaoffset <int>|--chroma-qp-offset|
|-rc_eq <string>|--rc_eq|
|-threads <int>|--threads|
|-cmp <-chroma/+chroma>|--no-chroma-me|
|-partitions|--partitions|
|+parti8×8|i8×8|
|+parti4×4|i4×4|
|+partp8×8|p8×8|
|+partp4×4|p4×4|
|+partb8×8|b8×8|
|-flags||
|-loop/+loop|--no-deblock/–deblock|
|-psnr/+psnr|--no-psnr/nothing|
|-flags2||
|+bpyramid|--b-pyramid|
|+wpred|--weightb|
|+brdo|--b-rdo 我这里的ffmpeg已经不能用这个了|
|+mixed_refs|--mixed-refs|
|+dct8×8|--8×8dct|
|-fastpskip/+fastpskip|--no-fast-pskip|
|+aud|--aud|

[http://rhingheart.blogspot.com/2009/03/ffmpegx264-ffmpeg-x264.html](http://rhingheart.blogspot.com/2009/03/ffmpegx264-ffmpeg-x264.html)
[http://www.jianshu.com/p/f8c713f2e63d](http://www.jianshu.com/p/f8c713f2e63d)
