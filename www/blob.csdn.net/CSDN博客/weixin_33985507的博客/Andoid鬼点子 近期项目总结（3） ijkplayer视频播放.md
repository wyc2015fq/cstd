# Andoid鬼点子 近期项目总结（3） ijkplayer视频播放 - weixin_33985507的博客 - CSDN博客
2017年12月15日 01:35:19[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：2
视频播放是一个很常见的功能，根据功能需求的不同，有不同的实现方式。
如果只是类似预览的功能，可以直接调取系统的视频播放功能：
```bash
Intent intent = new Intent();
 intent.setAction(Intent.ACTION_VIEW);
 intent.setDataAndType(Uri.fromFile(new File(path)), "video/mp4");
 activity.startActivity(intent);
复制代码
```
这样做的话，就会跳出App,好处就是用起来简单，坏处就是离开的应用，如果有其他需求的话则无法实现。
最近的项目中用到的视频播放，有一些特殊的功能，比如不允许用户快进，但是可以退回，用户看过的部分可以快进。要记录播放进度，再次进入时要恢复进度。可以设置断点，断点暂停后用户需要手动点击继续播放。综上，上面的做法就不能用了，只能自己写一个播放器了。
之前用过Vitamio,整体的使用感觉还是比较顺利，文档示例都比较全。也没有什么大bug。但是商用收费！如果你对Vitamio感兴趣可以[看这里](https://link.juejin.im?target=http%3A%2F%2Fgreendami.top%2F2017%2F01%2F03%2FAndroid%25E9%25AC%25BC%25E7%2582%25B9%25E5%25AD%2590-Vitamio%25E7%259A%2584%25E5%25B0%258Ftip%2F)。
这次就用了ijkplayer。ijkplayer的文档和示例都没有Vitamio那么多，我是在示例上修修改改的。它是可以支持的在线播放和本地播放的。
它们都是基于FFmpeg的，你也可以直接干FFmpeg。
按照[ijkplayer](https://link.juejin.im?target=https%3A%2F%2Fgithub.com%2FBilibili%2Fijkplayer)的github一步步集成进来，还是比较顺利的。就是这样：
[官方示例](https://link.juejin.im?target=https%3A%2F%2Fgithub.com%2FBilibili%2Fijkplayer%2Ftree%2Fmaster%2Fandroid%2Fijkplayer%2Fijkplayer-example)
上面例子最好down下来，跑一下。
我用到了一个VideoView来播放视频，它是一个FrameLayout。
[我是在这里扒的](https://link.juejin.im?target=https%3A%2F%2Fgithub.com%2FBilibili%2Fijkplayer%2Fblob%2Fmaster%2Fandroid%2Fijkplayer%2Fijkplayer-example%2Fsrc%2Fmain%2Fjava%2Ftv%2Fdanmaku%2Fijk%2Fmedia%2Fexample%2Fwidget%2Fmedia%2FIjkVideoView.java)，这里代码还用到了其他的调用，一并copy过来，最后是这样的。
其他的代码你也可以在[示例](https://link.juejin.im?target=https%3A%2F%2Fgithub.com%2FBilibili%2Fijkplayer%2Ftree%2Fmaster%2Fandroid%2Fijkplayer%2Fijkplayer-example)中找到。我把上面的代码放到了自己的项目中。
然后在布局中放入你写（拷）的IjkVideoView。
```xml
<?xml version="1.0" encoding="utf-8"?>
<LinearLayout xmlns:android="http://schemas.android.com/apk/res/android"
    android:layout_width="match_parent"
    android:layout_height="match_parent"
    android:orientation="vertical"
    android:background="@color/black">
    
    <com.greendami.video.widget.media.IjkVideoView
        android:id="@+id/video"
        android:layout_width="match_parent"
        android:layout_height="0dp"
        android:layout_weight="1"></com.greendami.video.widget.media.IjkVideoView>
</LinearLayout>
复制代码
```
如果你只是想简单的播放视频，对界面没有什么要求的话，可以使用ijkplayer提供的MediaController，直接就有进度条，暂停，播放等功能。
video.setMediaController(AndroidMediaController(this))，不需要的话就传个null进去就行。
只要这样就行：
```bash
package com.greendami.actvity.worknotes
import android.content.pm.ActivityInfo
import android.content.res.Configuration
import android.net.Uri
import android.os.Environment
import android.view.WindowManager
import android.widget.FrameLayout
import android.widget.LinearLayout
import com.allrun.dangjianshisanshi.R
import com.allrun.dangjianshisanshi.actvity.BaseActivity
import com.allrun.dangjianshisanshi.video.widget.media.AndroidMediaController
import com.allrun.dangjianshisanshi.widget.LoadingDialog
import kotlinx.android.synthetic.main.activity_worknote_videoplayer.*
import org.jetbrains.anko.toast
import tv.danmaku.ijk.media.player.IjkMediaPlayer
/**
 * Created by greendami on 2017/8/30.
 */
class WorkNoteVideoPlayerActivity : BaseActivity() {
    private val SIZE_DEFAULT = 0
    private val SIZE_4_3 = 1
    private val SIZE_16_9 = 2
    private val currentSize = SIZE_DEFAULT
    private var screenWidth = 0
    private var screenHeight = 0
    ////http://www.modrails.com/videos/passenger_nginx.mov
    var uri = Uri.parse(Environment.getExternalStorageDirectory().path + "/test.mp4")
    var path = ""
    override fun setContentView() {
        setContentView(R.layout.activity_worknote_videoplayer)
    }
    override fun initView() {
        IjkMediaPlayer.loadLibrariesOnce(null)
        IjkMediaPlayer.native_profileBegin("libijkplayer.so")
        LoadingDialog.showDialog(this)
        initEvent()
    }
    private fun initEvent() {
        back.setOnClickListener { finish() }
        video.setOnCompletionListener {
            toast("播放完毕")
            finish()
        }
        video.setOnPreparedListener {
            LoadingDialog.dismissDialog()
            video.start()
            setVideoLayoutSize()
        }
    }
    private fun setVideoLayoutSize() {
        initScreenInfo()
        var width = video.width
        var height = video.height
        if (video.getmVideoWidth() / video.getmVideoHeight() > width / height) {
            height = width * video.getmVideoHeight() / video.getmVideoWidth()
        } else {
            width = height * video.getmVideoWidth() / video.getmVideoHeight()
        }
        if (width > 0 && height > 0) {
            val lp = video.getmRenderView().view.layoutParams as FrameLayout.LayoutParams
            lp.width = width
            lp.height = height
            video.getmRenderView().view.layoutParams = lp
        }
    }
    override fun bindData() {
        video.setVideoPath(path)
        video.setMediaController(AndroidMediaController(this))
    }
    override fun loadData() {
        path = intent.extras["path"].toString()
    }
    private fun setScreenRate(newConfig: Configuration) {
        var width = 0
        var height = 0
        if (newConfig.orientation === Configuration.ORIENTATION_LANDSCAPE) {//切换为横屏
            when (currentSize) {
                SIZE_DEFAULT -> {
                    width = video.getmVideoWidth()
                    height = video.getmVideoHeight()
                }
                SIZE_4_3 -> {
                    width = screenHeight / 3 * 4
                    height = screenHeight
                }
                SIZE_16_9 -> {
                    width = screenHeight / 9 * 16
                    height = screenHeight
                }
            }
        } else { //竖屏
            when (currentSize) {
                SIZE_DEFAULT -> {
                    width = video.getmVideoWidth()
                    height = video.getmVideoHeight()
                }
                SIZE_4_3 -> {
                    width = screenWidth
                    height = screenWidth * 3 / 4
                }
                SIZE_16_9 -> {
                    width = screenWidth
                    height = screenWidth * 9 / 16
                }
            }
        }
        if (width > 0 && height > 0) {
            val lp = video.getmRenderView().view.layoutParams as FrameLayout.LayoutParams
            lp.width = width
            lp.height = height
            video.getmRenderView().view.layoutParams = lp
        }
    }
    private fun fullChangeScreen() {
        requestedOrientation = if (requestedOrientation == ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE) {// 切换为竖屏
            ActivityInfo.SCREEN_ORIENTATION_PORTRAIT
        } else {
            ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE
        }
    }
    override fun onConfigurationChanged(newConfig: Configuration) {
        super.onConfigurationChanged(newConfig)
        //重新获取屏幕宽高
        initScreenInfo()
        if (newConfig.orientation === Configuration.ORIENTATION_LANDSCAPE) {//切换为横屏
            //去掉通知栏
            //获得 WindowManager.LayoutParams 属性对象
            val lp2 = window.attributes
            //直接对它flags变量操作   LayoutParams.FLAG_FULLSCREEN 表示设置全屏
            lp2.flags = lp2.flags or WindowManager.LayoutParams.FLAG_FULLSCREEN
            //设置属性
            window.attributes = lp2
            //意思大致就是  允许窗口扩展到屏幕之外
            window.addFlags(WindowManager.LayoutParams.FLAG_LAYOUT_NO_LIMITS)
            val lp = video.layoutParams as LinearLayout.LayoutParams
            lp.height = screenHeight
            lp.width = screenWidth
            video.layoutParams = lp
        } else {
            //恢复通知栏
            //获得 WindowManager.LayoutParams 属性对象
            val lp2 = window.attributes
            //LayoutParams.FLAG_FULLSCREEN 强制屏幕状态条栏弹出
            lp2.flags = lp2.flags and WindowManager.LayoutParams.FLAG_FULLSCREEN.inv()
            //设置属性
            window.attributes = lp2
            //不允许窗口扩展到屏幕之外  clear掉了
            window.clearFlags(WindowManager.LayoutParams.FLAG_LAYOUT_NO_LIMITS)
            val lp = video.layoutParams as LinearLayout.LayoutParams
            when (currentSize) {
                SIZE_DEFAULT -> {
                    lp.height = video.getmVideoHeight() * screenWidth / video.getmVideoWidth()
                }
                SIZE_4_3 -> {
                    lp.height = screenWidth * 3 / 4
                }
                SIZE_16_9 -> {
                    lp.height = screenWidth * 9 / 16
                }
            }
            lp.width = screenWidth
            video.layoutParams = lp
        }
        setScreenRate(newConfig)
    }
    private fun initScreenInfo() {
        val wm = this.windowManager
        screenWidth = wm.defaultDisplay.width
        screenHeight = wm.defaultDisplay.height
    }
    override fun onDestroy() {
        video.release(true)
        LoadingDialog.dismissDialog()
        super.onDestroy()
    }
}
复制代码
```
如果不把BaseActivity放上来，可能看起来费劲,这个是我随便写的，请不要在意。
```bash
abstract class BaseActivity : LifecycleActivity() {
    lateinit var modelHolder: ModelHolder
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        modelHolder = ViewModelProviders.of(this).get(ModelHolder::class.java)
        setContentView()
        loadData()
        initView()
        bindData()
    }
    abstract fun setContentView()
    /**
     * 请求数据
     */
    abstract fun loadData()
    abstract fun initView()
    /**
    * 把数据和控件绑定
    */
    abstract fun bindData()
}
复制代码
```
**大体**的步骤是：
```bash
//加载库文件
IjkMediaPlayer.loadLibrariesOnce(null)
IjkMediaPlayer.native_profileBegin("libijkplayer.so")
//设置文件路径可以是网络地址或者本地文件路径
video.setVideoPath(path)
//使用默认的控制界面，进度条快进等等
video.setMediaController(AndroidMediaController(this))
//绑定加载完成监听器，加载完了就播放
video.setOnPreparedListener {
        LoadingDialog.dismissDialog()
        video.start()
        //这里是设置视频的尺寸，不是必须
        setVideoLayoutSize()
    }
//到此就完成了，如果你需要重力感应，全屏切换，请往下看
//如果按钮切换横竖屏，调用这个方法
fullChangeScreen()
//这里是横竖屏切换事件的回调
override fun onConfigurationChanged(newConfig: Configuration)
//这里是屏幕方向改变后重新计算视频尺寸,我是默认不改变视频长宽比的前提下铺满屏幕
//video.getmVideoWidth()是视频的尺寸，是我自己加的方法，只是返回了tmVideoWidth
//video.getmRenderView().view.layoutParams = lp这句是真正设置视频尺寸
private fun setScreenRate(newConfig: Configuration) {
        initScreenInfo()
        var width = screenWidth
        var height = screenHeight
        if (video.getmVideoWidth() / video.getmVideoHeight() > width / height) {
            height = width * video.getmVideoHeight() / video.getmVideoWidth()
        } else {
            width = height * video.getmVideoWidth() / video.getmVideoHeight()
        }
        if (width > 0 && height > 0) {
            val lp = video.getmRenderView().view.layoutParams as FrameLayout.LayoutParams
            lp.width = width
            lp.height = height
            video.getmRenderView().view.layoutParams = lp
        }
    }
复制代码
```
如果你需要控制视频，下面的API你可能会用到：
```bash
//进度控制
video.seekTo(progress)
//视频播放完毕回调
video.setOnCompletionListener {toast("播放完毕")}
//视频长度
video.duration
//暂停
video.pause()
//继续播放
if (!video.isPlaying) video.start()
复制代码
```
