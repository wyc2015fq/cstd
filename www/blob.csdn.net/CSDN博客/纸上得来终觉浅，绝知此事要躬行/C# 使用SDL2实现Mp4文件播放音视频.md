# C# 使用SDL2实现Mp4文件播放音视频 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年03月08日 14:28:36[boonya](https://me.csdn.net/boonya)阅读数：1776








播放音视频的关键：视频的格式是H264，音频的格式是AAC。使用ffmpeg探测流的方式来实现音视频流的解码播放。

数据处理逻辑：H264->YUV     AAC->PCM。


#### SDL2工具类

```
using SDL2;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace CvNetVideo
{
    public unsafe class SDLHelper
    {
        private IntPtr screen;
        private IntPtr sdlrenderer;
        private IntPtr sdltexture;
        SDL.SDL_Rect sdlrect;
        SDL.SDL_Event sdlevent;
        bool isInit = false;
        public SDLHelper()
        {



        }

        
        public void SDL_MaximizeWindow()
        {

        }

        public int SDL_Init(int width, int height, IntPtr intPtr)
        {
            lock (this)
            {
                if (!isInit)
                {
                    // 初始化调用SDL.SDL_Init(SDL.SDL_INIT_VIDEO | SDL.SDL_INIT_AUDIO | SDL.SDL_INIT_TIMER)
                    if (SDL.SDL_Init(SDL.SDL_INIT_VIDEO | SDL.SDL_INIT_AUDIO | SDL.SDL_INIT_TIMER) < 0)
                    {
                        Console.WriteLine("Could not initialize SDL - {0}\n", SDL.SDL_GetError());
                        return -1;
                    }
                    isInit = true;
                }
                #region SDL调用
                if (sdltexture != IntPtr.Zero)
                {
                    SDL.SDL_DestroyTexture(sdltexture);
                }
                if (sdlrenderer != IntPtr.Zero)
                {
                    SDL.SDL_DestroyRenderer(sdlrenderer);
                }
                if (screen != IntPtr.Zero)
                {
                    SDL.SDL_DestroyWindow(screen);
                    SDL.SDL_RaiseWindow(screen);
                    SDL.SDL_RestoreWindow(screen);
                }
                //创建显示窗口 
                screen = SDL.SDL_CreateWindowFrom(intPtr);
                SDL.SDL_ShowWindow(screen);

                SDL.SDL_SetWindowSize(screen, width, height);
                //screen = SDL.SDL_CreateWindow("SDL EVENT TEST", SDL.SDL_WINDOWPOS_UNDEFINED, SDL.SDL_WINDOWPOS_UNDEFINED, width, height, SDL.SDL_WindowFlags.SDL_WINDOW_OPENGL | SDL.SDL_WindowFlags.SDL_WINDOW_RESIZABLE);
                //screen = SDL.SDL_CreateWindow("SDL EVENT TEST", SDL.SDL_WINDOWPOS_UNDEFINED, SDL.SDL_WINDOWPOS_UNDEFINED, screen_w, screen_h, SDL.SDL_WindowFlags.SDL_WINDOW_OPENGL | SDL.SDL_WindowFlags.SDL_WINDOW_RESIZABLE);
                if (screen == IntPtr.Zero)
                {
                    Console.WriteLine("Can't creat a window:{0}\n", SDL.SDL_GetError());
                    return -1;
                }

                //创建渲染器
                sdlrenderer = SDL.SDL_CreateRenderer(screen, -1, SDL.SDL_RendererFlags.SDL_RENDERER_ACCELERATED);
                //创建纹理 
                sdltexture = SDL.SDL_CreateTexture(sdlrenderer, SDL.SDL_PIXELFORMAT_IYUV, (int)SDL.SDL_TextureAccess.SDL_TEXTUREACCESS_STREAMING, width, height);
                #endregion

                return 0;
            }
        }


        public int SDL_Display(int width, int height, IntPtr pixels, int pixelsSize,
            int pitch)
        {
            lock (this)
            {
                #region SDL 视频数据渲染播放
                //设置纹理的数据
                sdlrect.x = 0;
                sdlrect.y = 0;
                sdlrect.w = width;
                sdlrect.h = height;
                //SDL.SDL_UpdateTexture(sdltexture, ref sdlrect, pixels, pitch);
                SDL.SDL_UpdateTexture(sdltexture, IntPtr.Zero, pixels, pitch);
                //复制纹理信息到渲染器目标
                SDL.SDL_RenderClear(sdltexture);
                //SDL.SDL_Rect srcRect = sdlrect;
                //SDL.SDL_RenderCopy(sdlrenderer, sdltexture, ref srcRect, ref sdlrect);

                SDL.SDL_RenderCopy(sdlrenderer, sdltexture, IntPtr.Zero, IntPtr.Zero);
                //视频渲染显示
                SDL.SDL_RenderPresent(sdlrenderer);
                return 0;
            }


          
            #endregion


         
        }
    }
    public unsafe class SDLAudio
    {
        class aa
        {
            public byte[] pcm;
            public int len;
        }
        int lastIndex = 0;

        private List<aa> data = new List<aa>();

        //private List<byte> data = new List<byte>();
        SDL.SDL_AudioCallback Callback;
        public void PlayAudio(IntPtr pcm, int len)
        {
            lock (this)
            {
                byte[] bts = new byte[len];
                Marshal.Copy(pcm, bts, 0, len);
                data.Add(new aa
                {
                    len = len,
                    pcm = bts
                });
            }

            //SDL.SDL_Delay(10);
        }
        void SDL_AudioCallback(IntPtr userdata, IntPtr stream, int len)
        {
            ////SDL 2.0  
            ////SDL.SDL_RWFromMem(stream, 0, len);
            //if (audio_len == 0)
            //    return;
            //len = (len > audio_len ? audio_len : len);
            if (data.Count == 0)
            {
                for (int i = 0; i < len; i++)
                {
                    ((byte*)stream)[i] = 0;
                }
                return;
            }
            for (int i = 0; i < len; i++)
            {
                if (data[0].len > i)
                {
                    ((byte*)stream)[i] = data[0].pcm[i];
                }
                else
                    ((byte*)stream)[i] = 0;
            }
            data.RemoveAt(0);


           
        }
        public int SDL_Init()
        {
            Callback = SDL_AudioCallback;
            #region SDL调用
            //// 初始化调用SDL.SDL_Init(SDL.SDL_INIT_VIDEO | SDL.SDL_INIT_AUDIO | SDL.SDL_INIT_TIMER)
            //if (SDL.SDL_Init(SDL.SDL_INIT_VIDEO | SDL.SDL_INIT_AUDIO | SDL.SDL_INIT_TIMER) < 0)
            //{
            //    Console.WriteLine("Could not initialize SDL - {0}\n", SDL.SDL_GetError());
            //    return -1;
            //}

            #endregion


            SDL.SDL_AudioSpec wanted_spec = new SDL.SDL_AudioSpec();
            wanted_spec.freq = 8000;
            wanted_spec.format = SDL.AUDIO_S16;
            wanted_spec.channels = 1;
            wanted_spec.silence = 0;
            wanted_spec.samples = 320;
            wanted_spec.callback = Callback;


            if (SDL.SDL_OpenAudio(ref wanted_spec, IntPtr.Zero) < 0)
            {
                Console.WriteLine("can't open audio.");
                return -1;
            }
            //Play  
            SDL.SDL_PauseAudio(0);
            return 0;
        }

    }
   

}
```
SDL实现了基础的播放功能。

#### C# Mp4文件音视频编码器类

```
using CV.Video.Base;
using CV.Video.Base.FFmpeg;
using FFmpeg.AutoGen;
using JX;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace CvNetVideo.Codec.Video
{
    public unsafe class JT1078CodecForMp4
    {
       
        /// <summary>
        /// 指示当前解码是否在运行
        /// </summary>
        public bool IsRun { get; protected set; }
        /// <summary>
        /// 视频线程
        /// </summary>
        private Thread threadVideo;
        /// <summary>
        /// 音频线程
        /// </summary>
        private Thread threadAudio;
        /// <summary>
        /// 退出控制
        /// </summary>
        private bool exit_thread = false;
        /// <summary>
        /// 暂停控制
        /// </summary>
        private bool pause_thread = false;
        /// <summary>
        ///  视频输出流videoindex
        /// </summary>
        private int videoindex = -1;
        /// <summary>
        ///  音频输出流audioindex
        /// </summary>
        private int audioindex = -1;

        /// <summary>
        /// 视频H264转YUV并使用SDL进行播放
        /// </summary>
        /// <param name="fileName"></param>
        /// <param name="sdlVideo"></param>
        /// <returns></returns>
        public unsafe int RunVideo(string fileName,SDLHelper sdlVideo)
        {
            IsRun = true;
            exit_thread = false;
            pause_thread = false;
            threadVideo = Thread.CurrentThread;
            int error, frame_count = 0;
            int got_picture, ret;
            SwsContext* pSwsCtx = null;
            AVFormatContext* ofmt_ctx = null;
            IntPtr convertedFrameBufferPtr = IntPtr.Zero;
            try
            {
                // 注册编解码器
                ffmpeg.avcodec_register_all();

                // 获取文件信息上下文初始化
                ofmt_ctx = ffmpeg.avformat_alloc_context();

                // 打开媒体文件
                error = ffmpeg.avformat_open_input(&ofmt_ctx, fileName, null, null);
                if (error != 0)
                {
                    throw new ApplicationException(FFmpegBinariesHelper.GetErrorMessage(error));
                }

                // 获取流的通道
                for (int i = 0; i < ofmt_ctx->nb_streams; i++)
                {
                    if (ofmt_ctx->streams[i]->codec->codec_type == AVMediaType.AVMEDIA_TYPE_VIDEO)
                    {
                        videoindex = i;
                        Console.WriteLine("video.............."+videoindex);
                    }
                }

                if (videoindex == -1)
                {
                    Console.WriteLine("Couldn't find a video stream.（没有找到视频流）");
                    return -1;
                }

                // 视频流处理
                if (videoindex > -1)
                {
                    //获取视频流中的编解码上下文
                    AVCodecContext* pCodecCtx = ofmt_ctx->streams[videoindex]->codec;

                    //根据编解码上下文中的编码id查找对应的解码
                    AVCodec* pCodec = ffmpeg.avcodec_find_decoder(pCodecCtx->codec_id);
                    if (pCodec == null)
                    {
                        Console.WriteLine("没有找到编码器");
                        return -1;
                    }

                    //打开编码器
                    if (ffmpeg.avcodec_open2(pCodecCtx, pCodec, null) < 0)
                    {
                        Console.WriteLine("编码器无法打开");
                        return -1;
                    }
                    Console.WriteLine("Find a  video stream.channel=" + videoindex);

                    //输出视频信息
                    var format = ofmt_ctx->iformat->name->ToString();
                    var len = (ofmt_ctx->duration) / 1000000;
                    var width = pCodecCtx->width;
                    var height = pCodecCtx->height;
                    Console.WriteLine("video format：" + format);
                    Console.WriteLine("video length：" + len);
                    Console.WriteLine("video width&height：width=" + width + " height=" + height);
                    Console.WriteLine("video codec name：" + pCodec->name->ToString());

                    //准备读取
                    //AVPacket用于存储一帧一帧的压缩数据（H264）
                    //缓冲区，开辟空间
                    AVPacket* packet = (AVPacket*)ffmpeg.av_malloc((ulong)sizeof(AVPacket));

                    //AVFrame用于存储解码后的像素数据(YUV)
                    //内存分配
                    AVFrame* pFrame = ffmpeg.av_frame_alloc();
                    //YUV420
                    AVFrame* pFrameYUV = ffmpeg.av_frame_alloc();
                    //只有指定了AVFrame的像素格式、画面大小才能真正分配内存
                    //缓冲区分配内存
                    int out_buffer_size = ffmpeg.avpicture_get_size(AVPixelFormat.AV_PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height);
                    byte* out_buffer = (byte*)ffmpeg.av_malloc((ulong)out_buffer_size);
                    //初始化缓冲区
                    ffmpeg.avpicture_fill((AVPicture*)pFrameYUV, out_buffer, AVPixelFormat.AV_PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height);

                    //用于转码（缩放）的参数，转之前的宽高，转之后的宽高，格式等
                    SwsContext* sws_ctx = ffmpeg.sws_getContext(pCodecCtx->width, pCodecCtx->height, AVPixelFormat.AV_PIX_FMT_YUV420P /*pCodecCtx->pix_fmt*/, pCodecCtx->width, pCodecCtx->height, AVPixelFormat.AV_PIX_FMT_YUV420P, ffmpeg.SWS_BICUBIC, null, null, null);

                    while (ffmpeg.av_read_frame(ofmt_ctx, packet) >= 0)
                    {
                        // 退出线程
                        if (exit_thread)
                        {
                            break;
                        }
                        // 暂停解析
                        if (pause_thread)
                        {
                            while (pause_thread)
                            {
                                Thread.Sleep(100);
                            }
                        }
                        //只要视频压缩数据（根据流的索引位置判断）
                        if (packet->stream_index == videoindex)
                        {
                            //解码一帧视频压缩数据，得到视频像素数据
                            ret = ffmpeg.avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, packet);
                            if (ret < 0)
                            {
                                Console.WriteLine("视频解码错误");
                                return -1;
                            }

                            // 读取解码后的帧数据
                            if (got_picture>0)
                            {
                                frame_count++;
                                Console.WriteLine("视频帧数:第 " + frame_count + " 帧");

                                //AVFrame转为像素格式YUV420，宽高
                                ffmpeg.sws_scale(sws_ctx, pFrame->data, pFrame->linesize, 0, pCodecCtx->height, pFrameYUV->data, pFrameYUV->linesize);

                                //SDL播放YUV数据
                                var data = out_buffer;
                                sdlVideo.SDL_Display(pCodecCtx->width, pCodecCtx->height, (IntPtr)data, out_buffer_size, pFrameYUV->linesize[0]);
                            }
                        }

                        //释放资源
                        ffmpeg.av_free_packet(packet);
                    }

                }

            }
            catch (Exception ex)
            {
                Console.WriteLine(ex);
            }
            finally
            {
                if (&ofmt_ctx != null)
                {
                    ffmpeg.avformat_close_input(&ofmt_ctx);//关闭流文件 
                }
               
            }
            IsRun = false;
            return 0;
        }

        /// <summary>
        /// 音频AAC转PCM并使用SDL进行播放
        /// </summary>
        /// <param name="fileName"></param>
        /// <param name="sdlAudio"></param>
        /// <returns></returns>
        public unsafe int RunAudio(string fileName, SDLAudio sdlAudio)
        {
            IsRun = true;
            exit_thread = false;
            pause_thread = false;
            threadAudio = Thread.CurrentThread;
            int error, frame_count = 0;
            int got_frame, ret;
            AVFormatContext* ofmt_ctx = null;
            SwsContext* pSwsCtx = null;
            IntPtr convertedFrameBufferPtr = IntPtr.Zero;
            try
            {
                // 注册编解码器
                ffmpeg.avcodec_register_all();

                // 获取文件信息上下文初始化
                ofmt_ctx = ffmpeg.avformat_alloc_context();

                // 打开媒体文件
                error = ffmpeg.avformat_open_input(&ofmt_ctx, fileName, null, null);
                if (error != 0)
                {
                    throw new ApplicationException(FFmpegBinariesHelper.GetErrorMessage(error));
                }

                // 获取流的通道
                for (int i = 0; i < ofmt_ctx->nb_streams; i++)
                {
                    if (ofmt_ctx->streams[i]->codec->codec_type == AVMediaType.AVMEDIA_TYPE_AUDIO)
                    {
                        audioindex = i;
                        Console.WriteLine("audio.............." + audioindex);
                    }
                }

                if (audioindex == -1)
                {
                    Console.WriteLine("Couldn't find a  audio stream.（没有找到音频流）");
                    return -1;
                }

                // 音频流处理
                if (audioindex > -1)
                {
                    //根据索引拿到对应的流,根据流拿到解码器上下文
                    AVCodecContext* pCodeCtx = ofmt_ctx->streams[audioindex]->codec;

                    //再根据上下文拿到编解码id，通过该id拿到解码器
                    AVCodec* pCodec = ffmpeg.avcodec_find_decoder(pCodeCtx->codec_id);
                    if (pCodec == null)
                    {
                        Console.WriteLine("没有找到编码器");
                        return -1;
                    }
                    //打开编码器
                    if (ffmpeg.avcodec_open2(pCodeCtx,pCodec, null)<0)
                    {
                        Console.WriteLine("编码器无法打开");
                        return -1;
                    }
                    Console.WriteLine("Find a  audio stream. channel=" + audioindex);

                    //编码数据
                    AVPacket* packet = (AVPacket*)ffmpeg.av_malloc((ulong)(sizeof(AVPacket)));
                    //解压缩数据
                    AVFrame* frame = ffmpeg.av_frame_alloc();

                    //frame->16bit 44100 PCM 统一音频采样格式与采样率
                    SwrContext* swrCtx = ffmpeg.swr_alloc();
                    //重采样设置选项-----------------------------------------------------------start
                    //输入的采样格式
                    AVSampleFormat in_sample_fmt = pCodeCtx->sample_fmt;
                    //输出的采样格式 16bit PCM
                    AVSampleFormat out_sample_fmt = AVSampleFormat.AV_SAMPLE_FMT_S16;
                    //输入的采样率
                    int in_sample_rate = pCodeCtx->sample_rate;
                    //输出的采样率
                    int out_sample_rate = 44100;
                    //输入的声道布局
                    long in_ch_layout = (long)pCodeCtx->channel_layout;
                    //输出的声道布局
                    int out_ch_layout = ffmpeg.AV_CH_LAYOUT_MONO;

                    ffmpeg.swr_alloc_set_opts(swrCtx, out_ch_layout, out_sample_fmt, out_sample_rate, in_ch_layout, in_sample_fmt, in_sample_rate, 0, null);
                    ffmpeg.swr_init(swrCtx);
                    //重采样设置选项-----------------------------------------------------------end
                    //获取输出的声道个数
                    int out_channel_nb = ffmpeg.av_get_channel_layout_nb_channels((ulong)out_ch_layout);
                    //存储pcm数据
                    byte* out_buffer = (byte*)ffmpeg.av_malloc(2 * 44100);
                  
                    //一帧一帧读取压缩的音频数据AVPacket
                    while (ffmpeg.av_read_frame(ofmt_ctx, packet) >= 0)
                    {
                        // 退出线程
                        if (exit_thread)
                        {
                            break;
                        }
                        // 暂停解析
                        if (pause_thread)
                        {
                            while (pause_thread)
                            {
                                Thread.Sleep(100);
                            }
                        }
                        if (packet->stream_index == audioindex)
                        {
                            //解码AVPacket->AVFrame
                            ret = ffmpeg.avcodec_decode_audio4(pCodeCtx, frame, &got_frame, packet);
                            if (ret < 0)
                            {
                                Console.WriteLine("音频解码失败");
                                return -1;
                            }
                            // 读取帧数据
                            if (got_frame>0)
                            {
                                frame_count++;
                                Console.WriteLine("音频帧数:第 "+ frame_count + " 帧");
                                var data_ = frame->data;
                                ffmpeg.swr_convert(swrCtx, &out_buffer, 2 * 44100,(byte**)&data_, frame->nb_samples);
                                //获取sample的size
                                int out_buffer_size = ffmpeg.av_samples_get_buffer_size(null, out_channel_nb, frame->nb_samples, out_sample_fmt, 1);
                                //写入文件进行测试
                                var data=out_buffer;
                                sdlAudio.PlayAudio((IntPtr)data, out_buffer_size);
                            }
                        }
                        ffmpeg.av_free_packet(packet);
                    }

                }

            }
            catch (Exception ex)
            {
                Console.WriteLine(ex);
            }
            finally
            {
                if (&ofmt_ctx != null)
                {
                    ffmpeg.avformat_close_input(&ofmt_ctx);//关闭流文件 
                }

            }
            IsRun = false;
            return 0;
        }


        /// <summary>
        /// 开启线程
        /// </summary>
        /// <param name="fileName"></param>
        /// <param name="sdlVideo"></param>
        public void Start(string fileName, SDLHelper sdlVideo,SDLAudio sdlAudio)
        {
            // 视频线程
            threadVideo = new Thread(() =>
              {
                  try
                  {
                      RunVideo(fileName, sdlVideo);
                  }
                  catch (Exception ex)
                  {
                      SQ.Base.ErrorLog.WriteLog4Ex("JT1078CodecForMp4.Run Video", ex);
                  }
              });
            threadVideo.IsBackground = true;
            threadVideo.Start();

            // 音频线程
            threadAudio = new Thread(() =>
            {
                try
                {
                    RunAudio(fileName, sdlAudio);
                }
                catch (Exception ex)
                {
                    SQ.Base.ErrorLog.WriteLog4Ex("JT1078CodecForMp4.Run Audio", ex);
                }
            });
            threadAudio.IsBackground = true;
            threadAudio.Start();
        }

        /// <summary>
        /// 暂停继续
        /// </summary>
        public void GoOn()
        {
            pause_thread = false;

        }

        /// <summary>
        /// 暂停
        /// </summary>
        public void Pause()
        {
            pause_thread = true;
        }

        /// <summary>
        /// 停止
        /// </summary>
        public void Stop()
        {
            exit_thread = true;
        }
    }
}
```

暂停、继续、停止在此处的意义不大，因为解析的速度很快。


#### 测试代码及效果图

```
/// <summary>
        /// 播放
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnPlay_Click(object sender, EventArgs e)
        {
            // 音视频媒体文件路径
            string fileName = "test.mp4";// 表示${Project_home}/bin/Debug/test.mp4
            // 线程读取音视频流
            jt1078CodecForMp4 = new JT1078CodecForMp4();
            jt1078CodecForMp4.Start(fileName,sdlVideo,sdlAudio);
        }
```




![](https://img-blog.csdn.net/20180308142628129)


注意：此处出现绿色，是不正常的。修改播放方法的数据设置方式：

```
/// <summary>
        /// 播放视频
        /// </summary>
        /// <param name="width"></param>
        /// <param name="height"></param>
        /// <param name="pixels"></param>
        /// <param name="pixelsSize"></param>
        /// <param name="pitch"></param>
        /// <returns></returns>
        public int SDL_Display(int width, int height, IntPtr pixels, int pixelsSize,
            int pitch)
        {
            lock (this)
            {
                while (isPause)
                {
                    SDL.SDL_Delay(20);//延迟播放
                }

                #region SDL 视频数据渲染播放
                //设置纹理的数据
                sdlrect.x = 0;
                sdlrect.y = 0;
                sdlrect.w = width;
                sdlrect.h = height;
                SDL.SDL_UpdateTexture(sdltexture, ref sdlrect, pixels, pitch);
                //SDL.SDL_UpdateTexture(sdltexture, IntPtr.Zero, pixels, pitch);//此处代码导致播放窗口绿色阴影
                //复制纹理信息到渲染器目标
                SDL.SDL_RenderClear(sdltexture);
                //SDL.SDL_Rect srcRect = sdlrect;
                //SDL.SDL_RenderCopy(sdlrenderer, sdltexture, ref srcRect, ref sdlrect);

                SDL.SDL_RenderCopy(sdlrenderer, sdltexture, IntPtr.Zero, IntPtr.Zero);
                //视频渲染显示
                SDL.SDL_RenderPresent(sdlrenderer);
                //SDL.SDL_Delay(40);
                //SDL.SDL_PollEvent(out sdlevent);
                //switch (sdlevent.type)
                //{
                //    case SDL.SDL_EventType.SDL_QUIT:
                //        SDL.SDL_Quit();
                //        return -1;
                //    default:
                //        break;
                //}
                return 0;
            }


            //SDL.SDL_RenderClear(sdlrenderer);
            //SDL.SDL_RenderCopy(sdlrenderer, sdltexture, ref srcRect, ref sdlrect);
            //SDL.SDL_RenderPresent(sdlrenderer);
            ////Delay 40ms  
            //SDL.SDL_Delay(40);
            #endregion


            //#region SDL 视频数据渲染播放
            //////设置纹理的数据
            ////sdlrect.x = 0;
            ////sdlrect.y = 0;
            ////sdlrect.w = width;
            ////sdlrect.h = height;
            ////SDL.SDL_UpdateTexture(sdltexture, ref sdlrect, pixels, pitch);
            //////复制纹理信息到渲染器目标
            ////SDL.SDL_Rect srcRect = sdlrect;
            ////SDL.SDL_RenderCopy(sdlrenderer, sdltexture, ref srcRect, ref sdlrect);
            //////视频渲染显示
            ////SDL.SDL_RenderPresent(sdlrenderer);
            //////SDL.SDL_Delay(40);
            ////SDL.SDL_PollEvent(out sdlevent);
            ////switch (sdlevent.type)
            ////{
            ////    case SDL.SDL_EventType.SDL_QUIT:
            ////        SDL.SDL_Quit();
            ////        return -1;
            ////    default:
            ////        break;
            ////}
            ////return 0;
            //#endregion
        }
    }
```

关键代码：



```
SDL.SDL_UpdateTexture(sdltexture, ref sdlrect, pixels, pitch);
                //SDL.SDL_UpdateTexture(sdltexture, IntPtr.Zero, pixels, pitch);//此处代码导致播放窗口绿色阴影
```

修改后效果：

![](https://img-blog.csdn.net/20180312161145586)


代码改进，采用同一个线程播放音视频：

```
/// <summary>
    /// MP4播放(音视频使用同一个线程)
    /// </summary>
    public unsafe class JT1078CodecToPlayMp4Two
    {

        /// <summary>
        /// 指示当前解码是否在运行
        /// </summary>
        public bool IsRun { get; protected set; }
        /// <summary>
        /// 当前线程
        /// </summary>
        private Thread thread;
        /// <summary>
        /// 退出控制
        /// </summary>
        private bool exit_thread = false;
        /// <summary>
        /// 暂停控制
        /// </summary>
        private bool pause_thread = false;
        /// <summary>
        ///  视频输出流videoindex
        /// </summary>
        private int videoindex = -1;
        /// <summary>
        ///  音频输出流audioindex
        /// </summary>
        private int audioindex = -1;

        private bool isInit = false;


        int error;
        AVFormatContext* ofmt_ctx = null;
        AVPacket* packet;
        AVCodecContext* pCodecCtx_Video;
        AVCodec* pCodec_Video;
        AVFrame* pFrame_Video;
        AVFrame* pFrameYUV_Video;
        SwsContext* sws_ctx_video;
        SDLHelper sdlVideo;
        SDLAudio sdlAudio;

        int out_buffer_size_video;
        byte* out_buffer_video;
        int video_frame_count, audio_frame_count;


        AVCodecContext* pCodeCtx_Audio;
        AVCodec* pCodec_Audio;
        AVFrame* frame_Audio;
        SwrContext* swrCtx_Audio;

        byte* out_buffer_audio;
        int out_buffer_size_audio;
        int out_channel_nb;
        AVSampleFormat out_sample_fmt;

        /// <summary>
        /// 初始化
        /// </summary>
        /// <param name="fileName"></param>
        /// <param name="sdlVideo"></param>
        /// <param name="sdlAudio"></param>
        /// <returns></returns>
        public int Init(string fileName, SDLHelper sdlVideo, SDLAudio sdlAudio)
        {
            AVFormatContext* ofmt_ctx;

            // 注册编解码器
            ffmpeg.avcodec_register_all();

            // 获取文件信息上下文初始化
            ofmt_ctx = ffmpeg.avformat_alloc_context();
            this.ofmt_ctx = ofmt_ctx;

            // 打开媒体文件
            error = ffmpeg.avformat_open_input(&ofmt_ctx, fileName, null, null);
            if (error != 0)
            {
                throw new ApplicationException(FFmpegBinariesHelper.GetErrorMessage(error));
            }

            // 获取流的通道
            for (int i = 0; i < ofmt_ctx->nb_streams; i++)
            {
                if (ofmt_ctx->streams[i]->codec->codec_type == AVMediaType.AVMEDIA_TYPE_VIDEO)
                {
                    videoindex = i;
                    Console.WriteLine("video.............." + videoindex);
                }
                if (ofmt_ctx->streams[i]->codec->codec_type == AVMediaType.AVMEDIA_TYPE_AUDIO)
                {
                    audioindex = i;
                    Console.WriteLine("audio.............." + audioindex);
                }
            }

            if (videoindex == -1)
            {
                Console.WriteLine("Couldn't find a video stream.（没有找到视频流）");
                return -1;
            }

            if (audioindex == -1)
            {
                Console.WriteLine("Couldn't find a  audio stream.（没有找到音频流）");
                return -1;
            }

            #region 初始化视频

            // 视频流处理
            if (videoindex > -1)
            {
                //获取视频流中的编解码上下文
                pCodecCtx_Video = ofmt_ctx->streams[videoindex]->codec;

                //根据编解码上下文中的编码id查找对应的解码
                pCodec_Video = ffmpeg.avcodec_find_decoder(pCodecCtx_Video->codec_id);
                if (pCodec_Video == null)
                {
                    Console.WriteLine("没有找到编码器");
                    return -1;
                }

                //打开编码器
                if (ffmpeg.avcodec_open2(pCodecCtx_Video, pCodec_Video, null) < 0)
                {
                    Console.WriteLine("编码器无法打开");
                    return -1;
                }
                Console.WriteLine("Find a  video stream.channel=" + videoindex);

                //输出视频信息
                var format = ofmt_ctx->iformat->name->ToString();
                var len = (ofmt_ctx->duration) / 1000000;
                var width = pCodecCtx_Video->width;
                var height = pCodecCtx_Video->height;
                Console.WriteLine("video format：" + format);
                Console.WriteLine("video length：" + len);
                Console.WriteLine("video width&height：width=" + width + " height=" + height);
                Console.WriteLine("video codec name：" + pCodec_Video->name->ToString());

                //准备读取
                //AVPacket用于存储一帧一帧的压缩数据（H264）

                //AVFrame用于存储解码后的像素数据(YUV)
                //内存分配
                pFrame_Video = ffmpeg.av_frame_alloc();
                //YUV420
                pFrameYUV_Video = ffmpeg.av_frame_alloc();
                //只有指定了AVFrame的像素格式、画面大小才能真正分配内存
                //缓冲区分配内存
                out_buffer_size_video = ffmpeg.avpicture_get_size(AVPixelFormat.AV_PIX_FMT_YUV420P, pCodecCtx_Video->width, pCodecCtx_Video->height);
                out_buffer_video = (byte*)ffmpeg.av_malloc((ulong)out_buffer_size_video);
                //初始化缓冲区
                ffmpeg.avpicture_fill((AVPicture*)pFrameYUV_Video, out_buffer_video, AVPixelFormat.AV_PIX_FMT_YUV420P, pCodecCtx_Video->width, pCodecCtx_Video->height);
                //用于转码（缩放）的参数，转之前的宽高，转之后的宽高，格式等
                sws_ctx_video = ffmpeg.sws_getContext(pCodecCtx_Video->width, pCodecCtx_Video->height, AVPixelFormat.AV_PIX_FMT_YUV420P /*pCodecCtx->pix_fmt*/, pCodecCtx_Video->width, pCodecCtx_Video->height, AVPixelFormat.AV_PIX_FMT_YUV420P, ffmpeg.SWS_BICUBIC, null, null, null);
            }
            #endregion

            #region 初始化音频
            // 音频流处理
            if (audioindex > -1)
            {
                //根据索引拿到对应的流,根据流拿到解码器上下文
                pCodeCtx_Audio = ofmt_ctx->streams[audioindex]->codec;

                //再根据上下文拿到编解码id，通过该id拿到解码器
                pCodec_Audio = ffmpeg.avcodec_find_decoder(pCodeCtx_Audio->codec_id);
                if (pCodec_Audio == null)
                {
                    Console.WriteLine("没有找到编码器");
                    return -1;
                }
                //打开编码器
                if (ffmpeg.avcodec_open2(pCodeCtx_Audio, pCodec_Audio, null) < 0)
                {
                    Console.WriteLine("编码器无法打开");
                    return -1;
                }
                Console.WriteLine("Find a  audio stream. channel=" + audioindex);

                //解压缩数据
                frame_Audio = ffmpeg.av_frame_alloc();

                //frame->16bit 44100 PCM 统一音频采样格式与采样率
                swrCtx_Audio = ffmpeg.swr_alloc();
                //重采样设置选项-----------------------------------------------------------start
                //输入的采样格式
                AVSampleFormat in_sample_fmt = pCodeCtx_Audio->sample_fmt;
                //输出的采样格式 16bit PCM
                out_sample_fmt = AVSampleFormat.AV_SAMPLE_FMT_S16;
                //输入的采样率
                int in_sample_rate = pCodeCtx_Audio->sample_rate;
                //输出的采样率
                int out_sample_rate = 44100;
                //输入的声道布局
                long in_ch_layout = (long)pCodeCtx_Audio->channel_layout;
                //输出的声道布局
                int out_ch_layout = ffmpeg.AV_CH_LAYOUT_MONO;

                ffmpeg.swr_alloc_set_opts(swrCtx_Audio, out_ch_layout, out_sample_fmt, out_sample_rate, in_ch_layout, in_sample_fmt, in_sample_rate, 0, null);
                ffmpeg.swr_init(swrCtx_Audio);
                //重采样设置选项-----------------------------------------------------------end
                //获取输出的声道个数
                out_channel_nb = ffmpeg.av_get_channel_layout_nb_channels((ulong)out_ch_layout);
                //存储pcm数据
                out_buffer_audio = (byte*)ffmpeg.av_malloc(2 * 44100);
            }
            #endregion

            //缓冲区，开辟空间
            packet = (AVPacket*)ffmpeg.av_malloc((ulong)sizeof(AVPacket));

            // 设置SDL播放对象
            this.sdlVideo = sdlVideo;
            this.sdlAudio = sdlAudio;

            isInit = true;

            return 0;
        }


        /// <summary>
        /// 读取音视频流文件并进行播放
        /// </summary>
        public unsafe int ReadAndPlay()
        {
            IsRun = true;
            exit_thread = false;
            pause_thread = false;
            thread = Thread.CurrentThread;
            //int error, frame_count = 0;
            int got_frame, ret;
            //SwsContext* pSwsCtx = null;

            byte* out_audio_buffer = out_buffer_audio;

            try
            {

                while (ffmpeg.av_read_frame(ofmt_ctx, packet) >= 0)
                {

                    // 退出线程
                    if (exit_thread)
                    {
                        break;
                    }
                    // 暂停解析
                    while (pause_thread)
                    {
                        Thread.Sleep(100);
                    }
                    #region 视频H264转YUV并使用SDL进行播放
                    if (packet->stream_index == videoindex)
                    {
                        //解码一帧视频压缩数据，得到视频像素数据
                        ret = ffmpeg.avcodec_decode_video2(pCodecCtx_Video, pFrame_Video, &got_frame, packet);
                        if (ret < 0)
                        {
                            Console.WriteLine("视频解码错误");
                            return -1;
                        }

                        // 读取解码后的帧数据
                        if (got_frame > 0)
                        {
                            double pts = 0; //ffmpeg.av_frame_get_best_effort_timestamp(pFrameYUV_Video);
                            //VideoState* vs = null;
                            //vs->video_clock = pts;
                            //vs->video_st = ofmt_ctx->streams[videoindex];
                            //pts = synchronize_video(vs, pFrame_Video, pts);
                            //if (queue_picture(is, pFrame, pts) < 0)
                            //{
                            //    break;
                            //}
                            video_frame_count++;
                            //存在问题的PTS计算

                            //int pts = video_frame_count++ * (pCodecCtx_Video->pkt_timebase.num * 1000 / 25 /* pCodecCtx->pkt_timebase.den*/);

                            Console.WriteLine("视频帧数:第 " + video_frame_count + " 帧");

                            //AVFrame转为像素格式YUV420，宽高
                            ffmpeg.sws_scale(sws_ctx_video, pFrame_Video->data, pFrame_Video->linesize, 0, pCodecCtx_Video->height, pFrameYUV_Video->data, pFrameYUV_Video->linesize);

                            Console.WriteLine("视频: pts= " + packet->pts + " dts=" + packet->dts);

                            // SDL播放YUV数据：下面两种方式都可以进行播放
                            sdlVideo.SDL_Display(pCodecCtx_Video->width, pCodecCtx_Video->height, (IntPtr)out_buffer_video, out_buffer_size_video, pFrameYUV_Video->linesize[0]);
                            //sdlVideo.SDL_Display(pCodecCtx_Video->width, pCodecCtx_Video->height, (IntPtr)pFrameYUV_Video->data[0], out_buffer_size_video, pFrameYUV_Video->linesize[0]);

                            //DeleyToPlay_Video(packet->pts);
                        }
                    }
                    #endregion

                    #region 音频AAC转PCM并使用SDL进行播放
                    if (packet->stream_index == audioindex)
                    {
                        //解码AVPacket->AVFrame
                        ret = ffmpeg.avcodec_decode_audio4(pCodeCtx_Audio, frame_Audio, &got_frame, packet);
                        if (ret < 0)
                        {
                            Console.WriteLine("音频解码失败");
                            return -1;
                        }
                        // 读取帧数据
                        if (got_frame > 0)
                        {
                            audio_frame_count++;
                            Console.WriteLine("音频帧数:第 " + audio_frame_count + " 帧");
                            // 变换音频
                            ffmpeg.swr_convert(swrCtx_Audio, &out_audio_buffer, 2 * 44100, (byte**)&frame_Audio->data, frame_Audio->nb_samples);

                            // 获取sample的size
                            out_buffer_size_audio = ffmpeg.av_samples_get_buffer_size(null, out_channel_nb, frame_Audio->nb_samples, out_sample_fmt, 1);

                            Console.WriteLine("音频: pts= " + packet->pts + " dts=" + packet->dts);

                            // SDL进行音频播放
                            sdlAudio.PlayAudio((IntPtr)out_audio_buffer, out_buffer_size_audio);

                            //DeleyToPlay_Audio(packet->pts);

                        }
                    }
                    #endregion
                    Thread.Sleep(20);
                    //释放资源
                    ffmpeg.av_free_packet(packet);
                }


            }
            catch (Exception ex)
            {
                Console.WriteLine(ex);
            }
            finally
            {
                //if (&ofmt_ctx != null)
                //{
                //    ffmpeg.avformat_close_input(&ofmt_ctx);//关闭流文件 
                //}

            }
            IsRun = false;
            return 0;
        }

        /// <summary>
        /// 开启线程
        /// </summary>
        /// <param name="fileName"></param>
        /// <param name="sdlVideo"></param>
        /// <param name="sdlAudio"></param>
        public void Start()
        {
            if (!isInit)
            {
                MessageBox.Show("没有初始化");
            }
            thread = new Thread(() =>
            {
                try
                {
                    ReadAndPlay();
                }
                catch (Exception ex)
                {
                    SQ.Base.ErrorLog.WriteLog4Ex("JT1078CodecForMp4.Run Video", ex);
                }
            });
            thread.IsBackground = true;
            thread.Start();


        }

        /// <summary>
        /// 暂停继续
        /// </summary>
        public void GoOnPlay()
        {
            pause_thread = false;
            sdlVideo.PlayVideo();
            sdlAudio.PlayAudio();
        }

        /// <summary>
        /// 暂停
        /// </summary>
        public void Pause()
        {
            pause_thread = true;
            sdlVideo.PauseVideo();
            sdlAudio.PauseAudio();
        }

        /// <summary>
        /// 停止
        /// </summary>
        public void Stop()
        {
            exit_thread = true;
        }

        long lastPts_Video = 0;
        DateTime lastTS_Video;

        long lastPts_Audio = 0;
        DateTime lastTS_Audio;

        private void DeleyToPlay_Video(long pts)
        {
            if (lastPts_Video > 0 && lastTS_Video != null)
            {
                double delay = (DateTime.Now - lastTS_Video).TotalMilliseconds;
                var i = (int)(pts - lastPts_Video - delay);
                if (i >= 1)
                {
                    Thread.Sleep(i);
                }
            }
            lastTS_Video = DateTime.Now;
            lastPts_Video = pts;
        }

        private void DeleyToPlay_Audio(long pts)
        {
            if (lastPts_Audio > 0 && lastTS_Audio != null)
            {
                double delay = (DateTime.Now - lastTS_Audio).TotalMilliseconds;
                var i = (int)(pts - lastPts_Audio - delay);
                if (i >= 1)
                {
                    Thread.Sleep(i);
                }
            }
            lastTS_Audio = DateTime.Now;
            lastPts_Audio = pts;
        }

        ////# http://dranger.com/ffmpeg/tutorial05.html
        //public struct VideoState
        //{
        //    public double video_clock; // pts of last decoded frame / predicted pts of next decoded frame

        //    public AVStream* video_st;// video stream
        //}

        //public unsafe double synchronize_video(VideoState* vs, AVFrame* src_frame, double pts)
        //{

        //    double frame_delay;

        //    if (pts != 0)
        //    {
        //        /* if we have pts, set video clock to it */
        //        vs->video_clock = pts;
        //    }
        //    else
        //    {
        //        /* if we aren't given a pts, set it to the clock */
        //        pts = vs->video_clock;
        //    }
        //    /* update the video clock */
        //    frame_delay = av_q2d(vs->video_st->codec->time_base);
        //    /* if we are repeating a frame, adjust clock accordingly */
        //    frame_delay += src_frame->repeat_pict * (frame_delay * 0.5);
        //    vs->video_clock += frame_delay;
        //    return pts;
        //}

        //struct VideoPicture
        //{
        //    double pts;
        //}
        //int queue_picture(VideoState* vs, AVFrame* pFrame, double pts)
        //{
        //    if (vp->bmp)
        //    {
        //    ... convert picture ...
        //         vp->pts = pts;
        //    ... alert queue ...
        //  }

        //}
    }
```

##### 解决音视频同步问题版本

```
using CV.Media.Utils.Filter;
using CV.Video.Base;
using CV.Video.Base.FFmpeg;
using FFmpeg.AutoGen;
using JX;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using static CvNetVideo.UCVideo;

namespace CvNetVideo.Codec.Video
{

    /// <summary>
    /// MP4播放(音视频使用同一个线程)
    /// </summary>
    public unsafe class JT1078CodecToPlayMp4
    {

        /// <summary>
        /// 指示当前解码是否在运行
        /// </summary>
        public bool IsRun { get; protected set; }
        /// <summary>
        /// 指示当前解码是否在暂停
        /// </summary>
        public bool IsPause { get; protected set; }
        /// <summary>
        /// 当前线程
        /// </summary>
        public Thread thread;
        /// <summary>
        /// 退出控制
        /// </summary>
        private bool exit_thread = false;
        /// <summary>
        /// 暂停控制
        /// </summary>
        private bool pause_thread = false;
        /// <summary>
        ///  视频输出流videoindex
        /// </summary>
        private int videoindex = -1;
        /// <summary>
        ///  音频输出流audioindex
        /// </summary>
        private int audioindex = -1;
        /// <summary>
        /// 是否初始化
        /// </summary>
        private bool isInit = false;


        int error;
        AVFormatContext* ofmt_ctx = null;
        AVPacket* packet;
        AVCodecContext* pCodecCtx_Video;
        AVCodec* pCodec_Video;
        AVFrame* pFrame_Video;
        AVFrame* pFrameYUV_Video;
        SwsContext* sws_ctx_video;
        SDLHelper sdlVideo;
        SDLAudio sdlAudio;

        int out_buffer_size_video;
        byte* out_buffer_video;
        int video_frame_count, audio_frame_count;


        AVCodecContext* pCodeCtx_Audio;
        AVCodec* pCodec_Audio;
        AVFrame* frame_Audio;
        SwrContext* swrCtx_Audio;

        byte* out_buffer_audio;
        int out_buffer_size_audio;
        int out_channel_nb;
        AVSampleFormat out_sample_fmt;

        int contrast;// 对比度
        int brightness;// 亮度
        int contrast_last;// 对比度
        int brightness_last;// 亮度

        //对比度亮度
        private VideoFiltering m_video_filtering = new VideoFiltering();

        /// <summary>
        /// 设置图像对比度和亮度
        /// </summary>
        /// <param name="contrast"></param>
        /// <param name="brightness"></param>
        /// <returns></returns>
        public void SetContrastAndBrightness(int contrast, int brightness)
        {
            this.contrast = contrast;
            this.brightness = brightness;
        }
        /// <summary>
        /// YUV宽度
        /// </summary>
        public int YuvWidth { get; set; }
        /// <summary>
        /// YUV高度
        /// </summary>
        public int YuvHeight { get; set; }

        /// <summary>
        /// 记录上一帧数据
        /// </summary>
        List<AVVideo> list = new List<AVVideo>();

        /// <summary>
        /// 初始化
        /// </summary>
        /// <param name="fileName"></param>
        /// <param name="sdlVideo"></param>
        /// <param name="sdlAudio"></param>
        /// <returns></returns>
        public int Init(string fileName, SDLHelper sdlVideo, SDLAudio sdlAudio)
        {
            AVFormatContext* ofmt_ctx;

            // 注册编解码器
            ffmpeg.avcodec_register_all();

            // 获取文件信息上下文初始化
            ofmt_ctx = ffmpeg.avformat_alloc_context();
            this.ofmt_ctx = ofmt_ctx;

            // 打开媒体文件
            error = ffmpeg.avformat_open_input(&ofmt_ctx, fileName, null, null);
            if (error != 0)
            {
                throw new ApplicationException(FFmpegBinariesHelper.GetErrorMessage(error));
            }

            // 获取流的通道
            for (int i = 0; i < ofmt_ctx->nb_streams; i++)
            {
                if (ofmt_ctx->streams[i]->codec->codec_type == AVMediaType.AVMEDIA_TYPE_VIDEO)
                {
                    videoindex = i;
                    Console.WriteLine("video.............." + videoindex);
                }
                if (ofmt_ctx->streams[i]->codec->codec_type == AVMediaType.AVMEDIA_TYPE_AUDIO)
                {
                    audioindex = i;
                    Console.WriteLine("audio.............." + audioindex);
                }
            }

            if (videoindex == -1)
            {
                Console.WriteLine("Couldn't find a video stream.（没有找到视频流）");
                return -1;
            }

            if (audioindex == -1)
            {
                Console.WriteLine("Couldn't find a  audio stream.（没有找到音频流）");
                return -1;
            }

            #region 初始化视频

            // 视频流处理
            if (videoindex > -1)
            {
                //获取视频流中的编解码上下文
                pCodecCtx_Video = ofmt_ctx->streams[videoindex]->codec;

                //根据编解码上下文中的编码id查找对应的解码
                pCodec_Video = ffmpeg.avcodec_find_decoder(pCodecCtx_Video->codec_id);
                if (pCodec_Video == null)
                {
                    Console.WriteLine("没有找到编码器");
                    return -1;
                }

                //打开编码器
                if (ffmpeg.avcodec_open2(pCodecCtx_Video, pCodec_Video, null) < 0)
                {
                    Console.WriteLine("编码器无法打开");
                    return -1;
                }
                Console.WriteLine("Find a  video stream.channel=" + videoindex);

                //输出视频信息
                var format = ofmt_ctx->iformat->name->ToString();
                var len = (ofmt_ctx->duration) / 1000000;
                var width = pCodecCtx_Video->width;
                var height = pCodecCtx_Video->height;
                Console.WriteLine("video format：" + format);
                Console.WriteLine("video length：" + len);
                Console.WriteLine("video width&height：width=" + width + " height=" + height);
                Console.WriteLine("video codec name：" + pCodec_Video->name->ToString());

                //准备读取
                //AVPacket用于存储一帧一帧的压缩数据（H264）

                //AVFrame用于存储解码后的像素数据(YUV)
                //内存分配
                pFrame_Video = ffmpeg.av_frame_alloc();
                //YUV420
                pFrameYUV_Video = ffmpeg.av_frame_alloc();
                //只有指定了AVFrame的像素格式、画面大小才能真正分配内存
                //缓冲区分配内存
                out_buffer_size_video = ffmpeg.avpicture_get_size(AVPixelFormat.AV_PIX_FMT_YUV420P, pCodecCtx_Video->width, pCodecCtx_Video->height);
                out_buffer_video = (byte*)ffmpeg.av_malloc((ulong)out_buffer_size_video);
                //初始化缓冲区
                ffmpeg.avpicture_fill((AVPicture*)pFrameYUV_Video, out_buffer_video, AVPixelFormat.AV_PIX_FMT_YUV420P, pCodecCtx_Video->width, pCodecCtx_Video->height);
                //用于转码（缩放）的参数，转之前的宽高，转之后的宽高，格式等
                sws_ctx_video = ffmpeg.sws_getContext(pCodecCtx_Video->width, pCodecCtx_Video->height, AVPixelFormat.AV_PIX_FMT_YUV420P /*pCodecCtx->pix_fmt*/, pCodecCtx_Video->width, pCodecCtx_Video->height, AVPixelFormat.AV_PIX_FMT_YUV420P, ffmpeg.SWS_BICUBIC, null, null, null);
            }
            #endregion

            #region 初始化音频
            // 音频流处理
            if (audioindex > -1)
            {
                //根据索引拿到对应的流,根据流拿到解码器上下文
                pCodeCtx_Audio = ofmt_ctx->streams[audioindex]->codec;

                //再根据上下文拿到编解码id，通过该id拿到解码器
                pCodec_Audio = ffmpeg.avcodec_find_decoder(pCodeCtx_Audio->codec_id);
                if (pCodec_Audio == null)
                {
                    Console.WriteLine("没有找到编码器");
                    return -1;
                }
                //打开编码器
                if (ffmpeg.avcodec_open2(pCodeCtx_Audio, pCodec_Audio, null) < 0)
                {
                    Console.WriteLine("编码器无法打开");
                    return -1;
                }
                Console.WriteLine("Find a  audio stream. channel=" + audioindex);

                //解压缩数据
                frame_Audio = ffmpeg.av_frame_alloc();

                //frame->16bit 8000 PCM 统一音频采样格式与采样率
                swrCtx_Audio = ffmpeg.swr_alloc();
                //重采样设置选项-----------------------------------------------------------start
                //输入的采样格式
                AVSampleFormat in_sample_fmt = pCodeCtx_Audio->sample_fmt;
                //输出的采样格式 16bit PCM
                out_sample_fmt = AVSampleFormat.AV_SAMPLE_FMT_S16;
                //输入的采样率
                int in_sample_rate = pCodeCtx_Audio->sample_rate;
                //输出的采样率
                int out_sample_rate = 8000;
                //输入的声道布局
                long in_ch_layout = (long)pCodeCtx_Audio->channel_layout;
                //输出的声道布局
                int out_ch_layout = ffmpeg.AV_CH_LAYOUT_MONO;

                ffmpeg.swr_alloc_set_opts(swrCtx_Audio, out_ch_layout, out_sample_fmt, out_sample_rate, in_ch_layout, in_sample_fmt, in_sample_rate, 0, null);
                ffmpeg.swr_init(swrCtx_Audio);
                //重采样设置选项-----------------------------------------------------------end
                //获取输出的声道个数
                out_channel_nb = ffmpeg.av_get_channel_layout_nb_channels((ulong)out_ch_layout);
                //存储pcm数据
                out_buffer_audio = (byte*)ffmpeg.av_malloc(2 * 8000);
            }
            #endregion

            //缓冲区，开辟空间
            packet = (AVPacket*)ffmpeg.av_malloc((ulong)sizeof(AVPacket));

            // 设置SDL播放对象
            this.sdlVideo = sdlVideo;
            this.sdlAudio = sdlAudio;

            isInit = true;

            return 0;
        }


        /// <summary>
        /// 读取音视频流文件并进行播放
        /// </summary>
        public unsafe int ReadAndPlay(PlayFinishedDo playFinishedDo)
        {
            IsRun = true;
            exit_thread = false;
            pause_thread = false;
            thread = Thread.CurrentThread;
            //int error, frame_count = 0;
            int got_frame, ret;
            //SwsContext* pSwsCtx = null;

            byte* out_audio_buffer = out_buffer_audio;

            try
            {
                AVStream* video_stream = ofmt_ctx->streams[videoindex];

                while (ffmpeg.av_read_frame(ofmt_ctx, packet) >= 0&& !exit_thread)
                {
                    // 暂停解析
                    while (pause_thread||isLastFrame)
                    {
                        // 退出线程
                        if (exit_thread)
                        {
                            break;
                        }
                        Thread.Sleep(10);
                    }
                    // 退出线程
                    if (exit_thread)
                    {
                        break;
                    }

                    // 此处记录视频的第一帧和第一帧的开始时间
                    if (firstPts == -1 && packet->stream_index == videoindex)
                    {
                        firstPts = packet->pts * 1000 / (video_stream->time_base.den / video_stream->time_base.num);
                        startTS = DateTime.Now;
                    }
                    // 针对视频做延时播放，音频自然播放就行不做处理
                    if (packet->stream_index == videoindex)
                    {
                        long pts_1 = packet->pts * 1000 / (video_stream->time_base.den / video_stream->time_base.num);
                        DeleyToPlay(pts_1);
                    }

                    #region 视频H264转YUV并使用SDL进行播放
                    if (packet->stream_index == videoindex)
                    {
                        //解码一帧视频压缩数据，得到视频像素数据
                        ret = ffmpeg.avcodec_decode_video2(pCodecCtx_Video, pFrame_Video, &got_frame, packet);
                        if (ret < 0)
                        {
                            Console.WriteLine("视频解码错误");
                            return -1;
                        }

                        //滤波,亮度，对比度===参考JT1078ToYuv -----------开始
                        int width = pCodecCtx_Video->width;
                        int height = pCodecCtx_Video->height;
                        if (contrast != contrast_last || brightness != brightness_last)
                        {
                            m_video_filtering.Reset(width, height, contrast, brightness);
                            contrast_last = contrast;
                            brightness_last = brightness;
                        }

                        //滤波,亮度，对比度===参考JT1078ToYuv -----------结束

                        // 读取解码后的帧数据
                        if (got_frame > 0)
                        {
                            video_frame_count++;
                            //>>>>滤波,亮度，对比度===参考JT1078ToYuv -----------开始
                            AVFrame* frame_filter;
                            ret = m_video_filtering.Filter(pFrame_Video, &frame_filter);
                            //>>>>滤波,亮度，对比度===参考JT1078ToYuv -----------结束


                            //AVFrame转为像素格式YUV420，宽高
                            ffmpeg.sws_scale(sws_ctx_video, frame_filter->data, frame_filter->linesize, 0, pCodecCtx_Video->height, pFrameYUV_Video->data, pFrameYUV_Video->linesize);

                            // 记录上一帧图像保持10个帧数
                            AVVideo videoFrame = new AVVideo(pCodecCtx_Video->width, pCodecCtx_Video->height, (IntPtr)out_buffer_video, out_buffer_size_video, pFrameYUV_Video->linesize[0]);
                            list.Add(videoFrame);
                            if (list.Count > 10) list.RemoveAt(0);

                            // SDL播放YUV数据：下面两种方式都可以进行播放
                            sdlVideo.SDL_Display(pCodecCtx_Video->width, pCodecCtx_Video->height,YuvWidth, YuvHeight, (IntPtr)out_buffer_video, out_buffer_size_video, pFrameYUV_Video->linesize[0]);
                            //sdlVideo.SDL_Display(pCodecCtx_Video->width, pCodecCtx_Video->height, (IntPtr)pFrameYUV_Video->data[0], out_buffer_size_video, pFrameYUV_Video->linesize[0]);

                            // 播放下一帧时进行暂停
                            if (isNextFrame)
                            {
                                Pause();
                                isNextFrame = false;
                            }

                            // 释放滤波
                            m_video_filtering.UnrefFrame();
                        }
                    }
                    #endregion

                    #region 音频AAC转PCM并使用SDL进行播放
                    if (packet->stream_index == audioindex)
                    {
                        //解码AVPacket->AVFrame
                        ret = ffmpeg.avcodec_decode_audio4(pCodeCtx_Audio, frame_Audio, &got_frame, packet);
                        if (ret < 0)
                        {
                            Console.WriteLine("音频解码失败");
                            return -1;
                        }
                        // 读取帧数据
                        if (got_frame > 0)
                        {
                            audio_frame_count++;
             
                            // 变换音频
                            ffmpeg.swr_convert(swrCtx_Audio, &out_audio_buffer, 2 * 8000, (byte**)&frame_Audio->data, frame_Audio->nb_samples);

                            // 获取sample的size
                            out_buffer_size_audio = ffmpeg.av_samples_get_buffer_size(null, out_channel_nb, frame_Audio->nb_samples, out_sample_fmt, 1);

                            // SDL进行音频播放
                            sdlAudio.PlayAudio((IntPtr)out_audio_buffer, out_buffer_size_audio);

                        }
                    }
                    #endregion
                  
                    //释放资源
                    ffmpeg.av_free_packet(packet);

                    Thread.Sleep(10);
                }


            }
            catch (Exception ex)
            {
                Console.WriteLine(ex);
            }
            finally
            {
                // 释放文件流
                ffmpeg.avformat_free_context(ofmt_ctx);
                // 修改右键菜单回调函数
                playFinishedDo.Invoke();
            }
            IsRun = false;
            IsPause = false;
            return 0;
        }

        bool isLastFrame = false;
        bool isNextFrame = false;
        bool playFastly = false;
        bool playSlowly = false;
        int play_speed = 1;
        long firstPts = -1;
        DateTime startTS;
        /// <summary>
        /// 控制快慢
        /// </summary>
        /// <param name="pts"></param>
        /// <param name="speed"></param>
        private void DeleyToPlay(long pts)
        {
            int delayTime = 0;
            try
            {
                // 计算延时
                double delay = (DateTime.Now - startTS).TotalMilliseconds;
                var i = (int)(pts - firstPts - delay);
                if (i >= 100)
                {
                    delayTime = 40;
                    delayTime = ControlFastOrSlow(delayTime);
                }
                else if (i >= 300)
                {
                    delayTime = 60;
                    delayTime = ControlFastOrSlow(delayTime);
                }
                else if (i >= 500)
                {
                    delayTime = 100;
                    delayTime = ControlFastOrSlow(delayTime);
                }
            }
            catch
            {
                Console.WriteLine("Counting delay time error ");
            }
            finally
            {
                Console.WriteLine("Counting delay time = " + delayTime+ " play_speed="+ play_speed);
                if (delayTime > 0)
                    Thread.Sleep(delayTime);
            }

        }

        /// <summary>
        /// 控制快慢
        /// </summary>
        /// <param name="delayTime"></param>
        private int ControlFastOrSlow(int delayTime)
        {
            if (playFastly)
            {
                // 快放
                delayTime /= play_speed;
            }
            else if (playSlowly)
            {
                // 慢放
                delayTime *= play_speed;
            }
            return delayTime;
        }

        /// <summary>
        /// 开启线程
        /// </summary>
        /// <param name="fileName"></param>
        /// <param name="sdlVideo"></param>
        /// <param name="sdlAudio"></param>
        public void Start(PlayFinishedDo playFinishedDo)
        {
            if (!isInit)
            {
                MessageBox.Show("没有初始化");
            }
            thread = new Thread(() =>
            {
                try
                {
                    ReadAndPlay(playFinishedDo);
                }
                catch (Exception ex)
                {
                    SQ.Base.ErrorLog.WriteLog4Ex("JT1078CodecForMp4.Run Video", ex);
                }
            });
            thread.IsBackground = true;
            thread.Start();


        }

        /// <summary>
        /// 暂停继续
        /// </summary>
        public void GoOnPlay()
        {
            // 重置第一帧pts，处理暂停后音视频不同步
            firstPts = -1;
            // 继续的相关操作和变量修改
            pause_thread = false;
            IsPause = pause_thread;
            sdlVideo.PlayVideo();
            sdlAudio.PlayAudio();
        }

        /// <summary>
        /// 暂停
        /// </summary>
        public void Pause()
        {
            // 暂停的相关操作和变量修改
            pause_thread = true;
            IsPause = pause_thread;
            sdlVideo.PauseVideo();
            sdlAudio.PauseAudio();
        }

        /// <summary>
        /// 停止
        /// </summary>
        public void Stop()
        {
            exit_thread = true;
            if (thread != null && thread.IsAlive)
            {
                thread.Abort();
                thread.Join();
                thread = null;
            }
        }

        /// <summary>
        /// 快放
        /// </summary>
        public void PlayFast()
        {
            if (pause_thread)
            {
                // 激活播放
                GoOnPlay();
            }
            if (playSlowly)
            {
                play_speed = 1;
                playSlowly = false;
            }
            else
            {
                play_speed++;
            }
            playFastly = true;

        }


        /// <summary>
        /// 慢放
        /// </summary>
        public void PlaySlow()
        {
            if (pause_thread)
            {
                // 激活播放
                GoOnPlay();
            }
            if (playFastly)
            {
                play_speed = 1;
                playFastly = false;
            }
            else
            {
                play_speed++;
            }
            playSlowly = true;

        }

        /// <summary>
        /// 上一帧
        /// </summary>
        public void PlayLastFrame()
        {
            // 修改上一帧标志
            isLastFrame = true;
            // 每点击一次向前播一帧
            if (list.Count>0)
            {
                Console.WriteLine("剩余播放帧:"+ list.Count);
                // 激活播放
                GoOnPlay();
                AVVideo lastFrame = list.Last();
                // 播放上一帧图像
                sdlVideo.SDL_Display(lastFrame.width, lastFrame.height, lastFrame.pixels, lastFrame.pixelsSize, lastFrame.pitch);
                // 修改上一帧标志
                isLastFrame = false;
                // 移除已看过的帧
                list.Remove(lastFrame);
                Thread.Sleep(10);
                Pause();
            }
            
           
        }

        /// <summary>
        /// 下一帧
        /// </summary>
        public void PlayNextFrame()
        {
            // 暂停以区分帧
            Pause();
            // 播放以完成下一帧图像显示或声音播放
            GoOnPlay();
            // 下一帧播放完成暂停标志
            isNextFrame = true;
        }
    }

    class Media
    {
        /// <summary>
        /// 0:video,1:audio
        /// </summary>
        public int type { get; set; }

        /// <summary>
        /// pts value
        /// </summary>
        public long pts { get; set; }
    }

    class AVVideo : Media
    {
        public int width { get; set; }
        public int height { get; set; }
        public IntPtr pixels { get; set; }
        public int pixelsSize { get; set; }
        public int pitch { get; set; }

        public AVVideo(int width, int height, IntPtr pixels, int pixelsSize, int pitch)
        {
            this.width = width;
            this.height = height;
            this.pixels = pixels;
            this.pixelsSize = pixelsSize;
            this.pitch = pitch;
        }
    }
    class AVAudio : Media
    {
        public IntPtr pcm { get; set; }
        public int len { get; set; }

        public AVAudio(IntPtr pcm, int len)
        {
            this.pcm = pcm;
            this.len = len;
        }
    }
}
```









