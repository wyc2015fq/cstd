# 安卓开发23：Service详细解读 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2013年07月17日 14:33:25[initphp](https://me.csdn.net/initphp)阅读数：1222
个人分类：[安卓开发笔记系列](https://blog.csdn.net/initphp/article/category/1439903)

所属专栏：[安卓开发](https://blog.csdn.net/column/details/androidgo.html)









### 关于Service

**Service说明：**Service是android 系统中的四大组件之一（Activity、Service、BroadcastReceiver、ContentProvider），它跟Activity的级别差不多，但不能自己运行只能后台运行，并且可以和其他组件进行交互。一般适合在后台运行。




### Service启动流程

**context.startService() 启动流程：**


context.startService()  -> onCreate()  -> onStartCommand()  -> Service running  -> context.stopService()  -> onDestroy()  -> Service stop


启动Service方法->创建Service->启动运行（onStartCommand可多次运行，新版本不用onStart了）->关闭Service->销毁->Service停止




**context.bindService()启动流程：**

context.bindService()  -> onCreate()  -> onBind()  -> Service running  -> onUnbind()  -> onDestroy()  -> Service stop





### Service生命周期

![](https://img-blog.csdn.net/20130717143506484?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5pdHBocA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




### Service实例

activity_main.xml，两个按钮，一个启动TestService 一个点击后停止Service



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
        android:text="Start" />

    <Button
        android:id="@+id/button2"
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:layout_alignBaseline="@+id/button1"
        android:layout_alignBottom="@+id/button1"
        android:layout_marginLeft="38dp"
        android:layout_toRightOf="@+id/button1"
        android:text="Stop" />

</RelativeLayout>
```



MainActivity.java:



```java
package com.example.test.com;


import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.Window;
import android.widget.Button;

public class MainActivity extends Activity {

    private Button btn;
    private Button btn2;

    private Intent intent;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        setContentView(R.layout.activity_main);
        intent = new Intent(this, TestService.class);

        //start按钮
        btn = (Button) findViewById(R.id.button1);
        btn.setOnClickListener(new OnClickListener() {

            @Override
            public void onClick(View v) {
                System.out.println("Click");
                Bundle bundle = new Bundle();
                bundle.putString("op", "Hello World2");
                intent.putExtras(bundle);
                startService(intent); //启动后端服务
            }

        });

        //stop按钮
        btn2 = (Button) findViewById(R.id.button2);
        btn2.setOnClickListener(new OnClickListener() {

            @Override
            public void onClick(View v) {
                if (intent != null) {
                    stopService(intent);
                }
            }

        });
    }

    @Override
    protected void onDestroy() {
        //退出的时候停止Service
        if (intent != null) {
            stopService(intent);
        }
    }

}
```





创建一个TestService.java:



```java
package com.example.test.com;


import android.app.Service;
import android.content.Intent;
import android.os.Bundle;
import android.os.IBinder;

public class TestService extends Service {

    Intent intent = new Intent();

    @Override
    public IBinder onBind(Intent intent) {
        System.out.println("Service onBind");
        return null;
    }

    /**
     * Service启动的时候创建一次
     */
    @Override
    public void onCreate() {
        //只有在创建Service的时候 onCreate才会调用一次
        System.out.println("Create TestService");
        super.onCreate();
    }

    /**
     * 每次调用startService都会调用该函数
     */
    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        //接收传递过来的参数
        Bundle bundle = intent.getExtras();
        System.out.println("Service onStartCommand VAL:" + bundle.getString("op"));
        return super.onStartCommand(intent, flags, startId);
    }

    /**
     * 调用stopService的时候销毁Service
     */
    @Override
    public void onDestroy() {
        //停止这个Service的时候会销毁
        System.out.println("Service onDestroy");
        super.onDestroy();
    }

}
```


AndroidManifest.xml 中注册Service：





```
<service android:name=".TestService" >
        </service>
```


效果：

![](https://img-blog.csdn.net/20130717152711312?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5pdHBocA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)






