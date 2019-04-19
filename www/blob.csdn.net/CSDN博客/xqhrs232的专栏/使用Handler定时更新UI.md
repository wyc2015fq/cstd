# 使用Handler定时更新UI - xqhrs232的专栏 - CSDN博客
2011年03月12日 10:38:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：877标签：[android																[layout																[button																[import																[encoding																[thread](https://so.csdn.net/so/search/s.do?q=thread&t=blog)](https://so.csdn.net/so/search/s.do?q=encoding&t=blog)](https://so.csdn.net/so/search/s.do?q=import&t=blog)](https://so.csdn.net/so/search/s.do?q=button&t=blog)](https://so.csdn.net/so/search/s.do?q=layout&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://blog.csdn.net/wtao158/archive/2010/01/06/5145046.aspx](http://blog.csdn.net/wtao158/archive/2010/01/06/5145046.aspx)
网上相关文章
Android 平台上，界面元素在定时器的响应函数里刷新
[http://yansaibo.javaeye.com/blog/716990](http://yansaibo.javaeye.com/blog/716990)
在Android里定时更新 UI，通常使用的是 java.util.Timer, java.util.TimerTask, android.os.Handler组合。实际上Handler 自身已经提供了定时的功能。
参考android.os.Handler 的文档 
There are two main uses for a Handler: (1) to schedule messages and runnables to be executed as some point in the future; and (2) to enqueue an action to be performed on a different thread than your own.
Scheduling messages is accomplished with the post(Runnable), postAtTime(Runnable, long), postDelayed(Runnable, long), sendEmptyMessage(int), sendMessage(Message), sendMessageAtTime(Message, long), and sendMessageDelayed(Message, long) methods. The post versions
 allow you to enqueue Runnable objects to be called by the message queue when they are received; the sendMessage versions allow you to enqueue a Message object containing a bundle of data that will be processed by the Handler's handleMessage(Message) method
 (requiring that you implement a subclass of Handler)
When posting or sending to a Handler , you can either allow the item to be processed as soon as the message queue is ready to do so, or specify a delay before it gets processed or absolute time for it to be processed. The latter two allow you to implement
 timeouts, ticks, and other timing-based behavior.
下面是一个简单的计数器程序，每隔一秒递增计数器 
main.xml
<?xml version="1.0" encoding="utf-8"?>  
<LinearLayout xmlns:android="[http://schemas.android.com/apk/res/android](http://schemas.android.com/apk/res/android)" 
    android:orientation="vertical" android:layout_width="fill_parent"  
    android:layout_height="fill_parent">  
    <TextView android:id="@+id/counter" android:layout_width="fill_parent"  
        android:layout_height="wrap_content" android:text="Count: 0" />  
    <LinearLayout android:orientation="horizontal"  
        android:layout_width="fill_parent" android:layout_height="wrap_content"> 
        <Button android:text="start" android:id="@+id/Button01"  
            android:layout_width="fill_parent" android:layout_height="wrap_content" android:layout_weight="1.0"></Button> 
        <Button android:text="stop" android:id="@+id/Button02"  
            android:layout_width="fill_parent" android:layout_height="wrap_content" android:layout_weight="1.0" android:enabled="false"></Button> 
        <Button android:text="reset" android:id="@+id/Button03"  
            android:layout_width="fill_parent" android:layout_height="wrap_content" android:layout_weight="1.0"></Button> 
    </LinearLayout>  
</LinearLayout>  
java代码：
import android.app.Activity;   
import android.os.Bundle;   
import android.os.Handler;   
import android.view.View;   
import android.view.View.OnClickListener;   
import android.widget.Button;   
import android.widget.TextView;   
public class TestTimer extends Activity {   
    private Button btnStart;   
    private Button btnStop;   
    private Button btnReset;   
    private TextView tvCounter;   
    private long count = 0;   
    private boolean run = false;   
    private Handler handler = new Handler();   
    private Runnable task = new Runnable() {   
        public void run() {   
            // TODO Auto-generated method stub   
            if (run) {   
                handler.postDelayed(this, 1000);   
                count++;   
            }   
            tvCounter.setText("Count: " + count);   
        }   
    };   
    /** Called when the activity is first created. */  
    @Override  
    public void onCreate(Bundle savedInstanceState) {   
        super.onCreate(savedInstanceState);   
        setContentView(R.layout.main);   
        btnStart = (Button) findViewById(R.id.Button01);   
        btnStop = (Button) findViewById(R.id.Button02);   
        btnReset = (Button) findViewById(R.id.Button03);   
        tvCounter = (TextView) findViewById(R.id.counter);   
        btnStart.setOnClickListener(new OnClickListener() {   
            public void onClick(View v) {   
                // TODO Auto-generated method stub   
                run = true;   
                updateButton();   
                handler.postDelayed(task, 1000);   
            }   
        });   
        btnStop.setOnClickListener(new OnClickListener() {   
            public void onClick(View v) {   
                // TODO Auto-generated method stub   
                run = false;   
                updateButton();   
                handler.post(task);   
            }   
        });   
        btnReset.setOnClickListener(new OnClickListener() {   
            public void onClick(View v) {   
                // TODO Auto-generated method stub   
                count = 0;   
                run = false;   
                updateButton();   
                handler.post(task);   
            }   
        });   
    }   
    private void updateButton() {   
        btnStart.setEnabled(!run);   
        btnStop.setEnabled(run);   
    }   
}  
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/wtao158/archive/2010/01/06/5145046.aspx](http://blog.csdn.net/wtao158/archive/2010/01/06/5145046.aspx)

