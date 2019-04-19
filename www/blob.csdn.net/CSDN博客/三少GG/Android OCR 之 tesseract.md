# Android OCR 之 tesseract - 三少GG - CSDN博客
2012年06月14日 19:32:43[三少GG](https://me.csdn.net/scut1135)阅读数：3968标签：[android																[path																[string																[matrix																[git																[图像处理](https://so.csdn.net/so/search/s.do?q=图像处理&t=blog)](https://so.csdn.net/so/search/s.do?q=git&t=blog)](https://so.csdn.net/so/search/s.do?q=matrix&t=blog)](https://so.csdn.net/so/search/s.do?q=string&t=blog)](https://so.csdn.net/so/search/s.do?q=path&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android 常规](https://blog.csdn.net/scut1135/article/category/1057214)
## [Android OCR 之 tesseract](http://www.cnblogs.com/hangxin1940/archive/2012/01/13/2321507.html)
OCR属于CV的范畴，也就是计算机视觉，目前来看，除了opencv这个龙头老大，也就是hp开发的tesseract比较好用，虽然年头比较长了，但现在归google维护并托管在google code上了。
现在有android版本的
地址：http://code.google.com/p/tesseract-android-tools/
这个版本得自己git 三个库 leptonica  tesseract  libjpeg ，我自己是编译成功了，但测试的时候native层总是crash。
于是发现了tess的android的另一个分支 tess-two
推荐linux上编译
一、下载&编译
1、首先下载tess-two
```
git clone git://github.com/rmtheis/tess-two tess
```
2、进入 tess目录，里面有三个项目，我们只需要进入tess-two就可以直接编译了
```
cd tess/tess-two
ndk-build
```
 3、编译好后，将src下的两个包以及libs导入到自己的项目就可以用啦
这里把我我把编译好后的东西放出来，用的话不用再编译了
下载：[tess-two.zip](http://files.cnblogs.com/hangxin1940/tess-two.zip)
二、使用
tesseract 使用了 leptonica的图像处理库，对于图像处理还是比较强大的
Android官方地址:[tesseract-android-tools](http://code.google.com/p/tesseract-android-tools/)
但它必须要一个匹配库，即tessdata，我们可以从官方拷贝，在前面git的项目里面tesseract源码目录有现成的tessdata可以用，对于中文，google code上也有下载，当然也可以自己训练不同语言的tessdata。
包leptonica的类我们不必使用，只要使用tess包的类就行了
TessBaseAPI
使用时，首先创建TessBaseAPI对象
```
[]()
TessBaseAPI baseApi=new TessBaseAPI();
//初始化tess
//android下面，tessdata肯定得放到sd卡里了
//如果tessdata这个目录放在sd卡的根目录
//那么path直接传入sd卡的目录
//eng就是英文，关于语言，按ISO 639-3标准的代码就行，具体请移步wiki
baseApi.init("tessdata文件夹的父级目录", "eng");
//options是为了缩放图片，这个酌情缩放，图片小的话可以不缩放
BitmapFactory.Options options=new BitmapFactory.Options();
//缩小为原来的1/2
options.inSampleSize=2;
//bitmap，我这里是以流的形式，只要能形成Bitmap就行
Bitmap bitmap = BitmapFactory.decodeStream(instream,null,options); 
        instream.close();    
//如果图片有Alpha值，那么最好设置一下
/*ExifInterface exif = new ExifInterface(filename)
    int exifOrientation = exif.getAttributeInt(ExifInterface.TAG_ORIENTATION, ExifInterface.ORIENTATION_NORMAL); 
    int rotate = 0;
    switch (exifOrientation) {
        case ExifInterface.ORIENTATION_ROTATE_90:
            rotate = 90;
            break;
        case ExifInterface.ORIENTATION_ROTATE_180:
            rotate = 180;
            break;
        case ExifInterface.ORIENTATION_ROTATE_270:
            rotate = 270;
            break;
    } 
    if (rotate != 0) { 
        // Getting width & height of the given image.
        int w = bitmap.getWidth();
        int h = bitmap.getHeight(); 
        // Setting pre rotate
        Matrix mtx = new Matrix();
        mtx.preRotate(rotate); 
        // Rotating Bitmap
        bitmap = Bitmap.createBitmap(bitmap, 0, 0, w, h, mtx, false);
        // tesseract req. ARGB_8888
        bitmap = bitmap.copy(Bitmap.Config.ARGB_8888, true);
    } */
//设置要ocr的图片bitmap
baseApi.setImage(bitmap);
//根据Init的语言，获得ocr后的字符串
String text= baseApi.getUTF8Text();
//释放bitmap
baseApi.clear();
//如果连续ocr多张图片，这个end可以不调用，但每次ocr之后，必须调用clear来对bitmap进行释放
//释放native内存
baseApi.end();
///////////////////////////其它方法//////////////////////////////////
        
        //获取字符边框
        
        Pixa pixa= baseApi.getCharacters();
        
        //同上，这个是整段文字的边框
        baseApi.getRegions();
        
        //同上，只不过这里是条线
        baseApi.getTextlines();
        
        //剩下的自己测试吧。
        
        //转为rect数组 ，之后，可以很方便的在图片上框出方框
        //怎么框由你
        ArrayList<Rect> rects=pixa.getBoxRects();
[]()
```
结束。
android下ocr就这么简单，好好使用第三方库就行了。
ps:在使用这套库对12306的验证码进行ocr时，耗时50ms左右。
android应用中ocr的解决方案大致有两种，而采用最多的还是tesseract.小弟就在这里将我最近两天解决思路写下来，如有缺陷，欢迎拍砖：
　　有两种解决方案，一种是采用tesseract cloud-service,这钟是把图片信息发送到云端，然后获得图片分析数据；第二种就是不用联网，本地化分析图片上信息。我就说说第二种,第一种我会在最后给大家一个链接（文章很不错）。
　　搜先就是下载Tesseract native android library.这里有两个链接，你选哪个链接都可以：
　　a.svn checkout[http://tesseract-android-tools.googlecode.com/svn/trunk/](http://tesseract-android-tools.googlecode.com/svn/trunk/)tesseract-android-tools。(如果不能checkout到，废话别说就到官方上下:http://code.google.com/p/tesseract-android-tools/)
　　b.可能上面一个下载后编译有些人会遇到一些问题，比如找不到jgep库，编译不成功。所以有了这个项目：git clone git://github.com/rmtheis/tess-two.git （这个包里面内容太多，不过也省得下那么多库了）
　　这里先说采用第一个源下载：下载成功后，打开README文件，做下修改（如下）：
git clone git://android.git.kernel.org/platform/external/jpeg.git libjpeg
修改为：
git clone[https://android.googlesource.com/platform/external/jpeg](https://android.googlesource.com/platform/external/jpeg)libjpeg
n
　　对于第二个源[下载](http://www.2cto.com/soft)，由于里面没有README文件，操作命令如下：
cd <project-directory>/tess-two
export TESSERACT_PATH=${PWD}/external/tesseract-3.01
export LEPTONICA_PATH=${PWD}/external/leptonica-1.68
export LIBJPEG_PATH=${PWD}/external/libjpeg
ndk-build
android update project --path .
ant release
　　最终两个都得到你想要的libs里面的so文件和src里面的对so文件的封装类。这个就是我们开发所用到的东东啦。
　　然后新建工程，代码如下：
public class MainActivity extends Activity {
private static final String TAG = "MainActivity ...";
private static final String TESSBASE_PATH = "/mnt/sdcard/tesseract/";
private static final String DEFAULT_LANGUAGE = "eng";
private static final String IMAGE_PATH = "/mnt/sdcard/test1.jpg";
private static final String EXPECTED_FILE = TESSBASE_PATH + "tessdata/" + DEFAULT_LANGUAGE
+ ".traineddata";
private TessBaseAPI service;
@Override
protected void onCreate(Bundle savedInstanceState) {
super.onCreate(savedInstanceState);
setContentView(R.layout.main);
testOcr();
}
public void testOcr(){
mHandler.post(new Runnable() {
@Override
public void run() {
Log.d(TAG, "begin>>>>>>>");
ocr();
//test();
}
});
}
public void test(){
// First, make sure the eng.traineddata file exists.
/*assertTrue("Make sure that you've copied " + DEFAULT_LANGUAGE + ".traineddata to "
+ EXPECTED_FILE, new File(EXPECTED_FILE).exists());*/
final TessBaseAPI baseApi = new TessBaseAPI();
baseApi.init(TESSBASE_PATH, DEFAULT_LANGUAGE);
final Bitmap bmp = BitmapFactory.decodeResource(getResources(), R.drawable.test);
//digits is a .jpg image I found in one of the issues here.
ImageView img = (ImageView) findViewById(R.id.image);
img.setImageBitmap(bmp);//I can see the ImageView. So we know that it should work if I sent it to the setImage()
baseApi.setImage(bmp);
Log.v("Kishore","Kishore:Working");//This statement is never reached. Futhermore, on putting some more Log.v commands in the setImage function, I found out that the native function nativeSetImagePix is never accessed. I have attached the Logcat output below
 to show that it is not accessed.
String outputText = baseApi.getUTF8Text();
Log.v("Kishore","Kishore:"+outputText);
baseApi.end();
bmp.recycle();
}
protected void ocr() {
BitmapFactory.Options options = new BitmapFactory.Options();
options.inSampleSize = 2;
Bitmap bitmap = BitmapFactory.decodeFile(IMAGE_PATH, options);
try {
ExifInterface exif = new ExifInterface(IMAGE_PATH);
int exifOrientation = exif.getAttributeInt(ExifInterface.TAG_ORIENTATION, ExifInterface.ORIENTATION_NORMAL);
Log.v(TAG, "Orient: " + exifOrientation);
int rotate = 0;
switch (exifOrientation) {
case ExifInterface.ORIENTATION_ROTATE_90:
rotate = 90;
break;
case ExifInterface.ORIENTATION_ROTATE_180:
rotate = 180;
break;
case ExifInterface.ORIENTATION_ROTATE_270:
rotate = 270;
break;
}
Log.v(TAG, "Rotation: " + rotate);
if (rotate != 0) {
// Getting width & height of the given image.
int w = bitmap.getWidth();
int h = bitmap.getHeight();
// Setting pre rotate
Matrix mtx = new Matrix();
mtx.preRotate(rotate);
// Rotating Bitmap
bitmap = Bitmap.createBitmap(bitmap, 0, 0, w, h, mtx, false);
// tesseract req. ARGB_8888
bitmap = bitmap.copy(Bitmap.Config.ARGB_8888, true);
}
} catch (IOException e) {
Log.e(TAG, "Rotate or coversion failed: " + e.toString());
}
ImageView iv = (ImageView) findViewById(R.id.image);
iv.setImageBitmap(bitmap);
iv.setVisibility(View.VISIBLE);
Log.v(TAG, "Before baseApi");
TessBaseAPI baseApi = new TessBaseAPI();
baseApi.setDebug(true);
baseApi.init(TESSBASE_PATH, DEFAULT_LANGUAGE);
baseApi.setImage(bitmap);
String recognizedText = baseApi.getUTF8Text();
baseApi.end();
Log.v(TAG, "OCR Result: " + recognizedText);
// clean up and show
if (DEFAULT_LANGUAGE.equalsIgnoreCase("eng")) {
recognizedText = recognizedText.replaceAll("[^a-zA-Z0-9]+", " ");
}
if (recognizedText.length() != 0) {
((TextView) findViewById(R.id.field)).setText(recognizedText.trim());
}
}
private Handler mHandler = new Handler(){
public void handleMessage(android.os.Message msg) {
};
};
}
　　当你很欢喜的运行程序的时候，发现事情没有你想象的那么简单。这个文件必须要用到一个语言包。不然你怎么匹配呢？想想也是：
adb shell mkdir /mnt/sdcard/tesseract
adb shell mkdir /mnt/sdcard/tesseract/tessdata
adb push eng.traineddata /mnt/sdcard/tesseract/tessdata/eng.traineddata
adb shell ls -l /mnt/sdcard/tesseract/tessdata
ls -l bin/tesseract-android-tools-test.apk
adb install -r -s bin/tesseract-android-tools-test.apk
adb shell am instrument -w -e class com.googlecode.tesseract.android.test.TessBaseAPITest
　　上面的额eng.traineddata这个东西。你可以搜下，网络有的。（囧，我还不知到怎么上传附件）
　　最后效果如图（事实上解析结果是：44m><9。只不过那个字符不认识吧）：
![\](http://www.2cto.com/uploadfile/2012/0322/20120322030941515.png)
摘自 slider
