# android屏幕取词的简单实现（二）（使用OCR技术） - xqhrs232的专栏 - CSDN博客
2016年12月01日 17:36:24[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：2264
原文地址::[http://www.tuicool.com/articles/feeEZf](http://www.tuicool.com/articles/feeEZf)
相关文章
1、屏幕取词 被取对象为QT程序 ----[http://bbs.csdn.net/topics/390442683](http://bbs.csdn.net/topics/390442683)
2、[屏幕取词技术实现原理](http://blog.csdn.net/attilax/article/details/52962258)----[http://blog.csdn.net/attilax/article/details/52962258](http://blog.csdn.net/attilax/article/details/52962258)
很久之前就应该完成的东西 
直到今天才抽出时间来弄 
接着上一篇http://blog.csdn.net/libaier_gc/article/details/7295242 
上次这篇文章被无情的喷了，感觉还是有道理 
毕竟写的效率太低了 
这此换另外一种方法实现屏幕取词 
即使用OCR技术 
呵呵，当然这只是个demo程序 
与其说是“屏幕取词”，还不如说是“截图翻译” 
但屏幕取词的思想大概就是这样吧~（额，估计有该有人喷了。。） 
废话真多！直接开始说程序了~
程序运行情况截图 
![](http://img0.tuicool.com/a63mYn.png!web)
主要运行过程 
点击“截张小图~”从本地一张图片中截取自己想翻译的特定部分 
点击“ocr一下”讲自己截出的图片进行OCR取词翻译
真正实现屏幕取词的时候，先截取现在的屏幕，然后自己选取指定部分进行翻译即可~ 
由于以后一年可能会长期从事屏幕取词的开发，诸多细节还需完善，所以今天只发demo程序
首先贴出完整代码
布局文件
<?xml version="1.0" encoding="utf-8"?><RelativeLayoutxmlns:android="http://schemas.android.com/apk/res/android"android:id="@+id/RelativeLayout1"android:layout_width="fill_parent"android:layout_height="fill_parent"android:orientation="vertical" ><TextViewandroid:id="@+id/textView1"android:layout_width="fill_parent"android:layout_height="wrap_content"android:autoLink="web"android:gravity="center"android:text="www.libaier.net"android:textAppearance="?android:attr/textAppearanceLarge" /><Buttonandroid:id="@+id/getPic"android:layout_width="wrap_content"android:layout_height="wrap_content"android:layout_alignParentLeft="true"android:layout_alignParentRight="true"android:layout_below="@+id/textView1"android:layout_marginTop="41dp"android:text="截张小图~" /><ImageViewandroid:id="@+id/imageView1"android:layout_width="wrap_content"android:layout_height="wrap_content"android:layout_below="@+id/getPic"android:layout_centerHorizontal="true"android:layout_marginTop="72dp" /><TextViewandroid:id="@+id/result"android:layout_width="wrap_content"android:layout_height="wrap_content"android:layout_alignParentBottom="true"android:layout_alignParentLeft="true"android:layout_alignParentRight="true"android:gravity="center"android:textAppearance="?android:attr/textAppearanceLarge" /><Buttonandroid:id="@+id/ocr"android:layout_width="wrap_content"android:layout_height="wrap_content"android:layout_alignParentLeft="true"android:layout_alignParentRight="true"android:layout_below="@+id/imageView1"android:layout_marginTop="98dp"android:text="OCR一下~" /></RelativeLayout>
可执行程序
```java
package libaier.ocr;
import java.io.File;
import java.io.IOException;
import android.app.Activity;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Matrix;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;
import android.media.ExifInterface;
import android.net.Uri;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;
import com.googlecode.tesseract.android.TessBaseAPI;
public class OCRActivity extends Activity {
	private static int SELECT_PICTURE;// 返回标志位 filed
	private File tempFile;
	private Button getpicButton = null;
	private Button ocrButton = null;
	private ImageView imageView = null;
	private TextView textView = null;
	private Bitmap bitmap = null;
	// ** Called when the activity is first created. *//*
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		this.tempFile = new File("/sdcard/ocr.png");// 这句一定要在onCreate()里面调用
		setContentView(R.layout.main);
		getpicButton = (Button) findViewById(R.id.getPic);
		ocrButton = (Button) findViewById(R.id.ocr);
		textView = (TextView) findViewById(R.id.result);
		imageView = (ImageView) findViewById(R.id.imageView1);
		getpicButton.setOnClickListener(new OnClickListener() {
			public void onClick(View v) {
				Intent intent = new Intent(Intent.ACTION_GET_CONTENT);
				intent.setType("image/*");
				intent.putExtra("crop", "true");// crop=true 有这句才能出来最后的裁剪页面.
				intent.putExtra("aspectX", 2);// 这两项为裁剪框的比例.
				intent.putExtra("aspectY", 1);// x:y=1:2
				intent.putExtra("output", Uri.fromFile(tempFile));
				intent.putExtra("outputFormat", "PNG");// 返回格式
				startActivityForResult(Intent.createChooser(intent, "选择图片"),
						SELECT_PICTURE);
			}
		});
	}
	// **
	// 裁剪完图片后系统调用的方法:onActivityResult
	// *
	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		if (resultCode == RESULT_OK)
			if (requestCode == SELECT_PICTURE) {
				bitmap = BitmapFactory.decodeFile("/sdcard/ocr.png");
				Drawable drawable = new BitmapDrawable(bitmap);
				imageView.setBackgroundDrawable(drawable);
				// tesseract req. ARGB_8888
				bitmap = bitmap.copy(Bitmap.Config.ARGB_8888, true);
				ocrButton.setOnClickListener(new OnClickListener() {
					public void onClick(View v) {
						// TODO Auto-generated method stub
						TessBaseAPI baseApi = new TessBaseAPI();
						baseApi.init(android.os.Environment
								.getExternalStorageDirectory()
								.getAbsolutePath()
								+ "/ocr", "eng");
						baseApi.setImage(bitmap);
						// 根据Init的语言，获得ocr后的字符串
						String text = baseApi.getUTF8Text();
						// 释放bitmap
						baseApi.clear();
						// 如果连续ocr多张图片，这个end可以不调用，但每次ocr之后，必须调用clear来对bitmap进行释放
						// 释放native内存
						baseApi.end();
						textView.setText("识别结果为:\n"+text);
					}
				});
			}
	}
}
```
代码其实很简单
oncreate（）中主要进行截图 
onActivityResult（）进行OCR翻译 
这里使用了tesseract的 OCR库
要在自己的手机上运行此应用程序 
先要在SD卡上建立/ocr文件夹，将 [这个文件](http://download.csdn.net/detail/libaier_gc/4194228) （tesseract的
 OCR库）解压到这个文件夹里才能运行~
自己使用tesseract开发，还需在自己的android源码工程中添加/lib文件夹和.jar文件 
详情可参考 [这篇文章](http://www.cnblogs.com/hangxin1940/archive/2012/01/13/2321507.html) ~
最后是完整工程下载地址 [http://download.csdn.net/detail/libaier_gc/4194247](http://download.csdn.net/detail/libaier_gc/4194247)
相信大家应该能看懂吧~
这几天被Gallery3d搞的很乱
洗洗睡了~
愚人节快乐~
原创文章，转载请注明：  转载自 [libaier](http://libaier.net/)
本文链接地址:   [android屏幕取词的简单实现（使用OCR技术）](http://libaier.net/?p=408)
