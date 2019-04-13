
# Android硬件编解码与软件编解码 - 小小情意 - 博客园






# [Android硬件编解码与软件编解码](https://www.cnblogs.com/xiaoxiaoqingyi/p/8476839.html)
最近做了一个android项目用到编解码功能。大概需求是：通过摄像头拍摄一段视频，然后抽帧，生成一个短视频，以及倒序视频，刚开始直接用 H.264 编码格式，没有使用MP4容器封装。做了这些功能后，反而觉得使用MP4格式更加兼容各机型，减少BUG出现。举个明显例子：在Android硬编的时候，常常会用到 MediaCodec和MediaExtractor 相结合。但是，如果你用的 H.264 裸视频文件，MediaExtractor 的 setSource 函数会报异常，它在某些机型（如魅族Note2，系统是5.1）无法解析该视频文件。
得到大概的需求后，最初我们使用FFmpeg来做视频编解码，所谓软件编解码。由于在处理的过程中速率太慢，且需要在解码后快速展示，所以该方案无法达到我们的预想效果(一个FFmpeg视频解码，并保存为jpeg例子：[https://github.com/xiaoxiaoqingyi/ffmpeg-android-video-decoder](https://github.com/xiaoxiaoqingyi/ffmpeg-android-video-decoder))。但其也有一些优点，比如在兼容方面，颜色转换方面都做得很好，毕竟不是硬件编解码（国内这么多机型，你懂的），其次FFmpeg能输出指定帧，而Android硬解（MediaCodec）不能输出指定帧，需要输入好几帧到解码器，才能解码出一帧。目前我还是没有找到输入一帧解出一帧的方案，哪位大神知道的，可以指导指导。
在软件编解码不太适合的情况下，就只能考虑用硬件编解码了(MediaCodec)。在前些日子，我参加了腾讯2017LIVE 直播开发者大会，了解到，现在的直播已经大部分使用硬件来编解码了。刚说了，有些机型不能使用MediaExtractor来解析 h.264文件，为了兼容大部分的机型，需要自己来解析，通过分析h.264文件的每一个字节区分每一帧位置且是什么类型帧。实现该需求，首先在从摄像头获取的数据，如果使用 Camera，一般设置为NV21格式， 但有些人使用Camera2，设置的格式是IMAGE。不管是哪种格式，最终都需要转换成yuv420sp或yuv420p(注意：在转码时候，最好使用jni，用C/C++来转格式，效率会高很多倍)，才能供MediaCodec编码，然后保存h.264文件。在创建MediaCodec实例化的时候，除了设置必须参数外，也要注意一些地方，比如，选择哪种编码器，一般情况会选择如下：
MediaCodec.createEncoderByType("video/avc");
这看上去其实没什么问题，大概原理就是获取最优的Encoder，获取Android系统中编码器注册表最前的一个，一般都是硬件解码(MediaCodec也能调用软件编解码)。这样创建编码器其实不太靠谱，虽然官网也是这么推荐，但是在国内众多的Android机型中，有些手机就会出问题，有的编码出现蓝屏，有些直接就闪退了。有个国外的例子，大概的意思就是先获取 "video/avc" 类型的编码器，然后通过 try catch 一个个试验，如果没问题，就选用这个编码器。源码：[https://github.com/ldm520/android_mediacodec_rtsp_h264](https://github.com/ldm520/android_mediacodec_rtsp_h264)
还有一个问题就是在设置 I 帧间隔的时候，有些手机不起作用，如下设置：
mediaFormat.setInteger(MediaFormat.KEY_I_FRAME_INTERVAL, interval);

针对这种情况，需要使用另外一种设置I帧的方式，强制设置:
Bundle params =newBundle();
params.putInt(MediaCodec.PARAMETER_KEY_REQUEST_SYNC_FRAME,0);
mMediaCodec.setParameters(params);
在编解码时，当把所有的数据都输入编解码器的时候，要记得输入结束符，编解码器才会输出所有的帧。
还有一个抽帧问题，如果使用MediaCodec来抽帧，生成一个新的视频。是否可以直接把H.264文件里的帧去掉就行了？这样不行的，通常会出现花屏。这需要重新把h.264文件输入到解码器，然后获取到自己想要的帧，再输入到一个新的编码器中，生成你想要的H.264文件。在这里还有一个格式问题，并不是从解码器解码出来的数据，就能直接使用编码器来编码，有部分手机可以，有些会出现蓝屏，甚至闪退的情况。这时候需要统一解码器处理的格式。如果你使用这种形式获取：
mMediaCodec.getOutputBuffer()
出来的格式各种各样，你很难去兼容。google已经推出了一种新的格式：
mMediaCodec.getOutputImage(outIndex)
得出的是一个Image 对象，该对象可以保存为 JPEG格式图片，也可以转换成NV21(参考：[http://www.cnblogs.com/welhzh/p/6079631.html](http://www.cnblogs.com/welhzh/p/6079631.html))，像上面拍摄部分，转换成yuv422格式，再输入到编码器编码。这样不管什么机型都可以兼容了（我试用10多部不同厂商手机），虽然绕了很多弯路。
在使用MediaCodec还是遇到比较多的问题，毕竟官网都说它是一个轻量的编解码器封装。该总结适合使用过MediaCodec或有一定的解码编码经验的童鞋们。如果你还没了解过 MediaCodec，可以参考官网:
[https://developer.android.google.cn/reference/android/media/MediaCodec.html](https://developer.android.google.cn/reference/android/media/MediaCodec.html)
或者中文翻译版：[http://blog.csdn.net/yssjz960427031/article/details/70050142](http://blog.csdn.net/yssjz960427031/article/details/70050142)
在使用MediaCodec的时候还遇到很多问题，这里没有一一列举出来， 欢迎有遇到同样问题或类似问题的童鞋留言讨论！










