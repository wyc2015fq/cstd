# Android横竖屏切换及其对应布局加载问题 - DEVELOPER - CSDN博客





2014年07月25日 10:06:31[学术袁](https://me.csdn.net/u012827205)阅读数：11455








***第一，横竖屏切换连带加载多屏布局问题：***



如果要让软件在横竖屏之间切换，由于横竖屏的高宽会发生转换，有可能会要求不同的布局。

可以通过以下两种方法来切换布局：

1）在res目录下建立layout-land和layout-port目录,相应的layout文件名不变，比如：layout-land是横屏的layout,layout-port是竖屏的layout，其他的不用管，横竖屏切换时程序调用Activity的onCreate方法中的setOnContent(xxx)，并自动加载相应的布局。

2）假如布局资源不按照如上设置，则可以通过java代码来判断当前是横屏还是竖屏然后来加载相应的xml布局文件。因为当屏幕变为横屏的时候,系统会重新加载当前Activity的onCreate方法（也就是说：该Activity的生命周期要重头开始），你可以把以下方法放在你的onCreate中来检查当前的方向,然后可以让你的setContentView来载入不同的layout 。



```java
/** 1:竖屏   2:横屏 判断屏幕以旋转的方向 */
	private int orientation;
orientation=getResources().getConfiguration().orientation;
```

```
/**
如果使用该屏幕切换方式，则该Activity的生命周期在你切换屏幕的时刻会一直遵循：onPause()---->onStop--->onDestroy()---->onCreate()---->onStart()----->onResume(); 不会执行下面的方法：onConfigurationChanged(Configuration newConfig)
*/
@Override
	public void onConfigurationChanged(Configuration newConfig) {
		// TODO Auto-generated method stub
		super.onConfigurationChanged(newConfig);
		Toast.makeText(getApplicationContext(), "屏幕切换了", Toast.LENGTH_SHORT).show();
	}
```


***第二，强制设定屏幕的横、竖屏方向：***




Android横竖屏切换在手机开发中比较常见，很多软件在开发过程中为了避免横竖屏切换时引发不必要的麻烦，通常要强制设置横竖屏的方向，

通过在AndroidManifest.xml中设置activity中的android:screenOrientation属性值来实现。

比如下列设置：

```java
横屏显示设置：android:screenOrientation="lanscape"

竖屏显示设置：android:screenOrientation="portrait"
```




当然上述修改也可以在Java代码中通过代码来实现：（android屏幕的切换会重启Activity，所以在Activity销毁前保存当前活动的状态，并在Activity再次Create的时候载入配置）



```java
setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE)
```



***第三，拦截横竖屏切换 需要的配置文件：onConfigurationChanged***



Activity每次横竖屏切换都会重新调用onPause->onStop-> onDestory->onCreate->onStart->onResume（为此内容和数据要保存和读取，否则转屏之前的内容就会消失了）

很多时候这样的结果让程序繁琐，为此Android提供了在manifest中设置android:configChanges属性，从而让Activity不延续上述的重建流程；

方式一）在Android工程的Mainfest.xml中配置Activity：***android:configChanges="keyboardHidden|orientation***
 ，横竖屏切换之后就不会去执行OnCreat函数了，而是会去调用onConfigurationChanged（）这样就能控制横竖屏的切换了。

***<[Android横竖屏切换不重新调用onCreate()](http://www.cnblogs.com/savagemorgan/archive/2013/01/24/2875586.html)>但是，有一点请注意：A******ndroid2******.3之前使用******上面的******android:configChanges="keyboardHidden|orientation****就能行。但是，在4.0之后。必须就要使用这个属性***android:configChanges="***orientation|keyboardHidden|screenSize才能避免Activity重新加载该类不延续上述的重建流程！*



方式二）用户可以在Activity或View的：***onConfigurationChanged(Configurationnew   Config) ***，函数中获取当前横竖屏参数。至于其调用顺序跟touch时间的传递顺序相似，不过他没有消费事件的概念，会顺次调用到每一个onConfigurationChanged函数。

***需要重写Activity的onConfigurationChanged方法。实现方式如下，不需要做太多的内容***：

需要注意的是，onConfigurationChanged函数中只能获得横竖屏切换后的参数，在该函数中获取不到新的Layout和控件的尺寸位置信息，如果要处理尺寸和位置信息，必须通过消息异步或者延时调用；


```java
@Override
        public void onConfigurationChanged(Configuration newConfig) {
                super.onConfigurationChanged(newConfig);
                if (getResources().getConfiguration().orientation == Configuration.ORIENTATION_LANDSCAPE) {
                        // land do nothing is ok
                } else if (getResources().getConfiguration().orientation == Configuration.ORIENTATION_PORTRAIT) {
                        // port do nothing is ok
                }
        }
```



***第四，自适应切换屏幕：***


如果想让它启动的时候是横屏的话就横屏表示，纵屏的话就纵屏表示，然后手机切换横竖屏就不能用了该怎么解决呢？

首先：在Mainfest.xml中追加

```java
android:screenOrientation="sensor" android:configChanges="orientation|keyboardHidden"
```


然后：取得屏幕的长和宽，进行比较设置横竖屏的变量。



```java
Display display = getWindowManager().getDefaultDisplay();
		int width = display.getWidth();
		int height = display.getHeight();
		if (width > height) {
			orientation = ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE; // 横屏
		} else {
			orientation = ActivityInfo.SCREEN_ORIENTATION_PORTRAIT; // 竖屏
		}
```


然后：在onConfigurationChanged（）函数中追加this.setRequestedOrientation(orientation)





```java
public void onConfigurationChanged(Configuration newConfig) {  
			     super.onConfigurationChanged(newConfig);  
			     this.setRequestedOrientation(orientation);  
			 }
```




但是这样的话你切到别的画面的时候再回到原画面，它就仍然是横的或者是纵的。怎么让它从别的屏幕回来后，又重新横竖屏布局呢？

只要在OnResume()中在设定下就行了，但是这个仅仅只是支持横竖屏只有一个layout的；



```java
protected void onResume() {
		orientation = ActivityInfo.SCREEN_ORIENTATION_USER;
		this.setRequestedOrientation(orientation);
		Display display = getWindowManager().getDefaultDisplay();
		int width = display.getWidth();
		int height = display.getHeight();
		if (width > height) {
			orientation = ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE;
		} else {
			orientation = ActivityInfo.SCREEN_ORIENTATION_PORTRAIT;
		}
		super.onResume();
	}
```






***有三点请注意：***


**1、不设置Activity的android:configChanges时，切屏会重新调用各个生命周期，切横屏时会执行一次，切竖屏时会执行两次2、设置Activity的android:configChanges="orientation"时，切屏还是会重新调用各个生命周期，切横、竖屏时只会执行一次3、设置Activity的android:configChanges="orientation|keyboardHidden"时，切屏不会重新调用各个生命周期，只会执行onConfigurationChanged方法**




建一个群，方便大家交流：蹦蹦哒Android <群号：423923313>





