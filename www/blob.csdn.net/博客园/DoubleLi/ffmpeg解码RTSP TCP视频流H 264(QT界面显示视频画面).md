# ffmpeg解码RTSP/TCP视频流H.264(QT界面显示视频画面) - DoubleLi - 博客园






源码下载地址: [http://download.csdn.net/detail/liukang325/9489952](http://download.csdn.net/detail/liukang325/9489952)

**我用的ffmpeg版本为 ffmpeg-2.1.8.tar.bz2**
版本低了恐怕有些头文件和API找不到。 
在Linux下解压后编译，Linux下编译很简单，我这里生成的动态库： 
./configure –enable-shared 
make 
就能找到各个so动态库文件。 
移动位置后，记得手动链接 一下：

```
ln -s libavcodec.so.55 libavcodec.so
ln -s libavdevice.so.55 libavdevice.so
ln -s libavfilter.so.3 libavfilter.so
ln -s libavformat.so.55 libavformat.so
ln -s libavutil.so.52 libavutil.so
ln -s libswscale.so.2 libswscale.so
```



**QT pro文件中记得加入：**
INCLUDEPATH += ffmpeg/include 
// windows下用这几个 
win32: LIBS += ffmpeg/lib/libavcodec.dll.a ffmpeg/lib/libavfilter.dll.a ffmpeg/lib/libavformat.dll.a ffmpeg/lib/libswscale.dll.a ffmpeg/lib/libavutil.dll.a 
// Linux下用这几个 
LIBS += -lavcodec -lavdevice -lavfilter -lavformat -lavutil -lswscale -L./ffmpeg/so

**我这里对外提供三个接口：**

```
void VideoStream::setUrl(QString url)
{
    m_str_url = url;
}

void VideoStream::startStream()
{
    videoStreamIndex=-1;
    av_register_all();//注册库中所有可用的文件格式和解码器
    avformat_network_init();//初始化网络流格式,使用RTSP网络流时必须先执行
    pAVFormatContext = avformat_alloc_context();//申请一个AVFormatContext结构的内存,并进行简单初始化
    pAVFrame=av_frame_alloc();
    if (this->Init())
    {
        m_timerPlay->start();
    }
}
void VideoStream::stopStream()
{
    m_timerPlay->stop();

    avformat_free_context(pAVFormatContext);
    av_frame_free(&pAVFrame);
    sws_freeContext(pSwsContext);
}
```



**里面与ffmpeg解码相关的私有变量：**

```
QMutex mutex;
    AVPicture  pAVPicture;
    AVFormatContext *pAVFormatContext;
    AVCodecContext *pAVCodecContext;
    AVFrame *pAVFrame;
    SwsContext * pSwsContext;
    AVPacket pAVPacket;
```



**在QT里，用一个QLabel的对象来显示解码后的视频画面：**

```
connect(this,SIGNAL(GetImage(QImage)),this,SLOT(SetImageSlots(QImage)));

...
void VideoStream::SetImageSlots(const QImage &image)
{
    if (image.height()>0){
        QPixmap pix = QPixmap::fromImage(image.scaled(m_i_w,m_i_h));
        m_label->setPixmap(pix);
    }
}
```



**这里用的QTimer来进行一帧帧数据的解码，也可以用一个线程比如QThread来进行解码：**

```
m_timerPlay = new QTimer;
    m_timerPlay->setInterval(10);
    connect(m_timerPlay,SIGNAL(timeout()),this,SLOT(playSlots()));

    m_i_frameFinished = 0;

... 
bool VideoStream::Init()
{
    if(m_str_url.isEmpty())
        return false;
    //打开视频流
    int result=avformat_open_input(&pAVFormatContext, m_str_url.toStdString().c_str(),NULL,NULL);
    if (result<0){
        qDebug()<<"打开视频流失败";
        return false;
    }

    //获取视频流信息
    result=avformat_find_stream_info(pAVFormatContext,NULL);
    if (result<0){
        qDebug()<<"获取视频流信息失败";
        return false;
    }

    //获取视频流索引
    videoStreamIndex = -1;
    for (uint i = 0; i < pAVFormatContext->nb_streams; i++) {
        if (pAVFormatContext->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoStreamIndex = i;
            break;
        }
    }

    if (videoStreamIndex==-1){
        qDebug()<<"获取视频流索引失败";
        return false;
    }

    //获取视频流的分辨率大小
    pAVCodecContext = pAVFormatContext->streams[videoStreamIndex]->codec;
    videoWidth=pAVCodecContext->width;
    videoHeight=pAVCodecContext->height;

    avpicture_alloc(&pAVPicture,PIX_FMT_RGB24,videoWidth,videoHeight);

    AVCodec *pAVCodec;

    //获取视频流解码器
    pAVCodec = avcodec_find_decoder(pAVCodecContext->codec_id);
    pSwsContext = sws_getContext(videoWidth,videoHeight,PIX_FMT_YUV420P,videoWidth,videoHeight,PIX_FMT_RGB24,SWS_BICUBIC,0,0,0);

    //打开对应解码器
    result=avcodec_open2(pAVCodecContext,pAVCodec,NULL);
    if (result<0){
        qDebug()<<"打开解码器失败";
        return false;
    }

    qDebug()<<"初始化视频流成功";
    return true;
}

void VideoStream::playSlots()
{
    //一帧一帧读取视频
    if (av_read_frame(pAVFormatContext, &pAVPacket) >= 0){
        if(pAVPacket.stream_index==videoStreamIndex){
            qDebug()<<"开始解码"<<QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
            avcodec_decode_video2(pAVCodecContext, pAVFrame, &m_i_frameFinished, &pAVPacket);
            if (m_i_frameFinished){
                mutex.lock();
                sws_scale(pSwsContext,(const uint8_t* const *)pAVFrame->data,pAVFrame->linesize,0,videoHeight,pAVPicture.data,pAVPicture.linesize);
                //发送获取一帧图像信号
                QImage image(pAVPicture.data[0],videoWidth,videoHeight,QImage::Format_RGB888);
                emit GetImage(image);
                mutex.unlock();
            }
        }
    }
    av_free_packet(&pAVPacket);//释放资源,否则内存会一直上升
}
```



**备注：**
**头文件包含及注意事项**

```
//必须加以下内容,否则编译不能通过,为了兼容C和C99标准

#ifndef INT64_C
#define INT64_C
#define UINT64_C
#endif

//引入ffmpeg头文件
extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavfilter/avfilter.h>
#include <libswscale/swscale.h>
#include <libavutil/frame.h>
}
```

**启发：**
setUrl(QString url)； 
这里的url 一般情况下是一个RTSP流的播放地址，如rtsp://192.168.1.123:554/stream1 
但也可以是一个TCP流。 
我这边测试的是一个本地的socket流，设url地址为 [http://127.0.0.1:5858](http://127.0.0.1:5858/)
可直接解码播放。









