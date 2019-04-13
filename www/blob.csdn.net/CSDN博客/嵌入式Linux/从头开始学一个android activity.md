
# 从头开始学一个android activity - 嵌入式Linux - CSDN博客

2013年11月13日 10:42:05[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：594


一、类层次结构：
![](http://hi.csdn.net/attachment/201109/16/0_1316180782pNDg.gif)
二、什么是Activity，如何理解Activity
1、  用户与应用程序的交互的接口
2、  控件的容器，我们要把控件摆放在这个容器中

三、如何创建一个Activity
新建一个类：
1、  继承Activity类
**[java]**[view
 plain](http://blog.csdn.net/jiahui524/article/details/6783468#)[copy](http://blog.csdn.net/jiahui524/article/details/6783468#)[print](http://blog.csdn.net/jiahui524/article/details/6783468#)[?](http://blog.csdn.net/jiahui524/article/details/6783468#)

packagecom.jiahui.activity;

importandroid.app.Activity;
importandroid.os.Bundle;

publicclassMyActivity01ActivityextendsActivity {
/** Called when the activity is first created. */
@Override
publicvoidonCreate(Bundle savedInstanceState) {
super.onCreate(savedInstanceState);
setContentView(R.layout.main);
}
}

2、  重写onCreate()方法，Activity第一次运行时就会调用这个方法，这个方法的调用者是由应用程序框架系统调用
3、  在AndroidMainfest.xml中注册（至于AndroidMainfest.xml）
**[html]**[view
 plain](http://blog.csdn.net/jiahui524/article/details/6783468#)[copy](http://blog.csdn.net/jiahui524/article/details/6783468#)[print](http://blog.csdn.net/jiahui524/article/details/6783468#)[?](http://blog.csdn.net/jiahui524/article/details/6783468#)

<activityandroid:name=".MyActivity01Activity"
android:label="@string/app_name">
<intent-filter>
<actionandroid:name="android.intent.action.MAIN"/>
<categoryandroid:name="android.intent.category.LAUNCHER"/>
</intent-filter>
</activity>

这样一个Activity就被创建了
为了显示好看，我们可以为Activity中添加一些必要的控件，在这里我们添加一个TextView
打开Layout文件下的main.xml中所有的控件都必须在这里注册
**[html]**[view
 plain](http://blog.csdn.net/jiahui524/article/details/6783468#)[copy](http://blog.csdn.net/jiahui524/article/details/6783468#)[print](http://blog.csdn.net/jiahui524/article/details/6783468#)[?](http://blog.csdn.net/jiahui524/article/details/6783468#)

<?xmlversion="1.0"encoding="utf-8"?>
<LinearLayoutxmlns:android="http://schemas.android.com/apk/res/android"
android:orientation="vertical"android:layout_width="fill_parent"
android:layout_height="fill_parent">
<TextViewandroid:id="@+id/myText"android:layout_width="fill_parent"
android:layout_height="wrap_content"android:text="@string/hello"/>
<Buttonandroid:id="@+id/myButton"android:layout_width="fill_parent"
android:layout_height="wrap_content"/>
</LinearLayout>


回到前面在onCreate方法里
可以通过findViewById()方法得到每个控件，这个方法返回的是一个View对象所以我们要向下强转成所要的控件类
**[java]**[view
 plain](http://blog.csdn.net/jiahui524/article/details/6783468#)[copy](http://blog.csdn.net/jiahui524/article/details/6783468#)[print](http://blog.csdn.net/jiahui524/article/details/6783468#)[?](http://blog.csdn.net/jiahui524/article/details/6783468#)

packagecom.jiahui.activity;

importandroid.app.Activity;
importandroid.os.Bundle;
importandroid.widget.Button;
importandroid.widget.TextView;

publicclassMyActivity01ActivityextendsActivity {

publicvoidonCreate(Bundle savedInstanceState) {

super.onCreate(savedInstanceState);

setContentView(R.layout.main);// 设置这个Activtiy使用的布局文件

TextView myTextView = (TextView) findViewById(R.id.myText);
Button myButton = (Button) findViewById(R.id.myButton);

myTextView.setText("我的第一个TextView");
myButton.setText("我的第一个按钮");

}
}


运行结果：
![](http://hi.csdn.net/attachment/201109/16/0_1316180820R1j1.gif)

