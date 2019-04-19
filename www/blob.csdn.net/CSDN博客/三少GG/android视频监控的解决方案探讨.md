# android视频监控的解决方案探讨 - 三少GG - CSDN博客
2012年03月20日 11:18:36[三少GG](https://me.csdn.net/scut1135)阅读数：6918标签：[android																[byte																[h.264																[socket																[silverlight																[protocols](https://so.csdn.net/so/search/s.do?q=protocols&t=blog)](https://so.csdn.net/so/search/s.do?q=silverlight&t=blog)](https://so.csdn.net/so/search/s.do?q=socket&t=blog)](https://so.csdn.net/so/search/s.do?q=h.264&t=blog)](https://so.csdn.net/so/search/s.do?q=byte&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[流媒体基础](https://blog.csdn.net/scut1135/article/category/934830)
1. camera YUV420数据直接socket发送到服务器端，转换成RGB显示 占用带宽较多。
      原理阐述：基于socket，wifi。 android客户端获取的帧数据的格式为yuv420格式，本来将此data数据流直接传输到另一android端，由接收端完成图像的解码转换工作，但是发现如果直接传数据流数据量太大，还不如直接在本地解码转换完毕生成图片，然后将图片传到另外一android端进行重绘排列。
     考虑到效率，可将核心的解码转换封装成c实现.so，速度好些
     改进： **项目采用多播协议，即如果在多台电脑上同时运行服务端，则一台手机可以同时控制多台电脑做同样的事情**，同时操作所有开启服务端电脑的ppt。
    “   谈下实现思路：
1 socket实现传输，目前网络用的wifi
2 java端的视频获取我采用的是jmf框架
3 逻辑比较简单：总体实现无非就是android端视频捕获发送yuv原格式到pc端进行解码而后在pc上显示，
同时pc通过jmf上获取camera摄像头的数据，通过socket传输到android端进行重绘。
   缺点：
1)"
你这个实现方案流量大死了，根本就不用，代码还当宝一样。。
网上都有代码了，你还不是根据别人的改的，鄙视你。"
[http://www.eoeandroid.com.sixxs.org/thread-51460-4-1.html](http://www.eoeandroid.com.sixxs.org/thread-51460-4-1.html)
"
你这就是做着玩的，离视频聊天差远了。
实现方案是：移植ffmpeg获取原始数据，编码，发送流媒体服务器，接收数据，解码，显示。**流量10kb左右**。不是我喷你，你不公开代码你就做好点在来炫耀，要不你炫耀个毛线。"
2) "你这也好意思叫视频聊天，视频有了，声音呢？你实现的只是捕获YUV数据然后绘制图片而已吧~~~~"
2. RTP SDP结合+
    The following network protocols are supported for audio and video playback:
RTSP (RTP, SDP)
HTTP/HTTPS progressive streaming
HTTP/HTTPS live streaming draft protocol:
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
### [Android中抓取手机视频流数据。](http://blog.csdn.net/peijiangping1989/article/details/6942585)
目前实时抓取手机视频数据有2种方法，一种是通过camera的回调获取源数据，这里获取的源数据是没有编码的数据。
有的人发送yuv数据然后在那绘制图片，也说视频聊天，真是可笑。这种方式是可是实现视频聊天的，但是需要移植编码库
目前可移植的有，android的opencore编码，参考[http://www.shouyanwang.org/thread-184-1-1.html](http://www.shouyanwang.org/thread-184-1-1.html)。
ffmpeg编码，这个网上很难找，但是也有人实现哦。（编码效率好，但是支持不了高清视频）
编码完成可以通过rtp协议发送。就可以视频啦。rtp协议什么的就不说了哦。
以上方法的代码，在这个链接最好回复我贴了代码，不是完整的。链接：[http://www.eoeandroid.com/thread-51460-1-1.html](http://www.eoeandroid.com/thread-51460-1-1.html)
第2种方法，也就是我用的方法。
通过MediaRecorder录制。然后绑定一个localsocket，可以获取编码后的视频数据传输；
代码如下：
**[java]**[view
 plain](http://blog.csdn.net/peijiangping1989/article/details/6942585#)[copy](http://blog.csdn.net/peijiangping1989/article/details/6942585#)[print](http://blog.csdn.net/peijiangping1989/article/details/6942585#)[?](http://blog.csdn.net/peijiangping1989/article/details/6942585#)
- package com.pei;  
- 
- import java.io.DataInputStream;  
- import java.io.IOException;  
- import java.io.InputStream;  
- import android.app.Activity;  
- import android.os.Bundle;  
- import android.graphics.PixelFormat;  
- import android.media.MediaRecorder;  
- import android.net.LocalServerSocket;  
- import android.net.LocalSocket;  
- import android.net.LocalSocketAddress;  
- import android.view.SurfaceHolder;  
- import android.view.SurfaceView;  
- import android.view.View;  
- import android.view.Window;  
- import android.view.WindowManager;  
- 
- /**
-  * class name：VideoCameraActivity<BR>
-  * class description：CATCH THE VIDEODATA SEND TO RED5<BR>
-  * PS： <BR>
-  * 
-  * @version 1.00 2011/11/05
-  * @author CODYY)peijiangping
-  */
- publicclass VideoCameraActivity extends Activity implements
-         SurfaceHolder.Callback, MediaRecorder.OnErrorListener,  
-         MediaRecorder.OnInfoListener {  
- privatestaticfinalint mVideoEncoder = MediaRecorder.VideoEncoder.H264;  
- private LocalSocket receiver, sender;  
- private LocalServerSocket lss;  
- private MediaRecorder mMediaRecorder = null;  
- privateboolean mMediaRecorderRecording = false;  
- private SurfaceView mSurfaceView = null;  
- private SurfaceHolder mSurfaceHolder = null;  
- private Thread t;  
- 
- @Override
- publicvoid onCreate(Bundle savedInstanceState) {  
- super.onCreate(savedInstanceState);  
-         getWindow().setFormat(PixelFormat.TRANSLUCENT);  
-         requestWindowFeature(Window.FEATURE_NO_TITLE);  
-         getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,  
-                 WindowManager.LayoutParams.FLAG_FULLSCREEN);  
-         setContentView(R.layout.main);  
-         mSurfaceView = (SurfaceView) this.findViewById(R.id.surface_camera);  
-         SurfaceHolder holder = mSurfaceView.getHolder();  
-         holder.addCallback(this);  
-         holder.setType(SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS);  
-         mSurfaceView.setVisibility(View.VISIBLE);  
-         receiver = new LocalSocket();  
- try {  
-             lss = new LocalServerSocket("VideoCamera");  
-             receiver.connect(new LocalSocketAddress("VideoCamera"));  
-             receiver.setReceiveBufferSize(500000);  
-             receiver.setSendBufferSize(500000);  
-             sender = lss.accept();  
-             sender.setReceiveBufferSize(500000);  
-             sender.setSendBufferSize(500000);  
-         } catch (IOException e) {  
-             finish();  
- return;  
-         }  
-     }  
- 
- @Override
- publicvoid onPause() {  
- super.onPause();  
- if (mMediaRecorderRecording) {  
-             stopVideoRecording();  
- try {  
-                 lss.close();  
-                 receiver.close();  
-                 sender.close();  
-             } catch (IOException e) {  
-                 e.printStackTrace();  
-             }  
-         }  
-         finish();  
-     }  
- 
- privatevoid stopVideoRecording() {  
-         System.out.println("stopVideoRecording");  
- if (mMediaRecorderRecording || mMediaRecorder != null) {  
- if (t != null)  
-                 t.interrupt();  
-             releaseMediaRecorder();  
-         }  
-     }  
- 
- privatevoid startVideoRecording() {  
-         (t = new Thread() {  
- publicvoid run() {  
- int frame_size = 20000;  
- byte[] buffer = newbyte[1024 * 64];  
- int num, number = 0;  
-                 InputStream fis = null;  
- try {  
-                     fis = receiver.getInputStream();  
-                 } catch (IOException e1) {  
- return;  
-                 }  
-                 number = 0;  
-                 releaseMediaRecorder();  
- while (true) {  
-                     System.out.println("ok");  
- try {  
-                         num = fis.read(buffer, number, frame_size);  
-                         number += num;  
- if (num < frame_size) {  
-                             System.out.println("recoend break");  
- break;  
-                         }  
-                     } catch (IOException e) {  
-                         System.out.println("exception break");  
- break;  
-                     }  
-                 }  
-                 initializeVideo();  
-                 number = 0;  
-                 Consumer consumer = new Publisher();// Publisher继承了Consumer
-                 Thread consumerThread = new Thread((Runnable) consumer);  
-                 consumer.setRecording(true);// 设置线程状态;
-                 consumerThread.start();// 开始发布数据流
-                 DataInputStream dis = new DataInputStream(fis);  
- try {  
-                     dis.read(buffer, 0, 32);  
-                 } catch (IOException e1) {  
-                     e1.printStackTrace();  
-                 }  
- byte[] aa = { 0x01, 0x42, (byte) 0x80, 0x0A, (byte) 0xFF,  
-                         (byte) 0xE1, 0x00, 0x12, 0x67, 0x42, (byte) 0x80, 0x0A,  
-                         (byte) 0xE9, 0x02, (byte) 0xC1, 0x29, 0x08, 0x00, 0x00,  
- 0x1F, 0x40, 0x00, 0x04, (byte) 0xE2, 0x00, 0x20, 0x01,  
- 0x00, 0x04, 0x68, (byte) 0xCE, 0x3C, (byte) 0x80 };  
-                 consumer.putData(System.currentTimeMillis(), aa, 33);  
- while (true) {  
- try {  
- int h264length = dis.readInt();  
-                         number = 0;  
- while (number < h264length) {  
- int lost = h264length - number;  
-                             num = fis.read(buffer, 0,  
-                                     frame_size < lost ? frame_size : lost);  
-                             number += num;  
-                             consumer.putData(System.currentTimeMillis(),  
-                                     buffer, num);  
-                         }  
-                     } catch (IOException e) {  
- break;  
-                     }  
-                 }  
-                 consumer.setRecording(false);// 设置线程状态;
-             }  
-         }).start();  
-     }  
- 
- privateboolean initializeVideo() {  
-         System.out.println("initializeVideo");  
- if (mSurfaceHolder == null)  
- returnfalse;  
-         mMediaRecorderRecording = true;  
- if (mMediaRecorder == null)  
-             mMediaRecorder = new MediaRecorder();  
- else
-             mMediaRecorder.reset();  
-         mMediaRecorder.setVideoSource(MediaRecorder.VideoSource.CAMERA);  
-         mMediaRecorder.setOutputFormat(MediaRecorder.OutputFormat.THREE_GPP);  
-         mMediaRecorder.setVideoFrameRate(20);  
-         mMediaRecorder.setVideoSize(352, 288);  
-         mMediaRecorder.setVideoEncoder(mVideoEncoder);  
-         mMediaRecorder.setPreviewDisplay(mSurfaceHolder.getSurface());  
-         mMediaRecorder.setMaxDuration(0);  
-         mMediaRecorder.setMaxFileSize(0);  
-         mMediaRecorder.setOutputFile(sender.getFileDescriptor());  
- try {  
-             mMediaRecorder.setOnInfoListener(this);  
-             mMediaRecorder.setOnErrorListener(this);  
-             mMediaRecorder.prepare();  
-             mMediaRecorder.start();  
-         } catch (IOException exception) {  
-             releaseMediaRecorder();  
-             finish();  
- returnfalse;  
-         }  
- returntrue;  
-     }  
- 
- privatevoid releaseMediaRecorder() {  
-         System.out.println("Releasing media recorder.");  
- if (mMediaRecorder != null) {  
- if (mMediaRecorderRecording) {  
- try {  
-                     mMediaRecorder.setOnErrorListener(null);  
-                     mMediaRecorder.setOnInfoListener(null);  
-                     mMediaRecorder.stop();  
-                 } catch (RuntimeException e) {  
-                     System.out.println("stop fail: " + e.getMessage());  
-                 }  
-                 mMediaRecorderRecording = false;  
-             }  
-             mMediaRecorder.reset();  
-             mMediaRecorder.release();  
-             mMediaRecorder = null;  
-         }  
-     }  
- 
- @Override
- publicvoid surfaceChanged(SurfaceHolder holder, int format, int w, int h) {  
-         System.out.println("surfaceChanged");  
-         mSurfaceHolder = holder;  
- if (!mMediaRecorderRecording) {  
-             initializeVideo();  
-             startVideoRecording();  
-         }  
-     }  
- 
- @Override
- publicvoid surfaceCreated(SurfaceHolder holder) {  
-         System.out.println("surfaceCreated");  
-         mSurfaceHolder = holder;  
-     }  
- 
- @Override
- publicvoid surfaceDestroyed(SurfaceHolder holder) {  
-         System.out.println("surfaceDestroyed");  
-         mSurfaceView = null;  
-         mSurfaceHolder = null;  
-         mMediaRecorder = null;  
- if (t != null) {  
-             t.interrupt();  
-         }  
-     }  
- 
- @Override
- publicvoid onInfo(MediaRecorder mr, int what, int extra) {  
- switch (what) {  
- case MediaRecorder.MEDIA_RECORDER_INFO_UNKNOWN:  
-             System.out.println("MEDIA_RECORDER_INFO_UNKNOWN");  
- break;
  
-  
      case MediaRecorder.MEDIA_RECORDER_INFO_MAX_DURATION_REACHED:  
        System.out.println("MEDIA_RECORDER_INFO_MAX_DURATION_REACHED");  
        break;  
       case MediaRecorder.MEDIA_RECORDER_INFO_MAX_FILESIZE_REACHED:  
        System.out.println("MEDIA_RECORDER_INFO_MAX_FILESIZE_REACHED");  
        break;  
    }  
}  
@Override  
public void onError(MediaRecorder mr, int what, int extra) {  
    if (what == MediaRecorder.MEDIA_RECORDER_ERROR_UNKNOWN) {  
        System.out.println("MEDIA_RECORDER_ERROR_UNKNOWN");  
        finish();  
    }  
}  
### [h264中avc和flv数据的解析](http://blog.csdn.net/peijiangping1989/article/details/6934312)
- 
计算 AVCDecoderConfigurationRecord  得到 CodecPrivateData 数据（只有第一帧需要）；
- 
计算 NALUs 得到帧数据。
计算 AVCDecoderConfigurationRecord  得到 CodecPrivateData 数据
H.264 视频流的**CodecPrivateData** 实际上就是 AVCDecoderConfigurationRecord
 中 **SequenceParameterSets（SPS）和 PictureParameterSets（PPS）使用 byte[] {00, 00, 01} 连接的字节数组**。
注意！FLV 文件中第一个 VIDEOTAG 的 VIDEODATA 的 AVCVIDEOPACKET 的 Data 总是 AVCDecoderConfigurationRecord（在 ISO/IEC
 14496-15 中定义），解码的时候注意跳过这个 VIDOETAG。
AVCDecoderConfigurationRecord 结构的定义：
aligned(8) class AVCDecoderConfigurationRecord { 
unsigned int(8) configurationVersion = 1; 
unsigned int(8) AVCProfileIndication; 
unsigned int(8) profile_compatibility; 
unsigned int(8) AVCLevelIndication; 
bit(6) reserved = ‘111111’b; 
unsigned int(2) lengthSizeMinusOne; 
bit(3) reserved = ‘111’b; 
unsigned int(5) numOfSequenceParameterSets; 
for (i=0; i< numOfSequenceParameterSets; i++) { 
unsigned int(16) sequenceParameterSetLength ; 
bit(8*sequenceParameterSetLength) sequenceParameterSetNALUnit; 
} 
unsigned int(8) numOfPictureParameterSets; 
for (i=0; i< numOfPictureParameterSets; i++) { 
unsigned int(16) pictureParameterSetLength; 
bit(8*pictureParameterSetLength) pictureParameterSetNALUnit; 
} 
}
下面蓝色的部分就是 FLV 文件中的 AVCDecoderConfigurationRecord 部分。
00000130h: 00 00 00 17 00 00 00 00 01 4D 40 15 FF E1 00 0A ; .........M@.?. 
00000140h: 67 4D 40 15 96 53 01 00 4A 20 01 00 05 68 E9 23 ; gM@.朣..J ...h? 
00000150h: 88 00 00 00 00 2A 08 00 00 52 00 00 00 00 00 00 ; ?...*...R......
根据 AVCDecoderConfigurationRecord 结构的定义：
- 
configurationVersion = 01
- 
AVCProfileIndication = 4D
- 
profile_compatibility = 40
- 
AVCLevelIndication = 15
- 
lengthSizeMinusOne = FF <- 非常重要，是 H.264 视频中 NALU 的长度，计算方法是 1 + (lengthSizeMinusOne
 & 3)
- 
numOfSequenceParameterSets = E1 <- SPS 的个数，计算方法是 numOfSequenceParameterSets
 & 0x1F
- 
sequenceParameterSetLength = 00 0A <- SPS 的长度
- 
sequenceParameterSetNALUnits = 67 4D 40 15 96 53 01 00 4A 20 <- SPS
- 
numOfPictureParameterSets = 01 <- PPS 的个数
- 
pictureParameterSetLength = 00 05 <- PPS 的长度
- 
pictureParameterSetNALUnits = 68 E9 23 88 00 <- PPS
因此 CodecPrivateData 的字符串表示就是 000001674D4015965301004A2000000168E9238800
但是设置 MediaStreamAttributeKeys.CodecPrivateData 是没用的（只有 H.264 是这样，其他类型的视频流仍然需要设置），只有将
 CodecPrivateData 写入 H.264 视频流第一帧数据的前面 Silverlight 才能正常解码。
也就是说，Silverlight 的 H.264 解码器会读取第一帧前面的 CodecPrivateData 数据来进行配置。
因为 CodecPrivateData 数据已经包含视频流的解码器参数（包括视频的宽高），所以就不需要设置 MediaStreamAttributeKeys.CodecPrivateData、MediaStreamAttributeKeys.Width 和 MediaStreamAttributeKeys.Height 了。
计算 NALU 得到帧数据
FLV 文件中 VIDEOTAG 的 VIDEODATA 的 AVCVIDEOPACKET 的 Data 不是原始视频帧数据，而是一个或更多个 NALU 数据片段。在这篇文章中，你认为 H.264 视频帧数据是由多个 NALU 组成的。当然实际上并不是这样，关于这部分的概念请自行 Google，本文将不做讨论。
下面是 FLV 文件中 VIDEOTAG 的 VIDEODATA 的 AVCVIDEOPACKET 的 Data 属性的数据（第一帧数据）。
- 
红色的部分是 NALU 数据的长度，而红色部分的长度则由 lengthSizeMinusOne 决定。
- 
蓝色的部分是 NALU 数据部分。
- 
删除的部分是废弃的数据。
00000300h: 00 00 00 00 00 17 01 00 00 22 00 00 00 3165
 88 ; ........."...1e? 
00000310h: 80 40 05 B7 95 53 67 FF 84 6C 07 EB 00 F8 45 FB ; €@.窌Sg刲.?鳨? 
00000320h: F9 15 71 0D A4 C5 2C 00 00 03 00 00 03 00 3F 2B ; ?q.づ,.......?+ 
00000330h: 5B 06 57 48 29 F4 08 00 00 0A 10 02 D0 7A FE00 ;
 [.WH)?.....衵? 
00000340h: 00 00 3865
 01 22 22 01 00 17 B7 95 53 67 FF 84 ; ..8e.""...窌Sg? 
00000350h: 6C 07 EB 00 F8 45 FB F9 15 71 0D A4 C5 2C 00 E8 ; l.?鳨.q.づ,.? 
00000360h: F3 37 75 43 90 00 00 03 00 15 EF AA A8 53 86 01 ; ?uC?....铼⊿? 
00000370h: DD 57 60 00 00 03 01 59 0C F4 3C00
 00 00 3365 ; 軼`....Y.?...3e 
00000380h: 00 90 88 80 40 05 B7 95 53 67 FF 84 6C 07 EB 00 ; .悎€@.窌Sg刲.? 
00000390h: F8 45 FB F9 15 71 0D A4 C5 2C 00 00 03 00 00 03 ; 鳨.q.づ,...... 
000003a0h: 00 3F 2B 5B 06 57 48 29 F4 08 00 00 0A 10 02 D0 ; .?+[.WH)?.....? 
000003b0h: 7A FE00
 00 00 3865 00 D8 88 80 40 05 B7 95 53 ; z?..8e.貓€@.窌S 
000003c0h: 67 FF 84 6C 07 EB 00 F8 45 FB F9 15 71 0D A4 C5 ; g刲.?鳨.q.づ 
000003d0h: 2C 00 E8 F3 37 75 43 90 00 00 03 00 15 EF AA A8 ; ,.梵7uC?....铼? 
000003e0h: 53 86 01 DD 57 60 00 00 03 01 59 0C F4 3C00
 00 ; S?軼`....Y.?.. 
000003f0h: 00 F4 08 00 01 33 00 00 17 00 00 00 00 AF 01 27 ; .?..3.......?'
帧数据是将多个 NALU 使用 byte[] {00, 00, 01} 连接的字节数组。
byte[] = {
00,00,01,65,88, 
80,40,05,B7,95,53,67,FF,84,6C,07,EB,00,F8,45,FB, 
F9,15,71,0D,A4,C5,2C,00,00,03,00,00,03,00,3F,2B, 
5B,06,57,48,29,F4,08,00,00,0A,10,02,D0,7A,FE,
00,00,01,65,01,22,22,01,00,17,B7,95,53,67,FF,84, 
6C,07,EB,00,F8,45,FB,F9,15,71,0D,A4,C5,2C,00,E8, 
F3,37,75,43,90,00,00,03,00,15,EF,AA,A8,53,86,01, 
DD,57,60,00,00,03,01,59,0C,F4,3C,
00,00,01,65, 
00,90,88,80,40,05,B7,95,53,67,FF,84,6C,07,EB,00, 
F8,45,FB,F9,15,71,0D,A4,C5,2C,00,00,03,00,00,03, 
00,3F,2B,5B,06,57,48,29,F4,08,00,00,0A,10,02,D0, 
7A,FE,
00,00,01,65,00,D8,88,80,40,05,B7,95,53, 
67,FF,84,6C,07,EB,00,F8,45,FB,F9,15,71,0D,A4,C5, 
2C,00,E8,F3,37,75,43,90,00,00,03,00,15,EF,AA,A8, 
53,86,01,DD,57,60,00,00,03,01,59,0C,F4,3C
};
如果是第一帧数据，那么前面还要加上 CodecPrivateData 数据。
byte[] = {
00,00,01,67,4D,40,15,96,53,01,00,4A,20,
00,00,01,68,E9,23,88,00,
00,00,01,65,88, 
80,40,05,B7,95,53,67,FF,84,6C,07,EB,00,F8,45,FB, 
F9,15,71,0D,A4,C5,2C,00,00,03,00,00,03,00,3F,2B, 
5B,06,57,48,29,F4,08,00,00,0A,10,02,D0,7A,FE,
00,00,01,65,01,22,22,01,00,17,B7,95,53,67,FF,84, 
6C,07,EB,00,F8,45,FB,F9,15,71,0D,A4,C5,2C,00,E8, 
F3,37,75,43,90,00,00,03,00,15,EF,AA,A8,53,86,01, 
DD,57,60,00,00,03,01,59,0C,F4,3C,
00,00,01,65, 
00,90,88,80,40,05,B7,95,53,67,FF,84,6C,07,EB,00, 
F8,45,FB,F9,15,71,0D,A4,C5,2C,00,00,03,00,00,03, 
00,3F,2B,5B,06,57,48,29,F4,08,00,00,0A,10,02,D0, 
7A,FE,
00,00,01,65,00,D8,88,80,40,05,B7,95,53, 
67,FF,84,6C,07,EB,00,F8,45,FB,F9,15,71,0D,A4,C5, 
2C,00,E8,F3,37,75,43,90,00,00,03,00,15,EF,AA,A8, 
53,86,01,DD,57,60,00,00,03,01,59,0C,F4,3C
};
### [关于h264的avc box中数据的提取。sps pps的获取。](http://blog.csdn.net/peijiangping1989/article/details/6934317)
如果你想获取视频中的avc数据，而不要音频的数据，你就需要录制一段不带音频的视频。
录制完成后保存视频，然后用WinHex打开。如果你是手机需要的视频就录制3gp格式。
然后CTRL+F搜索avc.可以看到如图：
![](http://hi.csdn.net/attachment/201111/4/0_1320368867i320.gif)
把鼠标点击到avc附近，左边也就在01这个数据附近了。
好了avc box的内容就是从01开始，比如上图就是：
{ 0x01, 0x42, (byte) 0x80, 0x0A,(byte) 0xFF,
(byte) 0xE1, 0x00, 0x12, 0x67, 0x42, (byte) 0x80, 0x0A,
(byte) 0xE9, 0x02, (byte) 0xC1, 0x29, 0x08, 0x00, 0x00,
0x1F, 0x40, 0x00, 0x04, (byte) 0xE2, 0x00, 0x20, 0x01,
0x00, 0x04, 0x68, (byte) 0xCE, 0x3C, (byte) 0x80 };
sps信息就是67开头 67-20  pps就是68-80 具体数据意义可以看上篇博客。
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# [Android使用VideoView播放RTSP视频](http://yinjun622.sinaapp.com/2012/02/07/android%E4%BD%BF%E7%94%A8videoview%E6%92%AD%E6%94%BErtsp%E8%A7%86%E9%A2%91/)
昨天经过编译配置live555MediaServer.exe，然后通过VLC播放rtsp://88.88.88.200:554/1.mp3后，本以为很简单的就可以通过Android的VideoView播放音频。很纠结的提示不支持。
经过一天的搜索，得出了结论。
官方文档：[Android
 Supported Media Formats](http://developer.android.com/guide/appendix/media-formats.html)提到
The following network protocols are supported for audio and video playback:
> 
RTSP (RTP, SDP)
HTTP/HTTPS progressive streaming
HTTP/HTTPS live streaming draft protocol:
MPEG-2 TS media files only
Protocol version 3 (Android 4.0 and above)
Protocol version 2 (Android 3.x)
Not supported before Android 3.0
第一条说明了支持RTSP后面包含了RTP和SDP流。其他的流可能暂时不支持。
而对于网上一个代码例子
`<?``xml``version``=``"1.0"``encoding``=``"utf-8"``?>`
`<``LinearLayout``xmlns:android``=``"[http://schemas.android.com/apk/res/android](http://schemas.android.com/apk/res/android)"`
`    ``android:layout_width``=``"fill_parent"`
`    ``android:layout_height``=``"fill_parent"`
`    ``android:orientation``=``"vertical"``>`
`    ``<``TextView`
`        ``android:layout_width``=``"fill_parent"`
`        ``android:layout_height``=``"wrap_content"`
`        ``android:text``=``"@string/hello"``/>`
`    ``<``VideoView`
`        ``android:id``=``"@+id/videoView1"`
`        ``android:layout_width``=``"match_parent"`
`        ``android:layout_height``=``"wrap_content"``/>`
`</``LinearLayout``>`
`package``com.jouhu.live;`
`import``android.app.Activity;`
`import``android.net.Uri;`
`import``android.os.Bundle;`
`import``android.widget.VideoView;`
`public``class``Live555RTSPActivity ``extends`
```
Activity
 {
```
`    `
```
/**
 Called when the activity is first created. */
```
`    `
```
VideoView
 videoView ;
```
`    ``@Override`
`    ``public``void`
```
onCreate(Bundle
 savedInstanceState) {
```
`        ``super``.onCreate(savedInstanceState);`
`        ``setContentView(R.layout.main);`
`        `
```
videoView
 = (VideoView)findViewById(R.id.videoView1);
```
`        ``PlayRtspStream(``"[rtsp://218.204.223.237:554/live/1/66251FC11353191F/e7ooqwcfbqjoo80j.sdp]()"``);`
`    ``}`
`    ``private``void`
```
PlayRtspStream(String
 rtspUrl)
```
`    ``{`
`        `
```
videoView
 = (VideoView)findViewById(R.id.videoView1);
```
`        ``videoView.setVideoURI(Uri.parse(rtspUrl));`
`        ``videoView.requestFocus();`
`        ``videoView.start();`
`    ``}`
`}`
从代码编译结果，的确可以显示出来。跟后面使用的SDP应该有关。
然后开始搜索如何生成SDP的问题。
使用FFmpeg可以生成相应的SDP文件，相应的命令行为
> 
F:\AndroidDevelop\LiveVideoSolution\FFmpeg-FFServer\ffmpeg-git-d049257-win32-static\bin>ffmpeg -fflags +genpts -re -i 1.avi -vcodec copy -an -f rtp rtp://127.0.0.1:10000 -vn -acodec copy -f rtp rtp://127.0.0.1:20000 > 2.sdp
`SDP:`
`v=0`
```
o=-
 0 0 IN IP4 127.0.0.1
```
```
s=No
 Name
```
```
t=0
 0
```
```
a=tool:libavformat
 53.17.0
```
```
m=video
 10000 RTP/AVP 96
```
```
c=IN
 IP4 127.0.0.1
```
```
a=rtpmap:96
 H264/90000
```
```
a=fmtp:96
 packetization-mode=1; sprop-parameter-sets=Z2QAM6w07AgDHlkwEQAAAwPpAAC7gI8YMTg=,aO68sA==
```
```
m=audio
 20000 RTP/AVP 14
```
```
c=IN
 IP4 127.0.0.1
```
`b=AS:48`
参考文章：
1 http://www.cuitu.net/content/ffmpegzai-windowsxia-bu-huo-she-xiang-tou-shi-pin
2 http://developer.android.com/guide/appendix/media-formats.html
目前在做视频应用的时候，比较先进的技术就是RTSP流媒体了，那么如果利用Android的播放控件VideoView来播放RTSP的流呢?
RTSP流媒体链接:
[http://218.204.223.237:8081/wap/](http://218.204.223.237:8081/wap/)
这个链接含有所有的RTSP流媒体的链接，现在咱们就用VideoView来播放里面的RTSP的流,咱们以其中的一个链接来测试下好了:
rtsp://218.204.223.237:554/live/1/66251FC11353191F/e7ooqwcfbqjoo80j.sdp.
效果截图:
![VideoView播放RTSP流](http://www.shouyanwang.org/data/attachment/forum/201104/17/214351s88aa358jj7o5p88.png)
核心代码如下:
- 
package com.video.rtsp;
- 
import android.app.Activity;
- 
import android.net.Uri;
- 
import android.os.Bundle;
- 
import android.view.View;
- 
import android.widget.Button;
- 
import android.widget.EditText;
- 
import android.widget.VideoView;
- 
public class rtspActivity extends Activity {
- 
/** Called when the activity is first created. */
- 
Button playButton ;
- 
VideoView videoView ;
- 
EditText rtspUrl ;
- 
@Override
- 
public void onCreate(Bundle savedInstanceState) {
- 
super.onCreate(savedInstanceState);
- 
setContentView(R.layout.main);
- 
rtspUrl = (EditText)this.findViewById(R.id.url);
- 
playButton = (Button)this.findViewById(R.id.start_play);
- 
playButton.setOnClickListener(new Button.OnClickListener(){
- 
public void onClick(View v) {
- 
PlayRtspStream(rtspUrl.getEditableText().toString());
- 
}
- 
});
- 
videoView = (VideoView)this.findViewById(R.id.rtsp_player);
- 
}
- 
//play rtsp stream
- 
private void PlayRtspStream(String rtspUrl){
- 
videoView.setVideoURI(Uri.parse(rtspUrl));
- 
videoView.requestFocus();
- 
videoView.start();
- 
}
- 
}
复制代码
在点击开始播放后，一般要等个10几秒中才开始播放的，直接的设置需要播放的RTSP的地址:setVideoURI(rtsp的地址)
![](http://www.shouyanwang.org/static/image/filetype/rar.gif)[VideoViewRtsp.rar](http://www.shouyanwang.org/forum-attachment-aid-NzJ8ZTYxMTFjYTZ8MTMyODYyMDI4NHww.html)(23.4
[lonpo](http://f123.info/) 在 [2012
 年 3 月 17 日下午 3:09](http://yinjun622.sinaapp.com/2012/02/07/android%e4%bd%bf%e7%94%a8videoview%e6%92%ad%e6%94%bertsp%e8%a7%86%e9%a2%91/#comment-6)说道：
非常谢谢了，这篇文章给我很大的启示。为什么要使用
[rtsp://218.204.223.237:554/live/1/66251FC11353191F/e7ooqwcfbqjoo80j.sdp]()
而不是rtsp://。。。。.mp4 为什么要后缀sdp呢？不能直接调用视频文件吗？
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
### [Android用MediaRecorder实现MPEG4视频监控](http://tuojian-tj.iteye.com/blog/973995)
[Android](http://www.iteye.com/blogs/tag/Android)[Socket](http://www.iteye.com/blogs/tag/Socket)[HTC](http://www.iteye.com/blogs/tag/HTC)[网络应用](http://www.iteye.com/blogs/tag/%E7%BD%91%E7%BB%9C%E5%BA%94%E7%94%A8)[框架](http://www.iteye.com/blogs/tag/%E6%A1%86%E6%9E%B6)
都是摸着石头过河，花了整整一个星期，终于把技术难点给突破了，貌似网上对这个讨论的较少。 
主要需要实现的功能是在android手机上实时采集视频，并在远程比如PC机上实时显示出来，也就是以android手机作为监控摄像头。 
一开始查到的是smartcam的一个开源项目，看了下源代码，发现其实现原理是利用android 的camera图像的预采集，通过实现PreviewCallback类的回调函数onPreviewFrame，获得camera采集的原始图像数据之后，压成jpeg格式传到pc端。pc端对接收到的jpeg图像序列进行实时解压和显示，就达到了预想的效果。 
虽然这种方式稍微显得比较笨拙，这个方式还可以接受。但是不可接受的是jpeg只是帧内压缩，320x280的图片序列，FPS大概是10上下，网络流量就到达了100kb/s以上。这个几乎是无法实际应用的。 
于是必须直接传视频流，MPEG4或者H.264格式。貌似我的开发机上(HTC G8)只支持到MPEG4，所以还是选取MPEG4。但是如何实时采集视频流是一个大问题，毕竟在video方面，android并没有提供一个类似于OnPreviewFrame的回调函数。 
想到用opencore或者更为新一点的stagefright，大概看看了其sdk的框架后，马上泄气了，这个太庞大了。在http://blog.csdn.net/zblue78/archive/2010/12/18/6083374.aspx的帖子中提到一个很好的解决方案，就是利用MediaRecorder：MediaRecorder的输出路径（其实叫file descriptor）除了是本地文件路径之外，还可以绑定socket端口。也就是说，通过一个socket端口，就可以实时获得MediaRecorder的视频流数据。 
（其实上面博客的内容可以在开源项目sipdroid 的 videocamera文件中找到，但是非常感谢博客主人zhangzhenj对网友提问的回答，赞一个。） 
通过socket接收的视频流与直接写在本地文件的视频流数据有点不一样，因为是通过socket传输，就无法对视频文件的回写，通常MediaRecorder结束录像的时候都会对视频文件进行回写处理，这样才可以被播放器播放。所以通过socket接受到的数据，保存下来是无法播放的。16进制方式查看了一下其输出文件，发现其前32byte都是00，紧接着就是mdat。问题就出现在这了：缺少了一个ftyp box 的描述（28 bytes）以及mdat的长度描述(4 bytes).网上已经有人顺利解决这样的问题，在数据中查找moov的起始位置，发现前面会有ftyp的描述，长度刚刚好28bytes。你可以copy这28bytes到文件开始的28byte中。这ftyp的描述是从moov的起始位置
 的前32byte开始一直到前4byte(后面4byte是moov的长度描述)。然后mdat的长度就是 moov的起始位置 减去 0x20，道理就不解释了。然后把这个值写到mdat的前面4byte。刚刚好填满32byte，之后就能顺利播放了。 
保存好的文件能播放之后，最后一个问题，如何在实时显示这个视频流呢？查看一下mpeg4的文件格式，很快就会知道答案，答案就在mdat中。mdat之后紧跟的就是视频媒体数据，每一帧以 00 00 01 b6 为开始标志，貌似没有结束标志，分帧的话估计要用这个。开始标志后紧接着的两bit就是I、P、B帧的标志了，分别对应值为00,01,10，记住是两bit不是两byte 
好了，把mdat的一帧数据取出来，可以用ffmpeg解码，然后显示，这样的路子是可行的，不过细节还是有点麻烦，关键是ffmpeg在解码mpeg4的时候一定要先指定width和height，否则解码失败。 
大概思路就是这样了，完整的代码还没出来。以后再说。
