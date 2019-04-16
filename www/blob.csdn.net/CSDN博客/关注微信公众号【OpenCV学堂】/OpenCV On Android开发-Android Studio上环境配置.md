# OpenCV On Android开发 - Android Studio上环境配置 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2016年11月11日 10:19:25[gloomyfish](https://me.csdn.net/jia20003)阅读数：13900标签：[eclipse																[android																[opencv																[gradle																[图像处理](https://so.csdn.net/so/search/s.do?q=图像处理&t=blog)
个人分类：[OpenCV4Android教程](https://blog.csdn.net/jia20003/article/category/6514666)





## 在Android Studio 2.2上集成OpenCV4Android SDK

OpenCV官方的教程是基于Eclipse配置开发环境，但是Eclipse已经被Google抛弃了，所以我是写这篇文章的前三天刚刚开始用Android Studio 2.2版本，很多Gradle脚本也不熟悉，只能各种查找。经过一番痛苦的领悟终于把OpenCV4Android集成到我在Android Studio中创建的项目上了，并且写了测试程序。下面说一下如何实现集成步骤，首先是准备工作要做好：
- 下载好Android Studio 2.2版本
- 下载好OpenCV4AndroidSDK - 去OpenCV社区官网即可得到。
- 下载之后解压缩到D:\OpenCV-2.4.11-android-sdk\OpenCV-android-sdk 
![这里写图片描述](https://img-blog.csdn.net/20161111100023419)- 
apk目录里面放的是OpenCV Manager的安装文件，是不同CPU版本要选择不同apk安装文件，这种方式调用OpenCV比较麻烦。不推荐！

- 
doc目录里面放的是相关文档与OpenCV的LOGO

- 
samples里面放的是OpenCV4Android的演示代码，参考价值很大，值得关注

- 
sdk里面放内容就是我们要重点关注的，集成到Android Studio中的项目上去的东西。 

双击打开sdk文件夹就会看到： 
![这里写图片描述](https://img-blog.csdn.net/20161111100232968)
准备工作做好之后，首先就是要在Android Studio中创建一个Android项目，创建好之后，选择File->New->Import Module 
![这里写图片描述](https://img-blog.csdn.net/20161111100328205)

然后选择到SDK路径下的JAVA 
![这里写图片描述](https://img-blog.csdn.net/20161111100444019)

导入之后，你就会看到 
![这里写图片描述](https://img-blog.csdn.net/20161111100522969)

就说明成功导入了，然后打开Module Settings  
![这里写图片描述](https://img-blog.csdn.net/20161111100615037)
添加依赖之后，就可以在项目中引用OpenCV相关API代码了，如果你此刻运行测试apk程序，它就会提示你安装OpenCV Manager这个东西。对多数开发者来说这不算配置成功，这样自己的APP就无法独立安装，必须依赖OpenCV Manager这个apk文件才可以运行，这个时候就该放大招来解决这个问题，首先把我们准备阶段看到SDK下面native文件下所有的文件都copy到你创建好的项目libs目录下，然后在gradle中加上如下一段脚本： 
![这里写图片描述](https://img-blog.csdn.net/20161111100700173)
```
task nativeLibsToJar(type: Jar, description: 'create a jar archive of the native libs') {
    destinationDir file("$buildDir/native-libs")
    baseName 'native-libs'
    from fileTree(dir: 'libs', include: '**/*.so')
    into 'lib/'
}

tasks.withType(JavaCompile) {
    compileTask -> compileTask.dependsOn(nativeLibsToJar)
}
```

然后还要加上这句话：

```bash
compile fileTree(dir: "$buildDir/native-libs", include: 'native-libs.jar')
```

最后一步，检查一下gradle文件： 
![这里写图片描述](https://img-blog.csdn.net/20161111100939120)
如此配置之后你就再也不需要其它任何配置了，这样既避免了NDK繁琐又不用依赖OpenCV Manager第三方APP，你的APP就可以直接使用OpenCV了。
**特别说明：**
此配置方式OpenCV加载必须通过静态加载方式，以下为Demo测试程序代码：

```java
package com.example.administrator.helloworld;

import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.provider.MediaStore;
import android.support.annotation.NonNull;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.support.v7.view.ActionMode;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.Toast;

import org.opencv.android.BaseLoaderCallback;
import org.opencv.android.OpenCVLoader;
import org.opencv.android.Utils;
import org.opencv.core.Core;
import org.opencv.core.CvType;
import org.opencv.core.Mat;
import org.opencv.core.Size;
import org.opencv.imgproc.Imgproc;

import java.io.InputStream;

import static android.widget.Toast.makeText;

public class MainActivity extends AppCompatActivity {
    private double max_size = 1024;
    private int PICK_IMAGE_REQUEST = 1;
    private ImageView myImageView;
    private Bitmap selectbp;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        staticLoadCVLibraries();
        myImageView = (ImageView)findViewById(R.id.imageView);
        myImageView.setScaleType(ImageView.ScaleType.FIT_CENTER);
        Button selectImageBtn = (Button)findViewById(R.id.select_btn);
        selectImageBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                // makeText(MainActivity.this.getApplicationContext(), "start to browser image", Toast.LENGTH_SHORT).show();
                selectImage();
            }
        });

        Button processBtn = (Button)findViewById(R.id.process_btn);
        processBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                // makeText(MainActivity.this.getApplicationContext(), "hello, image process", Toast.LENGTH_SHORT).show();
                convertGray();
            }
        });

    }

    //OpenCV库静态加载并初始化
    private void staticLoadCVLibraries(){
        boolean load = OpenCVLoader.initDebug();
        if(load) {
            Log.i("CV", "Open CV Libraries loaded...");
        }
    }

    private void convertGray() {
        Mat src = new Mat();
        Mat temp = new Mat();
        Mat dst = new Mat();
        Utils.bitmapToMat(selectbp, src);
        Imgproc.cvtColor(src, temp, Imgproc.COLOR_BGRA2BGR);
        Log.i("CV", "image type:" + (temp.type() == CvType.CV_8UC3));
        Imgproc.cvtColor(temp, dst, Imgproc.COLOR_BGR2GRAY);
        Utils.matToBitmap(dst, selectbp);
        myImageView.setImageBitmap(selectbp);
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if(requestCode == PICK_IMAGE_REQUEST && resultCode == RESULT_OK && data != null && data.getData() != null) {
            Uri uri = data.getData();
            try {
                Log.d("image-tag", "start to decode selected image now...");
                InputStream input = getContentResolver().openInputStream(uri);
                BitmapFactory.Options options = new BitmapFactory.Options();
                options.inJustDecodeBounds = true;
                BitmapFactory.decodeStream(input, null, options);
                int raw_width = options.outWidth;
                int raw_height = options.outHeight;
                int max = Math.max(raw_width, raw_height);
                int newWidth = raw_width;
                int newHeight = raw_height;
                int inSampleSize = 1;
                if(max > max_size) {
                    newWidth = raw_width / 2;
                    newHeight = raw_height / 2;
                    while((newWidth/inSampleSize) > max_size || (newHeight/inSampleSize) > max_size) {
                        inSampleSize *=2;
                    }
                }

                options.inSampleSize = inSampleSize;
                options.inJustDecodeBounds = false;
                options.inPreferredConfig = Bitmap.Config.ARGB_8888;
                selectbp = BitmapFactory.decodeStream(getContentResolver().openInputStream(uri), null, options);

                myImageView.setImageBitmap(selectbp);

            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }

    private void selectImage() {
        Intent intent = new Intent();
        intent.setType("image/*");
        intent.setAction(Intent.ACTION_GET_CONTENT);
        startActivityForResult(Intent.createChooser(intent,"选择图像..."), PICK_IMAGE_REQUEST);
    }
}
```

选择一张图像加载之后显示： 
![这里写图片描述](https://img-blog.csdn.net/20161111101136466)
点击【处理】之后，通过调用OpenCV API实现了灰度转换 
![这里写图片描述](https://img-blog.csdn.net/20161111101223904)
**特别说明**：这种方式调用OpenCV无需NKD以及安装OpenCV Manager。应该是广大Android开发人员最喜欢的一种方式。 
**免费环境搭建视频查看**： 
[OpenCV For Android 基础入门视频](http://edu.51cto.com/course/course_id-8012.html)](https://so.csdn.net/so/search/s.do?q=gradle&t=blog)](https://so.csdn.net/so/search/s.do?q=opencv&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)](https://so.csdn.net/so/search/s.do?q=eclipse&t=blog)




