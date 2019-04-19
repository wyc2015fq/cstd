# Android学习一、MediaPlayer播放音频 - 建建的博客 - CSDN博客
2017年06月01日 11:45:25[纪建](https://me.csdn.net/u013898698)阅读数：236
个人分类：[Android](https://blog.csdn.net/u013898698/article/category/6941721)
想要使用MediaPlayer播放音频和视频，首先必须了解MediaPlayer的状态问题。下面是Google提供的MediaPlayer的状态图，基本上只要明白了，在MediaPlayer的状态上。就不会出问题了。
![](https://img-my.csdn.net/uploads/201204/21/1334976571_7675.png)
还有什么不明白的，可以详细的看看Google提供的API。
要想用MediaPlayer所写的的音频播放程序健壮的话，不仅需要了解MediaPlayer的各种状态，还必须对MediaPlayer的各个监听事件和回调方法做出必要的设定。
![](https://img-my.csdn.net/uploads/201204/21/1334980688_6752.png)
可以根据自己的需要，设置不同的监听方法。
下面是Copy别人的代码，写的不错，搬来壮壮场面。
1、这是布局的文件main.xml
<?xml version="1.0" encoding="utf-8"?>
<LinearLayout xmlns:[Android](http://lib.csdn.net/base/android)="http://schemas.android.com/apk/res/android"
    android:layout_width="fill_parent"
    android:layout_height="fill_parent"
    android:orientation="vertical" >
    <LinearLayout
        android:layout_width="fill_parent"
        android:layout_height="wrap_content"
        android:layout_gravity="top"
        android:orientation="horizontal" >
        <Button
            android:id="@+id/btnPlayUrl"
            android:layout_width="wrap_content"
            android:layout_height="wrap_content"
            android:text="播放网络音频" />
        <Button
            android:id="@+id/btnPause"
            android:layout_width="80dip"
            android:layout_height="wrap_content"
            android:text="暂停" />
        <Button
            android:id="@+id/btnStop"
            android:layout_width="80dip"
            android:layout_height="wrap_content"
            android:text="停止" />
    </LinearLayout>
    <LinearLayout
        android:layout_width="fill_parent"
        android:layout_height="wrap_content"
        android:layout_gravity="top"
        android:orientation="horizontal" >
        <SeekBar
            android:id="@+id/skbProgress"
            android:layout_width="wrap_content"
            android:layout_height="wrap_content"
            android:layout_gravity="center_vertical"
            android:layout_weight="1.0"
            android:max="100"
            android:paddingLeft="10dip"
            android:paddingRight="10dip" />
    </LinearLayout>
</LinearLayout>
2、Player.[Java](http://lib.csdn.net/base/javase)   封装了MediaPlayer的播放
package com.tao.test;
import java.io.IOException;
import java.util.Timer;
import java.util.TimerTask;
import android.media.AudioManager;
import android.media.MediaPlayer;
import android.media.MediaPlayer.OnBufferingUpdateListener;
import android.media.MediaPlayer.OnCompletionListener;
import android.media.MediaPlayer.OnErrorListener;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.widget.SeekBar;
public class Player implements OnBufferingUpdateListener, OnCompletionListener,
        MediaPlayer.OnPreparedListener ,OnErrorListener{
    public MediaPlayer mediaPlayer;
    private SeekBar skbProgress;
    private Timer mTimer = new Timer();
    public Player(SeekBar skbProgress) {
        this.skbProgress = skbProgress;
        try {
            mediaPlayer = new MediaPlayer();
            mediaPlayer.setAudioStreamType(AudioManager.STREAM_MUSIC);
            mediaPlayer.setOnBufferingUpdateListener(this);
            mediaPlayer.setOnPreparedListener(this);
            mediaPlayer.setOnCompletionListener(this);
        } catch (Exception e) {
            Log.e("mediaPlayer", "error", e);
        }
        mTimer.schedule(mTimerTask, 0, 1000);
    }
    /**
     * 通过定时器和Handler来更新进度条
     */
    TimerTask mTimerTask = new TimerTask() {
        @Override
        public void run() {
            if (mediaPlayer == null)
                return;
            if (mediaPlayer.isPlaying() && skbProgress.isPressed() == false) {
                handleProgress.sendEmptyMessage(0);
            }
        }
    };
    //处理消息
    Handler handleProgress = new Handler() {
        public void handleMessage(Message msg) {
            int position = mediaPlayer.getCurrentPosition();
            int duration = mediaPlayer.getDuration();
            if (duration > 0) {
                int pos = skbProgress.getMax() * position / duration;
                skbProgress.setProgress(pos);
            }
        };
    };
    public void playUrl(String videoUrl) {
        try {
            mediaPlayer.reset();
            mediaPlayer.setDataSource(videoUrl);
            mediaPlayer.prepare();// prepare之后自动播放
        } catch (IllegalArgumentException e) {
            e.printStackTrace();
        } catch (IllegalStateException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
    public void pause() {
        mediaPlayer.pause();
    }
    public void play() {
        mediaPlayer.start();
    }
    public void stop() {
        if (mediaPlayer != null) {
            mediaPlayer.stop();
            mediaPlayer.release();
            mediaPlayer = null;
        }
    }
    /**
     * 通过onPrepared播放
     */
    public void onPrepared(MediaPlayer arg0) {
        arg0.start();
        Log.e("mediaPlayer", "onPrepared");
    }
    public void onCompletion(MediaPlayer arg0) {
        Log.e("mediaPlayer", "onCompletion");
    }
    public void onBufferingUpdate(MediaPlayer arg0, int bufferingProgress) {
        skbProgress.setSecondaryProgress(bufferingProgress);
        int currentProgress = skbProgress.getMax()
                * mediaPlayer.getCurrentPosition() / mediaPlayer.getDuration();
        Log.e(currentProgress + "% play", bufferingProgress + "% buffer");
    }
    public boolean onError(MediaPlayer mp, int what, int extra) {
        return false;
    }
}
3、TestMedia.java    [测试](http://lib.csdn.net/base/softwaretest)用的Activity
package com.tao.test;
import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.SeekBar;
public class TestMedia extends Activity {
    private Button btnPause, btnPlayUrl, btnStop;
    private SeekBar skbProgress;
    private Player player;
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        this.setTitle("在线音乐播放---hellogv编写");
        btnPlayUrl = (Button) this.findViewById(R.id.btnPlayUrl);
        btnPlayUrl.setOnClickListener(new ClickEvent());
        btnPause = (Button) this.findViewById(R.id.btnPause);
        btnPause.setOnClickListener(new ClickEvent());
        btnStop = (Button) this.findViewById(R.id.btnStop);
        btnStop.setOnClickListener(new ClickEvent());
        skbProgress = (SeekBar) this.findViewById(R.id.skbProgress);
        skbProgress.setOnSeekBarChangeListener(new SeekBarChangeEvent());
        player = new Player(skbProgress);
    }
    class ClickEvent implements OnClickListener {
        public void onClick(View arg0) {
            if (arg0 == btnPause) {
                player.pause();
            } else if (arg0 == btnPlayUrl) {
                // 在百度MP3里随便搜索到的,大家可以试试别的链接
                String url = "http://219.138.125.22/myweb/mp3/CMP3/JH19.MP3";
                player.playUrl(url);
            } else if (arg0 == btnStop) {
                player.stop();
            }
        }
    }
    class SeekBarChangeEvent implements SeekBar.OnSeekBarChangeListener {
        int progress;
        public void onProgressChanged(SeekBar seekBar, int progress,
                boolean fromUser) {
            // 原本是(progress/seekBar.getMax())*player.mediaPlayer.getDuration()
            this.progress = progress * player.mediaPlayer.getDuration()
                    / seekBar.getMax();
        }
        public void onStartTrackingTouch(SeekBar seekBar) {
        }
        public void onStopTrackingTouch(SeekBar seekBar) {
            // seekTo()的参数是相对与影片时间的数字，而不是与seekBar.getMax()相对的数字
            player.mediaPlayer.seekTo(progress);
        }
    }
}

