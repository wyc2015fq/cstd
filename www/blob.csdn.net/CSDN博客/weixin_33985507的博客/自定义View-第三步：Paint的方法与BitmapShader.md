# 自定义View-第三步：Paint的方法与BitmapShader - weixin_33985507的博客 - CSDN博客
2017年02月16日 15:21:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：10
挑选几个Paint的方法讲解一下
#### 一、设置线冒(无、方、圆)->paint.setStrokeCap
![3679520-c7362c2f3352a13a.png](https://upload-images.jianshu.io/upload_images/3679520-c7362c2f3352a13a.png)
一般进度条尾部是圆的，就可以这样子设置啦！引用自http://blog.csdn.net/harvic880925/article/details/51010839
#### 二、设置外拐角(尖、直、圆)-> setStrokeJoin(Paint.Join join)
```
Paint paint = new Paint();  
paint.setStrokeWidth(40);  
paint.setColor(Color.GREEN);  
paint.setStyle(Paint.Style.STROKE);  
paint.setAntiAlias(true);  
  
Path path  = new Path();  
path.moveTo(100,100);  
path.lineTo(450,100);  
path.lineTo(100,300);  
paint.setStrokeJoin(Paint.Join.MITER);  
canvas.drawPath(path,paint);  
  
path.moveTo(100,400);  
path.lineTo(450,400);  
path.lineTo(100,600);  
paint.setStrokeJoin(Paint.Join.BEVEL);  
canvas.drawPath(path,paint);  
  
path.moveTo(100,700);  
path.lineTo(450,700);  
path.lineTo(100,900);  
paint.setStrokeJoin(Paint.Join.ROUND);  
canvas.drawPath(path,paint);
```
![3679520-ed3baf39cb5f6779.png](https://upload-images.jianshu.io/upload_images/3679520-ed3baf39cb5f6779.png)
引用自http://blog.csdn.net/harvic880925/article/details/51010839
可是内拐角呢？怎么办？别担心，继续看下去O(∩_∩)O~
#### 三、更强大点的效果->setPathEffect(PathEffect effect)
**1. CornerPathEffect**——圆形拐角效果，**有了CornerPathEffect，内外拐角都不用担心啦，可喜可贺！**
```
//将原来Path生硬的直线拐角，变成圆形拐角：
public CornerPathEffect(float radius)
```
- radius：即当前连接两条直线所使用的圆的半径
举个栗子：
```
paint.setColor(Color.BLACK);
        Path path = new Path();
        path.moveTo(100,500);
        path.lineTo(200,100);
        path.lineTo(400,500);
        canvas.drawPath(path,paint);
        paint.setColor(Color.RED);
        paint.setPathEffect(new CornerPathEffect(100));
        canvas.drawPath(path,paint);
        paint.setColor(Color.GREEN);
        paint.setPathEffect(new CornerPathEffect(200));
        canvas.drawPath(path,paint);
```
![3679520-2a45cd351284a69f.png](https://upload-images.jianshu.io/upload_images/3679520-2a45cd351284a69f.png)
CornerPathEffect效果图
**2. DashPathEffect**->虚线效果 ：**实现虚线段效果**
```
//paint设置为STROKE 和 FILL_AND_STROKE有效
public DashPathEffect(float intervals[], float phase)
```
- 
intervals[]：表示组成虚线的各个线段的长度；整条虚线就是由intervals[]中这些基本线段循环组成的.长度必须大于等于2；因为必须有一个实线段和一个空线段来组成虚线。
![3679520-e6ac5149282bf350.png](https://upload-images.jianshu.io/upload_images/3679520-e6ac5149282bf350.png)
> 
个数必须为偶数，如果是基数，最后一个数字将被忽略；这个很好理解，因为一组虚线的组成必然是一个实线和一个空线成对组成的。
- phase：开始绘制的偏移值 ，phase is an offset into the intervals array (mod the sum of all of the intervals). The intervals array controls the length of the dashes(囧囧翻译：phase是相对于 intervals[]数组中所有数据的和的偏移量， intervals[]数组控制破折号的长度)。
```
Path path = new Path();
        path.moveTo(100, 300);
        path.lineTo(300, 100);
        path.lineTo(400, 700);
        canvas.drawPath(path, paint);
        paint.setColor(Color.RED);
//使用DashPathEffect画线段
        paint.setPathEffect(new DashPathEffect(new float[]{20, 10, 10, 5}, 0));
        canvas.translate(10, 0);
        canvas.drawPath(path, paint);
//画同一条线段,偏移值为15
        paint.setPathEffect(new DashPathEffect(new float[]{20, 10, 10, 5}, 15));
        paint.setColor(Color.BLUE);
        canvas.translate(10, 0);
        canvas.drawPath(path, paint);
```
![3679520-262c6712a7550ca2.png](https://upload-images.jianshu.io/upload_images/3679520-262c6712a7550ca2.png)
DashPathEffect
利用这一特性，不断改变偏移值，就可以形成一个动画，
![3679520-16aedb914500a6b6.gif](https://upload-images.jianshu.io/upload_images/3679520-16aedb914500a6b6.gif)
小车出发，滴滴滴
代码在这里：
```
Path path = new Path();
        path.moveTo(0, 100);
        path.lineTo(getWidth(), 100);
        paint.setColor(Color.BLUE);
        canvas.drawPoint(200, 10, paint);
        //默认43
        paint.setPathEffect(new DashPathEffect(new float[]{25, 3, 5, 10}, phase));
        canvas.drawPath(path, paint);
        try {
            Thread.sleep(5);
        }
        catch (InterruptedException e) {
            e.printStackTrace();
        }
        phase--;
        if (phase <= 0) phase = 43;
        postInvalidate();
```
**3. DiscretePathEffect**—离散路径效果:将原来路径分隔成定长的线段，然后将每条线段随机偏移一段位置，我们可以用它来模拟一种类似生锈铁丝的效果
```
public DiscretePathEffect(float segmentLength, float deviation)
```
- segmentLength：将原来的路径切成多长的线段。这个值越小，所切成的小线段越多；这个值越大，所切成的小线段越少。
- deviation：表示被切成的每个小线段的可偏移距离。值越大，就表示每个线段的可偏移距离就越大，就显得越凌乱，值越小，每个线段的可偏移原位置的距离就越小
调用方式：
```
paint.setPathEffect(new DiscretePathEffect(6,5));
```
```
canvas.translate(0, 200);
        Random random1 = new Random(1);
        for (int i = 0; i < getWidth(); i += 40) {
            double height = random1.nextDouble();
            if (i == 0) path.moveTo(i, (float) (100 * height));
            else path.lineTo(i, (float) (100 * height));
        }
        //原图
        paint.setColor(Color.RED);
        canvas.drawPath(path, paint);
        DiscretePathEffect discretePathEffect = new DiscretePathEffect(10, 8);
        canvas.translate(0, 30);
        paint.setColor(Color.GREEN);
        paint.setPathEffect(discretePathEffect);
        canvas.drawPath(path, paint);
        DiscretePathEffect discretePathEffect2 = new DiscretePathEffect(20, 8);
        canvas.translate(0, 30);
        paint.setColor(Color.GRAY);
        paint.setPathEffect(discretePathEffect2);
        canvas.drawPath(path, paint);
        DiscretePathEffect discretePathEffect3 = new DiscretePathEffect(10, 18);
        canvas.translate(0, 30);
        paint.setColor(Color.BLUE);
        paint.setPathEffect(discretePathEffect3);
        canvas.drawPath(path, paint);
```
![3679520-191c1750ab581b22.png](https://upload-images.jianshu.io/upload_images/3679520-191c1750ab581b22.png)
效果对比图
**4. PathDashPathEffect**—相当于Photoshop中的印章功能,用另一个路径图案做为印章，沿着指定路径一个个绘制上去.
```
public PathDashPathEffect(Path shape, float advance, float phase,Style style)
```
- Path shape:表示印章路径，比如我们下面示例中的三角形加右上角一个点
- float advance：表示两个印章路径间的距离,很容易理解，印章间距离越大，间距就越大。
- float phase：路径绘制偏移距离，与上面DashPathEffect中的float phase参数意义相同
- Style style：表示在遇到转角时，如何操作印章以使转角平滑过渡，取值有：Style.ROTATE，Style.MORPH，Style.TRANSLATE;Style.ROTATE表示通过旋转印章来过渡转角；Style.MORPH表示通过变形印章来过渡转角；Style.TRANSLATE表示通过位移来过渡转角
5.**ComposePathEffect与SumPathEffect **
```
public ComposePathEffect(PathEffect outerpe, PathEffect innerpe)  
public SumPathEffect(PathEffect first, PathEffect second)
```
- 利用ComposePathEffect先应用innerpe特效,再应用outerpe特效
- 利用SumPathEffect,分别将first和second特效应用于原始路径,然后将生成的两条特效路径合并为新的特效
来个栗子吧
```
canvas.translate(0, 200);
        Random random1 = new Random(1);
        for (int i = 0; i < getWidth(); i += 40) {
            double height = random1.nextDouble();
            if (i == 0) path.moveTo(i, (float) (100 * height));
            else path.lineTo(i, (float) (100 * height));
        }
        //原图
        paint.setColor(Color.RED);
        canvas.drawPath(path, paint);
        //cornerPathEffect效果
        CornerPathEffect cornerPathEffect = new CornerPathEffect(100);
        canvas.translate(0, 50);
        paint.setColor(Color.GREEN);
        paint.setPathEffect(cornerPathEffect);
        canvas.drawPath(path, paint);
        //DiscretePathEffect效果
        DiscretePathEffect discretePathEffect = new DiscretePathEffect(10, 8);
        canvas.translate(0, 30);
        paint.setColor(Color.BLUE);
        paint.setPathEffect(discretePathEffect);
        canvas.drawPath(path, paint);
       //ComposePathEffect效果
        canvas.translate(0, 50);
        paint.setColor(Color.LTGRAY);
        paint.setPathEffect(new ComposePathEffect(cornerPathEffect, discretePathEffect));
        canvas.drawPath(path, paint);
        //SumPathEffect
        canvas.translate(0, 50);
        paint.setColor(Color.GRAY);
        paint.setPathEffect(new SumPathEffect(cornerPathEffect, discretePathEffect));
        canvas.drawPath(path, paint);
```
![3679520-006c42bf2f644872.png](https://upload-images.jianshu.io/upload_images/3679520-006c42bf2f644872.png)
效果图
#### 四、Paint的setShader(Shader shader)
```
它有五个子类，像PathEffect一样，它的每个子类都实现了一种Shader。
```
**1. BitmapShader：用来实现圆形图片的利器**
```
BitmapShader (Bitmap bitmap, Shader.TileMode tileX, Shader.TileMode tileY)
```
- bitmap: 要处理的bitmap对象
- tileX(tileY):在X(Y)轴处理的效果，Shader.TileMode 模式有三种：CLAMP、MIRROR和REPETA
**各个模式介绍**
|模式|效果|
|----|----|
|Shader.TileMode.CLAMP|会将边缘的一个像素进行拉伸、扩展|
|Shader.TileMode.MIRROR|镜面翻转|
|Shader.TileMode.REPEAT|平移复制|
**CLAMP：边缘像素扩展拉伸**
```
Bitmap bitmap = BitmapFactory.decodeResource(getResources(), R.mipmap.bears);
        paint.setShader(new BitmapShader(bitmap, Shader.TileMode.CLAMP, Shader.TileMode.CLAMP));
        //原图
        canvas.drawBitmap(bitmap, 0, 0, paint);
        //右图
        canvas.drawRect(0, bitmap.getHeight() + 50, bitmap.getWidth(), bitmap.getHeight() * 2 + 50, paint);
        //下图
        canvas.drawRect(bitmap.getWidth() + 50, 0, bitmap.getWidth() * 2, bitmap.getHeight(), paint);
        //右下
        canvas.drawRect(bitmap.getWidth() + 50, bitmap.getHeight() + 50, bitmap.getWidth()*2, bitmap.getHeight() * 2 + 50, paint);
```
![3679520-affca718de3e9681.png](https://upload-images.jianshu.io/upload_images/3679520-affca718de3e9681.png)
CLAMP效果图
**MIRROR：镜面翻转**
```
//只改这一句
        paint.setShader(new BitmapShader(bitmap, Shader.TileMode.MIRROR, Shader.TileMode.MIRROR));
```
![3679520-64ed950d3fc43ee8.png](https://upload-images.jianshu.io/upload_images/3679520-64ed950d3fc43ee8.png)
MIRROR
**MIRROR和CLAMP结合**
```
paint.setShader(new BitmapShader(bitmap, Shader.TileMode.CLAMP, Shader.TileMode.MIRROR));
```
![3679520-dfbe41472c4f65e0.png](https://upload-images.jianshu.io/upload_images/3679520-dfbe41472c4f65e0.png)
X:CLAMP;Y:MIRROR
```
paint.setShader(new BitmapShader(bitmap, Shader.TileMode.MIRROR, Shader.TileMode.CLAMP));
```
![3679520-f3a0fd02af4a651b.png](https://upload-images.jianshu.io/upload_images/3679520-f3a0fd02af4a651b.png)
X:MIRROR;Y:CLAMP
**由此可知:**
**  绘制过程是先采用Y轴模式，再使用X轴模式的。**
**REPEAT:平移复制**
![3679520-9e5c26dbdca0c53a.png](https://upload-images.jianshu.io/upload_images/3679520-9e5c26dbdca0c53a.png)
REPEAT效果图
**画个圆试试效果**
```
paint.setShader(new BitmapShader(bitmap, Shader.TileMode.REPEAT, Shader.TileMode.REPEAT));
        //原图
        canvas.drawBitmap(bitmap, 0, 0, paint);
        //右图
        canvas.drawCircle(bitmap.getWidth()+100,bitmap.getHeight()/2,50, paint);
        //下图
        canvas.drawCircle(bitmap.getWidth(), bitmap.getHeight()+300,250,paint);
```
![3679520-9779435ab380912a.png](https://upload-images.jianshu.io/upload_images/3679520-9779435ab380912a.png)
效果图
**圆角图片闪亮登场**
*前景提要：*
- 有一个 [Android BitmapShader 实战 实现圆形、圆角图片](https://link.jianshu.com?t=http://blog.csdn.net/lmj623565791/article/details/41967509)的博客，大家可以看一下如何实现圆角图片，下边也会把代码放上去的，改了一点点。另外，[github中的CircleImageView](https://link.jianshu.com?t=https://github.com/hdodenhof/CircleImageView)也是基于BitmapShader实现的，只是这个没有博客那么详细的介绍。
- 代码中用了封装控件，如果不懂的话，可以先看[封装控件](https://www.jianshu.com/p/5a672a593edb)这一章节内容.
- 另外，补充一下 [Android BitmapShader 实战 实现圆形、圆角图片](https://link.jianshu.com?t=http://blog.csdn.net/lmj623565791/article/details/41967509)中用到的一个知识点**TypedValue.applyDimension**：
> ``
Resources resources = getResources();
float fPx = TypedValue.applyDimension(TypedValue.COMPLEX_UNIT_DIP, 300, resources.getDisplayMetrics());
```
第一个参数为 单位，第二个参数为单位(第一个参数设置的单位)指定的值，返回值 都是像素,如TypedValue.applyDimension(TypedValue.COMPLEX_UNIT_PX, 300, resources.getDisplayMetrics()); 就是 像素 转 像素，还是300.
####放代码~O(∩_∩)O~
先来效果图：
![效果图](http://upload-images.jianshu.io/upload_images/3679520-ba49001ab565d0fc.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
这是原图片：
![原图](http://upload-images.jianshu.io/upload_images/3679520-e9d4c1a6624c6d9e.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/120)
实现的代码：
1. attr.xml
```
<?xml version="1.0" encoding="utf-8"?>
<resources>
<declare-styleable name="MineView">
<attr name="borderRadius" format="dimension"/>
<attr name="type">
<enum name="circle" value="0"/>
<enum name="round" value="1" />
</attr>
</declare-styleable>
</resources>
```
2. 自定义ImageView
```
public class MineView extends ImageView {
/**
* 图片的类型，圆形or圆角
*/private int type;private static final int TYPE_CIRCLE = 0;private static final int TYPE_ROUND = 1;private int borderRadius, mRadius;// 圆角或圆形半径private int mWidth; //圆的时候，设置view的大小private Matrix matrix;private Paint paint;private RectF rectF;private BitmapShader bitmapShader;/**
* 圆角大小的默认值
*/
private static final int BODER_RADIUS_DEFAULT = 10;
```
public MineView(Context context, AttributeSet attrs) {
    super(context, attrs);
```
//获取设置的数值
TypedArray typedArray = context.obtainStyledAttributes(attrs, R.styleable.MineView);
borderRadius = typedArray.getDimensionPixelSize(R.styleable.MineView_borderRadius,dp2px(BODER_RADIUS_DEFAULT));
type = typedArray.getInt(R.styleable.MineView_type, TYPE_CIRCLE);
typedArray.recycle();
//初始化
matrix = new Matrix();
paint = new Paint();
paint.setAntiAlias(true);
}
```
public MineView(Context context) {
    super(context, null);
}
@Override
protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
    super.onMeasure(widthMeasureSpec, heightMeasureSpec);
    /**
     * 如果类型是圆形，则强制改变view的宽高一致，以小值为准
     */
    if (type == TYPE_CIRCLE) {
        mWidth = Math.min(getMeasuredWidth(), getMeasuredHeight());
        mRadius = mWidth / 2;
        //设置类型为圆形时，让view宽高一致
        setMeasuredDimension(mWidth, mWidth);
    }
}
/**
 * 初始化BitmapShader
 */
private void setUpShader() {
    Drawable drawable = getDrawable();
    if (drawable == null) return;
    Bitmap bitmap = drawableToBitmap(drawable);
    //将bmp作为着色器，就是在指定区域内绘制bmp
    bitmapShader = new BitmapShader(bitmap, Shader.TileMode.CLAMP, Shader.TileMode.CLAMP);
    float scale = 1.0f;
    if (type == TYPE_CIRCLE) {
        scale = Math.max(mWidth * 1.0f / bitmap.getWidth(), mWidth * 1.0f / bitmap.getHeight());
```
//             int bitmapSize=Math.min(bitmap.getWidth(),bitmap.getHeight());
//              scale=mWidth*1.0f/bitmapSize;}else if (type == TYPE_ROUND) {//比如：view的宽高为10*20；图片的宽高为5*100 ； 最终我们应该按照宽的比例放大，而不是按照高的比例缩小；因为我们需要让缩放后的图片，自定大于我们的view宽高，并保证原图比例。相当于centerCrop
scale = Math.max(getWidth() * 1.0f / bitmap.getWidth(), getHeight() * 1.0f / bitmap.getHeight());
}
matrix.setScale(scale, scale);
bitmapShader.setLocalMatrix(matrix);
paint.setShader(bitmapShader);
}
```
@Override
protected void onDraw(Canvas canvas) {
    //TODO 不要调用super.onDraw!!!
   // super.onDraw(canvas);
    if (getDrawable() == null) return;
    setUpShader();
    if (type == TYPE_ROUND) {
        canvas.drawRoundRect(rectF, borderRadius, borderRadius, paint);
    }
    else
        canvas.drawCircle(mRadius, mRadius, mRadius, paint);
}
@Override
protected void onSizeChanged(int w, int h, int oldw, int oldh) {
    super.onSizeChanged(w, h, oldw, oldh);
    if (type == TYPE_ROUND) {
        // 圆角图片的范围
        rectF = new RectF(0, 0, getWidth(), getHeight());
    }
}
/**
 * 修改圆角大小
 */
public void setBorderRadius(int borderRadius) {
    int pxVal = dp2px(borderRadius);
    if (this.borderRadius != pxVal) {
        this.borderRadius = pxVal;
        invalidate();
    }
}
/**
 * 修改type
 */
public void setType(int type) {
    if (this.type != type) {
        this.type = type;
        if (this.type != TYPE_ROUND && this.type != TYPE_CIRCLE) {
            this.type = TYPE_CIRCLE;
        }
        //从measure开始，对于每一个含有标记位的view及其子View都会进行测量、布局、绘制
        requestLayout();
    }
}
public int dp2px(int dpVal) {
    return (int) TypedValue.applyDimension(TypedValue.COMPLEX_UNIT_DIP,
            dpVal, getResources().getDisplayMetrics());
}
/**
 * drawable转bitmap
 */
private Bitmap drawableToBitmap(Drawable drawable) {
    if (drawable instanceof BitmapDrawable) {
        BitmapDrawable bitmapDrawable = (BitmapDrawable) drawable;
        return bitmapDrawable.getBitmap();
    }
    try {
        Bitmap bitmap;
        /**
         * http://www.codexiu.cn/android/blog/14321/
         ColorDrawable、BitmapDrawable和ClipDrawable的区别
         */
        if (drawable instanceof ColorDrawable) {
            bitmap = Bitmap.createBitmap(2, 2, Bitmap.Config.ARGB_8888);
        }
        else
            // drawable.getIntrinsicHeight()返回的是dp
            bitmap = Bitmap.createBitmap(drawable.getIntrinsicWidth(), drawable.getIntrinsicHeight(), Bitmap.Config.ARGB_8888);
        Canvas canvas = new Canvas(bitmap);
        drawable.setBounds(0, 0, canvas.getWidth(), canvas.getHeight());
        drawable.draw(canvas);
        return bitmap;
    }
    catch (Exception e) {
        e.printStackTrace();
        return null;
    }
}
```
}
```
3.xml布局
```
<LinearLayout
xmlns:android="[http://schemas.android.com/apk/res/android](https://link.jianshu.com?t=http://schemas.android.com/apk/res/android)"
xmlns:app="[http://schemas.android.com/apk/res-auto](https://link.jianshu.com?t=http://schemas.android.com/apk/res-auto)"
xmlns:tools="[http://schemas.android.com/tools](https://link.jianshu.com?t=http://schemas.android.com/tools)"
android:id="@+id/activity_main"
android:layout_width="match_parent"
android:layout_height="match_parent"
android:orientation="vertical"
tools:context="com.crossroads.demo.demo.MainActivity">
<com.crossroads.demo.demo.MineView
android:layout_width="wrap_content"
android:layout_height="wrap_content"
android:src="@mipmap/heartflow"
app:borderRadius="30dp"
app:type="round"
/>
<com.crossroads.demo.demo.MineView
android:layout_width="wrap_content"
android:layout_height="wrap_content"
android:layout_marginBottom="10dp"
android:src="@mipmap/heartflow"
app:borderRadius="30sp"
app:type="round"/>
<com.crossroads.demo.demo.MineView
android:layout_width="wrap_content"
android:layout_height="wrap_content"
android:src="@mipmap/heartflow"
app:borderRadius="30px"
app:type="round"
/>
<com.crossroads.demo.demo.MineView
android:layout_width="wrap_content"
android:layout_height="wrap_content"
android:src="@mipmap/heartflow"/>
```
<!--15-->
<com.crossroads.demo.demo.MineView
    android:layout_width="60dp"
    android:layout_height="80dp"
    android:src="@mipmap/heartflow"/>
```
</LinearLayout>
```
##### 后记
 其他的ComposeShader、[LinearGradient](http://www.jianshu.com/p/c2b5d7fbf8ca)、[RadialGradient](http://www.jianshu.com/p/b20c4a6a1b0f)、[SweepGradient](http://www.jianshu.com/p/c2b5d7fbf8ca)将在之后详细讲解哦！
####补充->Paint的各种方法
```
1.图形绘制
setARGB(int a,int r,int g,int b); // 设置绘制的颜色，a代表透明度，r，g，b代表颜色值。
setAlpha(int a); //设置绘制图形的透明度。
setColor(int color); //设置绘制的颜色，使用颜色值来表示，该颜色值包括透明度和RGB颜色。
setAntiAlias(boolean aa); //设置是否使用抗锯齿功能，会消耗较大资源，绘制图形速度会变慢。
setDither(boolean dither); //设定是否使用图像抖动处理，会使绘制出来的图片颜色更加平滑和饱满，图像更加清晰
setFilterBitmap(boolean filter); //如果该项设置为true，则图像在动画进行中会滤掉对Bitmap图像的优化操作，加快显示 速度，本设置项依赖于dither和xfermode的设置
setMaskFilter(MaskFilter maskfilter); // 设置MaskFilter，可以用不同的MaskFilter实现滤镜的效果，如滤化，立体等
setColorFilter(ColorFilter colorfilter); // 设置颜色过滤器，可以在绘制颜色时实现不用颜色的变换效果
setPathEffect(PathEffect effect); // 设置绘制路径的效果，如点画线等
setShader(Shader shader); //设置图像效果，使用Shader可以绘制出各种渐变效果
setShadowLayer(float radius ,float dx,float dy,int color);//在图形下面设置阴影层，产生阴影效果，radius为阴影的角度，dx和dy为阴影在x轴和y轴上的距离，color为阴影的颜色
setStyle(Paint.Style style); //设置画笔的样式，为FILL，FILL_OR_STROKE，或STROKE
setStrokeCap(Paint.Cap cap);  //当画笔样式为STROKE或FILL_OR_STROKE时，设置笔刷的图形样式，如圆形样式  Cap.ROUND,或方形样式Cap.SQUARE .
setSrokeJoin(Paint.Join join); // 设置绘制时各图形的结合方式Join.MITER（结合处为锐角）Join.Round(结合处为圆弧) Join.BEVEL(结合处为直线)
setStrokeWidth(float width); // 当画笔样式为STROKE或FILL_OR_STROKE时，设置笔刷的粗细度
setXfermode(Xfermode xfermode); //设置图形重叠时的处理方式，如合并，取交集或并集，经常用来制作橡皮的擦除效果
2.文本绘制
setFakeBoldText(boolean fakeBoldText); // 模拟实现粗体文字，设置在小字体上效果会非常差
setSubpixelText(boolean subpixelText); //设置该项为true，将有助于文本在LCD屏幕上的显示效果
setTextAlign(Paint.Align align); // 设置绘制文字的对齐方向
setTextScaleX(float scaleX); //设置绘制文字x轴的缩放比例，可以实现文字的拉伸的效果
setTextSize(float textSize); //设置绘制文字的字号大小
setTextSkewX(float skewX); // 设置斜体文字，skewX为倾斜弧度
setTypeface(Typeface typeface); // 设置Typeface对象，即字体风格，包括粗体，斜体以及衬线体，非衬线体等
setUnderlineText(boolean underlineText); //设置带有下划线的文字效果
setStrikeThruText(boolean strikeThruText); //设置带有删除线的效果
```
####参考网址
[[Paint之函数大汇总](http://blog.csdn.net/harvic880925/article/details/51010839)](http://blog.csdn.net/harvic880925/article/details/51010839)
[paint的setShader](http://www.cnblogs.com/tianzhijiexian/p/4298660.html)
 [TypedValue.applyDimension,代码实现px与dip转换](http://blog.csdn.net/lzhang616/article/details/8945729)
```
