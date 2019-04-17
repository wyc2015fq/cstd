# 初识View-ONE - DEVELOPER - CSDN博客





2017年12月06日 17:07:09[学术袁](https://me.csdn.net/u012827205)阅读数：97
个人分类：[Android](https://blog.csdn.net/u012827205/article/category/1804657)

所属专栏：[Android-开发](https://blog.csdn.net/column/details/20398.html)









![颐和园一日游](https://img-blog.csdn.net/20171206142827123?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

## 开篇短说

做开发很久了，却一直没有真正的去面对自定义View这一块蛋糕。平时，在github或者博客上自学了很多这方面的知识，但是一直不是很系统，总是在真正使用的时候发现，自己在这方面的知识储备显得-很“笨拙”，只能应对一些简单的！！借着最近项目不紧张，再次进行View的系统学习，并在这里记录一下自己的学习过程。足迹是我学习的写照，请不要谎说，你一直在谈恋爱！

## 开门见山
- **自定义View，在extends View时，需要实现几个构造方法，有什么含义？ **
- **自定义属性，在./values/attrs.xml下的标签declare-styleable 是干嘛的？**
- **标签declare-styleable下的 format 有什么作用？**
- **不使用declare-styleable 可以吗？TypedArray 能干嘛？ **

## 开始详述

### 自定义View，在extends View时，需要实现几个构造方法，有什么含义？看图

![这里写图片描述](https://img-blog.csdn.net/20171206151741041?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
- **第一个构造方法使用环境：平时我们在通过 new 创建一个控件对象时候，被调用。如，MyView tView = new MyView (context);**
- **第二个构造方法使用环境：平时我们自定义了一个MyView ，然后要将自定义的MyView 放在xml布局中使用时。**
- **第三个构造方法使用环境：系统默认情况下，第三个构造函数是不会被调用的。她的调用只是在，通过我们通过自己的代码来启用。如上图！！**

### 自定义属性，在./values/attrs.xml下的标签declare-styleable 是干嘛的？

首先让我们来看一下系统的，在SDK下，如图： 
![这里写图片描述](https://img-blog.csdn.net/20171206152717358?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
然后我们可以看到一些代码：

```java
<declare-styleable name="View">
        <attr name="id" format="reference" />
        <attr name="tag" format="string" />
        <attr name="scrollX" format="dimension" />
        <attr name="scrollY" format="dimension" />
        <attr name="background" format="reference|color" />
        ... ...
    </declare-styleable>


    <declare-styleable name="TextView">
        <attr name="text" format="string" localization="suggested" />
        <attr name="hint" format="string" />
        <attr name="textColor" />
        <attr name="textSize" />
        <attr name="textScaleX" format="float" />
        <attr name="cursorVisible" format="boolean" />
        <attr name="maxLines" format="integer" min="0" />
        ... ...
   </declare-styleable>
```

**declare-styleable 解释：**  所有的资源文件在R中都会对应一个整型常量，并通过这个ID值能找到对应的资源文件。属性在R中对应的类是public static final class attr，如果我们写declare-styleable，就会在R文件中就会生成 对应的 styleable 类，这个类就是将每个控件的进行属性分组，然后记录属性的索引值。

通过观察上面 attrs.xml 中的代码，可以看到所有的控件属性设定都是通过”declare-styleable”标签进行分组划分，attrs.xml 文件中定义属性的固定模式：通过标签来设定属性，如上代码的 “declare-styleable”、”name”、”attr”、”format”等标签。 

还有一点，平时我们自定义一个View，然后有一些属性并没有定义。但是我们依然可以使用，为什么？因为所有的控件都是View的子类，这时我们使用的是View在系统下的默认属性。如 `android:layout_width="match_parent"`

但并不是每个View的子控件都能有所有属性，父控件View拥有的属性其子控件可共同拥有，而对于子控件自己所定义的特色属性，如TextView 的  `android:text="Hello World!"`  或 ImageView的  `android:src="@color/colorAccent"` 是每个控件独自持有的。 

由此，如果我们想要去实现一些特殊的效果。且当系统下现有的控件已经不足以满足我们的需求，这个时候，我们就要自己去自定义View。即自定义满足需求的属性。
**我想很多朋友会问，”format” 有什么用？**

当然，当我们自己进行自定义View的时候，都是仿照系统下的定义View格式进行的。所以我们进入到SDK中的 attrs.xml 属性文件进行查看时，发现这些attr属性表示的格式并不尽相同。有的有标签”format”，有的却没有！ 

 如：`<attr name="textSize" />  <attr name="textScaleX" format="float" />`

    解释一下，有”format”限定的属性表示：在此控件下正在自定义自己特色的且属于自己的属性；反之，是在使用其父类已经存在的属性； 

    但即使系统已经定义了的属性，只要是我们自己定义的控件，我们也需要申明此属性，表示我们要使用她。系统定义过的属性是给系统下的控件使用，而非给我们自定义的控件使用，而不申明则不能使用。所以有两种定义的表示方式-再次申明和重新定义：
![这里写图片描述](https://img-blog.csdn.net/20171206161220986?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

![这里写图片描述](https://img-blog.csdn.net/20171206161308933?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

在 attr.xml 中使用 format 标签的时候，请看： 
![这里写图片描述](https://img-blog.csdn.net/20171206164602910?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171206164815562?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**通过上面叙述，能够写出各种类型控件的完整属性。但是完之后，我们怎么在xml文件中获取控件的属性值，以便在控件中使用。**

这时，我们进入到一个 xml 文件中，看到红色线框中内容： 
![这里写图片描述](https://img-blog.csdn.net/20171206165531201?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
他们被称作”命名空间”，比如 “android:layout_width=”match_parent”” 的 “android” 表示通过命名空间查找属性 “layout_width” 的来源。当然这个是针对系统 android的，对自定义的则会有下面红色线框的命名空间。res-aouto 表示自动查找，也可以使用自定义控件所持有的包名替代 res-auto

### 不使用declare-styleable 可以吗？

答案是可以的。

```xml
<resources>
        <attr name="myText" format="string"/>
        <attr name="myColor" format="reference|color"/>
</resources>
```

而普遍是这样的

```xml
<resources>
    <declare-styleable name="MyView">
        <attr name="myText" format="string" />
        <attr name="myColor" format="reference|color" />
    </declare-styleable>
</resources>
```

or

```xml
<resources>
<!--定义属性-->
    <attr name="myText" format="string" />
    <attr name="myColor" format="reference|color" />
    <declare-styleable name="MyView">
    <!--生成索引-->
        <attr name="myText" />
        <attr name="myColor" />
    </declare-styleable>
</resources>
```

所有的资源文件在R中都会对应一个整型常量，并通过这个ID值能找到对应的资源文件。属性在R中对应的类是public static final class attr，如果我们写declare-styleable，就会在R文件中就会生成 对应的 styleable 类，这个类就是将每个控件的进行属性分组，然后记录属性的索引值。而TypedArray正好需要通过此索引值获取属性。如下图：

![这里写图片描述](https://img-blog.csdn.net/20171206172601447?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)








