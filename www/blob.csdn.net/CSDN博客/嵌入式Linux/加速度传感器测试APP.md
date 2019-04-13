
# 加速度传感器测试APP - 嵌入式Linux - CSDN博客

2015年01月26日 11:01:08[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：2606


java:

```python
package com.example.sensor;
import android.text.InputType;
import android.app.Activity;
import android.content.Context;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.Bundle;
import android.widget.EditText;
import android.widget.TextView;

public class MainActivity extends Activity implements SensorEventListener {
	SensorManager sensorManager;
	TextView teTxt01;
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.fragment_main);
		teTxt01 = (TextView)findViewById(R.id.text_01);
		sensorManager=(SensorManager)getSystemService(Context.SENSOR_SERVICE);
	}
	protected void onResume()
	{
			super.onResume();
			sensorManager.registerListener(this,
					sensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER),SensorManager.SENSOR_DELAY_GAME);
		
	}
	protected void onStop()
	{
		sensorManager.unregisterListener(this);
		super.onStop();
	}
	@Override
	public void onAccuracyChanged(Sensor arg0, int arg1) {
		// TODO Auto-generated method stub
		
	}
	private static int c=0;
	@Override
	public void onSensorChanged(SensorEvent arg0) {
		// TODO Auto-generated method stub
		c++;
		float[] values = arg0.values;
		StringBuilder sb = new StringBuilder();
		sb.append("X: ");
		sb.append(values[0]);
		sb.append("\nY: ");
		sb.append(values[1]);
		sb.append("\nZ: ");
		sb.append(values[2]);
		
		teTxt01.setText(sb.toString());
	}
}
```
fragment_main.xml
```python
<LinearLayout xmlns:android="http://schemas.android.com/apk/res/android"
    xmlns:tools="http://schemas.android.com/tools"
    android:background="@drawable/bg"
    android:orientation="vertical"
    android:layout_width="match_parent"
    android:layout_height="match_parent">
    <TextView 
        android:id="@+id/text_01"
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        />
</LinearLayout>
```
AndroidManifest.xml
```python
<?xml version="1.0" encoding="utf-8"?>
<manifest xmlns:android="http://schemas.android.com/apk/res/android"
    package="com.example.sensor"
    android:versionCode="1"
    android:versionName="1.0" >
    <uses-sdk
        android:minSdkVersion="8"
        android:targetSdkVersion="19" />
    <application
        android:allowBackup="true"
        android:icon="@drawable/ic_launcher"
        android:label="@string/app_name"
        android:theme="@style/AppTheme" >
        <activity
            android:name="com.example.sensor.MainActivity"
            android:screenOrientation="portrait"
            android:label="@string/app_name" >
            <intent-filter>
                <action android:name="android.intent.action.MAIN" />
                <category android:name="android.intent.category.LAUNCHER" />
            </intent-filter>
        </activity>
    </application>
</manifest>
```


