# live555接收实时视频流+ffmpeg解 - 建建的博客 - CSDN博客
2018年07月21日 13:17:14[纪建](https://me.csdn.net/u013898698)阅读数：391
    本人视频监控行业的码农一枚，将平时快速学习的内容进行记录和分享，因水平有限，没学过音视频的高深理论和技术，所写内容多侧重开源库的使用，比较肤浅，如果文中有什么不足，希望大家多多指点。 
    如本文题目所述，文中介绍一下如何用接收rtsp实时流后对实时流进行解码，解码成yuv420。
    live555 中的例子testRTSPClient.cpp 提供了一个创建rtsp客户端接收视频流的例子，使用该例子可从支持rtsp协议的摄像机获取视频流。因为我用的相机提供的是h264编码的实时流，并且我希望在接收实时流的时候保存录像，因此对testRTSPClient.cpp进行了简单的修改。 continueAfterSETUP 函数中，进行如下修改：
`scs.subsession->sink=H264VideoFileSink::createNew(env,"test.h264",NULL,550000,false);`
    使用H264VideoFileSink处理接收到的h264视频流。（我用的相机多是只有视频，如果有音频需要根据实际情况处理，本文所做的相应编码全部是只接收视频流的。如果对于有音频的相机，只想接收视频流可以在MediaSession.cpp  的createSourceObjects 函数中，在if (createSimpleRTPSource) 之前把createSimpleRTPSource 置为 false）。经过后期反思，可以自己继承filesink后添加和自己业务相关的逻辑。
    H264VideoFileSink继承了FileSink，实际上，每一帧视频都是在filesink中写入test.h264文件的。因此在filesink中进行处理。void FileSink::addData(unsigned char const* data, unsigned dataSize,struct timeval presentationTime) 函数的传入参数，unsigned char const* data, unsigned dataSize 分别是一帧的内容和一帧的长度（这个视频帧没有h264的开头0001，void H264VideoFileSink::afterGettingFrame 这个函数中有添加0001的 代码）。FileSink::addData 中获取的一帧视频(接收到的rtp包，有一个mark算一帧，因此NAL nal_unit_type中的6（SEI）、7（SPS）、8（PPS）都是分开的)。关于如何判断帧类型，可以利用可以利用data[1]和0x1f进行位或运算。在用ffmpeg解码时，sps，pps和之后的idr帧是要一起传入解码器的，其他的sei我是直接丢弃，其实也可以sps+pps+sei+idr一起传入ffmpeg。先上一段代码，这段代码用于将需要解码的一帧存入一个缓存区中，之后ffmpeg 解码缓存区的数据。
进行如下定义：
- 
`typedef struct `
- 
`{`
- 
`unsigned char * realbuf;//存储接收到的完整一帧`
- 
`int realsize;//帧长度`
- 
`}mybuf;//存储一帧`
- 
- 
`#define bufnum 200`
- 
`mybuf encodebuf[bufnum];`
- 
`if (fOutFid != NULL && data != NULL) {`
- 
`int iRet=-1; `
- 
`iRet=data[0] & 0x1f;`
- 
`if ((iRet==6||iRet==9)&&(m_start==1))//sei等直接丢弃`
- 
`{`
- 
`// fwrite(data, 1, dataSize, fOutFid);`
- 
`return ;`
- 
`}`
- 
`if ((iRet==7)&&(m_start==1))`
- 
`{`
- 
`m_pos=0;`
- 
`memmove(encodebuf[index].realbuf,sHead,4);`
- 
`encodebuf[index].realsize+=4;`
- 
`memmove(encodebuf[index].realbuf+ encodebuf[index].realsize,data,dataSize);`
- 
`encodebuf[index].realsize+=dataSize;`
- 
`}`
- 
`if ((iRet==8)&&(m_start==1))//pps`
- 
`{`
- 
- 
`memmove(encodebuf[index].realbuf+encodebuf[index].realsize,sHead,4);`
- 
`encodebuf[index].realsize+=4;`
- 
`memmove(encodebuf[index].realbuf+ encodebuf[index].realsize,data,dataSize);`
- 
`encodebuf[index].realsize+=dataSize;`
- 
`}`
- 
`if((iRet==7)&&(m_start==0))//sps`
- 
`{`
- 
`memmove(encodebuf[index].realbuf,sHead,4);`
- 
`encodebuf[index].realsize+=4;`
- 
`// fwrite(sHead,1,4,fOutFid);`
- 
`memmove(encodebuf[index].realbuf+ 4,data,dataSize);`
- 
`encodebuf[index].realsize+=dataSize;`
- 
- 
`m_start++;`
- 
`}`
- 
`if (m_start>0)`
- 
`{`
- 
`// fwrite(data, 1, dataSize, fOutFid);`
- 
`if (iRet!=0&&iRet!=7&&iRet!=8)`
- 
`{`
- 
`memmove(encodebuf[index].realbuf+ encodebuf[index].realsize,sHead,4);`
- 
`encodebuf[index].realsize+=4;`
- 
`memmove(encodebuf[index].realbuf+ encodebuf[index].realsize,data,dataSize);`
- 
`encodebuf[index].realsize+=dataSize;`
- 
`if (iRet==5)//idr`
- 
`{`
- 
`m_pos=1;`
- 
`}`
- 
`if (m_pos==1)`
- 
`{`
- 
`index++;`
- 
`if (encodebuf[index-1].realsize==0)`
- 
`{`
- 
`printf("");`
- 
`}`
- 
- 
`PostThreadMessage(ThreadID,MY_MSG_WRITE,index,0);`
- 
`if (index==bufnum)`
- 
`{`
- 
`index=0;`
- 
`}`
- 
`}`
- 
- 
- 
`}`
- 
- 
`}`
- 
- 
`}`
以上是在live555中针对拼凑解码一帧的修改逻辑。再重复一次：解码的第一帧要把sps+pps+idr传入ffmpeg进行解码，因此要把这三帧拼到一起。再从ffmpeg解码说起，本人对ffmpeg的学习，大多倚仗雷霄骅在csdn上的博客，具体对ffmpeg的讲解就不在赘述，雷神在csdn上对ffmpeg有很好的讲解。简而言之就是初始化ffmpeg 相关的变量后调用avcodec_decode_video2函数解码视频。
直接上解码的那小段代码，说明一下：
- 
`char* input=encodebuf[myindex].realbuf; `
- 
`int nLen=encodebuf[myindex].realsize;`
- 
`AVFrame *frame = NULL;`
- 
`frame = av_frame_alloc();`
- 
`AVPacket packet;`
- 
`av_init_packet(&packet);`
- 
`packet.size = nLen;//一帧的长度`
- 
`packet.data = input;//从缓冲区取出的一帧`
- 
`packet.pts=i_pts;`
- 
`if (!frame) {`
- 
`ret = AVERROR(ENOMEM);`
- 
`}`
- 
`ret = avcodec_decode_video2(dec_ctx, frame,&got_frame, &packet);`
经过以上过程，解码后的yuv数据就到了frame中 。yuv可以用来进行重新编码，也可以直接用来显示：如用opengl或者d3d进行渲染。
    对应该文，上传一份完整的测试代码，用了live555接收实时视频，ffmpeg 解码， x264编码，rtp打包 。代码比较乱，测试用，不过文中所述内容在代码中都有体现。上传的代码因为平时测试用所以有些地方有比较粗暴的修改，比如H264VideoFileSink中删除了一些函数，还有FileSink种的一些修改。
[点击打开链接](http://download.csdn.net/detail/dzhuang123/8434231)
