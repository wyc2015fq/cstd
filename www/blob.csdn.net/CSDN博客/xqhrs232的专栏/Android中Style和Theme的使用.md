# Android中Style和Theme的使用 - xqhrs232的专栏 - CSDN博客
2012年03月23日 11:00:29[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：609标签：[android																[xhtml																[encoding																[layout																[application																[styles](https://so.csdn.net/so/search/s.do?q=styles&t=blog)](https://so.csdn.net/so/search/s.do?q=application&t=blog)](https://so.csdn.net/so/search/s.do?q=layout&t=blog)](https://so.csdn.net/so/search/s.do?q=encoding&t=blog)](https://so.csdn.net/so/search/s.do?q=xhtml&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://blog.csdn.net/wuxianglong/article/details/6337945](http://blog.csdn.net/wuxianglong/article/details/6337945)
相关网帖::
1. Android用style简化layout文件----[http://mycoding.iteye.com/blog/966726](http://mycoding.iteye.com/blog/966726)
2.[Android开发之：如何使用样式和主题](http://www.ideasandroid.com/archives/322)----[http://www.ideasandroid.com/archives/322](http://www.ideasandroid.com/archives/322)
**Android平台的样式和主题**
样式见：[http://www.ideasandroid.com/android/sdk/styles.xml](http://www.ideasandroid.com/android/sdk/styles.xml)
主题见：[http://www.ideasandroid.com/android/sdk/themes.xml](http://www.ideasandroid.com/android/sdk/themes.xml)
### Style：
Style是View中一些属性的集合，包括height，padding，font color，background等等，Style单独定义在xml文件中，类似与web页面中css的角色，将设计和内容分开，便于修改和重复使用。
#### 定义Style：
style文件需要保存在res/values目录下，文件名任意，但是必须是xml文件，sytle文件的根标记必须是<resources>。写了一个简单示例，效果如下：
![](http://hi.csdn.net/attachment/201104/21/0_1303352432ql9x.gif)
程序目录结构如下图，其中mystyle.xml是自定义的style文件。
![](http://hi.csdn.net/attachment/201104/21/0_1303352440vApP.gif)
main.xml文件代码：
**[xhtml]**[view plain](http://blog.csdn.net/wuxianglong/article/details/6337945#)[copy](http://blog.csdn.net/wuxianglong/article/details/6337945#)[print](http://blog.csdn.net/wuxianglong/article/details/6337945#)[?](http://blog.csdn.net/wuxianglong/article/details/6337945#)
- **<?xml**version="1.0"encoding="utf-8"**?>**
- **<LinearLayout**
- xmlns:android="http://schemas.android.com/apk/res/android"
- android:layout_width="fill_parent"
- android:layout_height="fill_parent"**>**
- **<TextView**
- style="@style/CodeFont"
- android:text="测试style"**>**
- **</TextView>**
- **</LinearLayout>**
<?xml version="1.0" encoding="utf-8"?>
<LinearLayout
    xmlns:android="http://schemas.android.com/apk/res/android"
    android:layout_width="fill_parent"
    android:layout_height="fill_parent">
    <TextView
        style="@style/CodeFont"
        android:text="测试style">
    </TextView>
</LinearLayout>
声明style是CodeFont，对应的是style文件中的style name。mystyle.xml文件中定义了style name是CodeFont：
parent属性表示style之间可以继承，同时可以覆盖parent style的一些属性。 
**[xhtml]**[view plain](http://blog.csdn.net/wuxianglong/article/details/6337945#)[copy](http://blog.csdn.net/wuxianglong/article/details/6337945#)[print](http://blog.csdn.net/wuxianglong/article/details/6337945#)[?](http://blog.csdn.net/wuxianglong/article/details/6337945#)
- **<?xml**version="1.0"encoding="utf-8"**?>**
- **<resources>**
- **<style**name="CodeFont"parent="@android:style/TextAppearance.Medium"**>**
- **<item**name="android:layout_width"**>**fill_parent**</item>**
- **<item**name="android:layout_height"**>**wrap_content**</item>**
- **<item**name="android:textColor"**>**#00FF00**</item>**
- **<item**name="android:typeface"**>**monospace**</item>**
- **</style>**
- **</resources>**
<?xml version="1.0" encoding="utf-8"?>
<resources>
    <style name="CodeFont" parent="@android:style/TextAppearance.Medium">
        <item name="android:layout_width">fill_parent</item>
        <item name="android:layout_height">wrap_content</item>
        <item name="android:textColor">#00FF00</item>
        <item name="android:typeface">monospace</item>
    </style>
</resources>
#### Style的继承：
style继承有两种方式：
- style的继承可以通过parent属性，用来继承android已经定义好的style，例如: 
**[xhtml]**[view plain](http://blog.csdn.net/wuxianglong/article/details/6337945#)[copy](http://blog.csdn.net/wuxianglong/article/details/6337945#)[print](http://blog.csdn.net/wuxianglong/article/details/6337945#)[?](http://blog.csdn.net/wuxianglong/article/details/6337945#)
- **<style**name="GreenText"parent="@android:style/TextAppearance"**>**
- **<item**name="android:textColor"**>**#00FF00**</item>**
- **</style>**
<style name="GreenText" parent="@android:style/TextAppearance">
    <item name="android:textColor">#00FF00</item>
</style>
继承了android中的TextAppearance，同时覆盖了android:textColor属性。
- 如果要继承自定义的style，不需要通过parent属性，只要style的name以需要继承的style的name开始后跟新的style的name，中间用“.”隔开。注意：这种方式只适用与自定义的style继承 。
**[xhtml]**[view plain](http://blog.csdn.net/wuxianglong/article/details/6337945#)[copy](http://blog.csdn.net/wuxianglong/article/details/6337945#)[print](http://blog.csdn.net/wuxianglong/article/details/6337945#)[?](http://blog.csdn.net/wuxianglong/article/details/6337945#)
- **<style**name="CodeFont.Red"**>**
- **<item**name="android:textColor"**>**#FF0000**</item>**
- **</style>**
<style name="CodeFont.Red">
    <item name="android:textColor">#FF0000</item>
</style>
新的style继承了CodeFont，则在修改上边例子中的main.xml为：
**[xhtml]**[view plain](http://blog.csdn.net/wuxianglong/article/details/6337945#)[copy](http://blog.csdn.net/wuxianglong/article/details/6337945#)[print](http://blog.csdn.net/wuxianglong/article/details/6337945#)[?](http://blog.csdn.net/wuxianglong/article/details/6337945#)
- **<?xml**version="1.0"encoding="utf-8"**?>**
- **<LinearLayout**
- xmlns:android="http://schemas.android.com/apk/res/android"
- android:layout_width="fill_parent"
- android:layout_height="fill_parent"**>**
- **<TextView**
- style="@style/CodeFont.Red"
- android:text="测试style"**>**
- **</TextView>**
- **</LinearLayout>**
<?xml version="1.0" encoding="utf-8"?>
<LinearLayout
    xmlns:android="http://schemas.android.com/apk/res/android"
    android:layout_width="fill_parent"
    android:layout_height="fill_parent">
    <TextView
        style="@style/CodeFont.Red"
        android:text="测试style">
    </TextView>
</LinearLayout>
效果如下，字体颜色变为了红色：
![](http://hi.csdn.net/attachment/201104/21/0_1303352445q75J.gif)
style也可以多级继承：
**[xhtml]**[view plain](http://blog.csdn.net/wuxianglong/article/details/6337945#)[copy](http://blog.csdn.net/wuxianglong/article/details/6337945#)[print](http://blog.csdn.net/wuxianglong/article/details/6337945#)[?](http://blog.csdn.net/wuxianglong/article/details/6337945#)
- **<style**name="CodeFont.Red.Big"**>**
- **<item**name="android:textSize"**>**30sp**</item>**
- **</style>**
<style name="CodeFont.Red.Big">
    <item name="android:textSize">30sp</item>
</style>
字号变大，效果如下：
![](http://hi.csdn.net/attachment/201104/21/0_1303352451oOjC.gif)
sytle的更多属性见android包下的R.attr。需要注意，并不是所有的View都支持定义的style的属性，如果自定义的sytle中包含View不支持的属性，程序会自动忽略它。
### Theme：
如果声明一个style作为Theme，需要配置mainfest文件中<activity> 或 <application>的android:theme 属性。
将自定义的style作为application的theme：
修改mystyle.xml为：
**[xhtml]**[view plain](http://blog.csdn.net/wuxianglong/article/details/6337945#)[copy](http://blog.csdn.net/wuxianglong/article/details/6337945#)[print](http://blog.csdn.net/wuxianglong/article/details/6337945#)[?](http://blog.csdn.net/wuxianglong/article/details/6337945#)
- **<?xml**version="1.0"encoding="utf-8"**?>**
- **<resources>**
- **<style**name="CodeFont"**>**
- **<item**name="android:textSize"**>**20sp**</item>**
- **<item**name="android:typeface"**>**monospace**</item>**
- **</style>**
- **</resources>**
<?xml version="1.0" encoding="utf-8"?>
<resources>
    <style name="CodeFont">
        <item name="android:textSize">20sp</item>
        <item name="android:typeface">monospace</item>
    </style>
</resources>
在mainfest 的application中添加 android:theme属性：
**[xhtml]**[view plain](http://blog.csdn.net/wuxianglong/article/details/6337945#)[copy](http://blog.csdn.net/wuxianglong/article/details/6337945#)[print](http://blog.csdn.net/wuxianglong/article/details/6337945#)[?](http://blog.csdn.net/wuxianglong/article/details/6337945#)
- **<application**android:icon="@drawable/icon"
- android:label="@string/app_name"
- android:theme="@style/CodeFont"**>**
<application android:icon="@drawable/icon"
      android:label="@string/app_name"
      android:theme="@style/CodeFont">
则application中的所有text字体都会改变，效果如下：
![](http://hi.csdn.net/attachment/201104/21/0_1303352459BSWL.gif)
在每个<activity>标签中使用android:theme属性：
**[xhtml]**[view plain](http://blog.csdn.net/wuxianglong/article/details/6337945#)[copy](http://blog.csdn.net/wuxianglong/article/details/6337945#)[print](http://blog.csdn.net/wuxianglong/article/details/6337945#)[?](http://blog.csdn.net/wuxianglong/article/details/6337945#)
- **<activity**android:name=".MainActivity"
- android:label="@string/app_name"
- android:theme="@style/CodeFont"**>**
<activity android:name=".MainActivity"
          android:label="@string/app_name"
          android:theme="@style/CodeFont">
android:theme还可以配置android中已经存在的theme：
**[xhtml]**[view plain](http://blog.csdn.net/wuxianglong/article/details/6337945#)[copy](http://blog.csdn.net/wuxianglong/article/details/6337945#)[print](http://blog.csdn.net/wuxianglong/article/details/6337945#)[?](http://blog.csdn.net/wuxianglong/article/details/6337945#)
- **<activity**android:theme="@android:style/Theme.Translucent"**>**
<activity android:theme="@android:style/Theme.Translucent">
如果想调整android已经定义好的theme，则可以通过自定义style来实现，例如：
**[xhtml]**[view plain](http://blog.csdn.net/wuxianglong/article/details/6337945#)[copy](http://blog.csdn.net/wuxianglong/article/details/6337945#)[print](http://blog.csdn.net/wuxianglong/article/details/6337945#)[?](http://blog.csdn.net/wuxianglong/article/details/6337945#)
- **<color**name="custom_theme_color"**>**#b0b0ff**</color>**
- **<style**name="CustomTheme"parent="android:Theme.Light"**>**
- **<item**name="android:windowBackground"**>**@color/custom_theme_color**</item>**
- **<item**name="android:colorBackground"**>**@color/custom_theme_color**</item>**
- **</style>**
<color name="custom_theme_color">#b0b0ff</color>
<style name="CustomTheme" parent="android:Theme.Light">
    <item name="android:windowBackground">@color/custom_theme_color</item>
    <item name="android:colorBackground">@color/custom_theme_color</item>
</style>
效果如下：
![](http://hi.csdn.net/attachment/201104/21/0_1303352512N7FM.gif)
关于在<activity>中android的Theme的详细使用见：[android Theme使用总结](http://henzil.easymorse.com/?p=364)
#### 根据android版本选择主题：
在android新的版本中增加了新的theme，如果想在新版中利用新theme同时又兼容旧版本，可以通过配置两个theme文件实现，例如在res/values目录下配置sytle.xml文件：
**[xhtml]**[view plain](http://blog.csdn.net/wuxianglong/article/details/6337945#)[copy](http://blog.csdn.net/wuxianglong/article/details/6337945#)[print](http://blog.csdn.net/wuxianglong/article/details/6337945#)[?](http://blog.csdn.net/wuxianglong/article/details/6337945#)
- **<style**name="LightThemeSelector"parent="android:Theme.Light"**>**
- **</style>**
<style name="LightThemeSelector" parent="android:Theme.Light">
</style>
在res/values-11目录下配置文件style.xml：
**[xhtml]**[view plain](http://blog.csdn.net/wuxianglong/article/details/6337945#)[copy](http://blog.csdn.net/wuxianglong/article/details/6337945#)[print](http://blog.csdn.net/wuxianglong/article/details/6337945#)[?](http://blog.csdn.net/wuxianglong/article/details/6337945#)
- **<style**name="LightThemeSelector"parent="android:Theme.Holo.Light"mce_bogus="1"**>**
- **</style>**
<style name="LightThemeSelector" parent="android:Theme.Holo.Light" mce_bogus="1">
</style>
按照文档中说的在res下创建values-11目录，程序报错，需要找时间研究一下，说不定是android的一个bug。
#### 使用Android提供的Style和Theme：
Android平台提供了大量的styles和themes，可以在android包中的R.style下找到，但是Android现在并未提供关于styles和themes的相关文档说明，具体可以参考[styles.xml源码](http://android.git.kernel.org/?p=platform/frameworks/base.git;a=blob;f=core/res/res/values/styles.xml;h=d7b654e49809cb97a35682754b1394af5c8bc88b;hb=HEAD)
 和[themes.xml源码](http://android.git.kernel.org/?p=platform/frameworks/base.git;a=blob;f=core/res/res/values/themes.xml;h=6b3d7407d1c895a3c297e60d5beac98e2d34c271;hb=HEAD) ，扫了一下，描述的很清楚。


