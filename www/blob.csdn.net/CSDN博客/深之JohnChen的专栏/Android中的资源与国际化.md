# Android中的资源与国际化 - 深之JohnChen的专栏 - CSDN博客

2013年08月01日 22:01:07[byxdaz](https://me.csdn.net/byxdaz)阅读数：1919
个人分类：[Android](https://blog.csdn.net/byxdaz/article/category/715665)



Android中的资源与国际化

今天给大家分享一下Android中的资源与国际化的问题,通常我们新建一个Android工程，目录结构如下图所示:

![](http://pic001.cnblogs.com/img/oliveccc/201009/2010091522375060.png)　　我们主要看一下layout与values目录,layout里的xml文件的我们应用使用布局的文件，values里的sring.xml是用来存放文字资源，一个key对应一个value值。

　　但是在实际应用开发中，通常横屏(land)与竖屏(port)可能布局文件有所不同，这时候我们可以独自定义横屏与竖屏的布局文件(文件名字要一样)，默认情况是加载layout目录里的布局文件。同样应用还要支持不同的语言，如果我们应用里没有定义手机所用语言的资源时，会默认加载values的值。

　　为了方便大家理解下面做个简单的Demo.具体步骤如下:

　　第一步:新建一个Android工程，命名为ResourceDemo。

　　我们看一下layout目录下的自动生成的main.xml布局文件，代码如下:

[view plain](http://blog.csdn.net/Android_Tutor/archive/2010/09/09/5874367.aspx)[copy to
 clipboard](http://blog.csdn.net/Android_Tutor/archive/2010/09/09/5874367.aspx)[print](http://blog.csdn.net/Android_Tutor/archive/2010/09/09/5874367.aspx)[?](http://blog.csdn.net/Android_Tutor/archive/2010/09/09/5874367.aspx)

```
<?xml version="1.0" encoding="utf-8"?>  
<LinearLayout xmlns:android="http://schemas.android.com/apk/res/android"  
    android:orientation="vertical"  
    android:layout_width="fill_parent"  
    android:layout_height="fill_parent"  
     >  
 <TextView    
   android:layout_width="fill_parent"   
    android:layout_height="wrap_content"   
    android:text="@string/hello"  
  />  
</LinearLayout>
```

<?xml version="1.0" encoding="utf-8"?><LinearLayout xmlns:android="http://schemas.android.com/apk/res/android"  android:orientation="vertical"  android:layout_width="fill_parent"  android:layout_height="fill_parent"  ><TextView   android:layout_width="fill_parent"   android:layout_height="wrap_content"   android:text="@string/hello"  /></LinearLayout>

　　其中我们程序显示的文本内容是在values/string.xml文件中对应的hello的值，代码如下:

[view plain](http://blog.csdn.net/Android_Tutor/archive/2010/09/09/5874367.aspx)[copy to
 clipboard](http://blog.csdn.net/Android_Tutor/archive/2010/09/09/5874367.aspx)[print](http://blog.csdn.net/Android_Tutor/archive/2010/09/09/5874367.aspx)[?](http://blog.csdn.net/Android_Tutor/archive/2010/09/09/5874367.aspx)

```
<?xml version="1.0" encoding="utf-8"?>  
<resources>  
     <string name="hello">Hello World, ResourceDemo!</string>  
     <string name="app_name">ResourceDemo</string>  
   </resources>
```

<?xml version="1.0" encoding="utf-8"?><resources>  <string name="hello">Hello World, ResourceDemo!</string>  <string name="app_name">ResourceDemo</string></resources>

　　运行效果如下图所示:

　　port(竖屏模式)效果:

![](http://pic001.cnblogs.com/img/oliveccc/201009/2010091522381292.png)

　　Ctrl+F11快捷键模拟器变成横屏(land)模式:

![](http://pic001.cnblogs.com/img/oliveccc/201009/2010091522382644.png)

　　第二步:我们定义land与port模式的布局文件，即在res/目录下创建layout-land与layout-port两个文件夹，目录结果如下所示:

![](http://pic001.cnblogs.com/img/oliveccc/201009/2010091522385373.png)

　　layout-land目录下main.xml和layout内容基本一样，只是显示内容不同，代码如下:

[view plain](http://blog.csdn.net/Android_Tutor/archive/2010/09/09/5874367.aspx)[copy to
 clipboard](http://blog.csdn.net/Android_Tutor/archive/2010/09/09/5874367.aspx)[print](http://blog.csdn.net/Android_Tutor/archive/2010/09/09/5874367.aspx)[?](http://blog.csdn.net/Android_Tutor/archive/2010/09/09/5874367.aspx)

```
<?xml version="1.0" encoding="utf-8"?>  
<LinearLayout xmlns:android="http://schemas.android.com/apk/res/android"  
  android:orientation="vertical"  
   android:layout_width="fill_parent"  
   android:layout_height="fill_parent"  
   >  
<TextView    
  android:layout_width="fill_parent"   
  android:layout_height="wrap_content"   
  android:text="@string/land"  
    />  
</LinearLayout>
```

<?xml version="1.0" encoding="utf-8"?><LinearLayout xmlns:android="http://schemas.android.com/apk/res/android"  android:orientation="vertical"  android:layout_width="fill_parent"  android:layout_height="fill_parent"  ><TextView   android:layout_width="fill_parent"   android:layout_height="wrap_content"   android:text="@string/land"  /></LinearLayout>

　　同理layou-port目录下main.xml代码如下:

[view plain](http://blog.csdn.net/Android_Tutor/archive/2010/09/09/5874367.aspx)[copy to
 clipboard](http://blog.csdn.net/Android_Tutor/archive/2010/09/09/5874367.aspx)[print](http://blog.csdn.net/Android_Tutor/archive/2010/09/09/5874367.aspx)[?](http://blog.csdn.net/Android_Tutor/archive/2010/09/09/5874367.aspx)

```
<?xml version="1.0" encoding="utf-8"?>  
<LinearLayout xmlns:android="http://schemas.android.com/apk/res/android"  
  android:orientation="vertical"  
 android:layout_width="fill_parent"  
   android:layout_height="fill_parent"  
    >  
<TextView    
   android:layout_width="fill_parent"   
  android:layout_height="wrap_content"   
 android:text="@string/port"  
   />  
  </LinearLayout>
```

<?xml version="1.0" encoding="utf-8"?><LinearLayout xmlns:android="http://schemas.android.com/apk/res/android"  android:orientation="vertical"  android:layout_width="fill_parent"  android:layout_height="fill_parent"  ><TextView   android:layout_width="fill_parent"   android:layout_height="wrap_content"   android:text="@string/port"  /></LinearLayout>

　　当然我们显示的内容是在values/string.xml文件里定义的，这里加了两个值，代码如下:

[view plain](http://blog.csdn.net/Android_Tutor/archive/2010/09/09/5874367.aspx)[copy to
 clipboard](http://blog.csdn.net/Android_Tutor/archive/2010/09/09/5874367.aspx)[print](http://blog.csdn.net/Android_Tutor/archive/2010/09/09/5874367.aspx)[?](http://blog.csdn.net/Android_Tutor/archive/2010/09/09/5874367.aspx)

```
<?xml version="1.0" encoding="utf-8"?>  
<resources>  
<string name="hello">Hello World, ResourceDemo!</string>  
<string name="app_name">ResourceDemo</string>  
 <string name="land">This is land mode.</string>  
<string name="port">This is port mode.</string>  
 </resources>
```

<?xml version="1.0" encoding="utf-8"?><resources>  <string name="hello">Hello World, ResourceDemo!</string>  <string name="app_name">ResourceDemo</string>  <string name="land">This is land mode.</string>   <string name="port">This is port mode.</string></resources>

　　然后再次运行程序，效果如下:

　　竖屏(port)效果:

![](http://pic001.cnblogs.com/img/oliveccc/201009/2010091522392057.png)

　　横屏(land)下效果:

![](http://pic001.cnblogs.com/img/oliveccc/201009/2010091522393327.png)

　　通过上面例子可以得出如果我们已经定义了横屏与竖屏布局文件时，就不会在加载layout里的同名布局文件。

下面我们来讲点国际化，通常国际化我们只要在res/目录下在重新定义values-国家编号,如values-zh-rCN简体汉语，values-zh-rTW繁体，values-jp日语等。

　　目录结构如下图所示:

![](http://pic001.cnblogs.com/img/oliveccc/201009/2010091522395366.png)

　　这里我只在values-zh-rCN作了改动，代码如下:

[view plain](http://blog.csdn.net/Android_Tutor/archive/2010/09/09/5874367.aspx)[copy to
 clipboard](http://blog.csdn.net/Android_Tutor/archive/2010/09/09/5874367.aspx)[print](http://blog.csdn.net/Android_Tutor/archive/2010/09/09/5874367.aspx)[?](http://blog.csdn.net/Android_Tutor/archive/2010/09/09/5874367.aspx)

```
<?xml version="1.0" encoding="utf-8"?>  
 <resources>  
  <string name="hello">Hello World, ResourceDemo!</string>  
 <string name="app_name">ResourceDemo</string>  
  <string name="land">这是横屏模式.</string>  
 <string name="port">这是竖屏模式.</string>  
 </resources>
```

<?xml version="1.0" encoding="utf-8"?><resources>  <string name="hello">Hello World, ResourceDemo!</string>  <string name="app_name">ResourceDemo</string>  <string name="land">这是横屏模式.</string>   <string name="port">这是竖屏模式.</string></resources>

　　我们运行程序之前，把手机语言改成中文，在settings(设置)->language & keyboards(语言与键盘)目录下，选择简体中文，如下图所示:

![](http://pic001.cnblogs.com/img/oliveccc/201009/2010091522402347.png)

　　最然在运行上述工程，效果如下:

![](http://pic001.cnblogs.com/img/oliveccc/201009/2010091522403828.png)

　　这时候我们应用的显示内容就为中文了，而不去显示values/strings.xml里的内容。

        转载地址：[http://blog.csdn.net/Android_Tutor/article/details/5874367](http://blog.csdn.net/Android_Tutor/article/details/5874367)

