# Android系统中读写和显示图片 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2011年12月26日 16:29:44[gloomyfish](https://me.csdn.net/jia20003)阅读数：7625








********
**1.    在画布上绘制(Draw with a Canvas) **


Android提供了自定义的绘制API，可以绘制各种图形，文字，图像对象在Canvas上，而事

实上所有在Canvas对想都会被绘制到Canvas底层的Bitmap对象上面，创建一个Canvas

对象的语句如下：

Bitmap b =Bitmap.createBitmap(100, 100, Bitmap.Config.ARGB_8888);

Canvas c =new Canvas(b);

首先创建一个32位的位图对象，类型是ARGB，然后在这个位图对象上创建一个画布（Canvas）

对象。



view.draw(c);

FileOutputStreamfos = new FileOutputStream (new File (APP_FILE_PATH +"/Canvastoimage.png"));

bitmap.compress(Bitmap.CompressFormat.JPEG,100, fos);

将View中内容输出到图像，压缩图像到指定格式JPG.其中fos为文件输出流。



在Android对象上显示Canvas的内容，要重载View的onDraw()方法，这点跟Java Swing

中要重写JComponent的paintComponent()方法有点类似，这样可以获取图形系统的绘制对

象，从而实现自定义对象的绘制。一个完整的重写View实现Canvas绘制显示的代码如下：



```java
package com.gloomyfish;

import android.app.Activity;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Paint.Style;
import android.os.Bundle;
import android.view.View;
import android.view.Window;

public class GraphicHelloActivity extends Activity {
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        setContentView(new Panel(this));
    }
 
    class Panel extends View {
        public Panel(Context context) {
            super(context);
        }
 
        @Override
        public void onDraw(Canvas canvas) {
            Bitmap _scratch = BitmapFactory.decodeResource(getResources(), R.drawable.flower_001);
            canvas.drawColor(Color.WHITE);
            canvas.drawBitmap(_scratch, 0, 0, null);
            Paint paint = new Paint();
            paint.setColor(Color.GREEN);
            paint.setStyle(Style.FILL);
            paint.setTextSize(16);
            canvas.drawText("Hello, Android Canvas", 20, 80, paint);
        }
    }
}
```


程序效果如下：

![](http://hi.csdn.net/attachment/201112/26/0_1324886469tZHY.gif)






**2.    使用可绘制对象(Drawables Object)**

Android中还提供一种相对简单和便捷的API用来读写和显示图像文件，就是以resource的形式加载图片对象，

使用ImageView API来对图像显示，如果有多个图像资源还可以使用Gallery来实现显示。大致的顺序为：

1.    将要显示的图片放到资源文件夹中

2.    定义资源ID对象数组

3.    在layout.xml中定义Gallery

4.    实现一个BaseAdapter

5.    在Activity中加载和指派对应的Adapter.

**程序效果如下：**
![](http://hi.csdn.net/attachment/201112/26/0_1324888161J0aK.gif)




**程序是我从android自带的Demo中修改来的，人懒啊！**程序源代码如下：

Activity的代码：



```java
package com.gloomyfish.gallery;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.widget.AdapterView;
import android.widget.Gallery;
import android.widget.Toast;
import android.widget.AdapterView.OnItemClickListener;

public class HellGalleryActivity extends Activity {
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);

        Gallery gallery = (Gallery) findViewById(R.id.gallery);
        gallery.setAdapter(new ImageAdapter(this));

        gallery.setOnItemClickListener(new OnItemClickListener() {
            public void onItemClick(AdapterView parent, View v, int position, long id) {
                Toast.makeText(HellGalleryActivity.this, "" + position, Toast.LENGTH_SHORT).show();
            }
        });
    }
}
```


ImageAdapter /ImageView接口代码：



```java
package com.gloomyfish.gallery;

import android.content.Context;
import android.content.res.TypedArray;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.Gallery;
import android.widget.ImageView;

public class ImageAdapter extends BaseAdapter {
    int mGalleryItemBackground;
    private Context mContext;

    private Integer[] mImageIds = {
    		R.drawable.sample_0,
            R.drawable.sample_1,
            R.drawable.sample_2,
            R.drawable.sample_3,
            R.drawable.sample_4,
            R.drawable.sample_5,
            R.drawable.sample_6,
            R.drawable.sample_7
    };

    public ImageAdapter(Context c) {
        mContext = c;
        TypedArray attr = mContext.obtainStyledAttributes(R.styleable.HelloGallery);
        mGalleryItemBackground = attr.getResourceId(
                R.styleable.HelloGallery_android_galleryItemBackground, 0);
        attr.recycle();
    }

    public int getCount() {
        return mImageIds.length;
    }

    public Object getItem(int position) {
        return position;
    }

    public long getItemId(int position) {
        return position;
    }

    public View getView(int position, View convertView, ViewGroup parent) {
        ImageView imageView = new ImageView(mContext);

        imageView.setImageResource(mImageIds[position]);
        imageView.setLayoutParams(new Gallery.LayoutParams(200, 200));
        imageView.setScaleType(ImageView.ScaleType.FIT_XY);
        imageView.setBackgroundResource(mGalleryItemBackground);

        return imageView;
    }
}
```


layout/main.xml的配置如下：



```java
<?xml version="1.0" encoding="utf-8"?>
<Gallery xmlns:android="http://schemas.android.com/apk/res/android" 
    android:id="@+id/gallery"
    android:layout_width="fill_parent"
    android:layout_height="wrap_content"
/>
```








