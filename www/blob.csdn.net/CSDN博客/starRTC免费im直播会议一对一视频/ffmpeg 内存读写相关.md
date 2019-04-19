# ffmpeg 内存读写相关 - starRTC免费im直播会议一对一视频 - CSDN博客
2016年12月07日 17:33:40[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：335
需要的解码的视频数据在一段内存中。例如，通过其他系统送来的视频数据。同样，有的时候编码后的视频数据也未必要保存成一个文件。例如，要求将编码后的视频数据送给其他的系统进行下一步的处理。以上两种情况就要求FFmpeg不仅仅是对文件进行“读，写”操作，而是要对内存进行“读，写”操作。
**从内存中读取数据**
经过分析ffmpeg的源代码，发现其是可以从内存中读取数据的：
- AVFormatContext *ic = NULL;  
- ic = avformat_alloc_context();  
- unsigned **char** * iobuffer=(unsigned **char** *)av_malloc(32768);  
- AVIOContext *avio =avio_alloc_context(iobuffer, 32768,0,NULL,fill_iobuffer,NULL,NULL);  
- ic->pb=avio;  
- err = avformat_open_input(&ic, "nothing", NULL, NULL);  
关键要在avformat_open_input()之前初始化一个AVIOContext，而且将原本的AVFormatContext的指针pb（AVIOContext类型）指向这个自行初始化的AVIOContext。当自行指定了AVIOContext之后，avformat_open_input()里面的URL参数就不起作用了。示例代码开辟了一块空间iobuffer作为AVIOContext的缓存。
fill_iobuffer是将数据读取至iobuffer的回调函数。fill_iobuffer()形式（参数，返回值）是固定的，是一个回调函数，如下所示（只是个例子，具体怎么读取数据可以自行设计）。示例中回调函数将文件中的内容通过fread()读入内存。
- //读取数据的回调函数-------------------------
- //AVIOContext使用的回调函数！
- //注意：返回值是读取的字节数  
- //手动初始化AVIOContext只需要两个东西：内容来源的buffer，和读取这个Buffer到FFmpeg中的函数
- //回调函数，功能就是：把buf_size字节数据送入buf即可
- //第一个参数(void *opaque)一般情况下可以不用
- **int** fill_iobuffer(**void** * opaque,uint8_t *buf, **int** bufsize){  
- **if**(!feof(fp_open)){  
- **int** true_size=fread(buf,1,buf_size,fp_open);  
- **return** true_size;  
-     }**else**{  
- **return** -1;  
-     }  
- }  
整体结构大致如下：
- **FILE** *fp_open;  
- 
- **int** fill_iobuffer(**void** *opaque, uint8_t *buf, **int** buf_size){  
- ...  
- }  
- 
- **int** main(){  
-     ...  
-     fp_open=fopen("test.h264","rb+");  
-     AVFormatContext *ic = NULL;  
-     ic = avformat_alloc_context();  
-     unsigned **char** * iobuffer=(unsigned **char** *)av_malloc(32768);  
-     AVIOContext *avio =avio_alloc_context(iobuffer, 32768,0,NULL,fill_iobuffer,NULL,NULL);  
-     ic->pb=avio;  
-     err = avformat_open_input(&ic, "nothing", NULL, NULL);  
-     ...//解码
- }  
**将数据输出到内存**
也可以将处理后的数据输出到内存。
回调函数如下示例，可以将输出到内存的数据写入到文件中。
- //写文件的回调函数
- **int** write_buffer(**void** *opaque, uint8_t *buf, **int** buf_size){  
- **if**(!feof(fp_write)){  
- **int** true_size=fwrite(buf,1,buf_size,fp_write);  
- **return** true_size;  
-     }**else**{  
- **return** -1;  
-     }  
- }  
主函数如下所示。
- **FILE** *fp_write;  
- 
- **int** write_buffer(**void** *opaque, uint8_t *buf, **int** buf_size){  
- ...  
- }  
- 
- main(){  
-     ...  
-     fp_write=fopen("src01.h264","wb+"); //输出文件
-     ...  
-     AVFormatContext* ofmt_ctx=NULL;  
-     avformat_alloc_output_context2(&ofmt_ctx, NULL, "h264", NULL);  
-     unsigned **char*** outbuffer=(unsigned **char***)av_malloc(32768);  
- 
-     AVIOContext *avio_out =avio_alloc_context(outbuffer, 32768,0,NULL,NULL,write_buffer,NULL);    
- 
-     ofmt_ctx->pb=avio_out;   
-     ofmt_ctx->flags=AVFMT_FLAG_CUSTOM_IO;  
-     ...  
- }  
**关键点**
关键点就两个：
1.      初始化自定义的AVIOContext，指定自定义的回调函数。示例代码如下：
- //AVIOContext中的缓存
- unsigned **char** *aviobuffer=(unsigned **char***)av_malloc(32768);  
- AVIOContext *avio=avio_alloc_context(aviobuffer, 32768,0,NULL,read_buffer,NULL,NULL);  
- pFormatCtx->pb=avio;  
- 
- **if**(avformat_open_input(&pFormatCtx,NULL,NULL,NULL)!=0){  
-            printf("Couldn't open inputstream.（无法打开输入流）\n");  
- **return** -1;  
- }  
上述代码中，自定义了回调函数read_buffer()。在使用avformat_open_input()打开媒体数据的时候，就可以不指定文件的URL了，即其第2个参数为NULL（因为数据不是靠文件读取，而是由read_buffer()提供）
2.      自己写回调函数。示例代码如下：
- //Callback
- **int** read_buffer(**void** *opaque, uint8_t *buf, **int** buf_size){  
- **if**(!feof(fp_open)){  
-         int true_size=fread(buf,1,buf_size,fp_open);  
- **return** true_size;  
-     }**else**{  
- **return** -1;  
-     }  
- }  
当系统需要数据的时候，会自动调用该回调函数以获取数据。这个例子为了简单，直接使用fread()读取数据至内存。回调函数需要格外注意它的参数和返回值。

avio_alloc_context第3个参数Set to 1 if the buffer should be writable, 0 otherwise. 为0时表示ffmpeg要从回调函数里读取数据。为1表示ffmpeg会将数据输出到回调函数中。

**参考：**
[**从内存中读取数据（或将数据输出到内存）**](http://blog.csdn.net/leixiaohua1020/article/details/12980423)
[**内存播放器**](http://blog.csdn.net/leixiaohua1020/article/details/39759163)
[**内存转码器**](http://blog.csdn.net/leixiaohua1020/article/details/39759623)
