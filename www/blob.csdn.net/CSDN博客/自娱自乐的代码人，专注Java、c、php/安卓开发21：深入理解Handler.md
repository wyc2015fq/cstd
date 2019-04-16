# 安卓开发21：深入理解Handler - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2013年07月12日 12:43:13[initphp](https://me.csdn.net/initphp)阅读数：3069
个人分类：[安卓开发笔记系列](https://blog.csdn.net/initphp/article/category/1439903)

所属专栏：[安卓开发](https://blog.csdn.net/column/details/androidgo.html)









### Handler相关说明：

主要接受子线程发送的数据, 并用此数据配合主线程更新UI。

解释：安卓的UI线程（即OnCreate函数创建的线程）是线程非安全的。也就是说，在UI线程中，使用sleep这样的函数会导致整个线程延迟，但是我们在安卓开发中，往往会经常遇到一些延迟比较厉害的操作，（例如通过HTTP获取数据信息）如果放在主线程中，则会影响UI界面的渲染。但是如果另外新开一个线程，则由于UI线程只能在主线程中修改，而导致无法修改主线程的UI界面。这个时候Handler就出来解决这个问题。Handler主要两大作用：

1. 提供post操作。post操作主要将Runnable对象放进主线程（UI）线程中的队列中操作。post还支持延迟操作。使用post后，Runnable是按照队列的形式逐个执行的。

2. handlerMessage操作。主要用于新开一个线程与主线程中的通信。新开的线程执行完毕后，可以通过handlerMessage给主线程发送消息，并且传递一些参数，然后主线程就可以修改UI界面了。




Handler是运行在UI主线程中的，由于不是在新的线程中使用，所以千万别做复杂的计算逻辑。


![](https://img-blog.csdn.net/20130905144834468?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5pdHBocA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


### Handler提供的函数：

post类方法允许你排列一个Runnable对象到主线程队列中：


post(Runnable)

postAtTime(Runnable,long)

postDelayed(Runnable long)

sendMessage类方法, 允许你安排一个带数据的Message对象到队列中：

sendEmptyMessage(int)

sendMessage(Message)

 sendMessageAtTime(Message,long)

sendMessageDelayed(Message,long)




### Handler post例子：

```java
package com.example.test.com;


import android.annotation.SuppressLint;
import android.app.Activity;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.Window;
import android.widget.Button;
import android.widget.Toast;

@SuppressLint("HandlerLeak")
public class MainActivity extends Activity {

    private Button    btn;

    private MyHandler myHandler;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        setContentView(R.layout.activity_main);

        //实例化一个自定义的Handler
        myHandler = new MyHandler();
        btn = (Button) findViewById(R.id.button1);
        //图片点击的时候，启动动画效果  
        btn.setOnClickListener(new OnClickListener() {

            @Override
            public void onClick(View v) {
                Toast.makeText(getBaseContext(), "post start....",
                        Toast.LENGTH_SHORT).show();
                //发送一个POST操作，将Runnable加入主线程队列
                //这个是在主线程中执行，并且延迟10秒钟延迟加载
                myHandler.postDelayed(new Runnable() {

                    @Override
                    public void run() {
                        btn.setText("Hello Wolrd"); //修改Button文字 
                    }
                }, 10000);

            }
        });
    }

    /**
     * 实现一个自定义的Handler
     */
    public class MyHandler extends Handler {

        @Override
        public void handleMessage(Message msg) {
            Log.d("MyHandler", "handleMessage......");
            super.handleMessage(msg);
        }
    }

}
```


### Handler sendMessage例子：



```java
package com.example.test.com;


import android.annotation.SuppressLint;
import android.app.Activity;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.Window;
import android.widget.Button;
import android.widget.Toast;

@SuppressLint("HandlerLeak")
public class MainActivity extends Activity {

    private Button    btn;

    private MyHandler myHandler;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        setContentView(R.layout.activity_main);

        //实例化一个自定义的Handler
        myHandler = new MyHandler();
        btn = (Button) findViewById(R.id.button1);
        //图片点击的时候，启动动画效果  
        btn.setOnClickListener(new OnClickListener() {

            @Override
            public void onClick(View v) {
                Toast.makeText(getBaseContext(), "new Thread start....",
                        Toast.LENGTH_SHORT).show();
                //新开一个线程,如果新线程中修改UI界面，则直接程序崩溃
                new Thread(new Runnable() {

                    @Override
                    public void run() {
                        try {
                            Thread.sleep(10000); //线程sleep10秒
                        } catch (Exception e) {

                        }
                        Message msg = new Message();
                        Bundle bundle = new Bundle();
                        bundle.putString("val", "Hello Handler");
                        msg.setData(bundle);
                        //这边只能记录日志
                        Log.d("sendMessageDelayed", "send");
                        //线程发送一个Message消息，MyHandler类中的handleMessage
                        //会接收到数据，并且可以更新UI
                        myHandler.sendMessageDelayed(msg, 5000);
                        //myHandler.sendMessage(msg); //无延迟
                    }
                }).start();

            }
        });
    }

    /**
     * 实现一个自定义的Handler
     */
    public class MyHandler extends Handler {

        @Override
        public void handleMessage(Message msg) {
            //接收msg
            Toast.makeText(getBaseContext(), "getMessage....",
                    Toast.LENGTH_SHORT).show();
            super.handleMessage(msg);
            Bundle bundle = msg.getData();
            //修改UI界面
            btn.setText(bundle.getString("val"));
        }
    }

}
```


公用的xml：



```
<RelativeLayout xmlns:android="http://schemas.android.com/apk/res/android"
    xmlns:tools="http://schemas.android.com/tools"
    android:layout_width="match_parent"
    android:layout_height="match_parent"
    android:paddingBottom="@dimen/activity_vertical_margin"
    android:paddingLeft="@dimen/activity_horizontal_margin"
    android:paddingRight="@dimen/activity_horizontal_margin"
    android:paddingTop="@dimen/activity_vertical_margin"
    tools:context=".MainActivity" >

    <!-- button -->

    <Button
        android:id="@+id/button1"
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:layout_alignParentLeft="true"
        android:layout_alignParentTop="true"
        android:layout_marginLeft="70dp"
        android:layout_marginTop="116dp"
        android:text="Button" />

</RelativeLayout>
```













