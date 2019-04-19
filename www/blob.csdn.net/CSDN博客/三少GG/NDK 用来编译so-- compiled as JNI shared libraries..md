# NDK  用来编译so--> compiled as JNI shared libraries. - 三少GG - CSDN博客
2011年06月02日 21:38:00[三少GG](https://me.csdn.net/scut1135)阅读数：1516标签：[jni																[android																[eclipse																[module																[application																[windows](https://so.csdn.net/so/search/s.do?q=windows&t=blog)](https://so.csdn.net/so/search/s.do?q=application&t=blog)](https://so.csdn.net/so/search/s.do?q=module&t=blog)](https://so.csdn.net/so/search/s.do?q=eclipse&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)](https://so.csdn.net/so/search/s.do?q=jni&t=blog)
个人分类：[Android 多媒体](https://blog.csdn.net/scut1135/article/category/790993)
# [Android SDK Android NDK 官方下载地址](http://www.cnblogs.com/yaotong/archive/2011/01/25/1943615.html)
Android NDK r6
Android NDK r5b
Android NDK r5
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
- 在NDK的apps目录下创建helloworld目录，并且创建相关文件，目录结构如下： 
helloworld
–Application.mk
–project
   –jni
      –Android.mk
      –helloworld.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# [在Windows系统上安装与使用Android NDK r5](http://mobile.51cto.com/android-259890_1.htm)
二、JNI实例
1、安装成功后编译NDK中的一个例子命令行：
![](http://hi.csdn.net/attachment/201101/4/0_1294126980M9mL.gif)
编译完成后在E:/Android_NDK/android-ndk-r5/samples/hello-gl2文件夹中会生成一个文件夹libs，里面有一个子文件夹armeabi，里面生成libgl2jni.so文件。
2、一个简单的JNI例子
下面用一个简单的例子介绍怎么生成一个.so文件以及在Android项目中怎么调用.so文件。
①首先，用Eclipse建立一个ANDROID的工程，命名为MyFirstJNI,工程代码为：
②制作生成.so文件：可以通过获取以.h文件来获取函数申明。打开命令行，键入cd (这个路径是使用Eclipse建立的工程目录的src的路径，我的是F:/workspace/MyFirstJNI/src)，目的是设置工程目录。然后键入：javah -jni com.cn.lhq.JniTest,结果如果失败，那就是因为没有把MyFirst.class文件拷贝到F:/workspace/MyFirstJNI/src/com/cn/lhq之下，原因是Eclipse的文件管理机制会将.class文件和.java文件分开到不同的目录，解决方法是将F:/workspace/MyFirstJNI/bin/com/cn/lhq的所有.class文件都拷贝到他的目录下面。再次运行javah -jni com.cn.lhq.JniTest即可。然后就可以看到在F:/workspace/MyFirstJNI/src下多出了一个文件：com_cn_lhq_JniTest.h，通过这个文件，我们就知道了本地接口的函数名，于是我们就根据函数名写一个com_cn_lhq_JniTest.c文件， 代码如下：
注意：这个函数的参数与用javah生成出来的参数是不同的，这里只是需要函数名而已，因为函数名必须匹配，否则在模拟器上面跑的时候会出错，当然，如果对JNI命名规则熟悉的话可以跳过javah这步，直接写一个.c文件。
写好.c文件之后就开始生成.so文件了，这时候需要用到ANDROID.MK这个文件。于是把Eclipse生成的工程目录剪切，放到NDK目录的samples目录之下 （和示例hello-gl2一样），这个时候模仿hello-gl2文件层次结构，将MyFirstJNI文件也做成那样：在MyFirstJNI文件夹下面新建一个jni文件夹，把com_cn_lhq_JniTest.c剪切到此文件夹中，同时也拷贝一个ANDROID.MK文件到此文件夹，用UltraEdit打开ANDROID.MK文件进行修改，主要修改如下：
LOCAL_MODULE    := 工程名
LOCAL_SRC_FILES := .c的文件名（包含扩展名）
此例子应改为：
LOCAL_MODULE    := MyFirstJNI
LOCAL_SRC_FILES := com_cn_lhq_JniTest.c
然后打开cygwin， 输入cd
 $NDK (NDK为前面环境变量那设定的名字)
然后输入 make APP=MyFirstJNI
如果出现下面的错误说明你快要成功了：
$ make APP=MyFirst
Android NDK: Application MyFirst targets platform 'android-8'
Android NDK: which is not supported by this release of the Android NDK
build/core/add-application.mk:76: *** Android NDK: Aborting...    .  Stop.
因为你使用的是2.2版本的SDK所以检查的时候有问题，做法是：除了将jni文件目录保留，别的文件和目录都剪切到别的地方，然后再make APP=MyFirst
成功之后，再把这些前面的东西拷贝回来。
③在eclipse中重新导入生成了.so文件的MyFirstJNI项目，运行效果图如下：
![](http://hi.csdn.net/attachment/201101/4/0_129412721924w2.gif)
[view plain](#)[copy
 to clipboard](#)[print](#)[?](#)
- #include <string.h>
- #include <jni.h>
- jstring  
- Java_com_cn_lhq_JniTest_stringFromJNI( JNIEnv* env,  
- jobject thiz )  
- {  
- return (*env)->NewStringUTF(env, "Hello from JNI !");  
- }  
[view plain](#)[copy
 to clipboard](#)[print](#)[?](#)
- package com.cn.lhq;  
- import android.app.Activity;  
- import android.os.Bundle;  
- import android.widget.TextView;  
- publicclass JniTest extends Activity {  
- /** Called when the activity is first created. */
- @Override
- publicvoid onCreate(Bundle savedInstanceState) {  
- super.onCreate(savedInstanceState);  
- // setContentView(R.layout.main);
- TextView tv = new TextView(this);  
- tv.setText(stringFromJNI());  
- setContentView(tv);  
- }  
- publicnative String stringFromJNI();  
- static {  
- System.loadLibrary("MyFirstJNI");  
- }  
- } 
