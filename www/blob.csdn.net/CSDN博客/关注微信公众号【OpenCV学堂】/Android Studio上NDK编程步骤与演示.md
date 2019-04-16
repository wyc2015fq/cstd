# Android Studio上NDK编程步骤与演示 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2017年05月18日 17:02:50[gloomyfish](https://me.csdn.net/jia20003)阅读数：2744








# Android Studio上NDK编程步骤与演示

在AndroidStudio（AS）上搞NDK编程首先要下载与安装NDK，搞好了这步之后。只需要以下几步配置与操作就可以轻松开始NDK编程与运行。

### 新建一个纯Android项目（不包含C++支持）

在新项目中创建一个新Java文件为BitmapProcessor.java, 定义两个本地方法，代码实现如下：

```java
package com.gloomyfish.ndkdemo;

import android.graphics.Bitmap;

/**
 * Created by jia20003 on 2017/5/18.
 */

public class BitmapProcessor {

    static {
        System.loadLibrary("BitmapProcessor");
    }

    public native void gray(Bitmap bm);

    public native void inverse(Bitmap bm);

}
```

### 编译含有本地方法的Java文件和产生.h的C++文件

首先新建一个bat脚本文件javahrun.bat然后写入如下内容

```
set JAVA_HOME="C:\Program Files\Java\jdk1.8.0_92"
set path=%JAVA_HOME%\bin;%path%
set classpath=.;%classpath%;%JAVA_HOME%\lib;C:\Users\Administrator\AppData\Local\Android\sdk\platforms\android-24\android.jar
javah com.gloomyfish.ndkdemo.BitmapProcessor
```

首先用javac BitmapProcessor.java这句替换脚本文件中的最后一句，然后保存该文件到项目所在的目录下app\src\main\java\com\gloomyfish\ndkdemo位置之后，通过cmd命令行执行javahrun.bat即可。

成功执行的话会生成一个.class文件在相同目录下面 
![这里写图片描述](https://img-blog.csdn.net/20170518163822903?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhMjAwMDM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
然后退到目录app\src\main\java\下面，同时把bat文件修改为之前的脚本，直接执行就会生成.h的头文件，在app\src\main目录下新建jni与jniLIB两个目录文件夹之后，把.h头文件copy到jni文件夹中，这个时候在AS看到的目录结构应该如下： 
![这里写图片描述](https://img-blog.csdn.net/20170518164042938?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhMjAwMDM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
新建一个C++源文件命名为BitmapProcessor.cpp, 实现头文件中定义与声明的两个本地方法即可，注意这个可以在VS2015中完成。设置一下头文件即可，如果你不知道该include多少个或者哪个，直接把目录%disk_dir%Android\android-ndk-r13b\platforms\android-24\arch-arm\usr\include下的全部添加到VS2015的include配置中即可。这样写完程序，确保没有C++的语法错误，然后copy到AS中。

### 编译含有C++的NDK项目

首先要设置一下NDK的路径, 右键项目打开Module Settings 
![这里写图片描述](https://img-blog.csdn.net/20170518165007959?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhMjAwMDM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
然后设置好NDK路径即可。
其次，在app对应的build.gradle文件添加如下一段脚本 
![这里写图片描述](https://img-blog.csdn.net/20170518165118350?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhMjAwMDM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
最后在gradle.properties文件中添加上一句话 

android.useDeprecatedNdk=true 

如果不加的话，编译就会又错误出现。
然后【build】->【clean build】， 【rebuild】 

就编译好啦！

### 使用so库与编写java相关代码

首先把编译生成的app\build\intermediates\ndk\debug\lib下面的文件全部copy到之前建好的jniLIB里面即可。然后完成Java部分代码编写，实现用户交互与UI响应操作。 

这里我通过JNI实现了在C++层对Bitmap对象的灰度化操作与颜色取反操作然后返回结果。运行效果如下： 
![这里写图片描述](https://img-blog.csdn.net/20170518165548349?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhMjAwMDM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170518165600586?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhMjAwMDM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
### 相关Java与C++代码实现

MainActivity.java

```java
package com.gloomyfish.ndkdemo;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;

public class MainActivity extends AppCompatActivity implements View.OnClickListener{

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Button grayBtn = (Button)this.findViewById(R.id.gray_btn);
        Button invertBtn = (Button)this.findViewById(R.id.invert_btn);
        grayBtn.setOnClickListener(this);
        invertBtn.setOnClickListener(this);
    }

    @Override
    public void onClick(View v) {
        int id = v.getId();
        switch (id) {
            case R.id.gray_btn:
                convert2Gray();
                break;
            case R.id.invert_btn:
                invertImage();
                break;
            default:
                break;
        }
    }

    private void invertImage() {
        Bitmap bm = BitmapFactory.decodeResource(this.getResources(), R.drawable.test);
        BitmapProcessor processor = new BitmapProcessor();
        processor.inverse(bm);
        ImageView iv = (ImageView)this.findViewById(R.id.imageView_001);
        iv.setImageBitmap(bm);
    }

    private void convert2Gray() {
        Bitmap bm = BitmapFactory.decodeResource(this.getResources(), R.drawable.test);
        BitmapProcessor processor = new BitmapProcessor();
        processor.gray(bm);
        ImageView iv = (ImageView)this.findViewById(R.id.imageView_001);
        iv.setImageBitmap(bm);
    }
}
```

BitmapProcessor.cpp

```
//
// Created by gloomy fish on 2017/5/18.
//
#include<android/bitmap.h>
#include<android/log.h>
#include<com_gloomyfish_ndkdemo_BitmapProcessor.h>

#ifndef eprintf
#define eprintf(...) __android_log_print(ANDROID_LOG_ERROR,"@",__VA_ARGS__)
#endif

#define RGBA_A(p) (((p) & 0xFF000000) >> 24)
#define RGBA_R(p) (((p) & 0x00FF0000) >> 16)
#define RGBA_G(p) (((p) & 0x0000FF00) >>  8)
#define RGBA_B(p)  ((p) & 0x000000FF)
#define MAKE_RGBA(r,g,b,a) (((a) << 24) | ((r) << 16) | ((g) << 8) | (b))

JNIEXPORT void JNICALL Java_com_gloomyfish_ndkdemo_BitmapProcessor_gray
  (JNIEnv *env, jobject clazz, jobject bmpObj) {
    AndroidBitmapInfo bmpInfo={0};
    if(AndroidBitmap_getInfo(env,bmpObj,&bmpInfo)<0)
    {
        eprintf("invalid bitmap\n");
        return;
    }

    void * dataFromBmp = NULL;
    int res = AndroidBitmap_lockPixels(env, bmpObj, &dataFromBmp);
    if(dataFromBmp == NULL)
    {
        eprintf("could not retrieve pixels from bitmap\n");
        return;
    }
    eprintf("Effect: %dx%d, %d\n", bmpInfo.width, bmpInfo.height, bmpInfo.format);
    int x = 0;
    int y = 0;
    int width = bmpInfo.width;
    int height = bmpInfo.height;
    for(y=0; y<height; y++) {
        for(x=0; x<width; x++) {
            int a = 0, r = 0, g = 0, b = 0;
            void *pixel = NULL;
            pixel = ((uint32_t *)dataFromBmp) + y * width + x;
            uint32_t v = *(uint32_t *)pixel;
            a = RGBA_A(v);
            r = RGBA_R(v);
            g = RGBA_G(v);
            b = RGBA_B(v);

            // Grayscale
            int gray = (r * 38 + g * 75 + b * 15) >> 7;

            // Write the pixel back
            *((uint32_t *)pixel) = MAKE_RGBA(gray, gray, gray, a);
        }
    }

    AndroidBitmap_unlockPixels(env, bmpObj);
}

JNIEXPORT void JNICALL Java_com_gloomyfish_ndkdemo_BitmapProcessor_inverse
    (JNIEnv *env, jobject clazz, jobject bmpObj) {
AndroidBitmapInfo bmpInfo={0};
    if(AndroidBitmap_getInfo(env,bmpObj,&bmpInfo)<0)
    {
        eprintf("invalid bitmap\n");
        return;
    }

    void * dataFromBmp = NULL;
    int res = AndroidBitmap_lockPixels(env, bmpObj, &dataFromBmp);
    if(dataFromBmp == NULL)
    {
        eprintf("could not retrieve pixels from bitmap\n");
        return;
    }
    eprintf("Effect: %dx%d, %d\n", bmpInfo.width, bmpInfo.height, bmpInfo.format);
    int x = 0;
    int y = 0;
    int width = bmpInfo.width;
    int height = bmpInfo.height;
    for(y=0; y<height; y++) {
        for(x=0; x<width; x++) {
            int a = 0, r = 0, g = 0, b = 0;
            void *pixel = NULL;
            pixel = ((uint32_t *)dataFromBmp) + y * width + x;
            uint32_t v = *(uint32_t *)pixel;
            a = RGBA_A(v);
            r = RGBA_R(v);
            g = RGBA_G(v);
            b = RGBA_B(v);

            // 取反操作
            r = 255 - r;
            g = 255 - g;
            b = 255 - b;

            // Write the pixel back
            *((uint32_t *)pixel) = MAKE_RGBA(r, g, b, a);
        }
    }

    AndroidBitmap_unlockPixels(env, bmpObj);
}
```

PS： 
**我之所有把图像处理相关的都整到C++这一层来，是因为Android Studio 上用Java干这个事情真TMD太慢了，唯一C++才是唯一出路，想做AR或者图像处理与视频分析必须要学会此技能！**









