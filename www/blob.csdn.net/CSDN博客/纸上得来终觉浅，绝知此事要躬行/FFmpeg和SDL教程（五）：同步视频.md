# FFmpeg和SDL教程（五）：同步视频 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年01月13日 00:57:32[boonya](https://me.csdn.net/boonya)阅读数：281








英文原文地址：[http://dranger.com/ffmpeg/tutorial05.html](http://dranger.com/ffmpeg/tutorial05.html)

警告

当我第一次做这个教程时，我的所有同步代码都是从ffplay.c中提取的。今天，这是一个完全不同的程序，ffmpeg库（以及ffplay.c本身）的改进已经导致了一些策略的改变。虽然这段代码仍然有效，但看起来不太好，还有很多本教程可以使用的改进。


## 视频如何同步

所以这一整段时间，我们有一个本质上是无用的电影播放器​​。它播放的视频，是的，它播放音频，是的，但它不是我们所说的电影。那么我们该怎么办？


## PTS和DTS

幸运的是，音频和视频流都具有有关您在其中播放内容的速度和时间的信息。音频流具有采样率，视频流具有每秒帧数。但是，如果我们仅仅通过计数帧和乘以帧速率来同步视频，就有可能与音频不同步。相反，来自流的分组可能具有所谓的解码时间戳（DTS）和呈现时间戳（PTS）。要理解这两个值，你需要知道电影的存储方式。一些格式，如MPEG，使用他们所谓的“B”帧（B代表“双向”）。另外两种帧称为“I”帧和“P”帧（“I”为“内”，“P”为“预测”）。我的帧包含完整的图像。 P帧取决于先前的I帧和P帧，并且像diff或deltas。 B帧与P帧相同，但依赖于在它们之前和之后显示的帧中找到的信息！这就解释了为什么我们在调用[avcodec_decode_video2](http://dranger.com/ffmpeg/functions.html#avcodec_decode_video2)之后可能没有完成的框架。

所以我们假设我们有一个电影，并且帧被显示为：I B B P.现在，我们需要知道P中的信息，然后我们才能显示B帧。正因为如此，这些帧可能会像这样存储：这就是为什么我们在每一帧都有一个解码时间戳和一个表示时间戳。解码时间戳告诉我们什么时候需要解码什么东西，而显示时间戳告诉我们什么时候需要显示一些东西。所以，在这种情况下，我们的流可能看起来像这样：



```cpp
PTS: 1 4 2 3
   DTS: 1 2 3 4
Stream: I P B B
```
一般来说，PTS和DTS只有在我们正在播放的流中有B帧时才会有所不同。

当我们从[av_read_frame（）](http://dranger.com/ffmpeg/functions.html#av_read_frame)得到一个包时，它将包含该包内信息的PTS和DTS值。但是我们真正想要的是我们新解码的原始帧的PTS，所以我们知道何时显示它。

幸运的是，FFMpeg为我们提供了一个“尽力而为”的时间戳，你可以通过[av_frame_get_best_effort_timestamp（）](http://dranger.com/ffmpeg/functions.html#av_frame_get_best_effort_timestamp)

## 同步

现在，虽然知道什么时候我们应该展示一个特定的视频帧，但是我们是如何做到的呢？这是这样的想法：在我们展示一个框架之后，我们计算出什么时候应该显示下一个框架。然后，我们只需设置一个新的超时时间，以在该时间段之后再次刷新视频。正如你所期望的那样，我们根据系统时钟检查下一帧的PTS值，看看我们的超时应该有多长。这种方法是有效的，但有两个问题需要处理。

首先是知道下一个PTS的时间。现在，您可能会认为我们可以将视频速率添加到当前的PTS中 - 而您大部分都是正确的。然而，一些视频呼叫要重复帧。这意味着我们应该重复当前帧一定的次数。这可能会导致程序过早显示下一帧。所以我们需要说明这一点。

第二个问题是，随着节目的到来，视频和音频愉快地蹦蹦跳跳，根本没有同步的麻烦。如果一切正常，我们不用担心。但是你的电脑并不完美，许多视频文件也不是。所以我们有三种选择：将音频同步到视频，将视频同步到音频，或同步到外部时钟（如计算机）。现在，我们将把视频同步到音频。


## 编码：获得帧PTS

现在让我们进入代码来完成这一切。我们将需要添加更多的成员到我们的大结构中，但是我们会根据需要做到这一点。首先让我们看看我们的视频线程。请记住，这是我们拿起解码线程放在队列中的数据包的地方。在这部分代码中我们需要做的是获得由[avcodec_decode_video2](http://dranger.com/ffmpeg/functions.html#avcodec_decode_video2)给予我们的帧的PTS。我们谈到的第一种方式是获取最后一个数据包的DTS，这很简单：



```cpp
double pts;

  for(;;) {
    if(packet_queue_get(&is->videoq, packet, 1) < 0) {
      // means we quit getting packets
      break;
    }
    pts = 0;
    // Decode video frame
    len1 = avcodec_decode_video2(is->video_st->codec,
                                pFrame, &frameFinished, packet);
    if(packet->dts != AV_NOPTS_VALUE) {
      pts = av_frame_get_best_effort_timestamp(pFrame);
    } else {
      pts = 0;
    }
    pts *= av_q2d(is->video_st->time_base);
```
如果我们不知道它是什么，我们将PTS设置为0。

那很简单。技术说明：您可能已经注意到我们正在使用int64作为PTS。这是因为PTS存储为整数。该值是一个时间戳，对应于该流的time_base单位中的时间的度量。例如，如果一个数据流每秒有24帧，那么42的PTS将指示如果我们每1/24秒有一个帧（当然不一定是真的），那么帧应该在第42帧的位置， 。

我们可以通过除以帧率将这个值转换成秒。流的time_base值将是1 / framerate（对于fixed-fps内容），因此要在几秒钟内获得PTS，我们乘以time_base。
编码：同步和使用PTS

所以现在我们已经有了我们的PTS。现在我们必须关注上面讨论的两个同步问题。我们将定义一个名为synchronize_video的函数，将PTS更新为与所有内容同步。这个功能也将最终处理那些我们没有得到PTS值的情况。同时我们需要跟踪下一帧的预计时间，以便我们可以正确设置刷新率。我们可以通过使用一个内部的video_clock值来完成这个工作，该值跟踪视频已经过了多少时间。我们将这个值添加到我们的大结构中。

```cpp
typedef struct VideoState {
  double          video_clock; // pts of last decoded frame / predicted pts of next decoded frame
```
这里是synchronize_video函数，这是不言自明的：

```cpp
double synchronize_video(VideoState *is, AVFrame *src_frame, double pts) {

  double frame_delay;

  if(pts != 0) {
    /* if we have pts, set video clock to it */
    is->video_clock = pts;
  } else {
    /* if we aren't given a pts, set it to the clock */
    pts = is->video_clock;
  }
  /* update the video clock */
  frame_delay = av_q2d(is->video_st->codec->time_base);
  /* if we are repeating a frame, adjust clock accordingly */
  frame_delay += src_frame->repeat_pict * (frame_delay * 0.5);
  is->video_clock += frame_delay;
  return pts;
}
```
你会注意到我们也在这个函数中考虑了重复的帧。

现在让我们得到我们正确的PTS，并使用queue_picture排队，添加一个新的pts参数：


```cpp
// Did we get a video frame?
    if(frameFinished) {
      pts = synchronize_video(is, pFrame, pts);
      if(queue_picture(is, pFrame, pts) < 0) {
	break;
      }
    }
```
关于queue_picture的唯一改变就是我们将这个pts值保存到我们排队的VideoPicture结构中。所以我们必须在结构中添加一个pts变量并添加一行代码：


```cpp
typedef struct VideoPicture {
  ...
  double pts;
}
int queue_picture(VideoState *is, AVFrame *pFrame, double pts) {
  ... stuff ...
  if(vp->bmp) {
    ... convert picture ...
    vp->pts = pts;
    ... alert queue ...
  }
```
所以，现在我们已经把图片排列在我们的图片队列中，并具有适当的PTS值，所以让我们来看看我们的视频刷新功能。你可能还记得上次我们只是伪造了一个80ms的刷新。那么，现在我们要找出如何真正弄清楚。

我们的策略是预测下一个PTS的时间，方法是简单地测量前一个PTS和这个PTS之间的时间。同时，我们需要将视频同步到音频。我们将制作一个音频时钟：一个内部值，用于跟踪我们正在播放的音频的位置。这就像任何MP3播放器上的数字读出。由于我们将视频同步到音频，因此视频线程使用此值来确定它是太遥远还是太遥远。

稍后我们会进行实施。现在让我们假设我们有一个get_audio_clock函数，它会给我们音频时钟上的时间。一旦我们有了这个价值，但是，如果视频和音频不同步，我们该怎么办？简单地尝试通过寻找或跳跃到正确的数据包是愚蠢的。相反，我们只是调整我们为下一次刷新计算的值：如果PTS离音频时间太远，我们将计算的延迟加倍。如果PTS在音频时间之前太远，我们只需尽快刷新。现在我们调整了刷新时间，或者延迟了，我们将通过保持一个正在运行的frame_timer与计算机的时钟进行比较。这个帧计时器将总结我们所有计算的延迟，同时播放电影。换句话说，这个frame_timer是什么时候应该是我们显示下一帧。我们只需将新的延迟添加到帧定时器，将其与计算机时钟的时间进行比较，然后使用该值安排下一次刷新。这可能有点令人困惑，所以仔细研究代码：


```cpp
void video_refresh_timer(void *userdata) {

  VideoState *is = (VideoState *)userdata;
  VideoPicture *vp;
  double actual_delay, delay, sync_threshold, ref_clock, diff;
  
  if(is->video_st) {
    if(is->pictq_size == 0) {
      schedule_refresh(is, 1);
    } else {
      vp = &is->pictq[is->pictq_rindex];

      delay = vp->pts - is->frame_last_pts; /* the pts from last time */
      if(delay <= 0 || delay >= 1.0) {
	/* if incorrect delay, use previous one */
	delay = is->frame_last_delay;
      }
      /* save for next time */
      is->frame_last_delay = delay;
      is->frame_last_pts = vp->pts;

      /* update delay to sync to audio */
      ref_clock = get_audio_clock(is);
      diff = vp->pts - ref_clock;

      /* Skip or repeat the frame. Take delay into account
	 FFPlay still doesn't "know if this is the best guess." */
      sync_threshold = (delay > AV_SYNC_THRESHOLD) ? delay : AV_SYNC_THRESHOLD;
      if(fabs(diff) < AV_NOSYNC_THRESHOLD) {
	if(diff <= -sync_threshold) {
	  delay = 0;
	} else if(diff >= sync_threshold) {
	  delay = 2 * delay;
	}
      }
      is->frame_timer += delay;
      /* computer the REAL delay */
      actual_delay = is->frame_timer - (av_gettime() / 1000000.0);
      if(actual_delay < 0.010) {
	/* Really it should skip the picture instead */
	actual_delay = 0.010;
      }
      schedule_refresh(is, (int)(actual_delay * 1000 + 0.5));
      /* show the picture! */
      video_display(is);
      
      /* update queue for next picture! */
      if(++is->pictq_rindex == VIDEO_PICTURE_QUEUE_SIZE) {
	is->pictq_rindex = 0;
      }
      SDL_LockMutex(is->pictq_mutex);
      is->pictq_size--;
      SDL_CondSignal(is->pictq_cond);
      SDL_UnlockMutex(is->pictq_mutex);
    }
  } else {
    schedule_refresh(is, 100);
  }
}
```
我们做了一些检查：首先，确保临时秘书处和以前的临时秘书处之间的延迟是有意义的。如果不是，我们只是猜测并使用最后的延迟。接下来，我们确保我们有一个同步阈值，因为事情永远不会完全同步。ffplay的值为0.01。我们也确保同步阈值永远不会小于PTS值之间的差距。最后，我们将最小刷新值设置为10毫秒*。*实际上，我们应该跳过帧，但是我们不打算去打扰。

我们在大结构中添加了一堆变量，所以不要忘记检查代码。另外，不要忘记在stream_component_open中初始化帧定时器和初始的前一帧延迟：


```cpp
is->frame_timer = (double)av_gettime() / 1000000.0;
    is->frame_last_delay = 40e-3;
```

## 同步：音频时钟

现在是我们实施音频时钟的时候了。我们可以更新audio_decode_frame函数中的时钟时间，这是我们对音频进行解码的地方。现在请记住，我们并不总是每次调用这个函数都要处理一个新的包，所以有两个地方需要更新时钟。第一个地方是我们得到新数据包的地方：我们只需将音频时钟设置为数据包的PTS。然后，如果一个数据包有多个帧，我们通过计数样本数量并将它们乘以给定的每秒样本速率来保持音频播放的时间。所以一旦我们有了这个包：


```cpp
/* if update, update the audio clock w/pts */
    if(pkt->pts != AV_NOPTS_VALUE) {
      is->audio_clock = av_q2d(is->audio_st->time_base)*pkt->pts;
    }
```
一旦我们正在处理数据包：


```cpp
/* Keep audio_clock up-to-date */
      pts = is->audio_clock;
      *pts_ptr = pts;
      n = 2 * is->audio_st->codec->channels;
      is->audio_clock += (double)data_size /
	(double)(n * is->audio_st->codec->sample_rate);
```
一些细节：函数的模板已经改为包含pts_ptr，所以请确保你改变了。pts_ptr是我们用来通知audio_callback音频数据包的点的指针。这将在下次用于同步音频和视频。

现在我们可以最终实现我们的get_audio_clock函数。想到获取is-> audio_clock的价值并不那么简单。请注意，我们在每次处理音频PTS时都会设置音频PTS，但是如果您查看audio_callback函数，则需要一段时间才能将所有数据从音频数据包移动到输出缓冲区。这意味着我们音频时钟的价值可能会遥遥领先。所以我们必须检查我们还剩下多少。以下是完整的代码：


```cpp
double get_audio_clock(VideoState *is) {
  double pts;
  int hw_buf_size, bytes_per_sec, n;
  
  pts = is->audio_clock; /* maintained in the audio thread */
  hw_buf_size = is->audio_buf_size - is->audio_buf_index;
  bytes_per_sec = 0;
  n = is->audio_st->codec->channels * 2;
  if(is->audio_st) {
    bytes_per_sec = is->audio_st->codec->sample_rate * n;
  }
  if(bytes_per_sec) {
    pts -= (double)hw_buf_size / bytes_per_sec;
  }
  return pts;
}
```
你应该能够知道为什么这个功能现在工作;）

就是这样了！继续编译它：

```cpp
gcc -o tutorial05 tutorial05.c -lavutil -lavformat -lavcodec -lswscale -lz -lm \
`sdl-config --cflags --libs`
```
终于！您可以在自己的电影播放器上观看电影。下一次，我们将看音频同步，然后我们将讨论寻找教程。





