# ffmpeg怎么样处理网络流 - 建建的博客 - CSDN博客
2018年04月11日 16:32:41[纪建](https://me.csdn.net/u013898698)阅读数：135

```java
#include
```
```java
"utils.h"
```
```java
#include <pthread.h>
```
```java
#include <libavcodec/avcodec.h>
```
```java
#include <libavformat/avformat.h>
```
```java
UdpQueue recvqueue;
```
```java
UdpParam udpParam;
```
```java
//注册av_read_frame的[回调函数](https://www.baidu.com/s?wd=%E5%9B%9E%E8%B0%83%E5%87%BD%E6%95%B0&from=1012015a&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1dBPHcsmyFBnA7hnjI-nAFh0ZwV5Hcvrjm3rH6sPfKWUMw85HfYnjn4nH6sgvPsT6KdThsqpZwYTjCEQLGCpyw9Uz4Bmy-bIi4WUvYETgN-TLwGUv3EPWcdP1cYnHm3nHDLnHRkrjfY)，这里只是最简处理，实际应用中应加上出错处理，超时等待...
```
```java
int
```
```java
read_data(
```
```java
void
```
```java
*opaque, uint8_t *buf,
```
```java
int
```
```java
buf_size) {
```
```java
int
```
```java
size = buf_size;
```
```java
int
```
```java
ret;
```
```java
// printf("read data %d\n", buf_size);
```
```java
do
```
```java
{
```
```java
ret = get_queue(&recvqueue, buf, buf_size);
```
```java
}
```
```java
while
```
```java
(ret);
```
```java
// printf("read data Ok %d\n", buf_size);
```
```java
return
```
```java
size;
```
```java
}
```
```java
#define BUF_SIZE
```
```java
4096
```
```java
*
```
```java
500
```
```java
int
```
```java
main(
```
```java
int
```
```java
argc,
```
```java
char
```
```java
** argv) {
```
```java
init_queue(&recvqueue,
```
```java
1024
```
```java
*
```
```java
500
```
```java
);
```
```java
udpParam.argv = argv;
```
```java
udpParam.queue = &recvqueue;
```
```java
uint8_t *buf = av_mallocz(sizeof(uint8_t)*BUF_SIZE);
```
```java
//UDP接收线程
```
```java
[pthread_t](https://www.baidu.com/s?wd=pthread_t&from=1012015a&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1dBPHcsmyFBnA7hnjI-nAFh0ZwV5Hcvrjm3rH6sPfKWUMw85HfYnjn4nH6sgvPsT6KdThsqpZwYTjCEQLGCpyw9Uz4Bmy-bIi4WUvYETgN-TLwGUv3EPWcdP1cYnHm3nHDLnHRkrjfY) udp_recv_thread;
```
```java
[pthread_create](https://www.baidu.com/s?wd=pthread_create&from=1012015a&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1dBPHcsmyFBnA7hnjI-nAFh0ZwV5Hcvrjm3rH6sPfKWUMw85HfYnjn4nH6sgvPsT6KdThsqpZwYTjCEQLGCpyw9Uz4Bmy-bIi4WUvYETgN-TLwGUv3EPWcdP1cYnHm3nHDLnHRkrjfY)(&udp_recv_thread, NULL, udp_ts_recv, &udpParam);
```
```java
[pthread_detach](https://www.baidu.com/s?wd=pthread_detach&from=1012015a&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1dBPHcsmyFBnA7hnjI-nAFh0ZwV5Hcvrjm3rH6sPfKWUMw85HfYnjn4nH6sgvPsT6KdThsqpZwYTjCEQLGCpyw9Uz4Bmy-bIi4WUvYETgN-TLwGUv3EPWcdP1cYnHm3nHDLnHRkrjfY)(udp_recv_thread);
```
```java
```
```java
av_register_all();
```
```java
AVCodec *pVideoCodec, *pAudioCodec;
```
```java
AVCodecContext *pVideoCodecCtx = NULL;
```
```java
AVCodecContext *pAudioCodecCtx = NULL;
```
```java
AVIOContext * pb = NULL;
```
```java
AVInputFormat *piFmt = NULL;
```
```java
AVFormatContext *pFmt = NULL;
```
```java
//step1:申请一个AVIOContext
```
```java
pb = avio_alloc_context(buf, BUF_SIZE,
```
```java
0
```
```java
, NULL, read_data, NULL, NULL);
```
```java
if
```
```java
(!pb) {
```
```java
fprintf(stderr,
```
```java
"avio alloc failed!\n"
```
```java
);
```
```java
return
```
```java
-
```
```java
1
```
```java
;
```
```java
}
```
```java
//step2:探测流格式
```
```java
if
```
```java
(av_probe_input_buffer(pb, &piFmt,
```
```java
""
```
```java
, NULL,
```
```java
0
```
```java
,
```
```java
0
```
```java
) <
```
```java
0
```
```java
) {
```
```java
fprintf(stderr,
```
```java
"probe failed!\n"
```
```java
);
```
```java
return
```
```java
-
```
```java
1
```
```java
;
```
```java
}
```
```java
else
```
```java
{
```
```java
fprintf(stdout,
```
```java
"probe success!\n"
```
```java
);
```
```java
fprintf(stdout,
```
```java
"format: %s[%s]\n"
```
```java
, piFmt->name, piFmt->long_name);
```
```java
}
```
```java
pFmt = avformat_alloc_context();
```
```java
pFmt->pb = pb;
```
```java
//step3:这一步很关键
```
```java
//step4:打开流
```
```java
if
```
```java
(avformat_open_input(&pFmt,
```
```java
""
```
```java
, piFmt, NULL) <
```
```java
0
```
```java
) {
```
```java
fprintf(stderr,
```
```java
"avformat open failed.\n"
```
```java
);
```
```java
return
```
```java
-
```
```java
1
```
```java
;
```
```java
}
```
```java
else
```
```java
{
```
```java
fprintf(stdout,
```
```java
"open stream success!\n"
```
```java
);
```
```java
}
```
```java
//以下就和文件处理一致了
```
```java
if
```
```java
(av_find_stream_info(pFmt) <
```
```java
0
```
```java
) {
```
```java
fprintf(stderr,
```
```java
"could not fine stream.\n"
```
```java
);
```
```java
return
```
```java
-
```
```java
1
```
```java
;
```
```java
}
```
```java
av_dump_format(pFmt,
```
```java
0
```
```java
,
```
```java
""
```
```java
,
```
```java
0
```
```java
);
```
```java
int
```
```java
videoindex = -
```
```java
1
```
```java
;
```
```java
int
```
```java
audioindex = -
```
```java
1
```
```java
;
```
```java
for
```
```java
(
```
```java
int
```
```java
i =
```
```java
0
```
```java
; i < pFmt->nb_streams; i++) {
```
```java
if
```
```java
( (pFmt->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) &&
```
```java
(videoindex <
```
```java
0
```
```java
) ) {
```
```java
videoindex = i;
```
```java
}
```
```java
if
```
```java
( (pFmt->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO) &&
```
```java
(audioindex <
```
```java
0
```
```java
) ) {
```
```java
audioindex = i;
```
```java
}
```
```java
}
```
```java
if
```
```java
(videoindex <
```
```java
0
```
```java
|| audioindex <
```
```java
0
```
```java
) {
```
```java
fprintf(stderr,
```
```java
"videoindex=%d, audioindex=%d\n"
```
```java
, videoindex, audioindex);
```
```java
return
```
```java
-
```
```java
1
```
```java
;
```
```java
}
```
```java
AVStream *pVst,*pAst;
```
```java
pVst = pFmt->streams[videoindex];
```
```java
pAst = pFmt->streams[audioindex];
```
```java
pVideoCodecCtx = pVst->codec;
```
```java
pAudioCodecCtx = pAst->codec;
```
```java
pVideoCodec = avcodec_find_decoder(pVideoCodecCtx->codec_id);
```
```java
if
```
```java
(!pVideoCodec) {
```
```java
fprintf(stderr,
```
```java
"could not find video decoder!\n"
```
```java
);
```
```java
return
```
```java
-
```
```java
1
```
```java
;
```
```java
}
```
```java
if
```
```java
(avcodec_open(pVideoCodecCtx, pVideoCodec) <
```
```java
0
```
```java
) {
```
```java
fprintf(stderr,
```
```java
"could not open video codec!\n"
```
```java
);
```
```java
return
```
```java
-
```
```java
1
```
```java
;
```
```java
}
```
```java
pAudioCodec = avcodec_find_decoder(pAudioCodecCtx->codec_id);
```
```java
if
```
```java
(!pAudioCodec) {
```
```java
fprintf(stderr,
```
```java
"could not find audio decoder!\n"
```
```java
);
```
```java
return
```
```java
-
```
```java
1
```
```java
;
```
```java
}
```
```java
if
```
```java
(avcodec_open(pAudioCodecCtx, pAudioCodec) <
```
```java
0
```
```java
) {
```
```java
fprintf(stderr,
```
```java
"could not open audio codec!\n"
```
```java
);
```
```java
return
```
```java
-
```
```java
1
```
```java
;
```
```java
}
```
```java
int
```
```java
got_picture;
```
```java
uint8_t samples[AVCODEC_MAX_AUDIO_FRAME_SIZE*
```
```java
3
```
```java
/
```
```java
2
```
```java
];
```
```java
AVFrame *pframe = avcodec_alloc_frame();
```
```java
AVPacket pkt;
```
```java
av_init_packet(&pkt);
```
```java
while
```
```java
(
```
```java
1
```
```java
) {
```
```java
if
```
```java
(av_read_frame(pFmt, &pkt) >=
```
```java
0
```
```java
) {
```
```java
if
```
```java
(pkt.stream_index == videoindex) {
```
```java
fprintf(stdout,
```
```java
"pkt.size=%d,pkt.pts=%lld, pkt.data=0x%x."
```
```java
, pkt.size, pkt.pts,(unsigned
```
```java
int
```
```java
)pkt.data);
```
```java
avcodec_decode_video2(pVideoCodecCtx, pframe, &got_picture, &pkt);
```
```java
if
```
```java
(got_picture) {
```
```java
fprintf(stdout,
```
```java
"decode one video frame!\n"
```
```java
);
```
```java
}
```
```java
}
```
```java
else
```
```java
if
```
```java
(pkt.stream_index == audioindex) {
```
```java
int
```
```java
frame_size = AVCODEC_MAX_AUDIO_FRAME_SIZE*
```
```java
3
```
```java
/
```
```java
2
```
```java
;
```
```java
if
```
```java
(avcodec_decode_audio3(pAudioCodecCtx, (int16_t *)samples, &frame_size, &pkt) >=
```
```java
0
```
```java
) {
```
```java
fprintf(stdout,
```
```java
"decode one audio frame!\n"
```
```java
);
```
```java
}
```
```java
}
```
```java
av_free_packet(&pkt);
```
```java
}
```
```java
}
```
```java
av_free(buf);
```
```java
av_free(pframe);
```
```java
free_queue(&recvqueue);
```
```java
return
```
```java
0
```
```java
;
```
```java
}
```
