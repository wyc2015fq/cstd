# Android启动画面Splash - xqhrs232的专栏 - CSDN博客
2011年05月05日 21:19:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：863
原文地址::[http://blog.csdn.net/ghj1976/archive/2011/05/04/6394132.aspx](http://blog.csdn.net/ghj1976/archive/2011/05/04/6394132.aspx)
方法一，两个Activity
核心代码：
package ghj1976.HelloWorld;
import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
public class SplashActivity extends Activity {
 private final int SPLASH_DISPLAY_LENGHT = 8000; // 延迟八秒
 @Override
 protected void onCreate(Bundle savedInstanceState) {
  super.onCreate(savedInstanceState);
  setContentView(R.layout.splash);
  new Handler().postDelayed(new Runnable() {
   public void run() {
    Intent mainIntent = new Intent(SplashActivity.this,
      HelloWorldActivity.class);
    SplashActivity.this.startActivity(mainIntent);
    SplashActivity.this.finish();
   }
  }, SPLASH_DISPLAY_LENGHT);
 }
}说明：
Handler().postDelayed  是延迟指定的时间再执行
Handler类主要可以使用如下3个方法来设置执行Runnable对象的时间：
//  立即执行Runnable对象  
public final boolean post(Runnable r);  
//  在指定的时间（uptimeMillis）执行Runnable对象  
public final boolean postAtTime(Runnable r, long uptimeMillis);  
//  在指定的时间间隔（delayMillis）执行Runnable对象  
public final boolean postDelayed(Runnable r, long delayMillis); 有关 Handler 类的更详细可以看这篇文章：[http://book.51cto.com/art/201006/207064.htm](http://book.51cto.com/art/201006/207064.htm)
下面两行代码启动一个新的Activity，同时关闭当前Activity。
SplashActivity.this.startActivity(mainIntent); 
SplashActivity.this.finish(); 
对 finish 方法的解释如下： [http://android.toolib.net/reference/android/app/Activity.html](http://android.toolib.net/reference/android/app/Activity.html)
Call this when your activity is done and should be closed. The ActivityResult is propagated back to whoever launched you via onActivityResult().
图来自： [http://www.ibm.com/developerworks/cn/opensource/os-cn-android-actvt/](http://www.ibm.com/developerworks/cn/opensource/os-cn-android-actvt/)
如上所示，Android 程序员可以决定一个 Activity 的“生”，但不能决定它的“死”，也就时说程序员可以启动一个 Activity，但是却不能手动的“结束”一个 Activity。
当你调用 Activity.finish()方法时，结果和用户按下 BACK 键一样：告诉 Activity Manager 该 Activity 实例完成了相应的工作，可以被“回收”。
随后 Activity Manager 激活处于栈第二层的 Activity 并重新入栈，同时原 Activity 被压入到栈的第二层，从 Active 状态转到 Paused 状态。
例如上面例子中：从 SplashActivity 中启动了 HelloWorldActivity，则当前处于栈顶端的是 HelloWorldActivity，第二层是 SplashActivity 。
当我们调用 SplashActivity.finish()方法时（我们是在SplashActivity中通过SplashActivity.this.finish()调用的），SplashActivity 从 Active 状态转换 Stoped 状态，并被系统从栈中移除，标志可以被“回收”。
Activity 的状态与它在栈中的位置关系如下图：
上图的例子是
从 Activity1 中启动了 Activity2，则当前处于栈顶端的是 Activity2，第二层是 Activity1，当我们在 Activity2中调用 Activity2.finish()方法时，Activity Manager 重新激活 Activity1 并入栈，Activity2 从 Active 状态转换 Stoped 状态，同时标注Activity2可以被“回收” 。Activity1. onActivityResult(int requestCode, int resultCode,
 Intent data)方法被执行，Activity2 返回的数据通过 data参数返回给 Activity1。
方法二：一个 Activity
布局文件：
1.0" encoding="utf-8"?>
[http://schemas.android.com/apk/res/android](http://schemas.android.com/apk/res/android)"
 android:orientation="vertical" android:layout_width="fill_parent"
 android:layout_height="fill_parent">
 @+id/splashscreen"
  android:orientation="vertical" android:layout_width="fill_parent"
  android:layout_height="fill_parent">
  @+id/info" android:layout_width="fill_parent"
   android:layout_height="wrap_content" android:gravity="center"
   android:paddingTop="10px" android:text="This is a splash !" />
 fill_parent"
  android:paddingTop="10px" android:layout_height="wrap_content"
  android:text="This is a Context" />
说明：
这里有一个id为splashscreen的LinearLayout，是程序启动时显现的部分。当启动完成后，它会被隐藏。
核心代码：
package ghj1976.AndroidTest;
import android.app.Activity;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.os.SystemClock;
import android.view.View;
import android.view.Window;
import android.widget.LinearLayout;
import android.widget.TextView;
public class MainActivity extends Activity {
 private LinearLayout splash;
 private TextView tv;
 private static final int STOPSPLASH = 0;
 // time in milliseconds
 private static final long SPLASHTIME = 1000;
 private Handler splashHandler = new Handler() {
  public void handleMessage(Message msg) {
   switch (msg.what) {
   case STOPSPLASH:
    SystemClock.sleep(4000); 
    splash.setVisibility(View.GONE);
     break;
   }
   super.handleMessage(msg);
  }
 };
 @Override
 public void onCreate(Bundle savedInstanceState) {
  super.onCreate(savedInstanceState);
  getWindow().requestFeature(Window.FEATURE_PROGRESS);
  setContentView(R.layout.main);
  splash = (LinearLayout) findViewById(R.id.splashscreen);
  tv = (TextView) findViewById(R.id.info);
  tv.setText("正在建立数据连接");
  Message msg = new Message();
  msg.what = STOPSPLASH;
  splashHandler.sendMessageDelayed(msg, SPLASHTIME);
 }
}
说明
我们在应用启动后发送一个消息，把 指定区域设置为隐藏， splash.setVisibility(View.GONE);  就实现了 启动界面。
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/ghj1976/archive/2011/05/04/6394132.aspx](http://blog.csdn.net/ghj1976/archive/2011/05/04/6394132.aspx)
