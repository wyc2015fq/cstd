# 《android多媒体api》之摄像头camera采集原始视频数据 - 建建的博客 - CSDN博客
2018年09月10日 09:52:38[纪建](https://me.csdn.net/u013898698)阅读数：108
《android多媒体api》系列是整合梳理android开发中经常用到的媒体相关api；多媒体开发主要内容有音频、视频录制播放、摄像头操作、录制操作、流媒体、直播、推流、拉流等方面；最近几年移动直播和视频应用发展犹如雨后春笋一般直插云霄，呃。。好吧这段比喻可以不用看了！！，反正行业兴起肯定催生了很多多媒体相关应用开发程序员。那么怎样才能成为多媒体开发程序员，首先必须要熟练使用和了解android自带的多媒体api，并且还要掌握pcm、yuv、rgb、h264、aac、flv、mpegts、mp4、udp、rtp、rtmp等等众多文件格式和流媒体协议等等。所以这里整理android相关多媒体api，提供给想从事流媒体同学作为参照，同样还是要鸣谢网络上那些具有分享精神大神们！！
基本概念：
- 视频播放：demuxer（解复用）->分离出音频流和视频流->decoder(解码)->播放原始数据(例如：pcm yuv)
- 视频录制：采集原始数据(例如：pcm yuv)->encoder(编码)->muxer(封装格式 例如：mp4 3gp)
- 流媒体协议：udp、rtp、rtmp、rtcp、rtsp等
- 音视频封装格式：mp4 、3gp、flv等
- 音视频编码格式：aac、amr、h264、h265等
- 原始音视频数据格式：pcm 、yuv、rgb等
流程图： 
![image](https://img-blog.csdn.net/20160118002918690)
文章目录：
- [VideoView 视频播放控件](http://blog.csdn.net/mhhyoucom/article/details/78802693)
- [camera配合surface预览相机画面和拍照](http://blog.csdn.net/mhhyoucom/article/details/78803076)
- [MediaPlayer自定义视频播放器](http://blog.csdn.net/mhhyoucom/article/details/78810375)
- [MediaRecorder音视频录制api](http://blog.csdn.net/mhhyoucom/article/details/78811499)
- [AudioTrack原始音频pcm播放api](http://blog.csdn.net/mhhyoucom/article/details/78812958)
- [AudioRecord原始音频pcm采集api](http://blog.csdn.net/mhhyoucom/article/details/78812593)
- [摄像头camera采集原始视频数据](http://blog.csdn.net/mhhyoucom/article/details/78825369)
如何通过camera摄像头采集nv21原始数据？ 
直播开发中经常需要获取视频原始帧数据然后前置处理例如：美颜、水印、特效等然后通过编码在通过rtmp或者rtsp等协议方式推流出去，可以完成实时图像传递。那么如何获取原始视频帧数据？安装camera api有一个设置回调的方法，可以将可以通过它来获取原始视频数据如nv21 、 nv12 、 yv12 等。下面构建了一个demo就是当surfaceview控件构建完成后可以获取配置camera来开启预览，这样可以可以在回调方法setPreviewCallback 并且在public void onPreviewFrame(byte[] data, Camera camera) 中获取实时数据了。
由于摄像头是用户隐私权限所以需要申请摄像头权限：
```
<uses-permission android:name="android.permission.CAMERA" />
    <uses-permission android:name="android.permission.WRITE_EXTERNAL_STORAGE"/>
    <uses-feature android:name="android.hardware.camera" />
    <uses-feature android:name="android.hardware.camera.autofocus" />
```
- 1
- 2
- 3
- 4
- 5
基于camera获取原始视频帧数据：
xml布局文件：
```
<RelativeLayout xmlns:android="http://schemas.android.com/apk/res/android"
    xmlns:tools="http://schemas.android.com/tools"
    android:layout_width="match_parent"
    android:layout_height="match_parent"
    tools:context=".MainActivity" >
    <SurfaceView 
        android:id="@+id/show"
        android:layout_width="match_parent"
        android:layout_height="match_parent"/>
</RelativeLayout>
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
java代码：
```
package com.example.getyuvandpush;
import java.io.IOException;
import java.util.List;
import android.app.Activity;
import android.graphics.ImageFormat;
import android.hardware.Camera;
import android.hardware.Camera.Parameters;
import android.hardware.Camera.PreviewCallback;
import android.hardware.Camera.Size;
import android.os.Bundle;
import android.view.SurfaceHolder;
import android.view.SurfaceHolder.Callback;
import android.view.SurfaceView;
@SuppressWarnings("deprecation")
public class MainActivity extends Activity implements Callback, PreviewCallback {
    private SurfaceView show;
    private Camera camera;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        // TODO Auto-generated method stub
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        show=(SurfaceView) findViewById(R.id.show);
        show.getHolder().addCallback(this);
    }
    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        // TODO Auto-generated method stub
    }
    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width,
            int height) {
        // surfaceview 构建完毕并且获取有效宽高时候打开摄像头
        if (camera!=null) {
            return;
        }
        camera= Camera.open();
        Parameters parms = camera.getParameters();
        List<Size> listsize = parms.getSupportedPictureSizes();
        Size sizeOut=null;
        for (Size size : listsize) {
            if (size.width>=width) {
                break;
            }
            sizeOut=size;
        }
        parms.setPreviewSize(sizeOut.width, sizeOut.height);
        parms.setPictureSize(sizeOut.width, sizeOut.height);
        parms.setPreviewFormat(ImageFormat.NV21);
        camera.setParameters(parms);
        camera.setPreviewCallback(this);
        try {
            camera.setPreviewDisplay(holder);
            camera.startPreview();//开始预览
        } catch (IOException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
    }
    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
        // surfaceview 析构关闭摄像头
        if (camera==null) {
            return;
        }
        camera.setPreviewCallback(null);
        camera.stopPreview();// 停止预览
        camera.release();
        camera=null;
    }
    @Override
    public void onPreviewFrame(byte[] data, Camera camera) {
        //可以获取nv21 原始数据 data
    }
}
```
