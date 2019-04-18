# Android系统音视频架构 - 一世豁然的专栏 - CSDN博客





2017年05月16日 10:04:28[一世豁然](https://me.csdn.net/Explorer_day)阅读数：4584








本博客转载于：[http://leeeyou.xyz/2017/04/15/blog-2017-04-15-Android系统音视频架构/](http://leeeyou.xyz/2017/04/15/blog-2017-04-15-Android%E7%B3%BB%E7%BB%9F%E9%9F%B3%E8%A7%86%E9%A2%91%E6%9E%B6%E6%9E%84/)









# 1、Android系统上的音频框架




一个好的系统架构需要尽可能地降低上层与具体硬件的耦合，这既是涉及操作系统的目的，对于音频系统也是如此。音频系统的雏形框架可以简单的用下图来表示：

![](https://github.com/LeeeYou/Img/blob/master/leeeyou/audio.png?raw=true)


在这个图中，除去Linux本身的Audio驱动外，整个Android音频实现都被看成了User。因而我们可以认为Audio Driver就是上层与硬件间的“隔离板”。但是如果单纯采用上图所示的框架来设计音频系统，对上层应用使用音频功能是不小的负担，显然Android开发团队还会根据自身的实际情况来进一步细化“User”部分。


细化的根据还是Android的几个层次结构，包括应用层、framework层、库层以及HAL层，如下图所示：
![](https://github.com/LeeeYou/Img/blob/master/leeeyou/audio2.png?raw=true)

## [](http://leeeyou.xyz/2017/04/15/blog-2017-04-15-Android%E7%B3%BB%E7%BB%9F%E9%9F%B3%E8%A7%86%E9%A2%91%E6%9E%B6%E6%9E%84/#Framework)Framework


相信大家可以马上想到MediaPlayer和MediaRecorder，因为这是我们在开发音频相关产品时使用最广泛的两个类。实际上，Android也提供了另两个相似功能的类，即AudioTrack和AudioRecorder，MediaPlayerService内部的实现就是通过它们来完成的,只不过MediaPlayer/MediaRecorder提供了更强大的控制功能，相比前者也更易于使用。我们后面还会有详细介绍。


除此以外，Android系统还为我们控制音频系统提供了AudioManager、AudioService及AudioSystem类。这些都是framework为便利上层应用开发所设计的。

## [](http://leeeyou.xyz/2017/04/15/blog-2017-04-15-Android%E7%B3%BB%E7%BB%9F%E9%9F%B3%E8%A7%86%E9%A2%91%E6%9E%B6%E6%9E%84/#Libraries)Libraries


我们知道，framework层的很多类，实际上只是应用程序使用Android库文件的“中介”而已。因为上层应用采用java语言编写，它们需要最直接的java接口的支持，这就是framework层存在的意义之一。而作为“中介”，它们并不会真正去实现具体的功能，或者只实现其中的一部分功能，而把主要重心放在库中来完成。比如上面的AudioTrack、AudioRecorder、MediaPlayer和MediaRecorder等等在库中都能找到相对应的类。这一部分代码集中放置在工程的frameworks/av/media/libmedia中，多数是C++语言编写的。


除了上面的类库实现外，音频系统还需要一个“核心中控”，或者用Android中通用的实现来讲，需要一个系统服务(比如ServiceManager、LocationManagerService、ActivityManagerService等等)，这就是AudioFlinger和AudioPolicyService。它们的代码放置在frameworks/av/services/audioflinger，生成的最主要的库叫做libaudioflinger。


音频体系中另一个重要的系统服务是MediaPlayerService，它的位置在frameworks/av/media/libmediaplayerservice。


因为涉及到的库和相关类是非常多的，建议大家在理解的时候分为两条线索。

其一，以库为线索。比如AudioPolicyService和AudioFlinger都是在libaudioflinger库中;而AudioTrack、AudioRecorder等一系列实现则在libmedia库中。


其二，以进程为线索。库并不代表一个进程，进程则依赖于库来运行。虽然有的类是在同一个库中实现的，但并不代表它们会在同一个进程中被调用。比如AudioFlinger和AudioPolicyService都驻留于名为mediaserver的系统进程中;而AudioTrack/AudioRecorder和MediaPlayer/MediaRecorder一样实际上只是应用进程的一部分，它们通过binder服务来与其它系统进程通信。


在分析源码的过程中，一定要紧抓这两条线索，才不至于觉得混乱。

## [](http://leeeyou.xyz/2017/04/15/blog-2017-04-15-Android%E7%B3%BB%E7%BB%9F%E9%9F%B3%E8%A7%86%E9%A2%91%E6%9E%B6%E6%9E%84/#HAL)HAL


从设计上来看，硬件抽象层是AudioFlinger直接访问的对象。这说明了两个问题，一方面AudioFlinger并不直接调用底层的驱动程序;另一方面，AudioFlinger上层(包括和它同一层的MediaPlayerService)的模块只需要与它进行交互就可以实现音频相关的功能了。因而我们可以认为AudioFlinger是Android音频系统中真正的“隔离板”，无论下面如何变化，上层的实现都可以保持兼容。


音频方面的硬件抽象层主要分为两部分，即AudioFlinger和AudioPolicyService。实际上后者并不是一个真实的设备，只是采用虚拟设备的方式来让厂商可以方便地定制出自己的策略。


抽象层的任务是将AudioFlinger/AudioPolicyService真正地与硬件设备关联起来，但又必须提供灵活的结构来应对变化——特别是对于Android这个更新相当频繁的系统。比如以前Android系统中的Audio系统依赖于ALSA-lib，但后期就变为了tinyalsa，这样的转变不应该对上层造成破坏。**因而Audio HAL提供了统一的接口来定义它与AudioFlinger/AudioPolicyService之间的通信方式，这就是audio_hw_device、audio_stream_in及audio_stream_out等等存在的目的，** 这些Struct数据类型内部大多只是函数指针的定义，是一些“壳”。当AudioFlinger/AudioPolicyService初始化时，它们会去寻找系统中最匹配的实现(这些实现驻留在以audio.primary.*, audio.a2dp.* 为名的各种库中)来填充这些“壳”。


根据产品的不同，音频设备存在很大差异，在Android的音频架构中，这些问题都是由HAL层的audio.primary等等库来解决的，而不需要大规模地修改上层实现。换句话说，厂商在定制时的重点就是如何提供这部分库的高效实现了。

# [](http://leeeyou.xyz/2017/04/15/blog-2017-04-15-Android%E7%B3%BB%E7%BB%9F%E9%9F%B3%E8%A7%86%E9%A2%91%E6%9E%B6%E6%9E%84/#2%E3%80%81Android%E7%B3%BB%E7%BB%9F%E4%B8%8A%E7%9A%84%E8%A7%86%E9%A2%91%E6%A1%86%E6%9E%B6)2、Android系统上的视频框架

![](https://github.com/LeeeYou/Img/blob/master/leeeyou/video.png?raw=true)


multimedia framework 架构 由三大部分构成：供上层程序调用的java API，连接java和C/C++的jni部分，多媒体引擎（stagefright）和codec接口（openmax interface）。

![](https://github.com/LeeeYou/Img/blob/master/leeeyou/video2.png?raw=true)

![](https://github.com/LeeeYou/Img/blob/master/leeeyou/video3.png?raw=true)


我们可以发现上层APK要播放视频，首先得获得一个player，而这个player的类型根据你媒体文件的类型来决定的，分配的任务由mediaplayerservice来完成，除了获得player外最主要的是到底选用哪种编码器进行编解码，这个过程由awesomeplayer和omxcodec来完成，至于声音和图像就交由audioflinger和surfaceflinger来完成了。

## [](http://leeeyou.xyz/2017/04/15/blog-2017-04-15-Android%E7%B3%BB%E7%BB%9F%E9%9F%B3%E8%A7%86%E9%A2%91%E6%9E%B6%E6%9E%84/#android%E4%B8%AD%E6%92%AD%E6%94%BE%E8%A7%86%E9%A2%91%E7%9A%84%E4%B8%A4%E7%A7%8D%E6%96%B9%E5%BC%8F)android中播放视频的两种方式

### [](http://leeeyou.xyz/2017/04/15/blog-2017-04-15-Android%E7%B3%BB%E7%BB%9F%E9%9F%B3%E8%A7%86%E9%A2%91%E6%9E%B6%E6%9E%84/#%E9%80%9A%E8%BF%87VideoView%E9%85%8D%E5%90%88MediaPlayerControl%E5%AE%9E%E7%8E%B0)通过VideoView配合MediaPlayerControl实现

#### [](http://leeeyou.xyz/2017/04/15/blog-2017-04-15-Android%E7%B3%BB%E7%BB%9F%E9%9F%B3%E8%A7%86%E9%A2%91%E6%9E%B6%E6%9E%84/#%E6%A6%82%E5%BF%B5%E4%BB%8B%E7%BB%8D)概念介绍


VideoView是android系统提供的一个媒体播放显示和控制的控件。其结构层次如下：


```
VideoView extends SurfaceView implements MediaController.MediaPlayerControl
```




通过VideoView的原型可知：如果构建更为复杂和有特色个性的视频View，需要继承SurfaceView 和实现MediaPlayerControl接口。**其中SurfaceView 为显示提供支持，MediaPlayerControl则为媒体控制提供了支持**

#### [](http://leeeyou.xyz/2017/04/15/blog-2017-04-15-Android%E7%B3%BB%E7%BB%9F%E9%9F%B3%E8%A7%86%E9%A2%91%E6%9E%B6%E6%9E%84/#%E5%9F%BA%E6%9C%AC%E6%96%B9%E6%B3%95%E4%BB%8B%E7%BB%8D)基本方法介绍


VideoView：用于播放一段视频媒体，它继承了SurfaceView，位于”android.widget.VideoView”，是一个视频控件。既然是播放一段视频，那么不可避免的要涉及到一些开始、暂停、停止等操作，VideoView也为开发人员提供了对应的方法，这里简单介绍一些常用的：


```
int getCurrentPosition()：获取当前播放的位置。

    int getDuration()：获取当前播放视频的总长度。

    isPlaying()：当前VideoView是否在播放视频。

    void pause()：暂停

    void seekTo(int msec)：从第几毫秒开始播放。

    void resume()：重新播放。

    void setVideoPath(String path)：以文件路径的方式设置VideoView播放的视频源。

    void setVideoURI(Uri uri)：以Uri的方式设置VideoView播放的视频源，可以是网络Uri或本地Uri。

    void start()：开始播放。

    void stopPlayback()：停止播放。

    setMediaController(MediaController controller)：设置MediaController控制器。

    setOnCompletionListener(MediaPlayer.onCompletionListener l)：监听播放完成的事件。

    setOnErrorListener(MediaPlayer.OnErrorListener l)：监听播放发生错误时候的事件。

    setOnPreparedListener(MediaPlayer.OnPreparedListener l)：：监听视频装载完成的事件。
```




上面的一些方法通过方法名就可以了解用途。和MediaPlayer配合SurfaceView播放视频不同，VideoView播放之前无需编码装载视频，它会在start()开始播放的时候自动装载视频。并且VideoView在使用完之后，无需编码回收资源。

#### [](http://leeeyou.xyz/2017/04/15/blog-2017-04-15-Android%E7%B3%BB%E7%BB%9F%E9%9F%B3%E8%A7%86%E9%A2%91%E6%9E%B6%E6%9E%84/#%E5%9F%BA%E6%9C%AC%E4%BD%BF%E7%94%A8)基本使用


要使用 VideoView类播放视频，首先要在布局文件中添加VideoView组件，然后在Activity中获取该组件，并使用 VideoView.setVideoPath()或VideoView.setVideoURI() 方法加载需要播放的视频，最后调用 start()方法播放视频。VideoView类还提供了stop()和pause()方法，用于停止或暂停视频播放。

#### [](http://leeeyou.xyz/2017/04/15/blog-2017-04-15-Android%E7%B3%BB%E7%BB%9F%E9%9F%B3%E8%A7%86%E9%A2%91%E6%9E%B6%E6%9E%84/#%E4%BB%A3%E7%A0%81%E7%A4%BA%E4%BE%8B)代码示例

```
package com.example.leeyou.drr.di;



import java.io.File;



import android.app.Activity;

import android.media.MediaPlayer;

import android.media.MediaPlayer.OnCompletionListener;

import android.os.Bundle;

import android.widget.MediaController;

import android.widget.Toast;

import android.widget.VideoView;



public class MainActivity extends Activity {

    private VideoView video;



    /**

     * Called when the activity is firstcreated.

     */

    @Override

    publicvoidonCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);

        setContentView(R.layout.main);



        video = (VideoView) findViewById(R.id.video);

        File file = new File("/sdcard/1.mp4");

        MediaController mc = newMediaController(MainActivity.this);       // 创建一个MediaController对象

        if (file.exists()) {

            video.setVideoPath(file.getAbsolutePath());

            video.setMediaController(mc);       // 将VideoView与 MediaController关联起来

            video.requestFocus();       // 设置VideoView获取焦点

            try {

                video.start();      // 播放视频

            } catch (Exception e) {

                e.printStackTrace();

            }



            // 设置 VideoView的Completion事件监听器

            video.setOnCompletionListener( new OnCompletionListener() {

                @Override

                public voidonCompletion(MediaPlayer mp) {

                    Toast.makeText(MainActivity.this, "视频播放完毕！", Toast.LENGTH_SHORT).show();

                }

            });

        } else {

            Toast.makeText (this , "要播放的视频文件不存在 ", Toast.LENGTH_SHORT ).show();

        }

    }

}
```

### [](http://leeeyou.xyz/2017/04/15/blog-2017-04-15-Android%E7%B3%BB%E7%BB%9F%E9%9F%B3%E8%A7%86%E9%A2%91%E6%9E%B6%E6%9E%84/#%E9%80%9A%E8%BF%87MediaPlay%E9%85%8D%E5%90%88SurfaceView%E5%AE%9E%E7%8E%B0)通过MediaPlay配合SurfaceView实现

#### [](http://leeeyou.xyz/2017/04/15/blog-2017-04-15-Android%E7%B3%BB%E7%BB%9F%E9%9F%B3%E8%A7%86%E9%A2%91%E6%9E%B6%E6%9E%84/#%E5%9F%BA%E6%9C%AC%E4%BD%BF%E7%94%A8-1)基本使用


1、在布局文件中插入SurfaceView组件，其语法格式如下：


```
<SurfaceView

         android:id=”@+id/ID号”

         android:background=”背景”

         android:keepScreenOn=”true|false”

         android:layout_width=”宽度”

         android:layout_height=”高度” />
```




2、创建MediaPlayer对象，并加载要播放的视频。加载视频的方法和上一篇文章中介绍的加载音频的方法一样，这里不再详述。


3、将视频画面输出到SurfaceView，语法格式如下：

MediaPlayer.setDisplay(SurfaceHolder sh)

参数sh用于指定SurfaceHolder对象，可以通过SurfaceView.getHolder()方法获得。SurfaceHolder可以理解为SurfaceView装载需要显示的一帧帧图像的容器


4、调用MediaPlayer的play()、stop()、pause()等方法控制视频播放。

#### [](http://leeeyou.xyz/2017/04/15/blog-2017-04-15-Android%E7%B3%BB%E7%BB%9F%E9%9F%B3%E8%A7%86%E9%A2%91%E6%9E%B6%E6%9E%84/#%E4%BB%A3%E7%A0%81%E7%A4%BA%E4%BE%8B-1)代码示例

```
package com.example.leeyou.drr.di;



import java.io.IOException;



import android.app.Activity;

import android.media.MediaPlayer;

import android.media.MediaPlayer.OnCompletionListener;

import android.os.Bundle;

import android.view.SurfaceView;

import android.view.View;

import android.view.View.OnClickListener;

import android.widget.Button;

import android.widget.Toast;



public class MainActivity extends Activity {

    private MediaPlayer mp;

    private SurfaceView sv;



    @Override

    publicvoidonCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);

        setContentView(R.layout.main);

        mp = new MediaPlayer();

        sv = (SurfaceView) findViewById(R.id.surfaceView1);

        Button play = (Button) findViewById(R.id.play);

        final Button pause = (Button) findViewById(R.id.pause);

        Button stop = (Button) findViewById(R.id.stop);



        play.setOnClickListener(new OnClickListener() {



            @Override

            public void onClick(View v) {

                mp.reset();

                try {

                    mp.setDataSource( "/sdcard/1.mp4");

                    mp.setDisplay( sv.getHolder());

                    mp.prepare();

                    mp.start();

                    pause.setText( "暂停");

                    pause.setEnabled( true);

                } catch (IllegalArgumentException e) {

                    e.printStackTrace();

                } catch (SecurityException e) {

                    e.printStackTrace();

                } catch (IllegalStateException e) {

                    e.printStackTrace();

                } catch (IOException e) {

                    e.printStackTrace();

                }



            }

        });



        stop.setOnClickListener(new OnClickListener() {



            @Override

            public void onClick(View v) {

                if (mp.isPlaying()) {

                    mp.stop();

                    pause.setEnabled( false);

                }



            }

        });



        pause.setOnClickListener(new OnClickListener() {



            @Override

            public void onClick(View v) {

                if (mp.isPlaying()) {

                    mp.pause();

                    ((Button) v).setText( "继续");

                } else {

                    mp.start();

                    ((Button) v).setText( "暂停");

                }



            }

        });



        mp.setOnCompletionListener(new OnCompletionListener() {



            @Override

            public voidonCompletion(MediaPlayer mp) {

                Toast.makeText(MainActivity.this, "视频播放完毕！", Toast.LENGTH_SHORT ).show();

            }

        });



    }



    @Override

    protected void onDestroy() {

        if (mp.isPlaying()) {

            mp.stop();

        }

        mp.release();

        super.onDestroy();

    }



}
```

## [](http://leeeyou.xyz/2017/04/15/blog-2017-04-15-Android%E7%B3%BB%E7%BB%9F%E9%9F%B3%E8%A7%86%E9%A2%91%E6%9E%B6%E6%9E%84/#SurfaceView%E4%BB%8B%E7%BB%8D)SurfaceView介绍

### [](http://leeeyou.xyz/2017/04/15/blog-2017-04-15-Android%E7%B3%BB%E7%BB%9F%E9%9F%B3%E8%A7%86%E9%A2%91%E6%9E%B6%E6%9E%84/#%E5%9F%BA%E6%9C%AC%E4%BB%8B%E7%BB%8D)基本介绍


先来介绍一下大部分软件如何解析一段视频流。首先它需要先确定视频的格式，这个和解码相关，不同的格式视频编码不同，不是这里的重点。知道了视频的编码格式后，再通过编码格式进行解码，最后得到一帧一帧的图像，并把这些图像快速的显示在界面上，即为播放一段视频。SurfaceView在Android中就是完成这个功能的。


既然SurfaceView是配合MediaPlayer使用的，MediaPlayer也提供了相应的方法设置SurfaceView显示图片，只需要为MediaPlayer指定SurfaceView显示图像即可。它的完整签名如下：


```
void setDisplay(SurfaceHolder sh)
```




它需要传递一个SurfaceHolder对象，SurfaceHolder可以理解为SurfaceView装载需要显示的一帧帧图像的容器，它可以通过SurfaceHolder.getHolder()方法获得。

使用MediaPlayer配合SurfaceView播放视频的步骤与播放使用MediaPlayer播放MP3大体一致，只需要额外设置显示的SurfaceView即可。

### [](http://leeeyou.xyz/2017/04/15/blog-2017-04-15-Android%E7%B3%BB%E7%BB%9F%E9%9F%B3%E8%A7%86%E9%A2%91%E6%9E%B6%E6%9E%84/#%E7%BC%93%E5%86%B2%E5%8E%9F%E7%90%86%E4%BB%8B%E7%BB%8D)缓冲原理介绍


上面有提到，SurfaceView和大部分视频应用一样，把视频流解析成一帧帧的图像进行显示，但是如果把这个解析的过程放到一个线程中完成，可能在上一帧图像已经显示过后，下一帧图像还没有来得及解析，这样会导致画面的不流畅或者声音和视频不同步的问题。


所以SurfaceView和大部分视频应用一样，通过双缓冲的机制来显示帧图像。


那么什么是双缓冲呢？双缓冲可以理解为有两个线程轮番去解析视频流的帧图像，当一个线程解析完帧图像后，把图像渲染到界面中，同时另一线程开始解析下一帧图像，使得两个线程轮番配合去解析视频流，以达到流畅播放的效果。

![](https://github.com/LeeeYou/Img/blob/master/leeeyou/surfaceview.png?raw=true)

### [](http://leeeyou.xyz/2017/04/15/blog-2017-04-15-Android%E7%B3%BB%E7%BB%9F%E9%9F%B3%E8%A7%86%E9%A2%91%E6%9E%B6%E6%9E%84/#SurfaceHolder)SurfaceHolder


SurfaceView内部实现了双缓冲的机制，但是实现这个功能是非常消耗系统内存的。因为移动设备的局限性，Android在设计的时候规定，SurfaceView如果为用户可见的时候，创建SurfaceView的SurfaceHolder用于显示视频流解析的帧图片，如发现SurfaceView变为用户不可见的时候，则立即销毁SurfaceView的SurfaceHolder，以达到节约系统资源的目的。


如果开发人员不对SurfaceHolder进行维护，会出现最小化程序后，再打开应用的时候，视频的声音在继续播放，但是不显示画面了的情况，这就是因为当SurfaceView不被用户可见的时候，之前的SurfaceHolder已经被销毁了，再次进入的时候，界面上的SurfaceHolder已经是新的SurfaceHolder了。


所以SurfaceHolder需要我们开发人员去编码维护，维护SurfaceHolder需要用到它的一个回调，SurfaceHolder.Callback()，它需要实现三个如下三个方法：


```
void surfaceDestroyed(SurfaceHolder holder)：当SurfaceHolder被销毁的时候回调。

void surfaceCreated(SurfaceHolder holder)：当SurfaceHolder被创建的时候回调。

void surfaceChange(SurfaceHolder holder)：当SurfaceHolder的尺寸发生变化的时候被回调。
```




以下是这三个方法的调用过程，在应用中分别为SurfaceHolder实现了这三个方法，先进入应用，SurfaceHolder被创建，创建好之后会改变SurfaceHolder的大小，然后按Home键回退到桌面销毁SurfaceHolder，最后再进入应用，重新SurfaceHolder并改变其大小。

## [](http://leeeyou.xyz/2017/04/15/blog-2017-04-15-Android%E7%B3%BB%E7%BB%9F%E9%9F%B3%E8%A7%86%E9%A2%91%E6%9E%B6%E6%9E%84/#%E6%89%A9%E5%B1%95)扩展

### [](http://leeeyou.xyz/2017/04/15/blog-2017-04-15-Android%E7%B3%BB%E7%BB%9F%E9%9F%B3%E8%A7%86%E9%A2%91%E6%9E%B6%E6%9E%84/#MediaStore)MediaStore


MediaStore这个类是android系统提供的一个多媒体数据库，android中多媒体信息都可以从这里提取。这个MediaStore包括了多媒体数据库的所有信息，包括音频，视频和图像，android把所有的多媒体数据库接口进行了封装，所有的数据库不用自己进行创建，直接调用利用ContentResolver去调用那些封装好的接口就可以进行数据库的操作，多媒体数据库的使用方法和SQLITE3的方法是一样的。


MediaStore中的数据是在MediaScanner扫描后通过MediaProvider中的一个service进行更新的。

### [](http://leeeyou.xyz/2017/04/15/blog-2017-04-15-Android%E7%B3%BB%E7%BB%9F%E9%9F%B3%E8%A7%86%E9%A2%91%E6%9E%B6%E6%9E%84/#MediaScanner)MediaScanner


在Android系统中，多媒体库是通过MediaScanner去扫描磁盘文件，对元信息的处理，并通过MediaProvider保存到MediaStore中。

MediaScanner可以通过手动控制，在ANDROID系统中，已经定制了三种事件会触发MediaScanner去扫描磁盘文件：


ACTION_BOOT_COMPLETED【系统启动完后发出这个消息】

ACTION_MEDIA_MOUNTED 【插卡事件触发的消息】

ACTION_MEDIA_SCANNER_SCAN_FILE【一般是在一些文件操作后，开发人员手动发出的一个重新扫描多媒体文件的消息】


发送消息通过sendBroadcast函数完成，比如广播一个ACTION_MEDIA_MOUNTED消息：


```
sendBroadcast(new Intent(Intent.ACTION_MEDIA_MOUNTED, Uri.parse("file://"+ Environment.getExternalStorageDirectory())));
```



## [](http://leeeyou.xyz/2017/04/15/blog-2017-04-15-Android%E7%B3%BB%E7%BB%9F%E9%9F%B3%E8%A7%86%E9%A2%91%E6%9E%B6%E6%9E%84/#%E8%A7%86%E9%A2%91%E7%BC%96%E8%A7%A3%E7%A0%81)视频编解码


微校中采用的是FFmpeg开源框架进行视频的解码处理。FFmpeg在Linux平台下开发，但它同样也可以在其它操作系统环境中编译运行，包括Windows、Mac OS X等。


FFmpeg项目的名称来自MPEG视频编码标准，前面的”FF”代表”Fast Forward”。


其实电影文件有很多基本的组成部分。首先，文件本身被称为容器Container，容器的类型决定了文件信息存放的位置。接着，你有一组流，例如，你经常有的是一个音频流和一个视频流。在流中的数据元素被称为帧Frame。每个流是由不同的编码器来编码生成的。接着从流中被读出来的叫做包Packets。包是一段数据，它包含了一段可以被解码成方便我们最后在应用程序中操作的原始帧的数据。


我们可以利用GraphEdit工具对视频文件进行过滤，看到其中的解码思路。大致分为七个模块分别为：读文件模块，解复用模块，视频解码模块，音频解码音频，颜色空间转换模块，视频显示模块（显卡），音频播放模块（声卡）。


对应具体的代码实现，粗略的分为五类，分别是Source filter, Demux flter, Decoder filter, Color Space converter filter，Render filter


Source filter 源过滤器的作用是为下级 demux filter 以包的形式源源不断的提供数据流。Sourcefilter 另外一个作用就是屏蔽读本地文件和获取网络数据的差别，在下一级的 demux filter 看来，本地文件和网络数据是一样的。


Demux flter解复用过滤器的作用是识别文件类型，媒体类型，分离出各媒体原始数据流，打上时钟信息后送给下级 decoder filter。在本例中，AVI Splitter 是 Demux filter。


Decoder filter解码过滤器的作用就是解码数据包，并且把同步时钟信息传递下去。对视频媒体而言，通常是解码成 YUV 数据，然后利用显卡直接支持 YUV 格式数据 Overlay 快速显示的特性让显卡极速显示。对音频媒体而言，通常是解码成 PCM 数据，然后送给声卡直接输出。在本例中，AVI Decompress 和 ACM Warper 是 decoder filter。


Color Space converter filter颜色空间转换过滤器的作用是把视频解码器解码出来的数据转换成当前显示系统支持的颜色格式。通常视频解码器解码出来的是 YUV 数据，PC 系统是直接支持 YUV 格式的，也支持 RGB 格式，有些嵌入式系统只支持 RGB 格式的。在本例中，视频解码器解码出来的是 RGB8 格式的数据，Color space converter filter 把 RGB8 转换成 RGB32 显示。


Render filter渲染过滤器的作用就是在适当的时间渲染相应的媒体，对视频媒体就是直接显示图像，对音频就是播放声音。视频必须要使用同步时钟信息来决定什么时候显示,而音频是可以基于视频的时钟信息通过计算进行播放。在本例中 VideoRender 和 Default DirectSound Device 是 Render filter，同时也是 Sink filter。

### [](http://leeeyou.xyz/2017/04/15/blog-2017-04-15-Android%E7%B3%BB%E7%BB%9F%E9%9F%B3%E8%A7%86%E9%A2%91%E6%9E%B6%E6%9E%84/#HLS-HTTP-Live-Streaming-%E8%A7%86%E9%A2%91%E7%9B%B4%E6%92%AD%E6%8A%80%E6%9C%AF)HLS(HTTP Live Streaming)视频直播技术


常用的流媒体协议主要有 HTTP 渐进下载和基于 RTSP/RTP 的实时流媒体协议，这二种基本是完全不同的东西，目前比较方便又好用的是 HTTP 渐进下载的方法。


处理步骤：

视频采集 ->编码器 -> 流分割 -> 普通 web 服务(索引文件和视频文件) -> 客户端


RTSP（Real Time Streaming Protocol），实时流传输协议，是TCP/IP协议体系中的一个应用层协议，由哥伦比亚大学、网景和RealNetworks公司提交的IETF RFC标准。该协议定义了一对多应用程序如何有效地通过IP网络传送多媒体数据。RTSP在体系结构上位于RTP和RTCP之上，它使用TCP或RTP完成数据传输。

HTTP与RTSP相比，HTTP传送HTML，而RTSP传送的是多媒体数据。HTTP请求由客户机发出，服务器作出响应；使用RTSP时，客户机和服务器都可以发出请求，即RTSP可以是双向的。


RTP（Real-time Transport Protocol，实时传输协议）是一个网络传输协议，它是由IETF的多媒体传输工作小组1996年在RFC 1889中公布的，后在RFC3550中进行更新。


对于视频采集，编码器首先将摄像机实时采集的音视频数据压缩编码为符合特定标准的音视频基本流，也可以用编码完了的文件，有一点必须保证，就是一定要使用H.264视频和AAC音频，因为发明这个的是苹果公司，只支持这个。然后给这些封装成为符合MPEG-2(MPEG 2 TS、MPEG2 PS之所以使用这个，主要是因为声音和视频会交织在一起，也会有关键帧来让视频可以直接播放).


流分割部分比起 RTSP 之类和普通点播的最大不同，就是他会给 MPEG-2 分割成很多个 ts 的文件。分割过程大多是按时间来切，建议切 10s 一个的文件，如果码流高可以 5 s一次。在分割还有一点不同，就是这时流分割器会生成一个含有指向这些小ts文件指针的索引文件.


所以这个文件也必须在 web 服务器上，不能少。每多 10s 时，就会多一个 ts 文件，所以索引也会跟着修改成最新的几段视频。


最后，这些切分了的小的一系列的 ts 文件，放到普通的 web 服务器中就行了。因为请求这些文件会使用标准的 HTTP 协议。索引文件后缀是.m3u8 , 索引文件采用扩展的M3U播放列表格式, 其实就一文本。


内部的视频的地址会是如下
[http://media.example.com/s_96ksegment1.ts](http://media.example.com/s_96ksegment1.ts)
[http://media.example.com/s_96ksegment2.ts](http://media.example.com/s_96ksegment2.ts)
[http://media.example.com/s_96ksegment3.ts](http://media.example.com/s_96ksegment3.ts)

所以这时可以直接做 Cache 和直接放到 Web 服务器中,简单方便。如果 MIME 的信息输出不对的话，记的要修改这加入 ts 和 m3u8 的后缀支持.m3u8 application/x-mpegURL .ts video/MP2T


最后就是客户端，如果是 HTML 直接在 HTML5 中直接支持这种视频可以使用如下标签


```
<video tabindex="0" height="480" width="640"> <source src="/content1/content1.m3u8"> </video>
```




如果是应用客户端(Safari QuickTime之类)，就得装软件来支持，客户端会根据选择的流的索引来下载文件，当下载了最少二段后开始播放。直接 m3u8 的索引结束。


另外，HTTP可以设计成的自适应比特率流，在不同网络环境，选择下载不同码流的视频。


所以整个 HTTP Live Streaming 无论是直播还是点播，都能做到近似实时的方式来进行流播放。理论的最小时延是每个切片的长.




