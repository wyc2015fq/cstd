# Android库so文件及skia函数的调用 - xqhrs232的专栏 - CSDN博客
2011年06月21日 16:10:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1351标签：[android																[tortoisesvn																[eclipse																[jni																[文档																[git](https://so.csdn.net/so/search/s.do?q=git&t=blog)](https://so.csdn.net/so/search/s.do?q=文档&t=blog)](https://so.csdn.net/so/search/s.do?q=jni&t=blog)](https://so.csdn.net/so/search/s.do?q=eclipse&t=blog)](https://so.csdn.net/so/search/s.do?q=tortoisesvn&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://www.wpf123.com/news/?6002.html](http://www.wpf123.com/news/?6002.html)
以图形库skia为例，具体过程如下：
1、建立完整的**android**开发环境，JAVA层的环境SDK、JDK、eclipse都要装，C&C++层的环境NDK、cygwin、git也都要装好。
2、下载完整的skia代码，代码自己到google上搜吧（重要提示：wiki，TortoiseSVN）
3、通过git下载android源码：xxxx/core，xxxx/base，xxxx/skia三个文件夹就够了。（找到android源码git文件的下载列表，搜索页面就可以了，都下载，都下载！！）
4、编译skia文件夹，获得skia对应的so文件。
4.1 如果你的skia文件夹来自skia官网，那就看看目录下的config文件，编写你的make.out文件，之后编译，出错，再编译，再出错。。。是因为缺少头文件，对吧？，这些头文件与硬件或图像编码有关，比如说atomic.h啊png.h啊什么的。找吧！！补上！！还是报错的话，就是因为还缺少某些定义，报错一大推“undeclared”，找不到就只能自己写，当然前提是你对你的开发板足够熟悉，比如skia要求mutex的申明，并实现这个自旋锁的占用和释放接口。(本步骤可以省略)
4.2 在ndk文件夹里搜索，获得libskia.so, libskiagl.so, libjnigraphics.so
5、新建项目文件夹，把第三步的core，base，skia三个文件夹扔进去。在项目文件夹里新建个lib文件夹，把第四步生成的so文件扔到lib里。新建jni文件夹，在jni里编写你的cpp文件。（重要提示：你可以不仔细看我写的东西，但如果你是在没有仔细看NDK自带doc文件夹里那十几篇英文参考的情况下这么做，那你一定会生不如死的）
6、搜索skia文件，找到skcanvas.h，看看我们能用skia干什么。和android相比，skia的自述文件俨然像是个天使。不理解什么是天使？请参看android NDK下的文档。
7、以画画写字为例，我的cpp文件如下：
- #include <jni.h>
- #include <time.h>
- #include <android/log.h>
- #include <android/bitmap.h>
- #include <pthread.h>
- #include <stdio.h>
- #include <stdlib.h>
- #include <math.h>
- #include "SkBitmap.h"  
- #include "SkDevice.h"  
- #include "SkPaint.h"  
- #include "SkRect.h"  
- #include "SkImageEncoder.h"  
- extern "C"  
- {  
- JNIEXPORT jint JNICALL Java_JNItestForGraph_JNItestForGraph_TestBackView_changeback  
-   (JNIEnv * env, jobject  obj, jobject bitmap, jint time);  
- }  
- JNIEXPORT jint JNICALL Java_JNItestForGraph_JNItestForGraph_TestBackView_changeback  
-     (JNIEnv * env, jobject  obj, jobject bitmap, jint time)  
- {  
-                  AndroidBitmapInfo  info;  
-              int*              pixels;  
-                 int ret;  
- ret = AndroidBitmap_getInfo(env, bitmap, &info);  
- ret = AndroidBitmap_lockPixels(env, bitmap, (void**)&pixels);  
-                 int width=(int)info.width;  
-                 int height=(int)info.height;  
- 
-                 SkBitmap bmp = SkBitmap();  
-                 bmp.setConfig(SkBitmap::kARGB_8888_Config,width,height,0);  
-                 bmp.setPixels((void*)pixels);  
-                 SkCanvas backgroud(bmp);  
-                 SkPaint paint;  
-                     paint.setARGB(255, 255, 0, 0);  
-                 SkRect r;  
-                 r.set(25, 25, 145, 145);  
-                 backgroud.drawRect(r, paint);  
-                 AndroidBitmap_unlockPixels(env, bitmap);  
-                 backgroud.drawRect(r, paint);  
-                 paint.setARGB(255, 0, 255, 0);  
-                     r.offset(20, 20);  
-                     backgroud.drawRect(r, paint);  
-                     paint.setARGB(255, 0, 0, 255);  
-                     r.offset(20, 20);  
-                     backgroud.drawRect(r, paint);  
-                 paint.setARGB(255, 0, 0, 0);  
-                 backgroud.drawText("ABCD", 4*sizeof(char), 0, 100 , paint);  
-                 return (ret);  
- } 
（重要提示：文件名的命名很有讲究，网上搜搜NDK native C或好好看看NDK自述文档HOWTO.TXT）
8、编写android.mk和appliction文件，具体怎么写，直接拷贝NDK下sample里的同名文件，再修改一下就可以了。具体怎么改。。。兄弟！不是我蒙你，还是看看NDK下的自述文档吧，我收回在第5步所说的话。用NDK文件夹下的ndk-build编译，根据出错信息关联所需头文件，都在你之前下载的几个文件夹里了，真的。编译成功，会在lib文件夹得到新的so文件。（重要提示：mk文件中记得带上-lskia，-lskiagl，-ljnigraphics）
9、编写JAVA程序调用上面的函数。该怎么写呢? 打开eclipse，新建工程，在程序中先Loadlibrary，再native C申明，具体。。。网上搜吧，要么就看看NDK下面sample里随便哪个例子，那个plasma就不错
10、运行ecplise里的项目，之前先看看有没有把so文件放到项目所在地的lib中。
11、下载到真机上运行，较低版本的机器，会缺少libjnipraphics.so库，需要在apk中添加。
![](http://images.51cto.com/files/uploadimg/20110602/0918340.jpg)
