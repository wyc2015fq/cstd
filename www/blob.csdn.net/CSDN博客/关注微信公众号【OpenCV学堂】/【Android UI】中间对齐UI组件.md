# 【Android UI】中间对齐UI组件 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2013年10月21日 20:10:11[gloomyfish](https://me.csdn.net/jia20003)阅读数：6432








每次看一下android编程都是浏览一下文档，Setup下开发环境，跑个Hello World

就以为自己会啦，这样不行啊！下决心踏踏实实学习一下android编程，UI都调不

好，所以还要拒绝浮躁，认认真真的学习，顺便记录一下学习的历程。

**屏幕中间对齐**：

比较LinerLayout与RelativeLayout中如何实现组件中间对齐的方法，演示程序

LinerLayout中要实现组件中间对齐，必须设置


`android:gravity="center"`完整的xml配置如下：




```
<?xml version="1.0" encoding="utf-8"?>
<LinearLayout xmlns:android="http://schemas.android.com/apk/res/android"
	android:orientation="vertical" 
	android:gravity="center"
	android:layout_width="fill_parent"
	android:layout_height="fill_parent" 
	android:background="#FFFFFF">
	<TextView android:layout_width="wrap_content"
		android:layout_height="wrap_content"
		android:textColor="@color/black" 
		android:text="@string/hello" />
		
	<Button android:text="Test Me" 
		android:id="@+id/button1"
		android:textColor="@color/black"
		android:layout_width="wrap_content" 
		android:layout_height="wrap_content">
	</Button>
</LinearLayout>
```
RelativeLayout中要实现多个组件的中间对齐，组件必须设置


android:layout_centerInParent="true"

完整的xml配置如下：



```
<?xml version="1.0" encoding="utf-8"?>
<RelativeLayout xmlns:android="http://schemas.android.com/apk/res/android"
    android:layout_width="fill_parent"
    android:layout_height="fill_parent"
    android:background="#FFFFFF">
    <LinearLayout xmlns:android="http://schemas.android.com/apk/res/android"
		android:orientation="vertical" 
		android:gravity="center"
		android:layout_centerInParent="true"
		android:layout_width="wrap_content"
		android:layout_height="wrap_content" 
		android:background="#FFFFFF">
	    <Button
	        android:id="@+id/button1"
	        android:layout_width="wrap_content"
	        android:layout_height="wrap_content"
	        android:layout_margin="5dp"
	        android:text="Test Button1"
	        style="@style/btnStyleBeige"/>
	    <Button
			android:id="@+id/button2"
			android:layout_width="wrap_content"
			android:layout_height="wrap_content"
			android:layout_margin="5dp"
			android:text="Test Button2"
			style="@style/btnStyleBeige"/>
	</LinearLayout>
</RelativeLayout>
```






