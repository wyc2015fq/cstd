# FFmpeg和SDL教程（七）：查找(音视频帧) - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年01月13日 01:24:15[boonya](https://me.csdn.net/boonya)阅读数：185标签：[seeking																[movie](https://so.csdn.net/so/search/s.do?q=movie&t=blog)](https://so.csdn.net/so/search/s.do?q=seeking&t=blog)
个人分类：[C/C++																[音频视频开发](https://blog.csdn.net/boonya/article/category/6325036)](https://blog.csdn.net/boonya/article/category/7395156)







英文原文地址：[http://dranger.com/ffmpeg/tutorial07.html](http://dranger.com/ffmpeg/tutorial07.html)


## 处理寻找命令
现在我们要为我们的播放器添加一些搜索功能，因为当你无法倒带一部电影的时候真的很烦人。另外，这会告诉你av_seek_frame函数是多么容易使用。


我们要让电影中的左右箭头在电影中来回移动，上下箭头很多，“一点”是10秒，“很多”是60秒。所以我们需要设置主循环，以便捕捉按键。但是，当我们获得按键时，我们不能直接调用av_seek_frame。我们必须在我们的主要解码循环decode_thread循环中这样做。所以相反，我们要添加一些值到大的结构中，这些结构将包含寻求新的位置和寻求标志：



```cpp
int             seek_req;
int             seek_flags;
int64_t         seek_pos;
```
现在我们需要设置我们的主循环来捕捉按键：




```cpp
for(;;) {
    double incr, pos;

    SDL_WaitEvent(&event);
    switch(event.type) {
    case SDL_KEYDOWN:
      switch(event.key.keysym.sym) {
      case SDLK_LEFT:
	incr = -10.0;
	goto do_seek;
      case SDLK_RIGHT:
	incr = 10.0;
	goto do_seek;
      case SDLK_UP:
	incr = 60.0;
	goto do_seek;
      case SDLK_DOWN:
	incr = -60.0;
	goto do_seek;
      do_seek:
	if(global_video_state) {
	  pos = get_master_clock(global_video_state);
	  pos += incr;
	  stream_seek(global_video_state, 
                      (int64_t)(pos * AV_TIME_BASE), incr);
	}
	break;
      default:
	break;
      }
      break;
```
要检测按键，我们先看看是否有SDL_KEYDOWN事件。然后我们使用event.key.keysym.sym检查并查看哪个键被击中。一旦我们知道我们想要寻找哪种方法，我们通过增加新的get_master_clock函数的值来计算新的时间。然后我们调用stream_seek函数来设置seek_pos等值。我们将我们的新时间转换为avcodec的内部时间戳单位。回想一下，流中的时间戳是以帧为单位而不是以秒为单位来衡量的，公式为秒=帧*时间基准（fps）。avcodec的默认值为1,000,000 fps（所以2秒的位置将是2000000的时间戳）。我们将看到为什么我们需要稍后再转换这个值。

这是我们的stream_seek函数。注意，如果我们倒退，我们会设置一个标志：




```cpp
void stream_seek(VideoState *is, int64_t pos, int rel) {

  if(!is->seek_req) {
    is->seek_pos = pos;
    is->seek_flags = rel < 0 ? AVSEEK_FLAG_BACKWARD : 0;
    is->seek_req = 1;
  }
}
```
现在让我们继续我们的decode_thread，在那里我们将实际执行我们的寻求。你会注意到在源文件中我们标记了一个区域“寻找东西到这里”。那我们现在就把它放在那里

在[av_seek_frame](http://dranger.com/ffmpeg/functions.html#av_seek_frame)函数周围寻找中心。该函数采用格式上下文，流，时间戳和一组标志作为参数。该功能将寻求你给它的时间戳。时间戳的单位是您传递函数的流的time_base。但是，您不必传递一个流（通过传递值-1来表示）。如果你这样做，time_base将在avcodec的内部时间戳单位，或10​​00000fps。这就是为什么当我们设置seek_pos时，我们乘以AV_TIME_BASE的位置。

然而，如果你传递一个流av_seek_frame -1，有时你可以（很少）遇到一些文件的问题，所以我们将选择文件中的第一个流，并将其传递给av_seek_frame。不要忘记，我们也必须重新调整我们的时间戳，以便在新单位中。




```cpp
if(is->seek_req) {
  int stream_index= -1;
  int64_t seek_target = is->seek_pos;

  if     (is->videoStream >= 0) stream_index = is->videoStream;
  else if(is->audioStream >= 0) stream_index = is->audioStream;

  if(stream_index>=0){
    seek_target= av_rescale_q(seek_target, AV_TIME_BASE_Q,
                      pFormatCtx->streams[stream_index]->time_base);
  }
  if(av_seek_frame(is->pFormatCtx, stream_index, 
                    seek_target, is->seek_flags) < 0) {
    fprintf(stderr, "%s: error while seeking\n",
            is->pFormatCtx->filename);
  } else {
     /* handle packet queues... more later... */
```
[av_rescale_q（a，b，c）](http://dranger.com/ffmpeg/functions.html#av_rescale_q)是一个函数，它可以重新调整从一个基地到另一个基地的时间戳。它基本上计算一个* b / c但是这个函数是必需的，因为这个计算可能会溢出。 AV_TIME_BASE_Q是AV_TIME_BASE的小数版本。他们是完全不同的：AV_TIME_BASE * time_in_seconds = avcodec_timestamp和AV_TIME_BASE_Q * avcodec_timestamp = time_in_seconds（但是请注意，AV_TIME_BASE_Q实际上是一个[AVRational](http://dranger.com/ffmpeg/data.html#AVRational)对象，所以你必须在avcodec中使用特殊的q函数来处理它）。



## 冲洗我们的缓冲区

所以我们正确的设定了方向，但还没有完成。请记住，我们有一个队列设置来累积数据包。现在我们在一个不同的地方，我们必须冲洗那个队列，否则电影不会去寻找！不仅如此，avcodec还有自己的内部缓冲区，每个线程也需要刷新。

要做到这一点，我们需要先写一个函数来清除我们的数据包队列。然后，我们需要有一些方法来指导音频和视频线程，他们需要刷新avcodec的内部缓冲区。我们可以通过在我们刷新队列后在队列中放置一个特殊的数据包来做到这一点，当他们检测到特殊的数据包时，他们只会刷新缓冲区。

我们从flush函数开始吧。这真的很简单，所以我只是告诉你的代码：



```cpp
static void packet_queue_flush(PacketQueue *q) {
  AVPacketList *pkt, *pkt1;

  SDL_LockMutex(q->mutex);
  for(pkt = q->first_pkt; pkt != NULL; pkt = pkt1) {
    pkt1 = pkt->next;
    av_free_packet(&pkt->pkt);
    av_freep(&pkt);
  }
  q->last_pkt = NULL;
  q->first_pkt = NULL;
  q->nb_packets = 0;
  q->size = 0;
  SDL_UnlockMutex(q->mutex);
}
```
现在队列被刷新了，让我们把我们的“清除数据包”。但是首先我们要定义它是什么并创建它：




```cpp
AVPacket flush_pkt;

main() {
  ...
  av_init_packet(&flush_pkt);
  flush_pkt.data = "FLUSH";
  ...
}
```
现在我们把这个包放在队列中：




```cpp
} else {
    if(is->audioStream >= 0) {
      packet_queue_flush(&is->audioq);
      packet_queue_put(&is->audioq, &flush_pkt);
    }
    if(is->videoStream >= 0) {
      packet_queue_flush(&is->videoq);
      packet_queue_put(&is->videoq, &flush_pkt);
    }
  }
  is->seek_req = 0;
}
```
（这个代码片断还继续了decode_thread的上面的代码片段。）我们还需要更改packet_queue_put，以便我们不复制特殊的flush数据包：




```cpp
int packet_queue_put(PacketQueue *q, AVPacket *pkt) {

  AVPacketList *pkt1;
  if(pkt != &flush_pkt && av_dup_packet(pkt) < 0) {
    return -1;
  }
```
然后在音频线程和视频线程中，我们把这个调用在packet_queue_get之后立即发送到[avcodec_flush_buffers](http://dranger.com/ffmpeg/functions.html#avcodec_flush_buffers)：




```cpp
if(packet_queue_get(&is->audioq, pkt, 1) < 0) {
      return -1;
    }
    if(pkt->data == flush_pkt.data) {
      avcodec_flush_buffers(is->audio_st->codec);
      continue;
    }
```
上面的代码片段与视频线程完全相同，“音频”被“视频”替代。

就是这样！我们完成了！继续编译你的播放器：




```cpp
gcc -o tutorial07 tutorial07.c -lavutil -lavformat -lavcodec -lswscale -lz -lm \
`sdl-config --cflags --libs`
```
并享受您的电影播放器在不到1000行的C！

当然，我们可以添加很多东西。









