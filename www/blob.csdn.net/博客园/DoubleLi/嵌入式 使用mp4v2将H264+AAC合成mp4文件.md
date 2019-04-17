# 嵌入式 使用mp4v2将H264+AAC合成mp4文件 - DoubleLi - 博客园






录制程序要添加新功能：录制CMMB电视节目，我们的板卡发送出来的是RTP流（H264视频和AAC音频），录制程序要做的工作是：

（1）接收并解析RTP包，分离出H264和AAC数据流；

（2）将H264视频和AAC音频以某种格式封装，最后存成文件，供用户查看。



第一步已经有部分代码可供参考，因此很快就完成了。

第二步，我们决定封装成mp4，查找了一些资料后，决定使用开源库mp4v2来合成mp4文件。

技术路线已确定，就开工干活。



(一)mp4格式的基础知识。

关于mp4格式，网上介绍的不少，有以下内容可供参考：

（1）两个ISO标准：

[ISO/IEC 14496-12]:ISO base media file format --”is a general format forming the basis for a number of other more specific file formats. This format contains the timing, structure, and media information for timed sequences of media data, such as audio-visual presentations ”

[ISO/IEC 14496-14]:MP4 file format --”This specification defines MP4 as an instance of the ISO Media File format [ISO/IEC 14496-12 and ISO/IEC 
15444-12]. ”

定义了mp4文件格式标准。

（2）[http://wenku.baidu.com/view/673482284b73f242336c5f4c.html](http://wenku.baidu.com/view/673482284b73f242336c5f4c.html)

是上面两个标准的解释，建议先看这个，了解大概，具体细节再看ISO标准文件。



（二）技术验证。主要就是写验证代码，验证技术可行性。

去官网下载mp4v2源码、编译、安装过程略过不提。所有资料可以在[http://code.google.com/p/mp4v2/](http://code.google.com/p/mp4v2/)找到。

先写部分验证代码，很快完成了，但封装出来的文件有问题，无法播放。



合成部分，代码如下：

```
![复制代码](https://common.cnblogs.com/images/copycode.gif)

 1 static void* writeThread(void* arg)
 2 {
 3     rtp_s* p_rtp = (rtp_s*) arg;
 4     if (p_rtp == NULL)
 5     {
 6         printf("ERROR!\n");
 7         return;
 8     }
 9 
10     MP4FileHandle file = MP4CreateEx("test.mp4", MP4_DETAILS_ALL, 0, 1, 1, 0, 0, 0, 0);//创建mp4文件
11     if (file == MP4_INVALID_FILE_HANDLE)
12     {
13         printf("open file fialed.\n");
14         return;
15     }
16 
17     MP4SetTimeScale(file, 90000);
18 
19     //添加h264 track    
20     MP4TrackId video = MP4AddH264VideoTrack(file, 90000, 90000 / 25, 320, 240,
21                                             0x64, //sps[1] AVCProfileIndication
22                                             0x00, //sps[2] profile_compat
23                                             0x1f, //sps[3] AVCLevelIndication
24                                             3); // 4 bytes length before each NAL unit
25     if (video == MP4_INVALID_TRACK_ID)
26     {
27         printf("add video track failed.\n");
28         return;
29     }
30     MP4SetVideoProfileLevel(file, 0x7F);
31 
32     //添加aac音频
33     MP4TrackId audio = MP4AddAudioTrack(file, 48000, 1024, MP4_MPEG4_AUDIO_TYPE);
34     if (video == MP4_INVALID_TRACK_ID)
35     {
36         printf("add audio track failed.\n");
37         return;
38     }
39     MP4SetAudioProfileLevel(file, 0x2);
40 
41 
42     int ncount = 0;
43     while (1)
44     {
45         frame_t* pf = NULL; //frame
46         pthread_mutex_lock(&p_rtp->mutex);
47         pf = p_rtp->p_frame_header;
48         if (pf != NULL)
49         {
50             if (pf->i_type == 1)//video
51             {
52                MP4WriteSample(file, video, pf->p_frame, pf->i_frame_size, MP4_INVALID_DURATION, 0, 1);
53              }
54             else if (pf->i_type == 2)//audio
55             {
56                 MP4WriteSample(file, audio, pf->p_frame, pf->i_frame_size , MP4_INVALID_DURATION, 0, 1);
57             }
58 
59             ncount++;
60 
61             //clear frame.
62             p_rtp->i_buf_num--;
63             p_rtp->p_frame_header = pf->p_next;
64             if (p_rtp->i_buf_num <= 0)
65             {
66                 p_rtp->p_frame_buf = p_rtp->p_frame_header;
67             }
68             free_frame(&pf);
69             pf = NULL;
70 
71             if (ncount >= 1000)
72             {
73                 break;
74             }
75         }
76         else
77         {
78             //printf("BUFF EMPTY, p_rtp->i_buf_num:%d\n", p_rtp->i_buf_num);
79         }
80         pthread_mutex_unlock(&p_rtp->mutex);
81         usleep(10000);
82     }
83 
84     MP4Close(file);
85 }

![复制代码](https://common.cnblogs.com/images/copycode.gif)
```



现象：没有图像，也没有声音，根本无法播放。

于是，艰苦的工作开始了：跟踪查找原因。



（1）使用 vlc播放合成的mp4文件，查看详细输出：

```
![复制代码](https://common.cnblogs.com/images/copycode.gif)

1 vlc -vvv test.mp4
2 [0x8e9357c] mp4 stream debug: found Box: ftyp size 24 
3 [0x8e9357c] mp4 stream debug: found Box: free size 136 
4 [0x8e9357c] mp4 stream debug: skip box: "free" 
5 [0x8e9357c] mp4 stream debug: found Box: mdat size 985725 
6 [0x8e9357c] mp4 stream debug: skip box: "mdat" 
7 [0x8e9357c] mp4 stream debug: found Box: moov size 5187 
8 [0x8e9357c] mp4 stream debug: found Box: mvhd size 108 
9 [0x8e9357c] mp4 stream debug: read box: "mvhd" creation 734515d-06h:22m:03s modification 734515d-06h:22m:23s time scale 90000 duration 694977d-48h:00m:29s rate 1.000000 volume 1.000000 next track id 3 

![复制代码](https://common.cnblogs.com/images/copycode.gif)
```

可以看到vlc(实际上是调用libmp4库)解析box都正确的，mdat的大小也是正确的。
但接下来一行：
skip box: "mdat" 
就比较奇怪了，明明解析正确了，为什么要将mdat忽略掉呢？要知道，mdat里存放的可是真正的音视频数据阿？如果skip掉了，后面解码时没有数据，当然播放不了了？



（2）既然找到疑点，继续跟踪。

查看vlc的源代码，在文件modules/demux/mp4/libmp4.c中发现：skip信息是由MP4_ReadBoxSkip()函数打印的，而调用的地方在libmp4.c中2641行：

```
![复制代码](https://common.cnblogs.com/images/copycode.gif)

 1 /* Nothing to do with this box */
 2 { FOURCC_mdat,  MP4_ReadBoxSkip,        MP4_FreeBox_Common },
 3 { FOURCC_skip,  MP4_ReadBoxSkip,        MP4_FreeBox_Common },{ FOURCC_free,  MP4_ReadBoxSkip,        MP4_FreeBox_Common },
 4 { FOURCC_wide,  MP4_ReadBoxSkip,        MP4_FreeBox_Common },
 5 
 6 而在libmp4.h中：
 7 #define FOURCC_mdat VLC_FOURCC( 'm', 'd', 'a', 't' )
 8 #define FOURCC_skip VLC_FOURCC( 's', 'k', 'i', 'p' )
 9 #define FOURCC_free VLC_FOURCC( 'f', 'r', 'e', 'e' )
10 #define FOURCC_wide VLC_FOURCC( 'w', 'i', 'd', 'e' )

![复制代码](https://common.cnblogs.com/images/copycode.gif)
```

从代码看，vlc调用libmp4解析文件时，主动忽略了mdat,skip,free,wide这四种类型的box。
为什么呢？

（3）继续查看modules/demux/mp4/mp4.c中的Open()函数（解析模块的入口函数），可以看到本模块的主要工作是初始化一个demux_sys_t结构体，该结构体定义如下：

```
![复制代码](https://common.cnblogs.com/images/copycode.gif)

 1 struct demux_sys_t
 2 {
 3     MP4_Box_t    *p_root;      /* container for the whole file */
 4     mtime_t      i_pcr;
 5     uint64_t     i_time;         /* time position of the presentation * in movie timescale */
 6     uint64_t     i_timescale;    /* movie time scale */
 7     uint64_t     i_duration;     /* movie duration */
 8     unsigned int i_tracks;       /* number of tracks */
 9     mp4_track_t  *track;         /* array of track */
10     float        f_fps;          /* number of frame per seconds */
11 
12     /* */
13     MP4_Box_t    *p_tref_chap;
14 
15     /* */
16     input_title_t *p_title;
17 };

![复制代码](https://common.cnblogs.com/images/copycode.gif)
```

似乎只是为了获取mp4的tracks,moov,duration, timescale等基本信息，实际上并不解码数据，因此就不需要关注mdat这个box了。

综上：vlc的输出是正常的，libmp4忽略了mdat这个box也不是造成mp4文件无法播放的原因，只是因为libmp4这个模块并不真正解码数据，所以不需要关注这个box。
既然问题不在这，那在哪里呢？



（4）继续看vlc的输出：
AVC: nal size -1710483062 
no frame! 
[0x8e93eb4] avcodec decoder warning: cannot decode one frame (3595 bytes) 
可以看到，vlc实际上是调用avcodec（ffmpeg）来解码数据的，我们的视频是AVC(H264)格式的。
从错误信息可以确定，是H264的NAL大小错误，似乎跟mp4文件本身关系不大。

不管那么多，先看看代码再说。

vlc是以lib的形式使用ffmpeg的，所以我们必须看ffmpeg的代码：

```
![复制代码](https://common.cnblogs.com/images/copycode.gif)

libavcodec/h264.c:
static int decode_nal_units(H264Context *h, const uint8_t *buf, int buf_size){

    ….

    for(;;){
        if(buf_index >= next_avc) {
            if(buf_index >= buf_size) break;
            nalsize = 0;
            for(i = 0; i < h->nal_length_size; i++)
                nalsize = (nalsize << 8) | buf[buf_index++];
            if(nalsize <= 0 || nalsize > buf_size - buf_index){
                av_log(h->s.avctx, AV_LOG_ERROR, "AVC: nal size %d\n", nalsize);
                break;
            }
            next_avc= buf_index + nalsize;
        } 

…

}

![复制代码](https://common.cnblogs.com/images/copycode.gif)
```

可以看到，正是这里报错的。
但是，为什么报错呢？根据ffmpeg的信息，知道取出来的 nalsize为负数。
怀疑是h264流本身有问题，于是用Elecard查看了生成的mp4文件，视频播放又非常正常。似乎h264流是正常的？
愁呀愁。。。。



（5）在mp4v2的论坛上找到一个帖子，与我的情况非常类似：
[http://groups.google.com/group/mp4v2/browse_thread/thread/a4ac6824e585f003/0169c48511f5827a?lnk=gst&q=avc#0169c48511f5827a](http://groups.google.com/group/mp4v2/browse_thread/thread/a4ac6824e585f003/0169c48511f5827a?lnk=gst&q=avc#0169c48511f5827a)

> 
内容如下：
Ottavio Campana 
“question about MP4AddH264VideoTrack。
What's the meaning of the profile_compat and 
sampleLenFieldSizeMinusOne fields?”

Jeremy Noring
"Usually an NALU is prefixed by the start code 0x00000001. To write it 
as a sample in MP4 file format, just replace the start code with size 
of the NALU(without 4-byte start code) in big endian. You also need to 
specify how many bytes of the size value requires. Take libmp4v2 for 
example, the last parameter in MP4AddH264VideoTrack(.., uint8_t 
sampleLenFieldSizeMinusOne) indicate the number of byes minus one." 

...so each sample you and to mp4v2 should be prefixed with a size code 
(in big-endian, of course). I use a 4 byte size code, so 
sampleLenFieldSizeMinusOne gets set to 3. This seems to work; my 
files playback on just about everything. Perhaps one of the project 
maintainers can clarify this, and it'd also be good to update the 
documentation of that call to make this clear.”

Ottavio Campana 
that's the code I used as reference to write my program :-( 
but my doubt is that there must be something wrong somewhere, because 
boxes seem to be correctly written, but when I try to decode them I 
get errors like 
[h264 @ 0xb40fa0]AVC: nal size -502662121 
have you ever seen an error like this?

Ottavio Campana 
> Not sure, but it looks you're not converting it to big-endian before 
> prefixing it to your sample. 
well, eventually using ffmpeg to dump the read frames, I discovered 
that I had to strip che NALU start code, i.e. the 0x00000001, and to 
put the NALU size at its place. 
It works perfectly now, but I still wonder why I had to put the size 
at the begin of the data, since it is a parameter which is passed to 
MP4WriteSample, so I expected the function to add it.


从中得到如下关键信息：
(1)h264流中的NAL，头四个字节是0x00000001;
(2)mp4中的h264track，头四个字节要求是NAL的长度，并且是大端顺序；
(3)mp4v2很可能针对此种情况并没有做处理，所以写到mp4文件中的每个NAL头四个字节还是0x00000001.

那好说，我将每个sample(也就是NAL)的头四个字节内容改成NAL的长度，且试试看：

```
if(pf->i_frame_size >= 4)
{
    uint32_t* p = (&pf->p_frame[0]);
      *p = htonl(pf->i_frame_size -4);//大端,去掉头部四个字节
}
MP4WriteSample(file, video, pf->p_frame, pf->i_frame_size,     MP4_INVALID_DURATION, 0, 1);
```

测试下来，果然OK了！



（6）视频已经解决了，音频还有问题：播放的声音太快。

尝试调整参数：

MP4TrackId audio = MP4AddAudioTrack(file, 48000, 1024, MP4_MPEG4_AUDIO_TYPE);

第三个参数sampleDuration，表示每个sample持续多少个duration，网上看到的都是1024。

我尝试了几个不同的值：128,256,512,4096都不行，最后发现设为2048就正常了。

（为什么是2048??????我不清楚，也许是因为我们的音频是双声道？有时间再研究。。。）

正确代码如下：

```
MP4TrackId audio = MP4AddAudioTrack(file, 48000, 2048, MP4_MPEG4_AUDIO_TYPE);
```



至此，已经成功的将rtp流合成了mp4文件，证明了技术上是可行的。



关于该demo,可以在这里 [https://github.com/ZhengfengRao/rtp2mp4](https://github.com/ZhengfengRao/rtp2mp4) 看到。

注意：很多参数都是针对我们的具体应用写死的，仅供参考。









