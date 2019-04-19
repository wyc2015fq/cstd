# sws_scale - maopig的专栏 - CSDN博客
2011年08月03日 23:14:16[maopig](https://me.csdn.net/maopig)阅读数：11113标签：[dst																[null																[struct																[image																[编程																[2010](https://so.csdn.net/so/search/s.do?q=2010&t=blog)](https://so.csdn.net/so/search/s.do?q=编程&t=blog)](https://so.csdn.net/so/search/s.do?q=image&t=blog)](https://so.csdn.net/so/search/s.do?q=struct&t=blog)](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=dst&t=blog)
个人分类：[x264 h264																[ffmpeg](https://blog.csdn.net/maopig/article/category/847674)](https://blog.csdn.net/maopig/article/category/847676)
 SwsContext* encoderSwsContext; 
AVFrame* encoderRawFrame; 
AVFrame* encoderRescaledFrame; 
// init and fill the encoderRawFrame with your image data 
// init encoderRescaledFrame 
encoderSwsContext = sws_getCachedContext(encoderSwsContext, src_width, src_height, PIX_FMT_RGB24, dst_width, dst_height, YUV420P, SWS_BICUBIC, NULL, NULL, NULL);
sws_scale(encoderSwsContext, encoderRawFrame->data, encoderRawFrame->linesize, 0, src_height, encoderRescaledFrame->data, encoderRescaledFrame->linesize);
## undefined reference to 'img_convert'，sws_scale替换了img_convert
(2010-10-25 13:56:11)
![](http://simg.sinajs.cn/blog7style/images/common/sg_trans.gif)转载
|标签：### [杂谈](http://uni.sina.com.cn/c.php?t=blog&k=%D4%D3%CC%B8&ts=bpost&stype=tag)|分类：[ffmpeg编程开发](http://blog.sina.com.cn/s/articlelist_1548604211_7_1.html)|
早期的ffmpeg库中，采用img_convert图像格式转换，而较新的ffmpeg库中去掉了该函数，加入了swscale库，用该库中的sws_scale函数可以替代img_convert，两个函数原型如下：
int img_convert(AVPicture *dst, int dst_pix_fmt,
                const AVPicture *src, int src_pix_fmt,
                int src_width, int src_height)
int sws_scale(struct SwsContext *ctx, uint8_t* src[], int srcStride[],
                    int srcSliceY, int srcSliceH, uint8_t* dst[], int dstStride[])
为了方便，可以自己写一个img_convert函数，然后函数内部用sws_scale来实现，只是对于一些错误的处理及返回值处理不太严格，但基本能用，代码如下：
int img_convert(AVPicture *dst, int dst_pix_fmt,
                const AVPicture *src, int src_pix_fmt,
                int src_width, int src_height)
{
    int w;
    int h;
    SwsContext *pSwsCtx;
    w = src_width;
    h = src_height;
pSwsCtx = 
sws_getContext(w, h, src_pix_fmt, 
                            w, h, dst_pix_fmt,
                            SWS_BICUBIC, NULL, NULL, NULL);
sws_scale(pSwsCtx, src->data, src->linesize,
            0, h, dst->data, dst->linesize);
 //这里释放掉pSwsCtx的内存
    return 0;
}

