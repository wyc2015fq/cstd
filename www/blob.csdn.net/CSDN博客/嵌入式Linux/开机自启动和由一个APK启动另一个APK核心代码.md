
# 开机自启动和由一个APK启动另一个APK核心代码 - 嵌入式Linux - CSDN博客

2013年09月23日 09:52:48[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：3161个人分类：[Java																](https://blog.csdn.net/weiqifa0/article/category/1606957)



1、开机自启动

```python
public class BootReceiver extends BroadcastReceiver {
	private static final String ACTION = "android.intent.action.BOOT_COMPLETED";
    private static final String TAG = "ZED_Led_Receiver";
    private static final String FileName = "/sys/class/zed/led1";
	private static final String action_boot="android.intent.action.BOOT_COMPLETED"; //启动APK

    @Override
    public void onReceive(Context context, Intent intent) {
		//Log.e(TAG,"LedReceiver.");
		if (intent.getAction().equals(ACTION)){
			{   //启动APK
				Intent ootStartIntent=new Intent(context,BootActivity.class); //这一句的BootActivity是要启动的activity
				ootStartIntent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK); 
				context.startActivity(ootStartIntent); 
			}
			if(android.os.SystemProperties.get("persist.sys.skled").equals("1")){
				ledSet(1);
			}
			else{
				ledSet(0);
			} 
		}	
	}
```

```python
<?xml version="1.0" encoding="utf-8"?>
<manifest xmlns:android="http://schemas.android.com/apk/res/android"
    package="com.pstreets.demo"
    android:versionCode="1"
    android:versionName="1.0" >
    <uses-sdk android:minSdkVersion="10" />
    <uses-permission android:name="android.permission.RECEIVE_BOOT_COMPLETED" />
    <uses-permission android:name="android.permission.DISABLE_KEYGUARD" />
 
    <application
        android:icon="@drawable/ic_launcher"
        android:label="@string/app_name" >
        <activity
            android:name=".BootupDemoActivity"
            android:label="@string/app_name" >
            <intent-filter>
                <action android:name="android.intent.action.MAIN" />
                <category android:name="android.intent.category.LAUNCHER" />
            </intent-filter>
        </activity>
        
         <receiver android:name=".BootupReceiver" android:enabled="true"
          android:permission="android.permission.RECEIVE_BOOT_COMPLETED">            
        	<intent-filter>                
        		<action android:name="android.intent.action.BOOT_COMPLETED"></action>   
        		 <category android:name="android.intent.category.DEFAULT" />         
        	</intent-filter> 
        </receiver>
    </application>
</manifest>
```
2、一个APK启动另一个APK代码

```python
package com.motic.uitest;
import android.app.Activity;
import android.content.ComponentName;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
public class UITestActivity extends Activity {
    /** Called when the activity is first created. */
	Button m_btn;
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        
        m_btn = (Button)findViewById(R.id.but);
        m_btn.setOnClickListener(new BtnLister());      
    }
    
    
    public class BtnLister implements OnClickListener
    {
		public void onClick(View v) {
			
			Intent mIntent = new Intent( ); 
			ComponentName comp = new ComponentName("com.example", "com.example.UITutorial_V2Activity");
//	        ComponentName comp = new ComponentName("com.example.layout",
//	        		"com.example.layout.SmsActivity");
			mIntent.setComponent(comp); 
	        mIntent.setAction("android.intent.action.VIEW"); 
	        startActivity(mIntent);
	        finish();
		}   	
    }
    
  
}
```



