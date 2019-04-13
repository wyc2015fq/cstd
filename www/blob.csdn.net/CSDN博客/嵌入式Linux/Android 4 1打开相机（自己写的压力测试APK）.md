
# Android 4.1打开相机（自己写的压力测试APK） - 嵌入式Linux - CSDN博客

2014年12月25日 19:38:19[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：1541


JAVA：
```python
package com.example.cameratest;
import android.support.v7.app.ActionBarActivity;
import android.support.v4.app.Fragment;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.provider.MediaStore;
public class MainActivity extends ActionBarActivity {
	private Button button1;
    private int TIME = 1000;  
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.fragment_main);
		handler.postDelayed(runnable, TIME); //每隔1s执行 
		button1=(Button)findViewById(R.id.button1qqqq);
		button1.setOnClickListener(new Button.OnClickListener(){
			 public void onClick(View v)
			 {
				 System.out.println("finish.$$$$$$$$$$$$$$$$$$$$$$$"); 
				 handler.removeCallbacks(runnable);
				 //finish();
				 System.exit(0);
			 }
	});
	}
	private static int k=0;
	Handler handler = new Handler();  
    Runnable runnable = new Runnable() {  
     
        @Override  
        public void run() {  
            // handler自带方法实现定时器  
            try {  
                handler.postDelayed(this, TIME);  
                //tvShow.setText(Integer.toString(i++));  
                System.out.println("do..."); 
                k++;
                if(k==5){
                	Intent intent = new Intent(MediaStore.ACTION_IMAGE_CAPTURE);
                	startActivityForResult(intent,0);
                	System.out.println("do...33333"); 
                }
                if(k==12)
                {
                	System.out.println("do..6666."); 
                	 Intent intent=new Intent();
                     intent.setClass(MainActivity.this,MainActivity.class);
                    startActivity(intent);
                	k=0;
                }
            } catch (Exception e) {  
                // TODO Auto-generated catch block  
                e.printStackTrace();  
                System.out.println("exception...");  
            }  
        }  
    };  
}
```
XML：

```python
<?xml version="1.0" encoding="utf-8"?>
<manifest xmlns:android="http://schemas.android.com/apk/res/android"
      package="com.demouhf.uhfjni"
      android:versionCode="1"
      android:versionName="1.0">
    <uses-sdk android:minSdkVersion="3" />
    <uses-permission android:name="android.permission.CAMERA" />
    <uses-permission android:name="android.permission.CAPTURE_AUDIO_OUTPUT"/>
    <application android:label="@string/app_name">
        <activity android:name="com.android.gallery3d" />
        <activity android:name=".UHFActivity"
                  android:label="@string/app_name">
            <intent-filter>
                <action android:name="android.intent.action.MAIN" />
                <category android:name="android.intent.category.LAUNCHER" />
            </intent-filter>
        </activity>
    </application>
</manifest>
```



