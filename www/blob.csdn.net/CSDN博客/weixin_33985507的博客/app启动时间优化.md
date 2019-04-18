# app启动时间优化 - weixin_33985507的博客 - CSDN博客
2017年08月31日 14:05:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：9
app启动时间龟速，主要就是因为在Application的onCreate里面或者启动页的onCreate里面干了很多耗时的操作，比如各种第三方框架的初始化等等。
首先我们可以用TraceView来对方法对执行时间进行检测：
1.通过DDMS打开TraceView
![851999-1cfa5bc3cad55bad.jpg](https://upload-images.jianshu.io/upload_images/851999-1cfa5bc3cad55bad.jpg)
这里写图片描述
2.使用代码
```
// 在自己想要开始调试的地方start
Debug.startMethodTracing("GithubApp");
// 在合适的地方stop
Debug.stopMethodTracing();
```
需要权限：
```
<uses-permission android:name="android.permission.WRITE_EXTERNAL_STORAGE"/>
```
通过adb pull将其导出到本地
adb pull /sdcard/GithubApp.trace ~/temp
打开DDMS分析trace文件
![851999-e09194a9af548187.png](https://upload-images.jianshu.io/upload_images/851999-e09194a9af548187.png)
这里写图片描述
在下方的方法区点击"Real Time/Call", 按照方法每次调用耗时降序排.
耗时超过500ms都是值得注意的.
现在知道哪些是耗时比较厉害的第三方，除了一些马上用得到的第三方，其他的都可以放在子线程里初始化。
这就用到了我之前说到的IntentService
```
public class InitializeService extends IntentService {
    private static final String ACTION_INIT_WHEN_APP_CREATE = "com.anly.githubapp.service.action.INIT";
    public InitializeService() {
        super("InitializeService");
    }
    public static void start(Context context) {
        Intent intent = new Intent(context, InitializeService.class);
        intent.setAction(ACTION_INIT_WHEN_APP_CREATE);
        context.startService(intent);
    }
    @Override
    protected void onHandleIntent(Intent intent) {
        if (intent != null) {
            final String action = intent.getAction();
            if (ACTION_INIT_WHEN_APP_CREATE.equals(action)) {
                performInit();
            }
        }
    }
    private void performInit() {
        AppLog.d("performInit begin:" + System.currentTimeMillis());
        // init Drawer image loader
        DrawerImageLoader.init(new AbstractDrawerImageLoader() {
            @Override
            public void set(ImageView imageView, Uri uri, Drawable placeholder) {
                ImageLoader.loadWithCircle(getApplicationContext(), uri, imageView);
            }
        });
        // init crash helper
        CrashHelper.init(this.getApplicationContext());
        // init Push
        PushPlatform.init(this.getApplicationContext());
        // init Feedback
        FeedbackPlatform.init(this.getApplication());
        // init Share
        SharePlatform.init(this.getApplicationContext());
        AppLog.d("performInit end:" + System.currentTimeMillis());
    }
}
```
二.启动白屏
做一个启动页什么都不做
1.做一个logo_splash的背景:
```
<?xml version="1.0" encoding="utf-8"?>
<layer-list xmlns:android="http://schemas.android.com/apk/res/android">
    <!-- 底层白色 -->
    <item android:drawable="@color/white" />
    <!-- 顶层Logo居中 -->
    <item>
        <bitmap
            android:gravity="center"
            android:src="@drawable/ic_github" />
    </item>
</layer-list>
```
2.弄一个主题
```
<style name="SplashTheme" parent="AppTheme">
    <item name="android:windowBackground">@drawable/logo_splash</item>
</style>
```
```
public class LogoSplashActivity extends BaseActivity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        // 注意, 这里并没有setContentView, 单纯只是用来跳转到相应的Activity.
        // 目的是减少首屏渲染
        if (AppPref.isFirstRunning(this)) {
            IntroduceActivity.launch(this);
        }
        else {
            MainActivity.launch(this);
        }
        finish();
    }
}
```
3.在AndroidManifest.xml中设置其为启动屏, 并加上主题:
```
<activity
  android:name=".ui.module.main.LogoSplashActivity"
  android:screenOrientation="portrait"
  android:theme="@style/SplashTheme">
  <intent-filter>
      <action android:name="android.intent.action.MAIN"/>
      <category android:name="android.intent.category.LAUNCHER"/>
  </intent-filter>
</activity>
```
效果
![851999-82f582dbf29c0440.gif](https://upload-images.jianshu.io/upload_images/851999-82f582dbf29c0440.gif)
这里写图片描述
