# FFMpeg中apiexample.c例子分析——编码分析 - maopig的专栏 - CSDN博客
2011年06月19日 09:46:00[maopig](https://me.csdn.net/maopig)阅读数：1051标签：[video																[codec																[null																[api																[工作																[c](https://so.csdn.net/so/search/s.do?q=c&t=blog)](https://so.csdn.net/so/search/s.do?q=工作&t=blog)](https://so.csdn.net/so/search/s.do?q=api&t=blog)](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=codec&t=blog)](https://so.csdn.net/so/search/s.do?q=video&t=blog)
### FFMpeg中apiexample.c例子分析——编码分析apiexample.c例子教我们如何去利用ffmpeg库中的api函数来自己编写编解码程序。
![FFMpeg中apiexample.c例子分析——编码分析 - 小康 - 小康的博客](http://img308.ph.126.net/hBwwc3VVSuUs2pUfJ9-xoQ==/3893080402885214571.jpg)
           （1）首先，main函数中一开始会去调用avcodec_init()函数，该函数的作用是初始化libavcodec，而我们在使用avcodec库时，该函数必须被调用。
　　（2）avcodec_register_all()函数，注册所有的编解码器（codecs），解析器（parsers）以及码流过滤器（bitstream filters）。当然我们也可以使用个别的注册函数来注册我们所要支持的格式。
　　（3）video_encode_example()函数用于视频编码，由图可知，所有的编码工作都在该函数内完成。
　　（4）avcodec_find_encoder()函数用于查找一个与codec ID相匹配的已注册的编码器。
　　（5）avcodec_alloc_context()函数用于分配一个AVCodecContext并设置默认值，如果失败返回NULL，并可用av_free()进行释放。
　　（6）avcodec_alloc_frame()函数用于分配一个AVFrame并设置默认值，如果失败返回NULL，并可用av_free()进行释放。
　　（7）设置参数：
　　　　设置采样参数，即比特率。
　　　　c->bit_rate = 400000;
　　　　设置分辨率，必须是2的倍数。
　　　　c->width = 352;
　　　　c->height = 288;
　　　　设置帧率。
　　　　c->time_base = (AVRational){1,25}; 该帧率为25，其实timebase = 1/framerate，花括号内分别为分子和分母。
　　　　设置GOP大小。
　　　　c->gop_size = 10; 该值表示每10帧会插入一个I帧（intra frame）。
　　　　设置B帧最大数。
　　　　c->max_b_frames = 1; 该值表示在两个非B帧之间，所允许插入的B帧的最大帧数。
　　　　设置像素格式。
　　　　c->pix_fmt = PIX_FMT_YUV420P; 该值将像素格式设置为YUV420P。
　　（8）avcodec_open()函数用给定的AVCodec来初始化AVCodecContext。
　　（9）接着是打开文件，f = fopen( filename, “wb” );
    （10）分配图像和输出缓存。
　　　　申请100KB左右的内存作为输出缓存。
outbuf_size = 100000;
　　　　outbuf = malloc( outbuf_size );
　　　　根据帧的大小来确定YUV420所占内存大小，一个像素，RGB格式占用3个字节，而YUV420格式只占用两个字节。YUV420格式是指，每个像素都保留一个Y（亮度）分量，而在水平方向上，不是每行都取U和V分量，而是一行只取U分量，则其接着一行就只取V分量，以此重复，所以420不是指没有V，而是指一行采样只取U，另一行采样只取V。在取U和V时，每两个Y之间取一个U或V。但从4x4矩阵列来看，每4个矩阵点Y区域中，只有一个U和V，所以它们的比值是4:1。所以对于一个像素，RGB需要8 * 3 = 24位，即占3个字节；而YUV420P，8 + 8/4 + 8/4 = 12位，即占2个字节，其中8指Y分量，8/4指U和V分量。
　　　　size = c->width * c->height;
　　　　picture_buf = malloc( (size * 3) / 2 );  
　　　　picture->data[0] = picture_buf;
　　　　picture->data[1] = picture->data[0] + size;
　　　　picture->data[2] = picture->data[1] + size / 4;
　　　　picture->linesize[0] = c->width;
　　　　picture->linesize[1] = c->width / 2;
　　　　picture->linesize[2] = c->width / 2;
　　　　其中，data[0]存放Y，data[1]存放U，data[2]存放V【FixMe】。linesize[0]表示Y分量的宽度，linesize[1]表示U分量的宽度，linesize[2]表示V分量的宽度。
　　（11）编码一秒钟的视频，帧率为25，所以需要循环25次，每次编码一帧。
　　（11.1）准备一幅伪图像，即自己自定义往里面塞数据。
　　　　for(y=0;y<c->height;y++){
                     for(x=0;x<c->width;x++){
                            picture->data[0][y*picture->linesize[0]+x]=x+y+i*3;
}
}
for(y=0;y<c->height/2;y++){
       for(x=0;x<c->width/2;x++){
              picture->data[1][y*picture->linesize[1]+x]=128+y+i*2;
              picture->data[2][y*picture->linesize[2]+x]=64+x+i*5;
}
}
　　（11.2）avcodec_encode_video()从picture中编码一帧视频数据，并存入到outbuf中，而期间所使用的编码器为c。
　　（11.3）将编码完的数据写入到文件里。
　　（12）对延时的帧数据进行编码。因为像MPEG4中，I帧、P帧和B帧之间存在一定的延时【FixMe】。同样是avcodec_encode_video()，然后写入文件。
　　（13）添加结束代码，使其成为一个真正的mpeg文件。
　　　　outbuf[0] = 0x00;
　　　　outbuf[1] = 0x00;
　　　　outbuf[2] = 0x01;
　　　　outbuf[3] = 0xb7;
　　　　fwrite( outbuf, 1, 4, f );
　　这个结束代码表示什么？？？
　　（14）释放资源。
　　　　fclose(f);　　　free(picture_buf);
　　　　free(outbuf);
　　　　avcodec_close(c);
　　　　av_free(c);
　　　　av_free(picture);
