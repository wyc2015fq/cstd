# Android中使用OpenCV - 三少GG - CSDN博客
2012年07月26日 16:14:36[三少GG](https://me.csdn.net/scut1135)阅读数：1063
如何在Android中使用OpenCV
2011-09-21 10:22:35
标签：[Android](http://blog.51cto.com/tagindex.php?keyword=Android)[移动开发](http://blog.51cto.com/tagindex.php?keyword=%D2%C6%B6%AF%BF%AA%B7%A2)[JNI](http://blog.51cto.com/tagindex.php?keyword=JNI)[OpenCV](http://blog.51cto.com/tagindex.php?keyword=OpenCV)[NDK](http://blog.51cto.com/tagindex.php?keyword=NDK)
原创作品，允许转载，转载时请务必以超链接形式标明文章 [原始出处](http://underthehood.blog.51cto.com/2531780/670169) 、作者信息和本声明。否则将追究法律责任。[http://underthehood.blog.51cto.com/2531780/670169](http://underthehood.blog.51cto.com/2531780/670169)
看了网上的很多教程和官方[http://opencv.willowgarage.com/wiki/Android](http://opencv.willowgarage.com/wiki/Android)提供的如何在Android上使用OpenCV的教程，照着一步一步的做最后总有些问题，不是APK安装失败就是运行时突然报错退出。和同学一起摸索了一段时间后，终于弄成功，在这里做一个总结。最关键的问题是项目中各个文件夹和文件的位置要放置正确，而且目标机器的CPU架构要设置正确，下面是配置的详细过程。
一、Android开发环境
1.Sun JDK 6
访问[http://www.oracle.com/technetwork/java/javase/downloads/index.html](http://www.oracle.com/technetwork/java/javase/downloads/index.html)这里并且安装好JDK
注意：不要使用OpenJDK，Android SDK支持Sun JDK
2.Android SDK
访问[http://developer.android.com/sdk/index.html](http://developer.android.com/sdk/index.html)获取android
 sdk，如果选择的是Windows安装文件，则你还需要安装32bit JRE。
3.Android SDK组件
l Android SDK Tools, revision 12或者更新
l SDK平台Android 2.2, API 8, revision 2(also known as Java API)
这是OpenCV Java API支持的最低平台，OpenCV发布默认为Android 2.2
4. Eclipse IDE和ADT plugin for Eclipse
访问[http://www.eclipse.org/downloads/](http://www.eclipse.org/downloads/)下载Eclipse并解压即可。
打开Eclipse，选择Help->Install New Software菜单，但后点击Add按钮，在Add Repository对话框中的Name一栏输入"ADT Plugin"，Location一栏输入[https://dl-ssl.google.com/android/eclipse/](https://dl-ssl.google.com/android/eclipse/)，但后点击OK。在Available
 Software对话框中选中所有单选框，然后一路next直到finish为止，当安装ADT完毕后重启Eclipse即可。
5. Android NDK
访问[http://developer.android.com/sdk/ndk/index.html](http://developer.android.com/sdk/ndk/index.html) 下载最新的Android
 NDK，是一个ZIP解压包，只需解压到某个路径即可，例如"F:\android-ndk-r6b-windows\android-ndk-r6b"，再把这个路径添加到系统的环境变量PATH中。
6. Cygwin
访问[http://cygwin.com/index.html](http://cygwin.com/index.html)下载最新的Cygwin，最好安装全部的Cygwin组件。假设安装在"C:\cygwin"下，将"C:\cygwin\bin"添加到系统环境变量PATH中，为了方便的在命令行下调用Android
 NDK，找到"C:\cygwin\home\(你的用户名)"这个目录，打开文件".bash_profile"，在文件的最下面加上下面两行内容：
NDK=/cygdrive/f/android-ndk-r6b-windows/android-ndk-r6b
export NDK
这样便可以在命令行中以 "$NDK/ndk-build" 这种形式调用NDK了。
二、OpenCV
1.首先下载在[http://sourceforge.net/projects/opencvlibrary/files/opencv-android/2.3.1/](http://sourceforge.net/projects/opencvlibrary/files/opencv-android/2.3.1/) 已经预编译好的opencv包。
2.把下载好的包解压到某个路径上(最好不要带空格)，例如"F:\OpenCV-2.3.1-android-bin"
三、如何在Android程序中使用OpenCV
有两种方式（重点讲后面一种）：
1.使用OpenCV Java API。
OpenCV安装路径"F:\OpenCV-2.3.1-android-bin"下有两个文件夹，如下图
![wps_clip_image-10099](http://img1.51cto.com/attachment/201109/21/2531780_1316571750ix5N.png)
将文件夹"OpenCV-2.3.1"拷贝到你的Eclipse工作空间所在的目录，也就是在你的项目的上一级目录中，然后导入到工作空间中，在Package Explorer中选择你的项目，单机右键在弹出菜单中选择Properties，然后在弹出的Properties窗口中左侧选择Android，然后点击右下方的Add按钮，选择OpenCV-2.3.1并点击OK,如下图：
![wps_clip_image-17845](http://img1.51cto.com/attachment/201109/21/2531780_1316571750YMs7.png)
此时，展开你的项目树，你可以看到新加了一个OpenCV-2.3.1_src目录，如下图，那么就是正确添加了OpenCV Java API，否则就是你放置OpenCV-2.3.1的目录路径不正确。
![wps_clip_image-780](http://img1.51cto.com/attachment/201109/21/2531780_13165717513MEO.png)
然后就可以在你的Java源文件中导入OpenCV的API包，并且使用OpenCV API了，OpenCV API的包的形式如下：
Org.opencv.(OpenCV模块名).(OpenCV类名)
例如：
Org.opencv.core.Mat
2.利用JNI编写C++ OpenCV代码，通过Android NDK创建动态库（.so）
新建一个工作空间，例如"TestOpenCV"，在Window->Preferences中设置好Android SDK的路径，如下图所示。
![wps_clip_image-1955](http://img1.51cto.com/attachment/201109/21/2531780_1316571751sSad.png)
然后新建一个Android项目，Build Target选择Android2.2，命名为"HaveImgFun"，活动名改为HaveImgFun，Package name中填写com.testopencv.haveimgfun,最后点击finish。
如同使用OpenCV Java API那样，将OpenCV-2.3.1文件夹拷贝到与工作空间同一级目录中；另外，将"F:\OpenCV-2.3.1-android-bin\samples"下的includeOpenCV.mk文件拷贝到和项目HaveImgFun同一级目录中，如下图所示：
![wps_clip_image-30013](http://img1.51cto.com/attachment/201109/21/2531780_1316571752kE3I.png)
（上面这个各个文件夹和文件的放置很重要，因为OpenCV-2.3.1下的OpenCV.mk中有很多相对路径的指定，如果不是这样放置，在NDK生成动态库时可能会报文件或文件夹无法找到的错误）
选择Package Explorer中你的项目，右键选择new->folder，新建一个名为jni的文件夹，用来存放你的c/c++代码。
然后把res->layout下的main.xml的内容改为下面所示：
- <?xmlversion="1.0"encoding="utf-8"?>
- <LinearLayoutxmlns:android="http://schemas.android.com/apk/res/android"
- android:orientation="vertical"
- android:layout_width="fill_parent"
- android:layout_height="fill_parent"
- >
- <Buttonandroid:layout_height="wrap_content"
- android:layout_width="fill_parent"
- android:id="@+id/btnNDK"
- android:text="使用C++ OpenCV进行处理"/>
- <Buttonandroid:layout_height="wrap_content"
- android:layout_width="fill_parent"
- android:id="@+id/btnRestore"
- android:text="还原"/>
- <ImageViewandroid:id="@+id/ImageView01"
- android:layout_width="fill_parent"
- android:layout_height="fill_parent"/>
- 
- </LinearLayout>
上面的代码就是一个线性布局里面包含2个按钮加上一个显示图像的ImageView
在文件夹src下的com.testopencv.haveimgfun包中新建一个类用于包装使用了opencv c++代码的动态库的导出函数，类名为LibImgFun。
Eclipse会为你创建一个新的文件LibImgFun.java,将里面的内容改为：
- package com.testopencv.haveimgfun; 
- 
- publicclass LibImgFun {  
- 
- static {  
- 
-         System.loadLibrary("ImgFun");  
- 
-     }  
- 
- /**
- 
-     * @param width the current view width
- 
-     * @param height the current view height
- 
-     */
- 
- publicstaticnativeint[] ImgFun(int[] buf, int w, int h);  
- 
- }  
从上面的代码可以得知，我们的动态库名字应该为“libImgFun.so”，注意"public static native int[] ImgFun(int[] buf, int w, int h)"中的native关键字，表明这个函数来自native code。static表示这是一个静态函数，这样就可以直接用类名去调用。
在jni文件夹下建立一个"ImgFun.cpp"的文件，内容改为下面所示：
- #include <jni.h>
- 
- #include <stdio.h>
- 
- #include <stdlib.h>
- 
- #include <opencv2/opencv.hpp>
- 
- usingnamespace cv; 
- 
- extern"C" { 
- 
- JNIEXPORT jintArray JNICALL Java_com_testopencv_haveimgfun_LibImgFun_ImgFun( 
- 
-         JNIEnv* env, jobject obj, jintArray buf, int w, int h); 
- 
- JNIEXPORT jintArray JNICALL Java_com_testopencv_haveimgfun_LibImgFun_ImgFun( 
- 
-         JNIEnv* env, jobject obj, jintArray buf, int w, int h){ 
- 
- jint *cbuf; 
- 
- cbuf = env->GetIntArrayElements(buf, false); 
- 
- if(cbuf == NULL) 
- 
- { 
- 
- return 0; 
- 
- } 
- 
- Mat myimg(h, w, CV_8UC4, (unsigned char*)cbuf); 
- 
- for(int j=0;j<myimg.rows/2;j++) 
- 
- { 
- 
- myimg.row(j).setTo(Scalar(0,0,0,0)); 
- 
- } 
- 
- int size=w * h; 
- 
- jintArray result = env->NewIntArray(size); 
- 
- env->SetIntArrayRegion(result, 0, size, cbuf); 
- 
- env->ReleaseIntArrayElements(buf, cbuf, 0); 
- 
- return result; 
- 
- } 
- 
- } 
上面的代码中#include <jni.h>是必须要包含的头文件，#include <opencv2/opencv.hpp>是opencv要包含的头文件。
动态库要导出的函数如下声明：
JNIEXPORT jintArray JNICALL Java_com_testopencv_haveimgfun_LibImgFun_ImgFun(
        JNIEnv* env, jobject obj, jintArray buf, int w, int h);
JNIEXPORT 和JNICALL是必须要加的关键字
jintArray就是int[]，这里返回类型要么为空，要么为jni中定义的类型，事实上就是C\C++类型前面加上j，如果是数组，则在后面加上Array。
函数名的命名规则如下：
Java_(包路径)_（类名)_(函数名) (JNIEnv *env, jobject obj, 自己定义的参数...)
包路径中的"."用"_"(下划线)代替，类名就是上面包装该动态库函数的类的名字，最后一个才是真正的函数名；JNIEnv *env和jobject obj这两个参数时必须的，用来调用JNI环境下的一些函数；后面就是你自己定义的参数。在这里，jintArray buf代表了传进来的图像的数据，int w是图像的宽，int h是图像的高。
这个函数的功能是将传进来的图像的上半部分涂成黑色。
然后再在jni下新建两个文件"Android.mk"文件和"Application.mk"文件，这两个文件事实上就是简单的Makefile文件。
其中将Android.mk的内容改为如下所示：
- LOCAL_PATH := $(call my-dir) 
- 
- include $(CLEAR_VARS) 
- 
- include ../includeOpenCV.mk 
- 
- ifeq ("$(wildcard $(OPENCV_MK_PATH))","") 
- 
- #try toload OpenCV.mk fromdefault install location 
- 
- include $(TOOLCHAIN_PREBUILT_ROOT)/user/share/OpenCV/OpenCV.mk 
- 
- else
- 
- include $(OPENCV_MK_PATH) 
- 
- endif 
- 
- LOCAL_MODULE    := ImgFun 
- 
- LOCAL_SRC_FILES := ImgFun.cpp 
- 
- include $(BUILD_SHARED_LIBRARY) 
Application.mk的内容改为如下所示：
- APP_STL:=gnustl_static 
- 
- APP_CPPFLAGS:=-frtti -fexceptions 
- 
- APP_ABI:=armeabi armeabi-v7a 
其中APP_ABI指定的是目标平台的CPU架构。（经过很多测试,android2.2必须指定为armeabi，android2.2以上的使用armeabi-v7a，如果没有设置对，很有可能安装到android虚拟机失败，当然你同时如上面写上也是可以的）
上面的步骤完成后，就可以使用NDK生成动态库了，打开cygwin，cd到项目目录下，如下图所示：
![wps_clip_image-26301](http://img1.51cto.com/attachment/201109/21/2531780_13165717528JrY.png)
输入$NDK/ndk-build命令，开始创建动态库。成功的话如下图所示。
![wps_clip_image-7682](http://img1.51cto.com/attachment/201109/21/2531780_13165717538KGI.png)
这时候刷新Eclipse的Package Explorer会出现两个新的文件夹obj和libs。
现在，只剩最后一步完成这个测试程序。
将一张图片，例如"lena.jpg"放到项目res->drawable-hdpi目录中并刷新该目录。
然后将HaveImgFun.java的内容改为下面所示：
- package com.testopencv.haveimgfun; 
- 
- import android.app.Activity;  
- 
- import android.graphics.Bitmap;  
- 
- import android.graphics.Bitmap.Config;  
- 
- import android.graphics.drawable.BitmapDrawable;  
- 
- import android.os.Bundle;  
- 
- import android.widget.Button; 
- 
- import android.view.View;  
- 
- import android.widget.ImageView;  
- 
- publicclass HaveImgFun extends Activity { 
- 
- /** Called when the activity is first created. */
- 
- ImageView imgView; 
- 
- Button btnNDK, btnRestore; 
- 
- @Override
- 
- publicvoid onCreate(Bundle savedInstanceState) { 
- 
- super.onCreate(savedInstanceState);  
- 
-         setContentView(R.layout.main);  
- 
- this.setTitle("使用NDK转换灰度图");  
- 
- btnRestore=(Button)this.findViewById(R.id.btnRestore);  
- 
- btnRestore.setOnClickListener(new ClickEvent());  
- 
- btnNDK=(Button)this.findViewById(R.id.btnNDK);  
- 
- btnNDK.setOnClickListener(new ClickEvent());  
- 
- imgView=(ImageView)this.findViewById(R.id.ImageView01); 
- 
-         Bitmap img=((BitmapDrawable) getResources().getDrawable(R.drawable.lena)).getBitmap(); 
- 
- imgView.setImageBitmap(img); 
- 
-     } 
- 
- class ClickEvent implements View.OnClickListener{ 
- 
- publicvoid onClick(View v){ 
- 
- if(v == btnNDK)  
- 
-             {  
- 
- 
- long current=System.currentTimeMillis();  
- 
- Bitmap img1=((BitmapDrawable) getResources().getDrawable(R.drawable.lena)).getBitmap();  
- 
- int w=img1.getWidth(),h=img1.getHeight();  
- 
- int[] pix = newint[w * h];  
- 
- img1.getPixels(pix, 0, w, 0, 0, w, h);  
- 
- int[] resultInt=LibImgFun.ImgFun(pix, w, h);  
- 
- Bitmap resultImg=Bitmap.createBitmap(w, h, Config.RGB_565);  
- 
- resultImg.setPixels(resultInt, 0, w, 0, 0,w, h);  
- 
- long performance=System.currentTimeMillis()-current;  
- 
- imgView.setImageBitmap(resultImg);  
- 
- HaveImgFun.this.setTitle("w:"+String.valueOf(img1.getWidth())+",h:"+String.valueOf(img1.getHeight())  
- 
-                          +" NDK耗时 "+String.valueOf(performance)+" 毫秒");  
- 
-             }  
- 
- elseif(v == btnRestore) 
- 
- { 
- 
-         Bitmap img2=((BitmapDrawable) getResources().getDrawable(R.drawable.lena)).getBitmap();  
- 
- imgView.setImageBitmap(img2); 
- 
-         HaveImgFun.this.setTitle("使用OpenCV进行图像处理"); 
- 
- } 
- 
- } 
- 
-     } 
- 
- } 
点击全部保存，OK，现在可以选择一个Android虚拟机运行看一下效果，配置好Run Configuration然后点击Run，得到下面的结果：
![wps_clip_image-25816](http://img1.51cto.com/attachment/201109/21/2531780_1316571754IaFP.png)
点击使用C++ OpenCV进行处理，得到下面的结果：
![wps_clip_image-23188](http://img1.51cto.com/attachment/201109/21/2531780_1316571755JwrO.png)
本文出自 “[UnderTheHood](http://underthehood.blog.51cto.com/)”
 博客，请务必保留此出处[http://underthehood.blog.51cto.com/2531780/670169](http://underthehood.blog.51cto.com/2531780/670169)
