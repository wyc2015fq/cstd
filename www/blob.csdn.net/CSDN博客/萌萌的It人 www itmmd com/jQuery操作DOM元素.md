
# jQuery操作DOM元素 - 萌萌的It人 www.itmmd.com - CSDN博客


2015年01月19日 11:36:38[Jlins](https://me.csdn.net/dyllove98)阅读数：1370标签：[jquery																](https://so.csdn.net/so/search/s.do?q=jquery&t=blog)个人分类：[jquery																](https://blog.csdn.net/dyllove98/article/category/1349854)



HTML DOM常见操作:
查找节点,插入节点,删除节点,复制节点,替换节点,包裹节点.

## 查找节点
关于节点的查找,是利用选择器完成的.

## 插入节点
首先看一个例子,用原生的JavaScript如何创建并添加元素:
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<!DOCTYPE html><html><head><script>varaddItems=function(){varnumber=parseInt(document.getElementById("number").value);vardiv1=document.getElementById("div1");
            div1.innerHTML="";//clear div before appendfor(vari=0; i<number;++i){varinput=document.createElement("input");
                input.setAttribute("type","text");varbr=document.createElement("br")
                div1.appendChild(input);
                div1.appendChild(br);
            }
        }</script></head><body><inputtype="text"id="number"/><inputtype="button"id="btn"value="Click"onclick="addItems();"/><divid="div1"></div></body></html>![复制代码](http://common.cnblogs.com/images/copycode.gif)

**jQuery插入元素的方法**:
append(): 向每个匹配的元素内部追加内容.
appendTo(): 将所有匹配的元素追加到指定的元素中,即$(A).appendTo(B),是将A追加到B中.
prepend(): 向每个匹配的元素内部前置内容.
prependTo(): 将所有匹配的元素前置到指定的元素中,即$(A).prependTo(B),是将A前置到B中.
前面几个方法都是插入子元素,后面的这几个方法是插入兄弟元素.
after(): 在每个匹配的元素之后插入内容.
insertAfter(): 将所有匹配的元素插入到指定元素的后面.
before(): 在每个匹配的元素之前插入内容.
insertBefore(): 将所有匹配的元素插入到指定元素的前面.
**jQuery支持方法链式的调用****,即append()等方法返回的仍然是这个jQuery对象,后面可以继续调用append()方法.**
下面我们将开始的那个例子用jQuery实现:
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<!DOCTYPE html><html><head><scriptsrc="/jquery/jquery-1.11.1.min.js"></script><script>varaddItems=function(){varnumber=parseInt($("\#number").val());vardiv1=$("\#div1");
            div1.html("");//clear div before appendvarstring="";for(vari=0; i<number;++i){
                string+="<input type='text'/><br/>"}
            div1.append(string);
        }</script></head><body><inputtype="text"id="number"/><inputtype="button"id="btn"value="Click"onclick="addItems();"/><divid="div1"></div></body></html>![复制代码](http://common.cnblogs.com/images/copycode.gif)

**插入已有节点->移动节点**
除了可以插入新建的节点之外,还可以选择已有节点,插入到别的地方,以完成节点的移动操作.
注意这里是移动操作,即原来的节点会改变位置:
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<!DOCTYPE html><html><head><scriptsrc="/jquery/jquery-1.11.1.min.js"></script><script>$(document).ready(function(){
$("ul li:eq(0)").appendTo("ul");
});</script></head><body><ul><lititle="li1">1</li><lititle="li2">2</li><lititle="li3">3</li><lititle="li4">4</li><lititle="li5">5</li></ul></body></html>![复制代码](http://common.cnblogs.com/images/copycode.gif)

运行结果为:
![](http://images.cnitblog.com/blog/325852/201501/161821076671573.png)

## 删除节点
jQuery中删除节点的方法:
remove(): 移除所有匹配的元素.
empty(): 删除匹配的元素集合中所有内容,包括子节点.注意,元素本身没有被删除.
关于remove()方法,有几点需要说明一下:
1.remove()方法的返回值:
remove()方法会返回被删除节点的jQuery对象.可以把这个对象插入到其他的地方.
所以也可以用这种方法来移动节点:
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<!DOCTYPE html><html><head><scriptsrc="/jquery/jquery-1.11.1.min.js"></script><script>$(document).ready(function(){
$("ul li:eq(0)").appendTo("ul");//move li method1//move method2:varremoveLi=$("ul li:eq(0)").remove();//delete liremoveLi.appendTo($("ul"));//add removed li});</script></head><body><ul><lititle="li1">1</li><lititle="li2">2</li><lititle="li3">3</li><lititle="li4">4</li><lititle="li5">5</li></ul></body></html>![复制代码](http://common.cnblogs.com/images/copycode.gif)
运行结果为:
![](http://images.cnitblog.com/blog/325852/201501/161823406362672.png)
注意直接写标签名(例如:appendTo(“ul”))和取jQuery对象(例如:appendTo($(“ul")))的结果一样.
2.remove()方法的参数:
remove()方法默认情况下会删除选择器选中的所有元素.
比如下面的代码将删除所有<ul>下的<li>:
//remove all li$("ul li").remove();
remove()方法还可以接收参数,设置一些筛选条件,指定到底要删除其中的哪些节点.
比如除了指定title的,其他全部移除:
//remove some$("ul li").remove("li[title!='li2']");

empty()方法:删除匹配元素集合中的所有子节点,清空内容.但是元素本身还存在.
比如:
//empty children$("ul").empty();
会删除<ul>中的所有子节点.
而:
//empty content$("ul li:eq(3)").empty();
会得到这样的效果:
![](http://images.cnitblog.com/blog/325852/201501/161827087766227.png)


## 复制节点
jQuery中复制节点的方法:
clone(): 创建匹配元素集合的副本.
clone()方法返回被复制的节点.
看一个例子,每次点击<li>都复制同样的元素并添加在<ul>末尾:
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<!DOCTYPE html><html><head><scriptsrc="/jquery/jquery-1.11.1.min.js"></script><script>$(document).ready(function(){
$("ul > li").click(function(){
$(this).clone().appendTo("ul");
});
});</script></head><body><ul><lititle="li1">1</li><lititle="li2">2</li><lititle="li3">3</li><lititle="li4">4</li><lititle="li5">5</li></ul></body></html>![复制代码](http://common.cnblogs.com/images/copycode.gif)

注意,clone出来的<li>对象就没有click事件了,即点击复制添加出来的<li>,不会再添加新的<li>.
jQuery考虑到有时候会有需要clone所有的事件,所以clone方法可以带一个参数.
clone(true): 复制节点,包括所有的事件处理.
clone(false): 复制节点,但不包括事件处理.不带参数时默认是这种情况.

## 替换节点
jQuery中替换节点的方法:
replaceAll(): 用指定的HTML内容或元素替换被选元素.
语法:$(content).replaceAll(selector).
replaceWith(): 用新内容替换所匹配到的元素.
语法:$(selector).replaceWith(content).
其中的content可以是HTML代码,可以是新元素,也可以是已经存在的元素.
关于用已有元素替换:
w3school的相关文档([http://www.w3school.com.cn/jquery/manipulation_replaceall.asp](http://www.w3school.com.cn/jquery/manipulation_replaceall.asp))中说:
"已经存在的元素不会被移动,只会被复制."这种说法是不正确的.
实际做了实验之后,发现**用已有元素替换,原来的元素是会被移除的**:
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<html><head><scripttype="text/javascript"src="/jquery/jquery.js"></script><scripttype="text/javascript">$(document).ready(function(){
$("\#button1").click(function(){
alert("click event");
$("\#button1").replaceAll($("div"));
});
});</script><style>div{height:20px;background-color:yellow}</style></head><body><p>This is paragraph:<divid="div1">This is a div 1</div><divid="div2">This is a div 2</div><p>This is button area:<br><buttonid="button1"class="btn1">Button1</button></body></html>![复制代码](http://common.cnblogs.com/images/copycode.gif)

页面初始状态:
![](http://images.cnitblog.com/blog/325852/201501/190844531414832.png)
点击按钮之后:
![](http://images.cnitblog.com/blog/325852/201501/190845142979962.png)
点击新得到的两个Button1,是可以弹出alert的,说明**替换节点的时候连同节点事件一起放到了新的地方**.
查看官方文档:
replaceAll():[http://api.jquery.com/replaceAll/](http://api.jquery.com/replaceAll/)
replaceWith():[http://api.jquery.com/replaceWith/](http://api.jquery.com/replaceWith/)
可以看到使用已有节点是不会复制该节点的,而是会移动该节点到新的地方.

## 包裹节点
包裹节点,意思是把匹配的元素用指定的内容或者元素包裹起来,即增加一个父元素.
方法有:
wrap(): 把匹配的元素用指定的内容或元素包裹起来.
wrapAll(): 把所有匹配的元素用指定的内容或元素包裹起来,这里会将所有匹配的元素移动到一起,合成一组,只包裹一个parent.
wrapInner(): 将每一个匹配元素的内容用指定的内容或元素包裹起来.
例子代码:
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<html><head><scripttype="text/javascript"src="/jquery/jquery.js"></script><scripttype="text/javascript">$(document).ready(function(){
$("\#button1").click(function(){
$("\#div1").wrap("<a href='www.google.com'>Google Wrap div1</a>");
});
$("\#button2").click(function(){
$("div").wrapAll("<a href='www.baidu.com'>Baidu Wrap all div</a>");
});
$("\#button3").click(function(){
$("div").wrapInner("<a href='www.sogou.com'>Sogou Wrap inner div</a>");
});
});</script><style>div{height:20px;background-color:yellow}</style></head><body>div1:<br/><divid="div1">This is a div 1</div>div2:<br/><divid="div2">This is a div 2</div>div3:<br/><divid="div3">This is a div 3</div>This is button area:<br/><buttonid="button1">Wrap</button><br/><buttonid="button2">Wrap All</button><br/><buttonid="button3">Wrap Inner</button></body></html>![复制代码](http://common.cnblogs.com/images/copycode.gif)

页面初始状态:
![](http://images.cnitblog.com/blog/325852/201501/190847098917876.png)
按第一个按钮,wrap特定的一个div:
![](http://images.cnitblog.com/blog/325852/201501/190848134064580.png)![](http://images.cnitblog.com/blog/325852/201501/190848500942623.png)

在按了第一个按钮之后,按第二个按钮,wrapAll将所有的div包裹进一个group:
![](http://images.cnitblog.com/blog/325852/201501/190849282035964.png)![](http://images.cnitblog.com/blog/325852/201501/190849372972244.png)

按了第一个按钮和第二个按钮之后,按第三个按钮,wrapInner,在每一个div内部添加一层嵌套:
![](http://images.cnitblog.com/blog/325852/201501/190850008911318.png)![](http://images.cnitblog.com/blog/325852/201501/190850098604624.png)



## 相关杂项知识
1.关于变量命名的一点题外话:
变量命名习惯(不是标准):
var $v jQuery对象变量以$符号开头命名.
var v 普通HTML DOM对象变量.
但是个人觉得初学者用这样的命名可能会有点混乱,会比较难以区分什么时候是命名,什么时候是jQuery操作符.
所以目前的例子里先不这样用.
2.小知识: 在HTML标签的字符串前面加上$符号即可将其转换为jQuery对象,比如:
var child = $("<input type='text'/><br/>");
3.从第一个例子可以看出,JavaScript操纵DOM元素的方法和jQuery的相关方法名称区别如下:
JavaScript: appendChild(), removeChild().
jQuery: append(), remove(). 不过这个remove()移除的是自身.


## 参考资料

**其他精彩文章**
## [jQuery教程(29)-jQuery插件开发之为插件方法指定参数](http://www.itmmd.com/201501/519.html)
## [jQuery教程(28)-jQuery插件开发之使用插件](http://www.itmmd.com/201501/518.html)
## [jQuery教程(27)-jQueryajax操作之修改默认选项](http://www.itmmd.com/201501/515.html)
## [jQuery教程(26)-ajax操作之使用JSONP加载远程数据](http://www.itmmd.com/201501/512.html)
## [jQuery教程(25)-ajax操作之安全限制](http://www.itmmd.com/201501/511.html)

更多关于[android开发](http://www.itmmd.com/mobile.html)文章


