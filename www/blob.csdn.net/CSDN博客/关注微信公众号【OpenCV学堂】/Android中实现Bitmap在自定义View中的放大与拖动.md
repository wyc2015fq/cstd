# Android中实现Bitmap在自定义View中的放大与拖动 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2015年11月04日 01:46:30[gloomyfish](https://me.csdn.net/jia20003)阅读数：6791







**一基本实现思路：**

基于View类实现自定义View –MyImageView类。在使用View的Activity类中完成OnTouchListener接口，实现对MotionEvent事件的监听与处理，常见的MotionEvent事件如下：

ACTION_DOWN事件，记录平移开始点
ACTION_UP事件，结束平移事件处理
ACTION_MOVE事件，记录平移点，计算与开始点距离，实现Bitmap平移，在多点触控时候，计算两点之间的距离，实现图像放大
ACTION_POINTER_DOWN事件，计算两点之间的距离，作为初始距离，实现图像手势放大时候使用。
ACTION_POINTER_UP事件，结束两点触控放大图像处理

**放大与拖动**
基于单点触控实现Bitmap对象在View上的拖动、并且检测View的边缘，防止拖动过界。基于两个点触控实现Bitmap对象在View上的放大、并且检测放大倍数。基于Matrix对象实现对Bitmap在View上放大与平移变换，Matrix对象是android中实现图像几何变换的矩阵，支持平移、放大、缩小、错切、旋转等常见操作。

**Bitmap对象在View中的更新与显示**
通过重载onDraw方法，使用canvas实现绘制Bitmap对象、通过view.invalidate()方法实现View的刷新。

**MyImageView类的重要方法说明：**
initParameters()初始化所有需要用到的参数
setStartPoint()设置图像平移的开始点坐标

setMovePoint()设置图像平移的移动点坐标，然后集合开始点位置，计算它们之间的距离，从而得到Bitmap对象需要平移的两个参数值sx、sy。其中还包括保证图像不会越过View边界的检查代码。

savePreviousResult()保存当前的平移数据，下次可以继续在次基础上平移Bitmap对象。

zoomIn()根据两个点之间的欧几里德距离，通过初始距离比较，得到放大比例，实现Bitmap在View对象上的放大




**Matrix中关于放大与平移的API**

Matrix.postScale方法与Matrix.postTranslate方法可以不改变Bitmap对象本身实现平移与放大。




二：代码实现

自定义View类使用xml布局如下：



```
<RelativeLayout xmlns:android="http://schemas.android.com/apk/res/android"
    xmlns:tools="http://schemas.android.com/tools"
    android:layout_width="match_parent"
    android:layout_height="match_parent"
    android:paddingBottom="@dimen/activity_vertical_margin"
    android:paddingLeft="@dimen/activity_horizontal_margin"
    android:paddingRight="@dimen/activity_horizontal_margin"
    android:paddingTop="@dimen/activity_vertical_margin"
    tools:context=".MainActivity" >

    <com.example.matrixdemo.MyImageView
        android:id="@+id/myView"
        android:layout_width="fill_parent"
        android:layout_height="fill_parent"
        android:text="@string/hello_world" />

</RelativeLayout>
```
自定义View实现代码如下：




```java
package com.example.matrixdemo;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Matrix;
import android.graphics.Paint;
import android.graphics.Paint.Style;
import android.graphics.Point;
import android.graphics.Rect;
import android.util.AttributeSet;
import android.view.View;

public class MyImageView extends View {
	private Paint mPaint;
	private Bitmap bitmap;
	private Matrix matrix;
	
	// 平移开始点与移动点
	private Point startPoint;
	private Point movePoint;
	private float initDistance;

	// 记录当前平移距离
	private int sx;
	private int sy;
	
	// 保存平移状态
	private int oldsx;
	private int oldsy;
	
	// scale rate
	private float widthRate;
	private float heightRate;
	
	public MyImageView(Context context) {
		super(context);
	}
	
	public MyImageView(Context context, AttributeSet attrs) {
		super(context, attrs);
	}
	
	public void setBitmap(Bitmap bitmap) {
		this.bitmap = bitmap;
	}
	
	private void initParameters() {
		// 初始化画笔
		mPaint = new Paint();
		mPaint.setColor(Color.BLACK);
		matrix = new Matrix();
		if(bitmap != null)
		{
			float iw = bitmap.getWidth();
			float ih = bitmap.getHeight();
			float width = this.getWidth();
			float height = this.getHeight();
			// 初始放缩比率
			widthRate = width / iw;
			heightRate = height / ih;
		}
		
		sx = 0;
		sy = 0;
		
		oldsx = 0;
		oldsy = 0;
		
	}
	
	public void setStartPoint(Point startPoint) {
		this.startPoint = startPoint;
	}
	
	public void setInitDistance(float initDistance) {
		this.initDistance = initDistance;
	}
	
	public void zoomIn(float distance)
	{
		float rate = distance / this.initDistance;
		float iw = bitmap.getWidth();
		float ih = bitmap.getHeight();
		float width = this.getWidth();
		float height = this.getHeight();
		// get scale rate
		widthRate = (width / iw ) * rate;
		heightRate = (height / ih) * rate;
		
		// make it same as view size
		float iwr = (width / iw );
		float ihr = (height / ih);
		if(iwr >= widthRate)
		{
			widthRate = (width / iw );
		}
		if(ihr >= heightRate)
		{
			heightRate = (height / ih);
		}
		
		// go to center
		oldsx = (int)((width - widthRate * iw) / 2);
		oldsy = (int)((height - heightRate * ih) / 2);
	}

	public void setMovePoint(Point movePoint) {
		this.movePoint = movePoint;
		sx = this.movePoint.x - this.startPoint.x;
		sy = this.movePoint.y - this.startPoint.y;
		
		float iw = bitmap.getWidth();
		float ih = bitmap.getHeight();
		
		// 检测边缘
		int deltax = (int)((widthRate * iw) - this.getWidth());
		int deltay = (int)((heightRate * ih) - this.getHeight());
		if((sx + this.oldsx) >= 0)
		{
			this.oldsx = 0;
			sx = 0;
		}
		else if((sx + this.oldsx) <= -deltax)
		{
			this.oldsx = -deltax;
			sx = 0;
		}
		
		if((sy + this.oldsy) >= 0)
		{
			this.oldsy = 0;
			this.sy = 0;
		}
		else if((sy + this.oldsy) <= -deltay)
		{
			this.oldsy = -deltay;
			this.sy = 0;
		}
		
		float width = this.getWidth();
		
		// 初始放缩比率
		float iwr = width / iw;
		if(iwr == widthRate)
		{
			sx = 0;
			sy = 0;
			oldsx = 0;
			oldsy = 0;
		}
	}
	
	public void savePreviousResult()
	{
		this.oldsx = this.sx + this.oldsx;
		this.oldsy = this.sy + this.oldsy;
		
		// zero
		sx = 0;
		sy = 0;
	}

	@Override
	protected void onDraw(Canvas canvas) {
		if(matrix == null)
		{
			initParameters();
		}
		if(bitmap != null)
		{
			matrix.reset();
			matrix.postScale(widthRate, heightRate);
			matrix.postTranslate(oldsx+sx, oldsy + sy);
			canvas.drawBitmap(bitmap, matrix, mPaint);
		}
		else
		{
			// fill rect
			Rect rect = new Rect(0, 0, getWidth(), getHeight());
			mPaint.setAntiAlias(true);
			mPaint.setColor(Color.BLACK);
			mPaint.setStyle(Style.FILL_AND_STROKE);
			canvas.drawRect(rect, mPaint);
		}
	}
}
```
Activity类中实现对View的OnTouchListener监听与MotionEvent事件处理的代码如下：




```java
package com.example.matrixdemo;

import android.app.Activity;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Point;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnTouchListener;

public class MainActivity extends Activity implements OnTouchListener {

	public static final int SCALE_MODE = 4;
	public static final int TRANSLATION_MODE = 2;
	public static final int NULL_MODE = 1;
	private MyImageView myView;
	private int mode;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		startMyImageView();
	}

	private void startMyImageView() {
		myView = (MyImageView) this.findViewById(R.id.myView);
		Bitmap bitmap = BitmapFactory.decodeResource(this.getResources(),
				R.drawable.flower_001);
		myView.setBitmap(bitmap);
		myView.setOnTouchListener(this);
		myView.invalidate();
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

	@Override
	public boolean onTouch(View view, MotionEvent event) {
		Log.i("touch event","touch x = " + event.getX());
		switch (MotionEvent.ACTION_MASK & event.getAction()) 
		{
			case MotionEvent.ACTION_DOWN:
				mode = TRANSLATION_MODE;
				myView.setStartPoint(new Point((int)event.getX(), (int)event.getY()));
				break;
			case MotionEvent.ACTION_POINTER_UP:
			case MotionEvent.ACTION_OUTSIDE:
			case MotionEvent.ACTION_UP:
				mode = NULL_MODE;
				myView.savePreviousResult();
				break;
			case MotionEvent.ACTION_POINTER_DOWN:
				mode = SCALE_MODE;
				myView.setInitDistance(calculateDistance(event));
				break;
			case MotionEvent.ACTION_MOVE:
				if(mode == SCALE_MODE)
				{
					float dis = calculateDistance(event);
					myView.zoomIn(dis);
				}
				else if(mode == TRANSLATION_MODE)
				{
					myView.setMovePoint(new Point((int)event.getX(), (int)event.getY()));
				}
				else
				{
					Log.i("unknow mode tag","do nothing......");
				}
				break;
		}
		myView.invalidate();
		return true;
	}

	private float calculateDistance(MotionEvent event) {
		float dx = event.getX(0) - event.getX(1);
		float dy = event.getY(0)  - event.getY(1);
		float distance = (float)Math.sqrt(dx*dx + dy*dy);
		return distance;
	}

}
```
三：运行效果如下


![](https://img-blog.csdn.net/20151104014530381)




