# 【苦逼bug系列】之ffmpeg 学习篇 - 三少GG - CSDN博客
2011年09月19日 15:13:57[三少GG](https://me.csdn.net/scut1135)阅读数：2019
                
1. Partial rewrite stream probe code.
Fix issue2312.
Signed-off-by: Michael Niedermayer <michaelni at gmx.at>
libavformat/utils.c |   25 ++++++++++++++-----------
 1 files changed, 14 insertions(+), 11 deletions(-)
diff --git a/libavformat/utils.c b**/libavformat/utils.c**index be756c5..8b0060f 100644
--- a/libavformat/utils.c
+++ b/libavformat/utils.c
@@ -689,11 +689,7 @@ int av_read_packet(AVFormatContext *s, AVPacket *pkt)
         if (pktl) {
             *pkt = pktl->pkt;
             if(s->streams[pkt->stream_index]->codec->codec_id != CODEC_ID_PROBE ||
-               !s->streams[pkt->stream_index]->probe_packets ||
-               s->raw_packet_buffer_remaining_size < pkt->size){
-                AVProbeData *pd = &s->streams[pkt->stream_index]->probe_data;
-                av_freep(&pd->buf);
-                pd->buf_size = 0;
+               !s->streams[pkt->stream_index]->probe_packets){
                 s->raw_packet_buffer = pktl->next;
                 s->raw_packet_buffer_remaining_size += pkt->size;
                 av_free(pktl);
@@ -731,9 +727,10 @@ int av_read_packet(AVFormatContext *s, AVPacket *pkt)
         add_to_pktbuf(&s->raw_packet_buffer, pkt, &s->raw_packet_buffer_end);
         s->raw_packet_buffer_remaining_size -= pkt->size;
 
-        if(st->codec->codec_id == CODEC_ID_PROBE){
+        if(st->codec->codec_id == CODEC_ID_PROBE && st->probe_packets){
             AVProbeData *pd = &st->probe_data;
-            **av_log(s, AV_LOG_DEBUG, "probing stream %d\n", st->index);**
+            int end;
+            av_log(s, AV_LOG_DEBUG, "probing stream %d pp:%d\n", st->index, st->probe_packets);
             --st->probe_packets;
 
             pd->buf = av_realloc(pd->buf, pd->buf_size+pkt->size+AVPROBE_PADDING_SIZE);
@@ -741,13 +738,19 @@ int av_read_packet(AVFormatContext *s, AVPacket *pkt)
             pd->buf_size += pkt->size;
             memset(pd->buf+pd->buf_size, 0, AVPROBE_PADDING_SIZE);
 
-            if(av_log2(pd->buf_size) != av_log2(pd->buf_size - pkt->size)){
-                //FIXME we dont reduce score to 0 for the case of running out of buffer space in bytes
-                set_codec_from_probe_data(s, st, pd, st->probe_packets > 0 ? AVPROBE_SCORE_MAX/4 : 0);
-                if(st->codec->codec_id != CODEC_ID_PROBE){
+            end=    s->raw_packet_buffer_remaining_size <= 0
+                 || st->probe_packets<=0;
+
+            if(end || av_log2(pd->buf_size) != av_log2(pd->buf_size - pkt->size)){
+                set_codec_from_probe_data(s, st, pd, end ? 0 : AVPROBE_SCORE_MAX/4);
+                if(st->codec->codec_id != CODEC_ID_PROBE || end){
                     pd->buf_size=0;
                     av_freep(&pd->buf);
+                    st->probe_packets= 0;
+                    if(st->codec->codec_id != CODEC_ID_PROBE){
                   **  av_log(s, AV_LOG_DEBUG, "probed stream %d\n", st->index);+                    }else+                        av_log(s, AV_LOG_WARNING, "probed stream %d failed\n", st->index);**
                 }
             }
         }
2.  [**warning: sh_link not set for section `.ARM.exidx'**](http://www.9php.com/FAQ/cxsjl/c/2007/11/5445366106458.html)
sh_link是elf文件section头部中的一个字段。
sh_link This member holds a section header table index link, whose interpretation depends
on the section type. A table below describes the values.
.ARM.exidx是你的动态库中的一个section的名字。
BFD: mylib.so: warning: sh_link not set for section `.ARM.exidx' 错误的原因在gdb源代码中的注释是：
**The Intel C compiler generates SHT_IA_64_UNWIND with SHF_LINK_ORDER.  But it doesn't set the sh_link or sh_info fields.**
没调过动态库。。。
2007-11-26 20:16**雨丝风片**
刚才在linux版那边看到一篇文章：
[http://linux.chinaunix.net/bbs/viewthread.php?tid=857435&highlight=%B6%AF%CC%AC%BF%E2](http://linux.chinaunix.net/bbs/viewthread.php?tid=857435&highlight=%B6%AF%CC%AC%BF%E2)
可以试试。
3. FFMPEG 配置！！！
