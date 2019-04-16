# 安卓开发31：异步加载图片框架ImageLoader的使用 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2013年08月27日 20:08:11[initphp](https://me.csdn.net/initphp)阅读数：12358
所属专栏：[安卓开发](https://blog.csdn.net/column/details/androidgo.html)









### 需要下载ImageLoader.jar

下载地址：http://code.google.com/p/libs-for-android/wiki/ImageLoader




### 使用：

MainActivity.java



```java
package com.example.test.com;

import java.io.File;

import android.app.Activity;
import android.os.Bundle;
import android.view.ViewGroup;
import android.view.Window;
import android.widget.ImageView;

import com.nostra13.universalimageloader.cache.disc.impl.UnlimitedDiscCache;
import com.nostra13.universalimageloader.cache.disc.naming.HashCodeFileNameGenerator;
import com.nostra13.universalimageloader.cache.memory.impl.LruMemoryCache;
import com.nostra13.universalimageloader.core.DisplayImageOptions;
import com.nostra13.universalimageloader.core.ImageLoader;
import com.nostra13.universalimageloader.core.ImageLoaderConfiguration;
import com.nostra13.universalimageloader.core.assist.QueueProcessingType;
import com.nostra13.universalimageloader.core.decode.BaseImageDecoder;
import com.nostra13.universalimageloader.core.download.BaseImageDownloader;
import com.nostra13.universalimageloader.utils.StorageUtils;


public class MainActivity extends Activity {
    
    private ImageLoader imageLoader = ImageLoader.getInstance();


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        requestWindowFeature(Window.FEATURE_NO_TITLE);

        ViewGroup viewGroup = (ViewGroup) getLayoutInflater().inflate(R.layout.main, null);
        setContentView(viewGroup);
        getLayoutInflater();
        //ImageView对象用于显示图片
        ImageView imageView = (ImageView) findViewById(R.id.imageView1);
        loadImage(imageView);

    }
    
    private void loadImage(final ImageView imageView) {
        //图片保存系统路径
        File cacheDir = StorageUtils.getOwnCacheDirectory(MainActivity.this, "data");
        DisplayImageOptions imageOptions = new DisplayImageOptions.Builder().cacheInMemory().cacheOnDisc()
                .build();
        //一些相关的设置
        ImageLoaderConfiguration config = new ImageLoaderConfiguration.Builder(MainActivity.this).threadPoolSize(5)
                .threadPriority(Thread.NORM_PRIORITY - 1).tasksProcessingOrder(QueueProcessingType.LIFO).denyCacheImageMultipleSizesInMemory()
                .memoryCache(new LruMemoryCache(1024 * 1024)).memoryCacheSize(1 * 1024 * 1024).discCache(new UnlimitedDiscCache(cacheDir)) // default
                .discCacheSize(20 * 1024 * 1024).discCacheFileCount(400).discCacheFileNameGenerator(new HashCodeFileNameGenerator()) // default
                .imageDownloader(new BaseImageDownloader(MainActivity.this)) // default
                .imageDecoder(new BaseImageDecoder()) // default
                .defaultDisplayImageOptions(imageOptions) // default
                .enableLogging().build();
        //初始化配置
        imageLoader.init(config);
        //显示图片
        imageLoader.displayImage("http://www.baidu.com/img/bdlogo.gif", imageView);
    }

}
```





