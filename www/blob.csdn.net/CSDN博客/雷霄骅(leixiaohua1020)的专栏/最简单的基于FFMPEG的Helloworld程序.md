# 最简单的基于FFMPEG的Helloworld程序 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2015年07月17日 10:43:41[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：23139
所属专栏：[FFmpeg](https://blog.csdn.net/column/details/ffmpeg-devel.html)











=====================================================

最简单的基于FFmpeg的视频播放器系列文章列表：

[100行代码实现最简单的基于FFMPEG+SDL的视频播放器（SDL1.x）](http://blog.csdn.net/leixiaohua1020/article/details/8652605)

[最简单的基于FFMPEG+SDL的视频播放器 ver2 （采用SDL2.0）](http://blog.csdn.net/leixiaohua1020/article/details/38868499)

[最简单的基于FFmpeg的解码器-纯净版（不包含libavformat）](http://blog.csdn.net/leixiaohua1020/article/details/42181571)

[最简单的基于FFMPEG+SDL的视频播放器：拆分-解码器和播放器](http://blog.csdn.net/leixiaohua1020/article/details/46889389)

[最简单的基于FFMPEG的Helloworld程序](http://blog.csdn.net/leixiaohua1020/article/details/46889849)

=====================================================





本文记录一个基于FFmpeg的HelloWorld程序。该程序可以打印出FFmpeg类库的基本信息。使用该程序通常可以验证FFmpeg是否正确的安装配置。


## 源代码

```cpp
/**
 * 最简单的FFmpeg Helloworld程序
 * Simplest FFmpeg HelloWorld
 *
 * 雷霄骅 Lei Xiaohua
 * leixiaohua1020@126.com
 * 中国传媒大学/数字电视技术
 * Communication University of China / Digital TV Technology
 * http://blog.csdn.net/leixiaohua1020
 *
 * 
 * 本程序是基于FFmpeg函数的最简单的程序。它可以打印出FFmpeg类库的下列信息：
 * Protocol:  FFmpeg类库支持的协议
 * AVFormat:  FFmpeg类库支持的封装格式
 * AVCodec:   FFmpeg类库支持的编解码器
 * AVFilter:  FFmpeg类库支持的滤镜
 * Configure: FFmpeg类库的配置信息
 * 
 * This is the simplest program based on FFmpeg API. It can show following 
 * informations about FFmpeg library:
 * Protocol:  Protocols supported by FFmpeg.
 * AVFormat:  Container format supported by FFmpeg.
 * AVCodec:   Encoder/Decoder supported by FFmpeg.
 * AVFilter:  Filters supported by FFmpeg.
 * Configure: configure information of FFmpeg.
 *
 */

#include <stdio.h>

#define __STDC_CONSTANT_MACROS

#ifdef _WIN32
//Windows
extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavfilter/avfilter.h"
};
#else
//Linux...
#ifdef __cplusplus
extern "C"
{
#endif
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavfilter/avfilter.h>
#ifdef __cplusplus
};
#endif
#endif

//FIX
struct URLProtocol;
/**
 * Protocol Support Information
 */
char * urlprotocolinfo(){
	
	char *info=(char *)malloc(40000);
	memset(info,0,40000);

	av_register_all();

	struct URLProtocol *pup = NULL;
	//Input
	struct URLProtocol **p_temp = &pup;
	avio_enum_protocols((void **)p_temp, 0);
	while ((*p_temp) != NULL){
		sprintf(info, "%s[In ][%10s]\n", info, avio_enum_protocols((void **)p_temp, 0));
	}
	pup = NULL;
	//Output
	avio_enum_protocols((void **)p_temp, 1);
	while ((*p_temp) != NULL){
		sprintf(info, "%s[Out][%10s]\n", info, avio_enum_protocols((void **)p_temp, 1));
	}

	return info;
}

/**
 * AVFormat Support Information
 */
char * avformatinfo(){

	char *info=(char *)malloc(40000);
	memset(info,0,40000);

	av_register_all();

	AVInputFormat *if_temp = av_iformat_next(NULL);
	AVOutputFormat *of_temp = av_oformat_next(NULL);
	//Input
	while(if_temp!=NULL){
		sprintf(info, "%s[In ] %10s\n", info, if_temp->name);
		if_temp=if_temp->next;
	}
	//Output
	while (of_temp != NULL){
		sprintf(info, "%s[Out] %10s\n", info, of_temp->name);
		of_temp = of_temp->next;
	}
	return info;
}

/**
 * AVCodec Support Information
 */
char * avcodecinfo()
{
	char *info=(char *)malloc(40000);
	memset(info,0,40000);

	av_register_all();

	AVCodec *c_temp = av_codec_next(NULL);

	while(c_temp!=NULL){
		if (c_temp->decode!=NULL){
			sprintf(info, "%s[Dec]", info);
		}
		else{
			sprintf(info, "%s[Enc]", info);
		}
		switch (c_temp->type){
		case AVMEDIA_TYPE_VIDEO:
			sprintf(info, "%s[Video]", info);
			break;
		case AVMEDIA_TYPE_AUDIO:
			sprintf(info, "%s[Audio]", info);
			break;
		default:
			sprintf(info, "%s[Other]", info);
			break;
		}

		sprintf(info, "%s %10s\n", info, c_temp->name);

		c_temp=c_temp->next;
	}
	return info;
}

/**
 * AVFilter Support Information
 */
char * avfilterinfo()
{
	char *info=(char *)malloc(40000);
	memset(info,0,40000);

	avfilter_register_all();

	AVFilter *f_temp = (AVFilter *)avfilter_next(NULL);
	
	while (f_temp != NULL){
		sprintf(info, "%s[%15s]\n", info, f_temp->name);
		f_temp=f_temp->next;
	}
	return info;
}

/**
 * Configuration Information
 */
char * configurationinfo()
{
	char *info=(char *)malloc(40000);
	memset(info,0,40000);

	av_register_all();

	sprintf(info, "%s\n", avcodec_configuration());

	return info;
}

int main(int argc, char* argv[])
{
	char *infostr=NULL;
	infostr=configurationinfo();
	printf("\n<<Configuration>>\n%s",infostr);
	free(infostr);

	infostr=urlprotocolinfo();
	printf("\n<<URLProtocol>>\n%s",infostr);
	free(infostr);

	infostr=avformatinfo();
	printf("\n<<AVFormat>>\n%s",infostr);
	free(infostr);

	infostr=avcodecinfo();
	printf("\n<<AVCodec>>\n%s",infostr);
	free(infostr);

	infostr=avfilterinfo();
	printf("\n<<AVFilter>>\n%s",infostr);
	free(infostr);

	return 0;
}
```




## 运行结果




Configure信息格式如下所示。
`--disable-static --enable-shared --enable-gpl --enable-version3 --disable-w32threads --enable-avisynth --enable-bzlib --enable-fontconfig --enable-frei0r --enable-gnutls --enable-iconv --enable-libass --enable-libbluray --enable-libcaca --enable-libfreetype --enable-libgsm --enable-libilbc --enable-libmodplug --enable-libmp3lame --enable-libopencore-amrnb --enable-libopencore-amrwb --enable-libopenjpeg --enable-libopus --enable-librtmp --enable-libschroedinger --enable-libsoxr --enable-libspeex --enable-libtheora --enable-libtwolame --enable-libvidstab --enable-libvo-aacenc --enable-libvo-amrwbenc --enable-libvorbis --enable-libvpx --enable-libwavpack --enable-libx264 --enable-libx265 --enable-libxavs --enable-libxvid --enable-decklink --enable-zlib`

Protocol信息格式如下所示。

```
[In ][     cache]
[In ][    concat]
[In ][    crypto]
[In ][      data]
[In ][      file]
[In ][       ftp]
[In ][    gopher]
[In ][       hls]
[In ][      http]
[In ][ httpproxy]
[In ][     https]
[In ][      mmsh]
[In ][      mmst]
[In ][      pipe]
[In ][       rtp]
[In ][      srtp]
[In ][   subfile]
[In ][       tcp]
[In ][       tls]
[In ][       udp]
[In ][      rtmp]
[In ][     rtmpe]
[In ][     rtmps]
[In ][     rtmpt]
[In ][    rtmpte]
[In ][    (null)]
[Out][       ftp]
[Out][    gopher]
[Out][      http]
[Out][ httpproxy]
[Out][     https]
[Out][       md5]
[Out][      pipe]
[Out][       rtp]
[Out][      srtp]
[Out][       tcp]
[Out][       tls]
[Out][       udp]
[Out][      rtmp]
[Out][     rtmpe]
[Out][     rtmps]
[Out][     rtmpt]
[Out][    rtmpte]
[Out][    (null)]
```




AVFormat信息格式如下所示。




```
[In ]        aac
[In ]        ac3
[In ]        act
[In ]        adf
[In ]        adp
[In ]        adx
[In ]        aea
[In ]        afc
[In ]       aiff
[In ]        amr
[In ]        anm
[In ]        apc
[In ]        ape
[In ]    aqtitle
[In ]        asf
[In ]        ass
[In ]        ast
[In ]         au
[In ]        avi
[In ]   avisynth
[In ]        avr
[In ]        avs
[In ] bethsoftvid
[In ]        bfi
[In ]        bin
[In ]       bink
[In ]        bit
[In ]        bmv
[In ]      brstm
[In ]        boa
[In ]        c93
[In ]        caf
[In ]  cavsvideo
[In ]        cdg
[In ]       cdxl
[In ]       cine
[In ]     concat
[In ]       data
[In ]       daud
[In ]        dfa
[In ]      dirac
[In ]      dnxhd
[In ]        dsf
[In ]     dsicin
[In ]        dts
[In ]      dtshd
[In ]         dv
[In ]        dxa
[In ]         ea
[In ]   ea_cdata
[In ]       eac3
[In ]       epaf
[In ]        ffm
[In ] ffmetadata
[In ]  filmstrip
[In ]       flac
[In ]       flic
[In ]        flv
[In ]        4xm
[In ]        frm
[In ]       g722
[In ]     g723_1
[In ]       g729
[In ]        gif
[In ]        gsm
[In ]        gxf
[In ]       h261
[In ]       h263
[In ]       h264
[In ]       hevc
[In ] hls,applehttp
[In ]        hnm
[In ]        ico
[In ]      idcin
[In ]        idf
[In ]        iff
[In ]       ilbc
[In ]     image2
[In ] image2pipe
[In ]  alias_pix
[In ] brender_pix
[In ]  ingenient
[In ]    ipmovie
[In ]      ircam
[In ]        iss
[In ]        iv8
[In ]        ivf
[In ]    jacosub
[In ]         jv
[In ]       latm
[In ]      lmlm4
[In ]       loas
[In ]        lvf
[In ]        lxf
[In ]        m4v
[In ] matroska,webm
[In ]      mgsts
[In ]   microdvd
[In ]      mjpeg
[In ]        mlp
[In ]        mlv
[In ]         mm
[In ]        mmf
[In ] mov,mp4,m4a,3gp,3g2,mj2
[In ]        mp3
[In ]        mpc
[In ]       mpc8
[In ]       mpeg
[In ]     mpegts
[In ]  mpegtsraw
[In ]  mpegvideo
[In ]       mpl2
[In ]      mpsub
[In ]   msnwctcp
[In ]        mtv
[In ]         mv
[In ]        mvi
[In ]        mxf
[In ]        mxg
[In ]         nc
[In ] nistsphere
[In ]        nsv
[In ]        nut
[In ]        nuv
[In ]        ogg
[In ]        oma
[In ]        paf
[In ]       alaw
[In ]      mulaw
[In ]      f64be
[In ]      f64le
[In ]      f32be
[In ]      f32le
[In ]      s32be
[In ]      s32le
[In ]      s24be
[In ]      s24le
[In ]      s16be
[In ]      s16le
[In ]         s8
[In ]      u32be
[In ]      u32le
[In ]      u24be
[In ]      u24le
[In ]      u16be
[In ]      u16le
[In ]         u8
[In ]        pjs
[In ]        pmp
[In ]        pva
[In ]        pvf
[In ]        qcp
[In ]        r3d
[In ]   rawvideo
[In ]   realtext
[In ]   redspark
[In ]        rl2
[In ]         rm
[In ]        roq
[In ]        rpl
[In ]        rsd
[In ]        rso
[In ]        rtp
[In ]       rtsp
[In ]       sami
[In ]        sap
[In ]        sbg
[In ]        sdp
[In ]       sdr2
[In ]   film_cpk
[In ]        shn
[In ]       siff
[In ]        smk
[In ]     smjpeg
[In ]      smush
[In ]        sol
[In ]        sox
[In ]      spdif
[In ]        srt
[In ]     psxstr
[In ] subviewer1
[In ]  subviewer
[In ]        swf
[In ]        tak
[In ] tedcaptions
[In ]        thp
[In ] tiertexseq
[In ]        tmv
[In ]     truehd
[In ]        tta
[In ]        txd
[In ]        tty
[In ]        vc1
[In ]    vc1test
[In ]       vivo
[In ]        vmd
[In ]     vobsub
[In ]        voc
[In ]    vplayer
[In ]        vqf
[In ]        w64
[In ]        wav
[In ]   wc3movie
[In ]     webvtt
[In ]      wsaud
[In ]      wsvqa
[In ]        wtv
[In ]         wv
[In ]         xa
[In ]       xbin
[In ]        xmv
[In ]       xwma
[In ]        yop
[In ] yuv4mpegpipe
[In ] libmodplug
[Out]        a64
[Out]        ac3
[Out]       adts
[Out]        adx
[Out]       aiff
[Out]        amr
[Out]        asf
[Out]        ass
[Out]        ast
[Out] asf_stream
[Out]         au
[Out]        avi
[Out]       avm2
[Out]        bit
[Out]        caf
[Out]  cavsvideo
[Out]        crc
[Out]       data
[Out]       daud
[Out]      dirac
[Out]      dnxhd
[Out]        dts
[Out]         dv
[Out]       eac3
[Out]        f4v
[Out]        ffm
[Out] ffmetadata
[Out]  filmstrip
[Out]       flac
[Out]        flv
[Out]   framecrc
[Out]   framemd5
[Out]       g722
[Out]     g723_1
[Out]        gif
[Out]        gxf
[Out]       h261
[Out]       h263
[Out]       h264
[Out]        hds
[Out]       hevc
[Out]        hls
[Out]        ico
[Out]       ilbc
[Out]     image2
[Out] image2pipe
[Out]       ipod
[Out]      ircam
[Out]       ismv
[Out]        ivf
[Out]    jacosub
[Out]       latm
[Out]        m4v
[Out]        md5
[Out]   matroska
[Out]   matroska
[Out]   microdvd
[Out]      mjpeg
[Out]        mlp
[Out]        mmf
[Out]        mov
[Out]        mp2
[Out]        mp3
[Out]        mp4
[Out]       mpeg
[Out]        vcd
[Out] mpeg1video
[Out]        dvd
[Out]       svcd
[Out] mpeg2video
[Out]        vob
[Out]     mpegts
[Out]     mpjpeg
[Out]        mxf
[Out]    mxf_d10
[Out]       null
[Out]        nut
[Out]        oga
[Out]        ogg
[Out]        oma
[Out]       opus
[Out]       alaw
[Out]      mulaw
[Out]      f64be
[Out]      f64le
[Out]      f32be
[Out]      f32le
[Out]      s32be
[Out]      s32le
[Out]      s24be
[Out]      s24le
[Out]      s16be
[Out]      s16le
[Out]         s8
[Out]      u32be
[Out]      u32le
[Out]      u24be
[Out]      u24le
[Out]      u16be
[Out]      u16le
[Out]         u8
[Out]        psp
[Out]   rawvideo
[Out]         rm
[Out]        roq
[Out]        rso
[Out]        rtp
[Out]       rtsp
[Out]        sap
[Out]    segment
[Out] stream_segment,ssegment
[Out]     smjpeg
[Out] smoothstreaming
[Out]        sox
[Out]      spdif
[Out]      speex
[Out]        srt
[Out]        swf
[Out]        tee
[Out]        3g2
[Out]        3gp
[Out] mkvtimestamp_v2
[Out]     truehd
[Out] uncodedframecrc
[Out]        vc1
[Out]    vc1test
[Out]        voc
[Out]        w64
[Out]        wav
[Out]       webm
[Out]     webvtt
[Out]        wtv
[Out]         wv
[Out] yuv4mpegpipe
```




AVCodec信息格式如下所示。

```
[Enc][Video]   a64multi
[Enc][Video]  a64multi5
[Dec][Video]       aasc
[Dec][Video]        aic
[Enc][Video]  alias_pix
[Dec][Video]  alias_pix
[Enc][Video]        amv
[Dec][Video]        amv
[Dec][Video]        anm
[Dec][Video]       ansi
[Enc][Video]       asv1
[Dec][Video]       asv1
[Enc][Video]       asv2
[Dec][Video]       asv2
[Dec][Video]       aura
[Dec][Video]      aura2
[Enc][Video]       avrp
[Dec][Video]       avrp
[Dec][Video]       avrn
[Dec][Video]        avs
[Enc][Video]       avui
[Dec][Video]       avui
[Enc][Video]       ayuv
[Dec][Video]       ayuv
[Dec][Video] bethsoftvid
[Dec][Video]        bfi
[Dec][Video]  binkvideo
[Enc][Video]        bmp
[Dec][Video]        bmp
[Dec][Video]  bmv_video
[Dec][Video] brender_pix
[Dec][Video]        c93
[Dec][Video]       cavs
[Dec][Video] cdgraphics
[Dec][Video]       cdxl
[Enc][Video]    cinepak
[Dec][Video]    cinepak
[Enc][Video]       cljr
[Dec][Video]       cljr
[Dec][Video]       cllc
[Enc][Audio] comfortnoise
[Dec][Audio] comfortnoise
[Dec][Video]       cpia
[Dec][Video]  camstudio
[Dec][Video]       cyuv
[Dec][Video]        dfa
[Dec][Video]      dirac
[Enc][Video]      dnxhd
[Dec][Video]      dnxhd
[Enc][Video]        dpx
[Dec][Video]        dpx
[Dec][Video] dsicinvideo
[Enc][Video]    dvvideo
[Dec][Video]    dvvideo
[Dec][Video]        dxa
[Dec][Video]     dxtory
[Dec][Video]      eacmv
[Dec][Video]      eamad
[Dec][Video]      eatgq
[Dec][Video]      eatgv
[Dec][Video]      eatqi
[Dec][Video]       8bps
[Dec][Audio]   8svx_exp
[Dec][Audio]   8svx_fib
[Dec][Video]  escape124
[Dec][Video]  escape130
[Dec][Video]        exr
[Enc][Video]       ffv1
[Dec][Video]       ffv1
[Enc][Video]    ffvhuff
[Dec][Video]    ffvhuff
[Dec][Video]        fic
[Enc][Video]    flashsv
[Dec][Video]    flashsv
[Enc][Video]   flashsv2
[Dec][Video]   flashsv2
[Dec][Video]       flic
[Enc][Video]        flv
[Dec][Video]        flv
[Dec][Video]        4xm
[Dec][Video]      fraps
[Dec][Video]       frwu
[Dec][Video]        g2m
[Enc][Video]        gif
[Dec][Video]        gif
[Enc][Video]       h261
[Dec][Video]       h261
[Enc][Video]       h263
[Dec][Video]       h263
[Dec][Video]      h263i
[Enc][Video]      h263p
[Dec][Video]      h263p
[Dec][Video]       h264
[Dec][Video]       hevc
[Dec][Video]  hnm4video
[Enc][Video]    huffyuv
[Dec][Video]    huffyuv
[Dec][Video] idcinvideo
[Dec][Video]        iff
[Dec][Video]        iff
[Dec][Video]     indeo2
[Dec][Video]     indeo3
[Dec][Video]     indeo4
[Dec][Video]     indeo5
[Dec][Video] interplayvideo
[Enc][Video]   jpeg2000
[Dec][Video]   jpeg2000
[Enc][Video]     jpegls
[Dec][Video]     jpegls
[Dec][Video]         jv
[Dec][Video]       kgv1
[Dec][Video]       kmvc
[Dec][Video]   lagarith
[Enc][Video]      ljpeg
[Dec][Video]       loco
[Dec][Video]       mdec
[Dec][Video]      mimic
[Enc][Video]      mjpeg
[Dec][Video]      mjpeg
[Dec][Video]     mjpegb
[Dec][Video]    mmvideo
[Dec][Video] motionpixels
[Enc][Video] mpeg1video
[Dec][Video] mpeg1video
[Enc][Video] mpeg2video
[Dec][Video] mpeg2video
[Enc][Video]      mpeg4
[Dec][Video]      mpeg4
[Dec][Video]  mpegvideo
[Dec][Video]       msa1
[Dec][Video]  msmpeg4v1
[Enc][Video]  msmpeg4v2
[Dec][Video]  msmpeg4v2
[Enc][Video]    msmpeg4
[Dec][Video]    msmpeg4
[Dec][Video]      msrle
[Dec][Video]       mss1
[Dec][Video]       mss2
[Enc][Video]   msvideo1
[Dec][Video]   msvideo1
[Dec][Video]       mszh
[Dec][Video]       mts2
[Dec][Video]       mvc1
[Dec][Video]       mvc2
[Dec][Video]      mxpeg
[Dec][Video]        nuv
[Dec][Video]  paf_video
[Enc][Video]        pam
[Dec][Video]        pam
[Enc][Video]        pbm
[Dec][Video]        pbm
[Enc][Video]        pcx
[Dec][Video]        pcx
[Enc][Video]        pgm
[Dec][Video]        pgm
[Enc][Video]     pgmyuv
[Dec][Video]     pgmyuv
[Dec][Video]     pictor
[Enc][Video]        png
[Dec][Video]        png
[Enc][Video]        ppm
[Dec][Video]        ppm
[Enc][Video]     prores
[Dec][Video]     prores
[Enc][Video]  prores_aw
[Enc][Video]  prores_ks
[Dec][Video] prores_lgpl
[Dec][Video]        ptx
[Dec][Video]      qdraw
[Dec][Video]       qpeg
[Enc][Video]      qtrle
[Dec][Video]      qtrle
[Enc][Video]       r10k
[Dec][Video]       r10k
[Enc][Video]       r210
[Dec][Video]       r210
[Enc][Video]   rawvideo
[Dec][Video]   rawvideo
[Dec][Video]        rl2
[Enc][Video]   roqvideo
[Dec][Video]   roqvideo
[Dec][Video]       rpza
[Enc][Video]       rv10
[Dec][Video]       rv10
[Enc][Video]       rv20
[Dec][Video]       rv20
[Dec][Video]       rv30
[Dec][Video]       rv40
[Enc][Audio]      s302m
[Dec][Audio]      s302m
[Dec][Video]       sanm
[Enc][Video]        sgi
[Dec][Video]        sgi
[Dec][Video]     sgirle
[Dec][Video]   smackvid
[Dec][Video]        smc
[Dec][Video]    smvjpeg
[Enc][Video]       snow
[Dec][Video]       snow
[Dec][Video]       sp5x
[Enc][Video]    sunrast
[Dec][Video]    sunrast
[Enc][Video]       svq1
[Dec][Video]       svq1
[Dec][Video]       svq3
[Enc][Video]      targa
[Dec][Video]      targa
[Dec][Video] targa_y216
[Dec][Video]     theora
[Dec][Video]        thp
[Dec][Video] tiertexseqvideo
[Enc][Video]       tiff
[Dec][Video]       tiff
[Dec][Video]        tmv
[Dec][Video] truemotion1
[Dec][Video] truemotion2
[Dec][Video]   camtasia
[Dec][Video]      tscc2
[Dec][Video]        txd
[Dec][Video] ultimotion
[Enc][Video]    utvideo
[Dec][Video]    utvideo
[Enc][Video]       v210
[Dec][Video]       v210
[Dec][Video]      v210x
[Enc][Video]       v308
[Dec][Video]       v308
[Enc][Video]       v408
[Dec][Video]       v408
[Enc][Video]       v410
[Dec][Video]       v410
[Dec][Video]         vb
[Dec][Video]       vble
[Dec][Video]        vc1
[Dec][Video]   vc1image
[Dec][Video]       vcr1
[Dec][Video]   vmdvideo
[Dec][Video]       vmnc
[Dec][Video]        vp3
[Dec][Video]        vp5
[Dec][Video]        vp6
[Dec][Video]       vp6a
[Dec][Video]       vp6f
[Dec][Video]        vp7
[Dec][Video]        vp8
[Dec][Video]        vp9
[Dec][Video]   vqavideo
[Dec][Video]       webp
[Enc][Video]       wmv1
[Dec][Video]       wmv1
[Enc][Video]       wmv2
[Dec][Video]       wmv2
[Dec][Video]       wmv3
[Dec][Video]  wmv3image
[Dec][Video]       wnv1
[Dec][Video]    xan_wc3
[Dec][Video]    xan_wc4
[Enc][Video]        xbm
[Dec][Video]        xbm
[Enc][Video]      xface
[Dec][Video]      xface
[Dec][Video]         xl
[Enc][Video]        xwd
[Dec][Video]        xwd
[Enc][Video]       y41p
[Dec][Video]       y41p
[Dec][Video]        yop
[Enc][Video]       yuv4
[Dec][Video]       yuv4
[Dec][Video]       012v
[Dec][Video]  zerocodec
[Enc][Video]       zlib
[Dec][Video]       zlib
[Enc][Video]       zmbv
[Dec][Video]       zmbv
[Enc][Audio]        aac
[Dec][Audio]        aac
[Dec][Audio]   aac_latm
[Enc][Audio]        ac3
[Dec][Audio]        ac3
[Enc][Audio]  ac3_fixed
[Dec][Audio]  ac3_fixed
[Enc][Audio]       alac
[Dec][Audio]       alac
[Dec][Audio]        als
[Dec][Audio]      amrnb
[Dec][Audio]      amrwb
[Dec][Audio]        ape
[Dec][Audio]     atrac1
[Dec][Audio]     atrac3
[Dec][Audio] atrac3plus
[Dec][Audio] binkaudio_dct
[Dec][Audio] binkaudio_rdft
[Dec][Audio]  bmv_audio
[Dec][Audio]       cook
[Enc][Audio]        dca
[Dec][Audio]        dca
[Dec][Audio]   dsd_lsbf
[Dec][Audio]   dsd_msbf
[Dec][Audio] dsd_lsbf_planar
[Dec][Audio] dsd_msbf_planar
[Dec][Audio] dsicinaudio
[Enc][Audio]       eac3
[Dec][Audio]       eac3
[Dec][Audio]       evrc
[Dec][Audio]  wavesynth
[Enc][Audio]       flac
[Dec][Audio]       flac
[Enc][Audio]     g723_1
[Dec][Audio]     g723_1
[Dec][Audio]       g729
[Dec][Audio]        gsm
[Dec][Audio]     gsm_ms
[Dec][Audio]        iac
[Dec][Audio]        imc
[Dec][Audio]      mace3
[Dec][Audio]      mace6
[Dec][Audio]  metasound
[Dec][Audio]        mlp
[Dec][Audio]        mp1
[Dec][Audio]   mp1float
[Enc][Audio]        mp2
[Dec][Audio]        mp2
[Dec][Audio]   mp2float
[Enc][Audio]   mp2fixed
[Dec][Audio]        mp3
[Dec][Audio]   mp3float
[Dec][Audio]     mp3adu
[Dec][Audio] mp3adufloat
[Dec][Audio]     mp3on4
[Dec][Audio] mp3on4float
[Dec][Audio]       mpc7
[Dec][Audio]       mpc8
[Enc][Audio] nellymoser
[Dec][Audio] nellymoser
[Dec][Audio]     on2avc
[Dec][Audio]  paf_audio
[Dec][Audio]      qcelp
[Dec][Audio]       qdm2
[Enc][Audio]   real_144
[Dec][Audio]   real_144
[Dec][Audio]   real_288
[Dec][Audio]       ralf
[Dec][Audio]    shorten
[Dec][Audio]       sipr
[Dec][Audio]   smackaud
[Enc][Audio]      sonic
[Dec][Audio]      sonic
[Enc][Audio]    sonicls
[Dec][Audio]        tak
[Dec][Audio]     truehd
[Dec][Audio] truespeech
[Enc][Audio]        tta
[Dec][Audio]        tta
[Dec][Audio]     twinvq
[Dec][Audio]   vmdaudio
[Enc][Audio]     vorbis
[Dec][Audio]     vorbis
[Enc][Audio]    wavpack
[Dec][Audio]    wavpack
[Dec][Audio] wmalossless
[Dec][Audio]     wmapro
[Enc][Audio]      wmav1
[Dec][Audio]      wmav1
[Enc][Audio]      wmav2
[Dec][Audio]      wmav2
[Dec][Audio]   wmavoice
[Dec][Audio]    ws_snd1
[Enc][Audio]   pcm_alaw
[Dec][Audio]   pcm_alaw
[Dec][Audio] pcm_bluray
[Dec][Audio]    pcm_dvd
[Enc][Audio]  pcm_f32be
[Dec][Audio]  pcm_f32be
[Enc][Audio]  pcm_f32le
[Dec][Audio]  pcm_f32le
[Enc][Audio]  pcm_f64be
[Dec][Audio]  pcm_f64be
[Enc][Audio]  pcm_f64le
[Dec][Audio]  pcm_f64le
[Dec][Audio]    pcm_lxf
[Enc][Audio]  pcm_mulaw
[Dec][Audio]  pcm_mulaw
[Enc][Audio]     pcm_s8
[Dec][Audio]     pcm_s8
[Enc][Audio] pcm_s8_planar
[Dec][Audio] pcm_s8_planar
[Enc][Audio]  pcm_s16be
[Dec][Audio]  pcm_s16be
[Enc][Audio] pcm_s16be_planar
[Dec][Audio] pcm_s16be_planar
[Enc][Audio]  pcm_s16le
[Dec][Audio]  pcm_s16le
[Enc][Audio] pcm_s16le_planar
[Dec][Audio] pcm_s16le_planar
[Enc][Audio]  pcm_s24be
[Dec][Audio]  pcm_s24be
[Enc][Audio] pcm_s24daud
[Dec][Audio] pcm_s24daud
[Enc][Audio]  pcm_s24le
[Dec][Audio]  pcm_s24le
[Enc][Audio] pcm_s24le_planar
[Dec][Audio] pcm_s24le_planar
[Enc][Audio]  pcm_s32be
[Dec][Audio]  pcm_s32be
[Enc][Audio]  pcm_s32le
[Dec][Audio]  pcm_s32le
[Enc][Audio] pcm_s32le_planar
[Dec][Audio] pcm_s32le_planar
[Enc][Audio]     pcm_u8
[Dec][Audio]     pcm_u8
[Enc][Audio]  pcm_u16be
[Dec][Audio]  pcm_u16be
[Enc][Audio]  pcm_u16le
[Dec][Audio]  pcm_u16le
[Enc][Audio]  pcm_u24be
[Dec][Audio]  pcm_u24be
[Enc][Audio]  pcm_u24le
[Dec][Audio]  pcm_u24le
[Enc][Audio]  pcm_u32be
[Dec][Audio]  pcm_u32be
[Enc][Audio]  pcm_u32le
[Dec][Audio]  pcm_u32le
[Dec][Audio]   pcm_zork
[Dec][Audio] interplay_dpcm
[Enc][Audio]   roq_dpcm
[Dec][Audio]   roq_dpcm
[Dec][Audio]   sol_dpcm
[Dec][Audio]   xan_dpcm
[Dec][Audio]  adpcm_4xm
[Enc][Audio]  adpcm_adx
[Dec][Audio]  adpcm_adx
[Dec][Audio]  adpcm_afc
[Dec][Audio]   adpcm_ct
[Dec][Audio]  adpcm_dtk
[Dec][Audio]   adpcm_ea
[Dec][Audio] adpcm_ea_maxis_xa
[Dec][Audio] adpcm_ea_r1
[Dec][Audio] adpcm_ea_r2
[Dec][Audio] adpcm_ea_r3
[Dec][Audio] adpcm_ea_xas
[Enc][Audio]       g722
[Dec][Audio]       g722
[Enc][Audio]       g726
[Dec][Audio]       g726
[Dec][Audio]     g726le
[Dec][Audio] adpcm_ima_amv
[Dec][Audio] adpcm_ima_apc
[Dec][Audio] adpcm_ima_dk3
[Dec][Audio] adpcm_ima_dk4
[Dec][Audio] adpcm_ima_ea_eacs
[Dec][Audio] adpcm_ima_ea_sead
[Dec][Audio] adpcm_ima_iss
[Dec][Audio] adpcm_ima_oki
[Enc][Audio] adpcm_ima_qt
[Dec][Audio] adpcm_ima_qt
[Dec][Audio] adpcm_ima_rad
[Dec][Audio] adpcm_ima_smjpeg
[Enc][Audio] adpcm_ima_wav
[Dec][Audio] adpcm_ima_wav
[Dec][Audio] adpcm_ima_ws
[Enc][Audio]   adpcm_ms
[Dec][Audio]   adpcm_ms
[Dec][Audio] adpcm_sbpro_2
[Dec][Audio] adpcm_sbpro_3
[Dec][Audio] adpcm_sbpro_4
[Enc][Audio]  adpcm_swf
[Dec][Audio]  adpcm_swf
[Dec][Audio]  adpcm_thp
[Dec][Audio] adpcm_vima
[Dec][Audio]   adpcm_xa
[Enc][Audio] adpcm_yamaha
[Dec][Audio] adpcm_yamaha
[Dec][Audio]       vima
[Enc][Other]        ssa
[Dec][Other]        ssa
[Enc][Other]        ass
[Dec][Other]        ass
[Enc][Other]     dvbsub
[Dec][Other]     dvbsub
[Enc][Other]     dvdsub
[Dec][Other]     dvdsub
[Dec][Other]    jacosub
[Dec][Other]   microdvd
[Enc][Other]   mov_text
[Dec][Other]   mov_text
[Dec][Other]       mpl2
[Dec][Other]     pgssub
[Dec][Other]        pjs
[Dec][Other]   realtext
[Dec][Other]       sami
[Enc][Other]        srt
[Dec][Other]        srt
[Enc][Other]     subrip
[Dec][Other]     subrip
[Dec][Other]  subviewer
[Dec][Other] subviewer1
[Dec][Other]       text
[Dec][Other]    vplayer
[Dec][Other]     webvtt
[Enc][Other]       xsub
[Dec][Other]       xsub
[Enc][Audio]     libgsm
[Dec][Audio]     libgsm
[Enc][Audio]  libgsm_ms
[Dec][Audio]  libgsm_ms
[Enc][Audio]    libilbc
[Dec][Audio]    libilbc
[Enc][Audio] libmp3lame
[Enc][Audio] libopencore_amrnb
[Dec][Audio] libopencore_amrnb
[Dec][Audio] libopencore_amrwb
[Enc][Video] libopenjpeg
[Dec][Video] libopenjpeg
[Enc][Audio]    libopus
[Dec][Audio]    libopus
[Enc][Video] libschroedinger
[Dec][Video] libschroedinger
[Enc][Audio]   libspeex
[Dec][Audio]   libspeex
[Enc][Video]  libtheora
[Enc][Audio] libtwolame
[Enc][Audio] libvo_aacenc
[Enc][Audio] libvo_amrwbenc
[Enc][Audio]  libvorbis
[Dec][Audio]  libvorbis
[Enc][Video]     libvpx
[Dec][Video]     libvpx
[Enc][Video] libvpx-vp9
[Dec][Video] libvpx-vp9
[Enc][Audio] libwavpack
[Enc][Video]    libx264
[Enc][Video] libx264rgb
[Enc][Video]    libx265
[Enc][Video]    libxavs
[Enc][Video]    libxvid
[Dec][Video]    bintext
[Dec][Video]       xbin
[Dec][Video]        idf
```



## 下载

**Simplest FFmpeg Player**



**项目主页**

SourceForge：[https://sourceforge.net/projects/simplestffmpegplayer/](https://sourceforge.net/projects/simplestffmpegplayer/)

Github：[https://github.com/leixiaohua1020/simplest_ffmpeg_player](https://github.com/leixiaohua1020/simplest_ffmpeg_player)

开源中国：[http://git.oschina.net/leixiaohua1020/simplest_ffmpeg_player](http://git.oschina.net/leixiaohua1020/simplest_ffmpeg_player)

CSDN下载地址：[http://download.csdn.net/detail/leixiaohua1020/8924321](http://download.csdn.net/detail/leixiaohua1020/8924321)



本程序实现了视频文件的解码和显示（支持HEVC，H.264，MPEG2等）。
是最简单的FFmpeg视频解码方面的教程。
通过学习本例子可以了解FFmpeg的解码流程。
项目包含6个工程：

> simplest_ffmpeg_player：标准版，FFmpeg学习的开始。
simplest_ffmpeg_player_su：SU（SDL Update）版，加入了简单的SDL的Event。
simplest_ffmpeg_decoder：一个包含了封装格式处理功能的解码器。使用了libavcodec和libavformat。
simplest_ffmpeg_decoder_pure：一个纯净的解码器。只使用libavcodec（没有使用libavformat）。
simplest_video_play_sdl2：使用SDL2播放YUV的例子。
simplest_ffmpeg_helloworld：输出FFmpeg类库的信息。



