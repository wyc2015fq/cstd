# 安卓开发18：动画之View Animation - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2013年07月09日 15:31:35[initphp](https://me.csdn.net/initphp)阅读数：1135
个人分类：[安卓开发笔记系列](https://blog.csdn.net/initphp/article/category/1439903)

所属专栏：[安卓开发](https://blog.csdn.net/column/details/androidgo.html)









**View Animation：**

主要能实现：Scale伸缩,Rotate旋转,Translate移动,Alpha透明度变化.




先看一个xml的一些属性表格，下面都需要用到：**这个表格很重要**


|属性[类型]|功能|备注|
|----|----|----|
|Duration[long]|属性为动画持续时间|时间以毫秒为单位|
|fillAfter [boolean]|当设置为true ，该动画转化在动画结束后被应用| |
|fillBefore[boolean]|当设置为true ，该动画转化在动画开始前被应用| |
|interpolator|指定一个动画的插入器|有一些常见的插入器accelerate_decelerate_interpolator加速-减速 动画插入器accelerate_interpolator加速-动画插入器decelerate_interpolator减速- 动画插入器其他的属于特定的动画效果|
|repeatCount[int]|动画的重复次数||
|RepeatMode[int]|定义重复的行为|1：重新开始  2：plays backward|
|startOffset[long]|动画之间的时间间隔，从上次动画停多少时间开始执行下个动画| |
|zAdjustment[int]|定义动画的Z Order的改变|0：保持Z Order不变1：保持在最上层-1：保持在最下层|
|表二| | |
|----|----|----|
|XML节点|功能说明| |
|alpha|**渐变透明度动画效果**| |
|**<alphaandroid:fromAlpha=”0.1″android:toAlpha=”1.0″android:duration=”3000″ />**| | |
|fromAlpha|属性为动画起始时透明度|0.0表示完全透明1.0表示完全不透明以上值取0.0-1.0之间的float数据类型的数字duration为动画持续时间，ms单位|
|> toAlpha|属性为动画结束时透明度| |
|表三| | | |
|----|----|----|----|
|scale|**渐变尺寸伸缩动画效果**| | |
|**<scaleandroid:interpolator= “@android:anim/accelerate_decelerate_interpolator”android:fromXScale=”0.0″android:toXScale=”1.4″android:fromYScale=”0.0″android:toYScale=”1.4″android:pivotX=”50%”android:pivotY=”50%”android:fillAfter=”false”android:startOffset=“700”android:duration=”700″android:repeatCount=”10″ />**| | | |
|fromXScale[float] fromYScale[float]|为动画起始时，X、Y坐标上的伸缩尺寸|0.0表示收缩到没有1.0表示正常无伸缩值小于1.0表示收缩值大于1.0表示放大| |
|toXScale [float]toYScale[float]|为动画结束时，X、Y坐标上的伸缩尺寸| | |
|pivotX[float]pivotY[float]|为动画相对于物件的X、Y坐标的开始位置|属性值说明：从0%-100%中取值，50%为物件的X或Y方向坐标上的中点位置| |
|||||
|表四| | | |
|----|----|----|----|
|translate|**画面转换位置移动动画效果**| | |
|**<translateandroid:fromXDelta=”30″android:toXDelta=”-80″android:fromYDelta=”30″android:toYDelta=”300″android:duration=”2000″ />**| | | |
|fromXDeltatoXDelta|为动画、结束起始时 X坐标上的位置|| |
|fromYDeltatoYDelta|为动画、结束起始时 Y坐标上的位置|| |
|||||
|表五| | |
|----|----|----|
|rotate|**画面转移旋转动画效果**| |
|**<rotateandroid:interpolator=”@android:anim/accelerate_decelerate_interpolator”android:fromDegrees=”0″android:toDegrees=”+350″android:pivotX=”50%”android:pivotY=”50%”android:duration=”3000″ />**| | |
|fromDegrees|为动画起始时物件的角度|说明当角度为负数——表示逆时针旋转当角度为正数——表示顺时针旋转(负数from——to正数:顺时针旋转)(负数from——to负数:逆时针旋转)(正数from——to正数:顺时针旋转)(正数from——to负数:逆时针旋转)|
|toDegrees|属性为动画结束时物件旋转的角度 可以大于360度| |
|pivotXpivotY|为动画相对于物件的X、Y坐标的开始位|说明：以上两个属性值 从0%-100%中取值50%为物件的X或Y方向坐标上的中点位置|




**一个丑陋的动画效果例子：**

创建一个activity_main.xml，主要是一个图片：



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


MainActivity.java代码：





```java
package com.example.test.com;


import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.Window;
import android.view.animation.Animation;
import android.view.animation.AnimationUtils;
import android.widget.ImageView;

public class MainActivity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        setContentView(R.layout.activity_main);

        
        ImageView imageView = (ImageView) findViewById(R.id.imageView1);
        //创建Animation对象
        final Animation animation = AnimationUtils.loadAnimation(this, R.anim.test_jump);  
        //图片点击的时候，启动动画效果
        imageView.setOnClickListener(new OnClickListener() {
            
            @Override
            public void onClick(View v) {
                v.startAnimation(animation);   
            }
        });
         
    }

}
```


在res/anim/test_jump.xml中，创建动画效果的具体实现配置：





```
<?xml version="1.0" encoding="utf-8"?>
<set xmlns:android="http://schemas.android.com/apk/res/android"
    android:shareInterpolator="false" >

    <!-- 这是一个丑陋的动画效果 -->
    <!-- 具体的这些动画效果的参数表格上面已经有写，请对照表格 -->
    <set android:shareInterpolator="false" >
        <!-- 图片的缩放 使用scale -->
        <scale
            android:duration="700"
            android:fillAfter="false"
            android:fromXScale="1.0"
            android:fromYScale="1.0"
            android:interpolator="@android:anim/accelerate_decelerate_interpolator"
            android:pivotX="0%"
            android:pivotY="0%"
            android:toXScale="1.4"
            android:toYScale="0.6" />

        <!-- 图片的平移 使用translate -->
        <translate
            android:duration="700"
            android:fromXDelta="0"
            android:fromYDelta="0"
            android:toXDelta="20"
            android:toYDelta="0" />

        <!-- 图片的透明度 -->
        <alpha
            android:duration="700"
            android:fromAlpha="1.0"
            android:toAlpha="0.5" />

        <!-- 上面三个效果是同步进行的，时间为700毫秒，继续set一个动画效果 -->
        <set android:interpolator="@android:anim/decelerate_interpolator" >

            <!-- 动画旋转 -->
            <rotate
                android:duration="400"
                android:fromDegrees="0"
                android:toDegrees="120"
                android:pivotX="50%"
                android:pivotY="50%"/>
        </set>
    </set>

</set>
```







