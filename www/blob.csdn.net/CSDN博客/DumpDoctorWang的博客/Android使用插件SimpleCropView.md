# Android使用插件SimpleCropView - DumpDoctorWang的博客 - CSDN博客





2017年08月29日 13:20:25[原我归来是少年](https://me.csdn.net/DumpDoctorWang)阅读数：650








# 一、引用列表--站在巨人的肩膀上
- [SimpleCropView](https://github.com/IsseiAoki/SimpleCropView) https://github.com/IsseiAoki/SimpleCropView
- 
# [android开发游记：自定义图片裁剪组件](http://blog.csdn.net/liaoinstan/article/details/48734283) http://blog.csdn.net/liaoinstan/article/details/48734283

- 
# [AndroidStudio怎样导入library项目开源库](http://jingyan.baidu.com/article/1974b2898917aff4b1f77415.html)http://jingyan.baidu.com/article/1974b2898917aff4b1f77415.html


# 二、导入库
- 下载项目[SimpleCropView](https://github.com/IsseiAoki/SimpleCropView)。
- 导入库。导入方法：[AndroidStudio怎样导入library项目开源库](http://jingyan.baidu.com/article/1974b2898917aff4b1f77415.html)
- 在[SimpleCropView](https://github.com/IsseiAoki/SimpleCropView)打开build.gradle，将

```
classpath 'com.jfrog.bintray.gradle:gradle-bintray-plugin:1.6'
classpath 'com.github.dcendents:android-maven-gradle-plugin:1.5'
classpath 'com.neenbedankt.gradle.plugins:android-apt:1.8'
```

复制到你项目中的build.gradle，整个build.gradle应该像这样

```
// Top-level build file where you can add configuration options common to all sub-projects/modules.
buildscript {
    repositories {
        jcenter()
    }
    dependencies {
        classpath 'com.android.tools.build:gradle:2.3.3'
        classpath 'com.jfrog.bintray.gradle:gradle-bintray-plugin:1.6'
        classpath 'com.github.dcendents:android-maven-gradle-plugin:1.5'
        classpath 'com.neenbedankt.gradle.plugins:android-apt:1.8'
        // NOTE: Do not place your application dependencies here; they belong
        // in the individual module build.gradle files
    }
}
 
allprojects {
    repositories {
        jcenter()
    }
}
 
task clean(type: Delete) {
    delete rootProject.buildDir
}
```

# 三、在项目中引用组件
- 创建Activity。假设创建的Activity叫做CropActivity.class,布局文件叫做layout_crop.xml。
- 在layout_crop.xml中添加SimpleCropView组件

```
<com.isseiaoki.simplecropview.CropImageView
xmlns:custom="http://schemas.android.com/apk/res-auto"
android:id="@+id/cropImageView"
android:layout_weight="1"
android:paddingTop="16dp"
android:paddingBottom="16dp"
android:paddingLeft="16dp"
android:paddingRight="16dp"
android:layout_width="match_parent"
android:layout_height="wrap_content"
custom:scv_crop_mode="fit_image"
custom:scv_background_color="@color/windowBackground"
custom:scv_handle_color="@color/colorAccent"
custom:scv_guide_color="@color/colorAccent"
custom:scv_overlay_color="@color/overlay"
custom:scv_frame_color="@color/colorAccent"
custom:scv_handle_size="14dp"
custom:scv_touch_padding="8dp"
custom:scv_handle_show_mode="show_always"
custom:scv_guide_show_mode="show_always"
custom:scv_min_frame_size="50dp"
custom:scv_frame_stroke_weight="1dp"
custom:scv_guide_stroke_weight="1dp"/>
```
- 在CropActivity.class的OnCreate()添加如下代码，更多的其他选项请参考[SimpleCropView](https://github.com/IsseiAoki/SimpleCropView)。

```java
mCropView = (CropImageView)findViewById(R.id.cropImageView);
mCropView.setInitialFrameScale(0.8f);

String srcPicPath = "图片路径";
File file = new File(srcPicPath);
BitmapFactory.Options option = new BitmapFactory.Options();
if(file.length()>2*1024*1024){ //如果原图大于2M，压缩，避免程序崩溃
    option.inSampleSize = (int)file.length()/(2*1024*1024);
    Log.d(TAG, "initCropView: "+option.inSampleSize);
}
Bitmap bitmap = BitmapFactory.decodeFile(srcPicPath,option);
mCropView.setImageBitmap(bitmap);
if(bitmap.isRecycled()){
    bitmap.recycle();
}
```
- 保存裁剪后的图片

```java
private void saveBitmapToPath(String path){ //path为图片存储路径
    Bitmap bitmap = mCropView.getCroppedBitmap();
    try {
        File f = new File(path);
        if (f.exists()) {
            f.delete();
        }
        FileOutputStream out = new FileOutputStream(f);
        bitmap.compress(Bitmap.CompressFormat.JPEG, 90, out);
        out.flush();
        out.close();
    } catch (IOException e) {
        Log.d(TAG, "saveBitmapToPath: "+e.toString());
    }
}
```



# 四、总结
- 1、SimpleCropView有很多有用的方法，怎么用要去看[SimpleCropView](https://github.com/IsseiAoki/SimpleCropView)。
- 2、需要文件的读写权限。
- 3、写好的一个模组，可以直接拿过去用 http://download.csdn.net/download/dumpdoctorwang/9955403。文件如下
- layout_crop.xml
	
```
<?xml version="1.0" encoding="utf-8"?>
<android.support.constraint.ConstraintLayout xmlns:android="http://schemas.android.com/apk/res/android"
    xmlns:app="http://schemas.android.com/apk/res-auto"
    xmlns:tools="http://schemas.android.com/tools"
    android:layout_width="match_parent"
    android:layout_height="match_parent"
    tools:context="cn.edu.scu.creator.networkclient.CropActivity"
    android:id="@+id/crop_layout"
    android:background="@color/windowBackground"
    tools:layout_editor_absoluteY="25dp"
    tools:layout_editor_absoluteX="0dp">

    <com.isseiaoki.simplecropview.CropImageView
        xmlns:custom="http://schemas.android.com/apk/res-auto"
        android:id="@+id/cropImageView"
        android:layout_weight="1"
        android:paddingTop="16dp"
        android:paddingBottom="16dp"
        android:paddingLeft="16dp"
        android:paddingRight="16dp"
        android:layout_width="361dp"
        android:layout_height="489dp"
        custom:scv_crop_mode="fit_image"
        custom:scv_background_color="@color/windowBackground"
        custom:scv_handle_color="@color/colorAccent"
        custom:scv_guide_color="@color/colorAccent"
        custom:scv_overlay_color="@color/overlay"
        custom:scv_frame_color="@color/colorAccent"
        custom:scv_handle_size="14dp"
        custom:scv_touch_padding="8dp"
        custom:scv_handle_show_mode="show_always"
        custom:scv_guide_show_mode="show_always"
        custom:scv_min_frame_size="50dp"
        custom:scv_frame_stroke_weight="1dp"
        custom:scv_guide_stroke_weight="1dp"
        tools:layout_constraintTop_creator="1"
        tools:layout_constraintRight_creator="1"
        tools:layout_constraintBottom_creator="1"
        custom:layout_constraintBottom_toTopOf="@+id/btCancel"
        android:layout_marginStart="2dp"
        android:layout_marginEnd="2dp"
        custom:layout_constraintRight_toRightOf="parent"
        tools:layout_constraintLeft_creator="1"
        android:layout_marginBottom="8dp"
        custom:layout_constraintLeft_toLeftOf="parent"
        custom:layout_constraintTop_toTopOf="parent"
        android:layout_marginLeft="2dp"
        android:layout_marginRight="2dp"
        custom:layout_constraintVertical_bias="1.0" />

    <Button
        android:id="@+id/btOK"
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:text="确定"
        tools:layout_constraintRight_creator="1"
        tools:layout_constraintBottom_creator="1"
        app:layout_constraintBottom_toBottomOf="parent"
        android:layout_marginEnd="16dp"
        app:layout_constraintRight_toRightOf="parent"
        android:layout_marginBottom="8dp"
        android:layout_marginRight="16dp" />

    <Button
        android:id="@+id/btCancel"
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:text="取消"
        tools:layout_constraintBottom_creator="1"
        android:layout_marginStart="16dp"
        app:layout_constraintBottom_toBottomOf="parent"
        tools:layout_constraintLeft_creator="1"
        android:layout_marginBottom="8dp"
        app:layout_constraintLeft_toLeftOf="parent"
        android:layout_marginLeft="16dp" />

    <Button
        android:id="@+id/btRotate"
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:text="旋转"
        tools:layout_constraintRight_creator="1"
        tools:layout_constraintBottom_creator="1"
        app:layout_constraintBottom_toBottomOf="parent"
        app:layout_constraintRight_toRightOf="parent"
        tools:layout_constraintLeft_creator="1"
        android:layout_marginBottom="8dp"
        app:layout_constraintLeft_toLeftOf="parent" />

</android.support.constraint.ConstraintLayout>
```

	colors.xml

	
```
<?xml version="1.0" encoding="utf-8"?>
<resources>
    <color name="colorPrimary">#3F51B5</color>
    <color name="colorPrimaryDark">#303F9F</color>
    <color name="colorAccent">#FF4081</color>
    <color name="overlay">#AA1C1C1C</color>
    <color name="windowBackground">#1C1C1C</color>
    <color name="frame">#5DAC81</color>
    <color name="handle">#4F916D</color>
    <color name="guide">#5DAC81</color>
    <color name="colorButtonNormal">#FFFFFB</color>
    <color name="textColorPrimary">#FFFFFB</color>
    <color name="textColorPrimaryInverse">#1C1C1C</color>
    <color name="divider">#333333</color>
</resources>
```

	CropActivity.class

	
```java
package cn.edu.scu.creator.xxx;

import android.content.Intent;
import android.content.res.Resources;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Build;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.widget.Button;

import com.isseiaoki.simplecropview.CropImageView;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;

import static android.R.attr.button;
import static android.R.attr.data;

public class CropActivity extends AppCompatActivity {

    private final static String TAG = MainActivity.TAG;
    private CropImageView mCropView ;
    private Button btOK;
    private Button btCancel;
    private Button btRotate;

    private String srcPicPath;
    private String dstPicPath;

    /**
     * 创建本Activity需要的Intent
     * @param activity 调用本Activity的Activity
     * @param srcPicPath 原图路径
     * @param dstPicPath 保存路径
     * @return
     */
    public static Intent createIntent(AppCompatActivity activity,String srcPicPath, String dstPicPath){
        Intent intent = new Intent(activity,CropActivity.class);
        intent.putExtra("srcPicPath", srcPicPath);
        intent.putExtra("dstPicPath", dstPicPath);
        return intent;
    }

    /**
     * 初始按钮和mCropView组件
     * @param savedInstanceState
     */
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_crop);
        mCropView = (CropImageView)findViewById(R.id.cropImageView);
        mCropView.setInitialFrameScale(0.8f);                      //设置裁剪框大小
        mCropView.setCropMode(CropImageView.CropMode.RATIO_9_16);  //设置裁剪框比例
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {      //设置裁剪框颜色
            mCropView.setFrameColor(getResources().getColor(R.color.frame,this.getTheme()));
            mCropView.setHandleColor(getResources().getColor(R.color.handle,this.getTheme()));
            mCropView.setGuideColor(getResources().getColor(R.color.guide,this.getTheme()));
        } else {
            mCropView.setFrameColor(getResources().getColor(R.color.frame));
            mCropView.setHandleColor(getResources().getColor(R.color.handle));
            mCropView.setGuideColor(getResources().getColor(R.color.guide));
        }

        btOK = (Button)findViewById(R.id.btOK);
        btOK.setOnClickListener(onClickListener);
        btCancel = (Button)findViewById(R.id.btCancel);
        btCancel.setOnClickListener(onClickListener);
        btRotate = (Button)findViewById(R.id.btRotate);
        btRotate.setOnClickListener(onClickListener);

        initCropView();
    }

    /**
     * 从Intent获取原图路径和保存图片路径，并载入到mCropView控件中
     */
    private void initCropView(){
        try {
            Intent intent = this.getIntent();
            Bundle bundle = intent.getExtras();
            if (bundle != null) {
                srcPicPath = bundle.getString("srcPicPath"); // 原图路径
                dstPicPath = bundle.getString("dstPicPath"); // 存储路径
                BitmapFactory.Options option = new BitmapFactory.Options();
                File file = new File(srcPicPath);
                if(file.length()>2*1024*1024){ //如果原图大于2M，压缩，避免程序崩溃
                    option.inSampleSize = (int)file.length()/(2*1024*1024);
                    Log.d(TAG, "initCropView: "+option.inSampleSize);
                }
                Bitmap bitmap = BitmapFactory.decodeFile(srcPicPath,option);
                mCropView.setImageBitmap(bitmap);
                if(bitmap.isRecycled()){
                    bitmap.recycle();
                }
            }
        }catch (Exception e){
            Log.d(TAG, "onCreate: "+e.toString());
        }
    }

    /**
     * 按钮监听器
     */
    private View.OnClickListener onClickListener = new View.OnClickListener() {
        @Override
        public void onClick(View v) {
            switch (v.getId()){
                case R.id.btOK:
                    saveBitmapToPath(mCropView.getCroppedBitmap(),dstPicPath);
                    break;
                case R.id.btCancel:
                    returnToMainActivity("",RESULT_CANCELED);
                    break;
                case R.id.btRotate:
                    rotateImage();
                    break;
                default:
                    break;
            }
        }
    };

    /**
     * 保存图像
     * @param bitmap 要保存的图像
     * @param path 保存路径
     */
    private void saveBitmapToPath(Bitmap bitmap,String path){
//        mCropView.setImageBitmap(bitmap);
        try {
            File f = new File(path);
            if (f.exists()) {
                f.delete();
            }
            FileOutputStream out = new FileOutputStream(f);
            bitmap.compress(Bitmap.CompressFormat.JPEG, 90, out);
            out.flush();
            out.close();
        } catch (IOException e) {
            Log.d(TAG, "saveBitmapToPath: "+e.toString());
        }
        returnToMainActivity(path,RESULT_OK);
    }

    /**
     * 返回上一个Activity，并将保存路径放在Intent中返回
     * @param dstPath 保存路径
     * @param result 结果
     */
    private void returnToMainActivity(String dstPath,int result){
        Intent intent = new Intent();
        intent.putExtra("dstPath", dstPath);
        setResult(result, intent);
        finish();
    }

    /**
     * 旋转图片
     */
    private void rotateImage(){
        mCropView.rotateImage(CropImageView.RotateDegrees.ROTATE_90D);
    }
}
```

- MainActivity.class
	
```java
public class MainActivity extends AppCompatActivity {

    public final static String TAG = "TAG"; //调试用TAG

    public final static int REQUEST_CROP = 2;

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
      if (requestCode == REQUEST_CROP && resultCode == RESULT_OK && data != null) {
            //裁剪成功的操作
        }
    }

   //裁剪图片，path为图片路径
    private void cropPicture(String path){
        try {
            File srcPic = new File(path);
            String toPath = Environment.getExternalStorageDirectory().getPath() + "/MyApp";
            File destDir = new File(toPath);
            if (!destDir.exists()) {
                destDir.mkdirs();
            }
            String savePath = toPath+"/bg.jpeg"; //裁剪后的图片保存路径
            startActivityForResult(CropActivity.createIntent(this,path,savePath), REQUEST_CROP);
        }catch (Exception e){
            Log.d(TAG, "cropPicture: "+e.toString());
        }
    }
}
```

	读我.txt

	`将activity_crop.xml、colors.xml、CropActivity.java放到该放的位置,使用方法在MainActivity.java中。`



