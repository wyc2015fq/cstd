# android: 设置中文字体效果的一些方法 - xqhrs232的专栏 - CSDN博客
2011年06月22日 17:16:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1591标签：[android																[string																[float																[测试																[path																[html](https://so.csdn.net/so/search/s.do?q=html&t=blog)](https://so.csdn.net/so/search/s.do?q=path&t=blog)](https://so.csdn.net/so/search/s.do?q=测试&t=blog)](https://so.csdn.net/so/search/s.do?q=float&t=blog)](https://so.csdn.net/so/search/s.do?q=string&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://blog.csdn.net/uyu2yiyi/archive/2011/04/18/6331238.aspx](http://blog.csdn.net/uyu2yiyi/archive/2011/04/18/6331238.aspx)
开发应用程序使用最多的还是String（字符串），下面我们就如何显示String详细的说明。 
引用Android SDK中显示String的函数，列举如下：
drawText(String text, int start, int end, float x, float y, Paint paint) Draw the text, with origin at (x,y), using the specified paint.
void drawText(char[] text, int index, int count, float x, float y, Paint paint) Draw the text, with origin at (x,y), using the specified paint.
void drawText(String text, float x, float y, Paint paint) Draw the text, with origin at (x,y), using the specified paint.
void drawText(CharSequence text, int start, int end, float x, float y, Paint paint) Draw the specified range of text, specified by start/end, with its origin at (x,y), in the specified Paint.
void drawTextOnPath(String text, Path path, float hOffset, float vOffset, Paint paint) Draw the text, with origin at (x,y), using the specified paint, along the specified path.
void drawTextOnPath(char[] text, int index, int count, Path path, float hOffset, float vOffset, Paint paint) Draw the text, with origin at (x,y), using the specified paint, along the specified path.
在所有的函数中，参数主要分为3部分：字符串（String、char、CharSequence），长度（start—end、index—count），如何显示String（paint）。前2个参数一看就明白，这里我们主要介绍第3个参数Paint paint。
首先，什么是Paint？
引用Android SDK 中的说明，Paint类包含样式和颜色有关如何绘制几何形状，文本和位图的信息 。Canvas是一块画布，具体的文本和位图如何显示，这就是在Paint类中定义了。
然后，Paint有哪些功能？
在了解Paint的功能前，我们按照Word文档的功能，说明下对String的显示，有影响的因素有哪些？字体、大小（TextSize）、颜色（TextColor）、对齐方式(TextAlign)、粗体（Bold）、斜体（Italic）、下划线（Underline）等，下面我们就按照这些影响String显示的因素，结合Android SDK中Paint类的介绍，详细说明Paint类有哪些功能。
在Android SDK中使用Typeface类来定义字体，Typeface类：指定字体和字体的固有风格，用于Paint，类似于Paint的其他 textSize，textSkewX，textScaleX一样，来说明如何绘制文本。归纳起来，Typeface类主要包括以下3个方面：
一些常量的定义（BOLD，BOLD_ITALIC，ITALIC，NORMAL） 
常量字体的定义：
字体（Typeface） 说明 
DEFAULT The default NORMAL typeface object 
DEFAULT_BOLD The default BOLD typeface object. 
MONOSPACE The NORMAL style of the default monospace typeface. 
SANS_SERIF The NORMAL style of the default sans serif typeface. 
SERIF The NORMAL style of the default serif typeface. 
这些常量字体，在程序中是可以直接使用的，例如：Typeface. SERIF
函数：创建字体(Create())，获取字体属性(getStyle()、isBold()、isItalic())；
Typeface类不仅定义了字体，还包括粗体（Bold）、斜体（Italic）。 
其它对显示String有影响的因素，我们都可以在Paint类中找到它们的影子，如下：
类型
 功能
 Paint中的相关操作
Typeface 字体 Typeface  setTypeface(Typeface typeface)
Typeface   getTypeface() 
class Paint.Align 对齐方式 setTextAlign(Paint.Align align)
Paint.Align getTextAlign() 
 字体大小 int getTextSize()
setTextSize(float textSize) 
 颜色 setColor(int color)
int getColor() 
 下划线 boolean  isUnderlineText()
setUnderlineText(boolean underlineText) 
看了这些，想必大家对Paint类也有些基本的了解，实际上在Paint类中还有其他一些功能，比如说Alpha、Dither等，这些也只有大家去Android SDK中仔细阅读了，由于篇幅有限，就不在此详细说明。
最后，如何使用Paint显示String？
实际上，在前面的一些篇幅中的例子程序中都使用了Paint类，在Android画图学习总结（二）——Bitmap 例子的基础上修改下，说明如何使用Paint，如下：
public void onDraw(Canvas canvas)
{
super.onDraw(canvas);
Paint p = new Paint();
String familyName = “宋体”;
Typeface font = Typeface.create(familyName,Typeface.BOLD);
p.setColor(Color.RED);
p.setTypeface(font);
p.setTextSize(22);
canvas.drawText(mstrTitle,0,100,p);
}
程序运行后，界面显示如下：
![Paint](http://www.moandroid.com/wp-content/uploads/2009/09/Paint.JPG)
总结说明
String是我们开发应用程序最经常处理的数据，如何显示String是开发应用程序最基本的要求，在这里我只是抛砖引玉下，简要介绍了Paint类，更加深入的学习请大家到Android SDK中去详细阅读吧！
转自: [http://www.moandroid.com/?p=937](http://www.moandroid.com/?p=937)
-----------------------------------------------------------------
01./*  
02.     * 在xml文件中使用android:textStyle="bold" 可以将英文设置成粗体，但是不能将中文设置成粗体，  
03.     * 将中文设置成粗体的方法是： TextView tv = (TextView)findViewById(R.id.TextView01);  
04.     * TextPaint tp = tv.getPaint(); tp.setFakeBoldText(true);  
05.     * 用Html来格式化字符，例如要实现如下的显示：  
06.     *   
07.     * ＂这只是一个测试字符串，测试黑体字、斜体字、下划线、红色字的显示。＂  
08.     *   
09.     * 可以将字符串格式化成Html格式，如下： Java代码  
10.     *   
11.     * 1. String source =  
12.     * "这只是一个测试字符串，测试<b>黑体字</b>、<i>斜体字</i>、<u>下划线</u>、<font color='red'>红色字</font>的显示。" 
13.     *  * String source =  
14.     * "这只是一个测试字符串，测试<b>黑体字</b>、<i>斜体字</i>、<u>下划线</u>、<font color='red'>红色字</font>的显示。" 
15.     * 然后调用TextView里面setText函数即可 Java代码  
16.     *   
17.     * 1. textView.setText(Html.fromHtml(source));  
18.     */
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/uyu2yiyi/archive/2011/04/18/6331238.aspx](http://blog.csdn.net/uyu2yiyi/archive/2011/04/18/6331238.aspx)

