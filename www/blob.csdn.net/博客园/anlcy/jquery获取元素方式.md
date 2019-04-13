
# jquery获取元素方式 - anlcy - 博客园






# [jquery获取元素方式](https://www.cnblogs.com/camilla/p/6892728.html)
Jquery 获取元素的方法分为两种：jQuery选择器、jQuery遍历函数。
**1、获取本身：**
**a.只需要一种jQuery选择器**
|选择器|实例|说明|
|\#Id|$('\#myId')|ID选择器： 可以获取到ID为“myId”的元素，区分大小写|

**b.多种jQuery选择器组合**
**b1.jQuery选择器**
|选择器|实例|说明|
|.class|$('.myClass')|类选择器：可以获取到class为‘myClass’的所有元素|
|element|$('p')|获取所有的<p>元素|
|:header|$(':header')|获取所有的标题元素：<h1> ~ <h6>|
|:animated|$(':animated')|获取所有的动画元素|
|:contains(text)|$('p:contains(Hello)')|获取所有包含文本为Hello的<p>元素，中间的文本区分大小写|
|:hidden|$(':hidden')|获取所有的隐藏元素：width和height为0、display:none、type=hidden、|
|[attribute]|$('[href]')|属性选择器：获取所有含有属性为href的元素|
|[attribute=value]|$('[href=a.html]')|=   获取所有带有属性href，并且值为a.html的元素
|!=  获取所有带有属性href，并且值不等于为a.html的元素
|$=  获取所有带有属性href，并且值以a.html结尾的元素
|^=  获取所有带有属性href，并且值以a.html开头的元素
|~=  获取所有带有属性href，并且值包含单词”a.html“的元素
|*=  获取所有带有属性href，并且值包含文本”a.html“的元素
|
|:input|$(':input')|获取所有input元素|
|:radio|$(':radio')|所有带有 type="radio" 的 input 元素
|相似的有：
|:text、:chexbox、:password、:submit、:reset、:button、:file
|
|:enabled|$(':enabled')|所有启用的input元素。 :disabled  则相反|
|:checked|$(':checked')|所有选中的input选择（单选框、复选框）|
|:gt(index)|$('p:gt(2)')|index从0开始，获取index大于（不包含）2的所有<p>元素|
|:lt(index)|$('p:lt(2)')|index从0开始，获取index小于（不包含）2的所有<p>元素|
|:even|$('tr:even')|所有偶数<tr>元素|
|:odd|$('tr:odd')|所有奇数<tr>元素|












**b2.jQuery选择器****jQuery遍历函数混合**
|选择器|实例|说明|
|:first|$('p:first')|第一个<p>元素|
|:last|$('p:last')|最后一个<p>元素|
|:eq(index)|$("p:eq(1)")|第二个<p>元素，index从0开始|


**b3.jQuery遍历函数**
|方法|描述|
|eq()|返回带有被选元素的指定索引号的元素|
|first()|返回被选元素的第一个元素|
|last()|返回被选元素的最后一个元素|


**2、选择同级元素**
**jQuery选择器**
$('div + p') 每个div相邻的下一个<p>元素
$('div ~ p')	获取跟div同级的所有的<p>元素
**jQuery遍历函数**
next() 返回被选元素的后一个同级元素
nextAll()	返回被选元素之后的所有同级元素
prev()	返回被选元素的前一个同级元素
prevAll()	返回被选元素之前的所有同级元素
**3、获取父级元素**
**jQuery选择器**
$("p:parent")获取所有p元素的父级元素
**jQuery遍历函数**
parent() 获取被选元素的父级元素
parents()	获取被选元素的所有祖先元素
**4.获取子级元素**
**jQuery选择器**
$('div > p') 获取div直接子元素的所有<p>元素
$('div p')	 获取div所有后代的<p>元素
**jQuery遍历函数**
children() 返回被选元素的所有直接子元素
contents()	返回被选元素的所有直接子元素(包含文本和注释节点)
find()	返回被选元素的后代元素





