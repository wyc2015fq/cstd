# android上FragmentTabHost实现自定义Tab Indicator - 关注微信公众号【OpenCV学堂】 - CSDN博客





2015年12月13日 15:05:38[gloomyfish](https://me.csdn.net/jia20003)阅读数：5633








最近一直在做安卓开发，发现Tab布局可以用FragmentTabHost来实现，唯一不好的就是不能实现带图标的tabindicator, V4版本中的虽然API有支持，但是无论怎么设置Drawable对象都不起作用，所以被逼无赖，发现indicator可以支持传进一个View做tabtitle，于是经过一番各种坑之后，我做了一个自定义的Tab indicator，可以实现切换提示、显示带图标的tabtitle，同时支持滑动切换到不同Tab。首先来看一下效果吧。

![](https://img-blog.csdn.net/20151213150251763)

我的实现思路是这样的：

第一步，当然是建立android V4版本的FragmentTabHost的布局XML文件，一般情况下是不建议改动的，我是直接copy官网上的。文件内容如下：



```
<?xml version="1.0" encoding="utf-8"?>
<android.support.v4.app.FragmentTabHost
    xmlns:android="http://schemas.android.com/apk/res/android"
    android:id="@android:id/tabhost"
    android:layout_width="match_parent"
    android:layout_height="match_parent" >

    <LinearLayout
        android:layout_width="match_parent"
        android:layout_height="match_parent"
        android:orientation="vertical" >

        <TabWidget
            android:id="@android:id/tabs"
            android:layout_width="fill_parent"
            android:layout_height="wrap_content"
            android:layout_gravity="bottom"/>

        <FrameLayout
            android:id="@android:id/tabcontent"
            android:layout_width="fill_parent"
            android:layout_height="0dp"
            android:layout_weight="1" />
    </LinearLayout>

</android.support.v4.app.FragmentTabHost>
```
第二步，建立自定义的tab-indicator XML布局文件，实现自定义tab title View，XML文件内容如下：


```
<?xml version="1.0" encoding="utf-8"?>
<LinearLayout xmlns:android="http://schemas.android.com/apk/res/android"
    android:layout_width="fill_parent"
    android:layout_height="fill_parent"
    android:orientation="horizontal" >

    <LinearLayout
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:orientation="vertical" >

        <LinearLayout
            android:layout_width="wrap_content"
            android:layout_height="wrap_content"
            android:orientation="horizontal" >

            <ImageView
                android:id="@+id/tab_icon_1"
                android:layout_width="wrap_content"
                android:layout_height="wrap_content"
                android:layout_margin="5dp"
                android:contentDescription="icon" />

            <TextView
                android:id="@+id/tab_name_1"
                android:layout_width="wrap_content"
                android:layout_height="wrap_content"
                android:textColor="#000"
                android:textSize="24sp" />
        </LinearLayout>

        <View
            android:id="@+id/tab_border_line_1"
            android:layout_width="fill_parent"
            android:layout_height="3dp"
            android:background="#ff0000" />
    </LinearLayout>

    <LinearLayout
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:orientation="vertical" >

        <LinearLayout
            android:layout_width="wrap_content"
            android:layout_height="wrap_content"
            android:orientation="horizontal" >

            <ImageView
                android:id="@+id/tab_icon_2"
                android:layout_width="wrap_content"
                android:layout_height="wrap_content"
                android:layout_margin="5dp"
                android:contentDescription="icon" />

            <TextView
                android:id="@+id/tab_name_2"
                android:layout_width="wrap_content"
                android:layout_height="wrap_content"
                android:textColor="#000"
                android:textSize="24sp" />
        </LinearLayout>

        <View
            android:id="@+id/tab_border_line_2"
            android:layout_width="fill_parent"
            android:layout_marginTop="1dp"
            android:layout_height="3dp"
            android:background="#ff0000" />
    </LinearLayout>

    <LinearLayout
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:orientation="vertical" >

        <LinearLayout
            android:layout_width="wrap_content"
            android:layout_height="wrap_content"
            android:orientation="horizontal" >

            <ImageView
                android:id="@+id/tab_icon_3"
                android:layout_width="wrap_content"
                android:layout_height="wrap_content"
                android:layout_margin="5dp"
                android:contentDescription="icon" />

            <TextView
                android:id="@+id/tab_name_3"
                android:layout_width="wrap_content"
                android:layout_height="wrap_content"
                android:textColor="#000"
                android:textSize="24sp" />
        </LinearLayout>

        <View
            android:id="@+id/tab_border_line_3"
            android:layout_width="fill_parent"
            android:layout_marginTop="0.5dp"
            android:layout_height="3dp"
            android:background="#ff0000" />
    </LinearLayout>

</LinearLayout>
```

第四步：当然是在MainActivity的onCreate方法中实现FragmentTabHost对象的初始化，建立一个真正的tab布局界面。代码实现如下：



```java
tabHost = (FragmentTabHost) findViewById(android.R.id.tabhost);
tabHost.setup(this, getSupportFragmentManager(), android.R.id.tabcontent);
```

第五步：也是最恶搞的一步，就是把View传到V4的每个tab的indicator中去，这里要特别说明一下，其实起作用的只有第一个设置进去的View，其它虽然是必须的，但是无需任何设置。否则FragmentTabHost将无法正常工作。这个是我吐血得到的教训：代码如下：



```java
ImageView piv = (ImageView)llView.findViewById(R.id.tab_icon_1);
TextView ptv = (TextView)llView.findViewById(R.id.tab_name_1);
ptv.setText("找人");
piv.setImageBitmap(personIcon);

piv = (ImageView)llView.findViewById(R.id.tab_icon_2);
ptv = (TextView)llView.findViewById(R.id.tab_name_2);
ptv.setText("圈子");
piv.setImageBitmap(circleIcon);

piv = (ImageView)llView.findViewById(R.id.tab_icon_3);
ptv = (TextView)llView.findViewById(R.id.tab_name_3);
ptv.setText("博客");
piv.setImageBitmap(blogIcon);

// 必须有这个，但是没有鸟用
View view2 = getLayoutInflater().inflate(R.layout.tab_indicator, (ViewGroup) mRoot.findViewById(android.R.id.tabs), false);
View view3 = getLayoutInflater().inflate(R.layout.tab_indicator, (ViewGroup) mRoot.findViewById(android.R.id.tabs), false);

Log.i("my-debug", "go here to setup tab content");
tabHost.addTab(
		tabHost.newTabSpec("找人").setIndicator(llView),
		PersonFragment.class, null);
tabHost.addTab(
		tabHost.newTabSpec("圈子").setIndicator(view2),
		CircleFragment.class, null);
tabHost.addTab(
		tabHost.newTabSpec("博客").setIndicator(view3),
		BlogFragment.class, null);
```
第六步，添加手势侧滑支持与现实默认为第一个tab上的内容，实现代码如下：


```java
// 显示第一tab内容Fragment
tabHost.setCurrentTab(0);
View v = (View)llView.findViewById(R.id.tab_border_line_1);
v.setBackgroundColor(Color.BLUE);
mDetector = new GestureDetectorCompat(this, new MyGestureListener());
```
其中手势侧滑支持基于SimpleOnGestureListener类实现，同时要重载onTouchEvent方法才可以工作。
完整的MainActivity的代码如下：



```java
package com.example.gesturedemo;

import android.annotation.SuppressLint;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Color;
import android.os.Bundle;
import android.support.v4.app.FragmentActivity;
import android.support.v4.app.FragmentTabHost;
import android.support.v4.view.GestureDetectorCompat;
import android.util.Log;
import android.view.GestureDetector;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

@SuppressLint("NewApi")
public class MainActivity extends FragmentActivity {
	private FragmentTabHost tabHost;
	private GestureDetectorCompat mDetector;
	private View llView;
	private static int tabIndex = 0;
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);

		// get tabhost and setup it
		tabHost = (FragmentTabHost) findViewById(android.R.id.tabhost);
		tabHost.setup(this, getSupportFragmentManager(), android.R.id.tabcontent);
		
		// load icons
		Bitmap personIcon = BitmapFactory.decodeResource(this.getResources(), R.drawable.person);
		Bitmap circleIcon = BitmapFactory.decodeResource(this.getResources(), R.drawable.circle);
		Bitmap blogIcon = BitmapFactory.decodeResource(this.getResources(), R.drawable.blog);
		
		// add tabs
		Log.i("my-debug", "go here to setup tab header");
		View mRoot = getLayoutInflater().inflate(R.layout.activity_main, null);
		llView = getLayoutInflater().inflate(R.layout.tab_indicator, (ViewGroup) mRoot.findViewById(android.R.id.tabs), false);
		
		ImageView piv = (ImageView)llView.findViewById(R.id.tab_icon_1);
		TextView ptv = (TextView)llView.findViewById(R.id.tab_name_1);
		ptv.setText("找人");
		piv.setImageBitmap(personIcon);
		
		piv = (ImageView)llView.findViewById(R.id.tab_icon_2);
		ptv = (TextView)llView.findViewById(R.id.tab_name_2);
		ptv.setText("圈子");
		piv.setImageBitmap(circleIcon);
		
		piv = (ImageView)llView.findViewById(R.id.tab_icon_3);
		ptv = (TextView)llView.findViewById(R.id.tab_name_3);
		ptv.setText("博客");
		piv.setImageBitmap(blogIcon);
		
		// 必须有这个，但是没有鸟用
		View view2 = getLayoutInflater().inflate(R.layout.tab_indicator, (ViewGroup) mRoot.findViewById(android.R.id.tabs), false);
		View view3 = getLayoutInflater().inflate(R.layout.tab_indicator, (ViewGroup) mRoot.findViewById(android.R.id.tabs), false);
		
		Log.i("my-debug", "go here to setup tab content");
		tabHost.addTab(
				tabHost.newTabSpec("找人").setIndicator(llView),
				PersonFragment.class, null);
		tabHost.addTab(
				tabHost.newTabSpec("圈子").setIndicator(view2),
				CircleFragment.class, null);
		tabHost.addTab(
				tabHost.newTabSpec("博客").setIndicator(view3),
				BlogFragment.class, null);

		// 显示第一tab内容Fragment
		tabHost.setCurrentTab(0);
		View v = (View)llView.findViewById(R.id.tab_border_line_1);
		v.setBackgroundColor(Color.BLUE);
		mDetector = new GestureDetectorCompat(this, new MyGestureListener());
	}
	
	@Override
	public boolean onTouchEvent(MotionEvent event) {
		this.mDetector.onTouchEvent(event);
		return super.onTouchEvent(event);
	}
	
	public void showNextTab()
	{
		tabIndex++;
		if(tabIndex >= 3)
		{
			tabIndex = 0;
		}
		tabHost.setCurrentTab(tabIndex);
		View v1 = (View)llView.findViewById(R.id.tab_border_line_1);
		View v2 = (View)llView.findViewById(R.id.tab_border_line_2);
		View v3 = (View)llView.findViewById(R.id.tab_border_line_3);
		v1.setBackgroundColor(Color.RED);
		v2.setBackgroundColor(Color.RED);
		v3.setBackgroundColor(Color.RED);
		if(tabIndex == 0) {
			v1.setBackgroundColor(Color.BLUE);
		}
		else if(tabIndex == 1) {
			v2.setBackgroundColor(Color.BLUE);
		}
		else if(tabIndex == 2) {
			v3.setBackgroundColor(Color.BLUE);
		}
	}
	
	class MyGestureListener extends GestureDetector.SimpleOnGestureListener {
		private static final String DEBUG_TAG = "Gestures";

		@Override
		public boolean onDown(MotionEvent event) {
			Log.d(DEBUG_TAG, "onDown: " + event.getX());
			return true;
		}

		@Override
		public boolean onFling(MotionEvent event1, MotionEvent event2, float velocityX, float velocityY) {
			Log.d(DEBUG_TAG, "onFling: " + event1.getX());
			float deltaX = event1.getX() - event2.getX();
			Log.i("gesture", "deltaX : " + deltaX);
			if(deltaX > 20) {   
				showNextTab();
			}
			else {
				Toast.makeText(getApplicationContext(), "从右向左滑动开始应用...", Toast.LENGTH_SHORT).show();   
			}
			return true;
		}
	}

}
```
使用的三个Fragment程序代码基本一样，无需一一给出，这里只给出PersonFragment类的实现代码如下：
```java
package com.example.gesturedemo;

import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

public class PersonFragment extends Fragment {

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        View v = inflater.inflate(R.layout.common_content, container, false);
        TextView tv = (TextView) v.findViewById(R.id.textView1);
        tv.setText("相逢未必曾相识");
        return v;
    }
}
```
使用的布局文件XML内容如下：


```
<?xml version="1.0" encoding="utf-8"?>
<LinearLayout xmlns:android="http://schemas.android.com/apk/res/android"
    android:layout_width="fill_parent"
    android:layout_height="fill_parent"
    android:orientation="vertical" >

    <TextView
        android:id="@+id/textView1"
        android:layout_width="fill_parent"
        android:layout_height="wrap_content"
        android:textSize="36sp"
        android:gravity="center"
        android:text="tabhost" />

</LinearLayout>
```



