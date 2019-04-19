# Android下fill_parent/match_parent和wrap_content分析 - xqhrs232的专栏 - CSDN博客
2012年04月03日 16:42:04[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1459
原文地址::[http://blog.csdn.net/jumping_android/article/details/7397991](http://blog.csdn.net/jumping_android/article/details/7397991)
fill_parent设置一个顶部布局或控件强制性让它布满整个屏幕。
wrap_content布局指根据视图内部内容自动扩展以适应其大小。
#### 1. wrap_content
<?xml version="1.0" encoding="utf-8"?>
<RelativeLayout xmlns:android="http://schemas.android.com/apk/res/android"
    android:layout_width="fill_parent"
    android:layout_height="fill_parent" >
    <Button
        android:id="@+id/btnButton1"
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:text="Button CJP"/>
</RelativeLayout>
![](https://img-my.csdn.net/uploads/201203/27/1332817001_2771.jpg)
#### 2. fill_parent – width
<?xml version="1.0" encoding="utf-8"?>
<RelativeLayout xmlns:android="http://schemas.android.com/apk/res/android"
    android:layout_width="fill_parent"
    android:layout_height="fill_parent" >
    <Button
        android:id="@+id/btnButton1"
        android:layout_width="fill_parent"
        android:layout_height="wrap_content"
        android:text="Button CJP"/>
</RelativeLayout>
![](https://img-my.csdn.net/uploads/201203/27/1332817163_2129.jpg)
#### 3. fill_parent – height
<?xml version="1.0" encoding="utf-8"?>
<RelativeLayout xmlns:android="http://schemas.android.com/apk/res/android"
    android:layout_width="fill_parent"
    android:layout_height="fill_parent" >
    <Button
        android:id="@+id/btnButton1"
        android:layout_width="wrap_content"
        android:layout_height="fill_parent"
        android:text="Button CJP"/>
</RelativeLayout>
![](https://img-my.csdn.net/uploads/201203/27/1332817319_9865.jpg)
#### 4. fill_parent – width, height
<?xml version="1.0" encoding="utf-8"?>
<RelativeLayout xmlns:android="http://schemas.android.com/apk/res/android"
    android:layout_width="fill_parent"
    android:layout_height="fill_parent" >
    <Button
        android:id="@+id/btnButton1"
        android:layout_width="fill_parent"
        android:layout_height="fill_parent"
        android:text="Button CJP"/>
</RelativeLayout>
![](https://img-my.csdn.net/uploads/201203/27/1332817500_6981.jpg)
android fill_parent和match_parent 的区别
[原文地址::http://w393185765.blog.163.com/blog/static/56722306201092223128453/](http://w393185765.blog.163.com/blog/static/56722306201092223128453/)
 Android2.2中match_parent和fill_parent是一个意思03 Sep 
标题里已经把这个意思表达清楚了，两个参数意思一样，只是某大仙觉得match_parent更贴切，于是从2.2开始你两个词都可以用。那么如果考虑低版本的使用情况你就需要用fill_parent了。
下面是两者相同的证据，看android.view.ViewGroup里的静态嵌套类LayoutParams中的代码：
01public static final int FILL_PARENT = -1;
02   /** 
03    * Special value for the height or width requested by a View. 
04    * MATCH_PARENT means that the view wants to be as big as its parent, 
05    * minus the parent's padding, if any. Introduced in API Level 8. 
06    */
07   public static final int MATCH_PARENT = -1;
08   /** 
09    * Special value for the height or width requested by a View. 
10    * WRAP_CONTENT means that the view wants to be just large enough to fit 
11    * its own internal content, taking its own padding into account. 
12    */
