# jQuery类选择器和模糊选择 - Big Smile - CSDN博客
2017年07月09日 20:54:22[王啸tr1912](https://me.csdn.net/tr1912)阅读数：1066
        我们对于jQuery来说已经很不陌生了，由于是前端各种框架都需要的一个组件类库形式的一个应用库，jQuery已经很是深入我们的使用中了，但是你真的会使用他吗？
# 一、jQuery的类选择器
        其实说道jQuery的类选择，我们经历的应该是已经不少了，尤其是选择一个对象，一个标签，或者是一个自定义的部件的时候，我们都会用到这个选择器来选择并操作我们的页面。我们可以很轻松的在W3School里面找到jQuery的类选择方法，里面有很多，这里给个地址：[http://www.w3school.com.cn/jquery/jquery_ref_selectors.asp](http://www.w3school.com.cn/jquery/jquery_ref_selectors.asp)
        总结一下里面的选择器，可以分成六大类，按照由上到下的顺序说：
        第一个就是根据标签、标签id，class选择，这一类一般都比较强大，优点是可以选择一个特定的对象。
        第二个就是从集合元素的里面选择一个有序的，比如第一个或者最后一个，常用到的标签有<p><tr>等成组的标签。
        第三个就是从有序列表中选择一个带有特定序号的元素，如<li>。
        第四个就是从header头中选择一些特性标题元素。
        第五个就是从匹配的条件中选择，如可见不可见，有没有元素，元素text等于什么这种条件中选择。
        第六种就是特殊属性，如input的type属性，和<a>等标签的href属性，通过这些特殊的属性可以找到我们自定义的特殊元素。
# 二、模糊选择
        上面的选择器直接对应条件或者标记的选择都是属于精确选择了，除了精确的选择之外，我们的jQuery还支持模糊的选择对象，具体表现和我们用正则表达式比较像，用一些站位符或者转义符来表示一些可以被替换的字符，从而达到我们模糊选择的目的。
**属性字头选择器（Attribute Contains Prefix Selector）**
        jQuery属性字头选择器的使用格式是 $(‘[attribute|=value]‘) ，例如 $(‘[herflang|=en]‘) 这句代码执行时将会选择文档中所有含有 herflang 属性，并且 herflang 的值以 “en” 开头的元素，即使 “en” 后面紧跟着连字符 “-” 也能进行选择。
**属性开头选择器（Attribute Starts With Selector）**
         $(‘[attribute^=value]‘) ，用于选择属性的值以某个字符串开头的元素，但和 $(‘[attribute|=value]‘) 的区别是，如果 value 的后面是一个连字符的话，这个元素不会被选择。例如 $(‘[rel^=no]‘) 将会选择所有 rel 的值以 “no” 开头的元素，但类似于 rel=”no-****” 的元素不会被选择。
**属性包含选择器（Attribute Contains Selector）**
        基本使用方法为 $(‘[attribute*=value]‘)，例如 $(‘[rel*=no]‘)，表示所有带有 rel 属性，并且 rel 的值里包含子字符串 “no” 的元素（如 rel=”nofollow”，rel=”yesorno” 等等）都将会被选择。
**属性单词选择器（Attribute Contains Word Selector）**
        $(‘[attribute~=value]‘)，这个选择器的特别之处在于 value 的值只能必须是一个独立的单词（或者是字符串），例如 $(‘[rel~=no]‘) ，此句在执行的时候会选择带有 rel=”yes or no” 的元素，但不会选择带有 rel=”yesorno” 的元素。这个选择器可以看做属性包含选择器的补充品，用于需要进行严格匹配的时候。
**属性结尾选择器（Attribute Ends With Selector）**
        $(‘[attribute$=value]‘) ，用于选择特定属性的值以某个字符串结尾的元素，例如 $(‘[rel$=no]‘) 将会选择 rel 属性的值以 “no” 结尾的元素。
**属性均等选择器（Attribute Equals Selector）**
       $(‘[attribute=value]‘) ，只选择属性的值完全相等的元素，如：$(‘[rel=nofollow]‘)，则只选择 rel=”nofollow” 的元素，差一点都不行！
**属性非等选择器（Attribute Not Equal Selector）**
         $(‘[attribute!=value]‘) ，和 :not([attr=value]) 的效果一样，用于选择属性不等于某个值的元素，例如 $(‘[rel!=nofollow]‘)，所有 rel=”nofollow” 的元素都不会被选择。
由于模糊查询的结果都是集合对象，所以我们可以用foreach来遍历。
.each(function(i){  
});
