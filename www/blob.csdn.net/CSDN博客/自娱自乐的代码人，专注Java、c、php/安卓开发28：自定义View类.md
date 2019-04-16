# 安卓开发28：自定义View类 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2013年08月24日 16:18:11[initphp](https://me.csdn.net/initphp)阅读数：1770
个人分类：[安卓开发笔记系列](https://blog.csdn.net/initphp/article/category/1439903)

所属专栏：[安卓开发](https://blog.csdn.net/column/details/androidgo.html)









### 自定义View类

通过自定义View类，可以自定义复杂的，按照自己需求的控件。

### 一个简单的例子

mainActivity.java 这个里面就是最普通的代码，但是给自定义的控件加上了一个onclick的时间



```java
package com.example.test.com;


import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.Window;

public class MainActivity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        setContentView(R.layout.main);
        
        MyView myView = (MyView) findViewById(R.id.myView);
        myView.setOnClickListener(new OnClickListener() {
            
            @Override
            public void onClick(View v) {
                Log.d("asdasd", ":asdasdasd");
            }
        });
    }
    
  

}
```


main.xml





```
<RelativeLayout xmlns:android="http://schemas.android.com/apk/res/android"
    android:layout_width="700dp"
    android:layout_height="300dp"
    android:orientation="horizontal"
    android:padding="10dp" >

	<!-- 自定义View -->
    <com.example.test.com.MyView
        android:id="@+id/myView"
        android:layout_width="fill_parent"
        android:layout_height="fill_parent" />

</RelativeLayout>
```


MyView.java





```java
package com.example.test.com;


import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Paint.Style;
import android.graphics.Rect;
import android.util.AttributeSet;
import android.view.View;

public class MyView extends View {

    private Paint mPaint;

    public MyView(Context context) {
        super(context);
    }

    public MyView(Context context, AttributeSet attrs) {
        super(context, attrs);
        setFocusable(true);
        setClickable(true); //设置是否能点击
    }

    /**
     * 通过canvas 画控件
     */
    @Override
    public void onDraw(Canvas canvas) {
        super.onDraw(canvas);
        mPaint = new Paint();
        //设置画笔颜色 
        mPaint.setColor(Color.RED);
        //设置填充 
        mPaint.setStyle(Style.FILL);
        //画一个矩形,前俩个是矩形左上角坐标，后面俩个是右下角坐标 
        canvas.drawRect(new Rect(10, 10, 100, 100), mPaint);
        mPaint.setColor(Color.BLUE);
        //绘制文字 
        canvas.drawText("Hello WORLD", 10, 110, mPaint);
    }

}
```


结果：



![](https://img-blog.csdn.net/20130824161802515?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5pdHBocA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





### 自定义复合控件

mainActivity



```java
package com.example.test.com;


import android.app.Activity;
import android.os.Bundle;
import android.view.ViewGroup;
import android.view.Window;
import android.widget.Button;

public class MainActivity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        requestWindowFeature(Window.FEATURE_NO_TITLE);

        ViewGroup viewGroup = (ViewGroup) getLayoutInflater().inflate(R.layout.test_f, null);
        setContentView(viewGroup);
        getLayoutInflater();
        
        //调用TestView复合控件中的对象
        Button btn = (Button) findViewById(R.id.button1);
        btn.setText("HELLO WORLD");

    }



}
```



main.xml


```
<RelativeLayout xmlns:android="http://schemas.android.com/apk/res/android"
    android:id="@+id/myView"
    android:layout_width="700dp"
    android:layout_height="300dp"
    android:orientation="horizontal"
    android:padding="10dp" >

    <!-- 自定义复合控件 -->
    <com.example.test.com.TestView
        android:id="@+id/textView1"
        android:layout_width="fill_parent"
        android:layout_height="fill_parent" />

</RelativeLayout>
```


TestView
```java
package com.example.test.com;

import android.content.Context;
import android.view.LayoutInflater;
import android.widget.RelativeLayout;

/**
 * 一个简单的复合控件
 * @author zhuli.zhul
 * @date 2013 2013-8-27 下午4:22:10
 */
public class TestView  extends RelativeLayout{

    public TestView(Context context) {
        super(context);
        LayoutInflater i = (LayoutInflater) getContext().getSystemService(Context.LAYOUT_INFLATER_SERVICE);
        i.inflate(R.layout.test_f, this); //xml
    }



}
```



test_f.xml


```
<RelativeLayout xmlns:android="http://schemas.android.com/apk/res/android"
    android:layout_width="700dp"
    android:layout_height="300dp"
    android:orientation="horizontal"
    android:padding="10dp" >

    <Button
        android:id="@+id/button1"
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:layout_alignParentLeft="true"
        android:layout_centerVertical="true"
        android:layout_marginLeft="148dp"
        android:text="Button2" />

    <Button
        android:id="@+id/button1"
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:layout_alignParentLeft="true"
        android:layout_alignParentTop="true"
        android:layout_marginLeft="64dp"
        android:layout_marginTop="67dp"
        android:text="Button1" />

</RelativeLayout>
```

![](https://img-blog.csdn.net/20130827162726906?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5pdHBocA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



### include方法导入公用xml控件，更加简单



```
<!-- include的方法，更加简单 -->
  	<include layout="@layout/test_f"/>
```





