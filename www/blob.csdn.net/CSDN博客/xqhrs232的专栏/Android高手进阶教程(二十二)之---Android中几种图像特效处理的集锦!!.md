# Android高手进阶教程(二十二)之---Android中几种图像特效处理的集锦!! - xqhrs232的专栏 - CSDN博客
2010年12月07日 22:50:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：573标签：[android																[matrix																[layout																[float																[output																[shader](https://so.csdn.net/so/search/s.do?q=shader&t=blog)](https://so.csdn.net/so/search/s.do?q=output&t=blog)](https://so.csdn.net/so/search/s.do?q=float&t=blog)](https://so.csdn.net/so/search/s.do?q=layout&t=blog)](https://so.csdn.net/so/search/s.do?q=matrix&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://blog.csdn.net/Android_Tutor/archive/2010/11/02/5981753.aspx](http://blog.csdn.net/Android_Tutor/archive/2010/11/02/5981753.aspx)
大家好,这一节给大家分享的是Android中几种图像特效处理的小技巧,比如圆角,倒影,还有就是图片缩放,Drawable转化为Bitmap,Bitmap转化为Drawable等等.
废话少说了,直接讲解今天的实例,本例主要是先获取壁纸(getWallpaper()),然后对当前壁纸的一些特效处理.大家按步骤一步一步来:
第一步:新建一个Android工程命名为ImageDemo,工程结构如下:
第二步:新建一个.java文件,命名为ImageUtil.java,在里面定义一些图片处理方法,代码如下:
package com.android.tutor;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.LinearGradient;
import android.graphics.Matrix;
import android.graphics.Paint;
import android.graphics.PixelFormat;
import android.graphics.PorterDuffXfermode;
import android.graphics.Rect;
import android.graphics.RectF;
import android.graphics.Bitmap.Config;
import android.graphics.PorterDuff.Mode;
import android.graphics.Shader.TileMode;
import android.graphics.drawable.Drawable;
public class ImageUtil {
 //放大缩小图片
 public static Bitmap zoomBitmap(Bitmap bitmap,int w,int h){
  int width = bitmap.getWidth();
  int height = bitmap.getHeight();
  Matrix matrix = new Matrix();
  float scaleWidht = ((float)w / width);
     float scaleHeight = ((float)h / height);
     matrix.postScale(scaleWidht, scaleHeight);
     Bitmap newbmp = Bitmap.createBitmap(bitmap, 0, 0, width, height, matrix, true);
  return newbmp;
 }
 //将Drawable转化为Bitmap
  public static Bitmap drawableToBitmap(Drawable drawable){
      int width = drawable.getIntrinsicWidth();
      int height = drawable.getIntrinsicHeight();
      Bitmap bitmap = Bitmap.createBitmap(width, height,
        drawable.getOpacity() != PixelFormat.OPAQUE ? Bitmap.Config.ARGB_8888
                         : Bitmap.Config.RGB_565);
      Canvas canvas = new Canvas(bitmap);
      drawable.setBounds(0,0,width,height);
      drawable.draw(canvas);
      return bitmap;
     }
  //获得圆角图片的方法
 public static Bitmap getRoundedCornerBitmap(Bitmap bitmap,float roundPx){
  Bitmap output = Bitmap.createBitmap(bitmap.getWidth(), bitmap
    .getHeight(), Config.ARGB_8888);
  Canvas canvas = new Canvas(output);
  final int color = 0xff424242;
  final Paint paint = new Paint();
  final Rect rect = new Rect(0, 0, bitmap.getWidth(), bitmap.getHeight());
  final RectF rectF = new RectF(rect);
  paint.setAntiAlias(true);
  canvas.drawARGB(0, 0, 0, 0);
  paint.setColor(color);
  canvas.drawRoundRect(rectF, roundPx, roundPx, paint);
  paint.setXfermode(new PorterDuffXfermode(Mode.SRC_IN));
  canvas.drawBitmap(bitmap, rect, rect, paint);
  return output;
 }
 //获得带倒影的图片方法
 public static Bitmap createReflectionImageWithOrigin(Bitmap bitmap){
  final int reflectionGap = 4;
  int width = bitmap.getWidth();
  int height = bitmap.getHeight();
  Matrix matrix = new Matrix();
  matrix.preScale(1, -1);
  Bitmap reflectionImage = Bitmap.createBitmap(bitmap, 
    0, height/2, width, height/2, matrix, false);
  Bitmap bitmapWithReflection = Bitmap.createBitmap(width, (height + height/2), Config.ARGB_8888);
  Canvas canvas = new Canvas(bitmapWithReflection);
  canvas.drawBitmap(bitmap, 0, 0, null);
  Paint deafalutPaint = new Paint();
  canvas.drawRect(0, height,width,height + reflectionGap,
    deafalutPaint);
  canvas.drawBitmap(reflectionImage, 0, height + reflectionGap, null);
  Paint paint = new Paint();
  LinearGradient shader = new LinearGradient(0,
    bitmap.getHeight(), 0, bitmapWithReflection.getHeight()
    + reflectionGap, 0x70ffffff, 0x00ffffff, TileMode.CLAMP);
  paint.setShader(shader);
  // Set the Transfer mode to be porter duff and destination in
  paint.setXfermode(new PorterDuffXfermode(Mode.DST_IN));
  // Draw a rectangle using the paint with our linear gradient
  canvas.drawRect(0, height, width, bitmapWithReflection.getHeight()
    + reflectionGap, paint);
  return bitmapWithReflection;
 }
}
第三步:修改main.xml布局文件,主要放了两个ImageView控件,代码如下:
<?xml version="1.0" encoding="utf-8"?>
<LinearLayout xmlns:android="[http://schemas.android.com/apk/res/android](http://schemas.android.com/apk/res/android)"
    android:orientation="vertical"
    android:layout_width="fill_parent"
    android:layout_height="fill_parent"
    >
 <ImageView 
  android:id="@+id/image01" 
     android:layout_width="wrap_content" 
     android:layout_height="wrap_content" 
     android:padding="10px"
     />
 <ImageView
  android:id="@+id/image02"
  android:layout_width="wrap_content" 
     android:layout_height="wrap_content" 
     android:padding="10px"
 />
</LinearLayout>
第四步:修改主核心程序,ImageDemo.java,代码如下:
package com.android.tutor;
import android.app.Activity;
import android.graphics.Bitmap;
import android.graphics.drawable.Drawable;
import android.os.Bundle;
import android.widget.ImageView;
public class Imagedemo extends Activity {
 private ImageView mImageView01,mImageView02;
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        setupViews();
    }
    private void setupViews(){
     mImageView01 = (ImageView)findViewById(R.id.image01);
     mImageView02 = (ImageView)findViewById(R.id.image02);
     //获取壁纸返回值是Drawable
     Drawable drawable = getWallpaper();
     //将Drawable转化为Bitmap
     Bitmap bitmap = ImageUtil.drawableToBitmap(drawable);
     //缩放图片
     Bitmap zoomBitmap = ImageUtil.zoomBitmap(bitmap, 100, 100);
     //获取圆角图片
     Bitmap roundBitmap = ImageUtil.getRoundedCornerBitmap(zoomBitmap, 10.0f);
     //获取倒影图片
     Bitmap reflectBitmap = ImageUtil.createReflectionImageWithOrigin(zoomBitmap);
     //这里可以让Bitmap再转化为Drawable
//     Drawable roundDrawable = new BitmapDrawable(roundBitmap);     
//     Drawable reflectDrawable = new BitmapDrawable(reflectBitmap);    
//     mImageView01.setBackgroundDrawable(roundDrawable);
//     mImageView02.setBackgroundDrawable(reflectDrawable);
     mImageView01.setImageBitmap(roundBitmap);
     mImageView02.setImageBitmap(reflectBitmap);
    }
} 
第五步:运行上述工程,查看效果如下:
OK大功告成了!!
本文参考文献:http://wiki.impjq.net/doku.php?id=code:android-code:image-convert&rev=1275640889&mddo=print
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/Android_Tutor/archive/2010/11/02/5981753.aspx](http://blog.csdn.net/Android_Tutor/archive/2010/11/02/5981753.aspx)

