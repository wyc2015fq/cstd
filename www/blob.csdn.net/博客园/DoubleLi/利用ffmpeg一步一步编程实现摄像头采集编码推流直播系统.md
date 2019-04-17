# 利用ffmpeg一步一步编程实现摄像头采集编码推流直播系统 - DoubleLi - 博客园






了解过ffmpeg的人都知道，利用ffmpeg命令即可实现将电脑中摄像头的画面发布出去，例如发布为UDP，RTP，RTMP等，甚至可以发布为HLS，将m3u8文件和视频ts片段保存至Web服务器，普通的播放器就可以直接播放他们。

的确，但是，但是作为一个技术爱好者，相信大家都是对里面的机制与原理是感兴趣的，我们希望通过我们写代码来实现它。另外，ffmpeg命令发布出来的摄像头画面我们似乎要加一些水印、显示一些自定义的文字，可能就不是那么灵活了。举个例子来说，我们想画面中主要画面是电脑桌面，左上角显示摄像头的画面，相信现有的ffmpeg命令实现不了了吧！那有什么办法吗？答案是肯定的，假如我们找到ffmpeg中采集视频的部分，把采集到的画面替换成我们需要的不就行了吗？即我们抓一下桌面画面，再抓一下摄像头画面，然后通过一定的缩放叠加在一块就行了。

讲到这里，我们就有一个目的了，想想刚才我们的设想，我们可以想到ffmpeg中的一个功能，将电脑桌面的画面保存到文件中（当然也可以发布到网络中），这个功能是不是和我们的需求特别相像，通过百度，我们可以得到以下一段代码：





**[cpp]**[view plain](https://blog.csdn.net/ce6581281/article/details/62898445#)[copy](https://blog.csdn.net/ce6581281/article/details/62898445#)



- AVFormatContext *pFormatCtx = avformat_alloc_context();    
- AVInputFormat *ifmt=av_find_input_format("gdigrap");    
- avformat_open_input(&pFormatCtx, 0, ifmt,NULL);  


通过以上代码我们可以打开一个设备gdigrap(录制windows桌面)，打开后，我们就可以从中一帧一帧地读出画面了，我们在libavdevice目录中可以找到一个叫gdigrap.c的文件，里面实现了ffmpeg一个设备的基本实现，结构如下：







**[cpp]**[view plain](https://blog.csdn.net/ce6581281/article/details/62898445#)[copy](https://blog.csdn.net/ce6581281/article/details/62898445#)



- /** gdi grabber device demuxer declaration */  
- AVInputFormat ff_gdigrab_demuxer = {  
-     .name           = "gdigrab",  
-     .long_name      = NULL_IF_CONFIG_SMALL("GDI API Windows frame grabber"),  
-     .priv_data_size = sizeof(struct gdigrab),  
-     .read_header    = <span style="color:#ff6666;">gdigrab_read_header</span>,  
-     .read_packet    = <span style="color:#ff6666;">gdigrab_read_packet</span>,  
-     .read_close     = <span style="color:#ff6666;">gdigrab_read_close</span>,  
-     .flags          = AVFMT_NOFILE,  
-     .priv_class     = &gdigrab_class,  
- };  



注意红色部分的三个方法，就是该设备的开启、读数据、关闭，简单地扫描一下三个方法的代码，里面的代码还是比较简单，就是一些DC的操作（熟悉windows窗口绘图相关知识的应该了解）,读到这，我们就有思路了，我们可以模仿这个文件来实现我们想要的，考虑到这个文件是编译到ffmpeg库的，所以我们希望不破坏ffmpeg的框架的情况下，增加几个接口（或者说回调），当打开设备时，回调一下我们的接口，当读画面的时候，回调一下我们的接口，当关闭设备时，也回调一下我们的接口，这样实际上我们新写的这个C文件只是一个架子，里面的具体实现交给外部使用者，所以我们定义以下三个接口：







**[cpp]**[view plain](https://blog.csdn.net/ce6581281/article/details/62898445#)[copy](https://blog.csdn.net/ce6581281/article/details/62898445#)



- typedef int (*fnVideoCapInitCallback)(int index, int width, int height, int framerate);  
- typedef int (*fnVideoCapReadCallback)(int index, unsigned char *buff, int len, int width, int height, int framerate, int format);  
- typedef int (*fnVideoCapCloseCallback)(int index);  




**[cpp]**[view plain](https://blog.csdn.net/ce6581281/article/details/62898445#)[copy](https://blog.csdn.net/ce6581281/article/details/62898445#)



- void av_setVideoCapInitCallback(fnVideoCapInitCallback callback);  
- void av_setVideoCapReadCallback(fnVideoCapReadCallback callback);  
- void av_setVideoCapCloseCallback(fnVideoCapCloseCallback callback);  







下面是实现结构体中的三个方法的具体实现：





**[cpp]**[view plain](https://blog.csdn.net/ce6581281/article/details/62898445#)[copy](https://blog.csdn.net/ce6581281/article/details/62898445#)



- static int mygrab_read_header(AVFormatContext *s1)  
- {  
- struct mygrab *mygrab = s1->priv_data;  
-     AVStream   *st       = NULL;  
- int ret = 0;  
-     printf("call mygrab_read_header\n");  
- if(mygrab->width <= 0 || mygrab->height <= 0){  
-         av_log(s1, AV_LOG_ERROR, "video size (%d %d) is invalid\n", mygrab->width, mygrab->height);  
- return -1;  
-     }  
- 
-     st = avformat_new_stream(s1, NULL);  
- if (!st) {   
-         ret = AVERROR(ENOMEM);  
- return -1;   
-     }  
-     printf("avpriv_set_pts_info\n");  
-     avpriv_set_pts_info(st, 64, 1, 1000000); /* 64 bits pts in us */  
- if(mygrab->framerate.num <= 0 || mygrab->framerate.den <= 0 ){  
-         av_log(s1, AV_LOG_WARNING, "not set framerate set default framerate\n");  
-         mygrab->framerate.num = 10;  
-         mygrab->framerate.den = 1;  
-     }  
- 
-     mygrab->time_base   = av_inv_q(mygrab->framerate);      
-     mygrab->time_frame  = av_gettime() / av_q2d(mygrab->time_base);  
-     mygrab->frame_size = mygrab->width * mygrab->height * 3/2;  
-     st->codec->codec_type = AVMEDIA_TYPE_VIDEO;  
-     st->codec->codec_id = AV_CODEC_ID_RAWVIDEO;  
-     st->codec->pix_fmt = AV_PIX_FMT_YUV420P;//AV_PIX_FMT_RGB24;  
-     st->codec->width = mygrab->width;  
-     st->codec->height = mygrab->height;   
-     st->codec->time_base = mygrab->time_base;  
-     st->codec->bit_rate = mygrab->frame_size * 1/av_q2d(st->codec->time_base) * 8;  
- 
- <span style="color:#ff6666;">   if(s_videoCapInitCallback != NULL){  
-         av_log(s1, AV_LOG_INFO, "video size (%d %d) frameRate:%d\n", st->codec->width, st->codec->height, mygrab->framerate.num/mygrab->framerate.den);  
-         s_videoCapInitCallback(0, st->codec->width, st->codec->height, mygrab->framerate.num/mygrab->framerate.den);  
- return 0;  
-     }</span>  
- 
-     av_log(s1, AV_LOG_ERROR, "video cap not call av_setVideoCapInitCallback\n");  
- return -1;  
- 
- }  
- 
- static int mygrab_read_packet(AVFormatContext *s1, AVPacket *pkt)  
- {  
- struct mygrab *s = s1->priv_data;  
-     int64_t curtime, delay;  
- /* Calculate the time of the next frame */   
-     s->time_frame += INT64_C(1000000);  
- 
- /* wait based on the frame rate */  
- for(;;) {  
-         curtime = av_gettime();  
-         delay = s->time_frame * s->time_base.num / s->time_base.den - curtime;  
- if (delay <= 0) {  
- if (delay < INT64_C(-1000000) * s->time_base.num / s->time_base.den) {  
- /* printf("grabbing is %d frames late (dropping)\n", (int) -(delay / 16666)); */  
-                 s->time_frame += INT64_C(1000000);  
-             }  
- break;  
-         }  
-         av_usleep(delay);  
-     }  
- if (av_new_packet(pkt, s->frame_size) < 0)  return AVERROR(EIO);  
- 
-     pkt->pts = curtime;  
- <span style="color:#ff6666;">   if(s_videoCapReadCallback != NULL){  
-         s_videoCapReadCallback(0, pkt->data, pkt->size, s->width, s->height, s->framerate.num/s->framerate.den, AV_PIX_FMT_YUV420P);  
- return pkt->size;  
-     }</span>  
-     av_log(s1, AV_LOG_ERROR, "video cap not call av_setVideoCapReadCallback\n");  
- return 0;  
- }  
- 
- 
- static int mygrab_read_close(AVFormatContext *s1)  
- {  
- //struct mygrab *s = s1->priv_data;  
- <span style="color:#ff6666;">   if(s_videoCapCloseCallback != NULL){  
-         s_videoCapCloseCallback(0);  
-     }</span>  
- return 0;  
- }  




然后我们在alldevices.c里注册我们的自定义设备，后面我们使用设备时就可以通过名字调用到我们刚才新建mygrab.c的逻辑了，即在



**[cpp]**[view plain](https://blog.csdn.net/ce6581281/article/details/62898445#)[copy](https://blog.csdn.net/ce6581281/article/details/62898445#)



- void avdevice_register_all(void)  
- {  
-     ......  
-     REGISTER_INDEV   (MYGRAB,          mygrab);  
-     ......  
- }  




同理，新建myoss.c实现声音的自定义处理采集，这样，我们就新增加了六个接口了，为了保持接口的简洁性，我们将六个接口合并为一个接口，即



**[cpp]**[view plain](https://blog.csdn.net/ce6581281/article/details/62898445#)[copy](https://blog.csdn.net/ce6581281/article/details/62898445#)



- void av_setVideoAudioCapCallbacks(fnVideoCapInitCallback callback1,fnVideoCapReadCallback callback2,fnVideoCapCloseCallback callback3  
-         ,fnAudioCapInitCallback callback4,fnAudioCapReadCallback callback5,fnAudioCapCloseCallback callback6);  






重新编译ffmpeg就支持我们的自定义视频采集、音频采集功能了，非常的灵活，比如我们要使用这些功能，一般就会写下下面几行代码：



**[cpp]**[view plain](https://blog.csdn.net/ce6581281/article/details/62898445#)[copy](https://blog.csdn.net/ce6581281/article/details/62898445#)



- av_setVideoAudioCapCallbacks(..,..,..,..,..,..,..); //将回调注册进去  
- AVFormatContext *pFormatCtx = avformat_alloc_context();    
- AVInputFormat *ifmt=av_find_input_format("mygrap");    
- avformat_open_input(&pFormatCtx, 0, ifmt,NULL); //打开自定义视频设备  
- 
- 
- AVFormatContext *pFormatCtx = avformat_alloc_context();    
- AVInputFormat *ifmt=av_find_input_format("myoss");    
- avformat_open_input(&pFormatCtx, 0, ifmt,NULL);//打开自定义音频设备  






然后按照操作其它设备一样的操作方法，第一步中设置的回调就会依次去调用了。通过上面的折腾，我们为后面的视频的采集音频的采集打好了基础，并且使用时不需要再进行麻烦的编译，专注于我们的回调的实现就好。
ffmpeg库编好了，就要开始实现我们最主要的摄像头采集编码推流了，我们注意到ffmpeg中有一个比较好的参考例子，叫做muxing.c，这个文件实现了将画面和声音保存为一个视频的功能，因此我们可以在这个文件的基础上实现我们的功能，muxing.c里的画面和声音是通过代码来生成的，换成从摄像头和声卡采集就行了，当然我们肯定不是在里面去简单地加代码，要不然上面这么大的折腾就没有意义了，我们要用上上面两个自定义的设备，然后从设备里读数据。
做好了以上的几个步骤，相信就可以把摄像头的视频和声音保存为mp4等文件了。
这已经离发布到网络上很近了，因此要使视频不保存在文件中，而是发布到网络上，只要改几行代码就可以了，例如以下是发布为HLS，RTMP，FILE三个方法：



**[cpp]**[view plain](https://blog.csdn.net/ce6581281/article/details/62898445#)[copy](https://blog.csdn.net/ce6581281/article/details/62898445#)



- if(type == TYPE_HLS){  
-         sprintf(filename, "%s\\playlist.m3u8", szPath);  
-         avformat_alloc_output_context2(&oc, NULL, "hls", filename);  
- 
- 
-     }else if(type == TYPE_RTMP){  
-         sprintf(filename, "%s", szPath);  
-         avformat_alloc_output_context2(&oc, NULL, "flv", filename);  
-     }else if(type == TYPE_FILE){  
-         sprintf(filename, "%s", szPath);  
-         avformat_alloc_output_context2(&oc, NULL, NULL, filename);  
-     }  








这样，我们就可以把我们的直播发布到网络中了，这里，我们再搭建一个流媒体服务器，将流推向这个服务器，比如red5,下载一个vlc或者ffplay就可以用来播放它了，这样一个完整的直播系统就编码实现好了，并且直播中的画面是任由我们发挥的，不光是摄像头画面，看想象力了。

以下是用MFC作出的一个摄像头采集直播Demo，电脑浏览器通过web测试页，手机PAD通过VLC等播放器就可以实时地查看，值得一提的是，web测试页中的延时比较小，如果用播放器来播放延时比较大，这是因为web测试页中的flash将rtmp的缓存设置成最小，而第三方播放器播放有一个缓存，而且相对比较大。

![](https://img-blog.csdn.net/20170317203441665?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2U2NTgxMjgx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


另外，Demo中摄像头采集是用的opencv，这是由于opencv对图像这一块处理比较方便，可以很方便地在图像中增加文字，反转，变换等。
以上所述的所有源码，可以在我的[个人网站](http://www.creater.net.cn/softwaredesign.html)[http://www.creater.net.cn/softwaredesign.html](http://www.creater.net.cn/softwaredesign.html)中获取



from:https://blog.csdn.net/ce6581281/article/details/62898445









