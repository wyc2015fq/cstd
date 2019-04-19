# 新版NDK环境搭建(免Cygwin，超级快) - xqhrs232的专栏 - CSDN博客
2015年11月11日 23:41:28[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：470
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://blog.csdn.net/alan00000/article/details/38398247](http://blog.csdn.net/alan00000/article/details/38398247)
相关文章
1、[Android NDK开发篇(一)：新版NDK环境搭建(免Cygwin，超级快)](http://blog.csdn.net/pocoyoshamoo/article/details/23533923)----[http://blog.csdn.net/pocoyoshamoo/article/details/23533923](http://blog.csdn.net/pocoyoshamoo/article/details/23533923)
2、[Android NDK开发篇(二)：NDK相关介绍](http://blog.csdn.net/pocoyoshamoo/article/details/23623493)----[http://blog.csdn.net/pocoyoshamoo/article/details/23623493](http://blog.csdn.net/pocoyoshamoo/article/details/23623493)
3、[Android NDK开发篇(三)：NDK开发步骤](http://blog.csdn.net/pocoyoshamoo/article/details/23706777)----[http://blog.csdn.net/pocoyoshamoo/article/details/23706777](http://blog.csdn.net/pocoyoshamoo/article/details/23706777)
4、[Android NDK开发篇(四)：Java与原生代码通信(原生方法声明与定义与数据类型)](http://blog.csdn.net/pocoyoshamoo/article/details/23926389)----[http://blog.csdn.net/pocoyoshamoo/article/details/23926389](http://blog.csdn.net/pocoyoshamoo/article/details/23926389)
5、[Android NDK开发篇(五)：Java与原生代码通信(数据操作)](http://blog.csdn.net/pocoyoshamoo/article/details/24299095)----[http://blog.csdn.net/pocoyoshamoo/article/details/24299095](http://blog.csdn.net/pocoyoshamoo/article/details/24299095)
6、[Android实战技巧之九：最新Android开发环境（Eclipse+ADT+Android 5.0）](http://blog.csdn.net/lincyang/article/details/42029257)----[http://blog.csdn.net/lincyang/article/details/42029257](http://blog.csdn.net/lincyang/article/details/42029257)
以前做Android的项目要用到NDK就必须要下载NDK，下载安装Cygwin(模拟Linux环境用的)，下载CDT（Eclipse C/C++开发插件），还要配置编译器，环境变量...
  麻烦到不想说了，Shamoo在网上查了一下资料，发现了一个超级快配置NDK的办法。
  Step1：到Android官网下载Android的开发工具ADT(Android Development Tool的缩写)，该工具集成了最新的ADT和NDK插件以及Eclipse，还有一个最新版本SDK。解压之后就可以用了，非常爽！
    ADT插件：管理Android SDK和相关的开发工具的
    NDK插件：用于开发Android NDK的插件，ADT版本在20以上，就能安装NDK插件，另外NDK集成了CDT插件
    也可以在线更新ADT、NDK插件，不过速度超级慢...所以果断在网上下载集成开发工具ADT，下载链接见：[http://developer.android.com/sdk/index.html](http://developer.android.com/sdk/index.html)
  Step2：到Android官网下载最新的NDK，注：NDK版本在r7以上之后就集成了Cygwin，而且还是十分精简版。比起下载Cygwin要方便多啦！下载链接见：[http://developer.android.com/tools/sdk/ndk/index.html](http://developer.android.com/tools/sdk/ndk/index.html)
    下载完成之后，解压搞定！
  Step3：打开Eclipse，点Window->Preferences->Android->NDK,设置NDK路径，例如Shamoo的是E:\android-ndk-r9c
![](https://img-blog.csdn.net/20140412153228156)
  Step4：新建一个Android工程，在工程上右键点击Android Tools->Add Native Support...,然后给我们的.so文件取个名字，例如:my-ndk
![](https://img-blog.csdn.net/20140412153922000)
  这时候工程就会多一个jni的文件夹，jni下有Android.mk和my-ndk.cpp文件。Android.mk是NDK工程的Makefile，my-ndk.cpp就是NDK的源文件。
  Step5：接下来仿着NDK的demo,Hello-JNI工程写一下。使用Alt + '/'可以代码提示！很爽！有木有？之前用CDT时候死活都按不出代码提示，郁闷...
![](https://img-blog.csdn.net/20140412154649187)
  JNI接口的命名规范是：Java_ + 调用该方法的包名(包名的点用_代替) + _ + 调用该接口的类名 + _ + 方法名，对于实例方法，有两个参数是必要的，一个JNI的环境指针JNIEnv *，另一个是调用该方法的Java实例jobject
  my-ndk.cpp:
**[cpp]**[view
 plain](http://blog.csdn.net/pocoyoshamoo/article/details/23533923#)[copy](http://blog.csdn.net/pocoyoshamoo/article/details/23533923#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/288453/fork)
- #include <jni.h> 
- 
- JNIEXPORT jstring JNICALL Java_com_shamoo_activity_TestActivity_stringFromJNI(JNIEnv *env,  
-         jobject thiz) {  
- return env->NewStringUTF("Hello jni");  
- }  
**[cpp]**[view
 plain](http://blog.csdn.net/alan00000/article/details/38398247#)[copy](http://blog.csdn.net/alan00000/article/details/38398247#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/288453/fork)
- #include <jni.h>
- 
- JNIEXPORT jstring JNICALL Java_com_shamoo_activity_TestActivity_stringFromJNI(JNIEnv *env,  
-         jobject thiz) {  
- return env->NewStringUTF("Hello jni");  
- }  
  TestActivity.java:
**[java]**[view
 plain](http://blog.csdn.net/pocoyoshamoo/article/details/23533923#)[copy](http://blog.csdn.net/pocoyoshamoo/article/details/23533923#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/288453/fork)
- publicclass TestActivity extends Activity {  
- 
- static {  
-         System.loadLibrary("my-ndk");  
-     }  
- 
- // 声明JNI层的原生方法，使用native关键字 
- publicnative String stringFromJNI();  
- 
- @Override
- protectedvoid onCreate(Bundle savedInstanceState) {  
- super.onCreate(savedInstanceState);  
-         TextView tvText = new TextView(this);  
-         tvText.setText(stringFromJNI());  
-         setContentView(tvText);  
-     }  
- 
- }  
**[java]**[view
 plain](http://blog.csdn.net/alan00000/article/details/38398247#)[copy](http://blog.csdn.net/alan00000/article/details/38398247#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/288453/fork)
- publicclass TestActivity extends Activity {  
- 
- static {  
-         System.loadLibrary("my-ndk");  
-     }  
- 
- // 声明JNI层的原生方法，使用native关键字
- publicnative String stringFromJNI();  
- 
- @Override
- protectedvoid onCreate(Bundle savedInstanceState) {  
- super.onCreate(savedInstanceState);  
-         TextView tvText = new TextView(this);  
-         tvText.setText(stringFromJNI());  
-         setContentView(tvText);  
-     }  
- 
- }  
  使用ndk库必须在static代码块里面用System.loadLaibrary加载.so库
  Step6：完成了，然后运行。运行之前先编译NDK，然后在编译JAVA代码。编译也许会遇到Unable to launch cygpath. Is Cygwin on the path?错误，解决办法如下：
    1.工程右键，点Properties->C/C++ Build的Building Settings中去掉Use default build command，然后输入${NDKROOT}/ndk-build.cmd
![](https://img-blog.csdn.net/20140412164805500)
    2.在C/C++ Build中点击Environment，点Add...添加环境变量NDKROOT，值为NDK的根目录
![](https://img-blog.csdn.net/20140412165011218)
    3.再编译，问题就解决啦！
  运行时崩溃，遇到java.lang.UnsatisfiedLinkError: stringFromJNI错误，解决办法：在C++文件中函数定义前添加extern "C"修饰
**[cpp]**[view
 plain](http://blog.csdn.net/pocoyoshamoo/article/details/23533923#)[copy](http://blog.csdn.net/pocoyoshamoo/article/details/23533923#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/288453/fork)
- extern"C" {  
- JNIEXPORT jstring JNICALL Java_com_shamoo_activity_TestActivity_stringFromJNI(JNIEnv *env,  
-         jobject thiz);  
- }  
- 
- JNIEXPORT jstring JNICALL Java_com_shamoo_activity_TestActivity_stringFromJNI(JNIEnv *env,  
-         jobject thiz) {  
- return env->NewStringUTF("Hello jni");  
- }  
**[cpp]**[view
 plain](http://blog.csdn.net/alan00000/article/details/38398247#)[copy](http://blog.csdn.net/alan00000/article/details/38398247#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/288453/fork)
- extern"C" {  
- JNIEXPORT jstring JNICALL Java_com_shamoo_activity_TestActivity_stringFromJNI(JNIEnv *env,  
-         jobject thiz);  
- }  
- 
- JNIEXPORT jstring JNICALL Java_com_shamoo_activity_TestActivity_stringFromJNI(JNIEnv *env,  
-         jobject thiz) {  
- return env->NewStringUTF("Hello jni");  
- }  
  原因是：使用extern "C"修饰，编译器会按C语言的方式编译和连接。在C语言中，函数编译之后函数名与C++函数编译之后不同，例如foo(int x, int y),C可能会编译成_foo的名字，而C++因为支持重载，所以会编译成像_foo_int_int这种带参数的函数名。如果是按照C语言的编译方式，调用foo函数是找不到_foo的函数名就会报出函数名找不到的错误。所以要添加extern
 "C"修饰。
原文：[http://blog.csdn.net/pocoyoshamoo/article/details/23533923](http://blog.csdn.net/pocoyoshamoo/article/details/23533923)
