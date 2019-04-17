# FFmpeg添加滤镜调整视频对比度和亮度 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年03月30日 09:55:25[boonya](https://me.csdn.net/boonya)阅读数：1083








原文地址：[https://blog.csdn.net/wer85121430/article/details/79642951](https://blog.csdn.net/wer85121430/article/details/79642951)


目的:调整视频的亮度和对比度。

可参考ffmpeg的源码：

\examples\filtering_video.c，视频滤波例子

\libavfilter\vf_eq.c，亮度对比度调整的具体实现

先使用ffmpeg.exe试试效果，女神的原始视频图片：

![](https://img-blog.csdn.net/20180322110208197)


ffmpeg.exe -i record.mp4 -vf eq=contrast=1:brightness=-0.2 output.mp4 处理后图片


![](https://img-blog.csdn.net/20180322110310546)


ffmpeg.exe -i record.mp4 -vf eq=contrast=1:brightness=0.2 output.mp4 处理后图片


![](https://img-blog.csdn.net/20180322110359573)


效果还可以.

写好了，贴代码，主要是参考这个文件\examples\filtering_video.c，C代码改C#遇到了几个坑花了大半天时间

Init函数的contrast和brightness参数分别对应对比度和亮度，取值1-9，5为原始图像。



**[csharp]**[view plain](https://blog.csdn.net/wer85121430/article/details/79642951#)[copy](https://blog.csdn.net/wer85121430/article/details/79642951#)

- 




**[csharp]**[view plain](https://blog.csdn.net/wer85121430/article/details/79642951#)[copy](https://blog.csdn.net/wer85121430/article/details/79642951#)

- publicunsafeclass VideoFiltering  
- {  
-     #region 类成员变量
-     AVFilterGraph* m_filter_graph = null;  
-     AVFilterContext* m_buffersink_ctx = null;  
-     AVFilterContext* m_buffersrc_ctx = null;  
-     AVFrame* m_filt_frame = null;  
-     Object m_lock_record = new Object();  
-     #endregion
- 
- publicint Init(int width, int height, int contrast, int brightness)  
-     {  
- lock (m_lock_record)  
-         {  
- // Critical code section
- if (m_filter_graph != null)  
- return -1;  
- 
-             contrast = contrast < 1 ? 1 : contrast;  
-             contrast = contrast > 9 ? 9 : contrast;  
-             brightness = brightness < 1 ? 1 : brightness;  
-             brightness = brightness > 9 ? 9 : brightness;  
- 
- float contrast_f = 1 + ((float)(contrast - 5)) / 10;  
- float brightness_f = 0 + ((float)(brightness - 5)) / 10;  
- string filters_descr = string.Format("eq=contrast=" + contrast_f.ToString() + ":brightness=" + brightness_f.ToString());  
- 
- return init_filters(width, height, filters_descr);  
-         }  
-    }  
- publicint Reset(int width, int height, int contrast, int brightness)  
-     {  
-         Deinit();  
- return Init(width, height, contrast, brightness);  
-     }  
- 
- publicint Filter(AVFrame * frame_src, AVFrame **frame_dst)  
-     {  
- lock (m_lock_record)  
-         {  
-             *frame_dst = frame_src;  
- if (m_filter_graph == null)  
-             {  
- return -1;  
-             }  
- 
- int ret;  
- 
- //AV_BUFFERSRC_FLAG_KEEP_REF = 8,
-             ret = ffmpeg.av_buffersrc_add_frame_flags(m_buffersrc_ctx, frame_src, 8);  
- if (ret < 0)  
- return ret;  
- 
-             ret = ffmpeg.av_buffersink_get_frame(m_buffersink_ctx, m_filt_frame);  
- if (ret < 0)  
- return ret;  
- 
-             *frame_dst = m_filt_frame;  
- 
- return 0;  
-         }  
-     }  
- publicvoid UnrefFrame()  
-     {  
- lock (m_lock_record)  
-         {  
- if (m_filter_graph == null)  
- return ;  
-             ffmpeg.av_frame_unref(m_filt_frame);  
-         }  
-     }  
- 
- publicvoid Deinit()  
-     {  
- if (m_filter_graph == null)  
- return ;  
- 
- if(m_filter_graph != null)  
-         {  
- fixed(AVFilterGraph** filter_graph = &m_filter_graph)  
-                 ffmpeg.avfilter_graph_free(filter_graph);  
-         }  
- 
- if (m_filt_frame != null)  
-         {  
- fixed (AVFrame** filt_frame = &m_filt_frame)  
-                 ffmpeg.av_frame_free(filt_frame);  
-         }  
- 
-     }  
- 
- privateint init_filters(int width, int height, string filters_descr)  
-     {  
- int ret = 0;  
- 
-         ffmpeg.avfilter_register_all();  
- 
- //AVPixelFormat.AV_PIX_FMT_YUV420P = 0;
- string args = string.Format("video_size=" + width.ToString() + "x" + height.ToString() +  
- ":pix_fmt=0:time_base=1/20");  
- 
-         AVFilter* buffersrc = ffmpeg.avfilter_get_by_name("buffer");  
-         AVFilter* buffersink = ffmpeg.avfilter_get_by_name("buffersink");  
-         AVFilterInOut* outputs = ffmpeg.avfilter_inout_alloc();  
-         AVFilterInOut* inputs = ffmpeg.avfilter_inout_alloc();  
- //AVRational time_base;
- 
- int* pix_fmts = (int*)ffmpeg.av_malloc(8);  
-         pix_fmts[0] = (int)AVPixelFormat.AV_PIX_FMT_YUV420P;  
-         pix_fmts[1] = (int)AVPixelFormat.AV_PIX_FMT_NONE;  
- //AVPixelFormat pix_fmts[] = { AVPixelFormat.AV_PIX_FMT_YUV420P, AVPixelFormat.AV_PIX_FMT_NONE };
- 
-         m_filter_graph = ffmpeg.avfilter_graph_alloc();  
- if(outputs == null || inputs == null || m_filter_graph == null)  
-         {  
-             ret = -1;  
- goto end;  
-         }  
- 
- fixed (AVFilterContext** buffersrc_ctx = &m_buffersrc_ctx)  
-         {  
-             ret = ffmpeg.avfilter_graph_create_filter(buffersrc_ctx, buffersrc, "in", args, null, m_filter_graph);  
- if (ret < 0)  
-             {  
- goto end;  
-             }  
-         }  
- 
- fixed (AVFilterContext** buffersink_ctx = &m_buffersink_ctx)  
-         {  
-             ret = ffmpeg.avfilter_graph_create_filter(buffersink_ctx, buffersink, "out", null, null, m_filter_graph);  
- if (ret < 0)  
-             {  
- goto end;  
-             }  
-         }  
- 
- 
- int size = (int)ffmpeg.av_int_list_length_for_size(1, (void*)pix_fmts, unchecked((ulong)AVPixelFormat.AV_PIX_FMT_NONE));  
-         ret = ffmpeg.av_opt_set_bin(m_buffersink_ctx, "pix_fmts", (byte*)pix_fmts, size, ffmpeg.AV_OPT_SEARCH_CHILDREN);  
- if(ret < 0)  
- goto end;  
- 
-         outputs->name = ffmpeg.av_strdup("in");  
-         outputs->filter_ctx = m_buffersrc_ctx;  
-         outputs->pad_idx = 0;  
-         outputs->next = null;  
- 
-         inputs->name = ffmpeg.av_strdup("out");  
-         inputs->filter_ctx = m_buffersink_ctx;  
-         inputs->pad_idx = 0;  
-         inputs->next = null;  
- 
-         ret = ffmpeg.avfilter_graph_parse_ptr(m_filter_graph, filters_descr, &inputs, &outputs, null);  
- if (ret < 0)  
- goto end;  
- 
-         ret = ffmpeg.avfilter_graph_config(m_filter_graph, null);  
- if (ret < 0)  
- goto end;  
- 
-         m_filt_frame = ffmpeg.av_frame_alloc();  
- 
-         end:  
-         ffmpeg.avfilter_inout_free(&inputs);  
-         ffmpeg.avfilter_inout_free(&outputs);  
- 
-         ffmpeg.av_free(pix_fmts);  
- 
- if (ret < 0)  
-             Deinit();  
- 
- return ret;  
-     }  
- }  






