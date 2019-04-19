# FFMPEG使用参数详解 - maopig的专栏 - CSDN博客
2011年07月16日 10:01:13[maopig](https://me.csdn.net/maopig)阅读数：2582
### [FFMPEG使用参数详解](http://blog.csdn.net/axdc_qa_team/article/details/4204358)
目前又开始测MPEG1和MPEG2的解码，在准备编码好的测试序列过程中，需要通过编码器来进行原始视频序列的编码操作，以生成我们需要的测试数据。由于IPP example提供的Encoder不支持MPEG1，经过查询，决定MPEG1的编码使用FFMPEG。FFMEPG是个啥子东西呢，这里简单说一下：FFmpeg是用于录制、转换和流化音频和视频的完整解决方案，一套领先的音/视频编解码类库。
FFmpeg的官方网址是 [http://ffmpeg.mplayerhq.hu/](http://ffmpeg.mplayerhq.hu/) 。
中文Wiki是 [http://www.ffmpeg.com.cn/](http://www.ffmpeg.com.cn/)  ，资料很多。
    由于其免费开源性，遂决定就用它来进行视频格式的压缩转换啦。首先再来啰嗦一点儿基本的FFmpeg主主要组成部分：
    1). libavcodec： 一个包含了所有FFmpeg音视频编解码器的库。
    2). libavformat： 一个包含了所有的普通音视格式的解析器和产生器的库。
三个实例程序(这三个实例基本可以作为API使用手册)：
ffmpeg：命令行的视频格式转换程序。 
ffplay：视频播放程序。（需要SDL支持） 
ffserver：多媒体服务器 
了解了它的大体组织结构，就明确了ffmpeg当然是我在寻找视频格式压缩转换的第一选择！
    由于FFmpeg 在Linux上开发的开源项目，把它放在windows下进行编译实在是比较复杂啊，这个过程往往弄得新手很是头大！！！，比如鄙人。好了，今天的重点不在于ffmpeg的编译，因而就不在此一一记录了，网上去找，关于ffmpeg编译的方法何其之多，这里给大家几个不错的参考链接：
   对于Linux上的用户，可以参考
[http://www.ffmpeg.com.cn/index.php/Ffmpeg%E7%BC%96%E8%AF%91%E8%AF%A6%E8%A7%A3](http://www.ffmpeg.com.cn/index.php/Ffmpeg%E7%BC%96%E8%AF%91%E8%AF%A6%E8%A7%A3)
   对于Windows用户，可参考
[http://bbs.chinavideo.org/viewthread.php?tid=1897&extra=page%3D1](http://bbs.chinavideo.org/viewthread.php?tid=1897&extra=page%3D1)  或
[http://blog.sina.com.cn/s/blog_4673bfa501008xie.html](http://blog.sina.com.cn/s/blog_4673bfa501008xie.html)
   总之编译是个很复杂的过程，我们还是加快速度，来领略ffmpeg的风采。
   在命令行下，键入ffmepg.exe -h ，以看其帮助，好家伙，其参数之多，还真是觉得无从下手啊，这么多参数，又该如何正确的设置呢？
   于是连Google带使用摸索，将ffmpeg的参数在这里做一简要整理和记录，以备日后使用时查看方便。
   基本使用方式：ffmpeg [[options][`-i' input_file]] {[options] output_file}
**a) 通用选项**
      -L license
      -h 帮助
      -fromats 显示可用的格式，编解码的，协议的。。。
      -f fmt 强迫采用格式fmt
      -i filename 输入文件
      -y 覆盖输出文件（即如果test.***文件已经存在的话，不经提示就覆盖掉了）
      -t duration 设置纪录时间 hh:mm:ss[.xxx]格式的记录时间也支持
      -ss position 搜索到指定的时间 [-]hh:mm:ss[.xxx]的格式也支持。使用-ss参数的作用，可以从指定时间点开始转换任务，-ss后的时间单位为秒
      -title string 设置标题（比如PSP中显示影片的标题）
      -author string 设置作者
      -copyright string 设置版权
      -comment string 设置评论
      -target type 设置目标文件类型(vcd,svcd,dvd) 所有的格式选项（比特率，编解码以及缓冲区大小）自动设置 ，只需要输入如下的就可以了：ffmpeg -i myfile.avi -target vcd vcd.mpg
      -hq 激活高质量设置
      -itsoffset offset 设置以秒为基准的时间偏移，该选项影响所有后面的输入文件。该偏移被加到输入文件的时戳，定义一个正偏移意味着相应的流被延迟了 offset秒。 [-]hh:mm:ss[.xxx]的格式也支持。
**b) 视频选项**
      -b bitrate 设置比特率，缺省200kb/s
      -vb bitrate   set bitrate (in bits/s)
      -vframes number  设置要编码多少帧 
      -r fps 设置帧频 缺省25
      -s size 设置帧大小 格式为W*H 缺省160X128.也可以直接使用简写，也认：Sqcif  qcif  cif 4cif 等
      -aspect aspect 设置横纵比 4:3 16:9 或 1.3333 1.7777
      -croptop size 设置顶部切除带大小 像素单位
      -cropbottom size -cropleft size -cropright size
      -padtop size 设置顶部补齐的大小 像素单位
      -padbottom size -padleft size -padright size -padcolor color 设置补齐条颜色(hex,6个16进制的数，红:绿:兰排列，比如 000000代表黑色)
      -vn 不做视频记录
**-bt tolerance **设置视频码率容忍度kbit/s （固定误差）
**-maxrate bitrate**设置最大视频码率容忍度 （可变误差）
**-minrate bitreate **设置最小视频码率容忍度（可变误差）
**-bufsize size** 设置码率控制缓冲区大小
      -vcodec codec 强制使用codec编解码方式，如-vcodec xvid 使用xvid压缩 如果用copy表示原始编解码数据必须被拷贝。
      -sameq 使用同样视频质量作为源（VBR）
      -pass n 选择处理遍数（1或者2）。两遍编码非常有用。第一遍生成统计信息，第二遍生成精确的请求的码率
      -passlogfile file 选择两遍的纪录文件名为file
**c)高级视频选项**
-g gop_size 设置图像组大小 这里设置GOP大小，也表示两个I帧之间的间隔
-intra 仅适用帧内编码
**-qscale q**  使用固定的视频量化标度(VBR)  以<q>质量为基础的VBR，取值0.01-255，约小质量越好，即qscale 4和-qscale 6，4的质量比6高 。此参数使用次数较多，实际使用时发现，qscale是种固定量化因子，设置qscale之后，前面设置的-b好像就无效了，而是自动调整了比特率。
-qmin q 最小视频量化标度(VBR) 设定最小质量，与-qmax（设定最大质量）共用
-qmax q 最大视频量化标度(VBR) 使用了该参数，就可以不使用qscale参数
-qdiff q 量化标度间最大偏差 (VBR)
-qblur blur 视频量化标度柔化(VBR)
-qcomp compression 视频量化标度压缩(VBR)
-rc_init_cplx complexity 一遍编码的初始复杂度
-b_qfactor factor 在p和b帧间的qp因子
-i_qfactor factor 在p和i帧间的qp因子
-b_qoffset offset 在p和b帧间的qp偏差
-i_qoffset offset 在p和i帧间的qp偏差
-rc_eq equation 设置码率控制方程 默认tex^qComp
-rc_override override 特定间隔下的速率控制重载
-me method 设置运动估计的方法 可用方法有 zero phods log x1 epzs(缺省) full
-dct_algo algo 设置dct的算法 可用：
*0 FF_DCT_AUTO 缺省的DCT  *
*1 FF_DCT_FASTINT*
*2 FF_DCT_INT*
*3 FF_DCT_MMX*
*4 FF_DCT_MLIB*
*5 FF_DCT_ALTIVEC*
-idct_algo algo 设置idct算法。可用的有：
* 0 FF_IDCT_AUTO 缺省的IDCT*
*1 FF_IDCT_INT  *
*2 FF_IDCT_SIMPLE  *
*3 FF_IDCT_SIMPLEMMX  *
*4 FF_IDCT_LIBMPEG2MMX *
*5 FF_IDCT_PS2  *
*6 FF_IDCT_MLIB *
*7 FF_IDCT_ARM *
*8 FF_IDCT_ALTIVEC *
*9 FF_IDCT_SH4 *
*10 FF_IDCT_SIMPLEARM*
-er n 设置错误残留为n 
*1 FF_ER_CAREFULL 缺省 *
*2 FF_ER_COMPLIANT *
*3 FF_ER_AGGRESSIVE *
*4 FF_ER_VERY_AGGRESSIVE *
-ec bit_mask 设置错误掩蔽为bit_mask,该值为如下值的位掩码 1 FF_EC_GUESS_MVS (default=enabled) 2 FF_EC_DEBLOCK (default=enabled) 
**-bf frames 使用frames个B 帧，支持mpeg1,mpeg2,mpeg4（即如果-bf 2的话，在两个非b帧中间隔的b帧数目为2，即IBBPBBPBBP结构）**
-mbd mode 宏块决策 
*0 FF_MB_DECISION_SIMPLE 使用mb_cmp *
*1 FF_MB_DECISION_BITS 2 FF_MB_DECISION_RD*
-4mv 使用4个运动矢量 仅用于mpeg4
-part 使用数据划分 仅用于mpeg4
-bug param 绕过没有被自动监测到编码器的问题
-strict strictness 跟标准的严格性
-aic 使能高级帧内编码 h263+
-umv 使能无限运动矢量 h263+
-deinterlace 不采用交织方法
-interlace 强迫交织法编码 仅对mpeg2和mpeg4有效。当你的输入是交织的并且你想要保持交织以最小图像损失的时候采用该选项。可选的方法是不交织，但是损失更大
-psnr 计算压缩帧的psnr
-vstats 输出视频编码统计到vstats_hhmmss.log
-vhook module 插入视频处理模块 module 包括了模块名和参数，用空格分开
-bitexact 使用标准比特率
-max_qdiff  视频中所有桢（包括i/b/P）的最大Q值差距 
-b_qfactor 表示i/p与B的Q值比例因子,值越大B桢劣化越严重
-b_qoffset 表示1/p与B的Q值比例的偏移量,值越大B桢劣化越严重.如果大于0,那么下一个B的Q=前一个P的Q乘以b_quant_factor再加上offset,如果小于0,则B的Q=负的normal_Q乘以factor加上offset.
-i_qfactor   p和i的Q值比例因子,越接近1则P越优化.
-i_qoffset   p和i的Q的偏移量
**D)音频选项**
   -ab bitrate 设置音频码率
   -ar freq 设置音频采样率
   -ac channels 设置通道 缺省为1，即单通道
   -an 不使能音频纪录
   -acodec codec 使用codec编解码 如：-acodec AAC 使用AAC音频编码
