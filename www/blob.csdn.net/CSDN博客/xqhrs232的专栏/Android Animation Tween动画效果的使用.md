# Android Animation Tween动画效果的使用 - xqhrs232的专栏 - CSDN博客
2010年12月31日 11:39:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1207标签：[animation																[android																[import																[null																[encoding																[2010](https://so.csdn.net/so/search/s.do?q=2010&t=blog)](https://so.csdn.net/so/search/s.do?q=encoding&t=blog)](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=import&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)](https://so.csdn.net/so/search/s.do?q=animation&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://dev.firnow.com/course/3_program/java/javajs/20100719/459467.html](http://dev.firnow.com/course/3_program/java/javajs/20100719/459467.html)
Animation Tween动画可以通过java代码实现，也可以通过xml布局来实现
1.通过java代码实现：
显示代码打印
001 package com.Aina.Android;  
002    
003 import android.content.Context;  
004 import android.graphics.Bitmap;  
005 import android.graphics.Canvas;  
006 import android.graphics.Paint;  
007 import android.graphics.drawable.BitmapDrawable;  
008 import android.view.KeyEvent;  
009 import android.view.View;  
010 import android.view.animation.AlphaAnimation;  
011 import android.view.animation.Animation;  
012 import android.view.animation.RotateAnimation;  
013 import android.view.animation.ScaleAnimation;  
014 import android.view.animation.TranslateAnimation;  
015    
016 /**  
017  * com.Aina.Android Pro_AnimationTween  
018  *   
019  * @author Aina.huang E-mail: [674023920@qq.com](mailto:674023920@qq.com)
020  * @version 创建时间：2010 Jun 17, 2010 5:15:36 PM 类说明  
021  */ 
022 public class GameView extends View {  
023    
024     private Paint mPaint = null;  
025     private Animation mAlphaAnimation = null;  
026     private Animation mScaleAnimation = null;  
027     private Animation mTranslateAnimation = null;  
028     private Animation mRotateAnimation = null;  
029     private Bitmap mBitmap = null;  
030    
031     public GameView(Context context) {  
032         super(context);  
033         mBitmap = ((BitmapDrawable) this.getResources().getDrawable(  
034                 R.drawable.img)).getBitmap();  
035     }  
036    
037     @Override 
038     protected void onDraw(Canvas canvas) {  
039         super.onDraw(canvas);  
040         mPaint = new Paint();  
041         mPaint.setAntiAlias(true);  
042         canvas.drawBitmap(mBitmap, 0, 0, mPaint);  
043     }  
044    
045     public boolean onKeyDown(int keyCode, KeyEvent event) {  
046         switch (keyCode) {  
047         case KeyEvent.KEYCODE_DPAD_UP:  
048             mAlphaAnimation = new AlphaAnimation(0.1f, 1.0f);// 透明度  
049             mAlphaAnimation.setDuration(3000);  
050             this.startAnimation(mAlphaAnimation);  
051             break;  
052         case KeyEvent.KEYCODE_DPAD_DOWN:  
053             mScaleAnimation = new ScaleAnimation(0.0f, 1.0f, 0.0f,  
054                     1.0f,// 整个屏幕就0.0到1.0的大小//缩放  
055                     Animation.RELATIVE_TO_SELF, 0.5f,  
056                     Animation.RELATIVE_TO_SELF, 0.0f);  
057             mScaleAnimation.setDuration(3000);  
058             this.startAnimation(mScaleAnimation);  
059             break;  
060         case KeyEvent.KEYCODE_DPAD_LEFT:  
061             mTranslateAnimation = new TranslateAnimation(0, 100, 0, 100);// 移动 
062             mTranslateAnimation.setDuration(2000);  
063             this.startAnimation(mTranslateAnimation);  
064             break;  
065         case KeyEvent.KEYCODE_DPAD_RIGHT:  
066             mRotateAnimation = new RotateAnimation(0.0f, 360.0f,//旋转  
067                     Animation.RELATIVE_TO_SELF, 0.5f,  
068                     Animation.RELATIVE_TO_SELF, 0.5f); 
 069             mRotateAnimation.setDuration(3000);  
070             this.startAnimation(mRotateAnimation);  
071             break;  
072         default:  
073             break;  
074         }  
075         return super.onKeyDown(keyCode, event);  
076     }  
077 }  
078    
079    
080    
081    
082    
083 package com.Aina.Android;  
084    
085 import android.app.Activity;  
086 import android.os.Bundle;  
087 import android.view.KeyEvent;  
088    
089 public class Test_AnimationTween extends Activity {  
090     /** Called when the activity is first created. */ 
091     private GameView gv = null;  
092     @Override 
093     public void onCreate(Bundle savedInstanceState) {  
094         super.onCreate(savedInstanceState);  
095         gv = new GameView(this);  
096         this.setContentView(gv);  
097     }  
098     @Override 
099     public boolean onKeyDown(int keyCode, KeyEvent event) {  
100         return gv.onKeyDown(keyCode, event);  
101     }  
102        
103 } 
2.通过xml布局实现：
显示代码打印
01 <?xml version="1.0" encoding="utf-8"?>  
02 <set xmlns:android="[http://schemas.android.com/apk/res/android](http://schemas.android.com/apk/res/android)"> 
03     <alpha android:fromAlpha="0.1" android:toAlpha="1.0" 
04         android:duration="3000">  
05     </alpha>  
06 </set>  
07    
08    
09    
10    
11 <?xml version="1.0" encoding="utf-8"?>  
12 <set xmlns:android="[http://schemas.android.com/apk/res/android](http://schemas.android.com/apk/res/android)"> 
13     <scale android:fromXScale="0.0" android:toXScale="1.0" 
14         android:fromYScale="0.0" android:toYScale="1.0" android:pivotX="50%"
15         android:pivotY="50%" android:fillAfter="false" 
16         android:duration="3000">  
17     </scale>  
18 </set>  
19    
20    
21    
22    
23 <?xml version="1.0" encoding="utf-8"?>  
24 <set xmlns:android="[http://schemas.android.com/apk/res/android](http://schemas.android.com/apk/res/android)"> 
25     <translate android:fromXDelta="0" android:toXDelta="100" 
26         android:fromYDelta="0" android:toYDelta="100" android:duration="3000"> 
27     </translate>  
28 </set>  
29    
30    
31    
32    
33 <?xml version="1.0" encoding="utf-8"?>  
34 <set xmlns:android="[http://schemas.android.com/apk/res/android](http://schemas.android.com/apk/res/android)"> 
35     <rotate 
36         android:interpolator="@android:anim/accelerate_decelerate_interpolator"
37         android:fromDegrees="0" android:toDegrees="+360" android:pivotX="50%"
38         android:pivotY="50%" android:duration="3000">  
39     </rotate>  
40 </set> 
显示代码打印
01 package com.Aina.Android;  
02    
03 import android.content.Context;  
04 import android.graphics.Bitmap;  
05 import android.graphics.Canvas;  
06 import android.graphics.Paint;  
07 import android.graphics.drawable.BitmapDrawable;  
08 import android.view.KeyEvent;  
09 import android.view.View;  
10 import android.view.animation.AlphaAnimation;  
11 import android.view.animation.Animation;  
12 import android.view.animation.AnimationUtils;  
13 import android.view.animation.RotateAnimation;  
14 import android.view.animation.ScaleAnimation;  
15 import android.view.animation.TranslateAnimation;  
16    
17 /**  
18  * com.Aina.Android Pro_AnimationTween  
19  *   
20  * @author Aina.huang E-mail: [674023920@qq.com](mailto:674023920@qq.com)
21  * @version 创建时间：2010 Jun 17, 2010 5:15:36 PM 类说明  
22  */ 
23 public class GameView extends View {  
24    
25     private Paint mPaint = null;  
26     private Animation mAlphaAnimation = null;  
27     private Animation mScaleAnimation = null;  
28     private Animation mTranslateAnimation = null;  
29     private Animation mRotateAnimation = null;  
30     private Bitmap mBitmap = null;  
31     private Context mContext = null;  
32     public GameView(Context context) {  
33         super(context);  
34         mContext = context;  
35         mBitmap = ((BitmapDrawable) this.getResources().getDrawable(  
36                 R.drawable.img)).getBitmap();  
37     }  
38    
39     @Override 
40     protected void onDraw(Canvas canvas) {  
41         super.onDraw(canvas);  
42         mPaint = new Paint();  
43         mPaint.setAntiAlias(true);  
44         canvas.drawBitmap(mBitmap, 0, 0, mPaint);  
45     }  
46    
47     public boolean onKeyDown(int keyCode, KeyEvent event) {  
48         switch (keyCode) {  
49         case KeyEvent.KEYCODE_DPAD_UP:  
50 //          mAlphaAnimation = new AlphaAnimation(0.1f, 1.0f);// 透明度  
51 //          mAlphaAnimation.setDuration(3000);  
52             mAlphaAnimation = AnimationUtils.loadAnimation(mContext, R.anim.alpha); 
53             this.startAnimation(mAlphaAnimation);  
54             break;  
55         case KeyEvent.KEYCODE_DPAD_DOWN:  
56 //          mScaleAnimation = new ScaleAnimation(0.0f, 1.0f, 0.0f,  
57 //                  1.0f,// 整个屏幕就0.0到1.0的大小//缩放  
58 //                  Animation.RELATIVE_TO_SELF, 0.5f,  
59 //                  Animation.RELATIVE_TO_SELF, 0.0f);  
60 //          mScaleAnimation.setDuration(3000);  
61             mScaleAnimation = AnimationUtils.loadAnimation(mContext, R.anim.scale); 
62             this.startAnimation(mScaleAnimation);  
63             break;  
64         case KeyEvent.KEYCODE_DPAD_LEFT:  
65 //          mTranslateAnimation = new TranslateAnimation(0, 100, 0, 100);// 移动 
66 //          mTranslateAnimation.setDuration(2000);  
67             mTranslateAnimation = AnimationUtils.loadAnimation(mContext, R.anim.translate); 
68             this.startAnimation(mTranslateAnimation);  
69             break;  
70         case KeyEvent.KEYCODE_DPAD_RIGHT:  
71 //          mRotateAnimation = new RotateAnimation(0.0f, 360.0f,//旋转  
72 //                  Animation.RELATIVE_TO_SELF, 0.5f,  
73 //                  Animation.RELATIVE_TO_SELF, 0.5f);            
74 //          mRotateAnimation.setDuration(3000);  
75             mRotateAnimation = AnimationUtils.loadAnimation(mContext, R.anim.rotate); 
76             this.startAnimation(mRotateAnimation);  
77             break;  
78         default:  
79             break;  
80         }  
81         return super.onKeyDown(keyCode, event);  
82     }  
83 
} 
文章出处：飞诺网([www.firnow.com):http://dev.firnow.com/course/3_program/java/javajs/20100719/459467.html]()

