# H264中的SPS、PPS提取与作用 - DoubleLi - 博客园






**牛逼的视频会议网站：[http://wmnmtm.blog.163.com/blog/#m=0](http://wmnmtm.blog.163.com/blog/#m=0)**

++++++++++++++++++++++++++++++++++++++++++++++++++++

[http://wmnmtm.blog.163.com/blog/static/38245714201192491746701/](http://wmnmtm.blog.163.com/blog/static/38245714201192491746701/)

使用RTP传输H264的时候,需要用到sdp协议描述,其中有两项:Sequence Parameter Sets (SPS) 和Picture Parameter Set (PPS)需要用到,那么这两项从哪里获取呢?答案是从H264码流中获取.在H264码流中,都是以"0x00 0x00 0x01"或者"0x00 0x00 0x00 0x01"为开始码的,找到开始码之后,使用开始码之后的第一个字节的低5位判断是否为7(sps)或者8(pps), 及data[4] & 0x1f == 7 || data[4] & 0x1f == 8.然后对获取的nal去掉开始码之后进行base64编码,得到的信息就可以用于sdp.sps和pps需要用逗号分隔开来.

+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++



# [如何解析SDP中包含的H.264的SPS和PPS串](http://www.pernet.tv.sixxs.org/thread-109-1-1.html)



[http://www.pernet.tv.sixxs.org/thread-109-1-1.html](http://www.pernet.tv.sixxs.org/thread-109-1-1.html)

SDP中的H.264的SPS和PPS串，包含了初始化H.264解码器所需要的信息参数，包括编码所用的profile，level，图像的宽和高，deblock滤波器等。
由于SDP中的SPS和PPS都是BASE64编码形式的，不容易理解，附件有一个工具软件可以对SDP中的SPS和PPS进行解析。
用法是在命令行中输入：
spsparser sps.txt pps.txt output.txt

例如sps.txt中的内容为：
Z0LgFNoFglE=
pps.txt中的内容为：
aM4wpIA=

最终解析的到的结果为：

Start dumping SPS:
  profile_idc = 66
  constrained_set0_flag = 1
  constrained_set1_flag = 1
  constrained_set2_flag = 1
  constrained_set3_flag = 0
  level_idc = 20
  seq_parameter_set_id = 0
  chroma_format_idc = 1
  bit_depth_luma_minus8 = 0
  bit_depth_chroma_minus8 = 0
  seq_scaling_matrix_present_flag = 0
  log2_max_frame_num_minus4 = 0
  pic_order_cnt_type = 2
  log2_max_pic_order_cnt_lsb_minus4 = 0
  delta_pic_order_always_zero_flag = 0
  offset_for_non_ref_pic = 0
  offset_for_top_to_bottom_field = 0
  num_ref_frames_in_pic_order_cnt_cycle = 0
  num_ref_frames = 1
  gaps_in_frame_num_value_allowed_flag = 0
  pic_width_in_mbs_minus1 = 21
  pic_height_in_mbs_minus1 = 17
  frame_mbs_only_flag = 1
  mb_adaptive_frame_field_flag = 0
  direct_8x8_interence_flag = 0
  frame_cropping_flag = 0
  frame_cropping_rect_left_offset = 0
  frame_cropping_rect_right_offset = 0
  frame_cropping_rect_top_offset = 0
  frame_cropping_rect_bottom_offset = 0
  vui_parameters_present_flag = 0

Start dumping PPS:
  pic_parameter_set_id = 0
  seq_parameter_set_id = 0
  entropy_coding_mode_flag = 0
  pic_order_present_flag = 0
  num_slice_groups_minus1 = 0
  slice_group_map_type = 0
  num_ref_idx_l0_active_minus1 = 0
  num_ref_idx_l1_active_minus1 = 0
  weighted_pref_flag = 0
  weighted_bipred_idc = 0
  pic_init_qp_minus26 = 0
  pic_init_qs_minus26 = 0
  chroma_qp_index_offset = 10
  deblocking_filter_control_present_flag = 1
  constrained_intra_pred_flag = 0
  redundant_pic_cnt_present_flag = 0
  transform_8x8_mode_flag = 0
  pic_scaling_matrix_present_flag = 0
  second_chroma_qp_index_offset = 10

/////////////////////////////////////////////////////////////////////////////////////////////////
这里需要特别提一下这两个参数
pic_width_in_mbs_minus1 = 21
  pic_height_in_mbs_minus1 = 17
分别表示图像的宽和高，以宏块（16x16）为单位的值减1
因此，实际的宽为 (21+1)*16 = 352
![](http://www.pernet.tv.sixxs.org/static/image/filetype/rar.gif)[spsparser.rar](http://www.pernet.tv.sixxs.org/forum.php?mod=attachment&aid=MTk4fGQ5N2EzMGI5fDEzMzIxMjUxMzh8MHwxMDk%3D)




++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


[http://krdai.info.sixxs.org/blog/mp4-sps-pps-data.html](http://krdai.info.sixxs.org/blog/mp4-sps-pps-data.html)

最近在做跟 h264 encode/decode 相關的研究，目標是希望可以從 [Android](http://lib.csdn.net/base/15) 的 MediaRecorder 當中取出 h264 的資訊。目前問題是在於 SPS 以及 PPS 到底要怎樣得到。由於 MediaRecorder 是寫入 mp4 檔案中，所以不得已只好來去分析一下 mp4 的檔案格式，發現沒有想像中的困難. 主要是參照 ISO/IEC 14496-15 這部份. 在 mp4 的檔案之中, 找到 avcC 這個字串, 之後就是接上 AVCDecoderConfigurationRecord. AVCDecoderConfigurationRecord 的 format 如下:





**[cpp]**[view plain](http://blog.csdn.net/scut1135/article/details/7367641#)[copy](http://blog.csdn.net/scut1135/article/details/7367641#)





- aligned(8) class AVCDecoderConfigurationRecord {  
-    unsigned int(8) configurationVersion = 1;  
-    unsigned int(8) AVCProfileIndication;  
-    unsigned int(8) profile_compatibility;  
-    unsigned int(8) AVCLevelIndication;  
- 
- bit(6) reserved = '111111'b;  
-    unsigned int(2) lengthSizeMinusOne;  
- 
- bit(3) reserved = '111'b;  
-    unsigned int(5) numOfSequenceParameterSets;  
- 
- for (i=0; i< numOfSequenceParameterSets; i++) {  
-       unsigned int(16) sequenceParameterSetLength ;  
-       bit(8*sequenceParameterSetLength) sequenceParameterSetNALUnit;  
-    }  
-    unsigned int(8) numOfPictureParameterSets;  
- for (i=0; i< numOfPictureParameterSets; i++) {  
-       unsigned int(16) pictureParameterSetLength;  
-       bit(8*pictureParameterSetLength) pictureParameterSetNALUnit;  
-    }  
- }  





對照一下這樣就可以找到 SPS 和 PPS


+++++++++++++++++++++++++++++++++++++++++++++



vlc没有收到pps和sps

2010-10-08 16:16
|问题 packetizer_h264 packetizer warning: waiting for SPS/PPS是因为解码器只是在第一次执行编码的时候，才编码出 SPS、PPS、和I_Frame； h264 packetizer has set so, that it sends sps/pps only first keyframe, I'm trying to figure what breaks if that is changed so sps/pps is written in every keyframe. [出自| [http://trac.videolan.org/vlc/ticket/1384](http://trac.videolan.org/vlc/ticket/1384)]解决办法：1、编码器编码出每个关键帧都加上SPS、PPS ，据说通常情况编码器编出的 SPS、PPS是一样的，所以这种方法耗费资源。2、在服务器接收到客户端请求时，发送第一个package 加上 SPS、PPS。具体如下：- 1、在 [VideoOpenFileSource](http://server-linux/wiki/VideoOpenFileSource) 添加一个变量 isFirstFrame；- 2、构造时初始化 isFirstFrame = true;- 3、在int [VideoOpenFileSource](http://server-linux/wiki/VideoOpenFileSource)::readFromBufferChain() 修改如下：-    1         if(isFirstFrame == true)   2         {   3                 memcpy(fTo, h264_header, sizeof(h264_header)); /* h264_header = pps +sps*/   4                 offset = sizeof(h264_header);   5                 framesize = BufferChain_get(fInput.video_bufs, fTo + offset);   6                 offset += framesize;   7                 isFirstFrame = false;   8                 printf("this is the first fime\n");   9                 sleep(1);  10         }  11         else  12         {  13                 framesize = BufferChain_get(fInput.video_bufs, fTo + offset);  14                 offset += framesize;  15         }  1[[http://topic.csdn.net/u/20100801/17/ef35e664-92ff-4144-a35f-3984dcf11da3.html](http://topic.csdn.net/u/20100801/17/ef35e664-92ff-4144-a35f-3984dcf11da3.html)| 参考] ========================================================================sdp 关于pps和sps的疑问：packetization-mode 主要是定义包的模式，单一 NALU单元模式（0）；非交错(non-interleaved)封包模式（1）；交错(interleaved)封包模式（2）sprop-parameter-sets 等于H.264 的序列参数集和图像参数 NAL单元，base64转换；（即＝ sps+pps）profile-level-id 这个参数用于指示 H.264 流的 profile 类型和级别。这知道这个是啥东东参考 黑暗长老 [www.cppblog.com/czanyou/](http://www.cppblog.com/czanyou/)ffmpeg decode 关于pps sps问题：[stackoverflow.com/questions/3493742/problem-to-decode-h264-video-over-rtp-with-ffmpeg-libavcodec/3500432#3500432](http://stackoverflow.com/questions/3493742/problem-to-decode-h264-video-over-rtp-with-ffmpeg-libavcodec/3500432#3500432)|
|----|







如何用C语言取出H.264ES文件里的nal(sps,pps)信息。比如width, height, profile等等

请高手指点指点。。。 [http://www.oschina.net/question/225813_35707](http://www.oschina.net/question/225813_35707)

解析sps,pps的代码在ffmpeg里面就有, 抄出来就行了, 我以前也自己写过...
ffmpeg的libavcodec/h264_parser.c,
h264_ps.c
函数
ff_h264_decode_seq_parameter_set
ff_h264_decode_picture_parameter_set
自己可以看代码.



H264参数语法文档： SPS、PPS、IDR [http://blog.csdn.net/heanyu/article/details/6205390](http://blog.csdn.net/heanyu/article/details/6205390)

H.264码流第一个 NALU 是 SPS（序列参数集Sequence Parameter Set）
对应H264标准文档 7.3.2.1 序列参数集的语法进行解析









### 关于H264通过RTP传输的打包方式  


|字号订阅








Q:现在小弟初次尝试H264的编码通过RTP方式传输，具体实验环境的问题如下：
环境：
服务器端，H264的帧数据（可能超过64k），分成N个1460字节的包，然后加上RTP头发送。
客户端，VLC播放器，通过RTSP协议建立连接，然后接收数据解码播放。
结果：
VLC不能解码接收到的数据，解码出错，VLC的信息中显示不能解码帧数据。
我已经阅读了一遍rfc3984的文档，对里面的如何进行打包和用rtp传输不是非常理解，希望各位大虾能够帮小弟一把，告诉小弟这些和H264的帧该如何发送，该如何分包，该如何加头信息等等。
（其中看到FUs的方式好像适合分包发送，因为小弟的数据帧可能超过64k，所以忘大虾们能够仔细解释一下对于小弟这种情况下的RTP传输）

A:我觉得所有的问题在 RFC3984 里面都已经说得很清楚了。不知道你有哪点不懂，请具体提出来。



Q:斑竹好，我这边是用VLC和服务器端进行通讯的，他们是用RTSP协议建立 开始时的连接的，服务器返回DISCRIBERS请求的SDP和下面描述的相同，我使用的packetization-mode=1，即FU-As方式打 包，因为我这边上来的数据帧可能超过64k数据。能否麻烦斑竹看看我这边的SDP写的是否正确。
SDP:
v=0
o=- 1 1 IN IP4 127.0.0.1
s=VStream Live
a=type:broadcast
t=0 0
c=IN   IP4 0.0.0.0
m=video 49170 RTP/AVP 99
a=rtpmap:99 H264/90000
a=fmtp:99 profile-level-id=42A01E; packetization-mode=1; sprop-parameter-ets=Z0IACpZTBYmI, aMljiA==
a=control:trackID=0

还有就是在RTP发送时，我打好包的数据方式如下面所示：
上来的帧数据为：NALU头＋EBSP数据
因为帧数据大于1460字节，所以我把数据分为N个不大于1460字节的包，每个包前面加上RTP头发出去。
其 中NALU头的数值I帧为0x65，参数集为0x67和0x68，这个值是不是有点错误，我看RFC3984上面说的好像和我现在的有点不 同，RFC3984上面说FU-As方式打包类型值为28，我不知道这个是否十进制的，如果按照RFC3984上说的NALU头应该是多少？还是用FU- As方式的FU indicator代替原来的NALU头。
还有这个FU-As方式的头好像是有两个值，一个是FU indicator，另外一个是FU header，这两个值我应该填写什么？

按照我现在填写的内容，VLC会出现解不出码的情况，希望斑竹可以帮我回答的细致一点。谢谢了。

A:我觉得 RFC3984 上面说得非常清楚啊。
首先你把一个 NALU 的 EBSP 根据需求拆分为多个包，例如 3 个，则：

第一个 FU-A 包的 FU indicator 应该是：F = NALU 头中的 F；NRI = NALU 头中的 NRI；Type = 28。FU header 应该是：S = 1；E = 0；R = 0；Type = NALU 头中的 Type。

第二个 FU-A 包的 FU indicator 应该是：F = NALU 头中的 F；NRI = NALU 头中的 NRI；Type = 28。FU header 应该是：S = 0；E = 0；R = 0；Type = NALU 头中的 Type。

第三个 FU-A 包的 FU indicator 应该是：F = NALU 头中的 F；NRI = NALU 头中的 NRI；Type = 28。FU header 应该是：S = 0；E = 1；R = 0；Type = NALU 头中的 Type。

Q:版主，我按照你的方式分好包发送了，发现VLC不会出现不能解帧的情况了， 但是，还是出不来图像。我想可能是因为发送序列参数集和图像参数集的方法不对，他们两个的长度都很小，只要一个包就可以了，我现在将他们按照singal NALU的方式发送，就是直接在NALU包前加一个RTP的头，然后发出去。
是不是我这样发参数集存在着问题，反正我这边VLC是解不了这个参数集，因为参数集解不了，所以下面的帧肯定解不了，所以出不了图像。
麻烦版主再解释一下如何发参数集。

A:今天刚接受了流媒体的相关培训。懂得看你的   SDP 了。

对 于你的问题，不知道 SPS、PPS 打包是否有问题。按照 RFC3984，而且感觉你打单一包的方式也是错的。我希望你能通过自己学习的方式去把这个问题弄清楚，因为 RFC3984 里面说得很清楚，请你自己学习学习 RFC3984 吧。既然你在做这个工作，还是应该仔细学习一下 RFC3984。

另外， SDP 中的 sprop-parameter-ets=Z0IACpZTBYmI 实际就是 SPS 和 PPS 的 BASE64 转码，你不用在码流中再传输 SPS/PPS，直接从 SDP 就可以得到。

A2:1.SDP中已经包括SPS&PPS，码流中完全可以不用传输SPS&PPS
2. profile-level-id=42A01E，这是SPS的开头几个字节，剩下的在sprop-parameter- ets=Z0IACpZTBYmI, aMljiA==中，BASE64编码，把“Z0IACpZTBYmI, aMljiA==”反BASE64转换回去，应该刚好是SPS&PPS的内容
3. 打包注意，要求H.264码流不是byte stream格式的，即没有0x000001分隔，也没有插入0x03，具体如何生成，检查你的编码器选项。
4. packetization-mode=1模式下，要求每个RTP中只有一个NAL单元，或者一个FU，不分段的NAL不做任何修改，直接作为RTP负 载；分段的NAL注意，NAL头不传输，有效负载从NAL头之后开始，根据NAL头的信息生成FU的头两个字节（相当于NAL头拆为两部分），具体生成方 式版主已经讲得很清楚。
5. RTP的payload type要与SDP中一致，不然解的出才怪



===========================================



**H.264 Profiles**

H.264有四种profile,每个profile支持一组特定的编码功能，并支持一类特定的应用,分别是BP、EP、MP、HP：

1、BP-Baseline Profile：

       a、I/P slices

       b、Multiple reference frames (–refs <int>, >1 in the x264 CLI)

       c、In-loop deblocking(环路滤波)

       d、CAVLC entropy coding (–no-cabac in the x264 CLI)

利用I片和P片支持帧内和帧间编码，支持利用基于上下文的自适应的变长编码进行的熵编码（CAVLC）和只支持无交错（Progressive）。主要用于可视电话、会议电视、无线通信等实时视频通信。

2、EP-Extendedprofile：


         支持I/P/B/SP/SI帧，即支持码流之间有效的切换（SP和SI片）、改进误码性能；只支持无交错（Progressive）和CAVLC；但不支持隔行视频和CABAC。


3、MP-Main profile：

       a、Baseline Profile features mentioned above

       b、B slices

       c、CABACentropy coding

       d、Interlacedcoding – PAFF/MBAFF

       e、Weightedprediction

提供I/P/B 帧，支持无交错（Progressive）和交错（Interlaced），也支持CAVLC 和CABAC 的支持；主要用于数字广播电视与数字视频存储。
4、HP-High profile：
        a、Main Profile features mentioned above

        b、8×8 transform option (–8×8dct in the x264 CLI)
c、Custom quantisation matrices
        在mainProfile的基础上增加了8x8内部预测、自定义量化、 无损视频编码和更多的YUV格式。


   H.264 FRExt（即：FidelityRange Extensions）扩展部分（Amendment），包括High profile（HP）、High10 profile（Hi10P）、High 4:2:2 profile（Hi422P）、High4:4:4 profile（Hi444P）4个profile。H.264 Baseline profile、Extended profile和Main profile都是针对8位样本数据、4:2:0格式的视频序列，FRExt将其扩展到8～12位样本数据，视频格式可以为4:2:0、4:2:2、4:4:4，设立了High profile（HP）、High 10profile（Hi10P）、High 4:2:2 profile（Hi422P）、High 4:4:4 profile（Hi444P） 4个profile，这4个profile都以Main profile为基础

   在相同配置情况下，Highprofile（HP）可以比Mainprofile（MP）节省10%的码流量，比MPEG-2 MP节省60%的码流量，具有更好的编码性能。根据应用领域的不同，Baselineprofile多应用于实时通信领域，Main profile多应用于流媒体领域，High profile则多应用于广电和存储领域。

![](http://img.blog.csdn.net/20131223002743343?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3Bob25lODk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
---------------------------
下面是x264关于profile的简单特点描述

--profile<string>      Force the limits ofan H.264 profile

                                  Overrides allsettings.

                                  - baseline:

                                    --no-8x8dct--bframes 0 --no-cabac

                                    --cqm flat--weightp 0

                                    Nointerlaced.

                                    Nolossless.

                                  - main:

                                    --no-8x8dct--cqm flat

                                    Nolossless.

                                  - high:

                                    Nolossless.

                                  - high10:

                                    Nolossless.

                                    Support forbit depth 8-10.

                                  - high422:

                                    Nolossless.

                                    Support forbit depth 8-10.

                                    Support for4:2:0/4:2:2 chroma subsampling.

                                  - high444:

                                    Support forbit depth 8-10.

                                    Support for4:2:0/4:2:2/4:4:4 chroma subsampling.
-----------------------------------------------------------------------------------------------
符合BaselineProfile 的比特流应该遵守如下的限制：

-- 比特流中只能出现I 和 P 片。

-- NAL 单元比特流的nal_unit_type 值不应该落在[2，4]（包括2，4，下同）范围内。

注释：nal_unit_type=2对应片分区A；nal_unit_type=3对应片分区B；nal_unit_type=4对应片分区C。因为baselineprofile不采用数据分区的方法。

-- 序列参数集中的frame_mbs_only_flag 应该等于1。

注释：frame_mbs_only_flag 等于0 表示编码视频序列的编码图像可能是编码场或编码帧。frame_mbs_only_flag 等于1表示编码视频序列的每个编码图像都是一个仅包含帧宏块的编码帧。

-- 图像参数集中的weighted_pred_flag 和weighted_bipred_idc 都应该等于0。

注释：weighted_pred_flagequal to 0 specifies that weighted prediction（加权预测）shall not be applied to P and SPslices.weighted_pred_flag equal to 1 specifies that weighted prediction shallbe applied to P and SP slices.

weighted_bipred_idcequal to 0 specifies that the default weighted prediction（默认的加权预测）shall be applied to B slices.weighted_bipred_idcequal to 1 specifies that explicit weighted prediction（指定的加权预测方式）shall be applied to B slices.weighted_bipred_idcequal to 2 specifies that implicit weighted prediction（隐含的加权预测方式）shall be applied to B slices. The value ofweighted_bipred_idcshall be in the range of 0 to 2, inclusive.

-- 图像参数集中的entropy_coding_mode_flag 应该等于0。

注释：entropy_coding_mode_flag=0，表示熵编码使用CAVLC。

-- 图像参数集中的num_slice_groups_minus1 应该在[0，7]范围之内。

注释：本句法元素加1表示图像中片组的个数。H.264中没有专门的句法元素用于指明是否使用片组模式，当本句法元素等于0时（即只有一个片组），表示不使用片组模式，后面也不会跟有用于计算片组映射到句法元素。

-- 应该满足在A.3 节中规定的Baseline profile 级别限制。

与Baselineprofile 相符的比特流，它的profile_idc 应该等于66。对一个遵守Baseline profile某个指定级别的解码器，它能够解码任何符合如下要求的比特流：1. profile_idc 等于66 或者constraint_set0_flag 等于1; 2. level_idc所代表的级别小于或等于这个指定的级别。

注释：constraint_set0_flag  其值等于1表示必须遵从标准中附录A.2.1（即上面）所指明的所有约束条件；等于0表示不必遵从所以条件。


--------------------------------------------------------------------------------------------

**h264 level**

   每个profile 都规定了一个算法特征和限制的子集，任何遵守某个profile 的解码器都应该支持与其相应的子集。

   每个level都规定了一组对标准中语法成员（syntax element）所采用的各种参数值的限制。

在给定的profile下，level通常与解码器的处理能力和内存容量相对应。每一个档次设置不同的参数（如取样速率、图像尺寸、编码比特率等），得到对应的编解码器性能的不同level。

下图是levels参数表格
**![](http://img.blog.csdn.net/20131223003009312?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3Bob25lODk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)**
**几种h.264 software encoder**
**![](http://img.blog.csdn.net/20131223003144093?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3Bob25lODk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)**









