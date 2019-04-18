# Android换肤 - weixin_33985507的博客 - CSDN博客
2018年05月10日 23:57:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：11
### 前言
> 
近些天, 看到简书和知乎的换肤功能,感觉非常的棒,于是就想着自己去实现一下换肤功能,研究了几种方法
### 方法一: 使用 setTheme 方法
我们先来看看使用 setTheme 方法来实现日间/夜间模式切换的方案。这种方案的思路很简单，就是在用户选择夜间模式时，Activity 设置成夜间模式的主题，之后再让 Activity 调用 recreate() 方法重新创建一遍就行了。
#### 实际操作
- 
在colors.xml定义两组颜色,分别代表日夜间模式
```
<resources>
 //日间模式
<color name="colorPrimary">#3F51B5</color>
<color name="colorPrimaryDark">#303F9F</color>
 <color name="colorAccent">#FF4081</color>
 //夜间模式
 <color name="nightColorPrimary">#3b3b3b</color>
 <color name="nightColorPrimaryDark">#383838</color>
 <color name="nightColorAccent">#a72b55</color>
 </resources>
```
- 
在styles.xml下定义两组主题,分别代表日间模式和夜间模式
```
<resources>
<!-- Base application theme. -->
  //日间模式主题
 <style name="AppTheme" parent="Theme.AppCompat.Light.DarkActionBar">
 <item name="colorPrimary">@color/colorPrimary</item>
 <item name="colorPrimaryDark">@color/colorPrimaryDark</item>
 <item name="colorAccent">@color/colorAccent</item>
 <!--自定义的属性 用作背景色-->
 <item name="ColorBackground">@color/backgroundColor</item>
 <!--文字颜色-->
 <item name="android:textColor">@color/textColor</item>
 </style>
 //夜间模式主题
 <style name="NightAppTheme" parent="Theme.AppCompat.Light.DarkActionBar">
 <item name="colorPrimary">@color/nightColorPrimary</item>
 <item name="colorPrimaryDark">@color/nightColorPrimaryDark</item>
 <item name="colorAccent">@color/nightColorAccent</item>
  <!--自定义的属性 用作背景色-->
 <item name="ColorBackground">@color/nightColorPrimary</item>
  <!--文字颜色-->
 <item name="android:textColor">@android:color/white</item>
 </style>
 </resources>
```
- 
在主题中的 ColorBackground属性是我们自定义的属性，用来表示背景色：
```
<?xml version="1.0" encoding="utf-8"?>
<resources>
<attr name="ColorBackground" format="color|reference"/>
</resources>
```
- 
接下来看看activity_main.xml布局文件
```
<?xml version="1.0" encoding="utf-8"?>
 <LinearLayout xmlns:android="http://schemas.android.com/apk/res/android"
 xmlns:tools="http://schemas.android.com/tools"
 android:layout_width="match_parent"
 android:background="?attr/colorBackground"
 android:layout_height="match_parent"
 android:orientation="vertical"
 android:gravity="center"
 tools:context="com.dream.nightmodedemo.MainActivity">
 <TextView
 android:layout_width="wrap_content"
 android:layout_height="wrap_content"
 android:text="设置主题" />
 <Button
 android:layout_marginTop="20dp"
 android:text="设置主题"
 android:onClick="btnClick"
 android:layout_width="wrap_content"
 android:layout_height="wrap_content" />
</LinearLayout>
```
在 <LinearLayout> 的 android:background 属性中，我们使用 "?attr/colorBackground" 来表示，这样就代表着 LinearLayout的背景色会去引用在主题中事先定义好的 colorBackground属性的值。这样就实现了日间/夜间模式切换的换色了。
- 
MainActivity.java代码如下
```
public class MainActivity extends AppCompatActivity {
private int theme = R.style.AppTheme;
@Override
 protected void onCreate(Bundle savedInstanceState) {
 super.onCreate(savedInstanceState);
 if(savedInstanceState != null){
     theme = savedInstanceState.getInt("theme");
     setTheme(theme);
 }
 setContentView(R.layout.activity_main);
 }
 public void btnClick(View view){
 theme = (theme == R.style.AppTheme ? R.style.NightAppTheme : R.style.AppTheme);
 recreate();
 }
 @Override
 protected void onSaveInstanceState(Bundle outState) {
 super.onSaveInstanceState(outState);
 outState.putInt("theme",theme);
 }
 }
```
在 MainActivity 中有几点要注意一下：
- 
调用 recreate() 方法后 Activity 的生命周期会调用 onSaveInstanceState(Bundle outState) 来备份相关的数据，之后也会调用 onRestoreInstanceState(Bundle savedInstanceState) 来还原相关的数据，因此我们把 theme 的值保存进去，以便 Activity 重新创建后使用.
- 
因为onCreate()方法中也有Bundle的实例,所以我是在onCreate()方法中还原数据的,我们在onCreate(Bundle savedInstanceState) 方法中还原得到了 theme 值后，setTheme() 方法一定要在 setContentView() 方法之前调用，否则的话就看不到效果了。
- 
recreate() 方法是在 API 11 中添加进来的，所以在 Android 2.X 中使用会抛异常。
效果如下:
日间模式:
![4311670-f51b736f1095cc6b.png](https://upload-images.jianshu.io/upload_images/4311670-f51b736f1095cc6b.png)
日间模式
夜间模式:
![4311670-a63d35a5a2b16475.png](https://upload-images.jianshu.io/upload_images/4311670-a63d35a5a2b16475.png)
夜间模式
### 方法二: 使用 Android Support Library V7包中的 UiMode 方法
使用 UiMode 的方法也很简单，我们需要把 colors.xml 定义为日间/夜间两种。之后根据不同的模式会去选择不同的 colors.xml 。在 Activity 调用 recreate() 之后，就实现了切换日/夜间模式的功能。
#### 实际操作
- 
colors.xml代码:
```
<?xml version="1.0" encoding="utf-8"?>
 <resources>
 <color name="colorPrimary">#3F51B5</color>
 <color name="colorPrimaryDark">#303F9F</color>
 <color name="colorAccent">#FF4081</color>
 <color name="textColor">#FF000000</color>
 <color name="backgroundColor">#FFFFFF</color>
 </resources>
```
- 
除了 values/colors.xml 之外，我们还要创建一个 values-night/colors.xml 文件，用来设置夜间模式的颜色，其中 <color> 的 name 必须要和 values/colors.xml 中的相对应:
```
<?xml version="1.0" encoding="utf-8"?>
 <resources>
 <color name="colorPrimary">#3b3b3b</color>
 <color name="colorPrimaryDark">#383838</color>
 <color name="colorAccent">#a72b55</color>
 <color name="textColor">#FFFFFF</color>
 <color name="backgroundColor">#3b3b3b</color>
</resources>
```
- 
在 styles.xml 中去引用我们在 colors.xml 中定义好的颜色:
```
<resources>
 <!-- Base application theme. -->
 <style name="AppTheme" parent="Theme.AppCompat.Light.DarkActionBar">
 <item name="colorPrimary">@color/colorPrimary</item>
 <item name="colorPrimaryDark">@color/colorPrimaryDark</item>
 <item name="colorAccent">@color/colorAccent</item>
 <!--自定义的属性 用作背景色-->
 <item name="colorBackground">@color/backgroundColor</item>
 <!--文字颜色-->
 <item name="android:textColor">@color/textColor</item>
 </style>
 </resources>
```
activity_main.xml布局和setTheme方法中一样,基本没变
- 
在MyApplication中进行模式的设置
```
public class MyApplication extends Application{
@Override
 public void onCreate() {
 super.onCreate();
 AppCompatDelegate.setDefaultNightMode(AppCompatDelegate.MODE_NIGHT_NO);
 }
 }
```
要注意的是，这里的 Mode 有四种类型可以选择：
- MODE_NIGHT_NO： 使用亮色(light)主题，不使用夜间模式；
- MODE_NIGHT_YES：使用暗色(dark)主题，使用夜间模式；
- MODE_NIGHT_AUTO：根据当前时间自动切换 亮色(light)/暗色(dark)主题；
- MODE_NIGHT_FOLLOW_SYSTEM(默认选项)：设置为跟随系统，通常为 MODE_NIGHT_NO
- 
MainActivity.java代码:
```
public class MainActivity extends AppCompatActivity {
 @Override
 protected void onCreate(Bundle savedInstanceState) {
     super.onCreate(savedInstanceState);
     setContentView(R.layout.activity_main);
 }
 public void btnClick(View view){
     int currentNightMode = getResources().getConfiguration().uiMode & Configuration.UI_MODE_NIGHT_MASK;
     AppCompatDelegate.setDefaultNightMode((currentNightMode == Configuration.UI_MODE_NIGHT_NO ? AppCompatDelegate.MODE_NIGHT_YES : AppCompatDelegate.MODE_NIGHT_NO));
     recreate();
 }
 }
```
实现效果:
日间模式:
![4311670-9e6c3c2914ddddad.png](https://upload-images.jianshu.io/upload_images/4311670-9e6c3c2914ddddad.png)
日间模式
夜间模式:
![4311670-5fdfe5fc58e5aabe.png](https://upload-images.jianshu.io/upload_images/4311670-5fdfe5fc58e5aabe.png)
夜间模式
就前两种方法而言，配置比较简单，最后的实现效果也都基本上是一样的。但是缺点就是需要调用 recreate() 使之生效,切换时会有黑屏闪现的现象,用户体验不是很好,实现不了简书和知乎的换肤效果
### 方法三: 通过第三方换肤框架
[传送门](https://link.jianshu.com?t=https%3A%2F%2Fgithub.com%2Fximsfei%2FAndroid-skin-support),这个库基本解决了上面两种方法的问题,而且功能非常的强大,能够实现简书和知乎的换肤效果
#### 如果对我的文章感兴趣的话,请为我点赞,谢谢!!!
