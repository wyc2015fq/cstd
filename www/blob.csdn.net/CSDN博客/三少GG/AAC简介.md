# AAC简介 - 三少GG - CSDN博客
2012年06月11日 14:35:59[三少GG](https://me.csdn.net/scut1135)阅读数：3665
```cpp
/**
 * copyright (c) 2007 Luca Abeni
 *
 * This file is part of FFmpeg.
 *
 * FFmpeg is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * FFmpeg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with FFmpeg; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */
#include "avformat.h"
#include "rtpenc.h"
void ff_rtp_send_aac(AVFormatContext *s1, const uint8_t *buff, int size)
{
    RTPMuxContext *s = s1->priv_data;
    int len, max_packet_size;
    uint8_t *p;
    const int max_frames_per_packet = s->max_frames_per_packet ? s->max_frames_per_packet : 5;
    const int max_au_headers_size = 2 + 2 * max_frames_per_packet;
    /** skip ADTS header, if present */
    if ((s1->streams[0]->codec->extradata_size) == 0) {
        size -= 7;
        buff += 7;
    }
    max_packet_size = s->max_payload_size - max_au_headers_size;
    /** test if the packet must be sent */
    len = (s->buf_ptr - s->buf);
    if ((s->num_frames == max_frames_per_packet) || (len && (len + size) > s->max_payload_size)) {
        int au_size = s->num_frames * 2;
        p = s->buf + max_au_headers_size - au_size - 2;
        if (p != s->buf) {
            memmove(p + 2, s->buf + 2, au_size);
        }
        /** Write the AU header size */
        p[0] = ((au_size * 8) & 0xFF) >> 8;
        p[1] = (au_size * 8) & 0xFF;
        ff_rtp_send_data(s1, p, s->buf_ptr - p, 1);
        s->num_frames = 0;
    }
    if (s->num_frames == 0) {
        s->buf_ptr = s->buf + max_au_headers_size;
        s->timestamp = s->cur_timestamp;
    }
    if (size <= max_packet_size) {
        p = s->buf + s->num_frames++ * 2 + 2;
        *p++ = size >> 5;
        *p = (size & 0x1F) << 3;
        memcpy(s->buf_ptr, buff, size);
        s->buf_ptr += size;
    } else {
        int au_size = size;
        max_packet_size = s->max_payload_size - 4;
        p = s->buf;
        p[0] = 0;
        p[1] = 16;
        while (size > 0) {
            len = FFMIN(size, max_packet_size);
            p[2] = au_size >> 5;
            p[3] = (au_size & 0x1F) << 3;
            memcpy(p + 4, buff, len);
            ff_rtp_send_data(s1, p, len + 4, len == size);
            size -= len;
            buff += len;
        }
    }
}
```
AAC简介
![](http://hiphotos.baidu.com/cumt_xiaopeng/pic/item/27f29498033b5bb50f8cf74836d3d539b700bc73.jpg)
在MP3之后，[3G](http://baike.ofweek.com/575.html)PP又制定了一系列的语音编码(压缩)新标准──AAC和AMR家族。AAC家族包含了AAC、HE-AAC、EAAC+;其竞争对手AMR家族则包含了：AMR、AMR-WB、AMR-WB+。这些新技术足够让人眼花缭乱，但对手机芯片、制造业者和工程师而言，这是一个崭新的机会。
　　1.1 AAC
　　AAC(Advanced Audio Coding)也称为MPEG-2 AAC，是一种数据会遗失(lossy)的语音串流压缩标准。AAC是MPEG-2的一部份，是用来取代MP3的，但是AAC和MP3不同，AAC无法向后兼容，MP3可以，例如：MP3可以和MP2兼容。
　　AAC最多可以支持48个频道，取样率从8 kHz到96 kHz。AAC的语音分辨率(resolution)比MP3高，一般而言，96 kbps的AAC之语音品质优于或等同于128 kbps的MP3，因此，AAC可以在低速的[网络](http://ee.ofweek.com/CAT-2828-CommunicationNetwork.html)内传输语音串流，而且，不会影响语音的品质。
　　1.2 HE-AAC(AAC+)
　　HE(high eff[ic](http://ee.ofweek.com/CAT-2801-ICDesign.html)iency) AAC又称为MPEG-4 HE-AAC或简称AAC+，它是MPEG-2 AAC和SBR(Spectral Band Replication)带宽延伸修订版的技术组合。HE-AAC不是要取代AAC，而是要延伸MPEG-4的语音品质，能够以更低的速率传输(32
 kbps)。而且，HE-AAC译码器可以对AAC解碼。
　　若要产生48 kbps的HE-AAC立体声，HE-AAC编码器会产生两种信号：一个是42 kbps的MPEG AAC信号，另一个是6 kbps的SBR信号。然后，此SBR信号置于MPEG AAC的辅助字段内(该字段是MPEG-4定义的)。最后，构成一个完整48 kbps的MPEG-4 HE-AAC串流。SBR代表高频的成份，而AAC代表低频的成份。HE-AAC译码器使用AAC和SBR信号，产生全频信号;而AAC译码器只使用AAC信号，亦即，只有低频成份被它解碼。
　　HE-AAC能传输48 kbps的CD立体声，或128 kbps、5.1声道的“环场声”(surround sound)。这样的效率，使它适用于Internet传输，或[移动](http://ee.ofweek.com/CAT-2831-Mobile.html)数字广播。不过，由于HE-AAC的高延迟特性，使它不适用于双向的[通信](http://ee.ofweek.com/CAT-2828-CommunicationNetwork.html)应用。
　　1.3 EAAC+
　　Enhanced AAC+(EAAC+)是在2004年时，纳入3GPP的第6版标准中。根据3GPP，它是由MPEG-4 AAC、MPEG-4 SBR和MPEG-4“参数立体声”(Parametric Stereo)技术组合的。“参数立体声”技术能够在低传输率中，进行“立体声”的编码，其基本原理类似SBR。

++++++++++++++++++++++++++++++++++++++++++++++++++++
Grant推荐：
- [Google Android](http://blog.csdn.net/evilcode/article/category/702111)(114)
- [Audio and Video](http://blog.csdn.net/evilcode/article/category/932884)(10)
- [ALSA](http://blog.csdn.net/evilcode/article/category/1152440)(12)
AAC音频编码在不同的领域一共有9种
MPEG-2 AAC Main
MPEG-2 AAC LC (Low Complexity)
MPEG-2 AAC SSR (Scalable Sampling Rate)
MPEG-4 AAC Main
MPEG-4 AAC LC (Low Complexity)
MPEG-4 AAC SSR (Scalable Sample Rate)
MPEG-4 AAC LTP (Long Term Predicition)
MPEG-4 AAC LD (Low Delay)
MPEG-4 AAC HE (High Efficiency)    AACPlusV1/V2(3GPP)
MPEG-4 AAC LC (Low Complexity) 是最常用的规格，我们叫“低复杂度规格”，我们简称“LC-AAC”，这种规格在中等码率的编码效率以及音质方面，都能找到平衡点。所谓中等码率，就是指：96kbps-192kbps之间的码率。因此，如果要使用LC-AAC规格，请尽可能把码率控制在之前说的那个区间内。在这个区间内，LC-AAC
 可以完全打败同码率的用LAME最高质量慢速编码模式的MP3。我认为，不论是随身听，还是互联网音乐传播，都建议采用128kbps-192kbps规格的LC-AAC编码规格。
MPEG-4 AAC HE (High Efficiency)AACPlusV1/V2(3GPP)，我们叫“高效性规格”，我们简称为“HE-AAC”或者“AACPlus”，这种规格适合用于低码率编码，因为这种规格也融入了Mp3Pro中的那种SBR技术。当您需要编码32kbps-96kbps之间的音频文件时，建议首选“HE-AAC”编码规格。使用这种规格，在相同码率下，在这个区间内，您可以轻易把WMA9以及WMAPro等微软的格式给干掉。如果您是有声小说爱好者，或者广播剧爱好者，不妨考虑He-AAC规格，即使很低的码率，也能提供非常清晰的人声对白效果。
那么，如果您需要编码相对较高码率的AAC（192kbps-384kbps，或更高），应该如何选择？如果您的码率不超过256kbps（如：192kbps-256kbps），“LC-AAC”同样可以适用。如果实在需要更高的码率，请选择Main（主规格）编码。在高码率下，AAC同样可以轻易战胜那个炒得很火热的LameMP3，即使320kbps CBR的MP3，也根本不是Mian-AAC的对手。甚至不少人认为，中码率的LC-AAC，192kbpsCBR已经达到了320 CBR LameMP3的音质。当然，能和高码率AAC抗衡的，也只有MPC格式，不过MPC格式通用性太差了。话说回来，其实没必要用高码率的AAC，而且Main-AAC在某些通用的播放器或解码器会存在问题，因为之前说的LC-AAC音质已经足够好了，并且通用性好，或者干脆直接用无损，比如FLAC等。
## AAC ADTS LATM 格式分析
## [分享一段H264视频和AAC音频的RTP封包代码](http://www.rosoo.net/a/201107/14649.html)
**2. AAC的RTP封包**
static int aac_parse(Track *tr, uint8_t *data, size_t len)
{
    //XXX handle the last packet on EOF
    int off = 0;
    uint32_t payload = DEFAULT_MTU - AU_HEADER_SIZE;
    uint8_t *packet = g_malloc0(DEFAULT_MTU);
    if(!packet) return ERR_ALLOC;
// trim away extradata
//    data += AAC_EXTRA;
//    len -= AAC_EXTRA;
    packet[0] = 0x00;
    packet[1] = 0x10;
    packet[2] = (len & 0x1fe0) >> 5;
    packet[3] = (len & 0x1f) << 3;
    if (len > payload) {
        while (len > payload) {
            memcpy(packet + AU_HEADER_SIZE, data + off, payload);
            mparser_buffer_write(tr,
                                 tr->properties.pts,
                                 tr->properties.dts,
                                 tr->properties.frame_duration,
                                 0,
                                 packet, DEFAULT_MTU);
            len -= payload;
            off += payload;
        }
    }
    memcpy(packet + AU_HEADER_SIZE, data + off, len);
    mparser_buffer_write(tr,
                         tr->properties.pts,
                         tr->properties.dts,
                         tr->properties.frame_duration,
                         1,
                         packet, len + AU_HEADER_SIZE);
    g_free(packet);
    return ERR_NOERROR;
}
上面的变量 AU_HEADER_SIZE=4
/* au header
      +---------------------------------------+
      |     AU-size                           |
      +---------------------------------------+
      |     AU-Index / AU-Index-delta         |
      +---------------------------------------+
      |     CTS-flag                          |
      +---------------------------------------+
      |     CTS-delta                         |
      +---------------------------------------+
      |     DTS-flag                          |
      +---------------------------------------+
      |     DTS-delta                         |
      +---------------------------------------+
      |     RAP-flag                          |
      +---------------------------------------+
      |     Stream-state                      |
      +---------------------------------------+
*/

RTP发送AAC音频的问题
使用ffmpeg库对声音进行aac编码，写入aac格式的文件，使用播放器可以正常播放，这说明编码没有问题。现在要使用RTP把编好的aac音频发送到摄像机，摄像机能收到，但解不了码，查了资料rfc3640里，应该是我封装AAC的RTP包时，不标准，看rfc3640也看不明白，有知道怎么把aac封RTP包的，请给点指示或例子，谢谢了
------解决方案--------------------------------------------------------
AAC封装RTP比较简单
将AAC的ADTS头去掉
12字节RTP头后紧跟着2个字节的AU_HEADER_LENGTH,
然后是2字节的AU_HEADER(2 bytes: 13 bits = length of frame, 3 bits = AU-Index(-delta)))，之后就是AAC payload
FFMPEG库中的rtpenc_aac.c文件就是将AAC打包RTP格式。。。。

