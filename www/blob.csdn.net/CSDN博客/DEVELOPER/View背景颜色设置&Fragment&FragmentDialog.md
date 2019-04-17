# View背景颜色设置&Fragment&FragmentDialog - DEVELOPER - CSDN博客





2014年04月19日 12:13:47[学术袁](https://me.csdn.net/u012827205)阅读数：3534








* Android 开发中使用的颜色可以分为两种，自定义颜色和系统颜色**：*

*** 一>自定义颜色：***
 颜色值的定义是通过RGB三原色和一个alpha值来定义的（加色原理）。以井号（#）开始，后面是Alpha-Red-Green-Blue的格式，形如：
     #RGB 
       #ARGB 
         #RRGGBB 
            #AARRGGBB

*通常使用#RRGGBB 或者#AARRGGBB的形式！*

*   one.在资源文件中定义颜色：*

一般在 res/values/color/color_name下建立colors.xml文件，定义颜色，如下：


```java
<?xml version="1.0" encoding="utf-8"?>
<resourses>
    <color name="red">#ff0000</color>
</resourses>
```
*  and then. 在代码中使用颜色：*
res/values/color/color_name例如：



```java
Button btn = (Button) findViewById(R.id.button);      
int color = Resources.getSystem().getColor(R.color.red);  
btn.setBackgroundColor(color);

或者：
Button btn = (Button) findViewById(R.id.button);      
int color=getResources().getColor(R.color.red);
btn.setBackgroundColor(color);
```


*  then. 在布局文件中使用颜色*


   res/values/color/color_name    例如：


```java
android:layout_width="match_parent"
 android:layout_height="wrap_content"
 android:background="@color/blue"
 android:text="Address book" />
```







***  二>系统颜色:***

android也有一些内置的颜色，例如系统资源中定义的颜色，十分有限。

android.graphics.Color类中也提供了一些颜色常量和构造颜色值的静态方法。

*one.在代码中使用系统颜色：*


```java
Color类中的颜色常量：
Button btn = (Button) findViewById(R.id.button);      
	btn.setBackgroundColor(Color.CYAN); //btn.setBackground(Color.CYAN);
或者使用Color类中的静态常量：
Button btn = (Button) findViewById(R.id.button1);      
        btn.setBackgroundColor(Color.argb(0xff, 0xff, 0x00, 0x00)); //btn.setBackground(Color.argb(0xff, 0xff, 0x00, 0x00));
```






          在这里做一下Fragment的补充，Fragment的使用能大大提升你在做UI方面的功底以及水平。那么，我就给大家一个网址。从这里你能学会Fragment，而且从这里你能重新认识Fragment。在这里包里面有一PDF文档，还有三个为使读者透彻理解Fragment的项目源码。真正的详细介绍了怎么使用Fragment。

[好吧，点击这里进入下载页面](http://download.csdn.net/detail/u012827205/7221577)





在文档中提到了FragmentDialog。所以下面我为大家提供了Fragment的源码。页面不是很美观，这是我为看一下它的效果随便填充了一个布局。效果图：

![](https://img-blog.csdn.net/20140420211309734?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


[给你个福利，FragmentDialog对话框源码。点击下载](http://download.csdn.net/detail/u012827205/7223701)




