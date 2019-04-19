# 关于UI的AutoLayout自动布局的解析-腾讯游戏学院
发表于2017-02-06
[评论*0*](#commit)*460*浏览
想免费获取内部独家PPT资料库？观看行业大牛直播？点击加入腾讯游戏学院游戏程序行业精英群
[711501594](//shang.qq.com/wpa/qunwpa?idkey=3fcf6147441efc8b343eb7447e6a996eabb6a33093306529c4d1e9d2ab75423d)
　　UGUI的自动布局系统主要由Layout Element(布局元素)、Layout Controller(布局控制器)两部分组成。本篇文章将会对这两部分内容进行比较详细地介绍。
**1.****LayoutElement****（布局元素）**
　　只要附加了Rect Transform组件的游戏物体都可以称为布局元素。布局元素包含了关于设置自身Size的信息数据，而Layout Controller组件使可以使用Layout Element提供的Size相关信息来计算决定Layout Element的Size。另外，Rect Transform、Layout Element以及其他如Image、Text的组件之间的关系如下图所示。
![](http://gameweb-img.qq.com/gad/20170206/image001.1486372600.png)
　　而且，如果你想覆盖游戏对象诸如Mininum/Prefered/Flexible 的Size属性时，是需要将Layout Element组件附加到游戏对象上，相当于把附加了RectTransform组件的游戏对象关于这方面的属性“暴露”出来，供我们手动修改。特别值得注意的是，LayoutElement布局元素所涉及的游戏对象的属性改变导致游戏对象Size改变的直观效果，是必须与Layout Controller布局控制器“联合运用”才能看到的（已测试验证过）。
![](http://gameweb-img.qq.com/gad/20170206/image002.1486372600.png)
**2.****LayoutController****（布局控制器）**
　　Layout Controller是能够控制自身和子级Layout Element即包含Rect Transform组件的GameObjects的size与position的组件。这里有几种常见的布局控制器：Content Size Fitter、Aspect Ratio Fitter、Layout Groups(HorizontalLayout Group/Vertical Layout Group/Grid Layout Group)。
**（1）Content Size Fitter（内容大小过滤组件）**
内容大小过滤组件根据LayoutElement的属性控制自己的Layout Element的Size。
![](http://gameweb-img.qq.com/gad/20170206/image003.1486372601.png)
|属性|值|作用|
|----|----|----|
|横向适配|Unconstrained|不受LayoutElement约束|
|Min Size|受LayoutElement的mininum width约束| |
|Preferred Size|受LayoutElement的preferred width约束| |
|纵向适配|Unconstrained|不受LayoutElement约束|
|Min Size|受LayoutElement的mininum height约束| |
|Preferred Size|受LayoutElement的preferred height约束| |
**（2）****AspectRatio Fitter****（宽高比过滤组件）**
　　宽高比过滤组件根据宽高比预设的模式来控制自己的Layout Element的Size。
![](http://gameweb-img.qq.com/gad/20170206/image004.1486372601.png)
|属性|值|作用|
|----|----|----|
|Aspect Mode|None|不根据宽高比适配|
|Width Controls  Height|高度根据宽度自适应| |
|Height Controls  Width|宽度根据高度自适应| |
|Fit In Parent|宽或高和父物体一样，另一个小于父物体| |
|Envelope Parent|宽或高和父物体一样，另一个包围父物体| |
|Aspect Ratio||宽高比例|
**（3）Layout Groups(Horizontal Layout Group/Vertical Layout Group/GridLayout Group)**
　　主要用来控制子级LayoutElements的sizes与positions，但是不能控制自己的size。另外，自己的Size可以被其他LayoutController控制。Layout Groups根据子级LayoutElements提供的关于Size的mininum/prefered/flexible的值计算与分配适合的空间给每个子级的Layout Element，另外，Layout Group自己的Size无论如何变化，都不会影响它给子级LayoutElement的分配。
**Horizontal(Vertical)LayoutGroup****（水平、垂直布局）**
![](http://gameweb-img.qq.com/gad/20170206/image005.1486372601.png)
|属性|作用|
|----|----|
|Padding_Left/Right/Top/Bottom|子级距离父级的左/右/上/下边框的间隔距离|
|Spacing|子级与子级之间的间隔距离|
|Child Alignment|子级的对齐方式|
|Child Force  Expand_Width/Height|是否强制拉伸长宽到父物体大小|
**Grid Layout Group(****网格布局)**
![](http://gameweb-img.qq.com/gad/20170206/image006.1486372601.png)
|属性|作用|
|----|----|
|Padding_Left/Right/Top/Bottm|子级距离父级的左/右/上/下边框的间隔距离|
|Cell Size|子级的大小|
|Spacing|子级与子级X/Y方向之间的间隔距离|
|Start Corner|设定第1个子级的位置（如左上角等）|
|Start Axis|设定子级的排列方向（如按行排列）|
|Child Alignment|子级的对齐方式|
|Constraint|约束（按照父物体长宽自动横竖、指定行列数）|
　　总结：实际项目中，我们经常会遇到对多个UI进行统一排版的情况（如水平排列、垂直排列、网格排列等）。这需要综合运用上述的这些组件才能够满足要求。这里只是对这些布局组件做了一个简单介绍，读者可以实践测试一下，这样可以加深对它们的理解。另外，如果有不对之处，还望指正！
