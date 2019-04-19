# ffmpeg使用x264编码的配置+ ffmpeg与 x264编码器参数完整对照表 - maopig的专栏 - CSDN博客
2011年07月16日 11:23:20[maopig](https://me.csdn.net/maopig)阅读数：2541
### [ffmpeg使用x264编码的配置+ ffmpeg与 x264编码器参数完整对照表](http://blog.csdn.net/wfqxx/article/details/5731328)
分类： [多媒体](http://blog.csdn.net/wfqxx/article/category/585962)2010-07-13 11:311072人阅读[评论](http://blog.csdn.net/wfqxx/article/details/5731328#comments)(0)收藏举报
转载自：**扶凯**[[http://www.php-oa.com](http://www.php-oa.com/)] 
本文链接: [http://www.php-oa.com/2009/03/09/ffmpeg_x264.html](http://www.php-oa.com/2009/03/09/ffmpeg_x264.html)
转帖:http://rhingheart.blogspot.com/2009/03/ffmpegx264-ffmpeg-x264.html
注:讲实话，有关这些使用的文章真的不错,我们在这谢谢"[SV的边界](http://rhingheart.blogspot.com/)"本来我自己还想总结一下的。看来没有必要了，呵呵，在次多谢"[SV的边界](http://rhingheart.blogspot.com/)".下面原文
使用ffmpeg进行.264编码的相关文章比较少，google了一下，特总结如下：
> - qscale的取值可以是0.01-255但实际使用超过50就很糟糕了 
- ffmpeg的cbr模式可以把码率控制的不错，但是vbr无法限制最高码率（虽然有max的设置，但是程序没有实现） 
- x264标准的封装是x264+aac in flv或者x264+aac in MP4 
接下来说明下ffmpeg命令行的语法规则（本块内容来自2009-03-02官方文档）：
语法规则结构：
`ffmpeg [[infile options][`-i' infile]]... {[outfile options] outfile}...`
一个最简单的命令形式：
`ffmpeg -i input.avi -b 64k output.avi`
这个命令把视频以64k的码率重编码。
显然，输入文件前面要加一个-i选项下面介绍一些有用的全局参数：
- -formats 参数。会显示你机器当前支持的封装、编码、解码器的信息 
- -y参数,会指示ffmpeg覆盖输出文件 
- -t 指定视频流持续的时常,支持以秒为单位的数字或"时:分:秒[.毫秒]" 
- -fs 指定输出文件大小的限制 
- -ss 指定开始的时间,和-t的单位一样 
- -target 直接设定你想要转成的目标格式,所有的相关设置都会采用内设值,当然也你也可以加上自己要修改的参数。可用的选择有:
"vcd", "svcd", "dvd", "dv", "dv50", "pal-vcd", "ntsc-svcd", …
这个例子把视频转换成vcd的格式 
ffmpeg -i myfile.avi -target vcd /tmp/vcd.mpg 
接下来介绍视频选项：
- -b 指定码率注意单位是bit/s,所以我们一般要加k,比如 -b 1000k 就是1000kb/s 
- -g 设置组的大小 
- -vframes 指定要编码的帧数,比如-vframes 1 就是编码1帧,截图的时候就这样写. 
- -r 指定帧率,默认是25 
- -s 指定图像分辨率,用wxh的格式,比如320×240 
- -aspect 指定宽高比 可以些16:9这种,也可以写小数比如1.3333 
- -croptop 指定顶部裁减多少像素,类似的还有-cropleft -cropright -cropbuttom 
- -bt 设置比特率容许的误差,默认4000k,在第一阶段时使用这个参数会告诉码率控制器能够偏移平均码率多远,这个选项和最大最小码率无关.设太小了不利于质量 
- -maxrate 和-minrate 指定允许的最大和最小码率,一般如果要用cbr模式编码的话会用这个:
`ffmpeg -i myfile.avi -b 4000k -minrate 4000k -maxrate 4000k -bufsize 1835k out.m2v`
否则用处不大 
- -vcodec 强制使用某种编码器 
- -sameq 使用和源文件相同的质量,这个选项是动态码率的 
- -pass 指定编码阶段,这个只有1和2,第一阶段的时候应该不处理音频,并且把输出导向空,比如: `ffmpeg -i foo.mov -vcodec libxvid -pass 1 -an -f rawvideo -y NULffmpeg -i foo.mov -vcodec libxvid -pass 1 -an -f rawvideo -y /dev/null`
- -qscale 使用固定量化因子来量化视频这个是在vbr模式的,前面有提到,越小质量越好,不要超过50,相关的参数还有-qmin –qmax用来设定最大最小可使用的量化值 
- -qdiff 指定固定量化器因子允许的最大偏差 
- -qblur 指定量化器模糊系数，可用范围是0.0-1.0越大使得码率在时间上分配的越平均 
- -qcomp 指定视频量化器压缩系数，默认0.5 
- -me_method 指定运动估计方法（motion estimation method），可用的选择从垃圾到好排列如下：
```
zero （0向量）
phods
log
x1
hex
umh
epzs （默认）
full （完全搜索，很慢又好不到哪里去）
```
- -mbd 设定宏模块决策，可用的值：
0 使用mb_cmp，不知道是什么东西，所以这些参数我参考一下mencoder里面的
1 选用需要比特最少的宏块模式
2 选用码率失真最优的宏块模式 
- -4mv 使用宏块的4个运动向量，只支持mpeg4 
- -part 使用数据划分，只支持mpeg4 
- -ilme 强制允许交错的编码，只支持mpeg2和mpeg4，你可以选择用-deinterlace来去交错 
音频部分:
- -ar 设置采样频率,默认44100hz 
- -ab 设置比特率,默认64k 
- -an 禁用音频录制 
- -acodec 指定音频编码器 
下面举几个x264编码的例子：
我使用mencoder调用x264编码一个psp用的视频：
x264+aac in mp4 (我修改过的，原作者的不能使用)
`mencoder test_video.vob -oac lavc -lavcopts acodec=libfaac:abitrate=94 -ovc x264 -x264encopts`/
```
cabac=1:ref=1:deblock=1,0,0:analyse=0x1,0x111:me=hex:subme=6:psy_rd=1.0,0.0:mixed_refs=0:me_range=32:chroma_me=1:trellis=1:8x/
8dct=0:no-chroma-me=0:chroma_qp_offset=-/
2:nr=0:dct_decimate=1:bframes=3:b_pyramid=0:b_adapt=1:b_bias=0:direct=3:keyint=250:keyint_min=25:scenecut=40:qp=18:bitrate=80/
0 -lavdopts er=2 -of lavf -lavfopts format=mp4 -vf scale=720:480 -o men.mp4
```
对应的ffmpeg编码参数是：
ffmpeg
`ffmpeg -i inputfile.avi -f psp -acodec libfaac -ab 94k -vcodec libx264 -cqp 28 -coder 1 -refs 3 -deblockalpha 1 -deblockbeta -1 -me_method umh -subq 9 -me_range 32 -trellis 2 -chromaoffset -2 -nr 0 -bf 2 -b_strategy 1 -bframebias 0 -directpred 3 -g 250 -i_qfactor 1.3 -b_qfactor 1.4 -flags2 +bpyramid+wpred+mixed_refs+8x8dct -er 2 -s 480x320`
需要注意的是,flags2里面那块,似乎要按照一定顺序才能正常工作,其他地方都差不多,详细情况可以从下面两篇文章得出:
第一篇是这个在网上被传了很多,但有些问题的对照表格,我修改了有问题的部分:
|FFmpeg option|x264 option|
|----|----|
|-g <frames>|–keyint|
|-b <bits per second>|–bitrate|
|-bufsize <bits>|–vbv-bufsize|
|-maxrate <bits>|–vbv-maxrate|
|-pass <1,2,3>|–pass|
|-crf <float>|–crf|
|-cqp <int>|–qp|
|-bf <int>|–bframes|
|-coder <0,1>|–no-cabac|
|-bframebias <int>|–b-bias|
|-keyint_min <int>|–min-keyint|
|-sc_threshold <int>|–scenecut|
|-deblockalpha <int>-deblockbeta <int>|–deblock|
|-qmin <int>|–qpmin|
|-qmax <int>|–qpmax|
|-qdiff <int>|–qpstep|
|-qcomp <float>|–qcomp|
|-qblur <float>|–qblur|
|-complexityblur <float>|–cplxblur|
|-refs <int>|–ref|
|-directpred <int>|–direct|
|-me_method <epzs,hex,umh,full>|–me|
|-me_range <int>|–merange|
|-subq <int>|–subme|
|-bidir_refine <0,1>|–bime|
|-trellis <0,1,2>|–trellis|
|-nr <int>|–nr|
|-level <int>|–level|
|-bt <bits>|–ratetol = -bt / -b|
|-rc_init_occupancy <bits>|–vbv-init = -rc_init_occupancy / -bufsize|
|-i_qfactor <float>|–ipratio = 1 / -i_qfactor|
|-b_qfactor <float>|–pbratio|
|-chromaoffset <int>|–chroma-qp-offset|
|-rc_eq <string>|–rc_eq|
|-threads <int>|–threads|
|-cmp <-chroma/+chroma>|–no-chroma-me|
|**-partitions**|–partitions|
|+parti8×8|i8×8|
|+parti4×4|i4×4|
|+partp8×8|p8×8|
|+partp4×4|p4×4|
|+partb8×8|b8×8|
|**-flags**||
|-loop/+loop|–no-deblock/–deblock|
|-psnr/+psnr|–no-psnr/nothing|
|**-flags2**||
|+bpyramid|–b-pyramid|
|+wpred|–weightb|
|+brdo|–b-rdo 我这里的ffmpeg已经不能用这个了|
|+mixed_refs|–mixed-refs|
|+dct8×8|–8×8dct|
|-fastpskip/+fastpskip|–no-fast-pskip|
|+aud|–aud|
下面是一篇关于这些参数详细解释的文章,转自:http://ffmpeg.x264.googlepages.com/mapping
一下是这篇文章在2008年11月19日更新的版本:
> 
### Frame-type options:
- –keyint <integer> (x264)**-g <integer>** (FFmpeg)
Keyframe interval, also known as GOP length. This determines the maximum distance between I-frames. Very high GOP lengths will result in slightly more efficient compression, but will make seeking in the video somewhat more difficult. Recommended default: 250 
- –min-keyint <integer> (x264)
**-keyint_min <integer>** (FFmpeg)
Minimum GOP length, the minimum distance between I-frames. Recommended default: 25 
- –scenecut <integer> (x264)
**-sc_threshold <integer>** (FFmpeg)
Adjusts the sensitivity of x264′s scenecut detection. Rarely needs to be adjusted. Recommended default: 40 
- –pre-scenecut (x264)
**UNKNOWN** (FFmpeg)
Slightly faster (but less precise) scenecut detection. Normal scenecut detection decides whether a frame is a scenecut after the frame is encoded, and if so then re-encodes the frame as an I-frame. This is not compatible with threading, however, and so –pre-scenecut is automatically activated when multiple encoding threads are used. 
- –bframes <integer> (x264)
**-bf <integer>** (FFmpeg)
B-frames are a core element of H.264 and are more efficient in H.264 than any previous standard. Some specific targets, such as HD-DVD and Blu-Ray, have limitations on the number of consecutive B-frames. Most, however, do not; as a result, there is rarely any negative effect to setting this to the maximum (16) since x264 will, if B-adapt is used, automatically choose the best number of B-frames anyways. This parameter simply serves to limit the max number of B-frames. Note that Baseline Profile, such as that used by iPods, does not support B-frames. Recommended default: 16 
- –b-adapt <integer> (x264)
**-b_strategy <integer>** (FFmpeg)
x264, by default, adaptively decides through a low-resolution lookahead the best number of B-frames to use. It is possible to disable this adaptivity; this is not recommended. Recommended default: 1 
0: Very fast, but not recommended. Does not work with pre-scenecut (scenecut must be off to force off b-adapt).
1: Fast, default mode in x264. A good balance between speed and quality.
2: A much slower but more accurate B-frame decision mode that correctly detects fades and generally gives considerably better quality. Its speed gets considerably slower at high bframes values, so its recommended to keep bframes relatively low (perhaps around 3) when using this option. It also may slow down the first pass of x264 when in threaded mode.
- –b-bias 0 (x264)
**-bframebias 0** (FFmpeg)
Make x264 more likely to choose higher numbers of B-frames during the adaptive lookahead. Not generally recommended. Recommended default: 0 
- –b-pyramid (x264)
**-flags2 +bpyramid** (FFmpeg)
Allows B-frames to be kept as references. The name is technically misleading, as x264 does not actually use pyramid coding; it simply adds B-references to the normal reference list. B-references get a quantizer halfway between that of a B-frame and P-frame. This setting is generally beneficial, but it increases the DPB (decoding picture buffer) size required for playback, so when encoding for hardware, disabling it may help compatibility. 
- –no-cabac (x264)
**-coder 0,1 **(FFmpeg)
CABAC is the default entropy encoder used by x264. Though somewhat slower on both the decoding and encoding end, it offers 10-15% improved compression on live-action sources and considerably higher improvements on animated sources, especially at low bitrates. It is also required for the use of trellis quantization. Disabling CABAC may somewhat improve decoding performance, especially at high bitrates. CABAC is not allowed in Baseline Profile. Recommended default: -coder 1 (CABAC enabled) 
- –ref <integer> (x264)
**-refs <integer>** (FFmpeg)
One of H.264′s most useful features is the abillity to reference frames other than the one immediately prior to the current frame. This parameter lets one specify how many references can be used, through a maximum of 16. Increasing the number of refs increases the DPB (Decoded Picture Buffer) requirement, which means hardware playback devices will often have strict limits to the number of refs they can handle. In live-action sources, more reference have limited use beyond 4-8, but in cartoon sources up to the maximum value of 16 is often useful. More reference frames require more processing power because every frame is searched by the motion search (except when an early skip decision is made). The slowdown is especially apparent with slower motion estimation methods. Recommended default: -refs 6 
- –no-deblock (x264)
**-flags -loop **(FFmpeg)
Disable loop filter. Recommended default: -flags +loop (Enabled) 
- –deblock (x264)
**-deblockalpha <integer>**(FFmpeg)
**-deblockbeta <integer>**(FFmpeg)
One of H.264′s main features is the in-loop deblocker, which avoids the problem of blocking artifacts disrupting motion estimation. This requires a small amount of decoding CPU, but considerably increases quality in nearly all cases. Its strength may be raised or lowered in order to avoid more artifacts or keep more detail, respectively. Deblock has two parameters: alpha (strength) and beta (threshold). Recommended defaults:-deblockalpha 0 -deblockbeta 0 (Must have ‘-flags +loop’) 
- –interlaced (x264)
**UNKNOWN **(FFmpeg)
Enables interlaced encoding. x264′s interlaced encoding is not as efficient as its progressive encoding; consider deinterlacing for maximum effectiveness. 
### Ratecontrol:
- –qp <integer>(x264)
**-cqp <integer>**(FFmpeg)
Constant quantizer mode. Not exactly constant completely–B-frames and I-frames have different quantizers from P-frames. Generally should not be used, since CRF gives better quality at the same bitrate. 
- –bitrate <integer>(x264)
**-b <integer>**(FFmpeg)
Enables target bitrate mode. Attempts to reach a specific bitrate. Should be used in 2-pass mode whenever possible; 1-pass bitrate mode is generally the worst ratecontrol mode x264 has. 
- –crf <float>(x264)
**-crf <float>**(FFmpeg)
Constant quality mode (also known as constant ratefactor). Bitrate corresponds approximately to that of constant quantizer, but gives better quality overall at little speed cost. The best one-pass option in x264. 
- –vbv-maxrate <integer>(x264)
**-maxrate <integer>**(FFmpeg)
Specifies the maximum bitrate at any point in the video. Requires the VBV buffersize to be set. This option is generally used when encoding for a piece of hardware with bitrate limitations. 
- –vbv-bufsize <integer>(x264)
**-bufsize <integer>** (FFmpeg)
Depends on the profile level of the video being encoded. Set only if you’re encoding for a hardware device. 
- –vbv-init <float>(x264)
**-rc_init_occupancy <float>**(FFmpeg)
Initial VBV buffer occupancy. Note: Don’t mess with this. 
- –qpmin <integer> (x264)
**-qmin <integer>** (FFmpeg)
Minimum quantizer. Doesn’t need to be changed. Recommended default: -qmin 10 
- –qpmax <integer> (x264)
**-qmax <integer>** (FFmpeg)
Maximum quantizer. Doesn’t need to be changed. Recommended default: -qmax 51 
- –qpstep <integer> (x264)
**-qdiff <integer>** (FFmpeg)
Set max QP step. Recommended default: -qdiff 4 
- –ratetol <float>(x264)
**-bt <float>**(FFmpeg)
Allowed variance of average bitrate 
- –ipratio <float>(x264)
**-i_qfactor <float>**(FFmpeg)
Qscale difference between I-frames and P-frames. 
- –pbratio <float>(x264)
**-b_qfactor <float>**(FFmpeg)
Qscale difference between P-frames and B-frames. 
- –chroma-qp-offset <integer>(x264)
**-chromaoffset <integer>**(FFmpeg)
QP difference between chroma and luma. 
- –aq-strength <float> (x264)
**UNKNOWN** (FFmpeg)
Adjusts the strength of adaptive quantization. Higher values take more bits away from complex areas and edges and move them towards simpler, flatter areas to maintain fine detail. Default: 1.0 
- –pass <1,2,3> (x264)
**-pass <1,2,3>**(FFmpeg)
Used with –bitrate. Pass 1 writes the stats file, pass 2 reads it, and 3 both reads and writes it. If you want to use three pass, this means you will have to use –pass 1 for the first pass, –pass 3 for the second, and –pass 2 or 3 for the third. 
- –stats <string>(x264)
**UNKNOWN **(FFmpeg)
Allows setting a specific filename for the firstpass stats file. 
- –rceq <string>(x264)
**-rc_eq <string>**(FFmpeg)
Ratecontrol equation. Recommended default: -rc_eq ‘blurCplx^(1-qComp)’ 
- –qcomp <float>(x264)
**-qcomp <float>** (FFmpeg)
QP curve compression: 0.0 => CBR, 1.0 => CQP. Recommended default: -qcomp 0.60 
- –cplxblur <float>(x264)
**-complexityblur <float>**(FFmpeg)
Reduce fluctuations in QP (before curve compression) [20.0] 
- –qblur <float>(x264)
**-qblur <float>**(FFmpeg)
Reduce fluctuations in QP (after curve compression) [0.5] 
- –zones /(x264)
**UNKNOWN **(FFmpeg)
Allows setting a specific quantizer for a specific region of video. 
- –qpfile (x264)
**UNKNOWN **(FFmpeg)
Allows one to read in a set of frametypes and quantizers from a file. Useful for testing various encoding options while ensuring the exact same quantizer distribution. 
### Analysis:
- –partitions <string> (x264)
**-partitions <string> **(FFmpeg) 
p8x8 (x264) /**+partp8x8 (FFmpeg) **
p4x4 (x264) /**+partp4x4 (FFmpeg)**
b8x8 (x264) /**+partb8x8 (FFmpeg)**
i8x8 (x264) /**+parti8x8 (FFmpeg)**
i4x4 (x264) /**+parti4x4 (FFmpeg)**
One of H.264′s most useful features is the ability to choose among many combinations of inter and intra partitions. P-macroblocks can be subdivided into 16×8, 8×16, 8×8, 4×8, 8×4, and 4×4 partitions. B-macroblocks can be divided into 16×8, 8×16, and 8×8 partitions. I-macroblocks can be divided into 4×4 or 8×8 partitions. Analyzing more partition options improves quality at the cost of speed. The default is to analyze all partitions except p4x4 (p8x8, i8x8, i4x4, b8x8), since p4x4 is not particularly useful except at high bitrates and lower resolutions. Note that i8x8 requires 8x8dct, and is therefore a High Profile-only partition. p8x8 is the most costly, speed-wise, of the partitions, but also gives the most benefit. Generally, whenever possible, all partition types except p4x4 should be used. 
- –direct <integer>(x264)
**-directpred <integer>**(FFmpeg)
B-frames in H.264 can choose between spatial and temporal prediction mode. Auto allows x264 to pick the best of these; the heuristic used is whichever mode allows more skip macroblocks. Auto should generally be used. 
- –direct-8×8 (x264)
**UNKONWN **(FFmpeg)
This should be left at the default (-1). 
- –weightb (x264)
**-flags2 +wpred**(FFmpeg)
This allows B-frames to use weighted prediction options other than the default. There is no real speed cost for this, so it should always be enabled. 
- –me 
- –merange <integer>(x264)
**-me_range <integer>**(FFmpeg)
MErange controls the max range of the motion search. For HEX and DIA, this is clamped to between 4 and 16, with a default of 16. For UMH and ESA, it can be increased beyond the default 16 to allow for a wider-range motion search, which is useful on HD footage and for high-motion footage. Note that for UMH and ESA, increasing MErange will significantly slow down encoding. 
- –mvrange(x264)
**UNKNOWN **(FFmpeg)
Limits the maximum motion vector range. Since x264 by default limits this to 511.75 for standards compliance, this should not be changed. 
- –subme 6(x264)
**-subq 6**(FFmpeg) 
1: Fastest, but extremely low quality. Should be avoided except on first pass encoding.
2-5: Progressively better and slower, 5 serves as a good medium for higher speed encoding.
6-7: 6 is the default. Activates rate-distortion optimization for partition decision. This can considerably improve efficiency, though it has a notable speed cost. 6 activates it in I/P frames, and subme7 activates it in B frames.
8-9: Activates rate-distortion refinement, which uses RDO to refine both motion vectors and intra prediction modes. Slower than subme 6, but again, more efficient.
An extremely important encoding parameter which determines what algorithms are used for both subpixel motion searching and partition decision. 
- –psy-rd <float>:<float> (x264)
**UNKNOWN** (FFmpeg)
First value represents the amount that x264 biases in favor of detail retention instead of max PSNR in mode decision. Requires subme >= 6. Second value is psy-trellis, an experimental algorithm that tries to improve sharpness and detail retention at the expense of more artifacting. Recommended starting values are 0.1-0.2. Requires trellis >= 1. Recommended default: 1.0:0.0 
- –mixed-refs(x264)
**-flags2 +mixed_refs**(FFmpeg)
H.264 allows p8x8 blocks to select different references for each p8x8 block. This option allows this analysis to be done, and boosts quality with little speed impact. It should generally be used, though it obviously has no effect with only one reference frame. 
- –no-chroma-me(x264)
**UNKNOWN **(FFmpeg)
Chroma is used in the last steps of the subpixel refinement by default. For a slight speed increase, this can be disabled (at the cost of quality). 
- –8x8dct (x264)
**-flags2 +dct8x8**(FFmpeg)
Gives a notable quality boost by allowing x264 to choose between 8×8 and 4×4 frequency transform size. Required for i8x8 partitions. Speed cost for this option is near-zero both for encoding and decoding; the only reason to disable it is when one needs support on a device not compatible with High Profile. 
- –trellis <0,1,2>(x264)
**-trellis <0,1,2>**(FFmpeg) 
0: disabled
1: enabled only on the final encode of a MB
2: enabled on all mode decisions
The main decision made in quantization is which coefficients to round up and which to round down. Trellis chooses the optimal rounding choices for the maximum rate-distortion score, to maximize PSNR relative to bitrate. This generally increases quality relative to bitrate by about 5% for a somewhat small speed cost. It should generally be enabled. Note that trellis requires CABAC. 
- –no-fast-pskip(x264)
**-flags2 -fastpskip**(FFmpeg)
By default, x264 will skip macroblocks in P-frames that don’t appear to have changed enough between two frames to justify encoding the difference. This considerably speeds up encoding. However, for a slight quality boost, P-skip can be disabled. In this case, the full analysis will be done on all P-blocks, and the only skips in the output stream will be the blocks whose motion vectors happen to match that of the skip vector and motion vectors happen to match that of the skip vector and which have no residual. The speed cost of enabling no-fast-pskip is relatively high, especially with many reference frames. There is a similar B-skip internal to x264, which is why B-frames generally encode much faster than P-frames, but it cannot be disabled on the commandline. 
- –no-dct-decimate(x264)
**UNKNOWN **(FFmpeg)
By default, x264 will decimate (remove all coefficients from) P-blocks that are extremely close to empty of coefficents. This can improve overall efficiency with little visual cost, but may work against an attempt to retain grain or similar. DCT decimation should be left on unless there’s a good reason to disable it. 
- –nr(x264)
**UNKNOWN **(FFmpeg)
a fast, built-in noise reduction routine. Not as effective as external filters such as hqdn3d, but faster. Since x264 already naturally reduces noise through its quantization process, this parameter is not usually necessary. 
- –deadzone-inter(264)
–deadzone-intra (x264)
**UNKNOWN **(FFmpeg)
**UNKNOWN **(FFmpeg)
When trellis isn’t activated, deadzone parameters determine how many DCT coefficients are rounded up or down. Rounding up results in higher quality and more detail retention, but costs more bits–so rounding is a balance between quality and bit cost. Lowering these settings will result in more coefficients being rounded up, and raising the settings will result in more coefficients being rounded down. Recommended: keep them at the defaults. 
- –cqm (264)
–cqpfile(x264)**UNKNOWN **(FFmpeg)
**UNKNOWN **(FFmpeg)
Allows the use of a custom quantization matrix to weight frequencies differently in the quantization process. The presets quant matrices are "jvt" and "flat". –cqpfile reads a custom quant matrices from a JM-compatible file. Recommended only if you know what you’re doing. 
