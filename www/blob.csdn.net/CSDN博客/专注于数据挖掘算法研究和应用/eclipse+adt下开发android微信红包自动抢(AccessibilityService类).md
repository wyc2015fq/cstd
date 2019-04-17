# eclipse+adt下开发android微信红包自动抢(AccessibilityService类) - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年06月16日 11:05:16[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：3000








纯粹是兴趣，google提供了android操作系统的钩子AccessibilityService类，用于监听我们手机的焦点、窗口变化、按钮点击、通知栏变化等。微信红包自动抢通过AccessibilityService类，截取通知栏中有[微信红包]字样的通知，然后跳到微信红包界面领取红包。从网上的公开的代码调测了通过，但具体部署到手机上实操未进行，因此代码只做参考，留待后续需要时再深入研究AccessibilityService类。

1、eclipse+adt环境下新建android工程，命名为wxhb，AndroidManifest.xml配置如下：



```
<manifest xmlns:android="http://schemas.android.com/apk/res/android"
    package="com.wxhb"
    android:versionCode="1"
    android:versionName="1.0" >

    <uses-sdk
        android:minSdkVersion="17"
        android:targetSdkVersion="17" />

    <application
        android:allowBackup="true"
        android:icon="@drawable/ic_launcher"
        android:label="@string/app_name"
        android:theme="@style/AppTheme" >
        
        <activity  
            android:name=".MainActivity"  
            android:label="@string/app_name" >  
            <intent-filter>  
                <action android:name="android.intent.action.MAIN" />  
                <category android:name="android.intent.category.LAUNCHER" />  
            </intent-filter>  
        </activity>  
  
        <service  
            android:enabled="true"  
            android:exported="true"  
            android:label="@string/app_name"  
            android:name=".EnvelopeService"  
            android:permission="android.permission.BIND_ACCESSIBILITY_SERVICE">  
            <intent-filter>  
                <action android:name="android.accessibilityservice.AccessibilityService"/>  
            </intent-filter>  
            <meta-data  
                android:name="android.accessibilityservice"  
                android:resource="@xml/envelope_service_config"/>  
        </service>  
        
    </application>

</manifest>
```



2、wxhb工程res目录下新建xml文件夹，并创建envelope_service_config.xml，代码如下：



```
<?xml version="1.0" encoding="utf-8"?>  
<accessibility-service xmlns:android="http://schemas.android.com/apk/res/android"  
    android:accessibilityEventTypes="typeNotificationStateChanged|typeWindowStateChanged"  
    android:accessibilityFeedbackType="feedbackGeneric"  
    android:accessibilityFlags=""  
    android:canRetrieveWindowContent="true"  
    android:description="@string/accessibility_description"  
    android:notificationTimeout="100"  
    android:packageNames="com.tencent.mm" />
```



3、wxhb工程res目录下layout的文件activity_main.xml代码如下：



```
<?xml version="1.0" encoding="utf-8"?>
<RelativeLayout xmlns:android="http://schemas.android.com/apk/res/android"  
    xmlns:tools="http://schemas.android.com/tools"  
    android:layout_width="match_parent"  
    android:layout_height="match_parent"  
    android:paddingBottom="@dimen/activity_vertical_margin"
    android:paddingLeft="@dimen/activity_horizontal_margin"  
    android:paddingRight="@dimen/activity_horizontal_margin"  
    android:paddingTop="@dimen/activity_vertical_margin"  
    tools:context=".MainActivity">  
  
    <Button  
        android:id="@+id/start"  
        android:layout_width="wrap_content"  
        android:layout_height="wrap_content"  
        android:padding="10dp"  
        android:layout_centerInParent="true"  
        android:textSize="18sp"  
        android:text="打开辅助服务"/>  
  
</RelativeLayout>
```
4、wxhb工程res目录下values的文件strings.xml代码如下：





```
<resources>

    <string name="app_name">wxhb</string>
    <string name="accessibility_description">wechatdesc</string>

</resources>
```


5、wxhb工程res目录下values的文件dimens.xml代码如下：

```
<?xml version="1.0" encoding="utf-8"?>
<resources>
    <dimen name="activity_vertical_margin">10dp</dimen>          
    <dimen name="activity_horizontal_margin">10dp</dimen> 
</resources>
```


6、wxhb工程src目录下新建包com.wxhb，主类MainActivity代码如下：





```java
package com.wxhb;

import android.app.Activity;    
import android.content.Intent;    
import android.os.Bundle;    
import android.view.Menu;    
import android.view.MenuItem;    
import android.view.View;    
import android.widget.Button;    
import android.widget.Toast;    
    
public class MainActivity extends Activity {    
    private Button startBtn;    
    
    @Override    
    protected void onCreate(Bundle savedInstanceState) {    
        super.onCreate(savedInstanceState);    
        setContentView(R.layout.activity_main);    
    
        startBtn = (Button) findViewById(R.id.start);    
        startBtn.setOnClickListener(new View.OnClickListener() {    
            @Override    
            public void onClick(View v) {    
                try {    
                    //打开系统设置中辅助功能    
                    Intent intent = new Intent(android.provider.Settings.ACTION_ACCESSIBILITY_SETTINGS);    
                    startActivity(intent);    
                    Toast.makeText(MainActivity.this, "找到抢红包，然后开启服务即可", Toast.LENGTH_LONG).show();    
                } catch (Exception e) {    
                    e.printStackTrace();    
                }    
            }    
        });    
    }    
    
    /*
    @Override    
    public boolean onCreateOptionsMenu(Menu menu) {    
        // Inflate the menu; this adds items to the action bar if it is present.    
        getMenuInflater().inflate(R.menu.menu_main, menu);    
        return true;    
    }    
  
    @Override    
    public boolean onOptionsItemSelected(MenuItem item) {    
        // Handle action bar item clicks here. The action bar will    
        // automatically handle clicks on the Home/Up button, so long    
        // as you specify a parent activity in AndroidManifest.xml.    
        int id = item.getItemId();    
    
        //noinspection SimplifiableIfStatement    
        if (id == R.id.action_settings) {    
            return true;    
        }    
    
        return super.onOptionsItemSelected(item);    
    }   */ 
}
```


7、wxhb工程src目录下新建包com.wxhb，集成AccessibilityService类的代码如下：





```java
package com.wxhb;

import android.accessibilityservice.AccessibilityService;  
import android.annotation.TargetApi;  
import android.app.Notification;  
import android.app.PendingIntent;  
import android.os.Build;  
import android.os.Handler;  
import android.util.Log;  
import android.view.accessibility.AccessibilityEvent;  
import android.view.accessibility.AccessibilityManager;  
import android.view.accessibility.AccessibilityNodeInfo;  
import android.widget.Toast;  
  
import java.util.List;  
  
/** 
 * <p>Created by Administrator</p> 
 * <p/> 
 * 抢红包外挂服务 
 */  
public class EnvelopeService extends AccessibilityService {  
  
    static final String TAG = "Jason";  
  
    /** 
     * 微信的包名 
     */  
    static final String WECHAT_PACKAGENAME = "com.tencent.mm";  
    /** 
     * 红包消息的关键字 
     */  
    static final String ENVELOPE_TEXT_KEY = "[微信红包]";  
  
    Handler handler = new Handler();  
  
    @Override  
    public void onAccessibilityEvent(AccessibilityEvent event) {  
        final int eventType = event.getEventType();  
  
        Log.d(TAG, "事件---->" + event);  
  
        //通知栏事件  
        if (eventType == AccessibilityEvent.TYPE_NOTIFICATION_STATE_CHANGED) {  
            List<CharSequence> texts = event.getText();  
            if (!texts.isEmpty()) {  
                for (CharSequence t : texts) {  
                    String text = String.valueOf(t);  
                    if (text.contains(ENVELOPE_TEXT_KEY)) {  
                        openNotification(event);  
                        break;  
                    }  
                }  
            }  
        } else if (eventType == AccessibilityEvent.TYPE_WINDOW_STATE_CHANGED) {  
            openEnvelope(event);  
        }  
    }  
  
    /*@Override 
    protected boolean onKeyEvent(KeyEvent event) { 
        //return super.onKeyEvent(event); 
        return true; 
    }*/  
  
    @Override  
    public void onInterrupt() {  
        Toast.makeText(this, "中断抢红包服务", Toast.LENGTH_SHORT).show();  
    }  
  
    @Override  
    protected void onServiceConnected() {  
        super.onServiceConnected();  
        Toast.makeText(this, "连接抢红包服务", Toast.LENGTH_SHORT).show();  
    }  
  
    private void sendNotificationEvent() {  
        AccessibilityManager manager = (AccessibilityManager) getSystemService(ACCESSIBILITY_SERVICE);  
        if (!manager.isEnabled()) {  
            return;  
        }  
        AccessibilityEvent event = AccessibilityEvent.obtain(AccessibilityEvent.TYPE_NOTIFICATION_STATE_CHANGED);  
        event.setPackageName(WECHAT_PACKAGENAME);  
        event.setClassName(Notification.class.getName());  
        CharSequence tickerText = ENVELOPE_TEXT_KEY;  
        event.getText().add(tickerText);  
        manager.sendAccessibilityEvent(event);  
    }  
  
    /** 
     * 打开通知栏消息 
     */  
    @TargetApi(Build.VERSION_CODES.JELLY_BEAN)  
    private void openNotification(AccessibilityEvent event) {  
        if (event.getParcelableData() == null || !(event.getParcelableData() instanceof Notification)) {  
            return;  
        }  
        //以下是精华，将微信的通知栏消息打开  
        Notification notification = (Notification) event.getParcelableData();  
        PendingIntent pendingIntent = notification.contentIntent;  
        try {  
            pendingIntent.send();  
        } catch (PendingIntent.CanceledException e) {  
            e.printStackTrace();  
        }  
    }  
  
    @TargetApi(Build.VERSION_CODES.JELLY_BEAN)  
    private void openEnvelope(AccessibilityEvent event) {  
        if ("com.tencent.mm.plugin.luckymoney.ui.LuckyMoneyReceiveUI".equals(event.getClassName())) {  
            //点中了红包，下一步就是去拆红包  
            checkKey1();  
        } else if ("com.tencent.mm.plugin.luckymoney.ui.LuckyMoneyDetailUI".equals(event.getClassName())) {  
            //拆完红包后看详细的纪录界面  
            //nonething  
        } else if ("com.tencent.mm.ui.LauncherUI".equals(event.getClassName())) {  
            //在聊天界面,去点中红包  
            checkKey2();  
        }  
    }  
  
    @TargetApi(Build.VERSION_CODES.JELLY_BEAN)  
    private void checkKey1() {  
        AccessibilityNodeInfo nodeInfo = getRootInActiveWindow();  
        if (nodeInfo == null) {  
            Log.w(TAG, "rootWindow为空");  
            return;  
        }  
        List<AccessibilityNodeInfo> list = nodeInfo.findAccessibilityNodeInfosByText("拆红包");  
        for (AccessibilityNodeInfo n : list) {  
            n.performAction(AccessibilityNodeInfo.ACTION_CLICK);  
        }  
    }  
  
    @TargetApi(Build.VERSION_CODES.JELLY_BEAN)  
    private void checkKey2() {  
        AccessibilityNodeInfo nodeInfo = getRootInActiveWindow();  
        if (nodeInfo == null) {  
            Log.w(TAG, "rootWindow为空");  
            return;  
        }  
        List<AccessibilityNodeInfo> list = nodeInfo.findAccessibilityNodeInfosByText("领取红包");  
        if (list.isEmpty()) {  
            list = nodeInfo.findAccessibilityNodeInfosByText(ENVELOPE_TEXT_KEY);  
            for (AccessibilityNodeInfo n : list) {  
                Log.i(TAG, "-->微信红包:" + n);  
                n.performAction(AccessibilityNodeInfo.ACTION_CLICK);  
                break;  
            }  
        } else {  
            //最新的红包领起  
            for (int i = list.size() - 1; i >= 0; i--) {  
                AccessibilityNodeInfo parent = list.get(i).getParent();  
                Log.i(TAG, "-->领取红包:" + parent);  
                if (parent != null) {  
                    parent.performAction(AccessibilityNodeInfo.ACTION_CLICK);  
                    break;  
                }  
            }  
        }  
    }  
}
```







