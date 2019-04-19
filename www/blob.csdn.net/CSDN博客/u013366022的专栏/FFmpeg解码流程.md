# FFmpeg解码流程 - u013366022的专栏 - CSDN博客
2015年04月14日 22:35:33[slitaz](https://me.csdn.net/u013366022)阅读数：308
FFmpeg解码流程：
1. 注册所有容器格式和CODEC: av_register_all()
2. 打开文件: av_open_input_file()    → avformat_open_input()
3. 从文件中提取流信息: av_find_stream_info()
4. 穷举所有的流，查找其中种类为CODEC_TYPE_VIDEO  →AVMEDIA_TYPE_VIDEO
5. 查找对应的解码器: avcodec_find_decoder()
6. 打开编解码器: avcodec_open()  →avcodec_open2()
7. 为解码帧分配内存: avcodec_alloc_frame()
8. 不停地从码流中提取出帧数据: av_read_frame()
9. 判断帧的类型，对于视频帧调用: avcodec_decode_video()  →avcodec_decode_video2
10. 解码完后，释放解码器: avcodec_close()
11. 关闭输入文件:av_close_input_file()
### 视频播放过程
首先简单介绍以下视频文件的相关知识。我们平时看到的视频文件有许多格式，比如 avi， mkv， rmvb， mov， mp4等等，这些被称为[容器](http://en.wikipedia.org/wiki/Digital_container_format)（[Container](http://wiki.multimedia.cx/index.php?title=Category:Container_Formats)），
 不同的容器格式规定了其中音视频数据的组织方式（也包括其他数据，比如字幕等）。容器中一般会封装有视频和音频轨，也称为视频流（stream）和音频 流，播放视频文件的第一步就是根据视频文件的格式，解析（demux）出其中封装的视频流、音频流以及字幕（如果有的话），解析的数据读到包 （packet）中，每个包里保存的是视频帧（frame）或音频帧，然后分别对视频帧和音频帧调用相应的解码器（decoder）进行解码，比如使用 H.264编码的视频和MP3编码的音频，会相应的调用H.264解码器和MP3解码器，解码之后得到的就是原始的图像(YUV
 or RGB)和声音(PCM)数据，然后根据同步好的时间将图像显示到屏幕上，将声音输出到声卡，最终就是我们看到的视频。
FFmpeg的API就是根据这个过程设计的，因此使用FFmpeg来处理视频文件的方法非常直观简单。下面就一步一步介绍从视频文件中解码出图片的过程。
### 声明变量
首先定义整个过程中需要使用到的变量：
|`01`|`int``main(``int``argc, ``const``char``*argv[])`|
|`02`|`{`|
|`03`|`  ````AVFormatContext *pFormatCtx = NULL;```|
|`04`|`  ``int````i, videoStream;```|
|`05`|`  ````AVCodecContext  *pCodecCtx;```|
|`06`|`  ````AVCodec         *pCodec;```|
|`07`|`  ````AVFrame         *pFrame;```|
|`08`|`  ````AVFrame         *pFrameRGB;```|
|`09`|`  ````AVPacket        packet;```|
|`10`|`  ``int``frameFinished;`|
|`11`|`  ``int``numBytes;`|
|`12`|`  ````uint8_t         *buffer;```|
- [AVFormatContext](http://ffmpeg.org/doxygen/trunk/structAVFormatContext.html)：保存需要读入的文件的格式信息，比如流的个数以及流数据等
- [AVCodecCotext](http://ffmpeg.org/doxygen/trunk/structAVCodecContext.html)：保存了相应流的详细编码信息，比如视频的宽、高，编码类型等。
- [pCodec](http://ffmpeg.org/doxygen/trunk/structAVCodec.html)：真正的编解码器，其中有编解码需要调用的函数
- [AVFrame](http://ffmpeg.org/doxygen/trunk/structAVFrame.html)：用于保存数据帧的数据结构，这里的两个帧分别是保存颜色转换前后的两帧图像
- [AVPacket](http://ffmpeg.org/doxygen/trunk/structAVPacket.html)：解析文件时会将音/视频帧读入到packet中
### 打开文件
接下来我们打开一个视频文件。
|`1`|`av_register_all();`|
[av_register_all](http://ffmpeg.org/doxygen/trunk/group__lavf__core.html#g917265caec45ef5a0646356ed1a507e3) 定义在 [libavformat](http://ffmpeg.org/doxygen/trunk/avformat_8h.html) 里，调用它用以注册所有支持的文件格式以及编解码器，从其[实现代码](http://ffmpeg.org/doxygen/trunk/allformats_8c-source.html#l00040)里可以看到它会调用
 avcodec_register_all，因此之后就可以用所有ffmpeg支持的codec了。
|`1`|`if````( avformat_open_input(&pFormatCtx, argv[1], NULL, NULL) != 0 )```|
|`2`|`    ``return``-1;`|
使用新的API [avformat_open_input](http://ffmpeg.org/doxygen/trunk/group__lavf__decoding.html#g10a404346c646e4ab58f4ed798baca32)来打开一个文件，第一个参数是一个AVFormatContext指针变量的地址，它会根据打开的文件信息填充AVFormatContext，需要注意的是，此处的pFormatContext必须为NULL或由[avformat_alloc_context](http://ffmpeg.org/doxygen/trunk/group__lavf__core.html#gc7a91abf2f59648d995894711f070f62)分配得到，这也是上一节中将其初始化为NULL的原因，否则此函数调用会出问题。第二个参数是打开的文件名，通过argv[1]指定，也就是命令行的第一个参数。后两个参数分别用于指定特定的输入格式（[AVInputFormat](http://ffmpeg.org/doxygen/trunk/structAVInputFormat.html)）以及指定文件打开额外参数的[AVDictionary](http://ffmpeg.org/doxygen/trunk/structAVDictionary.html)结构，这里均留作NULL。
|`1`|`if````( avformat_find_stream_info(pFormatCtx, NULL ) < 0 )```|
|`2`|`    ``return``-1;`|
|`3`|` `|
|`4`|`  ````av_dump_format(pFormatCtx, -1, argv[1], 0);```|
[avformat_open_input](http://ffmpeg.org/doxygen/trunk/libavformat_2utils_8c-source.html#l00660)函数只是读文件头，并不会填充流信息，因此我们需要接下来调用[avformat_find_stream_info](http://ffmpeg.org/doxygen/trunk/group__lavf__decoding.html#gd42172e27cddafb81096939783b157bb)获取文件中的流信息，此函数会读取packet，并确定文件中所有的流信息，设置pFormatCtx->streams指向文件中的流，但此函数并不会改变文件指针，读取的packet会给后面的解码进行处理。
最后调用一个帮助函数[av_dump_format](http://ffmpeg.org/doxygen/trunk/group__lavf__misc.html#ge2645941f2dc779c307eb6314fd39f10)，输出文件的信息，也就是我们在使用ffmpeg时能看到的文件详细信息。第二个参数指定输出哪条流的信息，-1表示给ffmpeg自己选择。最后一个参数用于指定dump的是不是输出文件，我们dump的是输入文件，因此一定要是0。
现在 pFormatCtx->streams 中已经有所有流了，因此现在我们遍历它找到第一条视频流:
|`1`|```videoStream = -1;```|
|`2`|`for````( i = 0; i < pFormatCtx->nb_streams; i++ )```|
|`3`|`  ``if````( pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {```|
|`4`|`    ````videoStream = i;```|
|`5`|`    ``break``;`|
|`6`|`  ``}`|
|`7`||
|`8`|`if````( videoStream == -1 )```|
|`9`|`  ``return``-1;`|
codec_type 的宏定义已经由以前的 CODEC_TYPE_VIDEO 改为 AVMEDIA_TYPE_VIDEO 了。接下来我们通过这条 video stream 的编解码信息打开相应的解码器：
|`1`|```pCodecCtx = pFormatCtx->streams[videoStream]->codec;```|
|`2`||
|`3`|```pCodec = avcodec_find_decoder(pCodecCtx->codec_id);```|
|`4`|`if````( pCodec == NULL )```|
|`5`|`  ``return``-1;`|
|`6`||
|`7`|`if````( avcodec_open2(pCodecCtx, pCodec, NULL) < 0 )```|
|`8`|`  ``return``-1;`|
### 分配图像缓存
接下来我们准备给即将解码的图片分配内存空间。
|`1`|```pFrame = avcodec_alloc_frame();```|
|`2`|`  ``if````( pFrame == NULL )```|
|`3`|`    ``return``-1;`|
|`4`|` `|
|`5`|`  ````pFrameRGB = avcodec_alloc_frame();```|
|`6`|`  ``if````( pFrameRGB == NULL )```|
|`7`|`    ``return``-1;`|
调用 [avcodec_alloc_frame](http://ffmpeg.org/doxygen/trunk/libavcodec_2avcodec_8h.html#d5f9212dec34c9fff0124171fa684a18) 分配帧，因为最后我们会将图像写成
 24-bits RGB 的 PPM 文件，因此这里需要两个 AVFrame，pFrame用于存储解码后的数据，pFrameRGB用于存储转换后的数据:
|`1`|```numBytes = avpicture_get_size(PIX_FMT_RGB24, pCodecCtx->width,```|
|`2`|`              ``pCodecCtx->height);`|
这里调用 [avpicture_get_size](http://ffmpeg.org/doxygen/trunk/libavcodec_2avcodec_8h.html#c35e88cdafc18d57d131c7642615377d)，根据
 pCodecCtx 中原始图像的宽高计算 RGB24 格式的图像需要占用的空间大小，这是为了之后给 pFrameRGB 分配空间:
|`1`|```buffer = av_malloc(numBytes);```|
|`2`|` `|
|`3`|`  ````avpicture_fill( (AVPicture *)pFrameRGB, buffer, PIX_FMT_RGB24,```|
|`4`|`          ````pCodecCtx->width, pCodecCtx->height);```|
接着上面的，首先是用 [av_malloc](http://ffmpeg.org/doxygen/trunk/group__lavu__mem.html#gcbca30ebc510a7e4156d66e7aceb2dc8) 分配上面计算大小的内存空间，然后调用 [avpicture_fill](http://ffmpeg.org/doxygen/trunk/libavcodec_2avcodec_8h.html#54da074edd8339905bc90b75b1402d4a) 将
 pFrameRGB 跟 buffer 指向的内存关联起来。
### 获取图像
OK，一切准备好就可以开始从文件中读取视频帧并解码得到图像了。
|`01`|```i = 0;```|
|`02`|`while````( av_read_frame(pFormatCtx, &packet) >= 0 ) {```|
|`03`|`  ``if````( packet.stream_index == videoStream ) {```|
|`04`|`    ````avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished, &packet);```|
|`05`||
|`06`|`    ``if````( frameFinished ) {```|
|`07`|`  ``struct````SwsContext *img_convert_ctx = NULL;```|
|`08`|`  ````img_convert_ctx =```|
|`09`|`    ````sws_getCachedContext(img_convert_ctx, pCodecCtx->width,```|
|`10`|`                 ````pCodecCtx->height, pCodecCtx->pix_fmt,```|
|`11`|`                 ````pCodecCtx->width, pCodecCtx->height,```|
|`12`|`                 ````PIX_FMT_RGB24, SWS_BICUBIC,```|
|`13`|`                 ````NULL, NULL, NULL);```|
|`14`|`  ``if````( !img_convert_ctx ) {```|
|`15`|`    ``fprintf``(stderr, ````"Cannot initialize sws conversion context\n"````);`|
|`16`|`    ``exit``(1);`|
|`17`|`  ``}`|
|`18`|`  ````sws_scale(img_convert_ctx, (````const``uint8_t* ``const``*)pFrame->data,`|
|`19`|`        ````pFrame->linesize, 0, pCodecCtx->height, pFrameRGB->data,```|
|`20`|`        ``pFrameRGB->linesize);`|
|`21`|`  ``if````( i++ < 50 )```|
|`22`|`    ````SaveFrame(pFrameRGB, pCodecCtx->width, pCodecCtx->height, i);```|
|`23`|`    ``}`|
|`24`|`  ``}`|
|`25`|`  ``av_free_packet(&packet);`|
|`26`|`}`|
[av_read_frame](http://ffmpeg.org/doxygen/trunk/group__lavf__decoding.html#g4fdb3084415a82e3810de6ee60e46a61) 从文件中读取一个packet，对于视频来说一个packet里面包含一帧图像数据，音频可能包含多个帧（当音频帧长度固定时），读到这一帧后，如果是视频帧，则使用 [avcodec_decode_video2](http://ffmpeg.org/doxygen/trunk/libavcodec_2avcodec_8h.html#3e4048fd6893d4a001bdaa3d251f3c03) 对packet中的帧进行解码，有时候解码器并不能从一个packet中解码得到一帧图像数据（比如在需要其他参考帧的情况下），因此会设置
 frameFinished，如果已经得到下一帧图像则设置 frameFinished 非零，否则为零。所以这里我们判断 frameFinished 是否为零来确定 pFrame 中是否已经得到解码的图像。注意在每次处理完后需要调用 [av_free_packet](http://ffmpeg.org/doxygen/trunk/libavcodec_2avcodec_8h.html#e65881462e78f98b268f34661d921ee0) 释放读取的packet。
解码得到图像后，很有可能不是我们想要的 RGB24 格式，因此需要使用 [swscale](http://ffmpeg.org/doxygen/trunk/swscale_8h.html) 来做转换，调用[sws_getCachedContext](http://ffmpeg.org/doxygen/trunk/swscale_8h.html#2e12a713a6ac7b1cf96e17c4c049ab15) 得到转换上下文，使用 [sws_scale](http://ffmpeg.org/doxygen/trunk/swscale_8h.html#e531c9754c9205d90ad6800015046d74) 将图形从解码后的格式转换为
 RGB24，最后将前50帧写人 ppm 文件。最后释放图像以及关闭文件：
[view
 source](http://my.oschina.net/u/555701/blog/56616#viewSource)
[print](http://my.oschina.net/u/555701/blog/56616#printSource)[?](http://my.oschina.net/u/555701/blog/56616#about)
|`01`|`av_free(buffer);`|
|`02`|`  ``av_free(pFrameRGB);`|
|`03`|`  ``av_free(pFrame);`|
|`04`|`  ``avcodec_close(pCodecCtx);`|
|`05`|`  ``avformat_close_input(&pFormatCtx);`|
|`06`|` `|
|`07`|`  ``return``0;`|
|`08`|`}`|
|`09`|` `|
|`10`|`static``void````SaveFrame(AVFrame *pFrame,````int``width, ``int``height, ``int``iFrame)`|
|`11`|`{`|
|`12`|`  ``FILE``*pFile;`|
|`13`|`  ``char``szFilename[32];`|
|`14`|`  ``int``y;`|
|`15`|` `|
|`16`|`  ``sprintf``(szFilename, ``"frame%d.ppm"````, iFrame);```|
|`17`|`  ````pFile =````fopen``(szFilename, ``"wb"``);`|
|`18`|`  ``if````( !pFile )```|
|`19`|`    ``return``;`|
|`20`|`  ``fprintf``(pFile, ````"P6\n%d %d\n255\n"``````, width, height);```|
|`21`|` `|
|`22`|`  ``for````( y = 0; y < height; y++ )```|
|`23`|`    ``fwrite````(pFrame->data[0] + y * pFrame->linesize[0], 1, width * 3, pFile);```|
|`24`|` `|
|`25`|`  ``fclose``(pFile);`|
|`26`|`}`|
