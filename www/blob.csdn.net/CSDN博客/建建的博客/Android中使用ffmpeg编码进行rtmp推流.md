# Android中使用ffmpeg编码进行rtmp推流 - 建建的博客 - CSDN博客
2018年09月10日 10:08:02[纪建](https://me.csdn.net/u013898698)阅读数：415
要理解RTMP推流，我们就要知道详细原理，这方面的文章有很多，我也看到过学习过很多这样的文章，但是很多都没有详细的去给大家展示，都没有一个完整的流程，使得初学者难以弄懂其中的原理，下面我将详细的来给大家介绍RTMP推流原理以及如何推送到服务器，首先我们了解一下推流的全过程：
![](https://upload-images.jianshu.io/upload_images/10437603-f336c859cf8e89ea.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
image
我们将会分为几个小节来展开：
一. 本文用到的库文件：
1.1 本项目用到的库文件如下图所示，用到了ffmpeg库，以及编码视频的x264，编码音频的fdk-aac，推流使用的rtmp等：
![](https://upload-images.jianshu.io/upload_images/10437603-32f5a1422651e1dd.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/462/format/webp)
image.png
![](https://upload-images.jianshu.io/upload_images/10437603-834e083f509d6231.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/428/format/webp)
image.png
使用静态链接库，最终把这些.a文件打包到libstream中，Android.mk如下
```
LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := avformat
LOCAL_SRC_FILES := $(LOCAL_PATH)/lib/libavformat.a
include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)
LOCAL_MODULE := avcodec
LOCAL_SRC_FILES := $(LOCAL_PATH)/lib/libavcodec.a
include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)
LOCAL_MODULE := swscale
LOCAL_SRC_FILES := $(LOCAL_PATH)/lib/libswscale.a
include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)
LOCAL_MODULE := avutil
LOCAL_SRC_FILES := $(LOCAL_PATH)/lib/libavutil.a
include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)
LOCAL_MODULE := swresample
LOCAL_SRC_FILES := $(LOCAL_PATH)/lib/libswresample.a
include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)
LOCAL_MODULE := postproc
LOCAL_SRC_FILES := $(LOCAL_PATH)/lib/libpostproc.a
include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)
LOCAL_MODULE := x264
LOCAL_SRC_FILES := $(LOCAL_PATH)/lib/libx264.a
include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)
LOCAL_MODULE :=  libyuv
LOCAL_SRC_FILES := $(LOCAL_PATH)/lib/libyuv.a
include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)
LOCAL_MODULE :=  libfdk-aac
#LOCAL_C_INCLUDES += $(LOCAL_PATH)/include/
LOCAL_SRC_FILES := $(LOCAL_PATH)/lib/libfdk-aac.a
include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)
LOCAL_MODULE :=  polarssl
LOCAL_SRC_FILES := $(LOCAL_PATH)/lib/libpolarssl.a
include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)
LOCAL_MODULE :=  rtmp
LOCAL_SRC_FILES := $(LOCAL_PATH)/lib/librtmp.a
include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)
LOCAL_MODULE    := libstream
LOCAL_SRC_FILES := StreamProcess.cpp FrameEncoder.cpp AudioEncoder.cpp wavreader.c RtmpLivePublish.cpp
LOCAL_C_INCLUDES += $(LOCAL_PATH)/include/
LOCAL_STATIC_LIBRARIES := libyuv avformat avcodec swscale avutil swresample postproc x264 libfdk-aac polarssl rtmp
LOCAL_LDLIBS += -L$(LOCAL_PATH)/prebuilt/ -llog -lz -Ipthread
include $(BUILD_SHARED_LIBRARY)
```
具体使用到哪些库中的接口我们将再下面进行细节展示。
二 . 如何从Camera摄像头获取视频流：
2.1 Camera获取视频流，这个就不用多说了，只需要看到这个回调就行了，我们需要获取到这个数据：
```
//CameraSurfaceView.java中
    @Override
    public void onPreviewFrame(byte[] data, Camera camera) {
        camera.addCallbackBuffer(data);
        if (listener != null) {
            listener.onCallback(data);
        }
    }
```
```
//阻塞线程安全队列，生产者和消费者
    private LinkedBlockingQueue<byte[]> mQueue = new LinkedBlockingQueue<>();
...........
@Override
    public void onCallback(final byte[] srcData) {
        if (srcData != null) {
            try {
                mQueue.put(srcData);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
.......
```
2.2 NV21转化为YUV420P数据
我们知道一般的摄像头的数据都是NV21或者是NV12，接下来我们会用到第一个编码库libyuv库，我们先来看看这个消费者怎么从NV21的数据转化为YUV的
```
workThread = new Thread() {
            @Override
            public void run() {
                while (loop && !Thread.interrupted()) {
                    try {
                        //获取阻塞队列中的数据，没有数据的时候阻塞
                        byte[] srcData = mQueue.take();
                        //生成I420(YUV标准格式数据及YUV420P)目标数据，
                        //生成后的数据长度width * height * 3 / 2
                        final byte[] dstData = new byte[scaleWidth * scaleHeight * 3 / 2];
                        final int morientation = mCameraUtil.getMorientation();
                        //压缩NV21(YUV420SP)数据，元素数据位1080 * 1920，很显然
                        //这样的数据推流会很占用带宽，我们压缩成480 * 640 的YUV数据
                        //为啥要转化为YUV420P数据？因为是在为转化为H264数据在做
                        //准备，NV21不是标准的，只能先通过转换，生成标准YUV420P数据，
                        //然后把标准数据encode为H264流
                        StreamProcessManager.compressYUV(srcData, mCameraUtil.getCameraWidth(), mCameraUtil.getCameraHeight(), dstData, scaleHeight, scaleWidth, 0, morientation, morientation == 270);
                        //进行YUV420P数据裁剪的操作，测试下这个借口，
                        //我们可以对数据进行裁剪，裁剪后的数据也是I420数据，
                        //我们采用的是libyuv库文件
                        //这个libyuv库效率非常高，这也是我们用它的原因
                        final byte[] cropData = new byte[cropWidth * cropHeight * 3 / 2];
                        StreamProcessManager.cropYUV(dstData, scaleWidth, scaleHeight, cropData, cropWidth, cropHeight, cropStartX, cropStartY);
                        //自此，我们得到了YUV420P标准数据，这个过程实际上就是NV21转化为YUV420P数据
                        //注意，有些机器是NV12格式，只是数据存储不一样，我们一样可以用libyuv库的接口转化
                        if (yuvDataListener != null) {
                            yuvDataListener.onYUVDataReceiver(cropData, cropWidth, cropHeight);
                        }
                        //设置为true，我们把生成的YUV文件用播放器播放一下，看我们
                        //的数据是否有误，起调试作用
                        if (SAVE_FILE_FOR_TEST) {
                            fileManager.saveFileData(cropData);
                        }
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                        break;
                    }
                }
            }
        };
```
2.3 介绍一下摄像头的数据流格式
视频流的转换，android中一般摄像头的格式是NV21或者是NV12，它们都是YUV420sp的一种，那么什么是YUV格式呢？
何为YUV格式，有三个分量，Y表示明亮度，也就是灰度值，U和V则表示色度，即影像色彩饱和度，用于指定像素的颜色，（直接点就是Y是亮度信息，UV是色彩信息），YUV格式分为两大类，planar和packed两种：
```
对于planar的YUV格式，先连续存储所有像素点Y，紧接着存储所有像素点U，随后所有像素点V
对于packed的YUV格式，每个像素点YUV是连续交替存储的
```
YUV格式为什么后面还带数字呢，比如YUV 420，444，442
YUV444：每一个Y对应一组UV分量
YUV422：每两个Y共用一组UV分量
YUV420：每四个Y公用一组UV分量
实际上NV21，NV12就是属于YUV420，是一种two-plane模式，即Y和UV分为两个Plane，UV为交错存储，他们都属于YUV420SP，举个例子就会很清晰了
```
NV21格式数据排列方式是YYYYYYYY（w*h）VUVUVUVU(w*h/2)，
对于NV12的格式，排列方式是YYYYYYYY（w*h）UVUVUVUV（w*h/2）
```
正如代码注释中所说的那样，我们以标准的YUV420P为例，对于这样的格式，我们要取出Y，U，V这三个分量，我们看怎么取？
```
比如480 * 640大小的图片，其字节数为 480 * 640 * 3 >> 1个字节
Y分量：480 * 640个字节
U分量：480 * 640 >>2个字节
V分量：480 * 640 >>2个字节，加起来就为480 * 640 * 3 >> 1个字节
存储都是行优先存储，三部分之间顺序是YUV依次存储，即
0 ~ 480*640是Y分量；480 * 640 ~ 480 * 640 * 5 / 4为U分量；480 * 640 * 5 / 4 ~ 480 * 640 * 3 / 2是V分量，
```
记住这个计算方法，等下在JNI中马上会体现出来
那么YUV420SP和YUV420P的区别在哪里呢？显然Y的排序是完全相同的，但是UV排列上原理是完全不同的，420P它是先吧U存放完后，再放V，也就是说UV是连续的，而420SP它是UV，UV这样交替存放：
YUV420SP格式：
![](https://upload-images.jianshu.io/upload_images/10437603-25f0efeaaae6fc1d.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/370/format/webp)
image.png
YUV420P格式：
![](https://upload-images.jianshu.io/upload_images/10437603-895c99d794020bba.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/371/format/webp)
image.png
所以NV21(YUV420SP)的数据如下:
同样的以480 * 640大小的图片为例，其字节数为 480 * 640 * 3 >> 1个字节
Y分量：480 * 640个字节
UV分量：480 * 640 >>1个字节(注意，我们没有把UV分量分开)
加起来就为480 * 640 * 3 >> 1个字节
下面我们来看看两个JNI函数，这个是摄像头转化的两个最关键的函数
```
/**
     * NV21转化为YUV420P数据
     * @param src         原始数据
     * @param width       原始数据宽度
     * @param height      原始数据高度
     * @param dst         生成数据
     * @param dst_width   生成数据宽度
     * @param dst_height  生成数据高度
     * @param mode        模式
     * @param degree      角度
     * @param isMirror    是否镜像
     * @return
     */
    public static native int compressYUV(byte[] src, int width, int height, byte[] dst, int dst_width, int dst_height, int mode, int degree, boolean isMirror);
    /**
     * YUV420P数据的裁剪
     * @param src         原始数据
     * @param width       原始数据宽度
     * @param height      原始数据高度
     * @param dst         生成数据
     * @param dst_width   生成数据宽度
     * @param dst_height  生成数据高度
     * @param left        裁剪的起始x点
     * @param top         裁剪的起始y点
     * @return
     */
    public static native int cropYUV(byte[] src, int width, int height, byte[] dst, int dst_width, int dst_height, int left, int top);
```
再看一看具体实现
```
JNIEXPORT jint JNICALL Java_com_riemannlee_liveproject_StreamProcessManager_compressYUV
    (JNIEnv *env, jclass type,
     jbyteArray src_, jint width,
     jint height, jbyteArray dst_,
     jint dst_width, jint dst_height,
     jint mode, jint degree,
     jboolean isMirror) {
    jbyte *Src_data = env->GetByteArrayElements(src_, NULL);
    jbyte *Dst_data = env->GetByteArrayElements(dst_, NULL);
    //nv21转化为i420(标准YUV420P数据) 这个temp_i420_data大小是和Src_data是一样的
    nv21ToI420(Src_data, width, height, temp_i420_data);
    //进行缩放的操作，这个缩放，会把数据压缩
    scaleI420(temp_i420_data, width, height, temp_i420_data_scale, dst_width, dst_height, mode);
    //如果是前置摄像头，进行镜像操作
    if (isMirror) {
        //进行旋转的操作
        rotateI420(temp_i420_data_scale, dst_width, dst_height, temp_i420_data_rotate, degree);
        //因为旋转的角度都是90和270，那后面的数据width和height是相反的
        mirrorI420(temp_i420_data_rotate, dst_height, dst_width, Dst_data);
    } else {
        //进行旋转的操作
        rotateI420(temp_i420_data_scale, dst_width, dst_height, Dst_data, degree);
    }
    env->ReleaseByteArrayElements(dst_, Dst_data, 0);
    env->ReleaseByteArrayElements(src_, Src_data, 0);
    return 0;
}
```
我们从java层传递过来的参数可以看到，原始数据是1080 * 1920，先转为1080 * 1920的标准的YUV420P的数据，下面的代码就是上面我举的例子，如何拆分YUV420P的Y，U，V分量和如何拆分YUV420SP的Y，UV分量，最后调用libyuv库的libyuv::NV21ToI420数据就完成了转换；然后进行缩放，调用了libyuv::I420Scale的函数完成转换
```
//NV21转化为YUV420P数据
void nv21ToI420(jbyte *src_nv21_data, jint width, jint height, jbyte *src_i420_data) {
    //Y通道数据大小
    jint src_y_size = width * height;
    //U通道数据大小
    jint src_u_size = (width >> 1) * (height >> 1);
    //NV21中Y通道数据
    jbyte *src_nv21_y_data = src_nv21_data;
    //由于是连续存储的Y通道数据后即为VU数据，它们的存储方式是交叉存储的
    jbyte *src_nv21_vu_data = src_nv21_data + src_y_size;
    //YUV420P中Y通道数据
    jbyte *src_i420_y_data = src_i420_data;
    //YUV420P中U通道数据
    jbyte *src_i420_u_data = src_i420_data + src_y_size;
    //YUV420P中V通道数据
    jbyte *src_i420_v_data = src_i420_data + src_y_size + src_u_size;
    //直接调用libyuv中接口，把NV21数据转化为YUV420P标准数据，此时，它们的存储大小是不变的
    libyuv::NV21ToI420((const uint8 *) src_nv21_y_data, width,
                       (const uint8 *) src_nv21_vu_data, width,
                       (uint8 *) src_i420_y_data, width,
                       (uint8 *) src_i420_u_data, width >> 1,
                       (uint8 *) src_i420_v_data, width >> 1,
                       width, height);
}
//进行缩放操作，此时是把1080 * 1920的YUV420P的数据 ==> 480 * 640的YUV420P的数据
void scaleI420(jbyte *src_i420_data, jint width, jint height, jbyte *dst_i420_data, jint dst_width,
               jint dst_height, jint mode) {
    //Y数据大小width*height，U数据大小为1/4的width*height，V大小和U一样，一共是3/2的width*height大小
    jint src_i420_y_size = width * height;
    jint src_i420_u_size = (width >> 1) * (height >> 1);
    //由于是标准的YUV420P的数据，我们可以把三个通道全部分离出来
    jbyte *src_i420_y_data = src_i420_data;
    jbyte *src_i420_u_data = src_i420_data + src_i420_y_size;
    jbyte *src_i420_v_data = src_i420_data + src_i420_y_size + src_i420_u_size;
    //由于是标准的YUV420P的数据，我们可以把三个通道全部分离出来
    jint dst_i420_y_size = dst_width * dst_height;
    jint dst_i420_u_size = (dst_width >> 1) * (dst_height >> 1);
    jbyte *dst_i420_y_data = dst_i420_data;
    jbyte *dst_i420_u_data = dst_i420_data + dst_i420_y_size;
    jbyte *dst_i420_v_data = dst_i420_data + dst_i420_y_size + dst_i420_u_size;
    //调用libyuv库，进行缩放操作
    libyuv::I420Scale((const uint8 *) src_i420_y_data, width,
                      (const uint8 *) src_i420_u_data, width >> 1,
                      (const uint8 *) src_i420_v_data, width >> 1,
                      width, height,
                      (uint8 *) dst_i420_y_data, dst_width,
                      (uint8 *) dst_i420_u_data, dst_width >> 1,
                      (uint8 *) dst_i420_v_data, dst_width >> 1,
                      dst_width, dst_height,
                      (libyuv::FilterMode) mode);
}
```
至此，我们就把摄像头的NV21数据转化为YUV420P的标准数据了，这样，我们就可以把这个数据流转化为H264了，接下来，我们来看看如何把YUV420P流数据转化为h264数据，从而为推流做准备
三 . 标准YUV420P数据编码为H264
多说无用，直接上代码
3.1 代码如何实现h264编码的：
```
/**
 * 编码类MediaEncoder，主要是把视频流YUV420P格式编码为h264格式,把PCM裸音频转化为AAC格式
 */
public class MediaEncoder {
    private static final String TAG = "MediaEncoder";
    private Thread videoEncoderThread, audioEncoderThread;
    private boolean videoEncoderLoop, audioEncoderLoop;
    //视频流队列
    private LinkedBlockingQueue<VideoData> videoQueue;
    //音频流队列
    private LinkedBlockingQueue<AudioData> audioQueue;
.........
//摄像头的YUV420P数据，put到队列中，生产者模型
    public void putVideoData(VideoData videoData) {
        try {
            videoQueue.put(videoData);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
.........
 videoEncoderThread = new Thread() {
            @Override
            public void run() {
                //视频消费者模型，不断从队列中取出视频流来进行h264编码
                while (videoEncoderLoop && !Thread.interrupted()) {
                    try {
                        //队列中取视频数据
                        VideoData videoData = videoQueue.take();
                        fps++;
                        byte[] outbuffer = new byte[videoData.width * videoData.height];
                        int[] buffLength = new int[10];
                        //对YUV420P进行h264编码，返回一个数据大小，里面是编码出来的h264数据
                        int numNals = StreamProcessManager.encoderVideoEncode(videoData.videoData, videoData.videoData.length, fps, outbuffer, buffLength);
                        //Log.e("RiemannLee", "data.length " +  videoData.videoData.length + " h264 encode length " + buffLength[0]);
                        if (numNals > 0) {
                            int[] segment = new int[numNals];
                            System.arraycopy(buffLength, 0, segment, 0, numNals);
                            int totalLength = 0;
                            for (int i = 0; i < segment.length; i++) {
                                totalLength += segment[i];
                            }
                            //Log.i("RiemannLee", "###############totalLength " + totalLength);
                            //编码后的h264数据
                            byte[] encodeData = new byte[totalLength];
                            System.arraycopy(outbuffer, 0, encodeData, 0, encodeData.length);
                            if (sMediaEncoderCallback != null) {
                                sMediaEncoderCallback.receiveEncoderVideoData(encodeData, encodeData.length, segment);
                            }
                            //我们可以把数据在java层保存到文件中，看看我们编码的h264数据是否能播放，h264裸数据可以在VLC播放器中播放
                            if (SAVE_FILE_FOR_TEST) {
                                videoFileManager.saveFileData(encodeData);
                            }
                        }
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                        break;
                    }
                }
            }
        };
        videoEncoderLoop = true;
        videoEncoderThread.start();
    }
```
这个就是如何把YUV420P数据转化为h264流，主要代码是这个JNI函数，接下来我们看是如何编码成h264的，编码函数如下：
```
/**
     * 编码视频数据接口
     * @param srcFrame      原始数据(YUV420P数据)
     * @param frameSize     帧大小
     * @param fps           fps
     * @param dstFrame      编码后的数据存储
     * @param outFramewSize 编码后的数据大小
     * @return
     */
    public static native int encoderVideoEncode(byte[] srcFrame, int frameSize, int fps, byte[] dstFrame, int[] outFramewSize);
```
JNI中视频流的编码接口，我们看到的是初始化一个FrameEncoder类，然后调用这个类的encodeFrame接口去编码
```
//初始化视频编码
JNIEXPORT jint JNICALL Java_com_riemannlee_liveproject_StreamProcessManager_encoderVideoinit
        (JNIEnv *env, jclass type, jint jwidth, jint jheight, jint joutwidth, jint joutheight)
{
    frameEncoder = new FrameEncoder();
    frameEncoder->setInWidth(jwidth);
    frameEncoder->setInHeight(jheight);
    frameEncoder->setOutWidth(joutwidth);
    frameEncoder->setOutHeight(joutheight);
    frameEncoder->setBitrate(128);
    frameEncoder->open();
    return 0;
}
//视频编码主要函数，注意JNI函数GetByteArrayElements和ReleaseByteArrayElements成对出现，否则回内存泄露
JNIEXPORT jint JNICALL Java_com_riemannlee_liveproject_StreamProcessManager_encoderVideoEncode
        (JNIEnv *env, jclass type, jbyteArray jsrcFrame, jint jframeSize, jint counter, jbyteArray jdstFrame, jintArray jdstFrameSize)
{
    jbyte *Src_data = env->GetByteArrayElements(jsrcFrame, NULL);
    jbyte *Dst_data = env->GetByteArrayElements(jdstFrame, NULL);
    jint *dstFrameSize = env->GetIntArrayElements(jdstFrameSize, NULL);
    int numNals = frameEncoder->encodeFrame((char*)Src_data, jframeSize, counter, (char*)Dst_data, dstFrameSize);
    env->ReleaseByteArrayElements(jdstFrame, Dst_data, 0);
    env->ReleaseByteArrayElements(jsrcFrame, Src_data, 0);
    env->ReleaseIntArrayElements(jdstFrameSize, dstFrameSize, 0);
    return numNals;
}
```
下面我们来详细的分析FrameEncoder这个C++类，这里我们用到了多个库，第一个就是鼎鼎大名的ffmpeg库，还有就是X264库，下面我们先来了解一下h264的文件结构，这样有利于我们理解h264的编码流程
3.2 h264我们必须知道的一些概念：
```
首先我们来介绍h264字节流，先来了解下面几个概念，h264由哪些东西组成呢？
1.VCL   video coding layer    视频编码层；
2.NAL   network abstraction layer   网络提取层；
其中，VCL层是对核心算法引擎，块，宏块及片的语法级别的定义，他最终输出编码完的数据 SODB
SODB：String of Data Bits，数据比特串，它是最原始的编码数据
RBSP：Raw Byte Sequence Payload，原始字节序载荷，它是在SODB的后面添加了结尾比特和若干比特0，以便字节对齐
EBSP：Encapsulate Byte Sequence Payload，扩展字节序列载荷，它是在RBSP基础上添加了防校验字节0x03后得到的。
关系大致如下：
SODB + RBSP STOP bit + 0bits = RBSP
RBSP part1+0x03+RBSP part2+0x03+…+RBSP partn = EBSP
NALU Header+EBSP=NALU（NAL单元）
start code+NALU+…+start code+NALU=H.264 Byte Stream
```
NALU头结构
```
长度：1byte(1个字节)
forbidden_bit(1bit) + nal_reference_bit(2bit) + nal_unit_type(5bit)
1.  forbidden_bit：禁止位，初始为0，当网络发现NAL单元有比特错误时可设置该比特为1,以便接收方纠错或
丢掉该单元。
2.  nal_reference_bit：nal重要性指示，标志该NAL单元的重要性，值越大，越重要，解码器在解码处理不过来
的时候，可以丢掉重要性为0的NALU。
```
NALU类型结构图：
![](https://upload-images.jianshu.io/upload_images/10437603-798feb59e4119926.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/561/format/webp)
image.png
其中，nal_unit_type为1， 2， 3， 4， 5及12的NAL单元称为VCL的NAL单元，其他类型的NAL单元为非VCL的NAL单元。
对应的代码定义如下
```
public static final int NAL_UNKNOWN     = 0;
    public static final int NAL_SLICE       = 1; /* 非关键帧 */
    public static final int NAL_SLICE_DPA   = 2;
    public static final int NAL_SLICE_DPB   = 3;
    public static final int NAL_SLICE_DPC   = 4;
    public static final int NAL_SLICE_IDR   = 5; /* 关键帧 */
    public static final int NAL_SEI         = 6;
    public static final int NAL_SPS         = 7; /* SPS */
    public static final int NAL_PPS         = 8; /* PPS */
    public static final int NAL_AUD         = 9;
    public static final int NAL_FILLER      = 12;
```
由上面我们可以知道，h264字节流，就是由一些start code + NALU组成的，要组成一个NALU单元，首先要有原始数据，称之为SODB，它是原始的H264数据编码得到到，不包括3字节(0x000001)/4字节（0x00000001）的start code，也不会包括1字节的NALU头， NALU头部信息包括了一些基础信息，比如NALU类型。
ps：起始码包括两种，3字节0x000001和4字节0x00000001，在sps和pps和Access Unit的第一个NALU使用4字节起始码，其余情况均使用3字节起始码
在 H264 SPEC 中，RBSP 定义如下：
在SODB结束处添加表示结束的bit 1来表示SODB已经结束，因此添加的bit 1成为rbsp_stop_one_bit，RBSP也需要字节对齐，为此需要在rbsp_stop_one_bit后添加若干0补齐，简单来说，要在SODB后面追加两样东西就形成了RBSP
rbsp_stop_one_bit = 1
rbsp_alignment_zero_bit(s) = 0(s)
RBSP的生成过程：
![](https://upload-images.jianshu.io/upload_images/10437603-a98979b5a66b5bf8.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/538/format/webp)
image.png
即RBSP最后一个字节包含SODB最后几个比特，以及trailing bits其中，第一个比特位1，其余的比特位0，保证字节对齐，最后再结尾处添加0x0000，即CABAC_ZERO_WORD，从而形成 RBSP。
EBSP的生成过程：NALU数据+起始码就形成了AnnexB格式(下面有介绍H264的两种格式，AnnexB为常用的格式)，起始码包括两种，0x000001和0x00000001，为了不让NALU的主体和起始码之间产生竞争，在对RBSP进行扫描的时候，如果遇到连续两个0x00字节，则在该两个字节后面添加一个0x03字节，在解码的时候将该0x03字节去掉，也称为脱壳操作。解码器在解码时，首先逐个字节读取NAL的数据，统计NAL的长度，然后再开始解码。
替换规则如下：
0x000000 => 0x00000300
0x000001 => 0x00000301
0x000002 => 0x00000302
0x000003 => 0x00000303
3.3 下面我们找一个h264文件来看看
![](https://upload-images.jianshu.io/upload_images/10437603-d482273d7175b030.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/967/format/webp)
image.png
```
00 00 00 01 67 ... 这个为SPS，67为NALU Header，有type信息，后面即为我们说的EBSP
00 00 00 01 68 ...  这个为PPS
00 00 01 06 ...  为SEI补充增强信息
00 00 01 65...  为IDR关键帧，图像中的编码slice
```
![](https://upload-images.jianshu.io/upload_images/10437603-186f6583ae0b2a5b.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/974/format/webp)
image.png
对于这个SPS集合，从67type后开始计算，
即42 c0 33 a6 80 b4 1e 68 40 00 00 03 00 40 00 00 0c a3 c6 0c a8
正如前面的描述，解码的时候直接03 这个03是竞争检测
![](https://upload-images.jianshu.io/upload_images/10437603-81f599069abb54fc.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/969/format/webp)
image.png
从前面我们分析知道，VCL层出来的是编码完的视频帧数据，这些帧可能是I，B,P帧，而且这些帧可能属于不同的序列，在这同一个序列还有相对应的一套序列参数集和图片参数集，所以要完成视频的解码，不仅需要传输VCL层编码出来的视频帧数据，还需要传输序列参数集，图像参数集等数据。
参数集：包括序列参数集SPS和图像参数集PPS
SPS：包含的是针对一连续编码视频序列的参数，如标识符seq_parameter_set_id，帧数以及POC的约束，参数帧数目，解码图像尺寸和帧场编码模式选择标识等等
PPS：对应的是一个序列中某一副图像或者某几幅图像，其参数如标识符pic_parameter_set_id、可选的 seq_parameter_set_id、熵编码模式选择标识，片组数目，初始量化参数和去方块滤波系数调整标识等等
数据分割：组成片的编码数据存放在3个独立的DP（数据分割A,B,C）中，各自包含一个编码片的子集，
分割A包含片头和片中宏块头数据
分割Ｂ包含帧内和 SI 片宏块的编码残差数据。
分割 C包含帧间宏块的编码残差数据。
每个分割可放在独立的 NAL 单元并独立传输。
NALU的顺序要求
H264/AVC标准对送到解码器的NAL单元是由严格要求的，如果NAL单元的顺序是混乱的，必须将其重新依照规范组织后送入解码器，否则不能正确解码
```
1.  序列参数集NAL单元
必须在传送所有以此参数集为参考的其它NAL单元之前传送，不过允许这些NAL单元中中间出现重复的序列参数集合NAL单元。
所谓重复的详细解释为：序列参数集NAL单元都有其专门的标识，如果两个序列参数集NAL单元的标识相同，就可以认为后一个只不过是前一个的拷贝，而非新的序列参数集
2.  图像参数集NAL单元
必须在所有此参数集为参考的其它NAL单元之前传送，不过允许这些NAL单元中间出现重复的图像参数集NAL单元，这一点与上述的序列参数集NAL单元是相同的。
3.  不同基本编码图像中的片段（slice）单元和数据划分片段（data partition）单元在顺序上不可以相互交叉，即不允许属于某一基本编码图像的一系列片段（slice）单元和数据划分片段（data partition）单元中忽然出现另一个基本编码图像的片段（slice）单元片段和数据划分片段（data partition）单元。
4.  参考图像的影响：如果一幅图像以另一幅图像为参考，则属于前者的所有片段（slice）单元和数据划分片段（data partition）单元必须在属于后者的片段和数据划分片段之后，无论是基本编码图像还是冗余编码图像都必须遵守这个规则。
5.  基本编码图像的所有片段（slice）单元和数据划分片段（data partition）单元必须在属于相应冗余编码图像的片段（slice）单元和数据划分片段（data partition）单元之前。
6.  如果数据流中出现了连续的无参考基本编码图像，则图像序号小的在前面。
7.  如果arbitrary_slice_order_allowed_flag置为1，一个基本编码图像中的片段（slice）单元和数据划分片段（data partition）单元的顺序是任意的，如果arbitrary_slice_order_allowed_flag置为零，则要按照片段中第一个宏块的位置来确定片段的顺序，若使用数据划分，则A类数据划分片段在B类数据划分片段之前，B类数据划分片段在C类数据划分片段之前，而且对应不同片段的数据划分片段不能相互交叉，也不能与没有数据划分的片段相互交叉。
8.  如果存在SEI（补充增强信息）单元的话，它必须在它所对应的基本编码图像的片段（slice）单元和数据划分片段（data partition）单元之前，并同时必须紧接在上一个基本编码图像的所有片段（slice）单元和数据划分片段（data partition）单元后边。假如SEI属于多个基本编码图像，其顺序仅以第一个基本编码图像为参照。
9.  如果存在图像分割符的话，它必须在所有SEI 单元、基本编码图像的所有片段slice）单元和数据划分片段（data partition）单元之前，并且紧接着上一个基本编码图像那些NAL单元。
10.  如果存在序列结束符，且序列结束符后还有图像，则该图像必须是IDR（即时解码器刷新）图像。序列结束符的位置应当在属于这个IDR图像的分割符、SEI 单元等数据之前，且紧接着前面那些图像的NAL单元。如果序列结束符后没有图像了，那么它的就在比特流中所有图像数据之后。
11.  流结束符在比特流中的最后。
```
h264有两种封装，
一种是Annexb模式，传统模式，有startcode，SPS和PPS是在ES中
一种是mp4模式，一般mp4 mkv会有，没有startcode，SPS和PPS以及其它信息被封装在container中，每一个frame前面是这个frame的长度
很多解码器只支持annexb这种模式，因此需要将mp4做转换
我们讨论的是第一种Annexb传统模式，
3.4 下面我们直接看代码，了解一下如何使用X264来编码h264文件
```
x264_param_default_preset()：为了方便使用x264，只需要根据编码速度的要求和视频质量的要求选择模型，
并修改部分视频参数即可
x264_picture_alloc()：为图像结构体x264_picture_t分配内存。
x264_encoder_open()：打开编码器。
x264_encoder_encode()：编码一帧图像。
x264_encoder_close()：关闭编码器。
x264_picture_clean()：释放x264_picture_alloc()申请的资源。
 
存储数据的结构体如下所示。
x264_picture_t：存储压缩编码前的像素数据。
x264_nal_t：存储压缩编码后的码流数据。
下面介绍几个重要的结构体
/********************************************************************************************
 x264_image_t 结构用于存放一帧图像实际像素数据。该结构体定义在x264.h中
*********************************************************************************************/
typedef struct
{
    int     i_csp;          // 设置彩色空间，通常取值 X264_CSP_I420，所有可能取值定义在x264.h中
    int     i_plane;        // 图像平面个数，例如彩色空间是YUV420格式的，此处取值3
    int     i_stride[4];    // 每个图像平面的跨度，也就是每一行数据的字节数
    uint8_t *plane[4];      // 每个图像平面存放数据的起始地址, plane[0]是Y平面，
                            // plane[1]和plane[2]分别代表U和V平面
}  x264_image_t;
/********************************************************************************************
x264_picture_t 结构体描述视频帧的特征，该结构体定义在x264.h中。
*********************************************************************************************/
typedef struct
{
int   i_type;           // 帧的类型，取值有X264_TYPE_KEYFRAME X264_TYPE_P
                        // X264_TYPE_AUTO等。初始化为auto，则在编码过程自行控制。
int   i_qpplus1;        // 此参数减1代表当前帧的量化参数值
int   i_pic_struct;     // 帧的结构类型，表示是帧还是场，是逐行还是隔行，
                        // 取值为枚举值 pic_struct_e，定义在x264.h中
int   b_keyframe;       // 输出：是否是关键帧
int64_t   i_pts;        // 一帧的显示时间戳
int64_t   i_dts;        // 输出：解码时间戳。当一帧的pts非常接近0时，该dts值可能为负。
/* 编码器参数设置，如果为NULL则表示继续使用前一帧的设置。某些参数
   (例如aspect ratio) 由于收到H264本身的限制，只能每隔一个GOP才能改变。
   这种情况下，如果想让这些改变的参数立即生效，则必须强制生成一个IDR帧。*/ 
x264_param_t    *param;
x264_image_t     img;    // 存放一帧图像的真实数据
x264_image_properties_t    prop;
x264_hrd_t    hrd_timing;// 输出：HRD时间信息，仅当i_nal_hrd设置了才有效
void    *opaque;         // 私有数据存放区，将输入数据拷贝到输出帧中
} x264_picture_t ;
/****************************************************************************************************************
x264_nal_t中的数据在下一次调用x264_encoder_encode之后就无效了，因此必须在调用
x264_encoder_encode 或 x264_encoder_headers 之前使用或拷贝其中的数据。
*****************************************************************************************************************/
typedef struct
{
int  i_ref_idc;        // Nal的优先级
int  i_type;           // Nal的类型
int  b_long_startcode; // 是否采用长前缀码0x00000001
int  i_first_mb;       // 如果Nal为一条带，则表示该条带第一个宏块的指数
int  i_last_mb;        // 如果Nal为一条带，则表示该条带最后一个宏块的指数
int  i_payload;        // payload 的字节大小
uint8_t *p_payload;    // 存放编码后的数据，已经封装成Nal单元
} x264_nal_t;
```
再来看看编码h264源码
```
//初始化视频编码
JNIEXPORT jint JNICALL Java_com_riemannlee_liveproject_StreamProcessManager_encoderVideoinit
        (JNIEnv *env, jclass type, jint jwidth, jint jheight, jint joutwidth, jint joutheight)
{
    frameEncoder = new FrameEncoder();
    frameEncoder->setInWidth(jwidth);
    frameEncoder->setInHeight(jheight);
    frameEncoder->setOutWidth(joutwidth);
    frameEncoder->setOutHeight(joutheight);
    frameEncoder->setBitrate(128);
    frameEncoder->open();
    return 0;
}
FrameEncoder.cpp 源文件
//供测试文件使用,测试的时候打开
//#define ENCODE_OUT_FILE_1
//供测试文件使用
//#define ENCODE_OUT_FILE_2
FrameEncoder::FrameEncoder() : in_width(0), in_height(0), out_width(
        0), out_height(0), fps(0), encoder(NULL), num_nals(0) {
#ifdef ENCODE_OUT_FILE_1
    const char *outfile1 = "/sdcard/2222.h264";
    out1 = fopen(outfile1, "wb");
#endif
#ifdef ENCODE_OUT_FILE_2
    const char *outfile2 = "/sdcard/3333.h264";
    out2 = fopen(outfile2, "wb");
#endif
}
bool FrameEncoder::open() {
    int r = 0;
    int nheader = 0;
    int header_size = 0;
    if (!validateSettings()) {
        return false;
    }
    if (encoder) {
        LOGI("Already opened. first call close()");
        return false;
    }
    // set encoder parameters
    setParams();
    //按照色度空间分配内存，即为图像结构体x264_picture_t分配内存，并返回内存的首地址作为指针
    //i_csp(图像颜色空间参数，目前只支持I420/YUV420)为X264_CSP_I420
    x264_picture_alloc(&pic_in, params.i_csp, params.i_width, params.i_height);
    //create the encoder using our params 打开编码器
    encoder = x264_encoder_open(¶ms);
    if (!encoder) {
        LOGI("Cannot open the encoder");
        close();
        return false;
    }
    // write headers
    r = x264_encoder_headers(encoder, &nals, &nheader);
    if (r < 0) {
        LOGI("x264_encoder_headers() failed");
        return false;
    }
    return true;
}
//编码h264帧
int FrameEncoder::encodeFrame(char* inBytes, int frameSize, int pts,
                               char* outBytes, int *outFrameSize) {
    //YUV420P数据转化为h264
    int i420_y_size = in_width * in_height;
    int i420_u_size = (in_width >> 1) * (in_height >> 1);
    int i420_v_size = i420_u_size;
    uint8_t *i420_y_data = (uint8_t *)inBytes;
    uint8_t *i420_u_data = (uint8_t *)inBytes + i420_y_size;
    uint8_t *i420_v_data = (uint8_t *)inBytes + i420_y_size + i420_u_size;
    //将Y,U,V数据保存到pic_in.img的对应的分量中，还有一种方法是用AV_fillPicture和sws_scale来进行变换
    memcpy(pic_in.img.plane[0], i420_y_data, i420_y_size);
    memcpy(pic_in.img.plane[1], i420_u_data, i420_u_size);
    memcpy(pic_in.img.plane[2], i420_v_data, i420_v_size);
    // and encode and store into pic_out
    pic_in.i_pts = pts;
    //最主要的函数，x264编码，pic_in为x264输入，pic_out为x264输出
    int frame_size = x264_encoder_encode(encoder, &nals, &num_nals, &pic_in,
                                         &pic_out);
    if (frame_size) {
        /*Here first four bytes proceeding the nal unit indicates frame length*/
        int have_copy = 0;
        //编码后，h264数据保存为nal了，我们可以获取到nals[i].type的类型判断是sps还是pps
        //或者是否是关键帧，nals[i].i_payload表示数据长度，nals[i].p_payload表示存储的数据
        //编码后，我们按照nals[i].i_payload的长度来保存copy h264数据的，然后抛给java端用作
        //rtmp发送数据，outFrameSize是变长的，当有sps pps的时候大于1，其它时候值为1
        for (int i = 0; i < num_nals; i++) {
            outFrameSize[i] = nals[i].i_payload;
            memcpy(outBytes + have_copy, nals[i].p_payload, nals[i].i_payload);
            have_copy += nals[i].i_payload;
        }
#ifdef ENCODE_OUT_FILE_1
        fwrite(outBytes, 1, frame_size, out1);
#endif
#ifdef ENCODE_OUT_FILE_2
        for (int i = 0; i < frame_size; i++) {
            outBytes[i] = (char) nals[0].p_payload[i];
        }
        fwrite(outBytes, 1, frame_size, out2);
        *outFrameSize = frame_size;
#endif
        return num_nals;
    }
    return -1;
}
```
最后，我们来看看抛往java层的h264数据，在MediaEncoder.java中，函数startVideoEncode：
```
public void startVideoEncode() {
        if (videoEncoderLoop) {
            throw new RuntimeException("必须先停止");
        }
        videoEncoderThread = new Thread() {
            @Override
            public void run() {
                //视频消费者模型，不断从队列中取出视频流来进行h264编码
                while (videoEncoderLoop && !Thread.interrupted()) {
                    try {
                        //队列中取视频数据
                        VideoData videoData = videoQueue.take();
                        fps++;
                        byte[] outbuffer = new byte[videoData.width * videoData.height];
                        int[] buffLength = new int[10];
                        //对YUV420P进行h264编码，返回一个数据大小，里面是编码出来的h264数据
                        int numNals = StreamProcessManager.encoderVideoEncode(videoData.videoData, videoData.videoData.length, fps, outbuffer, buffLength);
                        //Log.e("RiemannLee", "data.length " +  videoData.videoData.length + " h264 encode length " + buffLength[0]);
                        if (numNals > 0) {
                            int[] segment = new int[numNals];
                            System.arraycopy(buffLength, 0, segment, 0, numNals);
                            int totalLength = 0;
                            for (int i = 0; i < segment.length; i++) {
                                totalLength += segment[i];
                            }
                            //Log.i("RiemannLee", "###############totalLength " + totalLength);
                            //编码后的h264数据
                            byte[] encodeData = new byte[totalLength];
                            System.arraycopy(outbuffer, 0, encodeData, 0, encodeData.length);
                            if (sMediaEncoderCallback != null) {
                                sMediaEncoderCallback.receiveEncoderVideoData(encodeData, encodeData.length, segment);
                            }
                            //我们可以把数据在java层保存到文件中，看看我们编码的h264数据是否能播放，h264裸数据可以在VLC播放器中播放
                            if (SAVE_FILE_FOR_TEST) {
                                videoFileManager.saveFileData(encodeData);
                            }
                        }
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                        break;
                    }
                }
            }
        };
        videoEncoderLoop = true;
        videoEncoderThread.start();
    }
```
此时，h264数据已经出来了，我们就实现了YUV420P的数据到H264数据的编码，接下来，我们再来看看音频数据。
3.5 android音频数据如何使用fdk-aac库来编码音频，转化为AAC数据的，直接上代码
```
public class AudioRecoderManager {
    private static final String TAG = "AudioRecoderManager";
    // 音频获取
    private final static int SOURCE = MediaRecorder.AudioSource.MIC;
    // 设置音频采样率，44100是目前的标准，但是某些设备仍然支 2050 6000 1025
    private final static int SAMPLE_HZ = 44100;
    // 设置音频的录制的声道CHANNEL_IN_STEREO为双声道，CHANNEL_CONFIGURATION_MONO为单声道
    private final static int CHANNEL_CONFIG = AudioFormat.CHANNEL_IN_STEREO;
    // 音频数据格式:PCM 16位每个样本保证设备支持。PCM 8位每个样本 不一定能得到设备支持
    private final static int FORMAT = AudioFormat.ENCODING_PCM_16BIT;
    private int mBufferSize;
    private AudioRecord mAudioRecord = null;
    private int bufferSizeInBytes = 0;
............
    public AudioRecoderManager() {
        if (SAVE_FILE_FOR_TEST) {
            fileManager = new FileManager(FileManager.TEST_PCM_FILE);
        }
        bufferSizeInBytes = AudioRecord.getMinBufferSize(SAMPLE_HZ, CHANNEL_CONFIG, FORMAT);
        mAudioRecord = new AudioRecord(SOURCE, SAMPLE_HZ, CHANNEL_CONFIG, FORMAT, bufferSizeInBytes);
        mBufferSize = 4 * 1024;
    }
        public void startAudioIn() {
        workThread = new Thread() {
            @Override
            public void run() {
                mAudioRecord.startRecording();
                byte[] audioData = new byte[mBufferSize];
                int readsize = 0;
                //录音，获取PCM裸音频，这个音频数据文件很大，我们必须编码成AAC，这样才能rtmp传输
                while (loop && !Thread.interrupted()) {
                    try {
                        readsize += mAudioRecord.read(audioData, readsize, mBufferSize);
                        byte[] ralAudio = new byte[readsize];
                        //每次录音读取4K数据
                        System.arraycopy(audioData, 0, ralAudio, 0, readsize);
                        if (audioDataListener != null) {
                            //把录音的数据抛给MediaEncoder去编码AAC音频数据
                            audioDataListener.audioData(ralAudio);
                        }
                        //我们可以把裸音频以文件格式存起来，判断这个音频是否是好的，只需要加一个WAV头
                        //即形成WAV无损音频格式
                        if (SAVE_FILE_FOR_TEST) {
                            fileManager.saveFileData(ralAudio);
                        }
                        readsize = 0;
                        Arrays.fill(audioData, (byte)0);
                    }
                    catch(Exception e) {
                        e.printStackTrace();
                    }
                }
            }
        };
        loop = true;
        workThread.start();
    }
    public void stopAudioIn() {
        loop = false;
        workThread.interrupt();
        mAudioRecord.stop();
        mAudioRecord.release();
        mAudioRecord = null;
        if (SAVE_FILE_FOR_TEST) {
            fileManager.closeFile();
            //测试代码，以WAV格式保存数据啊
            PcmToWav.copyWaveFile(FileManager.TEST_PCM_FILE, FileManager.TEST_WAV_FILE, SAMPLE_HZ, bufferSizeInBytes);
        }
    }
```
我们再来看看MediaEncoder是如何编码PCM裸音频的
```
public MediaEncoder() {
        if (SAVE_FILE_FOR_TEST) {
            videoFileManager = new FileManager(FileManager.TEST_H264_FILE);
            audioFileManager = new FileManager(FileManager.TEST_AAC_FILE);
        }
        videoQueue = new LinkedBlockingQueue<>();
        audioQueue = new LinkedBlockingQueue<>();
        //这里我们初始化音频数据，为什么要初始化音频数据呢？音频数据里面我们做了什么事情？
        audioEncodeBuffer = StreamProcessManager.encoderAudioInit(Contacts.SAMPLE_RATE,
                Contacts.CHANNELS, Contacts.BIT_RATE);
    }
............
public void startAudioEncode() {
        if (audioEncoderLoop) {
            throw new RuntimeException("必须先停止");
        }
        audioEncoderThread = new Thread() {
            @Override
            public void run() {
                byte[] outbuffer = new byte[1024];
                int haveCopyLength = 0;
                byte[] inbuffer = new byte[audioEncodeBuffer];
                while (audioEncoderLoop && !Thread.interrupted()) {
                    try {
                        AudioData audio = audioQueue.take();
                        //Log.e("RiemannLee", " audio.audioData.length " + audio.audioData.length + " audioEncodeBuffer " + audioEncodeBuffer);
                        final int audioGetLength = audio.audioData.length;
                        if (haveCopyLength < audioEncodeBuffer) {
                            System.arraycopy(audio.audioData, 0, inbuffer, haveCopyLength, audioGetLength);
                            haveCopyLength += audioGetLength;
                            int remain = audioEncodeBuffer - haveCopyLength;
                            if (remain == 0) {
                                int validLength = StreamProcessManager.encoderAudioEncode(inbuffer, audioEncodeBuffer, outbuffer, outbuffer.length);
                                //Log.e("lihuzi", " validLength " + validLength);
                                final int VALID_LENGTH = validLength;
                                if (VALID_LENGTH > 0) {
                                    byte[] encodeData = new byte[VALID_LENGTH];
                                    System.arraycopy(outbuffer, 0, encodeData, 0, VALID_LENGTH);
                                    if (sMediaEncoderCallback != null) {
                                        sMediaEncoderCallback.receiveEncoderAudioData(encodeData, VALID_LENGTH);
                                    }
                                    if (SAVE_FILE_FOR_TEST) {
                                        audioFileManager.saveFileData(encodeData);
                                    }
                                }
                                haveCopyLength = 0;
                            }
                        }
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                        break;
                    }
                }
            }
        };
        audioEncoderLoop = true;
        audioEncoderThread.start();
    }
```
进入audio的jni编码
```
//音频初始化
JNIEXPORT jint JNICALL Java_com_riemannlee_liveproject_StreamProcessManager_encoderAudioInit
        (JNIEnv *env, jclass type, jint jsampleRate, jint jchannels, jint jbitRate)
{
    audioEncoder = new AudioEncoder(jchannels, jsampleRate, jbitRate);
    int value = audioEncoder->init();
    return value;
}
```
现在，我们进入了AudioEncoder，进入了音频编码的世界
```
AudioEncoder::AudioEncoder(int channels, int sampleRate, int bitRate)
{
    this->channels = channels;
    this->sampleRate = sampleRate;
    this->bitRate = bitRate;
}
............
/**
 * 初始化fdk-aac的参数，设置相关接口使得
 * @return
 */
int AudioEncoder::init() {
    //打开AAC音频编码引擎，创建AAC编码句柄
    if (aacEncOpen(&handle, 0, channels) != AACENC_OK) {
        LOGI("Unable to open fdkaac encoder\n");
        return -1;
    }
    // 下面都是利用aacEncoder_SetParam设置参数
    // AACENC_AOT设置为aac lc
    if (aacEncoder_SetParam(handle, AACENC_AOT, 2) != AACENC_OK) {
        LOGI("Unable to set the AOT\n");
        return -1;
    }
    if (aacEncoder_SetParam(handle, AACENC_SAMPLERATE, sampleRate) != AACENC_OK) {
        LOGI("Unable to set the sampleRate\n");
        return -1;
    }
    // AACENC_CHANNELMODE设置为双通道
    if (aacEncoder_SetParam(handle, AACENC_CHANNELMODE, MODE_2) != AACENC_OK) {
        LOGI("Unable to set the channel mode\n");
        return -1;
    }
    if (aacEncoder_SetParam(handle, AACENC_CHANNELORDER, 1) != AACENC_OK) {
        LOGI("Unable to set the wav channel order\n");
        return 1;
    }
    if (aacEncoder_SetParam(handle, AACENC_BITRATE, bitRate) != AACENC_OK) {
        LOGI("Unable to set the bitrate\n");
        return -1;
    }
    if (aacEncoder_SetParam(handle, AACENC_TRANSMUX, 2) != AACENC_OK) { //0-raw 2-adts
        LOGI("Unable to set the ADTS transmux\n");
        return -1;
    }
    if (aacEncoder_SetParam(handle, AACENC_AFTERBURNER, 1) != AACENC_OK) {
        LOGI("Unable to set the ADTS AFTERBURNER\n");
        return -1;
    }
    if (aacEncEncode(handle, NULL, NULL, NULL, NULL) != AACENC_OK) {
        LOGI("Unable to initialize the encoder\n");
        return -1;
    }
    AACENC_InfoStruct info = { 0 };
    if (aacEncInfo(handle, &info) != AACENC_OK) {
        LOGI("Unable to get the encoder info\n");
        return -1;
    }
    //返回数据给上层，表示每次传递多少个数据最佳，这样encode效率最高
    int inputSize = channels * 2 * info.frameLength;
    LOGI("inputSize = %d", inputSize);
    return inputSize;
}
```
我们终于知道MediaEncoder构造函数中初始化音频数据的用意了，它会返回设备中传递多少inputSize为最佳，这样，我们每次只需要传递相应的数据，就可以使得音频效率更优化
```
public void startAudioEncode() {
        if (audioEncoderLoop) {
            throw new RuntimeException("必须先停止");
        }
        audioEncoderThread = new Thread() {
            @Override
            public void run() {
                byte[] outbuffer = new byte[1024];
                int haveCopyLength = 0;
                byte[] inbuffer = new byte[audioEncodeBuffer];
                while (audioEncoderLoop && !Thread.interrupted()) {
                    try {
                        AudioData audio = audioQueue.take();
                        //我们通过fdk-aac接口获取到了audioEncodeBuffer的数据，即每次编码多少数据为最优
                        //这里我这边的手机每次都是返回的4096即4K的数据，其实为了简单点，我们每次可以让
                        //MIC录取4K大小的数据，然后把录取的数据传递到AudioEncoder.cpp中取编码
                        //Log.e("RiemannLee", " audio.audioData.length " + audio.audioData.length + " audioEncodeBuffer " + audioEncodeBuffer);
                        final int audioGetLength = audio.audioData.length;
                        if (haveCopyLength < audioEncodeBuffer) {
                            System.arraycopy(audio.audioData, 0, inbuffer, haveCopyLength, audioGetLength);
                            haveCopyLength += audioGetLength;
                            int remain = audioEncodeBuffer - haveCopyLength;
                            if (remain == 0) {
                                //fdk-aac编码PCM裸音频数据，返回可用长度的有效字段
                                int validLength = StreamProcessManager.encoderAudioEncode(inbuffer, audioEncodeBuffer, outbuffer, outbuffer.length);
                                //Log.e("lihuzi", " validLength " + validLength);
                                final int VALID_LENGTH = validLength;
                                if (VALID_LENGTH > 0) {
                                    byte[] encodeData = new byte[VALID_LENGTH];
                                    System.arraycopy(outbuffer, 0, encodeData, 0, VALID_LENGTH);
                                    if (sMediaEncoderCallback != null) {
                                        //编码后，把数据抛给rtmp去推流
                                        sMediaEncoderCallback.receiveEncoderAudioData(encodeData, VALID_LENGTH);
                                    }
                                    //我们可以把Fdk-aac编码后的数据保存到文件中，然后用播放器听一下，音频文件是否编码正确
                                    if (SAVE_FILE_FOR_TEST) {
                                        audioFileManager.saveFileData(encodeData);
                                    }
                                }
                                haveCopyLength = 0;
                            }
                        }
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                        break;
                    }
                }
            }
        };
        audioEncoderLoop = true;
        audioEncoderThread.start();
    }
```
我们看AudioEncoder是如何利用fdk-aac编码的
```
/**
 * Fdk-AAC库压缩裸音频PCM数据，转化为AAC，这里为什么用fdk-aac，这个库相比普通的aac库，压缩效率更高
 * @param inBytes
 * @param length
 * @param outBytes
 * @param outLength
 * @return
 */
int AudioEncoder::encodeAudio(unsigned char *inBytes, int length, unsigned char *outBytes, int outLength) {
    void *in_ptr, *out_ptr;
    AACENC_BufDesc in_buf = {0};
    int in_identifier = IN_AUDIO_DATA;
    int in_elem_size = 2;
    //传递input数据给in_buf
    in_ptr = inBytes;
    in_buf.bufs = &in_ptr;
    in_buf.numBufs = 1;
    in_buf.bufferIdentifiers = &in_identifier;
    in_buf.bufSizes = &length;
    in_buf.bufElSizes = &in_elem_size;
    AACENC_BufDesc out_buf = {0};
    int out_identifier = OUT_BITSTREAM_DATA;
    int elSize = 1;
    //out数据放到out_buf中
    out_ptr = outBytes;
    out_buf.bufs = &out_ptr;
    out_buf.numBufs = 1;
    out_buf.bufferIdentifiers = &out_identifier;
    out_buf.bufSizes = &outLength;
    out_buf.bufElSizes = &elSize;
    AACENC_InArgs in_args = {0};
    in_args.numInSamples = length / 2;  //size为pcm字节数
    AACENC_OutArgs out_args = {0};
    AACENC_ERROR err;
    //利用aacEncEncode来编码PCM裸音频数据，上面的代码都是fdk-aac的流程步骤
    if ((err = aacEncEncode(handle, &in_buf, &out_buf, &in_args, &out_args)) != AACENC_OK) {
        LOGI("Encoding aac failed\n");
        return err;
    }
    //返回编码后的有效字段长度
    return out_args.numOutBytes;
}
```
至此，我们终于把视频数据和音频数据编码成功了
```
视频数据：NV21==>YUV420P==>H264
音频数据：PCM裸音频==>AAC
```
四 . RTMP如何推送音视频流
最后我们看看rtmp是如何推流的：我们看看MediaPublisher这个类
```
public MediaPublisher() {
        mediaEncoder = new MediaEncoder();
        MediaEncoder.setsMediaEncoderCallback(new MediaEncoder.MediaEncoderCallback() {
            @Override
            public void receiveEncoderVideoData(byte[] videoData, int totalLength, int[] segment) {
                onEncoderVideoData(videoData, totalLength, segment);
            }
            @Override
            public void receiveEncoderAudioData(byte[] audioData, int size) {
                onEncoderAudioData(audioData, size);
            }
        });
        rtmpThread = new Thread("publish-thread") {
            @Override
            public void run() {
                while (loop && !Thread.interrupted()) {
                    try {
                        Runnable runnable = mRunnables.take();
                        runnable.run();
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
            }
        };
        loop = true;
        rtmpThread.start();
    }
............
    private void onEncoderVideoData(byte[] encodeVideoData, int totalLength, int[] segment) {
        int spsLen = 0;
        int ppsLen = 0;
        byte[] sps = null;
        byte[] pps = null;
        int haveCopy = 0;
        //segment为C++传递上来的数组，当为SPS，PPS的时候，视频NALU数组大于1，其它时候等于1
        for (int i = 0; i < segment.length; i++) {
            int segmentLength = segment[i];
            byte[] segmentByte = new byte[segmentLength];
            System.arraycopy(encodeVideoData, haveCopy, segmentByte, 0, segmentLength);
            haveCopy += segmentLength;
            int offset = 4;
            if (segmentByte[2] == 0x01) {
                offset = 3;
            }
            int type = segmentByte[offset] & 0x1f;
            //Log.d("RiemannLee", "type= " + type);
            //获取到NALU的type，SPS，PPS，SEI，还是关键帧
            if (type == NAL_SPS) {
                spsLen = segment[i] - 4;
                sps = new byte[spsLen];
                System.arraycopy(segmentByte, 4, sps, 0, spsLen);
                //Log.e("RiemannLee", "NAL_SPS spsLen " + spsLen);
            } else if (type == NAL_PPS) {
                ppsLen = segment[i] - 4;
                pps = new byte[ppsLen];
                System.arraycopy(segmentByte, 4, pps, 0, ppsLen);
                //Log.e("RiemannLee", "NAL_PPS ppsLen " + ppsLen);
                sendVideoSpsAndPPS(sps, spsLen, pps, ppsLen, 0);
            } else {
                sendVideoData(segmentByte, segmentLength, videoID++);
            }
        }
    }
............
   private void onEncoderAudioData(byte[] encodeAudioData, int size) {
        if (!isSendAudioSpec) {
            Log.e("RiemannLee", "#######sendAudioSpec######");
            sendAudioSpec(0);
            isSendAudioSpec = true;
        }
        sendAudioData(encodeAudioData, size, audioID++);
    }
```
向rtmp发送视频和音频数据的时候，实际上就是下面几个JNI函数
```
/**
     * 初始化RMTP，建立RTMP与RTMP服务器连接
     * @param url
     * @return
     */
    public static native int initRtmpData(String url);
    /**
     * 发送SPS,PPS数据
     * @param sps       sps数据
     * @param spsLen    sps长度
     * @param pps       pps数据
     * @param ppsLen    pps长度
     * @param timeStamp 时间戳
     * @return
     */
    public static native int sendRtmpVideoSpsPPS(byte[] sps, int spsLen, byte[] pps, int ppsLen, long timeStamp);
    /**
     * 发送视频数据，再发送sps，pps之后
     * @param data
     * @param dataLen
     * @param timeStamp
     * @return
     */
    public static native int sendRtmpVideoData(byte[] data, int dataLen, long timeStamp);
    /**
     * 发送AAC Sequence HEAD 头数据
     * @param timeStamp
     * @return
     */
    public static native int sendRtmpAudioSpec(long timeStamp);
    /**
     * 发送AAC音频数据
     * @param data
     * @param dataLen
     * @param timeStamp
     * @return
     */
    public static native int sendRtmpAudioData(byte[] data, int dataLen, long timeStamp);
    /**
     * 释放RTMP连接
     * @return
     */
    public static native int releaseRtmp();
```
再来看看RtmpLivePublish是如何完成这几个jni函数的
```
//初始化rtmp，主要是在RtmpLivePublish类完成的
JNIEXPORT jint JNICALL Java_com_riemannlee_liveproject_StreamProcessManager_initRtmpData
        (JNIEnv *env, jclass type, jstring jurl)
{
    const char *url_cstr = env->GetStringUTFChars(jurl, NULL);
    //复制url_cstr内容到rtmp_path
    char *rtmp_path = (char*)malloc(strlen(url_cstr) + 1);
    memset(rtmp_path, 0, strlen(url_cstr) + 1);
    memcpy(rtmp_path, url_cstr, strlen(url_cstr));
    rtmpLivePublish = new RtmpLivePublish();
    rtmpLivePublish->init((unsigned char*)rtmp_path);
    return 0;
}
//发送sps，pps数据
JNIEXPORT jint JNICALL Java_com_riemannlee_liveproject_StreamProcessManager_sendRtmpVideoSpsPPS
        (JNIEnv *env, jclass type, jbyteArray jspsArray, jint spsLen, jbyteArray ppsArray, jint ppsLen, jlong jstamp)
{
    if (rtmpLivePublish) {
        jbyte *sps_data = env->GetByteArrayElements(jspsArray, NULL);
        jbyte *pps_data = env->GetByteArrayElements(ppsArray, NULL);
        rtmpLivePublish->addSequenceH264Header((unsigned char*) sps_data, spsLen, (unsigned char*) pps_data, ppsLen);
        env->ReleaseByteArrayElements(jspsArray, sps_data, 0);
        env->ReleaseByteArrayElements(ppsArray, pps_data, 0);
    }
    return 0;
}
//发送视频数据
JNIEXPORT jint JNICALL Java_com_riemannlee_liveproject_StreamProcessManager_sendRtmpVideoData
        (JNIEnv *env, jclass type, jbyteArray jvideoData, jint dataLen, jlong jstamp)
{
    if (rtmpLivePublish) {
        jbyte *video_data = env->GetByteArrayElements(jvideoData, NULL);
        rtmpLivePublish->addH264Body((unsigned char*)video_data, dataLen, jstamp);
        env->ReleaseByteArrayElements(jvideoData, video_data, 0);
    }
    return 0;
}
//发送音频Sequence头数据
JNIEXPORT jint JNICALL Java_com_riemannlee_liveproject_StreamProcessManager_sendRtmpAudioSpec
        (JNIEnv *env, jclass type, jlong jstamp)
{
    if (rtmpLivePublish) {
        rtmpLivePublish->addSequenceAacHeader(44100, 2, 0);
    }
    return 0;
}
//发送音频Audio数据
JNIEXPORT jint JNICALL Java_com_riemannlee_liveproject_StreamProcessManager_sendRtmpAudioData
        (JNIEnv *env, jclass type, jbyteArray jaudiodata, jint dataLen, jlong jstamp)
{
    if (rtmpLivePublish) {
        jbyte *audio_data = env->GetByteArrayElements(jaudiodata, NULL);
        rtmpLivePublish->addAccBody((unsigned char*) audio_data, dataLen, jstamp);
        env->ReleaseByteArrayElements(jaudiodata, audio_data, 0);
    }
    return 0;
}
//释放RTMP连接
JNIEXPORT jint JNICALL Java_com_riemannlee_liveproject_StreamProcessManager_releaseRtmp
        (JNIEnv *env, jclass type)
{
    if (rtmpLivePublish) {
        rtmpLivePublish->release();
    }
    return 0;
}
```
最后再来看看RtmpLivePublish这个推流类是如何推送音视频的，rtmp的音视频流的推送有一个前提，需要首先发送
```
AVC sequence header 视频同步包的构造
AAC sequence header 音频同步包的构造
```
下面我们来看看AVC sequence的结构，AVC sequence header就是AVCDecoderConfigurationRecord结构
![](https://upload-images.jianshu.io/upload_images/10437603-caf5e7146daaa797.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/723/format/webp)
image.png
这个协议对应于下面的代码：
```
/*AVCDecoderConfigurationRecord*/
    //configurationVersion版本号，1
    body[i++] = 0x01;
    //AVCProfileIndication sps[1]
    body[i++] = sps[1];
    //profile_compatibility sps[2]
    body[i++] = sps[2];
    //AVCLevelIndication sps[3]
    body[i++] = sps[3];
    //6bit的reserved为二进制位111111和2bitlengthSizeMinusOne一般为3，
    //二进制位11，合并起来为11111111，即为0xff
    body[i++] = 0xff;
    /*sps*/
    //3bit的reserved，二进制位111，5bit的numOfSequenceParameterSets，
    //sps个数，一般为1，及合起来二进制位11100001，即为0xe1
    body[i++]   = 0xe1;
    //SequenceParametersSetNALUnits（sps_size + sps）的数组
    body[i++] = (sps_len >> 8) & 0xff;
    body[i++] = sps_len & 0xff;
    memcpy(&body[i], sps, sps_len);
    i +=  sps_len;
    /*pps*/
    //numOfPictureParameterSets一般为1，即为0x01
    body[i++]   = 0x01;
    //SequenceParametersSetNALUnits（pps_size + pps）的数组
    body[i++] = (pps_len >> 8) & 0xff;
    body[i++] = (pps_len) & 0xff;
    memcpy(&body[i], pps, pps_len);
    i +=  pps_len;
```
对于AAC sequence header存放的是AudioSpecificConfig结构，该结构则在“ISO-14496-3 Audio”中描述。AudioSpecificConfig结构的描述非常复杂，这里我做一下简化，事先设定要将要编码的音频格式，其中，选择"AAC-LC"为音频编码，音频采样率为44100，于是AudioSpecificConfig简化为下表：
![](https://upload-images.jianshu.io/upload_images/10437603-9c8e07d87095919c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/723/format/webp)
image.png
这个协议对应于下面的代码：
```
//如上图所示
    //5bit audioObjectType 编码结构类型，AAC-LC为2 二进制位00010
    //4bit samplingFrequencyIndex 音频采样索引值，44100对应值是4，二进制位0100
    //4bit channelConfiguration 音频输出声道，对应的值是2，二进制位0010
    //1bit frameLengthFlag 标志位用于表明IMDCT窗口长度 0 二进制位0
    //1bit dependsOnCoreCoder 标志位，表面是否依赖与corecoder 0 二进制位0
    //1bit extensionFlag 选择了AAC-LC,这里必须是0 二进制位0
    //上面都合成二进制0001001000010000
    uint16_t audioConfig = 0 ;
    //这里的2表示对应的是AAC-LC 由于是5个bit，左移11位，变为16bit，2个字节
    //与上一个1111100000000000(0xF800)，即只保留前5个bit
    audioConfig |= ((2 << 11) & 0xF800) ;
    int sampleRateIndex = getSampleRateIndex( sampleRate ) ;
    if( -1 == sampleRateIndex ) {
        free(packet);
        packet = NULL;
        LOGE("addSequenceAacHeader: no support current sampleRate[%d]" , sampleRate);
        return;
    }
    //sampleRateIndex为4，二进制位0000001000000000 & 0000011110000000(0x0780)（只保留5bit后4位）
    audioConfig |= ((sampleRateIndex << 7) & 0x0780) ;
    //sampleRateIndex为4，二进制位000000000000000 & 0000000001111000(0x78)（只保留5+4后4位）
    audioConfig |= ((channel << 3) & 0x78) ;
    //最后三个bit都为0保留最后三位111(0x07)
    audioConfig |= (0 & 0x07) ;
    //最后得到合成后的数据0001001000010000，然后分别取这两个字节
    body[2] = ( audioConfig >> 8 ) & 0xFF ;
    body[3] = ( audioConfig & 0xFF );
```
至此，我们就分别构造了AVC sequence header 和AAC sequence header，这两个结构是推流的先决条件，没有这两个东西，解码器是无法解码的，最后我们再来看看我们把解码的音视频如何rtmp推送
```
/**
 * 发送H264数据
 * @param buf
 * @param len
 * @param timeStamp
 */
void RtmpLivePublish::addH264Body(unsigned char *buf, int len, long timeStamp) {
    //去掉起始码(界定符)
    if (buf[2] == 0x00) {
        //00 00 00 01
        buf += 4;
        len -= 4;
    } else if (buf[2] == 0x01) {
        // 00 00 01
        buf += 3;
        len -= 3;
    }
    int body_size = len + 9;
    RTMPPacket *packet = (RTMPPacket *)malloc(RTMP_HEAD_SIZE + 9 + len);
    memset(packet, 0, RTMP_HEAD_SIZE);
    packet->m_body = (char *)packet + RTMP_HEAD_SIZE;
    unsigned char *body = (unsigned char*)packet->m_body;
    //当NAL头信息中，type（5位）等于5，说明这是关键帧NAL单元
    //buf[0] NAL Header与运算，获取type，根据type判断关键帧和普通帧
    //00000101 & 00011111(0x1f) = 00000101
    int type = buf[0] & 0x1f;
    //Pframe  7:AVC
    body[0] = 0x27;
    //IDR I帧图像
    //Iframe  7:AVC
    if (type == NAL_SLICE_IDR) {
        body[0] = 0x17;
    }
    //AVCPacketType = 1
    /*nal unit,NALUs（AVCPacketType == 1)*/
    body[1] = 0x01;
    //composition time 0x000000 24bit
    body[2] = 0x00;
    body[3] = 0x00;
    body[4] = 0x00;
    //写入NALU信息，右移8位，一个字节的读取
    body[5] = (len >> 24) & 0xff;
    body[6] = (len >> 16) & 0xff;
    body[7] = (len >> 8) & 0xff;
    body[8] = (len) & 0xff;
    /*copy data*/
    memcpy(&body[9], buf, len);
    packet->m_hasAbsTimestamp = 0;
    packet->m_nBodySize = body_size;
    //当前packet的类型：Video
    packet->m_packetType = RTMP_PACKET_TYPE_VIDEO;
    packet->m_nChannel = 0x04;
    packet->m_headerType = RTMP_PACKET_SIZE_LARGE;
    packet->m_nInfoField2 = rtmp->m_stream_id;
    //记录了每一个tag相对于第一个tag（File Header）的相对时间
    packet->m_nTimeStamp = RTMP_GetTime() - start_time;
    //send rtmp h264 body data
    if (RTMP_IsConnected(rtmp)) {
        RTMP_SendPacket(rtmp, packet, TRUE);
        //LOGD("send packet sendVideoData");
    }
    free(packet);
}
/**
 * 发送rtmp AAC data
 * @param buf
 * @param len
 * @param timeStamp
 */
void RtmpLivePublish::addAccBody(unsigned char *buf, int len, long timeStamp) {
    int body_size = 2 + len;
    RTMPPacket * packet = (RTMPPacket *)malloc(RTMP_HEAD_SIZE + len + 2);
    memset(packet, 0, RTMP_HEAD_SIZE);
    packet->m_body = (char *)packet + RTMP_HEAD_SIZE;
    unsigned char * body = (unsigned char *)packet->m_body;
    //头信息配置
    /*AF 00 + AAC RAW data*/
    body[0] = 0xAF;
    //AACPacketType:1表示AAC raw
    body[1] = 0x01;
    /*spec_buf是AAC raw数据*/
    memcpy(&body[2], buf, len);
    packet->m_packetType = RTMP_PACKET_TYPE_AUDIO;
    packet->m_nBodySize = body_size;
    packet->m_nChannel = 0x04;
    packet->m_hasAbsTimestamp = 0;
    packet->m_headerType = RTMP_PACKET_SIZE_LARGE;
    packet->m_nTimeStamp = RTMP_GetTime() - start_time;
    //LOGI("aac m_nTimeStamp = %d", packet->m_nTimeStamp);
    packet->m_nInfoField2 = rtmp->m_stream_id;
    //send rtmp aac data
    if (RTMP_IsConnected(rtmp)) {
        RTMP_SendPacket(rtmp, packet, TRUE);
        //LOGD("send packet sendAccBody");
    }
    free(packet);
}
```
我们推送RTMP都是调用的libRtmp库的RTMP_SendPacket接口，先判断是否rtmp是通的，是的话推流即可，最后，我们看看rtmp是如何连接服务器的：
```
/**
 * 初始化RTMP数据，与rtmp连接
 * @param url
 */
void RtmpLivePublish::init(unsigned char * url) {
    this->rtmp_url = url;
    rtmp = RTMP_Alloc();
    RTMP_Init(rtmp);
    rtmp->Link.timeout = 5;
    RTMP_SetupURL(rtmp, (char *)url);
    RTMP_EnableWrite(rtmp);
    if (!RTMP_Connect(rtmp, NULL) ) {
        LOGI("RTMP_Connect error");
    } else {
        LOGI("RTMP_Connect success.");
    }
    if (!RTMP_ConnectStream(rtmp, 0)) {
        LOGI("RTMP_ConnectStream error");
    } else {
        LOGI("RTMP_ConnectStream success.");
    }
    start_time = RTMP_GetTime();
    LOGI(" start_time = %d", start_time);
}
```
至此，我们终于完成了rtmp推流的整个过程。
五 . 代码以及如何调试
本工程的源码地址如下：
[https://github.com/liweiping1314521/RiemannLeeLiveProject](https://link.jianshu.com?t=https%3A%2F%2Fgithub.com%2Fliweiping1314521%2FRiemannLeeLiveProject)
如何搭建RTMP服务器，可以参考下面网址
[https://www.cnblogs.com/jys509/p/5649066.html](https://link.jianshu.com?t=https%3A%2F%2Fwww.cnblogs.com%2Fjys509%2Fp%2F5649066.html)
然后再运行命令:nginx，
然后再服务器端开启VLC播放器，输入服务器IP地址，推流过来双击即可使用VLC播放器展示我们的推流结果：
![](https://upload-images.jianshu.io/upload_images/10437603-517b5e047bfd9c02.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
image.png
最后附上两张效果图
![](https://upload-images.jianshu.io/upload_images/10437603-3a1049f6a827bc0c.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
mmexport1518689120173.jpg
![](https://upload-images.jianshu.io/upload_images/10437603-185fcbf987f0eb1d.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
mmexport1518689099568.jpg
由于本人水平有限，里面如果有错误的地方，望大家包涵，一起学习，谢谢！
参考文档：
[http://www.cnblogs.com/jingzhishen/p/3965868.html](https://link.jianshu.com?t=http%3A%2F%2Fwww.cnblogs.com%2Fjingzhishen%2Fp%2F3965868.html)
[http://www.cnblogs.com/lidabo/p/4602422.html](https://link.jianshu.com?t=http%3A%2F%2Fwww.cnblogs.com%2Flidabo%2Fp%2F4602422.html)
[http://blog.csdn.net/chenchong_219/article/details/37990541](https://link.jianshu.com?t=http%3A%2F%2Fblog.csdn.net%2Fchenchong_219%2Farticle%2Fdetails%2F37990541)
[http://blog.csdn.net/mincheat/article/details/48713047](https://link.jianshu.com?t=http%3A%2F%2Fblog.csdn.net%2Fmincheat%2Farticle%2Fdetails%2F48713047)
[http://guoh.org/lifelog/2013/10/h-264-bit-stream-sps-pps-idr-nalu/](https://link.jianshu.com?t=http%3A%2F%2Fguoh.org%2Flifelog%2F2013%2F10%2Fh-264-bit-stream-sps-pps-idr-nalu%2F)
[http://lazybing.github.io/blog/2017/06/22/sodb-rbsp-ebsp/](https://link.jianshu.com?t=http%3A%2F%2Flazybing.github.io%2Fblog%2F2017%2F06%2F22%2Fsodb-rbsp-ebsp%2F)
[https://www.jianshu.com/p/ea3fadae9d47](https://www.jianshu.com/p/ea3fadae9d47)
[http://www.cnblogs.com/lihaiping/p/4167844.html](https://link.jianshu.com?t=http%3A%2F%2Fwww.cnblogs.com%2Flihaiping%2Fp%2F4167844.html)
[http://blog.csdn.net/a992036795/article/details/54572335](https://link.jianshu.com?t=http%3A%2F%2Fblog.csdn.net%2Fa992036795%2Farticle%2Fdetails%2F54572335)
[http://blog.csdn.net/u011003120/article/details/78378632](https://link.jianshu.com?t=http%3A%2F%2Fblog.csdn.net%2Fu011003120%2Farticle%2Fdetails%2F78378632)
作者：RiemannLee_22dc
链接：https://www.jianshu.com/p/f3a55d3d1f5d
來源：简书
简书著作权归作者所有，任何形式的转载都请联系作者获得授权并注明出处。
