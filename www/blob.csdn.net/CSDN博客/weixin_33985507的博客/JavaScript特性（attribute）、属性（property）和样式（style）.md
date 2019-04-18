# JavaScript特性（attribute）、属性（property）和样式（style） - weixin_33985507的博客 - CSDN博客
2017年03月15日 15:57:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
# 一、DOM特性和DOM属性
**attribute（特性）**，是我们赋予某个事物的特质或对象，attribute是HTML标签上的特性，它的值只能够是字符串
**property（属性）**，是早已存在的不需要外界赋予的特质，property是DOM中的属性，是JavaScript里的对象
在访问元素特性值时有两种方式：
1. 传统DOM方法getAttribute和setAttribute。
2. 使用DOM对象上与之对应的属性。
例如通过两种方式获取id的值：
```
e.getAttribute('id');
e.id;
```
![](https://images2015.cnblogs.com/blog/211606/201611/211606-20161106165157611-1349671932.png)
**1）跨浏览器命名**
特性和属性命名之间的差异会更多。
在大多数浏览器中可以用class获取到class特性，但IE却要使用className。
**2）命名限制**
特性表示为传递给DOM方法的字符串，其命名规范是非常自由的。
而属性名称，由于可以作为标识符使用点表示法进行访问，所以其命名规范更受限制。
![](https://images2015.cnblogs.com/blog/211606/201611/211606-20161106170128643-1089823647.png)
**3）HTML和XML之间的差异**
在处理一个XML DOM的时候，不会在元素上自动创建属性值来表示特性值。
因此，我们需要使用传统的DOM特性方法获取特性的值。
[elem.ownerDocument](https://developer.mozilla.org/zh-CN/docs/Web/API/Node/ownerDocument)只读属性会返回当前节点的顶层的 document 对象。
```
function isXML(elem) {
　　return (elem.ownerDocument || elem.documentElement).nodeName.toLowerCase() !== 'html';
}
```
**4）自定义特性的行为**
要想访问自定义特性值，需要使用DOM方法getAttribute()与setAttribute()。
在HTML5中，对所有的自定义特性使用“data-”前缀。
**5）性能注意事项**
总的来说，属性的访问速度比相应DOM特性方法的访问速度要快，特别是在IE浏览器中。
下面做个测试，分别用两种方式读取和设置500W次。可以在不同浏览器中点开这个地址来查看测试结构。
下面的截图是在Chrome中的结果：
![](https://images2015.cnblogs.com/blog/211606/201611/211606-20161106172258533-769443274.png)
# 二、跨浏览器的Attribute问题
**1）DOM中的id/name膨胀**
5大浏览器都会将表单input元素的id和name特性作为<form>元素的属性值进行引用。
产生的这些属性，会主动覆盖form元素上已经存在的同名属性。
此外，IE浏览器不仅会替换属性值，甚至还会替换该属性值上的特性值。
```
<form id="testForm" action="/">                       
      <input type="text" id="id"/>
      <input type="text" name="action"/>
</form>
```
```
var form = document.getElementById('testForm');
assert(form.id === 'testForm', "the id property is untouched");
assert(form.action === '/', "the action property is untouched");
assert(form.getAttribute('id') === 'testForm',  "the id attribute is untouched");
assert(form.getAttribute('action') === '/', "the action attribute is untouched");
```
![](https://images2015.cnblogs.com/blog/211606/201611/211606-20161106173600830-717603212.png)
在所有的现代浏览器中，由于这些input定义的id和name的值，表单的id和action属性都会被input元素的引用所替代。
解决方法是获取描述元素特性本身的原始DOM节点，该节点没有被浏览器修改：
```
var action = element.getAttributeNode('action').nodeValue;
```
**2）URL规范化**
在访问一个引用了URL属性时（例如src、href或action），该URL值会自动将原始值转换成完整规范的URL。
```
<a href="listing-11.5.html" id="testSubject">Self</a>
```
```
var link = document.getElementById('testSubject');
var linkHref = link.getAttributeNode('href').nodeValue; //#1
assert(linkHref === 'listing-11.5.html',  'link node value is ok');
assert(link.href === 'listing-11.5.html',  'link property value is ok');
assert(link.getAttribute('href') == linkHref, 'link attribute not modified');
```
![](https://images2015.cnblogs.com/blog/211606/201611/211606-20161106182917768-1552552098.png)
**3）style特性**
HTML DOM元素有一个style属性，通过该属性我们能获取元素的样式信息，例如element.style.color。
```
<span style="color:red"></span>
```
如果要获取“color:red”字符串，那么style属性没用，得用getAttribute('style')方法获取。
但IE中得用element.style.cssText获取。
**4）节点名称**
在HTML文档中，nodeName属性返回的名称将是大写（例如HTML、BODY）。
在XML或XHTML文档中，nodeName返回的名称是用户指定的名称，可以大写、小写或混合。
# 三、令人头疼的样式特性
**1）样式在何处**
```
<style>
      div { font-size: 1.8em; border: 0 solid gold; }
</style>
<div style="color:#000;" title="Ninja power!">
      忍者
</div>
```
```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
var div = document.getElementsByTagName("div")[0];
assert(div.style.color == 'rgb(0, 0, 0)' ||  div.style.color == '#000','color was recorded');
assert(div.style.fontSize == '1.8em', 'fontSize was recorded');
assert(div.style.borderWidth == '0', 'borderWidth was recorded');
div.style.borderWidth = "4px";                    //#6
assert(div.style.borderWidth == '4px', 'borderWidth was replaced');
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
![](https://images2015.cnblogs.com/blog/211606/201611/211606-20161106183932955-1837282439.png)
1. 大多数颜色都会转换成RGB颜色符号，有一些会转换为颜色的名称。
2. font-size和border都被记录在了style标签内，样式是继承获取的，但div.style属性中没有获取到。
3. 一个元素的style属性中的任何一个样式的优先级都要高于样式表所继承的样式（即便使用了“!important”注解规则）。
**2）样式属性命名**
CSS特性将多余一个单词的用连字符分隔，例如font-size、font-weight等。
在JS中可以用带有连字符的样式名称，但不能使用点运算符来访问样式了。
```
var size = element.style['font-size'];//允许
var size = element.style.font-size;//不允许
var size = element.style.fontSize;//允许
```
在JS中多个单词的CSS样式名称用驼峰的方式命名，例如fontSize、fontWeight。
具体的CSS样式表对应属性可以参考《[CSS Properties Reference](https://developer.mozilla.org/en-US/docs/Web/CSS/CSS_Properties_Reference)》
**3）设置多个样式属性**
不能通过直接给style属性设置字符串（如：elt.style = "color: blue;"）来设置style，因为style应被当成是**只读**的。
设置多个属性可以用cssText或setAttribute，其中cssText会将重复属性覆盖屌。
```
elt.style.cssText = "color: blue"; // 设置多个样式属性 当带前缀的时候-webkit-transform:rotate(0deg)，会过滤掉-webkit，只能用下面的方式
elt.setAttribute("style", "color: blue"); // 设置多个样式属性
```
**4）float样式属性**
float是JS中的保留关键字，浏览器需要提供另外一个替代名称。
大部分浏览器使用cssFloat，但IE使用styleFloat。
**5）像素值的转换过程**
为style属性设置数字时，必须要指定单位，以便在所有的浏览器上都能使用。
```
element.style.height = '10px';//安全
element.style.height = 10;//不安全
```
**6）测量元素的高度和宽度**
offsetHeight和offsetWidth可以获取元素的高度和宽度，不过这两个属性值包括了padding值，更多元素尺寸属性可以参考《[JavaScript中尺寸、坐标](http://www.cnblogs.com/strick/p/4826273.html)》
但如果元素隐藏（例如display:none），获取到的数值都是0。对于隐藏元素可以用如下的方法：
![](https://images2015.cnblogs.com/blog/211606/201611/211606-20161106191009611-1285363492.png)
可以查看在线[demo实例](http://www.pwstrick.com/ninja-code/chapter-12/listing-12.9.html)：
![](https://images2015.cnblogs.com/blog/211606/201611/211606-20161106191135658-1558087015.png)
![](https://images2015.cnblogs.com/blog/211606/201611/211606-20161106191622924-175728586.png)
“Pole”是显示的图片，就是那个忍者。
而“Shuriken”是隐藏的图片，一开始是获取不到宽度的，在使用了上面的技巧后就能获取到了。
**7）通过opacity看透明度**
所有现代浏览器，包括IE9支持opacity属性，但IE9之前的版本需要使用专用的alpha过滤法。
```
opacity: .5;
filter: alpha(opacity=50)
```
支持opacity的浏览器，总会将值规范为小于1.0且以0开头的值。
例如“opacity:.5”，那么支持的将返回“0.5”，不支持的将返回“.5”。
通过特性仿真，可以判断浏览器是否支持opacity。
```
//特性仿真检测
var div = document.createElement("div");                 //#1
div. setAttribute('style','opacity:.5');
var OPACITY_SUPPORTED = div.style.opacity === "0.5";
assert(OPACITY_SUPPORTED,   "Opacity is supported.");
```
**8）颜色属性**
通过不同的计算样式方法访问这些颜色值的时候，各种浏览器的返回值几乎都不一致。
![](https://images2015.cnblogs.com/blog/211606/201611/211606-20161106192916486-1292832744.png)
下面是一个[在线demo](http://www.pwstrick.com/ninja-code/chapter-12/listing-12.11.html)，在不同浏览器中展示的颜色值：
|IE8|Firefox|Chrome|
|----|----|----|
|![](https://images2015.cnblogs.com/blog/211606/201611/211606-20161106193233705-1580072516.png)|![](https://images2015.cnblogs.com/blog/211606/201611/211606-20161106193302049-267055998.png)|![](https://images2015.cnblogs.com/blog/211606/201611/211606-20161106193211408-1604581050.png)|
|IE8不支持RGBA和HSL格式|Firefox不仅保留了颜色名称，而且标准颜色是RGB和RGBA格式|Webkit浏览器(Chrome、Safari)标准颜色是RGB和RGBA格式|
# 四、获取计算样式
**1）window.getComputedStyle()与element.currentStyle**
一个元素的计算样式（computed style）都是应用在该元素上的所有样式组合。
这些样式包括样式表、元素的style特性，以及脚本对style属性的各种操作。
现代浏览器（包括IE9）的实现方法为：[window.getComputedStyle()](https://developer.mozilla.org/zh-CN/docs/Web/API/Window/getComputedStyle)，返回接口提供了一个getPropertyValue()的方法。
IE9之前的版本，通过附加到所有元素上的[currentStyle](https://developer.mozilla.org/zh-CN/docs/Web/API/Element/currentStyle)属性，表现和style属性一样。
```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
/**
* property可以传入驼峰或分隔符方式
*/
function fetchComputedStyle(element, property) { //#3
  //现代浏览器 包括IE9
  if (window.getComputedStyle) {
    var computedStyles = window.getComputedStyle(element); //#4
    if (computedStyles) { //#5
      property = property.replace(/([A-Z])/g, '-$1').toLowerCase();
      return computedStyles.getPropertyValue(property);//getPropertyValue中需要传入分隔符字符串，例如font-size
    }
  } else if (element.currentStyle) { //IE9以下
    property = property.replace(
      /-([a-z])/ig,
      function(all, letter) {
        return letter.toUpperCase();
      });
    return element.currentStyle[property];
  }
}
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
