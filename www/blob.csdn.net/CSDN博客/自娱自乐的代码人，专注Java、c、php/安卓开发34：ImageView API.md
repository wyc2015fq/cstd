# 安卓开发34：ImageView API - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2013年09月03日 20:49:53[initphp](https://me.csdn.net/initphp)阅读数：2023
所属专栏：[安卓开发](https://blog.csdn.net/column/details/androidgo.html)









**ImageView**

译者署名： cnmahj、农民伯伯

译者博客：[http://blog.toolib.net/cnmahj](http://blog.toolib.net/cnmahj)

版本：Android 3.1 r1



**结构**

继承关系

***public classView.OnClickListner extends View***



java.lang.Object

android.view.View

android.widget.ImageView



直接子类

ImageButton, QuickContactBadge 



间接子类

ZoomButton



**类概述**

显示任意图像，例如图标。ImageView类可以加载各种来源的图片（如资源或图片库），需要计算图像的尺寸，比便它可以在其他布局中使用，并提供例如缩放和着色（渲染）各种显示选项。



**嵌套类**

enum       ImageView.ScaleType

将图片边界缩放，以适应视图边界时的可选项



**XML属性**
|属性名称|描述| | | | | | | | | | | | | | | | | | | | | | | | | | | |
|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|
|**android:adjustViewBounds**|设置该属性为真可以在 ImageView调整边界时保持图片的纵横比例。（译者注：需要与maxWidth、MaxHeight一起使用，否则单独使用没有效果。）| | | | | | | | | | | | | | | | | | | | | | | | | | | |
|**android:baseline**|视图内基线的偏移量| | | | | | | | | | | | | | | | | | | | | | | | | | | |
|**android:baselineAlignBottom**|如果为true，图像视图将基线与父控件底部边缘对齐。| | | | | | | | | | | | | | | | | | | | | | | | | | | |
|**android:cropToPadding**|如果为真，会剪切图片以适应内边距的大小。（译者注：是否截取指定区域用空白代替。单独设置无效果，需要与scrollY一起使用，效果如下，实现代码见代码部分：![](http://images.cnblogs.com/cnblogs_com/over140/2010/9/2010-9-17_2.jpg)）| | | | | | | | | | | | | | | | | | | | | | | | | | | |
|**android:maxHeight**|为视图提供最大高度的可选参数。（译者注：单独使用无效，需要与setAdjustViewBounds一起使用。如果想设置图片固定大小，又想保持图片宽高比，需要如下设置：1） 设置setAdjustViewBounds为true；2） 设置maxWidth、MaxHeight；3） 设置设置layout_width和layout_height为wrap_content。）| | | | | | | | | | | | | | | | | | | | | | | | | | | |
|**android:maxWidth**|为视图提供最大宽度的可选参数。| | | | | | | | | | | | | | | | | | | | | | | | | | | |
|**android:scaleType**|控制为了使图片适合 ImageView的大小，应该如何变更图片大小或移动图片。一定是下列常量之一：|常量|值|描述||----|----|----||matrix|0|用矩阵来绘图||fitXY|1|拉伸图片（不按比例）以填充View的宽高||fitStart|2|按比例拉伸图片，拉伸后图片的高度为View的高度，且显示在View的左边||fitCenter|3|按比例拉伸图片，拉伸后图片的高度为View的高度，且显示在View的中间||fitEnd|4|按比例拉伸图片，拉伸后图片的高度为View的高度，且显示在View的右边||center|5|按原图大小显示图片，但图片宽高大于View的宽高时，截图图片中间部分显示||centerCrop|6|按比例放大原图直至等于某边View的宽高显示。||centerInside|7|当原图宽高或等于View的宽高时，按原图大小居中显示；反之将原图缩放至View的宽高居中显示。|（译者注：设置图片的填充方式。）|常量|值|描述|matrix|0|用矩阵来绘图|fitXY|1|拉伸图片（不按比例）以填充View的宽高|fitStart|2|按比例拉伸图片，拉伸后图片的高度为View的高度，且显示在View的左边|fitCenter|3|按比例拉伸图片，拉伸后图片的高度为View的高度，且显示在View的中间|fitEnd|4|按比例拉伸图片，拉伸后图片的高度为View的高度，且显示在View的右边|center|5|按原图大小显示图片，但图片宽高大于View的宽高时，截图图片中间部分显示|centerCrop|6|按比例放大原图直至等于某边View的宽高显示。|centerInside|7|当原图宽高或等于View的宽高时，按原图大小居中显示；反之将原图缩放至View的宽高居中显示。|
|常量|值|描述| | | | | | | | | | | | | | | | | | | | | | | | | | |
|matrix|0|用矩阵来绘图| | | | | | | | | | | | | | | | | | | | | | | | | | |
|fitXY|1|拉伸图片（不按比例）以填充View的宽高| | | | | | | | | | | | | | | | | | | | | | | | | | |
|fitStart|2|按比例拉伸图片，拉伸后图片的高度为View的高度，且显示在View的左边| | | | | | | | | | | | | | | | | | | | | | | | | | |
|fitCenter|3|按比例拉伸图片，拉伸后图片的高度为View的高度，且显示在View的中间| | | | | | | | | | | | | | | | | | | | | | | | | | |
|fitEnd|4|按比例拉伸图片，拉伸后图片的高度为View的高度，且显示在View的右边| | | | | | | | | | | | | | | | | | | | | | | | | | |
|center|5|按原图大小显示图片，但图片宽高大于View的宽高时，截图图片中间部分显示| | | | | | | | | | | | | | | | | | | | | | | | | | |
|centerCrop|6|按比例放大原图直至等于某边View的宽高显示。| | | | | | | | | | | | | | | | | | | | | | | | | | |
|centerInside|7|当原图宽高或等于View的宽高时，按原图大小居中显示；反之将原图缩放至View的宽高居中显示。| | | | | | | | | | | | | | | | | | | | | | | | | | |
|**android:src**|设置可绘制对象作为 ImageView显示的内容| | | | | | | | | | | | | | | | | | | | | | | | | | | |
|**android:tint**|为图片设置着色颜色。（译者注：将图片渲染成指定的颜色。见下图：左边为原图，右边为设置后的效果，见后面代码。）| | | | | | | | | | | | | | | | | | | | | | | | | | | |



**公共方法**

public final void
**clearColorFilter** ()

（译者注：清除颜色过滤，参见[这里](http://blog.csdn.net/wanglong0537/archive/2011/04/02/6298713.aspx)）



public int **getBaseline** ()

返回部件顶端到文本基线的偏移量。如果小部件不支持基线对齐，该方法返回 -1。

返回值

小部件顶端到文本基线的偏移量；或者是 -1 
当小部件不支持基线对齐时。



public boolean **getBaselineAlignBottom** ()

返回当前视图基线是否将考虑视图的底部。

参见

setBaselineAlignBottom(boolean)



public Drawable **getDrawable** ()

返回视图的可绘制对象；如果没有关联可绘制对象，返回空。



public Matrix **getImageMatrix** ()

返回视图的选项矩阵。当绘制时，应用于视图的可绘制对象。如果没有矩阵，
函数返回空。不要更改这个矩阵。如果你要为可绘制对象设置不同的矩阵，请调用 setImageMatrix()。



public ImageView.ScaleType **getScaleType** ()

返回当前 ImageView 
使用的缩放类型。

相关 XML 
属性

android:scaleType

参见

               ImageView.ScaleType



public void **invalidateDrawable** (Drawable dr)

使指定的可绘制对象失效。

参数

dr 要设为失效的可绘制对象。



public void **jumpDrawablesToCurrentState** ()

调用与视图相关的所有可绘制对象的Drawable.jumpToCurrentState()方法。



public int[] **onCreateDrawableState** (int extraSpace)

为当前视图生成新的 Drawable
状态时发生。当视图系统检测到缓存的可绘制对象失效时，调用该方法.你可以使用 getDrawableState()
方法重新取得当前的状态。

参数

　　extraSpace 如果为非零，该值为你要在返回值的数组中存放的你自己的状态信息的数量。

返回值

返回保存了视图的当前 Drawable
状态的数组。



public void **setAdjustViewBounds** (boolean adjustViewBounds)

当你需要在 ImageView
调整边框时保持可绘制对象的比例时，将该值设为真。

参数

adjustViewBounds 是否调整边框，以保持可绘制对象的原始比例。

相关 XML
属性

             android:adjustViewBounds



public void **setAlpha** (int alpha)

（译者注：设置透明度）



public void **setBaseline** (int baseline)

设置部件顶部边界文本基线的偏移量。这个值覆盖setBaselineAlignBottom(boolean)设置的属性值。

参数

baseline 
使用的基线，或不提供设置为-1。

相关 XML属性

               android:baseline

参见

             setBaseline(int)



public void **setBaselineAlignBottom** (boolean aligned)

设置是否设置视图底部的视图基线。设置这个值覆盖setBaseline()的所有调用。

参数

aligned 如果为true，图像视图将基线与父控件底部边缘对齐。

相关 XML属性

             android:baselineAlignBottom



public final void **setColorFilter** (int color)

为图片设置着色选项。采用SRC_ATOP合成模式。

参数

color 应用的着色颜色。

相关 XML 
属性

   android:tint



public void **setColorFilter** (ColorFilter cf)

为图片应用任意颜色滤镜。

参数

cf 要应用的颜色滤镜（可能为空）



public final void **setColorFilter** (int color, PorterDuff.Mode mode)

为图片设置着色选项。

参数

color 应用的着色颜色。

mode 如何着色。标准模式为 SRC_ATOP。

相关 XML 
属性

    android:tint



public void **setImageBitmap** (Bitmap bm)

设置位图作为该 ImageView
的内容。

参数

bm 
设置的位图。



public void **setImageDrawable** (Drawable drawable)

设置可绘制对象为该 ImageView
显示的内容。

参数

drawable 设置的可绘制对象。



public void **setImageLevel** (int level)

设置图片的等级，当图片来自于 LevelListDrawable
时。（译者注：使用参见[这里](http://www.20864.com/wp/165.html)）

参数

level 图片的新的等级。



public void **setImageMatrix** (Matrix matrix)

（译者注：矩阵变换）



public void **setImageResource** (int resId)

通过资源ID设置可绘制对象为该 ImageView
显示的内容。

注意：该操作读取位图，并在 UI
线程中解码，因此可能导致反应迟缓。
如果反应迟缓，可以考虑用 setImageDrawable(Drawable)、 setImageBitmap(Bitmap)
或者 BitmapFactory 
代替。

参数

resId 可绘制对象的资源标识。

相关 XML
属性

               android:src



public void **setImageState** (int[] state, boolean merge)

（译者注：设置视图的可见和不可见，使用参见[这里](http://www.20864.com/wp/165.html)）



public void **setImageURI** (Uri uri)

设置指定的 URI 
为该 ImageView 显示的内容。

注意：该操作读取位图，并在 UI
线程中解码，因此可能导致反应迟缓。
如果反应迟缓，可以考虑用 setImageDrawable(Drawable)、 setImageBitmap(Bitmap)
或者 BitmapFactory 
代替。

参数

uri  图像的 URI。



public void **setMaxHeight** (int maxHeight)

用于设置该视图支持的最大高度的可选参数。只有 setAdjustViewBounds(boolean)
为真时有效。要设置图像最大尺寸为 100×100，并保持原始比率，做法如下：

1)设置 adjustViewBounds
为真；

2)设置 maxWidth 
和 maxHeight 为 100；

3)设置宽、高的布局参数为 WRAP_CONTENT。

注意，如果原始图像较小，即使设置了该参数，图像仍然要比 100×100
小。如果要设置图片为固定大小，需要在布局参数中指定大小，并使用 setScaleType(ImageView.ScaleType)
函数来检测，如何
将其调整到适当的大小。

参数

maxHeight 该视图的最大高度。

相关 XML 
属性

    android:maxHeight



public void **setMaxWidth** (int maxWidth)

用于设置该视图支持的最大宽度的可选参数。只有 setAdjustViewBounds(boolean)
为真时有效。要设置图像最大尺寸为 100×100，并保持原始比率，做法如下：

4)设置 adjustViewBounds
为真；

5)设置 maxWidth 
和 maxHeight 为 100；

6)设置宽、高的布局参数为 WRAP_CONTENT。

注意，如果原始图像较小，即使设置了该参数，图像仍然要比 100×100
小。如果要设置图片为固定大小，需要在布局参数中指定大小，并使用 setScaleType(ImageView.ScaleType)
函数来检测，如何
将其调整到适当的大小。

参数

maxWidth 该视图的最大宽度。

相关 XML 
属性

                            android:maxWidth



public void **setScaleType** (ImageView.ScaleType scaleType)

控制图像应该如何缩放和移动，以使图像与 ImageView
一致。

参数

scaleType 需要的缩放方式。

相关 XML 
属性

    android:scaleType



public void **setSelected** (boolean selected)

改变视图的选中状态。视图有选中和未选中两个状态。注意，选择状态不同于焦点。典型的选中的视图是象 ListView 
和 GridView 这样的 AdapterView 
中显示的内容；选中的内容会显示为高亮。

参数

selected 为真，将视图设为选中状态；否则为假。



**受保护方法**

protected void **drawableStateChanged** ()

在视图状态的变化影响到所显示可绘制对象的状态时调用该方法。

覆盖该方法时，要确保调用了父类的该方法。



protected void **onDraw** (Canvas canvas)

实现该方法，用于自己绘制内容。

参数

canvas 用于绘制背景的画布。



protected void **onMeasure** (int widthMeasureSpec, int heightMeasureSpec)

评估视图及其内容，以决定其宽度和高度.此方法由 measure(int, int)
调用，子类可以重载以提供更精确、更有效率的衡量其内容尺寸的方法。

约定：
覆盖该方法时，必须调用 setMeasuredDimension(int, int) 方法来保存评估结果的视图的宽度和高度.如果忘记将导致 measure(int, int)
方法抛出IllegalStateException异常。要有效的利用父类的 onMeasure(int, int)方法。

基类测量的是背景的大小，除非 MeasureSpec
允许超过背景.子类应该重写 onMeasure(int, int)
方法，以为其内容提供更适合的大小。

如果重写了该方法，子类要确保其高度和宽度大于等于视图的最小高度和宽度.
（getSuggestedMinimumHeight() 和 getSuggestedMinimumWidth()）

参数

widthMeasureSpec 父视图要求的横向空间大小.该要求由 View.MeasureSpec
进行了编码处理。

heightMeasureSpec 父视图要求的纵向空间大小.该要求由 View.MeasureSpec
进行了编码处理。



protected boolean **onSetAlpha** (int alpha)

透明度改变时执行。子类可以使用该方法指定透明度值，然后返回真；在调用 onDraw() 时，使用该透明度值。如果返回假，则先在不可见的缓存中绘制视图，完成该请求；看起来不错，但是可能相对于在子类中绘制要慢。默认实现返回假。

参数

alpha 应用到视图的透明度值 (0…255)。

返回值

如果该类可以绘制该阿尔法值返回真。



protected boolean **setFrame** (int l, int t, int r, int b)

为视图指定大小和位置。
该方法有布局调用。

参数

l 左侧位置，相对于父容器。

t 顶部位置，相对于父容器。

r 右侧位置，相对于父容器。

b 底部位置，相对于父容器。

返回值

             true 如果新的大小和位置与之前的不同，返回真。



protected boolean **verifyDrawable** (Drawable dr)

如果你的视图子类显示自己的可绘制对象，他应该重写此方法并为自己的每个可绘制对象返回真。该函数允许为这些可绘制对象准备动画效果。

重写此方法时，要保证调用其父类的该方法。

参数

dr 待校验的可绘制对象.如果是你显示的对象之一，返回真；否则返回调用父类的返回值。

返回值

    boolean 如果可绘制对象已经显示在视图上了，返回真；否则返回假，不允许动画效果。



**补充**

文章精选

[Android ImageView 加边框](http://www.cnblogs.com/shadowjl/archive/2011/04/01/2002799.html)

[Android用ImageView显示本地和网上的图片](http://blog.3gstdy.com/archives/38)

[imageView动画效果](http://wang-peng1.iteye.com/blog/830311)

示例代码

                  android:tint

<ImageViewandroid:background=*"@android:color/white"*android:src=*"@drawable/btn_mode_switch_bg"*

android:layout_width=*"wrap_content"*android:layout_height=*"wrap_content"*></ImageView>

<ImageViewandroid:layout_marginLeft=*"5dp"*android:background=*"@android:color/white"*android:tint=*"#ffff00"*android:src=*"@drawable/btn_mode_switch_bg"*


android:layout_width=*"wrap_content"*android:layout_height=*"wrap_content"*></ImageView>



android:cropToPadding

<ImageViewandroid:background=*"@android:color/white"*android:scrollY=*"-10px"*android:cropToPadding=*"true"*android:src=*"@drawable/btn_mode_switch_bg"*

android:layout_width=*"wrap_content"*android:layout_height=*"wrap_content"*></ImageView>

<ImageViewandroid:background=*"@android:color/white"*android:scrollY=*"10px"*android:cropToPadding=*"true"*android:src=*"@drawable/btn_mode_switch_bg"*

android:layout_width=*"wrap_content"*android:layout_height=*"wrap_content"*></ImageView>

<ImageViewandroid:paddingTop=*"10px"*android:background=*"@android:color/white"*android:scrollY=*"10px"*android:cropToPadding=*"true"*android:src=*"@drawable/btn_mode_switch_bg"*

android:layout_width=*"wrap_content"*android:layout_height=*"wrap_content"*></ImageView>

<ImageViewandroid:paddingTop=*"10px"*android:background=*"@android:color/white"*android:scrollY=*"10px"*android:cropToPadding=*"false"*android:src=*"@drawable/btn_mode_switch_bg"*

android:layout_width=*"wrap_content"*android:layout_height=*"wrap_content"*></ImageView>






**ImageView.ScaleType**

译者署名：小易

译者博客：[http://blog.sina.com.cn/xjtuyi](http://blog.sina.com.cn/xjtuyi)

版本：Android 3.1 r1



**结构**

继承关系

***public static final enumImageView.ScaleType extends Enum<E extends Enum<E>>***



java.lang.Object

java.lang.Enum<E extends java.lang.Enum<E>>

android.widget.ImageView.ScaleType



**类概述**

将图片边界缩放，以适应视图边界时的可选项。



**常量**

public static final ImageView.ScaleType
**CENTER**

在视图中使图像居中，不执行缩放。在 XML 中可以使用的语法：android:scaleType="center"。



public static final ImageView.ScaleType
**CENTER_CROP**

均衡的缩放图像（保持图像原始比例），使图片的两个坐标（宽、高）都大于等于相应的视图坐标（负的内边距）。图像则位于视图的中央。
在 XML 中可以使用的语法：android:scaleType="centerCrop"。



public static final ImageView.ScaleType
**CENTER_INSIDE**

均衡的缩放图像（保持图像原始比例），使图片的两个坐标（宽、高）都小于等于相应的视图坐标（负的内边距）。图像则位于视图的中央。
在 XML 中可以使用的语法：android:scaleType="centerInside"。



public static final ImageView.ScaleType
**FIT_CENTER**

使用 CENTER
方式缩放图像。在 XML
中可以使用的语法：android:scaleType="fitCenter"。



public static final ImageView.ScaleType
**FIT_END**

使用 END
方式缩放图像。在 XML
中可以使用的语法：android:scaleType="fitEnd"。



public static final ImageView.ScaleType
**FIT_START**

使用 START
方式缩放图像。在 XML
中可以使用的语法：android:scaleType="fitStart"。



public static final ImageView.ScaleType
**FIT_XY**

使用 FILL
方式缩放图像。在 XML
中可以使用的语法：android:scaleType="fitXY"。



public static final ImageView.ScaleType
**MATRIX**

绘制时，使用图像矩阵方式缩放。图像矩阵可以通过 setImageMatrix(Matrix)
设置。在 XML 中可以使用的语法：android:scaleType="matrix"。




