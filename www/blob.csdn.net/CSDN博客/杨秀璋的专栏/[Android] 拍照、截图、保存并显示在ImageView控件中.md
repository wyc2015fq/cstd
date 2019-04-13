
# [Android] 拍照、截图、保存并显示在ImageView控件中 - 杨秀璋的专栏 - CSDN博客

2014年10月23日 22:03:24[Eastmount](https://me.csdn.net/Eastmount)阅读数：14379所属专栏：[Android实例开发与学习](https://blog.csdn.net/column/details/eastmount-android.html)



最近在做Android的项目,其中部分涉及到图像处理的内容.这里先讲述如何调用Camera应用程序进行拍照,并截图和保存显示在ImageView控件中以及遇到的困难和解决方法.
PS:作者购买了本《Android第一行代码 著：郭霖》,参照里面的内容完成(推荐该书,前面的布局及应用非常不错).网上这类资料非常多,作者仅仅分享给初学者同时在线记录些内容,希望对大家有所帮助.
首先,设置activity_main.xml为LinearLayout布局且 android:orientation=*"vertical"*

```python
<Button
        android:id="@+id/button1"
        android:layout_width="match_parent"
        android:layout_height="wrap_content"
        android:text="TakePhoto Button" />
   
<ImageView
        android:id="@+id/imageView1"    
        android:layout_width="wrap_content"  
        android:layout_height="wrap_content"
        android:layout_gravity="center_horizontal" />
```
**然后,在****MainActivity.java文件****中public class MainActivity extends Activity修改源代码.添加自定义变量：**

```python
//自定义变量
public static final int TAKE_PHOTO = 1;
public static final int CROP_PHOTO = 2;
private Button takePhotoBn;
private ImageView showImage;
private Uri imageUri; //图片路径
private String filename; //图片名称
```
**添加函数实现点击拍照功能：**

```python
@Override
protected void onCreate(Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);
    setContentView(R.layout.activity_main);
    takePhotoBn = (Button) findViewById(R.id.button1);
    showImage = (ImageView) findViewById(R.id.imageView1);
    //点击"Photo Button"按钮照相
    takePhotoBn.setOnClickListener(new OnClickListener() {
    	@Override
    	public void onClick(View v) {
    		//图片名称 时间命名
    		SimpleDateFormat format = new SimpleDateFormat("yyyyMMddHHmmss");
            Date date = new Date(System.currentTimeMillis());
            filename = format.format(date);
    		//创建File对象用于存储拍照的图片 SD卡根目录           
    		//File outputImage = new File(Environment.getExternalStorageDirectory(),"test.jpg");
    		//存储至DCIM文件夹
    		File path = Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DCIM);  
    		File outputImage = new File(path,filename+".jpg");
    		try {
    			if(outputImage.exists()) {
     				outputImage.delete();
    			}
    			outputImage.createNewFile();
    		} catch(IOException e) {
    			e.printStackTrace();
    		}
    		//将File对象转换为Uri并启动照相程序
    		imageUri = Uri.fromFile(outputImage);
    		Intent intent = new Intent("android.media.action.IMAGE_CAPTURE"); //照相
    		intent.putExtra(MediaStore.EXTRA_OUTPUT, imageUri); //指定图片输出地址
    		startActivityForResult(intent,TAKE_PHOTO); //启动照相
    		//拍完照startActivityForResult() 结果返回onActivityResult()函数
    	}
    });
    if (savedInstanceState == null) {
        getFragmentManager().beginTransaction()
                .add(R.id.container, new PlaceholderFragment())
                .commit();
    }
}
```
**通过startActivityForResult和onActivityResult方法实现拍照截图和保存功能：**

```python
protected void onActivityResult(int requestCode, int resultCode, Intent data) {
	super.onActivityResult(requestCode, resultCode, data);
	if (resultCode != RESULT_OK) { 
		Toast.makeText(MainActivity.this, "ActivityResult resultCode error", Toast.LENGTH_SHORT).show();
		return; 
	}
	switch(requestCode) {
	case TAKE_PHOTO:
		Intent intent = new Intent("com.android.camera.action.CROP"); //剪裁
		intent.setDataAndType(imageUri, "image/*");
		intent.putExtra("scale", true);
		//设置宽高比例
		intent.putExtra("aspectX", 1);
        intent.putExtra("aspectY", 1);
        //设置裁剪图片宽高
        intent.putExtra("outputX", 340);
	    intent.putExtra("outputY", 340);
		intent.putExtra(MediaStore.EXTRA_OUTPUT, imageUri);
		Toast.makeText(MainActivity.this, "剪裁图片", Toast.LENGTH_SHORT).show();
		//广播刷新相册 
	    Intent intentBc = new Intent(Intent.ACTION_MEDIA_SCANNER_SCAN_FILE);
		intentBc.setData(imageUri);     
		this.sendBroadcast(intentBc);    
		startActivityForResult(intent, CROP_PHOTO); //设置裁剪参数显示图片至ImageView
		break;
	case CROP_PHOTO:
		try {	 
			//图片解析成Bitmap对象
			Bitmap bitmap = BitmapFactory.decodeStream(
					getContentResolver().openInputStream(imageUri));
			Toast.makeText(MainActivity.this, imageUri.toString(), Toast.LENGTH_SHORT).show();
			showImage.setImageBitmap(bitmap); //将剪裁后照片显示出来
		} catch(FileNotFoundException e) {
			e.printStackTrace();
		}
		break;
	default:
		break;
	}
}
```
**由于涉及到SD卡中写数据操作和Camera操作,需要在AndroidMainfest.xml文件中声明权限：**

```python
<uses-permission android:name="android.permission.WRITE_EXTERNAL_STORAGE" />
<uses-permission android:name="android.permission.CAMERA" />
```
**运行结果如下图所示：**
![](https://img-blog.csdn.net/20141023203527625?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)![](https://img-blog.csdn.net/20141023204034648?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**需要注意以下几个问题：**
**1.拍照和截图都涉及到startActivityForResult和onActivityResult的交互操作.**

```python
startActivityForResult(
	Intent intent,   //Intent对象
	int requestCode  //>=0 当Activity结束时requestCode将归还在onActivityResult()中
)
onActivityResult(
	int requestCode,  //提供给onActivityResult,以确认返回的数据是从哪个Activity返回的
	int resultCode,   //由子Activity通过其setResult()方法返回 通常为RESULT_CANCELED或RESULT_OK
	Intent data       //一个Intent对象，带有返回的数据
)
```
其中onActivityResult的requestCode和startActivityForResult中的requestCode相对应.同时结合Intent意图实现拍照和截图,核心代码如下:(intent的参数设置省略)
Intent intent = new Intent("android.media.action.IMAGE_CAPTURE");
startActivityForResult(intent,TAKE_PHOTO);
Intent intent = new Intent("com.android.camera.action.CROP");
startActivityForResult(intent, CROP_PHOTO);
2.使用Android拍照保存在系统相册,图库不能立刻显示最新照片.**解决方法是发送系统内置的广播去刷新相册实现显示.代码如下：**
```python
Intent intentBc = new Intent(Intent.ACTION_MEDIA_SCANNER_SCAN_FILE);
intentBc.setData(imageUri);     
this.sendBroadcast(intentBc);
```
**可能你会使用下面这条广播扫描整个SD卡,但4.4已禁止这样的操作：**
**sendBroadcast(new Intent(Intent.ACTION_MEDIA_MOUNTED, Uri.parse(...)))**
**参考资料****[http://blog.csdn.net/xiaanming/article/details/8990627](http://blog.csdn.net/xiaanming/article/details/8990627)**
**3.当运行程序是可能会发现结果图像显示很小,当通过一个Intent意图触发时,Camera程序不会将全尺寸图像返回给主调活动,这样需要大量的内存,而移动设备内存会有一定限制.通常Camera将在返回的意图中返回一幅很小的缩略图,大图可能会导致OOM问题.参考：《Android多媒体开发高级编程 著：Shawn Van Every》**
**针对大图像Android提供BitmapFactory类,允许通过各种资源加载Bitmap图像.调用BitmapFactory.Options类可以定义如何将Bitmap读入内存,当加载图像时,可设置BitmapFactory采样大小.并指定inSampleSize参数表明加载时结果Bitmap图像所占的比例.如inSampleSize=8表明产生一副大小为原始图像1/8的图像.参考下面代码：**
```python
if(resultCode==RESULT_OK) {
	DisplayMetrics dm = new DisplayMetrics();
	getWindowManager().getDefaultDisplay().getMetrics(dm);
	int width = dm.widthPixels; //宽度
	int height = dm.heightPixels ; //高度
	//加载图像尺寸而不是图像本身
	BitmapFactory.Options bmpFactoryOptions = new BitmapFactory.Options();
	bmpFactoryOptions.inJustDecodeBounds = true; //bitmap为null 只是把图片的宽高放在Options里
     Bitmap bitmap = BitmapFactory.decodeFile(imageUri.toString(), bmpFactoryOptions);
	int heightRatio = (int)Math.ceil(bmpFactoryOptions.outHeight/(float)height);
	int widthRatio = (int)Math.ceil(bmpFactoryOptions.outWidth/(float)width);
	//设置图片压缩比例 如果两个比例大于1 图像一边将大于屏幕
	if(heightRatio>1&&widthRatio>1) {
		if(heightRatio>widthRatio) {
			bmpFactoryOptions.inSampleSize = heightRatio;
		}
		else {
			bmpFactoryOptions.inSampleSize = widthRatio;
		}
	}
	//图像真正解码
	bmpFactoryOptions.inJustDecodeBounds = false;
	bitmap = BitmapFactory.decodeFile(imageUri.toString(), bmpFactoryOptions);
	showImage.setImageBitmap(bitmap); //将剪裁后照片显示出来	
}
```
**4.使用nexus 4 剪裁图片后不能setImageBitmap显示在ImageView控件中,其中只有保存按钮,没有剪裁按钮.测试发现没有返回RESULT_OK.这个问题不能解决.Why?**
**参考：****[Unable to Save Photo Edits](https://productforums.google.com/forum/#!topic/nexus/nP1m_BpSObU%5B1-25-false%5D)**
**最后希望文章对大家有所帮助,这是我学习Android图像处理部分的基础性文章与解决过程.下载地址：****[http://download.csdn.net/detail/eastmount/8074833](http://download.csdn.net/detail/eastmount/8074833)**
**参考资料和推荐博文：(都是非常不错的资料-.-)**
**《Android第一行代码》著郭霖 参考8.3 调用摄像头和相册**
**android拍照图片选取与图片剪裁 By:Lee_Allen**
**Android_照相机Camera_调用系统照相机返回data为空 By:strawberry2013**
**Android图片剪裁功能实心详解
 By:小马**
**Android开发 拍照、图片集保存照片技巧**
**Android 拍照并显示在ImageView中（进阶）
 By:leesa**
**android调动系统的照相机并把照片显示在ImageView上**
**cameraintent
 data null in onActivityResult(int requestCode, int resultCode, Intentdata)**
**Android高效加载大图、多图解决方案,有效避免程序OOM By:guolin**
**Android相机、相册获取图片显示并保存到SD卡
 By:唐韧_Ryan**
**android、获取本地图片|直接获取照相图片
 By:zcljy0318**
**(By:Eastmount 2014-10-23 晚10点****http://blog.csdn.net/eastmount/****)**


