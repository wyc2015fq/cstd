# 初见@html.textboxfor - Big Smile - CSDN博客
2016年09月11日 13:32:58[王啸tr1912](https://me.csdn.net/tr1912)阅读数：3587
        其实最初见到这个的时候以为和<%* *>这个里面的东西差不多呢，然后通过调整样式才发现里面的东西差点还是太多了，下面来说一下。
# 一、初见
        第一次见其实是在ITOO中了，当时也不知道他是一个什么东西，也没有管过他，所以也没有在意，直到我们需要给人事系统美化界面的时候才知道原来和这个东西里面的数据也要进行样式化。并且里面获得数据的语句和lambda表达式也有类似的地方，肯定用了linq语句来索引数据。看文件的最上方，还有一个ViewModel瞬间就明白了许多。
![](https://img-blog.csdn.net/20160911132210424?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
        其实在fireBug中看当前的数据行就是一个<input>标签所在的地方，所以当时也就没有多想，直接把input这个标签在css里面写了一个样式的规则，但是发现用不了啊，于是就又开始发愁了。
# 二、探究
        但是这个究竟怎么才能在数据中改变样式呢？在百度上一查，发现了许多有趣的东西，修改这个里面的样式是可以直接在后面加style的标记的。如代码：
`@Html.TextBoxFor(s=>s.dd,new {@style="width:20px;"})    @*这是直接加的style*@   `
`<pre name="code" class="html"><span style="font-size:14px;">@Html.TextBoxFor(s=>s.dd,new {@</span><span style="font-size:12px;"><span style="font-family: Arial, Helvetica, sans-serif;">class="main"</span><span style="font-family: Arial, Helvetica, sans-serif;">})    @*这是引用的css*@   </span></span>`
        探究完了之后开始尝试，发现这样并不能改变样式，难道是有什么东西已经使他改变样式了吗？于是又开始寻找，终于在网页的最上面找到了设置的JS语句：
```java
$(function () {
     $(":text").attr("readonly", "true"); //设为不可用
     $(":text").css({ "background": "#FFFFFF", "border-style": "none", "height": "22px", "font-size": "20px", "width": "360px","font-weight":"500" });
});
```
       这样一来就清楚不过了。
# 三、总结
        总的来说，这样的数据传输的东西在整个cshtml页面中还是有很多的，有一些还是值得我们去探索的，这样的传输数据的方式也要深入的学习。
