# 最简单的基于FFmpeg的移动端例子附件：Android 自带播放器 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2015年07月25日 20:00:30[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：24659
所属专栏：[FFmpeg](https://blog.csdn.net/column/details/ffmpeg-devel.html)











=====================================================

最简单的基于FFmpeg的移动端例子系列文章列表：

[最简单的基于FFmpeg的移动端例子：Android HelloWorld](http://blog.csdn.net/leixiaohua1020/article/details/47008825)

[最简单的基于FFmpeg的移动端例子：Android 视频解码器](http://blog.csdn.net/leixiaohua1020/article/details/47010637)

[最简单的基于FFmpeg的移动端例子：Android 视频解码器-单个库版](http://blog.csdn.net/leixiaohua1020/article/details/47011021)

[最简单的基于FFmpeg的移动端例子：Android 推流器](http://blog.csdn.net/leixiaohua1020/article/details/47056051)

[最简单的基于FFmpeg的移动端例子：Android 视频转码器](http://blog.csdn.net/leixiaohua1020/article/details/47056365)

[最简单的基于FFmpeg的移动端例子附件：Android 自带播放器](http://blog.csdn.net/leixiaohua1020/article/details/47056733)

[最简单的基于FFmpeg的移动端例子附件：SDL Android HelloWorld](http://blog.csdn.net/leixiaohua1020/article/details/47059553)

[最简单的基于FFmpeg的移动端例子：IOS HelloWorld](http://blog.csdn.net/leixiaohua1020/article/details/47071547)

[最简单的基于FFmpeg的移动端例子：IOS 视频解码器](http://blog.csdn.net/leixiaohua1020/article/details/47072257)

[最简单的基于FFmpeg的移动端例子：IOS 推流器](http://blog.csdn.net/leixiaohua1020/article/details/47072519)

[最简单的基于FFmpeg的移动端例子：IOS 视频转码器](http://blog.csdn.net/leixiaohua1020/article/details/47072673)

[最简单的基于FFmpeg的移动端例子附件：IOS自带播放器](http://blog.csdn.net/leixiaohua1020/article/details/47072861)

[最简单的基于FFmpeg的移动端例子：Windows Phone HelloWorld](http://blog.csdn.net/leixiaohua1020/article/details/47191283)

=====================================================


前几篇文章记录的都是安卓平台下基于FFmpeg的多媒体程序。作为对比，本文记录一个基于安卓自带多媒体框架OpenCore的视频播放器。
安卓自带的多媒体框架的使用方法相对移植FFmpeg来说简单太多了，只需要调用几个接口函数就能控制视频的播放。不再过多记录它的使用方法。

![](https://img-blog.csdn.net/20150801182504426)





## 源代码

项目的目录结构如图所示。Java源代码位于src目录。

![](https://img-blog.csdn.net/20150725152201771)

Android程序Java端代码位于src\com\leixiaohua1020\sandroidplayer\MainActivity.java，如下所示。

```java
/**
 * 
 * 最简单的安卓视频播放器
 * Simplest Android Player
 * 
 * 雷霄骅 Lei Xiaohua
 * leixiaohua1020@126.com
 * 中国传媒大学/数字电视技术
 * Communication University of China / Digital TV Technology
 * http://blog.csdn.net/leixiaohua1020
 * 
 * 本程序是基于安卓接口的视频播放器。
 * 
 * Video player based on Android API.
 * 
 */
package com.leixiaohua1020.sandroidplayer;

import android.os.Bundle;
import android.os.Environment;
import android.app.Activity;
import android.util.Log;
import android.view.Menu;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.VideoView;

public class MainActivity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

		final VideoView videoView = (VideoView) findViewById(R.id.videoView1);
		
		Button setButton = (Button) this.findViewById(R.id.button_set);
		Button startButton = (Button) this.findViewById(R.id.button_start);
		Button pauseButton = (Button) this.findViewById(R.id.button_pause);
		Button stopButton = (Button) this.findViewById(R.id.button_stop);
		final EditText urlEdittext= (EditText) this.findViewById(R.id.input_url);
		
		
        setButton.setOnClickListener(new OnClickListener() {
			public void onClick(View arg0){
				String folderurl=Environment.getExternalStorageDirectory().getPath();
				String urltext=urlEdittext.getText().toString();
				//Small FIX, Avoid '/'
				if(urltext.charAt(0)=='/'){
					urltext=urltext.substring(1);
				}
		        String inputurl=folderurl+"/"+urltext;
		        Log.e("url",inputurl);
		       
				videoView.setVideoPath(inputurl);
				//videoView.setMediaController(new MediaController(MainActivity.this));
			}
		});
        
		startButton.setOnClickListener(new OnClickListener() {
			public void onClick(View arg0){
				videoView.start();
			}
		});
		
		pauseButton.setOnClickListener(new OnClickListener() {
			public void onClick(View arg0){
				videoView.pause();
			}
		});
		
		stopButton.setOnClickListener(new OnClickListener() {
			public void onClick(View arg0){
				videoView.stopPlayback();
			}
		});
        
    }


    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.main, menu);
        return true;
    }
    
}
```


## 运行结果

App在手机上运行后的结果如下图所示。单击“Set”、“Start”、“Pause”、“Stop”几个按钮可以对视频做不同的操作。

注意需要把等待播放的视频文件拷贝至存储卡相应的目录中。例如对于下面截图的情况，需要将sintel.mp4拷贝至存储卡的根目录中。



![](https://img-blog.csdn.net/20150725152021319)




## 下载

**simplest ffmpeg mobile**



**项目主页**

Github：[https://github.com/leixiaohua1020/simplest_ffmpeg_mobile](https://github.com/leixiaohua1020/simplest_ffmpeg_mobile)

开源中国：[https://git.oschina.net/leixiaohua1020/simplest_ffmpeg_mobile](https://git.oschina.net/leixiaohua1020/simplest_ffmpeg_mobile)

SourceForge：[https://sourceforge.net/projects/simplestffmpegmobile/](https://sourceforge.net/projects/simplestffmpegmobile/)





CSDN工程下载地址：[http://download.csdn.net/detail/leixiaohua1020/8924391](http://download.csdn.net/detail/leixiaohua1020/8924391)



本解决方案包含了使用FFmpeg在移动端处理多媒体的各种例子：

> [Android]
simplest_android_player: 基于安卓接口的视频播放器
simplest_ffmpeg_android_helloworld: 安卓平台下基于FFmpeg的HelloWorld程序
simplest_ffmpeg_android_decoder: 安卓平台下最简单的基于FFmpeg的视频解码器
simplest_ffmpeg_android_decoder_onelib: 安卓平台下最简单的基于FFmpeg的视频解码器-单库版
simplest_ffmpeg_android_streamer: 安卓平台下最简单的基于FFmpeg的推流器
simplest_ffmpeg_android_transcoder: 安卓平台下移植的FFmpeg命令行工具
simplest_sdl_android_helloworld: 移植SDL到安卓平台的最简单程序
[IOS]
simplest_ios_player: 基于IOS接口的视频播放器
simplest_ffmpeg_ios_helloworld: IOS平台下基于FFmpeg的HelloWorld程序
simplest_ffmpeg_ios_decoder: IOS平台下最简单的基于FFmpeg的视频解码器
simplest_ffmpeg_ios_streamer: IOS平台下最简单的基于FFmpeg的推流器
simplest_ffmpeg_ios_transcoder: IOS平台下移植的ffmpeg.c命令行工具
simplest_sdl_ios_helloworld: 移植SDL到IOS平台的最简单程序




