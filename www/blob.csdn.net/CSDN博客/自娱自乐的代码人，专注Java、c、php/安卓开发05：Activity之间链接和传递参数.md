# 安卓开发05：Activity之间链接和传递参数 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2013年06月05日 15:36:01[initphp](https://me.csdn.net/initphp)阅读数：2146
所属专栏：[安卓开发](https://blog.csdn.net/column/details/androidgo.html)









Activity之间链接和传递参数主要通过Intent安卓的一个对象来实现。

**首先我们创建一个MainActivity：**



```java
package com.example.androidtest;


import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.Window;
import android.widget.Toast;

/**
 * 安卓入口文件
 * @author zhuli
 * @date 2013 2013-6-5 下午3:09:14
 */
public class MainActivity extends Activity {

    private String tag          = "MainActivity";

    private int    request_code = 1;

    /**
     * 页面布局
     */
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        setContentView(R.layout.activity_main); //设置一个View模板
        Log.d(tag, "In the onCreate() event");
    }

    /**
     * 点击开启一个新的Activity页面
     * @param view
     */
    public void onClickStartActivity(View view) {
        Log.d(tag, "onClickStartActivity");
        //下面可以传递参数，以及点击后，打开一个新的Activity
        Intent i = new Intent("android.intent.action.OpenActivity");
        i.putExtra("str", "这个是传递一个参数");
        i.putExtra("int", 100);
        startActivityForResult(i, request_code);

    }

    /**
     * 监控打开的Activity页面关闭后回传的参数
     */
    public void onActivityResult(int requestCode, int resultCode, Intent data) {
        Toast.makeText(getBaseContext(), data.getData().toString(), Toast.LENGTH_SHORT).show();
    }
}
```


同时在layout文件夹下创建一个布局文件activity_main.xml





```
<?xml version="1.0" encoding="utf-8"?>  
<LinearLayout xmlns:android="http://schemas.android.com/apk/res/android"  
    android:layout_width="fill_parent"  
    android:layout_height="fill_parent"  
    android:orientation="vertical" >  
      
    
<Button  
    android:id="@+id/btn_dialogs"  
    android:layout_width="fill_parent"   
    android:layout_height="wrap_content"  
    android:text="@string/startActivity"  
    android:onClick="onClickStartActivity" />  
   
</LinearLayout>
```


点击布局页面上的按钮后，就会触发onClickStartActivity方法，该方法会启动一个OpenActivity：

同时我们传递了str和int参数过去。




```java
/**
     * 点击开启一个新的Activity页面
     * @param view
     */
    public void onClickStartActivity(View view) {
        Log.d(tag, "onClickStartActivity");
        //下面可以传递参数，以及点击后，打开一个新的Activity
        Intent i = new Intent("android.intent.action.OpenActivity");
        i.putExtra("str", "这个是传递一个参数");
        i.putExtra("int", 100);
        startActivityForResult(i, request_code);

    }
```


**创建一个OpenActivity：**

OpenActivity中，创建了一个布局的页面，并且布局页面中，有通过消息的方式输出MainActivity的参数

并且OpenActivity中，有一个OnClick方法，主要用来点击布局页面上的一个按钮后触发，并且OnClick方法主要操作就是返回MainActivity，并且传递参数“Hello World”到MainActivity****



```java
package com.example.androidtest;


import android.app.Activity;
import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.Window;
import android.widget.Toast;

public class OpenActivity extends Activity {

    /**
     * 页面布局
     */
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        setContentView(R.layout.activity_open); //设置一个View模板
        Log.d("SECOND", "In the onCreate() event");

        //捕获MainActivity打开该Activity后 接收传递过来的参数
        String str = getIntent().getStringExtra("str");
        Toast.makeText(getBaseContext(), str, Toast.LENGTH_SHORT).show();

        int i = getIntent().getIntExtra("int", 0);
        Toast.makeText(getBaseContext(), "年龄是：" + i, Toast.LENGTH_SHORT).show();
    }

    /**
     * 点击按钮事件后，返回MainActivity页面
     * @param view
     */
    public void onClick(View view) {
        Intent data = new Intent();
        data.setData(Uri.parse("Hello World"));
        setResult(1, data);
        finish();
    }
}
```


创建一个布局文件activity_open：



```
<?xml version="1.0" encoding="utf-8"?>  
<LinearLayout xmlns:android="http://schemas.android.com/apk/res/android"  
    android:layout_width="fill_parent"  
    android:layout_height="fill_parent"  
    android:orientation="vertical" >

    <AnalogClock
        android:id="@+id/analogClock1"
        android:layout_width="wrap_content"
        android:layout_height="wrap_content" />
    
    <Button    
    android:id="@+id/btn_dialogs"    
    android:layout_width="fill_parent"     
    android:layout_height="wrap_content"    
    android:text="@string/progressDialog"    
    android:onClick="onClick" />  
   
</LinearLayout>
```

**修改AndroidManifest.xml：**


新增或者修改Activity都需要在AndroidManifest.xml中进行相关修改：



```
<?xml version="1.0" encoding="utf-8"?>
<manifest xmlns:android="http://schemas.android.com/apk/res/android"
    package="com.example.androidtest"
    android:versionCode="1"
    android:versionName="1.0" >

    <uses-sdk
        android:minSdkVersion="8"
        android:targetSdkVersion="17" />

    <application
        android:allowBackup="true"
        android:icon="@drawable/ic_launcher"
        android:label="@string/app_name"
        android:theme="@style/AppTheme" >
        <!-- 默认的Activity -->
        <activity
            android:name="com.example.androidtest.MainActivity"
            android:label="@string/app_name" >
            <intent-filter>
                <action android:name="android.intent.action.MAIN" />

                <category android:name="android.intent.category.LAUNCHER" />
            </intent-filter>
        </activity>
        <!-- 新增的Activity -->
        <activity
            android:name=".OpenActivity"
            android:label="Open Activity" >
            <intent-filter>
                <action android:name="android.intent.action.OpenActivity" />

                <category android:name="android.intent.category.DEFAULT" />
            </intent-filter>
        </activity>
    </application>

</manifest>
```


**说明：**

打开新的Activity和传递参数：



```java
Intent i = new Intent("android.intent.action.OpenActivity");
        i.putExtra("str", "这个是传递一个参数");
        i.putExtra("int", 100);
        startActivityForResult(i, request_code);
```


接收传递的参数：





```java
//捕获MainActivity打开该Activity后 接收传递过来的参数
        String str = getIntent().getStringExtra("str");
        Toast.makeText(getBaseContext(), str, Toast.LENGTH_SHORT).show();

        int i = getIntent().getIntExtra("int", 0);
        Toast.makeText(getBaseContext(), "年龄是：" + i, Toast.LENGTH_SHORT).show();
```


打开的Activity结束关闭后并传递参数到前一个Activity：





```java
Intent data = new Intent();
        data.setData(Uri.parse("Hello World")); //传递参数
        setResult(RESULT_OK, data); //传递参数 RESULT_OK - 状态  data - 具体参数
        finish();
```


监听回调的参数：





```java
/**
     * 监控打开的Activity页面关闭后回传的参数
     */
    public void onActivityResult(int requestCode, int resultCode, Intent data) {
        //requestCode 是startActivityForResult 传递的request_code，用于区分一个页面上打开的不同的Activity
        //resultCode 是setResult(RESULT_OK, data) 中的RESULT_OK 这个参数，主要是回调的数据状态
        //data就是传递的数据
        if (requestCode == request_code) {
            if (resultCode == RESULT_OK) {
                Toast.makeText(getBaseContext(), data.getData().toString(), Toast.LENGTH_SHORT).show();
            }
        }  
    }
```

![](https://img-blog.csdn.net/20130605160238375)
****






**Bundle:不同的Activity之间的通信，还可以通过Bundle来传递数据。**



```java
Bundle bundle = new Bundle();
bundle.putSerializable("myOrderList",(Serializable) myOrderList); //传递一个序列化对象
intent.putExtras(bundle);
context.startActivity(intent);
```


```java
Bundle bundle = getIntent().getExtras();
ArrayList<JuOrderMO> myOrderList = (ArrayList<JuOrderMO>) bundle.get("myOrderList"); //接收这个对象
```













