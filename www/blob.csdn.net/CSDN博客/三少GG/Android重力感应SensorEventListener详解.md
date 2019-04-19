# Android重力感应SensorEventListener详解 - 三少GG - CSDN博客
2012年01月31日 00:41:59[三少GG](https://me.csdn.net/scut1135)阅读数：10643

两个类：
1、SensorTest类，是Acvitivity
2、SensorListenerTest类，实现了SensorEventListener类，
SensorTest.java:
```java
package com.sensor;
import android.app.Activity;
import android.os.Bundle;
public class SensorTest extends Activity {
    /** Called when the activity is first created. */
    SensorListenerTest sensorListenerTest;
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        // 传参数的时候，用this，不是自己新建的Context
        sensorListenerTest = new SensorListenerTest(this);
    }
    // 添加OnResume和OnStop，注册和取消Sensor服务。
    @Override
    protected void onResume() {
        // 监听服务，在activity和Listener里面都要注册，这是一个难点。不要只在一个地方注册。
        // TODO Auto-generated method stub
        super.onResume();
        sensorListenerTest.enableSensor();
    }
    @Override
    protected void onStop() {
        // TODO Auto-generated method stub
        super.onStop();
        sensorListenerTest.disableSensor();
    }
}
```
SensorListenerTest.java:
```java
package com.sensor;
import android.content.Context;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.util.Log;
public class SensorListenerTest implements SensorEventListener {
    private SensorManager mSensorManager;
    private Sensor sensor;
    private float mLastX, mLastY, mLastZ;
    private Context mContext;
    public SensorListenerTest(Context context) {
        mContext = context;
        // 在构造函数里面注册Sensor服务
        enableSensor();
    }
    // 注册传感器服务，在本java和Activity里面都要注册，但是取消注册的时候，只在activity里面取消注册即可。
    public void enableSensor() {
        // 在这里真正注册Service服务
        mSensorManager = (SensorManager) mContext
                .getSystemService(Context.SENSOR_SERVICE);
        sensor = mSensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
        if (mSensorManager == null) {
            Log.v("sensor..", "Sensors not supported");
        }
        mSensorManager.registerListener(this, sensor,
                SensorManager.SENSOR_DELAY_NORMAL);
    }
    // 取消注册传感器服务
    public void disableSensor() {
        if (mSensorManager != null) {
            mSensorManager.unregisterListener(this);
            mSensorManager = null;
        }
    }
    @Override
    public void onAccuracyChanged(Sensor arg0, int arg1) {
        // TODO Auto-generated method stub
    }
    @Override
    public void onSensorChanged(SensorEvent event) {
        // TODO Auto-generated method stub
        if (event.sensor == null) {
            return;
        }
        if (event.sensor.getType() == Sensor.TYPE_ACCELEROMETER) {
            mLastX = event.values[SensorManager.DATA_X];
            mLastY = event.values[SensorManager.DATA_Y];
            mLastZ = event.values[SensorManager.DATA_Z];
            String sX = String.valueOf(mLastX);
            String sY = String.valueOf(mLastY);
            String sZ = String.valueOf(mLastZ);
            Log.v("mLastX==", sX);
            Log.v("mLastY==", sY);
            Log.v("mLastZ==", sZ);
        }
    }
}
```
总结：在普通的java类里面，获得SensorManager的服务和具体的Sensor，不像在activity里面（不需要context作为参数，就可以直接获取），
所以用构造函数的context作为参数，让它获得SensorManager的服务和具体的Sensor，**在activity里面，传递的context参数，不是像普通的java类那样定义一个context，而是传递this。这点需要注意。**
**【以上转自 :[http://www.j2megame.com/html/xwzx/ty/2704.html](http://www.j2megame.com/html/xwzx/ty/2704.html)】**
**有用参考：**
### **[Android教程之实现动作感应技术](http://yarin.iteye.com/blog/412534)**
**4.SensorListener必须实现两个方法**
**1.发现Android提供了SensorListener接口 2.要使用还得有个动作感应管理器SensorManager**
**Java代码  ![收藏代码](http://yarin.iteye.com/images/icon_star.png)**
- **//取得SensorManager **
- **SensorManager sensorMgr = (SensorManager)getSystemService(SENSOR_SERVICE);     **
**3.注册我们获得的SensorManager**
**Java代码  ![收藏代码](http://yarin.iteye.com/images/icon_star.png)**
- **//返回true表示注册成功，flase则反之 **
- **sensorMgr.registerListene(this,SensorManager.SENSOR_ACCELEROMETER,SensorManager.SENSOR_DELAY_GAME);   **
**如果注册SensorManager不成功就调用**
**Java代码  ![收藏代码](http://yarin.iteye.com/images/icon_star.png)**
- **sensorMgr.unregisterListener(this,SensorManager.SENSOR_ACCELEROMETER);  **
**4.SensorListener必须实现两个方法**
**Java代码  ![收藏代码](http://yarin.iteye.com/images/icon_star.png)**
- **publicvoid onAccuracyChanged(int arg0, int arg1) {}   **
- **publicvoid onSensorChanged(int sensor, float[] values) {}   **

