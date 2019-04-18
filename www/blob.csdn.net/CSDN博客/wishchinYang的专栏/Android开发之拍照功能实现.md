# Android开发之拍照功能实现 - wishchinYang的专栏 - CSDN博客
2015年06月25日 00:32:07[wishchin](https://me.csdn.net/wishchin)阅读数：377
参考链接：http://www.linuxidc.com/Linux/2013-11/92892p3.htm
原文链接：[http://blog.csdn.net/tangcheng_ok/article/details/7036871](http://blog.csdn.net/tangcheng_ok/article/details/7036871)
增加使用相机和写入权限：
```
<uses-permission android:name="android.permission.CAMERA" />
    <uses-feature android:name="android.hardware.camera" />
    <uses-feature android:name="android.hardware.camera.autofocus" />
    <uses-permission android:name="android.permission.WRITE_EXTERNAL_STORAGE" />
```
主程序代码：
```java
package com.example.carejulygpsmap;
import android.support.v7.app.ActionBarActivity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.File;  
import java.io.FileOutputStream;  
import java.io.IOException;  
import java.util.Date;  
  
import android.app.Activity;  
import android.content.pm.ActivityInfo;  
import android.graphics.PixelFormat;  
import android.hardware.Camera;  
import android.os.AsyncTask;  
import android.os.Bundle;  
import android.os.Environment;  
import android.text.format.DateFormat;  
import android.util.Log;  
import android.view.KeyEvent;  
import android.view.SurfaceHolder;  
import android.view.SurfaceView;  
import android.view.View;  
import android.view.Window;  
import android.view.View.OnClickListener;  
import android.widget.Button; 
import android.app.Activity;
import android.content.Intent;
import android.graphics.Bitmap;
import android.net.Uri;
import android.os.Bundle;
import android.os.Environment;
import android.provider.MediaStore;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
public class CareJulyActivity extends ActionBarActivity {
    private final static String TAG = "CameraActivity";  
    private SurfaceView surfaceView;  
    private SurfaceHolder surfaceHolder;  
    private Camera camera;  
    private File picture;  
    private Button btnSave;  
	
    private static final int NONE = 0;
    private static final int PHOTO_GRAPH = 1;// 拍照
    private static final int PHOTO_ZOOM = 2; // 缩放
    private static final int PHOTO_RESOULT = 3;// 结果
    private static final String IMAGE_UNSPECIFIED = "image/*";
    private ImageView imageView = null;
    private Button btnPhone = null;
    private Button btnTakePicture = null;
    
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
        setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);  
        this.requestWindowFeature(Window.FEATURE_NO_TITLE);  
        setContentView(R.layout.activity_care_july);  
        setupViews();  
//        imageView = (ImageView) findViewById(R.id.imageView);
//        btnPhone = (Button) findViewById(R.id.btnPhone);
//        btnPhone.setOnClickListener(onClickListener);
//        btnTakePicture = (Button) findViewById(R.id.btnTakePicture);
//        btnTakePicture.setOnClickListener(onClickListener);
	}
    
    
//	@Override//src
//	protected void onCreate(Bundle savedInstanceState) {
//		super.onCreate(savedInstanceState);
//		setContentView(R.layout.activity_care_july);
//	}
	@Override//src
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.care_july, menu);
		return true;
	}
	@Override//src
	public boolean onOptionsItemSelected(MenuItem item) {
		// Handle action bar item clicks here. The action bar will
		// automatically handle clicks on the Home/Up button, so long
		// as you specify a parent activity in AndroidManifest.xml.
		int id = item.getItemId();
		if (id == R.id.action_settings) {
			return true;
		}
		return super.onOptionsItemSelected(item);
	}
	
	
	//CamCap
    private void setupViews(){  
        surfaceView = (SurfaceView) findViewById(R.id.camera_preview); // Camera interface to instantiate components  
        surfaceHolder = surfaceView.getHolder(); // Camera interface to instantiate components  
        surfaceHolder.addCallback(surfaceCallback); // Add a callback for the SurfaceHolder  
        surfaceHolder.setType(SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS);  
          
        btnSave = (Button) findViewById(R.id.save_pic);  
          
        btnSave.setOnClickListener(new OnClickListener() {  
              
            @Override  
            public void onClick(View v) {  
                takePic();  
            }  
        });  
    }  
      
      
    @Override  
    public boolean onKeyDown(int keyCode, KeyEvent event) {  
        if (keyCode == KeyEvent.KEYCODE_CAMERA  
                || keyCode == KeyEvent.KEYCODE_SEARCH) {  
            takePic();  
            return true;  
        }  
        return super.onKeyDown(keyCode, event);  
    }  
  
    private void takePic() {  
  
        camera.stopPreview();// stop the preview  
  
        camera.takePicture(null, null, pictureCallback); // picture  
    }  
  
    // Photo call back  
    Camera.PictureCallback pictureCallback = new Camera.PictureCallback() {  
        //@Override  
        public void onPictureTaken(byte[] data, Camera camera) {  
            new SavePictureTask().execute(data);  
            camera.startPreview();  
        }  
    };  
  
    // save pic  
    //保存图片，不过保存的图片较小，保存在根目录 的wishchin文件夹里面
    class SavePictureTask extends AsyncTask<byte[], String, String> {  
        @Override  
        protected String doInBackground(byte[]... params) {  
            String fname = DateFormat.format("yyyyMMddhhmmss", new Date()).toString()+".jpg";  
              
            Log.i(TAG, "fname="+fname+";dir="+Environment.getExternalStorageDirectory());  
            //picture = new File(Environment.getExternalStorageDirectory(),fname);// create file  
              
            picture = new File(Environment.getExternalStorageDirectory()+"/wishchin/"+fname);  
              
            try {  
                FileOutputStream fos = new FileOutputStream(picture.getPath()); // Get file output stream  
                fos.write(params[0]); // Written to the file  
                fos.close();   
            } catch (Exception e) {  
                e.printStackTrace();  
            }  
            return null;  
        }  
    }  
  
    // SurfaceHodler Callback handle to open the camera, off camera and photo size changes  
    SurfaceHolder.Callback surfaceCallback = new SurfaceHolder.Callback() {  
  
        public void surfaceCreated(SurfaceHolder holder) {  
            Log.i(TAG, "surfaceCallback====");  
            camera = Camera.open(); // Turn on the camera  
            try {  
                camera.setPreviewDisplay(holder); // Set Preview  
            } catch (IOException e) {  
                camera.release();// release camera  
                camera = null;  
            }  
        }  
  
        public void surfaceChanged(SurfaceHolder holder, int format, int width,  
                int height) {  
            Log.i(TAG,"====surfaceChanged");  
            Camera.Parameters parameters = camera.getParameters(); // Camera parameters to obtain  
            parameters.setPictureFormat(PixelFormat.JPEG);// Setting Picture Format  
//          parameters.set("rotation", 180); // Arbitrary rotation  
            camera.setDisplayOrientation(0);  
//          parameters.setPreviewSize(400, 300); // Set Photo Size  
            camera.setParameters(parameters); // Setting camera parameters  
            camera.startPreview(); // Start Preview  
        }  
  
        public void surfaceDestroyed(SurfaceHolder holder) {  
            Log.i(TAG,"====surfaceDestroyed");  
            camera.stopPreview();// stop preview  
            camera.release(); // Release camera resources  
            camera = null;  
        }  
    }; 
}
```
界面代码：
```cpp
<?xml version="1.0" encoding="utf-8"?>  
<LinearLayout xmlns:android="http://schemas.android.com/apk/res/android"  
    android:layout_width="fill_parent"  
    android:layout_height="fill_parent"  
    android:orientation="vertical" >  
  
    <android.view.SurfaceView
        android:id="@+id/camera_preview"
        android:layout_width="462dp"
        android:layout_height="534dp"
        android:layout_alignParentTop="true"
        android:layout_centerInParent="true"
        android:layout_gravity="center_vertical|center_horizontal" />
  
    <Button  
        android:id="@+id/save_pic"  
        android:layout_width="fill_parent"  
        android:layout_height="wrap_content"  
        android:text="txt_save" />  
  
</LinearLayout>
```
