# Android高手进阶教程(四)之----Android 中自定义属性(attr.xml,TypedArray)的使用! - xqhrs232的专栏 - CSDN博客
2012年04月05日 14:50:23[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：483标签：[android																[layout																[encoding																[xml																[float																[扩展](https://so.csdn.net/so/search/s.do?q=扩展&t=blog)](https://so.csdn.net/so/search/s.do?q=float&t=blog)](https://so.csdn.net/so/search/s.do?q=xml&t=blog)](https://so.csdn.net/so/search/s.do?q=encoding&t=blog)](https://so.csdn.net/so/search/s.do?q=layout&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://www.cnblogs.com/thomas-lee/archive/2011/04/17/TypedArray.html](http://www.cnblogs.com/thomas-lee/archive/2011/04/17/TypedArray.html)
相关网帖
1.[Android中自定义属性的使用------res/values文件下定义一个attrs.xml](http://blog.csdn.net/liguangzhenghi/article/details/7285956)----[http://blog.csdn.net/liguangzhenghi/article/details/7285956](http://blog.csdn.net/liguangzhenghi/article/details/7285956)
2.[Android中自定义属性（attrs.xml，TypedArray）的使用](http://www.oschina.net/code/snippet_163910_6283)----[http://www.oschina.net/code/snippet_163910_6283](http://www.oschina.net/code/snippet_163910_6283)
3.[attrs.xml文件中属性类型format值的格式](http://www.oschina.net/question/54100_32467)----[http://www.oschina.net/question/54100_32467](http://www.oschina.net/question/54100_32467)
4.attrs.xml - Android开发社区----[http://www.oschina.net/android/125/attrs-xml](http://www.oschina.net/android/125/attrs-xml)
今天我们的教程是根据前面一节扩展进行的，如果你没有看,请点击 [**Android高手进阶教程(三)**](http://blog.csdn.net/Android_Tutor/archive/2010/04/18/5499731.aspx)查看第三课,这样跟容易方便你的理解!
在**xml** 文件里定义控件的属性，我们已经习惯了**android:attrs=""** ,那么我们能不能定义自己的属性能，比如:**test:attrs=""** 呢？答案是肯定的.
好了我就不卖关子了，直接进入主题。大致以下步骤:
**一、** 在**res/values** 文件下定义一个**attrs.xml** 文件.代码如下:
[view
 plain](http://blog.csdn.net/Android_Tutor/archive/2010/04/20/5508615.aspx#)[copy
 to clipboard](http://blog.csdn.net/Android_Tutor/archive/2010/04/20/5508615.aspx#)[print](http://blog.csdn.net/Android_Tutor/archive/2010/04/20/5508615.aspx#)[?](http://blog.csdn.net/Android_Tutor/archive/2010/04/20/5508615.aspx#)
- 一、在res/values文件下定义一个attrs.xml文件.代码如下:  
- <?xml version="1.0" encoding="utf-8"?>  
- <resources>  
-     <declare-styleable name="MyView">  
-         <attr name="textColor" format="color" />  
-         <attr name="textSize" format="dimension" />  
-     </declare-styleable>  
- </resources>  
**二、** 我们在**MyView.java** 代码修改如下，其中下面的构造方法是重点，我们获取定义的属性我们**R.sytleable.MyView_textColor,** 获取方法中后面通常设定默认值(**float textSize = a.getDimension(R.styleable.MyView_textSize, 36 );** )**，** 防止我们在**xml** 文件中没有定义.从而使用默认值!
获取，**MyView** 就是定义在**<declare-styleable name="MyView "></declare-styleable>** 里的 名字,获取里面属性用** 名字_ 属性** 连接起来就可以.**TypedArray** 通常最后调用** .recycle()** 方法，为了保持以后使用该属性一致性！
[view
 plain](http://blog.csdn.net/Android_Tutor/archive/2010/04/20/5508615.aspx#)[copy
 to clipboard](http://blog.csdn.net/Android_Tutor/archive/2010/04/20/5508615.aspx#)[print](http://blog.csdn.net/Android_Tutor/archive/2010/04/20/5508615.aspx#)[?](http://blog.csdn.net/Android_Tutor/archive/2010/04/20/5508615.aspx#)
- public MyView(Context context,AttributeSet attrs)  
-     {  
- super(context,attrs);  
-         mPaint = new Paint();  
- 
-         TypedArray a = context.obtainStyledAttributes(attrs,  
-                 R.styleable.MyView);  
- 
- int textColor = a.getColor(R.styleable.MyView_textColor,  
- 0XFFFFFFFF);  
- float textSize = a.getDimension(R.styleable.MyView_textSize, 36);  
- 
-         mPaint.setTextSize(textSize);  
-         mPaint.setColor(textColor);  
- 
-         a.recycle();  
-     }  
**MyView.java** 全部代码如下:
[view
 plain](http://blog.csdn.net/Android_Tutor/archive/2010/04/20/5508615.aspx#)[copy
 to clipboard](http://blog.csdn.net/Android_Tutor/archive/2010/04/20/5508615.aspx#)[print](http://blog.csdn.net/Android_Tutor/archive/2010/04/20/5508615.aspx#)[?](http://blog.csdn.net/Android_Tutor/archive/2010/04/20/5508615.aspx#)
- package com.android.tutor;  
- import android.content.Context;  
- import android.content.res.TypedArray;  
- import android.graphics.Canvas;  
- import android.graphics.Color;  
- import android.graphics.Paint;  
- import android.graphics.Rect;  
- import android.graphics.Paint.Style;  
- import android.util.AttributeSet;  
- import android.view.View;  
- publicclass MyView extends View {  
- private Paint mPaint;  
- private Context mContext;  
- privatestaticfinal String mString = "Welcome to Mr Wei's blog";  
- 
- public MyView(Context context) {  
- super(context);  
-         mPaint = new Paint();  
-     }  
- public MyView(Context context,AttributeSet attrs)  
-     {  
- super(context,attrs);  
-         mPaint = new Paint();  
- 
-         TypedArray a = context.obtainStyledAttributes(attrs,  
-                 R.styleable.MyView);  
- 
- int textColor = a.getColor(R.styleable.MyView_textColor,  
- 0XFFFFFFFF);  
- float textSize = a.getDimension(R.styleable.MyView_textSize, 36);  
- 
-         mPaint.setTextSize(textSize);  
-         mPaint.setColor(textColor);  
- 
-         a.recycle();  
-     }  
- @Override
- protectedvoid onDraw(Canvas canvas) {  
- // TODO Auto-generated method stub
- super.onDraw(canvas);  
- //设置填充
-         mPaint.setStyle(Style.FILL);  
- 
- //画一个矩形,前俩个是矩形左上角坐标，后面俩个是右下角坐标
-         canvas.drawRect(new Rect(10, 10, 100, 100), mPaint);  
- 
-         mPaint.setColor(Color.BLUE);  
- //绘制文字
-         canvas.drawText(mString, 10, 110, mPaint);  
-     }  
- }  
三、将我们自定义的**MyView** 加入布局**main.xml** 文件中，平且使用自定义属性,自定义属性必须加上:
**xmlns:test ="http://schemas.android.com/apk/res/com.android.tutor**"**蓝色**是自定义属性的前缀，**红色**是我们包名.
**main.xml** 全部代码如下:
[view
 plain](http://blog.csdn.net/Android_Tutor/archive/2010/04/20/5508615.aspx#)[copy
 to clipboard](http://blog.csdn.net/Android_Tutor/archive/2010/04/20/5508615.aspx#)[print](http://blog.csdn.net/Android_Tutor/archive/2010/04/20/5508615.aspx#)[?](http://blog.csdn.net/Android_Tutor/archive/2010/04/20/5508615.aspx#)
- <?xml   
- version="1.0" encoding="utf-8"?>  
- <LinearLayout   
- xmlns:android="http://schemas.android.com/apk/res/android"
- 
- xmlns:test="http://schemas.android.com/apk/res/com.android.tutor"
-     android:orientation="vertical"
-     android:layout_width="fill_parent"
-     android:layout_height="fill_parent"
-     >  
- <TextView    
-     android:layout_width="fill_parent"
-     android:layout_height="wrap_content"
-     android:text="@string/hello"
-     />  
- <com.android.tutor.MyView  
-     android:layout_width="fill_parent"
-     android:layout_height="fill_parent"
-     test:textSize="20px"
-     test:textColor="#fff"
- />  
- </LinearLayout>  
四、运行之效果如下图:
![](http://hi.csdn.net/attachment/201004/20/0_1271776011r46v.gif)
今天就到此结束，大家有什么疑问的，请留言，我会及时答复大家!谢谢~

