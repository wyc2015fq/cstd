# 图形绘制Canvas类的方法save()和restore()的区别 - DEVELOPER - CSDN博客





2017年07月12日 13:02:55[学术袁](https://me.csdn.net/u012827205)阅读数：437








最近一直在学习图形绘制这一块内容，但是在学习的过程中，写着写着突然意识到，一直在绘制过程中使用Canvas的两个方法save()和restore()。但是也一直没有注意到，他们俩个到底是什么关系，都是做什么的，起到了什么作用？好尴尬啊。。。

在网上找了一大堆的“详解”，但是我还是没有看明白，然后没办法，我就自己写了几行代码自己跟着代码和显示的效果进行判断和总结！

但是还是要先看一看网络上是怎么对这两个方法进行总结和定义的，是这样的：

`save()`和`restore()`方法允许你保存和恢复一个`CanvasRenderingContext2D`对象的状态。`save()`把当前状态推入到绘图堆栈中，而`restore()`从绘图堆栈中的顶端弹出最近保存的状态，并且根据这些存储的值来设置当前绘图状态。

简单的说就是：通过`save()`函数将目前Canvas的状态推到绘图堆栈中（保存了一个图层）；而通过`restore()`函数从绘图堆栈中弹出（Pop出）上一个Canvas所保存（save）的状态（图层的对象、属性）。

![](https://img-blog.csdn.net/20170712115122676)


接下来使用代码来解释一下：


```java
/**
 * Created by yuanjunhua on 2017/7/12.
 * 功能：测试验证save和restore的区别
 */

public class TestView extends View {
    private Context context;

    public TestView(Context context) {
        this(context, null);
    }

    public TestView(Context context, @Nullable AttributeSet attrs) {
        this(context, attrs, 0);
    }

    public TestView(Context context, @Nullable AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        this.context = context;
        init(attrs);
    }

    private void init(AttributeSet attrs) {

    }

    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);

        Paint paint = new Paint(Paint.ANTI_ALIAS_FLAG);
        paint.setStyle(Paint.Style.FILL);
        paint.setStrokeWidth(12);
        paint.setColor(ContextCompat.getColor(context, R.color.yellow_test));
        canvas.drawLine(0, 0, 400, 400, paint);//线1-黄色

        canvas.save();
        paint.setColor(ContextCompat.getColor(context, R.color.nodeTextColor));
        canvas.drawLine(100, 250, 400, 400, paint);//线2-绿色

        canvas.drawLine(600, 250, 400, 400, paint);//线3-未设置颜色-在没有使用save方法时调用
        canvas.save();

        canvas.drawLine(600, 0, 400, 400, paint);//线4-未设置颜色-在已经使用save方法时调用
        canvas.save();

        canvas.translate(150, 300);
        paint.setColor(ContextCompat.getColor(context, R.color.colorPrimary));
        canvas.drawLine(0, 0, 400, 400, paint);//线5-蓝色
        canvas.save();

        canvas.restore();
        paint.setColor(ContextCompat.getColor(context, R.color.red_test));
        canvas.drawLine(140, 550, 400, 400, paint);//线6-红色
        canvas.save();

        canvas.restore();
        canvas.restore();
        canvas.drawLine(140, 600, 400, 400, paint);//线7-
    }
}
```
效果图是这样的：

![](https://img-blog.csdn.net/20170712130841403)



看到这个截图是不是觉得，瞬间心里很乱。没关系，分析一下代码就明白了。

线1到线4，绘制的中心原点都在（0， 0）；而线5到线7，绘制的中心远点在（150， 300）。

在代码中， 绘制线1之后并保存图层，然后开始绘制线2（绘制2的图形对象是使用线1的，只是绘制颜色改变了），但是绘制完线2之后并没有保存，而是直接绘制线3（绘制3的图形对象是使用线1的），绘制完3保存，继续绘制线4（绘制4的图形对象是使用线3的）再保存图层。我们可以看到，绘制线3、线4并没有重新设置color，这时候会继续沿用上一个图层的对象属性特点进行绘制。如果在这个时候我们在线4的方法save()之后调用方法restore()，然后绘制一条新的线
 会发生什么呢？如果这样的话会绘制一条和线4的颜色、线条属性相同的直线（即是用线4的对象进行绘制吗？）。因为通过上面四条线的操作，在图形的堆栈中通过save方法保存了线1、3、4三个图层。当restore()操作就会pop出线4的图层对象。

真的是这样的吗？我们接着往下分析，到底这句话“save()只是保存图层到图层堆栈，restore()是从图层堆栈中pop出最外层的图层对象”对否？

在绘制线5之前对画布的坐标原点进行了移动，在此情况下绘制了线5（绘制5的图形对象是使用线4的，只是绘制原点、颜色改变了）并保存了图层。然后又以同样的方式绘制并保存了线6（绘制6的图形对象是使用线5的，只是绘制颜色改变了）。关键部分就是线7的绘制，她被绘制之前两次调用了方法restore()，也就是说从图层堆栈中pop了两次，如果上面的理论正确那么绘制线7的对象属性必须是和线线5一致。但是结果是，所绘制的线7的颜色和线6的一样，说明绘制线7的对象属性反而是和线线6是一致的，跟线5没有毛关系。这个结果也说明了每次的pop并不是无限制的取出图层栈的图层对象，而是只能取出距离调用restore()方法最近的所save()的那一层图层对象。

所以说，“通过`save()`函数将目前Canvas的状态推到绘图堆栈中（保存了一个图层）；而通过`restore()`函数从绘图堆栈中弹出（Pop出）上一个Canvas所保存（save）的状态（图层的对象、属性），并进行下一个的绘制。”这句话是正确的。

而且save()
 和 restore()这俩方法使用上有一个特点：save()可以单独使用，但是使用restore()之前一定会使用save()；而且使用retore()的频率并不高，一般是在对图进行大的操作之后，要回到原先的状态才会使用restore()。比如，... ...canvas.save(); ... ... canvas.translate(x,y);
 ... ...canvas.restore();








