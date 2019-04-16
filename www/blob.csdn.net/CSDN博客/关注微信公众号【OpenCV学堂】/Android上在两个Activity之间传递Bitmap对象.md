# Android上在两个Activity之间传递Bitmap对象 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2015年12月07日 23:22:53[gloomyfish](https://me.csdn.net/jia20003)阅读数：8914








**Android上在两个Activity之间传递Bitmap对象**

因为我做Android应用的时间不长，所以当我第一次在android之间传递Bitmap对象的时候我直接用了Intent.putExtra的方法实现了，当时我选择的是一张图像的ROI区域，所以工作的很好，但是当我尝试将整个图片做为Bitmap对象传到另外一个Activity中的时候总是得到一个错误fatal bind error我当时很郁闷了好久，后来google才发现是这样传递Bitmap对象的方式不对，Activity之间传递Bitmap有大小限制，而且这个大小限制很小，512x512的图片肯定会得到我上述的这个错误，于是我想到了先保存到SD卡上然后传递路径的方法，但是SD卡属于外部存储，可能不是特别方便，后来看到有人通过内部存储方式实现了在两个Activity之间传递Bitmap对象以及其它支持串行化的Java对象，我觉得很好，所以就写了这篇博客。关键点有如下：

1．  HTTP客户端下载图片，通过ImageView对象显示

2．  把ImageView上的Bitmap对象从当前Activity传递到另外一个

Activity中并显示出来

3．  基于串行化传递Java对象数据



首先看我是怎么实现HTTP客户端下载图片，通过异步Task接口实现HTTP客户端下载图片并通过Handler来更新ImageView，代码如下：



```java
package com.example.sharedemo;

import java.io.IOException;
import java.io.InputStream;

import org.apache.http.HttpEntity;
import org.apache.http.HttpResponse;
import org.apache.http.HttpStatus;
import org.apache.http.client.ClientProtocolException;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.impl.client.DefaultHttpClient;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.AsyncTask;
import android.os.Handler;
import android.os.Message;
import android.util.Log;

public class ImageLoadTask extends AsyncTask<String, Void, Bitmap> {
	private Handler handler;

	public ImageLoadTask(Handler handler) {
		this.handler = handler;
	}

	protected void onPostExecute(Bitmap bitmap) {
		Message msg = new Message();
		msg.obj = bitmap;
		handler.sendMessage(msg);
	}

	@Override
	protected Bitmap doInBackground(String... urls) {
		Bitmap bitmap = null;
		// create HTTP client
		HttpClient httpclient = new DefaultHttpClient();
		try {
			// GET request
			Log.i("image-url", urls[0]);
			HttpGet httpRequest = new HttpGet(urls[0]);
			HttpResponse httpResponse = httpclient.execute(httpRequest);
			if (httpResponse.getStatusLine().getStatusCode() == HttpStatus.SC_OK) {
				// get entity from response
				HttpEntity httpEntity = httpResponse.getEntity();
				// read stream
				InputStream is = httpEntity.getContent();
				bitmap = BitmapFactory.decodeStream(is);
				is.close();
				Log.i("image", "already get the image by url : " + urls[0]);
			}

		} catch (ClientProtocolException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		} finally {
			httpclient.getConnectionManager().shutdown();
		}
		return bitmap;
	}

}
```



在当前的Activity中通过按钮上的事件响应实现切换View到对应的Activity中去，同时实现Java串行化数据传递。MainActivity的代码如下：



```java
package com.example.sharedemo;

import java.io.ByteArrayOutputStream;
import java.io.FileOutputStream;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.Matrix;
import android.graphics.drawable.BitmapDrawable;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.ImageView;

public class MainActivity extends Activity implements OnClickListener{
	public final static String SHARE_BITMAP_COMMAND = "share-image";
	public final static String SHARE_TEXT_DATA_COMMAND = "share-text-data";
	private Handler handler;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		setupOnClickListener();
	}

	private void setupOnClickListener() {
		Button bitmapBtn = (Button)this.findViewById(R.id.bitmapShareBtn);
		bitmapBtn.setTag(SHARE_BITMAP_COMMAND);
		bitmapBtn.setOnClickListener(this);
		Button textDataBtn = (Button)this.findViewById(R.id.mapShareBtn);
		textDataBtn.setTag(SHARE_TEXT_DATA_COMMAND);
		textDataBtn.setOnClickListener(this);
		final ImageView imageView = (ImageView)this.findViewById(R.id.imageView1);
		
		handler = new Handler()
		{
			public void handleMessage(Message msg) {
				Bitmap bitmap = (Bitmap)msg.obj;
				if(bitmap != null)
				{
					/*
					// 为防止原始图片过大导致内存溢出，这里先缩小原图显示，然后释放原始Bitmap占用的内存
					Bitmap smallBitmap = zoomBitmap(bitmap, bitmap.getWidth()/ 5, bitmap.getHeight() / 5);
					// 释放资源
					bitmap.recycle();
					// 显示图片
					imageView.setImageBitmap(smallBitmap);
					imageView.invalidate();
					*/
					imageView.setImageBitmap(bitmap);
				}
			}
		};
		ImageLoadTask task = new ImageLoadTask(handler);
		task.execute("https://img-blog.csdn.net/20150607143208238");
	}
	
	public static Bitmap zoomBitmap(Bitmap bitmap, int width, int height) {
		int w = bitmap.getWidth();
		int h = bitmap.getHeight();
		Matrix matrix = new Matrix();
		float scaleWidth = ((float) width / w);
		float scaleHeight = ((float) height / h);
		matrix.postScale(scaleWidth, scaleHeight); // 不改变原来图像大小
		Bitmap newbmp = Bitmap.createBitmap(bitmap, 0, 0, w, h, matrix, true);
		return newbmp;
	}

	@Override
	public void onClick(View v) {
		Object tag = v.getTag();
		Log.i("command", tag.toString());
		if(SHARE_BITMAP_COMMAND.equals(tag))
		{
			Intent intent = new Intent(this.getApplicationContext(), ImageProcessActivity.class);
			ImageView imageView = (ImageView)this.findViewById(R.id.imageView1);
			Bitmap bitmap = ((BitmapDrawable)imageView.getDrawable()).getBitmap();
			intent.putExtra("selectedImage", bitmap);
			intent.putExtra("name", "lena");
			intent.putExtra("description", "超级大美女");
			this.startActivity(intent);			
		}
		else if(SHARE_TEXT_DATA_COMMAND.equals(tag))
		{
			Intent intent = new Intent(this.getApplicationContext(), ImageProcessActivity.class);
			ImageView imageView = (ImageView)this.findViewById(R.id.imageView1);
			Bitmap bitmap = ((BitmapDrawable)imageView.getDrawable()).getBitmap();
			// save it first then pass URI
			ImageInfoBean dto = new ImageInfoBean();
			String uri = createImageFromBitmap(bitmap);
			dto.setDescription("超级大美女");
			dto.setName("lena");
			dto.setUri(uri);
			
			intent.putExtra("tiger", dto);
			this.startActivity(intent);	
		}
	}
	
	public String createImageFromBitmap(Bitmap bitmap) {
	    String fileName = "myImage";
	    try {
	        ByteArrayOutputStream bytes = new ByteArrayOutputStream();
	        bitmap.compress(Bitmap.CompressFormat.JPEG, 100, bytes);
	        FileOutputStream fo = openFileOutput(fileName, Context.MODE_PRIVATE);
	        fo.write(bytes.toByteArray());
	        fo.close();
	    } catch (Exception e) {
	        e.printStackTrace();
	        fileName = null;
	    }
	    Log.i("fileName", fileName);
	    return fileName;
	}
}
```



对应另外一个Activity中实现读取与组装Bitmap对象显示的代码如下：



```java
package com.example.sharedemo;

import java.io.FileNotFoundException;

import android.app.Activity;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.widget.ImageView;
import android.widget.TextView;

public class ImageProcessActivity extends Activity {

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.share_info);
		backFillData();
	}

	private void backFillData() {
		Object obj = this.getIntent().getExtras().get("tiger");
		ImageView imageView = (ImageView)this.findViewById(R.id.imageView1);
		TextView text1 = (TextView)this.findViewById(R.id.textView1);
		TextView text2 = (TextView)this.findViewById(R.id.textView2);
		try {
			if(obj != null && obj instanceof ImageInfoBean)
			{
				ImageInfoBean dto = (ImageInfoBean)obj;
				Bitmap bitmap = BitmapFactory.decodeStream(this.openFileInput(dto.getUri()));
				imageView.setImageBitmap(bitmap);
				imageView.invalidate(); // refresh
				
				text1.setText("名称: " + dto.getName());
				text2.setText("描述: " + dto.getDescription());
				return;
			}
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		}
		
		Bitmap bitmap = (Bitmap) this.getIntent().getParcelableExtra("selectedImage");
		String name = this.getIntent().getExtras().getString("name");
		String description = this.getIntent().getExtras().getString("description");
		if(bitmap != null)
		{
			imageView.setImageBitmap(bitmap);
			imageView.invalidate(); // refresh
		}
		if(name != null)
		{
			text1.setText("名称: " + name);
		}
		if(description != null)
		{
			text2.setText("描述: " + description);
		}
	}

}
```
对应的Java串行化对象类代码如下：


```java
package com.example.sharedemo;

import java.io.Serializable;

public class ImageInfoBean implements Serializable {
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;

	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}

	public String getDescription() {
		return description;
	}

	public void setDescription(String description) {
		this.description = description;
	}

	public String getUri() {
		return uri;
	}

	public void setUri(String uri) {
		this.uri = uri;
	}

	private String name;
	private String description;
	private String uri;

}
```
**最后声明：**
别忘记在Manifest文件添加网络访问的权限


`<uses-permission android:name="android.permission.INTERNET"/>`![](https://img-blog.csdn.net/20151207231749474)
第一个按钮【传递图片】将会演示我遇到错误，第二按钮【传递文本数据】
将会演示正确的处理结果，显示如下：

![](https://img-blog.csdn.net/20151207231953697)




