# FFmpeg和SDL教程（六）：同步音频 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年01月13日 01:13:04[boonya](https://me.csdn.net/boonya)阅读数：257








英文原文地址：[http://dranger.com/ffmpeg/tutorial06.html](http://dranger.com/ffmpeg/tutorial06.html)


## 同步音频

所以现在我们有一个足够体面的球员来看电影，那么让我们看看我们有什么样的松散结局。最后一次，我们掩盖了一点同步，即将音频同步到视频时钟，而不是相反。我们将以与视频相同的方式来做到这一点：制作一个内部视频时钟，以跟踪视频线程的距离，并将音频同步到该视频线程。稍后我们将介绍如何将音频和视频同步到外部时钟。


## 实现视频时钟

现在我们要实现一个类似于我们上次音频时钟的视频时钟：一个内部值，给出当前正在播放的视频的当前时间偏移量。起初，你会认为这将像用最后一帧的当前PTS更新定时器一样简单。但是，不要忘记，当我们达到毫秒级时，视频帧之间的时间可能相当长。解决方案是跟踪另一个值，即将视频时钟设置为最后一帧的PTS的时间。这样视频时钟的当前值将是PTS_of_last_frame +（current_time  -  time_elapsed_since_PTS_value_was_set）。这个解决方案和我们用get_audio_clock做的非常相似。

所以，在我们的大结构中，我们将放置一个double video_current_pts和一个int64_t video_current_pts_time。时钟更新将在video_refresh_timer函数中进行：



```cpp
void video_refresh_timer(void *userdata) {

  /* ... */

  if(is->video_st) {
    if(is->pictq_size == 0) {
      schedule_refresh(is, 1);
    } else {
      vp = &is->pictq[is->pictq_rindex];

      is->video_current_pts = vp->pts;
      is->video_current_pts_time = av_gettime();
```
不要忘记在stream_component_open中初始化它：


```cpp
is->video_current_pts_time = av_gettime();
```
而现在我们所需要的只是一种获取信息的方式：


```cpp
double get_video_clock(VideoState *is) {
  double delta;

  delta = (av_gettime() - is->video_current_pts_time) / 1000000.0;
  return is->video_current_pts + delta;
}
```

## 抽象的时钟

但为什么强迫自己使用视频时钟？我们不得不去改变我们的视频同步代码，这样音频和视频就不会互相同步。想象一下，如果我们试图使它成为一个命令行选项，就像在ffplay中一样。所以让我们抽象一下：我们要创建一个新的包装函数，get_master_clock用于检查av_sync_type变量，然后调用get_audio_clock，get_video_clock或其他我们想要使用的时钟。我们甚至可以使用电脑时钟，我们将调用get_external_clock：


```cpp
enum {
  AV_SYNC_AUDIO_MASTER,
  AV_SYNC_VIDEO_MASTER,
  AV_SYNC_EXTERNAL_MASTER,
};

#define DEFAULT_AV_SYNC_TYPE AV_SYNC_VIDEO_MASTER

double get_master_clock(VideoState *is) {
  if(is->av_sync_type == AV_SYNC_VIDEO_MASTER) {
    return get_video_clock(is);
  } else if(is->av_sync_type == AV_SYNC_AUDIO_MASTER) {
    return get_audio_clock(is);
  } else {
    return get_external_clock(is);
  }
}
main() {
...
  is->av_sync_type = DEFAULT_AV_SYNC_TYPE;
...
}
```

## 同步音频

现在最困难的部分是：将音频同步到视频时钟。我们的策略是测量音频的位置，将其与视频时钟进行比较，然后找出需要调整的样本数量，也就是说，我们是否需要通过删除样本来加快速度，或者是否需要通过添加他们减慢？
每当我们处理每一组音频样本时，我们都会运行一个synchronize_audio函数来收缩或扩展它们。但是，我们不希望每次都关闭，因为进程音频比视频数据包要频繁得多。所以我们要设置连续调用synchronize_audio函数的最小数量，在我们干扰任何事情之前，必须不同步。当然，就像上次一样，“不同步”意味着音频时钟和视频时钟相差超过我们的同步阈值。

注意：这里发生了什么？这个等式看起来很神奇！那么，它基本上是一个使用几何系列作为权重的加权平均值。我不知道是否有这个名字（我甚至查了维基百科！）但是对于更多的信息，这里是一个解释（或在weightedmean.txt）所以我们要使用一个分数系数，比如c，所以现在假设我们已经得到了N个不同步的音频样本集。我们不同步的数量也可能会有很大差异，所以我们将平均估算每个数据的同步程度。例如，第一个电话可能表明我们已经不同步了40毫秒，接下来是50毫秒，依此类推。但是我们不打算取简单的平均值，因为最近的值比以前的值更重要。所以我们将使用一个分数系数，比如说c，然后像下面这样求和：diff_sum = new_diff + diff_sum * c。当我们准备找到平均差值时，我们只需计算avg_diff = diff_sum *（1-c）。

以下是我们的功能到目前为止：


```cpp
/* Add or subtract samples to get a better sync, return new
   audio buffer size */
int synchronize_audio(VideoState *is, short *samples,
		      int samples_size, double pts) {
  int n;
  double ref_clock;
  
  n = 2 * is->audio_st->codec->channels;
  
  if(is->av_sync_type != AV_SYNC_AUDIO_MASTER) {
    double diff, avg_diff;
    int wanted_size, min_size, max_size, nb_samples;
    
    ref_clock = get_master_clock(is);
    diff = get_audio_clock(is) - ref_clock;

    if(diff < AV_NOSYNC_THRESHOLD) {
      // accumulate the diffs
      is->audio_diff_cum = diff + is->audio_diff_avg_coef
	* is->audio_diff_cum;
      if(is->audio_diff_avg_count < AUDIO_DIFF_AVG_NB) {
	is->audio_diff_avg_count++;
      } else {
	avg_diff = is->audio_diff_cum * (1.0 - is->audio_diff_avg_coef);

       /* Shrinking/expanding buffer code.... */

      }
    } else {
      /* difference is TOO big; reset diff stuff */
      is->audio_diff_avg_count = 0;
      is->audio_diff_cum = 0;
    }
  }
  return samples_size;
}
```
所以我们做得很好，我们大概知道音频来自视频或者我们用于时钟的时间。现在我们来计算需要添加或删除多少个样本，方法是将此代码放在“收缩/展开缓冲区代码”部分：


```cpp
if(fabs(avg_diff) >= is->audio_diff_threshold) {
  wanted_size = samples_size + 
  ((int)(diff * is->audio_st->codec->sample_rate) * n);
  min_size = samples_size * ((100 - SAMPLE_CORRECTION_PERCENT_MAX)
                             / 100);
  max_size = samples_size * ((100 + SAMPLE_CORRECTION_PERCENT_MAX) 
                             / 100);
  if(wanted_size < min_size) {
    wanted_size = min_size;
  } else if (wanted_size > max_size) {
    wanted_size = max_size;
  }
```
请记住，audio_length *（sample_rate *通道数* 2）是audio_length秒音频中的采样数。因此，我们想要的样本数量将是我们已经拥有的样本数量加上或减去对应于音频漂移时间量的样本数量。我们也会对我们的修正大小做一个限制，因为如果我们更改缓冲区太多，对用户来说就太难了。

## 纠正样本数量

现在我们必须实际改正音频。您可能已经注意到，我们的synchronize_audio函数返回一个样本大小，然后告诉我们要发送到流的字节数。所以我们只需要将样本大小调整为wanted_size。这可以使样本尺寸变小。但是如果我们想把它做得更大，我们不能仅仅增加样本的大小，因为缓冲区中没有更多的数据！所以我们必须添加它。但是我们应该添加什么？尝试和推断音频是愚蠢的，所以让我们通过用最后一个样本的值填充缓冲区来使用我们已有的音频。



```cpp
if(wanted_size < samples_size) {
  /* remove samples */
  samples_size = wanted_size;
} else if(wanted_size > samples_size) {
  uint8_t *samples_end, *q;
  int nb;

  /* add samples by copying final samples */
  nb = (samples_size - wanted_size);
  samples_end = (uint8_t *)samples + samples_size - n;
  q = samples_end + n;
  while(nb > 0) {
    memcpy(q, samples_end, n);
    q += n;
    nb -= n;
  }
  samples_size = wanted_size;
}
```
现在我们返回样本大小，我们完成了这个功能。我们现在需要做的就是使用它：


```cpp
void audio_callback(void *userdata, Uint8 *stream, int len) {

  VideoState *is = (VideoState *)userdata;
  int len1, audio_size;
  double pts;

  while(len > 0) {
    if(is->audio_buf_index >= is->audio_buf_size) {
      /* We have already sent all our data; get more */
      audio_size = audio_decode_frame(is, is->audio_buf, sizeof(is->audio_buf), &pts);
      if(audio_size < 0) {
	/* If error, output silence */
	is->audio_buf_size = 1024;
	memset(is->audio_buf, 0, is->audio_buf_size);
      } else {
	audio_size = synchronize_audio(is, (int16_t *)is->audio_buf,
				       audio_size, pts);
	is->audio_buf_size = audio_size;
```
我们所做的只是将调用插入到synchronize_audio。（另外，请确保检查我们初始化上述变量的源代码，我没有打算定义。）

在完成之前最后一件事情：我们需要添加一个if子句，以确保我们不会同步视频，如果它是主时钟：

```cpp
if(is->av_sync_type != AV_SYNC_VIDEO_MASTER) {
  ref_clock = get_master_clock(is);
  diff = vp->pts - ref_clock;

  /* Skip or repeat the frame. Take delay into account
     FFPlay still doesn't "know if this is the best guess." */
  sync_threshold = (delay > AV_SYNC_THRESHOLD) ? delay :
                    AV_SYNC_THRESHOLD;
  if(fabs(diff) < AV_NOSYNC_THRESHOLD) {
    if(diff <= -sync_threshold) {
      delay = 0;
    } else if(diff >= sync_threshold) {
      delay = 2 * delay;
    }
  }
}
```
这就是它！确保你通过源文件检查来初始化任何我没有打扰定义或初始化的变量。然后编译它：


```cpp
gcc -o tutorial06 tutorial06.c -lavutil -lavformat -lavcodec -lswscale -lz -lm \
`sdl-config --cflags --libs`
```
你会很好走。

下一次，我们将这样做，所以你可以倒带和快进你的电影。





