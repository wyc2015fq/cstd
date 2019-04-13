
# jquery学习笔记-jQuery操纵DOM元素属性 attr()和removeAtrr()方法 - 萌萌的It人 www.itmmd.com - CSDN博客


2015年01月22日 13:38:38[Jlins](https://me.csdn.net/dyllove98)阅读数：2192


jQuery中操纵元素属性的方法:
**attr()**: 读或者写匹配元素的属性值.
**removeAttr()**: 从匹配的元素中移除指定的属性.

## attr()方法 读操作
**attr()**读操作. 读取的是匹配元素中第一个元素的指定属性值.
格式:**.attr(attributeName)**,返回值类型:String.读取不存在的属性会返回**undefined**.
注意选择器的选择结果可能是一个集合,**这里仅仅获取的是集合中第一个元素的该属性值**.
看例子:
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<!DOCTYPE html><html><head><scripttype="text/javascript"src="/jquery/jquery.js"></script><scripttype="text/javascript">$(document).ready(function() {
            $("button").click(function() {
                alert($("p").attr("title"));//获取属性//this code can only get the first element's attribute.});
        });</script></head><body><ptitle="title1">paragraph 1</p><ptitle="title2">paragraph 2</p><br/><button>get title</button></body></html>![复制代码](http://common.cnblogs.com/images/copycode.gif)

运行结果:弹框显示: title1.
想要分别获取每一个元素的属性,需要使用jQuery的循环结构,比如.each()或.map()方法.
上面的例子可以改成:
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<script type="text/javascript">$(document).ready(function() {
        $("button").click(function() {//get attribute for every element in selection.$("p").each(function() {
                alert($(this).attr("title"));
            });
        });
    });</script>![复制代码](http://common.cnblogs.com/images/copycode.gif)
即可分别获取每个元素的属性.

## attr()方法 写操作
**attr()**写操作. 为匹配元素的一个或多个属性赋值.
一般格式:**.attr(attributeName, value)**, 即为属性设置value.
返回值类型:jQuery.也即支持链式方法调用.
执行写操作的时候,**如果指定的属性名不存在,将会增加一个该名字的属性**,即增加自定义属性,其名为属性名,其值为value值.
写属性是为匹配集合中的每一个元素都进行操作的,见例子:
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<!DOCTYPE html><html><head><scripttype="text/javascript"src="/jquery/jquery.js"></script><scripttype="text/javascript">$(document).ready(function(){
            $("\#button1").click(function(){
                $("p").attr("title","Hello World");
            });
        });</script></head><body><inputtype="button"id="button1"value="button1"></input><p>This is a paragraph.</p><div>This is a div.</div><p>This is another paragraph.</p><div>This is another div.</div></body></html>![复制代码](http://common.cnblogs.com/images/copycode.gif)

点击按钮之后所有的p都加上了title="Hello World”的属性.
写操作还有以下两种格式:
**.attr(attributes)**和**.attr(attributeName, function)**.
下面分别介绍.

**.attr(attributes):**
这里attributes类型是**PlainObject**,可以用于一次性设置多个属性.
什么是PlainObject呢,简单理解就是大括号包围的键值对序列.可以参考问后链接说明.
键和值之间用冒号(:)分隔,每个键值对之间用逗号(,)分隔.
注意:设置多个属性值时,属性名的引号是可选的(可以有,也可以没有).但是class属性是个例外,必须加上引号.
例子:
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<!DOCTYPE html><html><head><scripttype="text/javascript"src="/jquery/jquery.js"></script><scripttype="text/javascript">$(document).ready(function() {
            $("\#button1").click(function() {
                $("p").attr("title","Hello World");
            });
            $("\#button2").click(function() {
                $("div").attr({"title":"some title for div","hello":"World"});
            });
        });</script></head><body><inputtype="button"id="button1"value="button1"></input><inputtype="button"id="button2"value="button2"></input><p>This is a paragraph.</p><div>This is a div.</div><p>This is another paragraph.</p><div>This is another div.</div></body></html>![复制代码](http://common.cnblogs.com/images/copycode.gif)

点击两个按钮之后,元素变为:
![](http://images.cnitblog.com/blog/325852/201501/211223567359179.png)
其中<div>的hello是一个新增的自定义属性,其value为World.
**.attr(attributeName, function(index, oldValue)):**
使用一个function来设置属性值.function的第一个参数是index,第二个参数是该属性之前的值.
看例子:
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<!DOCTYPE html><html><head><style>div{color:blue;}span{color:red;}b{font-weight:bolder;}</style><scripttype="text/javascript"src="/jquery/jquery.js"></script><scripttype="text/javascript">$(document).ready(function() {
$("div")
.attr("id",function(index, oldAttr) {if(oldAttr) {return"div-id"+index+oldAttr;
}else{return"div-id"+index;
}
})
.each(function() {
$("span",this).html("(id = '<b>"+this.id+"</b>')");
});
});</script></head><body><divid="someId">Zero-th<span></span></div><div>First<span></span></div><div>Second<span></span></div></body></html>![复制代码](http://common.cnblogs.com/images/copycode.gif)
上面的例子,对应的页面结果如下:
![](http://images.cnitblog.com/blog/325852/201501/211226195473744.png)
**当使用一个方法来设定属性值的时候,如果这个set的function没有返回值,或者返回了undefined,当前的值是不会被改变的.**
即操作会被忽略.
还是上面的例子,attr()其中的function返回undefined:
如下:
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<script type="text/javascript">$(document).ready(function() {
        $("div").attr("id",function(index, oldAttr) {returnundefined;
        }).each(function() {
            $("span",this).html("(id = '<b>" +this.id + "</b>')");
        });
    });</script>![复制代码](http://common.cnblogs.com/images/copycode.gif)

返回的页面效果如下:
![](http://images.cnitblog.com/blog/325852/201501/211330246101532.png)
即没有进行任何修改操作,还是保持原来的属性值.

注意:**jQuery不能修改<input>和<button>的type属性**,如果修改会在浏览器报错.
这是因为IE浏览器中不能修改type属性.

## removeAttr()方法
移除匹配元素集合中每一个元素的指定属性.
**removeAttr()**方法调用的是JavaScript的removeAttribute()方法,但是它能用jQuery对象直接调用,并且它考虑到并处理了各个浏览器上的属性名称可能不统一的问题.
例子:
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<!DOCTYPE html><html><head><scripttype="text/javascript"src="/jquery/jquery.js"></script><scripttype="text/javascript">$(document).ready(function() {
            $("input[type=button]").click(function() {
                $("div").removeAttr("title");
            });
        });</script></head><body><inputtype="button"value="ClickMe"></input><divtitle="hello">Zero</div></body></html>![复制代码](http://common.cnblogs.com/images/copycode.gif)

点击按钮后,<div>的title属性会被删除.
注意: 用**removeAttr()移除onclick在IE6-8上都不会起作用**,为了避免这个问题,应该使用**.prop()**方法.

参考信息如下
[jquery](http://www.itmmd.com/tag/jquery.html):http://www.itmmd.com/tag/jquery.html
[jQuery教程(31)-jQuery插件开发之全局函数插件](http://www.itmmd.com/201501/534.html)
[jQuery教程(30)-jQuery插件开发之自定义选择符](http://www.itmmd.com/201501/533.html)
[jQuery教程(29)-jQuery插件开发之为插件方法指定参数](http://www.itmmd.com/201501/519.html)
[jQuery教程(28)-jQuery插件开发之使用插件](http://www.itmmd.com/201501/518.html)
[jQuery教程(27)-jQueryajax操作之修改默认选项](http://www.itmmd.com/201501/515.html)

比如:
$element.prop( "onclick",null);
console.log("onclick property: ", $element[ 0 ].onclick );


