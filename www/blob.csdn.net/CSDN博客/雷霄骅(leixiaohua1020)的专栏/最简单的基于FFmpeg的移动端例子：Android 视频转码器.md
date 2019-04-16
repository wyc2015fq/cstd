# 最简单的基于FFmpeg的移动端例子：Android 视频转码器 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2015年07月25日 17:10:54[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：72262标签：[ffmpeg																[Android																[转码																[JNI																[视频](https://so.csdn.net/so/search/s.do?q=视频&t=blog)
个人分类：[Android多媒体																[FFMPEG](https://blog.csdn.net/leixiaohua1020/article/category/1360795)](https://blog.csdn.net/leixiaohua1020/article/category/5680665)

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


本文记录一个安卓平台下基于FFmpeg的视频转码器。该转码器实际上移植自ffmpeg工程中的ffmpeg.c源代码。有关ffmpeg.c的源代码可以参考文章《[ffmpeg.c函数结构简单分析（画图）](http://blog.csdn.net/leixiaohua1020/article/details/39760711)》，在这里就不重复记录了。

![](https://img-blog.csdn.net/20150801182543446)





## 源代码

项目的目录结构如图所示。Java源代码位于src目录，而C代码位于jni目录。

![](https://img-blog.csdn.net/20150725145120980)

Android程序Java端代码位于src\com\leixiaohua1020\sffmpegandroidtranscoder\MainActivity.java，如下所示。

```java
/**
 * 最简单的基于FFmpeg的转码器-安卓
 * Simplest FFmpeg Android Transcoder
 * 
 * 雷霄骅  Lei Xiaohua
 * leixiaohua1020@126.com
 * 马小雨  Ma Xiaoyu
 * maxiaoyucuc@163.com
 * 中国传媒大学/数字电视技术
 * Communication University of China / Digital TV Technology
 * http://blog.csdn.net/leixiaohua1020
 * 
 * 
 * 本程序是安卓平台下的转码器。它移植于ffmpeg.c命令行工具。
 * 
 * This software is a Transcoder in Android.
 * It is transplanted from ffmpeg.c command line tools.
 * 
 */
package com.leixiaohua1020.sffmpegandroidtranscoder;

import android.os.Bundle;
import android.os.Environment;
import android.app.Activity;
import android.util.Log;
import android.view.Menu;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;

public class MainActivity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        
        final EditText cmdEdittext= (EditText) this.findViewById(R.id.editText_cmd);
        Button startButton= (Button) this.findViewById(R.id.button_start);
        
        startButton.setOnClickListener(new OnClickListener() {
			public void onClick(View arg0){
				String cmdline=cmdEdittext.getText().toString();
		        String[] argv=cmdline.split(" ");
		        Integer argc=argv.length;
		        ffmpegcore(argc,argv);
			}
		});
    }

    public native int ffmpegcore(int argc,String[] argv);
    static{
    	System.loadLibrary("avutil-54");
    	System.loadLibrary("swresample-1");
    	System.loadLibrary("avcodec-56");
    	System.loadLibrary("avformat-56");
    	System.loadLibrary("swscale-3");
    	System.loadLibrary("postproc-53");
    	System.loadLibrary("avfilter-5");
    	System.loadLibrary("avdevice-56");
    	System.loadLibrary("sfftranscoder");
    }
    
    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.main, menu);
        return true;
    }
    
}
```

C语言端源代码包含多个文件。其中下面文件拷贝自FFmpeg源代码：

> *cmdutils.ccmdutils.hcmdutils_common_opts.hconfig.hffmpeg.hffmpeg_filter.cffmpeg_opt.c*

在编译ffmpeg.c的时候，除了需要FFmpeg SDK中的头文件之外，还需要从源代码中拷贝下面头文件。

> *libavformat/ffm.hlibavformat/network.hlibavformat/os_support.hlibavformat/url.hlibavutil/libm.h*

ffmpeg_mod.c是修改后的ffmpeg.c文件。ffmpeg.c本身是用于命令行使用的程序，因此在改为类库调用的时候需要对源代码做一些修改。例如去掉一些exit(0)，释放一些变量，main()函数改名为ffmpegmain()等。

ffmpeg_jni.c是ffmpeg.c和Android系统之间的接口。该部分的源代码如下所示。



```cpp
/**
 * 最简单的基于FFmpeg的转码器-安卓
 * Simplest FFmpeg Android Transcoder
 *
 * 雷霄骅  Lei Xiaohua
 * leixiaohua1020@126.com
 * 马小雨  Ma Xiaoyu
 * maxiaoyucuc@163.com
 * 中国传媒大学/数字电视技术
 * Communication University of China / Digital TV Technology
 * http://blog.csdn.net/leixiaohua1020
 *
 *
 * 本程序是安卓平台下的转码器。它移植于ffmpeg.c命令行工具。
 *
 * This software is a Transcoder in Android.
 * It is transplanted from ffmpeg.c command line tools.
 *
 */

#include <string.h>
#include <jni.h>
#include <ffmpeg.h>

#ifdef ANDROID
#include <jni.h>
#include <android/log.h>
#define LOGE(format, ...)  __android_log_print(ANDROID_LOG_ERROR, "(>_<)", format, ##__VA_ARGS__)
#define LOGI(format, ...)  __android_log_print(ANDROID_LOG_INFO,  "(^_^)", format, ##__VA_ARGS__)
#else
#define LOGE(format, ...)  printf("(>_<) " format "\n", ##__VA_ARGS__)
#define LOGI(format, ...)  printf("(^_^) " format "\n", ##__VA_ARGS__)
#endif


int ffmpegmain(int argc, char **argv);

//Output FFmpeg's av_log()
void custom_log(void *ptr, int level, const char* fmt, va_list vl){

	//To TXT file

	FILE *fp=fopen("/storage/emulated/0/av_log.txt","a+");
	if(fp){
		vfprintf(fp,fmt,vl);
		fflush(fp);
		fclose(fp);
	}


	//To Logcat
	//LOGE(fmt, vl);
}

JNIEXPORT jint JNICALL Java_com_leixiaohua1020_sffmpegandroidtranscoder_MainActivity_ffmpegcore( JNIEnv * env, jobject thiz, jint cmdnum, jobjectArray cmdline)
{

  //FFmpeg av_log() callback
  av_log_set_callback(custom_log);

  int argc=cmdnum;
  char** argv=(char**)malloc(sizeof(char*)*argc);
  
  int i=0;
  for(i=0;i<argc;i++)
  {
    jstring string=(*env)->GetObjectArrayElement(env,cmdline,i);
    const char* tmp=(*env)->GetStringUTFChars(env,string,0);
    argv[i]=(char*)malloc(sizeof(char)*1024);
    strcpy(argv[i],tmp);
  }
  
  ffmpegmain(argc,argv);

  for(i=0;i<argc;i++){
    free(argv[i]);
  }
  free(argv);
  return 0;

}
```



Android.mk文件位于jni/Android.mk，如下所示。



```
# Android.mk for FFmpeg
#
# Lei Xiaohua 雷霄骅
# leixiaohua1020@126.com
# http://blog.csdn.net/leixiaohua1020
# 

LOCAL_PATH := $(call my-dir)

# FFmpeg library
include $(CLEAR_VARS)
LOCAL_MODULE := avcodec
LOCAL_SRC_FILES := libavcodec-56.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := avdevice
LOCAL_SRC_FILES := libavdevice-56.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := avfilter
LOCAL_SRC_FILES := libavfilter-5.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := avformat
LOCAL_SRC_FILES := libavformat-56.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := avutil
LOCAL_SRC_FILES := libavutil-54.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := postproc
LOCAL_SRC_FILES := libpostproc-53.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := swresample
LOCAL_SRC_FILES := libswresample-1.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := swscale
LOCAL_SRC_FILES := libswscale-3.so
include $(PREBUILT_SHARED_LIBRARY)

# Program
include $(CLEAR_VARS)
LOCAL_MODULE := sfftranscoder
LOCAL_SRC_FILES :=ffmpeg_jni.c ffmpeg_mod.c ffmpeg_opt.c ffmpeg_filter.c cmdutils.c
LOCAL_C_INCLUDES += $(LOCAL_PATH)/include
LOCAL_LDLIBS := -llog -lz
LOCAL_SHARED_LIBRARIES := avcodec avdevice avfilter avformat avutil postproc swresample swscale
include $(BUILD_SHARED_LIBRARY)
```




## 运行结果

App在手机上运行后的结果如下图所示。

![](https://img-blog.csdn.net/20150725145258593)


注意需要把等待转码的视频文件拷贝至存储卡相应的目录中。例如对于上述截图的情况，需要将sintel.mp4拷贝至存储卡的根目录中。


单击“Start”按钮开始转码，将存储卡中的sintel.mp4转码为sintel.mkv。

![](https://img-blog.csdn.net/20150725145145986)



## 下载

**simplest ffmpeg mobile项目主页**

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
simplest_sdl_ios_helloworld: 移植SDL到IOS平台的最简单程序](https://so.csdn.net/so/search/s.do?q=JNI&t=blog)](https://so.csdn.net/so/search/s.do?q=转码&t=blog)](https://so.csdn.net/so/search/s.do?q=Android&t=blog)](https://so.csdn.net/so/search/s.do?q=ffmpeg&t=blog)




