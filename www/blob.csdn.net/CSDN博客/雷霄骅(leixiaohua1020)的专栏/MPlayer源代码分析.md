# MPlayer源代码分析 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年09月22日 01:49:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：35
个人分类：[Mplayer](https://blog.csdn.net/leixiaohua1020/article/category/2286593)










![](https://img-blog.csdn.net/20140616104036421)

一.Mplayer支持的格式

MPlayer是一个LINUX下的视频播放器，它支持相当多的媒体格式，无论在音频播放还是在视频播放方面，可以说它支持的格式是相当全面的。
视频格式支持：MPEG、AVI、ASF 与WMV、QuickTime 与 OGG/OGM、SDP、PVA、GIF。
音频格式支持：MP3、WAV、OGG/OGM 文件(Vorbis)、WMA 与 ASF、MP4、CD音频、XMMS。

二. Mplayer 中头文件的功能分析



```cpp
config.h // 各种本地配置宏定义头
version.h // 版本定义头 #define VERSION "1.0pre7try2-3.4.2"
mp_msg.h // 消息处理头
help_mp.h // 根据配置自动生成的帮助头 #include "help/help_mpen.h"
cfg-mplayer-def.h // Mplayer 运行时的选项缺省值头文件 char*
default_config =
sub_reader.h // 拥有格式自动发现功能的字幕(subtitle)阅读器
libvo/video_out.h // 该文件包含 libvo 视频输出的公共函数、变量
libvo/font_load.h // 有关字体装载的例程
libao2/audio_out.h // 音频输出驱动程序相关结构定义和全局数据
libmpcodecs/dec_audio.h // 音频解码
libmpcodecs/dec_video.h // 视频解码
libmpdemux/matroska.h // 多路解复用，媒体容器格式 matroska 处理头
libmpdemux/stream.h // 流处理
libmpdemux/demuxer.h // 多路解复用头文件
libmpdemux/stheader.h // 媒体流头处理
get_path.c // 路径获取头文件
spudec.h // SPU 子画面单元头，DVD 字幕流
edl.h // 剪辑控制清单
m_option.h // 选项类型处理头
m_config.h // 配置处理头文件
```





三. MPlayer.main 主流程简要说明



```cpp
int main() {
1) 变量声明，电影信息 movie info:
2) 初始化，消息系统……
play_next_file:
3)播放文件 filename 的循环 goto play_next_file 开始
main:
4) 主处理 main
5) 播放真正主循环 2010 ~3541 while (!eof)
while (!eof) {
5.1) 播放音频 PLAY AUDIO 2017 ~ 2064 decode_audio(sh_audio, ...);
5.2) 播放视频 PLAY VIDEO, 2068 ~ 2300 decode_video(sh_video, ...);
5.3) 处理暂停 PAUSE
5.4) 处理 EDL
5.5) 键盘事件处理, 搜索2400~3216 while (!brk_cmd &&
(cmd=mp_input_get_cmd(0,0,0))!=NULL)
5.6) 时间寻道(秒) if (seek_to_sec)
5.7) 寻道 3243 ~ 3306, if (rel_seek_secs || abs_seek_pos)
5.8) 处理 GUI
5.9) 变更 Update OSD
5.10) 找到字幕 find sub
5.11) 处理 X11 窗口
5.12) DVD 字幕 sub:
}
goto_next_file:
6) 播放结束，转到下个文件 goto_next_file:
}
```




四.Mplayer源码分析
从Mplayer.c的main开始处理参数

```cpp
mconfig = m_config_new();
m_config_register_options(mconfig,mplayer_opts);
// TODO : add something to let modules register their options
mp_input_register_options(mconfig);
parse_cfgfiles(mconfig);
```

初始化mpctx结构体，mpctx应该是mplayer context的意思，顾名思义是一个统筹全局的变量。

```cpp
static MPContext *mpctx = &mpctx_s;
// Not all functions in mplayer.c take the context as an argument yet
static MPContext mpctx_s = {
.osd_function = OSD_PLAY,
.begin_skip = MP_NOPTS_VALUE,
.play_tree_step = 1,
.global_sub_pos = -1,
.set_of_sub_pos = -1,
.file_format = DEMUXER_TYPE_UNKNOWN,
.loop_times = -1,
#ifdef HAS_DVBIN_SUPPORT
.last_dvb_step = 1,
#endif
};
```

原型



```cpp
//真正统筹全局的结构
typedef struct MPContext {
    int osd_show_percentage;
    int osd_function;
    const ao_functions_t *audio_out;
    play_tree_t *playtree;
    play_tree_iter_t *playtree_iter;
    int eof;
    int play_tree_step;
    int loop_times;

    stream_t *stream;
    demuxer_t *demuxer;
    sh_audio_t *sh_audio;
    sh_video_t *sh_video;
    demux_stream_t *d_audio;
    demux_stream_t *d_video;
    demux_stream_t *d_sub;
    mixer_t mixer;
    const vo_functions_t *video_out;
    // Frames buffered in the vo ready to flip. Currently always 0 or 1.
    // This is really a vo variable but currently there's no suitable vo
    // struct.
    int num_buffered_frames;

    // used to retry decoding after startup/seeking to compensate for codec delay
    int startup_decode_retry;
    // how long until we need to display the "current" frame
    float time_frame;

    // AV sync: the next frame should be shown when the audio out has this
    // much (in seconds) buffered data left. Increased when more data is
    // written to the ao, decreased when moving to the next frame.
    // In the audio-only case used as a timer since the last seek
    // by the audio CPU usage meter.
    double delay;

    float begin_skip; ///< start time of the current skip while on edlout mode
    // audio is muted if either EDL or user activates mute
    short edl_muted; ///< Stores whether EDL is currently in muted mode.
    short user_muted; ///< Stores whether user wanted muted mode.

    int global_sub_size; // this encompasses all subtitle sources
    int global_sub_pos; // this encompasses all subtitle sources
    int set_of_sub_pos;
    int set_of_sub_size;
    int sub_counts[SUB_SOURCES];
#ifdef CONFIG_ASS
    // set_of_ass_tracks[i] contains subtitles from set_of_subtitles[i]
    // parsed by libass or NULL if format unsupported
    ASS_Track* set_of_ass_tracks[MAX_SUBTITLE_FILES];
#endif
    sub_data* set_of_subtitles[MAX_SUBTITLE_FILES];

    int file_format;

#ifdef CONFIG_DVBIN
    int last_dvb_step;
    int dvbin_reopen;
#endif

    int was_paused;

#ifdef CONFIG_DVDNAV
    struct mp_image *nav_smpi;   ///< last decoded dvdnav video image
    unsigned char *nav_buffer;   ///< last read dvdnav video frame
    unsigned char *nav_start;    ///< pointer to last read video buffer
    int            nav_in_size;  ///< last read size
#endif
} MPContext;
```





一些GUI相关的操作
打开字幕流
打开音视频流

```cpp
mpctx->stream=open_stream(filename,0,&mpctx->file_format);
fileformat 文件还是TV 流DEMUXER_TYPE_PLAYLIST 或DEMUXER_TYPE_UNKNOWN
DEMUXER_TYPE_TV
current_module记录状态vobsub open_stream handle_playlist dumpstream
stream_reset(mpctx->stream);
stream_seek(mpctx->stream,mpctx->stream->start_pos);
f=fopen(stream_dump_name,”wb”); dump文件流
stream->type==STREAMTYPE_DVD
```

//============ Open DEMUXERS — DETECT file type ======================
Demux。分离视频流和音频流

```cpp
mpctx->demuxer=demux_open(mpctx->stream,mpctx-
>file_format,audio_id,video_id,dvdsub_id,filename);
Demux过程
demux_open
get_demuxer_type_from_name
……
mpctx->d_audio=mpctx->demuxer->audio;
mpctx->d_video=mpctx->demuxer->video;
mpctx->d_sub=mpctx->demuxer->sub;
mpctx->sh_audio=mpctx->d_audio->sh;
mpctx->sh_video=mpctx->d_video->sh;
```

分离了之后就开始分别Play audio和video
这里只关心play video

```cpp
/*======================== PLAY VIDEO ============================*/
vo_pts=mpctx->sh_video->timer*90000.0;
vo_fps=mpctx->sh_video->fps;
if (!mpctx->num_buffered_frames) {
double frame_time = update_video(&blit_frame);
mp_dbg(MSGT_AVSYNC,MSGL_DBG2,”*** ftime=%5.3f ***\n”,frame_time);
if (mpctx->sh_video->vf_inited < 0) {
mp_msg(MSGT_CPLAYER,MSGL_FATAL, MSGTR_NotInitializeVOPorVO);
mpctx->eof = 1; goto goto_next_file;
}
if (frame_time < 0)
mpctx->eof = 1;
else {
// might return with !eof && !blit_frame if !correct_pts
mpctx->num_buffered_frames += blit_frame;
time_frame += frame_time / playback_speed; // for nosound
}
}
```



关键的函数是update_video根据pts是否正确调整一下同步并在必要的时候丢帧处理。最终调用decode_video开始解码（包括generate_video_frame里）。mpi = mpvdec->decode(sh_video, start, in_size, drop_frame);mpvdec是在main里通过reinit_video_chain的一系列调用动态选定的解码程序。其实就一结构体。它的原型是

```cpp
typedef struct vd_functions_s
{
vd_info_t *info;
int (*init)(sh_video_t *sh);
void (*uninit)(sh_video_t *sh);
int (*control)(sh_video_t *sh,int cmd,void* arg, …);
mp_image_t* (*decode)(sh_video_t *sh,void* data,int len,int flags);
} vd_functions_t;
```

这是所有解码器必须实现的接口。
int (*init)(sh_video_t *sh);是一个名为init的指针，指向一个接受sh_video_t *类型参数，并返回int类型值的函数地址。那些vd_开头的文件都是解码相关的。随便打开一个vd文件以上几个函数和info变量肯定都包含了。mpi被mplayer用来存储解码后的图像。在mp_image.h里定义。



```cpp
typedef struct mp_image_s {
unsigned short flags;
unsigned char type;
unsigned char bpp; // bits/pixel. NOT depth! for RGB it will be n*8
unsigned int imgfmt;
int width,height; // stored dimensions
int x,y,w,h; // visible dimensions
unsigned char* planes[MP_MAX_PLANES];
int stride[MP_MAX_PLANES];
char * qscale;
int qstride;
int pict_type; // 0->unknown, 1->I, 2->P, 3->B
int fields;
int qscale_type; // 0->mpeg1/4/h263, 1->mpeg2
int num_planes;
/* these are only used by planar formats Y,U(Cb),V(Cr) */
int chroma_width;
int chroma_height;
int chroma_x_shift; // horizontal
int chroma_y_shift; // vertical
/* for private use by filter or vo driver (to store buffer id or dmpi) */
void* priv;
} mp_image_t;
```





图像在解码以后会输出到显示器，mplayer本来就是一个视频播放器么。但也有可能作为输入提供给编码器进行二次编码，MP附带的mencoder.exe就是专门用来编码的。在这之前可以定义filter对图像进行处理，以实现各种效果。所有以vf_开头的文件，都是这样的filter。图像的显示是通过vo，即video out来实现的。解码器只负责把解码完成的帧传给vo，怎样显示就不用管了。这也是平台相关性最大的部分，单独分出来的好处是不言而喻的，像在Windows下有通过direcx实现的vo，Linux下有输出到X的vo。vo_*文件是各种不同的vo实现，只是他们不都是以显示为目的，像vo_md5sum.c只是计算一下图像的md5值。在解码完成以后，即得到mpi以后，filter_video被调用，其结果是整个filter链上的所有filter都被调用了一遍，包括最后的VO，在vo的put_image里把图像输出到显示器。这个时候需要考虑的是图像存储的方法即用哪种色彩空间。

附上两张MPlayer结构图：

![](https://img-blog.csdn.net/20130922015822468?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![](https://img-blog.csdn.net/20130922015827609?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

MPLayer源代码下载地址：[http://download.csdn.net/detail/leixiaohua1020/6374337](http://download.csdn.net/detail/leixiaohua1020/6374337)




