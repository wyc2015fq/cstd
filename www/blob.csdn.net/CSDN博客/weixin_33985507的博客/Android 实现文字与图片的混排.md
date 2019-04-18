# Android 实现文字与图片的混排 - weixin_33985507的博客 - CSDN博客
2017年07月02日 14:38:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：8

在我们的项目中，常常会碰到图片与文字混排的问题。解决这类问题的方法有非常多，本文给出的方法不是唯一的。仅仅有依据实际场景才干找到更适合的方法。
本文主要通过xml布局来实现图片与文字的混排（水平排列）。
1.利用TextView实现图片与文字混排，
android:drawableBottom在text的下方输出一个drawable。如图片。
假设指定一个颜色的话会把text的背景设为该颜色。而且同一时候和background使用时覆盖后者。
android:drawableLeft在text的左边输出一个drawable，如图片。
android:drawablePadding设置text与drawable(图片)的间隔，
与drawableLeft、 drawableRight、drawableTop、drawableBottom一起使用，可设置为负数。单独使用没有效果。
android:drawableRight在text的右边输出一个drawable。
android:drawableTop在text的正上方输出一个drawable。
```
<span style="font-size:18px;">        <TextView
            android:id="@+id/my_tv"
            android:layout_width="wrap_content"
            android:layout_height="wrap_content"
            android:text="在线"
            android:textColor="#85898f"
            android:layout_marginTop="5dp"
            android:drawablePadding="5dp"
            android:drawableLeft="@drawable/user_online"/></span>
```
当中， android:drawablePaddingh非常好的攻克了图片与文字的间距问题。
2.TextView动态的设置图片
```java
Drawable drawable= context.getResources().getDrawable(R.drawable.text_img);
// 调用setCompoundDrawables时。必须调用Drawable.setBounds()方法,否则图片不显示
drawable.setBounds(0, 0, drawable.getMinimumWidth(), drawable.getMinimumHeight());
textView.setCompoundDrawables(drawable, null, null, null); //设置左图标
```
3.利用RelativeLayout(LinearLayout) 加入 TextView 和 ImageView(ButtonView)来实现
```
<span style="font-size:18px;">   <RelativeLayout
        android:layout_width="wrap_content"
        android:layout_height="wrap_content" >
        <ImageView
            android:id="@+id/my_iv"
            android:layout_width="wrap_content"
            android:layout_height="wrap_content" 
            android:background="@drawable/user_online"
            android:layout_alignParentLeft="true"
            android:layout_centerVertical="true"
            android:layout_marginLeft="5dp"
            />
        
        <TextView
            android:id="@+id/my_tv"
            android:layout_width="wrap_content"
            android:layout_height="wrap_content" 
            android:layout_toRightOf="@id/my_iv"
            android:layout_centerVertical="true"
            android:layout_marginLeft="5dp"
            />
    </RelativeLayout></span>
```
事实上也能够通过java代码来实现图片和文字的混排。

