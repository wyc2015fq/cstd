# MediaCodec文档翻译&&一些FAQ和例子 - u013366022的专栏 - CSDN博客
2014年05月14日 15:17:18[slitaz](https://me.csdn.net/u013366022)阅读数：2725
转自：http://www.cnblogs.com/Xiegg/p/3428529.html
前段时间项目需要，接触到android 4.0以后新添加的mediacodec类，并用mediacodec类实现了一个无缝播放视频的播放器，并用这个播放器简单的实现了dash协议。
接触到这么多，感觉GOOGLE给的文档写的不够详细，网络上也没有一个标准的例子，为了避免后来人走一些不必要的弯路，我在这里打算先用中文翻译一下codec的文档，然后再把自己在实现播放器时候的一些注意事项和问题列出来。
首先这篇文章是给半新手看的，你得有一定的安卓基础，但是由于作者本人的水平有限，也不是什么高手，所以有的地方会说的不清楚，还请包涵。
给技术牛人推荐一个 mediacodec 例子很多的网站：http://bigflake.com/mediacodec/
感觉像是官方人员编写的，里面有各种关于mediacodec的例子，还有4.3最新的muxer，都是以CTS的形式写的，本人没有接触过，但是参考代码还是很好的，毕竟实现方法都差不多。
# MediaCodec|文档翻译
## 　classoverView
　mediacodec类可以用来调用系统底层的编码/解码软件。
 　mediacodec一般是这么用的：
```
MediaCodec codec = MediaCodec.createDecoderByType(type);
 codec.configure(format, ...);
 codec.start();
 ByteBuffer[] inputBuffers = codec.getInputBuffers();
 ByteBuffer[] outputBuffers = codec.getOutputBuffers();
 for (;;) {
   int inputBufferIndex = codec.dequeueInputBuffer(timeoutUs);
   if (inputBufferIndex >= 0) {
     // fill inputBuffers[inputBufferIndex] with valid data
     ...
     codec.queueInputBuffer(inputBufferIndex, ...);
   }
   int outputBufferIndex = codec.dequeueOutputBuffer(timeoutUs);
   if (outputBufferIndex >= 0) {
     // outputBuffer is ready to be processed or rendered.
     ...
     codec.releaseOutputBuffer(outputBufferIndex, ...);
   } else if (outputBufferIndex == MediaCodec.INFO_OUTPUT_BUFFERS_CHANGED) {
     outputBuffers = codec.getOutputBuffers();
   } else if (outputBufferIndex == MediaCodec.INFO_OUTPUT_FORMAT_CHANGED) {
     // Subsequent data will conform to new format.
     MediaFormat format = codec.getOutputFormat();
     ...
   }
 }
 codec.stop();
 codec.release();
 codec = null;
```
//等会再逐一介绍上述代码每一句的作用
每一个Codec类都拥有input buffer和outputbuffer，这些buffer是通过API提供的函数按索引调用(Index)，这些buffer的格式是bytebuffer,实际上inputbuffer和outputbuffer都是bytebuffer的数组，用getInputBuffer和getOutputBuffer可以得到bytebuffer的调用。在成功调用Start()函数后，对象（原文是Client，在这里我理解为MediaCodec的实例化对象）并没有“拥有”input和output
 buffer,相应的，可以通过调用`[dequeueInputBuffer(long)](http://developer.android.com/reference/android/media/MediaCodec.html#dequeueInputBuffer(long))` 和
```
[dequeueOutputBuffer(MediaCodec.BufferInfo,
 long)](http://developer.android.com/reference/android/media/MediaCodec.html#dequeueOutputBuffer(android.media.MediaCodec.BufferInfo,%20long))把buffer的所有权从codec编解码器交换给对象。MediaCodec对象没有必要立刻把buffer传递给解码器或者释放到surface,上面的代码仅仅是一个简单的例子。
```
当MediaCodec对象有一个可用的input buffer时候，可以调用
```
[queueInputBuffer(int,
 int, int, long, int)](http://developer.android.com/reference/android/media/MediaCodec.html#queueInputBuffer(int,%20int,%20int,%20long,%20int))函数把buffer传递给解码器，当然，buffer里需要有数据。
```
解码器会相应的调用
```
[dequeueOutputBuffer(MediaCodec.BufferInfo,
 long)](http://developer.android.com/reference/android/media/MediaCodec.html#dequeueOutputBuffer(android.media.MediaCodec.BufferInfo,%20long))返回一个output buffer,如果继续调用 
```
[releaseOutputBuffer(int,
 boolean)](http://developer.android.com/reference/android/media/MediaCodec.html#releaseOutputBuffer(int,%20boolean))
```
 ，这个output buffer将被返回到解码器中，如果再
```
[configure(MediaFormat,
 Surface, MediaCrypto, int](http://developer.android.com/reference/android/media/MediaCodec.html#configure(android.media.MediaFormat,%20android.view.Surface,%20android.media.MediaCrypto,%20int)))
```
的时候传入了一个videoSurface(一般是SurfaceView).那么这个这个output buffer的内容将会显示在surface上。
```
不管是用于解码器的input buffer还是编码器的output buffer ,其中都包含了编码好的多媒体数据.对于视频格式的文件来说，buffer里包含的是一个短暂的时间片（比如一帧），对于音频格式的文件，可能是一个多帧的声音片段。不管哪种情况，buffer里的数据并不是任意字节边界的二进制数据，buffer里存储的不是数据流，而是一个元单元（access units）流。
很多媒体格式还需要多媒体文件头数据（这些数据一般是由一些包含set up data的buffer组成），或者编码器需要的特定数据。因此，最初传递给解码器的buffer必须是带有[BUFFER_FLAG_CODEC_CONFIG](http://developer.android.com/reference/android/media/MediaCodec.html#BUFFER_FLAG_CODEC_CONFIG)下标的编码特定数据，一般这个下标是由[queueInputBuffer(int,
 int, int, long, int)](http://developer.android.com/reference/android/media/MediaCodec.html#queueInputBuffer(int,%20int,%20int,%20long,%20int))生成的。编码器需要的数据，包括媒体格式信息，都通过调用 
```
[configure(MediaFormat,
 Surface, MediaCrypto, int)](http://developer.android.com/reference/android/media/MediaCodec.html#configure(android.media.MediaFormat,%20android.view.Surface,%20android.media.MediaCrypto,%20int))
```
 (in ByteBuffer entries with keys "csd-0", "csd-1", 括号里这句不知道怎么翻译)，自动传递给了codec类，不需要对象自己把格式信息传给codec（也就是configure会自动传递带有[BUFFER_FLAG_CODEC_CONFIG](http://developer.android.com/reference/android/media/MediaCodec.html#BUFFER_FLAG_CODEC_CONFIG)的buffer和媒体格式信息给编码器）。在输入数据的末尾，对象会通过[queueInputBuffer(int,
 int, int, long, int)](http://developer.android.com/reference/android/media/MediaCodec.html#queueInputBuffer(int,%20int,%20int,%20long,%20int))函数发送一个带有`[BUFFER_FLAG_END_OF_STREAM](http://developer.android.com/reference/android/media/MediaCodec.html#BUFFER_FLAG_END_OF_STREAM)` 下标的信号。为了解码与之前数据无关的数据(例如执行一个seek)，解码器需要调用flush()。在调用flush()后，对象里的inputbuffer或者outputbuffer都会清空，也就是说对象不再有任何buffer,但是数据的媒体格式信息不会改变，如果需要改变媒体格式信息，依次调用stop,configure,start.
## Summary
![](https://img-blog.csdn.net/20140514151706015?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMzM2NjAyMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## Public Methods
#### public void configure([MediaFormat](http://developer.android.com/reference/android/media/MediaFormat.html) format, [Surface](http://developer.android.com/reference/android/view/Surface.html) surface, [MediaCrypto](http://developer.android.com/reference/android/media/MediaCrypto.html) crypto, int flags)
Added in [API level 16](http://developer.android.com/guide/topics/manifest/uses-sdk-element.html#ApiLevels)
配置一个组件。
##### Parameters
flags
如果正在配置的对象是用作编码器，此处加上`[CONFIGURE_FLAG_ENCODE](http://developer.android.com/reference/android/media/MediaCodec.html#CONFIGURE_FLAG_ENCODE)` 标签。
#### public static [MediaCodec](http://developer.android.com/reference/android/media/MediaCodec.html)createByCodecName([String](http://developer.android.com/reference/java/lang/String.html) name)
Added in [API level 16](http://developer.android.com/guide/topics/manifest/uses-sdk-element.html#ApiLevels)
如果你知道你想实例化的组件确切名字， 用这个方法来初始化它，谨慎使用这个函数，最好配合从 `[MediaCodecList](http://developer.android.com/reference/android/media/MediaCodecList.html)获得的信息来使用。`
##### Parameters
name
需要实例化的组件的名字.
#### public static [MediaCodec](http://developer.android.com/reference/android/media/MediaCodec.html)createDecoderByType([String](http://developer.android.com/reference/java/lang/String.html) type)
Added in [API level 16](http://developer.android.com/guide/topics/manifest/uses-sdk-element.html#ApiLevels)
用给定的媒体格式来创建一个解码器.下面是部分媒体格式和他们所对应的key的列表：
- "video/x-vnd.on2.vp8" - VP8 video (i.e. video in .webm)
- "video/x-vnd.on2.vp9" - VP9 video (i.e. video in .webm)
- "video/avc" - H.264/AVC video
- "video/mp4v-es" - MPEG4 video
- "video/3gpp" - H.263 video
- "audio/3gpp" - AMR narrowband audio
- "audio/amr-wb" - AMR wideband audio
- "audio/mpeg" - MPEG1/2 audio layer III
- "audio/mp4a-latm" - AAC audio (note, this is raw AAC packets, not packaged in LATM!)
- "audio/vorbis" - vorbis audio
- "audio/g711-alaw" - G.711 alaw audio
- "audio/g711-mlaw" - G.711 ulaw audio
##### Parameters
type
输入数据的多媒体格式.
#### public static [MediaCodec](http://developer.android.com/reference/android/media/MediaCodec.html)createEncoderByType([String](http://developer.android.com/reference/java/lang/String.html) type)
Added in [API level 16](http://developer.android.com/guide/topics/manifest/uses-sdk-element.html#ApiLevels)
按给定的媒体类型创建一个编码器。
##### Parameters
type
所需要的输出媒体格式.
#### public final [Surface](http://developer.android.com/reference/android/view/Surface.html)createInputSurface()
Added in [API level 18](http://developer.android.com/guide/topics/manifest/uses-sdk-element.html#ApiLevels)
请求一个surface作为解码器的输入（替代掉inputbuffer?).这个函数需要在  
```
[configure(MediaFormat,
 Surface, MediaCrypto, int)](http://developer.android.com/reference/android/media/MediaCodec.html#configure(android.media.MediaFormat,%20android.view.Surface,%20android.media.MediaCrypto,%20int))
```
 之后 `[start()](http://developer.android.com/reference/android/media/MediaCodec.html#start())之前被调用。`
程序应该自己调用release去释放这个 surface而不是等gc 去做。
#### public final int dequeueInputBuffer(long timeoutUs)
Added in [API level 16](http://developer.android.com/guide/topics/manifest/uses-sdk-element.html#ApiLevels)
返回一个可用来填充有效数据的inputbuffer的索引（Index)，如果返回-1表示暂无可用的buffer. 如果 timeoutUs == 0,该方法会立刻返回值， 如果 timeoutUs < 0 则一直等待，如果 timeoutUs > 0则等待对应时间.
##### Parameters
timeoutUs
单位为微秒, 负数表示无穷大.
#### public final int dequeueOutputBuffer([MediaCodec.BufferInfo](http://developer.android.com/reference/android/media/MediaCodec.BufferInfo.html) info, long timeoutUs)
Added in [API level 16](http://developer.android.com/guide/topics/manifest/uses-sdk-element.html#ApiLevels)
排一个output buffer, 等待“timeoutUs"后若无有效值则阻塞，单位为 microseconds. 返回成功解码的outputbuffer的索引 或者 一个 INFO_* constants 常量.
##### Parameters
timeoutUs单位为微秒, 负数表示无穷大.
#### public final void flush()
Added in [API level 16](http://developer.android.com/guide/topics/manifest/uses-sdk-element.html#ApiLevels)
flush，重洗组件的input和output接口, 之前调用 `[dequeueInputBuffer(long)](http://developer.android.com/reference/android/media/MediaCodec.html#dequeueInputBuffer(long))` 和
```
[dequeueOutputBuffer(MediaCodec.BufferInfo,
 long)](http://developer.android.com/reference/android/media/MediaCodec.html#dequeueOutputBuffer(android.media.MediaCodec.BufferInfo,%20long))
```
 排好的buffer都变成不可用。
#### public [MediaCodecInfo](http://developer.android.com/reference/android/media/MediaCodecInfo.html)getCodecInfo()
Added in [API level 18](http://developer.android.com/guide/topics/manifest/uses-sdk-element.html#ApiLevels)
获得codec的信息.如果codec是 createDecoderByType 或createEncoderByType创建的, 事先并不知道用的什么组件，那么 调用这个函数不会返回CodecInfo。
#### public [ByteBuffer[]](http://developer.android.com/reference/java/nio/ByteBuffer.html)getInputBuffers()
Added in [API level 16](http://developer.android.com/guide/topics/manifest/uses-sdk-element.html#ApiLevels)
在start()之后可调用这个函数获得inputbuffer
#### public final [String](http://developer.android.com/reference/java/lang/String.html)getName()
Added in [API level 18](http://developer.android.com/guide/topics/manifest/uses-sdk-element.html#ApiLevels)
获得这个组件的名字. 
#### public [ByteBuffer[]](http://developer.android.com/reference/java/nio/ByteBuffer.html)getOutputBuffers()
Added in [API level 16](http://developer.android.com/guide/topics/manifest/uses-sdk-element.html#ApiLevels)
Call this after start() returns and whenever dequeueOutputBuffer signals an output buffer change by returning `[INFO_OUTPUT_BUFFERS_CHANGED](http://developer.android.com/reference/android/media/MediaCodec.html#INFO_OUTPUT_BUFFERS_CHANGED)`
这句话不好翻译，大意是在start()调用，或者在dequeueOutputBuffer 返回 [INFO_OUTPUT_BUFFERS_CHANGED](http://developer.android.com/reference/android/media/MediaCodec.html#INFO_OUTPUT_BUFFERS_CHANGED)信息后调用。
#### public final [MediaFormat](http://developer.android.com/reference/android/media/MediaFormat.html)getOutputFormat()
Added in [API level 16](http://developer.android.com/guide/topics/manifest/uses-sdk-element.html#ApiLevels)
如果 dequeueOutputBuffer 返回`[INFO_OUTPUT_FORMAT_CHANGED](http://developer.android.com/reference/android/media/MediaCodec.html#INFO_OUTPUT_FORMAT_CHANGED) ，调用这个函数。`
#### public final void queueInputBuffer(int index, int offset, int size, long presentationTimeUs, int flags)
Added in [API level 16](http://developer.android.com/guide/topics/manifest/uses-sdk-element.html#ApiLevels)
再给指定索引的inputbuffer填充完数据后，把它交给编码器. 很多解码器需要媒体文件的文件头,例如 vorbis audio中的编码表 ，AVC video中的PPS/SPS，. `[MediaExtractor](http://developer.android.com/reference/android/media/MediaExtractor.html)` 类提供了codec所需要的多媒体格式信息
 ... 这些buffer应该加上 `[BUFFER_FLAG_CODEC_CONFIG](http://developer.android.com/reference/android/media/MediaCodec.html#BUFFER_FLAG_CODEC_CONFIG)标签`.
 如果这是最后一个输入数据 (接下来没有其他的数据输入，除非马上要调用flush()) 则应该加上 `[BUFFER_FLAG_END_OF_STREAM](http://developer.android.com/reference/android/media/MediaCodec.html#BUFFER_FLAG_END_OF_STREAM)标签`.
##### Parameters
flags
根据需要从 `[BUFFER_FLAG_SYNC_FRAME](http://developer.android.com/reference/android/media/MediaCodec.html#BUFFER_FLAG_SYNC_FRAME)`, `[BUFFER_FLAG_CODEC_CONFIG](http://developer.android.com/reference/android/media/MediaCodec.html#BUFFER_FLAG_CODEC_CONFIG)` ， `[BUFFER_FLAG_END_OF_STREAM](http://developer.android.com/reference/android/media/MediaCodec.html#BUFFER_FLAG_END_OF_STREAM)选一个，或者0~`
##### Throws
[MediaCodec.CryptoException](http://developer.android.com/reference/android/media/MediaCodec.CryptoException.html)
如果在 
```
[configure(MediaFormat,
 Surface, MediaCrypto, int)](http://developer.android.com/reference/android/media/MediaCodec.html#configure(android.media.MediaFormat,%20android.view.Surface,%20android.media.MediaCrypto,%20int))被加密则抛出异常
```
#### public final void queueSecureInputBuffer(int index, int offset, [MediaCodec.CryptoInfo](http://developer.android.com/reference/android/media/MediaCodec.CryptoInfo.html) info, long presentationTimeUs, int flags)
Added in [API level 16](http://developer.android.com/guide/topics/manifest/uses-sdk-element.html#ApiLevels)
与 
```
[queueInputBuffer(int,
 int, int, long, int)](http://developer.android.com/reference/android/media/MediaCodec.html#queueInputBuffer(int,%20int,%20int,%20long,%20int))相似，但是
```
 传入的是一个被加密的buffer。
##### Parameters
flags
A bitmask of flags `[BUFFER_FLAG_SYNC_FRAME](http://developer.android.com/reference/android/media/MediaCodec.html#BUFFER_FLAG_SYNC_FRAME)`, `[BUFFER_FLAG_CODEC_CONFIG](http://developer.android.com/reference/android/media/MediaCodec.html#BUFFER_FLAG_CODEC_CONFIG)` or `[BUFFER_FLAG_END_OF_STREAM](http://developer.android.com/reference/android/media/MediaCodec.html#BUFFER_FLAG_END_OF_STREAM)`.
##### Throws
[MediaCodec.CryptoException](http://developer.android.com/reference/android/media/MediaCodec.CryptoException.html)
if an error occurs while attempting to decrypt the buffer. An error code associated with the exception helps identify the reason for the failure.
#### public final void release()
Added in [API level 16](http://developer.android.com/guide/topics/manifest/uses-sdk-element.html#ApiLevels)
在你调用这个函数的时候，确保释放了所以不需要用的组件，而不是依赖gc为你做这些事
#### public final void releaseOutputBuffer(int index, boolean render)
Added in [API level 16](http://developer.android.com/guide/topics/manifest/uses-sdk-element.html#ApiLevels)
如果你处理完这个buffer, 调用这个函数把buffer重新返回给codec. 如果你在之前configure()的时候为组件配置了一个surface，那么codec类会在那个surface上显示这个buffer
##### Parameters
render
如果有可用的显示surface，传递为true可表示显示。
#### public final void setParameters([Bundle](http://developer.android.com/reference/android/os/Bundle.html) params)
Added in [API level 19](http://developer.android.com/guide/topics/manifest/uses-sdk-element.html#ApiLevels)
可以给组件加上一些附加参数（具体参数文档没写，估计是仍在测试阶段）
#### public final void setVideoScalingMode(int mode)
Added in [API level 16](http://developer.android.com/guide/topics/manifest/uses-sdk-element.html#ApiLevels)
如果在 
```
[configure(MediaFormat,
 Surface, MediaCrypto, int)](http://developer.android.com/reference/android/media/MediaCodec.html#configure(android.media.MediaFormat,%20android.view.Surface,%20android.media.MediaCrypto,%20int))传入了surface,可以通过这个函数来设置显示模式。
```
#### public final void signalEndOfInputStream()
Added in [API level 18](http://developer.android.com/guide/topics/manifest/uses-sdk-element.html#ApiLevels)
标志输入流的结束。 作用于传递 `[BUFFER_FLAG_END_OF_STREAM](http://developer.android.com/reference/android/media/MediaCodec.html#BUFFER_FLAG_END_OF_STREAM)` 下标相同.
 但是这个函数只用在把[createInputSurface()](http://developer.android.com/reference/android/media/MediaCodec.html#createInputSurface())返回的surface作为输入，且codec是编码器的情况。
#### public final void start()
Added in [API level 16](http://developer.android.com/guide/topics/manifest/uses-sdk-element.html#ApiLevels)
在成功配置组件后, 调用start()函数。相应的可以通过函数 操作inputbuffer和outputbuffer。
#### public final void stop()
Added in [API level 16](http://developer.android.com/guide/topics/manifest/uses-sdk-element.html#ApiLevels)
停止，但是可以调用start()重开。如果要完全停止，调用`[release()](http://developer.android.com/reference/android/media/MediaCodec.html#release())` 。
