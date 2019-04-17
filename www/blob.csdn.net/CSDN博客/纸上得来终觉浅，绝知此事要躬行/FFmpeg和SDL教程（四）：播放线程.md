# FFmpeg和SDL教程（四）：播放线程 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年01月13日 00:32:29[boonya](https://me.csdn.net/boonya)阅读数：245标签：[threads](https://so.csdn.net/so/search/s.do?q=threads&t=blog)
个人分类：[C/C++																[音频视频开发](https://blog.csdn.net/boonya/article/category/6325036)](https://blog.csdn.net/boonya/article/category/7395156)








英文原文地址：[http://dranger.com/ffmpeg/tutorial04.html](http://dranger.com/ffmpeg/tutorial04.html)


## 概览



上次我们利用SDL的音频功能添加了音频支持。 SDL启动了一个线程，可以对每次需要音频时定义的函数进行回调。现在我们要用视频显示来做同样的事情。这使代码更加模块化，更容易处理 - 特别是当我们想要添加同步时。那么我们从哪里开始？



首先我们注意到，我们的主要功能是处理一个非常多的事情：它通过事件循环运行，读取数据包和解码视频。所以我们要做的就是将所有这些分开：我们将有一个负责解码数据包的线程;这些数据包将被添加到队列中，并由相应的音频和视频线程读取。我们已经建立了音频线程我们想要的方式;视频线程会更复杂一些，因为我们必须自己显示视频。我们将把实际的显示代码添加到主循环中。但是，我们不是每次循环显示视频，而是将视频显示集成到事件循环中。这个想法是解码视频，将结果帧保存在另一个队列，然后创建一个自定义事件（FF_REFRESH_EVENT），我们添加到事件系统，然后当我们的事件循环看到这个事件，它会显示队列中的下一帧。下面是一个方便的ASCII艺术插图：



```cpp
________ audio  _______      _____
|        | pkts |       |    |     | to spkr
| DECODE |----->| AUDIO |--->| SDL |-->
|________|      |_______|    |_____|
    |  video     _______
    |   pkts    |       |
    +---------->| VIDEO |
 ________       |_______|   _______
|       |          |       |       |
| EVENT |          +------>| VIDEO | to mon.
| LOOP  |----------------->| DISP. |-->
|_______|<---FF_REFRESH----|_______|
```
通过事件循环移动控制视频显示的主要目的是使用SDL_Delay线程，我们可以精确控制下一个视频帧在屏幕上显示的时间。当我们在下一个教程中最终同步视频时，添加将调度下一个视频刷新的代码将是一件简单的事情，以便在正确的时间在屏幕上显示正确的图片。

## 简化代码

我们也打算清理一下这个代码。我们拥有所有这些音频和视频编解码器信息，我们将添加队列和缓冲区，谁知道还有什么。所有这些东西都是为了一个逻辑单位，即电影。所以我们要制作一个大的结构体来保存所有这些称为VideoState的信息。


```cpp
typedef struct VideoState {

  AVFormatContext *pFormatCtx;
  int             videoStream, audioStream;
  AVStream        *audio_st;
  AVCodecContext  *audio_ctx;
  PacketQueue     audioq;
  uint8_t         audio_buf[(AVCODEC_MAX_AUDIO_FRAME_SIZE * 3) / 2];
  unsigned int    audio_buf_size;
  unsigned int    audio_buf_index;
  AVPacket        audio_pkt;
  uint8_t         *audio_pkt_data;
  int             audio_pkt_size;
  AVStream        *video_st;
  AVCodecContext  *video_ctx;
  PacketQueue     videoq;

  VideoPicture    pictq[VIDEO_PICTURE_QUEUE_SIZE];
  int             pictq_size, pictq_rindex, pictq_windex;
  SDL_mutex       *pictq_mutex;
  SDL_cond        *pictq_cond;
  
  SDL_Thread      *parse_tid;
  SDL_Thread      *video_tid;

  char            filename[1024];
  int             quit;
} VideoState;
```
在这里，我们看到了我们将要到达的一瞥。首先我们看到基本信息 - 音频和视频流的格式上下文和索引，以及相应的AVStream对象。然后我们可以看到，我们已经将这些音频缓冲区中的一部分移入了这个结构。这些（audio_buf，audio_buf_size等）全部是关于音频的信息，这些信息还处于（或缺乏）状态。我们为视频添加了另一个队列，并为解码的帧添加了一个缓冲区（将用作队列;我们不需要任何奇怪的队列）。VideoPicture结构是我们自己创作的（我们将会看到它的内容）。我们还注意到我们已经为我们将要创建的两个额外的线程以及电影的退出标志和文件名分配了指针。

所以现在我们回到主要的功能，看看这是如何改变我们的计划。让我们建立我们的VideoState结构：


```cpp
int main(int argc, char *argv[]) {

  SDL_Event       event;

  VideoState      *is;

  is = av_mallocz(sizeof(VideoState));
```
av_mallocz（）是一个很好的函数，它会为我们分配内存并将其清零。

然后，我们将初始化显示缓冲区（pictq）的锁定，因为由于事件循环调用了我们的显示功能，请记住，显示功能将从pictq中提取预解码的帧。同时，我们的视频解码器将把信息放入其中 - 我们不知道谁先到达那里。希望你认识到这是一个经典的竞赛条件。所以我们现在在开始任何线程之前分配它。让我们也复制我们的电影的文件名到我们的VideoState。


```cpp
av_strlcpy(is->filename, argv[1], sizeof(is->filename));

is->pictq_mutex = SDL_CreateMutex();
is->pictq_cond = SDL_CreateCond();
```
av_strlcpy是ffmpeg中的一个函数，它在strncpy之外执行一些额外的边界检查。
## 我们的第一个线程

现在让我们终于启动我们的线程，并完成真正的工作：



```cpp
schedule_refresh(is, 40);

is->parse_tid = SDL_CreateThread(decode_thread, is);
if(!is->parse_tid) {
  av_free(is);
  return -1;
}
```
schedule_refresh是我们稍后定义的一个函数。它的基本功能是告诉系统在指定的毫秒数之后推送FF_REFRESH_EVENT。当我们在事件队列中看到它时，这又会调用视频刷新功能。但是现在，我们来看看[SDL_CreateThread（）](http://dranger.com/ffmpeg/functions.html#SDL_CreateThread)。

SDL_CreateThread（）就是这样做的 - 它产生了一个新的线程，它可以完全访问原始进程的所有内存，并启动运行在我们提供的函数上的线程。它也将传递该函数用户定义的数据。在这种情况下，我们调用了decode_thread（），并附加了我们的VideoState结构。功能的前半部分没有新的东西;它只是做打开文件和找到音频和视频流的索引的工作。我们唯一不同的是将格式上下文保存在我们的大结构中。在找到我们的流索引之后，我们调用另一个我们将要定义的函数stream_component_open（）。这是一个非常自然的方式来分解事情，因为我们做了很多类似的东西来设置视频和音频编解码器，我们通过使这个函数重用一些代码。

stream_component_open（）函数是我们可以找到我们的编解码器，设置我们的音频选项，保存重要信息到我们的大结构，并启动我们的音频和视频线程。这也是我们插入其他选项的地方，比如强制编解码器而不是自动检测等等。这里是：

```cpp
int stream_component_open(VideoState *is, int stream_index) {

  AVFormatContext *pFormatCtx = is->pFormatCtx;
  AVCodecContext *codecCtx;
  AVCodec *codec;
  SDL_AudioSpec wanted_spec, spec;

  if(stream_index < 0 || stream_index >= pFormatCtx->nb_streams) {
    return -1;
  }

  codec = avcodec_find_decoder(pFormatCtx->streams[stream_index]->codec->codec_id);
  if(!codec) {
    fprintf(stderr, "Unsupported codec!\n");
    return -1;
  }

  codecCtx = avcodec_alloc_context3(codec);
  if(avcodec_copy_context(codecCtx, pFormatCtx->streams[stream_index]->codec) != 0) {
    fprintf(stderr, "Couldn't copy codec context");
    return -1; // Error copying codec context
  }


  if(codecCtx->codec_type == AVMEDIA_TYPE_AUDIO) {
    // Set audio settings from codec info
    wanted_spec.freq = codecCtx->sample_rate;
    /* ...etc... */
    wanted_spec.callback = audio_callback;
    wanted_spec.userdata = is;
    
    if(SDL_OpenAudio(&wanted_spec, &spec) < 0) {
      fprintf(stderr, "SDL_OpenAudio: %s\n", SDL_GetError());
      return -1;
    }
  }
  if(avcodec_open2(codecCtx, codec, NULL) < 0) {
    fprintf(stderr, "Unsupported codec!\n");
    return -1;
  }

  switch(codecCtx->codec_type) {
  case AVMEDIA_TYPE_AUDIO:
    is->audioStream = stream_index;
    is->audio_st = pFormatCtx->streams[stream_index];
    is->audio_ctx = codecCtx;
    is->audio_buf_size = 0;
    is->audio_buf_index = 0;
    memset(&is->audio_pkt, 0, sizeof(is->audio_pkt));
    packet_queue_init(&is->audioq);
    SDL_PauseAudio(0);
    break;
  case AVMEDIA_TYPE_VIDEO:
    is->videoStream = stream_index;
    is->video_st = pFormatCtx->streams[stream_index];
    is->video_ctx = codecCtx;
    
    packet_queue_init(&is->videoq);
    is->video_tid = SDL_CreateThread(video_thread, is);
    is->sws_ctx = sws_getContext(is->video_st->codec->width, is->video_st->codec->height,
				 is->video_st->codec->pix_fmt, is->video_st->codec->width,
				 is->video_st->codec->height, PIX_FMT_YUV420P,
				 SWS_BILINEAR, NULL, NULL, NULL
				 );
    break;
  default:
    break;
  }
}
```
这跟我们以前的代码几乎一样，除了现在已经广泛用于音频和视频。请注意，我们不是使用aCodecCtx，而是将我们的大结构设置为音频回调的用户数据。我们还将这些流本身保存为audio_st和video_st。我们也添加了我们的视频队列，并以与我们设置音频队列相同的方式进行设置。大部分的重点是启动视频和音频线程。这些位做到这一点：


```cpp
SDL_PauseAudio(0);
    break;

/* ...... */

    is->video_tid = SDL_CreateThread(video_thread, is);
```
我们记得上一次的[SDL_PauseAudio（）](http://dranger.com/ffmpeg/functions.html#SDL_PauseAudio)，并且[SDL_CreateThread（）](http://dranger.com/ffmpeg/functions.html#SDL_CreateThread)的使用方式与以前完全一样。我们将回到我们的video_thread（）函数。

在此之前，我们回到decode_thread（）函数的后半部分。它基本上只是一个for循环，将读取一个数据包，并将其放在正确的队列：


```cpp
for(;;) {
    if(is->quit) {
      break;
    }
    // seek stuff goes here
    if(is->audioq.size > MAX_AUDIOQ_SIZE ||
       is->videoq.size > MAX_VIDEOQ_SIZE) {
      SDL_Delay(10);
      continue;
    }
    if(av_read_frame(is->pFormatCtx, packet) < 0) {
      if((is->pFormatCtx->pb->error) == 0) {
	SDL_Delay(100); /* no error; wait for user input */
	continue;
      } else {
	break;
      }
    }
    // Is this a packet from the video stream?
    if(packet->stream_index == is->videoStream) {
      packet_queue_put(&is->videoq, packet);
    } else if(packet->stream_index == is->audioStream) {
      packet_queue_put(&is->audioq, packet);
    } else {
      av_free_packet(packet);
    }
  }
```
这里没有什么新东西，除了现在我们的音频和视频队列拥有最大尺寸，并且我们已经添加了对读取错误的检查。格式上下文中有一个名为pb的ByteIOContext结构体。ByteIOContext是基本上保留所有低级文件信息的结构。

在我们的for循环之后，我们有所有代码来等待程序的其余部分结束，或者通知它我们已经结束了。这段代码是有启发性的，因为它向我们展示了我们如何推动事件 - 这是我们稍后必须展示视频的事情。


```cpp
while(!is->quit) {
    SDL_Delay(100);
  }

 fail:
  if(1){
    SDL_Event event;
    event.type = FF_QUIT_EVENT;
    event.user.data1 = is;
    SDL_PushEvent(&event);
  }
  return 0;
```
我们通过使用SDL常量SDL_USEREVENT来获取用户事件的值。应为第一个用户事件分配值SDL_USEREVENT，下一个SDL_USEREVENT + 1，依此类推。FF_QUIT_EVENT在我们的程序中定义为SDL_USEREVENT + 1.如果我们也喜欢，我们也可以传递用户数据，在这里我们将指针传递给大结构。最后我们调用[SDL_PushEvent（）](http://dranger.com/ffmpeg/functions.html#SDL_PushEvent)。在我们的事件循环切换中，我们只是把它放在之前的SDL_QUIT_EVENT部分。我们将更详细地看到我们的事件循环。现在只要放心，当我们推FF_QUIT_EVENT的时候，我们稍后会抓到它，并提高我们的退出标志。

## 获取框架：video_thread

编好我们的编解码器后，我们开始我们的视频线程。该线程从视频队列中读入数据包，将视频解码为帧，然后调用queue_picture函数将处理后的帧放到图片队列中：



```cpp
int video_thread(void *arg) {
  VideoState *is = (VideoState *)arg;
  AVPacket pkt1, *packet = &pkt1;
  int frameFinished;
  AVFrame *pFrame;

  pFrame = av_frame_alloc();

  for(;;) {
    if(packet_queue_get(&is->videoq, packet, 1) < 0) {
      // means we quit getting packets
      break;
    }
    // Decode video frame
    avcodec_decode_video2(is->video_st->codec, pFrame, &frameFinished, packet);

    // Did we get a video frame?
    if(frameFinished) {
      if(queue_picture(is, pFrame) < 0) {
	break;
      }
    }
    av_free_packet(packet);
  }
  av_free(pFrame);
  return 0;
}
```
这个功能大部分应该熟悉这一点。我们已经移动了我们的avcodec_decode_video2函数，只是替换了一些参数。例如，我们有[AVStream](http://dranger.com/ffmpeg/data.html#AVStream)存储在我们的大结构，所以我们从那里得到我们的编解码器。我们只是不停地从视频队列中获取数据包，直到有人告诉我们退出或遇到错误。

## 排队帧

让我们看看我们的解码帧，pFrame存储在我们的图片队列中的函数。由于我们的图片队列是一个SDL覆盖（大概是为了让视频显示功能尽可能少的计算），我们需要把我们的帧转换成。我们存储在图片队列中的数据是我们制作的一个结构：


```cpp
typedef struct VideoPicture {
  SDL_Overlay *bmp;
  int width, height; /* source height & width */
  int allocated;
} VideoPicture;
```
我们的大结构有一个这样的缓冲区，我们可以存储它们。但是，我们需要自己分配[SDL_Overlay](http://dranger.com/ffmpeg/data.html#SDL_Overlay)（注意分配的标志，表示我们是否已经这样做）。

要使用这个队列，我们有两个指针 - 写作索引和阅读索引。我们也跟踪缓冲区中有多少实际的图片。要写入队列，我们将首先等待我们的缓冲区清除，所以我们有空间来存储我们的VideoPicture。然后我们检查一下，看看我们是否已经在我们的写作索引上分配了叠加层。如果不是，我们将不得不分配一些空间。如果窗口大小已经改变，我们也必须重新分配缓冲区！


```cpp
int queue_picture(VideoState *is, AVFrame *pFrame) {

  VideoPicture *vp;
  int dst_pix_fmt;
  AVPicture pict;

  /* wait until we have space for a new pic */
  SDL_LockMutex(is->pictq_mutex);
  while(is->pictq_size >= VIDEO_PICTURE_QUEUE_SIZE &&
	!is->quit) {
    SDL_CondWait(is->pictq_cond, is->pictq_mutex);
  }
  SDL_UnlockMutex(is->pictq_mutex);

  if(is->quit)
    return -1;

  // windex is set to 0 initially
  vp = &is->pictq[is->pictq_windex];

  /* allocate or resize the buffer! */
  if(!vp->bmp ||
     vp->width != is->video_st->codec->width ||
     vp->height != is->video_st->codec->height) {
    SDL_Event event;

    vp->allocated = 0;
    alloc_picture(is);
    if(is->quit) {
      return -1;
    }
  }
```
让我们看一下alloc_picture（）函数：


```cpp
void alloc_picture(void *userdata) {

  VideoState *is = (VideoState *)userdata;
  VideoPicture *vp;

  vp = &is->pictq[is->pictq_windex];
  if(vp->bmp) {
    // we already have one make another, bigger/smaller
    SDL_FreeYUVOverlay(vp->bmp);
  }
  // Allocate a place to put our YUV image on that screen
  SDL_LockMutex(screen_mutex);
  vp->bmp = SDL_CreateYUVOverlay(is->video_st->codec->width,
				 is->video_st->codec->height,
				 SDL_YV12_OVERLAY,
				 screen);
  SDL_UnlockMutex(screen_mutex);
  vp->width = is->video_st->codec->width;
  vp->height = is->video_st->codec->height;  
  vp->allocated = 1;
}
```
您应该认识到我们已经从主循环移到本节的[SDL_CreateYUVOverlay](http://dranger.com/ffmpeg/functions.html#SDL_CreateYUVOverlay)函数。这个代码现在应该是相当自我解释的。但是，现在我们有一个互斥锁，因为两个线程不能同时向屏幕写信息！这将阻止我们的alloc_picture函数踩在将显示图片的函数的脚趾上。（我们已经创建了这个锁作为一个全局变量，并在main（）中初始化它;请参阅代码。）请记住，我们在VideoPicture结构中保存宽度和高度，因为我们需要确保我们的视频大小不会改变一些原因。

好的，我们都解决了，我们有我们的YUV覆盖分配，并准备接收图片。让我们回到queue_picture，看看代码将框架复制到覆盖。你应该认识到它的一部分：


```cpp
int queue_picture(VideoState *is, AVFrame *pFrame) {

  /* Allocate a frame if we need it... */
  /* ... */
  /* We have a place to put our picture on the queue */

  if(vp->bmp) {

    SDL_LockYUVOverlay(vp->bmp);
    
    dst_pix_fmt = PIX_FMT_YUV420P;
    /* point pict at the queue */

    pict.data[0] = vp->bmp->pixels[0];
    pict.data[1] = vp->bmp->pixels[2];
    pict.data[2] = vp->bmp->pixels[1];
    
    pict.linesize[0] = vp->bmp->pitches[0];
    pict.linesize[1] = vp->bmp->pitches[2];
    pict.linesize[2] = vp->bmp->pitches[1];
    
    // Convert the image into YUV format that SDL uses
    sws_scale(is->sws_ctx, (uint8_t const * const *)pFrame->data,
	      pFrame->linesize, 0, is->video_st->codec->height,
	      pict.data, pict.linesize);
    
    SDL_UnlockYUVOverlay(vp->bmp);
    /* now we inform our display thread that we have a pic ready */
    if(++is->pictq_windex == VIDEO_PICTURE_QUEUE_SIZE) {
      is->pictq_windex = 0;
    }
    SDL_LockMutex(is->pictq_mutex);
    is->pictq_size++;
    SDL_UnlockMutex(is->pictq_mutex);
  }
  return 0;
}
```
这部分的大部分只是我们之前用来填充YUV覆盖图的代码。最后一位是将我们的值“添加”到队列中。队列的工作方式是添加到队列中，直到队列满满为止。因此，一切都取决于is-> pictq_size的值，要求我们锁定它。所以我们在这里做的是增加写指针（如果需要的话翻转），然后锁定队列并增加它的大小。现在我们的读者会知道队列中有更多的信息，如果这使得我们的队列满了，我们的作者将会知道。

## 显示视频

这就是我们的视频线程！现在我们把所有的松散线程都包括进去了 - 记得我们调用了schedule_refresh（）函数吗？我们来看看实际做了什么：



```cpp
/* schedule a video refresh in 'delay' ms */
static void schedule_refresh(VideoState *is, int delay) {
  SDL_AddTimer(delay, sdl_refresh_timer_cb, is);
}
```
[SDL_AddTimer（）](http://dranger.com/ffmpeg/functions.html#SDL_AddTimer)是一个SDL函数，它在一定的毫秒数后（可选地携带一些用户数据）简单地回调用户指定的函数。我们将使用这个函数来调度视频更新 - 每次我们调用这个函数，它都会设置定时器，这将触发一个事件，这将使我们的main（）函数调用一个函数，我们的图片队列，并显示它！唷！

但首先是第一件事。让我们触发这个事件。这使我们转到：

```cpp
static Uint32 sdl_refresh_timer_cb(Uint32 interval, void *opaque) {
  SDL_Event event;
  event.type = FF_REFRESH_EVENT;
  event.user.data1 = opaque;
  SDL_PushEvent(&event);
  return 0; /* 0 means stop timer */
}
```
这是现在熟悉的事件推动。FF_REFRESH_EVENT在这里被定义为SDL_USEREVENT + 1.有一点需要注意的是，当我们返回0时，SDL停止定时器，所以不再进行回调。

现在我们推了一个FF_REFRESH_EVENT，我们需要在事件循环中处理它：


```cpp
for(;;) {

  SDL_WaitEvent(&event);
  switch(event.type) {
  /* ... */
  case FF_REFRESH_EVENT:
    video_refresh_timer(event.user.data1);
    break;
```
并发送给我们这个函数，它实际上会从我们的图片队列中提取数据：


```cpp
void video_refresh_timer(void *userdata) {

  VideoState *is = (VideoState *)userdata;
  VideoPicture *vp;
  
  if(is->video_st) {
    if(is->pictq_size == 0) {
      schedule_refresh(is, 1);
    } else {
      vp = &is->pictq[is->pictq_rindex];
      /* Timing code goes here */

      schedule_refresh(is, 80);
      
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
现在，这是一个非常简单的功能：当我们有东西的时候它从队列中拉出来，为下一个视频帧的显示设置我们的定时器，调用video_display在屏幕上实际显示视频，然后增加队列，并减小其大小。你可能会注意到我们在这个函数中并没有对vp做任何事情，这就是为什么：我们会这样做。后来。当我们开始将视频同步到音频时，我们将使用它来访问时间信息。看看它在哪里说“时间码在这里”？在这一节中，我们要弄清楚我们应该多快显示下一个视频帧，然后将该值输入到schedule_refresh（）函数中。现在我们只是把一个虚拟值设置为80.从技术上讲，你可以猜测和检查这个值，并为你观看的每一部电影重新编译它，但是1）它会在一段时间后漂移，2）它很愚蠢。不过，我们稍后再回来。

我们差不多完成了我们只需要做最后一件事：显示视频！这是video_display功能：


```cpp
void video_display(VideoState *is) {

  SDL_Rect rect;
  VideoPicture *vp;
  float aspect_ratio;
  int w, h, x, y;
  int i;

  vp = &is->pictq[is->pictq_rindex];
  if(vp->bmp) {
    if(is->video_st->codec->sample_aspect_ratio.num == 0) {
      aspect_ratio = 0;
    } else {
      aspect_ratio = av_q2d(is->video_st->codec->sample_aspect_ratio) *
	is->video_st->codec->width / is->video_st->codec->height;
    }
    if(aspect_ratio <= 0.0) {
      aspect_ratio = (float)is->video_st->codec->width /
	(float)is->video_st->codec->height;
    }
    h = screen->h;
    w = ((int)rint(h * aspect_ratio)) & -3;
    if(w > screen->w) {
      w = screen->w;
      h = ((int)rint(w / aspect_ratio)) & -3;
    }
    x = (screen->w - w) / 2;
    y = (screen->h - h) / 2;
    
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;
    SDL_LockMutex(screen_mutex);
    SDL_DisplayYUVOverlay(vp->bmp, &rect);
    SDL_UnlockMutex(screen_mutex);
  }
}
```
由于我们的屏幕可以是任意大小的（我们将其设置为640x480，并且有设置它的方法，所以可以由用户调整大小），所以我们需要动态地计算出我们想要的矩形的大小。所以首先我们需要弄清楚我们的电影的宽高比，这只是宽度除以高度。一些编解码器将有一个奇数的样本宽高比，这只是单个像素或样本的宽度/高度无线电。由于我们编解码器上下文中的高度和宽度值是以像素为单位进行测量的，因此实际宽高比等于纵横比乘以示例宽高比。某些编解码器将显示宽高比为0，这表示每个像素的大小都是1x1。然后，我们将电影放大以适应屏幕上的大小。那里的＆3位旋转只是将值四舍五入到最接近的4的倍数。然后我们将影片居中，并调用[SDL_DisplayYUVOverlay（）](http://dranger.com/ffmpeg/functions.html#SDL_DisplayYUVOverlay)，确保使用屏幕互斥量来访问它。

那是这样吗？我们完了吗？那么，我们仍然需要重写音频代码来使用新的VideoStruct，但这些都是微不足道的变化，你可以看看示例代码中的那些。我们最后要做的就是改变我们对ffmpeg内部“退出（quit）”回调函数的回调：


```cpp
VideoState *global_video_state;

int decode_interrupt_cb(void) {
  return (global_video_state && global_video_state->quit);
}
```
我们将global_video_state设置为main（）中的大结构。

就是这样了！继续编译它：


```cpp
gcc -o tutorial04 tutorial04.c -lavutil -lavformat -lavcodec -lswscale -lz -lm \
`sdl-config --cflags --libs`
```
并享受你的未确认电影！下次我们终于会建立一个真正有效的视频播放器！
















