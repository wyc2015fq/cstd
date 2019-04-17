# FFmpeg和SDL教程（三）：播放声音 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年01月12日 18:36:28[boonya](https://me.csdn.net/boonya)阅读数：1389








英文原文地址：[http://dranger.com/ffmpeg/tutorial03.html](http://dranger.com/ffmpeg/tutorial03.html)

## 音频
所以现在我们要播放声音。 SDL也为我们提供了输出声音的方法。 [SDL_OpenAudio（）](http://dranger.com/ffmpeg/functions.html#SDL_OpenAudio)函数用于打开音频设备本身。它将[SDL_AudioSpec](http://dranger.com/ffmpeg/data.html#SDL_AudioSpec)结构作为参数，其中包含有关将要输出的音频的所有信息。


在我们展示如何设置之前，让我们首先解释一下计算机如何处理音频。数字音频由很长的样本组成。每个样本表示音频波形的值。声音以一定的采样率记录，简单地说每个采样播放的速度有多快，并以每秒采样数来衡量。示例采样率分别为每秒22,050和44,100个采样率，分别是无线电和CD的采样率。另外，大多数音频可以有多个通道用于立体声或环绕声，例如，如果采样是立体声，则样本将一次变为2。当我们从电影文件中获取数据时，我们不知道会得到多少样本，但是ffmpeg不会给我们部分样本 - 这也意味着它不会将立体样本分割。


SDL播放音频的方法是：设置音频选项：采样率（在SDL结构中称为频率“freq”），通道数量等等，并设置回调函数和userdata。当我们开始播放音频时，SDL将不断调用这个回调函数，并要求它填充一定数量的字节的音频缓冲区。在将这些信息放入SDL_AudioSpec结构中之后，我们调用SDL_OpenAudio（），这将打开音频设备，并将其返回给另一个AudioSpec结构体。这些是我们将实际使用的规格 - 我们不能保证得到我们所要求的！
## 设置音频

把这一切都留在脑海，因为我们实际上还没有关于音频流的任何信息！让我们回到我们的代码中找到视频流的地方，找到哪个流是音频流。



```cpp
// Find the first video stream
videoStream=-1;
audioStream=-1;
for(i=0; i < pFormatCtx->nb_streams; i++) {
  if(pFormatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_VIDEO
     &&
       videoStream < 0) {
    videoStream=i;
  }
  if(pFormatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_AUDIO &&
     audioStream < 0) {
    audioStream=i;
  }
}
if(videoStream==-1)
  return -1; // Didn't find a video stream
if(audioStream==-1)
  return -1;
```
从这里我们可以从流中获取AVCodecContext所需的所有信息，就像我们对视频流所做的一样：

```cpp
AVCodecContext *aCodecCtxOrig;
AVCodecContext *aCodecCtx;

aCodecCtxOrig=pFormatCtx->streams[audioStream]->codec;
```
如果您还记得以前的教程，我们仍然需要打开音频编解码器本身。 这很简单：


```cpp
AVCodec         *aCodec;

aCodec = avcodec_find_decoder(aCodecCtx->codec_id);
if(!aCodec) {
  fprintf(stderr, "Unsupported codec!\n");
  return -1;
}
// Copy context
aCodecCtx = avcodec_alloc_context3(aCodec);
if(avcodec_copy_context(aCodecCtx, aCodecCtxOrig) != 0) {
  fprintf(stderr, "Couldn't copy codec context");
  return -1; // Error copying codec context
}
/* set up SDL Audio here */

avcodec_open2(aCodecCtx, aCodec, NULL);
```
包含在编解码器上下文中的是我们设置音频所需的所有信息：


```cpp
wanted_spec.freq = aCodecCtx->sample_rate;
wanted_spec.format = AUDIO_S16SYS;
wanted_spec.channels = aCodecCtx->channels;
wanted_spec.silence = 0;
wanted_spec.samples = SDL_AUDIO_BUFFER_SIZE;
wanted_spec.callback = audio_callback;
wanted_spec.userdata = aCodecCtx;

if(SDL_OpenAudio(&wanted_spec, &spec) < 0) {
  fprintf(stderr, "SDL_OpenAudio: %s\n", SDL_GetError());
  return -1;
}
```
我们来看看这些：
- freq：采样率，如前所述。
- 格式：这告诉SDL我们将给它什么格式。 “S16SYS”中的“S”代表“有符号”，16表示每个样本长16位，“SYS”表示顺序取决于您所在的系统。这是avcodec_decode_audio2给我们音频的格式。
- 频道：音频频道的数量。
- 沉默：这是表示沉默的价值。由于音频是有符号的，0当然是通常的值。
- 样本：这是SDL在要求更多音频时给予我们的音频缓冲区的大小。这里的价值在512到8192之间。 ffplay使用1024。
- callback：这是我们传递实际回调函数的地方。我们稍后会详细讨论回调函数。
- userdata：SDL会给我们的回调一个void指针，指向我们想要回调函数的用户数据。我们想让它知道我们的编解码器上下文;你会明白为什么。
最后，我们用[SDL_OpenAudio](http://dranger.com/ffmpeg/functions.html#SDL_OpenAudio)打开音频。

## 队列
那里！ 现在我们准备开始从流中提取音频信息。 但是，我们如何处理这些信息呢？ 我们将不断从电影文件中获取数据包，但同时SDL将调用回调函数！ 解决方案将是创建一种全局结构，我们可以填充音频数据包，所以我们的audio_callback有东西来获取音频数据！ 所以我们要做的是创建一个包的队列。 ffmpeg甚至有一个结构来帮助我们：AVPacketList，它只是一个数据包的链表。 这是我们的队列结构：

```cpp
typedef struct PacketQueue {
  AVPacketList *first_pkt, *last_pkt;
  int nb_packets;
  int size;
  SDL_mutex *mutex;
  SDL_cond *cond;
} PacketQueue;
```
首先，我们应该指出，nb_packets和size是不一样的 - size是指我们从packet-> size获得的字节大小。 你会注意到我们有一个互斥锁和一个condtion变量。 这是因为SDL正在将音频流程作为单独的线程运行。 如果我们不正确地锁定队列，我们可能真的搞砸了我们的数据。 我们将看到如何在队列的执行。 每个程序员都应该知道如何创建一个队列，但是我们包含了这个功能，以便学习SDL的功能。
首先我们做一个函数来初始化队列：


```cpp
void packet_queue_init(PacketQueue *q) {
  memset(q, 0, sizeof(PacketQueue));
  q->mutex = SDL_CreateMutex();
  q->cond = SDL_CreateCond();
}
```
接下来我们会做一个把东西放在队列里的功能：


```cpp
int packet_queue_put(PacketQueue *q, AVPacket *pkt) {

  AVPacketList *pkt1;
  if(av_dup_packet(pkt) < 0) {
    return -1;
  }
  pkt1 = av_malloc(sizeof(AVPacketList));
  if (!pkt1)
    return -1;
  pkt1->pkt = *pkt;
  pkt1->next = NULL;
  
  
  SDL_LockMutex(q->mutex);
  
  if (!q->last_pkt)
    q->first_pkt = pkt1;
  else
    q->last_pkt->next = pkt1;
  q->last_pkt = pkt1;
  q->nb_packets++;
  q->size += pkt1->pkt.size;
  SDL_CondSignal(q->cond);
  
  SDL_UnlockMutex(q->mutex);
  return 0;
}
```
[SDL_LockMutex（）](http://dranger.com/ffmpeg/functions.html#SDL_LockMutex)锁定队列中的互斥锁，所以我们可以添加一些东西，然后[SDL_CondSignal（）](http://dranger.com/ffmpeg/functions.html#SDL_CondSignal)通过我们的条件变量向我们的get函数发送一个信号（如果它在等待的话），告诉它有数据，它可以继续 ，然后解锁互斥体，让它走。
这里是相应的get函数。 注意，如果我们告诉[SDL_CondWait（）](http://dranger.com/ffmpeg/functions.html#SDL_CondWait)，SDL_CondWait（）如何使功能块（即暂停，直到我们得到数据）。

```cpp
int quit = 0;

static int packet_queue_get(PacketQueue *q, AVPacket *pkt, int block) {
  AVPacketList *pkt1;
  int ret;
  
  SDL_LockMutex(q->mutex);
  
  for(;;) {
    
    if(quit) {
      ret = -1;
      break;
    }

    pkt1 = q->first_pkt;
    if (pkt1) {
      q->first_pkt = pkt1->next;
      if (!q->first_pkt)
	q->last_pkt = NULL;
      q->nb_packets--;
      q->size -= pkt1->pkt.size;
      *pkt = pkt1->pkt;
      av_free(pkt1);
      ret = 1;
      break;
    } else if (!block) {
      ret = 0;
      break;
    } else {
      SDL_CondWait(q->cond, q->mutex);
    }
  }
  SDL_UnlockMutex(q->mutex);
  return ret;
}
```
正如你所看到的，我们已经将这个函数封装在一个永久循环中，所以如果我们想阻塞的话，我们一定会得到一些数据。 我们通过使用SDL的SDL_CondWait（）函数来避免循环。 基本上，所有的CondWait都在等待来自SDL_CondSignal（）（或SDL_CondBroadcast（））的信号，然后继续。 然而，看起来好像我们已经把它困在了我们的互斥量中 - 如果我们持有这个锁，我们的put函数就不能把任何东西放到队列中！ 但是，SDL_CondWait（）对我们来说也是解锁我们给它的互斥锁，然后一旦我们得到信号就试图再次锁定它。



## 如遇灾难
您还会注意到我们有一个全局的退出变量，我们检查以确保我们没有将程序设置为退出信号（SDL自动处理TERM信号等）。 否则，线程会一直持续，我们将不得不杀死程序。

```cpp
SDL_PollEvent(&event);
  switch(event.type) {
  case SDL_QUIT:
    quit = 1;
```
我们确保将quit标志设置为1。


## 喂食数据包
唯一剩下的就是建立我们的队列：


```cpp
PacketQueue audioq;
main() {
...
  avcodec_open2(aCodecCtx, aCodec, NULL);

  packet_queue_init(&audioq);
  SDL_PauseAudio(0);
```
[SDL_PauseAudio（）](http://dranger.com/ffmpeg/functions.html#SDL_PauseAudio)最终启动音频设备。 如果没有获取数据，它会播放静音; 它不会马上。
所以，我们已经建立了队列，现在我们已经准备好开始给它提供数据包了。 我们去我们的数据包阅读循环：

```cpp
while(av_read_frame(pFormatCtx, &packet)>=0) {
  // Is this a packet from the video stream?
  if(packet.stream_index==videoStream) {
    // Decode video frame
    ....
    }
  } else if(packet.stream_index==audioStream) {
    packet_queue_put(&audioq, &packet);
  } else {
    av_free_packet(&packet);
  }
```
请注意，我们在放入队列后不会释放数据包。 我们将在解码之后释放它。


## 获取数据包
现在，让我们最终使我们的audio_callback函数获取队列上的数据包。 回调的形式必须是void callback（void * userdata，Uint8 * stream，int len），其中userdata当然是我们给SDL的指针，stream是我们将要写入音频数据的缓冲区，len是 该缓冲区的大小。 代码如下：

```cpp
void audio_callback(void *userdata, Uint8 *stream, int len) {

  AVCodecContext *aCodecCtx = (AVCodecContext *)userdata;
  int len1, audio_size;

  static uint8_t audio_buf[(AVCODEC_MAX_AUDIO_FRAME_SIZE * 3) / 2];
  static unsigned int audio_buf_size = 0;
  static unsigned int audio_buf_index = 0;

  while(len > 0) {
    if(audio_buf_index >= audio_buf_size) {
      /* We have already sent all our data; get more */
      audio_size = audio_decode_frame(aCodecCtx, audio_buf,
                                      sizeof(audio_buf));
      if(audio_size < 0) {
	/* If error, output silence */
	audio_buf_size = 1024;
	memset(audio_buf, 0, audio_buf_size);
      } else {
	audio_buf_size = audio_size;
      }
      audio_buf_index = 0;
    }
    len1 = audio_buf_size - audio_buf_index;
    if(len1 > len)
      len1 = len;
    memcpy(stream, (uint8_t *)audio_buf + audio_buf_index, len1);
    len -= len1;
    stream += len1;
    audio_buf_index += len1;
  }
}
```
这基本上是一个简单的循环，它将从我们将写入的另一个函数（audio_decode_frame（））中提取数据，将结果存储在中间缓冲区中，尝试将len字节写入流中，并在没有足够数据的情况下获取更多数据 或者如果我们有一些遗留下来的话，将它保存下来。 audio_buf的大小是ffmpeg给我们的最大音频帧的1.5倍，这给了我们一个很好的缓冲。

## 最后解码音频
让我们来看解码器的真正意义，audio_decode_frame：

```cpp
int audio_decode_frame(AVCodecContext *aCodecCtx, uint8_t *audio_buf,
                       int buf_size) {

  static AVPacket pkt;
  static uint8_t *audio_pkt_data = NULL;
  static int audio_pkt_size = 0;
  static AVFrame frame;

  int len1, data_size = 0;

  for(;;) {
    while(audio_pkt_size > 0) {
      int got_frame = 0;
      len1 = avcodec_decode_audio4(aCodecCtx, &frame, &got_frame, &pkt);
      if(len1 < 0) {
	/* if error, skip frame */
	audio_pkt_size = 0;
	break;
      }
      audio_pkt_data += len1;
      audio_pkt_size -= len1;
      data_size = 0;
      if(got_frame) {
	data_size = av_samples_get_buffer_size(NULL, 
					       aCodecCtx->channels,
					       frame.nb_samples,
					       aCodecCtx->sample_fmt,
					       1);
	assert(data_size <= buf_size);
	memcpy(audio_buf, frame.data[0], data_size);
      }
      if(data_size <= 0) {
	/* No data yet, get more frames */
	continue;
      }
      /* We have data, return it and come back for more later */
      return data_size;
    }
    if(pkt.data)
      av_free_packet(&pkt);

    if(quit) {
      return -1;
    }

    if(packet_queue_get(&audioq, &pkt, 1) < 0) {
      return -1;
    }
    audio_pkt_data = pkt.data;
    audio_pkt_size = pkt.size;
  }
}
```
整个过程实际上是在函数结束的时候开始的，在这里我们调用了packet_queue_get（）。我们从队列中选择数据包，并保存其信息。然后，一旦我们有一个包处理，我们称之为avcodec_decode_audio4（），其行为很像其姐妹功能，avcodec_decode_video（），除了在这种情况下，一个数据包可能有多个帧，所以你可能不得不打电话它几次从数据包中获取所有的数据。一旦我们有框架，我们简单地将它复制到我们的音频缓冲区，确保data_size比我们的音频缓冲区小。此外，请记住强制转换为audio_buf，因为SDL提供了一个8位整型缓冲区，ffmpeg在16位整型缓冲区中提供了数据。你也应该注意到len1和data_size的区别。 len1是我们使用的数据包数量，data_size是返回的原始数据量。
当我们有了一些数据的时候，我们马上回来看看是否还需要从队列中获得更多的数据，或者如果我们完成了。如果我们仍然有更多的数据包要处理，我们将其保存以备后用。如果我们完成一个数据包，我们终于可以释放这个数据包。

就是这样了！我们已经将音频从主读取循环传送到队列，然后由audio_callback函数读取音频，audio_callback函数将该数据交给SDL，SDL将声音传送到您的声卡。继续编译：

```cpp
gcc -o tutorial03 tutorial03.c -lavutil -lavformat -lavcodec -lswscale -lz -lm \
`sdl-config --cflags --libs`
```
万岁！ 视频仍在尽可能快地播放，但音频正在播放。 为什么是这样？ 这是因为音频信息具有采样率 - 我们正在尽可能快地抽出音频信息，但是音频只是根据采样率从该流中随意播放。
我们几乎准备好开始同步自己的视频和音频，但首先我们需要做一些小程序重组。 排队音频和使用单独的线程播放它的方法工作得非常好：它使代码更易于管理和更模块化。 在开始将视频同步到音频之前，我们需要让代码更容易处理。 下次：[产卵线程](http://dranger.com/ffmpeg/tutorial04.html)！








