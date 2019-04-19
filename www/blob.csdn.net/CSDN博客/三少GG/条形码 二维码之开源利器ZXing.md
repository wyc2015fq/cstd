# 条形码/二维码之开源利器ZXing - 三少GG - CSDN博客
2012年06月16日 12:32:44[三少GG](https://me.csdn.net/scut1135)阅读数：8032
## 条形码/二维码之开源利器ZXing图文介绍
作者:[Michael](http://www.micmiu.com.sixxs.org/author/michael/)日期:
 2012 年 3 月 30 日[发表评论](http://www.micmiu.com.sixxs.org/enterprise-app/java-zxing/#respond)(2)[查看评论](http://www.micmiu.com.sixxs.org/enterprise-app/java-zxing/#comments)
继前面介绍的一个日本开源软件（该软件只能实现QRCode）原文： [Java实现二维码QRCode的编码和解码](http://www.micmiu.com.sixxs.org/enterprise-app/java-qrcode/)(http://www.micmiu.com.sixxs.org/enterprise-app/java-qrcode/ )，今发现又一优秀的开源利器–
 ZXing，相比而言它更加灵活方便，可以实现多种编码格式。
全文目录：
- 
基本介绍
- 
二维码（比如：QRCode）的编码和解码演示
- 
条形码（比如：EAN-13）的编码和解码演示
【一】、 基本介绍 ：
1-1. ZXing是一个开源Java类库用于解析多种格式的条形码和二维码.
官网：[http://code.google.com.sixxs.org/p/zxing/](http://code.google.com.sixxs.org/p/zxing/)
截止目前为止最新版本为1.7，提供以下编码格式的支持：
- 
UPC-A and UPC-E
- 
EAN-8 and EAN-13
- 
Code 39
- 
Code 93
- 
Code 128
- 
QR Code
- 
ITF
- 
Codabar
- 
RSS-14 (all variants)
- 
Data Matrix
- 
PDF 417 (‘alpha’ quality)
- 
Aztec (‘alpha’ quality)
同时官网提供了 Android、cpp、C#、iPhone、j2me、j2se、jruby、objc、rim、symbian等多种应用的类库，具体详情可以参考下载的源码包中。
1-2. 本文和之前的那篇文章一样，主要是在PC上实现条形码（EAN-13）和二维码（QRCode） 的编码和解码的示例，以供大家参考，用到了源码中core和javase下面的相关源代码，附件提供自己编译之后的lib包：
- [zxing.jar](http://www.micmiu.com.sixxs.org/wp-content/uploads/2012/03/zxing-jar.zip)
- [zxing-j2se.jar](http://www.micmiu.com.sixxs.org/wp-content/uploads/2012/03/zxing-j2se-jar.zip)
有关各种手机系统的应用，有兴趣的朋友可以下载官方源码包，包下有具体详细的应用介绍。
【二】、 二维码（QRCode）的编码和解码演示：
2-1. 编码示例：
|`1`|`package``michael.zxing;`|
|`2`||
|`3`|`import``java.io.File;`|
|`4`|`import``java.util.Hashtable;`|
|`5`||
|`6`|`import``com.google.zxing.BarcodeFormat;`|
|`7`|`import``com.google.zxing.EncodeHintType;`|
|`8`|`import``com.google.zxing.MultiFormatWriter;`|
|`9`|`import``com.google.zxing.client.j2se.MatrixToImageWriter;`|
|`10`|`import``com.google.zxing.common.BitMatrix;`|
|`11`|`import``com.google.zxing.qrcode.decoder.ErrorCorrectionLevel;`|
|`12`||
|`13`|`/**`|
|`14`|` ````* @blog [http://www.micmiu.com.sixxs.org](http://www.micmiu.com.sixxs.org/)```|
|`15`|` ````* @author Michael```|
|`16`|` ``*/`|
|`17`|`public``class````ZxingEncoderHandler {```|
|`18`||
|`19`|`    ``/**`|
|`20`|`     ````* 编码```|
|`21`|`     ````* @param contents```|
|`22`|`     ````* @param width```|
|`23`|`     ````* @param height```|
|`24`|`     ````* @param imgPath```|
|`25`|`     ``*/`|
|`26`|`    ``public``void````encode(String contents,````int``width, ``int````height, String imgPath) {```|
|`27`|`        ````Hashtable<Object, Object> hints =````new````Hashtable<Object, Object>();```|
|`28`|`        ````// 指定纠错等级```|
|`29`|`        ````hints.put(EncodeHintType.ERROR_CORRECTION, ErrorCorrectionLevel.L);```|
|`30`|`        ````// 指定编码格式```|
|`31`|`        ``hints.put(EncodeHintType.CHARACTER_SET, ``"GBK"``);`|
|`32`|`        ``try``{`|
|`33`|`            ````BitMatrix bitMatrix =````new``MultiFormatWriter().encode(contents,`|
|`34`|`                    ````BarcodeFormat.QR_CODE, width, height, hints);```|
|`35`||
|`36`|`            ``MatrixToImageWriter`|
|`37`|`                    ``.writeToFile(bitMatrix, ``"png"``, ``new``File(imgPath));`|
|`38`||
|`39`|`        ``} ``catch````(Exception e) {```|
|`40`|`            ``e.printStackTrace();`|
|`41`|`        ``}`|
|`42`|`    ``}`|
|`43`||
|`44`|`    ``/**`|
|`45`|`     ````* @param args```|
|`46`|`     ``*/`|
|`47`|`    ``public``static``void````main(String[] args) {```|
|`48`|`        ````String imgPath =````"d:/test/twocode/michael_zxing.png"``;`|
|`49`|`        ````String contents =``````"Hello Michael(大大),welcome to Zxing!"```|
|`50`|`                ``+ ````"\nMichael’s blog [ [http://sjsky.iteye.com.sixxs.org](http://sjsky.iteye.com.sixxs.org/) ]"```|
|`51`|`                ``+ ````"\nEMail [ sjsky007@gmail.com ]"````+ ````"\nTwitter [ @suncto ]"````;`|
|`52`|`        ``int````width =````300````, height =````300``;`|
|`53`|`        ````ZxingEncoderHandler handler =````new``ZxingEncoderHandler();`|
|`54`|`        ````handler.encode(contents, width, height, imgPath);```|
|`55`||
|`56`|`        ``System.out.println(````"Michael ,you have finished zxing encode."````);`|
|`57`|`    ``}`|
|`58`|`}`|
运行后生成的二维码图片如下：
![](http://www.micmiu.com.sixxs.org/wp-content/uploads/2012/03/zxing-001.png)
和前篇介绍一样，用手机的二维码扫描软件（本人用的：android 快拍二维码 ）来测试下，识别成功的截图如下：
![](http://www.micmiu.com.sixxs.org/wp-content/uploads/2012/03/zxing-002.png)
2-2. 解码示例：
|`1`|`package``michael.zxing;`|
|`2`||
|`3`|`import``java.awt.image.BufferedImage;`|
|`4`|`import``java.io.File;`|
|`5`|`import``java.util.Hashtable;`|
|`6`||
|`7`|`import``javax.imageio.ImageIO;`|
|`8`||
|`9`|`import``com.google.zxing.BinaryBitmap;`|
|`10`|`import``com.google.zxing.DecodeHintType;`|
|`11`|`import``com.google.zxing.LuminanceSource;`|
|`12`|`import``com.google.zxing.MultiFormatReader;`|
|`13`|`import``com.google.zxing.Result;`|
|`14`|`import``com.google.zxing.client.j2se.BufferedImageLuminanceSource;`|
|`15`|`import``com.google.zxing.common.HybridBinarizer;`|
|`16`||
|`17`|`/**`|
|`18`|` ````* @blog [http://www.micmiu.com.sixxs.org](http://www.micmiu.com.sixxs.org/)```|
|`19`|` ````* @author Michael```|
|`20`|` ``*/`|
|`21`|`public``class````ZxingDecoderHandler {```|
|`22`||
|`23`|`    ``/**`|
|`24`|`     ````* @param imgPath```|
|`25`|`     ````* @return String```|
|`26`|`     ``*/`|
|`27`|`    ``public````String decode(String imgPath) {```|
|`28`|`        ````BufferedImage image =````null``;`|
|`29`|`        ````Result result =````null``;`|
|`30`|`        ``try``{`|
|`31`|`            ````image = ImageIO.read(````new``File(imgPath));`|
|`32`|`            ``if````(image ==````null````) {```|
|`33`|`                ``System.out.println(````"the decode image may be not exit."````);`|
|`34`|`            ``}`|
|`35`|`            ````LuminanceSource source =````new``BufferedImageLuminanceSource(image);`|
|`36`|`            ````BinaryBitmap bitmap =````new``BinaryBitmap(``new``HybridBinarizer(source));`|
|`37`||
|`38`|`            ````Hashtable<Object, Object> hints =````new````Hashtable<Object, Object>();```|
|`39`|`            ``hints.put(DecodeHintType.CHARACTER_SET, ``"GBK"``);`|
|`40`||
|`41`|`            ````result =````new````MultiFormatReader().decode(bitmap, hints);```|
|`42`|`            ``return``result.getText();`|
|`43`|`        ``} ``catch````(Exception e) {```|
|`44`|`            ``e.printStackTrace();`|
|`45`|`        ``}`|
|`46`|`        ``return``null``;`|
|`47`|`    ``}`|
|`48`||
|`49`|`    ``/**`|
|`50`|`     ````* @param args```|
|`51`|`     ``*/`|
|`52`|`    ``public``static``void````main(String[] args) {```|
|`53`|`        ````String imgPath =````"d:/test/twocode/michael_zxing.png"``;`|
|`54`|`        ````ZxingDecoderHandler handler =````new``ZxingDecoderHandler();`|
|`55`|`        ````String decodeContent = handler.decode(imgPath);```|
|`56`|`        ``System.out.println(``"解码内容如下："``);`|
|`57`|`        ``System.out.println(decodeContent);`|
|`58`|`        ``System.out.println(````"Michael ,you have finished zxing decode."````);`|
|`59`||
|`60`|`    ``}`|
|`61`|`}`|
运行结果如下：
> 
解码内容如下：
Hello Michael(大大),welcome to Zxing!
Michael’s blog [ http://sjsky.iteye.com.sixxs.org ]
EMail [ sjsky007@gmail.com ]
Twitter [ @suncto ]
Michael ,you have finished zxing decode.
从测试结果可见：解码出的内容和之前编码的内容是一致
【三】、 条形码（EAN-13）的编码和解码演示：
3-1. 编码示例：
|`1`|`package``michael.zxing;`|
|`2`||
|`3`|`import``java.io.File;`|
|`4`||
|`5`|`import``com.google.zxing.BarcodeFormat;`|
|`6`|`import``com.google.zxing.MultiFormatWriter;`|
|`7`|`import``com.google.zxing.client.j2se.MatrixToImageWriter;`|
|`8`|`import``com.google.zxing.common.BitMatrix;`|
|`9`||
|`10`|`/**`|
|`11`|` ````* @blog [http://www.micmiu.com.sixxs.org](http://www.micmiu.com.sixxs.org/)```|
|`12`|` ````* @author Michael```|
|`13`|` ``*/`|
|`14`|`public``class````ZxingEAN13EncoderHandler {```|
|`15`||
|`16`|`    ``/**`|
|`17`|`     ````* 编码```|
|`18`|`     ````* @param contents```|
|`19`|`     ````* @param width```|
|`20`|`     ````* @param height```|
|`21`|`     ````* @param imgPath```|
|`22`|`     ``*/`|
|`23`|`    ``public``void````encode(String contents,````int``width, ``int````height, String imgPath) {```|
|`24`|`        ``int````codeWidth =````3``+ ````// start guard```|
|`25`|`                ``(``7``* ``6````) +``````// left bars```|
|`26`|`                ``5``+ ````// middle guard```|
|`27`|`                ``(``7``* ``6````) +``````// right bars```|
|`28`|`                ``3``; ````// end guard```|
|`29`|`        ````codeWidth = Math.max(codeWidth, width);```|
|`30`|`        ``try``{`|
|`31`|`            ````BitMatrix bitMatrix =````new``MultiFormatWriter().encode(contents,`|
|`32`|`                    ````BarcodeFormat.EAN_13, codeWidth, height,````null``);`|
|`33`||
|`34`|`            ``MatrixToImageWriter`|
|`35`|`                    ``.writeToFile(bitMatrix, ``"png"``, ``new``File(imgPath));`|
|`36`||
|`37`|`        ``} ``catch````(Exception e) {```|
|`38`|`            ``e.printStackTrace();`|
|`39`|`        ``}`|
|`40`|`    ``}`|
|`41`||
|`42`|`    ``/**`|
|`43`|`     ````* @param args```|
|`44`|`     ``*/`|
|`45`|`    ``public``static``void````main(String[] args) {```|
|`46`|`        ````String imgPath =````"d:/test/twocode/zxing_EAN13.png"``;`|
|`47`|`        ````// 益达无糖口香糖的条形码```|
|`48`|`        ````String contents =````"6923450657713"``;`|
|`49`||
|`50`|`        ``int````width =````105````, height =````50``;`|
|`51`|`        ````ZxingEAN13EncoderHandler handler =````new``ZxingEAN13EncoderHandler();`|
|`52`|`        ````handler.encode(contents, width, height, imgPath);```|
|`53`||
|`54`|`        ``System.out.println(````"Michael ,you have finished zxing EAN13 encode."````);`|
|`55`|`    ``}`|
|`56`|`}`|
6 923450 657713对应的是益达无糖口香糖：
![](http://www.micmiu.com.sixxs.org/wp-content/uploads/2012/03/zxing-004.jpg)
运行后生成的条形码图片如下：
![](http://www.micmiu.com.sixxs.org/wp-content/uploads/2012/03/zxing-003.png)
用手机的扫描软件，识别成功的截图如下：
![](http://www.micmiu.com.sixxs.org/wp-content/uploads/2012/03/zxing-005.png)
3-2. 解码示例：
|`1`|`package``michael.zxing;`|
|`2`||
|`3`|`import``java.awt.image.BufferedImage;`|
|`4`|`import``java.io.File;`|
|`5`||
|`6`|`import``javax.imageio.ImageIO;`|
|`7`||
|`8`|`import``com.google.zxing.BinaryBitmap;`|
|`9`|`import``com.google.zxing.LuminanceSource;`|
|`10`|`import``com.google.zxing.MultiFormatReader;`|
|`11`|`import``com.google.zxing.Result;`|
|`12`|`import``com.google.zxing.client.j2se.BufferedImageLuminanceSource;`|
|`13`|`import``com.google.zxing.common.HybridBinarizer;`|
|`14`||
|`15`|`/**`|
|`16`|` ````* @blog [http://www.micmiu.com.sixxs.org](http://www.micmiu.com.sixxs.org/)```|
|`17`|` ````* @author Michael```|
|`18`|` ``*/`|
|`19`|`public``class````ZxingEAN13DecoderHandler {```|
|`20`||
|`21`|`    ``/**`|
|`22`|`     ````* @param imgPath```|
|`23`|`     ````* @return String```|
|`24`|`     ``*/`|
|`25`|`    ``public````String decode(String imgPath) {```|
|`26`|`        ````BufferedImage image =````null``;`|
|`27`|`        ````Result result =````null``;`|
|`28`|`        ``try``{`|
|`29`|`            ````image = ImageIO.read(````new``File(imgPath));`|
|`30`|`            ``if````(image ==````null````) {```|
|`31`|`                ``System.out.println(````"the decode image may be not exit."````);`|
|`32`|`            ``}`|
|`33`|`            ````LuminanceSource source =````new``BufferedImageLuminanceSource(image);`|
|`34`|`            ````BinaryBitmap bitmap =````new``BinaryBitmap(``new``HybridBinarizer(source));`|
|`35`||
|`36`|`            ````result =````new``MultiFormatReader().decode(bitmap, ``null``);`|
|`37`|`            ``return``result.getText();`|
|`38`|`        ``} ``catch````(Exception e) {```|
|`39`|`            ``e.printStackTrace();`|
|`40`|`        ``}`|
|`41`|`        ``return``null``;`|
|`42`|`    ``}`|
|`43`||
|`44`|`    ``/**`|
|`45`|`     ````* @param args```|
|`46`|`     ``*/`|
|`47`|`    ``public``static``void````main(String[] args) {```|
|`48`|`        ````String imgPath =````"d:/test/twocode/zxing_EAN13.png"``;`|
|`49`|`        ````ZxingEAN13DecoderHandler handler =````new``ZxingEAN13DecoderHandler();`|
|`50`|`        ````String decodeContent = handler.decode(imgPath);```|
|`51`|`        ``System.out.println(``"解码内容如下："``);`|
|`52`|`        ``System.out.println(decodeContent);`|
|`53`|`        ``System.out.println(````"Michael ,you have finished zxing EAN-13 decode."````);`|
|`54`||
|`55`|`    ``}`|
|`56`|`}`|
运行结果如下： 
> 
解码内容如下：
6923450657713
Michael ,you have finished zxing decode.
从测试结果可见：解码出的内容和之前编码的内容是一致。
**原创文章，转载请注明：**转载自[micmiu
 – 大大的技术 | 小小的生活](http://www.micmiu.com.sixxs.org/)[[http://www.micmiu.com.sixxs.org/](http://www.micmiu.com.sixxs.org/)]
**本文链接地址:**[http://www.micmiu.com.sixxs.org/enterprise-app/java-zxing/](http://www.micmiu.com.sixxs.org/enterprise-app/java-zxing/)
# [条码扫描二维码扫描——ZXing android 源码简化](http://www.cnblogs.com/keyindex/archive/2011/06/08/2074900.html)
# 前言
　　最近公司的Android项目需要用到摄像头做条码或二维码的扫描，Google一下，发现一个以[Apache
 License 2.0](http://www.apache.org/licenses/LICENSE-2.0) 开源的[ZXing](http://code.google.com/p/zxing/)项目。Zxing项目里的Android实现太过复杂多余东西太多，得对其进行简化。
## 前提条件
　　下载源代码：点击[这里](http://code.google.com/p/zxing/downloads/detail?name=ZXing-1.6.zip&can=2&q=)
　　编译核心库：Zxing的主页上有介绍具体步骤，大家也可以参照这篇博文：[android
 条码识别软件开发全解析(续2详解绝杀！)](http://www.cnblogs.com/tankaixiong/archive/2010/10/31/1865807.html)
# 导入项目
　　打开Eclipse 导入 源码中的 Android 项目，然后右击项目 选择“Build path”——》"Add External Archives" 把核心库 core.jar文件加入到项目中。
此时编译一下项目，会发现报错,“ Multiple substitutions specified in non-positional format; did you mean to add the formatted="false" attribute?”之类的。打开raw 下的Values
 发现错误是在一个<String>上。这里把 “preferences_custom_product_search_summary” 里的  %s  %f  全部都改成  %1$s  %1$f（因为我们用不到多国语言，建议只保留默认的Value ，其他全部删除）。
　　原因：由于新的SDK采用了新版本的aapt（Android项目编译器），这个版本的aapt编译起来会比老版本更加的严格，然后在Android最新的开发文档的描述String的部分，已经说明如何去设置 %s 等符号
“If you need to format your strings using String.format(String, Object...) , then you can do so by putting your format arguments in the string resource.
 For example, with the following resource:
　　<string name="welcome_messages">Hello, %1$s! You have %2$d new messages.</string>
　　In this example, the format string has two arguments: %1$s is a string and %2$d is a decimal number. You can format the string with arguements from
 your application...“
　　经过以上步骤后项目应该就可以运行了。
　　但是ZXing的android项目东西太多了，有很多是我们不需要的，得新建另一个项目简化它。
# 简化
　　在开始前大致介绍一下简化ZXing需要用到各个包 、类的职责。
- CaptureActivity。这个是启动Activity 也就是扫描器（如果是第一安装，它还会跳转到帮助界面）。
- CaptureActivityHandler 解码处理类，负责调用另外的线程进行解码。
- DecodeThread 解码的线程。
- com.google.zxing.client.android.camera 包，摄像头控制包。
- ViewfinderView 自定义的View，就是我们看见的拍摄时中间的框框了。
# 新建另一个项目
　　新建另一个项目将启动的Activity命名为CaptureActivity，并导入核心库。项目新建完成后我们打开 CaptureActivity 的布局文件，我这里为main。把里面的XML修改为：
```
[]()
1 <FrameLayout xmlns:android="http://schemas.android.com/apk/res/android"
2 android:layout_width="fill_parent" android:layout_height="fill_parent">
3 <SurfaceView android:id="@+id/preview_view"
4 android:layout_width="fill_parent" android:layout_height="fill_parent"
5 android:layout_centerInParent="true"/>
6 
7 <com.Zxing.Demo.view.ViewfinderView
8 android:id="@+id/viewfinder_view" android:layout_width="fill_parent"
9 android:layout_height="fill_parent" android:background="@android:color/transparent"/>
10 <TextView android:layout_width="wrap_content"
11 android:id="@+id/txtResult"
12 android:layout_height="wrap_content" android:text="@string/hello"/>
13 
14  </FrameLayout>
[]()
　　可以看到在XML里面用到了 ViewfinderView 自定义view 。所以新建一个View 的包，然后把：ViewfinderView 和 ViewfinderResultPointCallback 靠到里面（记得对应修改XML里面的包）。
打开 CaptureActivity 覆盖 onCreate 方法：
```
[]()
1 @Override
2 publicvoid onCreate(Bundle
 savedInstanceState) {
3 super.onCreate(savedInstanceState);
4 setContentView(R.layout.main);
5 //初始化
 CameraManager
6   CameraManager.init(getApplication());
7 
8 viewfinderView = (ViewfinderView)
 findViewById(R.id.viewfinder_view);
9 txtResult = (TextView)
 findViewById(R.id.txtResult);
10 hasSurface =false;
11 inactivityTimer =new InactivityTimer(this);
12 }
[]()
  这里调用到的 CameraManager 类是控制摄像头的包里的类。新建一个camera包把：com.google.zxing.client.android.camera 里面的类全部拷入，另外我把PlanarYUVLuminanceSource也拷入到这个包里面。根据错误的提示来修正代码，主要是修改正包结构。（整个简化的流程都是如此：“根据错误提示，修改代码”）。
![](http://pic002.cnblogs.com/images/2011/56457/2011060811041258.png)
　　在修改的过程中，有很多是关于R 资源的问题，在此我们需要将Values  里面的两个xml资源文件拷入项目中：colos.xml 和ids.xml 。 ctrl+b 一下看看error 是不是少了很多。在CameraManager中有些地方需要用到项目的配置，这里需要把配置直接写入代码中：
```
// SharedPreferences
 prefs = PreferenceManager.getDefaultSharedPreferences(context);
//是否使用前灯
// if (prefs.getBoolean(PreferencesActivity.KEY_FRONT_LIGHT,
 false)) {
// FlashlightManager.enableFlashlight();
// }
FlashlightManager.enableFlashlight();
```
 　　使用摄像头需要加入相应的权限：
```
<uses-permission
 android:name="android.permission.CAMERA"></uses-permission>
<uses-permission
 android:name="android.permission.WRITE_EXTERNAL_STORAGE"></uses-permission>
<uses-feature
 android:name="android.hardware.camera"/>
<uses-feature
 android:name="android.hardware.camera.autofocus"/>
<uses-permission
 android:name="android.permission.VIBRATE"/>
<uses-permission
 android:name="android.permission.FLASHLIGHT"/>
```
　　当View 和 camera 包里的错误修正完成后，我们继续来看CaptureActivity。
覆盖onResume方法初始化摄像头：
```
[]()
@Override
protectedvoid onResume()
 {
super.onResume();
SurfaceView surfaceView = (SurfaceView)
 findViewById(R.id.preview_view);
SurfaceHolder surfaceHolder = surfaceView.getHolder();
if (hasSurface)
 {
initCamera(surfaceHolder);
} else {
surfaceHolder.addCallback(this);
surfaceHolder.setType(SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS);
}
decodeFormats =null;
characterSet =null;
playBeep =true;
AudioManager audioService = (AudioManager)
 getSystemService(AUDIO_SERVICE);
if (audioService.getRingerMode() != AudioManager.RINGER_MODE_NORMAL)
 {
playBeep =false;
}
initBeepSound();
vibrate =true;
}
[]()
```
```
```
```
initCamera () 方法用于初始化摄像头，如果排除了所有的error ，运行项目时就可以看到大致扫描界面了。 surfaceHolder.addCallback(this);表示让CaptureActivity实现其callback接口。
handler = new CaptureActivityHandler(this, decodeFormats,characterSet) 用于进行扫描解码处理。
# 解码
　　上面的步骤主要都是用于对摄像头的控制，而解码的真正工作入口是在CaptureActivityHandler 里面的。新建一个Decoding包把以下文件拷入包中：
- CaptureActivityHandler
- DecodeFormatManager
- DecodeHandler
- DecodeThread
- FinishListener
- InactivityTimer
- Intents
由于我们的包结构和Zxing 项目的有所不同所以需要注意一下类的可访问性
同样开始ctrl+B 编译一下，然后开始修正错误。
　　在CaptureActivityHandler 里 把 handleMessage 里的部分方法先注释掉如：“decode_succeeded ”分支，这是解码成功时调用 CaptureActivity 展示解码的结果。
在DecodeThread 类里，修改部分涉及Preference配置的代码：
```
[]()
DecodeThread(CaptureActivity activity,
Vector<BarcodeFormat> decodeFormats,
String characterSet,
ResultPointCallback resultPointCallback) {
this.activity = activity;
handlerInitLatch =new CountDownLatch(1);
hints =new Hashtable<DecodeHintType,
 Object>(3);
//// The
 prefs can't change while the thread is running, so pick them up once here.
// if (decodeFormats
 == null || decodeFormats.isEmpty()) {
// SharedPreferences
 prefs = PreferenceManager.getDefaultSharedPreferences(activity);
// decodeFormats
 = new Vector<BarcodeFormat>();
// if (prefs.getBoolean(PreferencesActivity.KEY_DECODE_1D,
 true)) {
// decodeFormats.addAll(DecodeFormatManager.ONE_D_FORMATS);
// }
// if (prefs.getBoolean(PreferencesActivity.KEY_DECODE_QR,
 true)) {
// decodeFormats.addAll(DecodeFormatManager.QR_CODE_FORMATS);
// }
// if (prefs.getBoolean(PreferencesActivity.KEY_DECODE_DATA_MATRIX,
 true)) {
// decodeFormats.addAll(DecodeFormatManager.DATA_MATRIX_FORMATS);
// }
// }
if (decodeFormats ==null|| decodeFormats.isEmpty())
 {
decodeFormats =new Vector<BarcodeFormat>();
decodeFormats.addAll(DecodeFormatManager.ONE_D_FORMATS);
decodeFormats.addAll(DecodeFormatManager.QR_CODE_FORMATS);
decodeFormats.addAll(DecodeFormatManager.DATA_MATRIX_FORMATS);
}
hints.put(DecodeHintType.POSSIBLE_FORMATS, decodeFormats);
if (characterSet !=null)
 {
hints.put(DecodeHintType.CHARACTER_SET, characterSet);
}
hints.put(DecodeHintType.NEED_RESULT_POINT_CALLBACK, resultPointCallback);
}
[]()
这里是设置 解码的类型，我们现在默认将所有类型都加入。
错误类型基本上都是：包结构、PreferencesActivity 的配置 、类可访问性的问题。根据错误提示耐心把错误解决。
# 返回解码结果
 　　还记得在 CaptureActivityHandler 的 messagehandler 里注销掉的Case分支吗？现在CaptureActivity 里实现它。
```
[]()
publicvoid handleDecode(Result
 obj, Bitmap barcode) {
inactivityTimer.onActivity();
viewfinderView.drawResultBitmap(barcode);
playBeepSoundAndVibrate();
txtResult.setText(obj.getBarcodeFormat().toString() +":"
+ obj.getText());
}
[]()
# 最后
　　ZXing的简化已基本完成，有几位是可以运行成功的？呵呵。
下面是CaptureActivity的源码:
```
```
简化过的包结构图：
![](http://pic002.cnblogs.com/images/2011/56457/2011060813133926.gif)
　简化后的ZXing 更加方便我们了解ZXing项目 是如何解码的。只要仔细查看源码，进行单点跟踪调试，相信大家很容易能理解。
# 顾客是上帝
   很多人留言要源码， 其实我这不是什么源码，我只是把ZXing的东西简化了一下而已。事实上我也不喜欢直接放源码项目，这样大家就不想读ZXing的源码了。
下面是我简化的版本：[Zxing简化](http://files.cnblogs.com/keyindex/ZXingDemo.rar)
很多人需要Core 核心包（其实ZXing的源码里面就有），这里提供下我写文章时的版本 1.6的：
[Zxing](http://files.cnblogs.com/keyindex/zxing%E6%A0%B8%E5%BF%83%E5%8C%85.rar)
分类:[Android](http://www.cnblogs.com/keyindex/category/260729.html)
标签:[android](http://www.cnblogs.com/keyindex/tag/android/),[条码扫描](http://www.cnblogs.com/keyindex/tag/%E6%9D%A1%E7%A0%81%E6%89%AB%E6%8F%8F/),[二维码扫描](http://www.cnblogs.com/keyindex/tag/%E4%BA%8C%E7%BB%B4%E7%A0%81%E6%89%AB%E6%8F%8F/),[ZXing](http://www.cnblogs.com/keyindex/tag/ZXing/)
### [Google开源项目二维码读取与生成工具ZXing](http://blog.csdn.net/nickwar/article/details/5684134)
分类：[ZXing开源项目](http://blog.csdn.net/NickWar/article/category/701884)2010-06-21
 16:067878人阅读[评论](http://blog.csdn.net/nickwar/article/details/5684134#comments)(25)[收藏]()[举报](http://blog.csdn.net/nickwar/article/details/5684134#report)
上周五，闲逛Google Code的时候，查看了一下Google参与的开源项目，
在code.google.com上点击"开源计划"然后点击[使用 Google 的代码](http://code.google.com/hosting/projects.html)
即可查看Google所有的开源项目列表
翻了几页，发现一个zxing以前没听说过（孤陋寡闻了）
原来是个二维码的识别程序库，刚好前几个月还困惑火车票上的防伪码是怎么做的（才知道那种码叫QRcode），
于是把代码下载了下来，顺便说一下，这个库的示例数据是图片，所以体积较大，大概130M，
我用tortoise SVN， 由于网速太慢，下了三个小时，
顺便在网上也查了查相关资料，编译了java版本的试了一下
效果不错，可以使用，于是又把其.net版的工程编译了一下，是一个dll，debug版的212K
参照javase中的GUIRunner.java代码（很短的几句代码），在C#中实现了一个二维码的读取识别（QRCode）
参照javase中的MatrixToImageWriter.java（代码也很短），实现了二维码图片的生成（QRCode）
贴一下代码，这是识别的：
**[c-sharp]**[view
 plain](http://blog.csdn.net/nickwar/article/details/5684134#)[copy](http://blog.csdn.net/nickwar/article/details/5684134#)
- using System;  
- using System.Drawing;  
- using System.Windows.Forms;  
- using com.google.zxing;  
- using COMMON = com.google.zxing.common;  
- privatevoid button1_Click(object sender, EventArgs e)  
-         {  
- if (this.openFileDialog1.ShowDialog() != DialogResult.OK)  
-             {  
- return;  
-             }  
-             Image img = Image.FromFile(this.openFileDialog1.FileName);                          
-             Bitmap bmap;  
- try
-             {  
-                 bmap = new Bitmap(img);  
-             }  
- catch (System.IO.IOException ioe)  
-             {  
-                 MessageBox.Show(ioe.ToString());  
- return;  
-             }  
- if (bmap == null)  
-             {  
-                 MessageBox.Show("Could not decode image");  
- return;  
-             }  
-             LuminanceSource source = new RGBLuminanceSource(bmap, bmap.Width, bmap.Height);  
-             com.google.zxing.BinaryBitmap bitmap = new com.google.zxing.BinaryBitmap(new COMMON.HybridBinarizer(source));  
-             Result result;  
- try
-             {  
-                 result = new MultiFormatReader().decode(bitmap);  
-             }  
- catch(ReaderException re)  
-             {  
-                 MessageBox.Show(re.ToString());  
- return;  
-             }  
- 
-             MessageBox.Show(result.Text);          
-         }  

生成图片的代码：
**[c-sharp]**[view
 plain](http://blog.csdn.net/nickwar/article/details/5684134#)[copy](http://blog.csdn.net/nickwar/article/details/5684134#)
- privatevoid button2_Click(object sender, EventArgs e)  
- {  
-     SaveFileDialog sFD = new SaveFileDialog();  
-     sFD.DefaultExt = "*.png|*.png";  
-     sFD.AddExtension = true;              
- 
- try
-     {  
- if (sFD.ShowDialog() == DialogResult.OK)  
-         {  
- string content = "我的电话号码：110119112118;手机型号：Blackberry 8100";  
-             COMMON.ByteMatrix byteMatrix = new MultiFormatWriter().encode(content, BarcodeFormat.QR_CODE, 350, 350);  
-             writeToFile(byteMatrix, System.Drawing.Imaging.ImageFormat.Png, sFD.FileName);  
-         }  
-     }  
- catch (Exception ex)  
-     {  
-         MessageBox.Show(ex.Message);  
-     }  
- }  
- publicstaticvoid writeToFile(COMMON.ByteMatrix matrix, System.Drawing.Imaging.ImageFormat format, string file)  
- {  
-     Bitmap bmap = toBitmap(matrix);  
-     bmap.Save(file, format);  
- }  
- publicstatic Bitmap toBitmap(COMMON.ByteMatrix matrix)  
- {  
- int width = matrix.Width;  
- int height = matrix.Height;  
-     Bitmap bmap = new Bitmap(width, height, System.Drawing.Imaging.PixelFormat.Format32bppArgb);  
- for (int x = 0; x < width; x++)  
-     {  
- for (int y = 0; y < height; y++)  
-         {  
-             bmap.SetPixel(x, y, matrix.get_Renamed(x, y) != -1 ? ColorTranslator.FromHtml("0xFF000000") : ColorTranslator.FromHtml("0xFFFFFFFF"));  
-         }  
-     }  
- return bmap;              
- }  

源代码中有两处UTF-8的问题，会导致乱码，
其一：com.google.zxing.qrcode.encoder.encoder类中的
internal const System.String DEFAULT_BYTE_MODE_ENCODING = "ISO-8859-1";
此处，将ISO-8859-1改为UTF-8
其二：com.google.zxing.qrcode.decoder.DecodedBitStreamParser类的成员
private const System.String UTF8 = "UTF8";
应将UTF8改为UTF-8
手机像素，理论上30万的就可以，不过可能得有自动对焦的才行，
我的Blackberry 8100，130万的像素，拍出来的是无法识别，有rim的示例程序，
但是j2me的还没编译成功，所以还没办法尝试，提示找不到javax的一堆symbol（可能是wtk2.1版本有点旧）
生成的QRCode图：
![](http://hi.csdn.net/attachment/201006/21/4846_12771074187KDW.png)
这张图片里面的信息是本博客的地址，如下:
![qrCodeInfo](http://hi.csdn.net/attachment/201006/21/4846_1277107418xA99.png)
参考资料：
[Zxing调研](http://blessdyb.javaeye.com/blog/358235)
[使用ZXing进行二维码的生成](http://hi.baidu.com/baid/blog/item/fd446e06d199a77702088102.html)
### [zxing 源码解读日志 01](http://shappy1978.iteye.com/blog/1402219)
**博客分类：**- [iPhone](http://shappy1978.iteye.com/category/98431)
这两天看zxing的代码，只针对QRCode部分，首先还是对QRCode的版本和来源做一定的了解，广东发票的条码属于version10（57x57），解读和debug基本也是依靠这个条码来。
目标：了解解码原理，解读部分条码难度的原因，能否针对实际应用做一定的改进，比如处理前做一定的除噪，或者锐化，优化二值化的阙值设置，针对发票的红色边框和文字考虑去掉红色channel，避免打印错位造成无法处理。
QRCode的解码比较复杂，相信看过里面的数据分区就会觉得头皮发麻，特别是不同版本的读取区域又有所不同，所以基本上了解它的原理和读取方法即可。比较重要的还是识别定位点，三个定位点出来就可以获得条码的位置了，也可以做旋转并进一步处理。
首先对图像做了灰度化处理，处理位8bit的灰色图形，不过没有采用预料中的0.3r+0.59g+0.11b,而是直接绘制到content中转换，不知道效率和效果如何。
图形二值化看不出在哪里处理，貌似看到有一处居然是模糊化的函数，不接，而且这个函数确实被调用过。
1 zxing对于旋转一定角度的发票识别效果不好，是抓不到定位点还是其他原因呢？理论上来说旋转应该对抓取定位点影响不大。
2 条码定位点接触到任何神色的点必然影响效果，例如红色表格线，或者连线纸的孔，甚至版本区也会受影响，或者可以尝试取消红色channel，并取消对定位点最后一道线的检测要求，并根据这个形成的边界吧右边的边界清空为白色，避免影响数据读取（可能没有，确定矩形后应该不会读取矩形外的区域吧？）。
//************************************************************************************
* 消对定位点最后一道线的检测要求,修改后效果还可以
＊ 取消红色channel，效果待检测。。。
### [zxing 源码解读日志 02](http://shappy1978.iteye.com/blog/1404692)
**博客分类：**- [iPhone](http://shappy1978.iteye.com/category/98431)
zxing并不适合于单图像识别，或者作者本来就把它作为视频识别来处理的。因为预处理的时候会强制缩放原始图像到一定的大小，作者没有提供比较合适的图片检测接口。又或者你必需预先设定好剪裁的区域。否则识别效果很差，难道是缩放致？？？
iPhone拍摄出来的图片居然是有方向的，晕哦。处理的时候要注意。
//************************************************************************************
基本完成阅读：
1 定位点判断不需要修改
2 实际上红色channel看起来反而对红色表格线隐藏效果更好，但是这个和打印效果、光源有太大关系，测试了一下，只取红色channel对数据读取有影响，作用不大，放弃。
3 或者应该比对一下zbar做一些比较，粗略看了一下，zbar的代码会更难看，因为基本都堆在一个文件中做解码。
4 有一个想法就是在每一次扫描中都把bit matrix叠加在某个数组中，做一个经验积累数组，再以此读取数据，这个几乎要推翻整个结构重写了，可能堆3G／3GS的成像比较模糊的情况会有所改善吧。
Shappy Say
2012／02／13
分享到：![](http://shappy1978.iteye.com/images/sina.jpg)![](http://shappy1978.iteye.com/images/tec.jpg)
[[trans]iOS开发之多媒体播放](http://shappy1978.iteye.com/blog/1409660)|[zxing
 源码解读日志 01](http://shappy1978.iteye.com/blog/1402219)
- 
2012-02-13 17:14
- 
浏览 786
- [评论(14)](http://shappy1978.iteye.com/blog/1404692#comments)
- 
分类:[移动开发](http://www.iteye.com/blogs/category/mobile)
- [相关推荐](http://www.iteye.com/wiki/blog/1404692)
##### 评论
14 楼[ftp2010](http://ftp2010.iteye.com/)2012-04-20  
我测试过zbar
正确识别率不够高
13 楼[liuxing_iphone](http://liuxing-iphone.iteye.com/)2012-03-12  
好了，当时5.0的设备是touch，可能是分辨率的问题，我刚弄了个4s，测试了一下，一切正常
12 楼[smking](http://smking.iteye.com/)2012-03-02  
多谢楼主的回复。 我再研究研究
11 楼[shappy1978](http://shappy1978.iteye.com/)2012-03-01  
看起来你的静态库的编译方法有问题，估计是x86的，要编译真机用的一般是armv7,armv6，模拟器是i386,x86是mac系统用的都不同，编译的参数都不一样，看点怎么编译静态库的文章吧。
10 楼[shappy1978](http://shappy1978.iteye.com/)2012-03-01  
zbar我真的没有编译过，他自己提供的静态库是可以在真机上用的。具体怎么编译还是google一下吧，zbar的代码只是大概看了一下功能架构，这个没能力帮你，暂时这个上面没怎么花时间去看了。
9 楼[smking](http://smking.iteye.com/)2012-03-01  
继续请教楼主：
我是这样编译生成这个liazbar.a文件的，
1。 make distclean
2。 编译.a文件 ./configure --disable-video --without-qt --without-python
3。 sudo make install
然后就生成了新的.a文件, 在zbar文件夹下的libs下的libzbar.a， 然后加入到项目中来， 就出现了下面的错误：
ld: warning: ignoring file /Users/he/Desktop/GoodVa/libzbar.a, file was built for archive which is not the architecture being linked (armv7)
Undefined symbols for architecture armv7:
  "_OBJC_CLASS_$_ZBarReaderView", referenced from:
      objc-class-ref in M_ScanDecodeViewController.o
ld: symbol(s) not found for architecture armv7
clang: error: linker command failed with exit code 1 (use -v to see invocation)
1。 看错误提示像是说该文件不是在当armv7架构下生成的， 于是我换成模拟器， 还是提示该文件不是在i386架构下生成的。 那么这个文件到底是属于什么架构， 难道是Mac下的架构？
2。 如果我需要编译生成armv7下能使用的.a文件， 我应该怎么写./configure后面的参数呢？
楼主啊！ 拉一下受苦受难的人民脱离苦海吧， 把你知道的分享一下吧。 谢了。
8 楼[smking](http://smking.iteye.com/)2012-02-29  
楼主：
经过一段时间的折腾， 我已经把 ZBar编译好生成一个新的libzbar.a文件， 然后放在项目中进行使用，但在项目编译时，却报下面的错误， 还请楼主帮忙分析分析。
错误如下：（这是真机编译时报的错误）
ld: warning: ignoring file /Users/he/Desktop/GoodVa/libzbar.a, file was built for archive which is not the architecture being linked (armv7)
Undefined symbols for architecture armv7:
  "_OBJC_CLASS_$_ZBarReaderView", referenced from:
      objc-class-ref in M_ScanDecodeViewController.o
ld: symbol(s) not found for architecture armv7
clang: error: linker command failed with exit code 1 (use -v to see invocation)
在进行模拟器编译时报错和上面的一样， 只是armv7 变成了i386
我在想这个libzbar.a文件，在编译时也没有看到有什么命令啊， 麻烦楼主给指点一二。
不甚感激
![](http://shappy1978.iteye.com/images/smiles/icon_twisted.gif)![](http://shappy1978.iteye.com/images/smiles/icon_evil.gif)![](http://shappy1978.iteye.com/images/smiles/icon_evil.gif)![](http://shappy1978.iteye.com/images/smiles/icon_rolleyes.gif)![](http://shappy1978.iteye.com/images/smiles/icon_wink.gif)
7 楼[smking](http://smking.iteye.com/)2012-02-28  
楼主， 我把ZBar下载下来后， 编译始终不成功哇， 楼主能不能把编译ZBar的那个./Configure --disable XXX 啥的语句给帖出来。
另外， 我目前觉得ZBar在扫我上个回复中的图片时扫出乱码， 所以想去追踪ZBar的源码， 并修改之， 不知我的思路是否正确， 望楼主给予一些建议。
谢谢！
6 楼[smking](http://smking.iteye.com/)2012-02-27  
感谢楼主的回复
有两个疑问：
1. 对于ZXing扫一维码的问题， 随便拿一本书的封底的条形码， 用ZXing来扫， 发现扫不出来？
2. 用ZBar扫一些中文时出现乱码， 如：![](http://fmn.rrimg.com/fmn061/20120217/1840/original_VXGR_422100000398121b.jpg)
而用ZXing来扫这个图片， 就可以扫出来， 其中的信息是中文的“中”字
还望楼主帮忙分析分析是什么原因？ 有何良策
5 楼[shappy1978](http://shappy1978.iteye.com/)2012-02-25  
4.0和5.0都试过，这方面没有区别。提示你用debug模式查看扫描不出的原因
4 楼[shappy1978](http://shappy1978.iteye.com/)2012-02-25  
条码当然能表示中文了，无语。。。
zxing应该能扫一维码，但是一维码也有很多中格式，是否支持的格式比较少，这个没有实践过。据说zbar支持的格式多些，二维码比较流行的是QRCode，问题不大。
3 楼[smking](http://smking.iteye.com/)2012-02-23  
一起探讨一下ZBar和ZXing
2 楼[smking](http://smking.iteye.com/)2012-02-23  
![](http://shappy1978.iteye.com/images/smiles/icon_idea.gif)![](http://shappy1978.iteye.com/images/smiles/icon_idea.gif)![](http://shappy1978.iteye.com/images/smiles/icon_idea.gif)![](http://shappy1978.iteye.com/images/smiles/icon_idea.gif)![](http://shappy1978.iteye.com/images/smiles/icon_idea.gif)![](http://shappy1978.iteye.com/images/smiles/icon_idea.gif)![](http://shappy1978.iteye.com/images/smiles/icon_idea.gif)![](http://shappy1978.iteye.com/images/smiles/icon_idea.gif)
看到斑竹的这篇文章后， 专门申请了这个帐号来请教斑竹。（并且等了一天才能在这里发言， 唉，不容易啊， 希望楼主能看到我的回复）
我在网上也尝试找过ZBar和ZXing的源码， 但没有找到， 楼主能发一份给我不？ 多谢哈。 274019799@qq.com
(或者直接加我ＱＱ， 号码:274019799)
另外问一下楼主在博文中提到ZXing读取中文条码乱码的文章， 令我百思不得其解。
1。条码应该是条形码，条形码能表示中文吗？ 我的印象中好像都是些数字啥的。
2。据ZXing的官网上讲， ZXing应该是只能扫二维码，而不能扫一维码， 不知道是不是我哪里看错了， 还是需要进行什么样的设置
3。所以我猜测你可能是在扫二维码时，而这个二维码信息中含有中文才出现了这个问题， 所以请教一下什么样的二维码图片才会有乱码？ 方便的话贴一个或者发一个二维码图片到我邮箱里吧？
![](http://shappy1978.iteye.com/images/smiles/icon_smile.gif)![](http://shappy1978.iteye.com/images/smiles/icon_smile.gif)![](http://shappy1978.iteye.com/images/smiles/icon_smile.gif)![](http://shappy1978.iteye.com/images/smiles/icon_smile.gif)![](http://shappy1978.iteye.com/images/smiles/icon_smile.gif)![](http://shappy1978.iteye.com/images/smiles/icon_smile.gif)![](http://shappy1978.iteye.com/images/smiles/icon_smile.gif)![](http://shappy1978.iteye.com/images/smiles/icon_smile.gif)![](http://shappy1978.iteye.com/images/smiles/icon_smile.gif)![](http://shappy1978.iteye.com/images/smiles/icon_smile.gif)![](http://shappy1978.iteye.com/images/smiles/icon_smile.gif)![](http://shappy1978.iteye.com/images/smiles/icon_smile.gif)![](http://shappy1978.iteye.com/images/smiles/icon_smile.gif)![](http://shappy1978.iteye.com/images/smiles/icon_smile.gif)![](http://shappy1978.iteye.com/images/smiles/icon_smile.gif)![](http://shappy1978.iteye.com/images/smiles/icon_smile.gif)![](http://shappy1978.iteye.com/images/smiles/icon_smile.gif)![](http://shappy1978.iteye.com/images/smiles/icon_smile.gif)
看了博主的文章，真的是让人万分钦佩，期待你的回复。
1 楼[liuxing_iphone](http://liuxing-iphone.iteye.com/)2012-02-17  
斑竹，问个问题，我用那个zxing在4.0的设备上目前可以正常的扫描条形码，但是在5.0的上面就不行，zxing我也没看出个所以然来，该怎么解决呀，给个提示，最好能给点代码，谢谢啊
+++++++++++
要深入条码的识别的话自然就要去读zxing的源码，你读完之后就不只是懂得其源码了，
你会发现你其实收获到的不仅仅只有这点，还有更多的东西，比如你更深入学习android的又一个应用示例对Android的开发多了了解。
官网源码提供免费下载，但帮助文档不是很全，最近有在读它的源码，去调试运行它的源码对你去理解了解其结构是很有帮助的。
官网上只提供源码的下载，并没有提供编译好的开发包，也就是说你自己下过来后还要自己编译，不然把下下来的源码程序导入到myeclipse中会报错。
如图：
![](http://pic002.cnblogs.com/images/2010/142846/2010103122435038.jpg)
对于新手可能有些不知下手，这里我就讲下zxing的编译，为后面的开发做好一个最基本的准备！
有了核心包后我们可以就可以导入到项目工程里去了，现在是不是就不报错了！而且还可以直接运行的！
## 使用zxing生成QRCode二维码,在Android的webview中调用条形码扫描
作者：[lizongbo](http://618119.com.sixxs.org/)发表于：21:41. 星期二, 十一月 23rd, 2010
[版权声明](http://creativecommons.org.sixxs.org/licenses/by/3.0/deed.zh)：可以任意转载，转载时请务必以超链接形式标明文章[原始出处](http://618119.com.sixxs.org/archives/2010/11/23/194.html)和作者信息及本版权声明。
[http://618119.com.sixxs.org/archives/2010/11/23/194.html](http://618119.com.sixxs.org/archives/2010/11/23/194.html)
使用zxing生成QRCode二维码,在Android的webview中调用条形码扫描
zxing是一个条形码和二维码生成及识别的java开源组件，QRcode的中文名字是：QR快速响应矩阵码。
1.下载zxing最新的包
到zxing的主页：[http://code.google.com/p/zxing/](http://code.google.com.sixxs.org/p/zxing/)
下载[http://zxing.googlecode.com/files/ZXing-1.6.zip](http://zxing.googlecode.com.sixxs.org/files/ZXing-1.6.zip)
[http://zxing.googlecode.com/files/BarcodeScanner3.52.apk](http://zxing.googlecode.com.sixxs.org/files/BarcodeScanner3.52.apk)
来源：[http://code.google.com/p/zxing/downloads/list](http://code.google.com.sixxs.org/p/zxing/downloads/list)
其中BarcodeScanner3.5.2.apk是安装在Android操作系统的手机上的条码扫描器安装包。
条码扫描器的j2me版本下载地址为：
[http://zxing.org/w/BarcodeReader.jad](http://zxing.org.sixxs.org/w/BarcodeReader.jad)
[http://zxing.org/w/BarcodeReader.jar](http://zxing.org.sixxs.org/w/BarcodeReader.jar)
解压为：
D:\Java\[zxing](http://618119.com.sixxs.org/tag/zxing)-1.6
在dos窗口下进入到：D:\Java\zxing-1.6\core，然后运行ant命令进行编译
(必须先安装配置了ant。
ant最新版的下载地址为：[http://ant.apache.org/bindownload.cgi](http://ant.apache.org.sixxs.org/bindownload.cgi)
下载解压后在系统的环境变量中配置ANT_HOME：例如：ANT_HOME=D:\Java\apache-ant-1.7.0；
Path中必须有ant的bin目录，例如：
Path=D:\Java\jdk1.6.0_21\bin;D:\Java\apache-maven-2.0.9\bin;D:\Perl\site\bin;D:\
Perl\bin;D:\Java\apache-ant-1.7.0\bin;C:\WINDOWS\system32;C:\WINDOWS;C:\WINDOWS\
System32\Wbem;D:\Programs\svn-win32-1.5.4\bin;C:\Program Files\TortoiseSVN\bin;C
:\Program Files\QuickTime\QTSystem\
)
编译结果如下：
D:\Java\zxing-1.6\core>ant
Buildfile: build.xml
clean:
build:
init:
compile:
[mkdir] Created dir: D:\Java\zxing-1.6\core\build
[javac] Compiling 171 source files to D:\Java\zxing-1.6\core\build
[jar] Building jar: D:\Java\zxing-1.6\core\core.jar
BUILD SUCCESSFUL
Total time: 6 seconds
编译之后得到core.jar这个包。
再进入D:\Java\zxing-1.6\javase目录使用ant进行编译，编译结果如下：
D:\Java\zxing-1.6\javase>ant
Buildfile: build.xml
init:
build:
[mkdir] Created dir: D:\Java\zxing-1.6\javase\build
[javac] Compiling 6 source files to D:\Java\zxing-1.6\javase\build
[jar] Building jar: D:\Java\zxing-1.6\javase\javase.jar
BUILD SUCCESSFUL
Total time: 1 second
编译之后得到javase.jar这个包。
在Eclipse里新建Java工程：
编写java代码如下：
package com.lizongbo.[qrcode](http://618119.com.sixxs.org/tag/qrcode);
import java.awt.image.BufferedImage;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.Hashtable;
import javax.imageio.ImageIO;
import com.google.zxing.BarcodeFormat;
import com.google.zxing.EncodeHintType;
import com.google.zxing.MultiFormatWriter;
import com.google.zxing.WriterException;
import com.google.zxing.client.j2se.MatrixToImageWriter;
import com.google.zxing.common.BitMatrix;
public class QRCodeGen {
private static final int BLACK = 0xFF000000;
private static final int WHITE = 0xFFFFFFFF;
/**
* @param args
* @throws WriterException
* @throws IOException
*/
public static void main(String[] args) throws WriterException, IOException {
String picFormat = “png”;
StringBuilder sb = new StringBuilder(1024);
java.io.InputStreamReader fr = new InputStreamReader(
new FileInputStream(“d:\\quickli.vcf”), “GBK”);
BufferedReader br = new BufferedReader(fr);
String line = null;
while ((line = br.readLine()) != null) {
sb.append(line).append(“\r\n”);
}
String str = sb.toString();// 二维码内容
System.out.println(str.length() + “|str==” + str);
str = new String(str.getBytes(“GBK”), “ISO-8859-1″);
String path = “d:/lzb”;
Hashtable hints = new Hashtable();
// hints.put(EncodeHintType.CHARACTER_SET, “GBK”);
BitMatrix bitMatrix = new MultiFormatWriter().encode(str,
BarcodeFormat.QR_CODE, 400, 400, hints);
/**
* begin 这段代码等同于就是 MatrixToImageWriter.writeToFile(bitMatrix, picFormat,
* file);
* 直接这样写就不用引用javase.jar
*/
int width = bitMatrix.getWidth();
int height = bitMatrix.getHeight();
BufferedImage image = new BufferedImage(width, height,
BufferedImage.TYPE_INT_ARGB);
for (int x = 0; x < width; x++) {
for (int y = 0; y < height; y++) {
image.setRGB(x, y, bitMatrix.get(x, y) ? BLACK : WHITE);
}
}
File file = new File(path + “.” + picFormat);
ImageIO.write(image, picFormat, file);
/**
* end 这段代码等同于就是 MatrixToImageWriter.writeToFile(bitMatrix, picFormat,
* file);
*/
MatrixToImageWriter.writeToFile(bitMatrix, picFormat, file);
}
}
网址的QRcode生成如果不想编写代码，可以使用现成的google的api：
[http://chart.apis.google.com/chart?chs=150×150&cht=qr&chl=http](http://chart.apis.google.com.sixxs.org/chart?chs=150%C3%97150&cht=qr&chl=http)://zxing.googlecode.com/files/BarcodeScanner3.5.2.apk&chld=L|1&choe=UTF-8
Vcard也可以生成QRcode:
参考：[http://blog.yslifes.com/archives/686](http://blog.yslifes.com.sixxs.org/archives/686)
在线对二维码进行解码的url：[http://zxing.org/w/decode.jspx](http://zxing.org.sixxs.org/w/decode.jspx)
在Webview中通过js也可以调用条形码扫描，
封装的java代码如下：
final class BarCodeUtilHandler {
public void tryGetBarCode() {
Intent intent = new Intent(“com.google.zxing.client.[android](http://618119.com.sixxs.org/tag/android).SCAN”);
// intent.putExtra(“SCAN_MODE”, “QR_CODE_MODE”);
startActivityForResult(intent, 0);
}
};
@Override
public void onActivityResult(int requestCode, int resultCode, Intent intent) {
Log.v(TAG, “onActivityResult===” + requestCode + “,” + resultCode + “,”
+ intent);
if (requestCode == 0) {
if (resultCode == RESULT_OK) {
String contents = intent.getStringExtra(“SCAN_RESULT”);
String format = intent.getStringExtra(“SCAN_RESULT_FORMAT”);
webview.loadUrl(“javascript:showBarCode(\’” + format + “|”
+ contents + “\’)”);
} else if (resultCode == RESULT_CANCELED) {
webview.loadUrl(“javascript:showBarCode(\’没有找到条码!\’)”);
}
}
}
//webview.addJavascriptInterface(new BarCodeUtilHandler(), “barcodeutil”);
//javascript调用：window.barcodeutil.tryGetBarCode();
//function showBarCode(str) {
// try {
// document.getElementById(‘data’).value=str;
// } catch (ee) {
// }
//}
Tags:[Android](http://618119.com.sixxs.org/tag/android),[QRCode](http://618119.com.sixxs.org/tag/qrcode),[zxing](http://618119.com.sixxs.org/tag/zxing),[二维码](http://618119.com.sixxs.org/tag/%e4%ba%8c%e7%bb%b4%e7%a0%81),[条形码](http://618119.com.sixxs.org/tag/%e6%9d%a1%e5%bd%a2%e7%a0%81)
标签：[Android](http://618119.com.sixxs.org/tag/android),[QRCode](http://618119.com.sixxs.org/tag/qrcode),[zxing](http://618119.com.sixxs.org/tag/zxing),[二维码](http://618119.com.sixxs.org/tag/%e4%ba%8c%e7%bb%b4%e7%a0%81),[条形码](http://618119.com.sixxs.org/tag/%e6%9d%a1%e5%bd%a2%e7%a0%81)
或[分享到
 Google Buzz](http://www.google.com.sixxs.org/reader/link?url=http%3A%2F%2F618119.com%2Farchives%2F2010%2F11%2F23%2F194.html&title=%E4%BD%BF%E7%94%A8zxing%E7%94%9F%E6%88%90QRCode%E4%BA%8C%E7%BB%B4%E7%A0%81%2C%E5%9C%A8Android%E7%9A%84webview%E4%B8%AD%E8%B0%83%E7%94%A8%E6%9D%A1%E5%BD%A2%E7%A0%81%E6%89%AB%E6%8F%8F&srcURL=http%3A%2F%2F618119.com)
或![点此分享到QQ空间](http://qzonestyle.gtimg.cn.sixxs.org/ac/qzone_v5/app/app_share/btn_share.png)
或![点此分享到腾讯微博](http://v.t.qq.com.sixxs.org/share/images/s/b32.png)
#### 与《使用zxing生成QRCode二维码,在Android的webview中调用条形码扫描》相关的搜索：
- [如何在XtraReports中,使用二维码QRCode](http://www.baidu.com.sixxs.org/s?wd=%E5%A6%82%E4%BD%95%E5%9C%A8XtraReports%E4%B8%AD%2C%E4%BD%BF%E7%94%A8%E4%BA%8C%E7%BB%B4%E7%A0%81QRCode)
- [ZXing1.6 android 版本](http://www.baidu.com.sixxs.org/s?wd=ZXing1.6%20android%20%E7%89%88%E6%9C%AC)
- [android 做条形码生成](http://www.baidu.com.sixxs.org/s?wd=android%20%E5%81%9A%E6%9D%A1%E5%BD%A2%E7%A0%81%E7%94%9F%E6%88%90)
- [zxing 生成二维码](http://www.baidu.com.sixxs.org/s?wd=zxing%20%E7%94%9F%E6%88%90%E4%BA%8C%E7%BB%B4%E7%A0%81)
- [android Zxing二维码](http://www.baidu.com.sixxs.org/s?wd=android%20Zxing%E4%BA%8C%E7%BB%B4%E7%A0%81)
- [zxingQRcode](http://www.baidu.com.sixxs.org/s?wd=zxingQRcode)
- [zxing.qrcode](http://www.baidu.com.sixxs.org/s?wd=zxing.qrcode)
- [zxing
 生成二维码扫描问题解决](http://www.baidu.com.sixxs.org/s?wd=zxing+%E7%94%9F%E6%88%90%E4%BA%8C%E7%BB%B4%E7%A0%81%E6%89%AB%E6%8F%8F%E9%97%AE%E9%A2%98%E8%A7%A3%E5%86%B3)
这篇文章发布于 2010年11月23日，星期二，21:41，归类于[Android](http://618119.com.sixxs.org/category/android),[JavaScript](http://618119.com.sixxs.org/category/web/javascript-web),[前端开发](http://618119.com.sixxs.org/category/front-end-development)。
 您可以跟踪这篇文章的评论通过[RSS 2.0](http://618119.com.sixxs.org/archives/2010/11/23/194.html/feed)feed。 您可以[留下评论](http://618119.com.sixxs.org/archives/2010/11/23/194.html#respond)，或者从您的站点[trackback](http://618119.com.sixxs.org/archives/2010/11/23/194.html/trackback)。
```
```
```
```
```
