# Android 人脸识别+人脸匹配(OpenCV+JavaCV) - z69183787的专栏 - CSDN博客
2017年08月17日 16:26:01[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2903
[http://m.blog.csdn.net/sky286753213/article/details/11887913](http://m.blog.csdn.net/sky286753213/article/details/11887913)
项目源码 [http://download.csdn.net/detail/sky286753213/6617075](http://download.csdn.net/detail/sky286753213/6617075)
之前不想上传源码是因为代码写的实在很烂,参考价值不是很大,主要代码都在CameraActivity中
这里吐下槽,目前Android人脸识别技术分享的感觉很少
我也是参考一些资料和别人的代码加以改进
上个项目中用到的技术现在有些遗忘了,把关键性的代码给大家分享下
```java
public void DetectFace() {
		Mat image = Highgui.imread(FACE);
		MatOfRect faceDetections = new MatOfRect();
		mJavaDetector.detectMultiScale(image, faceDetections);
		for (Rect rect : faceDetections.toArray()) {
			Core.rectangle(image, new Point(rect.x, rect.y), new Point(rect.x
					+ rect.width, rect.y + rect.height), new Scalar(0, 255, 0));
			// 把检测到的人脸重新定义大小后保存成文件
			Mat sub = image.submat(rect);
			Mat mat = new Mat();
			Size size = new Size(100, 100);
			Imgproc.resize(sub, mat, size);
			Highgui.imwrite(FACEDONE, mat);
		}
	}
```
FACE和FACEDONE为两个路径 一个是拍照完成的图片路径,另一个是生成的人脸图片的路径
这里使用了OpenCV中的人脸识别方法,方法很简单,把你的照片路径带进去他会检测到这张图片里有多少个人头,然后进行循环输出人脸图片
这里需要注意下的是 如果进行人脸比对的话需要每张图片的大小一样,否则会报异常
另外,想使用OpenCV之前还需定义一个BaseLoaderCallback对象来加载识别的人脸xml库
```java
private BaseLoaderCallback mLoaderCallback = new BaseLoaderCallback(this) {
		public void onManagerConnected(int status) {
			switch (status) {
			case LoaderCallbackInterface.SUCCESS: {
				mJavaDetector = new CascadeClassifier(
						"/sdcard/FaceDetect/haarcascade_frontalface_alt2.xml");
			}
				break;
			default: {
				super.onManagerConnected(status);
			}
				break;
			}
		}
	};
```
然后在OnResume中加载BaseLoaderCallback对象
```java
OpenCVLoader.initAsync(OpenCVLoader.OPENCV_VERSION_2_4_5, this,
				mLoaderCallback);
```
人脸匹配使用的是JavaCV中的匹配方法
该方法的博客一会附上连接
```java
public int Identification() {
		FaceRecognizer fr = createFisherFaceRecognizer();
		MatVector mv = new MatVector(faceList.size());
		CvMat cvMat = CvMat.create(faceList.size(), 1, CV_32SC1);
		for (int i = 0; i < faceList.size(); i++) {
			IplImage img = cvLoadImage(faceList.get(i).getPath(),
					CV_LOAD_IMAGE_GRAYSCALE);
			mv.put(i, img);
			cvMat.put(i, 0, i);
		}
		fr.train(mv, cvMat);
		IplImage testImage = cvLoadImage(
				Environment.getExternalStorageDirectory()
						+ "/FaceDetect/faceDone.jpg", CV_LOAD_IMAGE_GRAYSCALE);
		return fr.predict(testImage);
	}
```
faceList中放的是人脸图片文件夹里所有人脸图片的路径
循环把他们放入MatVector和CvMat中
然后对FaceRecognizer进行训练
```java
/FaceDetect/faceDone.jpg
```
这个路径是经过上面识别方法处理完图片后所输出的人脸图片的路径
predict()方法把刚才的人脸图片放入训练模型中进行匹配 会返回一个模型中的索引值
获取到索引值后就大功告成了
下面一个方法可以获取到两张图片的相似度,最大值是100
```java
public double CmpPic(String path) {
		int l_bins = 20;
		int hist_size[] = { l_bins };
		float v_ranges[] = { 0, 100 };
		float ranges[][] = { v_ranges };
		IplImage Image1 = cvLoadImage(Environment.getExternalStorageDirectory()
				+ "/FaceDetect/faceDone.jpg", CV_LOAD_IMAGE_GRAYSCALE);
		IplImage Image2 = cvLoadImage(path, CV_LOAD_IMAGE_GRAYSCALE);
		IplImage imageArr1[] = { Image1 };
		IplImage imageArr2[] = { Image2 };
		CvHistogram Histogram1 = CvHistogram.create(1, hist_size,
				CV_HIST_ARRAY, ranges, 1);
		CvHistogram Histogram2 = CvHistogram.create(1, hist_size,
				CV_HIST_ARRAY, ranges, 1);
		cvCalcHist(imageArr1, Histogram1, 0, null);
		cvCalcHist(imageArr2, Histogram2, 0, null);
		cvNormalizeHist(Histogram1, 100.0);
		cvNormalizeHist(Histogram2, 100.0);
		return cvCompareHist(Histogram1, Histogram2, CV_COMP_CORREL);
	}
```
带入两张图片的路径即可,这里不在阐述
下面是源码,写的很烂,仅供参考
```java
package com.comdosoft.face;
import static com.googlecode.javacv.cpp.opencv_contrib.createFisherFaceRecognizer;
import static com.googlecode.javacv.cpp.opencv_core.CV_32SC1;
import static com.googlecode.javacv.cpp.opencv_highgui.CV_LOAD_IMAGE_GRAYSCALE;
import static com.googlecode.javacv.cpp.opencv_highgui.cvLoadImage;
import static com.googlecode.javacv.cpp.opencv_imgproc.CV_COMP_CORREL;
import static com.googlecode.javacv.cpp.opencv_imgproc.CV_HIST_ARRAY;
import static com.googlecode.javacv.cpp.opencv_imgproc.cvCalcHist;
import static com.googlecode.javacv.cpp.opencv_imgproc.cvCompareHist;
import static com.googlecode.javacv.cpp.opencv_imgproc.cvNormalizeHist;
import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.util.ArrayList;
import java.util.List;
import java.util.Timer;
import java.util.TimerTask;
import org.opencv.android.BaseLoaderCallback;
import org.opencv.android.LoaderCallbackInterface;
import org.opencv.android.OpenCVLoader;
import org.opencv.core.Core;
import org.opencv.core.Mat;
import org.opencv.core.MatOfRect;
import org.opencv.core.Point;
import org.opencv.core.Rect;
import org.opencv.core.Scalar;
import org.opencv.core.Size;
import org.opencv.highgui.Highgui;
import org.opencv.imgproc.Imgproc;
import org.opencv.objdetect.CascadeClassifier;
import android.app.Activity;
import android.app.ProgressDialog;
import android.content.Intent;
import android.content.pm.ActivityInfo;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Matrix;
import android.hardware.Camera;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.os.Message;
import android.view.LayoutInflater;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.view.ViewGroup;
import android.view.Window;
import android.view.WindowManager;
import android.widget.ImageView;
import android.widget.Toast;
import com.comdosoft.pojo.FacePojo;
import com.googlecode.javacv.cpp.opencv_contrib.FaceRecognizer;
import com.googlecode.javacv.cpp.opencv_core.CvMat;
import com.googlecode.javacv.cpp.opencv_core.IplImage;
import com.googlecode.javacv.cpp.opencv_core.MatVector;
import com.googlecode.javacv.cpp.opencv_imgproc.CvHistogram;
public class CameraActivity extends Activity implements SurfaceHolder.Callback,
		Camera.PictureCallback {
	private List<FacePojo> faceList = new ArrayList<FacePojo>();
	private ProgressDialog proDialog;
	private CascadeClassifier mJavaDetector;
	private SurfaceView mSurfaceView;
	private SurfaceHolder holder;
	private Camera camera;
	private String FACE = "/sdcard/FaceDetect/face.jpg";
	private String FACEDONE = "/sdcard/FaceDetect/faceDone.jpg";
	private int index;
	private int type;
	private int t = 2;
	private int angle = 0;
	private boolean flag = true;
	private double value;
	private Toast toast;
	private FacePojo fp;
	private int cameraCount;
	private CameraUtil cu = new CameraUtil();
	private int[] iconArr = new int[] { R.drawable.icon1, R.drawable.icon2,
			R.drawable.icon3 };
	private BaseLoaderCallback mLoaderCallback = new BaseLoaderCallback(this) {
		public void onManagerConnected(int status) {
			switch (status) {
			case LoaderCallbackInterface.SUCCESS: {
				mJavaDetector = new CascadeClassifier(
						"/sdcard/FaceDetect/haarcascade_frontalface_alt2.xml");
			}
				break;
			default: {
				super.onManagerConnected(status);
			}
				break;
			}
		}
	};
	private Handler updateUI = new Handler() {
		@Override
		public void handleMessage(Message msg) {
			switch (msg.what) {
			case 1:
				proDialog.dismiss();
				Intent intent = new Intent();
				if (type == 0 || type == 1) {
					index = Identification();
					fp = faceList.get(index);
					value = CmpPic(fp.getPath()) * 100;
				}
				// Toast.makeText(getApplicationContext(),
				// "相似度:" + value + "--" + num, 0).show();
				if (value < 70 && type != 2) {
					intent.setClass(getApplicationContext(), SignFail.class);
					startActivity(intent);
				} else {
					if (type == 0) {
						Bundle bundle = new Bundle();
						bundle.putString("id", fp.getId());
						bundle.putString("name", fp.getName());
						intent.putExtras(bundle);
						intent.setClass(getApplicationContext(), SignIn.class);
						startActivity(intent);
					} else if (type == 1) {
						Intent intent2 = getIntent();
						Bundle bundle2 = new Bundle();
						bundle2.putInt("status", 1);
						bundle2.putString("id", fp.getId());
						bundle2.putString("name", fp.getName());
						intent2.putExtras(bundle2);
						setResult(RESULT_OK, intent2);
						finish();
					} else if (type == 2) {
						Intent intent2 = getIntent();
						setResult(RESULT_OK, intent2);
						finish();
					}
				}
				break;
			}
		}
	};
	private Timer dataTimer = new Timer();
	private Handler dataHandler = new Handler() {
		@Override
		public void handleMessage(Message msg) {
			super.handleMessage(msg);
			int msgId = msg.what;
			switch (msgId) {
			case 1:
				if (flag && t == -1) {
					// camera.autoFocus(null);
					camera.takePicture(null, null, CameraActivity.this);
					flag = false;
				}
				if (flag) {
					timeDown();
				}
			}
		}
	};
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		getWindow().clearFlags(
				WindowManager.LayoutParams.FLAG_FORCE_NOT_FULLSCREEN);
		getWindow().addFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);
		requestWindowFeature(Window.FEATURE_NO_TITLE);// 设置横屏模式以及全屏模式
		setContentView(R.layout.camera);// 设置View
		proDialog = new ProgressDialog(this);
		Intent bundle = getIntent();
		type = bundle.getIntExtra("type", 0);
		mSurfaceView = (SurfaceView) findViewById(R.id.camera);
		holder = mSurfaceView.getHolder();
		holder.addCallback(this);
		holder.setType(SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS);// 指定Push Buffer
	}
	@Override
	protected void onStart() {
		super.onStart();
		LoadFaceData();
	}
	@Override
	protected void onResume() {
		super.onResume();
		if (getRequestedOrientation() != ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE) {
			setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
		}
		OpenCVLoader.initAsync(OpenCVLoader.OPENCV_VERSION_2_4_5, this,
				mLoaderCallback);
	}
	public void onPictureTaken(byte[] data, Camera camera) {
		try {
			String path = Environment.getExternalStorageDirectory()
					+ "/FaceDetect/face.jpg";
			// data2file(data, path);
			Bitmap bitmap = BitmapFactory.decodeByteArray(data, 0, data.length);
			Matrix matrix = new Matrix();
			// 设置图像的旋转角度
			matrix.setRotate(angle);
			// 旋转图像，并生成新的Bitmap对像
			bitmap = Bitmap.createBitmap(bitmap, 0, 0, bitmap.getWidth(),
					bitmap.getHeight(), matrix, true);
			cu.writePhoto(bitmap, bitmap.getWidth(), bitmap.getHeight(), path);
			if (type == 2) {
				proDialog.setMessage("正在检测...");
			} else {
				proDialog.setMessage("正在识别...");
			}
			proDialog.show();
			Thread loginThread = new Thread(new LoginFailureHandler());
			loginThread.start();
		} catch (Exception e) {
		}
		camera.startPreview();
	}
	class LoginFailureHandler implements Runnable {
		public void run() {
			DetectFace();
			Message message = new Message();
			message.what = 1;
			updateUI.sendMessage(message);
		}
	}
	public void surfaceCreated(SurfaceHolder holder) {
		try {
			camera = Camera.open(camera());// 摄像头的初始化
			camera.setPreviewDisplay(holder);
			dataTimer.schedule(new TimerTask() {
				@Override
				public void run() {
					Message message = new Message();
					message.what = 1;
					dataHandler.sendMessage(message);
				}
			}, 0, 2000);
		} catch (Exception e) {
		}
	}
	public void surfaceChanged(SurfaceHolder holder, int format, int width,
			int height) {
		Camera.Parameters parameters = camera.getParameters();
		// parameters.setPreviewSize(1280, 800);
		camera.setParameters(parameters);// 设置参数
		int flag = getWindowManager().getDefaultDisplay().getRotation();
		angle = 450 - flag * 90;
		if (angle >= 360) {
			angle = angle - 360;
		}
		camera.setDisplayOrientation(angle);
		camera.startPreview();// 开始预览
	}
	public void surfaceDestroyed(SurfaceHolder holder) {
		camera.setPreviewCallback(null);
		camera.stopPreview();
		camera.release();
		camera = null;
	}
	// private void data2file(byte[] w, String fileName) throws Exception {//
	// 将二进制数据转换为文件的函数
	// FileOutputStream out = null;
	// try {
	// out = new FileOutputStream(fileName);
	// out.write(w);
	// out.close();
	// } catch (Exception e) {
	// if (out != null)
	// out.close();
	// throw e;
	// }
	// }
	public void DetectFace() {
		Mat image = Highgui.imread(FACE);
		MatOfRect faceDetections = new MatOfRect();
		mJavaDetector.detectMultiScale(image, faceDetections);
		int k = 0;
		for (Rect rect : faceDetections.toArray()) {
			Core.rectangle(image, new Point(rect.x, rect.y), new Point(rect.x
					+ rect.width, rect.y + rect.height), new Scalar(0, 255, 0));
			// 把检测到的人脸重新定义大小后保存成文件
			Mat sub = image.submat(rect);
			Mat mat = new Mat();
			Size size = new Size(100, 100);
			Imgproc.resize(sub, mat, size);
			Highgui.imwrite(FACEDONE, mat);
			k++;
		}
		if (k == 0) {
			cu.writePhoto(BitmapFactory.decodeFile(FACE), 100, 100, FACEDONE);
		}
	}
	public int Identification() {
		FaceRecognizer fr = createFisherFaceRecognizer();
		MatVector mv = new MatVector(faceList.size());
		CvMat cvMat = CvMat.create(faceList.size(), 1, CV_32SC1);
		for (int i = 0; i < faceList.size(); i++) {
			IplImage img = cvLoadImage(faceList.get(i).getPath(),
					CV_LOAD_IMAGE_GRAYSCALE);
			mv.put(i, img);
			cvMat.put(i, 0, i);
		}
		fr.train(mv, cvMat);
		IplImage testImage = cvLoadImage(
				Environment.getExternalStorageDirectory()
						+ "/FaceDetect/faceDone.jpg", CV_LOAD_IMAGE_GRAYSCALE);
		return fr.predict(testImage);
	}
	public void LoadFaceData() {
		File[] files = new File("/sdcard/FaceData/").listFiles();
		File f;
		String id;
		String name;
		faceList.clear();
		for (int i = 0; i < files.length; i++) {
			f = files[i];
			if (!f.canRead()) {
				return;
			}
			if (f.isFile()) {
				id = f.getName().split("_")[0];
				name = f.getName().substring(f.getName().indexOf("_") + 1,
						f.getName().length() - 4);
				faceList.add(new FacePojo(id, name, Environment
						.getExternalStorageDirectory()
						+ "/FaceData/"
						+ f.getName()));
			}
		}
	}
	public double CmpPic(String path) {
		int l_bins = 20;
		int hist_size[] = { l_bins };
		float v_ranges[] = { 0, 100 };
		float ranges[][] = { v_ranges };
		IplImage Image1 = cvLoadImage(Environment.getExternalStorageDirectory()
				+ "/FaceDetect/faceDone.jpg", CV_LOAD_IMAGE_GRAYSCALE);
		IplImage Image2 = cvLoadImage(path, CV_LOAD_IMAGE_GRAYSCALE);
		IplImage imageArr1[] = { Image1 };
		IplImage imageArr2[] = { Image2 };
		CvHistogram Histogram1 = CvHistogram.create(1, hist_size,
				CV_HIST_ARRAY, ranges, 1);
		CvHistogram Histogram2 = CvHistogram.create(1, hist_size,
				CV_HIST_ARRAY, ranges, 1);
		cvCalcHist(imageArr1, Histogram1, 0, null);
		cvCalcHist(imageArr2, Histogram2, 0, null);
		cvNormalizeHist(Histogram1, 100.0);
		cvNormalizeHist(Histogram2, 100.0);
		return cvCompareHist(Histogram1, Histogram2, CV_COMP_CORREL);
	}
	public void writePhoto(Bitmap bmp) {
		File file = new File("/sdcard/FaceDetect/faceDone.jpg");
		try {
			Bitmap bm = Bitmap.createBitmap(bmp, 0, 0, 100, 100);
			BufferedOutputStream bos = new BufferedOutputStream(
					new FileOutputStream(file));
			if (bm.compress(Bitmap.CompressFormat.JPEG, 100, bos)) {
				bos.flush();
				bos.close();
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	public void timeDown() {
		LayoutInflater inflater = getLayoutInflater();
		View layout = inflater.inflate(R.layout.custom,
				(ViewGroup) findViewById(R.id.llToast));
		ImageView image = (ImageView) layout.findViewById(R.id.tvImageToast);
		image.setImageResource(iconArr[t--]);
		toast = new Toast(getApplicationContext());
		toast.setDuration(Toast.LENGTH_SHORT);
		toast.setView(layout);
		toast.show();
	}
	public int camera() {
		Camera.CameraInfo cameraInfo = new Camera.CameraInfo();
		cameraCount = Camera.getNumberOfCameras(); // get cameras number
		for (int camIdx = 0; camIdx < cameraCount; camIdx++) {
			Camera.getCameraInfo(camIdx, cameraInfo);
			if (cameraInfo.facing == Camera.CameraInfo.CAMERA_FACING_FRONT) {
				try {
					return camIdx;
				} catch (RuntimeException e) {
					e.printStackTrace();
				}
			}
		}
		return 1;
	}
}
```
