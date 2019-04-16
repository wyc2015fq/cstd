# 安卓开发20：动画之Animation 详细使用-主要通过java代码实现动画效果 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2013年07月11日 02:54:25[initphp](https://me.csdn.net/initphp)阅读数：8944
所属专栏：[安卓开发](https://blog.csdn.net/column/details/androidgo.html)









### AlphaAnimation 透明效果实现：

activity_main.xml中仅仅是一个简单的图片，下面的例子都会使用这个xml：



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

    <!-- 用于动画的图片 -->

    <ImageView
        android:id="@+id/imageView1"
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:layout_alignParentLeft="true"
        android:layout_alignParentTop="true"
        android:layout_marginLeft="70dp"
        android:layout_marginTop="138dp"
        android:src="@drawable/jhs_button1_h" />

</RelativeLayout>
```


透明效果的java代码：





```java
package com.example.test.com;


import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.Window;
import android.view.animation.AlphaAnimation;
import android.view.animation.Animation;
import android.view.animation.DecelerateInterpolator;
import android.widget.ImageView;

public class MainActivity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        setContentView(R.layout.activity_main);

        ImageView imageView = (ImageView) findViewById(R.id.imageView1);
        //图片点击的时候，启动动画效果  
        imageView.setOnClickListener(new OnClickListener() {

            @Override
            public void onClick(View v) {
                Animation anim = getAlphaAnimation();
                v.startAnimation(anim);
            }
        });

    }

    /**
     * 透明效果
     * @return
     */
    public Animation getAlphaAnimation() {
        //实例化 AlphaAnimation 主要是改变透明度
        //透明度 从 1-不透明 0-完全透明 
        Animation animation = new AlphaAnimation(1.0f, 0.5f);
        //设置动画插值器 被用来修饰动画效果,定义动画的变化率 
        animation.setInterpolator(new DecelerateInterpolator());
        //设置动画执行时间
        animation.setDuration(1000);
        return animation;
    }

}
```


### ScaleAnimation 缩放效果实现：

```java
package com.example.test.com;


import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.Window;
import android.view.animation.AlphaAnimation;
import android.view.animation.Animation;
import android.view.animation.DecelerateInterpolator;
import android.view.animation.ScaleAnimation;
import android.widget.ImageView;

public class MainActivity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        setContentView(R.layout.activity_main);

        ImageView imageView = (ImageView) findViewById(R.id.imageView1);
        //图片点击的时候，启动动画效果  
        imageView.setOnClickListener(new OnClickListener() {

            @Override
            public void onClick(View v) {
                Animation scalegetAnim = getScaleAnimation();
                v.startAnimation(scalegetAnim);
            }
        });

    }
    
    /**
     * 缩放动画
     * @return
     */
    public Animation getScaleAnimation() {
        //实例化 ScaleAnimation 主要是缩放效果
        //参数：fromX-动画开始前，x坐标   toX-动画结束后x坐标
        //fromY-动画开始前，Y坐标  toY-动画结束后Y坐标
        //pivotXType - 为动画相对于物件的X坐标的参照物   pivotXValue - 值
        //pivotYType - 为动画相对于物件的Y坐标的参照物   pivotYValue - 值
        Animation animation = new ScaleAnimation(1.0f, 0.0f, 1.0f, 0.0f, 
                Animation.RELATIVE_TO_SELF, 0.5f,
                Animation.RELATIVE_TO_SELF, 0.5f);
        //设置动画插值器 被用来修饰动画效果,定义动画的变化率 
        animation.setInterpolator(new DecelerateInterpolator());
        //设置动画执行时间
        animation.setDuration(1000);
        return animation;
    }

}
```


### RotateAnimation 旋转效果实现：

```java
package com.example.test.com;


import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.Window;
import android.view.animation.AlphaAnimation;
import android.view.animation.Animation;
import android.view.animation.DecelerateInterpolator;
import android.view.animation.RotateAnimation;
import android.view.animation.ScaleAnimation;
import android.widget.ImageView;

public class MainActivity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        setContentView(R.layout.activity_main);

        ImageView imageView = (ImageView) findViewById(R.id.imageView1);
        //图片点击的时候，启动动画效果  
        imageView.setOnClickListener(new OnClickListener() {

            @Override
            public void onClick(View v) {
                Animation rotateAnim = getRotateAnimation();
                v.startAnimation(rotateAnim);
            }
        });

    }
    
    /**
     * 旋转
     * @return
     */
    public Animation getRotateAnimation() {
        //实例化RotateAnimation
        //以自身中心为圆心，旋转360度 正值为顺时针旋转，负值为逆时针旋转
        RotateAnimation animation = new RotateAnimation(0, 360, 
                                        Animation.RELATIVE_TO_SELF, 0.5f,
                                        Animation.RELATIVE_TO_SELF, 0.5f);  
        //设置动画插值器 被用来修饰动画效果,定义动画的变化率 
        animation.setInterpolator(new DecelerateInterpolator());
        //设置动画执行时间
        animation.setDuration(1000);
        return animation;
    }

}
```


### TranslateAnimation 移动效果实现：



```java
package com.example.test.com;


import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.Window;
import android.view.animation.AlphaAnimation;
import android.view.animation.Animation;
import android.view.animation.DecelerateInterpolator;
import android.view.animation.RotateAnimation;
import android.view.animation.ScaleAnimation;
import android.view.animation.TranslateAnimation;
import android.widget.ImageView;

public class MainActivity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        setContentView(R.layout.activity_main);

        ImageView imageView = (ImageView) findViewById(R.id.imageView1);
        //图片点击的时候，启动动画效果  
        imageView.setOnClickListener(new OnClickListener() {

            @Override
            public void onClick(View v) {
                Animation translateAnim = getTranslateAnimation();
                v.startAnimation(translateAnim);
            }
        });

    }
    
    /**
     * 移动
     * @return
     */
    public Animation getTranslateAnimation() {
        //实例化TranslateAnimation
        //以自身为坐标系和长度单位，从(0,0)移动到(1,1)
        TranslateAnimation animation = new 
                TranslateAnimation(
                        Animation.RELATIVE_TO_SELF, 0.0f, 
                        Animation.RELATIVE_TO_SELF, 1.0f, 
                        Animation.RELATIVE_TO_SELF, 0.0f, 
                        Animation.RELATIVE_TO_SELF, 1.0f);  
        //设置动画插值器 被用来修饰动画效果,定义动画的变化率 
        animation.setInterpolator(new DecelerateInterpolator());
        //设置动画执行时间
        animation.setDuration(1000);
        return animation;
    }

}
```


### AnimationSet 动画集合实现和使用：



```java
package com.example.test.com;


import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.Window;
import android.view.animation.AlphaAnimation;
import android.view.animation.Animation;
import android.view.animation.AnimationSet;
import android.view.animation.DecelerateInterpolator;
import android.view.animation.RotateAnimation;
import android.view.animation.ScaleAnimation;
import android.view.animation.TranslateAnimation;
import android.widget.ImageView;

public class MainActivity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        setContentView(R.layout.activity_main);

        ImageView imageView = (ImageView) findViewById(R.id.imageView1);
        //图片点击的时候，启动动画效果  
        imageView.setOnClickListener(new OnClickListener() {

            @Override
            public void onClick(View v) {
                //AnimationSet类是一个Animation集合，里面可以许多Animation，
                //且在AnimationSet中设置的属性适用于里面的所有Animation。
                //参数true 则共享@Interpolator
                AnimationSet set = new AnimationSet(true);

                //透明
                Animation alphaAnim = getAlphaAnimation();
                set.addAnimation(alphaAnim);

                //缩放
                Animation scalegetAnim = getScaleAnimation();
                set.addAnimation(scalegetAnim);

                //旋转
                Animation rotateAnim = getRotateAnimation();
                set.addAnimation(rotateAnim);

                //移动 上面三个动画是同时进行的，我现在需要让移动这个动画在上面的动画之后执行
                //需要使用setStartOffset 设置动画开始的时间
                Animation translateAnim = getTranslateAnimation();
                translateAnim.setStartOffset(500);
                set.addAnimation(translateAnim);

                v.startAnimation(set);
            }
        });

    }

    /**
     * 透明效果
     * @return
     */
    public Animation getAlphaAnimation() {
        //实例化 AlphaAnimation 主要是改变透明度
        //透明度 从 1-不透明 0-完全透明 
        Animation animation = new AlphaAnimation(1.0f, 0.8f);
        //设置动画插值器 被用来修饰动画效果,定义动画的变化率 
        animation.setInterpolator(new DecelerateInterpolator());
        //设置动画执行时间
        animation.setDuration(1000);
        return animation;
    }

    /**
     * 缩放动画
     * @return
     */
    public Animation getScaleAnimation() {
        //实例化 ScaleAnimation 主要是缩放效果
        //参数：fromX-动画开始前，x坐标   toX-动画结束后x坐标
        //fromY-动画开始前，Y坐标  toY-动画结束后Y坐标
        //pivotXType - 为动画相对于物件的X坐标的参照物   pivotXValue - 值
        //pivotYType - 为动画相对于物件的Y坐标的参照物   pivotYValue - 值
        Animation animation = new ScaleAnimation(1.0f, 0.0f, 1.0f, 0.0f,
                Animation.RELATIVE_TO_SELF, 0.5f, Animation.RELATIVE_TO_SELF,
                0.5f);
        //设置动画插值器 被用来修饰动画效果,定义动画的变化率 
        animation.setInterpolator(new DecelerateInterpolator());
        //设置动画执行时间
        animation.setDuration(1000);
        return animation;
    }

    /**
     * 旋转
     * @return
     */
    public Animation getRotateAnimation() {
        //实例化RotateAnimation
        //以自身中心为圆心，旋转360度  正值为顺时针旋转，负值为逆时针旋转
        RotateAnimation animation = new RotateAnimation(0, 360,
                Animation.RELATIVE_TO_SELF, 0.5f, Animation.RELATIVE_TO_SELF,
                0.5f);
        //设置动画插值器 被用来修饰动画效果,定义动画的变化率 
        animation.setInterpolator(new DecelerateInterpolator());
        //设置动画执行时间
        animation.setDuration(1000);
        return animation;
    }

    /**
     * 移动
     * @return
     */
    public Animation getTranslateAnimation() {
        //实例化TranslateAnimation
        //以自身为坐标系和长度单位，从(0,0)移动到(1,1)
        TranslateAnimation animation = new TranslateAnimation(
                Animation.RELATIVE_TO_SELF, 0.0f, Animation.RELATIVE_TO_SELF,
                1.0f, Animation.RELATIVE_TO_SELF, 0.0f,
                Animation.RELATIVE_TO_SELF, 1.0f);
        //设置动画插值器 被用来修饰动画效果,定义动画的变化率 
        animation.setInterpolator(new DecelerateInterpolator());
        //设置动画执行时间
        animation.setDuration(1000);
        return animation;
    }

}
```



### Interpolator 描述动画的速率：



安卓默认的Interpolator：

AccelerateInterpolator：动画开始时比较慢，然后逐渐加速。

DecelerateInterpolator：动画开始时比较快，然后逐渐减速。

AccelerateDecelerateInterpolator：动画开始时和结束时比较慢，中间过程加速。

LinearInterpolator：动画匀速进行。

CycleInterpolator：动画循环播放指定次数，速率沿着正弦曲线改变。

DecelerateInterpolator代码：主要实现getInterpolation ，也可以自定义


```java
/*
 * Copyright (C) 2007 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package android.view.animation;

import android.content.Context;
import android.content.res.TypedArray;
import android.util.AttributeSet;

/**
 * An interpolator where the rate of change starts out quickly and 
 * and then decelerates.
 *
 */
public class DecelerateInterpolator implements Interpolator {
    public DecelerateInterpolator() {
    }

    /**
     * Constructor
     * 
     * @param factor Degree to which the animation should be eased. Setting factor to 1.0f produces
     *        an upside-down y=x^2 parabola. Increasing factor above 1.0f makes exaggerates the
     *        ease-out effect (i.e., it starts even faster and ends evens slower)
     */
    public DecelerateInterpolator(float factor) {
        mFactor = factor;
    }
    
    public DecelerateInterpolator(Context context, AttributeSet attrs) {
        TypedArray a =
            context.obtainStyledAttributes(attrs, com.android.internal.R.styleable.DecelerateInterpolator);
        
        mFactor = a.getFloat(com.android.internal.R.styleable.DecelerateInterpolator_factor, 1.0f);
        
        a.recycle();
    }
    
    public float getInterpolation(float input) {
        float result;
        if (mFactor == 1.0f) {
            result = (float)(1.0f - (1.0f - input) * (1.0f - input));
        } else {
            result = (float)(1.0f - Math.pow((1.0f - input), 2 * mFactor));
        }
        return result;
    }
    
    private float mFactor = 1.0f;
}
```


使用：



```java
//设置动画插值器 被用来修饰动画效果,定义动画的变化率 
        animation.setInterpolator(new DecelerateInterpolator());
```


### AnimationListener 监听器：

可以监听动画前，动画结束，动画repeat的时候的动作，对上面代码中的移动效果进行动画监听：



```java
/**
     * 移动
     * @return
     */
    public Animation getTranslateAnimation() {
        //实例化TranslateAnimation
        //以自身为坐标系和长度单位，从(0,0)移动到(1,1)
        TranslateAnimation animation = new TranslateAnimation(
                Animation.RELATIVE_TO_SELF, 0.0f, Animation.RELATIVE_TO_SELF,
                1.0f, Animation.RELATIVE_TO_SELF, 0.0f,
                Animation.RELATIVE_TO_SELF, 1.0f);
        //设置动画插值器 被用来修饰动画效果,定义动画的变化率 
        animation.setInterpolator(new DecelerateInterpolator());
        //设置重复动画
        animation.setRepeatCount(2); 
        //设置动画执行时间
        animation.setDuration(1000);
        //设置监听器
        animation.setAnimationListener(new AnimationListener() {
            
            @Override
            public void onAnimationStart(Animation animation) {
                //动画开始前
                Toast.makeText(getBaseContext(), "Strart!", Toast.LENGTH_SHORT).show();  
            }
            
            @Override
            public void onAnimationRepeat(Animation animation) {
                //重复动画的时候，
                Toast.makeText(getBaseContext(), "Repeat!", Toast.LENGTH_SHORT).show();  
            }
            
            @Override
            public void onAnimationEnd(Animation animation) {
                // 结束动画的时候
                Toast.makeText(getBaseContext(), "End!", Toast.LENGTH_SHORT).show();  
                
            }
        });
        return animation;
    }
```


### 备注内容



动画运动完毕后，停止在动画完成的位置



```java
animation.setFillAfter(true);
        animation.setFillEnabled(true);
```


动画结束后，清除动画



```java
tempLayout.clearAnimation();
```


动画结束后，改变View的偏移量



```java
tempLayout.offsetLeftAndRight(width);
```










