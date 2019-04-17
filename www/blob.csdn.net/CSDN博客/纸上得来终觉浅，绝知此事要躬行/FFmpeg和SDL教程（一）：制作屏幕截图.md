# FFmpeg和SDL教程（一）：制作屏幕截图 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年01月12日 18:23:57[boonya](https://me.csdn.net/boonya)阅读数：1062








英文原文地址：[http://dranger.com/ffmpeg/tutorial01.html](http://dranger.com/ffmpeg/tutorial01.html)



## 概观
电影文件有几个基本组件。首先，文件本身被称为容器，容器的类型决定了文件中信息的位置。容器的例子是AVI和Quicktime。接下来，你有一堆流;例如，你通常有一个音频流和一个视频流。 （“流”只是“随着时间的推移可用的一系列数据元素”的流行词）。流中的数据元素称为帧。每个流由不同种类的编解码器编码。编解码器定义了实际数据如何被编码和解码 - 因此被称为CODEC。编解码器的例子是DivX和MP3。数据包然后从流中读取。数据包是可以包含数据的数据片段，这些数据被解码成我们可以最终为我们的应用程序处理的原始帧。出于我们的目的，每个数据包包含完整的帧，或者在音频情况下包含多个帧。

在最基本的层面上，处理视频和音频流非常简单：



```cpp
10 OPEN video_stream FROM video.avi
20 READ packet FROM video_stream INTO frame
30 IF frame NOT COMPLETE GOTO 20
40 DO SOMETHING WITH frame
50 GOTO 20
```
使用ffmpeg处理多媒体就像这个程序一样简单，虽然有些程序可能有一个非常复杂的“DO SOMETHING”步骤。 所以在本教程中，我们将打开一个文件，从里面的视频流中读取文件，我们的DO SOMETHING将把帧写入PPM文件。


## 打开文件
首先，让我们看看我们如何打开一个文件。 用ffmpeg，你必须首先初始化库。

```cpp
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <ffmpeg/swscale.h>
...
int main(int argc, charg *argv[]) {
av_register_all();
```
这将在库中注册所有可用的文件格式和编解码器，以便在打开具有相应格式/编解码器的文件时自动使用它们。 请注意，您只需调用一次av_register_all（），所以我们在main（）中执行。 如果你喜欢，可以只注册某些单独的文件格式和编解码器，但通常没有理由为什么你必须这样做。
现在我们可以打开这个文件了：

```cpp
AVFormatContext *pFormatCtx = NULL;

// Open video file
if(avformat_open_input(&pFormatCtx, argv[1], NULL, 0, NULL)!=0)
  return -1; // Couldn't open file
```
我们从第一个参数中得到我们的文件名。 这个函数读取文件头，并在我们给出的AVFormatContext结构中存储关于文件格式的信息。 最后三个参数用于指定文件格式，缓冲区大小和格式选项，但通过将其设置为NULL或0，libavformat将自动检测这些参数。
这个函数只看标头，所以接下来我们需要检查文件中的流信息：


```cpp
// Retrieve stream information
if(avformat_find_stream_info(pFormatCtx, NULL)<0)
  return -1; // Couldn't find stream information
```
他的函数用适当的信息填充pFormatCtx->流。 我们引入一个方便的调试功能来向我们展示里面的内容：


```cpp
// Dump information about file onto standard error
av_dump_format(pFormatCtx, 0, argv[1], 0);
```
现在，pFormatCtx-> streams只是一个大小为pFormatCtx-> nb_streams的指针数组，所以让我们通过它直到找到一个视频流。


```cpp
int i;
AVCodecContext *pCodecCtxOrig = NULL;
AVCodecContext *pCodecCtx = NULL;

// Find the first video stream
videoStream=-1;
for(i=0; i<pFormatCtx->nb_streams; i++)
  if(pFormatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_VIDEO) {
    videoStream=i;
    break;
  }
if(videoStream==-1)
  return -1; // Didn't find a video stream

// Get a pointer to the codec context for the video stream
pCodecCtx=pFormatCtx->streams[videoStream]->codec;
```
流关于编解码器的信息就是我们所说的“编解码器上下文”。 这包含有关流正在使用的编解码器的所有信息，现在我们有一个指向它的指针。 但是我们仍然需要找到真正的编解码器并打开它：


```cpp
AVCodec *pCodec = NULL;

// Find the decoder for the video stream
pCodec=avcodec_find_decoder(pCodecCtx->codec_id);
if(pCodec==NULL) {
  fprintf(stderr, "Unsupported codec!\n");
  return -1; // Codec not found
}
// Copy context
pCodecCtx = avcodec_alloc_context3(pCodec);
if(avcodec_copy_context(pCodecCtx, pCodecCtxOrig) != 0) {
  fprintf(stderr, "Couldn't copy codec context");
  return -1; // Error copying codec context
}
// Open codec
if(avcodec_open2(pCodecCtx, pCodec)<0)
  return -1; // Could not open codec
```
请注意，我们不能直接使用视频流中的AVCodecContext！ 所以我们必须使用avcodec_copy_context（）将上下文复制到一个新的位置（当然，在为它分配内存之后）。


## 存储数据
现在我们需要一个地方来存储帧：


```cpp
AVFrame *pFrame = NULL;

// Allocate video frame
pFrame=av_frame_alloc();
```
由于我们打算输出存储在24位RGB中的PPM文件，因此我们将不得不将帧从原始格式转换为RGB。 ffmpeg会为我们做这些转换。 对于大多数项目（包括我们的），我们将要把我们的初始框架转换成特定的格式。 现在我们为转换的帧分配一个帧。


```cpp
// Allocate an AVFrame structure
pFrameRGB=av_frame_alloc();
if(pFrameRGB==NULL)
  return -1;
```
尽管我们已经分配了框架，但是在转换时仍然需要放置原始数据。 我们使用avpicture_get_size来获得我们需要的大小，并手动分配空间：


```cpp
uint8_t *buffer = NULL;
int numBytes;
// Determine required buffer size and allocate buffer
numBytes=avpicture_get_size(PIX_FMT_RGB24, pCodecCtx->width,
                            pCodecCtx->height);
buffer=(uint8_t *)av_malloc(numBytes*sizeof(uint8_t));
```
av_malloc是ffmpeg的malloc，它只是malloc的一个简单的包装，确保内存地址对齐等。 它不会保护你免受内存泄漏，双重释放或其他malloc问题。
现在我们使用avpicture_fill将帧与我们新分配的缓冲区关联起来。 关于AVPicture转换：AVPicture结构是AVFrame结构的子集 - AVFrame结构的开始与AVPicture结构相同。

```cpp
// Assign appropriate parts of buffer to image planes in pFrameRGB
// Note that pFrameRGB is an AVFrame, but AVFrame is a superset
// of AVPicture
avpicture_fill((AVPicture *)pFrameRGB, buffer, PIX_FMT_RGB24,
                pCodecCtx->width, pCodecCtx->height);
```
最后！ 现在我们准备从流中读取！


## 读取数据
我们要做的是通过读取数据包来读取整个视频流，将数据解码成我们的帧，一旦我们的帧完成，我们将转换并保存它。

```cpp
struct SwsContext *sws_ctx = NULL;
int frameFinished;
AVPacket packet;
// initialize SWS context for software scaling
sws_ctx = sws_getContext(pCodecCtx->width,
    pCodecCtx->height,
    pCodecCtx->pix_fmt,
    pCodecCtx->width,
    pCodecCtx->height,
    PIX_FMT_RGB24,
    SWS_BILINEAR,
    NULL,
    NULL,
    NULL
    );

i=0;
while(av_read_frame(pFormatCtx, &packet)>=0) {
  // Is this a packet from the video stream?
  if(packet.stream_index==videoStream) {
	// Decode video frame
    avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished, &packet);
    
    // Did we get a video frame?
    if(frameFinished) {
    // Convert the image from its native format to RGB
        sws_scale(sws_ctx, (uint8_t const * const *)pFrame->data,
		  pFrame->linesize, 0, pCodecCtx->height,
		  pFrameRGB->data, pFrameRGB->linesize);
	
        // Save the frame to disk
        if(++i<=5)
          SaveFrame(pFrameRGB, pCodecCtx->width, 
                    pCodecCtx->height, i);
    }
  }
    
  // Free the packet that was allocated by av_read_frame
  av_free_packet(&packet);
}
```
这个过程很简单：av_read_frame（）读入一个数据包，并将其存储在AVPacket结构中。 请注意，我们只分配了数据包结构 - ffmpeg为我们分配了内部数据，这是packet.data所指向的。 稍后由av_free_packet（）释放它。 avcodec_decode_video（）将数据包转换为我们的帧。 然而，在解码一个数据包之后，我们可能没有所有的信息，所以当我们有下一个帧的时候，avcodec_decode_video（）为我们设置frameFinished。 最后，我们使用sws_scale（）将原始格式（pCodecCtx-> pix_fmt）转换为RGB。 请记住，您可以将AVFrame指针强制转换为AVPicture指针。 最后，我们将框架以及高度和宽度信息传递给我们的SaveFrame函数。
现在我们所要做的就是使SaveFrame函数将RGB信息写入PPM格式的文件中。 我们将对PPM格式本身进行粗略的描述; 相信我们，它的工作。



```cpp
void SaveFrame(AVFrame *pFrame, int width, int height, int iFrame) {
  FILE *pFile;
  char szFilename[32];
  int  y;
  
  // Open file
  sprintf(szFilename, "frame%d.ppm", iFrame);
  pFile=fopen(szFilename, "wb");
  if(pFile==NULL)
    return;
  
  // Write header
  fprintf(pFile, "P6\n%d %d\n255\n", width, height);
  
  // Write pixel data
  for(y=0; y<height; y++)
    fwrite(pFrame->data[0]+y*pFrame->linesize[0], 1, width*3, pFile);
  
  // Close file
  fclose(pFile);
}
```
我们做一些标准的文件打开等，然后写入RGB数据。 我们一次写一行文件。 一个PPM文件只是一个文件，它具有以长字符串排列的RGB信息。 如果你知道HTML的颜色，就好像#ff0000＃ff0000 ....一样，将每个像素的颜色排列成一个红色的屏幕。 （这是存储在二进制，没有分隔符，但你明白了。）标题指出图像是多么宽，高，RGB值的最大值。
现在回到我们的main（）函数。 一旦我们完成了视频流的阅读，我们只需要清理一切：

```cpp
// Free the RGB image
av_free(buffer);
av_free(pFrameRGB);

// Free the YUV frame
av_free(pFrame);

// Close the codecs
avcodec_close(pCodecCtx);
avcodec_close(pCodecCtxOrig);

// Close the video file
avformat_close_input(&pFormatCtx);

return 0;
```
你会注意到我们用av_free来分配我们用avcode_alloc_frame和av_malloc分配的内存。
这就是代码！ 现在，如果你在Linux或类似的平台上，你将运行：


```cpp
gcc -o tutorial01 tutorial01.c -lavutil -lavformat -lavcodec -lz -lavutil -lm
```
如果你有一个旧版本的ffmpeg，你可能需要删除-lavutil：


```cpp
gcc -o tutorial01 tutorial01.c -lavformat -lavcodec -lz -lm
```
大多数图像程序应该能够打开PPM文件。 在一些电影文件上测试它。









