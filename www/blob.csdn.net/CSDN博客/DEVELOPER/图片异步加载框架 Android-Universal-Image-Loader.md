# 图片异步加载框架 Android-Universal-Image-Loader - DEVELOPER - CSDN博客





2014年03月26日 19:41:07[学术袁](https://me.csdn.net/u012827205)阅读数：1419标签：[android																[图片异步加载																[jar包下载](https://so.csdn.net/so/search/s.do?q=jar包下载&t=blog)




                
现在异步加载图片使用度最高的就数直接导入 Android-Universal-Image-Loader jar包的使用方式了；

现在看一下它的特点：


- 多线程图片加载
- 尽可能多的配置选项（线程池，加载器，解析器，内存/磁盘缓存，显示参数等等）
- 图片可以缓存在内存中，或者设备文件目录下，或者SD卡中
- 可以监听加载进度
- 可以自定义显示每一张图片时都带不同参数
- 支持Widget

[点击免费下载jar包](https://github.com/nostra13/Android-Universal-Image-Loader)

那么下面就开始解释使用这个jar包的步骤：

![](https://img-blog.csdn.net/20140326192955062?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


一》首先：

导入jar包，这个就不多讲了。

二》然后：

### 在AndroidManifest.xml文件的application标签里加入权限和名称，就像这样：


```java
<manifest xmlns:android="http://schemas.android.com/apk/res/android"
    package="com.asyncloader"
    android:versionCode="1"
    android:versionName="1.0" >

    <uses-sdk
        android:minSdkVersion="8"
        android:targetSdkVersion="8" />

    <!-- Include next permission if you want to allow UIL to cache images on SD card -->
    <uses-permission android:name="android.permission.INTERNET" />
    <uses-permission android:name="android.permission.WRITE_EXTERNAL_STORAGE" />

    <application
        android:allowBackup="true"
        android:icon="@drawable/book"
        android:label="@string/app_name"
        android:name=".MyApplycation"
        android:theme="@style/AppTheme" >
        <activity
            android:name="com.asyncloader.MainActivity"
            android:label="@string/app_name" >
            <intent-filter>
                <action android:name="android.intent.action.MAIN" />
                <category android:name="android.intent.category.LAUNCHER" />
            </intent-filter>
        </activity>
    </application>

</manifest>
```
三》然后：



在创建的比如上面的MyApplycation.java中，添加自己想要使用该Android-Universal-Image-Loader属性格式：

```java
public class MyApplycation extends Application {

	@Override  

	   public void onCreate() {  

	       super.onCreate();  

	       DisplayImageOptions options = new DisplayImageOptions.Builder()
			.cacheInMemory(true).cacheOnDisc(true)
			.imageScaleType(ImageScaleType.IN_SAMPLE_INT)
			.bitmapConfig(Bitmap.Config.RGB_565)// 防止内存溢出的，图片太多就这这个。还有其他设置
			// 如Bitmap.Config.ARGB_8888
			.showImageOnLoading(R.drawable.book) // 默认图片
			.showImageForEmptyUri(R.drawable.book) // url爲空會显示该图片，自己放在drawable里面的
			.showImageOnFail(R.drawable.book)// 加载失败显示的图片
//			.displayer(new RoundedBitmapDisplayer(5)) // 圆角，不需要请删除
			.build();

	ImageLoaderConfiguration config = new ImageLoaderConfiguration.Builder(
			this)
			.memoryCacheExtraOptions(480, 800)
			// 缓存在内存的图片的宽和高度
			.discCacheExtraOptions(480, 800, CompressFormat.PNG, 70, null)
			// CompressFormat.PNG类型，70质量（0-100）
			.memoryCache(new WeakMemoryCache()).threadPoolSize(5)
			.threadPriority(Thread.MIN_PRIORITY + 3)
			.denyCacheImageMultipleSizesInMemory()
			.memoryCacheSize(2 * 1024 * 1024)
			// 缓存到内存的最大数据
			.discCacheSize(5 * 1024 * 1024)
			// 缓存到文件的最大数据
			.discCacheFileCount(1000)
			// 文件数量
			.defaultDisplayImageOptions(options)
			.tasksProcessingOrder(QueueProcessingType.LIFO). // 上面的options对象，一些属性配置
			build();
	ImageLoader.getInstance().init(config); // 初始化
}
}
```


当然为实现某种效果，这些东西是必须写的。

四》然后：


在创建的类中使用imageLoader，使用方式：ImageLoader.getInstance().displayImage(imgUrl, imageView); 



```java
public class MainActivity extends Activity {

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_async_image_loader);
		ImageView img=(ImageView) findViewById(R.id.img);
		//网址不能用了
		String uri="http://chinacloudapp.cn/uploadfile/2014/0316/20140316055659717.jpg";
		ImageLoader.getInstance().displayImage(uri, img);
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

}
```
那么，接下来就没什么悬念了。运行跑起来就行了！


最后》介绍一下使用这种加载图片方式的一些方法及参数：



```java
//设置图片在下载期间显示的图片  
     showStubImage(R.drawable.ic_launcher)  
   
     //设置图片Uri为空或是错误的时候显示的图片  
     showImageForEmptyUri(R.drawable.ic_empty)  
   
     //设置图片加载/解码过程中错误时候显示的图片  
     showImageOnFail(R.drawable.ic_error)  
   
     //设置图片在下载前是否重置，复位  
     resetViewBeforeLoading()  
   
     //设置下载的图片是否缓存在内存中  
     cacheInMemory()  
   
     //设置下载的图片是否缓存在SD卡中  
     cacheOnDisc()  
   
     //设置图片的解码类型  
     bitmapConfig(Bitmap.Config.RGB_565)  
   
     //设置图片的解码配置  
     decodingOptions(android.graphics.BitmapFactory.Options decodingOptions)  
   
     //设置图片下载前的延迟  
     delayBeforeLoading(int delayInMillis)  
   
     //设置额外的内容给ImageDownloader  
     extraForDownloader(Object extra)  
   
     //设置图片加入缓存前，对bitmap进行设置  
     preProcessor(BitmapProcessor preProcessor)  
   
     //设置显示前的图片，显示后这个图片一直保留在缓存中  
     postProcessor(BitmapProcessor postProcessor)  
   
     //设置图片以如何的编码方式显示  
     imageScaleType(ImageScaleType imageScaleType)  
  
/** 
 * 设置图片的显示方式 
 * @param displayer 
 */  
displayer(BitmapDisplayer displayer)  
displayer：  
          RoundedBitmapDisplayer（int roundPixels）设置圆角图片  
          FakeBitmapDisplayer（）这个类什么都没做  
          FadeInBitmapDisplayer（int durationMillis）设置图片渐显的时间  
　　　  SimpleBitmapDisplayer()正常显示一张图片　　  
  
  
/** 
 * 图片的缩放方式 
 * @param imageScaleType 
 */  
    imageScaleType(ImageScaleType imageScaleType)  
    imageScaleType:  
                   EXACTLY :图像将完全按比例缩小的目标大小  
                   EXACTLY_STRETCHED:图片会缩放到目标大小完全  
                   IN_SAMPLE_INT:图像将被二次采样的整数倍  
                   IN_SAMPLE_POWER_OF_2:图片将降低2倍，直到下一减少步骤，使图像更小的目标大小  
                   NONE:图片不会调整  
  
/** 
*DisplayImageOptions简单示例 
*/  
// 创建默认的DisplayImageOptions  
DisplayImageOptions option_0=DisplayImageOptions.createSimple();  
// 使用DisplayImageOptions.Builder()创建DisplayImageOptions  
DisplayImageOptions options = new DisplayImageOptions.Builder()    
    .imageScaleType(ImageScaleType.EXACTLY_STRETCHED)  
    .showStubImage(R.drawable.loading)    
    .showImageForEmptyUri(R.drawable.load_fail)    
    .showImageOnFail(R.drawable.load_fail)    
    .cacheInMemory(true)    
    .cacheOnDisc(true)      
    .build();
```](https://so.csdn.net/so/search/s.do?q=图片异步加载&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)




