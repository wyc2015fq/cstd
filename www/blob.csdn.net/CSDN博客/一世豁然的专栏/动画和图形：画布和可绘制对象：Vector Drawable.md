# 动画和图形：画布和可绘制对象：Vector Drawable - 一世豁然的专栏 - CSDN博客





2017年05月19日 15:02:46[一世豁然](https://me.csdn.net/Explorer_day)阅读数：757








VectorDrawable是一个在XML文件中定义的向量图形，它是一组点，线和曲线以及相关联的颜色信息。 使用向量可绘制的主要优点是图像可扩展性。 它可以缩放而不损失显示质量，这意味着相同的文件被调整为不同的屏幕密度而不损失图像质量。 这会导致较小的APK文件和较少的开发人员维护。 您还可以通过使用多个XML文件而不是每个显示分辨率的多个图像来使用矢量图像进行动画。




我们来看一个例子来了解好处。 尺寸为100×100dp的图像可以在更小的显示分辨率上呈现良好的质量。 在较大的设备上，该应用可能需要使用400 x 400 dp版本的映像。 通常，开发人员创建资产的多个版本以满足不同的屏幕密度。 这种方法消耗更多的开发努力，并导致更大的APK，这在设备上占用更多的空间。





从Android 5.0（API级别21）开始，有两个类支持矢量图形作为可绘制资源：VectorDrawable和AnimatedVectorDrawable。 有关使用VectorDrawable和AnimatedVectorDrawable类的更多信息，请阅读关于VectorDrawable类和关于AnimatedVectorDrawable类部分。





在Android 5.0（API级别21）之前，支持库23.2或更高版本可以完全支持Vector Drawables和Animated Vector Drawable。 有关使用支持库的更多信息，请阅读“向量可绘制向后兼容性解决方案”部分。





一、关于VectorDrawable类

VectorDrawable定义了一个静态的可绘制对象。 类似于SVG格式，每个矢量图形被定义为一个树级别，它由路径和组对象组成。 每个路径包含对象轮廓的几何，组包含转换的细节。 所有路径都按照XML文件中显示的顺序绘制。


![](https://img-blog.csdn.net/20170519143742128?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


图1.向量可绘制资产的样本层次结构





Vector Asset Studio工具提供了一种将矢量图形作为XML文件添加到项目中的简单方法。





1、示例XML

下面是VectorDrawable XML文件，用于在充电模式下呈现电池的图像。




```
<!-- res/drawable/battery_charging.xml -->
<vector xmlns:android="http://schemas.android.com/apk/res/android"
    <!-- intrinsic size of the drawable -->
    android:height="24dp"
    android:width="24dp"
    <!-- size of the virtual canvas -->
    android:viewportWidth="24.0"
    android:viewportHeight="24.0">
   <group
         android:name="rotationGroup"
         android:pivotX="10.0"
         android:pivotY="10.0"
         android:rotation="15.0" >
      <path
        android:name="vect"
        android:fillColor="#FF000000"
        android:pathData="M15.67,4H14V2h-4v2H8.33C7.6,4 7,4.6 7,5.33V9h4.93L13,7v2h4V5.33C17,4.6 16.4,4 15.67,4z"
        android:fillAlpha=".3"/>
      <path
        android:name="draw"
        android:fillColor="#FF000000"
        android:pathData="M13,12.5h2L11,20v-5.5H9L11.93,9H7v11.67C7,21.4 7.6,22 8.33,22h7.33c0.74,0 1.34,-0.6 1.34,-1.33V9h-4v3.5z"/>
   </group>
</vector>
```


此XML呈现以下图像：

![](https://img-blog.csdn.net/20170519144057160?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





二、关于AnimatedVectorDrawable类

AnimatedVectorDrawable将动画添加到矢量图形的属性。 您可以将动画矢量图形定义为三个单独的资源文件或定义整个drawable的单个XML文件。 我们来看看这两种方法来更好地理解：多个XML文件和单个XML文件。





一）、多个XML文件

通过使用这种方法，您可以定义三个单独的XML文件：


 1）、一个VectorDrawable XML文件。

 2）、AnimatedVectorDrawable XML文件，用于定义目标VectorDrawable，目标路径和动画组，属性以及定义为ObjectAnimator对象或AnimatorSet对象的动画。

 3）、一个动画XML文件。




1、多个XML文件示例




以下XML文件演示了矢量图形的动画。


 1）、VectorDrawable的XML文件：vd.xml



```
<vector xmlns:android="http://schemas.android.com/apk/res/android"
   android:height="64dp"
   android:width="64dp"
   android:viewportHeight="600"
   android:viewportWidth="600" >
   <group
      android:name="rotationGroup"
      android:pivotX="300.0"
      android:pivotY="300.0"
      android:rotation="45.0" >
      <path
         android:name="vectorPath"
         android:fillColor="#000000"
         android:pathData="M300,70 l 0,-70 70,70 0,0 -70,70z" />
   </group>
</vector>
```


 2）、AnimatedVectorDrawable的XML文件：avd.xml



```
<animated-vector xmlns:android="http://schemas.android.com/apk/res/android"
   android:drawable="@drawable/vd" >
     <target
         android:name="rotationGroup"
         android:animation="@anim/rotation" />
     <target
         android:name="vectorPath"
         android:animation="@anim/path_morph" />
</animated-vector>
```


 3）、动画在AnimatedVectorDrawable的XML文件中使用的XML文件：rotation.xml和path_morph.xml



```
<objectAnimator
   android:duration="6000"
   android:propertyName="rotation"
   android:valueFrom="0"
   android:valueTo="360" />
```

```
<set xmlns:android="http://schemas.android.com/apk/res/android">
   <objectAnimator
      android:duration="3000"
      android:propertyName="pathData"
      android:valueFrom="M300,70 l 0,-70 70,70 0,0   -70,70z"
      android:valueTo="M300,70 l 0,-70 70,0  0,140 -70,0 z"
      android:valueType="pathType"/>
</set>
```



二）、单个XML文件

通过使用此方法，您可以通过XML捆绑格式将相关的XML文件合并为单个XML文件。 在构建应用程序时，aapt标签创建单独的资源，并在动画向量中引用它们。 此方法需要Build Tools 24或更高版本，并且输出向后兼容。





1、单个XML文件的示例



```
<animated-vector
    xmlns:android="http://schemas.android.com/apk/res/android"
    xmlns:aapt="http://schemas.android.com/aapt">
    <aapt:attr name="android:drawable">
        <vector
            android:width="24dp"
            android:height="24dp"
            android:viewportWidth="24"
            android:viewportHeight="24">
            <path
                android:name="root"
                android:strokeWidth="2"
                android:strokeLineCap="square"
                android:strokeColor="?android:colorControlNormal"
                android:pathData="M4.8,13.4 L9,17.6 M10.4,16.2 L19.6,7" />
        </vector>
    </aapt:attr>
    <target android:name="root">
        <aapt:attr name="android:animation">
            <objectAnimator
                android:propertyName="pathData"
                android:valueFrom="M4.8,13.4 L9,17.6 M10.4,16.2 L19.6,7"
                android:valueTo="M6.4,6.4 L17.6,17.6 M6.4,17.6 L17.6,6.4"
                android:duration="300"
                android:interpolator="@android:interpolator/fast_out_slow_in"
                android:valueType="pathType" />
        </aapt:attr>
    </target>
</animated-vector>
```







三、向量可拉式向后兼容性解决方案

为了支持在版本5.0之前运行Android版本（API级别21）的设备上的Vector Drawable和Animated Vector Drawable，VectorDrawableCompat和AnimatedVectorDrawableCompat可通过两个新的支持库：support-vector-drawable和animated-vector-drawable提供。




Android Studio 1.4通过在构建时生成PNG文件，为矢量绘图提供了有限的兼容性支持。 然而，矢量可绘制和动画矢量绘制支持库提供了灵活性和广泛的兼容性 - 它是一个支持库，因此您可以将其与所有Android平台版本一起使用到Android 2.1（API级别为7+）。 要将应用程序配置为使用向量支持库，请将vectorDrawables元素添加到应用程序模块中的build.gradle文件中。





使用以下代码片段来配置vectorDrawables元素：




```
//For Gradle Plugin 2.0+
 android {
   defaultConfig {
     vectorDrawables.useSupportLibrary = true
    }
 }
```


```
//For Gradle Plugin 1.5 or below
android {
  defaultConfig {
    // Stops the Gradle plugin’s automatic rasterization of vectors
    generatedDensities = []
  }
  // Flag notifies aapt to keep the attribute IDs around
  aaptOptions {
    additionalParameters "--no-version-vectors"
  }
}
```


您可以在所有API 11和更高版本的设备上使用VectorDrawableCompat，以及使用AnimatedVectorDrawableCompat回到API级别7。 Android加载可绘制的方式，并不是接受可绘制的ID的每个地方（如XML文件）都支持加载向量可绘图。 幸运的是，AppCompat已经添加了许多功能，使其易于使用矢量绘图。 首先，当将AppCompat与ImageView（或诸如ImageButton和FloatingActionButton等子类）一起使用时，您可以使用新的应用程序：srcCompat属性来引用矢量绘图以及任何其他可用的android：src：



```
<ImageView
  android:layout_width="wrap_content"
  android:layout_height="wrap_content"
  app:srcCompat="@drawable/ic_add" />
```


要在运行时更改drawables，您可以像以前一样使用setImageResource（）方法。 使用AppCompat和app：srcCompat是将矢量绘图集成到应用程序中的最简单的方法。




一）、支持库限制

在API 21之前的平台上运行时，动画矢量可以做什么样的动作有一些限制。以下列表说明与支持库不兼容的功能：


1、路径变形（PathType evaluation）用于将一条路径转变为另一条路径。




2、路径插补用于定义一个灵活的插值器（表示为路径），而不是像LinearInterpolator这样的系统定义的插值器。




3、沿着路径移动几何对象可以沿任意路径移动，作为动画的一部分。




二）、使用支持库的多个XML文件的示例

以下XML文件演示了使用多个XML文件来动画化矢量图形的方法。





1、VectorDrawable的XML文件：vd.xml



```
<vector xmlns:android="http://schemas.android.com/apk/res/android"
   android:height="64dp"
   android:width="64dp"
   android:viewportHeight="600"
   android:viewportWidth="600" >
   <group
      android:name="rotationGroup"
      android:pivotX="300.0"
      android:pivotY="300.0"
      android:rotation="45.0" >
      <path
         android:name="vectorPath"
         android:fillColor="#000000"
         android:pathData="M300,70 l 0,-70 70,70 0,0 -70,70z" />
   </group>
</vector>
```


2、AnimatedVectorDrawable的XML文件：avd.xml



```
<animated-vector xmlns:android="http://schemas.android.com/apk/res/android"
   android:drawable="@drawable/vd" >
     <target
         android:name="rotationGroup"
         android:animation="@anim/rotation" />
</animated-vector>
```


3、动画在AnimatedVectorDrawable的XML文件中使用的XML文件：rotation.xml



```
<objectAnimator
   android:duration="6000"
   android:propertyName="rotation"
   android:valueFrom="0"
   android:valueTo="360" />
```



三）、单个XML文件

以下XML文件演示了使用单个XML文件来动画化矢量图形的方法。 在构建应用程序时，aapt标签创建单独的资源，并在动画向量中引用它们。 此方法需要Build Tools 24或更高版本，并且输出向后兼容。





1、使用支持库的单个XML文件的示例



```
<animated-vector
    xmlns:android="http://schemas.android.com/apk/res/android"
    xmlns:aapt="http://schemas.android.com/aapt">
    <aapt:attr name="android:drawable">
        <vector xmlns:android="http://schemas.android.com/apk/res/android"
            android:width="64dp"
            android:height="64dp"
            android:viewportWidth="600"
            android:viewportHeight="600">
            <group
                android:name="rotationGroup"
                android:pivotX="300"
                android:pivotY="300"
                android:roation="45.0" >
                <path
                    android:name="vectorPath"
                    android:fillColor="#000000"
                    android:pathData="M300,70 l 0,-70 70,70 0,0 -70,70z" />
            </group>
        </vector>
    </aapt:attr>
    <target android:name="rotationGroup">
        <aapt:attr name="android:animation">
            <objectAnimator
                android:propertyName="rotation"
                android:valueFrom="0"
                android:valueTo="360"
                android:duration="6000"
                android:interpolator="@android:interpolator/fast_out_slow_in" />
        </aapt:attr>
    </target>
</animated-vector>
```





