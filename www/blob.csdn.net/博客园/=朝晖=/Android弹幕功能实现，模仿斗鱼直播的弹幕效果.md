# Android弹幕功能实现，模仿斗鱼直播的弹幕效果 - =朝晖= - 博客园
# [Android弹幕功能实现，模仿斗鱼直播的弹幕效果](https://www.cnblogs.com/dhcn/p/7125127.html)
转载出处：[http://blog.csdn.net/sinyu890807/article/details/51933728](http://blog.csdn.net/sinyu890807/article/details/51933728)
> 
本文同步发表于我的[微信](http://lib.csdn.net/base/wechat)公众号，扫一扫文章底部的二维码或在[微信](http://lib.csdn.net/base/wechat)搜索 郭霖 即可关注，每天都有文章更新。
大家好，感觉好像已经很久没更新博客了。前段时间主要是忙于新书的事情，时间比较紧张。而现在新书已经完稿，剩下的事情就都是出版社的工作了，那么我又可以抽出时间来写写博客了。
记得之前有位朋友在我的公众号里问过我，像直播的那种弹幕功能该如何实现？如今直播行业确实是非常火爆啊，大大小小的公司都要涉足一下直播的领域，用斗鱼的话来讲，现在就是千播之战。而弹幕则无疑是直播功能当中最为重要的一个功能之一，那么今天，我就带着大家一起来实现一个简单的[Android](http://lib.csdn.net/base/android)端弹幕效果。
# 分析
首先我们来看一下斗鱼上的弹幕效果，如下图所示：
![](http://img.blog.csdn.net/20161015102910407)
这是一个Dota2游戏直播的界面，我们可以看到，在游戏界面的上方有很多的弹幕，看直播的观众们就是在这里进行讨论的。
那么这样的一个界面该如何实现呢？其实并不复杂，我们只需要首先在布局中放置一个显示游戏界面的View，然后在游戏界面的上方再覆盖一个显示弹幕的View就可以了。弹幕的View必须要做成完全透明的，这样即使覆盖在游戏界面的上方也不会影响到游戏的正常观看，只有当有人发弹幕消息时，再将消息绘制到弹幕的View上面就可以了。原理示意图如下所示：
![](http://img.blog.csdn.net/20161015095851544)
但是我们除了要能看到弹幕之外也要能发弹幕才行，因此还要再在弹幕的View上面再覆盖一个操作界面的View，然后我们就可以在操作界面上发弹幕、送礼物等。原理示意图如下所示：
![](http://img.blog.csdn.net/20161015101707211)
这样我们就把基本的实现原理分析完了，下面就让我们开始一步步实现吧。
# 实现视频播放
由于本篇文章的主题是实现弹幕效果，并不涉及直播的任何其他功能，因此这里我们就简单地使用VideoView播放一个本地视频来模拟最底层的游戏界面。
首先使用[Android](http://lib.csdn.net/base/android) Studio新建一个DanmuTest项目，然后修改activity_main.xml中的代码，如下所示：
```xml
<RelativeLayout
    xmlns:android="http://schemas.android.com/apk/res/android"
    android:id="@+id/activity_main"
    android:layout_width="match_parent"
    android:layout_height="match_parent"
    android:background="#000">
    <VideoView
        android:id="@+id/video_view"
        android:layout_width="match_parent"
        android:layout_height="wrap_content"
        android:layout_centerInParent="true"/>
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
- 14
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
- 14
布局文件的代码非常简单，只有一个VideoView，我们将它设置为居中显示。 
然后修改MainActivity中的代码，如下所示：
```java
public class MainActivity extends AppCompatActivity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        VideoView videoView = (VideoView) findViewById(R.id.video_view);
        videoView.setVideoPath(Environment.getExternalStorageDirectory() + "/Pixels.mp4");
        videoView.start();
    }
    @Override
    public void onWindowFocusChanged(boolean hasFocus) {
        super.onWindowFocusChanged(hasFocus);
        if (hasFocus && Build.VERSION.SDK_INT >= 19) {
            View decorView = getWindow().getDecorView();
            decorView.setSystemUiVisibility(
                    View.SYSTEM_UI_FLAG_LAYOUT_STABLE
                            | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                            | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                            | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
                            | View.SYSTEM_UI_FLAG_FULLSCREEN
                            | View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY);
        }
    }
}
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
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
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
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
上面的代码中使用了VideoView的最基本用法。在onCreate()方法中获取到了VideoView的实例，给它设置了一个视频文件的地址，然后调用start()方法开始播放。当然，我事先已经在SD的根目录中准备了一个叫Pixels.mp4的视频文件。
这里使用到了SD卡的功能，但是为了代码简单起见，我并没有加入运行时权限的处理，因此一定要记得将你的项目的targetSdkVersion指定成23以下。
另外，为了让视频播放可以有最好的体验效果，这里使用了沉浸式模式的写法。对沉浸式模式还不理解的朋友可以参考我的上一篇文章 [Android状态栏微技巧，带你真正理解沉浸式模式](http://blog.csdn.net/sinyu890807/article/details/51763825) 。
最后，我们在AndroidManifest.xml中将Activity设置为横屏显示并加入权限声明，如下所示：
```xml
<manifest xmlns:android="http://schemas.android.com/apk/res/android"
          package="com.example.guolin.danmutest">
    <uses-permission android:name="android.permission.WRITE_EXTERNAL_STORAGE" />
    <application
        android:allowBackup="true"
        android:icon="@mipmap/ic_launcher"
        android:label="@string/app_name"
        android:supportsRtl="true"
        android:theme="@style/AppTheme">
        <activity android:name=".MainActivity" android:screenOrientation="landscape"
                  android:configChanges="orientation|keyboardHidden|screenLayout|screenSize">
            <intent-filter>
                <action android:name="android.intent.action.MAIN"/>
                <category android:name="android.intent.category.LAUNCHER"/>
            </intent-filter>
        </activity>
    </application>
</manifest>
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
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
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
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
OK，现在可以运行一下项目了，程序启动之后就会自动开始播放视频，效果如下图所示：
![](http://img.blog.csdn.net/20161015121231374)
这样我们就把第一步的功能实现了。
# 实现弹幕效果
接下来我们开始实现弹幕效果。弹幕其实也就是一个自定义的View，它的上面可以显示类似于跑马灯的文字效果。观众们发表的评论都会在弹幕上显示出来，但又会很快地移出屏幕，既可以起到互动的作用，同时又不会影响视频的正常观看。
我们可以自己来编写这样的一个自定义View，当然也可以直接使用网上现成的开源项目。那么为了能够简单快速地实现弹幕效果，这里我就准备直接使用由哔哩哔哩开源的弹幕效果库DanmakuFlameMaster了。
DanmakuFlameMaster库的项目主页地址是：[https://github.com/Bilibili/DanmakuFlameMaster](https://github.com/Bilibili/DanmakuFlameMaster)
话说现在使用[android](http://lib.csdn.net/base/android) Studio来引入一些开源库真的非常方便，只需要在build.gradle文件里面添加开源库的依赖就可以了。那么我们修改app/build.gradle文件，并在dependencies闭包中添加如下依赖：
```
dependencies {
    compile fileTree(dir: 'libs', include: ['*.jar'])
    compile 'com.android.support:appcompat-v7:24.2.1'
    testCompile 'junit:junit:4.12'
    compile 'com.github.ctiao:DanmakuFlameMaster:0.5.3'
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 1
- 2
- 3
- 4
- 5
- 6
这样我们就将DanmakuFlameMaster库引入到当前项目中了。然后修改activity_main.xml中的代码，如下所示：
```xml
<RelativeLayout
    xmlns:android="http://schemas.android.com/apk/res/android"
    android:id="@+id/activity_main"
    android:layout_width="match_parent"
    android:layout_height="match_parent"
    android:background="#000">
    <VideoView
        android:id="@+id/video_view"
        android:layout_width="match_parent"
        android:layout_height="wrap_content"
        android:layout_centerInParent="true"/>
    <master.flame.danmaku.ui.widget.DanmakuView
        android:id="@+id/danmaku_view"
        android:layout_width="match_parent"
        android:layout_height="match_parent" />
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
- 14
- 15
- 16
- 17
- 18
- 19
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
- 14
- 15
- 16
- 17
- 18
- 19
可以看到，这里在RelativeLayout中加入了一个DanmakuView控件，这个控件就是用于显示弹幕信息的了。注意一定要将DanmakuView写在VideoView的下面，因为RelativeLayout中后添加的控件会被覆盖在上面。
接下来修改MainActivity中的代码，我们在这里加入弹幕显示的逻辑，如下所示：
```java
public class MainActivity extends AppCompatActivity {
    private boolean showDanmaku;
    private DanmakuView danmakuView;
    private DanmakuContext danmakuContext;
    private BaseDanmakuParser parser = new BaseDanmakuParser() {
        @Override
        protected IDanmakus parse() {
            return new Danmakus();
        }
    };
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        VideoView videoView = (VideoView) findViewById(R.id.video_view);
        videoView.setVideoPath(Environment.getExternalStorageDirectory() + "/Pixels.mp4");
        videoView.start();
        danmakuView = (DanmakuView) findViewById(R.id.danmaku_view);
        danmakuView.enableDanmakuDrawingCache(true);
        danmakuView.setCallback(new DrawHandler.Callback() {
            @Override
            public void prepared() {
                showDanmaku = true;
                danmakuView.start();
                generateSomeDanmaku();
            }
            @Override
            public void updateTimer(DanmakuTimer timer) {
            }
            @Override
            public void danmakuShown(BaseDanmaku danmaku) {
            }
            @Override
            public void drawingFinished() {
            }
        });
        danmakuContext = DanmakuContext.create();
        danmakuView.prepare(parser, danmakuContext);
    }
    /**
     * 向弹幕View中添加一条弹幕
     * @param content
     *          弹幕的具体内容
     * @param  withBorder
     *          弹幕是否有边框
     */
    private void addDanmaku(String content, boolean withBorder) {
        BaseDanmaku danmaku = danmakuContext.mDanmakuFactory.createDanmaku(BaseDanmaku.TYPE_SCROLL_RL);
        danmaku.text = content;
        danmaku.padding = 5;
        danmaku.textSize = sp2px(20);
        danmaku.textColor = Color.WHITE;
        danmaku.setTime(danmakuView.getCurrentTime());
        if (withBorder) {
            danmaku.borderColor = Color.GREEN;
        }
        danmakuView.addDanmaku(danmaku);
    }
    /**
     * 随机生成一些弹幕内容以供测试
     */
    private void generateSomeDanmaku() {
        new Thread(new Runnable() {
            @Override
            public void run() {
                while(showDanmaku) {
                    int time = new Random().nextInt(300);
                    String content = "" + time + time;
                    addDanmaku(content, false);
                    try {
                        Thread.sleep(time);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
            }
        }).start();
    }
    /**
     * sp转px的方法。
     */
    public int sp2px(float spValue) {
        final float fontScale = getResources().getDisplayMetrics().scaledDensity;
        return (int) (spValue * fontScale + 0.5f);
    }
    @Override
    protected void onPause() {
        super.onPause();
        if (danmakuView != null && danmakuView.isPrepared()) {
            danmakuView.pause();
        }
    }
    @Override
    protected void onResume() {
        super.onResume();
        if (danmakuView != null && danmakuView.isPrepared() && danmakuView.isPaused()) {
            danmakuView.resume();
        }
    }
    @Override
    protected void onDestroy() {
        super.onDestroy();
        showDanmaku = false;
        if (danmakuView != null) {
            danmakuView.release();
            danmakuView = null;
        }
    }
    ......
}
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
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
- 44
- 45
- 46
- 47
- 48
- 49
- 50
- 51
- 52
- 53
- 54
- 55
- 56
- 57
- 58
- 59
- 60
- 61
- 62
- 63
- 64
- 65
- 66
- 67
- 68
- 69
- 70
- 71
- 72
- 73
- 74
- 75
- 76
- 77
- 78
- 79
- 80
- 81
- 82
- 83
- 84
- 85
- 86
- 87
- 88
- 89
- 90
- 91
- 92
- 93
- 94
- 95
- 96
- 97
- 98
- 99
- 100
- 101
- 102
- 103
- 104
- 105
- 106
- 107
- 108
- 109
- 110
- 111
- 112
- 113
- 114
- 115
- 116
- 117
- 118
- 119
- 120
- 121
- 122
- 123
- 124
- 125
- 126
- 127
- 128
- 129
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
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
- 44
- 45
- 46
- 47
- 48
- 49
- 50
- 51
- 52
- 53
- 54
- 55
- 56
- 57
- 58
- 59
- 60
- 61
- 62
- 63
- 64
- 65
- 66
- 67
- 68
- 69
- 70
- 71
- 72
- 73
- 74
- 75
- 76
- 77
- 78
- 79
- 80
- 81
- 82
- 83
- 84
- 85
- 86
- 87
- 88
- 89
- 90
- 91
- 92
- 93
- 94
- 95
- 96
- 97
- 98
- 99
- 100
- 101
- 102
- 103
- 104
- 105
- 106
- 107
- 108
- 109
- 110
- 111
- 112
- 113
- 114
- 115
- 116
- 117
- 118
- 119
- 120
- 121
- 122
- 123
- 124
- 125
- 126
- 127
- 128
- 129
可以看到，在onCreate()方法中我们先是获取到了DanmakuView控件的实例，然后调用了enableDanmakuDrawingCache()方法来提升绘制效率，又调用了setCallback()方法来设置回调函数。
接着调用DanmakuContext.create()方法创建了一个DanmakuContext的实例，DanmakuContext可以用于对弹幕的各种全局配置进行设定，如设置字体、设置最大显示行数等。这里我们并没有什么特殊的要求，因此一切都保持默认。
另外我们还需要创建一个弹幕的解析器才行，这里直接创建了一个全局的BaseDanmakuParser。
有了DanmakuContext和BaseDanmakuParser，接下来我们就可以调用DanmakuView的prepare()方法来进行准备，准备完成后会自动调用刚才设置的回调函数中的prepared()方法，然后我们在这里再调用DanmakuView的start()方法，这样DanmakuView就可以开始正常工作了。
虽说DanmakuView已经在正常工作了，但是屏幕上没有任何弹幕信息的话我们也看不出效果，因此我们还要增加一个添加弹幕消息的功能。
观察addDanmaku()方法，这个方法就是用于向DanmakuView中添加一条弹幕消息的。其中首先调用了createDanmaku()方法来创建一个BaseDanmaku实例，TYPE_SCROLL_RL表示这是一条从右向左滚动的弹幕，然后我们就可以对弹幕的内容、字体大小、颜色、显示时间等各种细节进行配置了。注意addDanmaku()方法中有一个withBorder参数，这个参数用于指定弹幕消息是否带有边框，这样才好将自己发送的弹幕和别人发送的弹幕进行区分。
这样我们就把最基本的弹幕功能就完成了，现在只需要当在接收到别人发送的弹幕消息时，调用addDanmaku()方法将这条弹幕添加到DanmakuView上就可以了。但接收别人发送来的消息又涉及到了即时通讯技术，显然这一篇文章中不可能将复杂的即时通讯技术也进行讲解，因此这里我专门写了一个generateSomeDanmaku()方法来随机生成一些弹幕消息，这样就可以模拟出和斗鱼类似的弹幕效果了。
除此之外，我们还需要在onPause()、onResume()、onDestroy()方法中进行一些逻辑处理，以保证DanmakuView的资源可以得到释放。
现在重新运行一下程序，效果如下图所示：
![](http://img.blog.csdn.net/20161016143201306)
这样我们就把第二步的功能也实现了。
# 加入操作界面
那么下面我们开始进行第三步功能实现，加入发送弹幕消息的操作界面。
首先修改activity_main.xml中的代码，如下所示：
```xml
<RelativeLayout
    xmlns:android="http://schemas.android.com/apk/res/android"
    android:id="@+id/activity_main"
    android:layout_width="match_parent"
    android:layout_height="match_parent"
    android:background="#000">
    ......
    <LinearLayout
        android:id="@+id/operation_layout"
        android:layout_width="match_parent"
        android:layout_height="50dp"
        android:layout_alignParentBottom="true"
        android:background="#fff"
        android:visibility="gone">
        <EditText
            android:id="@+id/edit_text"
            android:layout_width="0dp"
            android:layout_height="match_parent"
            android:layout_weight="1"
            />
        <Button
            android:id="@+id/send"
            android:layout_width="wrap_content"
            android:layout_height="match_parent"
            android:text="Send" />
    </LinearLayout>
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
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
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
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
可以看到，这里我们加入了一个LinearLayout来作为操作界面。LinearLayout中并没有什么复杂的控件，只有一个EditText用于输入内容，一个Button用于发送弹幕。注意我们一开始是将LinearLayout隐藏的，因为不能让这个操作界面一直遮挡着VideoView，只有用户想要发弹幕的时候才应该将它显示出来。
接下来修改MainActivity中的代码，在这里面加入发送弹幕的逻辑，如下所示：
```java
public class MainActivity extends AppCompatActivity {
    ......
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        ......
        final  LinearLayout operationLayout = (LinearLayout) findViewById(R.id.operation_layout);
        final Button send = (Button) findViewById(R.id.send);
        final EditText editText = (EditText) findViewById(R.id.edit_text);
        danmakuView.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if (operationLayout.getVisibility() == View.GONE) {
                    operationLayout.setVisibility(View.VISIBLE);
                } else {
                    operationLayout.setVisibility(View.GONE);
                }
            }
        });
        send.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                String content = editText.getText().toString();
                if (!TextUtils.isEmpty(content)) {
                    addDanmaku(content, true);
                    editText.setText("");
                }
            }
        });
        getWindow().getDecorView().setOnSystemUiVisibilityChangeListener (new View.OnSystemUiVisibilityChangeListener() {
            @Override
            public void onSystemUiVisibilityChange(int visibility) {
                if (visibility == View.SYSTEM_UI_FLAG_VISIBLE) {
                    onWindowFocusChanged(true);
                }
            }
        });
    }
    ......
}
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
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
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
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
这里的逻辑还是比较简单的，我们先是给DanmakuView设置了一个点击事件，当点击屏幕时就会触发这个点击事件。然后进行判断，如果操作界面是隐藏的就将它显示出来，如果操作界面是显示的就将它隐藏掉，这样就可以简单地通过点击屏幕来实现操作界面的隐藏和显示了。
接下来我们又给发送按钮注册了一个点击事件，当点击发送时，获取EditText中的输入内容，然后调用addDanmaku()方法将这条消息添加到DanmakuView上。另外，这条弹幕是由我们自己发送的，因此addDanmaku()方法的第二个参数要传入true。
最后，由于系统输入法弹出的时候会导致焦点丢失，从而退出沉浸式模式，因此这里还对系统全局的UI变化进行了监听，保证程序一直可以处于沉浸式模式。
这样我们就将所有的代码都完成了，现在可以运行一下看看最终效果了。由于电影播放的同时进行GIF截图生成的文件太大了，无法上传，因此这里我是在电影暂停的情况进行操作的。效果如下图所示：
![](http://img.blog.csdn.net/20161016174403841)
可以看到，我们自己发送的弹幕是有一个绿色边框包围的，很容易和其他弹幕区分开。
这样我们就把第三步的功能也实现了。
虽说现在我们已经成功实现了非常不错的弹幕效果，但其实这只是DanmakuFlameMaster库提供的最基本的功能而已。哔哩哔哩提供的这个弹幕开源库中拥有极其丰富的功能，包含各种不同的弹幕样式、特效等等。不过本篇文章的主要目标是带大家了解弹幕效果实现的思路，并不是要对DanmakuFlameMaster这个库进行全面的解析。如果你对这个库非常感兴趣，可以到它的github主页上面去学习更多的用法。
那么今天的文章到此结束。

